// Copyright 2022 The MathWorks, Inc.
// Generated 27-Oct-2023 23:36:28
#include "slros2_initialize.h"
// ThrusterSolver/Publish To ROS/Publish Thruster Forces1
SimulinkPublisher<std_msgs::msg::Int32MultiArray,SL_Bus_std_msgs_Int32MultiArray> Pub_ThrusterSolver_210;
// ThrusterSolver/Publish To ROS/Publish Thruster Forces2
SimulinkPublisher<std_msgs::msg::Int32MultiArray,SL_Bus_std_msgs_Int32MultiArray> Pub_ThrusterSolver_226;
// ThrusterSolver/Active Input
SimulinkSubscriber<geometry_msgs::msg::Twist,SL_Bus_geometry_msgs_Twist> Sub_ThrusterSolver_115;
// ThrusterSolver/FF Input
SimulinkSubscriber<geometry_msgs::msg::Twist,SL_Bus_geometry_msgs_Twist> Sub_ThrusterSolver_114;
// ThrusterSolver/Subscribe
SimulinkSubscriber<std_msgs::msg::Int32MultiArray,SL_Bus_std_msgs_Int32MultiArray> Sub_ThrusterSolver_272;
// For Block ThrusterSolver/Force To RPM Curve Parameters
SimulinkParameterArrayGetter<int64_T,std::vector<int64_t>> ParamGet_ThrusterSolver_205;
// For Block ThrusterSolver/Get Thruster Wrench Matrix
SimulinkParameterArrayGetter<real64_T,std::vector<int64_t>> ParamGet_ThrusterSolver_126;
// For Block ThrusterSolver/Individual Limit
SimulinkParameterGetter<int64_T,int64_t> ParamGet_ThrusterSolver_147;
// For Block ThrusterSolver/Individual Limit1
SimulinkParameterGetter<int64_T,int64_t> ParamGet_ThrusterSolver_327;
// For Block ThrusterSolver/Individual Limit2
SimulinkParameterGetter<boolean_T,bool> ParamGet_ThrusterSolver_335;
// For Block ThrusterSolver/Scaling Factor
SimulinkParameterGetter<int64_T,int64_t> ParamGet_ThrusterSolver_197;
// For Block ThrusterSolver/System Limit
SimulinkParameterGetter<int64_T,int64_t> ParamGet_ThrusterSolver_148;
