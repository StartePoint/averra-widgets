#ifndef AVERRA_PROPERTY_ITEM_H
#define AVERRA_PROPERTY_ITEM_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QFrame;
class QHBoxLayout;
class QLabel;
class QWidget;

class AVERRA_EXPORT AverraPropertyItem : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    explicit AverraPropertyItem(QWidget *parent = nullptr);
    explicit AverraPropertyItem(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QWidget *accessoryWidget() const;
    void setAccessoryWidget(QWidget *widget);

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
    QWidget *m_accessoryHost;
    QHBoxLayout *m_accessoryLayout;
    QWidget *m_accessoryWidget;
};

#endif // AVERRA_PROPERTY_ITEM_H
