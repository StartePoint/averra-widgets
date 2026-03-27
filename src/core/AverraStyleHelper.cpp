#include "AverraStyleHelper.h"

#include <QString>

namespace
{
QString colorToCss(const QColor &color)
{
    return color.name();
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
               "border-radius: 10px;"
               "padding: 10px 18px;"
               "font-size: 14px;"
               "font-weight: 600;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QPushButton:pressed {"
               "background-color: %6;"
               "border-color: %4;"
               "padding-top: 11px;"
               "padding-bottom: 9px;"
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
             disabledBorderColor);
}

QString AverraStyleHelper::badgeButtonStyleSheet(const AverraThemePalette &palette, bool accent)
{
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
               "border-radius: 12px;"
               "padding: 10px 44px 10px 18px;"
               "font-size: 13px;"
               "font-weight: 700;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "}"
               "QPushButton:pressed {"
               "background-color: %6;"
               "padding-top: 11px;"
               "padding-bottom: 9px;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             hoverBorderColor,
             pressedBackgroundColor);
}

QString AverraStyleHelper::badgeButtonBadgeStyleSheet(const AverraThemePalette &palette, bool accent)
{
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
               "border-radius: 999px;"
               "padding: 3px 7px;"
               "font-size: 11px;"
               "font-weight: 700;"
               "}")
        .arg(backgroundColor, textColor);
}

QString AverraStyleHelper::filterChipStyleSheet(const AverraThemePalette &palette, bool selected)
{
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
               "border-radius: 999px;"
               "padding: 8px 12px;"
               "font-size: 12px;"
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
               "padding-top: 9px;"
               "padding-bottom: 7px;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             hoverBorderColor,
             hoverTextColor,
             pressedBackgroundColor);
}

QString AverraStyleHelper::lineEditStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
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
               "border-radius: 10px;"
               "padding: 0 14px;"
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
             colorToCss(palette.textDisabledColor()));
}

QString AverraStyleHelper::textAreaStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
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
               "border-radius: 12px;"
               "padding: 10px 12px;"
               "selection-background-color: %4;"
               "selection-color: #FFFFFF;"
               "font-size: 13px;"
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
             colorToCss(palette.textDisabledColor()));
}

QString AverraStyleHelper::comboBoxStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
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
               "border-radius: 10px;"
               "padding: 0 34px 0 12px;"
               "font-size: 13px;"
               "min-height: 40px;"
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
               "width: 28px;"
               "}"
               "QComboBox::down-arrow {"
               "image: url(:/Averra/icons/chevron-down.svg);"
               "width: 12px;"
               "height: 12px;"
               "}"
               "QComboBox QAbstractItemView {"
               "background-color: %8;"
               "color: %2;"
               "border: 1px solid %3;"
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
             colorToCss(palette.accentColor()));
}

QString AverraStyleHelper::numberBoxStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
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
               "border-radius: 10px;"
               "padding: 0 28px 0 12px;"
               "font-size: 13px;"
               "min-height: 40px;"
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
               "width: 18px;"
               "background: transparent;"
               "}"
               "QSpinBox::up-arrow, QSpinBox::down-arrow {"
               "width: 8px;"
               "height: 8px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             borderColor,
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()),
             colorToCss(palette.surfaceDisabledColor()),
             colorToCss(palette.textDisabledColor()));
}

QString AverraStyleHelper::dateEditStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
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
               "border-radius: 10px;"
               "padding: 0 30px 0 12px;"
               "font-size: 13px;"
               "min-height: 40px;"
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
               "width: 26px;"
               "}"
               "QDateEdit::down-arrow {"
               "image: url(:/Averra/icons/chevron-down.svg);"
               "width: 12px;"
               "height: 12px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             borderColor,
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()),
             colorToCss(palette.surfaceDisabledColor()),
             colorToCss(palette.textDisabledColor()));
}

