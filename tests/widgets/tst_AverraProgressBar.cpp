#include <Averra/AverraProgressBar.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraProgressBar : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseDefaultRangeAndValue();
    void shouldEmitValueChangedWhenValueUpdates();
    void shouldClampValueToRange();
    void shouldToggleTextVisibility();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraProgressBar::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraProgressBar::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraProgressBar::shouldUseDefaultRangeAndValue()
{
    AverraProgressBar progressBar;

    QCOMPARE(progressBar.minimum(), 0);
    QCOMPARE(progressBar.maximum(), 100);
    QCOMPARE(progressBar.value(), 0);
    QCOMPARE(progressBar.showText(), true);
}

void TestAverraProgressBar::shouldEmitValueChangedWhenValueUpdates()
{
    AverraProgressBar progressBar;
    QSignalSpy spy(&progressBar, &AverraProgressBar::valueChanged);

    progressBar.setValue(45);

    QCOMPARE(progressBar.value(), 45);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 45);
}

void TestAverraProgressBar::shouldClampValueToRange()
{
    AverraProgressBar progressBar;
    progressBar.setRange(10, 20);

    progressBar.setValue(99);
    QCOMPARE(progressBar.value(), 20);

    progressBar.setValue(0);
    QCOMPARE(progressBar.value(), 10);
}

void TestAverraProgressBar::shouldToggleTextVisibility()
{
    AverraProgressBar progressBar;
    QSignalSpy spy(&progressBar, &AverraProgressBar::showTextChanged);

    progressBar.setShowText(false);

    QCOMPARE(progressBar.showText(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);
}

QObject *createTestAverraProgressBar()
{
    return new TestAverraProgressBar;
}

#include "tst_AverraProgressBar.moc"
