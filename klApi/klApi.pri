HOME     = $$getenv("HOME")
APISRC   = ../klApi

INCLUDEPATH += ../klApi
LIBS += -lcurl -lssl -lcrypto

HEADERS += \
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
    $$PWD/kl_collect_manage.h \
    $$PWD/kl_url/kl_res_voice_search_all.h \
    $$PWD/kl_url/kl_res_suggestion_word.h \
    $$PWD/kl_record_manage.h \
    $$PWD/save_manage_object.h \
    $$PWD/kl_save_record.h \
    $$PWD/kl_download_manage.h

SOURCES += \
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
    $$PWD/kl_collect_manage.cpp \
    $$PWD/kl_url/kl_res_voice_search_all.cpp \
    $$PWD/kl_url/kl_res_suggestion_word.cpp \
    $$PWD/kl_record_manage.cpp \
    $$PWD/kl_save_record.cpp \
    $$PWD/kl_download_manage.cpp
