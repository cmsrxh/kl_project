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


# qmake "INS_INC_DIR=/usr/include" "INS_LIB_DIR=/usr/lib"
# 安装头文件
# 命令行传递参数, qmake "INS_INC_DIR=/usr/include"
!isEmpty (INS_INC_DIR) {
header.path  = $${INS_INC_DIR}
header.files = \
    $$PWD/net_util/load_item.h \
    $$PWD/net_util/http_util.h \
    $$PWD/net_util/net_url.h \
    $$PWD/net_util/net_common.h \
    $$PWD/common_func/net_image.h \
    $$PWD/common_func/common_func.h \
    $$PWD/common_func/image_cache_manage.h \

INSTALLS += header
message("install header file dir: $${INS_INC_DIR}")
}
