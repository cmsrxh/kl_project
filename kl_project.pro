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
LIBS += -lcurl


# Input
HEADERS += app_common.h application.h \ 
    net_util/curl_load_proc.h \
    net_util/curl_load_item.h \
    net_util/load_item.h \
    net_util/http_util.h


SOURCES += application.cpp main.cpp \ 
    net_util/curl_load_proc.cpp \
    net_util/curl_load_item.cpp \
    net_util/load_item.cpp \
    net_util/http_util.cpp
