#include <Averra/AverraPagination.h>
#include <Averra/AverraThemeManager.h>

#include <QtTest>

class TestAverraPagination : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldUseSinglePageByDefault();
    void shouldEmitCurrentPageChangedWhenPageUpdates();
    void shouldClampCurrentPageToPageCount();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraPagination::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraPagination::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraPagination::shouldUseSinglePageByDefault()
{
    AverraPagination pagination;

    QCOMPARE(pagination.currentPage(), 1);
    QCOMPARE(pagination.pageCount(), 1);
}

void TestAverraPagination::shouldEmitCurrentPageChangedWhenPageUpdates()
{
    AverraPagination pagination;
    QSignalSpy spy(&pagination, &AverraPagination::currentPageChanged);

    pagination.setPageCount(5);
    pagination.setCurrentPage(3);

    QCOMPARE(pagination.currentPage(), 3);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toInt(), 3);
}

void TestAverraPagination::shouldClampCurrentPageToPageCount()
{
    AverraPagination pagination;
    pagination.setPageCount(3);
    pagination.setCurrentPage(3);
    pagination.setPageCount(2);

    QCOMPARE(pagination.currentPage(), 2);
}

QObject *createTestAverraPagination()
{
    return new TestAverraPagination;
}

#include "tst_AverraPagination.moc"
