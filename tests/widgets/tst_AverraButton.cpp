#include <Averra/AverraButton.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraButton : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseNonAccentStyleByDefault();
    void shouldEmitAccentChangedWhenAccentToggles();
    void shouldRefreshAccentStyleAfterThemeChange();
    void shouldRefreshStyleAfterStyleProfileChanges();
    void shouldUseThemeStateColorsInStyleSheet();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraButton::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
    AverraThemeManager::instance()->applyTheme(AverraThemeManager::OceanTheme);
    AverraThemeManager::instance()->resetStyleProfile();
}

void TestAverraButton::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
    AverraThemeManager::instance()->resetStyleProfile();
}

void TestAverraButton::shouldUseNonAccentStyleByDefault()
{
    AverraButton button;

    QCOMPARE(button.accent(), false);
    QVERIFY(button.styleSheet().contains(m_defaultPalette.surfaceRaisedColor().name()));
    QVERIFY(button.styleSheet().contains(m_defaultPalette.borderColor().name()));
}

void TestAverraButton::shouldEmitAccentChangedWhenAccentToggles()
{
    AverraButton button;
    QSignalSpy spy(&button, &AverraButton::accentChanged);

    button.setAccent(true);

    QCOMPARE(button.accent(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestAverraButton::shouldRefreshAccentStyleAfterThemeChange()
{
    AverraButton button;
    button.setAccent(true);

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentColor(QColor(QStringLiteral("#2E8B57")));
    AverraThemeManager::instance()->setPalette(palette);

    QTRY_VERIFY(button.styleSheet().contains(QStringLiteral("#2e8b57"), Qt::CaseInsensitive));
}

void TestAverraButton::shouldRefreshStyleAfterStyleProfileChanges()
{
    AverraButton button;
    button.setAccent(true);

    AverraThemeManager::instance()->applyTheme(AverraThemeManager::OceanTheme);
    AverraThemeManager::instance()->resetStyleProfile();
    const QString win11Style = button.styleSheet();

    AverraStyleProfile compactProfile = AverraStyleProfile::createDefaultProfile();
    compactProfile.setControlRadius(6);
    compactProfile.setButtonPaddingVertical(7);
    compactProfile.setButtonPaddingHorizontal(14);
    compactProfile.setTitleFontSize(15);
    AverraThemeManager::instance()->setStyleProfile(compactProfile);
    const QString customStyle = button.styleSheet();

    QVERIFY(win11Style != customStyle);
    QVERIFY(win11Style.contains(QStringLiteral("border-radius: 12px")));
    QVERIFY(customStyle.contains(QStringLiteral("border-radius: 6px")));
}

void TestAverraButton::shouldUseThemeStateColorsInStyleSheet()
{
    AverraButton button;
    button.setAccent(true);

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentHoverColor(QColor(QStringLiteral("#4B84F2")));
    palette.setAccentPressedColor(QColor(QStringLiteral("#1F58D0")));
    palette.setAccentDisabledColor(QColor(QStringLiteral("#B8CBF7")));
    AverraThemeManager::instance()->setPalette(palette);

    QVERIFY(button.styleSheet().contains(QStringLiteral("#4b84f2"), Qt::CaseInsensitive));
    QVERIFY(button.styleSheet().contains(QStringLiteral("#1f58d0"), Qt::CaseInsensitive));
    QVERIFY(button.styleSheet().contains(QStringLiteral("#b8cbf7"), Qt::CaseInsensitive));
}

QObject *createTestAverraButton()
{
    return new TestAverraButton;
}

#include "tst_AverraButton.moc"
