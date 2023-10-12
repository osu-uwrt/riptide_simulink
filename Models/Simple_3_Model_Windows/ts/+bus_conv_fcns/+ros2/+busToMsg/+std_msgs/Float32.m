function rosmsgOut = Float32(slBusIn, rosmsgOut)
%#codegen
%   Copyright 2021 The MathWorks, Inc.
    rosmsgOut.data = single(slBusIn.data);
end
