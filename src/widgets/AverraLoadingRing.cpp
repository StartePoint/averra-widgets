#include <Averra/AverraLoadingRing.h>
#include <Averra/AverraThemeManager.h>

#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QSizePolicy>
#include <QTimer>

AverraLoadingRing::AverraLoadingRing(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

bool AverraLoadingRing::running() const
{
    return m_running;
}

void AverraLoadingRing::setRunning(bool running)
{
    if (m_running == running) {
        return;
    }

    m_running = running;

    if (m_running) {
        m_timer->start();
    } else {
        m_timer->stop();
    }

    update();
    emit runningChanged(m_running);
}

int AverraLoadingRing::diameter() const
{
    return m_diameter;
}

void AverraLoadingRing::setDiameter(int diameter)
{
    if (diameter <= 0 || m_diameter == diameter) {
        return;
    }

    m_diameter = diameter;
    setFixedSize(sizeHint());
    update();
    emit diameterChanged(m_diameter);
}

void AverraLoadingRing::start()
{
    setRunning(true);
}

void AverraLoadingRing::stop()
{
    setRunning(false);
}

QSize AverraLoadingRing::sizeHint() const
{
    return QSize(m_diameter, m_diameter);
}

void AverraLoadingRing::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const QRectF ringRect(2.0, 2.0, width() - 4.0, height() - 4.0);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen trackPen(palette.surfaceColor(), 3.0);
    trackPen.setCapStyle(Qt::RoundCap);
    painter.setPen(trackPen);
    painter.drawArc(ringRect, 0, 360 * 16);

    QPen arcPen(palette.accentColor(), 3.0);
    arcPen.setCapStyle(Qt::RoundCap);
    painter.setPen(arcPen);
    painter.drawArc(ringRect, m_angle * 16, 110 * 16);
}

void AverraLoadingRing::initialize()
{
    m_running = false;
    m_diameter = 24;
    m_angle = 0;
    m_timer = new QTimer(this);

    m_timer->setInterval(60);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(sizeHint());

    connect(m_timer,
            &QTimer::timeout,
            this,
            &AverraLoadingRing::advanceFrame);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraLoadingRing::handleThemeChanged);
}

void AverraLoadingRing::advanceFrame()
{
    m_angle = (m_angle + 24) % 360;
    update();
}

void AverraLoadingRing::handleThemeChanged()
{
    update();
}

