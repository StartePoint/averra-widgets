#include "AverraStyleHelper.h"

#include <Averra/AverraThemeManager.h>

#include <QString>
#include <QtGlobal>

namespace
{
struct StyleMetrics
{
    int smallRadius;
    int controlRadius;
    int mediumRadius;
    int largeRadius;
    int cardRadius;
    int panelRadius;
    int pageRadius;
    int smallFontSize;
    int bodyFontSize;
    int titleFontSize;
    int pageTitleFontSize;
    int displayFontSize;
    int buttonPaddingVertical;
    int buttonPaddingHorizontal;
    int compactPaddingVertical;
    int compactPaddingHorizontal;
};

StyleMetrics styleMetrics()
{
    StyleMetrics metrics;
    AverraStyleProfile profile = AverraStyleProfile::createDefaultProfile();
    AverraThemeManager *manager = AverraThemeManager::instance();

    if (manager != nullptr) {
        profile = manager->styleProfile();
    }

    metrics.smallRadius = profile.smallRadius();
    metrics.controlRadius = profile.controlRadius();
    metrics.mediumRadius = profile.mediumRadius();
    metrics.largeRadius = profile.largeRadius();
    metrics.cardRadius = profile.cardRadius();
    metrics.panelRadius = profile.panelRadius();
    metrics.pageRadius = profile.pageRadius();
    metrics.smallFontSize = profile.smallFontSize();
    metrics.bodyFontSize = profile.bodyFontSize();
    metrics.titleFontSize = profile.titleFontSize();
    metrics.pageTitleFontSize = profile.pageTitleFontSize();
    metrics.displayFontSize = profile.displayFontSize();
    metrics.buttonPaddingVertical = profile.buttonPaddingVertical();
    metrics.buttonPaddingHorizontal = profile.buttonPaddingHorizontal();
    metrics.compactPaddingVertical = profile.compactPaddingVertical();
    metrics.compactPaddingHorizontal = profile.compactPaddingHorizontal();
    return metrics;
}

QString colorToCss(const QColor &color)
{
    return color.name();
}

QString numberCss(int value)
{
    return QString::number(value);
}

int regularControlHeight(const StyleMetrics &metrics)
{
    return qMax(metrics.bodyFontSize + (metrics.buttonPaddingVertical * 2) + 7,
                (metrics.controlRadius * 2) + 14);
}

int capsuleRadius(const StyleMetrics &metrics)
{
    return qMax(metrics.controlRadius,
                metrics.buttonPaddingVertical + metrics.compactPaddingHorizontal);
}

int compactInset(const StyleMetrics &metrics)
{
    return qMax(2, metrics.compactPaddingVertical - 3);
}

int compactInsetHorizontal(const StyleMetrics &metrics)
{
    return qMax(4, metrics.compactPaddingHorizontal - 2);
}

int searchInsetHorizontal(const StyleMetrics &metrics)
{
    return qMax(4, metrics.compactPaddingHorizontal - 6);
}

int comboDropDownWidth(const StyleMetrics &metrics)
{
    return qMax(24, metrics.buttonPaddingHorizontal + 10);
}

int dateDropDownWidth(const StyleMetrics &metrics)
{
    return qMax(24, metrics.buttonPaddingHorizontal + 8);
}

int spinButtonWidth(const StyleMetrics &metrics)
{
    return qMax(16, metrics.compactPaddingHorizontal + 8);
}

int regularArrowIconSize(const StyleMetrics &metrics)
{
    return qMax(10, metrics.smallFontSize);
}

int smallArrowIconSize(const StyleMetrics &metrics)
{
    return qMax(8, metrics.smallFontSize - 4);
}

int paginationMinWidth(const StyleMetrics &metrics)
{
    return qMax(28, regularControlHeight(metrics) - 6);
}

int menuItemHorizontalPadding(const StyleMetrics &metrics)
{
    return qMax(14, metrics.buttonPaddingHorizontal + 2);
}

int tableHeaderPadding(const StyleMetrics &metrics)
{
    return qMax(6, metrics.compactPaddingVertical + 1);
}

int badgeRightPadding(const StyleMetrics &metrics)
{
    return metrics.buttonPaddingHorizontal + 26;
}

int comboTrailingPadding(const StyleMetrics &metrics)
{
    return comboDropDownWidth(metrics) + 6;
}

int dateTrailingPadding(const StyleMetrics &metrics)
{
    return dateDropDownWidth(metrics) + 4;
}

int spinTrailingPadding(const StyleMetrics &metrics)
{
    return spinButtonWidth(metrics) + 10;
}

QPair<QColor, QColor> infoBarColors(const AverraThemePalette &palette, AverraStyleHelper::InfoBarType type)
{
    switch (type) {
    case AverraStyleHelper::InfoBarType::Info:
        return qMakePair(palette.accentColor(), palette.accentSurfaceColor());
    case AverraStyleHelper::InfoBarType::Success:
        return qMakePair(palette.successColor(), palette.successSurfaceColor());
    case AverraStyleHelper::InfoBarType::Warning:
        return qMakePair(palette.warningColor(), palette.warningSurfaceColor());
    case AverraStyleHelper::InfoBarType::Error:
        return qMakePair(palette.errorColor(), palette.errorSurfaceColor());
    }

    return qMakePair(palette.accentColor(), palette.accentSurfaceColor());
}

QPair<QColor, QColor> tagColors(const AverraThemePalette &palette, AverraStyleHelper::TagTone tone)
{
    switch (tone) {
    case AverraStyleHelper::TagTone::Neutral:
        return qMakePair(palette.textSecondaryColor(), palette.surfaceColor());
    case AverraStyleHelper::TagTone::Accent:
        return qMakePair(palette.accentColor(), palette.accentSurfaceColor());
    case AverraStyleHelper::TagTone::Success:
        return qMakePair(palette.successColor(), palette.successSurfaceColor());
    case AverraStyleHelper::TagTone::Warning:
        return qMakePair(palette.warningColor(), palette.warningSurfaceColor());
    case AverraStyleHelper::TagTone::Error:
        return qMakePair(palette.errorColor(), palette.errorSurfaceColor());
    }

    return qMakePair(palette.textSecondaryColor(), palette.surfaceColor());
}
}

