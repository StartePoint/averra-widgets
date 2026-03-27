#include <Averra/AverraButton.h>
#include <Averra/AverraRibbonBar.h>
#include <Averra/AverraThemeManager.h>

#include <QPushButton>
#include <QVBoxLayout>

#include <QtTest>

class TestAverraRibbonBar : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleAndSubtitle();
    void shouldAllowAddingPagesAndGroups();
    void shouldSwitchCurrentPage();
    void shouldRefreshTabStyleAfterThemeChange();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraRibbonBar::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraRibbonBar::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraRibbonBar::shouldStoreTitleAndSubtitle()
{
    AverraRibbonBar ribbon;

    ribbon.setTitle(QStringLiteral("项目工具区"));
    ribbon.setSubtitle(QStringLiteral("用于组织主操作和工具分组。"));

    QCOMPARE(ribbon.title(), QStringLiteral("项目工具区"));
    QCOMPARE(ribbon.subtitle(), QStringLiteral("用于组织主操作和工具分组。"));
}

void TestAverraRibbonBar::shouldAllowAddingPagesAndGroups()
{
    AverraRibbonBar ribbon;
    const int pageIndex = ribbon.addPage(QStringLiteral("开始"));
    const int groupIndex = ribbon.addGroup(pageIndex, QStringLiteral("剪贴板"));

    ribbon.addGroupWidget(pageIndex, groupIndex, new AverraButton(QStringLiteral("粘贴")));

    QCOMPARE(ribbon.pageCount(), 1);
    QVERIFY(ribbon.pageWidget(pageIndex) != nullptr);
    QVERIFY(ribbon.groupLayout(pageIndex, groupIndex) != nullptr);
    QCOMPARE(ribbon.groupLayout(pageIndex, groupIndex)->count(), 1);
}

void TestAverraRibbonBar::shouldSwitchCurrentPage()
{
    AverraRibbonBar ribbon;
    ribbon.addPage(QStringLiteral("开始"));
    ribbon.addPage(QStringLiteral("视图"));

    QCOMPARE(ribbon.currentPage(), 0);

    ribbon.setCurrentPage(1);

    QCOMPARE(ribbon.currentPage(), 1);
}

void TestAverraRibbonBar::shouldRefreshTabStyleAfterThemeChange()
{
    AverraRibbonBar ribbon;
    ribbon.addPage(QStringLiteral("开始"));
    ribbon.addPage(QStringLiteral("视图"));
    ribbon.setCurrentPage(1);

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentColor(QColor(QStringLiteral("#0F766E")));
    palette.setAccentSurfaceColor(QColor(QStringLiteral("#E6FFFB")));
    AverraThemeManager::instance()->setPalette(palette);

    const QList<QPushButton *> buttons = ribbon.findChildren<QPushButton *>();
    QVERIFY(!buttons.isEmpty());
    QVERIFY(buttons.last()->styleSheet().contains(QStringLiteral("#0f766e"), Qt::CaseInsensitive));
}

QObject *createTestAverraRibbonBar()
{
    return new TestAverraRibbonBar;
}

#include "tst_AverraRibbonBar.moc"
