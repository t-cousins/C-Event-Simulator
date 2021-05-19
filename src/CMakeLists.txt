cmake_minimum_required(VERSION 3.2)
project(simulator)

#Add subdirectories to the build. These must contain a CMakeLists.txt file.
add_subdirectory(simulation)

set(SOURCE_FILES main.cpp)
#The add_executable command tells CMake to create a binary
add_executable(simulator ${SOURCE_FILES})

target_link_libraries(simulator simulation)
install(TARGETS simulator DESTINATION ${SIMULATOR_INSTALL_BIN_DIR})