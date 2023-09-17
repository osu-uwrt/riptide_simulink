cd("Constants")

RobotConstants;
WorldData;

cd("../ThrusterSolver")

CreateWeights;

cd("..")

%add subsytems to path to allow matlab to find
addpath("Subsystems/")
addpath("GradientBouyancySystem/")
addpath("ThrusterSolver/")

Simple_3_Model

