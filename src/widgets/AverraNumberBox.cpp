#include <Averra/AverraNumberBox.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QSizePolicy>

AverraNumberBox::AverraNumberBox(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

bool AverraNumberBox::accentFrame() const
{
    return m_accentFrame;
}

void AverraNumberBox::setAccentFrame(bool accentFrame)
{
    if (m_accentFrame == accentFrame) {
        return;
    }

    m_accentFrame = accentFrame;
    refreshStyle();
    emit accentFrameChanged(m_accentFrame);
}

void AverraNumberBox::initialize()
{
    m_accentFrame = false;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(42);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraNumberBox::refreshStyle);

    refreshStyle();
}

void AverraNumberBox::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::numberBoxStyleSheet(palette, m_accentFrame));
}

