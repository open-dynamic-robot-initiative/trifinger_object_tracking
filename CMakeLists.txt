cmake_minimum_required(VERSION 3.1)
project(trifinger_object_tracking)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# stop build on first error
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wfatal-errors")

string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O3")

find_package(catkin REQUIRED COMPONENTS
    mpi_cmake_modules
    pybind11_catkin
    serialization_utils
    time_series
    roslib
)

search_for_eigen()

find_package(LAPACK)
find_package(BLAS REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Armadillo REQUIRED)

# TODO can this be removed?
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


catkin_python_setup()

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES simulation_object_tracker fake_object_tracker
)


include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${OPENMP_INCLUDES}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${ARMADILLO_INCLUDE_DIRS}
    ${BLAS_INCLUDE_DIRS}
)

add_library(lib_object_pose
    src/line_detector.cpp
    src/pose.cpp
    src/utils.cpp
    src/cube_model.cpp
)
target_link_libraries(lib_object_pose
    ${catkin_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES} ${ARMADILLO_LIBRARIES} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})

add_library(cv_sub_images src/cv_sub_images.cpp)
target_link_libraries(cv_sub_images ${catkin_LIBRARIES} ${OpenCV_LIBS})


add_executable(object_pose src/main.cpp)
target_link_libraries(object_pose
    lib_object_pose
    cv_sub_images
    pthread
    ${OpenMP_CXX_LIBRARIES}
)

add_executable(single_observation src/single_observation.cpp)
target_link_libraries(single_observation
    lib_object_pose
    cv_sub_images
    pthread
)


# TODO frontend should be in separate library

add_library(simulation_object_tracker
    src/object_tracker_data.cpp
    src/base_object_tracker_backend.cpp
    src/simulation_object_tracker_backend.cpp
    src/object_tracker_frontend.cpp
)
target_link_libraries(simulation_object_tracker ${catkin_LIBRARIES})


add_library(fake_object_tracker
    src/object_tracker_data.cpp
    src/base_object_tracker_backend.cpp
    src/fake_object_tracker_backend.cpp
)
target_link_libraries(fake_object_tracker ${catkin_LIBRARIES})


catkin_add_pybind11_module(py_object_tracker simulation_object_tracker fake_object_tracker)

if (CATKIN_ENABLE_TESTING)
    find_package(roslib REQUIRED)
    include_directories(${roslib_INCLUDE_DIRS})

    #catkin_add_gtest(test_image test/test_image.cpp)
    #target_link_libraries(test_image lib_object_pose ${roslib_LIBRARIES})
endif()
