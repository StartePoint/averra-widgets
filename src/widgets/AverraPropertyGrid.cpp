#include <Averra/AverraPropertyGrid.h>
#include <Averra/AverraComboBox.h>
#include <Averra/AverraFormModel.h>
#include <Averra/AverraLineEdit.h>
#include <Averra/AverraNumberBox.h>
#include <Averra/AverraSearchBar.h>
#include <Averra/AverraSwitch.h>
#include <Averra/AverraTextArea.h>
#include <Averra/AverraThemeManager.h>

#include <QAbstractButton>
#include <QFrame>
#include <QHash>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QLineEdit>
#include <QScrollArea>
#include <QSettings>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>

namespace
{
bool containsText(const QString &text, const QString &pattern)
{
    if (pattern.isEmpty()) {
        return true;
    }

    return text.contains(pattern, Qt::CaseInsensitive);
}

QString nonEmptyText(const QString &text)
{
    return text.trimmed();
}

QString normalizedGroupKey(const AverraFieldDefinition &fieldDefinition)
{
    if (!fieldDefinition.groupKey().trimmed().isEmpty()) {
        return fieldDefinition.groupKey().trimmed();
    }

    if (!fieldDefinition.groupTitle().trimmed().isEmpty()) {
        return fieldDefinition.groupTitle().trimmed();
    }

    return QStringLiteral("__default_group__");
}

QColor validationBorderColor(const AverraThemePalette &palette, AverraValidationIssue::Severity severity)
{
    switch (severity) {
    case AverraValidationIssue::InfoSeverity:
        return palette.accentColor();
    case AverraValidationIssue::SuccessSeverity:
        return palette.successColor();
    case AverraValidationIssue::WarningSeverity:
        return palette.warningColor();
    case AverraValidationIssue::ErrorSeverity:
        return palette.errorColor();
    }

    return palette.borderColor();
}

QColor validationSurfaceColor(const AverraThemePalette &palette, AverraValidationIssue::Severity severity)
{
    switch (severity) {
    case AverraValidationIssue::InfoSeverity:
        return palette.accentSurfaceColor();
    case AverraValidationIssue::SuccessSeverity:
        return palette.successSurfaceColor();
    case AverraValidationIssue::WarningSeverity:
        return palette.warningSurfaceColor();
    case AverraValidationIssue::ErrorSeverity:
        return palette.errorSurfaceColor();
    }

    return palette.surfaceRaisedColor();
}

QColor validationTextColor(const AverraThemePalette &palette, AverraValidationIssue::Severity severity)
{
    switch (severity) {
    case AverraValidationIssue::InfoSeverity:
        return palette.accentColor();
    case AverraValidationIssue::SuccessSeverity:
        return palette.successColor();
    case AverraValidationIssue::WarningSeverity:
        return palette.warningColor();
    case AverraValidationIssue::ErrorSeverity:
        return palette.errorColor();
    }

    return palette.textSecondaryColor();
}
}

