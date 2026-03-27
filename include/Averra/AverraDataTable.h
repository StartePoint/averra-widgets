#ifndef AVERRA_DATA_TABLE_H
#define AVERRA_DATA_TABLE_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class AverraEmptyState;
class AverraSearchBar;
class AverraToolbar;
class QLabel;
class QPushButton;
class QSortFilterProxyModel;
class QStandardItemModel;
class QTableView;

class AVERRA_EXPORT AverraDataTable : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(bool sortingEnabled READ sortingEnabled WRITE setSortingEnabled NOTIFY sortingEnabledChanged)
    Q_PROPERTY(bool rowActionsEnabled READ rowActionsEnabled WRITE setRowActionsEnabled NOTIFY rowActionsEnabledChanged)
    Q_PROPERTY(QString actionText READ actionText WRITE setActionText NOTIFY actionTextChanged)

public:
    explicit AverraDataTable(QWidget *parent = nullptr);
    explicit AverraDataTable(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString subtitle() const;
    void setSubtitle(const QString &subtitle);

    bool sortingEnabled() const;
    void setSortingEnabled(bool sortingEnabled);

    bool rowActionsEnabled() const;
    void setRowActionsEnabled(bool rowActionsEnabled);

    QString actionText() const;
    void setActionText(const QString &actionText);

    AverraToolbar *toolbar() const;
    AverraSearchBar *searchBar() const;
    AverraEmptyState *emptyState() const;
    QTableView *tableView() const;
    QStandardItemModel *model() const;
    void setHeaders(const QStringList &headers);
    void addRow(const QStringList &values);
    void clearRows();

signals:
    void titleChanged(const QString &title);
    void subtitleChanged(const QString &subtitle);
    void sortingEnabledChanged(bool sortingEnabled);
    void rowActionsEnabledChanged(bool rowActionsEnabled);
    void actionTextChanged(const QString &actionText);
    void rowActionTriggered(int row);

private slots:
    void initialize();
    void refreshStyle();
    void updateEmptyState();
    void applySearchFilter(const QString &text);
    void handleTableClicked(const QModelIndex &index);

private:
    void rebuildHeaders();
    void rebuildActionCells();

    QString m_title;
    QString m_subtitle;
    QString m_actionText;
    QStringList m_headers;
    bool m_sortingEnabled;
    bool m_rowActionsEnabled;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    AverraToolbar *m_toolbar;
    AverraSearchBar *m_searchBar;
    QPushButton *m_refreshButton;
    QTableView *m_tableView;
    QStandardItemModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
    AverraEmptyState *m_emptyState;
};

#endif // AVERRA_DATA_TABLE_H
