#include <Averra/AverraLoadingRing.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraLoadingRing : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseStoppedStateByDefault();
    void shouldEmitRunningChangedWhenStarted();
    void shouldEmitDiameterChangedWhenDiameterUpdates();
    void shouldStopWhenRequested();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraLoadingRing::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraLoadingRing::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraLoadingRing::shouldUseStoppedStateByDefault()
{
    AverraLoadingRing ring;

    QCOMPARE(ring.running(), false);
    QCOMPARE(ring.diameter(), 24);
}

void TestAverraLoadingRing::shouldEmitRunningChangedWhenStarted()
{
    AverraLoadingRing ring;
    QSignalSpy spy(&ring, &AverraLoadingRing::runningChanged);

    ring.start();

    QCOMPARE(ring.running(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestAverraLoadingRing::shouldEmitDiameterChangedWhenDiameterUpdates()
{
    AverraLoadingRing ring;
    QSignalSpy spy(&ring, &AverraLoadingRing::diameterChanged);

    ring.setDiameter(20);

    QCOMPARE(ring.diameter(), 20);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 20);
}

void TestAverraLoadingRing::shouldStopWhenRequested()
{
    AverraLoadingRing ring;
    ring.start();

    ring.stop();

    QCOMPARE(ring.running(), false);
}

QObject *createTestAverraLoadingRing()
{
    return new TestAverraLoadingRing;
}

#include "tst_AverraLoadingRing.moc"
