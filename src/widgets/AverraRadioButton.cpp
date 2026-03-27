#include <Averra/AverraRadioButton.h>
#include <Averra/AverraThemeManager.h>

#include <QEvent>
#include <QFontMetrics>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QSizePolicy>

AverraRadioButton::AverraRadioButton(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraRadioButton::AverraRadioButton(const QString &text, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    setText(text);
    initialize();
}

QSize AverraRadioButton::sizeHint() const
{
    QFontMetrics metrics(font());
    const int circleSize = 18;
    const int spacing = 10;
    const int width = circleSize + spacing + metrics.horizontalAdvance(text()) + 6;
    const int height = qMax(circleSize, metrics.height()) + 8;
    return QSize(width, height);
}

void AverraRadioButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    QColor circleBackground = palette.surfaceRaisedColor();
    QColor circleBorder = palette.borderColor();
    QColor textColor = palette.textPrimaryColor();

    if (isChecked()) {
        circleBorder = palette.accentColor();
    } else if (m_hovered && isEnabled()) {
        circleBorder = palette.accentColor();
    }

    if (!isEnabled()) {
        textColor = palette.textSecondaryColor();
        circleBackground = palette.surfaceColor();
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const int circleSize = 18;
    const int spacing = 10;
    const int circleY = (height() - circleSize) / 2;
    const QRectF circleRect(0.0, static_cast<qreal>(circleY), static_cast<qreal>(circleSize), static_cast<qreal>(circleSize));

    painter.setPen(QPen(circleBorder, 1.5));
    painter.setBrush(circleBackground);
    painter.drawEllipse(circleRect);

    if (isChecked()) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(palette.accentColor());
        painter.drawEllipse(QRectF(5.0, circleY + 5.0, 8.0, 8.0));
    }

    painter.setPen(textColor);
    const QRect textRect(circleSize + spacing, 0, width() - circleSize - spacing, height());
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());
}

void AverraRadioButton::enterEvent(QEvent *event)
{
    m_hovered = true;
    update();
    QAbstractButton::enterEvent(event);
}

void AverraRadioButton::leaveEvent(QEvent *event)
{
    m_hovered = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void AverraRadioButton::initialize()
{
    m_hovered = false;

    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    setMinimumHeight(sizeHint().height());

    connect(this, &QAbstractButton::toggled, this, &AverraRadioButton::checkedChanged);
    connect(this, &QAbstractButton::toggled, this, QOverload<>::of(&AverraRadioButton::update));
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraRadioButton::handleThemeChanged);
}

void AverraRadioButton::handleThemeChanged()
{
    update();
}

