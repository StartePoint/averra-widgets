#include <Averra/AverraDrawer.h>
#include <Averra/AverraThemeManager.h>

#include <QToolButton>

#include <QtTest>

class TestAverraDrawer : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void shouldStoreTitleAndDescription();
    void shouldToggleOpenedState();
    void shouldRefreshWindowControlsForLeadingTrafficLightsLayout();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraDrawer::init()
{
    m_defaultPalette = AverraThemeManager::instance()->palette();
    AverraThemeManager::instance()->applyTheme(AverraThemeManager::OceanTheme);
    AverraThemeManager::instance()->resetStyleProfile();
}

void TestAverraDrawer::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
    AverraThemeManager::instance()->resetStyleProfile();
}

void TestAverraDrawer::shouldStoreTitleAndDescription()
{
    AverraDrawer drawer;
    drawer.setTitle(QStringLiteral("详情"));
    drawer.setDescription(QStringLiteral("右侧面板"));

    QCOMPARE(drawer.title(), QStringLiteral("详情"));
    QCOMPARE(drawer.description(), QStringLiteral("右侧面板"));
}

void TestAverraDrawer::shouldToggleOpenedState()
{
    AverraDrawer drawer;
    drawer.setOpened(false);
    QCOMPARE(drawer.opened(), false);
    drawer.setOpened(true);
    QCOMPARE(drawer.opened(), true);
}

void TestAverraDrawer::shouldRefreshWindowControlsForLeadingTrafficLightsLayout()
{
    AverraDrawer drawer;
    QToolButton *closeButton = drawer.findChild<QToolButton *>(QStringLiteral("AverraDrawerCloseButton"));
    QToolButton *minimizeButton = drawer.findChild<QToolButton *>(QStringLiteral("AverraDrawerMinimizeButton"));

    QVERIFY(closeButton != nullptr);
    QVERIFY(minimizeButton != nullptr);
    QCOMPARE(closeButton->text(), QStringLiteral("×"));

    AverraStyleProfile customProfile = AverraStyleProfile::createDefaultProfile();
    customProfile.setWindowControlsLayout(AverraStyleProfile::LeadingTrafficLights);
    customProfile.setTitleTextCentered(true);
    customProfile.setWindowControlButtonWidth(10);
    customProfile.setWindowControlButtonHeight(10);
    customProfile.setTrafficLightDiameter(10);
    customProfile.setEnableTitleBarDrag(false);
    customProfile.setEnableTitleBarDoubleClickZoom(false);
    AverraThemeManager::instance()->setStyleProfile(customProfile);

    QCOMPARE(closeButton->text(), QString());
    QCOMPARE(minimizeButton->text(), QString());
}

QObject *createTestAverraDrawer()
{
    return new TestAverraDrawer;
}

#include "tst_AverraDrawer.moc"
