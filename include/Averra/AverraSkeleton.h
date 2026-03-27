#ifndef AVERRA_SKELETON_H
#define AVERRA_SKELETON_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QTimer;

class AVERRA_EXPORT AverraSkeleton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    explicit AverraSkeleton(QWidget *parent = nullptr);

    bool running() const;
    void setRunning(bool running);

    int radius() const;
    void setRadius(int radius);

    void start();
    void stop();

    QSize sizeHint() const override;

signals:
    void runningChanged(bool running);
    void radiusChanged(int radius);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void initialize();
    void advanceFrame();
    void handleThemeChanged();

private:
    bool m_running;
    int m_radius;
    int m_offset;
    QTimer *m_timer;
};

#endif // AVERRA_SKELETON_H

