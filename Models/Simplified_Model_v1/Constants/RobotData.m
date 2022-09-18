%Robot to be configured
%This file should be run prior to running simulink mode!

%The robot's orgin is on the back left wing!!!!!

%Robot Mass -----------------
importedData.robot.mass = 33.7848; %kg

%Robot Center of Mass -------------------
%This should be exactly what is in riptide_descriptions
%x and y should be flipped only when read of the com machine
importedData.robot.com = [-0.4125, 0.377, -0.112]; 

%Moments of interita -- i think
importedData.robot.inertia = [0.9978, 1.6934, 2.1284]; %kg*m^2

%Robot Center of Bouancy-------------------------
importedData.robot.cob = [-0.41001, 0.3811, -.10810]; %m

%Robot Volume ------------------------------
importedData.robot.volume = 0.0348;%m^3

if(exist("RobotOrginTransform", "var") ~= 1)
    disp("Cannot find RobotOrginTransform! Prep sim may need to be run...");
    return
end

%Do transforms
importedData.robot.com = transformPointsForward(RobotOrginTransform, importedData.robot.com);
importedData.robot.cob = transformPointsForward(RobotOrginTransform, importedData.robot.cob);

%Write transformations to file to make simulink happy
FID = fopen("Parameters/robotCenterOfMass.m", "w");
fprintf(FID, 'robotCoM=[%f %f %f]', importedData.robot.com(1), importedData.robot.com(2), importedData.robot.com(3));
FID = fopen("Parameters/robotMomentOfInertia.m", "w");
fprintf(FID, 'robotMoI=[%f %f %f]', importedData.robot.inertia(1), importedData.robot.inertia(2), importedData.robot.inertia(3));
FID = fopen("Parameters/robotMass.m", "w");
fprintf(FID, 'robot_Mass=%f', importedData.robot.mass);

