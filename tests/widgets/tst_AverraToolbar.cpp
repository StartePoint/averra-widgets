#include <Averra/AverraToolbar.h>
#include <Averra/AverraThemeManager.h>

#include <QHBoxLayout>
#include <QPushButton>

#include <QtTest>

class TestAverraToolbar : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleAndSubtitle();
    void shouldAllowAddingLeadingAndTrailingWidgets();
    void shouldEmitAccentChangedWhenAccentToggles();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraToolbar::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraToolbar::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraToolbar::shouldStoreTitleAndSubtitle()
{
    AverraToolbar toolbar;

    toolbar.setTitle(QStringLiteral("组件管理"));
    toolbar.setSubtitle(QStringLiteral("浏览当前工作区的所有组件"));

    QCOMPARE(toolbar.title(), QStringLiteral("组件管理"));
    QCOMPARE(toolbar.subtitle(), QStringLiteral("浏览当前工作区的所有组件"));
}

void TestAverraToolbar::shouldAllowAddingLeadingAndTrailingWidgets()
{
    AverraToolbar toolbar;
    QPushButton *leading = new QPushButton(QStringLiteral("L"));
    QPushButton *trailing = new QPushButton(QStringLiteral("R"));

    toolbar.addLeadingWidget(leading);
    toolbar.addTrailingWidget(trailing);

    QCOMPARE(toolbar.leadingLayout()->count(), 1);
    QCOMPARE(toolbar.trailingLayout()->count(), 1);
}

void TestAverraToolbar::shouldEmitAccentChangedWhenAccentToggles()
{
    AverraToolbar toolbar;
    QSignalSpy spy(&toolbar, &AverraToolbar::accentChanged);

    toolbar.setAccent(true);

    QCOMPARE(toolbar.accent(), true);
    QCOMPARE(spy.count(), 1);
}

QObject *createTestAverraToolbar()
{
    return new TestAverraToolbar;
}

#include "tst_AverraToolbar.moc"
