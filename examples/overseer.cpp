#include <riptide_simulink/simulink_model.hpp>

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("example_overseer");
    const auto file = node->declare_parameter<std::string>("parameter_file", "");
    const auto target = node->declare_parameter<std::string>("model_node", "/robot/model");

    // push the file's parameters whenever the model node (re)appears
    riptide_simulink::ModelOptions options;
    options.reload_service = "reload_model_parameters";
    riptide_simulink::SimulinkModel model(node, target, options, [file, target]() {
        // Specialized overseers may append computed parameters here.
        return riptide_simulink::load_ros_parameters(file, target);
    });

    model.on_reload([node](const auto &result) {
        RCLCPP_INFO(node->get_logger(), "Model parameter reload: %s (%s)", result.success ? "ready" : "failed", result.message.c_str());
    });

    rclcpp::spin(node);
    rclcpp::shutdown();
}
