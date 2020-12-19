TEMPLATE  = lib
CONFIG   += plugin
CONFIG   -= qt
TARGET    = playerIpcIface
DESTDIR   = $$PWD/../../target/lib

HOME     = $$getenv("HOME")
contains(QT_ARCH, arm|arm64) {
message("Platform T5")
PLATFORM = t5

isEmpty (EXTERN_LIB_DIR) {
INCLUDEPATH  += $$HOME/framework/$$PLATFORM/include
LIBS         += -L$$HOME/framework/$$PLATFORM/lib
} else {
INCLUDEPATH  += $${EXTERN_LIB_DIR}/libevents/include
LIBS         += -L$${EXTERN_LIB_DIR}/libevents/lib
}
} else {
message("Platform PC")
PLATFORM = pc
INCLUDEPATH  += $$HOME/framework/$$PLATFORM/include
LIBS         += -L$$HOME/framework/$$PLATFORM/lib
}

INCLUDEPATH += . ../iface

include (gdbus/gdbus.pri)
DEFINES += IFACE_CLIENT
include (socket/socket.pri)

HEADERS += \
    $$PWD/media_client_call_back.h \
    $$PWD/media_iface_common.h \
    $$PWD/media_service_call_back.h \
    $$PWD/media_service_proxy.h \
    $$PWD/media_client_proxy.h


# qmake "INS_INC_DIR=/usr/include" "INS_LIB_DIR=/usr/lib"
# 安装头文件
# 命令行传递参数, qmake "INS_INC_DIR=/usr/include"
!isEmpty (INS_INC_DIR) {
header.path  = $${INS_INC_DIR}
header.files = $$PWD/media_service_call_back.h \
               $$PWD/media_iface_common.h \
               $$PWD/media_client_proxy.h

INSTALLS += header
message("install header file dir: $${INS_INC_DIR}")
}

# 安装库文件
# 命令行传递参数, qmake "INS_LIB_DIR=/usr/lib"
!isEmpty (INS_LIB_DIR) {
libfile.path  = $${INS_LIB_DIR}
libfile.files = $${DESTDIR}/lib$${TARGET}.so
INSTALLS += libfile
message("install lib file dir: $${INS_LIB_DIR}")
}
