#include <Averra/AverraStatisticCard.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>

AverraStatisticCard::AverraStatisticCard(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraStatisticCard::AverraStatisticCard(const QString &caption, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setCaption(caption);
}

QString AverraStatisticCard::caption() const
{
    return m_caption;
}

void AverraStatisticCard::setCaption(const QString &caption)
{
    if (m_caption == caption) {
        return;
    }

    m_caption = caption;
    m_captionLabel->setText(m_caption);
    updateTextVisibility();
    emit captionChanged(m_caption);
}

QString AverraStatisticCard::value() const
{
    return m_value;
}

void AverraStatisticCard::setValue(const QString &value)
{
    if (m_value == value) {
        return;
    }

    m_value = value;
    m_valueLabel->setText(m_value);
    updateTextVisibility();
    emit valueChanged(m_value);
}

QString AverraStatisticCard::helperText() const
{
    return m_helperText;
}

void AverraStatisticCard::setHelperText(const QString &helperText)
{
    if (m_helperText == helperText) {
        return;
    }

    m_helperText = helperText;
    m_helperLabel->setText(m_helperText);
    updateTextVisibility();
    emit helperTextChanged(m_helperText);
}

QString AverraStatisticCard::badgeText() const
{
    return m_badgeText;
}

void AverraStatisticCard::setBadgeText(const QString &badgeText)
{
    if (m_badgeText == badgeText) {
        return;
    }

    m_badgeText = badgeText;
    m_badgeLabel->setText(m_badgeText);
    updateTextVisibility();
    emit badgeTextChanged(m_badgeText);
}

bool AverraStatisticCard::accent() const
{
    return m_accent;
}

void AverraStatisticCard::setAccent(bool accent)
{
    if (m_accent == accent) {
        return;
    }

    m_accent = accent;
    refreshStyle();
    emit accentChanged(m_accent);
}

void AverraStatisticCard::initialize()
{
    m_caption.clear();
    m_value.clear();
    m_helperText.clear();
    m_badgeText.clear();
    m_accent = false;
    m_rootFrame = new QFrame(this);
    m_captionLabel = new QLabel(m_rootFrame);
    m_valueLabel = new QLabel(m_rootFrame);
    m_helperLabel = new QLabel(m_rootFrame);
    m_badgeLabel = new QLabel(m_rootFrame);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_rootFrame->setObjectName(QStringLiteral("AverraStatisticCardRoot"));
    m_captionLabel->setObjectName(QStringLiteral("AverraStatisticCardCaption"));
    m_valueLabel->setObjectName(QStringLiteral("AverraStatisticCardValue"));
    m_helperLabel->setObjectName(QStringLiteral("AverraStatisticCardHelper"));
    m_badgeLabel->setObjectName(QStringLiteral("AverraStatisticCardBadge"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(10);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(10);

    m_captionLabel->setWordWrap(true);
    m_valueLabel->setWordWrap(true);
    m_helperLabel->setWordWrap(true);
    m_badgeLabel->setAlignment(Qt::AlignCenter);

    topLayout->addWidget(m_captionLabel, 1);
    topLayout->addWidget(m_badgeLabel, 0, Qt::AlignTop);

    rootLayout->addLayout(topLayout);
    rootLayout->addWidget(m_valueLabel);
    rootLayout->addWidget(m_helperLabel);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraStatisticCard::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraStatisticCard::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::statisticCardFrameStyleSheet(palette, m_accent));
    m_captionLabel->setStyleSheet(AverraStyleHelper::statisticCardCaptionLabelStyleSheet(palette, m_accent));
    m_valueLabel->setStyleSheet(AverraStyleHelper::statisticCardValueLabelStyleSheet(palette, m_accent));
    m_helperLabel->setStyleSheet(AverraStyleHelper::statisticCardHelperLabelStyleSheet(palette));
    m_badgeLabel->setStyleSheet(AverraStyleHelper::statisticCardBadgeLabelStyleSheet(palette, m_accent));
}

void AverraStatisticCard::updateTextVisibility()
{
    m_captionLabel->setVisible(!m_caption.isEmpty());
    m_valueLabel->setVisible(!m_value.isEmpty());
    m_helperLabel->setVisible(!m_helperText.isEmpty());
    m_badgeLabel->setVisible(!m_badgeText.isEmpty());
}