QString AverraStyleHelper::calendarWidgetStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
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
               "border-radius: 14px;"
               "}"
               "QCalendarWidget QWidget#qt_calendar_navigationbar {"
               "background-color: %4;"
               "border-bottom: 1px solid %3;"
               "border-top-left-radius: 14px;"
               "border-top-right-radius: 14px;"
               "}"
               "QCalendarWidget QToolButton {"
               "background-color: transparent;"
               "color: %2;"
               "border: none;"
               "border-radius: 8px;"
               "padding: 6px 10px;"
               "font-size: 13px;"
               "font-weight: 700;"
               "min-width: 28px;"
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
               "border-radius: 8px;"
               "padding: 4px 8px;"
               "min-width: 64px;"
               "selection-background-color: %8;"
               "selection-color: #FFFFFF;"
               "}"
               "QCalendarWidget QMenu {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "padding: 6px;"
               "}"
               "QCalendarWidget QMenu::item {"
               "padding: 6px 12px;"
               "border-radius: 6px;"
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
               "border-radius: 8px;"
               "padding: 4px;"
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
               "padding: 6px 0;"
               "font-size: 12px;"
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
             colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::searchBarFrameStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
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
               "border-radius: 12px;"
               "}"
               "QFrame#AverraSearchBarRoot:hover {"
               "background-color: %3;"
               "border-color: %4;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             borderColor,
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderHoverColor()));
}

QString AverraStyleHelper::searchBarLineEditStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLineEdit#AverraSearchBarLineEdit {"
               "background: transparent;"
               "border: none;"
               "color: %1;"
               "padding: 0 4px;"
               "font-size: 13px;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::searchBarButtonStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
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
               "border-radius: 9px;"
               "padding: 8px 12px;"
               "font-size: 12px;"
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
             pressedBackgroundColor);
}

QString AverraStyleHelper::cardFrameStyleSheet(const AverraThemePalette &palette, bool accentHeader)
{
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
               "border-radius: 16px;"
               "}")
        .arg(backgroundColor, borderColor);
}

QString AverraStyleHelper::cardTitleLabelStyleSheet(const AverraThemePalette &palette, bool accentHeader)
{
    QString textColor;

    if (accentHeader) {
        textColor = colorToCss(palette.accentColor());
    } else {
        textColor = colorToCss(palette.textPrimaryColor());
    }

    return QStringLiteral(
               "QLabel {"
               "color: %1;"
               "font-size: 16px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(textColor);
}

QString AverraStyleHelper::cardSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel {"
               "color: %1;"
               "font-size: 13px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
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
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);

    return QStringLiteral(
               "QFrame#AverraInfoBarRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 14px;"
               "}")
        .arg(colorToCss(colors.second), colorToCss(colors.first));
}

QString AverraStyleHelper::infoBarBadgeStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);

    return QStringLiteral(
               "QLabel#AverraInfoBarBadge {"
               "background-color: %1;"
               "color: #FFFFFF;"
               "border-radius: 12px;"
               "font-size: 12px;"
               "font-weight: 700;"
               "padding: 4px 8px;"
               "}")
        .arg(colorToCss(colors.first));
}

QString AverraStyleHelper::infoBarTitleLabelStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);

    return QStringLiteral(
               "QLabel#AverraInfoBarTitle {"
               "color: %1;"
               "font-size: 14px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(colors.first));
}

QString AverraStyleHelper::infoBarDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraInfoBarDescription {"
               "color: %1;"
               "font-size: 13px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::infoBarCloseButtonStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);

    return QStringLiteral(
               "QToolButton#AverraInfoBarCloseButton {"
               "background-color: transparent;"
               "border: none;"
               "color: %1;"
               "padding: 4px;"
               "border-radius: 8px;"
               "}"
               "QToolButton#AverraInfoBarCloseButton:hover {"
               "background-color: %2;"
               "}"
               "QToolButton#AverraInfoBarCloseButton:pressed {"
               "background-color: %3;"
               "}")
        .arg(colorToCss(colors.first),
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.surfacePressedColor()));
}

