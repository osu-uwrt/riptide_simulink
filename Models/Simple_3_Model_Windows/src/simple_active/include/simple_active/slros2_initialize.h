// Copyright 2022 The MathWorks, Inc.
// Generated 27-Oct-2023 03:27:14
#ifndef _SLROS2_INITIALIZE_H_
#define _SLROS2_INITIALIZE_H_
#include "simple_active_types.h"
// Generic pub-sub header
#include "slros2_generic_pubsub.h"
// Generic service header
#include "slros2_generic_service.h"
#include "slros2_generic_param.h"
extern rclcpp::Node::SharedPtr SLROSNodePtr;
#ifndef SET_QOS_VALUES
#define SET_QOS_VALUES(qosStruct, hist, dep, dur, rel)  \
    {                                                   \
        qosStruct.history = hist;                       \
        qosStruct.depth = dep;                          \
        qosStruct.durability = dur;                     \
        qosStruct.reliability = rel;                    \
    }
#endif
inline rclcpp::QoS getQOSSettingsFromRMW(const rmw_qos_profile_t& qosProfile) {
    rclcpp::QoS qos(rclcpp::QoSInitialization::from_rmw(qosProfile));
    if (RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL == qosProfile.durability) {
        qos.transient_local();
    } else {
        qos.durability_volatile();
    }
    if (RMW_QOS_POLICY_RELIABILITY_RELIABLE == qosProfile.reliability) {
        qos.reliable();
    } else {
        qos.best_effort();
    }
    return qos;
}
// simple_active/Publish Active Control
extern SimulinkPublisher<geometry_msgs::msg::Twist,SL_Bus_geometry_msgs_Twist> Pub_simple_active_53;
// simple_active/Subscribe
extern SimulinkSubscriber<nav_msgs::msg::Odometry,SL_Bus_nav_msgs_Odometry> Sub_simple_active_33;
// simple_active/Subscribe1
extern SimulinkSubscriber<geometry_msgs::msg::Pose,SL_Bus_geometry_msgs_Pose> Sub_simple_active_39;
// For Block simple_active/Get Parameter
extern SimulinkParameterGetter<int64_T,int64_t> ParamGet_simple_active_43;
// For Block simple_active/Get Parameter1
extern SimulinkParameterGetter<int64_T,int64_t> ParamGet_simple_active_44;
// For Block simple_active/Get Parameter2
extern SimulinkParameterArrayGetter<int64_T,std::vector<int64_t>> ParamGet_simple_active_46;
// For Block simple_active/Get Parameter3
extern SimulinkParameterArrayGetter<int64_T,std::vector<int64_t>> ParamGet_simple_active_47;
// For Block simple_active/Get Parameter4
extern SimulinkParameterArrayGetter<int64_T,std::vector<int64_t>> ParamGet_simple_active_48;
#endif
