#ifndef AVERRA_SECTION_HEADER_H
#define AVERRA_SECTION_HEADER_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QFrame;

class AVERRA_EXPORT AverraSectionHeader : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(QString metaText READ metaText WRITE setMetaText NOTIFY metaTextChanged)

public:
    explicit AverraSectionHeader(QWidget *parent = nullptr);
    explicit AverraSectionHeader(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString subtitle() const;
    void setSubtitle(const QString &subtitle);

    QString metaText() const;
    void setMetaText(const QString &metaText);

signals:
    void titleChanged(const QString &title);
    void subtitleChanged(const QString &subtitle);
    void metaTextChanged(const QString &metaText);

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateTextVisibility();

    QString m_title;
    QString m_subtitle;
    QString m_metaText;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QLabel *m_metaLabel;
    QFrame *m_divider;
};

#endif // AVERRA_SECTION_HEADER_H