QString AverraStyleHelper::buttonStyleSheet(const AverraThemePalette &palette, bool accent)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString borderColor;
    QString textColor;
    QString hoverBackgroundColor;
    QString pressedBackgroundColor;
    QString hoverBorderColor;
    QString disabledBackgroundColor;
    QString disabledBorderColor;
    QString disabledTextColor;

    if (accent) {
        backgroundColor = colorToCss(palette.accentColor());
        borderColor = colorToCss(palette.accentColor());
        textColor = QStringLiteral("#FFFFFF");
        hoverBackgroundColor = colorToCss(palette.accentHoverColor());
        pressedBackgroundColor = colorToCss(palette.accentPressedColor());
        hoverBorderColor = colorToCss(palette.accentHoverColor());
        disabledBackgroundColor = colorToCss(palette.accentDisabledColor());
        disabledBorderColor = colorToCss(palette.accentDisabledColor());
        disabledTextColor = colorToCss(palette.surfaceRaisedColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = colorToCss(palette.borderColor());
        textColor = colorToCss(palette.textPrimaryColor());
        hoverBackgroundColor = colorToCss(palette.surfaceHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
        hoverBorderColor = colorToCss(palette.borderHoverColor());
        disabledBackgroundColor = colorToCss(palette.surfaceDisabledColor());
        disabledBorderColor = colorToCss(palette.surfaceDisabledColor());
        disabledTextColor = colorToCss(palette.textDisabledColor());
    }

    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %10px;"
               "padding: %11px %12px;"
               "font-size: %13px;"
               "font-weight: 600;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QPushButton:pressed {"
               "background-color: %6;"
               "border-color: %4;"
               "padding-top: %14px;"
               "padding-bottom: %15px;"
               "}"
               "QPushButton:disabled {"
               "background-color: %7;"
               "color: %8;"
               "border-color: %9;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             hoverBorderColor,
             pressedBackgroundColor,
             disabledBackgroundColor,
             disabledTextColor,
             disabledBorderColor,
             numberCss(metrics.controlRadius),
             numberCss(metrics.buttonPaddingVertical),
             numberCss(metrics.buttonPaddingHorizontal),
             numberCss(metrics.titleFontSize - 2),
             numberCss(metrics.buttonPaddingVertical + 1),
             numberCss(metrics.buttonPaddingVertical - 1));
}

QString AverraStyleHelper::badgeButtonStyleSheet(const AverraThemePalette &palette, bool accent)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString borderColor;
    QString textColor;
    QString hoverBackgroundColor;
    QString pressedBackgroundColor;
    QString hoverBorderColor;

    if (accent) {
        backgroundColor = colorToCss(palette.accentColor());
        borderColor = colorToCss(palette.accentColor());
        textColor = QStringLiteral("#FFFFFF");
        hoverBackgroundColor = colorToCss(palette.accentHoverColor());
        pressedBackgroundColor = colorToCss(palette.accentPressedColor());
        hoverBorderColor = colorToCss(palette.accentHoverColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = colorToCss(palette.borderColor());
        textColor = colorToCss(palette.textPrimaryColor());
        hoverBackgroundColor = colorToCss(palette.surfaceHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
        hoverBorderColor = colorToCss(palette.borderHoverColor());
    }

    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %7px;"
               "padding: %8px %13px %8px %9px;"
               "font-size: %10px;"
               "font-weight: 700;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QPushButton:pressed {"
               "background-color: %6;"
               "padding-top: %11px;"
               "padding-bottom: %12px;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             hoverBorderColor,
             pressedBackgroundColor,
             numberCss(metrics.mediumRadius),
             numberCss(metrics.buttonPaddingVertical),
             numberCss(metrics.buttonPaddingHorizontal),
             numberCss(metrics.bodyFontSize),
             numberCss(metrics.buttonPaddingVertical + 1),
             numberCss(metrics.buttonPaddingVertical - 1),
             numberCss(badgeRightPadding(metrics)));
}

QString AverraStyleHelper::badgeButtonBadgeStyleSheet(const AverraThemePalette &palette, bool accent)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString textColor;

    if (accent) {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        textColor = colorToCss(palette.accentColor());
    } else {
        backgroundColor = colorToCss(palette.accentSurfaceColor());
        textColor = colorToCss(palette.accentColor());
    }

    return QStringLiteral(
               "QLabel#AverraBadgeButtonBadge {"
               "background-color: %1;"
               "color: %2;"
               "border-radius: %3px;"
               "padding: %4px %5px;"
               "font-size: %6px;"
               "font-weight: 700;"
               "}")
        .arg(backgroundColor,
             textColor,
             numberCss(capsuleRadius(metrics)),
             numberCss(compactInset(metrics)),
             numberCss(compactInsetHorizontal(metrics)),
             numberCss(metrics.smallFontSize - 1));
}

QString AverraStyleHelper::filterChipStyleSheet(const AverraThemePalette &palette, bool selected)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString borderColor;
    QString textColor;
    QString hoverBackgroundColor;
    QString hoverBorderColor;
    QString hoverTextColor;
    QString pressedBackgroundColor;

    if (selected) {
        backgroundColor = colorToCss(palette.accentSurfaceColor());
        borderColor = colorToCss(palette.accentColor());
        textColor = colorToCss(palette.accentColor());
        hoverBackgroundColor = colorToCss(palette.accentSurfaceColor());
        hoverBorderColor = colorToCss(palette.accentHoverColor());
        hoverTextColor = colorToCss(palette.accentHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = colorToCss(palette.borderColor());
        textColor = colorToCss(palette.textSecondaryColor());
        hoverBackgroundColor = colorToCss(palette.surfaceHoverColor());
        hoverBorderColor = colorToCss(palette.borderHoverColor());
        hoverTextColor = colorToCss(palette.textPrimaryColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
    }

    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %13px;"
               "padding: %8px %9px;"
               "font-size: %10px;"
               "font-weight: 700;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "color: %6;"
               "}"
               "QPushButton:pressed {"
               "background-color: %7;"
               "border-color: %4;"
               "padding-top: %11px;"
               "padding-bottom: %12px;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             hoverBorderColor,
             hoverTextColor,
             pressedBackgroundColor,
             numberCss(metrics.compactPaddingVertical + 1),
             numberCss(metrics.compactPaddingHorizontal + 2),
             numberCss(metrics.smallFontSize),
             numberCss(metrics.compactPaddingVertical + 2),
             numberCss(metrics.compactPaddingVertical),
             numberCss(capsuleRadius(metrics)));
}

