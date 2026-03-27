#ifndef AVERRA_NUMBER_RANGE_BOX_H
#define AVERRA_NUMBER_RANGE_BOX_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class AverraNumberBox;
class QLabel;

class AVERRA_EXPORT AverraNumberRangeBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int minimumValue READ minimumValue WRITE setMinimumValue NOTIFY rangeChanged)
    Q_PROPERTY(int maximumValue READ maximumValue WRITE setMaximumValue NOTIFY rangeChanged)
    Q_PROPERTY(bool accentFrame READ accentFrame WRITE setAccentFrame NOTIFY accentFrameChanged)

public:
    explicit AverraNumberRangeBox(QWidget *parent = nullptr);

    int minimumValue() const;
    void setMinimumValue(int value);

    int maximumValue() const;
    void setMaximumValue(int value);

    bool accentFrame() const;
    void setAccentFrame(bool accentFrame);

    AverraNumberBox *minimumBox() const;
    AverraNumberBox *maximumBox() const;

signals:
    void rangeChanged(int minimumValue, int maximumValue);
    void accentFrameChanged(bool accentFrame);

private slots:
    void initialize();
    void refreshStyle();
    void handleValueChanged();

private:
    bool m_accentFrame;
    QWidget *m_rootFrame;
    AverraNumberBox *m_minimumBox;
    AverraNumberBox *m_maximumBox;
    QLabel *m_separatorLabel;
};

#endif // AVERRA_NUMBER_RANGE_BOX_H

