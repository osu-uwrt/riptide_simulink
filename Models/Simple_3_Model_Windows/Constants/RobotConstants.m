importedData.robot.mass = 29.942;
importedData.robot.height = 0.32;
importedData.robot.volume = 0.0305;
importedData.robot.com = [-0.157, 0.037, -0.022];
importedData.robot.cob = [-0.157, 0.037, -0.022];
importedData.robot.inertia = [1.493, 0.549, 1.399];
importedData.robot.thrusters.VUSPos = [-0.14, -0.26, 0.141];
importedData.robot.thrusters.HUSDir = [0.785, 0, -1.571];
importedData.robot.thrusters.VUPPos = [-0.14, 0.328, 0.141];
importedData.robot.thrusters.HUPDir = [-0.785, 0, 1.571];
importedData.robot.thrusters.HUSPos = [-0.134, -0.369, 0.242];
importedData.robot.thrusters.VUSDir = [0.0, 0.0, 0.0];
importedData.robot.thrusters.HUPPos = [-0.134, 0.438, 0.242];
importedData.robot.thrusters.VUPDir = [0.0, 0.0, 0.0];
importedData.robot.thrusters.HLSPos = [-0.14, -0.26, -0.141];
importedData.robot.thrusters.HLSDir = [-0.785, 0, -1.571];
importedData.robot.thrusters.HLPPos = [-0.14, 0.328, -0.141];
importedData.robot.thrusters.HLPDir = [0.785, 0, 1.571];
importedData.robot.thrusters.VLSPos = [-0.134, -0.372, -0.244];
importedData.robot.thrusters.VLSDir = [0.0, 0.0, 0.0];
importedData.robot.thrusters.VLPPos = [-0.134, 0.44, -0.244];
importedData.robot.thrusters.VLPDir = [0.0, 0.0, 0.0];
importedData.robot.coefficients.roll = [-6.0, -2.0, 0];
importedData.robot.coefficients.pitch = [-12.0, -4.5, 0];
importedData.robot.coefficients.yaw = [-12.0, -2.5, 0];
importedData.robot.coefficients.x = [-180.0, -17.0, 0];
importedData.robot.coefficients.y = [-180.0, -17.0, 0];
importedData.robot.coefficients.z = [-460.0, -17.0, 0];

importedData.robot.thrusters.systemLimit = 200;
importedData.robot.thrusters.individualLimit = 22;

%need to flip x and y rots
%error in HUS and HUP dir
%x and z rot needs flipped for vector thrusters
%add in thruster contraints


