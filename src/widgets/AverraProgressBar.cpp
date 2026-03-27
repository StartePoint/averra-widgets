#include <Averra/AverraProgressBar.h>
#include <Averra/AverraThemeManager.h>

#include <QPainter>
#include <QPaintEvent>
#include <QSizePolicy>

AverraProgressBar::AverraProgressBar(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

int AverraProgressBar::minimum() const
{
    return m_minimum;
}

void AverraProgressBar::setMinimum(int minimum)
{
    setRange(minimum, m_maximum);
}

int AverraProgressBar::maximum() const
{
    return m_maximum;
}

void AverraProgressBar::setMaximum(int maximum)
{
    setRange(m_minimum, maximum);
}

void AverraProgressBar::setRange(int minimum, int maximum)
{
    if (minimum > maximum) {
        return;
    }

    if (m_minimum == minimum && m_maximum == maximum) {
        return;
    }

    m_minimum = minimum;
    m_maximum = maximum;

    if (m_value < m_minimum) {
        m_value = m_minimum;
    } else if (m_value > m_maximum) {
        m_value = m_maximum;
    }

    update();
    emit rangeChanged(m_minimum, m_maximum);
}

int AverraProgressBar::value() const
{
    return m_value;
}

void AverraProgressBar::setValue(int value)
{
    int boundedValue = value;

    if (boundedValue < m_minimum) {
        boundedValue = m_minimum;
    } else if (boundedValue > m_maximum) {
        boundedValue = m_maximum;
    }

    if (m_value == boundedValue) {
        return;
    }

    m_value = boundedValue;
    update();
    emit valueChanged(m_value);
}

bool AverraProgressBar::showText() const
{
    return m_showText;
}

void AverraProgressBar::setShowText(bool showText)
{
    if (m_showText == showText) {
        return;
    }

    m_showText = showText;
    update();
    emit showTextChanged(m_showText);
}

QSize AverraProgressBar::sizeHint() const
{
    return QSize(320, 30);
}

void AverraProgressBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF trackRect(0.0, 0.0, width(), height());
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette.surfaceColor());
    painter.drawRoundedRect(trackRect, 10.0, 10.0);

    QRectF fillRect = trackRect.adjusted(0.0, 0.0, 0.0, 0.0);
    fillRect.setWidth(trackRect.width() * progressRatio());
    painter.setBrush(palette.accentColor());
    painter.drawRoundedRect(fillRect, 10.0, 10.0);

    painter.setPen(QPen(palette.borderColor(), 1.0));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(trackRect.adjusted(0.5, 0.5, -0.5, -0.5), 10.0, 10.0);

    if (m_showText) {
        const int percent = static_cast<int>(progressRatio() * 100.0 + 0.5);
        painter.setPen(palette.textPrimaryColor());
        painter.drawText(rect(), Qt::AlignCenter, QStringLiteral("%1%").arg(percent));
    }
}

void AverraProgressBar::initialize()
{
    m_minimum = 0;
    m_maximum = 100;
    m_value = 0;
    m_showText = true;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(30);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraProgressBar::handleThemeChanged);
}

void AverraProgressBar::handleThemeChanged()
{
    update();
}

qreal AverraProgressBar::progressRatio() const
{
    if (m_maximum <= m_minimum) {
        return 0.0;
    }

    return static_cast<qreal>(m_value - m_minimum) / static_cast<qreal>(m_maximum - m_minimum);
}

