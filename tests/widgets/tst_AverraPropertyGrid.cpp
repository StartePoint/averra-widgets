#include <Averra/AverraComboBox.h>
#include <Averra/AverraFieldDefinition.h>
#include <Averra/AverraFormModel.h>
#include <Averra/AverraLineEdit.h>
#include <Averra/AverraNumberBox.h>
#include <Averra/AverraPropertyGrid.h>
#include <Averra/AverraSwitch.h>
#include <Averra/AverraThemeManager.h>

#include <QFrame>
#include <QLabel>
#include <QSettings>
#include <QSignalSpy>
#include <QTemporaryDir>

#include <QtTest>

class TestPropertyGridDelegate : public AverraPropertyGridDelegate
{
public:
    void polishRow(const QString &groupTitle,
                   const QString &rowTitle,
                   QFrame *rowFrame,
                   QLabel *titleLabel,
                   QLabel *descriptionLabel,
                   QWidget *editor) override
    {
        Q_UNUSED(descriptionLabel)
        Q_UNUSED(editor)

        rowFrame->setToolTip(groupTitle + QStringLiteral(":") + rowTitle);
        titleLabel->setText(rowTitle + QStringLiteral(" (委托)"));
    }
};

class TestAverraPropertyGrid : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreTitleDescriptionAndFlags();
    void shouldAllowAddingAndClearingRows();
    void shouldFilterRows();
    void shouldDisableEditorsWhenReadOnly();
    void shouldSupportGroupsAndDelegate();
    void shouldProvideStandardEditorFactoriesAndConvenienceRows();
    void shouldSupportTwoWayDataBinding();
    void shouldBuildRowsFromFormModelAndShowValidationState();
    void shouldPersistFormModelStateWithQSettings();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraPropertyGrid::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraPropertyGrid::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraPropertyGrid::shouldStoreTitleDescriptionAndFlags()
{
    AverraPropertyGrid grid;

    grid.setTitle(QStringLiteral("基础属性"));
    grid.setDescription(QStringLiteral("平铺展示当前对象属性。"));
    grid.setSearchEnabled(false);
    grid.setReadOnly(true);

    QCOMPARE(grid.title(), QStringLiteral("基础属性"));
    QCOMPARE(grid.description(), QStringLiteral("平铺展示当前对象属性。"));
    QCOMPARE(grid.searchEnabled(), false);
    QCOMPARE(grid.readOnly(), true);
}

void TestAverraPropertyGrid::shouldAllowAddingAndClearingRows()
{
    AverraPropertyGrid grid;
    grid.addRow(QStringLiteral("名称"), new AverraLineEdit);
    grid.addRow(QStringLiteral("编号"), new AverraLineEdit);

    QCOMPARE(grid.rowCount(), 2);

    grid.clearRows();

    QCOMPARE(grid.rowCount(), 0);
}

void TestAverraPropertyGrid::shouldFilterRows()
{
    AverraPropertyGrid grid;
    AverraLineEdit *nameEdit = new AverraLineEdit;
    nameEdit->setObjectName(QStringLiteral("nameEditor"));
    AverraLineEdit *timeoutEdit = new AverraLineEdit;
    timeoutEdit->setObjectName(QStringLiteral("timeoutEditor"));

    grid.addRow(QStringLiteral("名称"), nameEdit, QStringLiteral("用于显示节点标题"));
    grid.addRow(QStringLiteral("超时"), timeoutEdit, QStringLiteral("控制请求等待时间"));
    grid.setFilterText(QStringLiteral("超时"));

    QVERIFY(nameEdit->parentWidget()->isHidden());
    QVERIFY(!timeoutEdit->parentWidget()->isHidden());
}

void TestAverraPropertyGrid::shouldDisableEditorsWhenReadOnly()
{
    AverraPropertyGrid grid;
    AverraLineEdit *nameEdit = new AverraLineEdit;
    grid.addRow(QStringLiteral("名称"), nameEdit);

    grid.setReadOnly(true);
    QCOMPARE(nameEdit->isEnabled(), false);

    grid.setReadOnly(false);
    QCOMPARE(nameEdit->isEnabled(), true);
}

