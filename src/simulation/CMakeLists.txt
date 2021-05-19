cmake_minimum_required(VERSION 3.2)
project(simulation C CXX)

set(SOURCE_FILES
    simulation.h
    simulation.cpp
)

add_library(simulation SHARED STATIC ${SOURCE_FILES})

install(TARGETS simulation DESTINATION ${SIMULATOR_INSTALL_LIB_DIR})
install(FILES simulation.h DESTINATION ${SIMULATOR_INSTALL_INCLUDE_DIR})