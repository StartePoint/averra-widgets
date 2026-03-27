#include <Averra/AverraComboBox.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraComboBox : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void shouldUseDefaultStyle();
    void shouldEmitAccentFrameChangedWhenToggled();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraComboBox::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraComboBox::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraComboBox::shouldUseDefaultStyle()
{
    AverraComboBox comboBox;
    QCOMPARE(comboBox.accentFrame(), false);
}

void TestAverraComboBox::shouldEmitAccentFrameChangedWhenToggled()
{
    AverraComboBox comboBox;
    QSignalSpy spy(&comboBox, &AverraComboBox::accentFrameChanged);
    comboBox.setAccentFrame(true);
    QCOMPARE(spy.count(), 1);
}

QObject *createTestAverraComboBox()
{
    return new TestAverraComboBox;
}

#include "tst_AverraComboBox.moc"

