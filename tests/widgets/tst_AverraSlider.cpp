#include <Averra/AverraSlider.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraSlider : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseDefaultRangeAndValue();
    void shouldEmitValueChangedWhenValueUpdates();
    void shouldClampValueToRange();
    void shouldToggleValueWhenClicked();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraSlider::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSlider::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSlider::shouldUseDefaultRangeAndValue()
{
    AverraSlider slider;

    QCOMPARE(slider.minimum(), 0);
    QCOMPARE(slider.maximum(), 100);
    QCOMPARE(slider.value(), 0);
    QCOMPARE(slider.orientation(), Qt::Horizontal);
}

void TestAverraSlider::shouldEmitValueChangedWhenValueUpdates()
{
    AverraSlider slider;
    QSignalSpy spy(&slider, &AverraSlider::valueChanged);

    slider.setValue(33);

    QCOMPARE(slider.value(), 33);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 33);
}

void TestAverraSlider::shouldClampValueToRange()
{
    AverraSlider slider;
    slider.setRange(10, 20);

    slider.setValue(99);
    QCOMPARE(slider.value(), 20);

    slider.setValue(0);
    QCOMPARE(slider.value(), 10);
}

void TestAverraSlider::shouldToggleValueWhenClicked()
{
    AverraSlider slider;
    slider.resize(slider.sizeHint());
    slider.show();

    QTest::mouseClick(&slider, Qt::LeftButton, Qt::NoModifier, QPoint(slider.width() - 12, slider.height() / 2));

    QVERIFY(slider.value() > 80);
}

QObject *createTestAverraSlider()
{
    return new TestAverraSlider;
}

#include "tst_AverraSlider.moc"
