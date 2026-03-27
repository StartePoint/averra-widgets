#include <Averra/AverraStyleProfile.h>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

namespace
{
QString windowControlsLayoutToString(AverraStyleProfile::WindowControlsLayout layout)
{
    if (layout == AverraStyleProfile::LeadingTrafficLights) {
        return QStringLiteral("leadingTrafficLights");
    }

    return QStringLiteral("trailingWindowControls");
}

AverraStyleProfile::WindowControlsLayout windowControlsLayoutFromVariant(const QVariant &value)
{
    const QString stringValue = value.toString().trimmed();

    if (stringValue.compare(QStringLiteral("leadingTrafficLights"), Qt::CaseInsensitive) == 0) {
        return AverraStyleProfile::LeadingTrafficLights;
    }

    if (stringValue.compare(QStringLiteral("trailingWindowControls"), Qt::CaseInsensitive) == 0) {
        return AverraStyleProfile::TrailingWindowControls;
    }

    return value.toInt() == static_cast<int>(AverraStyleProfile::LeadingTrafficLights)
           ? AverraStyleProfile::LeadingTrafficLights
           : AverraStyleProfile::TrailingWindowControls;
}

void applyIntValue(const QVariantMap &values, const QString &key, void (AverraStyleProfile::*setter)(int), AverraStyleProfile *profile)
{
    if (profile == nullptr || !values.contains(key)) {
        return;
    }

    bool ok = false;
    const int value = values.value(key).toInt(&ok);

    if (ok) {
        (profile->*setter)(value);
    }
}

void applyBoolValue(const QVariantMap &values, const QString &key, void (AverraStyleProfile::*setter)(bool), AverraStyleProfile *profile)
{
    if (profile == nullptr || !values.contains(key)) {
        return;
    }

    (profile->*setter)(values.value(key).toBool());
}
}

AverraStyleProfile::AverraStyleProfile()
{
    m_smallRadius = 8;
    m_controlRadius = 12;
    m_mediumRadius = 14;
    m_largeRadius = 16;
    m_cardRadius = 20;
    m_panelRadius = 22;
    m_pageRadius = 24;
    m_smallFontSize = 12;
    m_bodyFontSize = 13;
    m_titleFontSize = 16;
    m_pageTitleFontSize = 18;
    m_displayFontSize = 28;
    m_buttonPaddingVertical = 10;
    m_buttonPaddingHorizontal = 18;
    m_compactPaddingVertical = 7;
    m_compactPaddingHorizontal = 10;
    m_windowControlsLayout = TrailingWindowControls;
    m_titleTextCentered = false;
    m_enableTitleBarDrag = true;
    m_enableTitleBarDoubleClickZoom = true;
    m_windowControlButtonWidth = 38;
    m_windowControlButtonHeight = 28;
    m_trafficLightDiameter = 12;
    m_dialogShowDuration = 220;
    m_dialogHideDuration = 160;
    m_panelShowDuration = 220;
    m_panelHideDuration = 190;
    m_dialogShowOffset = 18;
    m_dialogHideOffset = 12;
    m_panelSlideDistance = 28;
}

AverraStyleProfile AverraStyleProfile::createDefaultProfile()
{
    return AverraStyleProfile();
}

