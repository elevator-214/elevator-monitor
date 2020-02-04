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
set ( test2_HDRS
	channel.h
	KalmanTracker.h
	myFunction.h
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
	KalmanTracker.cpp
	myFunction.cpp
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

add_executable ( test2 ${test2_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( test2  Qt5::Core Qt5::Gui Qt5::Widgets )
