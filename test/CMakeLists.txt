cmake_minimum_required(VERSION 3.2)
#Treat each subdirectory as a new project
project(event-simulator-tests)

#Add subdirectories to the build. These must contain a CMakeLists.txt file.
add_subdirectory(lib/googletest)

#Tell the compiler to search for include files here
include_directories(${SIMULATOR_HEADERS_DIR})
include_directories(lib/googletest/googletest/include)

set(SOURCE_FILES main.cpp src/simulator_tests.cpp)

#The add_executable command tells CMake to create a binary
add_executable(simulator_tests ${SOURCE_FILES})

#target_link_libraries specifies libraries or flags to use 
#when linking a given target. The named target (first 
#argument) must have been created in the current directory
#with add_executable() or add_library()
target_link_libraries(simulator_tests simulation gtest)
install(TARGETS simulator_tests DESTINATION bin)