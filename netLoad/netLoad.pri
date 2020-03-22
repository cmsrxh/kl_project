HOME     = $$getenv("HOME")

INCLUDEPATH += ../netLoad
LIBS += -lcurl -lssl -lcrypto

HEADERS += \
    $$PWD/net_util/curl_load_proc.h \
    $$PWD/net_util/curl_load_item.h \
    $$PWD/net_util/load_item.h \
    $$PWD/net_util/http_util.h \
    $$PWD/net_util/net_url.h \
    $$PWD/net_util/test_data_load.h \
    $$PWD/net_util/net_common.h \
    $$PWD/net_util/cJSON.h \
    $$PWD/common_func/net_image.h \
    $$PWD/common_func/common_func.h \
    $$PWD/common_func/image_cache_manage.h \

SOURCES += \
    $$PWD/net_util/curl_load_proc.cpp \
    $$PWD/net_util/curl_load_item.cpp \
    $$PWD/net_util/load_item.cpp \
    $$PWD/net_util/http_util.cpp \
    $$PWD/net_util/net_url.cpp \
    $$PWD/net_util/test_data_load.cpp \
    $$PWD/net_util/cJSON.cpp \
    $$PWD/common_func/net_image.cpp \
    $$PWD/common_func/image_cache_manage.cpp \

