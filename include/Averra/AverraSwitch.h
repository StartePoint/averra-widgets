#ifndef AVERRA_SWITCH_H
#define AVERRA_SWITCH_H

#include <Averra/AverraGlobal.h>

#include <QAbstractButton>

class AVERRA_EXPORT AverraSwitch : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY checkedChanged)

public:
    explicit AverraSwitch(QWidget *parent = nullptr);

    QSize sizeHint() const override;

signals:
    void checkedChanged(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void initialize();
    void handleThemeChanged();

private:
    bool m_hovered;
};

#endif // AVERRA_SWITCH_H