int AverraStyleProfile::smallRadius() const { return m_smallRadius; }
void AverraStyleProfile::setSmallRadius(int value) { m_smallRadius = value; }
int AverraStyleProfile::controlRadius() const { return m_controlRadius; }
void AverraStyleProfile::setControlRadius(int value) { m_controlRadius = value; }
int AverraStyleProfile::mediumRadius() const { return m_mediumRadius; }
void AverraStyleProfile::setMediumRadius(int value) { m_mediumRadius = value; }
int AverraStyleProfile::largeRadius() const { return m_largeRadius; }
void AverraStyleProfile::setLargeRadius(int value) { m_largeRadius = value; }
int AverraStyleProfile::cardRadius() const { return m_cardRadius; }
void AverraStyleProfile::setCardRadius(int value) { m_cardRadius = value; }
int AverraStyleProfile::panelRadius() const { return m_panelRadius; }
void AverraStyleProfile::setPanelRadius(int value) { m_panelRadius = value; }
int AverraStyleProfile::pageRadius() const { return m_pageRadius; }
void AverraStyleProfile::setPageRadius(int value) { m_pageRadius = value; }
int AverraStyleProfile::smallFontSize() const { return m_smallFontSize; }
void AverraStyleProfile::setSmallFontSize(int value) { m_smallFontSize = value; }
int AverraStyleProfile::bodyFontSize() const { return m_bodyFontSize; }
void AverraStyleProfile::setBodyFontSize(int value) { m_bodyFontSize = value; }
int AverraStyleProfile::titleFontSize() const { return m_titleFontSize; }
void AverraStyleProfile::setTitleFontSize(int value) { m_titleFontSize = value; }
int AverraStyleProfile::pageTitleFontSize() const { return m_pageTitleFontSize; }
void AverraStyleProfile::setPageTitleFontSize(int value) { m_pageTitleFontSize = value; }
int AverraStyleProfile::displayFontSize() const { return m_displayFontSize; }
void AverraStyleProfile::setDisplayFontSize(int value) { m_displayFontSize = value; }
int AverraStyleProfile::buttonPaddingVertical() const { return m_buttonPaddingVertical; }
void AverraStyleProfile::setButtonPaddingVertical(int value) { m_buttonPaddingVertical = value; }
int AverraStyleProfile::buttonPaddingHorizontal() const { return m_buttonPaddingHorizontal; }
void AverraStyleProfile::setButtonPaddingHorizontal(int value) { m_buttonPaddingHorizontal = value; }
int AverraStyleProfile::compactPaddingVertical() const { return m_compactPaddingVertical; }
void AverraStyleProfile::setCompactPaddingVertical(int value) { m_compactPaddingVertical = value; }
int AverraStyleProfile::compactPaddingHorizontal() const { return m_compactPaddingHorizontal; }
void AverraStyleProfile::setCompactPaddingHorizontal(int value) { m_compactPaddingHorizontal = value; }
AverraStyleProfile::WindowControlsLayout AverraStyleProfile::windowControlsLayout() const { return m_windowControlsLayout; }
void AverraStyleProfile::setWindowControlsLayout(WindowControlsLayout value) { m_windowControlsLayout = value; }
bool AverraStyleProfile::titleTextCentered() const { return m_titleTextCentered; }
void AverraStyleProfile::setTitleTextCentered(bool value) { m_titleTextCentered = value; }
bool AverraStyleProfile::enableTitleBarDrag() const { return m_enableTitleBarDrag; }
void AverraStyleProfile::setEnableTitleBarDrag(bool value) { m_enableTitleBarDrag = value; }
bool AverraStyleProfile::enableTitleBarDoubleClickZoom() const { return m_enableTitleBarDoubleClickZoom; }
void AverraStyleProfile::setEnableTitleBarDoubleClickZoom(bool value) { m_enableTitleBarDoubleClickZoom = value; }
int AverraStyleProfile::windowControlButtonWidth() const { return m_windowControlButtonWidth; }
void AverraStyleProfile::setWindowControlButtonWidth(int value) { m_windowControlButtonWidth = value; }
int AverraStyleProfile::windowControlButtonHeight() const { return m_windowControlButtonHeight; }
void AverraStyleProfile::setWindowControlButtonHeight(int value) { m_windowControlButtonHeight = value; }
int AverraStyleProfile::trafficLightDiameter() const { return m_trafficLightDiameter; }
void AverraStyleProfile::setTrafficLightDiameter(int value) { m_trafficLightDiameter = value; }
int AverraStyleProfile::dialogShowDuration() const { return m_dialogShowDuration; }
void AverraStyleProfile::setDialogShowDuration(int value) { m_dialogShowDuration = value; }
int AverraStyleProfile::dialogHideDuration() const { return m_dialogHideDuration; }
void AverraStyleProfile::setDialogHideDuration(int value) { m_dialogHideDuration = value; }
int AverraStyleProfile::panelShowDuration() const { return m_panelShowDuration; }
void AverraStyleProfile::setPanelShowDuration(int value) { m_panelShowDuration = value; }
int AverraStyleProfile::panelHideDuration() const { return m_panelHideDuration; }
void AverraStyleProfile::setPanelHideDuration(int value) { m_panelHideDuration = value; }
int AverraStyleProfile::dialogShowOffset() const { return m_dialogShowOffset; }
void AverraStyleProfile::setDialogShowOffset(int value) { m_dialogShowOffset = value; }
int AverraStyleProfile::dialogHideOffset() const { return m_dialogHideOffset; }
void AverraStyleProfile::setDialogHideOffset(int value) { m_dialogHideOffset = value; }
int AverraStyleProfile::panelSlideDistance() const { return m_panelSlideDistance; }
void AverraStyleProfile::setPanelSlideDistance(int value) { m_panelSlideDistance = value; }

