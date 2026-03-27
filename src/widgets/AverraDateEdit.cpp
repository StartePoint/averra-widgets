#include <Averra/AverraDateEdit.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QAbstractButton>
#include <QDate>
#include <QFrame>
#include <QGuiApplication>
#include <QGridLayout>
#include <QHideEvent>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLocale>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPointer>
#include <QResizeEvent>
#include <QScreen>
#include <QShowEvent>
#include <QSizePolicy>
#include <QVBoxLayout>

namespace
{
class AverraCalendarHeaderButton : public QAbstractButton
{
public:
    enum Role
    {
        PreviousYearRole,
        PreviousMonthRole,
        TodayRole,
        NextMonthRole,
        NextYearRole
    };

    explicit AverraCalendarHeaderButton(Role role, QWidget *parent = nullptr)
        : QAbstractButton(parent)
    {
        m_role = role;
        m_accentFrame = false;

        setCursor(Qt::PointingHandCursor);
        setFocusPolicy(Qt::NoFocus);
        setMouseTracking(true);
        setObjectName(QStringLiteral("AverraDateEditPopupHeaderButton"));

        if (m_role == TodayRole) {
            setObjectName(QStringLiteral("AverraDateEditPopupTodayButton"));
            setText(QStringLiteral("今天"));
        }

        connect(AverraThemeManager::instance(),
                &AverraThemeManager::paletteChanged,
                this,
                [this]() {
                    update();
                });
    }

    void setAccentFrame(bool accentFrame)
    {
        if (m_accentFrame == accentFrame) {
            return;
        }

        m_accentFrame = accentFrame;
        update();
    }

    QSize sizeHint() const override
    {
        if (m_role == TodayRole) {
            return QSize(66, 30);
        }

        return QSize(30, 30);
    }

protected:
    void enterEvent(QEvent *event) override
    {
        QAbstractButton::enterEvent(event);
        update();
    }

    void leaveEvent(QEvent *event) override
    {
        QAbstractButton::leaveEvent(event);
        update();
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        QAbstractButton::mousePressEvent(event);
        update();
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        QAbstractButton::mouseReleaseEvent(event);
        update();
    }

    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event)

        const AverraThemePalette palette = AverraThemeManager::instance()->palette();
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        QRectF buttonRect = rect().adjusted(1.0, 1.0, -1.0, -1.0);
        QColor backgroundColor = palette.surfaceRaisedColor();
        QColor borderColor = m_accentFrame ? palette.accentColor() : palette.borderColor();
        QColor textColor = palette.textPrimaryColor();

        if (!isEnabled()) {
            backgroundColor = palette.surfaceDisabledColor();
            borderColor = palette.surfaceDisabledColor();
            textColor = palette.textDisabledColor();
        } else if (isDown()) {
            backgroundColor = palette.surfacePressedColor();
            borderColor = palette.borderHoverColor();
            textColor = palette.accentColor();
        } else if (underMouse()) {
            backgroundColor = palette.surfaceHoverColor();
            borderColor = palette.borderHoverColor();
            textColor = palette.accentColor();
        }

        painter.setPen(QPen(borderColor, 1.0));
        painter.setBrush(backgroundColor);
        painter.drawRoundedRect(buttonRect, 9.0, 9.0);

        if (m_role == TodayRole) {
            QFont font = painter.font();
            font.setBold(true);
            painter.setFont(font);
            painter.setPen(textColor);
            painter.drawText(rect(), Qt::AlignCenter, text());
            return;
        }

