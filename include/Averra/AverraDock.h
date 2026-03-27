#ifndef AVERRA_DOCK_H
#define AVERRA_DOCK_H

#include <Averra/AverraGlobal.h>

#include <QDockWidget>

class QFrame;
class QLabel;
class QToolButton;
class QVBoxLayout;

class AVERRA_EXPORT AverraDock : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(bool opened READ opened WRITE setOpened NOTIFY openedChanged)

public:
    explicit AverraDock(QWidget *parent = nullptr);
    explicit AverraDock(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    bool opened() const;
    void setOpened(bool opened);

    QWidget *contentWidget() const;
    QVBoxLayout *contentLayout() const;
    void addContentWidget(QWidget *widget);

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void openedChanged(bool opened);

private slots:
    void initialize();
    void refreshStyle();
    void syncOpenedState(bool visible);

private:
    void updateTextVisibility();

    QString m_title;
    QString m_description;
    bool m_opened;
    QFrame *m_titleBarFrame;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    QToolButton *m_closeButton;
    QFrame *m_rootFrame;
    QWidget *m_contentHost;
    QVBoxLayout *m_contentLayout;
};

#endif // AVERRA_DOCK_H
