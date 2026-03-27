#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraThemeManager : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldReturnSingleton();
    void shouldUpdatePaletteThroughStateSetters();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraThemeManager::init()
{
    m_defaultPalette = AverraThemeManager::instance()->palette();
    AverraThemeManager::instance()->setPalette(AverraThemePalette::createLightPalette());
}

void TestAverraThemeManager::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
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

QObject *createTestAverraThemeManager()
{
    return new TestAverraThemeManager;
}

#include "tst_AverraThemeManager.moc"
