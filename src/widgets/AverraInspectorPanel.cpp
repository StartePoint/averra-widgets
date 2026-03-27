#include <Averra/AverraInspectorPanel.h>
#include <Averra/AverraFormModel.h>
#include <Averra/AverraInfoBar.h>
#include <Averra/AverraPropertyEditor.h>
#include <Averra/AverraPropertyGrid.h>
#include <Averra/AverraSearchBar.h>
#include <Averra/AverraThemeManager.h>

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

namespace
{
AverraInfoBar::Type infoBarTypeForValidationLevel(AverraInspectorPanel::ValidationLevel level)
{
    switch (level) {
    case AverraInspectorPanel::InfoLevel:
        return AverraInfoBar::Info;
    case AverraInspectorPanel::SuccessLevel:
        return AverraInfoBar::Success;
    case AverraInspectorPanel::WarningLevel:
        return AverraInfoBar::Warning;
    case AverraInspectorPanel::ErrorLevel:
        return AverraInfoBar::Error;
    }

    return AverraInfoBar::Warning;
}

AverraInspectorPanel::ValidationLevel validationLevelForSeverity(AverraValidationIssue::Severity severity)
{
    switch (severity) {
    case AverraValidationIssue::InfoSeverity:
        return AverraInspectorPanel::InfoLevel;
    case AverraValidationIssue::SuccessSeverity:
        return AverraInspectorPanel::SuccessLevel;
    case AverraValidationIssue::WarningSeverity:
        return AverraInspectorPanel::WarningLevel;
    case AverraValidationIssue::ErrorSeverity:
        return AverraInspectorPanel::ErrorLevel;
    }

    return AverraInspectorPanel::WarningLevel;
}
}

