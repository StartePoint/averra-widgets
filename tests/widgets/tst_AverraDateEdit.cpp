#include <Averra/AverraDateEdit.h>

#include <QAbstractButton>
#include <QDate>
#include <QLabel>
#include <QWidget>

#include <QtTest>

class TestAverraDateEdit : public QObject
{
    Q_OBJECT

private slots:
    void shouldUseCurrentDateByDefault();
    void shouldToggleAccentFrame();
    void shouldStyleCalendarPopupWithAverraTheme();
};

void TestAverraDateEdit::shouldUseCurrentDateByDefault()
{
    AverraDateEdit edit;
    QVERIFY(edit.date().isValid());
}

void TestAverraDateEdit::shouldToggleAccentFrame()
{
    AverraDateEdit edit;
    edit.setAccentFrame(true);
    QCOMPARE(edit.accentFrame(), true);
}

void TestAverraDateEdit::shouldStyleCalendarPopupWithAverraTheme()
{
    AverraDateEdit edit;
    edit.setDate(QDate(2020, 1, 15));
    edit.resize(180, 42);
    edit.show();

    QVERIFY(edit.calendarPopup());
    QTest::mouseClick(&edit,
                      Qt::LeftButton,
                      Qt::NoModifier,
                      QPoint(edit.width() - 8, edit.height() / 2));

    QWidget *popup = edit.findChild<QWidget *>(QStringLiteral("AverraDateEditPopup"));
    QVERIFY(popup != nullptr);
    QTRY_VERIFY(popup->isVisible());
    QVERIFY(popup->findChild<QLabel *>(QStringLiteral("AverraDateEditPopupTitle")) != nullptr);
    QVERIFY(popup->findChild<QAbstractButton *>(QStringLiteral("AverraDateEditPopupTodayButton")) != nullptr);

    const QList<QAbstractButton *> dayButtons =
        popup->findChildren<QAbstractButton *>(QStringLiteral("AverraDateEditPopupDayButton"));
    QCOMPARE(dayButtons.size(), 42);

    edit.setAccentFrame(true);
    QTest::mouseClick(popup->findChild<QAbstractButton *>(QStringLiteral("AverraDateEditPopupTodayButton")),
                      Qt::LeftButton);
    QCOMPARE(edit.date(), QDate::currentDate());
}

QObject *createTestAverraDateEdit()
{
    return new TestAverraDateEdit;
}

#include "tst_AverraDateEdit.moc"
