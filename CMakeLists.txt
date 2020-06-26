project(elevator_monitor)
cmake_minimum_required( VERSION 3.1 )
set( CMAKE_CXX_STANDARD 11 )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )

set( CMAKE_BUILD_TYPE Debug )
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#set( CMAKE_BUILD_TYPE Release )
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build/lib)#static
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib)#shared
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)#exeutable

set(OpenCV_DIR /usr/local/opencv4.2/lib/cmake/opencv4)#qt

find_package ( Qt5Widgets REQUIRED )
find_package ( Qt5Core REQUIRED )
find_package ( Qt5Gui REQUIRED )
find_package( OpenCV  REQUIRED)
find_package(Boost REQUIRED)   #要使用的boost库
find_package(CUDA REQUIRED)

include_directories(${CUDA_INCLUDE_DIRS})


add_subdirectory(read_parameter)
add_subdirectory(munkres)
add_subdirectory(kalman_track)
add_subdirectory(thing_retention)
add_subdirectory(tiny-tensorrt)
add_subdirectory(gcn)
add_subdirectory(openpose)

add_subdirectory(keypoints_tracking)
add_subdirectory(main_src)


#

