#ifndef AVERRA_PAGINATION_H
#define AVERRA_PAGINATION_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QPushButton;
class QHBoxLayout;

class AVERRA_EXPORT AverraPagination : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(int pageCount READ pageCount WRITE setPageCount NOTIFY pageCountChanged)

public:
    explicit AverraPagination(QWidget *parent = nullptr);

    int currentPage() const;
    void setCurrentPage(int currentPage);

    int pageCount() const;
    void setPageCount(int pageCount);

signals:
    void currentPageChanged(int currentPage);
    void pageCountChanged(int pageCount);

private slots:
    void initialize();
    void refreshStyle();
    void goToPreviousPage();
    void goToNextPage();
    void handlePageButtonClicked();

private:
    void rebuildPageButtons();
    void refreshState();

    int m_currentPage;
    int m_pageCount;
    QLabel *m_infoLabel;
    QPushButton *m_previousButton;
    QPushButton *m_nextButton;
    QWidget *m_buttonsWidget;
    QHBoxLayout *m_buttonsLayout;
    QList<QPushButton *> m_pageButtons;
};

#endif // AVERRA_PAGINATION_H

