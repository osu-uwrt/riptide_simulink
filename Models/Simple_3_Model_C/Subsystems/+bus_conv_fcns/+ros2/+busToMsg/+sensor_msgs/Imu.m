function rosmsgOut = Imu(slBusIn, rosmsgOut)
%#codegen
%   Copyright 2021 The MathWorks, Inc.
    rosmsgOut.header = bus_conv_fcns.ros2.busToMsg.std_msgs.Header(slBusIn.header,rosmsgOut.header(1));
    rosmsgOut.orientation = bus_conv_fcns.ros2.busToMsg.geometry_msgs.Quaternion(slBusIn.orientation,rosmsgOut.orientation(1));
    rosmsgOut.orientation_covariance = double(slBusIn.orientation_covariance);
    rosmsgOut.angular_velocity = bus_conv_fcns.ros2.busToMsg.geometry_msgs.Vector3(slBusIn.angular_velocity,rosmsgOut.angular_velocity(1));
    rosmsgOut.angular_velocity_covariance = double(slBusIn.angular_velocity_covariance);
    rosmsgOut.linear_acceleration = bus_conv_fcns.ros2.busToMsg.geometry_msgs.Vector3(slBusIn.linear_acceleration,rosmsgOut.linear_acceleration(1));
    rosmsgOut.linear_acceleration_covariance = double(slBusIn.linear_acceleration_covariance);
end
