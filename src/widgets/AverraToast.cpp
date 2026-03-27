#include <Averra/AverraToast.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

namespace
{
AverraStyleHelper::InfoBarType toInfoBarType(AverraToast::Type type)
{
    switch (type) {
    case AverraToast::Info:
        return AverraStyleHelper::InfoBarType::Info;
    case AverraToast::Success:
        return AverraStyleHelper::InfoBarType::Success;
    case AverraToast::Warning:
        return AverraStyleHelper::InfoBarType::Warning;
    case AverraToast::Error:
        return AverraStyleHelper::InfoBarType::Error;
    }

    return AverraStyleHelper::InfoBarType::Info;
}
}

AverraToast::AverraToast(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraToast::Type AverraToast::type() const
{
    return m_type;
}

void AverraToast::setType(Type type)
{
    if (m_type == type) {
        return;
    }

    m_type = type;
    refreshStyle();
    emit typeChanged(m_type);
}

QString AverraToast::title() const
{
    return m_title;
}

void AverraToast::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraToast::description() const
{
    return m_description;
}

void AverraToast::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

int AverraToast::durationMs() const
{
    return m_durationMs;
}

void AverraToast::setDurationMs(int durationMs)
{
    if (durationMs <= 0 || m_durationMs == durationMs) {
        return;
    }

    m_durationMs = durationMs;
    emit durationMsChanged(m_durationMs);
}

void AverraToast::startAutoClose()
{
    m_timer->start(m_durationMs);
}

void AverraToast::stopAutoClose()
{
    m_timer->stop();
}

void AverraToast::initialize()
{
    m_type = Info;
    m_title.clear();
    m_description.clear();
    m_durationMs = 2500;
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);
    m_timer = new QTimer(this);

    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &QWidget::hide);

    m_rootFrame->setObjectName(QStringLiteral("AverraToastRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraToastTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraToastDescription"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(14, 12, 14, 12);
    rootLayout->setSpacing(4);
    rootLayout->addWidget(m_titleLabel);
    rootLayout->addWidget(m_descriptionLabel);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraToast::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraToast::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleHelper::InfoBarType infoType = toInfoBarType(m_type);
    m_rootFrame->setStyleSheet(AverraStyleHelper::toastFrameStyleSheet(palette, infoType));
    m_titleLabel->setStyleSheet(AverraStyleHelper::toastTitleLabelStyleSheet(palette, infoType));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::toastDescriptionLabelStyleSheet(palette));
}

void AverraToast::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}
