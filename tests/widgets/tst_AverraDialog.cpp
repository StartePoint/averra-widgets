#include <Averra/AverraDialog.h>

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include <QtTest>

class TestAverraDialog : public QObject
{
    Q_OBJECT

private slots:
    void shouldStoreTitleAndDescription();
    void shouldAllowAddingContentAndFooterWidgets();
    void shouldUseSquareCornerStyleForNativeWindowFrame();
};

void TestAverraDialog::shouldStoreTitleAndDescription()
{
    AverraDialog dialog;
    dialog.setTitle(QStringLiteral("确认删除"));
    dialog.setDescription(QStringLiteral("删除后不可恢复。"));

    QCOMPARE(dialog.title(), QStringLiteral("确认删除"));
    QCOMPARE(dialog.description(), QStringLiteral("删除后不可恢复。"));
}

void TestAverraDialog::shouldAllowAddingContentAndFooterWidgets()
{
    AverraDialog dialog;
    dialog.addContentWidget(new QLabel(QStringLiteral("内容")));
    dialog.addFooterWidget(new QLabel(QStringLiteral("按钮")));

    QVERIFY(dialog.contentLayout()->count() >= 1);
    QVERIFY(dialog.footerLayout()->count() >= 2);
}

void TestAverraDialog::shouldUseSquareCornerStyleForNativeWindowFrame()
{
    AverraDialog dialog;
    QFrame *rootFrame = dialog.findChild<QFrame *>(QStringLiteral("AverraDialogRoot"));

    QVERIFY(rootFrame != nullptr);
    QVERIFY(rootFrame->styleSheet().contains(QStringLiteral("border-radius: 0px;")));
}

QObject *createTestAverraDialog()
{
    return new TestAverraDialog;
}

#include "tst_AverraDialog.moc"
