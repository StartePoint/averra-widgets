#include <Averra/AverraSkeleton.h>
#include <Averra/AverraThemeManager.h>

#include <QLinearGradient>
#include <QPaintEvent>
#include <QPainter>
#include <QSizePolicy>
#include <QTimer>

AverraSkeleton::AverraSkeleton(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

bool AverraSkeleton::running() const
{
    return m_running;
}

void AverraSkeleton::setRunning(bool running)
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

int AverraSkeleton::radius() const
{
    return m_radius;
}

void AverraSkeleton::setRadius(int radius)
{
    if (radius < 0 || m_radius == radius) {
        return;
    }

    m_radius = radius;
    update();
    emit radiusChanged(m_radius);
}

void AverraSkeleton::start()
{
    setRunning(true);
}

void AverraSkeleton::stop()
{
    setRunning(false);
}

QSize AverraSkeleton::sizeHint() const
{
    return QSize(240, 56);
}

void AverraSkeleton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient gradient(0, 0, width(), 0);
    gradient.setColorAt(0.0, palette.surfaceColor());
    gradient.setColorAt(0.5, palette.surfaceRaisedColor());
    gradient.setColorAt(1.0, palette.surfaceColor());
    gradient.setStart(-width() + m_offset, 0);
    gradient.setFinalStop(m_offset, 0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient);
    painter.drawRoundedRect(rect(), m_radius, m_radius);
}

void AverraSkeleton::initialize()
{
    m_running = false;
    m_radius = 10;
    m_offset = 0;
    m_timer = new QTimer(this);
    m_timer->setInterval(40);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(56);

    connect(m_timer, &QTimer::timeout, this, &AverraSkeleton::advanceFrame);
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraSkeleton::handleThemeChanged);
}

void AverraSkeleton::advanceFrame()
{
    m_offset = (m_offset + 16) % (width() * 2 + 1);
    update();
}

void AverraSkeleton::handleThemeChanged()
{
    update();
}

