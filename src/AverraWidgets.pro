TEMPLATE = lib
TARGET = AverraWidgets
VERSION = 1.0.1

AVERRA_BUILD_ROOT = $$clean_path($$PWD/../build/qmake)
AVERRA_LIB_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/lib)
AVERRA_OBJ_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/src/obj)
AVERRA_MOC_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/src/moc)
AVERRA_RCC_OUTPUT_DIR = $$clean_path($$AVERRA_BUILD_ROOT/src/rcc)

QT += widgets
CONFIG += c++17 shared warn_on

DEFINES += AVERRA_BUILD_LIBRARY

INCLUDEPATH += \
    $$PWD \
    $$PWD/../include

DEPENDPATH += \
    $$PWD \
    $$PWD/../include

HEADERS += \
    $$PWD/../include/Averra/AverraGlobal.h \
    $$PWD/../include/Averra/AverraVersion.h \
    $$PWD/../include/Averra/AverraTheme.h \
    $$PWD/../include/Averra/AverraStyleProfile.h \
    $$PWD/../include/Averra/AverraThemeManager.h \
    $$PWD/../include/Averra/AverraFieldDefinition.h \
    $$PWD/../include/Averra/AverraValidationResult.h \
    $$PWD/../include/Averra/AverraFormModel.h \
    $$PWD/../include/Averra/AverraIcon.h \
    $$PWD/../include/Averra/AverraButton.h \
    $$PWD/../include/Averra/AverraLineEdit.h \
    $$PWD/../include/Averra/AverraComboBox.h \
    $$PWD/../include/Averra/AverraNumberBox.h \
    $$PWD/../include/Averra/AverraDateEdit.h \
    $$PWD/../include/Averra/AverraCard.h \
    $$PWD/../include/Averra/AverraInfoBar.h \
    $$PWD/../include/Averra/AverraNavigationPanel.h \
    $$PWD/../include/Averra/AverraSwitch.h \
    $$PWD/../include/Averra/AverraRadioButton.h \
    $$PWD/../include/Averra/AverraTag.h \
    $$PWD/../include/Averra/AverraProgressBar.h \
    $$PWD/../include/Averra/AverraCheckBox.h \
    $$PWD/../include/Averra/AverraSlider.h \
    $$PWD/../include/Averra/AverraSegmentedControl.h \
    $$PWD/../include/Averra/AverraTabs.h \
    $$PWD/../include/Averra/AverraSectionHeader.h \
    $$PWD/../include/Averra/AverraDivider.h \
    $$PWD/../include/Averra/AverraTextArea.h \
    $$PWD/../include/Averra/AverraEmptyState.h \
    $$PWD/../include/Averra/AverraStatisticCard.h \
    $$PWD/../include/Averra/AverraLoadingRing.h \
    $$PWD/../include/Averra/AverraSkeleton.h \
    $$PWD/../include/Averra/AverraDialog.h \
    $$PWD/../include/Averra/AverraToast.h \
    $$PWD/../include/Averra/AverraDialogManager.h \
    $$PWD/../include/Averra/AverraToastManager.h \
    $$PWD/../include/Averra/AverraBadgeButton.h \
    $$PWD/../include/Averra/AverraSearchBar.h \
    $$PWD/../include/Averra/AverraToolbar.h \
    $$PWD/../include/Averra/AverraRibbonBar.h \
    $$PWD/../include/Averra/AverraFilterChipGroup.h \
    $$PWD/../include/Averra/AverraPagination.h \
    $$PWD/../include/Averra/AverraPropertyItem.h \
    $$PWD/../include/Averra/AverraPropertyGrid.h \
    $$PWD/../include/Averra/AverraFormSection.h \
    $$PWD/../include/Averra/AverraSettingsPage.h \
    $$PWD/../include/Averra/AverraPropertyEditor.h \
    $$PWD/../include/Averra/AverraInspectorPanel.h \
    $$PWD/../include/Averra/AverraDataTable.h \
    $$PWD/../include/Averra/AverraTreeView.h \
    $$PWD/../include/Averra/AverraTreeTable.h \
    $$PWD/../include/Averra/AverraDock.h \
    $$PWD/../include/Averra/AverraDrawer.h \
    $$PWD/../include/Averra/AverraContextMenu.h \
    $$PWD/../include/Averra/AverraNumberRangeBox.h \
    $$PWD/core/AverraStyleHelper.h

