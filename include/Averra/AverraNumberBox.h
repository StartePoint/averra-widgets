#ifndef AVERRA_NUMBER_BOX_H
#define AVERRA_NUMBER_BOX_H

#include <Averra/AverraGlobal.h>

#include <QSpinBox>

class AVERRA_EXPORT AverraNumberBox : public QSpinBox
{
    Q_OBJECT
    Q_PROPERTY(bool accentFrame READ accentFrame WRITE setAccentFrame NOTIFY accentFrameChanged)

public:
    explicit AverraNumberBox(QWidget *parent = nullptr);

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

#endif // AVERRA_NUMBER_BOX_H

