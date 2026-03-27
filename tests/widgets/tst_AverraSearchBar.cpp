#include <Averra/AverraSearchBar.h>
#include <Averra/AverraThemeManager.h>

#include <QLineEdit>

#include <QtTest>

class TestAverraSearchBar : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTextAndPlaceholder();
    void shouldEmitSearchRequestedWhenReturnPressed();
    void shouldEmitAccentFrameChangedWhenToggled();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraSearchBar::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSearchBar::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraSearchBar::shouldStoreTextAndPlaceholder()
{
    AverraSearchBar searchBar;

    searchBar.setText(QStringLiteral("widgets"));
    searchBar.setPlaceholderText(QStringLiteral("搜索组件"));

    QCOMPARE(searchBar.text(), QStringLiteral("widgets"));
    QCOMPARE(searchBar.placeholderText(), QStringLiteral("搜索组件"));
}

void TestAverraSearchBar::shouldEmitSearchRequestedWhenReturnPressed()
{
    AverraSearchBar searchBar;
    QSignalSpy spy(&searchBar, &AverraSearchBar::searchRequested);
    QLineEdit *lineEdit = searchBar.findChild<QLineEdit *>(QStringLiteral("AverraSearchBarLineEdit"));

    QVERIFY(lineEdit != nullptr);
    searchBar.show();
    lineEdit->setText(QStringLiteral("theme"));
    QTest::keyClick(lineEdit, Qt::Key_Return);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QStringLiteral("theme"));
}

void TestAverraSearchBar::shouldEmitAccentFrameChangedWhenToggled()
{
    AverraSearchBar searchBar;
    QSignalSpy spy(&searchBar, &AverraSearchBar::accentFrameChanged);

    searchBar.setAccentFrame(true);

    QCOMPARE(searchBar.accentFrame(), true);
    QCOMPARE(spy.count(), 1);
}

QObject *createTestAverraSearchBar()
{
    return new TestAverraSearchBar;
}

#include "tst_AverraSearchBar.moc"
