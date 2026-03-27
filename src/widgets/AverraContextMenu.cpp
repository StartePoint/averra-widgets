#include <Averra/AverraContextMenu.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

AverraContextMenu::AverraContextMenu(QWidget *parent)
    : QMenu(parent)
{
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraContextMenu::refreshStyle);

    refreshStyle();
}

void AverraContextMenu::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::contextMenuStyleSheet(palette));
}

