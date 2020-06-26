set(GCN_source ${CMAKE_CURRENT_SOURCE_DIR}/test_gcn.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/my_gcn.cu
				${CMAKE_CURRENT_SOURCE_DIR}/my_gcn.hpp
	)
# cuda_add_executable(gcn ${GCN_source})
cuda_add_executable(test_gcn ${GCN_source})
target_link_libraries(test_gcn tinytrt)
target_link_libraries(test_gcn nvinfer)
target_link_libraries(test_gcn nvinfer_plugin)
target_link_libraries(test_gcn nvparsers)
target_link_libraries(test_gcn nvonnxparser)
target_link_libraries(test_gcn nvcaffe_parser)
target_link_libraries(test_gcn ${CUDART})
target_link_libraries(test_gcn ${OpenCV_LIBRARIES})
# message(STATUS "Install configuration:!!!!!!!")
