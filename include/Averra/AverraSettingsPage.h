#ifndef AVERRA_SETTINGS_PAGE_H
#define AVERRA_SETTINGS_PAGE_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QFrame;
class QVBoxLayout;

class AVERRA_EXPORT AverraSettingsPage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    explicit AverraSettingsPage(QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QVBoxLayout *contentLayout() const;
    void addSection(QWidget *widget);

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateTextVisibility();

    QString m_title;
    QString m_description;
    QFrame *m_rootFrame;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;
};

#endif // AVERRA_SETTINGS_PAGE_H

