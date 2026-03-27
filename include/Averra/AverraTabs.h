#ifndef AVERRA_TABS_H
#define AVERRA_TABS_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QHBoxLayout;
class QPushButton;
class QStackedWidget;

class AVERRA_EXPORT AverraTabs : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit AverraTabs(QWidget *parent = nullptr);

    int currentIndex() const;
    void setCurrentIndex(int index);

    int count() const;
    void addTab(QWidget *widget, const QString &title);

signals:
    void currentIndexChanged(int index);

private slots:
    void initialize();
    void refreshStyle();
    void handleTabClicked();

private:
    QWidget *m_tabsWidget;
    QHBoxLayout *m_tabsLayout;
    QStackedWidget *m_stack;
    QList<QPushButton *> m_tabButtons;
};

#endif // AVERRA_TABS_H

