#ifndef AVERRA_SEGMENTED_CONTROL_H
#define AVERRA_SEGMENTED_CONTROL_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QHBoxLayout;
class QPushButton;

class AVERRA_EXPORT AverraSegmentedControl : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit AverraSegmentedControl(QWidget *parent = nullptr);

    int currentIndex() const;
    void setCurrentIndex(int index);

    int count() const;
    void addSegment(const QString &text);

signals:
    void currentIndexChanged(int index);

private slots:
    void initialize();
    void refreshStyle();
    void handleSegmentClicked();

private:
    QHBoxLayout *m_layout;
    QList<QPushButton *> m_buttons;
    int m_currentIndex;
};

#endif // AVERRA_SEGMENTED_CONTROL_H

