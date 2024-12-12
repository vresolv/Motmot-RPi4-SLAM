cmake_minimum_required(VERSION 3.1)

project(MotMot)

find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core SerialPort)
find_package(Eigen3 REQUIRED)
find_package(Pangolin REQUIRED)

set(ORB_LIBRARY_PATH "ORBSLAM3/lib")
set(ORB_INCLUDE_PATH "ORBSLAM3/include")
set(ORB_CAM_MODELS_INCLUDE_PATH "ORBSLAM3/include/CameraModels")
set(ORB_THIRDPARTY_INCLUDE "ORBSLAM3")
set(ORB_THIRDPARTY_SOPHUS_INCLUDE "ORBSLAM3/Thirdparty/Sophus")
set(LIBCAMERA_LIB_PATH "LIBCAMERA/lib")
set(LIBCAMERA_INLCUDE_PATH "LIBCAMERA/include")
set(LCCV_LIB_PATH "LCCV/lib")
set(LCCV_INLCUDE_PATH "LCCV/include")

include_directories(
${ORB_INCLUDE_PATH}
${ORB_CAM_MODELS_INCLUDE_PATH}
${ORB_THIRDPARTY_INCLUDE}
${ORB_THIRDPARTY_SOPHUS_INCLUDE}
${LIBCAMERA_INLCUDE_PATH}
${LCCV_INLCUDE_PATH}
${OpenCV_INCLUDE_DIRS}
${Pangolin_INCLUDE_DIRS}
include
)

set(HEADERS
    include/cameraManager.hpp
    include/imuManager.hpp
    include/orbSlamManager.hpp
    include/motmotCore.hpp
)

set(CMAKE_AUTOMOC ON)

set(SOURCES
    src/cameraManager.cpp
    src/imuManager.cpp
    src/orbSlamManager.cpp
    src/motmotCore.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} 
    ${SOURCES} ${HEADERS})

find_library(ORB_LIBRARY NAMES ORB_SLAM3 PATHS ${ORB_LIBRARY_PATH})
find_library(LIBCAM_LIBRARY NAMES camera PATHS ${LIBCAMERA_LIB_PATH})
find_library(LIBCAMBASE_LIBRARY NAMES camera-base PATHS ${LIBCAMERA_LIB_PATH})
find_library(LCCV_LIBRARY NAMES lccv PATHS ${LCCV_LIB_PATH})

target_link_libraries(${PROJECT_NAME} 
    ${ORB_LIBRARY}
    ${LIBCAM_LIBRARY}
    ${LIBCAMBASE_LIBRARY}
    ${LCCV_LIBRARY} 
    ${OpenCV_LIBS}
    Qt5::Core
    Qt5::SerialPort
    Eigen3::Eigen
    ${Pangolin_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/install/bin)