#include <Averra/AverraNumberBox.h>

#include <QtTest>

class TestAverraNumberBox : public QObject
{
    Q_OBJECT

private slots:
    void shouldUseDefaultValue();
    void shouldToggleAccentFrame();
};

void TestAverraNumberBox::shouldUseDefaultValue()
{
    AverraNumberBox box;
    QCOMPARE(box.value(), 0);
}

void TestAverraNumberBox::shouldToggleAccentFrame()
{
    AverraNumberBox box;
    box.setAccentFrame(true);
    QCOMPARE(box.accentFrame(), true);
}

QObject *createTestAverraNumberBox()
{
    return new TestAverraNumberBox;
}

#include "tst_AverraNumberBox.moc"

