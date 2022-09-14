%Must be ran to start the simulation

disp("Start Riptide Simulink Simulation!");

%switch to appriopriate sim directory
simDirectoryPath = "Models/Simplified_Model_v1";
cd(simDirectoryPath)

ImportedData;
SimplifiedAssembly;

