#include <Averra/AverraDrawer.h>

#include <QtTest>

class TestAverraDrawer : public QObject
{
    Q_OBJECT

private slots:
    void shouldStoreTitleAndDescription();
    void shouldToggleOpenedState();
};

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

QObject *createTestAverraDrawer()
{
    return new TestAverraDrawer;
}

#include "tst_AverraDrawer.moc"

