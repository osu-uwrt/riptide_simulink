%set up inputs
setPoint1 = [2;.01;0;0;0;0;1];
currentWorldFramePosition1 = [0;0;0;0;0;0;1];
currentVelocities1 = [0;0;0;0;0;0];

%m/s ..ss ...sss
vMax1 = [.5;.5;.5;.1;.1;.1];
aMax1 = [.1;.1;.1;.02;.02;.02];
jMax1 = [.1;.1;.1;.02;.02;.02];

%call the curve generator
createMotionCurves(setPoint1, currentWorldFramePosition1, currentVelocities1, vMax1, aMax1, jMax1);


%actual function to be ran in motion planning block
function [motionPlanningCoefficents, R2, tEst] = createMotionCurves(setPoint, currentWorldFramePosition, currentVelocities, vMax, aMax, jMax)
    %change quaternion bas orentation into euler based
    %remember ros2 quat

    setOrientationEul = quat2eul([setPoint(7), setPoint(4), setPoint(5), setPoint(6)]);
    posOrientationEul = quat2eul([currentWorldFramePosition(7), currentWorldFramePosition(4), currentWorldFramePosition(5), currentWorldFramePosition(6)]);

    set = [setPoint(1); setPoint(2); setPoint(3); setOrientationEul(1); setOrientationEul(2); setOrientationEul(3)];
    pos = [currentWorldFramePosition(1); currentWorldFramePosition(2); currentWorldFramePosition(3); posOrientationEul(1); posOrientationEul(2); posOrientationEul(3)];

    %ignore second order effects and assume bidirectional symetry

    %determine which path  - dof - will take the longest to complete
    times = zeros(6, 1);
    for i = 1:6
        if(set(i) == pos(i))
            % there is no movement required
            times(i) = 0;

        else
            %gotta move
           
            %get distance needed to travel
            distance = abs(set(i) - pos(i));

            %ramp up to max acceleration
            jTime = aMax(i) / jMax(i);
            jDist = jTime^3 * jMax(i) / 6;
            jVel = jTime^2 * jMax(i) / 2;

            if(jDist * 2 > distance)
                %total distance is less than the distance required to jamp
                %up to max accel

                times(i) = ((distance / 2) * 6 / jMax(i)) ^ (1/3) * 2;
            else
                %accelerate at max until max velocity achieved
                 aTime = (vMax(i) - jVel) / aMax(i);
                 aDist = aMax(i) * aTime ^ 2 / 2 + jVel * aTime;

                 if((aDist + jDist) > distance / 2)
                    %distance taken to reach max velocity is greater than
                    %the distance needed to travel

                    %2 to account for accel and decel
                    times(i) = 2 * (jTime + (-jVel + sqrt(jVel^2 + 2 * aMax(i) * (distance / 2 - jDist))) / aMax(i));
                 else
                    %will achieve max velocity
                    times(i) = 2 * (aTime + jTime + (distance / 2 - jDist - aDist) / vMax(i));
                 end
            end
        end
    end

    %the duration of all the curve should be the longest minumum time
    dur = max(times);
    
    samplePoints = 10000;
    points = zeros(6, samplePoints);
    %go through a get the sample points for each curve
    for i=1:6
        if(pos(i) == set(i))
            %no movement required

            for t = 1:samplePoints
                points(i, t) = set(i);
            end
        
        else
            %get movement sign - accounted for zero case above
            sign = (set(i) - pos(i)) / abs(set(i) - pos(i));
           
            %get distance needed to travel
            distance = abs(set(i) - pos(i));

            %ramp up to max acceleration
            jTime = aMax(i) / jMax(i);
            jDist = jTime^3 * jMax(i) / 6;
            jVel = jTime^2 * jMax(i) / 2;

            if(distance / 2 < jDist)
                %distance needed to reach max accel > distance needed to
                %travel
                
                %determine the constant jerk needed
                j = (distance / 2) * 6 / ((dur / 2) ^ 3);

                %get sample points on curve
                tVals = linspace(0, dur, samplePoints);

                posVals = zeros(1, samplePoints);
                posVals(1:(samplePoints / 2)) = tVals(1:(samplePoints / 2)) .^ 3 .* j ./ 6;
                posVals((samplePoints / 2 + 1):samplePoints) = distance - (abs(dur - tVals((samplePoints / 2 + 1):samplePoints))) .^ 3 .* j ./ 6;

                points(i, :) = posVals * sign;
            else
                if((distance / 2) < jDist + (dur / 2 - jTime) * jVel)
                    %gotta use above algorithm

                    %determine the constant jerk needed
                    j = (distance / 2) * 6 / ((dur / 2) ^ 3);
    
                    %get sample points on curve
                    tVals = linspace(0, dur, samplePoints);
    
                    posVals = zeros(1, samplePoints);
                    posVals(1:(samplePoints / 2)) = tVals(1:(samplePoints / 2)) .^ 3 .* j ./ 6;
                    posVals((samplePoints / 2 + 1):samplePoints) = distance - (abs(dur - tVals((samplePoints / 2 + 1):samplePoints))) .^ 3 .* j ./ 6;
    
                    points(i, :) = posVals * sign;
                else
                    % need to accelerate more

                    %calculate the max velocity
                    a = 1 / (2 * aMax(i));
                    b = (jVel / (aMax(i)) - (dur / 2 - jTime));
                    c = (-3 * jVel^2 / (2 * aMax(i)) - 2 * jVel * (dur / 2 - jTime) + (distance / 2 - jDist));

                  
                    v = (-b + sqrt(b^2 - 4 * a *c)) / (2 * a);

                    %time spent in acceleration
                    aTime = (v - jVel) / aMax(i);
                    aDist = aTime ^2 * aMax(i) / 2 + aTime * jVel;

                    disp(v);

                    %get sample points on curve
                    tVals = linspace(0, dur, samplePoints);
                    posVals = zeros(1, samplePoints);

                    for t = 1:(samplePoints / 2)
                        %loop through each time

                        if(tVals(t) < jTime)
                           
                            %following max jerk curve
                            posVals(t) = tVals(t) ^ 3 / 6 * jMax(i);
                        elseif(tVals(t) < aTime + jTime)

                            % follow max acceleration curve
                            posVals(t) = (tVals(t) - jTime) ^ 2 * aMax(i) / 2 + (tVals(t) - jTime) * jVel + jDist;
                        else

                            %follow steady velocity curve
                            posVals(t) = aDist + jDist + (tVals(i) - jTime - aTime) * v;
                        end
                    end
                    
                    %curve is radially symetrical
                    for t = (samplePoints / 2 + 1):samplePoints
                        posVals(t) = distance - posVals(samplePoints - t + 1);
                    end
             
                    points(i, :) = posVals .* sign;
                end
            end

        end

    end

    plot(linspace(0, dur, samplePoints), points(1, :))
end