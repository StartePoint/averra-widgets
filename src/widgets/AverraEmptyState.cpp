#include <Averra/AverraEmptyState.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>

AverraEmptyState::AverraEmptyState(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraEmptyState::AverraEmptyState(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraEmptyState::iconText() const
{
    return m_iconText;
}

void AverraEmptyState::setIconText(const QString &iconText)
{
    if (m_iconText == iconText) {
        return;
    }

    m_iconText = iconText;
    m_iconLabel->setText(m_iconText);
    updateTextVisibility();
    emit iconTextChanged(m_iconText);
}

QString AverraEmptyState::title() const
{
    return m_title;
}

void AverraEmptyState::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraEmptyState::description() const
{
    return m_description;
}

void AverraEmptyState::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

void AverraEmptyState::initialize()
{
    m_iconText = QStringLiteral("?");
    m_title.clear();
    m_description.clear();
    m_rootFrame = new QFrame(this);
    m_iconLabel = new QLabel(m_rootFrame);
    m_titleLabel = new QLabel(m_rootFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_rootFrame->setObjectName(QStringLiteral("AverraEmptyStateRoot"));
    m_iconLabel->setObjectName(QStringLiteral("AverraEmptyStateIcon"));
    m_titleLabel->setObjectName(QStringLiteral("AverraEmptyStateTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraEmptyStateDescription"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(24, 28, 24, 28);
    rootLayout->setSpacing(10);
    rootLayout->setAlignment(Qt::AlignCenter);

    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_descriptionLabel->setAlignment(Qt::AlignCenter);
    m_descriptionLabel->setWordWrap(true);

    rootLayout->addWidget(m_iconLabel, 0, Qt::AlignCenter);
    rootLayout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
    rootLayout->addWidget(m_descriptionLabel, 0, Qt::AlignCenter);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraEmptyState::refreshStyle);

    m_iconLabel->setText(m_iconText);
    updateTextVisibility();
    refreshStyle();
}

void AverraEmptyState::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::emptyStateFrameStyleSheet(palette));
    m_iconLabel->setStyleSheet(AverraStyleHelper::emptyStateIconLabelStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::emptyStateTitleLabelStyleSheet(palette));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::emptyStateDescriptionLabelStyleSheet(palette));
}

void AverraEmptyState::updateTextVisibility()
{
    m_iconLabel->setVisible(!m_iconText.isEmpty());
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}

