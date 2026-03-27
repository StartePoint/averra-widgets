#include <Averra/AverraDock.h>
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
int dockTitleBarRadius(const AverraStyleProfile &styleProfile)
{
    return qMax(styleProfile.largeRadius(), styleProfile.cardRadius() - 4);
}

int dockWindowButtonWidth(const AverraStyleProfile &styleProfile)
{
    return qMax(24, styleProfile.windowControlButtonWidth() - 10);
}

int dockWindowButtonHeight(const AverraStyleProfile &styleProfile)
{
    return qMax(20, styleProfile.windowControlButtonHeight() - 6);
}

QString dockTitleBarStyleSheet(const AverraThemePalette &palette, const AverraStyleProfile &styleProfile)
{
    return QStringLiteral(
               "QFrame#AverraDockTitleBar {"
               "background-color: %1;"
               "border-bottom: 1px solid %2;"
               "border-top-left-radius: %3px;"
               "border-top-right-radius: %3px;"
               "}")
        .arg(palette.surfaceHoverColor().name(),
             palette.borderColor().name(),
             QString::number(dockTitleBarRadius(styleProfile)));
}

QString dockTrailingWindowControlButtonStyleSheet(const AverraThemePalette &palette,
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
             QString::number(dockWindowButtonWidth(styleProfile)),
             QString::number(dockWindowButtonHeight(styleProfile)),
             QString::number(qMax(10, styleProfile.smallFontSize() - 1)));
}

QString dockTrafficLightButtonStyleSheet(const AverraStyleProfile &styleProfile, const QColor &fillColor)
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

AverraDock::AverraDock(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraDock::AverraDock(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraDock::title() const
{
    return m_title;
}

void AverraDock::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    setWindowTitle(m_title);
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraDock::description() const
{
    return m_description;
}

void AverraDock::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

bool AverraDock::opened() const
{
    return m_opened;
}

void AverraDock::setOpened(bool opened)
{
    if (m_opened == opened) {
        return;
    }

    m_opened = opened;

    if (m_opened) {
        show();
        startShowAnimation();
    } else {
        startHideAnimation();
    }

    emit openedChanged(m_opened);
}

QWidget *AverraDock::contentWidget() const
{
    return m_contentHost;
}

QVBoxLayout *AverraDock::contentLayout() const
{
    return m_contentLayout;
}

void AverraDock::addContentWidget(QWidget *widget)
{
    if (widget != nullptr) {
        m_contentLayout->addWidget(widget);
    }
}

void AverraDock::initialize()
{
    m_title.clear();
    m_description.clear();
    m_opened = true;
    m_allowImmediateHide = false;
    m_titleBarFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_titleBarFrame);
    m_descriptionLabel = new QLabel(m_titleBarFrame);
    m_windowControlsWidget = new QWidget(m_titleBarFrame);
    m_minimizeButton = new QToolButton(m_windowControlsWidget);
    m_maximizeButton = new QToolButton(m_windowControlsWidget);
    m_closeButton = new QToolButton(m_windowControlsWidget);
    m_titleBarLayout = new QHBoxLayout;
    m_windowControlsLayout = new QHBoxLayout;
    m_rootFrame = new QFrame(this);
    m_contentHost = new QWidget(m_rootFrame);
    m_contentLayout = new QVBoxLayout;
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_showAnimation = new QParallelAnimationGroup(this);
    m_hideAnimation = new QParallelAnimationGroup(this);

    setObjectName(QStringLiteral("AverraDock"));
    setFeatures(QDockWidget::DockWidgetClosable
                | QDockWidget::DockWidgetMovable
                | QDockWidget::DockWidgetFloatable);
    setAllowedAreas(Qt::LeftDockWidgetArea
                    | Qt::RightDockWidgetArea
                    | Qt::BottomDockWidgetArea);
    setTitleBarWidget(m_titleBarFrame);
    setWidget(m_rootFrame);
    setMinimumWidth(280);
    setMinimumHeight(260);
    setGraphicsEffect(m_opacityEffect);
    m_opacityEffect->setOpacity(1.0);

    m_titleBarFrame->setObjectName(QStringLiteral("AverraDockTitleBar"));
    m_titleLabel->setObjectName(QStringLiteral("AverraDockTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraDockDescription"));
    m_minimizeButton->setObjectName(QStringLiteral("AverraDockMinimizeButton"));
    m_maximizeButton->setObjectName(QStringLiteral("AverraDockMaximizeButton"));
    m_closeButton->setObjectName(QStringLiteral("AverraDockCloseButton"));
    m_rootFrame->setObjectName(QStringLiteral("AverraDockRoot"));

    m_titleLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);

    m_windowControlsLayout->setContentsMargins(0, 0, 0, 0);
    m_windowControlsLayout->setSpacing(8);
    m_windowControlsWidget->setLayout(m_windowControlsLayout);

    m_titleBarLayout->setContentsMargins(14, 10, 10, 10);
    m_titleBarLayout->setSpacing(10);
    m_titleBarFrame->setLayout(m_titleBarLayout);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(14, 14, 14, 14);
    rootLayout->setSpacing(12);

    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(10);
    m_contentHost->setLayout(m_contentLayout);

    rootLayout->addWidget(m_contentHost);

    m_minimizeButton->setText(QStringLiteral("−"));
    m_maximizeButton->setText(QStringLiteral("⤢"));
    m_closeButton->setText(QStringLiteral("×"));
    m_minimizeButton->setCursor(Qt::PointingHandCursor);
    m_maximizeButton->setCursor(Qt::PointingHandCursor);
    m_closeButton->setCursor(Qt::PointingHandCursor);

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
                setFloating(!isFloating());
            });
    connect(m_closeButton,
            &QToolButton::clicked,
            this,
            [this]() {
                setOpened(false);
            });
    connect(this,
            &QDockWidget::visibilityChanged,
            this,
            &AverraDock::syncOpenedState);
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraDock::refreshStyle);
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

