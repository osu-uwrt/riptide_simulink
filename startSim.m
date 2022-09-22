%Must be ran to start the simulation

clear;

disp("Start Riptide Simulink Simulation!");

%switch to appriopriate sim directory
simDirectoryPath = "Models/Simplified_Model_v1";
cd(simDirectoryPath)

%import constants into workspace
disp("Importing constants!")

cd("Constants");
RobotData;
WorldData;

%run the prep sim
cd("..")

disp("Running Prep Sim!")

GatherDataSimulation;
set_param("GatherDataSimulation", "SimulationCommand", "start");
status = get_param("GatherDataSimulation", "SimulationStatus");

pause(10);

if(exist("out", "var") ~= 1)
    disp("Prep sim failed to run!!!")
    return
end

disp("Prep Sim Complete!")

%Define the transform from software orgin to tempest base frame
RobotOrginTransform = rigid3d(out.SoftwareTransform_transform.signals.values(:,:,1),[out.SoftwareTransform_x(:,:,1),out.SoftwareTransform_y(:,:,1),out.SoftwareTransform_z(:,:,1)]);

%import constants into workspace
disp("Transforming constants!")

cd("Constants");
RobotData;
WorldData;

%Run real sim
cd("..")

disp("Starting Robot Simulator!")
SimplifiedAssembly;

% finish back in starting directory
cd("../..")