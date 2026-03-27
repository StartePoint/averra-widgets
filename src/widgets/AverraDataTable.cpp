#include <Averra/AverraDataTable.h>
#include <Averra/AverraButton.h>
#include <Averra/AverraEmptyState.h>
#include <Averra/AverraSearchBar.h>
#include <Averra/AverraThemeManager.h>
#include <Averra/AverraToolbar.h>

#include "../core/AverraStyleHelper.h"

#include <QHeaderView>
#include <QLabel>
#include <QModelIndex>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>

namespace
{
constexpr int kActionColumnWidth = 92;
}

AverraDataTable::AverraDataTable(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraDataTable::AverraDataTable(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraDataTable::title() const
{
    return m_title;
}

void AverraDataTable::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    m_toolbar->setTitle(m_title);
    m_titleLabel->setVisible(!m_title.isEmpty());
    emit titleChanged(m_title);
}

QString AverraDataTable::subtitle() const
{
    return m_subtitle;
}

void AverraDataTable::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle) {
        return;
    }

    m_subtitle = subtitle;
    m_subtitleLabel->setText(m_subtitle);
    m_toolbar->setSubtitle(m_subtitle);
    m_subtitleLabel->setVisible(!m_subtitle.isEmpty());
    emit subtitleChanged(m_subtitle);
}

bool AverraDataTable::sortingEnabled() const
{
    return m_sortingEnabled;
}

void AverraDataTable::setSortingEnabled(bool sortingEnabled)
{
    if (m_sortingEnabled == sortingEnabled) {
        return;
    }

    m_sortingEnabled = sortingEnabled;
    m_tableView->setSortingEnabled(m_sortingEnabled);
    emit sortingEnabledChanged(m_sortingEnabled);
}

bool AverraDataTable::rowActionsEnabled() const
{
    return m_rowActionsEnabled;
}

void AverraDataTable::setRowActionsEnabled(bool rowActionsEnabled)
{
    if (m_rowActionsEnabled == rowActionsEnabled) {
        return;
    }

    m_rowActionsEnabled = rowActionsEnabled;
    rebuildHeaders();
    rebuildActionCells();
    emit rowActionsEnabledChanged(m_rowActionsEnabled);
}

QString AverraDataTable::actionText() const
{
    return m_actionText;
}

void AverraDataTable::setActionText(const QString &actionText)
{
    if (m_actionText == actionText) {
        return;
    }

    m_actionText = actionText;
    rebuildActionCells();
    emit actionTextChanged(m_actionText);
}

AverraToolbar *AverraDataTable::toolbar() const
{
    return m_toolbar;
}

AverraSearchBar *AverraDataTable::searchBar() const
{
    return m_searchBar;
}

AverraEmptyState *AverraDataTable::emptyState() const
{
    return m_emptyState;
}

QTableView *AverraDataTable::tableView() const
{
    return m_tableView;
}

QStandardItemModel *AverraDataTable::model() const
{
    return m_model;
}

void AverraDataTable::setHeaders(const QStringList &headers)
{
    m_headers = headers;
    rebuildHeaders();
}

void AverraDataTable::addRow(const QStringList &values)
{
    QList<QStandardItem *> rowItems;

    for (int index = 0; index < m_headers.size(); ++index) {
        const QString value = index < values.size() ? values.at(index) : QString();
        rowItems.append(new QStandardItem(value));
    }

    if (m_rowActionsEnabled) {
        rowItems.append(new QStandardItem(m_actionText));
    }

    m_model->appendRow(rowItems);
    rebuildActionCells();
    updateEmptyState();
}

void AverraDataTable::clearRows()
{
    m_model->removeRows(0, m_model->rowCount());
    updateEmptyState();
}

