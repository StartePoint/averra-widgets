#ifndef AVERRA_FORM_MODEL_H
#define AVERRA_FORM_MODEL_H

#include <Averra/AverraFieldDefinition.h>
#include <Averra/AverraGlobal.h>
#include <Averra/AverraValidationResult.h>

#include <QObject>
#include <QVariantMap>

class QSettings;

class AVERRA_EXPORT AverraFormModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString objectId READ objectId WRITE setObjectId NOTIFY objectIdChanged)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(bool dirty READ isDirty NOTIFY dirtyChanged)

public:
    explicit AverraFormModel(QObject *parent = nullptr);

    QList<AverraFieldDefinition> fields() const;
    void setFields(const QList<AverraFieldDefinition> &fields);
    void addField(const AverraFieldDefinition &field);
    void clearFields();

    QString objectId() const;
    void setObjectId(const QString &objectId);

    bool readOnly() const;
    void setReadOnly(bool readOnly);

    QVariantMap values() const;
    void setValues(const QVariantMap &values);

    QVariantMap committedValues() const;

    QVariant value(const QString &key) const;
    void setValue(const QString &key, const QVariant &value);

    void loadObject(const QString &objectId, const QVariantMap &values);
    void commit();
    void rollback();

    bool isDirty() const;

    AverraValidationResult validate() const;
    AverraValidationResult validationResult() const;
    void refreshValidation();

    bool saveState(QSettings &settings, const QString &settingsKey) const;
    bool restoreState(QSettings &settings, const QString &settingsKey);

signals:
    void fieldsChanged();
    void objectIdChanged(const QString &objectId);
    void readOnlyChanged(bool readOnly);
    void valuesChanged(const QVariantMap &values);
    void valueChanged(const QString &key, const QVariant &value);
    void committedValuesChanged(const QVariantMap &values);
    void dirtyChanged(bool dirty);
    void validationChanged(const AverraValidationResult &validationResult);

private:
    const AverraFieldDefinition *fieldDefinition(const QString &key) const;
    QVariant normalizeValue(const AverraFieldDefinition &field, const QVariant &value) const;
    QVariantMap mergedValues(const QVariantMap &values) const;
    AverraValidationResult validateValues(const QVariantMap &values) const;
    void updateValidation(bool emitSignal = true);
    void emitDirtyIfNeeded(bool previousDirty);

    QString m_objectId;
    bool m_readOnly;
    QList<AverraFieldDefinition> m_fields;
    QVariantMap m_values;
    QVariantMap m_committedValues;
    AverraValidationResult m_validationResult;
};

#endif // AVERRA_FORM_MODEL_H
