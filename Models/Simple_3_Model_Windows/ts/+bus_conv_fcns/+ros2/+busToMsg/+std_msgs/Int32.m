function rosmsgOut = Int32(slBusIn, rosmsgOut)
%#codegen
%   Copyright 2021 The MathWorks, Inc.
    rosmsgOut.data = int32(slBusIn.data);
end
