cmake_minimum_required(VERSION 3.1)
project(trifinger_object_tracking)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# stop build on first error
string(APPEND CMAKE_CXX_FLAGS " -Wfatal-errors")


find_package(catkin REQUIRED COMPONENTS
    mpi_cmake_modules
    serialization_utils
    time_series
)

search_for_eigen()

catkin_python_setup()

catkin_package(
    LIBRARIES simulation_object_tracker
)


include_directories(
    include
    ${catkin_INCLUDE_DIRS}
)

add_library(simulation_object_tracker
    src/object_tracker_data.cpp
    src/base_object_tracker_backend.cpp
    src/simulation_object_tracker_backend.cpp
)
target_link_libraries(simulation_object_tracker ${catkin_LIBRARIES})
