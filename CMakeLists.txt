cmake_minimum_required(VERSION 3.16)
project(riptide_simulink)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(std_srvs REQUIRED)
find_package(yaml_cpp_vendor REQUIRED)
find_package(yaml-cpp REQUIRED)

# header-only model wrapper, consumed as riptide_simulink::model
add_library(model INTERFACE)
add_library(riptide_simulink::model ALIAS model)
target_compile_features(model INTERFACE cxx_std_17)
target_include_directories(model INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_link_libraries(model INTERFACE rclcpp::rclcpp yaml-cpp ${rcl_interfaces_TARGETS} ${std_srvs_TARGETS})
install(TARGETS model EXPORT model_export)
install(DIRECTORY include/ DESTINATION include)
ament_export_targets(model_export)
ament_export_dependencies(rclcpp rcl_interfaces std_srvs yaml_cpp_vendor yaml-cpp)

# python manager, MATLAB entry points, examples and docs
ament_python_install_package(${PROJECT_NAME})
install(PROGRAMS riptide_simulink/model_manager.py DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY matlab examples DESTINATION share/${PROJECT_NAME})
install(FILES README.md DESTINATION share/${PROJECT_NAME})
install(DIRECTORY docs DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_cmake_pytest REQUIRED)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_pytest_test(manager test/test_manager.py)
  ament_add_pytest_test(publishing test/test_publishing.py)
  ament_add_pytest_test(interactive test/test_interactive.py)
  ament_add_pytest_test(resources test/test_resources.py)
  ament_add_gtest(model_test test/test_model.cpp TIMEOUT 90)
  target_link_libraries(model_test model)
endif()

ament_package()
