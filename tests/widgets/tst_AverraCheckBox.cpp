#include <Averra/AverraCheckBox.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraCheckBox : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseUncheckedStateByDefault();
    void shouldEmitToggledWhenCheckedChanges();
    void shouldToggleWhenClicked();
    void shouldProvideReasonableSizeHint();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraCheckBox::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraCheckBox::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraCheckBox::shouldUseUncheckedStateByDefault()
{
    AverraCheckBox checkBox;

    QCOMPARE(checkBox.isChecked(), false);
    QCOMPARE(checkBox.isCheckable(), true);
}

void TestAverraCheckBox::shouldEmitToggledWhenCheckedChanges()
{
    AverraCheckBox checkBox;
    QSignalSpy spy(&checkBox, &AverraCheckBox::toggled);

    checkBox.setChecked(true);

    QCOMPARE(checkBox.isChecked(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestAverraCheckBox::shouldToggleWhenClicked()
{
    AverraCheckBox checkBox(QStringLiteral("测试项"));
    checkBox.show();

    QTest::mouseClick(&checkBox, Qt::LeftButton);

    QCOMPARE(checkBox.isChecked(), true);
}

void TestAverraCheckBox::shouldProvideReasonableSizeHint()
{
    AverraCheckBox checkBox(QStringLiteral("测试项"));

    QVERIFY(checkBox.sizeHint().width() > checkBox.sizeHint().height());
    QVERIFY(checkBox.sizeHint().height() >= 18);
}

QObject *createTestAverraCheckBox()
{
    return new TestAverraCheckBox;
}

#include "tst_AverraCheckBox.moc"
