#include <Averra/AverraToastManager.h>

#include <QPoint>
#include <QWidget>

AverraToastManager *AverraToastManager::instance()
{
    static AverraToastManager *manager = nullptr;

    if (manager == nullptr) {
        manager = new AverraToastManager;
    }

    return manager;
}

AverraToastManager::AverraToastManager(QObject *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }
}

AverraToast *AverraToastManager::showToast(QWidget *parent,
                                           const QString &title,
                                           const QString &description,
                                           AverraToast::Type type)
{
    AverraToast *toast = new AverraToast(parent);
    toast->setTitle(title);
    toast->setDescription(description);
    toast->setType(type);

    if (parent != nullptr) {
        const int x = parent->width() - toast->sizeHint().width() - 20;
        const int y = 20;
        toast->move(x, y);
    }

    toast->show();
    toast->startAutoClose();
    return toast;
}
