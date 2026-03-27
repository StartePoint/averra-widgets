#include <Averra/AverraFormSection.h>
#include <Averra/AverraLineEdit.h>
#include <Averra/AverraPropertyEditor.h>
#include <Averra/AverraPropertyItem.h>
#include <Averra/AverraSearchBar.h>
#include <Averra/AverraSwitch.h>
#include <Averra/AverraThemeManager.h>

#include <QVBoxLayout>

#include <QtTest>

class TestAverraPropertyEditor : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleDescriptionAndFlags();
    void shouldAllowAddingAndClearingSections();
    void shouldFilterSectionsAndPropertyItems();
    void shouldDisableSectionsWhenReadOnly();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraPropertyEditor::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraPropertyEditor::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraPropertyEditor::shouldStoreTitleDescriptionAndFlags()
{
    AverraPropertyEditor editor;

    editor.setTitle(QStringLiteral("节点属性"));
    editor.setDescription(QStringLiteral("统一组织节点详情和编辑项。"));
    editor.setSearchEnabled(false);
    editor.setReadOnly(true);

    QCOMPARE(editor.title(), QStringLiteral("节点属性"));
    QCOMPARE(editor.description(), QStringLiteral("统一组织节点详情和编辑项。"));
    QCOMPARE(editor.searchEnabled(), false);
    QCOMPARE(editor.readOnly(), true);
    QVERIFY(editor.searchBar()->isHidden());
}

void TestAverraPropertyEditor::shouldAllowAddingAndClearingSections()
{
    AverraPropertyEditor editor;
    AverraFormSection *section = new AverraFormSection(QStringLiteral("基础信息"));

    editor.addSection(section);

    QCOMPARE(editor.contentLayout()->count(), 1);

    editor.clearSections();

    QCOMPARE(editor.contentLayout()->count(), 0);
}

void TestAverraPropertyEditor::shouldFilterSectionsAndPropertyItems()
{
    AverraPropertyEditor editor;

    AverraFormSection *generalSection = new AverraFormSection(QStringLiteral("基础信息"));
    AverraPropertyItem *nameItem = new AverraPropertyItem(QStringLiteral("节点名称"));
    nameItem->setDescription(QStringLiteral("用于展示当前节点标题。"));
    AverraPropertyItem *timeoutItem = new AverraPropertyItem(QStringLiteral("超时时间"));
    timeoutItem->setDescription(QStringLiteral("控制请求等待时长。"));
    generalSection->addRow(nameItem);
    generalSection->addRow(timeoutItem);

    AverraFormSection *displaySection = new AverraFormSection(QStringLiteral("显示设置"));
    AverraPropertyItem *themeItem = new AverraPropertyItem(QStringLiteral("主题"));
    themeItem->setDescription(QStringLiteral("控制界面显示主题。"));
    displaySection->addRow(themeItem);

    editor.addSection(generalSection);
    editor.addSection(displaySection);

    editor.setFilterText(QStringLiteral("超时"));

    QVERIFY(!generalSection->isHidden());
    QVERIFY(nameItem->isHidden());
    QVERIFY(!timeoutItem->isHidden());
    QVERIFY(displaySection->isHidden());

    editor.searchBar()->setText(QStringLiteral("显示"));

    QVERIFY(generalSection->isHidden());
    QVERIFY(!displaySection->isHidden());
    QVERIFY(!themeItem->isHidden());
}

void TestAverraPropertyEditor::shouldDisableSectionsWhenReadOnly()
{
    AverraPropertyEditor editor;
    AverraFormSection *section = new AverraFormSection(QStringLiteral("编辑"));
    AverraPropertyItem *switchItem = new AverraPropertyItem(QStringLiteral("自动更新"));
    AverraSwitch *switchWidget = new AverraSwitch;
    switchItem->setAccessoryWidget(switchWidget);
    section->addRow(switchItem);
    editor.addSection(section);

    editor.setReadOnly(true);

    QCOMPARE(section->isEnabled(), false);
    QCOMPARE(switchWidget->isEnabled(), false);
    QCOMPARE(editor.searchBar()->isEnabled(), true);

    editor.setReadOnly(false);

    QCOMPARE(section->isEnabled(), true);
    QCOMPARE(switchWidget->isEnabled(), true);
}

QObject *createTestAverraPropertyEditor()
{
    return new TestAverraPropertyEditor;
}

#include "tst_AverraPropertyEditor.moc"
