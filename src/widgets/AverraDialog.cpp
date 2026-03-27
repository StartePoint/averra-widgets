#include <Averra/AverraDialog.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

AverraDialog::AverraDialog(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QString AverraDialog::title() const
{
    return m_title;
}

void AverraDialog::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraDialog::description() const
{
    return m_description;
}

void AverraDialog::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

QVBoxLayout *AverraDialog::contentLayout() const
{
    return m_contentLayout;
}

QHBoxLayout *AverraDialog::footerLayout() const
{
    return m_footerLayout;
}

void AverraDialog::addContentWidget(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }

    m_contentLayout->addWidget(widget);
}

void AverraDialog::addFooterWidget(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }

    m_footerLayout->addWidget(widget);
}

void AverraDialog::initialize()
{
    m_title.clear();
    m_description.clear();
    setModal(false);

    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);
    m_contentWidget = new QWidget(m_rootFrame);
    m_footerWidget = new QWidget(m_rootFrame);
    m_contentLayout = new QVBoxLayout;
    m_footerLayout = new QHBoxLayout;

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    m_rootFrame->setObjectName(QStringLiteral("AverraDialogRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraDialogTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraDialogDescription"));

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(12);

    m_titleLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);

    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(10);
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
            &AverraDialog::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraDialog::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::dialogFrameStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::dialogTitleLabelStyleSheet(palette));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::dialogDescriptionLabelStyleSheet(palette));
}

void AverraDialog::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}