AverraPropertyGrid::AverraPropertyGrid(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QLabel *AverraPropertyGridEditors::createReadOnlyLabel(const QString &text, QWidget *parent)
{
    QLabel *label = new QLabel(parent);
    label->setText(text);
    label->setWordWrap(true);
    return label;
}

AverraSwitch *AverraPropertyGridEditors::createBooleanEditor(bool checked, QWidget *parent)
{
    AverraSwitch *editor = new AverraSwitch(parent);
    editor->setChecked(checked);
    return editor;
}

AverraComboBox *AverraPropertyGridEditors::createEnumEditor(const QStringList &items,
                                                            int currentIndex,
                                                            QWidget *parent)
{
    AverraComboBox *editor = new AverraComboBox(parent);
    editor->addItems(items);

    if (currentIndex >= 0 && currentIndex < editor->count()) {
        editor->setCurrentIndex(currentIndex);
    }

    return editor;
}

AverraNumberBox *AverraPropertyGridEditors::createNumberEditor(int minimum,
                                                               int maximum,
                                                               int value,
                                                               QWidget *parent)
{
    AverraNumberBox *editor = new AverraNumberBox(parent);
    editor->setRange(minimum, maximum);
    editor->setValue(value);
    return editor;
}

QString AverraPropertyGrid::title() const
{
    return m_title;
}

void AverraPropertyGrid::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraPropertyGrid::description() const
{
    return m_description;
}

void AverraPropertyGrid::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

QString AverraPropertyGrid::filterText() const
{
    return m_filterText;
}

void AverraPropertyGrid::setFilterText(const QString &filterText)
{
    if (m_filterText == filterText) {
        return;
    }

    m_filterText = filterText;

    if (m_searchBar->text() != m_filterText) {
        m_searchBar->setText(m_filterText);
    }

    applyFilter();
    emit filterTextChanged(m_filterText);
}

bool AverraPropertyGrid::searchEnabled() const
{
    return m_searchEnabled;
}

void AverraPropertyGrid::setSearchEnabled(bool searchEnabled)
{
    if (m_searchEnabled == searchEnabled) {
        return;
    }

    m_searchEnabled = searchEnabled;
    updateSearchVisibility();
    emit searchEnabledChanged(m_searchEnabled);
}

bool AverraPropertyGrid::readOnly() const
{
    return m_readOnly || (m_formModel != nullptr && m_formModel->readOnly());
}

void AverraPropertyGrid::setReadOnly(bool readOnly)
{
    if (m_readOnly == readOnly) {
        return;
    }

    m_readOnly = readOnly;
    syncReadOnlyState();
    emit readOnlyChanged(m_readOnly);
}

QVariantMap AverraPropertyGrid::dataModel() const
{
    return m_dataModel;
}

void AverraPropertyGrid::setDataModel(const QVariantMap &dataModel)
{
    if (m_dataModel == dataModel) {
        return;
    }

    m_dataModel = dataModel;
    syncModelToEditors();

    if (m_formModel != nullptr && m_formModel->values() != m_dataModel) {
        m_formModel->setValues(m_dataModel);
    }

    emit dataModelChanged(m_dataModel);
}

QVariant AverraPropertyGrid::value(const QString &key) const
{
    return m_dataModel.value(key);
}

void AverraPropertyGrid::setValue(const QString &key, const QVariant &value)
{
    if (key.trimmed().isEmpty()) {
        return;
    }

    if (m_dataModel.value(key) == value) {
        return;
    }

    m_dataModel.insert(key, value);
    syncModelToEditors();

    if (m_formModel != nullptr) {
        m_formModel->setValue(key, value);
    }

    emit boundValueChanged(key, value);
    emit dataModelChanged(m_dataModel);
}

AverraFormModel *AverraPropertyGrid::formModel() const
{
    return m_formModel;
}

void AverraPropertyGrid::setFormModel(AverraFormModel *formModel)
{
    if (m_formModel == formModel) {
        return;
    }

    if (m_formModel != nullptr) {
        disconnect(m_formModel, nullptr, this, nullptr);
    }

    m_formModel = formModel;

    if (m_formModel != nullptr) {
        connect(m_formModel,
                &AverraFormModel::fieldsChanged,
                this,
                &AverraPropertyGrid::rebuildFromFormModel);
        connect(m_formModel,
                &AverraFormModel::valuesChanged,
                this,
                [this](const QVariantMap &values) {
                    m_dataModel = values;
                    syncModelToEditors();
                    emit dataModelChanged(m_dataModel);
                });
        connect(m_formModel,
                &AverraFormModel::validationChanged,
                this,
                &AverraPropertyGrid::applyValidationResult);
        connect(m_formModel,
                &AverraFormModel::readOnlyChanged,
                this,
                [this](bool) {
                    syncReadOnlyState();
                });
        connect(m_formModel,
                &QObject::destroyed,
                this,
                [this, formModel]() {
                    if (m_formModel == formModel) {
                        m_formModel = nullptr;
                        emit formModelChanged(nullptr);
                    }
                });

        m_dataModel = m_formModel->values();
        rebuildFromFormModel();
        applyValidationResult(m_formModel->validationResult());
        emit dataModelChanged(m_dataModel);
    }

    syncReadOnlyState();
    emit formModelChanged(m_formModel);
}

QString AverraPropertyGrid::settingsKey() const
{
    return m_settingsKey;
}

void AverraPropertyGrid::setSettingsKey(const QString &settingsKey)
{
    if (m_settingsKey == settingsKey) {
        return;
    }

    m_settingsKey = settingsKey;
    emit settingsKeyChanged(m_settingsKey);
}

int AverraPropertyGrid::groupCount() const
{
    return m_groups.size();
}

int AverraPropertyGrid::rowCount() const
{
    int count = 0;

    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        count += m_groups.at(groupIndex).rows.size();
    }

    return count;
}

AverraSearchBar *AverraPropertyGrid::searchBar() const
{
    return m_searchBar;
}

AverraPropertyGridDelegate *AverraPropertyGrid::delegate() const
{
    return m_delegate;
}

void AverraPropertyGrid::setDelegate(AverraPropertyGridDelegate *delegate)
{
    if (m_delegate == delegate) {
        return;
    }

    m_delegate = delegate;

    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        GridGroup &group = m_groups[groupIndex];

        for (int rowIndex = 0; rowIndex < group.rows.size(); ++rowIndex) {
            polishRow(group, group.rows[rowIndex]);
        }
    }
}

