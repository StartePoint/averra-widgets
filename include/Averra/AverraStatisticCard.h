#ifndef AVERRA_STATISTIC_CARD_H
#define AVERRA_STATISTIC_CARD_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QFrame;

class AVERRA_EXPORT AverraStatisticCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString caption READ caption WRITE setCaption NOTIFY captionChanged)
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString helperText READ helperText WRITE setHelperText NOTIFY helperTextChanged)
    Q_PROPERTY(QString badgeText READ badgeText WRITE setBadgeText NOTIFY badgeTextChanged)
    Q_PROPERTY(bool accent READ accent WRITE setAccent NOTIFY accentChanged)

public:
    explicit AverraStatisticCard(QWidget *parent = nullptr);
    explicit AverraStatisticCard(const QString &caption, QWidget *parent = nullptr);

    QString caption() const;
    void setCaption(const QString &caption);

    QString value() const;
    void setValue(const QString &value);

    QString helperText() const;
    void setHelperText(const QString &helperText);

    QString badgeText() const;
    void setBadgeText(const QString &badgeText);

    bool accent() const;
    void setAccent(bool accent);

signals:
    void captionChanged(const QString &caption);
    void valueChanged(const QString &value);
    void helperTextChanged(const QString &helperText);
    void badgeTextChanged(const QString &badgeText);
    void accentChanged(bool accent);

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateTextVisibility();

    QString m_caption;
    QString m_value;
    QString m_helperText;
    QString m_badgeText;
    bool m_accent;
    QFrame *m_rootFrame;
    QLabel *m_captionLabel;
    QLabel *m_valueLabel;
    QLabel *m_helperLabel;
    QLabel *m_badgeLabel;
};

#endif // AVERRA_STATISTIC_CARD_H

