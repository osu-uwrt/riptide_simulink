######################################################################
# CMakeLists.txt generated for component Do_Model0
# Product type: shared library
######################################################################
cmake_minimum_required(VERSION 3.12)
project(Do_Model0)
if(DEFINED ENV{CMAKE_EXPORT_COMPILE_COMMANDS} AND NOT DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS $ENV{CMAKE_EXPORT_COMPILE_COMMANDS})
endif()


######################################################################
## Path variables
######################################################################
get_filename_component(START_DIR .. ABSOLUTE)
set(MATLAB_ROOT /usr/local/MATLAB/R2022b CACHE PATH "")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
if(${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(BUILDING_IN_SOURCE_DIR TRUE)
    set(BINARY_START_DIR "${START_DIR}")
else()
    set(BUILDING_IN_SOURCE_DIR FALSE)
    set(BINARY_START_DIR "${CMAKE_BINARY_DIR}")
endif()

######################################################################
## Target definition and commands
######################################################################
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()
add_library(Do_Model0 SHARED ${START_DIR}/Do_Model0_ert_shrlib_rtw/Do_Model0.c
    ${START_DIR}/Do_Model0_ert_shrlib_rtw/Do_Model0_data.c)
set_target_properties(Do_Model0 PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON)
if(CMAKE_CXX_COMPILER)
    set_target_properties(Do_Model0 PROPERTIES LINKER_LANGUAGE CXX)
endif()
target_compile_features(Do_Model0 PUBLIC c_std_99)
target_compile_definitions(Do_Model0 PRIVATE -DMODEL=Do_Model0
    -DNUMST=2
    -DNCSTATES=0
    -DHAVESTDIO
    -DMODEL_HAS_DYNAMICALLY_LOADED_SFCNS=0
    -DCLASSIC_INTERFACE=0
    -DALLOCATIONFCN=0
    -DTID01EQ=1
    -DTERMFCN=1
    -DONESTEPFCN=1
    -DMAT_FILE=0
    -DMULTI_INSTANCE_CODE=0
    -DINTEGER_CODE=0
    -DMT=0)
target_compile_options(Do_Model0 PRIVATE -fwrapv)
target_include_directories(Do_Model0 PUBLIC $<BUILD_INTERFACE:${START_DIR}>
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>>
    $<BUILD_INTERFACE:${START_DIR}/Do_Model0_ert_shrlib_rtw>
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/Do_Model0_ert_shrlib_rtw>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/extern/include>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/simulink/include>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/rtw/c/src>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/rtw/c/src/ext_mode/common>
    $<BUILD_INTERFACE:${MATLAB_ROOT}/rtw/c/ert>)
find_library(FOUND_LIBM m)
if(FOUND_LIBM)
   target_link_libraries(Do_Model0 PUBLIC m)
endif()


######################################################################
## Target install and export rules
######################################################################
install(TARGETS Do_Model0 EXPORT Do_Model0Targets RUNTIME DESTINATION .
    LIBRARY DESTINATION .
    ARCHIVE DESTINATION .)
install(EXPORT Do_Model0Targets NAMESPACE Do_Model0:: DESTINATION Do_Model0_ert_shrlib_rtw/export)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/Do_Model0.cmake 
 "include(\"\${CMAKE_CURRENT_LIST_DIR}/Do_Model0Targets.cmake\")" \n
    "target_include_directories(Do_Model0::Do_Model0 INTERFACE \n"
    "    \${MATLAB_ROOT}/extern/include" \n 
    "    \${MATLAB_ROOT}/simulink/include" \n 
    "    \${MATLAB_ROOT}/rtw/c/src" \n 
    "    \${MATLAB_ROOT}/rtw/c/src/ext_mode/common" \n 
    "    \${MATLAB_ROOT}/rtw/c/ert)" \n )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Do_Model0.cmake DESTINATION Do_Model0_ert_shrlib_rtw/export)

######################################################################
## Build success message
######################################################################
add_custom_command(TARGET Do_Model0 POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "\\#\\#\\# Created library: $<TARGET_FILE:Do_Model0>")

######################################################################
## Call toolchain hook function if defined
######################################################################
if(COMMAND toolchain_target_hook)
    toolchain_target_hook(Do_Model0)
endif()
