#include <Averra/AverraPropertyItem.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>

AverraPropertyItem::AverraPropertyItem(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraPropertyItem::AverraPropertyItem(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraPropertyItem::title() const
{
    return m_title;
}

void AverraPropertyItem::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraPropertyItem::description() const
{
    return m_description;
}

void AverraPropertyItem::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

QWidget *AverraPropertyItem::accessoryWidget() const
{
    return m_accessoryWidget;
}

void AverraPropertyItem::setAccessoryWidget(QWidget *widget)
{
    if (m_accessoryWidget == widget) {
        return;
    }

    if (m_accessoryWidget != nullptr) {
        m_accessoryLayout->removeWidget(m_accessoryWidget);
        m_accessoryWidget->setParent(nullptr);
    }

    m_accessoryWidget = widget;

    if (m_accessoryWidget != nullptr) {
        m_accessoryWidget->setParent(m_accessoryHost);
        m_accessoryLayout->addWidget(m_accessoryWidget);
    }
}

void AverraPropertyItem::initialize()
{
    m_title.clear();
    m_description.clear();
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);
    m_accessoryHost = new QWidget(m_rootFrame);
    m_accessoryLayout = new QHBoxLayout;
    m_accessoryWidget = nullptr;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_rootFrame->setObjectName(QStringLiteral("AverraPropertyItemRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraPropertyItemTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraPropertyItemDescription"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QHBoxLayout *rootLayout = new QHBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(16, 14, 16, 14);
    rootLayout->setSpacing(12);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(3);

    m_titleLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);

    m_accessoryLayout->setContentsMargins(0, 0, 0, 0);
    m_accessoryLayout->setSpacing(0);
    m_accessoryHost->setLayout(m_accessoryLayout);

    textLayout->addWidget(m_titleLabel);
    textLayout->addWidget(m_descriptionLabel);

    rootLayout->addLayout(textLayout, 1);
    rootLayout->addWidget(m_accessoryHost, 0, Qt::AlignVCenter);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraPropertyItem::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraPropertyItem::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::propertyItemFrameStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::propertyItemTitleLabelStyleSheet(palette));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::propertyItemDescriptionLabelStyleSheet(palette));
}

void AverraPropertyItem::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}

