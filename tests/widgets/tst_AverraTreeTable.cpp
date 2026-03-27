#include <Averra/AverraEmptyState.h>
#include <Averra/AverraThemeManager.h>
#include <Averra/AverraTreeTable.h>

#include <QCheckBox>
#include <QHeaderView>
#include <QModelIndex>
#include <QSettings>
#include <QSignalSpy>
#include <QStandardItem>
#include <QTemporaryDir>
#include <QTreeView>

#include <QtTest>

class TestAverraTreeTable : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldSetHeadersAndItems();
    void shouldFilterRowsRecursively();
    void shouldToggleSorting();
    void shouldEmitRowActionTriggered();
    void shouldPersistExpandedStateAfterFiltering();
    void shouldShowFilteredEmptyState();
    void shouldToggleColumnConfigAndRestoreViewState();
    void shouldSaveAndRestoreViewStateWithQSettings();
    void shouldExposeSelectionContextWithObjectIdAndRowData();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraTreeTable::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraTreeTable::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraTreeTable::shouldSetHeadersAndItems()
{
    AverraTreeTable treeTable;
    treeTable.setRowActionsEnabled(false);
    treeTable.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型"));

    QStandardItem *root = treeTable.addTopLevelItem(QStringList() << QStringLiteral("输入组件") << QStringLiteral("分类"));
    treeTable.addChildItem(root, QStringList() << QStringLiteral("AverraLineEdit") << QStringLiteral("输入"));

    QCOMPARE(treeTable.model()->columnCount(), 2);
    QCOMPARE(treeTable.model()->rowCount(), 1);
    QCOMPARE(root->rowCount(), 1);
}

void TestAverraTreeTable::shouldFilterRowsRecursively()
{
    AverraTreeTable treeTable;
    treeTable.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型"));

    QStandardItem *inputsRoot = treeTable.addTopLevelItem(QStringList() << QStringLiteral("输入组件") << QStringLiteral("分类"));
    treeTable.addChildItem(inputsRoot, QStringList() << QStringLiteral("AverraLineEdit") << QStringLiteral("输入"));
    treeTable.addTopLevelItem(QStringList() << QStringLiteral("反馈组件") << QStringLiteral("分类"));

    treeTable.setFilterText(QStringLiteral("LineEdit"));

    QVERIFY(!treeTable.treeView()->isRowHidden(inputsRoot->row(), QModelIndex()));
    QVERIFY(treeTable.treeView()->isRowHidden(1, QModelIndex()));
}

void TestAverraTreeTable::shouldToggleSorting()
{
    AverraTreeTable treeTable;

    treeTable.setSortingEnabled(false);
    QCOMPARE(treeTable.sortingEnabled(), false);

    treeTable.setSortingEnabled(true);
    QCOMPARE(treeTable.sortingEnabled(), true);
}

void TestAverraTreeTable::shouldEmitRowActionTriggered()
{
    AverraTreeTable treeTable;
    QSignalSpy spy(&treeTable, &AverraTreeTable::rowActionTriggered);

    treeTable.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型"));
    treeTable.setActionText(QStringLiteral("打开"));
    treeTable.addTopLevelItem(QStringList() << QStringLiteral("Workspace") << QStringLiteral("根节点"));

    QModelIndex index = treeTable.model()->index(0, treeTable.model()->columnCount() - 1);
    emit treeTable.treeView()->clicked(index);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QStringLiteral("Workspace"));
}

void TestAverraTreeTable::shouldPersistExpandedStateAfterFiltering()
{
    AverraTreeTable treeTable;
    treeTable.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型"));

    QStandardItem *root = treeTable.addTopLevelItem(QStringList() << QStringLiteral("Workspace") << QStringLiteral("根节点"));
    treeTable.addChildItem(root, QStringList() << QStringLiteral("AverraLineEdit") << QStringLiteral("输入"));

    treeTable.treeView()->setExpanded(root->index(), true);
    treeTable.setFilterText(QStringLiteral("LineEdit"));
    treeTable.setFilterText(QString());

    QVERIFY(treeTable.treeView()->isExpanded(root->index()));
}

void TestAverraTreeTable::shouldShowFilteredEmptyState()
{
    AverraTreeTable treeTable;
    treeTable.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型"));
    treeTable.addTopLevelItem(QStringList() << QStringLiteral("Workspace") << QStringLiteral("根节点"));

    treeTable.setFilterText(QStringLiteral("NoMatch"));

    QCOMPARE(treeTable.emptyState()->title(), QStringLiteral("没有匹配结果"));
}

