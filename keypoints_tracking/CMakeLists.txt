set(keypoints_tracking_src	${CMAKE_CURRENT_SOURCE_DIR}/keypoints_tracking.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/keypoints_tracking.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/my_tracking_function.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/my_tracking_function.h
			#PARENT_SCOPE
)
add_library(keypoints_tracking SHARED ${keypoints_tracking_src})
target_link_libraries(keypoints_tracking PUBLIC munkres ${OpenCV_LIBS})
target_include_directories(keypoints_tracking PRIVATE "${OpenCV_INCLUDE_DIRS}")
target_include_directories(keypoints_tracking PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")