SOURCES += \
    $$PWD/core/AverraTheme.cpp \
    $$PWD/core/AverraStyleProfile.cpp \
    $$PWD/core/AverraThemeManager.cpp \
    $$PWD/core/AverraFieldDefinition.cpp \
    $$PWD/core/AverraValidationResult.cpp \
    $$PWD/core/AverraFormModel.cpp \
    $$PWD/core/AverraIcon.cpp \
    $$PWD/core/AverraStyleHelper.cpp \
    $$PWD/widgets/AverraButton.cpp \
    $$PWD/widgets/AverraLineEdit.cpp \
    $$PWD/widgets/AverraComboBox.cpp \
    $$PWD/widgets/AverraNumberBox.cpp \
    $$PWD/widgets/AverraDateEdit.cpp \
    $$PWD/widgets/AverraCard.cpp \
    $$PWD/widgets/AverraInfoBar.cpp \
    $$PWD/widgets/AverraNavigationPanel.cpp \
    $$PWD/widgets/AverraSwitch.cpp \
    $$PWD/widgets/AverraRadioButton.cpp \
    $$PWD/widgets/AverraTag.cpp \
    $$PWD/widgets/AverraProgressBar.cpp \
    $$PWD/widgets/AverraCheckBox.cpp \
    $$PWD/widgets/AverraSlider.cpp \
    $$PWD/widgets/AverraSegmentedControl.cpp \
    $$PWD/widgets/AverraTabs.cpp \
    $$PWD/widgets/AverraSectionHeader.cpp \
    $$PWD/widgets/AverraDivider.cpp \
    $$PWD/widgets/AverraTextArea.cpp \
    $$PWD/widgets/AverraEmptyState.cpp \
    $$PWD/widgets/AverraStatisticCard.cpp \
    $$PWD/widgets/AverraLoadingRing.cpp \
    $$PWD/widgets/AverraSkeleton.cpp \
    $$PWD/widgets/AverraDialog.cpp \
    $$PWD/widgets/AverraToast.cpp \
    $$PWD/widgets/AverraDialogManager.cpp \
    $$PWD/widgets/AverraToastManager.cpp \
    $$PWD/widgets/AverraBadgeButton.cpp \
    $$PWD/widgets/AverraSearchBar.cpp \
    $$PWD/widgets/AverraToolbar.cpp \
    $$PWD/widgets/AverraRibbonBar.cpp \
    $$PWD/widgets/AverraFilterChipGroup.cpp \
    $$PWD/widgets/AverraPagination.cpp \
    $$PWD/widgets/AverraPropertyItem.cpp \
    $$PWD/widgets/AverraPropertyGrid.cpp \
    $$PWD/widgets/AverraFormSection.cpp \
    $$PWD/widgets/AverraSettingsPage.cpp \
    $$PWD/widgets/AverraPropertyEditor.cpp \
    $$PWD/widgets/AverraInspectorPanel.cpp \
    $$PWD/widgets/AverraDataTable.cpp \
    $$PWD/widgets/AverraTreeView.cpp \
    $$PWD/widgets/AverraTreeTable.cpp \
    $$PWD/widgets/AverraDock.cpp \
    $$PWD/widgets/AverraDrawer.cpp \
    $$PWD/widgets/AverraContextMenu.cpp \
    $$PWD/widgets/AverraNumberRangeBox.cpp

RESOURCES += \
    $$PWD/../resources/averra_resources.qrc

DESTDIR = $$AVERRA_LIB_OUTPUT_DIR
OBJECTS_DIR = $$AVERRA_OBJ_OUTPUT_DIR
MOC_DIR = $$AVERRA_MOC_OUTPUT_DIR
RCC_DIR = $$AVERRA_RCC_OUTPUT_DIR

target.path = $$DESTDIR
headers.path = $$PWD/../include
headers.files = $$files($$PWD/../include/Averra/*.h)

INSTALLS += target headers
