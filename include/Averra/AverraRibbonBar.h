#ifndef AVERRA_RIBBON_BAR_H
#define AVERRA_RIBBON_BAR_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QFrame;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QStackedWidget;
class QVBoxLayout;

class AVERRA_EXPORT AverraRibbonBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)

public:
    explicit AverraRibbonBar(QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString subtitle() const;
    void setSubtitle(const QString &subtitle);

    int currentPage() const;
    void setCurrentPage(int index);

    int pageCount() const;
    QHBoxLayout *leadingLayout() const;
    QHBoxLayout *trailingLayout() const;
    void addLeadingWidget(QWidget *widget);
    void addTrailingWidget(QWidget *widget);
    int addPage(const QString &title);
    QWidget *pageWidget(int index) const;
    int addGroup(int pageIndex, const QString &title);
    QVBoxLayout *groupLayout(int pageIndex, int groupIndex) const;
    void addGroupWidget(int pageIndex, int groupIndex, QWidget *widget);

signals:
    void titleChanged(const QString &title);
    void subtitleChanged(const QString &subtitle);
    void currentPageChanged(int index);

private slots:
    void initialize();
    void refreshStyle();
    void handlePageButtonClicked();

private:
    struct RibbonGroup
    {
        QFrame *frame;
        QLabel *titleLabel;
        QWidget *contentWidget;
        QVBoxLayout *contentLayout;
    };

    struct RibbonPage
    {
        QPushButton *button;
        QWidget *widget;
        QHBoxLayout *groupsLayout;
        QList<RibbonGroup> groups;
    };

    void updateTextVisibility();
    void refreshPageButtons();

    QString m_title;
    QString m_subtitle;
    QFrame *m_rootFrame;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QWidget *m_leadingWidget;
    QWidget *m_trailingWidget;
    QHBoxLayout *m_leadingLayout;
    QHBoxLayout *m_trailingLayout;
    QWidget *m_tabsWidget;
    QHBoxLayout *m_tabsLayout;
    QStackedWidget *m_pagesStack;
    QList<RibbonPage> m_pages;
};

#endif // AVERRA_RIBBON_BAR_H
