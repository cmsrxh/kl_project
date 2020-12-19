LIBS += -levents_ipc_client -levents_ipc_service  -levents_socket

##DEFINES += IFACE_CLIENT

HEADERS += \
    $$PWD/media_iface_ipc_socket_priv.h

contains (DEFINES, IFACE_CLIENT) {
SOURCES += $$PWD/media_client_proxy.cpp
} else {
SOURCES += $$PWD/media_service_proxy.cpp
}

