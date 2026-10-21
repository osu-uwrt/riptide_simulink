#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/srv/list_parameters.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <algorithm>
#include <chrono>
#include <deque>
#include <functional>
#include <mutex>
#include <set>
#include <stdexcept>
#include <utility>

#include "riptide_simulink/parameters.hpp"

namespace riptide_simulink {

struct ModelResult {
    bool success = false;
    std::string message;
    std::vector<std::string> names;
    std::vector<rclcpp::Parameter> parameters;
    std::vector<rcl_interfaces::msg::SetParametersResult> results;
};

struct ModelStatus {
    bool available = false;
    bool ready = false;
    bool ambiguous = false;
    std::string node_name;
};

struct ModelOptions {
    std::chrono::milliseconds discovery_period{500};
    std::chrono::milliseconds request_timeout{3000};
    std::size_t queue_capacity = 10;
    std::size_t reload_attempts = 3;
    std::chrono::milliseconds retry_period{1000};
    std::string reload_service; // empty disables the optional Trigger service
};

/// Remote supervision only. Callbacks must return promptly and must not spin/wait.
/// Destroying the facade cancels queued work; callbacks never retain the overseer.
class SimulinkModel {
public:
    using Completion = std::function<void(const ModelResult &)>;
    using Provider = std::function<std::vector<rclcpp::Parameter>()>;
    using StateChange = std::function<void(const ModelStatus &)>;

private:
    using List = rcl_interfaces::srv::ListParameters;
    using Get = rcl_interfaces::srv::GetParameters;
    using Set = rcl_interfaces::srv::SetParameters;
    using Clock = std::chrono::steady_clock;

    // shared with timer/service/client callbacks through weak pointers, so it can outlive the facade safely
    struct State : std::enable_shared_from_this<State> {
        std::weak_ptr<rclcpp::Node> owner;
        std::string target;
        ModelOptions options;
        Provider provider;
        Completion reload_completion;
        StateChange state_change;
        ModelStatus status;
        std::recursive_mutex mutex;
        rclcpp::CallbackGroup::SharedPtr group;
        rclcpp::TimerBase::SharedPtr timer;
        rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr reload_service;
        rclcpp::Client<List>::SharedPtr list;
        rclcpp::Client<Get>::SharedPtr get;
        rclcpp::Client<Set>::SharedPtr set;

        // one request in flight at a time; the rest wait in the queue
        struct Job { std::function<void(uint64_t)> start; Completion done; };
        std::deque<Job> queue;
        std::function<void()> cancel;
        Completion active_done;
        bool active = false, stopped = false, reloading = false;
        uint64_t sequence = 0, epoch = 0;
        Clock::time_point deadline, retry_at;
        std::size_t attempts = 0;

        State(const rclcpp::Node::SharedPtr &node, std::string name, ModelOptions opts, Provider source)
            : owner(node), target(std::move(name)), options(std::move(opts)), provider(std::move(source)) {}

        void notify() {
            auto callback = state_change;
            const auto snapshot = status;
            if (callback) callback(snapshot);
        }

        void finish(uint64_t token, ModelResult result) {
            std::lock_guard<std::recursive_mutex> lock(mutex);

            // stale tokens are responses to requests that already timed out or were reset
            if (stopped || !active || token != sequence) return;
            if (cancel) cancel();
            cancel = {};
            active = false;

            auto callback = std::move(active_done);
            active_done = {};
            if (callback) callback(result);
        }

        void reset(const std::string &reason) {
            // new epoch invalidates in-flight reloads and responses
            ++epoch;
            ++sequence;
            if (cancel) cancel();
            cancel = {};

            // fail everything pending
            std::vector<Completion> callbacks;
            if (active_done) callbacks.push_back(std::move(active_done));
            for (auto &job : queue) if (job.done) callbacks.push_back(std::move(job.done));
            queue.clear();
            active = false;
            reloading = false;
            attempts = 0;
            status.ready = false;
            for (auto &callback : callbacks) callback(ModelResult{false, reason, {}, {}, {}});
        }

        bool enqueue(Job job) {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            if (stopped || !status.available || queue.size() + (active ? 1 : 0) >= options.queue_capacity) return false;
            queue.push_back(std::move(job));
            return true;
        }

        template<class Service, class Convert>
        bool request(typename rclcpp::Client<Service>::SharedPtr client,
            typename Service::Request::SharedPtr request, Convert convert, Completion done)
        {
            auto weak = this->weak_from_this();
            return enqueue(Job{[weak, client, request, convert](uint64_t token) {
                auto self = weak.lock();
                if (!self) return;
                if (!client || !client->service_is_ready()) {
                    self->finish(token, ModelResult{false, "Parameter service unavailable", {}, {}, {}});
                    return;
                }

                auto future = client->async_send_request(request,
                    [weak, token, convert](typename rclcpp::Client<Service>::SharedFuture response) {
                        if (auto state = weak.lock()) {
                            try { state->finish(token, convert(response.get())); }
                            catch (const std::exception &e) { state->finish(token, ModelResult{false, e.what(), {}, {}, {}}); }
                        }
                    });

                // lets a timeout drop the pending request
                auto id = future.request_id;
                self->cancel = [client, id]() { client->remove_pending_request(id); };
            }, std::move(done)});
        }

