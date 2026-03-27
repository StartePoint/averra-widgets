#include <Averra/AverraDrawer.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QAbstractAnimation>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QToolButton>
#include <QtGlobal>
#include <QVBoxLayout>

namespace
{
int drawerTitleBarRadius(const AverraStyleProfile &styleProfile)
{
    return qMax(styleProfile.largeRadius(), styleProfile.panelRadius() - 4);
}

int drawerWindowButtonWidth(const AverraStyleProfile &styleProfile)
{
    return qMax(26, styleProfile.windowControlButtonWidth() - 6);
}

int drawerWindowButtonHeight(const AverraStyleProfile &styleProfile)
{
    return qMax(22, styleProfile.windowControlButtonHeight() - 4);
}

QString drawerTitleBarStyleSheet(const AverraThemePalette &palette, const AverraStyleProfile &styleProfile)
{
    return QStringLiteral(
               "QFrame#AverraDrawerTitleBar {"
               "background-color: %1;"
               "border-bottom: 1px solid %2;"
               "border-top-left-radius: %3px;"
               "border-top-right-radius: %3px;"
               "}")
        .arg(palette.surfaceHoverColor().name(),
             palette.borderColor().name(),
             QString::number(drawerTitleBarRadius(styleProfile)));
}

QString panelTrailingWindowControlButtonStyleSheet(const AverraThemePalette &palette,
                                                   const AverraStyleProfile &styleProfile,
                                                   const QString &role)
{
    QColor hoverColor = palette.surfaceHoverColor();
    QColor pressedColor = palette.surfacePressedColor();
    QColor textColor = palette.textSecondaryColor();

    if (role == QStringLiteral("close")) {
        hoverColor = palette.errorColor();
        pressedColor = palette.errorColor().darker(110);
    }

    return QStringLiteral(
               "QToolButton {"
               "background-color: transparent;"
               "color: %1;"
               "border: none;"
               "min-width: %5px;"
               "max-width: %5px;"
               "min-height: %6px;"
               "max-height: %6px;"
               "font-size: %7px;"
               "font-weight: 700;"
               "}"
               "QToolButton:hover {"
               "background-color: %2;"
               "color: %3;"
               "}"
               "QToolButton:pressed {"
               "background-color: %4;"
               "color: %3;"
               "}")
        .arg(textColor.name(),
             hoverColor.name(),
             role == QStringLiteral("close") ? QStringLiteral("#FFFFFF") : palette.textPrimaryColor().name(),
             pressedColor.name(),
             QString::number(drawerWindowButtonWidth(styleProfile)),
             QString::number(drawerWindowButtonHeight(styleProfile)),
             QString::number(styleProfile.smallFontSize()));
}

QString panelTrafficLightButtonStyleSheet(const AverraStyleProfile &styleProfile, const QColor &fillColor)
{
    const int diameter = qMax(8, styleProfile.trafficLightDiameter());
    const int radius = qMax(4, diameter / 2);

    return QStringLiteral(
               "QToolButton {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %5px;"
               "min-width: %6px;"
               "max-width: %6px;"
               "min-height: %6px;"
               "max-height: %6px;"
               "padding: 0px;"
               "}"
               "QToolButton:hover {"
               "background-color: %3;"
               "}"
               "QToolButton:pressed {"
               "background-color: %4;"
               "}")
        .arg(fillColor.name(),
             fillColor.darker(115).name(),
             fillColor.lighter(108).name(),
             fillColor.darker(110).name(),
             QString::number(radius),
             QString::number(diameter));
}
}

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

    if (m_opened) {
        setVisible(true);
        startShowAnimation();
    } else {
        startHideAnimation();
    }

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
    m_allowImmediateHide = false;
    m_expanded = false;
    m_expandedWidth = 0;
    m_rootFrame = new QFrame(this);
    m_titleBarFrame = new QFrame(m_rootFrame);
    m_titleLabel = new QLabel(m_titleBarFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);
    m_windowControlsWidget = new QWidget(m_titleBarFrame);
    m_contentWidget = new QWidget(m_rootFrame);
    m_footerWidget = new QWidget(m_rootFrame);
    m_minimizeButton = new QToolButton(m_windowControlsWidget);
    m_maximizeButton = new QToolButton(m_windowControlsWidget);
    m_closeButton = new QToolButton(m_windowControlsWidget);
    m_titleBarLayout = new QHBoxLayout;
    m_windowControlsLayout = new QHBoxLayout;
    m_contentLayout = new QVBoxLayout;
    m_footerLayout = new QHBoxLayout;
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_showAnimation = new QParallelAnimationGroup(this);
    m_hideAnimation = new QParallelAnimationGroup(this);

    setGraphicsEffect(m_opacityEffect);
    m_opacityEffect->setOpacity(1.0);
    setMinimumWidth(300);

    m_rootFrame->setObjectName(QStringLiteral("AverraDrawerRoot"));
    m_titleBarFrame->setObjectName(QStringLiteral("AverraDrawerTitleBar"));
    m_titleLabel->setObjectName(QStringLiteral("AverraDrawerTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraDrawerDescription"));
    m_minimizeButton->setObjectName(QStringLiteral("AverraDrawerMinimizeButton"));
    m_maximizeButton->setObjectName(QStringLiteral("AverraDrawerMaximizeButton"));
    m_closeButton->setObjectName(QStringLiteral("AverraDrawerCloseButton"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    m_titleLabel->setWordWrap(true);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_descriptionLabel->setWordWrap(true);

    m_windowControlsLayout->setContentsMargins(0, 0, 0, 0);
    m_windowControlsLayout->setSpacing(8);
    m_windowControlsWidget->setLayout(m_windowControlsLayout);

    m_titleBarLayout->setContentsMargins(14, 12, 14, 12);
    m_titleBarLayout->setSpacing(10);
    m_titleBarFrame->setLayout(m_titleBarLayout);

    QWidget *bodyWidget = new QWidget(m_rootFrame);
    QVBoxLayout *bodyLayout = new QVBoxLayout(bodyWidget);
    bodyLayout->setContentsMargins(18, 18, 18, 18);
    bodyLayout->setSpacing(12);

    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(10);
    m_contentWidget->setLayout(m_contentLayout);

    m_footerLayout->setContentsMargins(0, 0, 0, 0);
    m_footerLayout->setSpacing(8);
    m_footerLayout->addStretch(1);
    m_footerWidget->setLayout(m_footerLayout);

    bodyLayout->addWidget(m_descriptionLabel);
    bodyLayout->addWidget(m_contentWidget);
    bodyLayout->addWidget(m_footerWidget);

    rootLayout->addWidget(m_titleBarFrame);
    rootLayout->addWidget(bodyWidget);

    m_minimizeButton->setText(QStringLiteral("−"));
    m_maximizeButton->setText(QStringLiteral("⤢"));
    m_closeButton->setText(QStringLiteral("×"));

    connect(m_minimizeButton,
            &QToolButton::clicked,
            this,
            [this]() {
                setOpened(false);
            });
    connect(m_maximizeButton,
            &QToolButton::clicked,
            this,
            [this]() {
                if (!m_expanded) {
                    m_expandedWidth = qMax(width(), 300);
                    setMinimumWidth(qRound(m_expandedWidth * 1.25));
                    m_expanded = true;
                    m_maximizeButton->setText(QStringLiteral("⤡"));
                } else {
                    setMinimumWidth(300);
                    m_expanded = false;
                    m_maximizeButton->setText(QStringLiteral("⤢"));
                }
                updateGeometry();
            });
    connect(m_closeButton,
            &QToolButton::clicked,
            this,
            [this]() {
                setOpened(false);
            });
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraDrawer::refreshStyle);
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::styleProfileChanged,
            this,
            [this](const AverraStyleProfile &) {
                updateWindowControlLayout();
                updateWindowControlStyle();
            });

    updateWindowControlLayout();
    updateTextVisibility();
    refreshStyle();
}