QVariantMap AverraStyleProfile::toVariantMap() const
{
    QVariantMap values;

    values.insert(QStringLiteral("smallRadius"), m_smallRadius);
    values.insert(QStringLiteral("controlRadius"), m_controlRadius);
    values.insert(QStringLiteral("mediumRadius"), m_mediumRadius);
    values.insert(QStringLiteral("largeRadius"), m_largeRadius);
    values.insert(QStringLiteral("cardRadius"), m_cardRadius);
    values.insert(QStringLiteral("panelRadius"), m_panelRadius);
    values.insert(QStringLiteral("pageRadius"), m_pageRadius);
    values.insert(QStringLiteral("smallFontSize"), m_smallFontSize);
    values.insert(QStringLiteral("bodyFontSize"), m_bodyFontSize);
    values.insert(QStringLiteral("titleFontSize"), m_titleFontSize);
    values.insert(QStringLiteral("pageTitleFontSize"), m_pageTitleFontSize);
    values.insert(QStringLiteral("displayFontSize"), m_displayFontSize);
    values.insert(QStringLiteral("buttonPaddingVertical"), m_buttonPaddingVertical);
    values.insert(QStringLiteral("buttonPaddingHorizontal"), m_buttonPaddingHorizontal);
    values.insert(QStringLiteral("compactPaddingVertical"), m_compactPaddingVertical);
    values.insert(QStringLiteral("compactPaddingHorizontal"), m_compactPaddingHorizontal);
    values.insert(QStringLiteral("windowControlsLayout"), windowControlsLayoutToString(m_windowControlsLayout));
    values.insert(QStringLiteral("titleTextCentered"), m_titleTextCentered);
    values.insert(QStringLiteral("enableTitleBarDrag"), m_enableTitleBarDrag);
    values.insert(QStringLiteral("enableTitleBarDoubleClickZoom"), m_enableTitleBarDoubleClickZoom);
    values.insert(QStringLiteral("windowControlButtonWidth"), m_windowControlButtonWidth);
    values.insert(QStringLiteral("windowControlButtonHeight"), m_windowControlButtonHeight);
    values.insert(QStringLiteral("trafficLightDiameter"), m_trafficLightDiameter);
    values.insert(QStringLiteral("dialogShowDuration"), m_dialogShowDuration);
    values.insert(QStringLiteral("dialogHideDuration"), m_dialogHideDuration);
    values.insert(QStringLiteral("panelShowDuration"), m_panelShowDuration);
    values.insert(QStringLiteral("panelHideDuration"), m_panelHideDuration);
    values.insert(QStringLiteral("dialogShowOffset"), m_dialogShowOffset);
    values.insert(QStringLiteral("dialogHideOffset"), m_dialogHideOffset);
    values.insert(QStringLiteral("panelSlideDistance"), m_panelSlideDistance);
    return values;
}

