#include <Averra/AverraInfoBar.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

namespace
{
AverraStyleHelper::InfoBarType convertInfoBarType(AverraInfoBar::Type type)
{
    switch (type) {
    case AverraInfoBar::Info:
        return AverraStyleHelper::InfoBarType::Info;
    case AverraInfoBar::Success:
        return AverraStyleHelper::InfoBarType::Success;
    case AverraInfoBar::Warning:
        return AverraStyleHelper::InfoBarType::Warning;
    case AverraInfoBar::Error:
        return AverraStyleHelper::InfoBarType::Error;
    }

    return AverraStyleHelper::InfoBarType::Info;
}
}

AverraInfoBar::AverraInfoBar(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraInfoBar::AverraInfoBar(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraInfoBar::title() const
{
    return m_title;
}

void AverraInfoBar::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraInfoBar::description() const
{
    return m_description;
}

void AverraInfoBar::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

AverraInfoBar::Type AverraInfoBar::type() const
{
    return m_type;
}

void AverraInfoBar::setType(Type type)
{
    if (m_type == type) {
        return;
    }

    m_type = type;
    m_badgeLabel->setText(badgeText());
    refreshStyle();
    emit typeChanged(m_type);
}

bool AverraInfoBar::closable() const
{
    return m_closable;
}

void AverraInfoBar::setClosable(bool closable)
{
    if (m_closable == closable) {
        return;
    }

    m_closable = closable;
    updateCloseButtonVisibility();
    emit closableChanged(m_closable);
}

void AverraInfoBar::initialize()
{
    m_title.clear();
    m_description.clear();
    m_type = Info;
    m_closable = true;
    m_rootFrame = new QFrame(this);
    m_badgeLabel = new QLabel(m_rootFrame);
    m_titleLabel = new QLabel(m_rootFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);
    m_closeButton = new QToolButton(m_rootFrame);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_rootFrame->setObjectName(QStringLiteral("AverraInfoBarRoot"));
    m_badgeLabel->setObjectName(QStringLiteral("AverraInfoBarBadge"));
    m_titleLabel->setObjectName(QStringLiteral("AverraInfoBarTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraInfoBarDescription"));
    m_closeButton->setObjectName(QStringLiteral("AverraInfoBarCloseButton"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QHBoxLayout *rootLayout = new QHBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(16, 14, 16, 14);
    rootLayout->setSpacing(12);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(4);

    m_badgeLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);

    m_closeButton->setAutoRaise(true);
    m_closeButton->setCursor(Qt::PointingHandCursor);
    m_closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));

    textLayout->addWidget(m_titleLabel);
    textLayout->addWidget(m_descriptionLabel);

    rootLayout->addWidget(m_badgeLabel, 0, Qt::AlignTop);
    rootLayout->addLayout(textLayout, 1);
    rootLayout->addWidget(m_closeButton, 0, Qt::AlignTop);

    connect(m_closeButton,
            &QToolButton::clicked,
            this,
            &AverraInfoBar::handleCloseButtonClicked);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraInfoBar::refreshStyle);

    m_badgeLabel->setText(badgeText());
    updateTextVisibility();
    updateCloseButtonVisibility();
    refreshStyle();
}

void AverraInfoBar::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleHelper::InfoBarType styleType = convertInfoBarType(m_type);

    m_rootFrame->setStyleSheet(AverraStyleHelper::infoBarFrameStyleSheet(palette, styleType));
    m_badgeLabel->setStyleSheet(AverraStyleHelper::infoBarBadgeStyleSheet(palette, styleType));
    m_titleLabel->setStyleSheet(AverraStyleHelper::infoBarTitleLabelStyleSheet(palette, styleType));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::infoBarDescriptionLabelStyleSheet(palette));
    m_closeButton->setStyleSheet(AverraStyleHelper::infoBarCloseButtonStyleSheet(palette, styleType));
}

void AverraInfoBar::handleCloseButtonClicked()
{
    hide();
    emit dismissed();
}

void AverraInfoBar::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}

void AverraInfoBar::updateCloseButtonVisibility()
{
    m_closeButton->setVisible(m_closable);
}

QString AverraInfoBar::badgeText() const
{
    switch (m_type) {
    case Info:
        return QStringLiteral("INFO");
    case Success:
        return QStringLiteral("OK");
    case Warning:
        return QStringLiteral("WARN");
    case Error:
        return QStringLiteral("ERROR");
    }

    return QStringLiteral("INFO");
}
