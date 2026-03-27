#include <Averra/AverraTreeTable.h>
#include <Averra/AverraEmptyState.h>
#include <Averra/AverraSearchBar.h>
#include <Averra/AverraThemeManager.h>
#include <Averra/AverraToolbar.h>

#include "../core/AverraStyleHelper.h"

#include <QCheckBox>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QModelIndex>
#include <QPushButton>
#include <QSettings>
#include <QSet>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QFrame>
#include <QVBoxLayout>

namespace
{
constexpr int kActionColumnWidth = 92;
constexpr int kObjectIdRole = Qt::UserRole + 101;
constexpr int kContextDataRole = Qt::UserRole + 102;

bool containsText(const QString &text, const QString &pattern)
{
    if (pattern.isEmpty()) {
        return true;
    }

    return text.contains(pattern, Qt::CaseInsensitive);
}

QString columnConfigTitle(const QStringList &headers, int column)
{
    if (column >= 0 && column < headers.size()) {
        return headers.at(column);
    }

    return QStringLiteral("Column %1").arg(column);
}

QString headerTitleForColumn(QStandardItemModel *model, int column)
{
    if (model == nullptr) {
        return QStringLiteral("column_%1").arg(column);
    }

    QStandardItem *headerItem = model->horizontalHeaderItem(column);

    if (headerItem != nullptr && !headerItem->text().trimmed().isEmpty()) {
        return headerItem->text();
    }

    return QStringLiteral("column_%1").arg(column);
}
}

AverraTreeTable::AverraTreeTable(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraTreeTable::AverraTreeTable(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraTreeTable::title() const
{
    return m_title;
}

void AverraTreeTable::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_toolbar->setTitle(m_title);
    emit titleChanged(m_title);
}

QString AverraTreeTable::subtitle() const
{
    return m_subtitle;
}

void AverraTreeTable::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle) {
        return;
    }

    m_subtitle = subtitle;
    m_toolbar->setSubtitle(m_subtitle);
    emit subtitleChanged(m_subtitle);
}

QString AverraTreeTable::filterText() const
{
    return m_filterText;
}

void AverraTreeTable::setFilterText(const QString &filterText)
{
    if (m_filterText == filterText) {
        return;
    }

    m_filterText = filterText;

    if (m_searchBar->text() != m_filterText) {
        m_searchBar->setText(m_filterText);
    }

    applyFilter();
    emit filterTextChanged(m_filterText);
}

bool AverraTreeTable::searchEnabled() const
{
    return m_searchEnabled;
}

void AverraTreeTable::setSearchEnabled(bool searchEnabled)
{
    if (m_searchEnabled == searchEnabled) {
        return;
    }

    m_searchEnabled = searchEnabled;
    updateSearchVisibility();
    emit searchEnabledChanged(m_searchEnabled);
}

bool AverraTreeTable::sortingEnabled() const
{
    return m_sortingEnabled;
}

void AverraTreeTable::setSortingEnabled(bool sortingEnabled)
{
    if (m_sortingEnabled == sortingEnabled) {
        return;
    }

    m_sortingEnabled = sortingEnabled;
    m_treeView->setSortingEnabled(m_sortingEnabled);
    emit sortingEnabledChanged(m_sortingEnabled);
}

bool AverraTreeTable::rowActionsEnabled() const
{
    return m_rowActionsEnabled;
}

void AverraTreeTable::setRowActionsEnabled(bool rowActionsEnabled)
{
    if (m_rowActionsEnabled == rowActionsEnabled) {
        return;
    }

    m_rowActionsEnabled = rowActionsEnabled;
    rebuildHeaders();
    rebuildActionCells();
    emit rowActionsEnabledChanged(m_rowActionsEnabled);
}

QString AverraTreeTable::actionText() const
{
    return m_actionText;
}

void AverraTreeTable::setActionText(const QString &actionText)
{
    if (m_actionText == actionText) {
        return;
    }

    m_actionText = actionText;
    rebuildActionCells();
    emit actionTextChanged(m_actionText);
}

bool AverraTreeTable::expansionStatePersistenceEnabled() const
{
    return m_expansionStatePersistenceEnabled;
}

