TEMPLATE = app
CONFIG  += c++11
TARGET   = playerClientTest
#DESTDIR  = /mnt/hgfs/shareToUbuntu/serial/
DESTDIR  = $$PWD/../../target/test

CONFIG  -= qt

OBJECTS_DIR = build

HOME     = $$getenv("HOME")
contains(QT_ARCH, arm|arm64) {
PLATFORM = t5
isEmpty (EXTERN_LIB_DIR) {
INCLUDEPATH  += $$HOME/framework/$$PLATFORM/include
LIBS         += -L$$HOME/framework/$$PLATFORM/lib
} else {
INCLUDEPATH  += $${EXTERN_LIB_DIR}/libevents/include
LIBS         += -L$${EXTERN_LIB_DIR}/libevents/lib
}

} else {
PLATFORM = pc
QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib
}
INCLUDEPATH += .
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include
INCLUDEPATH += ../iface

LIBS += -L$$HOME/framework/$$PLATFORM/lib

DEFINES += IFACE_CLIENT
include (../iface/socket/socket.pri)

LIBS += -levents_core -levents_log -levents_util -lpthread

SOURCES += main.cpp

!isEmpty (INS_APP_DIR) {
appfile.path  = $${INS_APP_DIR}
appfile.files = $${DESTDIR}/$${TARGET}
INSTALLS += appfile
}

