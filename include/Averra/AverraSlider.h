#ifndef AVERRA_SLIDER_H
#define AVERRA_SLIDER_H

#include <Averra/AverraGlobal.h>

#include <QAbstractSlider>

class AVERRA_EXPORT AverraSlider : public QAbstractSlider
{
    Q_OBJECT

public:
    explicit AverraSlider(QWidget *parent = nullptr);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void initialize();
    void handleThemeChanged();

private:
    int valueFromPosition(int x) const;
    QRectF grooveRect() const;
    QRectF handleRect() const;

    bool m_hovered;
    bool m_dragging;
};

#endif // AVERRA_SLIDER_H
