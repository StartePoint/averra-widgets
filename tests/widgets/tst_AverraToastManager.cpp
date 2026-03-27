#include <Averra/AverraToastManager.h>

#include <QWidget>

#include <QtTest>

class TestAverraToastManager : public QObject
{
    Q_OBJECT

private slots:
    void shouldReturnSingleton();
    void shouldCreateToast();
};

void TestAverraToastManager::shouldReturnSingleton()
{
    QCOMPARE(AverraToastManager::instance(), AverraToastManager::instance());
}

void TestAverraToastManager::shouldCreateToast()
{
    QWidget host;
    host.resize(400, 300);

    AverraToast *toast = AverraToastManager::instance()->showToast(&host,
                                                                   QStringLiteral("完成"),
                                                                   QStringLiteral("数据已同步"),
                                                                   AverraToast::Success);
    QVERIFY(toast != nullptr);
    toast->hide();
    toast->deleteLater();
}

QObject *createTestAverraToastManager()
{
    return new TestAverraToastManager;
}

#include "tst_AverraToastManager.moc"