AverraInspectorPanel::AverraInspectorPanel(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

AverraInspectorPanel::ValidationLevel AverraInspectorPanel::validationLevel() const
{
    return m_validationLevel;
}

void AverraInspectorPanel::setValidationLevel(ValidationLevel level)
{
    if (m_validationLevel == level) {
        return;
    }

    m_validationLevel = level;
    m_validationBar->setType(infoBarTypeForValidationLevel(m_validationLevel));
    emit validationLevelChanged(m_validationLevel);
}

AverraInspectorPanel::SubmitState AverraInspectorPanel::submitState() const
{
    return m_submitState;
}

void AverraInspectorPanel::setSubmitState(SubmitState state)
{
    if (m_submitState == state) {
        return;
    }

    m_submitState = state;

    if (m_submitState == SubmitSubmitting) {
        m_submitButton->setEnabled(false);
    } else {
        m_submitButton->setEnabled(true);
    }

    updateFooterVisibility();
    refreshStyle();
    emit submitStateChanged(m_submitState);
}

QString AverraInspectorPanel::title() const
{
    return m_title;
}

void AverraInspectorPanel::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraInspectorPanel::subtitle() const
{
    return m_subtitle;
}

void AverraInspectorPanel::setSubtitle(const QString &subtitle)
{
    if (m_subtitle == subtitle) {
        return;
    }

    m_subtitle = subtitle;
    m_subtitleLabel->setText(m_subtitle);
    updateTextVisibility();
    emit subtitleChanged(m_subtitle);
}

QString AverraInspectorPanel::statusText() const
{
    return m_statusText;
}

void AverraInspectorPanel::setStatusText(const QString &statusText)
{
    if (m_statusText == statusText) {
        return;
    }

    m_statusText = statusText;
    m_statusLabel->setText(m_statusText);
    updateTextVisibility();
    emit statusTextChanged(m_statusText);
}

QString AverraInspectorPanel::validationTitle() const
{
    return m_validationTitle;
}

void AverraInspectorPanel::setValidationTitle(const QString &validationTitle)
{
    if (m_validationTitle == validationTitle) {
        return;
    }

    m_validationTitle = validationTitle;
    m_validationBar->setTitle(m_validationTitle);
    emit validationTitleChanged(m_validationTitle);
}

QString AverraInspectorPanel::validationDescription() const
{
    return m_validationDescription;
}

void AverraInspectorPanel::setValidationDescription(const QString &validationDescription)
{
    if (m_validationDescription == validationDescription) {
        return;
    }

    m_validationDescription = validationDescription;
    m_validationBar->setDescription(m_validationDescription);
    emit validationDescriptionChanged(m_validationDescription);
}

bool AverraInspectorPanel::validationVisible() const
{
    return m_validationVisible;
}

void AverraInspectorPanel::setValidationVisible(bool validationVisible)
{
    if (m_validationVisible == validationVisible) {
        return;
    }

    m_validationVisible = validationVisible;
    m_validationBar->setVisible(m_validationVisible);
    emit validationVisibleChanged(m_validationVisible);
}

QString AverraInspectorPanel::submitButtonText() const
{
    return m_submitButtonText;
}

void AverraInspectorPanel::setSubmitButtonText(const QString &submitButtonText)
{
    if (m_submitButtonText == submitButtonText) {
        return;
    }

    m_submitButtonText = submitButtonText;
    m_submitButton->setText(m_submitButtonText);
    emit submitButtonTextChanged(m_submitButtonText);
}

QString AverraInspectorPanel::submitResultText() const
{
    return m_submitResultText;
}

void AverraInspectorPanel::setSubmitResultText(const QString &submitResultText)
{
    if (m_submitResultText == submitResultText) {
        return;
    }

    m_submitResultText = submitResultText;
    m_submitResultLabel->setText(m_submitResultText);
    updateFooterVisibility();
    emit submitResultTextChanged(m_submitResultText);
}

bool AverraInspectorPanel::submitButtonVisible() const
{
    return m_submitButtonVisible;
}

void AverraInspectorPanel::setSubmitButtonVisible(bool submitButtonVisible)
{
    if (m_submitButtonVisible == submitButtonVisible) {
        return;
    }

    m_submitButtonVisible = submitButtonVisible;
    m_submitButton->setVisible(m_submitButtonVisible);
    updateFooterVisibility();
    emit submitButtonVisibleChanged(m_submitButtonVisible);
}

bool AverraInspectorPanel::retryButtonVisible() const
{
    return m_retryButtonVisible;
}

void AverraInspectorPanel::setRetryButtonVisible(bool retryButtonVisible)
{
    if (m_retryButtonVisible == retryButtonVisible) {
        return;
    }

    m_retryButtonVisible = retryButtonVisible;
    m_retryButton->setVisible(m_retryButtonVisible);
    updateFooterVisibility();
    emit retryButtonVisibleChanged(m_retryButtonVisible);
}

QString AverraInspectorPanel::filterText() const
{
    if (m_formModel != nullptr) {
        return m_propertyGrid->filterText();
    }

    return m_propertyEditor->filterText();
}

void AverraInspectorPanel::setFilterText(const QString &filterText)
{
    if (this->filterText() == filterText) {
        return;
    }

    m_propertyEditor->setFilterText(filterText);
    m_propertyGrid->setFilterText(filterText);
    emit filterTextChanged(filterText);
}

bool AverraInspectorPanel::searchEnabled() const
{
    if (m_formModel != nullptr) {
        return m_propertyGrid->searchEnabled();
    }

    return m_propertyEditor->searchEnabled();
}

void AverraInspectorPanel::setSearchEnabled(bool searchEnabled)
{
    if (this->searchEnabled() == searchEnabled) {
        return;
    }

    m_propertyEditor->setSearchEnabled(searchEnabled);
    m_propertyGrid->setSearchEnabled(searchEnabled);
    emit searchEnabledChanged(searchEnabled);
}

bool AverraInspectorPanel::readOnly() const
{
    if (m_formModel != nullptr) {
        return m_propertyGrid->readOnly();
    }

    return m_propertyEditor->readOnly();
}

void AverraInspectorPanel::setReadOnly(bool readOnly)
{
    if (this->readOnly() == readOnly) {
        return;
    }

    m_propertyEditor->setReadOnly(readOnly);
    m_propertyGrid->setReadOnly(readOnly);
    emit readOnlyChanged(readOnly);
}

AverraFormModel *AverraInspectorPanel::formModel() const
{
    return m_formModel;
}

void AverraInspectorPanel::setFormModel(AverraFormModel *formModel)
{
    if (m_formModel == formModel) {
        return;
    }

    if (m_formModel != nullptr) {
        disconnect(m_formModel, nullptr, this, nullptr);
    }

    m_formModel = formModel;
    m_propertyGrid->setFormModel(m_formModel);
    updateEditorMode();

    if (m_formModel != nullptr) {
        connect(m_formModel,
                &AverraFormModel::validationChanged,
                this,
                [this](const AverraValidationResult &) {
                    syncValidationFromFormModel();
                });
        connect(m_formModel,
                &AverraFormModel::readOnlyChanged,
                this,
                [this](bool) {
                    emit readOnlyChanged(readOnly());
                });
        connect(m_formModel,
                &QObject::destroyed,
                this,
                [this, formModel]() {
                    if (m_formModel == formModel) {
                        m_formModel = nullptr;
                        m_propertyGrid->setFormModel(nullptr);
                        updateEditorMode();
                        emit formModelChanged(nullptr);
                    }
                });

        syncValidationFromFormModel();
    } else {
        setValidationVisible(false);
    }

    emit formModelChanged(m_formModel);
}

bool AverraInspectorPanel::rollbackOnFailureEnabled() const
{
    return m_rollbackOnFailureEnabled;
}

void AverraInspectorPanel::setRollbackOnFailureEnabled(bool rollbackOnFailureEnabled)
{
    if (m_rollbackOnFailureEnabled == rollbackOnFailureEnabled) {
        return;
    }

    m_rollbackOnFailureEnabled = rollbackOnFailureEnabled;
    emit rollbackOnFailureEnabledChanged(m_rollbackOnFailureEnabled);
}

QString AverraInspectorPanel::settingsKey() const
{
    return m_settingsKey;
}

void AverraInspectorPanel::setSettingsKey(const QString &settingsKey)
{
    if (m_settingsKey == settingsKey) {
        return;
    }

    m_settingsKey = settingsKey;
    m_propertyGrid->setSettingsKey(m_settingsKey + QStringLiteral("/propertyGrid"));
    emit settingsKeyChanged(m_settingsKey);
}

AverraSearchBar *AverraInspectorPanel::searchBar() const
{
    if (m_formModel != nullptr) {
        return m_propertyGrid->searchBar();
    }

    return m_propertyEditor->searchBar();
}

AverraInfoBar *AverraInspectorPanel::validationBar() const
{
    return m_validationBar;
}

AverraPropertyEditor *AverraInspectorPanel::propertyEditor() const
{
    return m_propertyEditor;
}

AverraPropertyGrid *AverraInspectorPanel::propertyGrid() const
{
    return m_propertyGrid;
}

QHBoxLayout *AverraInspectorPanel::actionsLayout() const
{
    return m_actionsLayout;
}

QHBoxLayout *AverraInspectorPanel::footerLayout() const
{
    return m_footerLayout;
}

QPushButton *AverraInspectorPanel::submitButton() const
{
    return m_submitButton;
}

QPushButton *AverraInspectorPanel::retryButton() const
{
    return m_retryButton;
}

void AverraInspectorPanel::addActionWidget(QWidget *widget)
{
    if (widget != nullptr) {
        m_actionsLayout->addWidget(widget);
    }
}

void AverraInspectorPanel::addFooterWidget(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }

    m_footerLayout->insertWidget(qMax(0, m_footerLayout->count() - 2), widget);
    m_customFooterWidgetCount += 1;
    updateFooterVisibility();
}