QString AverraStyleHelper::lineEditStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    const StyleMetrics metrics = styleMetrics();
    QString borderColor;

    if (accentFrame) {
        borderColor = colorToCss(palette.accentColor());
    } else {
        borderColor = colorToCss(palette.borderColor());
    }

    return QStringLiteral(
               "QLineEdit {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %9px;"
               "padding: 0 %10px;"
               "selection-background-color: %4;"
               "selection-color: #FFFFFF;"
               "}"
               "QLineEdit:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QLineEdit:focus {"
               "border: 1px solid %6;"
               "background-color: %1;"
               "}"
               "QLineEdit:disabled {"
               "background-color: %7;"
               "color: %8;"
               "border-color: %7;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             borderColor,
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()),
             colorToCss(palette.accentColor()),
             colorToCss(palette.surfaceDisabledColor()),
             colorToCss(palette.textDisabledColor()),
             numberCss(metrics.controlRadius),
             numberCss(metrics.buttonPaddingHorizontal - 4));
}

QString AverraStyleHelper::textAreaStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    const StyleMetrics metrics = styleMetrics();
    QString borderColor;

    if (accentFrame) {
        borderColor = colorToCss(palette.accentColor());
    } else {
        borderColor = colorToCss(palette.borderColor());
    }

    return QStringLiteral(
               "QTextEdit {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %9px;"
               "padding: %10px %11px;"
               "selection-background-color: %4;"
               "selection-color: #FFFFFF;"
               "font-size: %12px;"
               "}"
               "QTextEdit:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QTextEdit:focus {"
               "border: 1px solid %6;"
               "background-color: %1;"
               "}"
               "QTextEdit:disabled {"
               "background-color: %7;"
               "color: %8;"
               "border-color: %7;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             borderColor,
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()),
             colorToCss(palette.accentColor()),
             colorToCss(palette.surfaceDisabledColor()),
             colorToCss(palette.textDisabledColor()),
             numberCss(metrics.mediumRadius),
             numberCss(metrics.buttonPaddingVertical),
             numberCss(metrics.compactPaddingHorizontal + 2),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::comboBoxStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    const StyleMetrics metrics = styleMetrics();
    QString borderColor;

    if (accentFrame) {
        borderColor = colorToCss(palette.accentColor());
    } else {
        borderColor = colorToCss(palette.borderColor());
    }

    return QStringLiteral(
               "QComboBox {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %10px;"
               "padding: 0 %14px 0 %11px;"
               "font-size: %12px;"
               "min-height: %15px;"
               "}"
               "QComboBox:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QComboBox:disabled {"
               "background-color: %6;"
               "color: %7;"
               "border-color: %6;"
               "}"
               "QComboBox::drop-down {"
               "border: none;"
               "width: %16px;"
               "}"
               "QComboBox::down-arrow {"
               "image: url(:/Averra/icons/chevron-down.svg);"
               "width: %17px;"
               "height: %17px;"
               "}"
               "QComboBox QAbstractItemView {"
               "background-color: %8;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %13px;"
               "selection-background-color: %9;"
               "selection-color: #FFFFFF;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             borderColor,
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()),
             colorToCss(palette.surfaceDisabledColor()),
             colorToCss(palette.textDisabledColor()),
             colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.accentColor()),
             numberCss(metrics.controlRadius),
             numberCss(metrics.compactPaddingHorizontal + 2),
             numberCss(metrics.bodyFontSize),
             numberCss(metrics.mediumRadius),
             numberCss(comboTrailingPadding(metrics)),
             numberCss(regularControlHeight(metrics)),
             numberCss(comboDropDownWidth(metrics)),
             numberCss(regularArrowIconSize(metrics)));
}

QString AverraStyleHelper::numberBoxStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    const StyleMetrics metrics = styleMetrics();
    QString borderColor;

    if (accentFrame) {
        borderColor = colorToCss(palette.accentColor());
    } else {
        borderColor = colorToCss(palette.borderColor());
    }

    return QStringLiteral(
               "QSpinBox {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %8px;"
               "padding: 0 %11px 0 %9px;"
               "font-size: %10px;"
               "min-height: %12px;"
               "}"
               "QSpinBox:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QSpinBox:disabled {"
               "background-color: %6;"
               "color: %7;"
               "border-color: %6;"
               "}"
               "QSpinBox::up-button, QSpinBox::down-button {"
               "border: none;"
               "width: %13px;"
               "background: transparent;"
               "}"
               "QSpinBox::up-arrow, QSpinBox::down-arrow {"
               "width: %14px;"
               "height: %14px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             borderColor,
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()),
             colorToCss(palette.surfaceDisabledColor()),
             colorToCss(palette.textDisabledColor()),
             numberCss(metrics.controlRadius),
             numberCss(metrics.compactPaddingHorizontal + 2),
             numberCss(metrics.bodyFontSize),
             numberCss(spinTrailingPadding(metrics)),
             numberCss(regularControlHeight(metrics)),
             numberCss(spinButtonWidth(metrics)),
             numberCss(smallArrowIconSize(metrics)));
}