int AverraPropertyGrid::addGroup(const QString &title, const QString &description)
{
    GridGroup group;
    group.frame = new QFrame(m_contentWidget);
    group.titleLabel = new QLabel(group.frame);
    group.descriptionLabel = new QLabel(group.frame);
    group.rowsWidget = new QWidget(group.frame);
    group.rowsLayout = new QVBoxLayout;
    group.title = title;
    group.description = description;

    group.frame->setObjectName(QStringLiteral("AverraPropertyGridGroup"));
    group.titleLabel->setObjectName(QStringLiteral("AverraPropertyGridGroupTitle"));
    group.descriptionLabel->setObjectName(QStringLiteral("AverraPropertyGridGroupDescription"));

    group.titleLabel->setWordWrap(true);
    group.descriptionLabel->setWordWrap(true);
    group.titleLabel->setText(group.title);
    group.descriptionLabel->setText(group.description);
    group.titleLabel->setVisible(!nonEmptyText(group.title).isEmpty());
    group.descriptionLabel->setVisible(!nonEmptyText(group.description).isEmpty());

    QVBoxLayout *groupLayout = new QVBoxLayout(group.frame);
    groupLayout->setContentsMargins(0, 0, 0, 0);
    groupLayout->setSpacing(10);

    group.rowsLayout->setContentsMargins(0, 0, 0, 0);
    group.rowsLayout->setSpacing(10);
    group.rowsWidget->setLayout(group.rowsLayout);

    groupLayout->addWidget(group.titleLabel);
    groupLayout->addWidget(group.descriptionLabel);
    groupLayout->addWidget(group.rowsWidget);

    m_contentLayout->insertWidget(m_contentLayout->count() - 1, group.frame);
    m_groups.append(group);

    refreshStyle();
    applyFilter();
    return m_groups.size() - 1;
}

int AverraPropertyGrid::addRow(const QString &title, QWidget *editor, const QString &description)
{
    if (m_groups.isEmpty()) {
        addGroup(QString(), QString());
    }

    return addRowToGroup(0, title, editor, description);
}

int AverraPropertyGrid::addRowToGroup(int groupIndex,
                                      const QString &title,
                                      QWidget *editor,
                                      const QString &description)
{
    if (groupIndex < 0 || groupIndex >= m_groups.size()) {
        return -1;
    }

    if (editor == nullptr) {
        return -1;
    }

    GridRow row;
    GridGroup &group = m_groups[groupIndex];

    row.frame = new QFrame(group.rowsWidget);
    row.titleLabel = new QLabel(row.frame);
    row.descriptionLabel = new QLabel(row.frame);
    row.validationLabel = new QLabel(row.frame);
    row.editor = editor;
    row.title = title;
    row.description = description;
    row.validationSeverity = AverraValidationIssue::SuccessSeverity;
    row.fieldReadOnly = false;

    row.frame->setObjectName(QStringLiteral("AverraPropertyGridRow"));
    row.titleLabel->setObjectName(QStringLiteral("AverraPropertyGridRowTitle"));
    row.descriptionLabel->setObjectName(QStringLiteral("AverraPropertyGridRowDescription"));
    row.validationLabel->setObjectName(QStringLiteral("AverraPropertyGridRowValidation"));

    row.titleLabel->setWordWrap(true);
    row.descriptionLabel->setWordWrap(true);
    row.validationLabel->setWordWrap(true);
    row.titleLabel->setText(row.title);
    row.descriptionLabel->setText(row.description);
    row.descriptionLabel->setVisible(!nonEmptyText(row.description).isEmpty());
    row.validationLabel->setVisible(false);
    row.editor->setParent(row.frame);
    row.editor->setEnabled(!m_readOnly);

    QHBoxLayout *rowLayout = new QHBoxLayout(row.frame);
    rowLayout->setContentsMargins(14, 12, 14, 12);
    rowLayout->setSpacing(16);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(3);
    textLayout->addWidget(row.titleLabel);
    textLayout->addWidget(row.descriptionLabel);
    textLayout->addWidget(row.validationLabel);

    rowLayout->addLayout(textLayout, 1);
    rowLayout->addWidget(row.editor, 0, Qt::AlignVCenter);

    group.rowsLayout->addWidget(row.frame);
    group.rows.append(row);
    connectEditorSignals(group.rows.last());

    polishRow(group, group.rows.last());

    refreshStyle();
    applyFilter();
    return group.rows.size() - 1;
}

bool AverraPropertyGrid::bindRow(int groupIndex, int rowIndex, const QString &key)
{
    if (groupIndex < 0 || groupIndex >= m_groups.size()) {
        return false;
    }

    GridGroup &group = m_groups[groupIndex];

    if (rowIndex < 0 || rowIndex >= group.rows.size()) {
        return false;
    }

    GridRow &row = group.rows[rowIndex];
    row.modelKey = key.trimmed();

    if (row.modelKey.isEmpty()) {
        return false;
    }

    row.editor->setProperty("averra_model_key", row.modelKey);
    syncEditorToModel(row);
    return true;
}

int AverraPropertyGrid::addReadOnlyRow(const QString &title, const QString &value, const QString &description)
{
    return addRow(title, AverraPropertyGridEditors::createReadOnlyLabel(value), description);
}

