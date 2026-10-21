#include <gtest/gtest.h>
#include <riptide_simulink/simulink_model.hpp>
#include <fstream>
#include <thread>
#include <atomic>

using namespace std::chrono_literals;
using riptide_simulink::SimulinkModel;

class ModelTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() { rclcpp::init(0, nullptr); }
    static void TearDownTestSuite() { rclcpp::shutdown(); }

    // spin until predicate holds, up to ~4 s
    template<class E, class P> bool until(E &exec, P predicate) {
        for (int i = 0; i < 400; ++i) {
            exec.spin_some();
            if (predicate()) return true;
            std::this_thread::sleep_for(10ms);
        }
        return predicate();
    }
};

TEST_F(ModelTest, AppliesAndReconnects) {
    auto owner = std::make_shared<rclcpp::Node>("overseer");
    auto remote = std::make_shared<rclcpp::Node>("plant", "/test");
    remote->declare_parameter("gain", 0.0);
    riptide_simulink::ModelOptions opts;
    opts.discovery_period = 20ms;
    SimulinkModel model(owner, "/test/plant", opts, []{return std::vector<rclcpp::Parameter>{{"gain", 2.5}};});
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(owner);
    exec.add_node(remote);

    // applied on discovery
    ASSERT_TRUE(until(exec, [&]{return model.status().ready;}));
    EXPECT_DOUBLE_EQ(remote->get_parameter("gain").as_double(), 2.5);
    bool got = false;
    ASSERT_TRUE(model.get_parameters({"gain"}, [&](const auto &r){got = r.success && r.parameters[0].as_double() == 2.5;}));
    ASSERT_TRUE(until(exec, [&]{return got;}));

    // node restarts: lost, then reapplied
    exec.remove_node(remote);
    remote.reset();
    ASSERT_TRUE(until(exec, [&]{return !model.status().available;}));
    remote = std::make_shared<rclcpp::Node>("plant", "/test");
    remote->declare_parameter("gain", 0.0);
    exec.add_node(remote);
    ASSERT_TRUE(until(exec, [&]{return model.status().ready;}));
    EXPECT_DOUBLE_EQ(remote->get_parameter("gain").as_double(), 2.5);
}

TEST_F(ModelTest, AmbiguityAndRejectionMultithreaded) {
    auto owner = std::make_shared<rclcpp::Node>("observer");
    auto one = std::make_shared<rclcpp::Node>("plant", "/one");
    auto two = std::make_shared<rclcpp::Node>("plant", "/two");
    one->declare_parameter("gain", 0.0);
    auto reject = one->add_on_set_parameters_callback([](const auto &){rcl_interfaces::msg::SetParametersResult r; r.successful = false; r.reason = "rejected";return r;});
    riptide_simulink::ModelOptions opts;
    opts.discovery_period = 20ms;
    SimulinkModel model(owner, "plant", opts, []{return std::vector<rclcpp::Parameter>{{"gain", 1.0}};});
    std::atomic<bool> failed{false};
    model.on_reload([&](const auto &r){failed = !r.success;});

    rclcpp::executors::MultiThreadedExecutor exec;
    exec.add_node(owner);
    exec.add_node(one);
    exec.add_node(two);
    std::thread worker([&]{exec.spin();});
    auto wait = [&](auto predicate){for (int i = 0; i < 400 && !predicate(); ++i) std::this_thread::sleep_for(10ms); return predicate();};

    // two matches is ambiguous; one left that rejects the set is not ready
    EXPECT_TRUE(wait([&]{return model.status().ambiguous;}));
    EXPECT_FALSE(model.status().ready);
    exec.remove_node(two);
    two.reset();
    EXPECT_TRUE(wait([&]{return failed.load();}));
    EXPECT_FALSE(model.status().ready);
    exec.cancel();
    worker.join();
}

TEST_F(ModelTest, TimeoutAndBoundedQueue) {
    auto owner = std::make_shared<rclcpp::Node>("timeout_observer");
    auto remote = std::make_shared<rclcpp::Node>("slow_plant");
    riptide_simulink::ModelOptions opts;
    opts.discovery_period = 20ms;
    opts.request_timeout = 100ms;
    opts.queue_capacity = 1;
    SimulinkModel model(owner, "slow_plant", opts);
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(owner);
    exec.add_node(remote);
    ASSERT_TRUE(until(exec, [&]{return model.status().ready;}));

    // remote stops answering: request times out and the queue is full meanwhile
    exec.remove_node(remote);
    bool timeout = false;
    ASSERT_TRUE(model.list_parameters([&](const auto &r){timeout = !r.success && r.message.find("timed out") != std::string::npos;}));
    EXPECT_FALSE(model.list_parameters({}));
    ASSERT_TRUE(until(exec, [&]{return timeout;}));

    // and works again once it answers
    exec.add_node(remote);
    bool success = false;
    ASSERT_TRUE(model.list_parameters([&](const auto &r){success = r.success;}));
    ASSERT_TRUE(until(exec, [&]{return success;}));
}

