#include <Averra/AverraFieldDefinition.h>
#include <Averra/AverraFormModel.h>
#include <Averra/AverraFormSection.h>
#include <Averra/AverraInfoBar.h>
#include <Averra/AverraInspectorPanel.h>
#include <Averra/AverraPropertyEditor.h>
#include <Averra/AverraPropertyGrid.h>
#include <Averra/AverraPropertyItem.h>
#include <Averra/AverraSwitch.h>
#include <Averra/AverraThemeManager.h>

#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QTemporaryDir>

#include <QtTest>

class TestAverraInspectorPanel : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void shouldStoreHeaderAndStatus();
    void shouldAllowAddingActionWidgets();
    void shouldDelegateSectionsAndReadOnlyState();
    void shouldDelegateFilterText();
    void shouldControlValidationPrompt();
    void shouldAllowAddingFooterWidgets();
    void shouldSupportValidationLevelAndSubmitStateModel();
    void shouldSupportAsyncFailureAndRecoveryFlow();
    void shouldSubmitWithFormModelWhenValidationPasses();
    void shouldRollbackFormModelStateOnSubmitFailure();
    void shouldPersistPerObjectFormState();

private:
    AverraThemePalette m_defaultPalette;
};

void TestAverraInspectorPanel::init()
{
    m_defaultPalette = AverraThemePalette::createLightPalette();
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraInspectorPanel::cleanup()
{
    AverraThemeManager::instance()->setPalette(m_defaultPalette);
}

void TestAverraInspectorPanel::shouldStoreHeaderAndStatus()
{
    AverraInspectorPanel panel;

    panel.setTitle(QStringLiteral("节点检查"));
    panel.setSubtitle(QStringLiteral("用于展示当前对象详情"));
    panel.setStatusText(QStringLiteral("Live"));

    QCOMPARE(panel.title(), QStringLiteral("节点检查"));
    QCOMPARE(panel.subtitle(), QStringLiteral("用于展示当前对象详情"));
    QCOMPARE(panel.statusText(), QStringLiteral("Live"));
}

void TestAverraInspectorPanel::shouldAllowAddingActionWidgets()
{
    AverraInspectorPanel panel;
    QPushButton *button = new QPushButton(QStringLiteral("应用"));

    panel.addActionWidget(button);

    QCOMPARE(panel.actionsLayout()->count(), 1);
}

void TestAverraInspectorPanel::shouldDelegateSectionsAndReadOnlyState()
{
    AverraInspectorPanel panel;
    AverraFormSection *section = new AverraFormSection(QStringLiteral("基础信息"));
    AverraPropertyItem *item = new AverraPropertyItem(QStringLiteral("自动同步"));
    AverraSwitch *sw = new AverraSwitch;
    item->setAccessoryWidget(sw);
    section->addRow(item);
    panel.addSection(section);

    panel.setReadOnly(true);

    QCOMPARE(panel.readOnly(), true);
    QCOMPARE(section->isEnabled(), false);
    QCOMPARE(sw->isEnabled(), false);
}

void TestAverraInspectorPanel::shouldDelegateFilterText()
{
    AverraInspectorPanel panel;

    panel.setFilterText(QStringLiteral("名称"));

    QCOMPARE(panel.filterText(), QStringLiteral("名称"));
    QCOMPARE(panel.propertyEditor()->filterText(), QStringLiteral("名称"));
}

void TestAverraInspectorPanel::shouldControlValidationPrompt()
{
    AverraInspectorPanel panel;

    panel.setValidationTitle(QStringLiteral("配置不完整"));
    panel.setValidationDescription(QStringLiteral("请先填写节点名称。"));
    panel.setValidationVisible(true);

    QCOMPARE(panel.validationTitle(), QStringLiteral("配置不完整"));
    QCOMPARE(panel.validationDescription(), QStringLiteral("请先填写节点名称。"));
    QCOMPARE(panel.validationVisible(), true);
    QVERIFY(!panel.validationBar()->isHidden());
}

void TestAverraInspectorPanel::shouldAllowAddingFooterWidgets()
{
    AverraInspectorPanel panel;
    QPushButton *saveButton = new QPushButton(QStringLiteral("保存"));

    panel.addFooterWidget(saveButton);

    QVERIFY(panel.footerLayout()->count() >= 4);
    QVERIFY(!saveButton->isHidden());
}

void TestAverraInspectorPanel::shouldSupportValidationLevelAndSubmitStateModel()
{
    AverraInspectorPanel panel;
    QSignalSpy submitSpy(&panel, &AverraInspectorPanel::submitRequested);

    panel.setValidationLevel(AverraInspectorPanel::ErrorLevel);
    QCOMPARE(panel.validationLevel(), AverraInspectorPanel::ErrorLevel);
    QCOMPARE(panel.validationBar()->type(), AverraInfoBar::Error);

    panel.setSubmitButtonText(QStringLiteral("保存"));
    QCOMPARE(panel.submitButtonText(), QStringLiteral("保存"));
    QCOMPARE(panel.submitButton()->text(), QStringLiteral("保存"));

    panel.triggerSubmit();

    QCOMPARE(submitSpy.count(), 1);
    QCOMPARE(panel.submitState(), AverraInspectorPanel::SubmitSubmitting);
    QCOMPARE(panel.submitButton()->isEnabled(), false);

    panel.setSubmitResultText(QStringLiteral("保存成功"));
    panel.setSubmitState(AverraInspectorPanel::SubmitSucceeded);

    QCOMPARE(panel.submitState(), AverraInspectorPanel::SubmitSucceeded);
    QCOMPARE(panel.submitResultText(), QStringLiteral("保存成功"));
    QCOMPARE(panel.submitButton()->isEnabled(), true);
}

void TestAverraInspectorPanel::shouldSupportAsyncFailureAndRecoveryFlow()
{
    AverraInspectorPanel panel;
    QSignalSpy submitSpy(&panel, &AverraInspectorPanel::submitRequested);
    QSignalSpy retrySpy(&panel, &AverraInspectorPanel::retryRequested);

    panel.triggerSubmit();

    QCOMPARE(submitSpy.count(), 1);
    QCOMPARE(panel.submitState(), AverraInspectorPanel::SubmitSubmitting);

    panel.completeSubmitFailure(QStringLiteral("提交失败"),
                                QStringLiteral("网络异常"),
                                QStringLiteral("请检查连接后重试。"),
                                AverraInspectorPanel::ErrorLevel);

    QCOMPARE(panel.submitState(), AverraInspectorPanel::SubmitFailed);
    QCOMPARE(panel.validationLevel(), AverraInspectorPanel::ErrorLevel);
    QCOMPARE(panel.submitResultText(), QStringLiteral("提交失败"));
    QVERIFY(!panel.retryButton()->isHidden());

    panel.triggerRetry();

    QCOMPARE(retrySpy.count(), 1);
    QCOMPARE(panel.submitState(), AverraInspectorPanel::SubmitIdle);
    QCOMPARE(panel.submitResultText(), QString());
    QVERIFY(panel.retryButton()->isHidden());
}

void TestAverraInspectorPanel::shouldSubmitWithFormModelWhenValidationPasses()
{
    AverraInspectorPanel panel;
    AverraFormModel model;
    QList<AverraFieldDefinition> fields;

    AverraFieldDefinition nameField = AverraFieldDefinition::createText(QStringLiteral("name"),
                                                                        QStringLiteral("节点名称"));
    nameField.addValidationRule(AverraValidationRule::required());
    fields.append(nameField);

    model.setFields(fields);
    model.loadObject(QStringLiteral("scene-indexer"),
                     QVariantMap{{QStringLiteral("name"), QStringLiteral("Scene Indexer")}});
    panel.setFormModel(&model);

    QSignalSpy submitSpy(&panel, SIGNAL(formSubmitRequested(QString,QVariantMap)));

    panel.triggerSubmit();

    QCOMPARE(panel.submitState(), AverraInspectorPanel::SubmitSubmitting);
    QCOMPARE(submitSpy.count(), 1);
    QCOMPARE(submitSpy.at(0).at(0).toString(), QStringLiteral("scene-indexer"));

    model.setValue(QStringLiteral("name"), QString());
    panel.resetSubmitState();
    panel.triggerSubmit();

    QCOMPARE(panel.submitState(), AverraInspectorPanel::SubmitFailed);
    QCOMPARE(submitSpy.count(), 1);
    QVERIFY(panel.validationBar()->title().contains(QStringLiteral("校验")));
}

void TestAverraInspectorPanel::shouldRollbackFormModelStateOnSubmitFailure()
{
    AverraInspectorPanel panel;
    AverraFormModel model;
    QList<AverraFieldDefinition> fields;

    fields.append(AverraFieldDefinition::createText(QStringLiteral("name"),
                                                    QStringLiteral("节点名称")));

    model.setFields(fields);
    model.loadObject(QStringLiteral("scene-indexer"),
                     QVariantMap{{QStringLiteral("name"), QStringLiteral("Scene Indexer")}});
    panel.setFormModel(&model);

    model.setValue(QStringLiteral("name"), QStringLiteral("Scene Indexer Updated"));
    QVERIFY(model.isDirty());

    panel.completeSubmitFailure(QStringLiteral("提交失败"),
                                QStringLiteral("服务拒绝"),
                                QStringLiteral("演示回滚"));

    QCOMPARE(model.value(QStringLiteral("name")).toString(), QStringLiteral("Scene Indexer"));
    QVERIFY(!model.isDirty());
    QCOMPARE(panel.submitState(), AverraInspectorPanel::SubmitFailed);
}

void TestAverraInspectorPanel::shouldPersistPerObjectFormState()
{
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    QSettings settings(tempDir.path() + QStringLiteral("/inspector.ini"), QSettings::IniFormat);

    AverraInspectorPanel sourcePanel;
    AverraFormModel sourceModel;
    QList<AverraFieldDefinition> fields;

    fields.append(AverraFieldDefinition::createText(QStringLiteral("name"),
                                                    QStringLiteral("节点名称"),
                                                    QStringLiteral("Workspace")));

    sourceModel.setFields(fields);
    sourceModel.loadObject(QStringLiteral("workspace-root"),
                           QVariantMap{{QStringLiteral("name"), QStringLiteral("Workspace")}});
    sourcePanel.setFormModel(&sourceModel);
    sourcePanel.setSettingsKey(QStringLiteral("workflow/inspector/workspace-root"));
    sourcePanel.setFilterText(QStringLiteral("节点"));

    sourceModel.setValue(QStringLiteral("name"), QStringLiteral("Workspace Updated"));
    QVERIFY(sourcePanel.saveState(settings));

    AverraInspectorPanel restoredPanel;
    AverraFormModel restoredModel;
    restoredModel.setFields(fields);
    restoredModel.loadObject(QStringLiteral("workspace-root"),
                             QVariantMap{{QStringLiteral("name"), QStringLiteral("Workspace")}});
    restoredPanel.setFormModel(&restoredModel);
    restoredPanel.setSettingsKey(QStringLiteral("workflow/inspector/workspace-root"));

    QVERIFY(restoredPanel.restoreState(settings));
    QCOMPARE(restoredPanel.filterText(), QStringLiteral("节点"));
    QCOMPARE(restoredModel.value(QStringLiteral("name")).toString(), QStringLiteral("Workspace Updated"));
}

QObject *createTestAverraInspectorPanel()
{
    return new TestAverraInspectorPanel;
}

#include "tst_AverraInspectorPanel.moc"
