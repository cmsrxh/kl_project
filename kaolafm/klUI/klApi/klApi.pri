HOME     = $$getenv("HOME")

INCLUDEPATH += klApi

HEADERS += \    
    $$PWD/kl_url/kl_active_manage.h \
    $$PWD/kl_url/kl_init_manage.h \
    $$PWD/kl_url/kl_object.h \
    $$PWD/kl_url/kl_broadcast_area_list.h \
    $$PWD/kl_url/kl_category_broadcast.h \
    $$PWD/kl_url/kl_category_sub_list.h \
    $$PWD/kl_url/kl_common.h \
    $$PWD/kl_url/kl_save_object.h \
    $$PWD/kl_url/kl_broadcast_item_list.h \
    $$PWD/kl_url/kl_broadcast_item_detail.h \
    $$PWD/kl_url/kl_item_object.h \
    $$PWD/kl_url/kl_broadcast_item_programlist.h \
    $$PWD/kl_url/kl_category_all.h \
    $$PWD/kl_url/kl_operate_list.h \
    $$PWD/kl_url/kl_type_radio_list.h \
    $$PWD/kl_url/kl_album_list.h \
    $$PWD/kl_url/kl_album_detail.h \
    $$PWD/kl_url/kl_chip_radio_list.h \
    $$PWD/kl_url/kl_chip_radio_detail.h \
    $$PWD/kl_url/kl_chip_audio_detail.h \
    $$PWD/kl_url/kl_chip_audio_list.h \
    $$PWD/kl_url/kl_res_voice_search_all.h \
    $$PWD/kl_url/kl_res_suggestion_word.h \
    $$PWD/config_local_info.h \
    $$PWD/kl_record_manage.h \
    $$PWD/kl_collect_manage.h \
    $$PWD/save_manage_object.h \
    $$PWD/kl_save_record.h \
    $$PWD/kl_download_manage.h \    
    $$PWD/kl_search_manage.h \
    $$PWD/kl_ip_positioning.h

SOURCES += \    
    $$PWD/kl_url/kl_active_manage.cpp \
    $$PWD/kl_url/kl_init_manage.cpp \
    $$PWD/kl_url/kl_object.cpp \
    $$PWD/kl_url/kl_broadcast_area_list.cpp \
    $$PWD/kl_url/kl_category_broadcast.cpp \
    $$PWD/kl_url/kl_category_sub_list.cpp \
    $$PWD/kl_url/kl_broadcast_item_list.cpp \
    $$PWD/kl_url/kl_broadcast_item_detail.cpp \
    $$PWD/kl_url/kl_broadcast_item_programlist.cpp \
    $$PWD/kl_url/kl_category_all.cpp \
    $$PWD/kl_url/kl_operate_list.cpp \
    $$PWD/kl_url/kl_type_radio_list.cpp \
    $$PWD/kl_url/kl_album_list.cpp \
    $$PWD/kl_url/kl_album_detail.cpp \
    $$PWD/kl_url/kl_chip_radio_list.cpp \
    $$PWD/kl_url/kl_chip_radio_detail.cpp \
    $$PWD/kl_url/kl_chip_audio_detail.cpp \
    $$PWD/kl_url/kl_chip_audio_list.cpp \
    $$PWD/kl_url/kl_res_voice_search_all.cpp \
    $$PWD/kl_url/kl_res_suggestion_word.cpp \
    $$PWD/kl_collect_manage.cpp \
    $$PWD/kl_record_manage.cpp \
    $$PWD/kl_save_record.cpp \
    $$PWD/kl_download_manage.cpp \    
    $$PWD/kl_search_manage.cpp \
    $$PWD/kl_ip_positioning.cpp
