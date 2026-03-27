#ifndef AVERRA_BADGE_BUTTON_H
#define AVERRA_BADGE_BUTTON_H

#include <Averra/AverraGlobal.h>

#include <QPushButton>

class QLabel;
class QResizeEvent;

class AVERRA_EXPORT AverraBadgeButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool accent READ accent WRITE setAccent NOTIFY accentChanged)
    Q_PROPERTY(QString badgeText READ badgeText WRITE setBadgeText NOTIFY badgeTextChanged)

public:
    explicit AverraBadgeButton(QWidget *parent = nullptr);
    explicit AverraBadgeButton(const QString &text, QWidget *parent = nullptr);

    bool accent() const;
    void setAccent(bool accent);

    QString badgeText() const;
    void setBadgeText(const QString &badgeText);

signals:
    void accentChanged(bool accent);
    void badgeTextChanged(const QString &badgeText);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateBadgeGeometry();

    bool m_accent;
    QString m_badgeText;
    QLabel *m_badgeLabel;
};

#endif // AVERRA_BADGE_BUTTON_H
