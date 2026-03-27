#include <Averra/AverraSwitch.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraSwitch : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseUncheckedStateByDefault();
    void shouldEmitToggledWhenCheckedChanges();
    void shouldToggleWhenClicked();
    void shouldProvideDesktopSizedHint();
    void shouldAdaptSizeHintForTrafficLightLayout();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraSwitch::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
    AverraThemeManager::instance()->applyTheme(AverraThemeManager::OceanTheme);
    AverraThemeManager::instance()->resetStyleProfile();
}

void TestAverraSwitch::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
    AverraThemeManager::instance()->applyTheme(AverraThemeManager::OceanTheme);
    AverraThemeManager::instance()->resetStyleProfile();
}

void TestAverraSwitch::shouldUseUncheckedStateByDefault()
{
    AverraSwitch switchButton;

    QCOMPARE(switchButton.isChecked(), false);
    QCOMPARE(switchButton.isCheckable(), true);
}

void TestAverraSwitch::shouldEmitToggledWhenCheckedChanges()
{
    AverraSwitch switchButton;
    QSignalSpy spy(&switchButton, &AverraSwitch::toggled);

    switchButton.setChecked(true);

    QCOMPARE(switchButton.isChecked(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestAverraSwitch::shouldToggleWhenClicked()
{
    AverraSwitch switchButton;
    switchButton.show();

    QTest::mouseClick(&switchButton, Qt::LeftButton);

    QCOMPARE(switchButton.isChecked(), true);
}

void TestAverraSwitch::shouldProvideDesktopSizedHint()
{
    AverraSwitch switchButton;

    QVERIFY(switchButton.sizeHint().width() > switchButton.sizeHint().height());
    QCOMPARE(switchButton.sizeHint(), QSize(52, 30));
}

void TestAverraSwitch::shouldAdaptSizeHintForTrafficLightLayout()
{
    AverraStyleProfile compactProfile = AverraStyleProfile::createDefaultProfile();
    compactProfile.setWindowControlsLayout(AverraStyleProfile::LeadingTrafficLights);
    compactProfile.setControlRadius(6);
    compactProfile.setButtonPaddingHorizontal(14);
    compactProfile.setButtonPaddingVertical(7);
    AverraThemeManager::instance()->setStyleProfile(compactProfile);
    AverraSwitch switchButton;

    QCOMPARE(switchButton.sizeHint(), QSize(46, 26));
}

QObject *createTestAverraSwitch()
{
    return new TestAverraSwitch;
}

#include "tst_AverraSwitch.moc"
