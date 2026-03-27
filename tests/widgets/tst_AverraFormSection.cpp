#include <Averra/AverraFormSection.h>
#include <Averra/AverraThemeManager.h>

#include <QLabel>
#include <QVBoxLayout>

#include <QtTest>

class TestAverraFormSection : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleDescriptionAndMetaText();
    void shouldAllowAddingRows();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraFormSection::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraFormSection::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraFormSection::shouldStoreTitleDescriptionAndMetaText()
{
    AverraFormSection section;

    section.setTitle(QStringLiteral("常规"));
    section.setDescription(QStringLiteral("常见同步和更新设置"));
    section.setMetaText(QStringLiteral("General"));

    QCOMPARE(section.title(), QStringLiteral("常规"));
    QCOMPARE(section.description(), QStringLiteral("常见同步和更新设置"));
    QCOMPARE(section.metaText(), QStringLiteral("General"));
}

void TestAverraFormSection::shouldAllowAddingRows()
{
    AverraFormSection section;
    QLabel *rowLabel = new QLabel(QStringLiteral("Row"));

    section.addRow(rowLabel);

    QCOMPARE(section.contentLayout()->count(), 1);
}

QObject *createTestAverraFormSection()
{
    return new TestAverraFormSection;
}

#include "tst_AverraFormSection.moc"
