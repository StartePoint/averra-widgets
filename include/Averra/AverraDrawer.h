#ifndef AVERRA_DRAWER_H
#define AVERRA_DRAWER_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QFrame;
class QHBoxLayout;
class QVBoxLayout;

class AVERRA_EXPORT AverraDrawer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(bool opened READ opened WRITE setOpened NOTIFY openedChanged)

public:
    explicit AverraDrawer(QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    bool opened() const;
    void setOpened(bool opened);

    QVBoxLayout *contentLayout() const;
    QHBoxLayout *footerLayout() const;
    void addContentWidget(QWidget *widget);
    void addFooterWidget(QWidget *widget);

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void openedChanged(bool opened);

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateTextVisibility();

    QString m_title;
    QString m_description;
    bool m_opened;
    QFrame *m_rootFrame;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    QWidget *m_contentWidget;
    QWidget *m_footerWidget;
    QVBoxLayout *m_contentLayout;
    QHBoxLayout *m_footerLayout;
};

#endif // AVERRA_DRAWER_H