QString AverraStyleHelper::dateEditStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    const StyleMetrics metrics = styleMetrics();
    QString borderColor;

    if (accentFrame) {
        borderColor = colorToCss(palette.accentColor());
    } else {
        borderColor = colorToCss(palette.borderColor());
    }

    return QStringLiteral(
               "QDateEdit {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %8px;"
               "padding: 0 %11px 0 %9px;"
               "font-size: %10px;"
               "min-height: %12px;"
               "}"
               "QDateEdit:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QDateEdit:disabled {"
               "background-color: %6;"
               "color: %7;"
               "border-color: %6;"
               "}"
               "QDateEdit::drop-down {"
               "border: none;"
               "width: %13px;"
               "}"
               "QDateEdit::down-arrow {"
               "image: url(:/Averra/icons/chevron-down.svg);"
               "width: %14px;"
               "height: %14px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             borderColor,
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()),
             colorToCss(palette.surfaceDisabledColor()),
             colorToCss(palette.textDisabledColor()),
             numberCss(metrics.controlRadius),
             numberCss(metrics.compactPaddingHorizontal + 2),
             numberCss(metrics.bodyFontSize),
             numberCss(dateTrailingPadding(metrics)),
             numberCss(regularControlHeight(metrics)),
             numberCss(dateDropDownWidth(metrics)),
             numberCss(regularArrowIconSize(metrics)));
}

QString AverraStyleHelper::calendarWidgetStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    const StyleMetrics metrics = styleMetrics();
    QString accentColor;

    if (accentFrame) {
        accentColor = colorToCss(palette.accentColor());
    } else {
        accentColor = colorToCss(palette.borderHoverColor());
    }

    return QStringLiteral(
               "QCalendarWidget {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %11px;"
               "}"
               "QCalendarWidget QWidget#qt_calendar_navigationbar {"
               "background-color: %4;"
               "border-bottom: 1px solid %3;"
               "border-top-left-radius: %11px;"
               "border-top-right-radius: %11px;"
               "}"
               "QCalendarWidget QToolButton {"
               "background-color: transparent;"
               "color: %2;"
               "border: none;"
               "border-radius: %12px;"
               "padding: %16px %17px;"
               "font-size: %13px;"
               "font-weight: 700;"
               "min-width: %18px;"
               "}"
               "QCalendarWidget QToolButton:hover {"
               "background-color: %5;"
               "color: %6;"
               "}"
               "QCalendarWidget QToolButton:pressed {"
               "background-color: %7;"
               "color: %6;"
               "}"
               "QCalendarWidget QSpinBox {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %12px;"
               "padding: %19px %20px;"
               "min-width: %21px;"
               "selection-background-color: %8;"
               "selection-color: #FFFFFF;"
               "}"
               "QCalendarWidget QMenu {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "padding: %22px;"
               "}"
               "QCalendarWidget QMenu::item {"
               "padding: %23px %24px;"
               "border-radius: %14px;"
               "}"
               "QCalendarWidget QMenu::item:selected {"
               "background-color: %5;"
               "color: %6;"
               "}"
               "QCalendarWidget QAbstractItemView {"
               "background-color: %1;"
               "color: %2;"
               "selection-background-color: %8;"
               "selection-color: #FFFFFF;"
               "outline: 0;"
               "}"
               "QCalendarWidget QAbstractItemView:enabled {"
               "background-color: %1;"
               "}"
               "QCalendarWidget QTableView {"
               "border: none;"
               "alternate-background-color: %4;"
               "gridline-color: %3;"
               "}"
               "QCalendarWidget QTableView::item {"
               "border-radius: %12px;"
               "padding: %19px;"
               "}"
               "QCalendarWidget QTableView::item:hover {"
               "background-color: %5;"
               "color: %6;"
               "}"
               "QCalendarWidget QTableView::item:selected {"
               "background-color: %8;"
               "color: #FFFFFF;"
               "}"
               "QCalendarWidget QTableView::item:disabled {"
               "color: %9;"
               "}"
               "QCalendarWidget QHeaderView {"
               "background-color: transparent;"
               "}"
               "QCalendarWidget QHeaderView::section {"
               "background-color: transparent;"
               "color: %10;"
               "border: none;"
               "padding: %23px 0;"
               "font-size: %15px;"
               "font-weight: 700;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             colorToCss(palette.borderColor()),
             colorToCss(palette.surfaceColor()),
             colorToCss(palette.accentSurfaceColor()),
             colorToCss(palette.accentColor()),
             colorToCss(palette.surfacePressedColor()),
             accentColor,
             colorToCss(palette.textDisabledColor()),
             colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.largeRadius),
             numberCss(metrics.smallRadius),
             numberCss(metrics.bodyFontSize),
             numberCss(metrics.smallRadius - 1),
             numberCss(metrics.smallFontSize),
             numberCss(qMax(4, metrics.compactPaddingVertical - 1)),
             numberCss(metrics.compactPaddingHorizontal),
             numberCss(qMax(24, metrics.buttonPaddingHorizontal + 10)),
             numberCss(compactInset(metrics)),
             numberCss(compactInsetHorizontal(metrics)),
             numberCss(qMax(56, regularControlHeight(metrics) + metrics.buttonPaddingHorizontal + 6)),
             numberCss(qMax(4, metrics.compactPaddingVertical - 1)),
             numberCss(menuItemHorizontalPadding(metrics)));
}

QString AverraStyleHelper::searchBarFrameStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    const StyleMetrics metrics = styleMetrics();
    QString borderColor;

    if (accentFrame) {
        borderColor = colorToCss(palette.accentColor());
    } else {
        borderColor = colorToCss(palette.borderColor());
    }

    return QStringLiteral(
               "QFrame#AverraSearchBarRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %5px;"
               "}"
               "QFrame#AverraSearchBarRoot:hover {"
               "background-color: %3;"
               "border-color: %4;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             borderColor,
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()),
             numberCss(metrics.mediumRadius));
}

QString AverraStyleHelper::searchBarLineEditStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLineEdit#AverraSearchBarLineEdit {"
               "background: transparent;"
               "border: none;"
               "color: %1;"
               "padding: 0 %3px;"
               "font-size: %2px;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.bodyFontSize),
             numberCss(searchInsetHorizontal(metrics)));
}

