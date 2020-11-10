cmake_minimum_required(VERSION 3.3)
project(trifinger_object_tracking)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# stop build on first error
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wfatal-errors")

# always enable optimization, otherwise object tracking is too slow
string(APPEND CMAKE_CXX_FLAGS " -O3")

# Specify for which cube version to build the object tracker
#
# Currently supported object versions:
#  - 1: CUBE_V1
#  - 2: CUBE_V2
#  - 3: CUBE_V3
#  - 4: CUBOID_2x2x8

set(OBJECT_VERSION 4)

find_package(catkin REQUIRED COMPONENTS
    mpi_cmake_modules
    pybind11_catkin
    serialization_utils
    time_series
    roslib
    trifinger_cameras
    robot_interfaces
)

search_for_eigen()

find_package(LAPACK)
find_package(BLAS REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Armadillo REQUIRED)


if (${HAS_PYLON_DRIVERS})
    add_definitions(-DPylon_FOUND)

    # Set library names to variables, so we can use the variable instead of the
    # direct name below.  In case, Pylon drivers are not built, the variables
    # will be empty and thus not cause trouble when used for linking etc.
    set(tricamera_object_tracking_driver tricamera_object_tracking_driver)
endif()


catkin_python_setup()

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES
        cv_sub_images
        cube_detector
        simulation_object_tracker
        fake_object_tracker
        ${tricamera_object_tracking_driver}
        pybullet_tricamera_object_tracker_driver
)


include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${OPENMP_INCLUDES}
    ${Boost_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${ARMADILLO_INCLUDE_DIRS}
    ${BLAS_INCLUDE_DIRS}
    ${XGBOOST_INCLUDE_DIRS}
)


add_definitions(-DOBJECT_VERSION=${OBJECT_VERSION})
if (${OBJECT_VERSION} EQUAL 1)
    set(cube_model_dir src/cube_v1/)
elseif(${OBJECT_VERSION} EQUAL 2)
    set(cube_model_dir src/cube_v2/)
elseif(${OBJECT_VERSION} EQUAL 4)
    set(cube_model_dir src/cuboid_2x2x8/)
else()
    message(FATAL_ERROR "Invalid OBJECT_VERSION '${OBJECT_VERSION}'")
endif()


add_library(cv_sub_images src/cv_sub_images.cpp)
target_link_libraries(cv_sub_images ${catkin_LIBRARIES} ${OpenCV_LIBS})


add_library(cube_detector
    src/cube_detector.cpp
    src/color_segmenter.cpp
    src/pose_detector.cpp
    src/utils.cpp
    src/cube_model.cpp
    ${cube_model_dir}/xgboost_classifier.cpp
)
target_link_libraries(cube_detector
    ${catkin_LIBRARIES}
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    ${ARMADILLO_LIBRARIES}
    ${BLAS_LIBRARIES}
    ${LAPACK_LIBRARIES}
    cv_sub_images
)

add_library(cube_visualizer
    src/cube_visualizer.cpp
)
target_link_libraries(cube_visualizer
    ${catkin_LIBRARIES}
    cube_detector
)

add_executable(single_observation src/single_observation.cpp)
target_link_libraries(single_observation
    cube_detector
    cv_sub_images
    pthread
)

add_executable(run_on_logfile src/run_on_logfile.cpp)
target_link_libraries(run_on_logfile
    ${catkin_LIBRARIES}
    cube_detector
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

if (${HAS_PYLON_DRIVERS})
    add_library(${tricamera_object_tracking_driver}
        src/tricamera_object_tracking_driver.cpp)
    target_link_libraries(${tricamera_object_tracking_driver}
        ${catkin_LIBRARIES} cube_detector)
endif()

add_library(pybullet_tricamera_object_tracker_driver
    src/pybullet_tricamera_object_tracker_driver.cpp)
target_link_libraries(pybullet_tricamera_object_tracker_driver
    ${catkin_LIBRARIES} cube_detector)


catkin_add_pybind11_module(py_object_tracker
    simulation_object_tracker
    fake_object_tracker
)
catkin_add_pybind11_module(py_tricamera_types
    ${tricamera_object_tracking_driver}
    pybullet_tricamera_object_tracker_driver
    cube_visualizer
)


if (CATKIN_ENABLE_TESTING)
    find_package(roslib REQUIRED)
    include_directories(${roslib_INCLUDE_DIRS})

    #catkin_add_gtest(test_image test/test_image.cpp)
    #target_link_libraries(test_image cube_detector ${roslib_LIBRARIES})
endif()
