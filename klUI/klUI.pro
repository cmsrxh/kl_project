TEMPLATE = app

QT += qml quick
CONFIG += c++11

HOME     = $$getenv("HOME")
PLATFORM = pc
APISRC   = ../klApi

INCLUDEPATH += .
INCLUDEPATH += $$APISRC
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include

QMAKE_CXXFLAGS += -g -std=c++11
QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib

LIBS += -L$$HOME/framework/$$PLATFORM/lib
LIBS += -levents_core -levents_log -levents_util -lpthread
LIBS += -lcurl -lssl -lcrypto


# Input
HEADERS += app_common.h application.h \
    $$APISRC/net_util/curl_load_proc.h \
    $$APISRC/net_util/curl_load_item.h \
    $$APISRC/net_util/load_item.h \
    $$APISRC/net_util/http_util.h \
    $$APISRC/kl_url/kl_active_manage.h \
    $$APISRC/net_util/net_url.h \
    $$APISRC/kl_url/kl_init_manage.h \
    $$APISRC/kl_url/kl_object.h \
    $$APISRC/kl_url/kl_image.h \
    $$APISRC/net_util/net_buffer.h \
    $$APISRC/net_util/test_data_load.h \
    $$APISRC/net_util/net_common.h \
    $$APISRC/net_util/cJSON.h \
    $$APISRC/config_local_info.h \
    $$APISRC/kl_url/kl_broadcast_area_list.h \
    $$APISRC/kl_url/kl_category_broadcast.h \
    $$APISRC/kl_url/kl_category_sub_list.h \
    $$APISRC/kl_url/kl_common.h \
    $$APISRC/kl_url/kl_save_object.h \
    $$APISRC/kl_url/kl_broadcast_item_list.h \
    $$APISRC/kl_url/kl_broadcast_item_detail.h \
    $$APISRC/kl_url/kl_item_object.h \
    $$APISRC/kl_url/kl_broadcast_item_programlist.h \
    $$APISRC/kl_url/kl_category_all.h \
    $$APISRC/kl_url/kl_operate_list.h \
    $$APISRC/kl_url/kl_type_radio_list.h \
    $$APISRC/kl_url/kl_album_list.h \
    $$APISRC/kl_url/kl_album_detail.h \
    $$APISRC/kl_url/kl_chip_radio_list.h \
    $$APISRC/kl_url/kl_chip_radio_detail.h \
    $$APISRC/kl_url/kl_chip_audio_detail.h \
    $$APISRC/kl_url/kl_chip_audio_list.h \
    model/category_all_model.h \
    model/cate_item_model.h


SOURCES += application.cpp main.cpp kl_ui_proc.cpp \
    $$APISRC/net_util/curl_load_proc.cpp \
    $$APISRC/net_util/curl_load_item.cpp \
    $$APISRC/net_util/load_item.cpp \
    $$APISRC/net_util/http_util.cpp \
    $$APISRC/kl_url/kl_active_manage.cpp \
    $$APISRC/net_util/net_url.cpp \
    $$APISRC/kl_url/kl_init_manage.cpp \
    $$APISRC/kl_url/kl_object.cpp \
    $$APISRC/kl_url/kl_image.cpp \
    $$APISRC/net_util/net_buffer.cpp \
    $$APISRC/net_util/test_data_load.cpp \
    $$APISRC/kl_url/kl_broadcast_area_list.cpp \
    $$APISRC/kl_url/kl_category_broadcast.cpp \
    $$APISRC/kl_url/kl_category_sub_list.cpp \
    $$APISRC/kl_url/kl_broadcast_item_list.cpp \
    $$APISRC/kl_url/kl_broadcast_item_detail.cpp \
    $$APISRC/net_util/cJSON.cpp \
    $$APISRC/kl_url/kl_broadcast_item_programlist.cpp \
    $$APISRC/kl_url/kl_category_all.cpp \
    $$APISRC/kl_url/kl_operate_list.cpp \
    $$APISRC/kl_url/kl_type_radio_list.cpp \
    $$APISRC/kl_url/kl_album_list.cpp \
    $$APISRC/kl_url/kl_album_detail.cpp \
    $$APISRC/kl_url/kl_chip_radio_list.cpp \
    $$APISRC/kl_url/kl_chip_radio_detail.cpp \
    $$APISRC/kl_url/kl_chip_audio_detail.cpp \
    $$APISRC/kl_url/kl_chip_audio_list.cpp \
    model/category_all_model.cpp \
    model/cate_item_model.cpp

RESOURCES += res/qml.qrc



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

HEADERS += \
    kl_ui_proc.h
