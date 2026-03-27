#include <Averra/AverraNavigationPanel.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QButtonGroup>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

AverraNavigationPanel::AverraNavigationPanel(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraNavigationPanel::AverraNavigationPanel(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraNavigationPanel::title() const
{
    return m_title;
}

void AverraNavigationPanel::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    m_titleLabel->setVisible(!m_title.isEmpty());
    emit titleChanged(m_title);
}

int AverraNavigationPanel::currentIndex() const
{
    return m_currentIndex;
}

void AverraNavigationPanel::setCurrentIndex(int index)
{
    if (index < 0 || index >= m_itemButtons.size()) {
        return;
    }

    if (m_currentIndex == index) {
        return;
    }

    m_currentIndex = index;
    refreshCurrentState();
    emit currentIndexChanged(m_currentIndex);
}

int AverraNavigationPanel::itemCount() const
{
    return m_itemButtons.size();
}

QString AverraNavigationPanel::itemText(int index) const
{
    if (index < 0 || index >= m_itemButtons.size()) {
        return QString();
    }

    return m_itemButtons.at(index)->text();
}

void AverraNavigationPanel::addItem(const QString &text)
{
    QPushButton *button = new QPushButton(m_itemsWidget);
    button->setText(text);
    button->setCheckable(true);
    button->setCursor(Qt::PointingHandCursor);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    button->setMinimumHeight(40);

    m_itemsLayout->insertWidget(m_itemsLayout->count() - 1, button);
    m_itemButtons.append(button);

    rebuildButtonIds();
    refreshCurrentState();
}

void AverraNavigationPanel::clearItems()
{
    for (int index = 0; index < m_itemButtons.size(); ++index) {
        QPushButton *button = m_itemButtons.at(index);
        m_buttonGroup->removeButton(button);
        m_itemsLayout->removeWidget(button);
        delete button;
    }

    m_itemButtons.clear();
    m_currentIndex = -1;
    refreshStyle();
}

void AverraNavigationPanel::initialize()
{
    m_title.clear();
    m_currentIndex = -1;
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_itemsWidget = new QWidget(m_rootFrame);
    m_itemsLayout = new QVBoxLayout;
    m_buttonGroup = new QButtonGroup(this);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_rootFrame->setObjectName(QStringLiteral("AverraNavigationPanelRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraNavigationPanelTitle"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(10);

    m_titleLabel->setVisible(false);

    m_itemsLayout->setContentsMargins(0, 0, 0, 0);
    m_itemsLayout->setSpacing(8);
    m_itemsLayout->addStretch(1);
    m_itemsWidget->setLayout(m_itemsLayout);

    rootLayout->addWidget(m_titleLabel);
    rootLayout->addWidget(m_itemsWidget);

    connect(m_buttonGroup,
            SIGNAL(buttonClicked(int)),
            this,
            SLOT(handleButtonGroupClicked(int)));

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraNavigationPanel::refreshStyle);

    refreshStyle();
}

void AverraNavigationPanel::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::navigationPanelFrameStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::navigationPanelTitleLabelStyleSheet(palette));

    for (int index = 0; index < m_itemButtons.size(); ++index) {
        QPushButton *button = m_itemButtons.at(index);
        const bool current = (index == m_currentIndex);
        button->setStyleSheet(AverraStyleHelper::navigationPanelItemButtonStyleSheet(palette, current));
    }
}

void AverraNavigationPanel::handleButtonGroupClicked(int id)
{
    setCurrentIndex(id);
    emit itemClicked(id);
}

void AverraNavigationPanel::rebuildButtonIds()
{
    for (int index = 0; index < m_itemButtons.size(); ++index) {
        QPushButton *button = m_itemButtons.at(index);
        m_buttonGroup->removeButton(button);
        button->setObjectName(QStringLiteral("AverraNavigationItemButton_%1").arg(index));
        m_buttonGroup->addButton(button, index);
    }

    if (m_currentIndex < 0 && !m_itemButtons.isEmpty()) {
        m_currentIndex = 0;
    } else if (m_currentIndex >= m_itemButtons.size()) {
        m_currentIndex = m_itemButtons.size() - 1;
    }
}

void AverraNavigationPanel::refreshCurrentState()
{
    for (int index = 0; index < m_itemButtons.size(); ++index) {
        QPushButton *button = m_itemButtons.at(index);
        button->setChecked(index == m_currentIndex);
    }

    refreshStyle();
}