void AverraDock::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    QVBoxLayout *rootLayout = qobject_cast<QVBoxLayout *>(m_rootFrame->layout());

    m_windowControlsLayout->setSpacing(qMax(6, styleProfile.compactPaddingHorizontal() - 2));
    m_titleBarLayout->setContentsMargins(qMax(10, styleProfile.buttonPaddingHorizontal() - 4),
                                         qMax(8, styleProfile.buttonPaddingVertical()),
                                         qMax(10, styleProfile.compactPaddingHorizontal()),
                                         qMax(8, styleProfile.buttonPaddingVertical()));
    m_titleBarLayout->setSpacing(styleProfile.compactPaddingHorizontal());
    if (rootLayout != nullptr) {
        rootLayout->setContentsMargins(qMax(12, styleProfile.buttonPaddingHorizontal() - 4),
                                       qMax(12, styleProfile.buttonPaddingVertical() + 4),
                                       qMax(12, styleProfile.buttonPaddingHorizontal() - 4),
                                       qMax(12, styleProfile.buttonPaddingVertical() + 4));
        rootLayout->setSpacing(styleProfile.compactPaddingHorizontal() + 2);
    }
    m_contentLayout->setSpacing(styleProfile.compactPaddingHorizontal());

    setStyleSheet(QStringLiteral("QDockWidget { border: none; background: transparent; }"));
    m_titleBarFrame->setStyleSheet(dockTitleBarStyleSheet(palette, styleProfile));
    m_titleLabel->setStyleSheet(AverraStyleHelper::dockTitleLabelStyleSheet(palette));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::dockDescriptionLabelStyleSheet(palette));
    m_rootFrame->setStyleSheet(AverraStyleHelper::dockFrameStyleSheet(palette));
    updateWindowControlStyle();
}

void AverraDock::syncOpenedState(bool visible)
{
    if (m_allowImmediateHide) {
        m_allowImmediateHide = false;
        m_opened = visible;
        emit openedChanged(m_opened);
        return;
    }

    if (m_opened == visible) {
        return;
    }

    m_opened = visible;
    emit openedChanged(m_opened);
}

