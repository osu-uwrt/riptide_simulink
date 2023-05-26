include("${CMAKE_CURRENT_LIST_DIR}/Do_Model0Targets.cmake")
target_include_directories(Do_Model0::Do_Model0 INTERFACE 
    ${MATLAB_ROOT}/extern/include
    ${MATLAB_ROOT}/simulink/include
    ${MATLAB_ROOT}/rtw/c/src
    ${MATLAB_ROOT}/rtw/c/src/ext_mode/common
    ${MATLAB_ROOT}/rtw/c/ert)
