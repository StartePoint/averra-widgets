#include <Averra/AverraSlider.h>
#include <Averra/AverraThemeManager.h>

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QSizePolicy>

AverraSlider::AverraSlider(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QSize AverraSlider::sizeHint() const
{
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    return QSize(320, qMax(24, styleProfile.controlRadius() * 2 + 4));
}

void AverraSlider::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    QColor grooveColor = palette.surfaceColor();
    QColor fillColor = palette.accentColor();
    QColor handleColor = palette.surfaceRaisedColor();
    QColor handleBorderColor = palette.borderColor();

    if (m_hovered || m_dragging) {
        fillColor = fillColor.lighter(105);
        handleBorderColor = palette.accentColor();
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF groove = grooveRect();
    painter.setPen(Qt::NoPen);
    painter.setBrush(grooveColor);
    painter.drawRoundedRect(groove, groove.height() / 2.0, groove.height() / 2.0);

    QRectF fill = groove;
    const qreal ratio = maximum() > minimum()
        ? static_cast<qreal>(value() - minimum()) / static_cast<qreal>(maximum() - minimum())
        : 0.0;
    fill.setWidth(groove.width() * ratio);
    painter.setBrush(fillColor);
    painter.drawRoundedRect(fill, groove.height() / 2.0, groove.height() / 2.0);

    const QRectF handle = handleRect();
    painter.setPen(QPen(handleBorderColor, 1.0));
    painter.setBrush(handleColor);
    painter.drawEllipse(handle);
}

void AverraSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        setValue(valueFromPosition(event->pos().x()));
        event->accept();
        return;
    }

    QAbstractSlider::mousePressEvent(event);
}

void AverraSlider::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        setValue(valueFromPosition(event->pos().x()));
        event->accept();
        return;
    }

    QAbstractSlider::mouseMoveEvent(event);
}

void AverraSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        update();
        event->accept();
        return;
    }

    QAbstractSlider::mouseReleaseEvent(event);
}

void AverraSlider::enterEvent(QEvent *event)
{
    m_hovered = true;
    update();
    QAbstractSlider::enterEvent(event);
}

void AverraSlider::leaveEvent(QEvent *event)
{
    m_hovered = false;
    m_dragging = false;
    update();
    QAbstractSlider::leaveEvent(event);
}

void AverraSlider::initialize()
{
    m_hovered = false;
    m_dragging = false;

    setOrientation(Qt::Horizontal);
    setRange(0, 100);
    setValue(0);
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(sizeHint().height());

    connect(this, &QAbstractSlider::valueChanged, this, QOverload<>::of(&AverraSlider::update));
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraSlider::handleThemeChanged);
}

void AverraSlider::handleThemeChanged()
{
    update();
}

int AverraSlider::valueFromPosition(int x) const
{
    const QRectF groove = grooveRect();
    const qreal clampedX = qBound(groove.left(), static_cast<qreal>(x), groove.right());

    if (maximum() <= minimum()) {
        return minimum();
    }

    const qreal ratio = (clampedX - groove.left()) / groove.width();
    return minimum() + static_cast<int>((maximum() - minimum()) * ratio + 0.5);
}

QRectF AverraSlider::grooveRect() const
{
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    const qreal heightValue = qMax<qreal>(6.0, styleProfile.smallRadius());
    const qreal margin = qMax<qreal>(10.0, styleProfile.compactPaddingHorizontal() + 2);
    return QRectF(margin, (height() - heightValue) / 2.0, width() - margin * 2.0, heightValue);
}

QRectF AverraSlider::handleRect() const
{
    const QRectF groove = grooveRect();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    const qreal handleSize = qMax<qreal>(14.0, styleProfile.controlRadius() + 4);
    qreal x = groove.left() - handleSize / 2.0;

    if (maximum() > minimum()) {
        const qreal ratio = static_cast<qreal>(value() - minimum()) / static_cast<qreal>(maximum() - minimum());
        x = groove.left() + groove.width() * ratio - handleSize / 2.0;
    }

    return QRectF(x, (height() - handleSize) / 2.0, handleSize, handleSize);
}
