#ifndef AVERRA_STYLE_HELPER_H
#define AVERRA_STYLE_HELPER_H

#include <Averra/AverraTheme.h>

#include <QPair>
#include <QString>

namespace AverraStyleHelper
{
enum class InfoBarType
{
    Info,
    Success,
    Warning,
    Error
};

enum class TagTone
{
    Neutral,
    Accent,
    Success,
    Warning,
    Error
};

QString buttonStyleSheet(const AverraThemePalette &palette, bool accent);
QString badgeButtonStyleSheet(const AverraThemePalette &palette, bool accent);
QString badgeButtonBadgeStyleSheet(const AverraThemePalette &palette, bool accent);
QString filterChipStyleSheet(const AverraThemePalette &palette, bool selected);
QString lineEditStyleSheet(const AverraThemePalette &palette, bool accentFrame);
QString textAreaStyleSheet(const AverraThemePalette &palette, bool accentFrame);
QString comboBoxStyleSheet(const AverraThemePalette &palette, bool accentFrame);
QString numberBoxStyleSheet(const AverraThemePalette &palette, bool accentFrame);
QString dateEditStyleSheet(const AverraThemePalette &palette, bool accentFrame);
QString calendarWidgetStyleSheet(const AverraThemePalette &palette, bool accentFrame);
QString searchBarFrameStyleSheet(const AverraThemePalette &palette, bool accentFrame);
QString searchBarLineEditStyleSheet(const AverraThemePalette &palette);
QString searchBarButtonStyleSheet(const AverraThemePalette &palette, bool accentFrame);
QString cardFrameStyleSheet(const AverraThemePalette &palette, bool accentHeader);
QString cardTitleLabelStyleSheet(const AverraThemePalette &palette, bool accentHeader);
QString cardSubtitleLabelStyleSheet(const AverraThemePalette &palette);
QString cardContentWidgetStyleSheet(const AverraThemePalette &palette);
QString infoBarFrameStyleSheet(const AverraThemePalette &palette, InfoBarType type);
QString infoBarBadgeStyleSheet(const AverraThemePalette &palette, InfoBarType type);
QString infoBarTitleLabelStyleSheet(const AverraThemePalette &palette, InfoBarType type);
QString infoBarDescriptionLabelStyleSheet(const AverraThemePalette &palette);
QString infoBarCloseButtonStyleSheet(const AverraThemePalette &palette, InfoBarType type);
QString navigationPanelFrameStyleSheet(const AverraThemePalette &palette);
QString navigationPanelTitleLabelStyleSheet(const AverraThemePalette &palette);
QString navigationPanelItemButtonStyleSheet(const AverraThemePalette &palette, bool current);
QString tagStyleSheet(const AverraThemePalette &palette, TagTone tone, bool filled);
QString sectionHeaderTitleLabelStyleSheet(const AverraThemePalette &palette);
QString sectionHeaderSubtitleLabelStyleSheet(const AverraThemePalette &palette);
QString sectionHeaderMetaLabelStyleSheet(const AverraThemePalette &palette);
QString sectionHeaderDividerStyleSheet(const AverraThemePalette &palette);
QString emptyStateFrameStyleSheet(const AverraThemePalette &palette);
QString emptyStateIconLabelStyleSheet(const AverraThemePalette &palette);
QString emptyStateTitleLabelStyleSheet(const AverraThemePalette &palette);
QString emptyStateDescriptionLabelStyleSheet(const AverraThemePalette &palette);
QString statisticCardFrameStyleSheet(const AverraThemePalette &palette, bool accent);
QString statisticCardCaptionLabelStyleSheet(const AverraThemePalette &palette, bool accent);
QString statisticCardValueLabelStyleSheet(const AverraThemePalette &palette, bool accent);
QString statisticCardHelperLabelStyleSheet(const AverraThemePalette &palette);
QString statisticCardBadgeLabelStyleSheet(const AverraThemePalette &palette, bool accent);
QString toolbarFrameStyleSheet(const AverraThemePalette &palette, bool accent);
QString toolbarTitleLabelStyleSheet(const AverraThemePalette &palette, bool accent);
QString toolbarSubtitleLabelStyleSheet(const AverraThemePalette &palette);
QString paginationButtonStyleSheet(const AverraThemePalette &palette, bool current);
QString paginationLabelStyleSheet(const AverraThemePalette &palette);
QString propertyItemFrameStyleSheet(const AverraThemePalette &palette);
QString propertyItemTitleLabelStyleSheet(const AverraThemePalette &palette);
QString propertyItemDescriptionLabelStyleSheet(const AverraThemePalette &palette);
QString formSectionFrameStyleSheet(const AverraThemePalette &palette);
QString settingsPageFrameStyleSheet(const AverraThemePalette &palette);
QString settingsPageTitleLabelStyleSheet(const AverraThemePalette &palette);
QString settingsPageDescriptionLabelStyleSheet(const AverraThemePalette &palette);
QString dataTableFrameStyleSheet(const AverraThemePalette &palette);
QString dataTableTitleLabelStyleSheet(const AverraThemePalette &palette);
QString dataTableSubtitleLabelStyleSheet(const AverraThemePalette &palette);
QString dataTableViewStyleSheet(const AverraThemePalette &palette);
QString dataTableToolbarFrameStyleSheet(const AverraThemePalette &palette);
QString dataTableActionButtonStyleSheet(const AverraThemePalette &palette);
QString tabButtonStyleSheet(const AverraThemePalette &palette, bool current);
QString segmentedButtonStyleSheet(const AverraThemePalette &palette, bool current, bool first, bool last);
QString dialogFrameStyleSheet(const AverraThemePalette &palette);
QString dialogTitleLabelStyleSheet(const AverraThemePalette &palette);
QString dialogDescriptionLabelStyleSheet(const AverraThemePalette &palette);
QString toastFrameStyleSheet(const AverraThemePalette &palette, InfoBarType type);
QString toastTitleLabelStyleSheet(const AverraThemePalette &palette, InfoBarType type);
QString toastDescriptionLabelStyleSheet(const AverraThemePalette &palette);
QString ribbonBarFrameStyleSheet(const AverraThemePalette &palette);
QString ribbonBarTitleLabelStyleSheet(const AverraThemePalette &palette);
QString ribbonBarSubtitleLabelStyleSheet(const AverraThemePalette &palette);
QString ribbonBarTabButtonStyleSheet(const AverraThemePalette &palette, bool current);
QString ribbonBarGroupFrameStyleSheet(const AverraThemePalette &palette);
QString ribbonBarGroupTitleLabelStyleSheet(const AverraThemePalette &palette);
QString treeViewFrameStyleSheet(const AverraThemePalette &palette);
QString treeViewTitleLabelStyleSheet(const AverraThemePalette &palette);
QString treeViewViewStyleSheet(const AverraThemePalette &palette);
QString dockFrameStyleSheet(const AverraThemePalette &palette);
QString dockTitleBarFrameStyleSheet(const AverraThemePalette &palette);
QString dockTitleLabelStyleSheet(const AverraThemePalette &palette);
QString dockDescriptionLabelStyleSheet(const AverraThemePalette &palette);
QString dockCloseButtonStyleSheet(const AverraThemePalette &palette);
QString drawerFrameStyleSheet(const AverraThemePalette &palette);
QString drawerTitleLabelStyleSheet(const AverraThemePalette &palette);
QString drawerDescriptionLabelStyleSheet(const AverraThemePalette &palette);
QString numberRangeBoxFrameStyleSheet(const AverraThemePalette &palette, bool accentFrame);
QString numberRangeBoxLabelStyleSheet(const AverraThemePalette &palette);
QString contextMenuStyleSheet(const AverraThemePalette &palette);
}

#endif // AVERRA_STYLE_HELPER_H
