timeVector = clock();
time = timeVector(1) * 3.514e7 + timeVector(2) * 2.628e6 + timeVector(3) * 86400 + timeVector(4) * 3600 + timeVector(5) * 60 + timeVector(6);
timeout = time + 10;

angularPosition = [pi/2; pi/2 + .00001; pi/2];
inverse = [nan, nan, nan; nan, nan, nan; nan, nan, nan];
singular = false;

while(sum(sum(isnan(inverse))) ~= 0)

    if(singular)
        disp("Singular Matrix Found -- Adding Noise!")
        angularPosition = angularPosition + [rand() * 10^-5; rand() * 10^-5; rand() * 10^-5];
    end


    timeVector = clock(); topic
    time = timeVector(1) * 3.514e7 + timeVector(2) * 2.628e6 + timeVector(3) * 86400 + timeVector(4) * 3600 + timeVector(5) * 60 + timeVector(6);

    if(time > timeout)
        break
    end

    rotation = eul2rotm([angularPosition(1,1), angularPosition(2,1), angularPosition(3,1)])
    inverse = inv(rotation)

    singular = true; % not nessecarily true, but if it loops again it is
end