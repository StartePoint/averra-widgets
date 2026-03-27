#include <Averra/AverraStyleProfile.h>

#include <QTemporaryDir>

#include <QtTest>

class TestAverraStyleProfile : public QObject
{
    Q_OBJECT

private slots:
    void shouldUseDefaultDesktopParameters();
    void shouldRoundTripThroughVariantMap();
    void shouldLoadAndSaveJsonFile();
};

void TestAverraStyleProfile::shouldUseDefaultDesktopParameters()
{
    const AverraStyleProfile profile = AverraStyleProfile::createDefaultProfile();

    QCOMPARE(profile.controlRadius(), 12);
    QCOMPARE(profile.windowControlsLayout(), AverraStyleProfile::TrailingWindowControls);
    QCOMPARE(profile.titleTextCentered(), false);
    QCOMPARE(profile.windowControlButtonWidth(), 38);
}

void TestAverraStyleProfile::shouldRoundTripThroughVariantMap()
{
    AverraStyleProfile sourceProfile = AverraStyleProfile::createDefaultProfile();
    sourceProfile.setControlRadius(9);
    sourceProfile.setPanelRadius(18);
    sourceProfile.setWindowControlsLayout(AverraStyleProfile::LeadingTrafficLights);
    sourceProfile.setTitleTextCentered(true);
    sourceProfile.setTrafficLightDiameter(11);

    AverraStyleProfile loadedProfile = AverraStyleProfile::createDefaultProfile();
    loadedProfile.applyVariantMap(sourceProfile.toVariantMap());

    QCOMPARE(loadedProfile, sourceProfile);
}

void TestAverraStyleProfile::shouldLoadAndSaveJsonFile()
{
    QTemporaryDir temporaryDir;
    QVERIFY(temporaryDir.isValid());

    const QString filePath = temporaryDir.path() + QStringLiteral("/style-profile.json");
    AverraStyleProfile sourceProfile = AverraStyleProfile::createDefaultProfile();
    sourceProfile.setSmallRadius(6);
    sourceProfile.setControlRadius(8);
    sourceProfile.setCompactPaddingHorizontal(12);
    sourceProfile.setWindowControlsLayout(AverraStyleProfile::LeadingTrafficLights);
    sourceProfile.setEnableTitleBarDrag(false);
    sourceProfile.setEnableTitleBarDoubleClickZoom(false);

    QString errorMessage;
    QVERIFY2(sourceProfile.saveToJsonFile(filePath, &errorMessage), qPrintable(errorMessage));

    AverraStyleProfile loadedProfile = AverraStyleProfile::createDefaultProfile();
    QVERIFY2(loadedProfile.loadFromJsonFile(filePath, &errorMessage), qPrintable(errorMessage));
    QCOMPARE(loadedProfile, sourceProfile);
}

QObject *createTestAverraStyleProfile()
{
    return new TestAverraStyleProfile;
}

#include "tst_AverraStyleProfile.moc"
