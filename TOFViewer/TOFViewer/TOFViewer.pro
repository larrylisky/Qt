#-------------------------------------------------
#
# Project created by QtCreator 2017-04-25T16:33:22
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TOFViewer
TEMPLATE = app

VOXEL_INC       += /usr/include/voxel /usr/include/voxel/pcl /usr/include/voxel/ti3dtof /usr/include/voxel/Filter
VOXEL_LIBS      += -lti3dtof -lvoxel

OPENCV_INC      += `pkg-config --cflags opencv`
OPENCV_LIBS     += `pkg-config --libs opencv`

PCL_INC         +=  /usr/include/pcl-1.7
PCL_LIBS        +=  -L/usr/lib/x86_64-linux-gnu -lpcl_common -lpcl_io -lpcl_outofcore -lpcl_sample_consensus -lpcl_tracking\
                    -lpcl_features -lpcl_kdtree -lpcl_people -lpcl_search -lpcl_visualization -lpcl_filters -lpcl_keypoints\
                    -lpcl_recognition -lpcl_segmentation -lpcl_io_ply -lpcl_octree -lpcl_registration -lpcl_surface

INCLUDEPATH     += . $$VOXEL_INC $$OPENCV_INC $$PCL_INC

SOURCES         += main.cpp\
                    mainwindow.cpp \
    tofopenglwidget.cpp \
    mainwindow_support.cpp \
    mainwindow_filemenu.cpp \
    mainwindow_profilemenu.cpp \
    mainwindow_maintoolbar.cpp \
    mainwindow_slider.cpp \
    mainwindow_timer.cpp \
    mainwindow_windowsmenu.cpp \
    mainwindow_dockwidgets.cpp \
    grabber.cpp \
    mainwindow_parameter.cpp \
    mainwindow_dataflow.cpp \
    dataflowgraphicsview.cpp

HEADERS         += mainwindow.h \
    tofopenglwidget.h \
    grabber.h \
    dataflowgraphicsview.h

QMAKE_CXXFLAGS  += -pthread -std=c++11 -fPIC -ffast-math -fpermissive

LIBS            += $$OPENCV_LIBS $$VOXEL_LIBS $$PCL_LIBS -lGL -lglut -lGLU

FORMS           += mainwindow.ui

RESOURCES += \
    resources.qrc
