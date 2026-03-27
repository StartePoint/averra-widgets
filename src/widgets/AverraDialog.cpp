#include <Averra/AverraDialog.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QCloseEvent>
#include <QAbstractAnimation>
#include <QEvent>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QShowEvent>
#include <QToolButton>
#include <QtGlobal>
#include <QVBoxLayout>

namespace
{
QPoint dialogAnimationOffset(const AverraStyleProfile &styleProfile, bool closing)
{
    return QPoint(0, closing ? styleProfile.dialogHideOffset() : styleProfile.dialogShowOffset());
}

int dialogTitleBarRadius(const AverraStyleProfile &styleProfile)
{
    return qMax(styleProfile.largeRadius(), styleProfile.cardRadius() - 4);
}

QString dialogTitleBarStyleSheet(const AverraThemePalette &palette, const AverraStyleProfile &styleProfile)
{
    return QStringLiteral(
               "QFrame#AverraDialogTitleBar {"
               "background-color: %1;"
               "border-bottom: 1px solid %2;"
               "border-top-left-radius: %3px;"
               "border-top-right-radius: %3px;"
               "}")
        .arg(palette.surfaceHoverColor().name(),
             palette.borderColor().name(),
             QString::number(dialogTitleBarRadius(styleProfile)));
}

QString trailingWindowControlButtonStyleSheet(const AverraThemePalette &palette,
                                              const AverraStyleProfile &styleProfile,
                                              const QString &role)
{
    QColor hoverColor = palette.surfaceHoverColor();
    QColor pressedColor = palette.surfacePressedColor();
    QColor textColor = palette.textSecondaryColor();

    if (role == QStringLiteral("close")) {
        hoverColor = palette.errorColor();
        pressedColor = palette.errorColor().darker(110);
        textColor = palette.textPrimaryColor();
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
               "font-weight: 600;"
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
             QString::number(styleProfile.windowControlButtonWidth()),
             QString::number(styleProfile.windowControlButtonHeight()),
             QString::number(styleProfile.bodyFontSize()));
}

QString trafficLightWindowControlButtonStyleSheet(const AverraStyleProfile &styleProfile, const QColor &fillColor)
{
    const int diameter = qMax(10, styleProfile.trafficLightDiameter());
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
    setWindowTitle(m_title);
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

bool AverraDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_titleBarFrame && event != nullptr) {
        const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();

        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

            if (mouseEvent->button() == Qt::LeftButton
                && styleProfile.enableTitleBarDrag()) {
                m_dragging = true;
                m_dragOffset = mouseEvent->globalPos() - frameGeometry().topLeft();
                return true;
            }
        } else if (event->type() == QEvent::MouseMove) {
            if (m_dragging) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                move(mouseEvent->globalPos() - m_dragOffset);
                return true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            m_dragging = false;
        } else if (event->type() == QEvent::MouseButtonDblClick) {
            if (styleProfile.enableTitleBarDoubleClickZoom()) {
                isMaximized() ? showNormal() : showMaximized();
                return true;
            }
        }
    }

    return QDialog::eventFilter(watched, event);
}

void AverraDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    startShowAnimation();
}

void AverraDialog::closeEvent(QCloseEvent *event)
{
    if (m_allowImmediateClose) {
        m_allowImmediateClose = false;
        QDialog::closeEvent(event);
        return;
    }

    event->ignore();
    startCloseAnimation();
}

void AverraDialog::initialize()
{
    m_title.clear();
    m_description.clear();
    m_dragging = false;
    m_allowImmediateClose = false;
    setModal(false);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_DeleteOnClose, false);
    resize(520, 320);

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
    m_closeAnimation = new QParallelAnimationGroup(this);

    setGraphicsEffect(m_opacityEffect);
    m_opacityEffect->setOpacity(1.0);

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    m_rootFrame->setObjectName(QStringLiteral("AverraDialogRoot"));
    m_titleBarFrame->setObjectName(QStringLiteral("AverraDialogTitleBar"));
    m_titleLabel->setObjectName(QStringLiteral("AverraDialogTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraDialogDescription"));
    m_minimizeButton->setObjectName(QStringLiteral("AverraDialogMinimizeButton"));
    m_maximizeButton->setObjectName(QStringLiteral("AverraDialogMaximizeButton"));
    m_closeButton->setObjectName(QStringLiteral("AverraDialogCloseButton"));

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
    m_titleBarFrame->installEventFilter(this);

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
    m_maximizeButton->setText(QStringLiteral("□"));
    m_closeButton->setText(QStringLiteral("×"));

    connect(m_minimizeButton,
            &QToolButton::clicked,
            this,
            [this]() {
                showMinimized();
            });
    connect(m_maximizeButton,
            &QToolButton::clicked,
            this,
            [this]() {
                isMaximized() ? showNormal() : showMaximized();
            });
    connect(m_closeButton,
            &QToolButton::clicked,
            this,
            [this]() {
                startCloseAnimation();
            });
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraDialog::refreshStyle);
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

void AverraDialog::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();

    m_windowControlsLayout->setSpacing(qMax(6, styleProfile.compactPaddingHorizontal() - 2));
    m_titleBarLayout->setContentsMargins(qMax(10, styleProfile.buttonPaddingHorizontal() - 4),
                                         qMax(8, styleProfile.buttonPaddingVertical() + 2),
                                         qMax(10, styleProfile.buttonPaddingHorizontal() - 4),
                                         qMax(8, styleProfile.buttonPaddingVertical() + 2));
    m_titleBarLayout->setSpacing(styleProfile.compactPaddingHorizontal());
    m_rootFrame->setStyleSheet(AverraStyleHelper::dialogFrameStyleSheet(palette));
    m_titleBarFrame->setStyleSheet(dialogTitleBarStyleSheet(palette, styleProfile));
    m_titleLabel->setStyleSheet(AverraStyleHelper::dialogTitleLabelStyleSheet(palette));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::dialogDescriptionLabelStyleSheet(palette));
    updateWindowControlStyle();
}

