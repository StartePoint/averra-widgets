#ifndef AVERRA_CONTEXT_MENU_H
#define AVERRA_CONTEXT_MENU_H

#include <Averra/AverraGlobal.h>

#include <QMenu>

class AVERRA_EXPORT AverraContextMenu : public QMenu
{
    Q_OBJECT

public:
    explicit AverraContextMenu(QWidget *parent = nullptr);

private slots:
    void refreshStyle();
};

#endif // AVERRA_CONTEXT_MENU_H