int AverraPropertyGrid::addReadOnlyRowToGroup(int groupIndex,
                                              const QString &title,
                                              const QString &value,
                                              const QString &description)
{
    return addRowToGroup(groupIndex,
                         title,
                         AverraPropertyGridEditors::createReadOnlyLabel(value),
                         description);
}

int AverraPropertyGrid::addBooleanRow(const QString &title, bool checked, const QString &description)
{
    return addRow(title, AverraPropertyGridEditors::createBooleanEditor(checked), description);
}

int AverraPropertyGrid::addBooleanRowToGroup(int groupIndex,
                                             const QString &title,
                                             bool checked,
                                             const QString &description)
{
    return addRowToGroup(groupIndex,
                         title,
                         AverraPropertyGridEditors::createBooleanEditor(checked),
                         description);
}

int AverraPropertyGrid::addEnumRow(const QString &title,
                                   const QStringList &items,
                                   int currentIndex,
                                   const QString &description)
{
    return addRow(title,
                  AverraPropertyGridEditors::createEnumEditor(items, currentIndex),
                  description);
}

int AverraPropertyGrid::addEnumRowToGroup(int groupIndex,
                                          const QString &title,
                                          const QStringList &items,
                                          int currentIndex,
                                          const QString &description)
{
    return addRowToGroup(groupIndex,
                         title,
                         AverraPropertyGridEditors::createEnumEditor(items, currentIndex),
                         description);
}

int AverraPropertyGrid::addNumberRow(const QString &title,
                                     int minimum,
                                     int maximum,
                                     int value,
                                     const QString &description)
{
    return addRow(title,
                  AverraPropertyGridEditors::createNumberEditor(minimum, maximum, value),
                  description);
}

int AverraPropertyGrid::addNumberRowToGroup(int groupIndex,
                                            const QString &title,
                                            int minimum,
                                            int maximum,
                                            int value,
                                            const QString &description)
{
    return addRowToGroup(groupIndex,
                         title,
                         AverraPropertyGridEditors::createNumberEditor(minimum, maximum, value),
                         description);
}

AverraValidationResult AverraPropertyGrid::validationResult() const
{
    return m_validationResult;
}

void AverraPropertyGrid::applyValidationResult(const AverraValidationResult &validationResult)
{
    const bool changed = m_validationResult != validationResult;

    m_validationResult = validationResult;

    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        GridGroup &group = m_groups[groupIndex];

        for (int rowIndex = 0; rowIndex < group.rows.size(); ++rowIndex) {
            GridRow &row = group.rows[rowIndex];
            row.validationMessage.clear();
            row.validationSeverity = AverraValidationIssue::SuccessSeverity;

            if (row.modelKey.trimmed().isEmpty()) {
                row.validationLabel->setVisible(false);
                row.validationLabel->clear();
                continue;
            }

            const QList<AverraValidationIssue> fieldIssues = m_validationResult.fieldIssues(row.modelKey);

            for (int issueIndex = 0; issueIndex < fieldIssues.size(); ++issueIndex) {
                const AverraValidationIssue &issue = fieldIssues.at(issueIndex);

                if (issueIndex == 0 || issue.severity() > row.validationSeverity) {
                    row.validationSeverity = issue.severity();
                    row.validationMessage = issue.message();
                }
            }

            row.validationLabel->setText(row.validationMessage);
            row.validationLabel->setToolTip(m_validationResult.fieldDescription(row.modelKey));
            row.validationLabel->setVisible(!row.validationMessage.trimmed().isEmpty());
        }
    }

    refreshStyle();

    if (changed) {
        emit validationResultChanged(m_validationResult);
    }
}

void AverraPropertyGrid::rebuildFromFormModel()
{
    if (m_formModel == nullptr) {
        return;
    }

    m_dataModel = m_formModel->values();
    clearGroups();

    QHash<QString, int> groupIndices;
    const QList<AverraFieldDefinition> fields = m_formModel->fields();

    for (int index = 0; index < fields.size(); ++index) {
        const AverraFieldDefinition &field = fields.at(index);
        const QString groupKey = normalizedGroupKey(field);
        int groupIndex = -1;

        if (groupIndices.contains(groupKey)) {
            groupIndex = groupIndices.value(groupKey);
        } else {
            QString groupTitle = field.groupTitle();
            QString groupDescription = field.groupDescription();

            if (groupKey == QStringLiteral("__default_group__")) {
                groupTitle.clear();
                groupDescription.clear();
            }

            groupIndex = addGroup(groupTitle, groupDescription);
            groupIndices.insert(groupKey, groupIndex);
        }

        QWidget *editor = createEditorForField(field);
        const int rowIndex = addRowToGroup(groupIndex,
                                           field.title(),
                                           editor,
                                           field.description());

        if (rowIndex < 0) {
            delete editor;
            continue;
        }

        GridRow &row = m_groups[groupIndex].rows[rowIndex];
        row.fieldDefinition = field;
        row.fieldReadOnly = field.readOnly() || field.fieldType() == AverraFieldDefinition::ReadOnlyTextField;
        bindRow(groupIndex, rowIndex, field.key());
    }

    syncReadOnlyState();
    applyValidationResult(m_formModel->validationResult());
    applyFilter();
}

