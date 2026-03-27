#include <Averra/AverraFieldDefinition.h>
#include <Averra/AverraFormModel.h>

#include <QSettings>
#include <QTemporaryDir>

#include <QtTest>

class TestAverraFormModel : public QObject
{
    Q_OBJECT

private slots:
    void shouldProvideDefaultValuesAndValidateFields();
    void shouldCommitRollbackAndRestorePersistedState();
};

void TestAverraFormModel::shouldProvideDefaultValuesAndValidateFields()
{
    AverraFormModel model;
    QList<AverraFieldDefinition> fields;

    AverraFieldDefinition nameField = AverraFieldDefinition::createText(QStringLiteral("name"),
                                                                        QStringLiteral("节点名称"));
    nameField.addValidationRule(AverraValidationRule::required());
    fields.append(nameField);

    AverraFieldDefinition retryField = AverraFieldDefinition::createInteger(QStringLiteral("retryCount"),
                                                                            QStringLiteral("重试次数"),
                                                                            0,
                                                                            10,
                                                                            3);
    retryField.addValidationRule(AverraValidationRule::maximum(8));
    fields.append(retryField);

    model.setFields(fields);

    QCOMPARE(model.value(QStringLiteral("retryCount")).toInt(), 3);
    QVERIFY(!model.validationResult().isValid());
    QVERIFY(model.validationResult().fieldMessage(QStringLiteral("name")).contains(QStringLiteral("不能为空")));

    model.setValue(QStringLiteral("name"), QStringLiteral("Scene Indexer"));

    QVERIFY(model.validationResult().isValid());
    QCOMPARE(model.value(QStringLiteral("name")).toString(), QStringLiteral("Scene Indexer"));
}

void TestAverraFormModel::shouldCommitRollbackAndRestorePersistedState()
{
    AverraFormModel sourceModel;
    QList<AverraFieldDefinition> fields;

    AverraFieldDefinition nameField = AverraFieldDefinition::createText(QStringLiteral("name"),
                                                                        QStringLiteral("节点名称"));
    nameField.addValidationRule(AverraValidationRule::required());
    fields.append(nameField);

    AverraFieldDefinition notesField = AverraFieldDefinition::createMultilineText(QStringLiteral("notes"),
                                                                                  QStringLiteral("备注"));
    fields.append(notesField);

    sourceModel.setFields(fields);
    sourceModel.loadObject(QStringLiteral("scene-indexer"),
                           QVariantMap{
                               {QStringLiteral("name"), QStringLiteral("Scene Indexer")},
                               {QStringLiteral("notes"), QStringLiteral("原始内容")}
                           });

    sourceModel.setValue(QStringLiteral("name"), QStringLiteral("Scene Indexer Updated"));
    sourceModel.setValue(QStringLiteral("notes"), QStringLiteral("已修改但未提交"));
    QVERIFY(sourceModel.isDirty());

    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    QSettings settings(tempDir.path() + QStringLiteral("/form-model.ini"), QSettings::IniFormat);
    QVERIFY(sourceModel.saveState(settings, QStringLiteral("workflow/scene-indexer")));

    sourceModel.rollback();
    QCOMPARE(sourceModel.value(QStringLiteral("name")).toString(), QStringLiteral("Scene Indexer"));
    QVERIFY(!sourceModel.isDirty());

    sourceModel.setValue(QStringLiteral("name"), QStringLiteral("Scene Indexer Committed"));
    sourceModel.commit();
    QCOMPARE(sourceModel.committedValues().value(QStringLiteral("name")).toString(),
             QStringLiteral("Scene Indexer Committed"));
    QVERIFY(!sourceModel.isDirty());

    AverraFormModel restoredModel;
    restoredModel.setFields(fields);
    restoredModel.loadObject(QStringLiteral("scene-indexer"),
                             QVariantMap{
                                 {QStringLiteral("name"), QStringLiteral("Scene Indexer")},
                                 {QStringLiteral("notes"), QStringLiteral("原始内容")}
                             });

    QVERIFY(restoredModel.restoreState(settings, QStringLiteral("workflow/scene-indexer")));
    QCOMPARE(restoredModel.objectId(), QStringLiteral("scene-indexer"));
    QCOMPARE(restoredModel.value(QStringLiteral("name")).toString(), QStringLiteral("Scene Indexer Updated"));
    QCOMPARE(restoredModel.value(QStringLiteral("notes")).toString(), QStringLiteral("已修改但未提交"));
    QVERIFY(restoredModel.isDirty());
}

QObject *createTestAverraFormModel()
{
    return new TestAverraFormModel;
}

#include "tst_AverraFormModel.moc"
