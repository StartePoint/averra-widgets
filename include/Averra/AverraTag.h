#ifndef AVERRA_TAG_H
#define AVERRA_TAG_H

#include <Averra/AverraGlobal.h>

#include <QLabel>

class AVERRA_EXPORT AverraTag : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(Tone tone READ tone WRITE setTone NOTIFY toneChanged)
    Q_PROPERTY(bool filled READ filled WRITE setFilled NOTIFY filledChanged)

public:
    enum Tone
    {
        Neutral,
        Accent,
        Success,
        Warning,
        Error
    };
    Q_ENUM(Tone)

    explicit AverraTag(QWidget *parent = nullptr);
    explicit AverraTag(const QString &text, QWidget *parent = nullptr);

    Tone tone() const;
    void setTone(Tone tone);

    bool filled() const;
    void setFilled(bool filled);

signals:
    void toneChanged(Tone tone);
    void filledChanged(bool filled);

private slots:
    void initialize();
    void refreshStyle();

private:
    Tone m_tone;
    bool m_filled;
};

#endif // AVERRA_TAG_H

