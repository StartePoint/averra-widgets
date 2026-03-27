#include <Averra/AverraRibbonBar.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QVBoxLayout>

AverraRibbonBar::AverraRibbonBar(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QString AverraRibbonBar::title() const
{
    return m_title;
}

void AverraRibbonBar::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraRibbonBar::subtitle() const
{
    return m_subtitle;
}

void AverraRibbonBar::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle) {
        return;
    }

    m_subtitle = subtitle;
    m_subtitleLabel->setText(m_subtitle);
    updateTextVisibility();
    emit subtitleChanged(m_subtitle);
}

int AverraRibbonBar::currentPage() const
{
    return m_pagesStack->currentIndex();
}

void AverraRibbonBar::setCurrentPage(int index)
{
    if (index < 0 || index >= m_pages.size() || index == m_pagesStack->currentIndex()) {
        return;
    }

    m_pagesStack->setCurrentIndex(index);
    refreshPageButtons();
    emit currentPageChanged(index);
}

int AverraRibbonBar::pageCount() const
{
    return m_pages.size();
}

QHBoxLayout *AverraRibbonBar::leadingLayout() const
{
    return m_leadingLayout;
}

QHBoxLayout *AverraRibbonBar::trailingLayout() const
{
    return m_trailingLayout;
}

void AverraRibbonBar::addLeadingWidget(QWidget *widget)
{
    if (widget != nullptr) {
        m_leadingLayout->addWidget(widget);
    }
}

void AverraRibbonBar::addTrailingWidget(QWidget *widget)
{
    if (widget != nullptr) {
        m_trailingLayout->addWidget(widget);
    }
}

int AverraRibbonBar::addPage(const QString &title)
{
    RibbonPage page;
    page.button = new QPushButton(title, m_tabsWidget);
    page.widget = new QWidget(m_pagesStack);
    page.groupsLayout = new QHBoxLayout;

    page.button->setCheckable(true);
    page.button->setCursor(Qt::PointingHandCursor);

    QWidget *pageHost = page.widget;
    QHBoxLayout *pageLayout = page.groupsLayout;
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setSpacing(12);
    pageLayout->addStretch(1);
    pageHost->setLayout(pageLayout);

    connect(page.button,
            &QPushButton::clicked,
            this,
            &AverraRibbonBar::handlePageButtonClicked);

    m_tabsLayout->insertWidget(m_tabsLayout->count() - 1, page.button);
    m_pagesStack->addWidget(page.widget);
    m_pages.append(page);

    if (m_pages.size() == 1) {
        m_pagesStack->setCurrentIndex(0);
    }

    refreshPageButtons();
    return m_pages.size() - 1;
}

QWidget *AverraRibbonBar::pageWidget(int index) const
{
    if (index < 0 || index >= m_pages.size()) {
        return nullptr;
    }

    return m_pages.at(index).widget;
}

int AverraRibbonBar::addGroup(int pageIndex, const QString &title)
{
    if (pageIndex < 0 || pageIndex >= m_pages.size()) {
        return -1;
    }

    RibbonGroup group;
    group.frame = new QFrame(m_pages.at(pageIndex).widget);
    group.titleLabel = new QLabel(group.frame);
    group.contentWidget = new QWidget(group.frame);
    group.contentLayout = new QVBoxLayout;

    group.frame->setObjectName(QStringLiteral("AverraRibbonGroupRoot"));
    group.titleLabel->setObjectName(QStringLiteral("AverraRibbonGroupTitle"));
    group.titleLabel->setText(title);

    QVBoxLayout *groupLayout = new QVBoxLayout(group.frame);
    groupLayout->setContentsMargins(12, 12, 12, 12);
    groupLayout->setSpacing(10);

    group.contentLayout->setContentsMargins(0, 0, 0, 0);
    group.contentLayout->setSpacing(8);
    group.contentWidget->setLayout(group.contentLayout);

    groupLayout->addWidget(group.contentWidget);
    groupLayout->addWidget(group.titleLabel);

    RibbonPage &page = m_pages[pageIndex];
    page.groupsLayout->insertWidget(page.groupsLayout->count() - 1, group.frame);
    page.groups.append(group);

    refreshStyle();
    return page.groups.size() - 1;
}