        painter.setPen(QPen(textColor, 2.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        const qreal centerY = rect().center().y();
        const qreal leftX = rect().center().x() - 4.0;
        const qreal rightX = rect().center().x() + 4.0;

        auto drawChevronLeft = [&painter, leftX, rightX, centerY]() {
            painter.drawLine(QPointF(rightX, centerY - 5.0), QPointF(leftX, centerY));
            painter.drawLine(QPointF(leftX, centerY), QPointF(rightX, centerY + 5.0));
        };

        auto drawChevronRight = [&painter, leftX, rightX, centerY]() {
            painter.drawLine(QPointF(leftX, centerY - 5.0), QPointF(rightX, centerY));
            painter.drawLine(QPointF(rightX, centerY), QPointF(leftX, centerY + 5.0));
        };

        if (m_role == PreviousMonthRole) {
            drawChevronLeft();
        } else if (m_role == NextMonthRole) {
            drawChevronRight();
        } else if (m_role == PreviousYearRole) {
            painter.save();
            painter.translate(-3.0, 0.0);
            drawChevronLeft();
            painter.restore();
            painter.save();
            painter.translate(3.0, 0.0);
            drawChevronLeft();
            painter.restore();
        } else if (m_role == NextYearRole) {
            painter.save();
            painter.translate(-3.0, 0.0);
            drawChevronRight();
            painter.restore();
            painter.save();
            painter.translate(3.0, 0.0);
            drawChevronRight();
            painter.restore();
        }
    }

private:
    Role m_role;
    bool m_accentFrame;
};

class AverraCalendarDayButton : public QAbstractButton
{
public:
    explicit AverraCalendarDayButton(QWidget *parent = nullptr)
        : QAbstractButton(parent)
    {
        m_currentMonth = true;
        m_selected = false;
        m_today = false;
        m_accentFrame = false;

        setCursor(Qt::PointingHandCursor);
        setFocusPolicy(Qt::NoFocus);
        setMouseTracking(true);
        setObjectName(QStringLiteral("AverraDateEditPopupDayButton"));

        connect(AverraThemeManager::instance(),
                &AverraThemeManager::paletteChanged,
                this,
                [this]() {
                    update();
                });
    }

    void setAccentFrame(bool accentFrame)
    {
        if (m_accentFrame == accentFrame) {
            return;
        }

        m_accentFrame = accentFrame;
        update();
    }

    void setDateInfo(const QDate &date, bool currentMonth, bool selected, bool today, bool enabled)
    {
        m_date = date;
        m_currentMonth = currentMonth;
        m_selected = selected;
        m_today = today;
        setEnabled(enabled);
        update();
    }

    QDate date() const
    {
        return m_date;
    }

    QSize sizeHint() const override
    {
        return QSize(38, 34);
    }

protected:
    void enterEvent(QEvent *event) override
    {
        QAbstractButton::enterEvent(event);
        update();
    }

    void leaveEvent(QEvent *event) override
    {
        QAbstractButton::leaveEvent(event);
        update();
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        QAbstractButton::mousePressEvent(event);
        update();
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        QAbstractButton::mouseReleaseEvent(event);
        update();
    }

    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event)

        const AverraThemePalette palette = AverraThemeManager::instance()->palette();
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        QRectF cellRect = rect().adjusted(2.0, 2.0, -2.0, -2.0);
        QColor backgroundColor(Qt::transparent);
        QColor borderColor(Qt::transparent);
        QColor textColor = palette.textPrimaryColor();

        if (!isEnabled()) {
            textColor = palette.textDisabledColor();
        } else if (m_selected) {
            backgroundColor = m_accentFrame ? palette.accentHoverColor() : palette.accentColor();
            textColor = QColor(QStringLiteral("#FFFFFF"));
        } else if (isDown()) {
            backgroundColor = palette.surfacePressedColor();
            textColor = palette.accentColor();
        } else if (underMouse()) {
            backgroundColor = palette.accentSurfaceColor();
            textColor = palette.accentColor();
        } else if (!m_currentMonth) {
            textColor = palette.textSecondaryColor();
        }

        if (m_today && !m_selected) {
            borderColor = palette.accentColor();
        }

        if (backgroundColor.alpha() > 0) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(backgroundColor);
            painter.drawRoundedRect(cellRect, 10.0, 10.0);
        }

        if (borderColor.alpha() > 0) {
            painter.setPen(QPen(borderColor, 1.2));
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundedRect(cellRect, 10.0, 10.0);
        }

        QFont font = painter.font();
        font.setBold(m_selected || m_today);
        painter.setFont(font);
        painter.setPen(textColor);
        painter.drawText(rect(), Qt::AlignCenter, QString::number(m_date.day()));
    }

private:
    QDate m_date;
    bool m_currentMonth;
    bool m_selected;
    bool m_today;
    bool m_accentFrame;
};
}

class AverraDatePopup : public QFrame
{
public:
    explicit AverraDatePopup(AverraDateEdit *owner);

