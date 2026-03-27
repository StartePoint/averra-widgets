#include <Averra/AverraDialogManager.h>
#include <Averra/AverraDialog.h>

#include <QSignalSpy>
#include <QWidget>

#include <QtTest>

class TestAverraDialogManager : public QObject
{
    Q_OBJECT

private slots:
    void shouldReturnSingleton();
    void shouldShowDialog();
    void shouldCloseAllManagedDialogs();
};

void TestAverraDialogManager::shouldReturnSingleton()
{
    QCOMPARE(AverraDialogManager::instance(), AverraDialogManager::instance());
}

void TestAverraDialogManager::shouldShowDialog()
{
    QWidget host;
    host.resize(900, 600);
    host.show();

    AverraDialog dialog(&host);
    QSignalSpy dialogShownSpy(AverraDialogManager::instance(), &AverraDialogManager::dialogShown);

    AverraDialog *shown = AverraDialogManager::instance()->showDialog(&dialog);

    QCOMPARE(shown, &dialog);
    QTRY_VERIFY(dialog.isVisible());
    QCOMPARE(dialog.windowModality(), Qt::WindowModal);
    QCOMPARE(dialogShownSpy.count(), 1);

    dialog.close();
}

void TestAverraDialogManager::shouldCloseAllManagedDialogs()
{
    QWidget host;
    host.resize(900, 600);
    host.show();

    AverraDialog *firstDialog = new AverraDialog(&host);
    AverraDialog *secondDialog = new AverraDialog(&host);

    AverraDialogManager::instance()->showDialog(firstDialog);
    AverraDialogManager::instance()->showDialog(secondDialog);

    QTRY_VERIFY(firstDialog->isVisible());
    QTRY_VERIFY(secondDialog->isVisible());

    AverraDialogManager::instance()->closeAll();

    QTRY_VERIFY(!firstDialog->isVisible());
    QTRY_VERIFY(!secondDialog->isVisible());
}

QObject *createTestAverraDialogManager()
{
    return new TestAverraDialogManager;
}

#include "tst_AverraDialogManager.moc"
