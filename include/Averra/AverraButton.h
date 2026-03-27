#ifndef AVERRA_BUTTON_H
#define AVERRA_BUTTON_H

#include <Averra/AverraGlobal.h>

#include <QPushButton>

class AVERRA_EXPORT AverraButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool accent READ accent WRITE setAccent NOTIFY accentChanged)

public:
    explicit AverraButton(QWidget *parent = nullptr);
    explicit AverraButton(const QString &text, QWidget *parent = nullptr);

    bool accent() const;
    void setAccent(bool accent);

signals:
    void accentChanged(bool accent);

private slots:
    void initialize();
    void refreshStyle();

private:
    bool m_accent;
};

#endif // AVERRA_BUTTON_H
