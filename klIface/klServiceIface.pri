
IFACE_DIR = ../klIface/
LIBS += -levents_ipc_service -levents_socket

# Input
SOURCES += \
    $${IFACE_DIR}kl_service_notify.cpp

HEADERS += \
    $${IFACE_DIR}kl_iface_common.h \
    $${IFACE_DIR}kl_service_notify.h
