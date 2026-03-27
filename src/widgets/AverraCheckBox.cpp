#include <Averra/AverraCheckBox.h>
#include <Averra/AverraThemeManager.h>

#include <QEvent>
#include <QFontMetrics>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QSizePolicy>

AverraCheckBox::AverraCheckBox(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraCheckBox::AverraCheckBox(const QString &text, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    setText(text);
    initialize();
}

QSize AverraCheckBox::sizeHint() const
{
    QFontMetrics metrics(font());
    const int boxSize = 18;
    const int spacing = 10;
    const int width = boxSize + spacing + metrics.horizontalAdvance(text()) + 6;
    const int height = qMax(boxSize, metrics.height()) + 8;
    return QSize(width, height);
}

void AverraCheckBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    QColor boxBackground = palette.surfaceRaisedColor();
    QColor boxBorder = palette.borderColor();
    QColor textColor = palette.textPrimaryColor();

    if (isChecked()) {
        boxBackground = palette.accentColor();
        boxBorder = palette.accentColor();
    } else if (m_hovered && isEnabled()) {
        boxBorder = palette.accentColor();
    }

    if (!isEnabled()) {
        textColor = palette.textSecondaryColor();
        boxBackground = palette.surfaceColor();
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const int boxSize = 18;
    const int spacing = 10;
    const int boxY = (height() - boxSize) / 2;
    const QRectF boxRect(0.0, static_cast<qreal>(boxY), static_cast<qreal>(boxSize), static_cast<qreal>(boxSize));

    painter.setPen(QPen(boxBorder, 1.0));
    painter.setBrush(boxBackground);
    painter.drawRoundedRect(boxRect, 5.0, 5.0);

    if (isChecked()) {
        QPen checkPen(QColor(QStringLiteral("#FFFFFF")), 2.0);
        checkPen.setCapStyle(Qt::RoundCap);
        checkPen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(checkPen);
        painter.drawLine(QPointF(4.5, boxY + 9.5), QPointF(8.0, boxY + 13.0));
        painter.drawLine(QPointF(8.0, boxY + 13.0), QPointF(14.0, boxY + 5.5));
    }

    painter.setPen(textColor);
    const QRect textRect(boxSize + spacing, 0, width() - boxSize - spacing, height());
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());
}

void AverraCheckBox::enterEvent(QEvent *event)
{
    m_hovered = true;
    update();
    QAbstractButton::enterEvent(event);
}

void AverraCheckBox::leaveEvent(QEvent *event)
{
    m_hovered = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void AverraCheckBox::initialize()
{
    m_hovered = false;

    setCheckable(true);
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    setMinimumHeight(sizeHint().height());

    connect(this, &QAbstractButton::toggled, this, &AverraCheckBox::checkedChanged);
    connect(this, &QAbstractButton::toggled, this, QOverload<>::of(&AverraCheckBox::update));
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraCheckBox::handleThemeChanged);
}

void AverraCheckBox::handleThemeChanged()
{
    update();
}
