TEMPLATE = app
TARGET = acad_ut
CONFIG += debug console
DESTDIR = ../../../bin
win32: DEFINES += USEXMLDTDLL _USEDDDLL
QT += xml

CONFIG += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = qtincludes.h

win32:QMAKE_CXXFLAGS += -MP 
INCLUDEPATH  += . ../../common/tut ../../common/utils ../src
DEPENDPATH   += . ../../common/tut ../../common/utils ../src

# Input
HEADERS +=  qtincludes.h TstUtils.h CardBase.h
SOURCES +=  main.cpp TstUtils.cpp UnitTests.cpp
            

QMAKE_LIBDIR += ../lib
win32: LIBS += acad_d.lib

DEFINES += _CRT_SECURE_NO_WARNINGS

OBJECTS_DIR = $$PWD/obj/Debug
MOC_DIR = $$PWD/obj/Debug/moc

include(../../common/img_proc/ImageProc.pri)