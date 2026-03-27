#include <Averra/AverraTreeView.h>

#include <QStandardItemModel>

#include <QtTest>

class TestAverraTreeView : public QObject
{
    Q_OBJECT

private slots:
    void shouldSetHeadersAndItems();
};

void TestAverraTreeView::shouldSetHeadersAndItems()
{
    AverraTreeView treeView;
    treeView.setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型"));
    QStandardItem *parent = treeView.addTopLevelItem(QStringList() << QStringLiteral("输入组件") << QStringLiteral("分类"));
    treeView.addChildItem(parent, QStringList() << QStringLiteral("AverraLineEdit") << QStringLiteral("输入"));

    QCOMPARE(treeView.model()->rowCount(), 1);
    QVERIFY(parent != nullptr);
    QCOMPARE(parent->rowCount(), 1);
}

QObject *createTestAverraTreeView()
{
    return new TestAverraTreeView;
}

#include "tst_AverraTreeView.moc"

