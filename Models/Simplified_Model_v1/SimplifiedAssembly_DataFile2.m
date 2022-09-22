% -------------------------------------------------------------------
%  Generated by MATLAB on 19-Sep-2022 16:23:05
%  MATLAB version: 9.12.0.2039608 (R2022a) Update 5
% -------------------------------------------------------------------
saveVarsMat = load('SimplifiedAssembly_DataFile2.mat');

importedData = struct;
importedData.world = struct;
importedData.world.densityOfWater = 997;
importedData.world.gravitationalAcceleration = ...
  [0 -9.80065 0];
importedData.world.g = -9.80065;

parameters = struct;
parameters.robot = struct;
parameters.robot.CoM = [0 0 0];

smiData = saveVarsMat.smiData; % <1x1 struct> unsupported class at smiData.RigidTransform(1).ID

clear saveVarsMat;