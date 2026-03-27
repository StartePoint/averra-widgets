#ifndef AVERRA_INFO_BAR_H
#define AVERRA_INFO_BAR_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QFrame;
class QToolButton;

class AVERRA_EXPORT AverraInfoBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool closable READ closable WRITE setClosable NOTIFY closableChanged)

public:
    enum Type
    {
        Info,
        Success,
        Warning,
        Error
    };
    Q_ENUM(Type)

    explicit AverraInfoBar(QWidget *parent = nullptr);
    explicit AverraInfoBar(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    Type type() const;
    void setType(Type type);

    bool closable() const;
    void setClosable(bool closable);

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void typeChanged(Type type);
    void closableChanged(bool closable);
    void dismissed();

private slots:
    void initialize();
    void refreshStyle();
    void handleCloseButtonClicked();

private:
    void updateTextVisibility();
    void updateCloseButtonVisibility();
    QString badgeText() const;

    QString m_title;
    QString m_description;
    Type m_type;
    bool m_closable;
    QFrame *m_rootFrame;
    QLabel *m_badgeLabel;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    QToolButton *m_closeButton;
};

#endif // AVERRA_INFO_BAR_H

