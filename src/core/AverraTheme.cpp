#include <Averra/AverraTheme.h>

namespace
{
QColor colorOrFallback(const QColor &preferred, const QColor &fallback)
{
    if (preferred.isValid()) {
        return preferred;
    }

    return fallback;
}
}

AverraThemePalette::AverraThemePalette()
{
}

AverraThemePalette AverraThemePalette::createLightPalette()
{
    AverraThemePalette palette;
    palette.setAccentColor(QColor(QStringLiteral("#2E6BE6")));
    palette.setSurfaceColor(QColor(QStringLiteral("#F7F8FA")));
    palette.setSurfaceRaisedColor(QColor(QStringLiteral("#FFFFFF")));
    palette.setSurfaceHoverColor(QColor(QStringLiteral("#F1F4F8")));
    palette.setSurfacePressedColor(QColor(QStringLiteral("#E7ECF3")));
    palette.setSurfaceDisabledColor(QColor(QStringLiteral("#EEF2F7")));
    palette.setBorderColor(QColor(QStringLiteral("#D5DBE5")));
    palette.setBorderHoverColor(QColor(QStringLiteral("#A9B4C4")));
    palette.setTextPrimaryColor(QColor(QStringLiteral("#1D2533")));
    palette.setTextSecondaryColor(QColor(QStringLiteral("#5B6472")));
    palette.setTextDisabledColor(QColor(QStringLiteral("#98A2B3")));
    palette.setAccentSurfaceColor(QColor(QStringLiteral("#EDF3FF")));
    palette.setAccentHoverColor(QColor(QStringLiteral("#3B78F0")));
    palette.setAccentPressedColor(QColor(QStringLiteral("#245FD7")));
    palette.setAccentDisabledColor(QColor(QStringLiteral("#A8C2F5")));
    palette.setSuccessColor(QColor(QStringLiteral("#15803D")));
    palette.setSuccessSurfaceColor(QColor(QStringLiteral("#ECFDF3")));
    palette.setWarningColor(QColor(QStringLiteral("#B45309")));
    palette.setWarningSurfaceColor(QColor(QStringLiteral("#FFF7ED")));
    palette.setErrorColor(QColor(QStringLiteral("#B91C1C")));
    palette.setErrorSurfaceColor(QColor(QStringLiteral("#FEF2F2")));
    return palette;
}

QColor AverraThemePalette::accentColor() const
{
    return m_accentColor;
}

void AverraThemePalette::setAccentColor(const QColor &color)
{
    m_accentColor = color;
}

QColor AverraThemePalette::surfaceColor() const
{
    return m_surfaceColor;
}

void AverraThemePalette::setSurfaceColor(const QColor &color)
{
    m_surfaceColor = color;
}

QColor AverraThemePalette::surfaceRaisedColor() const
{
    return m_surfaceRaisedColor;
}

void AverraThemePalette::setSurfaceRaisedColor(const QColor &color)
{
    m_surfaceRaisedColor = color;
}

QColor AverraThemePalette::surfaceHoverColor() const
{
    if (m_surfaceRaisedColor.isValid()) {
        return colorOrFallback(m_surfaceHoverColor, m_surfaceRaisedColor.darker(103));
    }

    return colorOrFallback(m_surfaceHoverColor, QColor(QStringLiteral("#F1F4F8")));
}

void AverraThemePalette::setSurfaceHoverColor(const QColor &color)
{
    m_surfaceHoverColor = color;
}

QColor AverraThemePalette::surfacePressedColor() const
{
    if (m_surfaceRaisedColor.isValid()) {
        return colorOrFallback(m_surfacePressedColor, m_surfaceRaisedColor.darker(108));
    }

    return colorOrFallback(m_surfacePressedColor, QColor(QStringLiteral("#E7ECF3")));
}

void AverraThemePalette::setSurfacePressedColor(const QColor &color)
{
    m_surfacePressedColor = color;
}

QColor AverraThemePalette::surfaceDisabledColor() const
{
    if (m_surfaceColor.isValid()) {
        return colorOrFallback(m_surfaceDisabledColor, m_surfaceColor.darker(102));
    }

    return colorOrFallback(m_surfaceDisabledColor, QColor(QStringLiteral("#EEF2F7")));
}

