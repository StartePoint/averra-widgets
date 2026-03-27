#include <Averra/AverraSwitch.h>
#include <Averra/AverraThemeManager.h>

#include <QEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QSizePolicy>

AverraSwitch::AverraSwitch(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QSize AverraSwitch::sizeHint() const
{
    return QSize(52, 30);
}

void AverraSwitch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    QColor trackColor;
    QColor borderColor;
    QColor thumbColor;
    QColor thumbBorderColor;

    if (isEnabled()) {
        if (isChecked()) {
            trackColor = palette.accentColor();
            borderColor = palette.accentColor();
            thumbColor = QColor(QStringLiteral("#FFFFFF"));
            thumbBorderColor = QColor(QStringLiteral("#D5DBE5"));
        } else {
            trackColor = palette.surfaceColor().darker(104);
            borderColor = palette.borderColor().darker(110);
            thumbColor = palette.surfaceRaisedColor();
            thumbBorderColor = palette.borderColor().darker(120);
        }
    } else {
        trackColor = palette.surfaceColor();
        borderColor = palette.borderColor();
        thumbColor = palette.surfaceRaisedColor();
        thumbBorderColor = palette.borderColor();
    }

    if (m_hovered && isEnabled()) {
        trackColor = trackColor.lighter(105);
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF trackRect(1.0, 1.0, width() - 2.0, height() - 2.0);
    painter.setPen(QPen(borderColor, 1.0));
    painter.setBrush(trackColor);
    painter.drawRoundedRect(trackRect, trackRect.height() / 2.0, trackRect.height() / 2.0);

    const qreal margin = 3.0;
    const qreal thumbSize = height() - margin * 2.0;
    qreal thumbX = margin;

    if (isChecked()) {
        thumbX = width() - margin - thumbSize;
    }

    const QRectF thumbRect(thumbX, margin, thumbSize, thumbSize);
    painter.setPen(QPen(thumbBorderColor, 1.0));
    painter.setBrush(thumbColor);
    painter.drawEllipse(thumbRect);
}

void AverraSwitch::enterEvent(QEvent *event)
{
    m_hovered = true;
    update();
    QAbstractButton::enterEvent(event);
}

void AverraSwitch::leaveEvent(QEvent *event)
{
    m_hovered = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void AverraSwitch::initialize()
{
    m_hovered = false;

    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(sizeHint());

    connect(this, &QAbstractButton::toggled, this, &AverraSwitch::checkedChanged);
    connect(this, &QAbstractButton::toggled, this, QOverload<>::of(&AverraSwitch::update));
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraSwitch::handleThemeChanged);
}

void AverraSwitch::handleThemeChanged()
{
    update();
}
