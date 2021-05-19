#Set minimum CMake version required to run this file
cmake_minimum_required(VERSION 3.2)
   
#Set name of project 
project("event-simulator")
   
#Set compiler flags:
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

#Define some paths:
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(SIMULATOR_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SIMULATOR_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(SIMULATOR_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(SIMULATOR_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/simulation)

#Tell the compiler to search for include files here
include_directories(${SIMULATOR_INSTALL_INCLUDE_DIR})
include_directories(${SIMULATOR_HEADERS_DIR})

#Add subdirectories to the build. These must contain a CMakeLists.txt file.
add_subdirectory(src)
add_subdirectory(test)