QString AverraStyleHelper::searchBarButtonStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString textColor;
    QString hoverBackgroundColor;
    QString pressedBackgroundColor;

    if (accentFrame) {
        backgroundColor = colorToCss(palette.accentColor());
        textColor = QStringLiteral("#FFFFFF");
        hoverBackgroundColor = colorToCss(palette.accentHoverColor());
        pressedBackgroundColor = colorToCss(palette.accentPressedColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceColor());
        textColor = colorToCss(palette.textPrimaryColor());
        hoverBackgroundColor = colorToCss(palette.surfaceHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
    }

    return QStringLiteral(
               "QPushButton#AverraSearchBarButton {"
               "background-color: %1;"
               "color: %2;"
               "border: none;"
               "border-radius: %5px;"
               "padding: %6px %7px;"
               "font-size: %8px;"
               "font-weight: 700;"
               "}"
               "QPushButton#AverraSearchBarButton:hover {"
               "background-color: %3;"
               "}"
               "QPushButton#AverraSearchBarButton:pressed {"
               "background-color: %4;"
               "}")
        .arg(backgroundColor,
             textColor,
             hoverBackgroundColor,
             pressedBackgroundColor,
             numberCss(metrics.smallRadius + 1),
             numberCss(metrics.compactPaddingVertical + 1),
             numberCss(metrics.compactPaddingHorizontal + 2),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::cardFrameStyleSheet(const AverraThemePalette &palette, bool accentHeader)
{
    const StyleMetrics metrics = styleMetrics();
    QString borderColor;
    QString backgroundColor;

    if (accentHeader) {
        borderColor = colorToCss(palette.accentColor());
        backgroundColor = colorToCss(palette.accentSurfaceColor());
    } else {
        borderColor = colorToCss(palette.borderColor());
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
    }

    return QStringLiteral(
               "QFrame#AverraCardRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(backgroundColor, borderColor, numberCss(metrics.cardRadius));
}

QString AverraStyleHelper::cardTitleLabelStyleSheet(const AverraThemePalette &palette, bool accentHeader)
{
    const StyleMetrics metrics = styleMetrics();
    QString textColor;

    if (accentHeader) {
        textColor = colorToCss(palette.accentColor());
    } else {
        textColor = colorToCss(palette.textPrimaryColor());
    }

    return QStringLiteral(
               "QLabel {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(textColor, numberCss(metrics.titleFontSize));
}

QString AverraStyleHelper::cardSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::cardContentWidgetStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QWidget#AverraCardContent {"
               "background-color: %1;"
               "border: none;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()));
}

QString AverraStyleHelper::infoBarFrameStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const StyleMetrics metrics = styleMetrics();
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);

    return QStringLiteral(
               "QFrame#AverraInfoBarRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(colors.second),
             colorToCss(colors.first),
             numberCss(metrics.largeRadius));
}

QString AverraStyleHelper::infoBarBadgeStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const StyleMetrics metrics = styleMetrics();
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);

    return QStringLiteral(
               "QLabel#AverraInfoBarBadge {"
               "background-color: %1;"
               "color: #FFFFFF;"
               "border-radius: %2px;"
               "font-size: %3px;"
               "font-weight: 700;"
               "padding: %4px %5px;"
               "}")
        .arg(colorToCss(colors.first),
             numberCss(metrics.controlRadius),
             numberCss(metrics.smallFontSize),
             numberCss(compactInset(metrics)),
             numberCss(compactInsetHorizontal(metrics)));
}

QString AverraStyleHelper::infoBarTitleLabelStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const StyleMetrics metrics = styleMetrics();
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);

    return QStringLiteral(
               "QLabel#AverraInfoBarTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(colors.first),
             numberCss(metrics.bodyFontSize + 1));
}

QString AverraStyleHelper::infoBarDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraInfoBarDescription {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::infoBarCloseButtonStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const StyleMetrics metrics = styleMetrics();
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);

    return QStringLiteral(
               "QToolButton#AverraInfoBarCloseButton {"
               "background-color: transparent;"
               "border: none;"
               "color: %1;"
               "padding: %4px;"
               "border-radius: %5px;"
               "}"
               "QToolButton#AverraInfoBarCloseButton:hover {"
               "background-color: %2;"
               "}"
               "QToolButton#AverraInfoBarCloseButton:pressed {"
               "background-color: %3;"
               "}")
        .arg(colorToCss(colors.first),
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.surfacePressedColor()),
             numberCss(compactInset(metrics)),
             numberCss(metrics.smallRadius));
}

QString AverraStyleHelper::navigationPanelFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraNavigationPanelRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.cardRadius));
}

QString AverraStyleHelper::navigationPanelTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraNavigationPanelTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.titleFontSize - 1));
}

QString AverraStyleHelper::navigationPanelItemButtonStyleSheet(const AverraThemePalette &palette, bool current)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString textColor;
    QString borderColor;
    QString hoverBackgroundColor;
    QString hoverTextColor;
    QString hoverBorderColor;
    QString pressedBackgroundColor;

    if (current) {
        backgroundColor = colorToCss(palette.accentSurfaceColor());
        textColor = colorToCss(palette.accentColor());
        borderColor = colorToCss(palette.accentColor());
        hoverBackgroundColor = colorToCss(palette.accentSurfaceColor());
        hoverTextColor = colorToCss(palette.accentHoverColor());
        hoverBorderColor = colorToCss(palette.accentHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        textColor = colorToCss(palette.textSecondaryColor());
        borderColor = QStringLiteral("transparent");
        hoverBackgroundColor = colorToCss(palette.surfaceHoverColor());
        hoverTextColor = colorToCss(palette.textPrimaryColor());
        hoverBorderColor = colorToCss(palette.borderHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
    }

    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %8px;"
               "padding: %9px %10px;"
               "text-align: left;"
               "font-size: %11px;"
               "font-weight: 600;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "color: %6;"
               "}"
               "QPushButton:pressed {"
               "background-color: %7;"
               "padding-top: %12px;"
               "padding-bottom: %13px;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             hoverBorderColor,
             hoverTextColor,
             pressedBackgroundColor,
             numberCss(metrics.controlRadius),
             numberCss(metrics.buttonPaddingVertical),
             numberCss(metrics.buttonPaddingHorizontal - 4),
             numberCss(metrics.bodyFontSize),
             numberCss(metrics.buttonPaddingVertical + 1),
             numberCss(metrics.buttonPaddingVertical - 1));
}

