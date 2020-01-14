
OBJECTS_DIR = build

HOME     = $$getenv("HOME")
PLATFORM = pc
INCLUDEPATH += .
#INCLUDEPATH += $$HOME/framework/$$PLATFORM/include
INCLUDEPATH += ../player/iface
INCLUDEPATH += ../player

# QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib

LIBS += -L$$HOME/framework/$$PLATFORM/lib
LIBS += -levents_ipc_client -levents_socket

SOURCES += \
    ../player/iface/media_service_i_face.cpp

HEADERS += \
    ../player/iface/media_service_i_face.h \
    ../player/iface/media_service_call_back.h \
    ../player/iface/media_iface_common.h

