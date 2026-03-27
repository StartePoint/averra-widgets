#ifndef AVERRA_LINE_EDIT_H
#define AVERRA_LINE_EDIT_H

#include <Averra/AverraGlobal.h>

#include <QLineEdit>

class AVERRA_EXPORT AverraLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(bool accentFrame READ accentFrame WRITE setAccentFrame NOTIFY accentFrameChanged)

public:
    explicit AverraLineEdit(QWidget *parent = nullptr);
    explicit AverraLineEdit(const QString &text, QWidget *parent = nullptr);

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

#endif // AVERRA_LINE_EDIT_H

