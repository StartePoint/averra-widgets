#include <Averra/AverraThemeManager.h>

#include <QApplication>

namespace
{
QColor themeAccentColor(AverraThemeManager::ThemePreset themePreset)
{
    switch (themePreset) {
    case AverraThemeManager::OceanTheme:
        return QColor(QStringLiteral("#2E6BE6"));
    case AverraThemeManager::ForestTheme:
        return QColor(QStringLiteral("#2E8B57"));
    case AverraThemeManager::SunsetTheme:
        return QColor(QStringLiteral("#D97706"));
    case AverraThemeManager::OrchidTheme:
        return QColor(QStringLiteral("#7C3AED"));
    case AverraThemeManager::CustomTheme:
        return QColor(QStringLiteral("#2E6BE6"));
    }

    return QColor(QStringLiteral("#2E6BE6"));
}

QColor themeAccentSurfaceColor(AverraThemeManager::ThemePreset themePreset)
{
    switch (themePreset) {
    case AverraThemeManager::OceanTheme:
        return QColor(QStringLiteral("#EDF3FF"));
    case AverraThemeManager::ForestTheme:
        return QColor(QStringLiteral("#EAF7F0"));
    case AverraThemeManager::SunsetTheme:
        return QColor(QStringLiteral("#FFF3E6"));
    case AverraThemeManager::OrchidTheme:
        return QColor(QStringLiteral("#F4EDFF"));
    case AverraThemeManager::CustomTheme:
        return QColor(QStringLiteral("#EDF3FF"));
    }

    return QColor(QStringLiteral("#EDF3FF"));
}

QColor themeAccentHoverColor(AverraThemeManager::ThemePreset themePreset)
{
    switch (themePreset) {
    case AverraThemeManager::OceanTheme:
        return QColor(QStringLiteral("#3B78F0"));
    case AverraThemeManager::ForestTheme:
        return QColor(QStringLiteral("#389E66"));
    case AverraThemeManager::SunsetTheme:
        return QColor(QStringLiteral("#EA8A16"));
    case AverraThemeManager::OrchidTheme:
        return QColor(QStringLiteral("#8B5CF6"));
    case AverraThemeManager::CustomTheme:
        return QColor(QStringLiteral("#3B78F0"));
    }

    return QColor(QStringLiteral("#3B78F0"));
}

QColor themeAccentPressedColor(AverraThemeManager::ThemePreset themePreset)
{
    switch (themePreset) {
    case AverraThemeManager::OceanTheme:
        return QColor(QStringLiteral("#245FD7"));
    case AverraThemeManager::ForestTheme:
        return QColor(QStringLiteral("#1F7C4B"));
    case AverraThemeManager::SunsetTheme:
        return QColor(QStringLiteral("#C46102"));
    case AverraThemeManager::OrchidTheme:
        return QColor(QStringLiteral("#6D28D9"));
    case AverraThemeManager::CustomTheme:
        return QColor(QStringLiteral("#245FD7"));
    }

    return QColor(QStringLiteral("#245FD7"));
}

QColor themeAccentDisabledColor(AverraThemeManager::ThemePreset themePreset)
{
    switch (themePreset) {
    case AverraThemeManager::OceanTheme:
        return QColor(QStringLiteral("#A8C2F5"));
    case AverraThemeManager::ForestTheme:
        return QColor(QStringLiteral("#A8D8BD"));
    case AverraThemeManager::SunsetTheme:
        return QColor(QStringLiteral("#F3C894"));
    case AverraThemeManager::OrchidTheme:
        return QColor(QStringLiteral("#CCB4FA"));
    case AverraThemeManager::CustomTheme:
        return QColor(QStringLiteral("#A8C2F5"));
    }

    return QColor(QStringLiteral("#A8C2F5"));
}

void applyDefaultNeutralColors(AverraThemePalette &palette)
{
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
    palette.setSuccessSurfaceColor(QColor(QStringLiteral("#ECFDF3")));
    palette.setWarningSurfaceColor(QColor(QStringLiteral("#FFF7ED")));
    palette.setErrorSurfaceColor(QColor(QStringLiteral("#FEF2F2")));
}
}

AverraThemeManager *AverraThemeManager::instance()
{
    static AverraThemeManager *manager = nullptr;

    if (manager == nullptr) {
        manager = new AverraThemeManager(qApp);
    }

    return manager;
}

AverraThemeManager::AverraThemeManager(QObject *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    m_themePreset = OceanTheme;
    m_styleProfile = AverraStyleProfile::createDefaultProfile();
    m_palette = createPalette(m_themePreset);
}

AverraThemePalette AverraThemeManager::createPalette(ThemePreset themePreset)
{
    AverraThemePalette palette = AverraThemePalette::createLightPalette();
    applyDefaultNeutralColors(palette);

    palette.setAccentColor(themeAccentColor(themePreset));
    palette.setAccentSurfaceColor(themeAccentSurfaceColor(themePreset));
    palette.setAccentHoverColor(themeAccentHoverColor(themePreset));
    palette.setAccentPressedColor(themeAccentPressedColor(themePreset));
    palette.setAccentDisabledColor(themeAccentDisabledColor(themePreset));
    return palette;
}

AverraThemePalette AverraThemeManager::palette() const
{
    return m_palette;
}

