#include <Averra/AverraSettingsPage.h>
#include <Averra/AverraThemeManager.h>

#include <QLabel>
#include <QVBoxLayout>

#include <QtTest>

class TestAverraSettingsPage : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleAndDescription();
    void shouldAllowAddingSections();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraSettingsPage::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSettingsPage::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSettingsPage::shouldStoreTitleAndDescription()
{
    AverraSettingsPage page;

    page.setTitle(QStringLiteral("通用设置"));
    page.setDescription(QStringLiteral("统一组织设置分组和属性项。"));

    QCOMPARE(page.title(), QStringLiteral("通用设置"));
    QCOMPARE(page.description(), QStringLiteral("统一组织设置分组和属性项。"));
}

void TestAverraSettingsPage::shouldAllowAddingSections()
{
    AverraSettingsPage page;
    QLabel *sectionLabel = new QLabel(QStringLiteral("Section"));

    page.addSection(sectionLabel);

    QCOMPARE(page.contentLayout()->count(), 1);
}

QObject *createTestAverraSettingsPage()
{
    return new TestAverraSettingsPage;
}

#include "tst_AverraSettingsPage.moc"
