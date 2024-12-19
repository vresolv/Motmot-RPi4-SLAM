cmake_minimum_required(VERSION 3.10)

project(CamMQServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSATLL_RPATH_USE_LINK_PATH TRUE)

set(ZMQ_INCLUDE_DIR "external/zeroMQ/include")
set(ZMQ_LIB_DIR "external/zeroMQ/lib")
set(LCCV_LIB_PATH "external/LCCV/lib")
set(LCCV_INLCUDE_PATH "external/LCCV/include")
set(LIBCAMERA_LIB_PATH "external/LIBCAMERA/lib")
set(LIBCAMERA_INLCUDE_PATH "external/LIBCAMERA/include")

find_package(OpenCV REQUIRED)

find_library(ZMQ_LIB NAMES zmq PATHS ${ZMQ_LIB_DIR})
find_library(LCCV_LIBRARY NAMES lccv PATHS ${LCCV_LIB_PATH})
find_library(LIBCAM_LIBRARY NAMES camera PATHS ${LIBCAMERA_LIB_PATH})
find_library(LIBCAMBASE_LIBRARY NAMES camera-base PATHS ${LIBCAMERA_LIB_PATH})

include_directories(
    ${ZMQ_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${LCCV_INLCUDE_PATH}
    ${LIBCAMERA_INLCUDE_PATH}
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_options(${PROJECT_NAME} PRIVATE -Wl,--disable-new-dtags)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${ZMQ_LIB}
    ${OpenCV_LIBS}
    ${LCCV_LIBRARY}
    ${LIBCAM_LIBRARY}
    ${LIBCAMBASE_LIBRARY}
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES ${LCCV_LIBRARY} DESTINATION lib)
install(DIRECTORY ${ZMQ_LIB_DIR}/ DESTINATION lib)