TEST(Parameters, NativeAndLegacyYaml) {
    const auto file = "/tmp/riptide-params-" + std::to_string(getpid()) + ".yaml";

    // native ROS file keeps types
    {std::ofstream out(file); out << "/test/plant:\n  ros__parameters:\n    gain: 1.5\n    enabled: true\n    counts: [1, 2]\n";}
    auto native = riptide_simulink::load_ros_parameters(file, "/test/plant");
    ASSERT_EQ(native.size(), 3u);
    for (const auto &p : native) {
        if (p.get_name() == "gain") EXPECT_DOUBLE_EQ(p.as_double(), 1.5);
        if (p.get_name() == "enabled") EXPECT_TRUE(p.as_bool());
        if (p.get_name() == "counts") EXPECT_EQ(p.as_integer_array(), (std::vector<int64_t>{1, 2}));
    }

    // legacy file is flattened and scaled
    {std::ofstream out(file); out << "controller:\n  gain: 1.5\n  enabled: true\n  offsets: [1.2, -2.0]\n";}
    auto values = riptide_simulink::load_legacy_parameters(file);
    EXPECT_EQ(values[0].get_name(), "controller__gain");
    EXPECT_EQ(values[0].as_int(), 1500000);
    EXPECT_TRUE(values[1].as_bool());
    EXPECT_EQ(values[2].as_integer_array()[1], -2000000);

    {std::ofstream out(file); out << "bad: 1.0e100\n";}
    EXPECT_THROW(riptide_simulink::load_legacy_parameters(file), std::out_of_range);
    std::remove(file.c_str());
}

TEST_F(ModelTest, LateNodeAndLateParameters) {
    auto owner = std::make_shared<rclcpp::Node>("late_observer");
    auto misleading = std::make_shared<rclcpp::Node>("wanted_suffix");
    riptide_simulink::ModelOptions opts;
    opts.discovery_period = 20ms;
    opts.retry_period = 50ms;
    opts.reload_attempts = 10;
    SimulinkModel model(owner, "wanted", opts, []{return std::vector<rclcpp::Parameter>{{"gain", 3.0}};});
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(owner);
    exec.add_node(misleading);
    exec.spin_some();
    EXPECT_FALSE(model.status().available);

    // node shows up before its parameter does; retries pick it up
    auto remote = std::make_shared<rclcpp::Node>("wanted");
    exec.add_node(remote);
    ASSERT_TRUE(until(exec, [&]{return model.status().available;}));
    EXPECT_FALSE(model.status().ready);
    remote->declare_parameter("gain", 0.0);
    ASSERT_TRUE(until(exec, [&]{return model.status().ready;}));
    EXPECT_DOUBLE_EQ(remote->get_parameter("gain").as_double(), 3.0);
}

TEST_F(ModelTest, DestroyWithPendingRequest) {
    auto owner = std::make_shared<rclcpp::Node>("destroy_observer");
    auto remote = std::make_shared<rclcpp::Node>("destroy_plant");
    riptide_simulink::ModelOptions opts;
    opts.discovery_period = 20ms;
    auto model = std::make_unique<SimulinkModel>(owner, "destroy_plant", opts);
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(owner);
    exec.add_node(remote);
    ASSERT_TRUE(until(exec, [&]{return model->status().ready;}));
    exec.remove_node(remote);

    // destroying fails the pending request exactly once
    int callbacks = 0;
    bool success = true;
    ASSERT_TRUE(model->list_parameters([&](const auto &r){++callbacks; success = r.success;}));
    for (int i = 0; i < 5; ++i) {
        exec.spin_some();
        std::this_thread::sleep_for(10ms);
    }
    model.reset();
    EXPECT_EQ(callbacks, 1);
    EXPECT_FALSE(success);
    exec.add_node(remote);
    exec.spin_some();
    EXPECT_EQ(callbacks, 1);
}

TEST_F(ModelTest, PartialRejectionAndReloadService) {
    auto owner = std::make_shared<rclcpp::Node>("reload_observer");
    auto remote = std::make_shared<rclcpp::Node>("reload_plant");
    remote->declare_parameter("good", 0);
    remote->declare_parameter("bad", 0);
    auto reject = remote->add_on_set_parameters_callback([](const auto &parameters){
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = parameters[0].get_name() != "bad";
        result.reason = result.successful ? "" : "consumer rejected bad";
        return result;
    });
    riptide_simulink::ModelOptions opts;
    opts.discovery_period = 20ms;
    opts.reload_attempts = 1;
    opts.reload_service = "reload_model";
    SimulinkModel model(owner, "reload_plant", opts, []{return std::vector<rclcpp::Parameter>{{"good", 1}, {"bad", 2}};});
    int completions = 0;
    riptide_simulink::ModelResult last;
    model.on_reload([&](const auto &r){last = r; ++completions;});
    rclcpp::executors::SingleThreadedExecutor exec;
    exec.add_node(owner);
    exec.add_node(remote);

    // one rejection fails the whole reload
    ASSERT_TRUE(until(exec, [&]{return completions == 1;}));
    ASSERT_EQ(last.results.size(), 2u);
    EXPECT_TRUE(last.results[0].successful);
    EXPECT_FALSE(last.results[1].successful);
    EXPECT_FALSE(last.success);
    EXPECT_FALSE(model.status().ready);

    // service triggers another attempt
    auto client = owner->create_client<std_srvs::srv::Trigger>("reload_model");
    ASSERT_TRUE(until(exec, [&]{return client->service_is_ready();}));
    auto response = client->async_send_request(std::make_shared<std_srvs::srv::Trigger::Request>());
    ASSERT_TRUE(until(exec, [&]{return response.wait_for(0ms) == std::future_status::ready;}));
    EXPECT_TRUE(response.get()->success);
    ASSERT_TRUE(until(exec, [&]{return completions == 2;}));
    EXPECT_FALSE(last.success);
}
