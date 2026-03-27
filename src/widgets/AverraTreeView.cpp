#include <Averra/AverraTreeView.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QHeaderView>
#include <QLabel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>

AverraTreeView::AverraTreeView(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraTreeView::AverraTreeView(const QString &title, QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
    setTitle(title);
}

QString AverraTreeView::title() const
{
    return m_title;
}

void AverraTreeView::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    m_titleLabel->setVisible(!m_title.isEmpty());
    emit titleChanged(m_title);
}

QTreeView *AverraTreeView::treeView() const
{
    return m_treeView;
}

QStandardItemModel *AverraTreeView::model() const
{
    return m_model;
}

void AverraTreeView::setHeaders(const QStringList &headers)
{
    m_model->setColumnCount(headers.size());
    m_model->setHorizontalHeaderLabels(headers);
}

QStandardItem *AverraTreeView::addTopLevelItem(const QStringList &values)
{
    QList<QStandardItem *> rowItems;

    for (int index = 0; index < values.size(); ++index) {
        rowItems.append(new QStandardItem(values.at(index)));
    }

    m_model->appendRow(rowItems);
    return rowItems.isEmpty() ? nullptr : rowItems.first();
}

QStandardItem *AverraTreeView::addChildItem(QStandardItem *parent, const QStringList &values)
{
    if (parent == nullptr) {
        return nullptr;
    }

    QList<QStandardItem *> rowItems;

    for (int index = 0; index < values.size(); ++index) {
        rowItems.append(new QStandardItem(values.at(index)));
    }

    parent->appendRow(rowItems);
    return rowItems.isEmpty() ? nullptr : rowItems.first();
}

void AverraTreeView::clearItems()
{
    m_model->removeRows(0, m_model->rowCount());
}

void AverraTreeView::initialize()
{
    m_title.clear();
    m_titleLabel = new QLabel(this);
    m_treeView = new QTreeView(this);
    m_model = new QStandardItemModel(this);

    m_titleLabel->setObjectName(QStringLiteral("AverraTreeViewTitle"));
    m_titleLabel->setVisible(false);

    m_treeView->setModel(m_model);
    m_treeView->setAlternatingRowColors(true);
    m_treeView->setMinimumHeight(220);
    m_treeView->header()->setStretchLastSection(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_treeView);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraTreeView::refreshStyle);

    refreshStyle();
}

void AverraTreeView::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    setStyleSheet(AverraStyleHelper::treeViewFrameStyleSheet(palette));
    m_titleLabel->setStyleSheet(AverraStyleHelper::treeViewTitleLabelStyleSheet(palette));
    m_treeView->setStyleSheet(AverraStyleHelper::treeViewViewStyleSheet(palette));
}

