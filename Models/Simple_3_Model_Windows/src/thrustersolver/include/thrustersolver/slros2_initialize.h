// Copyright 2022 The MathWorks, Inc.
// Generated 01-Oct-2023 13:05:46
#ifndef _SLROS2_INITIALIZE_H_
#define _SLROS2_INITIALIZE_H_
#include "ThrusterSolver_types.h"
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
// ThrusterSolver/Publish Thruster Forces
extern SimulinkPublisher<std_msgs::msg::Float32MultiArray,SL_Bus_std_msgs_Float32MultiArray> Pub_ThrusterSolver_113;
// ThrusterSolver/Active Input
extern SimulinkSubscriber<geometry_msgs::msg::Twist,SL_Bus_geometry_msgs_Twist> Sub_ThrusterSolver_115;
// ThrusterSolver/FF Input
extern SimulinkSubscriber<geometry_msgs::msg::Twist,SL_Bus_geometry_msgs_Twist> Sub_ThrusterSolver_114;
// For Block ThrusterSolver/Get Thruster Weights
extern SimulinkParameterArrayGetter<real64_T,std::vector<int64_t>> ParamGet_ThrusterSolver_146;
// For Block ThrusterSolver/Get Thruster Wrench Matrix
extern SimulinkParameterArrayGetter<real64_T,std::vector<int64_t>> ParamGet_ThrusterSolver_126;
// For Block ThrusterSolver/Individual Limit
extern SimulinkParameterGetter<int64_T,int64_t> ParamGet_ThrusterSolver_147;
// For Block ThrusterSolver/System Limit
extern SimulinkParameterGetter<int64_T,int64_t> ParamGet_ThrusterSolver_148;
#endif
