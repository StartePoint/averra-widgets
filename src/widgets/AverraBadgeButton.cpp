#include <Averra/AverraBadgeButton.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QLabel>
#include <QResizeEvent>
#include <QSizePolicy>

AverraBadgeButton::AverraBadgeButton(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraBadgeButton::AverraBadgeButton(const QString &text, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    setText(text);
    initialize();
}

bool AverraBadgeButton::accent() const
{
    return m_accent;
}

void AverraBadgeButton::setAccent(bool accent)
{
    if (m_accent == accent) {
        return;
    }

    m_accent = accent;
    refreshStyle();
    emit accentChanged(m_accent);
}

QString AverraBadgeButton::badgeText() const
{
    return m_badgeText;
}

void AverraBadgeButton::setBadgeText(const QString &badgeText)
{
    if (m_badgeText == badgeText) {
        return;
    }

    m_badgeText = badgeText;
    m_badgeLabel->setText(m_badgeText);
    m_badgeLabel->setVisible(!m_badgeText.isEmpty());
    updateBadgeGeometry();
    emit badgeTextChanged(m_badgeText);
}

void AverraBadgeButton::initialize()
{
    m_accent = false;
    m_badgeText.clear();
    m_badgeLabel = new QLabel(this);

    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setMinimumHeight(44);

    m_badgeLabel->setObjectName(QStringLiteral("AverraBadgeButtonBadge"));
    m_badgeLabel->setAlignment(Qt::AlignCenter);
    m_badgeLabel->setVisible(false);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraBadgeButton::refreshStyle);

    refreshStyle();
}

void AverraBadgeButton::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::badgeButtonStyleSheet(palette, m_accent));
    m_badgeLabel->setStyleSheet(AverraStyleHelper::badgeButtonBadgeStyleSheet(palette, m_accent));
    updateBadgeGeometry();
}

void AverraBadgeButton::resizeEvent(QResizeEvent *event)
{
    updateBadgeGeometry();
    QPushButton::resizeEvent(event);
}

void AverraBadgeButton::updateBadgeGeometry()
{
    const int badgeWidth = qMax(22, m_badgeLabel->fontMetrics().horizontalAdvance(m_badgeText) + 14);
    m_badgeLabel->setGeometry(width() - badgeWidth - 10, (height() - 20) / 2, badgeWidth, 20);
}
