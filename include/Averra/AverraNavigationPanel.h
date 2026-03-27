#ifndef AVERRA_NAVIGATION_PANEL_H
#define AVERRA_NAVIGATION_PANEL_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QButtonGroup;
class QFrame;
class QLabel;
class QPushButton;
class QVBoxLayout;

class AVERRA_EXPORT AverraNavigationPanel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit AverraNavigationPanel(QWidget *parent = nullptr);
    explicit AverraNavigationPanel(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    int currentIndex() const;
    void setCurrentIndex(int index);

    int itemCount() const;
    QString itemText(int index) const;

    void addItem(const QString &text);
    void clearItems();

signals:
    void titleChanged(const QString &title);
    void currentIndexChanged(int index);
    void itemClicked(int index);

private slots:
    void initialize();
    void refreshStyle();
    void handleButtonGroupClicked(int id);

private:
    void rebuildButtonIds();
    void refreshCurrentState();

    QString m_title;
    int m_currentIndex;
    QFrame *m_rootFrame;
    QLabel *m_titleLabel;
    QWidget *m_itemsWidget;
    QVBoxLayout *m_itemsLayout;
    QButtonGroup *m_buttonGroup;
    QList<QPushButton *> m_itemButtons;
};

#endif // AVERRA_NAVIGATION_PANEL_H

