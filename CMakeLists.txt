project( test2 )
cmake_minimum_required( VERSION 3.1 )
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_BUILD_TYPE Release )


add_definitions ( -Wall )
find_package ( Qt5Widgets REQUIRED )
find_package ( Qt5Core REQUIRED )
find_package ( Qt5Gui REQUIRED )
# 寻找OpenCV库
#message(STATUS "CMAKE_MODULE_PATH:${CMAKE_MODULE_PATH}
#                       CMAKE_PREFIX_PATH${CMAKE_PREFIX_PATH}
#                       CMAKE_FRAMEWORK_PATH${CMAKE_FRAMEWORK_PATH}
#                       CMAKE_APPBUNDLE_PATH${CMAKE_APPBUNDLE_PATH}
#                       OpenCV_DIR${OpenCV_DIR}

#                       OpenCV_VERSION：${OpenCV_VERSION}")
#message(STATUS "OpenCV_DIR:${Boost_DIR}")
find_package( OpenCV  REQUIRED)
find_package(Boost REQUIRED)   #要使用的boost库

set ( test2_HDRS
        channel.h
        )

set ( test2_SRCS
        main.cpp
        mainwindow.cpp
        camthread2.cpp
        camthread4.cpp
        camthread1.cpp
        camthread3.cpp
        communication.cpp
        dialog.cpp
        dialog_2.cpp
        dialog_3.cpp
        dialog_4.cpp
        dialog_5.cpp

        )

set ( test2_UIS
        mainwindow.ui
        dialog.ui
        dialog_2.ui
        dialog_3.ui
        dialog_4.ui
        dialog_5.ui
        )
QT5_WRAP_UI(UIS ${test2_UIS})

set ( test2_MOCS
        mainwindow.h
        camthread4.h
        camthread2.h
        camthread1.h
        camthread3.h
        communication.h
        dialog.h
        dialog_2.h
        dialog_3.h
        dialog_4.h
        dialog_5.h
        )
QT5_WRAP_CPP(MOCS ${test2_MOCS})
add_subdirectory(read_parameter)
add_subdirectory(munkres)
add_subdirectory(kalman_track)



add_executable ( test2 ${test2_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( test2 PUBLIC
    -Wl,--start-group
    read_parameter
    kalman_track
    munkres
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${OpenCV_LIBS}
    ${Boost_LIBS}
    -Wl,--end-group
    -lboost_system -lpthread -lm)
