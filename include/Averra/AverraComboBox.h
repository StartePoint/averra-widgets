#ifndef AVERRA_COMBO_BOX_H
#define AVERRA_COMBO_BOX_H

#include <Averra/AverraGlobal.h>

#include <QComboBox>

class AVERRA_EXPORT AverraComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(bool accentFrame READ accentFrame WRITE setAccentFrame NOTIFY accentFrameChanged)

public:
    explicit AverraComboBox(QWidget *parent = nullptr);

    bool accentFrame() const;
    void setAccentFrame(bool accentFrame);

signals:
    void accentFrameChanged(bool accentFrame);

private slots:
    void initialize();
    void refreshStyle();

private:
    bool m_accentFrame;
};

#endif // AVERRA_COMBO_BOX_H

