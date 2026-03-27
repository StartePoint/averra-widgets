#include <Averra/AverraToast.h>

#include <QtTest>

class TestAverraToast : public QObject
{
    Q_OBJECT

private slots:
    void shouldStoreTitleAndDescription();
    void shouldUpdateType();
};

void TestAverraToast::shouldStoreTitleAndDescription()
{
    AverraToast toast;
    toast.setTitle(QStringLiteral("保存成功"));
    toast.setDescription(QStringLiteral("设置已经更新。"));

    QCOMPARE(toast.title(), QStringLiteral("保存成功"));
    QCOMPARE(toast.description(), QStringLiteral("设置已经更新。"));
}

void TestAverraToast::shouldUpdateType()
{
    AverraToast toast;
    toast.setType(AverraToast::Warning);
    QCOMPARE(toast.type(), AverraToast::Warning);
}

QObject *createTestAverraToast()
{
    return new TestAverraToast;
}

#include "tst_AverraToast.moc"

