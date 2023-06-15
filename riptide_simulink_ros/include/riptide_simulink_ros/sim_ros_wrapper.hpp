#pragma once

#include <vector>

#include <rclcpp/rclcpp.hpp>

#include <nav_msgs/msg/odometry.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <riptide_msgs2/msg/controller_command.hpp>

#include <riptide_msgs2/action/follow_path.hpp>
#include <geometry_msgs/msg/transform.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <sensor_msgs/msg/imu.hpp>

namespace riptide_simulink_ros
{
    typedef geometry_msgs::msg::Vector3 Vector3Msg;
    typedef geometry_msgs::msg::Point PointMsg;
    typedef geometry_msgs::msg::Twist TwistMsg;
    typedef geometry_msgs::msg::Quaternion QuatMsg;

    typedef geometry_msgs::msg::PoseStamped PoseStampedMsg;
    typedef geometry_msgs::msg::TwistStamped TwistStampedMsg;
    typedef std_msgs::msg::Float32MultiArray ThrusterForceMsg;
    typedef sensor_msgs::msg::Imu ImuMsg;


    const size_t NUM_THRUSTERS = 8;
    // I don't actually understand what this constant is for
    // TODO: Ask Alex
    const double GRADIENT_ANG_COEFF = .017453292;

    class SimulinkWrapper : public rclcpp::Node {
    public:
        SimulinkWrapper();
        ~SimulinkWrapper();

        bool loadConfig(std::string path);

        void updateConfig();

        // callback for storing thruster forces
        void thrusterForceCallback(const ThrusterForceMsg msg);

        void tick();

    private:
        
        // Subscriptions
        rclcpp::Subscription<ThrusterForceMsg>::SharedPtr thruster_force_sub;

        // Publisher
        rclcpp::Publisher<PoseStampedMsg>::SharedPtr pose_pub;
        rclcpp::Publisher<TwistStampedMsg>::SharedPtr twist_pub;
        rclcpp::Publisher<ImuMsg>::SharedPtr imu_pub;

        // Timer for updating the simulation
         rclcpp::TimerBase::SharedPtr tick_timer;

        // Stores most inputs for the sim.
        // Note that this does not store anything related to gradient bouyancy, since that
        // is calculated right before running the C simulation
        struct RobotState {
            PointMsg prev_lin_pos;
            Vector3Msg prev_ang_pos; // Orientation is stored in RPY, rather than as a quaternion
            TwistMsg prev_vel;
            Vector3Msg prev_lin_accel;

            // thruster forces
            float hsf;
            float hsa;
            float hpa;
            float hpf;
            float vsf;
            float vsa;
            float vpa;
            float vpf;
        } robot_state;

        QuatMsg euler2quat(float roll, float pitch, float yaw);
    };
} // namespace riptide_alloc_controller