    void setAccentFrame(bool accentFrame);
    void syncFromEditor();
    void showRelativeTo();
    void repositionRelativeTo();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initialize();
    void refreshTheme();
    void refreshHeader();
    void refreshWeekdayLabels();
    void refreshDayButtons();
    void shiftMonth(int monthOffset);
    QDate clampedMonth(const QDate &monthDate) const;

    AverraDateEdit *m_owner;
    bool m_accentFrame;
    QDate m_displayedMonth;
    QWidget *m_headerWidget;
    AverraCalendarHeaderButton *m_previousYearButton;
    AverraCalendarHeaderButton *m_previousMonthButton;
    QLabel *m_titleLabel;
    AverraCalendarHeaderButton *m_todayButton;
    AverraCalendarHeaderButton *m_nextMonthButton;
    AverraCalendarHeaderButton *m_nextYearButton;
    QList<QLabel *> m_weekdayLabels;
    QList<AverraCalendarDayButton *> m_dayButtons;
};

AverraDatePopup::AverraDatePopup(AverraDateEdit *owner)
{
    m_owner = owner;
    m_accentFrame = false;
    m_displayedMonth = QDate::currentDate();

    setParent(owner);
    initialize();
}

void AverraDatePopup::setAccentFrame(bool accentFrame)
{
    if (m_accentFrame == accentFrame) {
        return;
    }

    m_accentFrame = accentFrame;

    for (int index = 0; index < m_dayButtons.size(); ++index) {
        m_dayButtons.at(index)->setAccentFrame(m_accentFrame);
    }

    m_previousYearButton->setAccentFrame(m_accentFrame);
    m_previousMonthButton->setAccentFrame(m_accentFrame);
    m_todayButton->setAccentFrame(m_accentFrame);
    m_nextMonthButton->setAccentFrame(m_accentFrame);
    m_nextYearButton->setAccentFrame(m_accentFrame);
    refreshTheme();
}

void AverraDatePopup::syncFromEditor()
{
    if (m_owner == nullptr) {
        return;
    }

    m_displayedMonth = QDate(m_owner->date().year(), m_owner->date().month(), 1);
    refreshHeader();
    refreshWeekdayLabels();
    refreshDayButtons();
}

void AverraDatePopup::showRelativeTo()
{
    syncFromEditor();
    adjustSize();
    repositionRelativeTo();
    show();
    raise();
    activateWindow();
}

void AverraDatePopup::repositionRelativeTo()
{
    if (m_owner == nullptr) {
        return;
    }

    adjustSize();

    const QPoint ownerBottomLeft = m_owner->mapToGlobal(QPoint(0, m_owner->height() + 6));
    const QPoint ownerTopLeft = m_owner->mapToGlobal(QPoint(0, -height() - 6));
    QScreen *screen = QGuiApplication::screenAt(m_owner->mapToGlobal(QPoint(m_owner->width() / 2,
                                                                             m_owner->height() / 2)));

    if (screen == nullptr) {
        screen = QGuiApplication::primaryScreen();
    }

    QRect availableGeometry = screen != nullptr ? screen->availableGeometry()
                                                : QRect(ownerBottomLeft, QSize(800, 600));

    int popupX = ownerBottomLeft.x();
    int popupY = ownerBottomLeft.y();

    if (popupX + width() > availableGeometry.right()) {
        popupX = qMax(availableGeometry.left(), availableGeometry.right() - width() + 1);
    }

    if (popupY + height() > availableGeometry.bottom()) {
        popupY = ownerTopLeft.y();
    }

    if (popupY < availableGeometry.top()) {
        popupY = qMax(availableGeometry.top(), availableGeometry.bottom() - height() + 1);
    }

    move(QPoint(popupX, popupY));
}

void AverraDatePopup::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF popupRect = rect().adjusted(0.5, 0.5, -0.5, -0.5);
    painter.setPen(QPen(palette.borderColor(), 1.0));
    painter.setBrush(palette.surfaceRaisedColor());
    painter.drawRoundedRect(popupRect, 18.0, 18.0);
}

