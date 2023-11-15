// Copyright 2022 The MathWorks, Inc.
// Generated 21-Oct-2023 17:56:58
#ifndef _SLROS2_INITIALIZE_H_
#define _SLROS2_INITIALIZE_H_
#include "FF_Controller_types.h"
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
// FF_Controller/Publish/Publish
extern SimulinkPublisher<geometry_msgs::msg::Twist,SL_Bus_geometry_msgs_Twist> Pub_FF_Controller_59;
// FF_Controller/Odom
extern SimulinkSubscriber<nav_msgs::msg::Odometry,SL_Bus_nav_msgs_Odometry> Sub_FF_Controller_60;
// For Block FF_Controller/Buoyant Force
extern SimulinkParameterGetter<int64_T,int64_t> ParamGet_FF_Controller_69;
// For Block FF_Controller/COB
extern SimulinkParameterArrayGetter<int64_T,std::vector<int64_t>> ParamGet_FF_Controller_67;
// For Block FF_Controller/COM
extern SimulinkParameterArrayGetter<int64_T,std::vector<int64_t>> ParamGet_FF_Controller_68;
// For Block FF_Controller/Mass
extern SimulinkParameterGetter<int64_T,int64_t> ParamGet_FF_Controller_70;
// For Block FF_Controller/Scaling Factor
extern SimulinkParameterGetter<int64_T,int64_t> ParamGet_FF_Controller_71;
#endif