void AverraStyleProfile::applyVariantMap(const QVariantMap &values)
{
    applyIntValue(values, QStringLiteral("smallRadius"), &AverraStyleProfile::setSmallRadius, this);
    applyIntValue(values, QStringLiteral("controlRadius"), &AverraStyleProfile::setControlRadius, this);
    applyIntValue(values, QStringLiteral("mediumRadius"), &AverraStyleProfile::setMediumRadius, this);
    applyIntValue(values, QStringLiteral("largeRadius"), &AverraStyleProfile::setLargeRadius, this);
    applyIntValue(values, QStringLiteral("cardRadius"), &AverraStyleProfile::setCardRadius, this);
    applyIntValue(values, QStringLiteral("panelRadius"), &AverraStyleProfile::setPanelRadius, this);
    applyIntValue(values, QStringLiteral("pageRadius"), &AverraStyleProfile::setPageRadius, this);
    applyIntValue(values, QStringLiteral("smallFontSize"), &AverraStyleProfile::setSmallFontSize, this);
    applyIntValue(values, QStringLiteral("bodyFontSize"), &AverraStyleProfile::setBodyFontSize, this);
    applyIntValue(values, QStringLiteral("titleFontSize"), &AverraStyleProfile::setTitleFontSize, this);
    applyIntValue(values, QStringLiteral("pageTitleFontSize"), &AverraStyleProfile::setPageTitleFontSize, this);
    applyIntValue(values, QStringLiteral("displayFontSize"), &AverraStyleProfile::setDisplayFontSize, this);
    applyIntValue(values, QStringLiteral("buttonPaddingVertical"), &AverraStyleProfile::setButtonPaddingVertical, this);
    applyIntValue(values, QStringLiteral("buttonPaddingHorizontal"), &AverraStyleProfile::setButtonPaddingHorizontal, this);
    applyIntValue(values, QStringLiteral("compactPaddingVertical"), &AverraStyleProfile::setCompactPaddingVertical, this);
    applyIntValue(values, QStringLiteral("compactPaddingHorizontal"), &AverraStyleProfile::setCompactPaddingHorizontal, this);
    applyBoolValue(values, QStringLiteral("titleTextCentered"), &AverraStyleProfile::setTitleTextCentered, this);
    applyBoolValue(values, QStringLiteral("enableTitleBarDrag"), &AverraStyleProfile::setEnableTitleBarDrag, this);
    applyBoolValue(values,
                   QStringLiteral("enableTitleBarDoubleClickZoom"),
                   &AverraStyleProfile::setEnableTitleBarDoubleClickZoom,
                   this);
    applyIntValue(values,
                  QStringLiteral("windowControlButtonWidth"),
                  &AverraStyleProfile::setWindowControlButtonWidth,
                  this);
    applyIntValue(values,
                  QStringLiteral("windowControlButtonHeight"),
                  &AverraStyleProfile::setWindowControlButtonHeight,
                  this);
    applyIntValue(values, QStringLiteral("trafficLightDiameter"), &AverraStyleProfile::setTrafficLightDiameter, this);
    applyIntValue(values, QStringLiteral("dialogShowDuration"), &AverraStyleProfile::setDialogShowDuration, this);
    applyIntValue(values, QStringLiteral("dialogHideDuration"), &AverraStyleProfile::setDialogHideDuration, this);
    applyIntValue(values, QStringLiteral("panelShowDuration"), &AverraStyleProfile::setPanelShowDuration, this);
    applyIntValue(values, QStringLiteral("panelHideDuration"), &AverraStyleProfile::setPanelHideDuration, this);
    applyIntValue(values, QStringLiteral("dialogShowOffset"), &AverraStyleProfile::setDialogShowOffset, this);
    applyIntValue(values, QStringLiteral("dialogHideOffset"), &AverraStyleProfile::setDialogHideOffset, this);
    applyIntValue(values, QStringLiteral("panelSlideDistance"), &AverraStyleProfile::setPanelSlideDistance, this);

    if (values.contains(QStringLiteral("windowControlsLayout"))) {
        setWindowControlsLayout(windowControlsLayoutFromVariant(values.value(QStringLiteral("windowControlsLayout"))));
    }
}

