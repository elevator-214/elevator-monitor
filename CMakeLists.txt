project(elevator_monitor)
cmake_minimum_required( VERSION 3.1 )
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_BUILD_TYPE Release )
#set( CMAKE_BUILD_TYPE Debug )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build/lib)#static
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib)#shared
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)#exeutable


find_package( OpenCV  REQUIRED)
find_package(Boost REQUIRED)   #要使用的boost库
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})

#include_directories(tiny-tensorrt)
#link_directories(/usr/lib/aarch64-linux-gnu)
#link_directories(lib/)
#find_library(CUDART cudart HINTS /usr/local/cuda/targets/aarch64-linux/lib/)




#QT5_WRAP_CPP(MOCS ${test2_MOCS})
add_subdirectory(read_parameter)
add_subdirectory(munkres)
add_subdirectory(kalman_track)
add_subdirectory(thing_retention)
add_subdirectory(tiny-tensorrt)
add_subdirectory(openpose)
add_subdirectory(main_src)

#message(STATUS "thing_retention_src:${thing_retention_src}+++")
#message(STATUS "TRS:${TRS}+++")
#message(STATUS "MOCS:${MOCS}+++")
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'+++")
#endforeach()


