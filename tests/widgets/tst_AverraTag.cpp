#include <Averra/AverraTag.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraTag : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseNeutralFilledStyleByDefault();
    void shouldEmitToneChangedWhenToneUpdates();
    void shouldEmitFilledChangedWhenFilledUpdates();
    void shouldRefreshAccentStyleAfterThemeChange();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraTag::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraTag::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraTag::shouldUseNeutralFilledStyleByDefault()
{
    AverraTag tag;

    QCOMPARE(tag.tone(), AverraTag::Neutral);
    QCOMPARE(tag.filled(), true);
    QVERIFY(tag.styleSheet().contains(m_defaultPalette.textSecondaryColor().name(), Qt::CaseInsensitive));
}

void TestAverraTag::shouldEmitToneChangedWhenToneUpdates()
{
    AverraTag tag;
    QSignalSpy spy(&tag, &AverraTag::toneChanged);

    tag.setTone(AverraTag::Success);

    QCOMPARE(tag.tone(), AverraTag::Success);
    QCOMPARE(spy.count(), 1);
}

void TestAverraTag::shouldEmitFilledChangedWhenFilledUpdates()
{
    AverraTag tag;
    QSignalSpy spy(&tag, &AverraTag::filledChanged);

    tag.setFilled(false);

    QCOMPARE(tag.filled(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);
}

void TestAverraTag::shouldRefreshAccentStyleAfterThemeChange()
{
    AverraTag tag;
    tag.setTone(AverraTag::Accent);
    tag.setFilled(true);

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentColor(QColor(QStringLiteral("#2563EB")));
    palette.setAccentSurfaceColor(QColor(QStringLiteral("#DBEAFE")));
    AverraThemeManager::instance()->setPalette(palette);

    QVERIFY(tag.styleSheet().contains(QStringLiteral("#2563eb"), Qt::CaseInsensitive));
}

QObject *createTestAverraTag()
{
    return new TestAverraTag;
}

#include "tst_AverraTag.moc"