void AverraThemeManager::setPalette(const AverraThemePalette &palette)
{
    updatePalette(palette, true);
}

AverraThemeManager::ThemePreset AverraThemeManager::themePreset() const
{
    return m_themePreset;
}

void AverraThemeManager::setThemePreset(ThemePreset themePreset)
{
    if (m_themePreset == themePreset) {
        return;
    }

    m_themePreset = themePreset;
    m_palette = createPalette(m_themePreset);
    emit themePresetChanged(m_themePreset);
    emit paletteChanged();
}

AverraStyleProfile AverraThemeManager::styleProfile() const
{
    return m_styleProfile;
}

void AverraThemeManager::setStyleProfile(const AverraStyleProfile &styleProfile)
{
    if (m_styleProfile == styleProfile) {
        return;
    }

    m_styleProfile = styleProfile;
    emit styleProfileChanged(m_styleProfile);
    emit paletteChanged();
}

void AverraThemeManager::resetStyleProfile()
{
    setStyleProfile(AverraStyleProfile::createDefaultProfile());
}

void AverraThemeManager::applyTheme(ThemePreset themePreset)
{
    if (m_themePreset == themePreset) {
        return;
    }

    m_themePreset = themePreset;
    m_palette = createPalette(m_themePreset);
    emit themePresetChanged(m_themePreset);
    emit paletteChanged();
}

void AverraThemeManager::applyTheme(ThemePreset themePreset, const AverraStyleProfile &styleProfile)
{
    const bool themeChanged = m_themePreset != themePreset;
    const bool styleChanged = m_styleProfile != styleProfile;

    if (!themeChanged && !styleChanged) {
        return;
    }

    m_themePreset = themePreset;
    m_styleProfile = styleProfile;
    m_palette = createPalette(m_themePreset);

    if (themeChanged) {
        emit themePresetChanged(m_themePreset);
    }

    if (styleChanged) {
        emit styleProfileChanged(m_styleProfile);
    }

    emit paletteChanged();
}

bool AverraThemeManager::loadStyleProfile(const QString &filePath, QString *errorMessage)
{
    AverraStyleProfile profile = m_styleProfile;

    if (!profile.loadFromJsonFile(filePath, errorMessage)) {
        return false;
    }

    setStyleProfile(profile);
    return true;
}

bool AverraThemeManager::saveStyleProfile(const QString &filePath, QString *errorMessage) const
{
    return m_styleProfile.saveToJsonFile(filePath, errorMessage);
}

void AverraThemeManager::setAccentColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setSurfaceColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfaceColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setSurfaceRaisedColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfaceRaisedColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setSurfaceHoverColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfaceHoverColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setSurfacePressedColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfacePressedColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setSurfaceDisabledColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfaceDisabledColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setBorderColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setBorderColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setBorderHoverColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setBorderHoverColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setTextPrimaryColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setTextPrimaryColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setTextSecondaryColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setTextSecondaryColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setTextDisabledColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setTextDisabledColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setAccentSurfaceColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentSurfaceColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setAccentHoverColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentHoverColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setAccentPressedColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentPressedColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::setAccentDisabledColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentDisabledColor(color);
    updatePalette(palette, true);
}

void AverraThemeManager::updatePalette(const AverraThemePalette &palette, bool markAsCustomTheme)
{
    const bool paletteChangedValue = m_palette.accentColor() != palette.accentColor()
                                     || m_palette.surfaceColor() != palette.surfaceColor()
                                     || m_palette.surfaceRaisedColor() != palette.surfaceRaisedColor()
                                     || m_palette.surfaceHoverColor() != palette.surfaceHoverColor()
                                     || m_palette.surfacePressedColor() != palette.surfacePressedColor()
                                     || m_palette.surfaceDisabledColor() != palette.surfaceDisabledColor()
                                     || m_palette.borderColor() != palette.borderColor()
                                     || m_palette.borderHoverColor() != palette.borderHoverColor()
                                     || m_palette.textPrimaryColor() != palette.textPrimaryColor()
                                     || m_palette.textSecondaryColor() != palette.textSecondaryColor()
                                     || m_palette.textDisabledColor() != palette.textDisabledColor()
                                     || m_palette.accentSurfaceColor() != palette.accentSurfaceColor()
                                     || m_palette.accentHoverColor() != palette.accentHoverColor()
                                     || m_palette.accentPressedColor() != palette.accentPressedColor()
                                     || m_palette.accentDisabledColor() != palette.accentDisabledColor()
                                     || m_palette.successColor() != palette.successColor()
                                     || m_palette.successSurfaceColor() != palette.successSurfaceColor()
                                     || m_palette.warningColor() != palette.warningColor()
                                     || m_palette.warningSurfaceColor() != palette.warningSurfaceColor()
                                     || m_palette.errorColor() != palette.errorColor()
                                     || m_palette.errorSurfaceColor() != palette.errorSurfaceColor();

    if (!paletteChangedValue && !(markAsCustomTheme && m_themePreset != CustomTheme)) {
        return;
    }

    m_palette = palette;

    if (markAsCustomTheme && m_themePreset != CustomTheme) {
        m_themePreset = CustomTheme;
        emit themePresetChanged(m_themePreset);
    }

    emit paletteChanged();
}