QString AverraStyleHelper::tagStyleSheet(const AverraThemePalette &palette, TagTone tone, bool filled)
{
    const StyleMetrics metrics = styleMetrics();
    const QPair<QColor, QColor> colors = tagColors(palette, tone);
    QString backgroundColor;
    QString borderColor;
    QString textColor;

    if (filled) {
        backgroundColor = colorToCss(colors.second);
        borderColor = colorToCss(colors.first);
        textColor = colorToCss(colors.first);
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = colorToCss(colors.first);
        textColor = colorToCss(colors.first);
    }

    return QStringLiteral(
               "QLabel {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %4px;"
               "padding: %5px %6px;"
               "font-size: %7px;"
               "font-weight: 700;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             numberCss(capsuleRadius(metrics)),
             numberCss(compactInset(metrics)),
             numberCss(metrics.compactPaddingHorizontal),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::sectionHeaderTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraSectionHeaderTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.titleFontSize));
}

QString AverraStyleHelper::sectionHeaderSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraSectionHeaderSubtitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::sectionHeaderMetaLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraSectionHeaderMeta {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.accentColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::sectionHeaderDividerStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraSectionHeaderDivider {"
               "background-color: %1;"
               "border: none;"
               "min-height: 1px;"
               "max-height: 1px;"
               "}")
        .arg(colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::emptyStateFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraEmptyStateRoot {"
               "background-color: %1;"
               "border: 1px dashed %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.panelRadius));
}

QString AverraStyleHelper::emptyStateIconLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraEmptyStateIcon {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.accentColor()),
             numberCss(metrics.displayFontSize));
}

QString AverraStyleHelper::emptyStateTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraEmptyStateTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.titleFontSize));
}

QString AverraStyleHelper::emptyStateDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraEmptyStateDescription {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::statisticCardFrameStyleSheet(const AverraThemePalette &palette, bool accent)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString borderColor;

    if (accent) {
        backgroundColor = colorToCss(palette.accentSurfaceColor());
        borderColor = colorToCss(palette.accentColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = colorToCss(palette.borderColor());
    }

    return QStringLiteral(
               "QFrame#AverraStatisticCardRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(backgroundColor, borderColor, numberCss(metrics.cardRadius));
}

QString AverraStyleHelper::statisticCardCaptionLabelStyleSheet(const AverraThemePalette &palette, bool accent)
{
    const StyleMetrics metrics = styleMetrics();
    QString textColor;

    if (accent) {
        textColor = colorToCss(palette.accentColor());
    } else {
        textColor = colorToCss(palette.textSecondaryColor());
    }

    return QStringLiteral(
               "QLabel#AverraStatisticCardCaption {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(textColor,
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::statisticCardValueLabelStyleSheet(const AverraThemePalette &palette, bool accent)
{
    const StyleMetrics metrics = styleMetrics();
    QString textColor;

    if (accent) {
        textColor = colorToCss(palette.accentColor());
    } else {
        textColor = colorToCss(palette.textPrimaryColor());
    }

    return QStringLiteral(
               "QLabel#AverraStatisticCardValue {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(textColor,
             numberCss(metrics.displayFontSize));
}

QString AverraStyleHelper::statisticCardHelperLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraStatisticCardHelper {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::statisticCardBadgeLabelStyleSheet(const AverraThemePalette &palette, bool accent)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString textColor;

    if (accent) {
        backgroundColor = colorToCss(palette.accentColor());
        textColor = QStringLiteral("#FFFFFF");
    } else {
        backgroundColor = colorToCss(palette.surfaceColor());
        textColor = colorToCss(palette.textPrimaryColor());
    }

    return QStringLiteral(
               "QLabel#AverraStatisticCardBadge {"
               "background-color: %1;"
               "color: %2;"
               "border-radius: %3px;"
               "padding: %4px %5px;"
               "font-size: %6px;"
               "font-weight: 700;"
               "}")
        .arg(backgroundColor,
             textColor,
             numberCss(capsuleRadius(metrics)),
             numberCss(compactInset(metrics)),
             numberCss(compactInsetHorizontal(metrics)),
             numberCss(metrics.smallFontSize - 1));
}

QString AverraStyleHelper::toolbarFrameStyleSheet(const AverraThemePalette &palette, bool accent)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString borderColor;

    if (accent) {
        backgroundColor = colorToCss(palette.accentSurfaceColor());
        borderColor = colorToCss(palette.accentColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = colorToCss(palette.borderColor());
    }

    return QStringLiteral(
               "QFrame#AverraToolbarRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(backgroundColor, borderColor, numberCss(metrics.cardRadius));
}

QString AverraStyleHelper::toolbarTitleLabelStyleSheet(const AverraThemePalette &palette, bool accent)
{
    const StyleMetrics metrics = styleMetrics();
    QString textColor;

    if (accent) {
        textColor = colorToCss(palette.accentColor());
    } else {
        textColor = colorToCss(palette.textPrimaryColor());
    }

    return QStringLiteral(
               "QLabel#AverraToolbarTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(textColor, numberCss(metrics.titleFontSize));
}

QString AverraStyleHelper::toolbarSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraToolbarSubtitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::paginationButtonStyleSheet(const AverraThemePalette &palette, bool current)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString borderColor;
    QString textColor;
    QString hoverBackgroundColor;
    QString hoverBorderColor;
    QString pressedBackgroundColor;
    QString disabledBackgroundColor;
    QString disabledBorderColor;
    QString disabledTextColor;

    if (current) {
        backgroundColor = colorToCss(palette.accentColor());
        borderColor = colorToCss(palette.accentColor());
        textColor = QStringLiteral("#FFFFFF");
        hoverBackgroundColor = colorToCss(palette.accentHoverColor());
        hoverBorderColor = colorToCss(palette.accentHoverColor());
        pressedBackgroundColor = colorToCss(palette.accentPressedColor());
        disabledBackgroundColor = colorToCss(palette.accentDisabledColor());
        disabledBorderColor = colorToCss(palette.accentDisabledColor());
        disabledTextColor = colorToCss(palette.surfaceRaisedColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = colorToCss(palette.borderColor());
        textColor = colorToCss(palette.textPrimaryColor());
        hoverBackgroundColor = colorToCss(palette.surfaceHoverColor());
        hoverBorderColor = colorToCss(palette.borderHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
        disabledBackgroundColor = colorToCss(palette.surfaceDisabledColor());
        disabledBorderColor = colorToCss(palette.surfaceDisabledColor());
        disabledTextColor = colorToCss(palette.textDisabledColor());
    }

    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %10px;"
               "padding: %11px %12px;"
               "font-size: %13px;"
               "font-weight: 700;"
               "min-width: %14px;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QPushButton:pressed {"
               "background-color: %6;"
               "}"
               "QPushButton:disabled {"
               "background-color: %7;"
               "color: %8;"
               "border-color: %9;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             hoverBorderColor,
             pressedBackgroundColor,
             disabledBackgroundColor,
             disabledTextColor,
             disabledBorderColor,
             numberCss(metrics.controlRadius),
             numberCss(metrics.compactPaddingVertical + 1),
             numberCss(metrics.compactPaddingHorizontal),
             numberCss(metrics.smallFontSize),
             numberCss(paginationMinWidth(metrics)));
}

QString AverraStyleHelper::paginationLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraPaginationLabel {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 600;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::propertyItemFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraPropertyItemRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.largeRadius));
}

QString AverraStyleHelper::propertyItemTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraPropertyItemTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::propertyItemDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraPropertyItemDescription {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::formSectionFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraFormSectionRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.panelRadius));
}

QString AverraStyleHelper::settingsPageFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraSettingsPageRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.pageRadius));
}

