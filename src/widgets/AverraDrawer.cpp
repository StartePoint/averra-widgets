#include <Averra/AverraDrawer.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

AverraDrawer::AverraDrawer(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QString AverraDrawer::title() const
{
    return m_title;
}

void AverraDrawer::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraDrawer::description() const
{
    return m_description;
}

void AverraDrawer::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

bool AverraDrawer::opened() const
{
    return m_opened;
}

void AverraDrawer::setOpened(bool opened)
{
    if (m_opened == opened) {
        return;
    }

    m_opened = opened;
    setVisible(m_opened);
    emit openedChanged(m_opened);
}

QVBoxLayout *AverraDrawer::contentLayout() const
{
    return m_contentLayout;
}

QHBoxLayout *AverraDrawer::footerLayout() const
{
    return m_footerLayout;
}

void AverraDrawer::addContentWidget(QWidget *widget)
{
    if (widget != nullptr) {
        m_contentLayout->addWidget(widget);
    }
}

void AverraDrawer::addFooterWidget(QWidget *widget)
{
    if (widget != nullptr) {
        m_footerLayout->addWidget(widget);
    }
}

void AverraDrawer::initialize()
{
    m_title.clear();
    m_description.clear();
    m_opened = true;
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);
    m_contentWidget = new QWidget(m_rootFrame);
    m_footerWidget = new QWidget(m_rootFrame);
    m_contentLayout = new QVBoxLayout;
    m_footerLayout = new QHBoxLayout;

    m_rootFrame->setObjectName(QStringLiteral("AverraDrawerRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraDrawerTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraDrawerDescription"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(12);

    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentWidget->setLayout(m_contentLayout);

    m_footerLayout->setContentsMargins(0, 0, 0, 0);
    m_footerLayout->setSpacing(8);
    m_footerLayout->addStretch(1);
    m_footerWidget->setLayout(m_footerLayout);

    rootLayout->addWidget(m_titleLabel);
    rootLayout->addWidget(m_descriptionLabel);
    rootLayout->addWidget(m_contentWidget);
    rootLayout->addWidget(m_footerWidget);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraDrawer::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraDrawer::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::drawerFrameStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::drawerTitleLabelStyleSheet(palette));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::drawerDescriptionLabelStyleSheet(palette));
}

void AverraDrawer::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}

