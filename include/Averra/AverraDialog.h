#ifndef AVERRA_DIALOG_H
#define AVERRA_DIALOG_H

#include <Averra/AverraGlobal.h>

#include <QDialog>

class QFrame;
class QLabel;
class QHBoxLayout;
class QGraphicsOpacityEffect;
class QParallelAnimationGroup;
class QToolButton;
class QVBoxLayout;
class QEvent;
class QCloseEvent;
class QShowEvent;

class AVERRA_EXPORT AverraDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    explicit AverraDialog(QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QVBoxLayout *contentLayout() const;
    QHBoxLayout *footerLayout() const;
    void addContentWidget(QWidget *widget);
    void addFooterWidget(QWidget *widget);

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateWindowControlLayout();
    void updateWindowControlStyle();
    void startShowAnimation();
    void startCloseAnimation();
    void updateTextVisibility();

    QString m_title;
    QString m_description;
    bool m_dragging;
    bool m_allowImmediateClose;
    QFrame *m_rootFrame;
    QFrame *m_titleBarFrame;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    QWidget *m_windowControlsWidget;
    QWidget *m_contentWidget;
    QWidget *m_footerWidget;
    QToolButton *m_minimizeButton;
    QToolButton *m_maximizeButton;
    QToolButton *m_closeButton;
    QHBoxLayout *m_titleBarLayout;
    QHBoxLayout *m_windowControlsLayout;
    QVBoxLayout *m_contentLayout;
    QHBoxLayout *m_footerLayout;
    QGraphicsOpacityEffect *m_opacityEffect;
    QParallelAnimationGroup *m_showAnimation;
    QParallelAnimationGroup *m_closeAnimation;
    QPoint m_dragOffset;
};

#endif // AVERRA_DIALOG_H
