HOME     = $$getenv("HOME")
APISRC   = ../klApi

INCLUDEPATH += ../klApi
LIBS += -lcurl -lssl -lcrypto

HEADERS += \
    $$APISRC/net_util/curl_load_proc.h \
    $$APISRC/net_util/curl_load_item.h \
    $$APISRC/net_util/load_item.h \
    $$APISRC/net_util/http_util.h \
    $$APISRC/net_util/net_url.h \
    $$APISRC/kl_url/kl_object.h \
    $$APISRC/kl_url/kl_image.h \
    $$APISRC/net_util/net_common.h \
    $$APISRC/net_util/cJSON.h


SOURCES += \
    $$APISRC/net_util/curl_load_proc.cpp \
    $$APISRC/net_util/curl_load_item.cpp \
    $$APISRC/net_util/load_item.cpp \
    $$APISRC/net_util/http_util.cpp \
    $$APISRC/net_util/net_url.cpp \
    $$APISRC/kl_url/kl_object.cpp \
    $$APISRC/kl_url/kl_image.cpp \
    $$APISRC/net_util/cJSON.cpp