void AverraInspectorPanel::addSection(QWidget *widget)
{
    m_propertyEditor->addSection(widget);
}

void AverraInspectorPanel::clearSections()
{
    m_propertyEditor->clearSections();
}

void AverraInspectorPanel::triggerSubmit()
{
    if (!m_submitButtonVisible || m_submitState == SubmitSubmitting) {
        return;
    }

    if (m_formModel != nullptr) {
        m_formModel->refreshValidation();
        syncValidationFromFormModel();

        if (!m_formModel->validationResult().isValid()) {
            setSubmitResultText(QStringLiteral("校验未通过"));
            setRetryButtonVisible(false);
            setSubmitState(SubmitFailed);
            return;
        }
    }

    setSubmitState(SubmitSubmitting);
    emit submitRequested();

    if (m_formModel != nullptr) {
        emit formSubmitRequested(m_formModel->objectId(), m_formModel->values());
    }
}

void AverraInspectorPanel::completeSubmitSuccess(const QString &resultText)
{
    if (m_formModel != nullptr) {
        m_formModel->commit();
        syncValidationFromFormModel();
    }

    setSubmitResultText(resultText);
    setRetryButtonVisible(false);
    setSubmitState(SubmitSucceeded);
}

void AverraInspectorPanel::completeSubmitFailure(const QString &resultText,
                                                 const QString &validationTitle,
                                                 const QString &validationDescription,
                                                 ValidationLevel level)
{
    if (m_formModel != nullptr && m_rollbackOnFailureEnabled) {
        m_formModel->rollback();
    }

    if (!validationTitle.isEmpty()) {
        setValidationTitle(validationTitle);
    }

    if (!validationDescription.isEmpty()) {
        setValidationDescription(validationDescription);
    }

    setValidationLevel(level);
    setValidationVisible(true);
    setSubmitResultText(resultText);
    setRetryButtonVisible(true);
    setSubmitState(SubmitFailed);
}

