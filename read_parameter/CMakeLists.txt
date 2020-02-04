add_library(read_parameter STATIC
                        ${CMAKE_CURRENT_SOURCE_DIR}/read_para.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/read_para.h			
			)
add_executable (test_read_parameter 
                        ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/read_para.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/read_para.h	)
#target_link_libraries(kalman_track PUBLIC munkres)
target_include_directories(read_parameter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