void AverraDatePopup::initialize()
{
    setObjectName(QStringLiteral("AverraDateEditPopup"));
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMinimumWidth(324);

    m_headerWidget = new QWidget(this);
    m_previousYearButton = new AverraCalendarHeaderButton(AverraCalendarHeaderButton::PreviousYearRole,
                                                          m_headerWidget);
    m_previousMonthButton = new AverraCalendarHeaderButton(AverraCalendarHeaderButton::PreviousMonthRole,
                                                           m_headerWidget);
    m_titleLabel = new QLabel(m_headerWidget);
    m_todayButton = new AverraCalendarHeaderButton(AverraCalendarHeaderButton::TodayRole,
                                                   m_headerWidget);
    m_nextMonthButton = new AverraCalendarHeaderButton(AverraCalendarHeaderButton::NextMonthRole,
                                                       m_headerWidget);
    m_nextYearButton = new AverraCalendarHeaderButton(AverraCalendarHeaderButton::NextYearRole,
                                                      m_headerWidget);

    m_titleLabel->setObjectName(QStringLiteral("AverraDateEditPopupTitle"));
    m_titleLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *headerLayout = new QHBoxLayout(m_headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(8);
    headerLayout->addWidget(m_previousYearButton);
    headerLayout->addWidget(m_previousMonthButton);
    headerLayout->addWidget(m_titleLabel, 1);
    headerLayout->addWidget(m_todayButton);
    headerLayout->addWidget(m_nextMonthButton);
    headerLayout->addWidget(m_nextYearButton);

    QWidget *gridWidget = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(gridWidget);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setHorizontalSpacing(4);
    gridLayout->setVerticalSpacing(4);

    for (int column = 0; column < 7; ++column) {
        QLabel *weekdayLabel = new QLabel(gridWidget);
        weekdayLabel->setAlignment(Qt::AlignCenter);
        weekdayLabel->setObjectName(QStringLiteral("AverraDateEditPopupWeekdayLabel"));
        m_weekdayLabels.append(weekdayLabel);
        gridLayout->addWidget(weekdayLabel, 0, column);
    }

    for (int row = 0; row < 6; ++row) {
        for (int column = 0; column < 7; ++column) {
            AverraCalendarDayButton *dayButton = new AverraCalendarDayButton(gridWidget);
            m_dayButtons.append(dayButton);
            gridLayout->addWidget(dayButton, row + 1, column);

            connect(dayButton,
                    &QAbstractButton::clicked,
                    this,
                    [this, dayButton]() {
                        if (m_owner == nullptr || !dayButton->isEnabled()) {
                            return;
                        }

                        m_owner->setDate(dayButton->date());
                        hide();
                    });
        }
    }

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(12, 12, 12, 12);
    rootLayout->setSpacing(10);
    rootLayout->addWidget(m_headerWidget);
    rootLayout->addWidget(gridWidget);

    connect(m_previousYearButton,
            &QAbstractButton::clicked,
            this,
            [this]() {
                shiftMonth(-12);
            });
    connect(m_previousMonthButton,
            &QAbstractButton::clicked,
            this,
            [this]() {
                shiftMonth(-1);
            });
    connect(m_todayButton,
            &QAbstractButton::clicked,
            this,
            [this]() {
                if (m_owner == nullptr) {
                    return;
                }

                const QDate today = QDate::currentDate();
                m_owner->setDate(today);
                m_displayedMonth = QDate(today.year(), today.month(), 1);
                refreshHeader();
                refreshDayButtons();
                hide();
            });
    connect(m_nextMonthButton,
            &QAbstractButton::clicked,
            this,
            [this]() {
                shiftMonth(1);
            });
    connect(m_nextYearButton,
            &QAbstractButton::clicked,
            this,
            [this]() {
                shiftMonth(12);
            });
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            [this]() {
                refreshTheme();
            });

    refreshTheme();
    refreshWeekdayLabels();
    refreshHeader();
    refreshDayButtons();
}

void AverraDatePopup::refreshTheme()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    m_titleLabel->setStyleSheet(
        QStringLiteral(
            "QLabel#AverraDateEditPopupTitle {"
            "color: %1;"
            "background: transparent;"
            "font-size: 14px;"
            "font-weight: 700;"
            "}")
            .arg(palette.textPrimaryColor().name()));

    for (int index = 0; index < m_weekdayLabels.size(); ++index) {
        m_weekdayLabels.at(index)->setStyleSheet(
            QStringLiteral(
                "QLabel#AverraDateEditPopupWeekdayLabel {"
                "color: %1;"
                "background: transparent;"
                "font-size: 11px;"
                "font-weight: 700;"
                "padding: 4px 0;"
                "}")
                .arg(palette.textSecondaryColor().name()));
    }

    update();
}

