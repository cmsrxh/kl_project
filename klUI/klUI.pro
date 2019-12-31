TEMPLATE = app

QT += qml quick
CONFIG += c++11

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build

HOME     = $$getenv("HOME")
PLATFORM = pc
INCLUDEPATH += .
INCLUDEPATH += ../player
INCLUDEPATH += ../klApi
INCLUDEPATH += $$HOME/framework/$$PLATFORM/include

QMAKE_CXXFLAGS += -g -std=c++11
QMAKE_LFLAGS   += -Wl,-rpath,$$HOME/framework/$$PLATFORM/lib

LIBS += -L$$HOME/framework/$$PLATFORM/lib
LIBS += -levents_core -levents_log -levents_util -lpthread

# Input
HEADERS += application.h \
    kl_ui_proc.h \
    model/cate_item_model.h \
    model/kl_ui_data_union.h \
    model/category_model.h \
    model/cate_item_union.h \
    model/category_union.h \
    model/kl_data_proc.h \
    qml_view_switch_stack.h \
    model/chip_item_model.h \
    model/chip_item_union.h \
    model/detail_union.h \
    model/detail_qobject.h \
    model/chip_item_play_manage.h

SOURCES += application.cpp main.cpp \
    kl_ui_proc.cpp \
    model/cate_item_model.cpp \
    model/category_model.cpp \
    model/cate_item_union.cpp \
    model/category_union.cpp \
    model/kl_data_proc.cpp \
    qml_view_switch_stack.cpp \
    model/chip_item_model.cpp \
    model/chip_item_union.cpp \
    model/detail_union.cpp \
    model/detail_qobject.cpp \
    model/chip_item_play_manage.cpp

RESOURCES += res/qml.qrc \
    res/image.qrc

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

include(../klApi/klApi.pri)
include(../player/mpvPlayer.pri)
