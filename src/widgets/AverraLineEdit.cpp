#include <Averra/AverraLineEdit.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QSizePolicy>

AverraLineEdit::AverraLineEdit(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraLineEdit::AverraLineEdit(const QString &text, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    setText(text);
    initialize();
}

bool AverraLineEdit::accentFrame() const
{
    return m_accentFrame;
}

void AverraLineEdit::setAccentFrame(bool accentFrame)
{
    if (m_accentFrame == accentFrame) {
        return;
    }

    m_accentFrame = accentFrame;
    refreshStyle();
    emit accentFrameChanged(m_accentFrame);
}

void AverraLineEdit::initialize()
{
    m_accentFrame = false;

    setClearButtonEnabled(true);
    setMinimumHeight(42);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraLineEdit::refreshStyle);

    refreshStyle();
}

void AverraLineEdit::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::lineEditStyleSheet(palette, m_accentFrame));
}

