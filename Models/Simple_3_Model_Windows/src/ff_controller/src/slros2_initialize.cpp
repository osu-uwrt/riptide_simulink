// Copyright 2022 The MathWorks, Inc.
// Generated 21-Oct-2023 17:56:58
#include "slros2_initialize.h"
// FF_Controller/Publish/Publish
SimulinkPublisher<geometry_msgs::msg::Twist,SL_Bus_geometry_msgs_Twist> Pub_FF_Controller_59;
// FF_Controller/Odom
SimulinkSubscriber<nav_msgs::msg::Odometry,SL_Bus_nav_msgs_Odometry> Sub_FF_Controller_60;
// For Block FF_Controller/Buoyant Force
SimulinkParameterGetter<int64_T,int64_t> ParamGet_FF_Controller_69;
// For Block FF_Controller/COB
SimulinkParameterArrayGetter<int64_T,std::vector<int64_t>> ParamGet_FF_Controller_67;
// For Block FF_Controller/COM
SimulinkParameterArrayGetter<int64_T,std::vector<int64_t>> ParamGet_FF_Controller_68;
// For Block FF_Controller/Mass
SimulinkParameterGetter<int64_T,int64_t> ParamGet_FF_Controller_70;
// For Block FF_Controller/Scaling Factor
SimulinkParameterGetter<int64_T,int64_t> ParamGet_FF_Controller_71;
