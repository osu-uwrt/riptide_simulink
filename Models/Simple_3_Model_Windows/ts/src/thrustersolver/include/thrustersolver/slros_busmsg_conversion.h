#ifndef _SLROS_BUSMSG_CONVERSION_H_
#define _SLROS_BUSMSG_CONVERSION_H_

#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <std_msgs/msg/int32_multi_array.hpp>
#include <std_msgs/msg/multi_array_dimension.hpp>
#include <std_msgs/msg/multi_array_layout.hpp>
#include "ThrusterSolver_types.h"
#include "slros_msgconvert_utils.h"


void convertFromBus(geometry_msgs::msg::Twist& msgPtr, SL_Bus_geometry_msgs_Twist const* busPtr);
void convertToBus(SL_Bus_geometry_msgs_Twist* busPtr, const geometry_msgs::msg::Twist& msgPtr);

void convertFromBus(geometry_msgs::msg::Vector3& msgPtr, SL_Bus_geometry_msgs_Vector3 const* busPtr);
void convertToBus(SL_Bus_geometry_msgs_Vector3* busPtr, const geometry_msgs::msg::Vector3& msgPtr);

void convertFromBus(std_msgs::msg::Int32MultiArray& msgPtr, SL_Bus_std_msgs_Int32MultiArray const* busPtr);
void convertToBus(SL_Bus_std_msgs_Int32MultiArray* busPtr, const std_msgs::msg::Int32MultiArray& msgPtr);

void convertFromBus(std_msgs::msg::MultiArrayDimension& msgPtr, SL_Bus_std_msgs_MultiArrayDimension const* busPtr);
void convertToBus(SL_Bus_std_msgs_MultiArrayDimension* busPtr, const std_msgs::msg::MultiArrayDimension& msgPtr);

void convertFromBus(std_msgs::msg::MultiArrayLayout& msgPtr, SL_Bus_std_msgs_MultiArrayLayout const* busPtr);
void convertToBus(SL_Bus_std_msgs_MultiArrayLayout* busPtr, const std_msgs::msg::MultiArrayLayout& msgPtr);


#endif