QString AverraStyleHelper::settingsPageTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraSettingsPageTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.pageTitleFontSize));
}

QString AverraStyleHelper::settingsPageDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraSettingsPageDescription {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::dataTableFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraDataTableRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.cardRadius));
}

QString AverraStyleHelper::dataTableTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraDataTableTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.bodyFontSize + 1));
}

QString AverraStyleHelper::dataTableSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraDataTableSubtitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::dataTableViewStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QTableView {"
               "background-color: %1;"
               "border: none;"
               "gridline-color: %2;"
               "selection-background-color: %3;"
               "selection-color: #FFFFFF;"
               "alternate-background-color: %4;"
               "}"
               "QHeaderView::section {"
               "background-color: %4;"
               "color: %5;"
               "border: none;"
               "padding: %6px;"
               "font-size: %7px;"
               "font-weight: 700;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             colorToCss(palette.accentColor()),
             colorToCss(palette.surfaceColor()),
             colorToCss(palette.textPrimaryColor()),
             numberCss(tableHeaderPadding(metrics)),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::dataTableToolbarFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraDataTableToolbar {"
               "background-color: %1;"
               "border: none;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()));
}

QString AverraStyleHelper::dataTableActionButtonStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %9px;"
               "padding: %10px %11px;"
               "font-size: %12px;"
               "font-weight: 700;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QPushButton:pressed {"
               "background-color: %6;"
               "}"
               "QPushButton:disabled {"
               "background-color: %7;"
               "color: %8;"
               "border-color: %7;"
               "}")
        .arg(colorToCss(palette.surfaceColor()),
             colorToCss(palette.textPrimaryColor()),
             colorToCss(palette.borderColor()),
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()),
             colorToCss(palette.surfacePressedColor()),
             colorToCss(palette.surfaceDisabledColor()),
             colorToCss(palette.textDisabledColor()),
             numberCss(metrics.smallRadius),
             numberCss(metrics.compactPaddingVertical - 1),
             numberCss(metrics.compactPaddingHorizontal - 2),
             numberCss(metrics.smallFontSize - 1));
}

QString AverraStyleHelper::tabButtonStyleSheet(const AverraThemePalette &palette, bool current)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString borderColor;
    QString textColor;
    QString hoverBackgroundColor;
    QString hoverBorderColor;
    QString pressedBackgroundColor;

    if (current) {
        backgroundColor = colorToCss(palette.accentSurfaceColor());
        borderColor = colorToCss(palette.accentColor());
        textColor = colorToCss(palette.accentColor());
        hoverBackgroundColor = colorToCss(palette.accentSurfaceColor());
        hoverBorderColor = colorToCss(palette.accentHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = QStringLiteral("transparent");
        textColor = colorToCss(palette.textSecondaryColor());
        hoverBackgroundColor = colorToCss(palette.surfaceHoverColor());
        hoverBorderColor = colorToCss(palette.borderHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
    }

    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %7px;"
               "padding: %8px %9px;"
               "font-size: %10px;"
               "font-weight: 700;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QPushButton:pressed {"
               "background-color: %6;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             hoverBorderColor,
             pressedBackgroundColor,
             numberCss(metrics.controlRadius),
             numberCss(metrics.compactPaddingVertical + 1),
             numberCss(metrics.buttonPaddingHorizontal - 4),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::segmentedButtonStyleSheet(const AverraThemePalette &palette, bool current, bool first, bool last)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString borderColor;
    QString textColor;
    QString hoverBackgroundColor;
    QString hoverBorderColor;
    QString pressedBackgroundColor;
    QString radiusLeft = first ? QStringLiteral("%1px").arg(metrics.controlRadius) : QStringLiteral("0px");
    QString radiusRight = last ? QStringLiteral("%1px").arg(metrics.controlRadius) : QStringLiteral("0px");

    if (current) {
        backgroundColor = colorToCss(palette.accentColor());
        borderColor = colorToCss(palette.accentColor());
        textColor = QStringLiteral("#FFFFFF");
        hoverBackgroundColor = colorToCss(palette.accentHoverColor());
        hoverBorderColor = colorToCss(palette.accentHoverColor());
        pressedBackgroundColor = colorToCss(palette.accentPressedColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = colorToCss(palette.borderColor());
        textColor = colorToCss(palette.textPrimaryColor());
        hoverBackgroundColor = colorToCss(palette.surfaceHoverColor());
        hoverBorderColor = colorToCss(palette.borderHoverColor());
        pressedBackgroundColor = colorToCss(palette.surfacePressedColor());
    }

    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-top-left-radius: %4;"
               "border-bottom-left-radius: %4;"
               "border-top-right-radius: %5;"
               "border-bottom-right-radius: %5;"
               "padding: %9px %10px;"
               "font-size: %11px;"
               "font-weight: 700;"
               "}"
               "QPushButton:hover {"
               "background-color: %6;"
               "border-color: %7;"
               "}"
               "QPushButton:pressed {"
               "background-color: %8;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             radiusLeft,
             radiusRight,
             hoverBackgroundColor,
             hoverBorderColor,
             pressedBackgroundColor,
             numberCss(metrics.compactPaddingVertical + 1),
             numberCss(metrics.compactPaddingHorizontal + 2),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::dialogFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraDialogRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.cardRadius));
}

