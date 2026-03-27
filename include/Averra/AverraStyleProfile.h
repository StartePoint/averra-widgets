#ifndef AVERRA_STYLE_PROFILE_H
#define AVERRA_STYLE_PROFILE_H

#include <Averra/AverraGlobal.h>

#include <QObject>
#include <QString>
#include <QVariantMap>

class AVERRA_EXPORT AverraStyleProfile
{
    Q_GADGET

public:
    enum WindowControlsLayout
    {
        LeadingTrafficLights,
        TrailingWindowControls
    };
    Q_ENUM(WindowControlsLayout)

    AverraStyleProfile();

    static AverraStyleProfile createDefaultProfile();

    int smallRadius() const;
    void setSmallRadius(int value);

    int controlRadius() const;
    void setControlRadius(int value);

    int mediumRadius() const;
    void setMediumRadius(int value);

    int largeRadius() const;
    void setLargeRadius(int value);

    int cardRadius() const;
    void setCardRadius(int value);

    int panelRadius() const;
    void setPanelRadius(int value);

    int pageRadius() const;
    void setPageRadius(int value);

    int smallFontSize() const;
    void setSmallFontSize(int value);

    int bodyFontSize() const;
    void setBodyFontSize(int value);

    int titleFontSize() const;
    void setTitleFontSize(int value);

    int pageTitleFontSize() const;
    void setPageTitleFontSize(int value);

    int displayFontSize() const;
    void setDisplayFontSize(int value);

    int buttonPaddingVertical() const;
    void setButtonPaddingVertical(int value);

    int buttonPaddingHorizontal() const;
    void setButtonPaddingHorizontal(int value);

    int compactPaddingVertical() const;
    void setCompactPaddingVertical(int value);

    int compactPaddingHorizontal() const;
    void setCompactPaddingHorizontal(int value);

    WindowControlsLayout windowControlsLayout() const;
    void setWindowControlsLayout(WindowControlsLayout value);

    bool titleTextCentered() const;
    void setTitleTextCentered(bool value);

    bool enableTitleBarDrag() const;
    void setEnableTitleBarDrag(bool value);

    bool enableTitleBarDoubleClickZoom() const;
    void setEnableTitleBarDoubleClickZoom(bool value);

    int windowControlButtonWidth() const;
    void setWindowControlButtonWidth(int value);

    int windowControlButtonHeight() const;
    void setWindowControlButtonHeight(int value);

    int trafficLightDiameter() const;
    void setTrafficLightDiameter(int value);

    int dialogShowDuration() const;
    void setDialogShowDuration(int value);

    int dialogHideDuration() const;
    void setDialogHideDuration(int value);

    int panelShowDuration() const;
    void setPanelShowDuration(int value);

    int panelHideDuration() const;
    void setPanelHideDuration(int value);

    int dialogShowOffset() const;
    void setDialogShowOffset(int value);

    int dialogHideOffset() const;
    void setDialogHideOffset(int value);

    int panelSlideDistance() const;
    void setPanelSlideDistance(int value);

    QVariantMap toVariantMap() const;
    void applyVariantMap(const QVariantMap &values);
    bool loadFromJsonFile(const QString &filePath, QString *errorMessage = nullptr);
    bool saveToJsonFile(const QString &filePath, QString *errorMessage = nullptr) const;

private:
    int m_smallRadius;
    int m_controlRadius;
    int m_mediumRadius;
    int m_largeRadius;
    int m_cardRadius;
    int m_panelRadius;
    int m_pageRadius;
    int m_smallFontSize;
    int m_bodyFontSize;
    int m_titleFontSize;
    int m_pageTitleFontSize;
    int m_displayFontSize;
    int m_buttonPaddingVertical;
    int m_buttonPaddingHorizontal;
    int m_compactPaddingVertical;
    int m_compactPaddingHorizontal;
    WindowControlsLayout m_windowControlsLayout;
    bool m_titleTextCentered;
    bool m_enableTitleBarDrag;
    bool m_enableTitleBarDoubleClickZoom;
    int m_windowControlButtonWidth;
    int m_windowControlButtonHeight;
    int m_trafficLightDiameter;
    int m_dialogShowDuration;
    int m_dialogHideDuration;
    int m_panelShowDuration;
    int m_panelHideDuration;
    int m_dialogShowOffset;
    int m_dialogHideOffset;
    int m_panelSlideDistance;
};

AVERRA_EXPORT bool operator==(const AverraStyleProfile &left, const AverraStyleProfile &right);
AVERRA_EXPORT bool operator!=(const AverraStyleProfile &left, const AverraStyleProfile &right);

Q_DECLARE_METATYPE(AverraStyleProfile)

#endif // AVERRA_STYLE_PROFILE_H
