#ifndef AVERRA_PROPERTY_EDITOR_H
#define AVERRA_PROPERTY_EDITOR_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class AverraSearchBar;
class QLabel;
class QFrame;
class QScrollArea;
class QVBoxLayout;

class AVERRA_EXPORT AverraPropertyEditor : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    Q_PROPERTY(bool searchEnabled READ searchEnabled WRITE setSearchEnabled NOTIFY searchEnabledChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)

public:
    explicit AverraPropertyEditor(QWidget *parent = nullptr);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QString filterText() const;
    void setFilterText(const QString &filterText);

    bool searchEnabled() const;
    void setSearchEnabled(bool searchEnabled);

    bool readOnly() const;
    void setReadOnly(bool readOnly);

    AverraSearchBar *searchBar() const;
    QVBoxLayout *contentLayout() const;
    void addSection(QWidget *widget);
    void clearSections();

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void filterTextChanged(const QString &filterText);
    void searchEnabledChanged(bool searchEnabled);
    void readOnlyChanged(bool readOnly);

private slots:
    void initialize();
    void refreshStyle();
    void applyFilter();

private:
    bool applyFilterToSection(QWidget *widget, const QString &pattern);
    bool widgetMatches(QWidget *widget, const QString &pattern) const;
    void syncReadOnlyState();
    void updateSearchVisibility();
    void updateTextVisibility();

    QString m_title;
    QString m_description;
    QString m_filterText;
    bool m_searchEnabled;
    bool m_readOnly;
    QFrame *m_rootFrame;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    AverraSearchBar *m_searchBar;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;
};

#endif // AVERRA_PROPERTY_EDITOR_H
