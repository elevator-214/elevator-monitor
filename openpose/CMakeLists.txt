file(GLOB openpose_source_file
     Array.cpp
     Point.cpp
     *.cu
     )




cuda_add_library(openpose SHARED ${openpose_source_file})

target_compile_options(openpose PUBLIC -std=c++11 -Wall -Wfloat-conversion)
target_link_libraries(openpose tinytrt)
target_link_libraries(openpose nvinfer)
target_link_libraries(openpose nvinfer_plugin)
target_link_libraries(openpose nvparsers)
target_link_libraries(openpose nvonnxparser)
target_link_libraries(openpose nvcaffe_parser)
target_link_libraries(openpose ${CUDART})
target_link_libraries(openpose ${OpenCV_LIBRARIES})


target_include_directories(openpose PRIVATE "${OpenCV_INCLUDE_DIRS}")
target_include_directories(openpose PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")










