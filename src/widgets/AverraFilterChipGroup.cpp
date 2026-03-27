#include <Averra/AverraFilterChipGroup.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSizePolicy>

AverraFilterChipGroup::AverraFilterChipGroup(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

bool AverraFilterChipGroup::multiSelect() const
{
    return m_multiSelect;
}

void AverraFilterChipGroup::setMultiSelect(bool multiSelect)
{
    if (m_multiSelect == multiSelect) {
        return;
    }

    m_multiSelect = multiSelect;
    m_buttonGroup->setExclusive(!m_multiSelect);

    if (!m_multiSelect) {
        QList<int> indices = selectedIndices();
        if (indices.size() > 1) {
            setSelectedIndices(QList<int>() << indices.first());
        }
    }

    emit multiSelectChanged(m_multiSelect);
}

int AverraFilterChipGroup::chipCount() const
{
    return m_buttons.size();
}

QString AverraFilterChipGroup::chipText(int index) const
{
    if (index < 0 || index >= m_buttons.size()) {
        return QString();
    }

    return m_buttons.at(index)->text();
}

QList<int> AverraFilterChipGroup::selectedIndices() const
{
    QList<int> indices;

    for (int index = 0; index < m_buttons.size(); ++index) {
        if (m_buttons.at(index)->isChecked()) {
            indices.append(index);
        }
    }

    return indices;
}

void AverraFilterChipGroup::setSelectedIndices(const QList<int> &indices)
{
    for (int index = 0; index < m_buttons.size(); ++index) {
        const bool selected = indices.contains(index);
        if (!m_multiSelect && !indices.isEmpty() && index != indices.first()) {
            m_buttons.at(index)->setChecked(false);
        } else {
            m_buttons.at(index)->setChecked(selected);
        }
    }

    refreshSelectionState();
    emit selectedIndicesChanged(selectedIndices());
}

void AverraFilterChipGroup::addChip(const QString &text)
{
    QPushButton *button = new QPushButton(text, this);
    button->setCheckable(true);
    button->setCursor(Qt::PointingHandCursor);
    button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    button->setMinimumHeight(34);

    m_buttons.append(button);
    m_buttonGroup->addButton(button, m_buttons.size() - 1);
    m_layout->insertWidget(m_layout->count() - 1, button);

    if (m_buttons.size() == 1) {
        button->setChecked(true);
    }

    refreshSelectionState();
}

void AverraFilterChipGroup::clearChips()
{
    for (int index = 0; index < m_buttons.size(); ++index) {
        QPushButton *button = m_buttons.at(index);
        m_buttonGroup->removeButton(button);
        m_layout->removeWidget(button);
        delete button;
    }

    m_buttons.clear();
}

void AverraFilterChipGroup::initialize()
{
    m_multiSelect = false;
    m_buttonGroup = new QButtonGroup(this);
    m_layout = new QHBoxLayout(this);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_buttonGroup->setExclusive(true);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(8);
    m_layout->addStretch(1);

    connect(m_buttonGroup,
            SIGNAL(buttonClicked(int)),
            this,
            SLOT(handleButtonClicked(int)));

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraFilterChipGroup::refreshStyle);
}

void AverraFilterChipGroup::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    for (int index = 0; index < m_buttons.size(); ++index) {
        QPushButton *button = m_buttons.at(index);
        button->setStyleSheet(AverraStyleHelper::filterChipStyleSheet(palette, button->isChecked()));
    }
}

void AverraFilterChipGroup::handleButtonClicked(int id)
{
    if (id < 0 || id >= m_buttons.size()) {
        return;
    }

    if (m_multiSelect) {
        refreshSelectionState();
    }

    emit chipClicked(id, m_buttons.at(id)->isChecked());
    emit selectedIndicesChanged(selectedIndices());
}

void AverraFilterChipGroup::refreshSelectionState()
{
    refreshStyle();
}

