#include <Averra/AverraPropertyItem.h>
#include <Averra/AverraThemeManager.h>

#include <QLabel>
#include <QPushButton>

#include <QtTest>

class TestAverraPropertyItem : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleAndDescription();
    void shouldAllowSettingAccessoryWidget();
    void shouldHideDescriptionWhenEmpty();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraPropertyItem::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraPropertyItem::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraPropertyItem::shouldStoreTitleAndDescription()
{
    AverraPropertyItem item;

    item.setTitle(QStringLiteral("自动更新"));
    item.setDescription(QStringLiteral("启动应用时自动检查新版本。"));

    QCOMPARE(item.title(), QStringLiteral("自动更新"));
    QCOMPARE(item.description(), QStringLiteral("启动应用时自动检查新版本。"));
}

void TestAverraPropertyItem::shouldAllowSettingAccessoryWidget()
{
    AverraPropertyItem item;
    QPushButton *button = new QPushButton(QStringLiteral("配置"));

    item.setAccessoryWidget(button);

    QCOMPARE(item.accessoryWidget(), button);
}

void TestAverraPropertyItem::shouldHideDescriptionWhenEmpty()
{
    AverraPropertyItem item(QStringLiteral("自动更新"));
    QLabel *descriptionLabel = item.findChild<QLabel *>(QStringLiteral("AverraPropertyItemDescription"));

    QVERIFY(descriptionLabel != nullptr);
    item.setDescription(QString());

    QVERIFY(descriptionLabel->isHidden());
}

QObject *createTestAverraPropertyItem()
{
    return new TestAverraPropertyItem;
}

#include "tst_AverraPropertyItem.moc"
