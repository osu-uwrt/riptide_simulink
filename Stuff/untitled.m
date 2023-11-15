tStep = .001;

%times to evaluate
tVal = 0:tStep:100;
sVal = zeros(1, length(tVal));
eVal = zeros(1, length(tVal));
posVal = zeros(1, length(tVal));

%initial difference from sliding surface
Si = 1;
eta = .01;
lamda = 1;
initalError = 10;
C = initalError - Si;

S = @(t, Sp) Sp - eta/lamda * sign(Sp);
E = @(t, Sp) C * exp(-t/lamda) + S(t, Sp);

posSlid = 0;
slideRate = .15;

Sp = Si;
for i = 1:length(tVal)
    sVal(i) = S(tVal(i), Sp);
    Sp = S(tVal(i), Sp);
    eVal(i) = E(tVal(i), Sp);

    %for time t = 20 to t = 30 - slide along (t - 20) * slidRate;

    if(tVal(i) > 20 && tVal(i) < 30)
        Sp = Sp + slideRate * tStep;
        posSlid = posSlid + slideRate *tStep;
    end

    %position of vehicle
    posVal(i) = posSlid + eVal(i);
    

end

figure(1);
plot(tVal,sVal);

figure(2);
plot(tVal, eVal);

figure(3);
plot(tVal, posVal);

