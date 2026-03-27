#include <Averra/AverraTextArea.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraTextArea : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseDefaultStyleAndPlainTextMode();
    void shouldEmitAccentFrameChangedWhenToggled();
    void shouldRefreshAccentStyleAfterThemeChange();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraTextArea::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraTextArea::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraTextArea::shouldUseDefaultStyleAndPlainTextMode()
{
    AverraTextArea textArea;

    QCOMPARE(textArea.accentFrame(), false);
    QCOMPARE(textArea.acceptRichText(), false);
    QVERIFY(textArea.styleSheet().contains(m_defaultPalette.surfaceRaisedColor().name(), Qt::CaseInsensitive));
    QVERIFY(textArea.styleSheet().contains(m_defaultPalette.borderColor().name(), Qt::CaseInsensitive));
}

void TestAverraTextArea::shouldEmitAccentFrameChangedWhenToggled()
{
    AverraTextArea textArea;
    QSignalSpy spy(&textArea, &AverraTextArea::accentFrameChanged);

    textArea.setAccentFrame(true);

    QCOMPARE(textArea.accentFrame(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestAverraTextArea::shouldRefreshAccentStyleAfterThemeChange()
{
    AverraTextArea textArea;
    textArea.setAccentFrame(true);

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentColor(QColor(QStringLiteral("#9333EA")));
    AverraThemeManager::instance()->setPalette(palette);

    QVERIFY(textArea.styleSheet().contains(QStringLiteral("#9333ea"), Qt::CaseInsensitive));
}

QObject *createTestAverraTextArea()
{
    return new TestAverraTextArea;
}

#include "tst_AverraTextArea.moc"
