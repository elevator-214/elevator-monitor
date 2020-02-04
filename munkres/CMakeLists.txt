add_library(munkres ${CMAKE_CURRENT_SOURCE_DIR}/lib/libmunkres.a)
target_include_directories(munkres PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
