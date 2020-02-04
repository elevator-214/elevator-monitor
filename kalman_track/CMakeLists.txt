add_library(kalman_track STATIC
                        ${CMAKE_CURRENT_SOURCE_DIR}/KalmanTracker.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/KalmanTracker.h
                        ${CMAKE_CURRENT_SOURCE_DIR}/myFunction.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/myFunction.h
			
			)
#message(STATUS "OpenCV_DIR:${munkres}")
target_link_libraries(kalman_track PUBLIC munkres)
target_include_directories(kalman_track PRIVATE "${OpenCV_INCLUDE_DIRS}")
target_include_directories(kalman_track PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
