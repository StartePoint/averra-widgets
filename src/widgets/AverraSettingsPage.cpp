#include <Averra/AverraSettingsPage.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

AverraSettingsPage::AverraSettingsPage(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QString AverraSettingsPage::title() const
{
    return m_title;
}

void AverraSettingsPage::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraSettingsPage::description() const
{
    return m_description;
}

void AverraSettingsPage::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

QVBoxLayout *AverraSettingsPage::contentLayout() const
{
    return m_contentLayout;
}

void AverraSettingsPage::addSection(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }

    m_contentLayout->addWidget(widget);
}

void AverraSettingsPage::initialize()
{
    m_title.clear();
    m_description.clear();
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);
    m_contentWidget = new QWidget(m_rootFrame);
    m_contentLayout = new QVBoxLayout;

    m_rootFrame->setObjectName(QStringLiteral("AverraSettingsPageRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraSettingsPageTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraSettingsPageDescription"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(14);

    m_titleLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);

    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(12);
    m_contentWidget->setLayout(m_contentLayout);

    rootLayout->addWidget(m_titleLabel);
    rootLayout->addWidget(m_descriptionLabel);
    rootLayout->addWidget(m_contentWidget);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraSettingsPage::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraSettingsPage::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::settingsPageFrameStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::settingsPageTitleLabelStyleSheet(palette));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::settingsPageDescriptionLabelStyleSheet(palette));
}

void AverraSettingsPage::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}

