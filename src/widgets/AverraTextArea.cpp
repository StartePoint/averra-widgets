#include <Averra/AverraTextArea.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QSizePolicy>

AverraTextArea::AverraTextArea(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

bool AverraTextArea::accentFrame() const
{
    return m_accentFrame;
}

void AverraTextArea::setAccentFrame(bool accentFrame)
{
    if (m_accentFrame == accentFrame) {
        return;
    }

    m_accentFrame = accentFrame;
    refreshStyle();
    emit accentFrameChanged(m_accentFrame);
}

void AverraTextArea::initialize()
{
    m_accentFrame = false;

    setMinimumHeight(96);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setAcceptRichText(false);
    setTabChangesFocus(true);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraTextArea::refreshStyle);

    refreshStyle();
}

void AverraTextArea::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::textAreaStyleSheet(palette, m_accentFrame));
}

