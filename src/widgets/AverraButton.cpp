#include <Averra/AverraButton.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QSizePolicy>

AverraButton::AverraButton(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraButton::AverraButton(const QString &text, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    setText(text);
    initialize();
}

bool AverraButton::accent() const
{
    return m_accent;
}

void AverraButton::setAccent(bool accent)
{
    if (m_accent == accent) {
        return;
    }

    m_accent = accent;
    refreshStyle();
    emit accentChanged(m_accent);
}

void AverraButton::initialize()
{
    m_accent = false;

    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setMinimumHeight(42);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraButton::refreshStyle);

    refreshStyle();
}

void AverraButton::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::buttonStyleSheet(palette, m_accent));
}

