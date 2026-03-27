#ifndef AVERRA_DIALOG_MANAGER_H
#define AVERRA_DIALOG_MANAGER_H

#include <Averra/AverraGlobal.h>

#include <QObject>

class AverraDialog;

class AVERRA_EXPORT AverraDialogManager : public QObject
{
    Q_OBJECT

public:
    static AverraDialogManager *instance();

    AverraDialog *showDialog(AverraDialog *dialog);
    void closeAll();

signals:
    void dialogShown(AverraDialog *dialog);

private:
    explicit AverraDialogManager(QObject *parent = nullptr);
};

#endif // AVERRA_DIALOG_MANAGER_H

