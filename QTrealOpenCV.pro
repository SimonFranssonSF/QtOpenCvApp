#-------------------------------------------------
#
# Project created by QtCreator 2017-11-08T05:08:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SomeApp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

PKGCONFIG += opencv

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /usr/local/Cellar/opencv3/3.2.0/include/
INCLUDEPATH += /usr/local/Cellar/boost/1.65.1/include/

SOURCES += main.cpp\
        mainwindow.cpp \
    filterbuttons.cpp \
    computervision.cpp \
    featurebuttons.cpp \
    leftcolwidget.cpp \
    factorygroupbox.cpp \
    info.cpp

HEADERS  += mainwindow.h \
    filterbuttons.h \
    computervision.h \
    featurebuttons.h \
    leftcolwidget.h \
    factorygroupbox.h \
    info.h

LIBS += -L/usr/local/Cellar/opencv3/3.2.0/lib \
     -lopencv_core \
     -lopencv_imgproc \
     -lopencv_features2d\
     -lopencv_highgui

LIBS += -L/usr/local/Cellar/boost/1.65.1/lib \
     -lboost_system \

FORMS    += mainwindow.ui

RESOURCES += \
    filterresources.qrc