bool AverraPropertyGrid::matchesFilterText(const QString &pattern) const
{
    const QString trimmedPattern = pattern.trimmed();

    if (trimmedPattern.isEmpty()) {
        return true;
    }

    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        const GridGroup &group = m_groups.at(groupIndex);

        if (containsText(group.title, trimmedPattern) || containsText(group.description, trimmedPattern)) {
            return true;
        }

        for (int rowIndex = 0; rowIndex < group.rows.size(); ++rowIndex) {
            if (rowMatches(group.rows.at(rowIndex), trimmedPattern)) {
                return true;
            }
        }
    }

    return false;
}

bool AverraPropertyGrid::saveState(QSettings &settings) const
{
    if (m_settingsKey.trimmed().isEmpty()) {
        return false;
    }

    settings.setValue(m_settingsKey + QStringLiteral("/filterText"), m_filterText);

    if (m_formModel != nullptr) {
        return m_formModel->saveState(settings, m_settingsKey + QStringLiteral("/formModel"));
    }

    settings.setValue(m_settingsKey + QStringLiteral("/dataModel"), m_dataModel);
    return true;
}

bool AverraPropertyGrid::restoreState(QSettings &settings)
{
    if (m_settingsKey.trimmed().isEmpty()) {
        return false;
    }

    setFilterText(settings.value(m_settingsKey + QStringLiteral("/filterText")).toString());

    if (m_formModel != nullptr) {
        return m_formModel->restoreState(settings, m_settingsKey + QStringLiteral("/formModel"));
    }

    const QVariant value = settings.value(m_settingsKey + QStringLiteral("/dataModel"));

    if (!value.isValid()) {
        return false;
    }

    setDataModel(value.toMap());
    return true;
}

void AverraPropertyGrid::clearRows()
{
    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        GridGroup &group = m_groups[groupIndex];

        while (group.rowsLayout->count() > 0) {
            QLayoutItem *item = group.rowsLayout->takeAt(0);

            if (item != nullptr && item->widget() != nullptr) {
                delete item->widget();
            }

            delete item;
        }

        group.rows.clear();
    }
}

void AverraPropertyGrid::clearGroups()
{
    while (m_contentLayout->count() > 1) {
        QLayoutItem *item = m_contentLayout->takeAt(0);

        if (item != nullptr && item->widget() != nullptr) {
            delete item->widget();
        }

        delete item;
    }

    m_groups.clear();
}

void AverraPropertyGrid::initialize()
{
    m_title.clear();
    m_description.clear();
    m_filterText.clear();
    m_settingsKey.clear();
    m_searchEnabled = true;
    m_readOnly = false;
    m_formModel = nullptr;
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);
    m_searchBar = new AverraSearchBar(m_rootFrame);
    m_scrollArea = new QScrollArea(m_rootFrame);
    m_contentWidget = new QWidget;
    m_contentLayout = new QVBoxLayout;
    m_delegate = nullptr;

    m_rootFrame->setObjectName(QStringLiteral("AverraPropertyGridRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraPropertyGridTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraPropertyGridDescription"));

    m_searchBar->setPlaceholderText(QStringLiteral("搜索属性项"));
    m_searchBar->setButtonText(QStringLiteral("筛选"));

    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(14);

    m_titleLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);

    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(10);
    m_contentLayout->addStretch(1);
    m_contentWidget->setLayout(m_contentLayout);
    m_scrollArea->setWidget(m_contentWidget);

    rootLayout->addWidget(m_titleLabel);
    rootLayout->addWidget(m_descriptionLabel);
    rootLayout->addWidget(m_searchBar);
    rootLayout->addWidget(m_scrollArea);

    connect(m_searchBar,
            &AverraSearchBar::textChanged,
            this,
            &AverraPropertyGrid::setFilterText);
    connect(m_searchBar,
            &AverraSearchBar::searchRequested,
            this,
            &AverraPropertyGrid::setFilterText);
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraPropertyGrid::refreshStyle);
    connect(this,
            &AverraPropertyGrid::boundValueChanged,
            this,
            [this](const QString &key, const QVariant &value) {
                if (m_formModel != nullptr) {
                    m_formModel->setValue(key, value);
                }
            });

    updateTextVisibility();
    updateSearchVisibility();
    refreshStyle();
}

