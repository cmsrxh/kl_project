TEMPLATE = app
TARGET   = qqmusic

QT += qml quick
CONFIG += c++11
CONFIG += debug
CONFIG += qml_debug

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build

DESTDIR  = $$PWD/../target/bin

HOME     = $$getenv("HOME")
PLATFORM = pc
INCLUDEPATH += .
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include

QMAKE_CXXFLAGS += -g -std=c++11
QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib
QMAKE_LFLAGS   += -Wl,-rpath,$${PWD}/../target/lib

LIBS += -L$$HOME/framework/$$PLATFORM/lib
LIBS += -levents_core -levents_log -levents_util -lpthread



#下载数据相关一些功能统集
#include(../netLoad/netLoad.pri)
INCLUDEPATH += $${PWD}/../netLoad

#qqmusic作为客户端接口，调用播放器服务程序的各种API
#include(../playerClient/playerClient.pri)
INCLUDEPATH += $${PWD}/../player/iface

LIBS        += -L$${PWD}/../target/lib
LIBS        += -lnetLoad  -lplayerIpcIface

#添加qqmusic api接口封装
include(qqmusicApi/qqmusicApi.pri)


# Input
HEADERS += app_common.h \
    application.h \
    model/cate_item_model.h \
    model/category_model.h \
    qml_view_switch_stack.h \
    model/chip_item_model.h \
    model/detail_qobject.h \
    model/collect_model.h \
    image_frame.h \
    pop_tip_manage.h \
    current_backup.h \
    model/category_union.h \
    model/cate_item_union.h \
    model/chip_item_play_manage.h \
    model/chip_item_union.h \
    model/detail_union.h \
    ui_proc.h \
    model/data_proc.h \
    model/ui_data_union.h \
    model/local_data_proc.h \
    singer_list_proc.h \
    category_playlist.h

SOURCES += application.cpp main.cpp \
    model/cate_item_model.cpp \
    model/category_model.cpp \
    qml_view_switch_stack.cpp \
    model/chip_item_model.cpp \
    model/detail_qobject.cpp \
    model/collect_model.cpp \
    image_frame.cpp \
    pop_tip_manage.cpp \
    current_backup.cpp \
    model/category_union.cpp \
    model/cate_item_union.cpp \
    model/chip_item_play_manage.cpp \
    model/chip_item_union.cpp \
    model/detail_union.cpp \
    ui_proc.cpp \
    model/data_proc.cpp \
    model/local_data_proc.cpp \
    singer_list_proc.cpp \
    category_playlist.cpp

RESOURCES += \
    res/qml.qrc \
    res/image.qrc


!isEmpty (INS_APP_DIR) {
appfile.path  = $${INS_APP_DIR}
appfile.files = $${DESTDIR}/$${TARGET}
INSTALLS += appfile
}