void AverraInspectorPanel::resetSubmitState()
{
    if (m_formModel != nullptr) {
        syncValidationFromFormModel();
    }

    setSubmitResultText(QString());
    setRetryButtonVisible(false);
    setSubmitState(SubmitIdle);
}

void AverraInspectorPanel::triggerRetry()
{
    resetSubmitState();
    emit retryRequested();
}

void AverraInspectorPanel::loadObject(const QString &objectId, const QVariantMap &values)
{
    if (m_formModel == nullptr) {
        return;
    }

    m_formModel->loadObject(objectId, values);
    syncValidationFromFormModel();
}

bool AverraInspectorPanel::saveState(QSettings &settings) const
{
    if (m_settingsKey.trimmed().isEmpty()) {
        return false;
    }

    settings.setValue(m_settingsKey + QStringLiteral("/filterText"), filterText());

    if (m_formModel != nullptr) {
        return m_formModel->saveState(settings, m_settingsKey + QStringLiteral("/formModel"));
    }

    return true;
}

bool AverraInspectorPanel::restoreState(QSettings &settings)
{
    if (m_settingsKey.trimmed().isEmpty()) {
        return false;
    }

    setFilterText(settings.value(m_settingsKey + QStringLiteral("/filterText")).toString());

    if (m_formModel != nullptr) {
        return m_formModel->restoreState(settings, m_settingsKey + QStringLiteral("/formModel"));
    }

    return true;
}