        bool list_parameters(Completion done) {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            return request<List>(list, std::make_shared<List::Request>(), [](List::Response::SharedPtr response) {
                ModelResult result;
                result.success = true;
                result.names = response->result.names;
                return result;
            }, std::move(done));
        }

        bool set_parameters(const std::vector<rclcpp::Parameter> &parameters, Completion done) {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto req = std::make_shared<Set::Request>();
            for (const auto &p : parameters) req->parameters.push_back(p.to_parameter_msg());

            return request<Set>(set, req, [parameters](Set::Response::SharedPtr response) {
                ModelResult result;
                result.parameters = parameters;
                result.results = response->results;

                // succeeds only if every parameter was accepted
                result.success = response->results.size() == parameters.size();
                for (const auto &r : response->results) {
                    if (!r.successful) {
                        result.success = false;
                        result.message += r.reason + "; ";
                    }
                }
                if (response->results.size() != parameters.size()) result.message = "Parameter response length mismatch";
                return result;
            }, std::move(done));
        }

        bool get_parameters(const std::vector<std::string> &names, Completion done) {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto req = std::make_shared<Get::Request>();
            req->names = names;

            return request<Get>(get, req, [names](Get::Response::SharedPtr response) {
                ModelResult result;
                result.names = names;
                result.success = response->values.size() == names.size();
                if (!result.success) {
                    result.message = "Parameter response length mismatch";
                    return result;
                }

                for (size_t i = 0; i < names.size(); ++i)
                    result.parameters.emplace_back(names[i], rclcpp::ParameterValue(response->values[i]));
                return result;
            }, std::move(done));
        }

        void reload_finished(uint64_t generation, const ModelResult &result) {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            if (stopped || generation != epoch) return;
            reloading = false;
            status.ready = status.available && result.success;
            retry_at = Clock::now() + options.retry_period;
            notify();

            auto callback = reload_completion;
            if (callback) callback(result);
        }

        bool reload(bool explicit_request = false) {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            if (stopped || !status.available || reloading) return false;
            if (explicit_request) attempts = 0;
            ++attempts;

            // get the desired values and reject duplicate names
            std::vector<rclcpp::Parameter> desired;
            try { if (provider) desired = provider(); }
            catch (const std::exception &e) {
                reload_finished(epoch, ModelResult{false, e.what(), {}, {}, {}});
                return false;
            }

            std::set<std::string> unique;
            for (const auto &p : desired) if (!unique.insert(p.get_name()).second) {
                reload_finished(epoch, ModelResult{false, "Duplicate desired parameter: " + p.get_name(), {}, {}, {}});
                return false;
            }

            auto weak = this->weak_from_this();
            const auto generation = epoch;
            reloading = true;
            status.ready = false;
            notify();

            // list first so missing parameters fail clearly, then set them all
            bool accepted = list_parameters([weak, generation, desired](const ModelResult &listed) {
                auto self = weak.lock();
                if (!self) return;
                std::lock_guard<std::recursive_mutex> lock(self->mutex);
                if (self->stopped || generation != self->epoch) return;
                if (!listed.success) {
                    self->reload_finished(generation, listed);
                    return;
                }

                for (const auto &p : desired) {
                    if (std::find(listed.names.begin(), listed.names.end(), p.get_name()) == listed.names.end()) {
                        self->reload_finished(generation,
                            ModelResult{false, "Required parameter missing: " + p.get_name(), {}, {}, {}});
                        return;
                    }
                }

                if (desired.empty()) {
                    self->reload_finished(generation, ModelResult{true, "No parameters required", {}, {}, {}});
                    return;
                }

                if (!self->set_parameters(desired, [weak, generation](const ModelResult &result) {
                    if (auto state = weak.lock()) state->reload_finished(generation, result);
                })) self->reload_finished(generation, ModelResult{false, "Request queue full", {}, {}, {}});
            });

            if (!accepted) {
                reloading = false;
                retry_at = Clock::now() + options.retry_period;
            }
            return accepted;
        }

