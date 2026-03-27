#include <Averra/AverraDialogManager.h>
#include <Averra/AverraDialog.h>

#include <QApplication>
#include <QList>
#include <QPointer>
#include <QScreen>
#include <QWidget>

namespace
{
QList<QPointer<AverraDialog> > &managedDialogs()
{
    static QList<QPointer<AverraDialog> > dialogs;
    return dialogs;
}

void compactManagedDialogs()
{
    QList<QPointer<AverraDialog> > &dialogs = managedDialogs();

    for (int index = dialogs.count() - 1; index >= 0; --index) {
        if (dialogs.at(index).isNull()) {
            dialogs.removeAt(index);
        }
    }
}

QWidget *resolveAnchorWidget(AverraDialog *dialog)
{
    if (dialog == nullptr) {
        return nullptr;
    }

    if (dialog->parentWidget() != nullptr) {
        return dialog->parentWidget()->window();
    }

    return QApplication::activeWindow();
}

void centerDialog(AverraDialog *dialog, QWidget *anchor)
{
    if (dialog == nullptr) {
        return;
    }

    QRect targetGeometry;

    if (anchor != nullptr) {
        targetGeometry = anchor->frameGeometry();
    } else if (dialog->screen() != nullptr) {
        targetGeometry = dialog->screen()->availableGeometry();
    } else if (QApplication::primaryScreen() != nullptr) {
        targetGeometry = QApplication::primaryScreen()->availableGeometry();
    }

    if (!targetGeometry.isValid()) {
        return;
    }

    dialog->adjustSize();

    const QSize dialogSize = dialog->frameSize().isValid() ? dialog->frameSize() : dialog->sizeHint();
    QPoint position = targetGeometry.center() - QPoint(dialogSize.width() / 2, dialogSize.height() / 2);

    if (position.x() < targetGeometry.left()) {
        position.setX(targetGeometry.left());
    }

    if (position.y() < targetGeometry.top()) {
        position.setY(targetGeometry.top());
    }

    const int maxX = targetGeometry.right() - dialogSize.width() + 1;
    const int maxY = targetGeometry.bottom() - dialogSize.height() + 1;

    if (position.x() > maxX) {
        position.setX(maxX);
    }

    if (position.y() > maxY) {
        position.setY(maxY);
    }

    dialog->move(position);
}
}

AverraDialogManager *AverraDialogManager::instance()
{
    static AverraDialogManager *manager = nullptr;

    if (manager == nullptr) {
        manager = new AverraDialogManager;
    }

    return manager;
}

AverraDialogManager::AverraDialogManager(QObject *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }
}

AverraDialog *AverraDialogManager::showDialog(AverraDialog *dialog)
{
    if (dialog == nullptr) {
        return nullptr;
    }

    compactManagedDialogs();

    if (!managedDialogs().contains(dialog)) {
        managedDialogs().append(QPointer<AverraDialog>(dialog));

        connect(dialog,
                &QObject::destroyed,
                this,
                []() {
                    compactManagedDialogs();
                });
    }

    QWidget *anchor = resolveAnchorWidget(dialog);

    dialog->setWindowFlag(Qt::Dialog, true);

    if (anchor != nullptr) {
        dialog->setWindowModality(Qt::WindowModal);
    } else {
        dialog->setWindowModality(Qt::ApplicationModal);
    }

    centerDialog(dialog, anchor);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();

    emit dialogShown(dialog);
    return dialog;
}

void AverraDialogManager::closeAll()
{
    compactManagedDialogs();
    const QList<QPointer<AverraDialog> > dialogs = managedDialogs();

    for (int index = 0; index < dialogs.size(); ++index) {
        if (!dialogs.at(index).isNull()) {
            dialogs.at(index)->close();
        }
    }

    compactManagedDialogs();
}