bool AverraStyleProfile::loadFromJsonFile(const QString &filePath, QString *errorMessage)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        if (errorMessage != nullptr) {
            *errorMessage = file.errorString();
        }
        return false;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();

    if (parseError.error != QJsonParseError::NoError) {
        if (errorMessage != nullptr) {
            *errorMessage = parseError.errorString();
        }
        return false;
    }

    if (!document.isObject()) {
        if (errorMessage != nullptr) {
            *errorMessage = QStringLiteral("样式配置文件必须是一个 JSON 对象。");
        }
        return false;
    }

    applyVariantMap(document.object().toVariantMap());
    return true;
}

bool AverraStyleProfile::saveToJsonFile(const QString &filePath, QString *errorMessage) const
{
    QSaveFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        if (errorMessage != nullptr) {
            *errorMessage = file.errorString();
        }
        return false;
    }

    const QJsonDocument document(QJsonObject::fromVariantMap(toVariantMap()));

    if (file.write(document.toJson(QJsonDocument::Indented)) < 0) {
        if (errorMessage != nullptr) {
            *errorMessage = file.errorString();
        }
        file.cancelWriting();
        return false;
    }

    if (!file.commit()) {
        if (errorMessage != nullptr) {
            *errorMessage = file.errorString();
        }
        return false;
    }

    return true;
}

bool operator==(const AverraStyleProfile &left, const AverraStyleProfile &right)
{
    return left.smallRadius() == right.smallRadius()
           && left.controlRadius() == right.controlRadius()
           && left.mediumRadius() == right.mediumRadius()
           && left.largeRadius() == right.largeRadius()
           && left.cardRadius() == right.cardRadius()
           && left.panelRadius() == right.panelRadius()
           && left.pageRadius() == right.pageRadius()
           && left.smallFontSize() == right.smallFontSize()
           && left.bodyFontSize() == right.bodyFontSize()
           && left.titleFontSize() == right.titleFontSize()
           && left.pageTitleFontSize() == right.pageTitleFontSize()
           && left.displayFontSize() == right.displayFontSize()
           && left.buttonPaddingVertical() == right.buttonPaddingVertical()
           && left.buttonPaddingHorizontal() == right.buttonPaddingHorizontal()
           && left.compactPaddingVertical() == right.compactPaddingVertical()
           && left.compactPaddingHorizontal() == right.compactPaddingHorizontal()
           && left.windowControlsLayout() == right.windowControlsLayout()
           && left.titleTextCentered() == right.titleTextCentered()
           && left.enableTitleBarDrag() == right.enableTitleBarDrag()
           && left.enableTitleBarDoubleClickZoom() == right.enableTitleBarDoubleClickZoom()
           && left.windowControlButtonWidth() == right.windowControlButtonWidth()
           && left.windowControlButtonHeight() == right.windowControlButtonHeight()
           && left.trafficLightDiameter() == right.trafficLightDiameter()
           && left.dialogShowDuration() == right.dialogShowDuration()
           && left.dialogHideDuration() == right.dialogHideDuration()
           && left.panelShowDuration() == right.panelShowDuration()
           && left.panelHideDuration() == right.panelHideDuration()
           && left.dialogShowOffset() == right.dialogShowOffset()
           && left.dialogHideOffset() == right.dialogHideOffset()
           && left.panelSlideDistance() == right.panelSlideDistance();
}

bool operator!=(const AverraStyleProfile &left, const AverraStyleProfile &right)
{
    return !(left == right);
}
