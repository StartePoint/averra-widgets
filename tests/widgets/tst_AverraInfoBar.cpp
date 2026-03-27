#include <Averra/AverraInfoBar.h>
#include <Averra/AverraThemeManager.h>

#include <QFrame>
#include <QLabel>
#include <QToolButton>

#include <QtTest>

class TestAverraInfoBar : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleAndDescription();
    void shouldEmitTypeChangedWhenTypeToggles();
    void shouldHideCloseButtonWhenClosableIsFalse();
    void shouldHideAndEmitDismissedWhenCloseButtonClicked();
    void shouldUseThemeStateColorsForCloseButtonStyle();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraInfoBar::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraInfoBar::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraInfoBar::shouldStoreTitleAndDescription()
{
    AverraInfoBar infoBar;

    infoBar.setTitle(QStringLiteral("更新成功"));
    infoBar.setDescription(QStringLiteral("你的设置已保存。"));

    QCOMPARE(infoBar.title(), QStringLiteral("更新成功"));
    QCOMPARE(infoBar.description(), QStringLiteral("你的设置已保存。"));
}

void TestAverraInfoBar::shouldEmitTypeChangedWhenTypeToggles()
{
    AverraInfoBar infoBar;
    QSignalSpy spy(&infoBar, &AverraInfoBar::typeChanged);

    infoBar.setType(AverraInfoBar::Success);

    QCOMPARE(infoBar.type(), AverraInfoBar::Success);
    QCOMPARE(spy.count(), 1);
}

void TestAverraInfoBar::shouldHideCloseButtonWhenClosableIsFalse()
{
    AverraInfoBar infoBar;
    QToolButton *closeButton = infoBar.findChild<QToolButton *>(QStringLiteral("AverraInfoBarCloseButton"));

    QVERIFY(closeButton != nullptr);
    infoBar.setClosable(false);

    QCOMPARE(infoBar.closable(), false);
    QVERIFY(closeButton->isHidden());
}

void TestAverraInfoBar::shouldHideAndEmitDismissedWhenCloseButtonClicked()
{
    AverraInfoBar infoBar;
    QToolButton *closeButton = infoBar.findChild<QToolButton *>(QStringLiteral("AverraInfoBarCloseButton"));
    QSignalSpy dismissedSpy(&infoBar, &AverraInfoBar::dismissed);

    QVERIFY(closeButton != nullptr);
    infoBar.show();
    QTest::mouseClick(closeButton, Qt::LeftButton);

    QCOMPARE(dismissedSpy.count(), 1);
    QVERIFY(infoBar.isHidden());
}

void TestAverraInfoBar::shouldUseThemeStateColorsForCloseButtonStyle()
{
    AverraInfoBar infoBar;
    QToolButton *closeButton = infoBar.findChild<QToolButton *>(QStringLiteral("AverraInfoBarCloseButton"));

    QVERIFY(closeButton != nullptr);

    AverraThemePalette palette = m_defaultPalette;
    palette.setSurfaceHoverColor(QColor(QStringLiteral("#F4F7FB")));
    palette.setSurfacePressedColor(QColor(QStringLiteral("#E5ECF5")));
    AverraThemeManager::instance()->setPalette(palette);

    QVERIFY(closeButton->styleSheet().contains(QStringLiteral("#f4f7fb"), Qt::CaseInsensitive));
    QVERIFY(closeButton->styleSheet().contains(QStringLiteral("#e5ecf5"), Qt::CaseInsensitive));
}

QObject *createTestAverraInfoBar()
{
    return new TestAverraInfoBar;
}

#include "tst_AverraInfoBar.moc"