void AverraInspectorPanel::initialize()
{
    m_validationLevel = WarningLevel;
    m_submitState = SubmitIdle;
    m_title.clear();
    m_subtitle.clear();
    m_statusText.clear();
    m_validationTitle.clear();
    m_validationDescription.clear();
    m_submitButtonText = QStringLiteral("提交");
    m_submitResultText.clear();
    m_settingsKey.clear();
    m_validationVisible = false;
    m_submitButtonVisible = true;
    m_retryButtonVisible = false;
    m_rollbackOnFailureEnabled = true;
    m_customFooterWidgetCount = 0;
    m_rootFrame = new QFrame(this);
    m_statusBarFrame = new QFrame(m_rootFrame);
    m_titleLabel = new QLabel(m_rootFrame);
    m_subtitleLabel = new QLabel(m_rootFrame);
    m_statusLabel = new QLabel(m_statusBarFrame);
    m_actionsWidget = new QWidget(m_rootFrame);
    m_actionsLayout = new QHBoxLayout;
    m_validationBar = new AverraInfoBar(m_rootFrame);
    m_footerWidget = new QWidget(m_rootFrame);
    m_footerLayout = new QHBoxLayout;
    m_submitResultLabel = new QLabel(m_footerWidget);
    m_submitButton = new QPushButton(m_footerWidget);
    m_retryButton = new QPushButton(m_footerWidget);
    m_propertyEditor = new AverraPropertyEditor(m_rootFrame);
    m_propertyGrid = new AverraPropertyGrid(m_rootFrame);
    m_formModel = nullptr;

    m_rootFrame->setObjectName(QStringLiteral("AverraInspectorPanelRoot"));
    m_statusBarFrame->setObjectName(QStringLiteral("AverraInspectorPanelStatusBar"));
    m_titleLabel->setObjectName(QStringLiteral("AverraInspectorPanelTitle"));
    m_subtitleLabel->setObjectName(QStringLiteral("AverraInspectorPanelSubtitle"));
    m_statusLabel->setObjectName(QStringLiteral("AverraInspectorPanelStatus"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(12);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(12);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(3);

    m_actionsLayout->setContentsMargins(0, 0, 0, 0);
    m_actionsLayout->setSpacing(8);
    m_actionsWidget->setLayout(m_actionsLayout);

    QHBoxLayout *statusBarLayout = new QHBoxLayout(m_statusBarFrame);
    statusBarLayout->setContentsMargins(12, 8, 12, 8);
    statusBarLayout->setSpacing(0);
    statusBarLayout->addWidget(m_statusLabel);
    statusBarLayout->addStretch(1);

    m_footerLayout->setContentsMargins(0, 0, 0, 0);
    m_footerLayout->setSpacing(8);
    m_footerLayout->addStretch(1);
    m_footerLayout->addWidget(m_submitResultLabel);
    m_footerLayout->addWidget(m_retryButton);
    m_footerLayout->addWidget(m_submitButton);
    m_footerWidget->setLayout(m_footerLayout);

    textLayout->addWidget(m_titleLabel);
    textLayout->addWidget(m_subtitleLabel);

    m_propertyEditor->setTitle(QString());
    m_propertyEditor->setDescription(QString());
    m_propertyGrid->setTitle(QString());
    m_propertyGrid->setDescription(QString());
    m_propertyGrid->setVisible(false);
    m_validationBar->setType(infoBarTypeForValidationLevel(m_validationLevel));
    m_validationBar->setClosable(false);
    m_validationBar->setVisible(false);
    m_statusBarFrame->setVisible(false);
    m_retryButton->setText(QStringLiteral("重试"));
    m_retryButton->setVisible(m_retryButtonVisible);
    m_submitButton->setText(m_submitButtonText);
    m_submitButton->setVisible(m_submitButtonVisible);
    m_submitResultLabel->setVisible(false);
    m_footerWidget->setVisible(true);

    headerLayout->addLayout(textLayout, 1);
    headerLayout->addWidget(m_actionsWidget, 0, Qt::AlignTop);

    rootLayout->addLayout(headerLayout);
    rootLayout->addWidget(m_validationBar);
    rootLayout->addWidget(m_propertyEditor);
    rootLayout->addWidget(m_propertyGrid);
    rootLayout->addWidget(m_statusBarFrame);
    rootLayout->addWidget(m_footerWidget);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraInspectorPanel::refreshStyle);
    connect(m_submitButton,
            &QPushButton::clicked,
            this,
            &AverraInspectorPanel::triggerSubmit);
    connect(m_retryButton,
            &QPushButton::clicked,
            this,
            &AverraInspectorPanel::triggerRetry);

    updateTextVisibility();
    updateFooterVisibility();
    updateEditorMode();
    refreshStyle();
}

void AverraInspectorPanel::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();

    m_rootFrame->setStyleSheet(
        QStringLiteral(
            "QFrame#AverraInspectorPanelRoot {"
            "background-color: %1;"
            "border: 1px solid %2;"
            "border-radius: 18px;"
            "}")
            .arg(palette.surfaceColor().name(), palette.borderColor().name()));
    m_statusBarFrame->setStyleSheet(
        QStringLiteral(
            "QFrame#AverraInspectorPanelStatusBar {"
            "background-color: %1;"
            "border: 1px solid %2;"
            "border-radius: 12px;"
            "}")
            .arg(palette.surfaceRaisedColor().name(), palette.borderColor().name()));
    m_titleLabel->setStyleSheet(
        QStringLiteral(
            "QLabel#AverraInspectorPanelTitle {"
            "color: %1;"
            "font-size: 18px;"
            "font-weight: 700;"
            "background: transparent;"
            "}")
            .arg(palette.textPrimaryColor().name()));
    m_subtitleLabel->setStyleSheet(
        QStringLiteral(
            "QLabel#AverraInspectorPanelSubtitle {"
            "color: %1;"
            "font-size: 13px;"
            "font-weight: 500;"
            "background: transparent;"
            "}")
            .arg(palette.textSecondaryColor().name()));
    m_statusLabel->setStyleSheet(
        QStringLiteral(
            "QLabel#AverraInspectorPanelStatus {"
            "color: %1;"
            "background: transparent;"
            "padding: 0px;"
            "font-size: 12px;"
            "font-weight: 700;"
            "}")
            .arg(palette.accentColor().name()));
    m_submitResultLabel->setStyleSheet(
        QStringLiteral(
            "QLabel {"
            "color: %1;"
            "background: transparent;"
            "font-size: 12px;"
            "font-weight: 600;"
            "}")
            .arg(m_submitState == SubmitFailed ? palette.errorColor().name()
                                               : (m_submitState == SubmitSucceeded ? palette.successColor().name()
                                                                                  : palette.textSecondaryColor().name())));
    m_submitButton->setStyleSheet(
        QStringLiteral(
            "QPushButton {"
            "background-color: %1;"
            "color: #FFFFFF;"
            "border: 1px solid %1;"
            "border-radius: 10px;"
            "padding: 8px 14px;"
            "font-size: 12px;"
            "font-weight: 700;"
            "}"
            "QPushButton:disabled {"
            "background-color: %2;"
            "border-color: %2;"
            "color: %3;"
            "}")
            .arg(palette.accentColor().name(),
                 palette.accentDisabledColor().name(),
                 palette.surfaceRaisedColor().name()));
    m_retryButton->setStyleSheet(
        QStringLiteral(
            "QPushButton {"
            "background-color: %1;"
            "color: %2;"
            "border: 1px solid %3;"
            "border-radius: 10px;"
            "padding: 8px 14px;"
            "font-size: 12px;"
            "font-weight: 700;"
            "}")
            .arg(palette.surfaceRaisedColor().name(),
                 palette.textPrimaryColor().name(),
                 palette.borderColor().name()));
}

