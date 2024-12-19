cmake_minimum_required(VERSION 3.10)

project(CamMQClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSATLL_RPATH_USE_LINK_PATH TRUE)

set(ZMQ_INCLUDE_DIR "external/zeroMQ/include")
set(ZMQ_LIB_DIR "external/zeroMQ/lib")

find_package(OpenCV REQUIRED)

find_library(ZMQ_LIB NAMES zmq PATHS ${ZMQ_LIB_DIR})

include_directories(
    ${ZMQ_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_options(${PROJECT_NAME} PRIVATE -Wl,--disable-new-dtags)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${ZMQ_LIB}
    ${OpenCV_LIBS}
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${ZMQ_LIB_DIR}/ DESTINATION lib)