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
else
    %Do transforms   
    importedData.robot.com = transformPointsForward(RobotOrginTransform, importedData.robot.com);
    importedData.robot.cob = transformPointsForward(RobotOrginTransform, importedData.robot.cob);
end

%Write transformations to file to make simulink happy
FID = fopen("Parameters/robotParameters.m", "w");
fprintf(FID, 'robotCoM=[%f %f %f]\n', importedData.robot.com(1), importedData.robot.com(2), importedData.robot.com(3));
fprintf(FID, 'robotMoI=[%f %f %f]\n', importedData.robot.inertia(1), importedData.robot.inertia(2), importedData.robot.inertia(3));
fprintf(FID, 'robot_Mass=%f\n', importedData.robot.mass);
fprintf(FID, 'robotCoB=[%f %f %f]\n', importedData.robot.cob(1), importedData.robot.cob(2), importedData.robot.cob(3));
fprintf(FID, 'robot_Volume=%f\n', importedData.robot.volume);
fprintf(FID, 'rollDragCoefficents [%f %f]' import)


cd("Parameters");
robotParameters;
cd("..");


