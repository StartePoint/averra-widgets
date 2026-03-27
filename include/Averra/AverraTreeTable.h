#ifndef AVERRA_TREE_TABLE_H
#define AVERRA_TREE_TABLE_H

#include <Averra/AverraGlobal.h>

#include <QSet>
#include <QVariantMap>
#include <QWidget>

class AverraEmptyState;
class AverraSearchBar;
class AverraToolbar;
class QSettings;
class QCheckBox;
class QFrame;
class QPushButton;
class QStandardItem;
class QStandardItemModel;
class QTreeView;
class QVBoxLayout;

class AVERRA_EXPORT AverraTreeTable : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    Q_PROPERTY(bool searchEnabled READ searchEnabled WRITE setSearchEnabled NOTIFY searchEnabledChanged)
    Q_PROPERTY(bool sortingEnabled READ sortingEnabled WRITE setSortingEnabled NOTIFY sortingEnabledChanged)
    Q_PROPERTY(bool rowActionsEnabled READ rowActionsEnabled WRITE setRowActionsEnabled NOTIFY rowActionsEnabledChanged)
    Q_PROPERTY(QString actionText READ actionText WRITE setActionText NOTIFY actionTextChanged)
    Q_PROPERTY(bool expansionStatePersistenceEnabled READ expansionStatePersistenceEnabled WRITE setExpansionStatePersistenceEnabled NOTIFY expansionStatePersistenceEnabledChanged)
    Q_PROPERTY(bool columnConfigEnabled READ columnConfigEnabled WRITE setColumnConfigEnabled NOTIFY columnConfigEnabledChanged)
    Q_PROPERTY(bool columnConfigVisible READ columnConfigVisible WRITE setColumnConfigVisible NOTIFY columnConfigVisibleChanged)
    Q_PROPERTY(QString settingsKey READ settingsKey WRITE setSettingsKey NOTIFY settingsKeyChanged)
    Q_PROPERTY(QString currentItemPath READ currentItemPath NOTIFY currentItemPathChanged)
    Q_PROPERTY(QString currentObjectId READ currentObjectId NOTIFY currentObjectIdChanged)
    Q_PROPERTY(QVariantMap currentRowData READ currentRowData NOTIFY currentRowDataChanged)