void AverraThemePalette::setSurfaceDisabledColor(const QColor &color)
{
    m_surfaceDisabledColor = color;
}

QColor AverraThemePalette::borderColor() const
{
    return m_borderColor;
}

void AverraThemePalette::setBorderColor(const QColor &color)
{
    m_borderColor = color;
}

QColor AverraThemePalette::borderHoverColor() const
{
    if (m_accentColor.isValid()) {
        return colorOrFallback(m_borderHoverColor, m_accentColor);
    }

    return colorOrFallback(m_borderHoverColor, m_borderColor);
}

void AverraThemePalette::setBorderHoverColor(const QColor &color)
{
    m_borderHoverColor = color;
}

QColor AverraThemePalette::textPrimaryColor() const
{
    return m_textPrimaryColor;
}

void AverraThemePalette::setTextPrimaryColor(const QColor &color)
{
    m_textPrimaryColor = color;
}

QColor AverraThemePalette::textSecondaryColor() const
{
    return m_textSecondaryColor;
}

void AverraThemePalette::setTextSecondaryColor(const QColor &color)
{
    m_textSecondaryColor = color;
}

QColor AverraThemePalette::textDisabledColor() const
{
    if (m_textSecondaryColor.isValid()) {
        return colorOrFallback(m_textDisabledColor, m_textSecondaryColor.lighter(145));
    }

    return colorOrFallback(m_textDisabledColor, QColor(QStringLiteral("#98A2B3")));
}

void AverraThemePalette::setTextDisabledColor(const QColor &color)
{
    m_textDisabledColor = color;
}

QColor AverraThemePalette::accentSurfaceColor() const
{
    return m_accentSurfaceColor;
}

void AverraThemePalette::setAccentSurfaceColor(const QColor &color)
{
    m_accentSurfaceColor = color;
}

QColor AverraThemePalette::accentHoverColor() const
{
    if (m_accentColor.isValid()) {
        return colorOrFallback(m_accentHoverColor, m_accentColor.lighter(110));
    }

    return colorOrFallback(m_accentHoverColor, QColor(QStringLiteral("#3B78F0")));
}

void AverraThemePalette::setAccentHoverColor(const QColor &color)
{
    m_accentHoverColor = color;
}

QColor AverraThemePalette::accentPressedColor() const
{
    if (m_accentColor.isValid()) {
        return colorOrFallback(m_accentPressedColor, m_accentColor.darker(110));
    }

    return colorOrFallback(m_accentPressedColor, QColor(QStringLiteral("#245FD7")));
}

void AverraThemePalette::setAccentPressedColor(const QColor &color)
{
    m_accentPressedColor = color;
}

QColor AverraThemePalette::accentDisabledColor() const
{
    if (m_accentColor.isValid()) {
        return colorOrFallback(m_accentDisabledColor, m_accentColor.lighter(150));
    }

    return colorOrFallback(m_accentDisabledColor, QColor(QStringLiteral("#A8C2F5")));
}

void AverraThemePalette::setAccentDisabledColor(const QColor &color)
{
    m_accentDisabledColor = color;
}

QColor AverraThemePalette::successColor() const
{
    return m_successColor;
}

void AverraThemePalette::setSuccessColor(const QColor &color)
{
    m_successColor = color;
}

QColor AverraThemePalette::successSurfaceColor() const
{
    return m_successSurfaceColor;
}

void AverraThemePalette::setSuccessSurfaceColor(const QColor &color)
{
    m_successSurfaceColor = color;
}

QColor AverraThemePalette::warningColor() const
{
    return m_warningColor;
}

void AverraThemePalette::setWarningColor(const QColor &color)
{
    m_warningColor = color;
}

QColor AverraThemePalette::warningSurfaceColor() const
{
    return m_warningSurfaceColor;
}

void AverraThemePalette::setWarningSurfaceColor(const QColor &color)
{
    m_warningSurfaceColor = color;
}

QColor AverraThemePalette::errorColor() const
{
    return m_errorColor;
}

void AverraThemePalette::setErrorColor(const QColor &color)
{
    m_errorColor = color;
}

QColor AverraThemePalette::errorSurfaceColor() const
{
    return m_errorSurfaceColor;
}

void AverraThemePalette::setErrorSurfaceColor(const QColor &color)
{
    m_errorSurfaceColor = color;
}
