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

%Drag -- these are in the descriptions coordinate frame
importedData.robot.linearLinearDragCoefficients = [-17.0, -17.0, -35.0]; %x,y,z
importedData.robot.angularLinearDragCoefficients = [-2.0, -4.5, -2.5]; %r,p,y
importedData.robot.linearQuadraticDragCoefficients = [-180.0, -180.0, -460.0];
importedData.robot.angularQuadraticDragCoefficients = [-6.0, -12.0, -12.0];

%Thrusters
importedData.robot.thrusters.HSFPos = [0.019, -0.051, -0.078];
importedData.robot.thrusters.HSFDir = [0.0, 1.57079, 0.0];
importedData.robot.thrusters.HSAPos = [-0.764, -0.051, -0.078];
importedData.robot.thrusters.HSADir = [0.0, 1.57079, 0.0];
importedData.robot.thrusters.HPAPos = [-0.764, 0.763, -0.078];
importedData.robot.thrusters.HPADir = [0.0, 1.57079, 0.0];
importedData.robot.thrusters.HPFPos = [0.019, 0.763, -0.078];
importedData.robot.thrusters.HPFDir = [0.0, 1.57079, 0.0];
importedData.robot.thrusters.VSFPos = [-0.04, 0.008, -0.0706];
importedData.robot.thrusters.VSFDir = [0.0, 0, -2.35619];
importedData.robot.thrusters.VSAPos = [-0.676, 0.008, -0.0706];
importedData.robot.thrusters.VSADir = [0.0, 0, -0.78540];
importedData.robot.thrusters.VPAPos = [-0.676 0.674, -0.0706];
importedData.robot.thrusters.VPADir = [0.0, 0, 0.78540];
importedData.robot.thrusters.VPFPos = [-0.04, 0.674, -0.0706];
importedData.robot.thrusters.VPFDir = [0.0, 0, 2.35619];


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

%drag will be applied after transform is solved;
%Can remian in descriptions frame
%The last term in all drag lists must be zero for zero constant drag!
%Because Matlab the highest order coefficent comes first...
fprintf(FID, 'robotRollDragCoefficients=[%f %f 0]\n', importedData.robot.angularQuadraticDragCoefficients(1), importedData.robot.angularLinearDragCoefficients(1));
fprintf(FID, 'robotPitchDragCoefficients=[%f %f 0]\n', importedData.robot.angularQuadraticDragCoefficients(2), importedData.robot.angularLinearDragCoefficients(2));
fprintf(FID, 'robotYawDragCoefficients=[%f %f 0]\n', importedData.robot.angularQuadraticDragCoefficients(3), importedData.robot.angularLinearDragCoefficients(3));
fprintf(FID, 'robotXDragCoefficients=[%f %f 0]\n', importedData.robot.linearQuadraticDragCoefficients(1), importedData.robot.linearLinearDragCoefficients(1));
fprintf(FID, 'robotYDragCoefficients=[%f %f 0]\n', importedData.robot.linearQuadraticDragCoefficients(2), importedData.robot.linearLinearDragCoefficients(2));
fprintf(FID, 'robotZDragCoefficients=[%f %f 0]\n', importedData.robot.linearQuadraticDragCoefficients(3), importedData.robot.linearLinearDragCoefficients(3));


cd("Parameters");
robotParameters;
cd("..");