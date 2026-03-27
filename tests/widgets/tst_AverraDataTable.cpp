#include <Averra/AverraDataTable.h>
#include <Averra/AverraThemeManager.h>

#include <QPushButton>
#include <QStandardItemModel>
#include <QTableView>

#include <QtTest>

class TestAverraDataTable : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldSetHeadersAndRows();
    void shouldEmitRowActionTriggered();
    void shouldUseThemeStateColorsForRefreshButton();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraDataTable::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraDataTable::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraDataTable::shouldSetHeadersAndRows()
{
    AverraDataTable table;
    table.setRowActionsEnabled(false);
    table.setHeaders(QStringList() << QStringLiteral("组件") << QStringLiteral("类型"));
    table.addRow(QStringList() << QStringLiteral("Button") << QStringLiteral("输入"));

    QCOMPARE(table.model()->columnCount(), 2);
    QCOMPARE(table.model()->rowCount(), 1);
}

void TestAverraDataTable::shouldEmitRowActionTriggered()
{
    AverraDataTable table;
    QSignalSpy spy(&table, &AverraDataTable::rowActionTriggered);
    table.setHeaders(QStringList() << QStringLiteral("名称"));
    table.setRowActionsEnabled(true);
    table.addRow(QStringList() << QStringLiteral("Button"));

    QModelIndex index = table.tableView()->model()->index(0, table.tableView()->model()->columnCount() - 1);
    emit table.tableView()->clicked(index);

    QCOMPARE(spy.count(), 1);
}

void TestAverraDataTable::shouldUseThemeStateColorsForRefreshButton()
{
    AverraDataTable table;
    const QList<QPushButton *> buttons = table.findChildren<QPushButton *>();
    QPushButton *refreshButton = nullptr;

    for (int index = 0; index < buttons.size(); ++index) {
        if (buttons.at(index)->text() == QStringLiteral("刷新")) {
            refreshButton = buttons.at(index);
            break;
        }
    }

    QVERIFY(refreshButton != nullptr);

    AverraThemePalette palette = m_defaultPalette;
    palette.setSurfaceHoverColor(QColor(QStringLiteral("#F3F6FA")));
    palette.setSurfacePressedColor(QColor(QStringLiteral("#E6EDF6")));
    palette.setTextDisabledColor(QColor(QStringLiteral("#97A3B5")));
    AverraThemeManager::instance()->setPalette(palette);

    QVERIFY(refreshButton->styleSheet().contains(QStringLiteral("#f3f6fa"), Qt::CaseInsensitive));
    QVERIFY(refreshButton->styleSheet().contains(QStringLiteral("#e6edf6"), Qt::CaseInsensitive));
    QVERIFY(refreshButton->styleSheet().contains(QStringLiteral("#97a3b5"), Qt::CaseInsensitive));
}

QObject *createTestAverraDataTable()
{
    return new TestAverraDataTable;
}

#include "tst_AverraDataTable.moc"