        void tick() {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            auto node = owner.lock();
            if (!node || stopped) return;

            // find the target: full name if it starts with /, otherwise any namespace
            std::vector<std::string> matches;
            for (const auto &entry : node->get_node_graph_interface()->get_node_names_and_namespaces()) {
                const auto fqn = (entry.second == "/" ? "/" : entry.second + "/") + entry.first;
                if ((!target.empty() && target.front() == '/' && fqn == target) ||
                    (!target.empty() && target.front() != '/' && entry.first == target)) matches.push_back(fqn);
            }
            const bool unique = matches.size() == 1;
            const std::string resolved = unique ? matches.front() : "";

            // node gone, ambiguous, or replaced
            if (!unique || status.node_name != resolved) {
                if (status.available || status.ambiguous != (matches.size() > 1) || status.node_name != resolved) {
                    status.available = false;
                    status.ambiguous = matches.size() > 1;
                    status.node_name = resolved;
                    reset(unique ? "Remote node changed" : "Remote node missing or ambiguous");
                    notify();
                }
            }

            // (re)connect clients, available once all three services are up
            if (unique && !status.available) {
                if (!list || status.node_name != resolved || list->get_service_name() != resolved + "/list_parameters") {
                    list = node->create_client<List>(resolved + "/list_parameters", rmw_qos_profile_parameters, group);
                    get = node->create_client<Get>(resolved + "/get_parameters", rmw_qos_profile_parameters, group);
                    set = node->create_client<Set>(resolved + "/set_parameters", rmw_qos_profile_parameters, group);
                }
                status.node_name = resolved;
                if (list->service_is_ready() && set->service_is_ready() && get->service_is_ready()) {
                    status.available = true;
                    status.ambiguous = false;
                    notify();
                    reload();
                }
            } else if (status.available && (!list->service_is_ready() || !set->service_is_ready() || !get->service_is_ready())) {
                status.available = false;
                reset("Remote parameter services disappeared");
                notify();
            }

            // retry failed reloads, time out the active request, start the next one
            if (status.available && !status.ready && !reloading && attempts < options.reload_attempts &&
                Clock::now() >= retry_at) reload();
            if (active && Clock::now() >= deadline) finish(sequence, ModelResult{false, "Parameter request timed out", {}, {}, {}});
            if (!active && !queue.empty() && status.available) {
                auto job = std::move(queue.front());
                queue.pop_front();
                active = true;
                active_done = std::move(job.done);
                deadline = Clock::now() + options.request_timeout;
                const auto token = ++sequence;
                try { job.start(token); }
                catch (const std::exception &e) { finish(token, ModelResult{false, e.what(), {}, {}, {}}); }
            }
        }

        void stop() {
            std::lock_guard<std::recursive_mutex> lock(mutex);
            stopped = true;
            if (timer) timer->cancel();
            state_change = {};
            reload_completion = {};
            reset("Model wrapper destroyed");
        }
    };

    std::shared_ptr<State> state_;

public:
    SimulinkModel(const rclcpp::Node::SharedPtr &overseer, const std::string &target,
        ModelOptions options = {}, Provider provider = {})
    {
        if (!overseer || target.empty() || options.queue_capacity == 0 ||
            options.discovery_period.count() <= 0 || options.request_timeout.count() <= 0 ||
            options.reload_attempts == 0 || options.retry_period.count() <= 0)
            throw std::invalid_argument("Invalid SimulinkModel options");

        state_ = std::make_shared<State>(overseer, target, std::move(options), std::move(provider));
        state_->group = overseer->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

        // discovery/queue timer
        std::weak_ptr<State> weak = state_;
        state_->timer = overseer->create_wall_timer(state_->options.discovery_period, [weak]() {
            if (auto state = weak.lock()) state->tick();
        }, state_->group);

        // optional reload trigger
        if (!state_->options.reload_service.empty()) {
            state_->reload_service = overseer->create_service<std_srvs::srv::Trigger>(state_->options.reload_service,
                [weak](const std_srvs::srv::Trigger::Request::SharedPtr, std_srvs::srv::Trigger::Response::SharedPtr response) {
                    auto state = weak.lock();
                    response->success = state && state->reload(true);
                    response->message = response->success ? "Reload accepted; completion is asynchronous" : "Reload not accepted";
                }, rmw_qos_profile_services_default, state_->group);
        }
    }

    ~SimulinkModel() { if (state_) state_->stop(); }

    SimulinkModel(const SimulinkModel &) = delete;
    SimulinkModel &operator=(const SimulinkModel &) = delete;

    ModelStatus status() const {
        std::lock_guard<std::recursive_mutex> lock(state_->mutex);
        return state_->status;
    }

    std::string remote_node_name() const { return status().node_name; }

    bool reload_parameters() {
        return state_->reload(true);
    }

    bool list_parameters(Completion done) { return state_->list_parameters(std::move(done)); }

    bool get_parameters(const std::vector<std::string> &names, Completion done) {
        return state_->get_parameters(names, std::move(done));
    }

    bool set_parameters(const std::vector<rclcpp::Parameter> &values, Completion done) {
        return state_->set_parameters(values, std::move(done));
    }

    void on_reload(Completion callback) {
        std::lock_guard<std::recursive_mutex> lock(state_->mutex);
        state_->reload_completion = std::move(callback);
    }

    void on_state_change(StateChange callback) {
        std::lock_guard<std::recursive_mutex> lock(state_->mutex);
        state_->state_change = std::move(callback);
    }
};

} // namespace riptide_simulink
