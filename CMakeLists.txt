cmake_minimum_required(VERSION 3.5)
project(trifinger_object_tracking)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# libraries need to be position independent otherwise building Python modules
# fails
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
set(OBJECT_VERSION 2)

find_package(ament_cmake REQUIRED)
find_package(mpi_cmake_modules REQUIRED)
find_package(pybind11 REQUIRED)
find_package(pybind11_opencv REQUIRED)
find_package(serialization_utils REQUIRED)
find_package(time_series REQUIRED)
find_package(trifinger_cameras REQUIRED)
find_package(robot_interfaces REQUIRED)

find_package(Eigen3 REQUIRED)
find_package(LAPACK REQUIRED)
find_package(BLAS REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Armadillo REQUIRED)


if (${HAS_PYLON_DRIVERS})
    add_definitions(-DPylon_FOUND)
endif()


ament_python_install_package(${PROJECT_NAME} PACKAGE_DIR python/${PROJECT_NAME})


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
target_include_directories(cv_sub_images PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(cv_sub_images ${OpenCV_LIBS})


add_library(cube_detector
    src/cube_detector.cpp
    src/color_segmenter.cpp
    src/pose_detector.cpp
    src/utils.cpp
    src/cube_model.cpp
    ${cube_model_dir}/xgboost_classifier.cpp
)
target_include_directories(cube_detector PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${ARMADILLO_INCLUDE_DIRS}
    ${BLAS_INCLUDE_DIRS}
)
target_link_libraries(cube_detector
    ${OpenCV_LIBS}
    ${ARMADILLO_LIBRARIES}
    ${BLAS_LIBRARIES}
    ${LAPACK_LIBRARIES}
    pthread
    Eigen3::Eigen
    serialization_utils::serialization_utils
    cv_sub_images
    trifinger_cameras::camera_calibration_parser
)


add_library(cube_visualizer
    src/cube_visualizer.cpp
)
target_include_directories(cube_visualizer PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(cube_visualizer
    cube_detector
)


add_executable(single_observation src/single_observation.cpp)
target_include_directories(single_observation PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(single_observation
    cube_detector
)


add_executable(run_on_logfile src/run_on_logfile.cpp)
target_include_directories(run_on_logfile PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(run_on_logfile
    robot_interfaces::robot_interfaces
    cube_detector
)


# TODO frontend should be in separate library

add_library(simulation_object_tracker
    src/object_tracker_data.cpp
    src/base_object_tracker_backend.cpp
    src/simulation_object_tracker_backend.cpp
    src/object_tracker_frontend.cpp
)
target_include_directories(simulation_object_tracker PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(simulation_object_tracker
    robot_interfaces::robot_interfaces
)
# using pybind11 types, therefore visibility needs to be hidden
set_target_properties(simulation_object_tracker
    PROPERTIES CXX_VISIBILITY_PRESET hidden)


add_library(fake_object_tracker
    src/object_tracker_data.cpp
    src/base_object_tracker_backend.cpp
    src/fake_object_tracker_backend.cpp
)
target_include_directories(fake_object_tracker PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(fake_object_tracker
    robot_interfaces::robot_interfaces
)


if (${HAS_PYLON_DRIVERS})
    add_library(tricamera_object_tracking_driver
        src/tricamera_object_tracking_driver.cpp)
    target_include_directories(tricamera_object_tracking_driver PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(tricamera_object_tracking_driver
        robot_interfaces::robot_interfaces
        trifinger_cameras::pylon_driver
        cube_detector
    )

    # Set library names to variables, so we can use the variable instead of the
    # direct name below.  In case, Pylon drivers are not built, the variables
    # will be empty and thus not cause trouble when used for linking etc.
    set(tricamera_object_tracking_driver tricamera_object_tracking_driver)
endif()


add_library(pybullet_tricamera_object_tracker_driver
    src/pybullet_tricamera_object_tracker_driver.cpp)
target_include_directories(pybullet_tricamera_object_tracker_driver PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(pybullet_tricamera_object_tracker_driver
    pybind11::pybind11
    pybind11_opencv::pybind11_opencv
    robot_interfaces::robot_interfaces
    serialization_utils::serialization_utils
    cube_detector
)
# using pybind11 types, therefore visibility needs to be hidden
set_target_properties(pybullet_tricamera_object_tracker_driver
    PROPERTIES CXX_VISIBILITY_PRESET hidden)


# Python Bindings
add_pybind11_module(py_object_tracker srcpy/py_object_tracker.cpp
    LINK_LIBRARIES
        Eigen3::Eigen
        pybind11_opencv::pybind11_opencv
        simulation_object_tracker
        fake_object_tracker
        cube_detector
)
add_pybind11_module(py_tricamera_types srcpy/py_tricamera_types.cpp
    LINK_LIBRARIES
        pybind11_opencv::pybind11_opencv
        ${tricamera_object_tracking_driver}
        pybullet_tricamera_object_tracker_driver
        cube_visualizer
)


# Installation

ament_export_interfaces(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

# install headers
install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION include/${PROJECT_NAME})

install(
    TARGETS
        cv_sub_images
        cube_detector
        cube_visualizer
        simulation_object_tracker
        fake_object_tracker
        ${tricamera_object_tracking_driver}
        pybullet_tricamera_object_tracker_driver
        single_observation
        run_on_logfile

    EXPORT export_${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install_scripts(
    scripts/analyze_durations.py
    scripts/create_background_data.py
    scripts/debayering_tests.py
    scripts/follow_cube.py
    scripts/get_xgb_dump.py
    scripts/plot_log_timestamps.py
    scripts/train_xgb_tree.py
    scripts/tricamera_log_analyzer.py
    scripts/tricamera_log_converter.py
    scripts/tricamera_log_extract.py
    scripts/tricamera_log_viewer.py
    scripts/view_masks.py
    scripts/visualise_cube_target_pose.py

    DESTINATION lib/${PROJECT_NAME}
)

install(
    PROGRAMS
        demos/demo_cameras.py
        demos/run_on_logfile.py

    DESTINATION lib/${PROJECT_NAME}
)


if (BUILD_TESTING)
    find_package(ament_cmake_gtest REQUIRED)
    find_package(ament_index_cpp REQUIRED)

    # install test images
    install(
        DIRECTORY test/images
        DESTINATION share/${PROJECT_NAME}/test
    )

    ament_add_gtest(test_color_segmenter test/test_color_segmenter.cpp)
    target_link_libraries(test_color_segmenter cube_detector)
    ament_target_dependencies(test_color_segmenter
        ament_index_cpp
    )
    install(TARGETS test_color_segmenter DESTINATION lib/${PROJECT_NAME})

    ament_add_gtest(test_pose_detector test/test_pose_detector.cpp)
    target_link_libraries(test_pose_detector cube_detector)
    ament_target_dependencies(test_pose_detector
        ament_index_cpp
    )
    install(TARGETS test_pose_detector DESTINATION lib/${PROJECT_NAME})

endif()


ament_export_include_directories(include)
ament_export_libraries(
    cv_sub_images
    cube_detector
    simulation_object_tracker
    fake_object_tracker
    ${tricamera_object_tracking_driver}
    pybullet_tricamera_object_tracker_driver
)
ament_package()
