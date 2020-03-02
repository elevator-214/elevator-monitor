add_definitions ( -Wall -g )
set(CMAKE_FIND_DEBUG_MODE 1)
find_package ( Qt5Widgets REQUIRED )
find_package ( Qt5Core REQUIRED )
find_package ( Qt5Gui REQUIRED )
find_package ( PythonLibs 3.6 REQUIRED)
if(PythonLibs_FOUND)
    message(STATUS "PYTHON_LIBRARIES:${PYTHON_LIBRARIES}+++")
    message(STATUS "PythonLibs_DIR:${PythonLibs_DIR}+++")
endif()
# 寻找OpenCV库
#message(STATUS "CMAKE_MODULE_PATH:${CMAKE_MODULE_PATH}
#                       CMAKE_PREFIX_PATH${CMAKE_PREFIX_PATH}
#                       CMAKE_FRAMEWORK_PATH${CMAKE_FRAMEWORK_PATH}
#                       CMAKE_APPBUNDLE_PATH${CMAKE_APPBUNDLE_PATH}
#                       OpenCV_DIR${OpenCV_DIR}

#                       OpenCV_VERSION：${OpenCV_VERSION}")
#message(STATUS "OpenCV_DIR:${Boost_DIR}")
#message(STATUS "CMAKE_MODULE_PATH:${CMAKE_MODULE_PATH}+++")
#message(STATUS "OpenCV_DIR:${OpenCV_DIR}+++")
#message(STATUS "Boost_DIR:${Boost_DIR}+++")
link_directories(/usr/lib/aarch64-linux-gnu)
link_directories(lib/)




set ( elevator_monitor_HDRS
        channel.h
        )

set ( elevator_monitor_SRCS
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

set ( elevator_monitor_UIS
        mainwindow.ui
        dialog.ui
        dialog_2.ui
        dialog_3.ui
        dialog_4.ui
        dialog_5.ui
        )
QT5_WRAP_UI(UIS ${elevator_monitor_UIS})

set ( elevator_monitor_MOCS
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
QT5_WRAP_CPP(MOCS ${elevator_monitor_MOCS})



#message(STATUS "python:${thing_retention_src}+++")
#message(STATUS "TRS:${TRS}+++")
#message(STATUS "MOCS:${MOCS}+++")
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'+++")
#endforeach()

add_executable ( elevator_monitor ${elevator_monitor_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( elevator_monitor PUBLIC
    -Wl,--start-group
    read_parameter
    kalman_track
    #${CMAKE_CURRENT_SOURCE_DIR}/libthing_retention.a
    thing_retention
    openpose
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets   
    ${OpenCV_LIBS}
    ${Boost_LIBS}
    ${PYTHON_LIBRARIES}
    -Wl,--end-group
    -lboost_system -lpthread -lm)
