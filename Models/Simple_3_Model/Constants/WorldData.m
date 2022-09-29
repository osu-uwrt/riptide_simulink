%World Data to be used
%This file should be run prior to running simulink model

%water data
importedData.world.densityOfWater = 997; %kg/m^3

%gravitational data
importedData.world.gravitationalAcceleration = [0, -9.80065, 0]; % m/s^2
importedData.world.g = importedData.world.gravitationalAcceleration(2);