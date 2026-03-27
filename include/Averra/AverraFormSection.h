#ifndef AVERRA_FORM_SECTION_H
#define AVERRA_FORM_SECTION_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class AverraSectionHeader;
class QFrame;
class QVBoxLayout;

class AVERRA_EXPORT AverraFormSection : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString metaText READ metaText WRITE setMetaText NOTIFY metaTextChanged)

public:
    explicit AverraFormSection(QWidget *parent = nullptr);
    explicit AverraFormSection(const QString &title, QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QString metaText() const;
    void setMetaText(const QString &metaText);

    QVBoxLayout *contentLayout() const;
    void addRow(QWidget *widget);

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void metaTextChanged(const QString &metaText);

private slots:
    void initialize();
    void refreshStyle();

private:
    QString m_title;
    QString m_description;
    QString m_metaText;
    QFrame *m_rootFrame;
    AverraSectionHeader *m_header;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;
};

#endif // AVERRA_FORM_SECTION_H