void AverraDataTable::initialize()
{
    m_title.clear();
    m_subtitle.clear();
    m_actionText = QStringLiteral("查看");
    m_sortingEnabled = true;
    m_rowActionsEnabled = true;
    m_titleLabel = new QLabel(this);
    m_subtitleLabel = new QLabel(this);
    m_toolbar = new AverraToolbar(this);
    m_searchBar = new AverraSearchBar(this);
    m_refreshButton = new QPushButton(QStringLiteral("刷新"), this);
    m_tableView = new QTableView(this);
    m_model = new QStandardItemModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_emptyState = new AverraEmptyState(this);

    m_titleLabel->setObjectName(QStringLiteral("AverraDataTableTitle"));
    m_subtitleLabel->setObjectName(QStringLiteral("AverraDataTableSubtitle"));

    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(-1);

    m_searchBar->setPlaceholderText(QStringLiteral("搜索当前表格"));
    m_searchBar->setButtonText(QStringLiteral("筛选"));

    m_toolbar->addTrailingWidget(m_searchBar);
    m_toolbar->addTrailingWidget(m_refreshButton);

    m_emptyState->setTitle(QStringLiteral("暂无表格数据"));
    m_emptyState->setDescription(QStringLiteral("当前表格为空。你可以先添加一行数据，或者调整筛选条件。"));
    m_emptyState->setIconText(QStringLiteral("T"));

    m_tableView->setModel(m_proxyModel);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->verticalHeader()->setVisible(false);
    m_tableView->setMinimumHeight(220);
    m_tableView->setSortingEnabled(m_sortingEnabled);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_subtitleLabel);
    layout->addWidget(m_toolbar);
    layout->addWidget(m_tableView);
    layout->addWidget(m_emptyState);

    m_titleLabel->setVisible(false);
    m_subtitleLabel->setVisible(false);
    m_emptyState->setVisible(false);

    connect(m_model,
            &QStandardItemModel::rowsInserted,
            this,
            &AverraDataTable::updateEmptyState);
    connect(m_model,
            &QStandardItemModel::rowsRemoved,
            this,
            &AverraDataTable::updateEmptyState);
    connect(m_searchBar,
            &AverraSearchBar::textChanged,
            this,
            &AverraDataTable::applySearchFilter);
    connect(m_refreshButton,
            &QPushButton::clicked,
            m_searchBar,
            [this]() { applySearchFilter(m_searchBar->text()); });
    connect(m_tableView,
            &QTableView::clicked,
            this,
            &AverraDataTable::handleTableClicked);
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraDataTable::refreshStyle);

    rebuildHeaders();
    refreshStyle();
    updateEmptyState();
}

void AverraDataTable::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::dataTableFrameStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::dataTableTitleLabelStyleSheet(palette));
    m_subtitleLabel->setStyleSheet(AverraStyleHelper::dataTableSubtitleLabelStyleSheet(palette));
    m_toolbar->setStyleSheet(AverraStyleHelper::dataTableToolbarFrameStyleSheet(palette));
    m_tableView->setStyleSheet(AverraStyleHelper::dataTableViewStyleSheet(palette));
    m_refreshButton->setStyleSheet(AverraStyleHelper::dataTableActionButtonStyleSheet(palette));
    rebuildActionCells();
}

void AverraDataTable::updateEmptyState()
{
    const bool hasRows = m_proxyModel->rowCount() > 0;
    m_tableView->setVisible(hasRows);
    m_emptyState->setVisible(!hasRows);
}

void AverraDataTable::applySearchFilter(const QString &text)
{
    m_proxyModel->setFilterFixedString(text);
    updateEmptyState();
}

void AverraDataTable::handleTableClicked(const QModelIndex &index)
{
    if (!m_rowActionsEnabled) {
        return;
    }

    const int actionColumn = m_model->columnCount() - 1;
    if (index.column() != actionColumn) {
        return;
    }

    emit rowActionTriggered(m_proxyModel->mapToSource(index).row());
}

void AverraDataTable::rebuildHeaders()
{
    QStringList headers = m_headers;
    if (m_rowActionsEnabled) {
        headers.append(QStringLiteral("操作"));
    }

    m_model->setColumnCount(headers.size());
    m_model->setHorizontalHeaderLabels(headers);
}

void AverraDataTable::rebuildActionCells()
{
    if (!m_rowActionsEnabled || m_model->columnCount() == 0) {
        return;
    }

    const int actionColumn = m_model->columnCount() - 1;
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    for (int row = 0; row < m_model->rowCount(); ++row) {
        QStandardItem *item = m_model->item(row, actionColumn);
        if (item == nullptr) {
            item = new QStandardItem;
            m_model->setItem(row, actionColumn, item);
        }

        item->setText(m_actionText);
        item->setTextAlignment(Qt::AlignCenter);
    }

    m_tableView->setColumnWidth(actionColumn, kActionColumnWidth);
    Q_UNUSED(palette)
}
