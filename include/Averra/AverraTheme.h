#ifndef AVERRA_THEME_H
#define AVERRA_THEME_H

#include <Averra/AverraGlobal.h>

#include <QColor>

class AVERRA_EXPORT AverraThemePalette
{
public:
    AverraThemePalette();

    static AverraThemePalette createLightPalette();

    QColor accentColor() const;
    void setAccentColor(const QColor &color);

    QColor surfaceColor() const;
    void setSurfaceColor(const QColor &color);

    QColor surfaceRaisedColor() const;
    void setSurfaceRaisedColor(const QColor &color);

    QColor surfaceHoverColor() const;
    void setSurfaceHoverColor(const QColor &color);

    QColor surfacePressedColor() const;
    void setSurfacePressedColor(const QColor &color);

    QColor surfaceDisabledColor() const;
    void setSurfaceDisabledColor(const QColor &color);

    QColor borderColor() const;
    void setBorderColor(const QColor &color);

    QColor borderHoverColor() const;
    void setBorderHoverColor(const QColor &color);

    QColor textPrimaryColor() const;
    void setTextPrimaryColor(const QColor &color);

    QColor textSecondaryColor() const;
    void setTextSecondaryColor(const QColor &color);

    QColor textDisabledColor() const;
    void setTextDisabledColor(const QColor &color);

    QColor accentSurfaceColor() const;
    void setAccentSurfaceColor(const QColor &color);

    QColor accentHoverColor() const;
    void setAccentHoverColor(const QColor &color);

    QColor accentPressedColor() const;
    void setAccentPressedColor(const QColor &color);

    QColor accentDisabledColor() const;
    void setAccentDisabledColor(const QColor &color);

    QColor successColor() const;
    void setSuccessColor(const QColor &color);

    QColor successSurfaceColor() const;
    void setSuccessSurfaceColor(const QColor &color);

    QColor warningColor() const;
    void setWarningColor(const QColor &color);

    QColor warningSurfaceColor() const;
    void setWarningSurfaceColor(const QColor &color);

    QColor errorColor() const;
    void setErrorColor(const QColor &color);

    QColor errorSurfaceColor() const;
    void setErrorSurfaceColor(const QColor &color);

private:
    QColor m_accentColor;
    QColor m_surfaceColor;
    QColor m_surfaceRaisedColor;
    QColor m_surfaceHoverColor;
    QColor m_surfacePressedColor;
    QColor m_surfaceDisabledColor;
    QColor m_borderColor;
    QColor m_borderHoverColor;
    QColor m_textPrimaryColor;
    QColor m_textSecondaryColor;
    QColor m_textDisabledColor;
    QColor m_accentSurfaceColor;
    QColor m_accentHoverColor;
    QColor m_accentPressedColor;
    QColor m_accentDisabledColor;
    QColor m_successColor;
    QColor m_successSurfaceColor;
    QColor m_warningColor;
    QColor m_warningSurfaceColor;
    QColor m_errorColor;
    QColor m_errorSurfaceColor;
};

#endif // AVERRA_THEME_H
