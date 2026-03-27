TEMPLATE = app
TARGET = AverraWidgetsTests

AVERRA_BUILD_ROOT = $$clean_path($$PWD/../../build/qmake)
AVERRA_BIN_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/bin)
AVERRA_OBJ_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/tests/widgets/obj)
AVERRA_MOC_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/tests/widgets/moc)
AVERRA_RCC_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/tests/widgets/rcc)
AVERRA_UI_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/tests/widgets/ui)

QT += testlib widgets
CONFIG += c++17 console testcase warn_on

INCLUDEPATH += \
    $$PWD \
    $$PWD/../../include

DEPENDPATH += \
    $$PWD \
    $$PWD/../../include

include(../../AverraWidgets.pri)

SOURCES += \
    $$PWD/test_main.cpp \
    $$PWD/tst_AverraBadgeButton.cpp \
    $$PWD/tst_AverraButton.cpp \
    $$PWD/tst_AverraCard.cpp \
    $$PWD/tst_AverraCheckBox.cpp \
    $$PWD/tst_AverraComboBox.cpp \
    $$PWD/tst_AverraContextMenu.cpp \
    $$PWD/tst_AverraDataTable.cpp \
    $$PWD/tst_AverraDateEdit.cpp \
    $$PWD/tst_AverraDialog.cpp \
    $$PWD/tst_AverraDialogManager.cpp \
    $$PWD/tst_AverraDivider.cpp \
    $$PWD/tst_AverraDock.cpp \
    $$PWD/tst_AverraDrawer.cpp \
    $$PWD/tst_AverraEmptyState.cpp \
    $$PWD/tst_AverraFilterChipGroup.cpp \
    $$PWD/tst_AverraFormModel.cpp \
    $$PWD/tst_AverraFormSection.cpp \
    $$PWD/tst_AverraInfoBar.cpp \
    $$PWD/tst_AverraInspectorPanel.cpp \
    $$PWD/tst_AverraLineEdit.cpp \
    $$PWD/tst_AverraLoadingRing.cpp \
    $$PWD/tst_AverraNavigationPanel.cpp \
    $$PWD/tst_AverraNumberBox.cpp \
    $$PWD/tst_AverraNumberRangeBox.cpp \
    $$PWD/tst_AverraPagination.cpp \
    $$PWD/tst_AverraProgressBar.cpp \
    $$PWD/tst_AverraPropertyGrid.cpp \
    $$PWD/tst_AverraPropertyEditor.cpp \
    $$PWD/tst_AverraPropertyItem.cpp \
    $$PWD/tst_AverraRadioButton.cpp \
    $$PWD/tst_AverraRibbonBar.cpp \
    $$PWD/tst_AverraSearchBar.cpp \
    $$PWD/tst_AverraSegmentedControl.cpp \
    $$PWD/tst_AverraSectionHeader.cpp \
    $$PWD/tst_AverraSettingsPage.cpp \
    $$PWD/tst_AverraSkeleton.cpp \
    $$PWD/tst_AverraSlider.cpp \
    $$PWD/tst_AverraStatisticCard.cpp \
    $$PWD/tst_AverraSwitch.cpp \
    $$PWD/tst_AverraTabs.cpp \
    $$PWD/tst_AverraTag.cpp \
    $$PWD/tst_AverraTheme.cpp \
    $$PWD/tst_AverraThemeManager.cpp \
    $$PWD/tst_AverraTextArea.cpp \
    $$PWD/tst_AverraToast.cpp \
    $$PWD/tst_AverraToastManager.cpp \
    $$PWD/tst_AverraToolbar.cpp \
    $$PWD/tst_AverraTreeTable.cpp \
    $$PWD/tst_AverraTreeView.cpp

DESTDIR = $$AVERRA_BIN_OUTPUT_DIR
OBJECTS_DIR = $$AVERRA_OBJ_OUTPUT_DIR
MOC_DIR = $$AVERRA_MOC_OUTPUT_DIR
RCC_DIR = $$AVERRA_RCC_OUTPUT_DIR
UI_DIR = $$AVERRA_UI_OUTPUT_DIR
