#ifndef AVERRA_DATE_EDIT_H
#define AVERRA_DATE_EDIT_H

#include <Averra/AverraGlobal.h>

#include <QDateEdit>
#include <QPointer>

class QHideEvent;
class QKeyEvent;
class QMouseEvent;
class QMoveEvent;
class QResizeEvent;
class AverraDatePopup;

class AVERRA_EXPORT AverraDateEdit : public QDateEdit
{
    Q_OBJECT
    Q_PROPERTY(bool accentFrame READ accentFrame WRITE setAccentFrame NOTIFY accentFrameChanged)

public:
    explicit AverraDateEdit(QWidget *parent = nullptr);

    bool accentFrame() const;
    void setAccentFrame(bool accentFrame);

signals:
    void accentFrameChanged(bool accentFrame);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private slots:
    void initialize();
    void refreshStyle();

private:
    void ensurePopup();
    void refreshPopupStyle();
    void syncPopupState();
    void togglePopup();
    void showPopup();
    void hidePopup();
    bool popupVisible() const;
    bool dropDownRectContains(const QPoint &position) const;

    bool m_accentFrame;
    bool m_dropDownPressed;
    QPointer<AverraDatePopup> m_popup;
};

#endif // AVERRA_DATE_EDIT_H
