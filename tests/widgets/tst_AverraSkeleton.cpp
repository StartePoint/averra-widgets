#include <Averra/AverraSkeleton.h>

#include <QtTest>

class TestAverraSkeleton : public QObject
{
    Q_OBJECT

private slots:
    void shouldUseStoppedStateByDefault();
    void shouldStartAndStop();
};

void TestAverraSkeleton::shouldUseStoppedStateByDefault()
{
    AverraSkeleton skeleton;
    QCOMPARE(skeleton.running(), false);
}

void TestAverraSkeleton::shouldStartAndStop()
{
    AverraSkeleton skeleton;
    skeleton.start();
    QCOMPARE(skeleton.running(), true);
    skeleton.stop();
    QCOMPARE(skeleton.running(), false);
}

QObject *createTestAverraSkeleton()
{
    return new TestAverraSkeleton;
}

#include "tst_AverraSkeleton.moc"

