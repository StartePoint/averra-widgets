#include <Averra/AverraPagination.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QVariant>

AverraPagination::AverraPagination(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

int AverraPagination::currentPage() const
{
    return m_currentPage;
}

void AverraPagination::setCurrentPage(int currentPage)
{
    if (currentPage < 1 || currentPage > m_pageCount) {
        return;
    }

    if (m_currentPage == currentPage) {
        return;
    }

    m_currentPage = currentPage;
    refreshState();
    emit currentPageChanged(m_currentPage);
}

int AverraPagination::pageCount() const
{
    return m_pageCount;
}

void AverraPagination::setPageCount(int pageCount)
{
    if (pageCount < 1 || m_pageCount == pageCount) {
        return;
    }

    m_pageCount = pageCount;

    if (m_currentPage > m_pageCount) {
        m_currentPage = m_pageCount;
    }

    rebuildPageButtons();
    refreshState();
    emit pageCountChanged(m_pageCount);
}

void AverraPagination::initialize()
{
    m_currentPage = 1;
    m_pageCount = 1;
    m_infoLabel = new QLabel(this);
    m_previousButton = new QPushButton(QStringLiteral("上一页"), this);
    m_nextButton = new QPushButton(QStringLiteral("下一页"), this);
    m_buttonsWidget = new QWidget(this);
    m_buttonsLayout = new QHBoxLayout;

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    m_infoLabel->setObjectName(QStringLiteral("AverraPaginationLabel"));

    QHBoxLayout *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(8);

    m_buttonsLayout->setContentsMargins(0, 0, 0, 0);
    m_buttonsLayout->setSpacing(6);
    m_buttonsWidget->setLayout(m_buttonsLayout);

    rootLayout->addWidget(m_previousButton);
    rootLayout->addWidget(m_buttonsWidget);
    rootLayout->addWidget(m_nextButton);
    rootLayout->addWidget(m_infoLabel);

    connect(m_previousButton,
            &QPushButton::clicked,
            this,
            &AverraPagination::goToPreviousPage);

    connect(m_nextButton,
            &QPushButton::clicked,
            this,
            &AverraPagination::goToNextPage);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraPagination::refreshStyle);

    rebuildPageButtons();
    refreshState();
}

void AverraPagination::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    m_previousButton->setStyleSheet(AverraStyleHelper::paginationButtonStyleSheet(palette, false));
    m_nextButton->setStyleSheet(AverraStyleHelper::paginationButtonStyleSheet(palette, false));
    m_infoLabel->setStyleSheet(AverraStyleHelper::paginationLabelStyleSheet(palette));

    for (int index = 0; index < m_pageButtons.size(); ++index) {
        const bool current = (index + 1 == m_currentPage);
        m_pageButtons.at(index)->setStyleSheet(AverraStyleHelper::paginationButtonStyleSheet(palette, current));
    }
}

void AverraPagination::goToPreviousPage()
{
    setCurrentPage(m_currentPage - 1);
}

void AverraPagination::goToNextPage()
{
    setCurrentPage(m_currentPage + 1);
}

void AverraPagination::handlePageButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button == nullptr) {
        return;
    }

    const int page = button->property("page").toInt();
    setCurrentPage(page);
}

void AverraPagination::rebuildPageButtons()
{
    for (int index = 0; index < m_pageButtons.size(); ++index) {
        QPushButton *button = m_pageButtons.at(index);
        m_buttonsLayout->removeWidget(button);
        delete button;
    }

    m_pageButtons.clear();

    for (int page = 1; page <= m_pageCount; ++page) {
        QPushButton *button = new QPushButton(QString::number(page), m_buttonsWidget);
        button->setProperty("page", page);
        button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(button, &QPushButton::clicked, this, &AverraPagination::handlePageButtonClicked);
        m_buttonsLayout->addWidget(button);
        m_pageButtons.append(button);
    }
}

void AverraPagination::refreshState()
{
    m_previousButton->setEnabled(m_currentPage > 1);
    m_nextButton->setEnabled(m_currentPage < m_pageCount);
    m_infoLabel->setText(QStringLiteral("第 %1 / %2 页").arg(m_currentPage).arg(m_pageCount));
    refreshStyle();
}