QVBoxLayout *AverraRibbonBar::groupLayout(int pageIndex, int groupIndex) const
{
    if (pageIndex < 0 || pageIndex >= m_pages.size()) {
        return nullptr;
    }

    const RibbonPage &page = m_pages.at(pageIndex);
    if (groupIndex < 0 || groupIndex >= page.groups.size()) {
        return nullptr;
    }

    return page.groups.at(groupIndex).contentLayout;
}

void AverraRibbonBar::addGroupWidget(int pageIndex, int groupIndex, QWidget *widget)
{
    QVBoxLayout *layout = groupLayout(pageIndex, groupIndex);

    if (layout != nullptr && widget != nullptr) {
        layout->addWidget(widget);
    }
}

void AverraRibbonBar::initialize()
{
    m_title.clear();
    m_subtitle.clear();
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_subtitleLabel = new QLabel(m_rootFrame);
    m_leadingWidget = new QWidget(m_rootFrame);
    m_trailingWidget = new QWidget(m_rootFrame);
    m_leadingLayout = new QHBoxLayout;
    m_trailingLayout = new QHBoxLayout;
    m_tabsWidget = new QWidget(m_rootFrame);
    m_tabsLayout = new QHBoxLayout;
    m_pagesStack = new QStackedWidget(m_rootFrame);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_rootFrame->setObjectName(QStringLiteral("AverraRibbonBarRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraRibbonBarTitle"));
    m_subtitleLabel->setObjectName(QStringLiteral("AverraRibbonBarSubtitle"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(12);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(12);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(2);

    m_leadingLayout->setContentsMargins(0, 0, 0, 0);
    m_leadingLayout->setSpacing(8);
    m_leadingWidget->setLayout(m_leadingLayout);

    m_trailingLayout->setContentsMargins(0, 0, 0, 0);
    m_trailingLayout->setSpacing(8);
    m_trailingWidget->setLayout(m_trailingLayout);

    m_tabsLayout->setContentsMargins(0, 0, 0, 0);
    m_tabsLayout->setSpacing(8);
    m_tabsLayout->addStretch(1);
    m_tabsWidget->setLayout(m_tabsLayout);

    textLayout->addWidget(m_titleLabel);
    textLayout->addWidget(m_subtitleLabel);

    headerLayout->addWidget(m_leadingWidget, 0, Qt::AlignVCenter);
    headerLayout->addLayout(textLayout, 1);
    headerLayout->addWidget(m_trailingWidget, 0, Qt::AlignVCenter);

    rootLayout->addLayout(headerLayout);
    rootLayout->addWidget(m_tabsWidget);
    rootLayout->addWidget(m_pagesStack);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraRibbonBar::refreshStyle);

    updateTextVisibility();
    refreshStyle();
}

void AverraRibbonBar::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::ribbonBarFrameStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::ribbonBarTitleLabelStyleSheet(palette));
    m_subtitleLabel->setStyleSheet(AverraStyleHelper::ribbonBarSubtitleLabelStyleSheet(palette));

    for (int pageIndex = 0; pageIndex < m_pages.size(); ++pageIndex) {
        const bool current = (pageIndex == m_pagesStack->currentIndex());
        m_pages.at(pageIndex).button->setStyleSheet(AverraStyleHelper::ribbonBarTabButtonStyleSheet(palette, current));

        for (int groupIndex = 0; groupIndex < m_pages.at(pageIndex).groups.size(); ++groupIndex) {
            const RibbonGroup &group = m_pages.at(pageIndex).groups.at(groupIndex);
            group.frame->setStyleSheet(AverraStyleHelper::ribbonBarGroupFrameStyleSheet(palette));
            group.titleLabel->setStyleSheet(AverraStyleHelper::ribbonBarGroupTitleLabelStyleSheet(palette));
        }
    }
}

void AverraRibbonBar::handlePageButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button == nullptr) {
        return;
    }

    for (int index = 0; index < m_pages.size(); ++index) {
        if (m_pages.at(index).button == button) {
            setCurrentPage(index);
            return;
        }
    }
}

void AverraRibbonBar::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_subtitleLabel->setVisible(!m_subtitle.isEmpty());
}

void AverraRibbonBar::refreshPageButtons()
{
    for (int index = 0; index < m_pages.size(); ++index) {
        m_pages.at(index).button->setChecked(index == m_pagesStack->currentIndex());
    }

    refreshStyle();
}
