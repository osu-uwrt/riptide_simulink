// Copyright 2022 The MathWorks, Inc.
// Generated 10-Oct-2023 19:23:45
#include "slros2_initialize.h"
// ThrusterSolver/Publish Thruster Forces
SimulinkPublisher<std_msgs::msg::Int16MultiArray,SL_Bus_std_msgs_Int16MultiArray> Pub_ThrusterSolver_113;
// ThrusterSolver/Active Input
SimulinkSubscriber<geometry_msgs::msg::Twist,SL_Bus_geometry_msgs_Twist> Sub_ThrusterSolver_115;
// ThrusterSolver/FF Input
SimulinkSubscriber<geometry_msgs::msg::Twist,SL_Bus_geometry_msgs_Twist> Sub_ThrusterSolver_114;
// For Block ThrusterSolver/Get Thruster Weights
SimulinkParameterArrayGetter<real64_T,std::vector<int64_t>> ParamGet_ThrusterSolver_146;
// For Block ThrusterSolver/Get Thruster Wrench Matrix
SimulinkParameterArrayGetter<real64_T,std::vector<int64_t>> ParamGet_ThrusterSolver_126;
// For Block ThrusterSolver/Individual Limit
SimulinkParameterGetter<real64_T,double> ParamGet_ThrusterSolver_147;
// For Block ThrusterSolver/RPM Spline Coeffs
SimulinkParameterArrayGetter<real64_T,std::vector<int64_t>> ParamGet_ThrusterSolver_160;
// For Block ThrusterSolver/RPM Spline Segment Cutoffs
SimulinkParameterArrayGetter<real64_T,std::vector<int64_t>> ParamGet_ThrusterSolver_165;
// For Block ThrusterSolver/System Limit
SimulinkParameterGetter<real64_T,double> ParamGet_ThrusterSolver_148;