void AverraPropertyGrid::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();

    m_rootFrame->setStyleSheet(
        QStringLiteral(
            "QFrame#AverraPropertyGridRoot {"
            "background-color: %1;"
            "border: 1px solid %2;"
            "border-radius: %3px;"
            "}")
            .arg(palette.surfaceColor().name(),
                 palette.borderColor().name(),
                 QString::number(styleProfile.panelRadius())));
    m_titleLabel->setStyleSheet(
        QStringLiteral(
            "QLabel#AverraPropertyGridTitle {"
            "color: %1;"
            "font-size: %2px;"
            "font-weight: 700;"
            "background: transparent;"
            "}")
            .arg(palette.textPrimaryColor().name(),
                 QString::number(styleProfile.pageTitleFontSize())));
    m_descriptionLabel->setStyleSheet(
        QStringLiteral(
            "QLabel#AverraPropertyGridDescription {"
            "color: %1;"
            "font-size: %2px;"
            "font-weight: 500;"
            "background: transparent;"
            "}")
            .arg(palette.textSecondaryColor().name(),
                 QString::number(styleProfile.bodyFontSize())));
    m_contentWidget->setStyleSheet(QStringLiteral("background: transparent;"));

    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        const GridGroup &group = m_groups.at(groupIndex);

        group.frame->setStyleSheet(QStringLiteral("background: transparent; border: none;"));
        group.titleLabel->setStyleSheet(
            QStringLiteral(
                "QLabel#AverraPropertyGridGroupTitle {"
                "color: %1;"
                "font-size: %2px;"
                "font-weight: 700;"
                "background: transparent;"
                "}")
                .arg(palette.textSecondaryColor().name(),
                     QString::number(styleProfile.smallFontSize())));
        group.descriptionLabel->setStyleSheet(
            QStringLiteral(
                "QLabel#AverraPropertyGridGroupDescription {"
                "color: %1;"
                "font-size: %2px;"
                "font-weight: 500;"
                "background: transparent;"
                "}")
                .arg(palette.textSecondaryColor().name(),
                     QString::number(styleProfile.smallFontSize())));

        for (int rowIndex = 0; rowIndex < group.rows.size(); ++rowIndex) {
            const GridRow &row = group.rows.at(rowIndex);
            const bool hasValidationMessage = !row.validationMessage.trimmed().isEmpty();
            const QColor borderColor = hasValidationMessage ? validationBorderColor(palette, row.validationSeverity)
                                                            : palette.borderColor();
            const QColor surfaceColor = hasValidationMessage ? validationSurfaceColor(palette, row.validationSeverity)
                                                             : palette.surfaceRaisedColor();
            row.frame->setStyleSheet(
                QStringLiteral(
                    "QFrame#AverraPropertyGridRow {"
                    "background-color: %1;"
                    "border: 1px solid %2;"
                    "border-radius: %3px;"
                    "}")
                    .arg(surfaceColor.name(),
                         borderColor.name(),
                         QString::number(styleProfile.largeRadius())));
            row.titleLabel->setStyleSheet(
                QStringLiteral(
                    "QLabel#AverraPropertyGridRowTitle {"
                    "color: %1;"
                    "font-size: %2px;"
                    "font-weight: 700;"
                    "background: transparent;"
                    "}")
                    .arg(palette.textPrimaryColor().name(),
                         QString::number(styleProfile.bodyFontSize())));
            row.descriptionLabel->setStyleSheet(
                QStringLiteral(
                    "QLabel#AverraPropertyGridRowDescription {"
                    "color: %1;"
                    "font-size: %2px;"
                    "font-weight: 500;"
                    "background: transparent;"
                    "}")
                    .arg(palette.textSecondaryColor().name(),
                         QString::number(styleProfile.smallFontSize())));
            row.validationLabel->setStyleSheet(
                QStringLiteral(
                    "QLabel#AverraPropertyGridRowValidation {"
                    "color: %1;"
                    "font-size: %2px;"
                    "font-weight: 600;"
                    "background: transparent;"
                    "}")
                    .arg(validationTextColor(palette, row.validationSeverity).name(),
                         QString::number(styleProfile.smallFontSize() - 1)));
        }
    }
}

void AverraPropertyGrid::applyFilter()
{
    const QString pattern = m_filterText.trimmed();

    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        GridGroup &group = m_groups[groupIndex];
        const bool groupVisible = groupMatches(group, pattern);
        group.frame->setVisible(groupVisible);
    }
}

bool AverraPropertyGrid::groupMatches(const GridGroup &group, const QString &pattern) const
{
    if (pattern.isEmpty()) {
        for (int rowIndex = 0; rowIndex < group.rows.size(); ++rowIndex) {
            group.rows.at(rowIndex).frame->setVisible(true);
        }

        return true;
    }

    const bool groupHeaderMatches = containsText(group.title, pattern)
                                    || containsText(group.description, pattern);
    bool hasVisibleRow = false;

    for (int rowIndex = 0; rowIndex < group.rows.size(); ++rowIndex) {
        const bool visible = groupHeaderMatches || rowMatches(group.rows.at(rowIndex), pattern);
        group.rows.at(rowIndex).frame->setVisible(visible);
        hasVisibleRow = hasVisibleRow || visible;
    }

    return groupHeaderMatches || hasVisibleRow;
}

