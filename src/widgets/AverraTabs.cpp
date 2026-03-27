#include <Averra/AverraTabs.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

AverraTabs::AverraTabs(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

int AverraTabs::currentIndex() const
{
    return m_stack->currentIndex();
}

void AverraTabs::setCurrentIndex(int index)
{
    if (index < 0 || index >= m_stack->count() || index == m_stack->currentIndex()) {
        return;
    }

    m_stack->setCurrentIndex(index);

    for (int tabIndex = 0; tabIndex < m_tabButtons.size(); ++tabIndex) {
        m_tabButtons.at(tabIndex)->setChecked(tabIndex == index);
    }

    refreshStyle();
    emit currentIndexChanged(index);
}

int AverraTabs::count() const
{
    return m_stack->count();
}

void AverraTabs::addTab(QWidget *widget, const QString &title)
{
    if (widget == nullptr) {
        return;
    }

    QPushButton *button = new QPushButton(title, m_tabsWidget);
    button->setCheckable(true);
    button->setCursor(Qt::PointingHandCursor);
    connect(button, &QPushButton::clicked, this, &AverraTabs::handleTabClicked);

    m_tabsLayout->insertWidget(m_tabsLayout->count() - 1, button);
    m_tabButtons.append(button);
    m_stack->addWidget(widget);

    if (m_tabButtons.size() == 1) {
        button->setChecked(true);
        m_stack->setCurrentIndex(0);
    }

    refreshStyle();
}

void AverraTabs::initialize()
{
    m_tabsWidget = new QWidget(this);
    m_tabsLayout = new QHBoxLayout;
    m_stack = new QStackedWidget(this);

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(12);

    m_tabsLayout->setContentsMargins(0, 0, 0, 0);
    m_tabsLayout->setSpacing(8);
    m_tabsLayout->addStretch(1);
    m_tabsWidget->setLayout(m_tabsLayout);

    rootLayout->addWidget(m_tabsWidget);
    rootLayout->addWidget(m_stack);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraTabs::refreshStyle);
}

void AverraTabs::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    for (int index = 0; index < m_tabButtons.size(); ++index) {
        m_tabButtons.at(index)->setStyleSheet(AverraStyleHelper::tabButtonStyleSheet(palette, index == m_stack->currentIndex()));
    }
}

void AverraTabs::handleTabClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button == nullptr) {
        return;
    }

    const int index = m_tabButtons.indexOf(button);
    setCurrentIndex(index);
}

