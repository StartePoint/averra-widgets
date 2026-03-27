#include <Averra/AverraDivider.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraDivider : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseHorizontalDividerByDefault();
    void shouldEmitOrientationChangedWhenOrientationUpdates();
    void shouldEmitThicknessChangedWhenThicknessUpdates();
    void shouldProvideDifferentSizeHintsForOrientations();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraDivider::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraDivider::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraDivider::shouldUseHorizontalDividerByDefault()
{
    AverraDivider divider;

    QCOMPARE(divider.orientation(), Qt::Horizontal);
    QCOMPARE(divider.thickness(), 1);
}

void TestAverraDivider::shouldEmitOrientationChangedWhenOrientationUpdates()
{
    AverraDivider divider;
    QSignalSpy spy(&divider, &AverraDivider::orientationChanged);

    divider.setOrientation(Qt::Vertical);

    QCOMPARE(divider.orientation(), Qt::Vertical);
    QCOMPARE(spy.count(), 1);
}

void TestAverraDivider::shouldEmitThicknessChangedWhenThicknessUpdates()
{
    AverraDivider divider;
    QSignalSpy spy(&divider, &AverraDivider::thicknessChanged);

    divider.setThickness(2);

    QCOMPARE(divider.thickness(), 2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 2);
}

void TestAverraDivider::shouldProvideDifferentSizeHintsForOrientations()
{
    AverraDivider divider;
    const QSize horizontalSize = divider.sizeHint();

    divider.setOrientation(Qt::Vertical);
    const QSize verticalSize = divider.sizeHint();

    QVERIFY(horizontalSize.width() > horizontalSize.height());
    QVERIFY(verticalSize.height() > verticalSize.width());
}

QObject *createTestAverraDivider()
{
    return new TestAverraDivider;
}

#include "tst_AverraDivider.moc"
