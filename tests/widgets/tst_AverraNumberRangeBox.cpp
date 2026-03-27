#include <Averra/AverraNumberRangeBox.h>

#include <QtTest>

class TestAverraNumberRangeBox : public QObject
{
    Q_OBJECT

private slots:
    void shouldStoreMinimumAndMaximumValues();
};

void TestAverraNumberRangeBox::shouldStoreMinimumAndMaximumValues()
{
    AverraNumberRangeBox box;
    box.setMinimumValue(5);
    box.setMaximumValue(10);

    QCOMPARE(box.minimumValue(), 5);
    QCOMPARE(box.maximumValue(), 10);
}

QObject *createTestAverraNumberRangeBox()
{
    return new TestAverraNumberRangeBox;
}

#include "tst_AverraNumberRangeBox.moc"
