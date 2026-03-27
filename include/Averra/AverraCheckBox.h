#ifndef AVERRA_CHECK_BOX_H
#define AVERRA_CHECK_BOX_H

#include <Averra/AverraGlobal.h>

#include <QAbstractButton>

class AVERRA_EXPORT AverraCheckBox : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY checkedChanged)

public:
    explicit AverraCheckBox(QWidget *parent = nullptr);
    explicit AverraCheckBox(const QString &text, QWidget *parent = nullptr);

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

#endif // AVERRA_CHECK_BOX_H