void AverraTreeTable::setExpansionStatePersistenceEnabled(bool enabled)
{
    if (m_expansionStatePersistenceEnabled == enabled) {
        return;
    }

    m_expansionStatePersistenceEnabled = enabled;

    if (!m_expansionStatePersistenceEnabled) {
        m_expandedItemPaths.clear();
    } else {
        restoreExpansionState();
    }

    emit expansionStatePersistenceEnabledChanged(m_expansionStatePersistenceEnabled);
}

bool AverraTreeTable::columnConfigEnabled() const
{
    return m_columnConfigEnabled;
}

void AverraTreeTable::setColumnConfigEnabled(bool enabled)
{
    if (m_columnConfigEnabled == enabled) {
        return;
    }

    m_columnConfigEnabled = enabled;
    m_columnConfigButton->setVisible(m_columnConfigEnabled);

    if (!m_columnConfigEnabled) {
        setColumnConfigVisible(false);
    }

    emit columnConfigEnabledChanged(m_columnConfigEnabled);
}

bool AverraTreeTable::columnConfigVisible() const
{
    return m_columnConfigVisible;
}

void AverraTreeTable::setColumnConfigVisible(bool visible)
{
    if (m_columnConfigVisible == visible) {
        return;
    }

    m_columnConfigVisible = visible && m_columnConfigEnabled;
    m_columnConfigPanel->setVisible(m_columnConfigVisible);
    emit columnConfigVisibleChanged(m_columnConfigVisible);
}

QString AverraTreeTable::settingsKey() const
{
    return m_settingsKey;
}

void AverraTreeTable::setSettingsKey(const QString &settingsKey)
{
    if (m_settingsKey == settingsKey) {
        return;
    }

    m_settingsKey = settingsKey;
    emit settingsKeyChanged(m_settingsKey);
}

AverraToolbar *AverraTreeTable::toolbar() const
{
    return m_toolbar;
}

AverraSearchBar *AverraTreeTable::searchBar() const
{
    return m_searchBar;
}

AverraEmptyState *AverraTreeTable::emptyState() const
{
    return m_emptyState;
}

QWidget *AverraTreeTable::columnConfigPanel() const
{
    return m_columnConfigPanel;
}

QTreeView *AverraTreeTable::treeView() const
{
    return m_treeView;
}

QStandardItemModel *AverraTreeTable::model() const
{
    return m_model;
}

QString AverraTreeTable::currentItemPath() const
{
    return m_currentItemPath;
}

QString AverraTreeTable::currentObjectId() const
{
    return m_currentObjectId;
}

QVariantMap AverraTreeTable::currentRowData() const
{
    return m_currentRowData;
}

QVariantMap AverraTreeTable::currentContextData() const
{
    return m_currentContextData;
}

void AverraTreeTable::setHeaders(const QStringList &headers)
{
    m_headers = headers;
    rebuildHeaders();
}

QStandardItem *AverraTreeTable::addTopLevelItem(const QStringList &values,
                                                const QString &objectId,
                                                const QVariantMap &contextData)
{
    const int columnCount = m_headers.isEmpty() ? values.size() : m_headers.size();
    QList<QStandardItem *> rowItems;

    if (m_model->columnCount() < columnCount) {
        m_model->setColumnCount(columnCount);
    }

    for (int index = 0; index < columnCount; ++index) {
        const QString value = index < values.size() ? values.at(index) : QString();
        rowItems.append(new QStandardItem(value));
    }

    if (m_rowActionsEnabled) {
        rowItems.append(new QStandardItem(m_actionText));
    }

    populateRowMetadata(rowItems, objectId, contextData);
    m_model->appendRow(rowItems);
    rebuildActionCells();
    applyFilter();
    return rowItems.isEmpty() ? nullptr : rowItems.first();
}

