#ifndef GALLERY_WINDOW_H
#define GALLERY_WINDOW_H

#include <QMainWindow>

class AverraNavigationPanel;
class AverraSearchBar;
class QFrame;
class QLabel;
class QPushButton;
class QStackedWidget;
class QWidget;

class GalleryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GalleryWindow(QWidget *parent = nullptr);
    ~GalleryWindow() override;

private slots:
    void applyOceanTheme();
    void applyForestTheme();
    void applySunsetTheme();
    void applyOrchidTheme();
    void switchCategory(int index);
    void updateCategoryGuide(const QString &text);

private:
    QWidget *createScrollableCategoryPage(const QString &title, const QString &subtitle, QLayout *contentLayout);
    QWidget *createGuidePage();
    QWidget *createActionsPage();
    QWidget *createInputPage();
    QWidget *createSelectionPage();
    QWidget *createDataPage();
    QWidget *createFeedbackPage();
    QWidget *createLayoutPage();
    QFrame *createSidebarFrame();
    QFrame *createPreviewFrame();
    void setupWindow();
    void populateCategories();

    QPushButton *m_oceanButton;
    QPushButton *m_forestButton;
    QPushButton *m_sunsetButton;
    QPushButton *m_orchidButton;
    AverraSearchBar *m_categorySearchBar;
    QLabel *m_categoryHintLabel;
    AverraNavigationPanel *m_categoryNavigation;
    QStackedWidget *m_previewStack;
};

#endif // GALLERY_WINDOW_H
