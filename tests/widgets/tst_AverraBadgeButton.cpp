#include <Averra/AverraBadgeButton.h>
#include <Averra/AverraThemeManager.h>

#include <QLabel>

#include <QtTest>

class TestAverraBadgeButton : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseEmptyBadgeByDefault();
    void shouldEmitBadgeTextChangedWhenBadgeUpdates();
    void shouldEmitAccentChangedWhenAccentToggles();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraBadgeButton::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraBadgeButton::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraBadgeButton::shouldUseEmptyBadgeByDefault()
{
    AverraBadgeButton button;

    QCOMPARE(button.accent(), false);
    QCOMPARE(button.badgeText(), QString());
}

void TestAverraBadgeButton::shouldEmitBadgeTextChangedWhenBadgeUpdates()
{
    AverraBadgeButton button;
    QSignalSpy spy(&button, &AverraBadgeButton::badgeTextChanged);

    button.setBadgeText(QStringLiteral("9"));

    QCOMPARE(button.badgeText(), QStringLiteral("9"));
    QCOMPARE(spy.count(), 1);
}

void TestAverraBadgeButton::shouldEmitAccentChangedWhenAccentToggles()
{
    AverraBadgeButton button;
    QSignalSpy spy(&button, &AverraBadgeButton::accentChanged);

    button.setAccent(true);

    QCOMPARE(button.accent(), true);
    QCOMPARE(spy.count(), 1);
}

QObject *createTestAverraBadgeButton()
{
    return new TestAverraBadgeButton;
}

#include "tst_AverraBadgeButton.moc"
