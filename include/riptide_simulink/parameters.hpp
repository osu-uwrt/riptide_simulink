#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rclcpp/parameter_map.hpp>
#include <yaml-cpp/yaml.h>
#include <algorithm>
#include <memory>
#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace riptide_simulink {

inline std::vector<rclcpp::Parameter> load_ros_parameters(
    const std::string &file, const std::string &node_name)
{
    // The ROS parser handles wildcard node selectors and preserves native types.
    rcl_params_t *raw = rcl_yaml_node_struct_init(rcutils_get_default_allocator());
    if (!raw) throw std::runtime_error("Cannot allocate YAML parameter parser");
    std::unique_ptr<rcl_params_t, decltype(&rcl_yaml_node_struct_fini)> guard(raw, rcl_yaml_node_struct_fini);
    if (!rcl_parse_yaml_file(file.c_str(), raw)) throw std::runtime_error("Invalid ROS parameter YAML: " + file);

    // merge every matching node section; a repeated name keeps the last one seen
    auto map = rclcpp::parameter_map_from(raw, node_name.c_str());
    std::vector<rclcpp::Parameter> result;
    for (const auto &entry : map) {
        for (const auto &p : entry.second) {
            auto existing = std::find_if(result.begin(), result.end(), [&](const auto &v) {return v.get_name() == p.get_name();});
            if (existing == result.end()) result.push_back(p); else *existing = p;
        }
    }
    return result;
}

struct LegacyYamlOptions { double numeric_scale = 1000000.0; std::string separator = "__"; };

inline int64_t scaled_integer(const YAML::Node &node, double scale) {
    const long double value = node.as<long double>() * static_cast<long double>(scale);
    // Upper bound exclusive avoids floating rounding of INT64_MAX to 2^63.
    if (!std::isfinite(value) || value < -9223372036854775808.0L || value >= 9223372036854775808.0L)
        throw std::out_of_range("Legacy parameter scaling overflows int64");
    return static_cast<int64_t>(value); // preserve legacy truncation towards zero
}

inline bool yaml_bool(const YAML::Node &node) {
    // quoted scalars are strings, never bools
    if (!node.IsScalar() || node.Tag() == "!") return false;
    const auto text = node.Scalar();
    return text == "true" || text == "false" || text == "True" || text == "False" || text == "TRUE" || text == "FALSE";
}

inline void flatten_legacy(const YAML::Node &node, const std::string &name,
    const LegacyYamlOptions &options, std::vector<rclcpp::Parameter> &result)
{
    // maps become separator-joined names
    if (node.IsMap()) {
        for (const auto &pair : node) {
            const auto key = pair.first.as<std::string>();
            if (key.empty() || key.find(options.separator) != std::string::npos)
                throw std::invalid_argument("Ambiguous legacy YAML key: " + key);
            flatten_legacy(pair.second, name.empty() ? key : name + options.separator + key, options, result);
        }
    } else if (yaml_bool(node)) {
        result.emplace_back(name, node.as<bool>());
    } else if (node.IsSequence()) {
        // arrays take their type from the first element
        if (node.size() == 0) throw std::invalid_argument("Cannot infer empty legacy array type: " + name);
        if (yaml_bool(node[0])) {
            std::vector<bool> values;
            for (const auto &value : node) {
                if (!yaml_bool(value)) throw std::invalid_argument("Mixed legacy array: " + name);
                values.push_back(value.as<bool>());
            }
            result.emplace_back(name, values);
        } else {
            std::vector<int64_t> values;
            for (const auto &value : node) {
                if (!value.IsScalar() || yaml_bool(value)) throw std::invalid_argument("Non-numeric legacy array: " + name);
                values.push_back(scaled_integer(value, options.numeric_scale));
            }
            result.emplace_back(name, values);
        }
    } else if (node.IsScalar()) {
        // numbers are scaled to integers, anything else stays a string
        if (node.Tag() == "!") result.emplace_back(name, node.as<std::string>());
        else {
            try { result.emplace_back(name, scaled_integer(node, options.numeric_scale)); }
            catch (const YAML::BadConversion &) { result.emplace_back(name, node.as<std::string>()); }
        }
    } else throw std::invalid_argument("Unsupported legacy YAML value: " + name);
}

inline std::vector<rclcpp::Parameter> load_legacy_parameters(
    const std::string &file, const LegacyYamlOptions &options = {})
{
    if (!std::isfinite(options.numeric_scale) || options.numeric_scale <= 0 || options.separator.empty())
        throw std::invalid_argument("Invalid legacy YAML scale/separator");

    const auto tree = YAML::LoadFile(file);
    if (!tree.IsMap()) throw std::invalid_argument("Legacy YAML root must be a map");
    std::vector<rclcpp::Parameter> result;
    flatten_legacy(tree, "", options, result);
    return result;
}

} // namespace riptide_simulink
