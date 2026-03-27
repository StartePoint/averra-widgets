#include <Averra/AverraSectionHeader.h>
#include <Averra/AverraThemeManager.h>

#include <QFrame>
#include <QLabel>

#include <QtTest>

class TestAverraSectionHeader : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleSubtitleAndMetaText();
    void shouldHideMetaLabelWhenMetaTextIsEmpty();
    void shouldRefreshMetaStyleAfterThemeChange();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraSectionHeader::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSectionHeader::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSectionHeader::shouldStoreTitleSubtitleAndMetaText()
{
    AverraSectionHeader header;

    header.setTitle(QStringLiteral("基础输入"));
    header.setSubtitle(QStringLiteral("文本、选择和滑块组件"));
    header.setMetaText(QStringLiteral("Forms"));

    QCOMPARE(header.title(), QStringLiteral("基础输入"));
    QCOMPARE(header.subtitle(), QStringLiteral("文本、选择和滑块组件"));
    QCOMPARE(header.metaText(), QStringLiteral("Forms"));
}

void TestAverraSectionHeader::shouldHideMetaLabelWhenMetaTextIsEmpty()
{
    AverraSectionHeader header(QStringLiteral("基础输入"));
    QLabel *metaLabel = header.findChild<QLabel *>(QStringLiteral("AverraSectionHeaderMeta"));

    QVERIFY(metaLabel != nullptr);
    header.setMetaText(QString());

    QVERIFY(metaLabel->isHidden());
}

void TestAverraSectionHeader::shouldRefreshMetaStyleAfterThemeChange()
{
    AverraSectionHeader header(QStringLiteral("基础输入"));
    header.setMetaText(QStringLiteral("Forms"));

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentColor(QColor(QStringLiteral("#0EA5E9")));
    AverraThemeManager::instance()->setPalette(palette);

    QLabel *metaLabel = header.findChild<QLabel *>(QStringLiteral("AverraSectionHeaderMeta"));
    QVERIFY(metaLabel != nullptr);
    QVERIFY(metaLabel->styleSheet().contains(QStringLiteral("#0ea5e9"), Qt::CaseInsensitive));
}

QObject *createTestAverraSectionHeader()
{
    return new TestAverraSectionHeader;
}

#include "tst_AverraSectionHeader.moc"