QString AverraStyleHelper::dialogTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraDialogTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.pageTitleFontSize));
}

QString AverraStyleHelper::dialogDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraDialogDescription {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::toastFrameStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const StyleMetrics metrics = styleMetrics();
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);
    return QStringLiteral(
               "QFrame#AverraToastRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(colors.second),
             colorToCss(colors.first),
             numberCss(metrics.largeRadius));
}

QString AverraStyleHelper::toastTitleLabelStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const StyleMetrics metrics = styleMetrics();
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);
    return QStringLiteral(
               "QLabel#AverraToastTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(colors.first),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::toastDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraToastDescription {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::ribbonBarFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraRibbonBarRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.panelRadius));
}

QString AverraStyleHelper::ribbonBarTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraRibbonBarTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.pageTitleFontSize));
}

QString AverraStyleHelper::ribbonBarSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraRibbonBarSubtitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::ribbonBarTabButtonStyleSheet(const AverraThemePalette &palette, bool current)
{
    const StyleMetrics metrics = styleMetrics();
    QString backgroundColor;
    QString borderColor;
    QString textColor;
    QString hoverBackgroundColor;

    if (current) {
        backgroundColor = colorToCss(palette.accentSurfaceColor());
        borderColor = colorToCss(palette.accentColor());
        textColor = colorToCss(palette.accentColor());
        hoverBackgroundColor = colorToCss(palette.accentSurfaceColor());
    } else {
        backgroundColor = colorToCss(palette.surfaceRaisedColor());
        borderColor = QStringLiteral("transparent");
        textColor = colorToCss(palette.textSecondaryColor());
        hoverBackgroundColor = colorToCss(palette.surfaceHoverColor());
    }

    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: %6px;"
               "padding: %7px %8px;"
               "font-size: %9px;"
               "font-weight: 700;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             colorToCss(current ? palette.accentHoverColor() : palette.borderHoverColor()),
             numberCss(metrics.controlRadius),
             numberCss(metrics.compactPaddingVertical + 1),
             numberCss(metrics.compactPaddingHorizontal + 2),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::ribbonBarGroupFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraRibbonGroupRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.largeRadius));
}

QString AverraStyleHelper::ribbonBarGroupTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraRibbonGroupTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize - 1));
}

QString AverraStyleHelper::treeViewFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraTreeViewRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.cardRadius));
}

QString AverraStyleHelper::treeViewTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraTreeViewTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.bodyFontSize + 1));
}

QString AverraStyleHelper::treeViewViewStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QTreeView {"
               "background-color: %1;"
               "border: none;"
               "alternate-background-color: %2;"
               "selection-background-color: %3;"
               "selection-color: #FFFFFF;"
               "}"
               "QHeaderView::section {"
               "background-color: %2;"
               "color: %4;"
               "border: none;"
               "padding: %5px;"
               "font-size: %6px;"
               "font-weight: 700;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.surfaceColor()),
             colorToCss(palette.accentColor()),
             colorToCss(palette.textPrimaryColor()),
             numberCss(tableHeaderPadding(metrics)),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::dockFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraDockRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.cardRadius));
}

QString AverraStyleHelper::dockTitleBarFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraDockTitleBar {"
               "background-color: %1;"
               "border-bottom: 1px solid %2;"
               "border-top-left-radius: %3px;"
               "border-top-right-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.cardRadius));
}

QString AverraStyleHelper::dockTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraDockTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.bodyFontSize + 1));
}

QString AverraStyleHelper::dockDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraDockDescription {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::dockCloseButtonStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QToolButton#AverraDockCloseButton {"
               "background-color: transparent;"
               "border: none;"
               "color: %1;"
               "border-radius: %4px;"
               "padding: %5px;"
               "}"
               "QToolButton#AverraDockCloseButton:hover {"
               "background-color: %2;"
               "}"
               "QToolButton#AverraDockCloseButton:pressed {"
               "background-color: %3;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.surfacePressedColor()),
             numberCss(metrics.smallRadius),
             numberCss(compactInset(metrics)));
}

QString AverraStyleHelper::drawerFrameStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QFrame#AverraDrawerRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             numberCss(metrics.panelRadius));
}

QString AverraStyleHelper::drawerTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraDrawerTitle {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()),
             numberCss(metrics.titleFontSize));
}

QString AverraStyleHelper::drawerDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraDrawerDescription {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.bodyFontSize));
}

QString AverraStyleHelper::numberRangeBoxFrameStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    const StyleMetrics metrics = styleMetrics();
    QString borderColor = accentFrame ? colorToCss(palette.accentColor())
                                      : colorToCss(palette.borderColor());

    return QStringLiteral(
               "QFrame#AverraNumberRangeBoxRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: %3px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             borderColor,
             numberCss(metrics.mediumRadius));
}

QString AverraStyleHelper::numberRangeBoxLabelStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QLabel#AverraNumberRangeBoxLabel {"
               "color: %1;"
               "font-size: %2px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             numberCss(metrics.smallFontSize));
}

QString AverraStyleHelper::contextMenuStyleSheet(const AverraThemePalette &palette)
{
    const StyleMetrics metrics = styleMetrics();
    return QStringLiteral(
               "QMenu {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "padding: %5px;"
               "}"
               "QMenu::item {"
               "padding: %6px %8px;"
               "border-radius: %7px;"
               "}"
               "QMenu::item:selected {"
               "background-color: %4;"
               "color: #FFFFFF;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             colorToCss(palette.borderColor()),
             colorToCss(palette.accentColor()),
             numberCss(metrics.compactPaddingVertical),
             numberCss(metrics.compactPaddingVertical + 1),
             numberCss(metrics.smallRadius),
             numberCss(menuItemHorizontalPadding(metrics)));
}