public:
    explicit AverraTreeTable(QWidget *parent = nullptr);
    explicit AverraTreeTable(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString subtitle() const;
    void setSubtitle(const QString &subtitle);

    QString filterText() const;
    void setFilterText(const QString &filterText);

    bool searchEnabled() const;
    void setSearchEnabled(bool searchEnabled);

    bool sortingEnabled() const;
    void setSortingEnabled(bool sortingEnabled);

    bool rowActionsEnabled() const;
    void setRowActionsEnabled(bool rowActionsEnabled);

    QString actionText() const;
    void setActionText(const QString &actionText);

    bool expansionStatePersistenceEnabled() const;
    void setExpansionStatePersistenceEnabled(bool enabled);

    bool columnConfigEnabled() const;
    void setColumnConfigEnabled(bool enabled);

    bool columnConfigVisible() const;
    void setColumnConfigVisible(bool visible);

    QString settingsKey() const;
    void setSettingsKey(const QString &settingsKey);

    AverraToolbar *toolbar() const;
    AverraSearchBar *searchBar() const;
    AverraEmptyState *emptyState() const;
    QWidget *columnConfigPanel() const;
    QTreeView *treeView() const;
    QStandardItemModel *model() const;
    void setHeaders(const QStringList &headers);
    QStandardItem *addTopLevelItem(const QStringList &values,
                                   const QString &objectId = QString(),
                                   const QVariantMap &contextData = QVariantMap());
    QStandardItem *addChildItem(QStandardItem *parent,
                                const QStringList &values,
                                const QString &objectId = QString(),
                                const QVariantMap &contextData = QVariantMap());
    void setItemObjectId(QStandardItem *item, const QString &objectId);
    QString itemObjectId(const QStandardItem *item) const;
    void setItemContextData(QStandardItem *item, const QVariantMap &contextData);
    QVariantMap itemContextData(const QStandardItem *item) const;
    QString currentItemPath() const;
    QString currentObjectId() const;
    QVariantMap currentRowData() const;
    QVariantMap currentContextData() const;
    void clearItems();
    void expandAllItems();
    void collapseAllItems();
    QString exportViewState() const;
    bool importViewState(const QString &viewState);
    bool saveViewState(QSettings &settings) const;
    bool restoreViewState(QSettings &settings);

signals:
    void titleChanged(const QString &title);
    void subtitleChanged(const QString &subtitle);
    void filterTextChanged(const QString &filterText);
    void searchEnabledChanged(bool searchEnabled);
    void sortingEnabledChanged(bool sortingEnabled);
    void rowActionsEnabledChanged(bool rowActionsEnabled);
    void actionTextChanged(const QString &actionText);
    void expansionStatePersistenceEnabledChanged(bool enabled);
    void columnConfigEnabledChanged(bool enabled);
    void columnConfigVisibleChanged(bool visible);
    void settingsKeyChanged(const QString &settingsKey);
    void currentItemPathChanged(const QString &itemPath);
    void currentObjectIdChanged(const QString &objectId);
    void currentRowDataChanged(const QVariantMap &rowData);
    void selectionContextChanged(const QString &itemPath, const QVariantMap &rowData, const QString &objectId);
    void rowActionTriggered(const QString &itemPath);

private slots:
    void initialize();
    void refreshStyle();
    void applyFilter();
    void updateEmptyState();
    void handleTreeClicked(const QModelIndex &index);
    void handleCurrentItemChanged(const QModelIndex &current, const QModelIndex &previous);
    void handleItemExpanded(const QModelIndex &index);
    void handleItemCollapsed(const QModelIndex &index);
    void handleSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void handleSectionResized(int logicalIndex, int oldSize, int newSize);
    void handleColumnConfigToggled(bool checked);
    void handleColumnVisibilityChanged(bool checked);

private:
    bool applyFilterToItem(QStandardItem *item, const QModelIndex &parentIndex, const QString &pattern);
    bool rowMatches(QStandardItem *item, const QString &pattern) const;
    void rebuildHeaders();
    void rebuildActionCells();
    void rebuildColumnConfigPanel();
    void populateRowMetadata(QList<QStandardItem *> &rowItems,
                             const QString &objectId,
                             const QVariantMap &contextData);
    QString itemPath(QStandardItem *item) const;
    QVariantMap rowDataForItem(QStandardItem *item) const;
    void updateSelectionContext(QStandardItem *item);
    void restoreExpansionState();
    void updateColumnConfigState();
    void updateSearchVisibility();

    QString m_title;
    QString m_subtitle;
    QString m_filterText;
    QString m_actionText;
    QString m_pendingViewState;
    QString m_settingsKey;
    QString m_currentItemPath;
    QString m_currentObjectId;
    QStringList m_headers;
    QVariantMap m_currentRowData;
    QVariantMap m_currentContextData;
    bool m_searchEnabled;
    bool m_sortingEnabled;
    bool m_rowActionsEnabled;
    bool m_expansionStatePersistenceEnabled;
    bool m_columnConfigEnabled;
    bool m_columnConfigVisible;
    AverraToolbar *m_toolbar;
    AverraSearchBar *m_searchBar;
    QPushButton *m_columnConfigButton;
    QFrame *m_columnConfigPanel;
    QVBoxLayout *m_columnConfigLayout;
    QList<QCheckBox *> m_columnVisibilityChecks;
    QTreeView *m_treeView;
    QStandardItemModel *m_model;
    AverraEmptyState *m_emptyState;
    QSet<QString> m_expandedItemPaths;
};

#endif // AVERRA_TREE_TABLE_H