void TestAverraTreeTable::shouldToggleColumnConfigAndRestoreViewState()
{
    AverraTreeTable treeTable;
    treeTable.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型") << QStringLiteral("状态"));
    treeTable.setRowActionsEnabled(false);
    treeTable.addTopLevelItem(QStringList() << QStringLiteral("Workspace") << QStringLiteral("根节点") << QStringLiteral("Live"));
    treeTable.treeView()->setColumnWidth(0, 180);
    treeTable.treeView()->setColumnWidth(1, 120);
    treeTable.treeView()->sortByColumn(1, Qt::DescendingOrder);
    treeTable.setColumnConfigVisible(true);

    QVERIFY(treeTable.columnConfigPanel() != nullptr);
    QVERIFY(!treeTable.columnConfigPanel()->isHidden());

    const QList<QCheckBox *> checkBoxes = treeTable.columnConfigPanel()->findChildren<QCheckBox *>();
    QVERIFY(checkBoxes.size() >= 2);
    checkBoxes.at(1)->setChecked(false);
    QVERIFY(treeTable.treeView()->isColumnHidden(1));

    const QString viewState = treeTable.exportViewState();
    QVERIFY(!viewState.isEmpty());

    treeTable.treeView()->setColumnHidden(1, false);
    treeTable.importViewState(viewState);

    QVERIFY(treeTable.treeView()->isColumnHidden(1));
    QCOMPARE(treeTable.treeView()->columnWidth(0), 180);
    QCOMPARE(treeTable.treeView()->header()->sortIndicatorSection(), 1);
    QCOMPARE(treeTable.treeView()->header()->sortIndicatorOrder(), Qt::DescendingOrder);
}

void TestAverraTreeTable::shouldSaveAndRestoreViewStateWithQSettings()
{
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    const QString settingsPath = tempDir.path() + QStringLiteral("/tree-table.ini");
    QSettings settings(settingsPath, QSettings::IniFormat);

    AverraTreeTable sourceTable;
    sourceTable.setSettingsKey(QStringLiteral("workspace/treeTable"));
    sourceTable.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型") << QStringLiteral("状态"));
    sourceTable.setRowActionsEnabled(false);
    QStandardItem *sourceRoot = sourceTable.addTopLevelItem(QStringList() << QStringLiteral("Workspace") << QStringLiteral("根节点") << QStringLiteral("Live"),
                                                            QStringLiteral("workspace-root"));
    sourceTable.treeView()->setColumnWidth(0, 210);
    sourceTable.treeView()->setColumnHidden(2, true);
    sourceTable.treeView()->sortByColumn(1, Qt::DescendingOrder);
    sourceTable.treeView()->setExpanded(sourceRoot->index(), true);
    sourceTable.setFilterText(QStringLiteral("Workspace"));
    sourceTable.treeView()->setCurrentIndex(sourceRoot->index());

    QVERIFY(sourceTable.saveViewState(settings));

    AverraTreeTable restoredTable;
    restoredTable.setSettingsKey(QStringLiteral("workspace/treeTable"));
    restoredTable.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型") << QStringLiteral("状态"));
    restoredTable.setRowActionsEnabled(false);
    QStandardItem *restoredRoot = restoredTable.addTopLevelItem(QStringList() << QStringLiteral("Workspace") << QStringLiteral("根节点") << QStringLiteral("Live"),
                                                                QStringLiteral("workspace-root"));

    QVERIFY(restoredTable.restoreViewState(settings));
    QCOMPARE(restoredTable.treeView()->columnWidth(0), 210);
    QVERIFY(restoredTable.treeView()->isColumnHidden(2));
    QCOMPARE(restoredTable.treeView()->header()->sortIndicatorSection(), 1);
    QCOMPARE(restoredTable.treeView()->header()->sortIndicatorOrder(), Qt::DescendingOrder);
    QCOMPARE(restoredTable.filterText(), QStringLiteral("Workspace"));
    QVERIFY(restoredTable.treeView()->isExpanded(restoredRoot->index()));
    QCOMPARE(restoredTable.currentObjectId(), QStringLiteral("workspace-root"));
}

void TestAverraTreeTable::shouldExposeSelectionContextWithObjectIdAndRowData()
{
    AverraTreeTable treeTable;
    QSignalSpy spy(&treeTable, SIGNAL(selectionContextChanged(QString,QVariantMap,QString)));

    treeTable.setRowActionsEnabled(false);
    treeTable.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型") << QStringLiteral("状态"));

    QStandardItem *root = treeTable.addTopLevelItem(QStringList() << QStringLiteral("Workspace") << QStringLiteral("根节点") << QStringLiteral("Live"),
                                                    QStringLiteral("workspace-root"),
                                                    QVariantMap{{QStringLiteral("summary"), QStringLiteral("根节点摘要")}});

    treeTable.treeView()->setCurrentIndex(root->index());

    QCOMPARE(spy.count(), 1);
    QCOMPARE(treeTable.currentItemPath(), QStringLiteral("Workspace"));
    QCOMPARE(treeTable.currentObjectId(), QStringLiteral("workspace-root"));
    QCOMPARE(treeTable.currentRowData().value(QStringLiteral("名称")).toString(), QStringLiteral("Workspace"));
    QCOMPARE(treeTable.currentContextData().value(QStringLiteral("summary")).toString(), QStringLiteral("根节点摘要"));
}

QObject *createTestAverraTreeTable()
{
    return new TestAverraTreeTable;
}

#include "tst_AverraTreeTable.moc"