void AverraDrawer::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();

    m_windowControlsLayout->setSpacing(qMax(6, styleProfile.compactPaddingHorizontal() - 2));
    m_titleBarLayout->setContentsMargins(qMax(10, styleProfile.buttonPaddingHorizontal() - 4),
                                         qMax(8, styleProfile.buttonPaddingVertical() + 2),
                                         qMax(10, styleProfile.buttonPaddingHorizontal() - 4),
                                         qMax(8, styleProfile.buttonPaddingVertical() + 2));
    m_titleBarLayout->setSpacing(styleProfile.compactPaddingHorizontal());
    m_contentLayout->setSpacing(styleProfile.compactPaddingHorizontal());
    m_footerLayout->setSpacing(qMax(6, styleProfile.compactPaddingHorizontal() - 2));
    m_rootFrame->setStyleSheet(AverraStyleHelper::drawerFrameStyleSheet(palette));
    m_titleBarFrame->setStyleSheet(drawerTitleBarStyleSheet(palette, styleProfile));
    m_titleLabel->setStyleSheet(AverraStyleHelper::drawerTitleLabelStyleSheet(palette));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::drawerDescriptionLabelStyleSheet(palette));
    updateWindowControlStyle();
}

void AverraDrawer::updateWindowControlLayout()
{
    while (m_titleBarLayout->count() > 0) {
        QLayoutItem *item = m_titleBarLayout->takeAt(0);
        delete item;
    }

    while (m_windowControlsLayout->count() > 0) {
        QLayoutItem *item = m_windowControlsLayout->takeAt(0);
        if (item->widget() != nullptr) {
            item->widget()->hide();
        }
        delete item;
    }

    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    const bool useLeadingTrafficLights = styleProfile.windowControlsLayout() == AverraStyleProfile::LeadingTrafficLights;

    m_minimizeButton->show();
    m_maximizeButton->show();
    m_closeButton->show();

    if (useLeadingTrafficLights) {
        m_windowControlsLayout->addWidget(m_closeButton);
        m_windowControlsLayout->addWidget(m_minimizeButton);
        m_windowControlsLayout->addWidget(m_maximizeButton);
        m_titleBarLayout->addWidget(m_windowControlsWidget, 0, Qt::AlignVCenter);
        m_titleBarLayout->addStretch(1);
        m_titleBarLayout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
        m_titleBarLayout->addStretch(1);
        m_titleBarLayout->addSpacing(styleProfile.windowControlButtonWidth() - 2);
        return;
    }

    m_windowControlsLayout->addWidget(m_minimizeButton);
    m_windowControlsLayout->addWidget(m_maximizeButton);
    m_windowControlsLayout->addWidget(m_closeButton);
    m_titleBarLayout->addWidget(m_titleLabel, 1, Qt::AlignVCenter);
    m_titleBarLayout->addWidget(m_windowControlsWidget, 0, Qt::AlignTop);
}