void AverraDatePopup::refreshHeader()
{
    const QLocale locale = QLocale::system();
    QString monthText = locale.standaloneMonthName(m_displayedMonth.month(), QLocale::LongFormat);

    if (monthText.trimmed().isEmpty()) {
        monthText = QStringLiteral("%1月").arg(m_displayedMonth.month());
    }

    m_titleLabel->setText(QStringLiteral("%1 %2").arg(QString::number(m_displayedMonth.year()), monthText));

    const QDate minimumMonth(m_owner->minimumDate().year(), m_owner->minimumDate().month(), 1);
    const QDate maximumMonth(m_owner->maximumDate().year(), m_owner->maximumDate().month(), 1);

    m_previousYearButton->setEnabled(m_displayedMonth.addYears(-1) >= minimumMonth);
    m_previousMonthButton->setEnabled(m_displayedMonth.addMonths(-1) >= minimumMonth);
    m_nextMonthButton->setEnabled(m_displayedMonth.addMonths(1) <= maximumMonth);
    m_nextYearButton->setEnabled(m_displayedMonth.addYears(1) <= maximumMonth);
}

void AverraDatePopup::refreshWeekdayLabels()
{
    const QLocale locale = QLocale::system();
    const Qt::DayOfWeek firstDayOfWeek = locale.firstDayOfWeek();

    for (int index = 0; index < m_weekdayLabels.size(); ++index) {
        const int weekdayValue = ((static_cast<int>(firstDayOfWeek) - 1 + index) % 7) + 1;
        QString weekdayText = locale.standaloneDayName(weekdayValue, QLocale::ShortFormat);

        if (weekdayText.trimmed().isEmpty()) {
            weekdayText = QStringLiteral("D%1").arg(weekdayValue);
        }

        m_weekdayLabels.at(index)->setText(weekdayText);
    }
}

void AverraDatePopup::refreshDayButtons()
{
    const QDate selectedDate = m_owner->date();
    const QDate firstDayOfMonth(m_displayedMonth.year(), m_displayedMonth.month(), 1);
    const QLocale locale = QLocale::system();
    const int firstDayValue = static_cast<int>(locale.firstDayOfWeek());
    int offset = firstDayOfMonth.dayOfWeek() - firstDayValue;

    if (offset < 0) {
        offset += 7;
    }

    const QDate gridStart = firstDayOfMonth.addDays(-offset);
    const QDate today = QDate::currentDate();

    for (int index = 0; index < m_dayButtons.size(); ++index) {
        const QDate dayDate = gridStart.addDays(index);
        const bool currentMonth = dayDate.month() == m_displayedMonth.month()
                                  && dayDate.year() == m_displayedMonth.year();
        const bool enabled = dayDate >= m_owner->minimumDate() && dayDate <= m_owner->maximumDate();
        const bool selected = dayDate == selectedDate;
        const bool isToday = dayDate == today;

        m_dayButtons.at(index)->setAccentFrame(m_accentFrame);
        m_dayButtons.at(index)->setDateInfo(dayDate, currentMonth, selected, isToday, enabled);
    }
}

void AverraDatePopup::shiftMonth(int monthOffset)
{
    m_displayedMonth = clampedMonth(m_displayedMonth.addMonths(monthOffset));
    refreshHeader();
    refreshDayButtons();
}

QDate AverraDatePopup::clampedMonth(const QDate &monthDate) const
{
    const QDate minimumMonth(m_owner->minimumDate().year(), m_owner->minimumDate().month(), 1);
    const QDate maximumMonth(m_owner->maximumDate().year(), m_owner->maximumDate().month(), 1);
    QDate normalizedMonth(monthDate.year(), monthDate.month(), 1);

    if (!normalizedMonth.isValid()) {
        normalizedMonth = QDate(m_owner->date().year(), m_owner->date().month(), 1);
    }

    if (normalizedMonth < minimumMonth) {
        return minimumMonth;
    }

    if (normalizedMonth > maximumMonth) {
        return maximumMonth;
    }

    return normalizedMonth;
}

