#ifndef AVERRA_CARD_H
#define AVERRA_CARD_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QFrame;
class QVBoxLayout;

class AVERRA_EXPORT AverraCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(bool accentHeader READ accentHeader WRITE setAccentHeader NOTIFY accentHeaderChanged)

public:
    explicit AverraCard(QWidget *parent = nullptr);
    explicit AverraCard(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString subtitle() const;
    void setSubtitle(const QString &subtitle);

    bool accentHeader() const;
    void setAccentHeader(bool accentHeader);

    QWidget *contentWidget() const;
    QVBoxLayout *contentLayout() const;

signals:
    void titleChanged(const QString &title);
    void subtitleChanged(const QString &subtitle);
    void accentHeaderChanged(bool accentHeader);

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateTextVisibility();

    QString m_title;
    QString m_subtitle;
    bool m_accentHeader;
    QFrame *m_rootFrame;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;
};

#endif // AVERRA_CARD_H

