#include <Averra/AverraTag.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QSizePolicy>

namespace
{
AverraStyleHelper::TagTone convertTagTone(AverraTag::Tone tone)
{
    switch (tone) {
    case AverraTag::Neutral:
        return AverraStyleHelper::TagTone::Neutral;
    case AverraTag::Accent:
        return AverraStyleHelper::TagTone::Accent;
    case AverraTag::Success:
        return AverraStyleHelper::TagTone::Success;
    case AverraTag::Warning:
        return AverraStyleHelper::TagTone::Warning;
    case AverraTag::Error:
        return AverraStyleHelper::TagTone::Error;
    }

    return AverraStyleHelper::TagTone::Neutral;
}
}

AverraTag::AverraTag(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraTag::AverraTag(const QString &text, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    setText(text);
    initialize();
}

AverraTag::Tone AverraTag::tone() const
{
    return m_tone;
}

void AverraTag::setTone(Tone tone)
{
    if (m_tone == tone) {
        return;
    }

    m_tone = tone;
    refreshStyle();
    emit toneChanged(m_tone);
}

bool AverraTag::filled() const
{
    return m_filled;
}

void AverraTag::setFilled(bool filled)
{
    if (m_filled == filled) {
        return;
    }

    m_filled = filled;
    refreshStyle();
    emit filledChanged(m_filled);
}

void AverraTag::initialize()
{
    m_tone = Neutral;
    m_filled = true;

    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraTag::refreshStyle);

    refreshStyle();
}

void AverraTag::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::tagStyleSheet(palette, convertTagTone(m_tone), m_filled));
}
