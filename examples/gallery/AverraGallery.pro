TEMPLATE = app
TARGET = AverraGallery

AVERRA_BUILD_ROOT = $$clean_path($$PWD/../../build/qmake)
AVERRA_BIN_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/bin)
AVERRA_OBJ_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/examples/gallery/obj)
AVERRA_MOC_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/examples/gallery/moc)
AVERRA_RCC_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/examples/gallery/rcc)

QT += widgets
CONFIG += c++17 warn_on

INCLUDEPATH += \
    $$PWD \
    $$PWD/../../include

DEPENDPATH += \
    $$PWD \
    $$PWD/../../include

include(../../AverraWidgets.pri)

HEADERS += \
    $$PWD/GalleryWindow.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/GalleryWindow.cpp

DESTDIR = $$AVERRA_BIN_OUTPUT_DIR
OBJECTS_DIR = $$AVERRA_OBJ_OUTPUT_DIR
MOC_DIR = $$AVERRA_MOC_OUTPUT_DIR
RCC_DIR = $$AVERRA_RCC_OUTPUT_DIR
