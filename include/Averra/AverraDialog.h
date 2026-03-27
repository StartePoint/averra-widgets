#ifndef AVERRA_DIALOG_H
#define AVERRA_DIALOG_H

#include <Averra/AverraGlobal.h>

#include <QDialog>

class QFrame;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;

class AVERRA_EXPORT AverraDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    explicit AverraDialog(QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QVBoxLayout *contentLayout() const;
    QHBoxLayout *footerLayout() const;
    void addContentWidget(QWidget *widget);
    void addFooterWidget(QWidget *widget);

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
    QWidget *m_footerWidget;
    QVBoxLayout *m_contentLayout;
    QHBoxLayout *m_footerLayout;
};

#endif // AVERRA_DIALOG_H

