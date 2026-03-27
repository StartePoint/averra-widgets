#ifndef AVERRA_DIVIDER_H
#define AVERRA_DIVIDER_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class AVERRA_EXPORT AverraDivider : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(int thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)

public:
    explicit AverraDivider(QWidget *parent = nullptr);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    int thickness() const;
    void setThickness(int thickness);

    QSize sizeHint() const override;

signals:
    void orientationChanged(Qt::Orientation orientation);
    void thicknessChanged(int thickness);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void initialize();
    void handleThemeChanged();

private:
    Qt::Orientation m_orientation;
    int m_thickness;
};

#endif // AVERRA_DIVIDER_H

