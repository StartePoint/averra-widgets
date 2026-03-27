#include <Averra/AverraRadioButton.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraRadioButton : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void shouldUseUncheckedStateByDefault();
    void shouldEmitCheckedChangedWhenToggled();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraRadioButton::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraRadioButton::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraRadioButton::shouldUseUncheckedStateByDefault()
{
    AverraRadioButton button;
    QCOMPARE(button.isChecked(), false);
}

void TestAverraRadioButton::shouldEmitCheckedChangedWhenToggled()
{
    AverraRadioButton button;
    QSignalSpy spy(&button, &AverraRadioButton::checkedChanged);
    button.setChecked(true);
    QCOMPARE(spy.count(), 1);
}

QObject *createTestAverraRadioButton()
{
    return new TestAverraRadioButton;
}

#include "tst_AverraRadioButton.moc"

