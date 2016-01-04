#-------------------------------------------------
#
# Project created by QtCreator 2015-12-31T14:32:57
#
#-------------------------------------------------

QT       += core serialport

QT       -= gui

TARGET = conboardSDK
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
            ../../../lib/inc \
            ../../../lib/DJIscript/inc \

SOURCES += main.cpp \
    conboardsdktask.cpp \
    QonboardSDK.cpp \
    ../../../lib/src/DJI_API.cpp \
    ../../../lib/src/DJI_App.cpp \
    ../../../lib/src/DJI_Camera.cpp \
    ../../../lib/src/DJI_Codec.cpp \
    ../../../lib/src/DJI_Flight.cpp \
    ../../../lib/src/DJI_Follow.cpp \
    ../../../lib/src/DJI_HardDriver.cpp \
    ../../../lib/src/DJI_HotPoint.cpp \
    ../../../lib/src/DJI_Link.cpp \
    ../../../lib/src/DJI_Memory.cpp \
    ../../../lib/src/DJI_Mission.cpp \
    ../../../lib/src/DJI_VirtualRC.cpp \
    ../../../lib/src/DJI_WayPoint.cpp \
    ../../../lib/DJIscript/src/DJI_Interpreter.cpp \
    ../../../lib/DJIscript/src/DJI_Script.cpp

HEADERS += \
    conboardsdktask.h \
    QonboardSDK.h
