#include <Averra/AverraContextMenu.h>

#include <QtTest>

class TestAverraContextMenu : public QObject
{
    Q_OBJECT

private slots:
    void shouldAddActions();
};

void TestAverraContextMenu::shouldAddActions()
{
    AverraContextMenu menu;
    menu.addAction(QStringLiteral("重命名"));
    menu.addAction(QStringLiteral("删除"));

    QCOMPARE(menu.actions().size(), 2);
}

QObject *createTestAverraContextMenu()
{
    return new TestAverraContextMenu;
}

#include "tst_AverraContextMenu.moc"