void AverraDialog::updateWindowControlLayout()
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
        m_titleBarLayout->addSpacing(styleProfile.windowControlButtonWidth() + 6);
        return;
    }

    m_windowControlsLayout->addWidget(m_minimizeButton);
    m_windowControlsLayout->addWidget(m_maximizeButton);
    m_windowControlsLayout->addWidget(m_closeButton);
    m_titleBarLayout->addWidget(m_titleLabel, 1, Qt::AlignVCenter);
    m_titleBarLayout->addWidget(m_windowControlsWidget, 0, Qt::AlignTop);
}

void AverraDialog::updateWindowControlStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    const bool useLeadingTrafficLights = styleProfile.windowControlsLayout() == AverraStyleProfile::LeadingTrafficLights;

    if (useLeadingTrafficLights) {
        m_minimizeButton->setText(QString());
        m_maximizeButton->setText(QString());
        m_closeButton->setText(QString());
        m_minimizeButton->setStyleSheet(trafficLightWindowControlButtonStyleSheet(styleProfile, QColor(QStringLiteral("#F5BF4F"))));
        m_maximizeButton->setStyleSheet(trafficLightWindowControlButtonStyleSheet(styleProfile, QColor(QStringLiteral("#61C454"))));
        m_closeButton->setStyleSheet(trafficLightWindowControlButtonStyleSheet(styleProfile, QColor(QStringLiteral("#EE6A5F"))));
        return;
    }

    m_minimizeButton->setText(QStringLiteral("−"));
    m_maximizeButton->setText(isMaximized() ? QStringLiteral("[ ]") : QStringLiteral("□"));
    m_closeButton->setText(QStringLiteral("×"));
    m_minimizeButton->setStyleSheet(trailingWindowControlButtonStyleSheet(palette, styleProfile, QStringLiteral("minimize")));
    m_maximizeButton->setStyleSheet(trailingWindowControlButtonStyleSheet(palette, styleProfile, QStringLiteral("maximize")));
    m_closeButton->setStyleSheet(trailingWindowControlButtonStyleSheet(palette, styleProfile, QStringLiteral("close")));
}

void AverraDialog::startShowAnimation()
{
    if (!isVisible()) {
        return;
    }

    m_showAnimation->stop();
    while (m_showAnimation->animationCount() > 0) {
        QAbstractAnimation *animation = m_showAnimation->animationAt(0);
        m_showAnimation->removeAnimation(animation);
        delete animation;
    }

    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", m_showAnimation);
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    opacityAnimation->setDuration(styleProfile.dialogShowDuration());
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);

    QPropertyAnimation *geometryAnimation = new QPropertyAnimation(this, "geometry", m_showAnimation);
    geometryAnimation->setDuration(styleProfile.dialogShowDuration());
    const QRect endRect = geometry();
    QRect startRect = endRect;
    startRect.translate(dialogAnimationOffset(styleProfile, false));
    startRect.setWidth(qRound(endRect.width() * 0.96));
    startRect.setHeight(qRound(endRect.height() * 0.96));
    startRect.moveCenter(endRect.center() + QPoint(0, styleProfile.dialogShowOffset() / 2));
    geometryAnimation->setStartValue(startRect);
    geometryAnimation->setEndValue(endRect);

    m_showAnimation->addAnimation(opacityAnimation);
    m_showAnimation->addAnimation(geometryAnimation);
    m_showAnimation->start();
}

void AverraDialog::startCloseAnimation()
{
    if (!isVisible()) {
        return;
    }

    m_closeAnimation->stop();
    while (m_closeAnimation->animationCount() > 0) {
        QAbstractAnimation *animation = m_closeAnimation->animationAt(0);
        m_closeAnimation->removeAnimation(animation);
        delete animation;
    }
    disconnect(m_closeAnimation, nullptr, this, nullptr);

    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(m_opacityEffect, "opacity", m_closeAnimation);
    opacityAnimation->setDuration(styleProfile.dialogHideDuration());
    opacityAnimation->setStartValue(m_opacityEffect->opacity());
    opacityAnimation->setEndValue(0.0);

    QPropertyAnimation *geometryAnimation = new QPropertyAnimation(this, "geometry", m_closeAnimation);
    geometryAnimation->setDuration(styleProfile.dialogHideDuration());
    const QRect startRect = geometry();
    QRect endRect = startRect;
    endRect.translate(dialogAnimationOffset(styleProfile, true));
    endRect.setWidth(qRound(startRect.width() * 0.97));
    endRect.setHeight(qRound(startRect.height() * 0.97));
    endRect.moveCenter(startRect.center() + QPoint(0, styleProfile.dialogHideOffset() / 2));
    geometryAnimation->setStartValue(startRect);
    geometryAnimation->setEndValue(endRect);

    m_closeAnimation->addAnimation(opacityAnimation);
    m_closeAnimation->addAnimation(geometryAnimation);

    connect(m_closeAnimation,
            &QParallelAnimationGroup::finished,
            this,
            [this]() {
                m_allowImmediateClose = true;
                close();
                m_opacityEffect->setOpacity(1.0);
            });

    m_closeAnimation->start();
}

void AverraDialog::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}
