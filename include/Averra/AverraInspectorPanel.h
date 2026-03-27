#ifndef AVERRA_INSPECTOR_PANEL_H
#define AVERRA_INSPECTOR_PANEL_H

#include <Averra/AverraGlobal.h>

#include <QVariantMap>
#include <QWidget>

class AverraFormModel;
class AverraPropertyEditor;
class AverraPropertyGrid;
class AverraSearchBar;
class AverraInfoBar;
class QFrame;
class QPushButton;
class QLabel;
class QHBoxLayout;
class QSettings;
class QVBoxLayout;

class AVERRA_EXPORT AverraInspectorPanel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(ValidationLevel validationLevel READ validationLevel WRITE setValidationLevel NOTIFY validationLevelChanged)
    Q_PROPERTY(SubmitState submitState READ submitState WRITE setSubmitState NOTIFY submitStateChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(QString statusText READ statusText WRITE setStatusText NOTIFY statusTextChanged)
    Q_PROPERTY(QString validationTitle READ validationTitle WRITE setValidationTitle NOTIFY validationTitleChanged)
    Q_PROPERTY(QString validationDescription READ validationDescription WRITE setValidationDescription NOTIFY validationDescriptionChanged)
    Q_PROPERTY(bool validationVisible READ validationVisible WRITE setValidationVisible NOTIFY validationVisibleChanged)
    Q_PROPERTY(QString submitButtonText READ submitButtonText WRITE setSubmitButtonText NOTIFY submitButtonTextChanged)
    Q_PROPERTY(QString submitResultText READ submitResultText WRITE setSubmitResultText NOTIFY submitResultTextChanged)
    Q_PROPERTY(bool submitButtonVisible READ submitButtonVisible WRITE setSubmitButtonVisible NOTIFY submitButtonVisibleChanged)
    Q_PROPERTY(bool retryButtonVisible READ retryButtonVisible WRITE setRetryButtonVisible NOTIFY retryButtonVisibleChanged)
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    Q_PROPERTY(bool searchEnabled READ searchEnabled WRITE setSearchEnabled NOTIFY searchEnabledChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(AverraFormModel *formModel READ formModel WRITE setFormModel NOTIFY formModelChanged)
    Q_PROPERTY(bool rollbackOnFailureEnabled READ rollbackOnFailureEnabled WRITE setRollbackOnFailureEnabled NOTIFY rollbackOnFailureEnabledChanged)
    Q_PROPERTY(QString settingsKey READ settingsKey WRITE setSettingsKey NOTIFY settingsKeyChanged)

public:
    enum ValidationLevel
    {
        InfoLevel,
        SuccessLevel,
        WarningLevel,
        ErrorLevel
    };
    Q_ENUM(ValidationLevel)

    enum SubmitState
    {
        SubmitIdle,
        SubmitSubmitting,
        SubmitSucceeded,
        SubmitFailed
    };
    Q_ENUM(SubmitState)

    explicit AverraInspectorPanel(QWidget *parent = nullptr);

    ValidationLevel validationLevel() const;
    void setValidationLevel(ValidationLevel level);

    SubmitState submitState() const;
    void setSubmitState(SubmitState state);

    QString title() const;
    void setTitle(const QString &title);

    QString subtitle() const;
    void setSubtitle(const QString &subtitle);

    QString statusText() const;
    void setStatusText(const QString &statusText);

    QString validationTitle() const;
    void setValidationTitle(const QString &validationTitle);

    QString validationDescription() const;
    void setValidationDescription(const QString &validationDescription);

    bool validationVisible() const;
    void setValidationVisible(bool validationVisible);

    QString submitButtonText() const;
    void setSubmitButtonText(const QString &submitButtonText);

    QString submitResultText() const;
    void setSubmitResultText(const QString &submitResultText);

    bool submitButtonVisible() const;
    void setSubmitButtonVisible(bool submitButtonVisible);

    bool retryButtonVisible() const;
    void setRetryButtonVisible(bool retryButtonVisible);

    QString filterText() const;
    void setFilterText(const QString &filterText);

    bool searchEnabled() const;
    void setSearchEnabled(bool searchEnabled);

    bool readOnly() const;
    void setReadOnly(bool readOnly);
    AverraFormModel *formModel() const;
    void setFormModel(AverraFormModel *formModel);
    bool rollbackOnFailureEnabled() const;
    void setRollbackOnFailureEnabled(bool rollbackOnFailureEnabled);
    QString settingsKey() const;
    void setSettingsKey(const QString &settingsKey);

    AverraSearchBar *searchBar() const;
    AverraInfoBar *validationBar() const;
    AverraPropertyEditor *propertyEditor() const;
    AverraPropertyGrid *propertyGrid() const;
    QHBoxLayout *actionsLayout() const;
    QHBoxLayout *footerLayout() const;
    QPushButton *submitButton() const;
    QPushButton *retryButton() const;
    void addActionWidget(QWidget *widget);
    void addFooterWidget(QWidget *widget);
    void addSection(QWidget *widget);
    void clearSections();
    void triggerSubmit();
    void completeSubmitSuccess(const QString &resultText = QString());
    void completeSubmitFailure(const QString &resultText,
                               const QString &validationTitle = QString(),
                               const QString &validationDescription = QString(),
                               ValidationLevel level = ErrorLevel);
    void resetSubmitState();
    void triggerRetry();
    void loadObject(const QString &objectId, const QVariantMap &values);
    bool saveState(QSettings &settings) const;
    bool restoreState(QSettings &settings);

signals:
    void validationLevelChanged(ValidationLevel level);
    void submitStateChanged(SubmitState state);
    void titleChanged(const QString &title);
    void subtitleChanged(const QString &subtitle);
    void statusTextChanged(const QString &statusText);
    void validationTitleChanged(const QString &validationTitle);
    void validationDescriptionChanged(const QString &validationDescription);
    void validationVisibleChanged(bool validationVisible);
    void submitButtonTextChanged(const QString &submitButtonText);
    void submitResultTextChanged(const QString &submitResultText);
    void submitButtonVisibleChanged(bool submitButtonVisible);
    void retryButtonVisibleChanged(bool retryButtonVisible);
    void filterTextChanged(const QString &filterText);
    void searchEnabledChanged(bool searchEnabled);
    void readOnlyChanged(bool readOnly);
    void formModelChanged(AverraFormModel *formModel);
    void rollbackOnFailureEnabledChanged(bool rollbackOnFailureEnabled);
    void settingsKeyChanged(const QString &settingsKey);
    void submitRequested();
    void formSubmitRequested(const QString &objectId, const QVariantMap &values);
    void retryRequested();

private slots:
    void initialize();
    void refreshStyle();

private:
    void syncValidationFromFormModel();
    void updateTextVisibility();
    void updateFooterVisibility();
    void updateEditorMode();

    ValidationLevel m_validationLevel;
    SubmitState m_submitState;
    QString m_title;
    QString m_subtitle;
    QString m_statusText;
    QString m_validationTitle;
    QString m_validationDescription;
    QString m_submitButtonText;
    QString m_submitResultText;
    QString m_settingsKey;
    bool m_validationVisible;
    bool m_submitButtonVisible;
    bool m_retryButtonVisible;
    bool m_rollbackOnFailureEnabled;
    int m_customFooterWidgetCount;
    QFrame *m_rootFrame;
    QFrame *m_statusBarFrame;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QLabel *m_statusLabel;
    QWidget *m_actionsWidget;
    QHBoxLayout *m_actionsLayout;
    AverraInfoBar *m_validationBar;
    QWidget *m_footerWidget;
    QHBoxLayout *m_footerLayout;
    QLabel *m_submitResultLabel;
    QPushButton *m_submitButton;
    QPushButton *m_retryButton;
    AverraPropertyEditor *m_propertyEditor;
    AverraPropertyGrid *m_propertyGrid;
    AverraFormModel *m_formModel;
};

#endif // AVERRA_INSPECTOR_PANEL_H
