#include <Averra/AverraFormSection.h>
#include <Averra/AverraSectionHeader.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QVBoxLayout>

AverraFormSection::AverraFormSection(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraFormSection::AverraFormSection(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraFormSection::title() const
{
    return m_title;
}

void AverraFormSection::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_header->setTitle(m_title);
    emit titleChanged(m_title);
}

QString AverraFormSection::description() const
{
    return m_description;
}

void AverraFormSection::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_header->setSubtitle(m_description);
    emit descriptionChanged(m_description);
}

QString AverraFormSection::metaText() const
{
    return m_metaText;
}

void AverraFormSection::setMetaText(const QString &metaText)
{
    if (m_metaText == metaText) {
        return;
    }

    m_metaText = metaText;
    m_header->setMetaText(m_metaText);
    emit metaTextChanged(m_metaText);
}

QVBoxLayout *AverraFormSection::contentLayout() const
{
    return m_contentLayout;
}

void AverraFormSection::addRow(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }

    m_contentLayout->addWidget(widget);
}

void AverraFormSection::initialize()
{
    m_title.clear();
    m_description.clear();
    m_metaText.clear();
    m_rootFrame = new QFrame(this);
    m_header = new AverraSectionHeader(m_rootFrame);
    m_contentWidget = new QWidget(m_rootFrame);
    m_contentLayout = new QVBoxLayout;

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    m_rootFrame->setObjectName(QStringLiteral("AverraFormSectionRoot"));

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(12);

    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(10);
    m_contentWidget->setLayout(m_contentLayout);

    rootLayout->addWidget(m_header);
    rootLayout->addWidget(m_contentWidget);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraFormSection::refreshStyle);

    refreshStyle();
}

void AverraFormSection::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::formSectionFrameStyleSheet(palette));
}

