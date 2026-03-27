#ifndef AVERRA_PROPERTY_GRID_H
#define AVERRA_PROPERTY_GRID_H

#include <Averra/AverraFieldDefinition.h>
#include <Averra/AverraGlobal.h>
#include <Averra/AverraValidationResult.h>

#include <QVariantMap>
#include <QWidget>

class AverraFormModel;
class AverraSearchBar;
class AverraSwitch;
class AverraComboBox;
class AverraNumberBox;
class QFrame;
class QLabel;
class QScrollArea;
class QSettings;
class QVBoxLayout;

class AVERRA_EXPORT AverraPropertyGridDelegate
{
public:
    virtual ~AverraPropertyGridDelegate()
    {
    }

    virtual void polishRow(const QString &groupTitle,
                           const QString &rowTitle,
                           QFrame *rowFrame,
                           QLabel *titleLabel,
                           QLabel *descriptionLabel,
                           QWidget *editor) = 0;
};

class AVERRA_EXPORT AverraPropertyGridEditors
{
public:
    static QLabel *createReadOnlyLabel(const QString &text, QWidget *parent = nullptr);
    static AverraSwitch *createBooleanEditor(bool checked, QWidget *parent = nullptr);
    static AverraComboBox *createEnumEditor(const QStringList &items,
                                            int currentIndex = 0,
                                            QWidget *parent = nullptr);
    static AverraNumberBox *createNumberEditor(int minimum,
                                               int maximum,
                                               int value = 0,
                                               QWidget *parent = nullptr);
};

class AVERRA_EXPORT AverraPropertyGrid : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    Q_PROPERTY(bool searchEnabled READ searchEnabled WRITE setSearchEnabled NOTIFY searchEnabledChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(QVariantMap dataModel READ dataModel WRITE setDataModel NOTIFY dataModelChanged)
    Q_PROPERTY(AverraFormModel *formModel READ formModel WRITE setFormModel NOTIFY formModelChanged)
    Q_PROPERTY(QString settingsKey READ settingsKey WRITE setSettingsKey NOTIFY settingsKeyChanged)

public:
    explicit AverraPropertyGrid(QWidget *parent = nullptr);

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

    QVariantMap dataModel() const;
    void setDataModel(const QVariantMap &dataModel);
    QVariant value(const QString &key) const;
    void setValue(const QString &key, const QVariant &value);
    AverraFormModel *formModel() const;
    void setFormModel(AverraFormModel *formModel);
    QString settingsKey() const;
    void setSettingsKey(const QString &settingsKey);

    int groupCount() const;
    int rowCount() const;
    AverraSearchBar *searchBar() const;
    AverraPropertyGridDelegate *delegate() const;
    void setDelegate(AverraPropertyGridDelegate *delegate);
    int addGroup(const QString &title, const QString &description = QString());
    int addRow(const QString &title, QWidget *editor, const QString &description = QString());
    int addRowToGroup(int groupIndex, const QString &title, QWidget *editor, const QString &description = QString());
    bool bindRow(int groupIndex, int rowIndex, const QString &key);
    int addReadOnlyRow(const QString &title, const QString &value, const QString &description = QString());
    int addReadOnlyRowToGroup(int groupIndex,
                              const QString &title,
                              const QString &value,
                              const QString &description = QString());
    int addBooleanRow(const QString &title, bool checked, const QString &description = QString());
    int addBooleanRowToGroup(int groupIndex,
                             const QString &title,
                             bool checked,
                             const QString &description = QString());
    int addEnumRow(const QString &title,
                   const QStringList &items,
                   int currentIndex = 0,
                   const QString &description = QString());
    int addEnumRowToGroup(int groupIndex,
                          const QString &title,
                          const QStringList &items,
                          int currentIndex = 0,
                          const QString &description = QString());
    int addNumberRow(const QString &title,
                     int minimum,
                     int maximum,
                     int value = 0,
                     const QString &description = QString());
    int addNumberRowToGroup(int groupIndex,
                            const QString &title,
                            int minimum,
                            int maximum,
                            int value = 0,
                            const QString &description = QString());
    AverraValidationResult validationResult() const;
    void applyValidationResult(const AverraValidationResult &validationResult);
    void rebuildFromFormModel();
    bool matchesFilterText(const QString &pattern) const;
    bool saveState(QSettings &settings) const;
    bool restoreState(QSettings &settings);
    void clearRows();
    void clearGroups();

signals:
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void filterTextChanged(const QString &filterText);
    void searchEnabledChanged(bool searchEnabled);
    void readOnlyChanged(bool readOnly);
    void dataModelChanged(const QVariantMap &dataModel);
    void formModelChanged(AverraFormModel *formModel);
    void settingsKeyChanged(const QString &settingsKey);
    void boundValueChanged(const QString &key, const QVariant &value);
    void validationResultChanged(const AverraValidationResult &validationResult);

private slots:
    void initialize();
    void refreshStyle();
    void applyFilter();

private:
    struct GridRow
    {
        QFrame *frame;
        QLabel *titleLabel;
        QLabel *descriptionLabel;
        QLabel *validationLabel;
        QWidget *editor;
        QString title;
        QString description;
        QString modelKey;
        AverraFieldDefinition fieldDefinition;
        AverraValidationIssue::Severity validationSeverity;
        QString validationMessage;
        bool fieldReadOnly;
    };

    struct GridGroup
    {
        QFrame *frame;
        QLabel *titleLabel;
        QLabel *descriptionLabel;
        QWidget *rowsWidget;
        QVBoxLayout *rowsLayout;
        QList<GridRow> rows;
        QString title;
        QString description;
    };

    bool groupMatches(const GridGroup &group, const QString &pattern) const;
    bool rowMatches(const GridRow &row, const QString &pattern) const;
    bool widgetMatches(QWidget *widget, const QString &pattern) const;
    QWidget *createEditorForField(const AverraFieldDefinition &fieldDefinition) const;
    void syncFormModel();
    void connectEditorSignals(GridRow &row);
    QVariant editorValue(QWidget *editor) const;
    void setEditorValue(QWidget *editor, const QVariant &value);
    void syncEditorToModel(GridRow &row);
    void syncModelToEditors();
    void polishRow(GridGroup &group, GridRow &row);
    void syncReadOnlyState();
    void updateTextVisibility();
    void updateSearchVisibility();

    QString m_title;
    QString m_description;
    QString m_filterText;
    QString m_settingsKey;
    bool m_searchEnabled;
    bool m_readOnly;
    QVariantMap m_dataModel;
    AverraFormModel *m_formModel;
    AverraValidationResult m_validationResult;
    QFrame *m_rootFrame;
    QLabel *m_titleLabel;
    QLabel *m_descriptionLabel;
    AverraSearchBar *m_searchBar;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;
    QList<GridGroup> m_groups;
    AverraPropertyGridDelegate *m_delegate;
};

#endif // AVERRA_PROPERTY_GRID_H
