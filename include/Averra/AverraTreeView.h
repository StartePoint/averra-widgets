#ifndef AVERRA_TREE_VIEW_H
#define AVERRA_TREE_VIEW_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QStandardItem;
class QStandardItemModel;
class QTreeView;

class AVERRA_EXPORT AverraTreeView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    explicit AverraTreeView(QWidget *parent = nullptr);
    explicit AverraTreeView(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QTreeView *treeView() const;
    QStandardItemModel *model() const;
    void setHeaders(const QStringList &headers);
    QStandardItem *addTopLevelItem(const QStringList &values);
    QStandardItem *addChildItem(QStandardItem *parent, const QStringList &values);
    void clearItems();

signals:
    void titleChanged(const QString &title);

private slots:
    void initialize();
    void refreshStyle();

private:
    QString m_title;
    QLabel *m_titleLabel;
    QTreeView *m_treeView;
    QStandardItemModel *m_model;
};

#endif // AVERRA_TREE_VIEW_H

