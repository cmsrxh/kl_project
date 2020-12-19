TEMPLATE = app
CONFIG  += c++11
TARGET   = mediaHmiTest
QT      += qml quick core
DESTDIR  = $$PWD/../../target/test

INCLUDEPATH += ../iface
INCLUDEPATH += .
HOME     = $$getenv("HOME")

contains(QT_ARCH, arm|arm64) {
message("Platform T5")
PLATFORM = t5
DEFINES += PLATFORM_QUANZHI
DEFINES += PLATFORM_ARM

isEmpty (EXTERN_LIB_DIR) {
INCLUDEPATH  += $$HOME/framework/$$PLATFORM/include
LIBS         += -L$$HOME/framework/$$PLATFORM/lib
} else {
INCLUDEPATH  += $${EXTERN_LIB_DIR}/libevents/include
LIBS         += -L$${EXTERN_LIB_DIR}/libevents/lib
}

!isEmpty (PUBLIC_LIB_DIR) {
INCLUDEPATH  += $${PUBLIC_LIB_DIR}/libDevicesManagerSocket/include
INCLUDEPATH  += $${PUBLIC_LIB_DIR}/libplayerIpcIface/include

LIBS         += -L$${PUBLIC_LIB_DIR}/libDevicesManagerSocket/lib
LIBS         += -L$${PUBLIC_LIB_DIR}/libplayerIpcIface/lib
} else {
INCLUDEPATH  += $${PWD}/../../devices_manager/iface
INCLUDEPATH  += $${PWD}/../../media/iface
LIBS         += -L$${PWD}/../../target/lib
}
} else {
message("Platform PC")
PLATFORM = pc
DEFINES += PLATFORM_PC
QMAKE_LFLAGS += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib
QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../target/lib
INCLUDEPATH  += $$HOME/framework/$$PLATFORM/include
LIBS         += -L$$HOME/framework/$$PLATFORM/lib
LIBS         += -L$${PWD}/../../target/lib
INCLUDEPATH  += $${PWD}/../../devices_manager/iface
INCLUDEPATH  += $${PWD}/../../media/iface

}
#-lDevicesManagerSocket
LIBS +=  -lplayerIpcIface
LIBS += -levents_core -levents_log -levents_util -lpthread
LIBS += -levents_ipc_client -levents_ipc_service  -levents_socket



## 播放服务socket ipc接口
## 设备管理服务socket ipc通讯接口

RESOURCES += \
    resources/image.qrc \
    resources/qml.qrc

HEADERS += \
    $$PWD/media_hmi_ctrl.h \
    $$PWD/image_provider.h \
    $$PWD/media_file_model.h \
    $$PWD/media_call_back.h \
    $$PWD/media_hmi_i_face.h \
    media_file_internal.h \
    manage/dir_tree.h \
    manage/file_travel.h \
    manage/app_common.h \
    manage/application.h

SOURCES += \
    $$PWD/media_hmi_ctrl.cpp \
    $$PWD/image_provider.cpp \
    $$PWD/media_file_model.cpp \
    $$PWD/media_call_back.cpp \
    $$PWD/media_hmi_i_face.cpp \
    $$PWD/main.cpp \
    manage/dir_tree.cpp \
    manage/file_travel.cpp \
    manage/application.cpp

