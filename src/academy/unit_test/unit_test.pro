TEMPLATE = app
TARGET = acad_ut
CONFIG += debug console
DESTDIR = ../../../bin
win32: DEFINES += USEXMLDTDLL _USEDDDLL
QT -= gui core
QT += xml

win32:QMAKE_CXXFLAGS += -MP 
INCLUDEPATH  += . ../../common/tut ../../common/utils
DEPENDPATH   += . ../../common/tut ../../common/utils

# Input
HEADERS +=  TstUtils.h
SOURCES +=  main.cpp TstUtils.cpp UnitTests.cpp

QMAKE_LIBDIR += ../lib
win32: LIBS += acad_d.lib

DEFINES += _CRT_SECURE_NO_WARNINGS

OBJECTS_DIR = $$PWD/obj/Debug
MOC_DIR = $$PWD/obj/Debug/moc
