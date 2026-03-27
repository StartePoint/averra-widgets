#include <Averra/AverraThemeManager.h>

#include <QApplication>

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

    m_palette = AverraThemePalette::createLightPalette();
}

AverraThemePalette AverraThemeManager::palette() const
{
    return m_palette;
}

void AverraThemeManager::setPalette(const AverraThemePalette &palette)
{
    m_palette = palette;
    emit paletteChanged();
}

void AverraThemeManager::setAccentColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentColor(color);
    setPalette(palette);
}

void AverraThemeManager::setSurfaceColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfaceColor(color);
    setPalette(palette);
}

void AverraThemeManager::setSurfaceRaisedColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfaceRaisedColor(color);
    setPalette(palette);
}

void AverraThemeManager::setSurfaceHoverColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfaceHoverColor(color);
    setPalette(palette);
}

void AverraThemeManager::setSurfacePressedColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfacePressedColor(color);
    setPalette(palette);
}

void AverraThemeManager::setSurfaceDisabledColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setSurfaceDisabledColor(color);
    setPalette(palette);
}

void AverraThemeManager::setBorderColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setBorderColor(color);
    setPalette(palette);
}

void AverraThemeManager::setBorderHoverColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setBorderHoverColor(color);
    setPalette(palette);
}

void AverraThemeManager::setTextPrimaryColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setTextPrimaryColor(color);
    setPalette(palette);
}

void AverraThemeManager::setTextSecondaryColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setTextSecondaryColor(color);
    setPalette(palette);
}

void AverraThemeManager::setTextDisabledColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setTextDisabledColor(color);
    setPalette(palette);
}

void AverraThemeManager::setAccentSurfaceColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentSurfaceColor(color);
    setPalette(palette);
}

void AverraThemeManager::setAccentHoverColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentHoverColor(color);
    setPalette(palette);
}

void AverraThemeManager::setAccentPressedColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentPressedColor(color);
    setPalette(palette);
}

void AverraThemeManager::setAccentDisabledColor(const QColor &color)
{
    AverraThemePalette palette = m_palette;
    palette.setAccentDisabledColor(color);
    setPalette(palette);
}
