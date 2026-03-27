#include <Averra/AverraCard.h>
#include <Averra/AverraThemeManager.h>

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

#include <QtTest>

class TestAverraCard : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleSubtitleAndExposeContentArea();
    void shouldEmitAccentHeaderChangedWhenToggled();
    void shouldRefreshAccentStyleAfterThemeChange();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraCard::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraCard::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraCard::shouldStoreTitleSubtitleAndExposeContentArea()
{
    AverraCard card;
    QLabel *bodyLabel = new QLabel(QStringLiteral("Body"));

    card.setTitle(QStringLiteral("概览"));
    card.setSubtitle(QStringLiteral("这里显示项目摘要"));
    card.contentLayout()->addWidget(bodyLabel);

    QCOMPARE(card.title(), QStringLiteral("概览"));
    QCOMPARE(card.subtitle(), QStringLiteral("这里显示项目摘要"));
    QVERIFY(card.contentWidget() != nullptr);
    QVERIFY(card.contentLayout() != nullptr);
    QCOMPARE(card.contentLayout()->count(), 1);
}

void TestAverraCard::shouldEmitAccentHeaderChangedWhenToggled()
{
    AverraCard card;
    QSignalSpy spy(&card, &AverraCard::accentHeaderChanged);

    card.setAccentHeader(true);

    QCOMPARE(card.accentHeader(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestAverraCard::shouldRefreshAccentStyleAfterThemeChange()
{
    AverraCard card;
    card.setAccentHeader(true);

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentColor(QColor(QStringLiteral("#0F766E")));
    palette.setAccentSurfaceColor(QColor(QStringLiteral("#E6FFFB")));
    AverraThemeManager::instance()->setPalette(palette);

    QFrame *rootFrame = card.findChild<QFrame *>(QStringLiteral("AverraCardRoot"));
    QVERIFY(rootFrame != nullptr);
    QTRY_VERIFY(rootFrame->styleSheet().contains(QStringLiteral("#0f766e"), Qt::CaseInsensitive));
}

QObject *createTestAverraCard()
{
    return new TestAverraCard;
}

#include "tst_AverraCard.moc"
