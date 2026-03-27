#ifndef AVERRA_THEME_MANAGER_H
#define AVERRA_THEME_MANAGER_H

#include <Averra/AverraGlobal.h>
#include <Averra/AverraTheme.h>

#include <QObject>

class AVERRA_EXPORT AverraThemeManager : public QObject
{
    Q_OBJECT

public:
    static AverraThemeManager *instance();

    AverraThemePalette palette() const;
    void setPalette(const AverraThemePalette &palette);

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

private:
    explicit AverraThemeManager(QObject *parent = nullptr);

    AverraThemePalette m_palette;
};

#endif // AVERRA_THEME_MANAGER_H
