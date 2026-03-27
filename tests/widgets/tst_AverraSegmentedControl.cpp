#include <Averra/AverraSegmentedControl.h>
#include <Averra/AverraThemeManager.h>

#include <QPushButton>

#include <QtTest>

class TestAverraSegmentedControl : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldAddSegmentsAndSwitchCurrentIndex();
    void shouldUseThemeStateColorsForSegments();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraSegmentedControl::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSegmentedControl::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSegmentedControl::shouldAddSegmentsAndSwitchCurrentIndex()
{
    AverraSegmentedControl control;
    control.addSegment(QStringLiteral("全部"));
    control.addSegment(QStringLiteral("进行中"));
    control.addSegment(QStringLiteral("已完成"));

    QCOMPARE(control.count(), 3);
    QCOMPARE(control.currentIndex(), 0);

    control.setCurrentIndex(2);
    QCOMPARE(control.currentIndex(), 2);
}

void TestAverraSegmentedControl::shouldUseThemeStateColorsForSegments()
{
    AverraSegmentedControl control;
    control.addSegment(QStringLiteral("全部"));
    control.addSegment(QStringLiteral("进行中"));

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentPressedColor(QColor(QStringLiteral("#1E56CB")));
    AverraThemeManager::instance()->setPalette(palette);

    const QList<QPushButton *> buttons = control.findChildren<QPushButton *>();
    QVERIFY(!buttons.isEmpty());
    QVERIFY(buttons.first()->styleSheet().contains(QStringLiteral("#1e56cb"), Qt::CaseInsensitive));
}

QObject *createTestAverraSegmentedControl()
{
    return new TestAverraSegmentedControl;
}

#include "tst_AverraSegmentedControl.moc"
