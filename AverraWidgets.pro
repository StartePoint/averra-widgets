TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    averrawidgets \
    gallery \
    averra_widgets_tests

averrawidgets.file = src/AverraWidgets.pro

gallery.file = examples/gallery/AverraGallery.pro
gallery.depends = averrawidgets

averra_widgets_tests.file = tests/widgets/AverraWidgetsTests.pro
averra_widgets_tests.depends = averrawidgets
