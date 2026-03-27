#include <Averra/AverraEmptyState.h>
#include <Averra/AverraThemeManager.h>

#include <QLabel>

#include <QtTest>

class TestAverraEmptyState : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreIconTitleAndDescription();
    void shouldHideDescriptionLabelWhenDescriptionIsEmpty();
    void shouldRefreshTitleStyleAfterThemeChange();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraEmptyState::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraEmptyState::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraEmptyState::shouldStoreIconTitleAndDescription()
{
    AverraEmptyState emptyState;

    emptyState.setIconText(QStringLiteral("A"));
    emptyState.setTitle(QStringLiteral("暂无数据"));
    emptyState.setDescription(QStringLiteral("请先创建一个项目，或者从模板导入。"));

    QCOMPARE(emptyState.iconText(), QStringLiteral("A"));
    QCOMPARE(emptyState.title(), QStringLiteral("暂无数据"));
    QCOMPARE(emptyState.description(), QStringLiteral("请先创建一个项目，或者从模板导入。"));
}

void TestAverraEmptyState::shouldHideDescriptionLabelWhenDescriptionIsEmpty()
{
    AverraEmptyState emptyState(QStringLiteral("暂无数据"));
    QLabel *descriptionLabel = emptyState.findChild<QLabel *>(QStringLiteral("AverraEmptyStateDescription"));

    QVERIFY(descriptionLabel != nullptr);
    emptyState.setDescription(QString());

    QVERIFY(descriptionLabel->isHidden());
}

void TestAverraEmptyState::shouldRefreshTitleStyleAfterThemeChange()
{
    AverraEmptyState emptyState(QStringLiteral("暂无数据"));

    AverraThemePalette palette = m_defaultPalette;
    palette.setTextPrimaryColor(QColor(QStringLiteral("#0F172A")));
    AverraThemeManager::instance()->setPalette(palette);

    QLabel *titleLabel = emptyState.findChild<QLabel *>(QStringLiteral("AverraEmptyStateTitle"));
    QVERIFY(titleLabel != nullptr);
    QVERIFY(titleLabel->styleSheet().contains(QStringLiteral("#0f172a"), Qt::CaseInsensitive));
}

QObject *createTestAverraEmptyState()
{
    return new TestAverraEmptyState;
}

#include "tst_AverraEmptyState.moc"
