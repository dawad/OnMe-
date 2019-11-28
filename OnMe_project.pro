#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T14:18:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OnMe_project
TEMPLATE = app
#QMAKE_CXXFLAGS_DEBUG += -Zi -MTd
#QMAKE_CXXFLAGS_RELEASE += -MT

INCLUDEPATH += "C:/opencv-3.1.0/opencv-3.1.0/build/install/include" \
               "C:/opencv-3.1.0/opencv-3.1.0/build/install/include/opencv" \
               "C:/opencv-3.1.0/opencv-3.1.0/build/install/include/opencv2"

LIBS += -LC:/opencv-3.1.0/opencv-3.1.0/build/install/x86/mingw/bin\
    -lopencv_aruco310\
    -lopencv_bioinspired310\
    -lopencv_dnn310\
    -lopencv_face310\
    -lopencv_ml310\
    -lopencv_tracking310\
    -lopencv_superres310\
    -lopencv_video310\
    -lopencv_videostab310\
    -lopencv_videoio310\
    -lopencv_ximgproc310\
    -lopencv_xfeatures2d310\
    -lopencv_stitching310\
    -lopencv_objdetect310\
    -lopencv_core310 \
    -lopencv_flann310\
    -lopencv_highgui310 \
    -lopencv_imgproc310 \
    -lopencv_features2d310 \
    -lopencv_calib3d310 \
    -lopencv_ccalib310\
    -lopencv_imgcodecs310 \

SOURCES += main.cpp\
        tabpointdialog.cpp \
    imageview.cpp \
    editeurpoints.cpp \
    mediafiles.cpp \
    dtgui2opencv.cpp \
    ismatching.cpp

HEADERS  += tabpointdialog.h \
    imageview.h \
    editeurpoints.h \
    mediafiles.h \
    dtgui2opencv.h \
    ismatching.h

FORMS    += tabpointdialog.ui
