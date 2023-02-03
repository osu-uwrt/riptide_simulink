function [force, x_acting, y_acting, z_acting]= fcn(x_position, y_position, z_position, roll, pitch, yaw)
     command = "./GradientBouyancy.out " + x_position + " " + y_position + " " + z_position + " " + roll + " " + pitch + " " + yaw;
 
     [~, cmdout] = system(command);
    outputs = strsplit(cmdout, "|");

    X = str2double(outputs);

    force = X(1);
    x_acting = X(2);
    y_acting = X(3);
    z_acting = X(4);

end

