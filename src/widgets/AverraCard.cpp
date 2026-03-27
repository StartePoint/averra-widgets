#include <Averra/AverraCard.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>

AverraCard::AverraCard(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraCard::AverraCard(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraCard::title() const
{
    return m_title;
}

void AverraCard::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraCard::subtitle() const
{
    return m_subtitle;
}

void AverraCard::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle) {
        return;
    }

    m_subtitle = subtitle;
    m_subtitleLabel->setText(m_subtitle);
    updateTextVisibility();
    emit subtitleChanged(m_subtitle);
}

bool AverraCard::accentHeader() const
{
    return m_accentHeader;
}

void AverraCard::setAccentHeader(bool accentHeader)
{
    if (m_accentHeader == accentHeader) {
        return;
    }

    m_accentHeader = accentHeader;
    refreshStyle();
    emit accentHeaderChanged(m_accentHeader);
}

QWidget *AverraCard::contentWidget() const
{
    return m_contentWidget;
}

QVBoxLayout *AverraCard::contentLayout() const
{
    return m_contentLayout;
}

void AverraCard::initialize()
{
    m_title.clear();
    m_subtitle.clear();
    m_accentHeader = false;
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_subtitleLabel = new QLabel(m_rootFrame);
    m_contentWidget = new QWidget(m_rootFrame);
    m_contentLayout = new QVBoxLayout;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_rootFrame->setObjectName(QStringLiteral("AverraCardRoot"));
    m_contentWidget->setObjectName(QStringLiteral("AverraCardContent"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(8);

    m_titleLabel->setWordWrap(true);
    m_subtitleLabel->setWordWrap(true);

    m_contentLayout->setContentsMargins(0, 4, 0, 0);
    m_contentLayout->setSpacing(10);
    m_contentWidget->setLayout(m_contentLayout);

    rootLayout->addWidget(m_titleLabel);
    rootLayout->addWidget(m_subtitleLabel);
    rootLayout->addWidget(m_contentWidget);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraCard::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraCard::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::cardFrameStyleSheet(palette, m_accentHeader));
    m_titleLabel->setStyleSheet(AverraStyleHelper::cardTitleLabelStyleSheet(palette, m_accentHeader));
    m_subtitleLabel->setStyleSheet(AverraStyleHelper::cardSubtitleLabelStyleSheet(palette));
    m_contentWidget->setStyleSheet(AverraStyleHelper::cardContentWidgetStyleSheet(palette));
}

void AverraCard::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_subtitleLabel->setVisible(!m_subtitle.isEmpty());
}
