TEMPLATE = app

QT += qml quick
CONFIG += c++11
CONFIG += debug
CONFIG += qml_debug

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build

HOME     = $$getenv("HOME")
PLATFORM = pc
INCLUDEPATH += .
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include

QMAKE_CXXFLAGS += -g -std=c++11
QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib

LIBS += -L$$HOME/framework/$$PLATFORM/lib
LIBS += -levents_core -levents_log -levents_util -lpthread

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

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#下载数据相关一些功能统集
include(../netLoad/netLoad.pri)

#qqmusic作为客户端接口，调用播放器服务程序的各种API
include(../playerClient/playerClient.pri)

#添加qqmusic api接口封装
include(qqmusicApi/qqmusicApi.pri)