void AverraDrawer::updateWindowControlStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    const bool useLeadingTrafficLights = styleProfile.windowControlsLayout() == AverraStyleProfile::LeadingTrafficLights;

    if (useLeadingTrafficLights) {
        m_minimizeButton->setText(QString());
        m_maximizeButton->setText(QString());
        m_closeButton->setText(QString());
        m_minimizeButton->setStyleSheet(panelTrafficLightButtonStyleSheet(styleProfile, QColor(QStringLiteral("#F5BF4F"))));
        m_maximizeButton->setStyleSheet(panelTrafficLightButtonStyleSheet(styleProfile, QColor(QStringLiteral("#61C454"))));
        m_closeButton->setStyleSheet(panelTrafficLightButtonStyleSheet(styleProfile, QColor(QStringLiteral("#EE6A5F"))));
        return;
    }

    m_minimizeButton->setText(QStringLiteral("−"));
    m_maximizeButton->setText(m_expanded ? QStringLiteral("⤡") : QStringLiteral("⤢"));
    m_closeButton->setText(QStringLiteral("×"));
    m_minimizeButton->setStyleSheet(panelTrailingWindowControlButtonStyleSheet(palette, styleProfile, QStringLiteral("minimize")));
    m_maximizeButton->setStyleSheet(panelTrailingWindowControlButtonStyleSheet(palette, styleProfile, QStringLiteral("maximize")));
    m_closeButton->setStyleSheet(panelTrailingWindowControlButtonStyleSheet(palette, styleProfile, QStringLiteral("close")));
}

void AverraDrawer::startShowAnimation()
{
    m_showAnimation->stop();
    while (m_showAnimation->animationCount() > 0) {
        QAbstractAnimation *animation = m_showAnimation->animationAt(0);
        m_showAnimation->removeAnimation(animation);
        delete animation;
    }

    const int targetWidth = qMax(width(), 300);
    setMaximumWidth(0);
    m_opacityEffect->setOpacity(0.0);

    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    QPropertyAnimation *widthAnimation = new QPropertyAnimation(this, "maximumWidth", m_showAnimation);
    widthAnimation->setDuration(styleProfile.panelShowDuration());
    widthAnimation->setStartValue(0);
    widthAnimation->setEndValue(targetWidth);

    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", m_showAnimation);
    opacityAnimation->setDuration(styleProfile.panelShowDuration());
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);

    m_showAnimation->addAnimation(widthAnimation);
    m_showAnimation->addAnimation(opacityAnimation);
    connect(m_showAnimation,
            &QParallelAnimationGroup::finished,
            this,
            [this]() {
                setMaximumWidth(QWIDGETSIZE_MAX);
            });
    m_showAnimation->start();
}

void AverraDrawer::startHideAnimation()
{
    if (!isVisible()) {
        if (!m_allowImmediateHide) {
            hide();
        }
        return;
    }

    m_hideAnimation->stop();
    while (m_hideAnimation->animationCount() > 0) {
        QAbstractAnimation *animation = m_hideAnimation->animationAt(0);
        m_hideAnimation->removeAnimation(animation);
        delete animation;
    }
    disconnect(m_hideAnimation, nullptr, this, nullptr);

    const int currentWidth = qMax(width(), 1);
    setMaximumWidth(currentWidth);

    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    QPropertyAnimation *widthAnimation = new QPropertyAnimation(this, "maximumWidth", m_hideAnimation);
    widthAnimation->setDuration(styleProfile.panelHideDuration());
    widthAnimation->setStartValue(currentWidth);
    widthAnimation->setEndValue(0);

    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", m_hideAnimation);
    opacityAnimation->setDuration(styleProfile.panelHideDuration());
    opacityAnimation->setStartValue(m_opacityEffect->opacity());
    opacityAnimation->setEndValue(0.0);

    m_hideAnimation->addAnimation(widthAnimation);
    m_hideAnimation->addAnimation(opacityAnimation);
    connect(m_hideAnimation,
            &QParallelAnimationGroup::finished,
            this,
            [this]() {
                hide();
                setMaximumWidth(QWIDGETSIZE_MAX);
                m_opacityEffect->setOpacity(1.0);
            });
    m_hideAnimation->start();
}

void AverraDrawer::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}