void AverraInspectorPanel::syncValidationFromFormModel()
{
    if (m_formModel == nullptr) {
        return;
    }

    const AverraValidationResult validationResult = m_formModel->validationResult();
    setValidationLevel(validationLevelForSeverity(validationResult.highestSeverity()));
    setValidationTitle(validationResult.summaryTitle());
    setValidationDescription(validationResult.summaryDescription());
    setValidationVisible(validationResult.hasIssues());
}

void AverraInspectorPanel::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_subtitleLabel->setVisible(!m_subtitle.isEmpty());
    m_statusLabel->setVisible(!m_statusText.isEmpty());
    m_statusBarFrame->setVisible(!m_statusText.isEmpty());
}

void AverraInspectorPanel::updateFooterVisibility()
{
    const bool hasResultText = !m_submitResultText.trimmed().isEmpty();
    m_submitResultLabel->setVisible(hasResultText);
    m_submitButton->setVisible(m_submitButtonVisible);
    m_retryButton->setVisible(m_retryButtonVisible);
    m_footerWidget->setVisible(m_customFooterWidgetCount > 0
                               || m_submitButtonVisible
                               || m_retryButtonVisible
                               || hasResultText);
}

void AverraInspectorPanel::updateEditorMode()
{
    const bool useFormModelMode = m_formModel != nullptr;
    m_propertyEditor->setVisible(!useFormModelMode);
    m_propertyGrid->setVisible(useFormModelMode);
}
