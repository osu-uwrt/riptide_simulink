# Add this pattern to an existing specialized overseer package.
find_package(riptide_simulink REQUIRED)
add_executable(example_overseer overseer.cpp)
target_link_libraries(example_overseer riptide_simulink::model)
install(TARGETS example_overseer DESTINATION lib/${PROJECT_NAME})
