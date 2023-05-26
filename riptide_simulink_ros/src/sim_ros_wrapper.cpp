#include <cstdlib>
#include <chrono>
#include <rclcpp/rclcpp.hpp>

#include "riptide_simulink_ros/sim_ros_wrapper.hpp"
#include "riptide_simulink_ros/GradientBouyancyLib.h"
extern "C" {
#include "Do_Model0.h"
}
using namespace riptide_simulink_ros;
using namespace std::chrono_literals;

using std::placeholders::_1;

SimulinkWrapper::SimulinkWrapper() : rclcpp::Node("ros_simulink_wrapper") {
    thruster_force_sub = create_subscription<ThrusterForceMsg>(
        "simulation/thruster_forces", 
        rclcpp::SensorDataQoS(), 
        std::bind(&SimulinkWrapper::thrusterForceCallback, 
        this, 
        _1));

    pose_pub = create_publisher<PoseStampedMsg>("simulator/pose", rclcpp::SystemDefaultsQoS());
    twist_pub = create_publisher<TwistStampedMsg>("simulator/twist", rclcpp::SystemDefaultsQoS());
    imu_pub = create_publisher<ImuMsg>("simulator/imu", rclcpp::SystemDefaultsQoS());

    this->create_wall_timer(33ms, std::bind(&SimulinkWrapper::tick, this));

    Do_Model0_initialize();
}

SimulinkWrapper::~SimulinkWrapper() {
    Do_Model0_terminate();
}

void SimulinkWrapper::thrusterForceCallback(const ThrusterForceMsg msg) {
    if (msg.data.size() != NUM_THRUSTERS) {
        RCLCPP_ERROR(this->get_logger(), "simulation/thruster_forces reports an unexpected number of thrusters. Reported: %d. Expected %d: ",
            msg.data.size(),
            NUM_THRUSTERS);
        exit(EXIT_FAILURE);
    }

    robot_state.hsf = msg.data[0];
    robot_state.hsa = msg.data[1];
    robot_state.hpa = msg.data[2];
    robot_state.hpf = msg.data[3];
    robot_state.vsf = msg.data[4];
    robot_state.vsa = msg.data[5];
    robot_state.vpa = msg.data[6];
    robot_state.vpf = msg.data[7];
}

void SimulinkWrapper::tick() {
    GradientBouyancyResults results = calculateBouyancy(
        0, 
        0, 
        robot_state.prev_lin_pos.z, 
        GRADIENT_ANG_COEFF * robot_state.prev_ang_pos.x, // roll
        GRADIENT_ANG_COEFF * robot_state.prev_ang_pos.y, // pitch
        0);
    
    Do_Model0_U.GradientBouyancyForcePosition[0] = results.location.x;
    Do_Model0_U.GradientBouyancyForcePosition[1] = results.location.y;
    Do_Model0_U.GradientBouyancyForcePosition[2] = results.location.z;

    Do_Model0_U.GradientBouyancyForce = results.force;

    Do_Model0_U.PreviousLinearPosition[0] = robot_state.prev_lin_pos.x;
    Do_Model0_U.PreviousLinearPosition[1] = robot_state.prev_lin_pos.y;
    Do_Model0_U.PreviousLinearPosition[2] = robot_state.prev_lin_pos.z;

    Do_Model0_U.PreviousAngularPosition[0] = robot_state.prev_ang_pos.x;
    Do_Model0_U.PreviousAngularPosition[1] = robot_state.prev_ang_pos.y;
    Do_Model0_U.PreviousAngularPosition[2] = robot_state.prev_ang_pos.z;

    Do_Model0_U.PreviousLinearVelocity[0] = robot_state.prev_lin_pos.x;
    Do_Model0_U.PreviousLinearVelocity[1] = robot_state.prev_lin_pos.y;
    Do_Model0_U.PreviousLinearVelocity[2] = robot_state.prev_lin_pos.z;

    Do_Model0_U.PreviousAngularVelocity[0] = robot_state.prev_vel.angular.x;
    Do_Model0_U.PreviousAngularVelocity[1] = robot_state.prev_vel.angular.y;
    Do_Model0_U.PreviousAngularVelocity[2] = robot_state.prev_vel.angular.z;

    Do_Model0_U.x1 = robot_state.hsf;
    Do_Model0_U.x2 = robot_state.hsa;
    Do_Model0_U.x3 = robot_state.hpa;
    Do_Model0_U.x4 = robot_state.hpf;
    Do_Model0_U.x5 = robot_state.vsf;
    Do_Model0_U.x6 = robot_state.vsa;
    Do_Model0_U.x7 = robot_state.vpa;
    Do_Model0_U.x8 = robot_state.vpf;

    Do_Model0_step();
}