QString AverraStyleHelper::navigationPanelFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraNavigationPanelRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 16px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::navigationPanelTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraNavigationPanelTitle {"
               "color: %1;"
               "font-size: 14px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::navigationPanelItemButtonStyleSheet(const AverraThemePalette &palette, bool current)
{
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
               "border-radius: 10px;"
               "padding: 10px 14px;"
               "text-align: left;"
               "font-size: 13px;"
               "font-weight: 600;"
               "}"
               "QPushButton:hover {"
               "background-color: %4;"
               "border-color: %5;"
               "color: %6;"
               "}"
               "QPushButton:pressed {"
               "background-color: %7;"
               "padding-top: 11px;"
               "padding-bottom: 9px;"
               "}")
        .arg(backgroundColor,
             textColor,
             borderColor,
             hoverBackgroundColor,
             hoverBorderColor,
             hoverTextColor,
             pressedBackgroundColor);
}

QString AverraStyleHelper::tagStyleSheet(const AverraThemePalette &palette, TagTone tone, bool filled)
{
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
               "border-radius: 999px;"
               "padding: 4px 10px;"
               "font-size: 12px;"
               "font-weight: 700;"
               "}")
        .arg(backgroundColor, textColor, borderColor);
}

QString AverraStyleHelper::sectionHeaderTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraSectionHeaderTitle {"
               "color: %1;"
               "font-size: 16px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::sectionHeaderSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraSectionHeaderSubtitle {"
               "color: %1;"
               "font-size: 13px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::sectionHeaderMetaLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraSectionHeaderMeta {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.accentColor()));
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
    return QStringLiteral(
               "QFrame#AverraEmptyStateRoot {"
               "background-color: %1;"
               "border: 1px dashed %2;"
               "border-radius: 18px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::emptyStateIconLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraEmptyStateIcon {"
               "color: %1;"
               "font-size: 28px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.accentColor()));
}

QString AverraStyleHelper::emptyStateTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraEmptyStateTitle {"
               "color: %1;"
               "font-size: 16px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::emptyStateDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraEmptyStateDescription {"
               "color: %1;"
               "font-size: 13px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::statisticCardFrameStyleSheet(const AverraThemePalette &palette, bool accent)
{
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
               "border-radius: 16px;"
               "}")
        .arg(backgroundColor, borderColor);
}

QString AverraStyleHelper::statisticCardCaptionLabelStyleSheet(const AverraThemePalette &palette, bool accent)
{
    QString textColor;

    if (accent) {
        textColor = colorToCss(palette.accentColor());
    } else {
        textColor = colorToCss(palette.textSecondaryColor());
    }

    return QStringLiteral(
               "QLabel#AverraStatisticCardCaption {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(textColor);
}

QString AverraStyleHelper::statisticCardValueLabelStyleSheet(const AverraThemePalette &palette, bool accent)
{
    QString textColor;

    if (accent) {
        textColor = colorToCss(palette.accentColor());
    } else {
        textColor = colorToCss(palette.textPrimaryColor());
    }

    return QStringLiteral(
               "QLabel#AverraStatisticCardValue {"
               "color: %1;"
               "font-size: 28px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(textColor);
}

QString AverraStyleHelper::statisticCardHelperLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraStatisticCardHelper {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::statisticCardBadgeLabelStyleSheet(const AverraThemePalette &palette, bool accent)
{
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
               "border-radius: 999px;"
               "padding: 4px 8px;"
               "font-size: 11px;"
               "font-weight: 700;"
               "}")
        .arg(backgroundColor, textColor);
}

QString AverraStyleHelper::toolbarFrameStyleSheet(const AverraThemePalette &palette, bool accent)
{
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
               "border-radius: 16px;"
               "}")
        .arg(backgroundColor, borderColor);
}

QString AverraStyleHelper::toolbarTitleLabelStyleSheet(const AverraThemePalette &palette, bool accent)
{
    QString textColor;

    if (accent) {
        textColor = colorToCss(palette.accentColor());
    } else {
        textColor = colorToCss(palette.textPrimaryColor());
    }

    return QStringLiteral(
               "QLabel#AverraToolbarTitle {"
               "color: %1;"
               "font-size: 15px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(textColor);
}

QString AverraStyleHelper::toolbarSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraToolbarSubtitle {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::paginationButtonStyleSheet(const AverraThemePalette &palette, bool current)
{
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
               "border-radius: 10px;"
               "padding: 7px 10px;"
               "font-size: 12px;"
               "font-weight: 700;"
               "min-width: 34px;"
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
             disabledBorderColor);
}

QString AverraStyleHelper::paginationLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraPaginationLabel {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 600;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::propertyItemFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraPropertyItemRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 14px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::propertyItemTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraPropertyItemTitle {"
               "color: %1;"
               "font-size: 13px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::propertyItemDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraPropertyItemDescription {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::formSectionFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraFormSectionRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 18px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::settingsPageFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraSettingsPageRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 20px;"
               "}")
        .arg(colorToCss(palette.surfaceColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::settingsPageTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraSettingsPageTitle {"
               "color: %1;"
               "font-size: 18px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::settingsPageDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraSettingsPageDescription {"
               "color: %1;"
               "font-size: 13px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::dataTableFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraDataTableRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 16px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::dataTableTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraDataTableTitle {"
               "color: %1;"
               "font-size: 14px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::dataTableSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraDataTableSubtitle {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::dataTableViewStyleSheet(const AverraThemePalette &palette)
{
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
               "padding: 8px;"
               "font-size: 12px;"
               "font-weight: 700;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()),
             colorToCss(palette.accentColor()),
             colorToCss(palette.surfaceColor()),
             colorToCss(palette.textPrimaryColor()));
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
    return QStringLiteral(
               "QPushButton {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "border-radius: 8px;"
               "padding: 4px 8px;"
               "font-size: 11px;"
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
             colorToCss(palette.textDisabledColor()));
}

QString AverraStyleHelper::tabButtonStyleSheet(const AverraThemePalette &palette, bool current)
{
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
               "border-radius: 10px;"
               "padding: 8px 14px;"
               "font-size: 12px;"
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
             pressedBackgroundColor);
}

QString AverraStyleHelper::segmentedButtonStyleSheet(const AverraThemePalette &palette, bool current, bool first, bool last)
{
    QString backgroundColor;
    QString borderColor;
    QString textColor;
    QString hoverBackgroundColor;
    QString hoverBorderColor;
    QString pressedBackgroundColor;
    QString radiusLeft = first ? QStringLiteral("10px") : QStringLiteral("0px");
    QString radiusRight = last ? QStringLiteral("10px") : QStringLiteral("0px");

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
               "padding: 8px 12px;"
               "font-size: 12px;"
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
             pressedBackgroundColor);
}

QString AverraStyleHelper::dialogFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraDialogRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 0px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::dialogTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraDialogTitle {"
               "color: %1;"
               "font-size: 18px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::dialogDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraDialogDescription {"
               "color: %1;"
               "font-size: 13px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::toastFrameStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);
    return QStringLiteral(
               "QFrame#AverraToastRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 14px;"
               "}")
        .arg(colorToCss(colors.second), colorToCss(colors.first));
}

QString AverraStyleHelper::toastTitleLabelStyleSheet(const AverraThemePalette &palette, InfoBarType type)
{
    const QPair<QColor, QColor> colors = infoBarColors(palette, type);
    return QStringLiteral(
               "QLabel#AverraToastTitle {"
               "color: %1;"
               "font-size: 13px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(colors.first));
}

QString AverraStyleHelper::toastDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraToastDescription {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::ribbonBarFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraRibbonBarRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 18px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::ribbonBarTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraRibbonBarTitle {"
               "color: %1;"
               "font-size: 18px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::ribbonBarSubtitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraRibbonBarSubtitle {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::ribbonBarTabButtonStyleSheet(const AverraThemePalette &palette, bool current)
{
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
               "border-radius: 10px;"
               "padding: 8px 12px;"
               "font-size: 12px;"
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
             colorToCss(current ? palette.accentHoverColor() : palette.borderHoverColor()));
}

QString AverraStyleHelper::ribbonBarGroupFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraRibbonGroupRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 14px;"
               "}")
        .arg(colorToCss(palette.surfaceColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::ribbonBarGroupTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraRibbonGroupTitle {"
               "color: %1;"
               "font-size: 11px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::treeViewFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraTreeViewRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 16px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::treeViewTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraTreeViewTitle {"
               "color: %1;"
               "font-size: 14px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::treeViewViewStyleSheet(const AverraThemePalette &palette)
{
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
               "padding: 8px;"
               "font-size: 12px;"
               "font-weight: 700;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.surfaceColor()),
             colorToCss(palette.accentColor()),
             colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::dockFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraDockRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 16px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::dockTitleBarFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraDockTitleBar {"
               "background-color: %1;"
               "border-bottom: 1px solid %2;"
               "border-top-left-radius: 16px;"
               "border-top-right-radius: 16px;"
               "}")
        .arg(colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::dockTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraDockTitle {"
               "color: %1;"
               "font-size: 14px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::dockDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraDockDescription {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::dockCloseButtonStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QToolButton#AverraDockCloseButton {"
               "background-color: transparent;"
               "border: none;"
               "color: %1;"
               "border-radius: 8px;"
               "padding: 4px;"
               "}"
               "QToolButton#AverraDockCloseButton:hover {"
               "background-color: %2;"
               "}"
               "QToolButton#AverraDockCloseButton:pressed {"
               "background-color: %3;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()),
             colorToCss(palette.surfaceHoverColor()),
             colorToCss(palette.surfacePressedColor()));
}

QString AverraStyleHelper::drawerFrameStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QFrame#AverraDrawerRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 18px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.borderColor()));
}

QString AverraStyleHelper::drawerTitleLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraDrawerTitle {"
               "color: %1;"
               "font-size: 16px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textPrimaryColor()));
}

QString AverraStyleHelper::drawerDescriptionLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraDrawerDescription {"
               "color: %1;"
               "font-size: 13px;"
               "font-weight: 500;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::numberRangeBoxFrameStyleSheet(const AverraThemePalette &palette, bool accentFrame)
{
    QString borderColor = accentFrame ? colorToCss(palette.accentColor())
                                      : colorToCss(palette.borderColor());

    return QStringLiteral(
               "QFrame#AverraNumberRangeBoxRoot {"
               "background-color: %1;"
               "border: 1px solid %2;"
               "border-radius: 12px;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()), borderColor);
}

QString AverraStyleHelper::numberRangeBoxLabelStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QLabel#AverraNumberRangeBoxLabel {"
               "color: %1;"
               "font-size: 12px;"
               "font-weight: 700;"
               "background: transparent;"
               "}")
        .arg(colorToCss(palette.textSecondaryColor()));
}

QString AverraStyleHelper::contextMenuStyleSheet(const AverraThemePalette &palette)
{
    return QStringLiteral(
               "QMenu {"
               "background-color: %1;"
               "color: %2;"
               "border: 1px solid %3;"
               "padding: 6px;"
               "}"
               "QMenu::item {"
               "padding: 8px 20px;"
               "border-radius: 8px;"
               "}"
               "QMenu::item:selected {"
               "background-color: %4;"
               "color: #FFFFFF;"
               "}")
        .arg(colorToCss(palette.surfaceRaisedColor()),
             colorToCss(palette.textPrimaryColor()),
             colorToCss(palette.borderColor()),
             colorToCss(palette.accentColor()));
}