QStandardItem *AverraTreeTable::addChildItem(QStandardItem *parent,
                                             const QStringList &values,
                                             const QString &objectId,
                                             const QVariantMap &contextData)
{
    if (parent == nullptr) {
        return nullptr;
    }

    const int columnCount = m_headers.isEmpty() ? values.size() : m_headers.size();
    QList<QStandardItem *> rowItems;

    for (int index = 0; index < columnCount; ++index) {
        const QString value = index < values.size() ? values.at(index) : QString();
        rowItems.append(new QStandardItem(value));
    }

    if (m_rowActionsEnabled) {
        rowItems.append(new QStandardItem(m_actionText));
    }

    populateRowMetadata(rowItems, objectId, contextData);
    parent->appendRow(rowItems);
    rebuildActionCells();
    applyFilter();
    return rowItems.isEmpty() ? nullptr : rowItems.first();
}

void AverraTreeTable::setItemObjectId(QStandardItem *item, const QString &objectId)
{
    if (item == nullptr) {
        return;
    }

    item->setData(objectId, kObjectIdRole);
}

QString AverraTreeTable::itemObjectId(const QStandardItem *item) const
{
    if (item == nullptr) {
        return QString();
    }

    return item->data(kObjectIdRole).toString();
}

void AverraTreeTable::setItemContextData(QStandardItem *item, const QVariantMap &contextData)
{
    if (item == nullptr) {
        return;
    }

    item->setData(contextData, kContextDataRole);
}

QVariantMap AverraTreeTable::itemContextData(const QStandardItem *item) const
{
    if (item == nullptr) {
        return QVariantMap();
    }

    return item->data(kContextDataRole).toMap();
}

void AverraTreeTable::clearItems()
{
    m_model->removeRows(0, m_model->rowCount());
    m_expandedItemPaths.clear();
    updateSelectionContext(nullptr);
    updateEmptyState();
}

void AverraTreeTable::expandAllItems()
{
    m_treeView->expandAll();
}

void AverraTreeTable::collapseAllItems()
{
    m_treeView->collapseAll();
}

QString AverraTreeTable::exportViewState() const
{
    QJsonObject rootObject;
    QJsonArray columnsArray;
    QJsonArray expandedPathsArray;
    QHeaderView *header = m_treeView->header();

    for (int column = 0; column < m_model->columnCount(); ++column) {
        QJsonObject columnObject;
        columnObject.insert(QStringLiteral("index"), column);
        columnObject.insert(QStringLiteral("title"), columnConfigTitle(m_model->horizontalHeaderItem(column) != nullptr
                                                                           ? QStringList() << m_model->horizontalHeaderItem(column)->text()
                                                                           : QStringList(),
                                                                       0));
        columnObject.insert(QStringLiteral("visible"), !m_treeView->isColumnHidden(column));
        columnObject.insert(QStringLiteral("width"), m_treeView->columnWidth(column));
        columnsArray.append(columnObject);
    }

    rootObject.insert(QStringLiteral("columns"), columnsArray);
    for (auto it = m_expandedItemPaths.constBegin(); it != m_expandedItemPaths.constEnd(); ++it) {
        expandedPathsArray.append(*it);
    }

    rootObject.insert(QStringLiteral("expandedPaths"), expandedPathsArray);
    rootObject.insert(QStringLiteral("filterText"), m_filterText);
    rootObject.insert(QStringLiteral("sortingEnabled"), m_sortingEnabled);
    rootObject.insert(QStringLiteral("sortSection"), header->sortIndicatorSection());
    rootObject.insert(QStringLiteral("sortOrder"), static_cast<int>(header->sortIndicatorOrder()));
    rootObject.insert(QStringLiteral("currentItemPath"), m_currentItemPath);
    return QString::fromUtf8(QJsonDocument(rootObject).toJson(QJsonDocument::Compact));
}

