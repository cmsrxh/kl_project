TEMPLATE = app

QT += qml quick
CONFIG += c++11
CONFIG += debug
CONFIG += qml_debug

OBJECTS_DIR = build
MOC_DIR     = build
UI_DIR      = build

HOME     = $$getenv("HOME")
PLATFORM = pc
INCLUDEPATH += .
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include

QMAKE_CXXFLAGS += -g -std=c++11
QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib

LIBS += -L$$HOME/framework/$$PLATFORM/lib
LIBS += -levents_core -levents_log -levents_util -lpthread

SOURCES += main.cpp \
    application.cpp \
    image_frame.cpp

RESOURCES += qml.qrc

include(./klApi.pri)

#下载数据相关一些功能统集
#include(../netLoad/netLoad.pri)
INCLUDEPATH += $${PWD}/../../netLoad
LIBS        += -L$${PWD}/../../target/lib
LIBS        += -lnetLoad -lcurl -lssl -lcrypto

HEADERS += \
    application.h \
    image_frame.h


