#-------------------------------------------------
#
# Project created by QtCreator 2017-08-13T17:36:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test2
TEMPLATE = app
CONFIG += c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    camthread2.cpp \
    camthread4.cpp \
    camthread1.cpp \
    camthread3.cpp \
    communication.cpp\
    dialog.cpp \
    dialog_2.cpp \
    dialog_3.cpp \
    dialog_4.cpp \
    dialog_5.cpp \
    KalmanTracker.cpp \
    myFunction.cpp

HEADERS  += mainwindow.h \
    camthread4.h \
    camthread2.h \
    camthread1.h \
    camthread3.h \
    communication.h\
    dialog.h \
    dialog_2.h \
    dialog_3.h \
    dialog_4.h \
    dialog_5.h \
    channel.h \
    KalmanTracker.h \
    myFunction.h

FORMS    += mainwindow.ui \
    dialog.ui \
    dialog_2.ui \
    dialog_3.ui \
    dialog_4.ui \
    dialog_5.ui

INCLUDEPATH += /usr/include/opencv4 \
                /usr/include/opencv4/opencv2 \
                 /usr/include/boost/ \
                 ./munkres/include

LIBS += /usr/lib/aarch64-linux-gnu/libopencv_* \
        /usr/lib/aarch64-linux-gnu/libboost_* \
        ./munkres/lib/libmunkres.a
#        /usr/local/opencv420/lib/libopencv_core.so \
#        /usr/local/opencv420/lib/libopencv_highgui.so \
#        /usr/local/opencv420/lib/libopencv_imgproc.so \
#        /usr/local/opencv420/lib/libopencv_ml.so \
#        /usr/local/opencv420/lib/libopencv_objdetect.so \
#        /usr/local/opencv420/lib/libopencv_video.so \
#        /usr/local/opencv420/lib/libopencv_videoio.so \
#        /usr/local/opencv420/lib/libopencv_imgcodecs.so \
#        /usr/local/opencv420/lib/libopencv_dnn.so \
#        /usr/local/opencv420/lib/libopencv_cudabgsegm.so \


