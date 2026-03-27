#ifndef AVERRA_THEME_MANAGER_H
#define AVERRA_THEME_MANAGER_H

#include <Averra/AverraGlobal.h>
#include <Averra/AverraStyleProfile.h>
#include <Averra/AverraTheme.h>

#include <QObject>
#include <QString>

class AVERRA_EXPORT AverraThemeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ThemePreset themePreset READ themePreset WRITE setThemePreset NOTIFY themePresetChanged)

public:
    enum ThemePreset
    {
        OceanTheme,
        ForestTheme,
        SunsetTheme,
        OrchidTheme,
        CustomTheme
    };
    Q_ENUM(ThemePreset)

    static AverraThemeManager *instance();
    static AverraThemePalette createPalette(ThemePreset themePreset);

    AverraThemePalette palette() const;
    void setPalette(const AverraThemePalette &palette);
    ThemePreset themePreset() const;
    void setThemePreset(ThemePreset themePreset);
    AverraStyleProfile styleProfile() const;
    void setStyleProfile(const AverraStyleProfile &styleProfile);
    void resetStyleProfile();
    void applyTheme(ThemePreset themePreset);
    void applyTheme(ThemePreset themePreset, const AverraStyleProfile &styleProfile);
    bool loadStyleProfile(const QString &filePath, QString *errorMessage = nullptr);
    bool saveStyleProfile(const QString &filePath, QString *errorMessage = nullptr) const;

    void setAccentColor(const QColor &color);
    void setSurfaceColor(const QColor &color);
    void setSurfaceRaisedColor(const QColor &color);
    void setSurfaceHoverColor(const QColor &color);
    void setSurfacePressedColor(const QColor &color);
    void setSurfaceDisabledColor(const QColor &color);
    void setBorderColor(const QColor &color);
    void setBorderHoverColor(const QColor &color);
    void setTextPrimaryColor(const QColor &color);
    void setTextSecondaryColor(const QColor &color);
    void setTextDisabledColor(const QColor &color);
    void setAccentSurfaceColor(const QColor &color);
    void setAccentHoverColor(const QColor &color);
    void setAccentPressedColor(const QColor &color);
    void setAccentDisabledColor(const QColor &color);

signals:
    void paletteChanged();
    void themePresetChanged(ThemePreset themePreset);
    void styleProfileChanged(const AverraStyleProfile &styleProfile);

private:
    explicit AverraThemeManager(QObject *parent = nullptr);
    void updatePalette(const AverraThemePalette &palette, bool markAsCustomTheme);

    AverraThemePalette m_palette;
    ThemePreset m_themePreset;
    AverraStyleProfile m_styleProfile;
};

#endif // AVERRA_THEME_MANAGER_H
