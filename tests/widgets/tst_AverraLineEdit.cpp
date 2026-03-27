#include <Averra/AverraLineEdit.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraLineEdit : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseDefaultPaletteAndEnableClearButton();
    void shouldEmitAccentFrameChangedWhenToggled();
    void shouldRefreshAccentBorderAfterThemeChange();
    void shouldUseThemeStateColorsForDisabledAndHoverStates();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraLineEdit::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraLineEdit::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraLineEdit::shouldUseDefaultPaletteAndEnableClearButton()
{
    AverraLineEdit lineEdit;

    QCOMPARE(lineEdit.accentFrame(), false);
    QCOMPARE(lineEdit.isClearButtonEnabled(), true);
    QVERIFY(lineEdit.styleSheet().contains(m_defaultPalette.surfaceRaisedColor().name(), Qt::CaseInsensitive));
    QVERIFY(lineEdit.styleSheet().contains(m_defaultPalette.borderColor().name(), Qt::CaseInsensitive));
}

void TestAverraLineEdit::shouldEmitAccentFrameChangedWhenToggled()
{
    AverraLineEdit lineEdit;
    QSignalSpy spy(&lineEdit, &AverraLineEdit::accentFrameChanged);

    lineEdit.setAccentFrame(true);

    QCOMPARE(lineEdit.accentFrame(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestAverraLineEdit::shouldRefreshAccentBorderAfterThemeChange()
{
    AverraLineEdit lineEdit;
    lineEdit.setAccentFrame(true);

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentColor(QColor(QStringLiteral("#D97706")));
    AverraThemeManager::instance()->setPalette(palette);

    QTRY_VERIFY(lineEdit.styleSheet().contains(QStringLiteral("#d97706"), Qt::CaseInsensitive));
}

void TestAverraLineEdit::shouldUseThemeStateColorsForDisabledAndHoverStates()
{
    AverraLineEdit lineEdit;

    AverraThemePalette palette = m_defaultPalette;
    palette.setSurfaceHoverColor(QColor(QStringLiteral("#F3F6FA")));
    palette.setSurfaceDisabledColor(QColor(QStringLiteral("#EEF3F8")));
    palette.setTextDisabledColor(QColor(QStringLiteral("#9AA5B5")));
    palette.setBorderHoverColor(QColor(QStringLiteral("#90A4C3")));
    AverraThemeManager::instance()->setPalette(palette);

    QVERIFY(lineEdit.styleSheet().contains(QStringLiteral("#f3f6fa"), Qt::CaseInsensitive));
    QVERIFY(lineEdit.styleSheet().contains(QStringLiteral("#eef3f8"), Qt::CaseInsensitive));
    QVERIFY(lineEdit.styleSheet().contains(QStringLiteral("#9aa5b5"), Qt::CaseInsensitive));
    QVERIFY(lineEdit.styleSheet().contains(QStringLiteral("#90a4c3"), Qt::CaseInsensitive));
}

QObject *createTestAverraLineEdit()
{
    return new TestAverraLineEdit;
}

#include "tst_AverraLineEdit.moc"
