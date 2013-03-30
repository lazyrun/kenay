#DEFINES += EXECUTOR_EXE

TARGET = acad

contains(DEFINES, EXECUTOR_EXE) {
TEMPLATE = app
DESTDIR = ../../bin
} else {
TEMPLATE = lib
DESTDIR = lib
DLLDESTDIR = ../../bin
DEFINES += MAKE_EXEC_DLL
}

DEFINES += _CRT_SECURE_NO_DEPRECATE
QT += xml sql

CONFIG += debug_and_release
CONFIG -= flat
CONFIG += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = qtincludes.h

INCLUDEPATH  += ./ src ../common/common ../common/utils src/sqlite
DEPENDPATH   += ./ src ../common/common ../common/utils src/sqlite

# Input
HEADERS += qtincludes.h \
           $$files(src/*.h) \
           Config.h \
           GlobVars.h \
           Hooker.h \
           DBManager.h \

SOURCES += $$files(src/*.cpp) \
           Config.cpp \
           GlobVars.cpp \           
           Hooker.cpp \
           DBManager.cpp
           
contains(DEFINES, EXECUTOR_EXE) {
SOURCES += main.cpp
} else {
HEADERS += acadlib.h
SOURCES += acadlib.cpp
}

win32:QMAKE_CXXFLAGS += -MP

CONFIG(debug, debug|release) {
     TARGET = $$join(TARGET,,,_d)
     OBJECTS_DIR = obj/Debug
     UI_DIR = obj/Debug/ui
     MOC_DIR = obj/Debug/moc
     RCC_DIR = obj/Debug
     DEPENDPATH += . obj/Debug/ui obj/Debug/moc
} else {
     OBJECTS_DIR = obj/Release
     UI_DIR = obj/Release/ui
     MOC_DIR = obj/Release/moc
     RCC_DIR = obj/Debug
     DEPENDPATH += . obj/Release/ui obj/Release/moc
}

include(../common/img_proc/ImageProc.pri)
include(../common/DirectMap/src/DirectMap.pri)

contains(DEFINES, EXECUTOR_EXE) {
include(../common/GlobalShortcut/GlobalShortcut.pri)
}
