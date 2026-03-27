#include <Averra/AverraSectionHeader.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>

AverraSectionHeader::AverraSectionHeader(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraSectionHeader::AverraSectionHeader(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraSectionHeader::title() const
{
    return m_title;
}

void AverraSectionHeader::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraSectionHeader::subtitle() const
{
    return m_subtitle;
}

void AverraSectionHeader::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle) {
        return;
    }

    m_subtitle = subtitle;
    m_subtitleLabel->setText(m_subtitle);
    updateTextVisibility();
    emit subtitleChanged(m_subtitle);
}

QString AverraSectionHeader::metaText() const
{
    return m_metaText;
}

void AverraSectionHeader::setMetaText(const QString &metaText)
{
    if (m_metaText == metaText) {
        return;
    }

    m_metaText = metaText;
    m_metaLabel->setText(m_metaText);
    updateTextVisibility();
    emit metaTextChanged(m_metaText);
}

void AverraSectionHeader::initialize()
{
    m_title.clear();
    m_subtitle.clear();
    m_metaText.clear();
    m_titleLabel = new QLabel(this);
    m_subtitleLabel = new QLabel(this);
    m_metaLabel = new QLabel(this);
    m_divider = new QFrame(this);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_titleLabel->setObjectName(QStringLiteral("AverraSectionHeaderTitle"));
    m_subtitleLabel->setObjectName(QStringLiteral("AverraSectionHeaderSubtitle"));
    m_metaLabel->setObjectName(QStringLiteral("AverraSectionHeaderMeta"));
    m_divider->setObjectName(QStringLiteral("AverraSectionHeaderDivider"));
    m_divider->setFrameShape(QFrame::NoFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 8, 0, 0);
    rootLayout->setSpacing(8);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(10);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(2);

    m_titleLabel->setWordWrap(true);
    m_subtitleLabel->setWordWrap(true);
    m_metaLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    textLayout->addWidget(m_titleLabel);
    textLayout->addWidget(m_subtitleLabel);

    topLayout->addLayout(textLayout, 1);
    topLayout->addWidget(m_metaLabel, 0, Qt::AlignTop);

    rootLayout->addLayout(topLayout);
    rootLayout->addWidget(m_divider);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraSectionHeader::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraSectionHeader::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_titleLabel->setStyleSheet(AverraStyleHelper::sectionHeaderTitleLabelStyleSheet(palette));
    m_subtitleLabel->setStyleSheet(AverraStyleHelper::sectionHeaderSubtitleLabelStyleSheet(palette));
    m_metaLabel->setStyleSheet(AverraStyleHelper::sectionHeaderMetaLabelStyleSheet(palette));
    m_divider->setStyleSheet(AverraStyleHelper::sectionHeaderDividerStyleSheet(palette));
}

void AverraSectionHeader::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_subtitleLabel->setVisible(!m_subtitle.isEmpty());
    m_metaLabel->setVisible(!m_metaText.isEmpty());
}