bool AverraPropertyGrid::rowMatches(const GridRow &row, const QString &pattern) const
{
    if (pattern.isEmpty()) {
        return true;
    }

    return containsText(row.title, pattern)
           || containsText(row.description, pattern)
           || containsText(row.validationMessage, pattern)
           || widgetMatches(row.editor, pattern);
}

bool AverraPropertyGrid::widgetMatches(QWidget *widget, const QString &pattern) const
{
    if (widget == nullptr) {
        return false;
    }

    return containsText(widget->windowTitle(), pattern)
           || containsText(widget->accessibleName(), pattern)
           || containsText(widget->toolTip(), pattern)
           || containsText(widget->objectName(), pattern);
}

QWidget *AverraPropertyGrid::createEditorForField(const AverraFieldDefinition &fieldDefinition) const
{
    if (fieldDefinition.fieldType() == AverraFieldDefinition::ReadOnlyTextField) {
        return AverraPropertyGridEditors::createReadOnlyLabel(fieldDefinition.defaultValue().toString());
    }

    if (fieldDefinition.fieldType() == AverraFieldDefinition::TextField) {
        AverraLineEdit *lineEdit = new AverraLineEdit;
        lineEdit->setPlaceholderText(fieldDefinition.placeholderText());
        lineEdit->setText(fieldDefinition.defaultValue().toString());
        return lineEdit;
    }

    if (fieldDefinition.fieldType() == AverraFieldDefinition::MultilineTextField) {
        AverraTextArea *textArea = new AverraTextArea;
        textArea->setPlaceholderText(fieldDefinition.placeholderText());
        textArea->setPlainText(fieldDefinition.defaultValue().toString());
        return textArea;
    }

    if (fieldDefinition.fieldType() == AverraFieldDefinition::BooleanField) {
        return AverraPropertyGridEditors::createBooleanEditor(fieldDefinition.defaultValue().toBool());
    }

    if (fieldDefinition.fieldType() == AverraFieldDefinition::EnumField) {
        AverraComboBox *comboBox = new AverraComboBox;
        comboBox->setProperty("averra_enum_uses_item_data", true);

        const QList<AverraFieldOption> options = fieldDefinition.options();

        for (int index = 0; index < options.size(); ++index) {
            comboBox->addItem(options.at(index).text(), options.at(index).value());
        }

        const QVariant currentValue = fieldDefinition.defaultValue();

        for (int index = 0; index < comboBox->count(); ++index) {
            if (comboBox->itemData(index) == currentValue
                || comboBox->itemText(index) == currentValue.toString()) {
                comboBox->setCurrentIndex(index);
                break;
            }
        }

        return comboBox;
    }

    if (fieldDefinition.fieldType() == AverraFieldDefinition::IntegerField) {
        return AverraPropertyGridEditors::createNumberEditor(fieldDefinition.minimumValue(),
                                                             fieldDefinition.maximumValue(),
                                                             fieldDefinition.defaultValue().toInt());
    }

    return new QWidget;
}

void AverraPropertyGrid::syncFormModel()
{
    if (m_formModel == nullptr) {
        return;
    }

    m_dataModel = m_formModel->values();
    syncModelToEditors();
    applyValidationResult(m_formModel->validationResult());
    syncReadOnlyState();
}

void AverraPropertyGrid::connectEditorSignals(GridRow &row)
{
    QWidget *editor = row.editor;

    if (editor == nullptr) {
        return;
    }

    auto syncFromEditor = [this, editor]() {
        const QString key = editor->property("averra_model_key").toString();

        if (key.trimmed().isEmpty()) {
            return;
        }

        const QVariant currentValue = editorValue(editor);

        if (m_dataModel.value(key) == currentValue) {
            return;
        }

        m_dataModel.insert(key, currentValue);
        emit boundValueChanged(key, currentValue);
        emit dataModelChanged(m_dataModel);
    };

    if (qobject_cast<QLabel *>(editor) != nullptr) {
        return;
    }

    QAbstractButton *abstractButton = qobject_cast<QAbstractButton *>(editor);
    if (abstractButton != nullptr) {
        connect(abstractButton,
                &QAbstractButton::toggled,
                this,
                [syncFromEditor](bool) { syncFromEditor(); });
        return;
    }

    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (lineEdit != nullptr) {
        connect(lineEdit,
                &QLineEdit::textChanged,
                this,
                [syncFromEditor](const QString &) { syncFromEditor(); });
        return;
    }

    QTextEdit *textEdit = qobject_cast<QTextEdit *>(editor);
    if (textEdit != nullptr) {
        connect(textEdit,
                &QTextEdit::textChanged,
                this,
                syncFromEditor);
        return;
    }

    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    if (comboBox != nullptr) {
        connect(comboBox,
                QOverload<int>::of(&QComboBox::currentIndexChanged),
                this,
                [syncFromEditor](int) { syncFromEditor(); });
        return;
    }

    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    if (spinBox != nullptr) {
        connect(spinBox,
                QOverload<int>::of(&QSpinBox::valueChanged),
                this,
                [syncFromEditor](int) { syncFromEditor(); });
    }
}

