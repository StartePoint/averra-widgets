#ifndef AVERRA_PROGRESS_BAR_H
#define AVERRA_PROGRESS_BAR_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class AVERRA_EXPORT AverraProgressBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum NOTIFY rangeChanged)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum NOTIFY rangeChanged)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool showText READ showText WRITE setShowText NOTIFY showTextChanged)

public:
    explicit AverraProgressBar(QWidget *parent = nullptr);

    int minimum() const;
    void setMinimum(int minimum);

    int maximum() const;
    void setMaximum(int maximum);

    void setRange(int minimum, int maximum);

    int value() const;
    void setValue(int value);

    bool showText() const;
    void setShowText(bool showText);

    QSize sizeHint() const override;

signals:
    void rangeChanged(int minimum, int maximum);
    void valueChanged(int value);
    void showTextChanged(bool showText);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void initialize();
    void handleThemeChanged();

private:
    qreal progressRatio() const;

    int m_minimum;
    int m_maximum;
    int m_value;
    bool m_showText;
};

#endif // AVERRA_PROGRESS_BAR_H

