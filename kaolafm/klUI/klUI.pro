TEMPLATE = app
TARGET   = kaolaApp

QT += qml quick
CONFIG += c++11
CONFIG += debug
CONFIG += qml_debug

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build

DESTDIR  = $$PWD/../../target/bin

HOME     = $$getenv("HOME")
PLATFORM = pc
INCLUDEPATH += .
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include

QMAKE_CXXFLAGS += -g -std=c++11
QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib
QMAKE_LFLAGS   += -Wl,-rpath,$${PWD}/../../target/lib

LIBS += -L$$HOME/framework/$$PLATFORM/lib
LIBS += -levents_core -levents_log -levents_util -lpthread


#kl所有的数据加载API
include(../klApi/klApi.pri)

#下载数据相关一些功能统集
#include(../netLoad/netLoad.pri)
INCLUDEPATH += $${PWD}/../../netLoad

#kl把播放器功能作为一个SDK作为调用
#include(../player/mpvPlayer.pri)
#kl作为客户端接口，调用播放器服务程序的各种API
#include(../playerClient/playerClient.pri)
INCLUDEPATH += $${PWD}/../../player/iface

LIBS        += -L$${PWD}/../../target/lib
LIBS        += -lnetLoad  -lplayerIpcIface
LIBS        += -lcurl -lssl -lcrypto

#kl作为服务端接口，给各种需要调用kl功能的APP使用
include(../klIface/klServiceIface.pri)


# Input
HEADERS += application.h \
    kl_ui_proc.h \
    model/cate_item_model.h \
    model/kl_ui_data_union.h \
    model/category_model.h \
    model/cate_item_union.h \
    model/category_union.h \
    model/kl_data_proc.h \
    qml_view_switch_stack.h \
    model/chip_item_model.h \
    model/chip_item_union.h \
    model/detail_union.h \
    model/detail_qobject.h \
    model/chip_item_play_manage.h \
    model/collect_model.h \
    model/kl_local_data_proc.h \
    image_frame.h \
    pop_tip_manage.h \
    current_backup.h

SOURCES += application.cpp main.cpp \
    kl_ui_proc.cpp \
    model/cate_item_model.cpp \
    model/category_model.cpp \
    model/cate_item_union.cpp \
    model/category_union.cpp \
    model/kl_data_proc.cpp \
    qml_view_switch_stack.cpp \
    model/chip_item_model.cpp \
    model/chip_item_union.cpp \
    model/detail_union.cpp \
    model/detail_qobject.cpp \
    model/chip_item_play_manage.cpp \
    model/collect_model.cpp \
    model/kl_local_data_proc.cpp \
    image_frame.cpp \
    pop_tip_manage.cpp \
    current_backup.cpp

RESOURCES += \
    res/qml.qrc \
    res/image.qrc

!isEmpty (INS_APP_DIR) {
appfile.path  = $${INS_APP_DIR}
appfile.files = $${DESTDIR}/$${TARGET}
INSTALLS += appfile
}
