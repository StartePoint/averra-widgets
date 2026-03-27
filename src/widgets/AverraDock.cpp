#include <Averra/AverraDock.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

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
    } else {
        hide();
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
    m_titleBarFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_titleBarFrame);
    m_descriptionLabel = new QLabel(m_titleBarFrame);
    m_closeButton = new QToolButton(m_titleBarFrame);
    m_rootFrame = new QFrame(this);
    m_contentHost = new QWidget(m_rootFrame);
    m_contentLayout = new QVBoxLayout;

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

    m_titleBarFrame->setObjectName(QStringLiteral("AverraDockTitleBar"));
    m_titleLabel->setObjectName(QStringLiteral("AverraDockTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraDockDescription"));
    m_closeButton->setObjectName(QStringLiteral("AverraDockCloseButton"));
    m_rootFrame->setObjectName(QStringLiteral("AverraDockRoot"));

    m_closeButton->setText(QStringLiteral("×"));
    m_closeButton->setCursor(Qt::PointingHandCursor);
    m_titleLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);

    QHBoxLayout *titleBarLayout = new QHBoxLayout(m_titleBarFrame);
    titleBarLayout->setContentsMargins(14, 10, 10, 10);
    titleBarLayout->setSpacing(10);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(2);
    textLayout->addWidget(m_titleLabel);
    textLayout->addWidget(m_descriptionLabel);

    titleBarLayout->addLayout(textLayout, 1);
    titleBarLayout->addWidget(m_closeButton, 0, Qt::AlignTop);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(14, 14, 14, 14);
    rootLayout->setSpacing(12);

    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(10);
    m_contentHost->setLayout(m_contentLayout);

    rootLayout->addWidget(m_contentHost);

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

    updateTextVisibility();
    refreshStyle();
}

void AverraDock::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    setStyleSheet(QStringLiteral("QDockWidget { border: none; background: transparent; }"));
    m_titleBarFrame->setStyleSheet(AverraStyleHelper::dockTitleBarFrameStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::dockTitleLabelStyleSheet(palette));
    m_descriptionLabel->setStyleSheet(AverraStyleHelper::dockDescriptionLabelStyleSheet(palette));
    m_closeButton->setStyleSheet(AverraStyleHelper::dockCloseButtonStyleSheet(palette));
    m_rootFrame->setStyleSheet(AverraStyleHelper::dockFrameStyleSheet(palette));
}

void AverraDock::syncOpenedState(bool visible)
{
    if (m_opened == visible) {
        return;
    }

    m_opened = visible;
    emit openedChanged(m_opened);
}

void AverraDock::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}