void TestAverraPropertyGrid::shouldSupportGroupsAndDelegate()
{
    AverraPropertyGrid grid;
    TestPropertyGridDelegate delegate;
    const int baseGroup = grid.addGroup(QStringLiteral("基础信息"), QStringLiteral("当前对象的基础属性。"));
    const int advancedGroup = grid.addGroup(QStringLiteral("高级设置"), QString());

    grid.setDelegate(&delegate);

    AverraLineEdit *nameEdit = new AverraLineEdit;
    AverraLineEdit *timeoutEdit = new AverraLineEdit;
    grid.addRowToGroup(baseGroup, QStringLiteral("名称"), nameEdit, QStringLiteral("显示名称"));
    grid.addRowToGroup(advancedGroup, QStringLiteral("超时"), timeoutEdit, QStringLiteral("请求超时"));

    QCOMPARE(grid.groupCount(), 2);
    QCOMPARE(grid.rowCount(), 2);
    QCOMPARE(nameEdit->parentWidget()->toolTip(), QStringLiteral("基础信息:名称"));

    QLabel *nameLabel = nameEdit->parentWidget()->findChild<QLabel *>(QStringLiteral("AverraPropertyGridRowTitle"));
    QVERIFY(nameLabel != nullptr);
    QVERIFY(nameLabel->text().contains(QStringLiteral("委托")));
}

void TestAverraPropertyGrid::shouldProvideStandardEditorFactoriesAndConvenienceRows()
{
    AverraPropertyGrid grid;
    const int groupIndex = grid.addGroup(QStringLiteral("标准编辑器"));

    QLabel *readOnlyLabel = AverraPropertyGridEditors::createReadOnlyLabel(QStringLiteral("只读值"));
    AverraSwitch *boolEditor = AverraPropertyGridEditors::createBooleanEditor(true);
    AverraComboBox *enumEditor = AverraPropertyGridEditors::createEnumEditor(QStringList()
                                                                             << QStringLiteral("默认")
                                                                             << QStringLiteral("海洋蓝"),
                                                                             1);
    AverraNumberBox *numberEditor = AverraPropertyGridEditors::createNumberEditor(1, 99, 12);

    QVERIFY(readOnlyLabel != nullptr);
    QVERIFY(boolEditor != nullptr);
    QVERIFY(enumEditor != nullptr);
    QVERIFY(numberEditor != nullptr);
    QCOMPARE(boolEditor->isChecked(), true);
    QCOMPARE(enumEditor->currentIndex(), 1);
    QCOMPARE(numberEditor->value(), 12);

    grid.addReadOnlyRowToGroup(groupIndex, QStringLiteral("版本"), QStringLiteral("0.2"));
    grid.addBooleanRowToGroup(groupIndex, QStringLiteral("自动更新"), true);
    grid.addEnumRowToGroup(groupIndex,
                           QStringLiteral("主题"),
                           QStringList() << QStringLiteral("默认") << QStringLiteral("森林绿"),
                           1);
    grid.addNumberRowToGroup(groupIndex, QStringLiteral("重试次数"), 0, 10, 3);

    QCOMPARE(grid.rowCount(), 4);
}

void TestAverraPropertyGrid::shouldSupportTwoWayDataBinding()
{
    AverraPropertyGrid grid;
    QSignalSpy modelSpy(&grid, &AverraPropertyGrid::dataModelChanged);
    const int groupIndex = grid.addGroup(QStringLiteral("绑定"));
    AverraLineEdit *nameEdit = new AverraLineEdit;
    grid.addRowToGroup(groupIndex, QStringLiteral("名称"), nameEdit);
    QVERIFY(grid.bindRow(groupIndex, 0, QStringLiteral("name")));

    QVariantMap model;
    model.insert(QStringLiteral("name"), QStringLiteral("Core Node A"));
    grid.setDataModel(model);

    QCOMPARE(nameEdit->text(), QStringLiteral("Core Node A"));

    nameEdit->setText(QStringLiteral("Core Node B"));

    QCOMPARE(grid.value(QStringLiteral("name")).toString(), QStringLiteral("Core Node B"));
    QVERIFY(modelSpy.count() >= 1);
}

