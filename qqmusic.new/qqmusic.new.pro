TEMPLATE = app

QT      += qml quick
CONFIG  += c++11


HOME     = $$getenv("HOME")
PLATFORM = pc
contains (QT_ARCH, arm|arm64) {
PLATFORM = t5
DEFINES += PLATFORM_ARM
} else {
PLATFORM = pc
DEFINES += PLATFORM_PC
}

INCLUDEPATH += .
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include

QMAKE_CXXFLAGS += -g -std=c++11
QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib

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

HEADERS += \
    application/app_common.h \
    application/application.h \
    global/ctrl/update_operate_action.h \
    global/ctrl/layer_manage_control.h \
    global/ctrl/global_controller.h \
    global/manage/pages_factory.h \
    global/manage/pages_manager.h \
    pages/home/home_controller.h \
    global/manage/page_qml_item.h \
    global/win_global_common.h \
    global/manage/page_queue.h \
    common/image_frame.h \
    application/loadApi/qqmusic_singer_list.h \
    application/loadApi/qqmusic_object.h \
    application/loadApi/qqmusic_list_object.h \
    application/loadApi/qqmusic_item_object.h \
    application/loadApi/qqmusic_category_playlist.h \
    application/loadApi/qqmusic_category.h \
    application/loadApi/qqmusic_api_common.h \
    pages/playlist/category_playlist.h \
    pages/playlist/cpl_cate_item_model.h \
    common/load_model_base.h \
    pages/playlist/pl_category_model.h \
    pages/singerlist/singer_list_proc.h \
    common/app_load_common.h \
    pages/singerlist/singer_list_model.h \
    pages/singerlist/singer_tag_info.h

SOURCES += main.cpp \
    application/application.cpp \
    global/ctrl/update_operate_action.cpp \
    global/ctrl/layer_manage_control.cpp \
    global/ctrl/global_controller.cpp \
    global/manage/pages_factory.cpp \
    global/manage/pages_manager.cpp \
    pages/home/home_controller.cpp \
    global/manage/page_qml_item.cpp \
    global/manage/page_queue.cpp \
    common/image_frame.cpp \
    application/loadApi/qqmusic_singer_list.cpp \
    application/loadApi/qqmusic_object.cpp \
    application/loadApi/qqmusic_category_playlist.cpp \
    application/loadApi/qqmusic_category.cpp \
    pages/playlist/category_playlist.cpp \
    pages/playlist/cpl_cate_item_model.cpp \
    common/load_model_base.cpp \
    pages/playlist/pl_category_model.cpp \
    pages/singerlist/singer_list_proc.cpp \
    pages/singerlist/singer_list_model.cpp \
    pages/singerlist/singer_tag_info.cpp

RESOURCES += resource/qml.qrc \
             resource/image.qrc

