#include <Averra/AverraSegmentedControl.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QHBoxLayout>
#include <QPushButton>

AverraSegmentedControl::AverraSegmentedControl(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

int AverraSegmentedControl::currentIndex() const
{
    return m_currentIndex;
}

void AverraSegmentedControl::setCurrentIndex(int index)
{
    if (index < 0 || index >= m_buttons.size() || index == m_currentIndex) {
        return;
    }

    m_currentIndex = index;
    refreshStyle();
    emit currentIndexChanged(index);
}

int AverraSegmentedControl::count() const
{
    return m_buttons.size();
}

void AverraSegmentedControl::addSegment(const QString &text)
{
    QPushButton *button = new QPushButton(text, this);
    button->setCheckable(true);
    button->setCursor(Qt::PointingHandCursor);
    connect(button, &QPushButton::clicked, this, &AverraSegmentedControl::handleSegmentClicked);
    m_layout->addWidget(button);
    m_buttons.append(button);

    if (m_buttons.size() == 1) {
        m_currentIndex = 0;
    }

    refreshStyle();
}

void AverraSegmentedControl::initialize()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_currentIndex = -1;

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraSegmentedControl::refreshStyle);
}

void AverraSegmentedControl::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    for (int index = 0; index < m_buttons.size(); ++index) {
        QPushButton *button = m_buttons.at(index);
        button->setChecked(index == m_currentIndex);
        button->setStyleSheet(AverraStyleHelper::segmentedButtonStyleSheet(palette,
                                                                           index == m_currentIndex,
                                                                           index == 0,
                                                                           index == m_buttons.size() - 1));
    }
}

void AverraSegmentedControl::handleSegmentClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button == nullptr) {
        return;
    }

    setCurrentIndex(m_buttons.indexOf(button));
}