bool AverraTreeTable::importViewState(const QString &viewState)
{
    const QJsonDocument document = QJsonDocument::fromJson(viewState.toUtf8());

    if (!document.isObject()) {
        return false;
    }

    if (m_model->columnCount() == 0) {
        m_pendingViewState = viewState;
        return true;
    }

    const QJsonObject rootObject = document.object();
    const QJsonArray columnsArray = rootObject.value(QStringLiteral("columns")).toArray();
    const QJsonArray expandedPathsArray = rootObject.value(QStringLiteral("expandedPaths")).toArray();

    m_expandedItemPaths.clear();

    for (int index = 0; index < expandedPathsArray.size(); ++index) {
        const QString itemPath = expandedPathsArray.at(index).toString();

        if (!itemPath.trimmed().isEmpty()) {
            m_expandedItemPaths.insert(itemPath);
        }
    }

    for (int index = 0; index < columnsArray.size(); ++index) {
        const QJsonObject columnObject = columnsArray.at(index).toObject();
        const int column = columnObject.value(QStringLiteral("index")).toInt(-1);

        if (column < 0 || column >= m_model->columnCount()) {
            continue;
        }

        const bool visible = columnObject.value(QStringLiteral("visible")).toBool(true);
        const int width = columnObject.value(QStringLiteral("width")).toInt(m_treeView->columnWidth(column));
        m_treeView->setColumnHidden(column, !visible);
        m_treeView->setColumnWidth(column, width);
    }

    if (rootObject.contains(QStringLiteral("sortingEnabled"))) {
        setSortingEnabled(rootObject.value(QStringLiteral("sortingEnabled")).toBool(m_sortingEnabled));
    }

    const int sortSection = rootObject.value(QStringLiteral("sortSection")).toInt(-1);
    const Qt::SortOrder sortOrder = static_cast<Qt::SortOrder>(rootObject.value(QStringLiteral("sortOrder")).toInt(static_cast<int>(Qt::AscendingOrder)));

    if (sortSection >= 0 && sortSection < m_model->columnCount()) {
        m_treeView->sortByColumn(sortSection, sortOrder);
    }

    if (rootObject.contains(QStringLiteral("filterText"))) {
        setFilterText(rootObject.value(QStringLiteral("filterText")).toString());
    } else {
        applyFilter();
    }

    const QString currentItemPath = rootObject.value(QStringLiteral("currentItemPath")).toString();

    if (!currentItemPath.trimmed().isEmpty()) {
        QList<QStandardItem *> stack;

        for (int row = 0; row < m_model->rowCount(); ++row) {
            QStandardItem *item = m_model->item(row, 0);
            if (item != nullptr) {
                stack.append(item);
            }
        }

        while (!stack.isEmpty()) {
            QStandardItem *item = stack.takeFirst();

            if (itemPath(item) == currentItemPath) {
                m_treeView->setCurrentIndex(item->index());
                updateSelectionContext(item);
                break;
            }

            for (int row = 0; row < item->rowCount(); ++row) {
                QStandardItem *child = item->child(row, 0);

                if (child != nullptr) {
                    stack.append(child);
                }
            }
        }
    }

    updateColumnConfigState();
    m_pendingViewState.clear();
    return true;
}

bool AverraTreeTable::saveViewState(QSettings &settings) const
{
    if (m_settingsKey.trimmed().isEmpty()) {
        return false;
    }

    settings.setValue(m_settingsKey, exportViewState());
    return true;
}

bool AverraTreeTable::restoreViewState(QSettings &settings)
{
    if (m_settingsKey.trimmed().isEmpty()) {
        return false;
    }

    const QVariant value = settings.value(m_settingsKey);

    if (!value.isValid()) {
        return false;
    }

    return importViewState(value.toString());
}

