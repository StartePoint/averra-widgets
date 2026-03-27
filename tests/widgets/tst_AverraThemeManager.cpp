#include <Averra/AverraThemeManager.h>

#include <QTemporaryDir>

#include <QtTest>

class TestAverraThemeManager : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldReturnSingleton();
    void shouldUpdatePaletteThroughStateSetters();
    void shouldSwitchThemeAndApplyCustomStyleProfile();
    void shouldLoadAndSaveStyleProfileFile();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraThemeManager::init()
{
    qRegisterMetaType<AverraStyleProfile>("AverraStyleProfile");
    m_defaultPalette = AverraThemeManager::instance()->palette();
    AverraThemeManager::instance()->setPalette(AverraThemePalette::createLightPalette());
    AverraThemeManager::instance()->applyTheme(AverraThemeManager::OceanTheme);
    AverraThemeManager::instance()->resetStyleProfile();
}

void TestAverraThemeManager::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
    AverraThemeManager::instance()->resetStyleProfile();
}

void TestAverraThemeManager::shouldReturnSingleton()
{
    QCOMPARE(AverraThemeManager::instance(), AverraThemeManager::instance());
}

void TestAverraThemeManager::shouldUpdatePaletteThroughStateSetters()
{
    QSignalSpy paletteChangedSpy(AverraThemeManager::instance(), &AverraThemeManager::paletteChanged);

    AverraThemeManager::instance()->setSurfaceHoverColor(QColor(QStringLiteral("#F5F7FB")));
    AverraThemeManager::instance()->setBorderHoverColor(QColor(QStringLiteral("#8AA0C5")));
    AverraThemeManager::instance()->setAccentPressedColor(QColor(QStringLiteral("#1D55C8")));

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    QCOMPARE(palette.surfaceHoverColor(), QColor(QStringLiteral("#F5F7FB")));
    QCOMPARE(palette.borderHoverColor(), QColor(QStringLiteral("#8AA0C5")));
    QCOMPARE(palette.accentPressedColor(), QColor(QStringLiteral("#1D55C8")));
    QCOMPARE(paletteChangedSpy.count(), 3);
}

void TestAverraThemeManager::shouldSwitchThemeAndApplyCustomStyleProfile()
{
    QSignalSpy paletteChangedSpy(AverraThemeManager::instance(), &AverraThemeManager::paletteChanged);
    QSignalSpy styleProfileChangedSpy(AverraThemeManager::instance(), &AverraThemeManager::styleProfileChanged);

    AverraThemeManager::instance()->setThemePreset(AverraThemeManager::ForestTheme);
    AverraStyleProfile customProfile = AverraStyleProfile::createDefaultProfile();
    customProfile.setControlRadius(6);
    customProfile.setWindowControlsLayout(AverraStyleProfile::LeadingTrafficLights);
    AverraThemeManager::instance()->setStyleProfile(customProfile);

    QCOMPARE(AverraThemeManager::instance()->themePreset(), AverraThemeManager::ForestTheme);
    QCOMPARE(AverraThemeManager::instance()->palette().accentColor(), QColor(QStringLiteral("#2E8B57")));
    QCOMPARE(AverraThemeManager::instance()->styleProfile().controlRadius(), 6);
    QCOMPARE(AverraThemeManager::instance()->styleProfile().windowControlsLayout(),
             AverraStyleProfile::LeadingTrafficLights);
    QCOMPARE(paletteChangedSpy.count(), 2);
    QCOMPARE(styleProfileChangedSpy.count(), 1);
}

void TestAverraThemeManager::shouldLoadAndSaveStyleProfileFile()
{
    QTemporaryDir temporaryDir;
    QVERIFY(temporaryDir.isValid());

    const QString filePath = temporaryDir.path() + QStringLiteral("/theme-manager-style.json");
    AverraStyleProfile profile = AverraStyleProfile::createDefaultProfile();
    profile.setMediumRadius(9);
    profile.setWindowControlsLayout(AverraStyleProfile::LeadingTrafficLights);
    profile.setEnableTitleBarDrag(false);
    AverraThemeManager::instance()->setStyleProfile(profile);

    QString errorMessage;
    QVERIFY2(AverraThemeManager::instance()->saveStyleProfile(filePath, &errorMessage), qPrintable(errorMessage));

    AverraThemeManager::instance()->resetStyleProfile();
    QVERIFY2(AverraThemeManager::instance()->loadStyleProfile(filePath, &errorMessage), qPrintable(errorMessage));

    QCOMPARE(AverraThemeManager::instance()->styleProfile(), profile);
}

QObject *createTestAverraThemeManager()
{
    return new TestAverraThemeManager;
}

#include "tst_AverraThemeManager.moc"
