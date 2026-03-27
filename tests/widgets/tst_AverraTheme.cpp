#include <Averra/AverraThemeManager.h>
#include <Averra/AverraTheme.h>

#include <QtTest>

class TestAverraTheme : public QObject
{
    Q_OBJECT

private slots:
    void shouldProvideDefaultStateColors();
    void shouldAllowOverridingStateColors();
    void shouldDeriveStateColorsWhenUnset();
    void shouldProvideDifferentThemePalettes();
};

void TestAverraTheme::shouldProvideDefaultStateColors()
{
    const AverraThemePalette palette = AverraThemePalette::createLightPalette();

    QCOMPARE(palette.surfaceHoverColor(), QColor(QStringLiteral("#F1F4F8")));
    QCOMPARE(palette.surfacePressedColor(), QColor(QStringLiteral("#E7ECF3")));
    QCOMPARE(palette.surfaceDisabledColor(), QColor(QStringLiteral("#EEF2F7")));
    QCOMPARE(palette.borderHoverColor(), QColor(QStringLiteral("#A9B4C4")));
    QCOMPARE(palette.textDisabledColor(), QColor(QStringLiteral("#98A2B3")));
    QCOMPARE(palette.accentHoverColor(), QColor(QStringLiteral("#3B78F0")));
    QCOMPARE(palette.accentPressedColor(), QColor(QStringLiteral("#245FD7")));
    QCOMPARE(palette.accentDisabledColor(), QColor(QStringLiteral("#A8C2F5")));
}

void TestAverraTheme::shouldAllowOverridingStateColors()
{
    AverraThemePalette palette = AverraThemePalette::createLightPalette();

    palette.setSurfaceHoverColor(QColor(QStringLiteral("#F6F9FD")));
    palette.setBorderHoverColor(QColor(QStringLiteral("#7E95BC")));
    palette.setAccentPressedColor(QColor(QStringLiteral("#1C4FB8")));

    QCOMPARE(palette.surfaceHoverColor(), QColor(QStringLiteral("#F6F9FD")));
    QCOMPARE(palette.borderHoverColor(), QColor(QStringLiteral("#7E95BC")));
    QCOMPARE(palette.accentPressedColor(), QColor(QStringLiteral("#1C4FB8")));
}

void TestAverraTheme::shouldDeriveStateColorsWhenUnset()
{
    AverraThemePalette palette;
    palette.setAccentColor(QColor(QStringLiteral("#2E6BE6")));
    palette.setSurfaceColor(QColor(QStringLiteral("#F7F8FA")));
    palette.setSurfaceRaisedColor(QColor(QStringLiteral("#FFFFFF")));
    palette.setBorderColor(QColor(QStringLiteral("#D5DBE5")));
    palette.setTextSecondaryColor(QColor(QStringLiteral("#5B6472")));

    QVERIFY(palette.surfaceHoverColor().isValid());
    QVERIFY(palette.surfacePressedColor().isValid());
    QVERIFY(palette.surfaceDisabledColor().isValid());
    QVERIFY(palette.borderHoverColor().isValid());
    QVERIFY(palette.textDisabledColor().isValid());
    QVERIFY(palette.accentHoverColor().isValid());
    QVERIFY(palette.accentPressedColor().isValid());
    QVERIFY(palette.accentDisabledColor().isValid());
}

void TestAverraTheme::shouldProvideDifferentThemePalettes()
{
    const AverraThemePalette oceanPalette =
        AverraThemeManager::createPalette(AverraThemeManager::OceanTheme);
    const AverraThemePalette sunsetPalette =
        AverraThemeManager::createPalette(AverraThemeManager::SunsetTheme);
    const AverraThemePalette forestPalette =
        AverraThemeManager::createPalette(AverraThemeManager::ForestTheme);

    QCOMPARE(oceanPalette.accentColor(), QColor(QStringLiteral("#2E6BE6")));
    QCOMPARE(sunsetPalette.accentColor(), QColor(QStringLiteral("#D97706")));
    QCOMPARE(forestPalette.accentColor(), QColor(QStringLiteral("#2E8B57")));
    QCOMPARE(oceanPalette.surfaceColor(), QColor(QStringLiteral("#F7F8FA")));
    QVERIFY(oceanPalette.accentColor() != sunsetPalette.accentColor());
}

QObject *createTestAverraTheme()
{
    return new TestAverraTheme;
}

#include "tst_AverraTheme.moc"
