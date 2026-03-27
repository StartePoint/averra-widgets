#include <QApplication>
#include <QObject>
#include <QtTest>

QObject *createTestAverraBadgeButton();
QObject *createTestAverraButton();
QObject *createTestAverraCard();
QObject *createTestAverraCheckBox();
QObject *createTestAverraComboBox();
QObject *createTestAverraContextMenu();
QObject *createTestAverraDataTable();
QObject *createTestAverraDateEdit();
QObject *createTestAverraDialog();
QObject *createTestAverraDialogManager();
QObject *createTestAverraDivider();
QObject *createTestAverraDock();
QObject *createTestAverraDrawer();
QObject *createTestAverraEmptyState();
QObject *createTestAverraFilterChipGroup();
QObject *createTestAverraFormModel();
QObject *createTestAverraFormSection();
QObject *createTestAverraInfoBar();
QObject *createTestAverraInspectorPanel();
QObject *createTestAverraLineEdit();
QObject *createTestAverraLoadingRing();
QObject *createTestAverraNavigationPanel();
QObject *createTestAverraNumberBox();
QObject *createTestAverraNumberRangeBox();
QObject *createTestAverraPagination();
QObject *createTestAverraProgressBar();
QObject *createTestAverraPropertyGrid();
QObject *createTestAverraPropertyEditor();
QObject *createTestAverraPropertyItem();
QObject *createTestAverraRadioButton();
QObject *createTestAverraRibbonBar();
QObject *createTestAverraSearchBar();
QObject *createTestAverraSegmentedControl();
QObject *createTestAverraSectionHeader();
QObject *createTestAverraSettingsPage();
QObject *createTestAverraSkeleton();
QObject *createTestAverraSlider();
QObject *createTestAverraStatisticCard();
QObject *createTestAverraStyleProfile();
QObject *createTestAverraSwitch();
QObject *createTestAverraTabs();
QObject *createTestAverraTag();
QObject *createTestAverraTheme();
QObject *createTestAverraThemeManager();
QObject *createTestAverraTextArea();
QObject *createTestAverraToast();
QObject *createTestAverraToastManager();
QObject *createTestAverraToolbar();
QObject *createTestAverraTreeTable();
QObject *createTestAverraTreeView();

struct TestEntry
{
    const char *name;
    QObject *(*factory)();
};

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    const TestEntry tests[] = {
        {"AverraBadgeButton", &createTestAverraBadgeButton},
        {"AverraButton", &createTestAverraButton},
        {"AverraCard", &createTestAverraCard},
        {"AverraCheckBox", &createTestAverraCheckBox},
        {"AverraComboBox", &createTestAverraComboBox},
        {"AverraContextMenu", &createTestAverraContextMenu},
        {"AverraDataTable", &createTestAverraDataTable},
        {"AverraDateEdit", &createTestAverraDateEdit},
        {"AverraDialog", &createTestAverraDialog},
        {"AverraDialogManager", &createTestAverraDialogManager},
        {"AverraDivider", &createTestAverraDivider},
        {"AverraDock", &createTestAverraDock},
        {"AverraDrawer", &createTestAverraDrawer},
        {"AverraEmptyState", &createTestAverraEmptyState},
        {"AverraFilterChipGroup", &createTestAverraFilterChipGroup},
        {"AverraFormModel", &createTestAverraFormModel},
        {"AverraFormSection", &createTestAverraFormSection},
        {"AverraInfoBar", &createTestAverraInfoBar},
        {"AverraInspectorPanel", &createTestAverraInspectorPanel},
        {"AverraLineEdit", &createTestAverraLineEdit},
        {"AverraLoadingRing", &createTestAverraLoadingRing},
        {"AverraNavigationPanel", &createTestAverraNavigationPanel},
        {"AverraNumberBox", &createTestAverraNumberBox},
        {"AverraNumberRangeBox", &createTestAverraNumberRangeBox},
        {"AverraPagination", &createTestAverraPagination},
        {"AverraProgressBar", &createTestAverraProgressBar},
        {"AverraPropertyGrid", &createTestAverraPropertyGrid},
        {"AverraPropertyEditor", &createTestAverraPropertyEditor},
        {"AverraPropertyItem", &createTestAverraPropertyItem},
        {"AverraRadioButton", &createTestAverraRadioButton},
        {"AverraRibbonBar", &createTestAverraRibbonBar},
        {"AverraSearchBar", &createTestAverraSearchBar},
        {"AverraSegmentedControl", &createTestAverraSegmentedControl},
        {"AverraSectionHeader", &createTestAverraSectionHeader},
        {"AverraSettingsPage", &createTestAverraSettingsPage},
        {"AverraSkeleton", &createTestAverraSkeleton},
        {"AverraSlider", &createTestAverraSlider},
        {"AverraStatisticCard", &createTestAverraStatisticCard},
        {"AverraStyleProfile", &createTestAverraStyleProfile},
        {"AverraSwitch", &createTestAverraSwitch},
        {"AverraTabs", &createTestAverraTabs},
        {"AverraTag", &createTestAverraTag},
        {"AverraTheme", &createTestAverraTheme},
        {"AverraThemeManager", &createTestAverraThemeManager},
        {"AverraTextArea", &createTestAverraTextArea},
        {"AverraToast", &createTestAverraToast},
        {"AverraToastManager", &createTestAverraToastManager},
        {"AverraToolbar", &createTestAverraToolbar},
        {"AverraTreeTable", &createTestAverraTreeTable},
        {"AverraTreeView", &createTestAverraTreeView}
    };

    int result = 0;

    for (int index = 0; index < static_cast<int>(sizeof(tests) / sizeof(TestEntry)); ++index) {
        QObject *testObject = tests[index].factory();
        result |= QTest::qExec(testObject, argc, argv);
        delete testObject;
    }

    return result;
}