void AverraTreeTable::initialize()
{
    m_title.clear();
    m_subtitle.clear();
    m_filterText.clear();
    m_actionText = QStringLiteral("查看");
    m_pendingViewState.clear();
    m_settingsKey = QStringLiteral("AverraTreeTable/viewState");
    m_currentItemPath.clear();
    m_currentObjectId.clear();
    m_currentRowData.clear();
    m_currentContextData.clear();
    m_searchEnabled = true;
    m_sortingEnabled = true;
    m_rowActionsEnabled = true;
    m_expansionStatePersistenceEnabled = true;
    m_columnConfigEnabled = true;
    m_columnConfigVisible = false;
    m_toolbar = new AverraToolbar(this);
    m_searchBar = new AverraSearchBar(this);
    m_columnConfigButton = new QPushButton(QStringLiteral("列配置"), this);
    m_columnConfigPanel = new QFrame(this);
    m_columnConfigLayout = new QVBoxLayout;
    m_treeView = new QTreeView(this);
    m_model = new QStandardItemModel(this);
    m_emptyState = new AverraEmptyState(this);

    m_searchBar->setPlaceholderText(QStringLiteral("搜索树表内容"));
    m_searchBar->setButtonText(QStringLiteral("筛选"));
    m_columnConfigPanel->setObjectName(QStringLiteral("AverraTreeTableColumnConfigPanel"));
    m_columnConfigButton->setVisible(m_columnConfigEnabled);
    m_columnConfigPanel->setVisible(false);

    m_toolbar->addTrailingWidget(m_searchBar);
    m_toolbar->addTrailingWidget(m_columnConfigButton);

    m_emptyState->setTitle(QStringLiteral("暂无树表数据"));
    m_emptyState->setDescription(QStringLiteral("当前树表为空。你可以先添加一条顶级节点或调整筛选条件。"));
    m_emptyState->setIconText(QStringLiteral("T"));

    m_treeView->setModel(m_model);
    m_treeView->setAlternatingRowColors(true);
    m_treeView->setMinimumHeight(240);
    m_treeView->setSortingEnabled(m_sortingEnabled);
    m_treeView->header()->setStretchLastSection(true);
    m_treeView->header()->setSortIndicatorShown(true);

    m_columnConfigLayout->setContentsMargins(12, 12, 12, 12);
    m_columnConfigLayout->setSpacing(8);
    m_columnConfigPanel->setLayout(m_columnConfigLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    layout->addWidget(m_toolbar);
    layout->addWidget(m_columnConfigPanel);
    layout->addWidget(m_treeView);
    layout->addWidget(m_emptyState);

    m_emptyState->setVisible(false);

    connect(m_searchBar,
            &AverraSearchBar::textChanged,
            this,
            &AverraTreeTable::setFilterText);
    connect(m_searchBar,
            &AverraSearchBar::searchRequested,
            this,
            &AverraTreeTable::setFilterText);
    connect(m_columnConfigButton,
            &QPushButton::clicked,
            this,
            [this]() {
                setColumnConfigVisible(!m_columnConfigVisible);
            });
    connect(m_treeView,
            &QTreeView::clicked,
            this,
            &AverraTreeTable::handleTreeClicked);
    connect(m_treeView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &AverraTreeTable::handleCurrentItemChanged);
    connect(m_treeView,
            &QTreeView::expanded,
            this,
            &AverraTreeTable::handleItemExpanded);
    connect(m_treeView,
            &QTreeView::collapsed,
            this,
            &AverraTreeTable::handleItemCollapsed);
    connect(m_treeView->header(),
            &QHeaderView::sortIndicatorChanged,
            this,
            &AverraTreeTable::handleSortIndicatorChanged);
    connect(m_treeView->header(),
            &QHeaderView::sectionResized,
            this,
            &AverraTreeTable::handleSectionResized);
    connect(m_model,
            &QStandardItemModel::rowsInserted,
            this,
            &AverraTreeTable::updateEmptyState);
    connect(m_model,
            &QStandardItemModel::rowsRemoved,
            this,
            &AverraTreeTable::updateEmptyState);
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraTreeTable::refreshStyle);

    rebuildHeaders();
    refreshStyle();
    updateSearchVisibility();
    updateEmptyState();
}

void AverraTreeTable::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    setStyleSheet(AverraStyleHelper::dataTableFrameStyleSheet(palette));
    m_toolbar->setStyleSheet(AverraStyleHelper::dataTableToolbarFrameStyleSheet(palette));
    m_treeView->setStyleSheet(AverraStyleHelper::treeViewViewStyleSheet(palette));
    m_columnConfigButton->setStyleSheet(AverraStyleHelper::dataTableActionButtonStyleSheet(palette));
    m_columnConfigPanel->setStyleSheet(QStringLiteral("QFrame#AverraTreeTableColumnConfigPanel { background-color: %1; border: 1px solid %2; border-radius: %3px; }")
                                           .arg(palette.surfaceRaisedColor().name(),
                                                palette.borderColor().name(),
                                                QString::number(styleProfile.largeRadius())));
    rebuildActionCells();
}