QVariant AverraPropertyGrid::editorValue(QWidget *editor) const
{
    if (editor == nullptr) {
        return QVariant();
    }

    QLabel *label = qobject_cast<QLabel *>(editor);
    if (label != nullptr) {
        return label->text();
    }

    QAbstractButton *abstractButton = qobject_cast<QAbstractButton *>(editor);
    if (abstractButton != nullptr) {
        return abstractButton->isChecked();
    }

    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (lineEdit != nullptr) {
        return lineEdit->text();
    }

    QTextEdit *textEdit = qobject_cast<QTextEdit *>(editor);
    if (textEdit != nullptr) {
        return textEdit->toPlainText();
    }

    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    if (comboBox != nullptr) {
        if (comboBox->property("averra_enum_uses_item_data").toBool()) {
            return comboBox->currentData();
        }

        return comboBox->currentIndex();
    }

    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    if (spinBox != nullptr) {
        return spinBox->value();
    }

    return QVariant();
}

void AverraPropertyGrid::setEditorValue(QWidget *editor, const QVariant &value)
{
    if (editor == nullptr) {
        return;
    }

    QLabel *label = qobject_cast<QLabel *>(editor);
    if (label != nullptr) {
        label->setText(value.toString());
        return;
    }

    QAbstractButton *abstractButton = qobject_cast<QAbstractButton *>(editor);
    if (abstractButton != nullptr) {
        const QSignalBlocker blocker(abstractButton);
        abstractButton->setChecked(value.toBool());
        return;
    }

    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (lineEdit != nullptr) {
        const QSignalBlocker blocker(lineEdit);
        lineEdit->setText(value.toString());
        return;
    }

    QTextEdit *textEdit = qobject_cast<QTextEdit *>(editor);
    if (textEdit != nullptr) {
        const QSignalBlocker blocker(textEdit);
        textEdit->setPlainText(value.toString());
        return;
    }

    QComboBox *comboBox = qobject_cast<QComboBox *>(editor);
    if (comboBox != nullptr) {
        const QSignalBlocker blocker(comboBox);

        if (comboBox->property("averra_enum_uses_item_data").toBool()) {
            for (int itemIndex = 0; itemIndex < comboBox->count(); ++itemIndex) {
                if (comboBox->itemData(itemIndex) == value
                    || comboBox->itemText(itemIndex) == value.toString()) {
                    comboBox->setCurrentIndex(itemIndex);
                    return;
                }
            }
        }

        const int index = value.canConvert<int>() ? value.toInt() : comboBox->findText(value.toString());

        if (index >= 0 && index < comboBox->count()) {
            comboBox->setCurrentIndex(index);
        }

        return;
    }

    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    if (spinBox != nullptr) {
        const QSignalBlocker blocker(spinBox);
        spinBox->setValue(value.toInt());
    }
}

void AverraPropertyGrid::syncEditorToModel(GridRow &row)
{
    if (row.modelKey.trimmed().isEmpty()) {
        return;
    }

    if (m_dataModel.contains(row.modelKey)) {
        setEditorValue(row.editor, m_dataModel.value(row.modelKey));
        return;
    }

    m_dataModel.insert(row.modelKey, editorValue(row.editor));
    emit dataModelChanged(m_dataModel);
}

void AverraPropertyGrid::syncModelToEditors()
{
    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        const GridGroup &group = m_groups.at(groupIndex);

        for (int rowIndex = 0; rowIndex < group.rows.size(); ++rowIndex) {
            const GridRow &row = group.rows.at(rowIndex);

            if (!row.modelKey.trimmed().isEmpty() && m_dataModel.contains(row.modelKey)) {
                setEditorValue(row.editor, m_dataModel.value(row.modelKey));
            }
        }
    }
}

void AverraPropertyGrid::syncReadOnlyState()
{
    const bool resolvedReadOnly = readOnly();

    for (int groupIndex = 0; groupIndex < m_groups.size(); ++groupIndex) {
        GridGroup &group = m_groups[groupIndex];

        for (int rowIndex = 0; rowIndex < group.rows.size(); ++rowIndex) {
            GridRow &row = group.rows[rowIndex];

            if (row.editor != nullptr) {
                row.editor->setEnabled(!(resolvedReadOnly || row.fieldReadOnly));
            }
        }
    }
}

void AverraPropertyGrid::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}

void AverraPropertyGrid::updateSearchVisibility()
{
    m_searchBar->setVisible(m_searchEnabled);
}

void AverraPropertyGrid::polishRow(GridGroup &group, GridRow &row)
{
    if (m_delegate == nullptr) {
        return;
    }

    m_delegate->polishRow(group.title,
                          row.title,
                          row.frame,
                          row.titleLabel,
                          row.descriptionLabel,
                          row.editor);
}
