#ifndef AVERRA_DRAWER_H
#define AVERRA_DRAWER_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QLabel;
class QFrame;
class QGraphicsOpacityEffect;
class QHBoxLayout;
class QParallelAnimationGroup;
class QPropertyAnimation;
class QToolButton;
class QVBoxLayout;

class AVERRA_EXPORT AverraDrawer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(bool opened READ opened WRITE setOpened NOTIFY openedChanged)

public:
    explicit AverraDrawer(QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    bool opened() const;
    void setOpened(bool opened);

    QVBoxLayout *contentLayout() const;
    QHBoxLayout *footerLayout() const;
    void addContentWidget(QWidget *widget);
    void addFooterWidget(QWidget *widget);

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void openedChanged(bool opened);

private slots:
    void initialize();
    void refreshStyle();

private:
    void updateWindowControlLayout();
    void updateWindowControlStyle();
    void startShowAnimation();
    void startHideAnimation();
    void updateTextVisibility();

    QString m_title;
    QString m_description;
    bool m_opened;
    bool m_allowImmediateHide;
    bool m_expanded;
    int m_expandedWidth;
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
    QParallelAnimationGroup *m_hideAnimation;
};

#endif // AVERRA_DRAWER_H
