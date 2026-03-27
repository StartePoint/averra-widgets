#include <Averra/AverraDivider.h>
#include <Averra/AverraThemeManager.h>

#include <QPainter>
#include <QPaintEvent>
#include <QSizePolicy>

AverraDivider::AverraDivider(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

Qt::Orientation AverraDivider::orientation() const
{
    return m_orientation;
}

void AverraDivider::setOrientation(Qt::Orientation orientation)
{
    if (m_orientation == orientation) {
        return;
    }

    m_orientation = orientation;

    if (m_orientation == Qt::Horizontal) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setMinimumHeight(m_thickness);
        setMinimumWidth(0);
    } else {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        setMinimumWidth(m_thickness);
        setMinimumHeight(0);
    }

    updateGeometry();
    update();
    emit orientationChanged(m_orientation);
}

int AverraDivider::thickness() const
{
    return m_thickness;
}

void AverraDivider::setThickness(int thickness)
{
    if (thickness <= 0 || m_thickness == thickness) {
        return;
    }

    m_thickness = thickness;

    if (m_orientation == Qt::Horizontal) {
        setMinimumHeight(m_thickness);
    } else {
        setMinimumWidth(m_thickness);
    }

    updateGeometry();
    update();
    emit thicknessChanged(m_thickness);
}

QSize AverraDivider::sizeHint() const
{
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    if (m_orientation == Qt::Horizontal) {
        return QSize(120, qMax(m_thickness, styleProfile.smallRadius() > 6 ? 2 : 1));
    }

    return QSize(qMax(m_thickness, styleProfile.smallRadius() > 6 ? 2 : 1), 24);
}

void AverraDivider::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette.borderColor());

    if (m_orientation == Qt::Horizontal) {
        const QRect rectValue(0, (height() - m_thickness) / 2, width(), m_thickness);
        painter.drawRect(rectValue);
    } else {
        const QRect rectValue((width() - m_thickness) / 2, 0, m_thickness, height());
        painter.drawRect(rectValue);
    }
}

void AverraDivider::initialize()
{
    m_orientation = Qt::Horizontal;
    m_thickness = 1;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(1);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraDivider::handleThemeChanged);
}

void AverraDivider::handleThemeChanged()
{
    update();
}
