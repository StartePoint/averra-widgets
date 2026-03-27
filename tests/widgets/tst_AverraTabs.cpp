#include <Averra/AverraTabs.h>
#include <Averra/AverraThemeManager.h>

#include <QLabel>
#include <QPushButton>

#include <QtTest>

class TestAverraTabs : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldAddTabsAndSwitchCurrentIndex();
    void shouldUseThemeStateColorsForTabButtons();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraTabs::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraTabs::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraTabs::shouldAddTabsAndSwitchCurrentIndex()
{
    AverraTabs tabs;
    tabs.addTab(new QLabel(QStringLiteral("A")), QStringLiteral("A"));
    tabs.addTab(new QLabel(QStringLiteral("B")), QStringLiteral("B"));

    QCOMPARE(tabs.count(), 2);
    QCOMPARE(tabs.currentIndex(), 0);

    tabs.setCurrentIndex(1);
    QCOMPARE(tabs.currentIndex(), 1);
}

void TestAverraTabs::shouldUseThemeStateColorsForTabButtons()
{
    AverraTabs tabs;
    tabs.addTab(new QLabel(QStringLiteral("A")), QStringLiteral("A"));
    tabs.addTab(new QLabel(QStringLiteral("B")), QStringLiteral("B"));

    AverraThemePalette palette = m_defaultPalette;
    palette.setBorderHoverColor(QColor(QStringLiteral("#8AA1C7")));
    AverraThemeManager::instance()->setPalette(palette);

    const QList<QPushButton *> buttons = tabs.findChildren<QPushButton *>();
    QVERIFY(buttons.size() >= 2);
    QVERIFY(buttons.at(1)->styleSheet().contains(QStringLiteral("#8aa1c7"), Qt::CaseInsensitive));
}

QObject *createTestAverraTabs()
{
    return new TestAverraTabs;
}

#include "tst_AverraTabs.moc"
