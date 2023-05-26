cmake_minimum_required(VERSION 3.8)
project(riptide_simulink_ros)
include(ExternalProject)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(riptide_msgs2 REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(yaml_cpp_vendor REQUIRED)

set(SIMULINK_SO_NAME Do_Model0.so)
set(SIMULINK_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Do_Model0_ert_shrlib_rtw)

ExternalProject_Add(generated_simulink_library
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/simulink_tmp
  SOURCE_DIR ${SIMULINK_SRC}
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/lib
)

add_library(simulink_generated_library SHARED IMPORTED)

set(SIMULINK_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/${SIMULINK_SO_NAME})

set(SIMULINK_INCLUDE ${SIMULINK_INSTALL_DIR}/Do_Model0_ert_shrlib_rtw)

set_target_properties(simulink_generated_library PROPERTIES
  IMPORTED_LOCATION "${SIMULINK_LIB}"
)

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(sim_ros_wrapper ${SRCS})

set(MATLAB_ROOT /usr/local/MATLAB/R2022b CACHE PATH "")

target_include_directories(sim_ros_wrapper PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Do_Model0_ert_shrlib_rtw>
  $<BUILD_INTERFACE:${SIMULINK_INCLUDE}>
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${MATLAB_ROOT}/extern/include>
  $<BUILD_INTERFACE:${MATLAB_ROOT}/simulink/include>
  $<BUILD_INTERFACE:${MATLAB_ROOT}/rtw/c/src>
  $<BUILD_INTERFACE:${MATLAB_ROOT}/rtw/c/src/ext_mode/common>
  $<BUILD_INTERFACE:${MATLAB_ROOT}/rtw/c/ert>)

target_compile_features(sim_ros_wrapper PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

target_link_libraries(sim_ros_wrapper simulink_generated_library)

ament_target_dependencies(sim_ros_wrapper
  rclcpp
  rclcpp_action
  tf2_geometry_msgs
  riptide_msgs2
  nav_msgs
  yaml_cpp_vendor
)

install(TARGETS sim_ros_wrapper
  DESTINATION lib/${PROJECT_NAME})

install(FILES ${SIMULINK_LIB} 
  DESTINATION lib)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
