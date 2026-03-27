#include <Averra/AverraComboBox.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QSizePolicy>

AverraComboBox::AverraComboBox(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

bool AverraComboBox::accentFrame() const
{
    return m_accentFrame;
}

void AverraComboBox::setAccentFrame(bool accentFrame)
{
    if (m_accentFrame == accentFrame) {
        return;
    }

    m_accentFrame = accentFrame;
    refreshStyle();
    emit accentFrameChanged(m_accentFrame);
}

void AverraComboBox::initialize()
{
    m_accentFrame = false;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(42);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraComboBox::refreshStyle);

    refreshStyle();
}

void AverraComboBox::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::comboBoxStyleSheet(palette, m_accentFrame));
}

