######################################################################
# Automatically generated by qmake (3.1) Fri Oct 11 14:08:12 2019
######################################################################

TEMPLATE = app
TARGET   = kl
CONFIG   -= qt

HOME     = $$getenv("HOME")
PLATFORM = pc
INCLUDEPATH += .
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include

QMAKE_CXXFLAGS += -g -std=c++11
QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib

LIBS += -L$$HOME/framework/$$PLATFORM/lib
LIBS += -levents_core -levents_log -levents_util -lpthread
LIBS += -lcurl -lssl -lcrypto


# Input
HEADERS += app_common.h application.h \ 
    net_util/curl_load_proc.h \
    net_util/curl_load_item.h \
    net_util/load_item.h \
    net_util/http_util.h \
    kl_url/kl_active_manage.h \
    net_util/net_url.h \
    kl_url/kl_init_manage.h \
    kl_url/kl_object.h \
    kl_url/kl_image.h \
    net_util/net_buffer.h \
    net_util/test_data_load.h \
    net_util/net_common.h \
    net_util/cJSON.h \
    config_local_info.h \
    kl_url/kl_broadcast_area_list.h \
    kl_url/kl_category_broadcast.h \
    kl_url/kl_category_sub_list.h \
    kl_url/kl_common.h \
    kl_url/kl_save_object.h \
    kl_url/kl_broadcast_item_list.h \
    kl_url/kl_broadcast_item_detail.h \
    kl_url/kl_item_object.h \
    kl_url/kl_broadcast_item_programlist.h \
    kl_url/kl_category_all.h \
    kl_url/kl_operate_list.h \
    kl_url/kl_type_radio_list.h \
    kl_url/kl_album_list.h \
    kl_url/kl_album_detail.h \
    kl_url/kl_chip_radio_list.h \
    kl_url/kl_chip_radio_detail.h \
    kl_url/kl_chip_audio_detail.h \
    kl_url/kl_chip_audio_list.h


SOURCES += application.cpp main.cpp \ 
    net_util/curl_load_proc.cpp \
    net_util/curl_load_item.cpp \
    net_util/load_item.cpp \
    net_util/http_util.cpp \
    kl_url/kl_active_manage.cpp \
    net_util/net_url.cpp \
    kl_url/kl_init_manage.cpp \
    kl_url/kl_object.cpp \
    kl_url/kl_image.cpp \
    net_util/net_buffer.cpp \
    net_util/test_data_load.cpp \
    kl_url/kl_broadcast_area_list.cpp \
    kl_url/kl_category_broadcast.cpp \
    kl_url/kl_category_sub_list.cpp \
    kl_url/kl_broadcast_item_list.cpp \
    kl_url/kl_broadcast_item_detail.cpp \
    net_util/cJSON.cpp \
    kl_url/kl_broadcast_item_programlist.cpp \
    kl_url/kl_category_all.cpp \
    kl_url/kl_operate_list.cpp \
    kl_url/kl_type_radio_list.cpp \
    kl_url/kl_album_list.cpp \
    kl_url/kl_album_detail.cpp \
    kl_url/kl_chip_radio_list.cpp \
    kl_url/kl_chip_radio_detail.cpp \
    kl_url/kl_chip_audio_detail.cpp \
    kl_url/kl_chip_audio_list.cpp