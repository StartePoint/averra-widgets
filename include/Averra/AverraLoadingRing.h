#ifndef AVERRA_LOADING_RING_H
#define AVERRA_LOADING_RING_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QTimer;

class AVERRA_EXPORT AverraLoadingRing : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(int diameter READ diameter WRITE setDiameter NOTIFY diameterChanged)

public:
    explicit AverraLoadingRing(QWidget *parent = nullptr);

    bool running() const;
    void setRunning(bool running);

    int diameter() const;
    void setDiameter(int diameter);

    void start();
    void stop();

    QSize sizeHint() const override;

signals:
    void runningChanged(bool running);
    void diameterChanged(int diameter);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void initialize();
    void advanceFrame();
    void handleThemeChanged();

private:
    bool m_running;
    int m_diameter;
    int m_angle;
    QTimer *m_timer;
};

#endif // AVERRA_LOADING_RING_H