void TestAverraPropertyGrid::shouldBuildRowsFromFormModelAndShowValidationState()
{
    AverraFormModel formModel;
    QList<AverraFieldDefinition> fields;

    AverraFieldDefinition nameField = AverraFieldDefinition::createText(QStringLiteral("name"),
                                                                        QStringLiteral("节点名称"));
    nameField.setGroup(QStringLiteral("identity"), QStringLiteral("基础信息"));
    nameField.addValidationRule(AverraValidationRule::required());
    fields.append(nameField);

    AverraFieldDefinition retryField = AverraFieldDefinition::createInteger(QStringLiteral("retryCount"),
                                                                            QStringLiteral("重试次数"),
                                                                            0,
                                                                            10,
                                                                            2);
    retryField.setGroup(QStringLiteral("display"), QStringLiteral("行为与展示"));
    fields.append(retryField);

    formModel.setFields(fields);

    AverraPropertyGrid grid;
    grid.setFormModel(&formModel);

    QCOMPARE(grid.groupCount(), 2);
    QCOMPARE(grid.rowCount(), 2);

    const QList<AverraLineEdit *> lineEdits = grid.findChildren<AverraLineEdit *>();
    QVERIFY(!lineEdits.isEmpty());

    lineEdits.first()->setText(QStringLiteral("Scene Indexer"));
    QCOMPARE(formModel.value(QStringLiteral("name")).toString(), QStringLiteral("Scene Indexer"));

    formModel.setValue(QStringLiteral("name"), QString());
    formModel.refreshValidation();

    const QList<QLabel *> validationLabels = grid.findChildren<QLabel *>(QStringLiteral("AverraPropertyGridRowValidation"));
    QVERIFY(!validationLabels.isEmpty());

    bool hasRequiredMessage = false;
    for (int index = 0; index < validationLabels.size(); ++index) {
        hasRequiredMessage = hasRequiredMessage || validationLabels.at(index)->text().contains(QStringLiteral("不能为空"));
    }

    QVERIFY(hasRequiredMessage);
}

void TestAverraPropertyGrid::shouldPersistFormModelStateWithQSettings()
{
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    QSettings settings(tempDir.path() + QStringLiteral("/property-grid.ini"), QSettings::IniFormat);

    AverraFormModel sourceModel;
    QList<AverraFieldDefinition> fields;

    AverraFieldDefinition nameField = AverraFieldDefinition::createText(QStringLiteral("name"),
                                                                        QStringLiteral("节点名称"),
                                                                        QStringLiteral("Workspace"));
    nameField.addValidationRule(AverraValidationRule::required());
    fields.append(nameField);

    sourceModel.setFields(fields);
    sourceModel.loadObject(QStringLiteral("workspace-root"),
                           QVariantMap{{QStringLiteral("name"), QStringLiteral("Workspace")}});

    AverraPropertyGrid sourceGrid;
    sourceGrid.setFormModel(&sourceModel);
    sourceGrid.setSettingsKey(QStringLiteral("workflow/propertyGrid"));
    sourceGrid.setFilterText(QStringLiteral("节点"));

    sourceModel.setValue(QStringLiteral("name"), QStringLiteral("Workspace Updated"));
    QVERIFY(sourceGrid.saveState(settings));

    AverraFormModel restoredModel;
    restoredModel.setFields(fields);
    restoredModel.loadObject(QStringLiteral("workspace-root"),
                             QVariantMap{{QStringLiteral("name"), QStringLiteral("Workspace")}});

    AverraPropertyGrid restoredGrid;
    restoredGrid.setFormModel(&restoredModel);
    restoredGrid.setSettingsKey(QStringLiteral("workflow/propertyGrid"));

    QVERIFY(restoredGrid.restoreState(settings));
    QCOMPARE(restoredGrid.filterText(), QStringLiteral("节点"));
    QCOMPARE(restoredModel.value(QStringLiteral("name")).toString(), QStringLiteral("Workspace Updated"));
}

QObject *createTestAverraPropertyGrid()
{
    return new TestAverraPropertyGrid;
}

#include "tst_AverraPropertyGrid.moc"
