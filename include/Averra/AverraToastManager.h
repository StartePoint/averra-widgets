#ifndef AVERRA_TOAST_MANAGER_H
#define AVERRA_TOAST_MANAGER_H

#include <Averra/AverraGlobal.h>
#include <Averra/AverraToast.h>

#include <QObject>

class QWidget;

class AVERRA_EXPORT AverraToastManager : public QObject
{
    Q_OBJECT

public:
    static AverraToastManager *instance();

    AverraToast *showToast(QWidget *parent,
                           const QString &title,
                           const QString &description,
                           AverraToast::Type type = AverraToast::Info);

private:
    explicit AverraToastManager(QObject *parent = nullptr);
};

#endif // AVERRA_TOAST_MANAGER_H

