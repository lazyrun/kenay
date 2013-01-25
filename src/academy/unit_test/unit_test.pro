TEMPLATE = app
TARGET = acad_ut
CONFIG += debug console
DESTDIR = ../../../bin
win32: DEFINES += USEXMLDTDLL _USEDDDLL
QT += xml sql

CONFIG += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = qtincludes.h

win32:QMAKE_CXXFLAGS += -MP 
INCLUDEPATH  += . ../../common/tut ../../common/utils ../src ../src/sqlite
DEPENDPATH   += . ../../common/tut ../../common/utils ../src ../src/sqlite

# Input
HEADERS +=  qtincludes.h TstUtils.h CardBase.h Config.h ProcAcad.h PotParser.h StackParser.h BetParser.h \
            Mind.h HoleCards.h DBManager.h Session.h Opp.h
SOURCES +=  main.cpp TstUtils.cpp Config.cpp ProcAcad.cpp PotParser.cpp StackParser.cpp BetParser.cpp \
            LogicTest.cpp ProcTest.cpp Mind.cpp HoleCards.cpp DBManager.cpp Session.cpp Opp.cpp

QMAKE_LIBDIR += ../lib
win32: LIBS += acad_d.lib

DEFINES += _CRT_SECURE_NO_WARNINGS

OBJECTS_DIR = $$PWD/obj/Debug
MOC_DIR = $$PWD/obj/Debug/moc

include(../../common/img_proc/ImageProc.pri)
include(../../common/DirectMap/src/DirectMap.pri)
