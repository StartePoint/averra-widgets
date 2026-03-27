AVERRA_ROOT_DIR = $$clean_path($$PWD)

isEmpty(AVERRA_INCLUDE_DIR) {
    AVERRA_INCLUDE_DIR = $$clean_path($$AVERRA_ROOT_DIR/include)
}

isEmpty(AVERRA_LIB_DIR) {
    exists($$AVERRA_ROOT_DIR/lib) {
        AVERRA_LIB_DIR = $$clean_path($$AVERRA_ROOT_DIR/lib)
    } else {
        AVERRA_LIB_DIR = $$clean_path($$AVERRA_ROOT_DIR/build/qmake/lib)
    }
}

INCLUDEPATH += $$AVERRA_INCLUDE_DIR
DEPENDPATH += $$AVERRA_INCLUDE_DIR

unix:!macx {
    LIBS += -L$$AVERRA_LIB_DIR -lAverraWidgets
    QMAKE_RPATHDIR += $$AVERRA_LIB_DIR
}

macx {
    LIBS += -L$$AVERRA_LIB_DIR -lAverraWidgets
    QMAKE_RPATHDIR += $$AVERRA_LIB_DIR
}

win32:CONFIG(release, debug|release) {
    LIBS += -L$$AVERRA_LIB_DIR -lAverraWidgets
}

win32:CONFIG(debug, debug|release) {
    LIBS += -L$$AVERRA_LIB_DIR -lAverraWidgets
}