void AverraDock::updateWindowControlLayout()
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
        m_titleBarLayout->addSpacing(qMax(6, styleProfile.compactPaddingHorizontal() - 2));

        QVBoxLayout *textLayout = new QVBoxLayout;
        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(qMax(1, styleProfile.compactPaddingVertical() - 5));
        textLayout->addWidget(m_titleLabel);
        textLayout->addWidget(m_descriptionLabel);
        m_titleBarLayout->addLayout(textLayout, 1);
        return;
    }

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(qMax(1, styleProfile.compactPaddingVertical() - 5));
    textLayout->addWidget(m_titleLabel);
    textLayout->addWidget(m_descriptionLabel);

    m_windowControlsLayout->addWidget(m_minimizeButton);
    m_windowControlsLayout->addWidget(m_maximizeButton);
    m_windowControlsLayout->addWidget(m_closeButton);

    m_titleBarLayout->addLayout(textLayout, 1);
    m_titleBarLayout->addWidget(m_windowControlsWidget, 0, Qt::AlignTop);
}

void AverraDock::updateWindowControlStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    const bool useLeadingTrafficLights = styleProfile.windowControlsLayout() == AverraStyleProfile::LeadingTrafficLights;

    if (useLeadingTrafficLights) {
        m_minimizeButton->setText(QString());
        m_maximizeButton->setText(QString());
        m_closeButton->setText(QString());
        m_minimizeButton->setStyleSheet(dockTrafficLightButtonStyleSheet(styleProfile, QColor(QStringLiteral("#F5BF4F"))));
        m_maximizeButton->setStyleSheet(dockTrafficLightButtonStyleSheet(styleProfile, QColor(QStringLiteral("#61C454"))));
        m_closeButton->setStyleSheet(dockTrafficLightButtonStyleSheet(styleProfile, QColor(QStringLiteral("#EE6A5F"))));
        return;
    }

    m_minimizeButton->setText(QStringLiteral("−"));
    m_maximizeButton->setText(isFloating() ? QStringLiteral("❐") : QStringLiteral("⤢"));
    m_closeButton->setText(QStringLiteral("×"));
    m_minimizeButton->setStyleSheet(dockTrailingWindowControlButtonStyleSheet(palette, styleProfile, QStringLiteral("minimize")));
    m_maximizeButton->setStyleSheet(dockTrailingWindowControlButtonStyleSheet(palette, styleProfile, QStringLiteral("maximize")));
    m_closeButton->setStyleSheet(dockTrailingWindowControlButtonStyleSheet(palette, styleProfile, QStringLiteral("close")));
}

void AverraDock::startShowAnimation()
{
    m_showAnimation->stop();
    while (m_showAnimation->animationCount() > 0) {
        QAbstractAnimation *animation = m_showAnimation->animationAt(0);
        m_showAnimation->removeAnimation(animation);
        delete animation;
    }

    m_opacityEffect->setOpacity(0.0);

    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", m_showAnimation);
    opacityAnimation->setDuration(AverraThemeManager::instance()->styleProfile().panelShowDuration());
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    m_showAnimation->addAnimation(opacityAnimation);
    m_showAnimation->start();
}

void AverraDock::startHideAnimation()
{
    if (!isVisible()) {
        return;
    }

    m_hideAnimation->stop();
    while (m_hideAnimation->animationCount() > 0) {
        QAbstractAnimation *animation = m_hideAnimation->animationAt(0);
        m_hideAnimation->removeAnimation(animation);
        delete animation;
    }
    disconnect(m_hideAnimation, nullptr, this, nullptr);

    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", m_hideAnimation);
    opacityAnimation->setDuration(AverraThemeManager::instance()->styleProfile().panelHideDuration());
    opacityAnimation->setStartValue(m_opacityEffect->opacity());
    opacityAnimation->setEndValue(0.0);
    m_hideAnimation->addAnimation(opacityAnimation);

    connect(m_hideAnimation,
            &QParallelAnimationGroup::finished,
            this,
            [this]() {
                m_allowImmediateHide = true;
                hide();
                m_opacityEffect->setOpacity(1.0);
            });
    m_hideAnimation->start();
}

void AverraDock::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}
