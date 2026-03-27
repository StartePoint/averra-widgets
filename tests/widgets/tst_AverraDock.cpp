#include <Averra/AverraDock.h>
#include <Averra/AverraThemeManager.h>

#include <QLabel>
#include <QMainWindow>
#include <QToolButton>
#include <QVBoxLayout>

#include <QtTest>

class TestAverraDock : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleAndDescription();
    void shouldAllowAddingContentWidgets();
    void shouldToggleOpenedState();
    void shouldRefreshCloseButtonStyleAfterThemeChange();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraDock::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraDock::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraDock::shouldStoreTitleAndDescription()
{
    AverraDock dock;

    dock.setTitle(QStringLiteral("属性面板"));
    dock.setDescription(QStringLiteral("用于编辑当前节点详情。"));

    QCOMPARE(dock.title(), QStringLiteral("属性面板"));
    QCOMPARE(dock.description(), QStringLiteral("用于编辑当前节点详情。"));
}

void TestAverraDock::shouldAllowAddingContentWidgets()
{
    AverraDock dock;
    QLabel *label = new QLabel(QStringLiteral("内容"));

    dock.addContentWidget(label);

    QCOMPARE(dock.contentLayout()->count(), 1);
    QCOMPARE(dock.contentWidget()->layout(), dock.contentLayout());
}

void TestAverraDock::shouldToggleOpenedState()
{
    QMainWindow window;
    AverraDock *dock = new AverraDock(&window);
    window.addDockWidget(Qt::RightDockWidgetArea, dock);
    window.show();
    QTest::qWait(20);

    dock->setOpened(false);
    QCOMPARE(dock->opened(), false);
    QVERIFY(dock->isHidden());

    dock->setOpened(true);
    QCOMPARE(dock->opened(), true);
    QVERIFY(dock->isVisible());
}

void TestAverraDock::shouldRefreshCloseButtonStyleAfterThemeChange()
{
    AverraDock dock;
    QToolButton *closeButton = dock.findChild<QToolButton *>(QStringLiteral("AverraDockCloseButton"));

    QVERIFY(closeButton != nullptr);

    AverraThemePalette palette = m_defaultPalette;
    palette.setSurfaceHoverColor(QColor(QStringLiteral("#F4F7FB")));
    palette.setSurfacePressedColor(QColor(QStringLiteral("#E7EDF6")));
    AverraThemeManager::instance()->setPalette(palette);

    QVERIFY(closeButton->styleSheet().contains(QStringLiteral("#f4f7fb"), Qt::CaseInsensitive));
    QVERIFY(closeButton->styleSheet().contains(QStringLiteral("#e7edf6"), Qt::CaseInsensitive));
}

QObject *createTestAverraDock()
{
    return new TestAverraDock;
}

#include "tst_AverraDock.moc"