void AverraTreeTable::applyFilter()
{
    const QString pattern = m_filterText.trimmed();
    bool hasVisibleRows = false;

    for (int row = 0; row < m_model->rowCount(); ++row) {
        QStandardItem *item = m_model->item(row, 0);

        if (item == nullptr) {
            continue;
        }

        const bool visible = applyFilterToItem(item, QModelIndex(), pattern);
        hasVisibleRows = hasVisibleRows || visible;
    }

    if (pattern.isEmpty()) {
        restoreExpansionState();
    } else {
        m_treeView->expandAll();
    }

    m_treeView->setVisible(hasVisibleRows);
    m_emptyState->setVisible(!hasVisibleRows);
    updateEmptyState();
}

void AverraTreeTable::updateEmptyState()
{
    if (m_model->rowCount() == 0) {
        m_treeView->setVisible(false);
        m_emptyState->setVisible(true);
        m_emptyState->setTitle(QStringLiteral("暂无树表数据"));
        m_emptyState->setDescription(QStringLiteral("当前树表为空。你可以先添加一条顶级节点或调整筛选条件。"));
        return;
    }

    bool hasVisibleRows = false;

    for (int row = 0; row < m_model->rowCount(); ++row) {
        if (!m_treeView->isRowHidden(row, QModelIndex())) {
            hasVisibleRows = true;
            break;
        }
    }

    if (hasVisibleRows) {
        m_treeView->setVisible(true);
        m_emptyState->setVisible(false);
        return;
    }

    m_treeView->setVisible(false);
    m_emptyState->setVisible(true);

    if (m_filterText.trimmed().isEmpty()) {
        m_emptyState->setTitle(QStringLiteral("暂无树表数据"));
        m_emptyState->setDescription(QStringLiteral("当前树表为空。你可以先添加一条顶级节点或调整筛选条件。"));
    } else {
        m_emptyState->setTitle(QStringLiteral("没有匹配结果"));
        m_emptyState->setDescription(QStringLiteral("当前筛选条件下没有匹配的节点。请尝试修改搜索关键词。"));
    }
}

bool AverraTreeTable::applyFilterToItem(QStandardItem *item, const QModelIndex &parentIndex, const QString &pattern)
{
    if (item == nullptr) {
        return false;
    }

    if (pattern.isEmpty()) {
        m_treeView->setRowHidden(item->row(), parentIndex, false);

        for (int row = 0; row < item->rowCount(); ++row) {
            applyFilterToItem(item->child(row, 0), item->index(), pattern);
        }

        return true;
    }

    bool childVisible = false;

    for (int row = 0; row < item->rowCount(); ++row) {
        childVisible = applyFilterToItem(item->child(row, 0), item->index(), pattern) || childVisible;
    }

    const bool visible = rowMatches(item, pattern) || childVisible;
    m_treeView->setRowHidden(item->row(), parentIndex, !visible);
    return visible;
}

bool AverraTreeTable::rowMatches(QStandardItem *item, const QString &pattern) const
{
    if (item == nullptr || pattern.isEmpty()) {
        return true;
    }

    QStandardItem *parentItem = item->parent();

    for (int column = 0; column < m_model->columnCount(); ++column) {
        QStandardItem *columnItem = parentItem != nullptr ? parentItem->child(item->row(), column)
                                                          : m_model->item(item->row(), column);

        if (columnItem != nullptr && containsText(columnItem->text(), pattern)) {
            return true;
        }
    }

    return false;
}

void AverraTreeTable::rebuildHeaders()
{
    QStringList headers = m_headers;

    if (m_rowActionsEnabled) {
        headers.append(QStringLiteral("操作"));
    }

    m_model->setColumnCount(headers.size());
    m_model->setHorizontalHeaderLabels(headers);
    rebuildColumnConfigPanel();

    if (!m_pendingViewState.isEmpty()) {
        importViewState(m_pendingViewState);
    }
}

