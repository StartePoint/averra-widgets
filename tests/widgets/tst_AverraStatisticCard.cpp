#include <Averra/AverraStatisticCard.h>
#include <Averra/AverraThemeManager.h>

#include <QFrame>
#include <QLabel>

#include <QtTest>

class TestAverraStatisticCard : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreCaptionValueAndBadge();
    void shouldEmitAccentChangedWhenAccentToggles();
    void shouldRefreshAccentStyleAfterThemeChange();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraStatisticCard::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraStatisticCard::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraStatisticCard::shouldStoreCaptionValueAndBadge()
{
    AverraStatisticCard card;

    card.setCaption(QStringLiteral("在线节点"));
    card.setValue(QStringLiteral("24"));
    card.setHelperText(QStringLiteral("较昨日 +3"));
    card.setBadgeText(QStringLiteral("Live"));

    QCOMPARE(card.caption(), QStringLiteral("在线节点"));
    QCOMPARE(card.value(), QStringLiteral("24"));
    QCOMPARE(card.helperText(), QStringLiteral("较昨日 +3"));
    QCOMPARE(card.badgeText(), QStringLiteral("Live"));
}

void TestAverraStatisticCard::shouldEmitAccentChangedWhenAccentToggles()
{
    AverraStatisticCard card;
    QSignalSpy spy(&card, &AverraStatisticCard::accentChanged);

    card.setAccent(true);

    QCOMPARE(card.accent(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestAverraStatisticCard::shouldRefreshAccentStyleAfterThemeChange()
{
    AverraStatisticCard card;
    card.setAccent(true);

    AverraThemePalette palette = m_defaultPalette;
    palette.setAccentColor(QColor(QStringLiteral("#2563EB")));
    palette.setAccentSurfaceColor(QColor(QStringLiteral("#DBEAFE")));
    AverraThemeManager::instance()->setPalette(palette);

    QFrame *rootFrame = card.findChild<QFrame *>(QStringLiteral("AverraStatisticCardRoot"));
    QVERIFY(rootFrame != nullptr);
    QVERIFY(rootFrame->styleSheet().contains(QStringLiteral("#2563eb"), Qt::CaseInsensitive));
}

QObject *createTestAverraStatisticCard()
{
    return new TestAverraStatisticCard;
}

#include "tst_AverraStatisticCard.moc"
