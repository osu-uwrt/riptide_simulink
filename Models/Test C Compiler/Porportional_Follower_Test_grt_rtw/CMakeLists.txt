######################################################################
# CMakeLists.txt generated for component Porportional_Follower_Test
# Product type: executable
######################################################################
cmake_minimum_required(VERSION 3.12)
project(Porportional_Follower_Test)
if(DEFINED ENV{CMAKE_EXPORT_COMPILE_COMMANDS} AND NOT DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS $ENV{CMAKE_EXPORT_COMPILE_COMMANDS})
endif()


######################################################################
## Path variables
######################################################################
get_filename_component(START_DIR .. ABSOLUTE)
set(MATLAB_ROOT /usr/local/MATLAB/R2022b CACHE PATH "")
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
add_executable(Porportional_Follower_Test  ${MATLAB_ROOT}/rtw/c/src/rt_logging.c
    ${START_DIR}/Porportional_Follower_Test_grt_rtw/Porportional_Follower_Test.cpp
    ${START_DIR}/Porportional_Follower_Test_grt_rtw/rtGetInf.cpp
    ${START_DIR}/Porportional_Follower_Test_grt_rtw/rtGetNaN.cpp
    ${START_DIR}/Porportional_Follower_Test_grt_rtw/rt_nonfinite.cpp
    ${MATLAB_ROOT}/rtw/c/src/common/rt_cppclass_main.cpp)
set_target_properties(Porportional_Follower_Test PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON
    RUNTIME_OUTPUT_DIRECTORY "${BINARY_START_DIR}/./$<0:>"
    LIBRARY_OUTPUT_DIRECTORY "${BINARY_START_DIR}/./$<0:>"
    ARCHIVE_OUTPUT_DIRECTORY "${BINARY_START_DIR}/./$<0:>")
if(CMAKE_CXX_COMPILER)
    set_target_properties(Porportional_Follower_Test PROPERTIES LINKER_LANGUAGE CXX)
endif()
target_compile_features(Porportional_Follower_Test PUBLIC cxx_std_11)
target_compile_definitions(Porportional_Follower_Test PRIVATE -DMODEL=Porportional_Follower_Test
    -DNUMST=2
    -DNCSTATES=0
    -DHAVESTDIO
    -DRT
    -DUSE_RTMODEL
    -DCLASSIC_INTERFACE=0
    -DALLOCATIONFCN=0
    -DTID01EQ=1
    -DMAT_FILE=1
    -DONESTEPFCN=1
    -DTERMFCN=1
    -DMULTI_INSTANCE_CODE=1
    -DINTEGER_CODE=0
    -DMT=0)
target_include_directories(Porportional_Follower_Test PRIVATE ${START_DIR}
    ${START_DIR}/Porportional_Follower_Test_grt_rtw
    ${MATLAB_ROOT}/extern/include
    ${MATLAB_ROOT}/simulink/include
    ${MATLAB_ROOT}/rtw/c/src
    ${MATLAB_ROOT}/rtw/c/src/ext_mode/common)
find_library(FOUND_LIBM m)
if(FOUND_LIBM)
   target_link_libraries(Porportional_Follower_Test PRIVATE m)
endif()
if(WIN32)
   target_link_libraries(Porportional_Follower_Test PRIVATE kernel32
       ws2_32
       mswsock
       advapi32)
endif()

######################################################################
## Extract debug symbols
######################################################################
if(APPLE AND (CMAKE_C_COMPILER_ID STREQUAL "AppleClang"))
    string(APPEND _dsymutilcmd "$<IF:$<CONFIG:DEBUG,RELWITHDEBINFO>,"
                "xcrun;dsymutil;$<TARGET_FILE:Porportional_Follower_Test>;--flat,"
                "true"
        >)
    add_custom_command(TARGET Porportional_Follower_Test
            POST_BUILD
            COMMAND "${_dsymutilcmd}"
            COMMAND_EXPAND_LISTS
            BYPRODUCTS Porportional_Follower_Test.dwarf)
endif()


######################################################################
## Build success message
######################################################################
add_custom_command(TARGET Porportional_Follower_Test POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "\\#\\#\\# Created executable: $<TARGET_FILE:Porportional_Follower_Test>")

######################################################################
## Call toolchain hook function if defined
######################################################################
if(COMMAND toolchain_target_hook)
    toolchain_target_hook(Porportional_Follower_Test)
endif()
