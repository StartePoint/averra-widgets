#include <Averra/AverraNavigationPanel.h>
#include <Averra/AverraThemeManager.h>

#include <QPushButton>

#include <QtTest>

class TestAverraNavigationPanel : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldAddItemsAndExposeCurrentItem();
    void shouldEmitCurrentIndexChangedWhenSelectionChanges();
    void shouldClearItemsAndResetSelection();
    void shouldRefreshSelectedStyleAfterThemeChange();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraNavigationPanel::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraNavigationPanel::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraNavigationPanel::shouldAddItemsAndExposeCurrentItem()
{
    AverraNavigationPanel panel;

    panel.addItem(QStringLiteral("概览"));
    panel.addItem(QStringLiteral("组件"));
    panel.addItem(QStringLiteral("主题"));

    QCOMPARE(panel.itemCount(), 3);
    QCOMPARE(panel.currentIndex(), 0);
    QCOMPARE(panel.itemText(1), QStringLiteral("组件"));
}

void TestAverraNavigationPanel::shouldEmitCurrentIndexChangedWhenSelectionChanges()
{
    AverraNavigationPanel panel;
    QSignalSpy spy(&panel, &AverraNavigationPanel::currentIndexChanged);

    panel.addItem(QStringLiteral("概览"));
    panel.addItem(QStringLiteral("组件"));
    panel.setCurrentIndex(1);

    QCOMPARE(panel.currentIndex(), 1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 1);
}

void TestAverraNavigationPanel::shouldClearItemsAndResetSelection()
{
    AverraNavigationPanel panel;
    panel.addItem(QStringLiteral("概览"));
    panel.addItem(QStringLiteral("组件"));

    panel.clearItems();

    QCOMPARE(panel.itemCount(), 0);
    QCOMPARE(panel.currentIndex(), -1);
}

void TestAverraNavigationPanel::shouldRefreshSelectedStyleAfterThemeChange()
{
    AverraNavigationPanel panel;
    panel.addItem(QStringLiteral("概览"));
    panel.addItem(QStringLiteral("组件"));
    panel.setCurrentIndex(1);

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentColor(QColor(QStringLiteral("#7C3AED")));
    palette.setAccentSurfaceColor(QColor(QStringLiteral("#F5F3FF")));
    AverraThemeManager::instance()->setPalette(palette);

    QPushButton *button = panel.findChild<QPushButton *>(QStringLiteral("AverraNavigationItemButton_1"));
    QVERIFY(button != nullptr);
    QVERIFY(button->styleSheet().contains(QStringLiteral("#7c3aed"), Qt::CaseInsensitive));
}

QObject *createTestAverraNavigationPanel()
{
    return new TestAverraNavigationPanel;
}

#include "tst_AverraNavigationPanel.moc"
