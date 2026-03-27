#ifndef AVERRA_TOOLBAR_H
#define AVERRA_TOOLBAR_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QFrame;
class QLabel;
class QHBoxLayout;

class AVERRA_EXPORT AverraToolbar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(bool accent READ accent WRITE setAccent NOTIFY accentChanged)

public:
    explicit AverraToolbar(QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString subtitle() const;
    void setSubtitle(const QString &subtitle);

    bool accent() const;
    void setAccent(bool accent);

    QHBoxLayout *leadingLayout() const;
    QHBoxLayout *trailingLayout() const;
    void addLeadingWidget(QWidget *widget);
    void addTrailingWidget(QWidget *widget);

signals:
    void titleChanged(const QString &title);
    void subtitleChanged(const QString &subtitle);
    void accentChanged(bool accent);

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateTextVisibility();

    QString m_title;
    QString m_subtitle;
    bool m_accent;
    QFrame *m_rootFrame;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QWidget *m_leadingWidget;
    QWidget *m_trailingWidget;
    QHBoxLayout *m_leadingLayout;
    QHBoxLayout *m_trailingLayout;
};

#endif // AVERRA_TOOLBAR_H

