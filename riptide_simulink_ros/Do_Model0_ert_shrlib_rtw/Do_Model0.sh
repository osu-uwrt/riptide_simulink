#!/bin/sh
export CMAKE_EXPORT_COMPILE_COMMANDS="TRUE"
/usr/local/MATLAB/R2022b/bin/glnxa64/cmake/bin/cmake -S . -B build -G "Unix Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release  -DCMAKE_PLATFORM_INFO_INITIALIZED:INTERNAL=1 --no-warn-unused-cli -DCMAKE_INSTALL_PREFIX=".."
COMMAND_STATUS=$?
if [ $COMMAND_STATUS -ne 0 ]
then
    echo "The cmake command returned a non-zero exit code $COMMAND_STATUS" >&2
    exit $COMMAND_STATUS
fi

/usr/local/MATLAB/R2022b/bin/glnxa64/cmake/bin/cmake --build build
COMMAND_STATUS=$?
if [ $COMMAND_STATUS -ne 0 ]
then
    echo "The cmake command returned a non-zero exit code $COMMAND_STATUS" >&2
    exit $COMMAND_STATUS
fi

/usr/local/MATLAB/R2022b/bin/glnxa64/cmake/bin/cmake --install build --prefix ..
COMMAND_STATUS=$?
if [ $COMMAND_STATUS -ne 0 ]
then
    echo "The cmake command returned a non-zero exit code $COMMAND_STATUS" >&2
    exit $COMMAND_STATUS
fi

