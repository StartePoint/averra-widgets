#ifndef AVERRA_TOAST_H
#define AVERRA_TOAST_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QFrame;
class QLabel;
class QTimer;

class AVERRA_EXPORT AverraToast : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int durationMs READ durationMs WRITE setDurationMs NOTIFY durationMsChanged)

public:
    enum Type
    {
        Info,
        Success,
        Warning,
        Error
    };
    Q_ENUM(Type)

    explicit AverraToast(QWidget *parent = nullptr);

    Type type() const;
    void setType(Type type);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    int durationMs() const;
    void setDurationMs(int durationMs);

    void startAutoClose();
    void stopAutoClose();

signals:
    void typeChanged(Type type);
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void durationMsChanged(int durationMs);

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateTextVisibility();

    Type m_type;
    QString m_title;
    QString m_description;
    int m_durationMs;
    QFrame *m_rootFrame;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    QTimer *m_timer;
};

#endif // AVERRA_TOAST_H