void AverraTreeTable::rebuildActionCells()
{
    if (!m_rowActionsEnabled || m_model->columnCount() == 0) {
        return;
    }

    const int actionColumn = m_model->columnCount() - 1;

    QList<QStandardItem *> stack;

    for (int row = 0; row < m_model->rowCount(); ++row) {
        QStandardItem *item = m_model->item(row, 0);
        if (item != nullptr) {
            stack.append(item);
        }
    }

    while (!stack.isEmpty()) {
        QStandardItem *item = stack.takeFirst();
        QStandardItem *parent = item->parent();
        QStandardItem *actionItem = parent != nullptr ? parent->child(item->row(), actionColumn)
                                                      : m_model->item(item->row(), actionColumn);

        if (actionItem == nullptr) {
            actionItem = new QStandardItem;

            if (parent != nullptr) {
                parent->setChild(item->row(), actionColumn, actionItem);
            } else {
                m_model->setItem(item->row(), actionColumn, actionItem);
            }
        }

        actionItem->setText(m_actionText);
        actionItem->setTextAlignment(Qt::AlignCenter);

        for (int row = 0; row < item->rowCount(); ++row) {
            QStandardItem *child = item->child(row, 0);
            if (child != nullptr) {
                stack.append(child);
            }
        }
    }

    m_treeView->setColumnWidth(actionColumn, kActionColumnWidth);
}

void AverraTreeTable::rebuildColumnConfigPanel()
{
    while (m_columnConfigLayout->count() > 0) {
        QLayoutItem *item = m_columnConfigLayout->takeAt(0);

        if (item != nullptr && item->widget() != nullptr) {
            delete item->widget();
        }

        delete item;
    }

    m_columnVisibilityChecks.clear();

    for (int column = 0; column < m_model->columnCount(); ++column) {
        if (m_rowActionsEnabled && column == m_model->columnCount() - 1) {
            continue;
        }

        QCheckBox *checkBox = new QCheckBox(columnConfigTitle(m_headers, column), m_columnConfigPanel);
        checkBox->setProperty("columnIndex", column);
        checkBox->setChecked(!m_treeView->isColumnHidden(column));
        connect(checkBox,
                &QCheckBox::toggled,
                this,
                &AverraTreeTable::handleColumnVisibilityChanged);
        m_columnConfigLayout->addWidget(checkBox);
        m_columnVisibilityChecks.append(checkBox);
    }

    m_columnConfigLayout->addStretch(1);
}

void AverraTreeTable::populateRowMetadata(QList<QStandardItem *> &rowItems,
                                          const QString &objectId,
                                          const QVariantMap &contextData)
{
    if (rowItems.isEmpty()) {
        return;
    }

    rowItems.first()->setData(objectId, kObjectIdRole);
    rowItems.first()->setData(contextData, kContextDataRole);
}

QString AverraTreeTable::itemPath(QStandardItem *item) const
{
    QStringList pathSegments;
    QStandardItem *current = item;

    while (current != nullptr) {
        pathSegments.prepend(current->text());
        current = current->parent();
    }

    return pathSegments.join(QStringLiteral("/"));
}

QVariantMap AverraTreeTable::rowDataForItem(QStandardItem *item) const
{
    QVariantMap rowData;

    if (item == nullptr) {
        return rowData;
    }

    QStandardItem *parentItem = item->parent();
    const int columnCount = m_rowActionsEnabled ? m_model->columnCount() - 1 : m_model->columnCount();

    for (int column = 0; column < columnCount; ++column) {
        QStandardItem *columnItem = parentItem != nullptr ? parentItem->child(item->row(), column)
                                                          : m_model->item(item->row(), column);

        if (columnItem != nullptr) {
            rowData.insert(headerTitleForColumn(m_model, column), columnItem->text());
        }
    }

    return rowData;
}

void AverraTreeTable::updateSelectionContext(QStandardItem *item)
{
    QString itemPathValue;
    QString objectIdValue;
    QVariantMap rowDataValue;
    QVariantMap contextDataValue;

    if (item != nullptr) {
        itemPathValue = itemPath(item);
        objectIdValue = itemObjectId(item);
        rowDataValue = rowDataForItem(item);
        contextDataValue = itemContextData(item);
    }

    const bool changed = m_currentItemPath != itemPathValue
                         || m_currentObjectId != objectIdValue
                         || m_currentRowData != rowDataValue
                         || m_currentContextData != contextDataValue;

    m_currentItemPath = itemPathValue;
    m_currentObjectId = objectIdValue;
    m_currentRowData = rowDataValue;
    m_currentContextData = contextDataValue;

    if (changed) {
        emit currentItemPathChanged(m_currentItemPath);
        emit currentObjectIdChanged(m_currentObjectId);
        emit currentRowDataChanged(m_currentRowData);
        emit selectionContextChanged(m_currentItemPath, m_currentRowData, m_currentObjectId);
    }
}

