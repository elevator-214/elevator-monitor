add_library(munkres ${CMAKE_CURRENT_SOURCE_DIR}/lib/libmunkres.a)
set_target_properties(munkres PROPERTIES LINKER_LANGUAGE CXX)#explicitly tell cmake what language the target is written in
target_include_directories(munkres PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
