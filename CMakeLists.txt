cmake_minimum_required(VERSION 3.1)
project(trifinger_object_tracking)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# stop build on first error
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wfatal-errors")

string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O3")

# Specify for which cube version to build the object tracker
set(CUBE_VERSION 1)

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


add_definitions(-DCUBE_VERSION=${CUBE_VERSION})
if (${CUBE_VERSION} EQUAL 1)
    set(cube_model_dir src/cube_v1/)
elseif(${CUBE_VERSION} EQUAL 2)
    set(cube_model_dir src/cube_v2/)
else()
    message(FATAL_ERROR "Invalid CUBE_VERSION '${CUBE_VERSION}'")
endif()


add_library(cv_sub_images src/cv_sub_images.cpp)
target_link_libraries(cv_sub_images ${catkin_LIBRARIES} ${OpenCV_LIBS})


add_library(cube_detector
    src/cube_detector.cpp
    src/line_detector.cpp
    src/pose_detector.cpp
    src/utils.cpp
    src/cube_model.cpp
    src/gmm_params.cpp
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


add_executable(object_pose src/main.cpp)
target_link_libraries(object_pose
    cube_detector
    cv_sub_images
    pthread
)

add_executable(single_observation src/single_observation.cpp)
target_link_libraries(single_observation
    cube_detector
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
)


if (CATKIN_ENABLE_TESTING)
    find_package(roslib REQUIRED)
    include_directories(${roslib_INCLUDE_DIRS})

    #catkin_add_gtest(test_image test/test_image.cpp)
    #target_link_libraries(test_image cube_detector ${roslib_LIBRARIES})
endif()
