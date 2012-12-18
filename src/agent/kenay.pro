TEMPLATE = app
TARGET = kenay
DESTDIR = ../../bin

CONFIG += debug_and_release
CONFIG -= flat
CONFIG += precompile_header

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = qtincludes.h

INCLUDEPATH  += ./ src ../common/common
DEPENDPATH   += ./ src ../common/common

RESOURCES += resources/agent.qrc

# Input
HEADERS += Agent.h \
           qtincludes.h \
           SettingsData.h \
           RangesData.h

SOURCES += main.cpp \
           Agent.cpp \
           SettingsData.cpp \
           RangesData.cpp
           
win32:RC_FILE = resources/agent.rc
win32:DEFINES += _CRT_SECURE_NO_DEPRECATE

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

include(../common/QtSingleApplication/src/qtsingleapplication.pri)
include(../common/GlobalShortcut/GlobalShortcut.pri)