AverraDateEdit::AverraDateEdit(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

bool AverraDateEdit::accentFrame() const
{
    return m_accentFrame;
}

void AverraDateEdit::setAccentFrame(bool accentFrame)
{
    if (m_accentFrame == accentFrame) {
        return;
    }

    m_accentFrame = accentFrame;
    refreshStyle();
    emit accentFrameChanged(m_accentFrame);
}

void AverraDateEdit::mousePressEvent(QMouseEvent *event)
{
    if (event != nullptr
        && event->button() == Qt::LeftButton
        && dropDownRectContains(event->pos())) {
        m_dropDownPressed = true;
        event->accept();
        return;
    }

    QDateEdit::mousePressEvent(event);
}

void AverraDateEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dropDownPressed && event != nullptr && event->button() == Qt::LeftButton) {
        const bool shouldToggle = dropDownRectContains(event->pos());
        m_dropDownPressed = false;

        if (shouldToggle) {
            togglePopup();
        }

        event->accept();
        return;
    }

    QDateEdit::mouseReleaseEvent(event);
}

void AverraDateEdit::keyPressEvent(QKeyEvent *event)
{
    if (event != nullptr) {
        const bool openPopupShortcut = event->key() == Qt::Key_F4
                                       || (event->key() == Qt::Key_Down
                                           && (event->modifiers() & Qt::AltModifier));

        if (openPopupShortcut) {
            togglePopup();
            event->accept();
            return;
        }

        if (event->key() == Qt::Key_Escape && popupVisible()) {
            hidePopup();
            event->accept();
            return;
        }
    }

    QDateEdit::keyPressEvent(event);
}

void AverraDateEdit::hideEvent(QHideEvent *event)
{
    hidePopup();
    QDateEdit::hideEvent(event);
}

void AverraDateEdit::resizeEvent(QResizeEvent *event)
{
    QDateEdit::resizeEvent(event);

    if (popupVisible()) {
        m_popup->repositionRelativeTo();
    }
}

void AverraDateEdit::moveEvent(QMoveEvent *event)
{
    QDateEdit::moveEvent(event);

    if (popupVisible()) {
        m_popup->repositionRelativeTo();
    }
}

void AverraDateEdit::initialize()
{
    m_accentFrame = false;
    m_dropDownPressed = false;

    setDate(QDate::currentDate());
    setCalendarPopup(true);
    setDisplayFormat(QStringLiteral("yyyy-MM-dd"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(42);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraDateEdit::refreshStyle);
    connect(this,
            &QDateEdit::dateChanged,
            this,
            [this](const QDate &) {
                syncPopupState();
            });

    refreshStyle();
}

void AverraDateEdit::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::dateEditStyleSheet(palette, m_accentFrame));
    refreshPopupStyle();
}

void AverraDateEdit::ensurePopup()
{
    if (m_popup != nullptr) {
        return;
    }

    m_popup = new AverraDatePopup(this);
    m_popup->setAccentFrame(m_accentFrame);
    m_popup->syncFromEditor();
}

void AverraDateEdit::refreshPopupStyle()
{
    ensurePopup();

    if (m_popup != nullptr) {
        m_popup->setAccentFrame(m_accentFrame);
    }
}

void AverraDateEdit::syncPopupState()
{
    if (m_popup != nullptr) {
        m_popup->syncFromEditor();
    }
}

void AverraDateEdit::togglePopup()
{
    if (popupVisible()) {
        hidePopup();
        return;
    }

    showPopup();
}

void AverraDateEdit::showPopup()
{
    ensurePopup();

    if (m_popup != nullptr) {
        m_popup->showRelativeTo();
    }
}

void AverraDateEdit::hidePopup()
{
    if (m_popup != nullptr) {
        m_popup->hide();
    }
}

bool AverraDateEdit::popupVisible() const
{
    return m_popup != nullptr && m_popup->isVisible();
}

bool AverraDateEdit::dropDownRectContains(const QPoint &position) const
{
    const QRect dropDownRect(width() - 34, 0, 34, height());
    return dropDownRect.contains(position);
}
