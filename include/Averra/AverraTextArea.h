#ifndef AVERRA_TEXT_AREA_H
#define AVERRA_TEXT_AREA_H

#include <Averra/AverraGlobal.h>

#include <QTextEdit>

class AVERRA_EXPORT AverraTextArea : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(bool accentFrame READ accentFrame WRITE setAccentFrame NOTIFY accentFrameChanged)

public:
    explicit AverraTextArea(QWidget *parent = nullptr);

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

#endif // AVERRA_TEXT_AREA_H

