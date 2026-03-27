#include <Averra/AverraFilterChipGroup.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraFilterChipGroup : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldAddChipsAndKeepSingleSelectionByDefault();
    void shouldAllowMultiSelectWhenEnabled();
    void shouldEmitSelectedIndicesChangedWhenSelectionChanges();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraFilterChipGroup::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraFilterChipGroup::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraFilterChipGroup::shouldAddChipsAndKeepSingleSelectionByDefault()
{
    AverraFilterChipGroup group;
    group.addChip(QStringLiteral("全部"));
    group.addChip(QStringLiteral("按钮"));
    group.addChip(QStringLiteral("表单"));

    QCOMPARE(group.chipCount(), 3);
    QCOMPARE(group.selectedIndices(), QList<int>() << 0);
}

void TestAverraFilterChipGroup::shouldAllowMultiSelectWhenEnabled()
{
    AverraFilterChipGroup group;
    group.addChip(QStringLiteral("全部"));
    group.addChip(QStringLiteral("按钮"));
    group.addChip(QStringLiteral("表单"));
    group.setMultiSelect(true);
    group.setSelectedIndices(QList<int>() << 1 << 2);

    QCOMPARE(group.multiSelect(), true);
    QCOMPARE(group.selectedIndices(), QList<int>({1, 2}));
}

void TestAverraFilterChipGroup::shouldEmitSelectedIndicesChangedWhenSelectionChanges()
{
    AverraFilterChipGroup group;
    QSignalSpy spy(&group, &AverraFilterChipGroup::selectedIndicesChanged);

    group.addChip(QStringLiteral("全部"));
    group.addChip(QStringLiteral("按钮"));
    group.setSelectedIndices(QList<int>() << 1);

    QVERIFY(spy.count() >= 1);
    QCOMPARE(group.selectedIndices(), QList<int>() << 1);
}

QObject *createTestAverraFilterChipGroup()
{
    return new TestAverraFilterChipGroup;
}

#include "tst_AverraFilterChipGroup.moc"
