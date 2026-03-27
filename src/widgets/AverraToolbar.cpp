#include <Averra/AverraToolbar.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>

AverraToolbar::AverraToolbar(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QString AverraToolbar::title() const
{
    return m_title;
}

void AverraToolbar::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraToolbar::subtitle() const
{
    return m_subtitle;
}

void AverraToolbar::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle) {
        return;
    }

    m_subtitle = subtitle;
    m_subtitleLabel->setText(m_subtitle);
    updateTextVisibility();
    emit subtitleChanged(m_subtitle);
}

bool AverraToolbar::accent() const
{
    return m_accent;
}

void AverraToolbar::setAccent(bool accent)
{
    if (m_accent == accent) {
        return;
    }

    m_accent = accent;
    refreshStyle();
    emit accentChanged(m_accent);
}

QHBoxLayout *AverraToolbar::leadingLayout() const
{
    return m_leadingLayout;
}

QHBoxLayout *AverraToolbar::trailingLayout() const
{
    return m_trailingLayout;
}

void AverraToolbar::addLeadingWidget(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }

    m_leadingLayout->addWidget(widget);
}

void AverraToolbar::addTrailingWidget(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }

    m_trailingLayout->addWidget(widget);
}

void AverraToolbar::initialize()
{
    m_title.clear();
    m_subtitle.clear();
    m_accent = false;
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_subtitleLabel = new QLabel(m_rootFrame);
    m_leadingWidget = new QWidget(m_rootFrame);
    m_trailingWidget = new QWidget(m_rootFrame);
    m_leadingLayout = new QHBoxLayout;
    m_trailingLayout = new QHBoxLayout;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_rootFrame->setObjectName(QStringLiteral("AverraToolbarRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraToolbarTitle"));
    m_subtitleLabel->setObjectName(QStringLiteral("AverraToolbarSubtitle"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QHBoxLayout *rootLayout = new QHBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(16, 14, 16, 14);
    rootLayout->setSpacing(12);

    QVBoxLayout *titleLayout = new QVBoxLayout;
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(2);

    m_titleLabel->setWordWrap(true);
    m_subtitleLabel->setWordWrap(true);

    m_leadingLayout->setContentsMargins(0, 0, 0, 0);
    m_leadingLayout->setSpacing(8);
    m_leadingWidget->setLayout(m_leadingLayout);

    m_trailingLayout->setContentsMargins(0, 0, 0, 0);
    m_trailingLayout->setSpacing(8);
    m_trailingWidget->setLayout(m_trailingLayout);

    titleLayout->addWidget(m_titleLabel);
    titleLayout->addWidget(m_subtitleLabel);

    rootLayout->addWidget(m_leadingWidget, 0, Qt::AlignVCenter);
    rootLayout->addLayout(titleLayout, 1);
    rootLayout->addWidget(m_trailingWidget, 0, Qt::AlignVCenter);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraToolbar::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraToolbar::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::toolbarFrameStyleSheet(palette, m_accent));
    m_titleLabel->setStyleSheet(AverraStyleHelper::toolbarTitleLabelStyleSheet(palette, m_accent));
    m_subtitleLabel->setStyleSheet(AverraStyleHelper::toolbarSubtitleLabelStyleSheet(palette));
}

void AverraToolbar::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_subtitleLabel->setVisible(!m_subtitle.isEmpty());
}

