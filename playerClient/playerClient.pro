TEMPLATE = app
CONFIG  += c++11
TARGET   = playerClientTest

CONFIG  -= qt

OBJECTS_DIR = build

HOME     = $$getenv("HOME")
PLATFORM = pc
INCLUDEPATH += .
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include
INCLUDEPATH += ../player/iface

QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib

LIBS += -L$$HOME/framework/$$PLATFORM/lib
LIBS += -levents_ipc_client -levents_core -levents_log -levents_util -lpthread

SOURCES += main.cpp \
    ../player/iface/media_service_i_face.cpp

HEADERS += \
    ../player/iface/media_service_i_face.h \
    ../player/iface/media_service_call_back.h \
    ../player/iface/media_iface_common.h

