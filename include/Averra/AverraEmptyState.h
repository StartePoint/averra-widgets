#ifndef AVERRA_EMPTY_STATE_H
#define AVERRA_EMPTY_STATE_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QFrame;

class AVERRA_EXPORT AverraEmptyState : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString iconText READ iconText WRITE setIconText NOTIFY iconTextChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    explicit AverraEmptyState(QWidget *parent = nullptr);
    explicit AverraEmptyState(const QString &title, QWidget *parent = nullptr);

    QString iconText() const;
    void setIconText(const QString &iconText);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

signals:
    void iconTextChanged(const QString &iconText);
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateTextVisibility();

    QString m_iconText;
    QString m_title;
    QString m_description;
    QFrame *m_rootFrame;
    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
};

#endif // AVERRA_EMPTY_STATE_H

