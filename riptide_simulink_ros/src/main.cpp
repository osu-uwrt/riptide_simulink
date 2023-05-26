#include <rclcpp/rclcpp.hpp>

#include "riptide_simulink_ros/sim_ros_wrapper.hpp"

using namespace riptide_simulink_ros;

int main (int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SimulinkWrapper>());
    rclcpp::shutdown();
}