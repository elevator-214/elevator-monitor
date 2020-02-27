set(thing_retention_src	${CMAKE_CURRENT_SOURCE_DIR}/Fore_extraction.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/Fore_extraction.h
			${CMAKE_CURRENT_SOURCE_DIR}/myFunction.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/myFunction.h
                        ${CMAKE_CURRENT_SOURCE_DIR}/ThingDetect.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/ThingDetect.h     
                        ${CMAKE_CURRENT_SOURCE_DIR}/ThingTrack.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/ThingTrack.h
			${CMAKE_CURRENT_SOURCE_DIR}/kd_tree.h
			${CMAKE_CURRENT_SOURCE_DIR}/ThingInterface.cpp  
			${CMAKE_CURRENT_SOURCE_DIR}/ThingInterface.h
			#PARENT_SCOPE
)
#add_subdirectory(thing_retention)
add_library(thing_retention SHARED ${thing_retention_src})
target_link_libraries(thing_retention PUBLIC munkres ${OpenCV_LIBS})
target_include_directories(thing_retention PRIVATE "${OpenCV_INCLUDE_DIRS}")
target_include_directories(thing_retention PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")





#set(test_thing_retention_src ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp				
              #          ${CMAKE_CURRENT_SOURCE_DIR}/ThingInterface.h
			#PARENT_SCOPE
#)
#add_executable(test_thing_retention  ${test_thing_retention_src})
#target_link_libraries(test_thing_retention PUBLIC thing_retention)


#add_subdirectory(munkres)