void AverraTreeTable::restoreExpansionState()
{
    if (!m_expansionStatePersistenceEnabled) {
        m_treeView->collapseAll();
        return;
    }

    for (int row = 0; row < m_model->rowCount(); ++row) {
        QList<QStandardItem *> stack;
        QStandardItem *item = m_model->item(row, 0);

        if (item != nullptr) {
            stack.append(item);
        }

        while (!stack.isEmpty()) {
            QStandardItem *current = stack.takeFirst();
            const bool expanded = m_expandedItemPaths.contains(itemPath(current));
            m_treeView->setExpanded(current->index(), expanded);

            for (int childRow = 0; childRow < current->rowCount(); ++childRow) {
                QStandardItem *child = current->child(childRow, 0);
                if (child != nullptr) {
                    stack.append(child);
                }
            }
        }
    }
}

void AverraTreeTable::updateColumnConfigState()
{
    for (int index = 0; index < m_columnVisibilityChecks.size(); ++index) {
        QCheckBox *checkBox = m_columnVisibilityChecks.at(index);
        const int column = checkBox->property("columnIndex").toInt();

        if (column >= 0 && column < m_model->columnCount()) {
            checkBox->blockSignals(true);
            checkBox->setChecked(!m_treeView->isColumnHidden(column));
            checkBox->blockSignals(false);
        }
    }
}

void AverraTreeTable::handleTreeClicked(const QModelIndex &index)
{
    if (!m_rowActionsEnabled || !index.isValid()) {
        return;
    }

    const int actionColumn = m_model->columnCount() - 1;

    if (index.column() != actionColumn) {
        return;
    }

    QStandardItem *item = m_model->itemFromIndex(index.sibling(index.row(), 0));
    emit rowActionTriggered(itemPath(item));
}

void AverraTreeTable::handleCurrentItemChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (!current.isValid()) {
        updateSelectionContext(nullptr);
        return;
    }

    QStandardItem *item = m_model->itemFromIndex(current.sibling(current.row(), 0));
    updateSelectionContext(item);
}

void AverraTreeTable::handleItemExpanded(const QModelIndex &index)
{
    if (!m_expansionStatePersistenceEnabled || !index.isValid()) {
        return;
    }

    QStandardItem *item = m_model->itemFromIndex(index);
    m_expandedItemPaths.insert(itemPath(item));
}

void AverraTreeTable::handleItemCollapsed(const QModelIndex &index)
{
    if (!m_expansionStatePersistenceEnabled || !index.isValid()) {
        return;
    }

    QStandardItem *item = m_model->itemFromIndex(index);
    m_expandedItemPaths.remove(itemPath(item));
}

void AverraTreeTable::handleSortIndicatorChanged(int logicalIndex, Qt::SortOrder order)
{
    Q_UNUSED(logicalIndex)
    Q_UNUSED(order)
}

void AverraTreeTable::handleSectionResized(int logicalIndex, int oldSize, int newSize)
{
    Q_UNUSED(logicalIndex)
    Q_UNUSED(oldSize)
    Q_UNUSED(newSize)
}

void AverraTreeTable::handleColumnConfigToggled(bool checked)
{
    Q_UNUSED(checked)
}

void AverraTreeTable::handleColumnVisibilityChanged(bool checked)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox *>(sender());

    if (checkBox == nullptr) {
        return;
    }

    const QVariant columnVariant = checkBox->property("columnIndex");
    const int column = columnVariant.isValid() ? columnVariant.toInt() : -1;

    if (column < 0 || column >= m_model->columnCount()) {
        return;
    }

    m_treeView->setColumnHidden(column, !checked);
    updateColumnConfigState();
}

void AverraTreeTable::updateSearchVisibility()
{
    m_searchBar->setVisible(m_searchEnabled);
}
