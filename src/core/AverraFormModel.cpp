#include <Averra/AverraFormModel.h>

#include <QMetaType>
#include <QRegularExpression>
#include <QSettings>

namespace
{
QString fieldLabel(const AverraFieldDefinition &field)
{
    if (!field.title().trimmed().isEmpty()) {
        return field.title();
    }

    return field.key();
}

QString requiredMessageForField(const AverraFieldDefinition &field)
{
    return QStringLiteral("%1不能为空。").arg(fieldLabel(field));
}

QString minimumMessageForField(const AverraFieldDefinition &field, double minimumValue)
{
    return QStringLiteral("%1不能小于 %2。")
        .arg(fieldLabel(field), QString::number(minimumValue));
}

QString maximumMessageForField(const AverraFieldDefinition &field, double maximumValue)
{
    return QStringLiteral("%1不能大于 %2。")
        .arg(fieldLabel(field), QString::number(maximumValue));
}

QString regularExpressionMessageForField(const AverraFieldDefinition &field)
{
    return QStringLiteral("%1格式不正确。").arg(fieldLabel(field));
}

bool isMissingValue(const QVariant &value)
{
    if (!value.isValid() || value.isNull()) {
        return true;
    }

    if (value.type() == QVariant::String) {
        return value.toString().trimmed().isEmpty();
    }

    return false;
}
}

AverraFormModel::AverraFormModel(QObject *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    qRegisterMetaType<AverraValidationIssue>("AverraValidationIssue");
    qRegisterMetaType<AverraValidationResult>("AverraValidationResult");
    qRegisterMetaType<QList<AverraValidationIssue>>("QList<AverraValidationIssue>");

    m_readOnly = false;
}

QList<AverraFieldDefinition> AverraFormModel::fields() const
{
    return m_fields;
}

void AverraFormModel::setFields(const QList<AverraFieldDefinition> &fields)
{
    const bool previousDirty = isDirty();
    m_fields = fields;
    m_values = mergedValues(m_values);
    m_committedValues = mergedValues(m_committedValues);
    updateValidation(false);
    emit fieldsChanged();
    emit valuesChanged(m_values);
    emit committedValuesChanged(m_committedValues);
    emitDirtyIfNeeded(previousDirty);
    emit validationChanged(m_validationResult);
}

void AverraFormModel::addField(const AverraFieldDefinition &field)
{
    QList<AverraFieldDefinition> fields = m_fields;
    fields.append(field);
    setFields(fields);
}

void AverraFormModel::clearFields()
{
    const bool previousDirty = isDirty();
    m_fields.clear();
    m_values.clear();
    m_committedValues.clear();
    updateValidation(false);
    emit fieldsChanged();
    emit valuesChanged(m_values);
    emit committedValuesChanged(m_committedValues);
    emitDirtyIfNeeded(previousDirty);
    emit validationChanged(m_validationResult);
}

QString AverraFormModel::objectId() const
{
    return m_objectId;
}

void AverraFormModel::setObjectId(const QString &objectId)
{
    if (m_objectId == objectId) {
        return;
    }

    m_objectId = objectId;
    emit objectIdChanged(m_objectId);
}

bool AverraFormModel::readOnly() const
{
    return m_readOnly;
}

void AverraFormModel::setReadOnly(bool readOnly)
{
    if (m_readOnly == readOnly) {
        return;
    }

    m_readOnly = readOnly;
    emit readOnlyChanged(m_readOnly);
}

QVariantMap AverraFormModel::values() const
{
    return m_values;
}

void AverraFormModel::setValues(const QVariantMap &values)
{
    const QVariantMap normalizedValues = mergedValues(values);

    if (m_values == normalizedValues) {
        return;
    }

    const bool previousDirty = isDirty();
    m_values = normalizedValues;
    updateValidation(false);
    emit valuesChanged(m_values);
    emitDirtyIfNeeded(previousDirty);
    emit validationChanged(m_validationResult);
}

QVariantMap AverraFormModel::committedValues() const
{
    return m_committedValues;
}

QVariant AverraFormModel::value(const QString &key) const
{
    return m_values.value(key);
}

void AverraFormModel::setValue(const QString &key, const QVariant &value)
{
    const QString normalizedKey = key.trimmed();

    if (normalizedKey.isEmpty()) {
        return;
    }

    const bool previousDirty = isDirty();
    QVariant normalizedValueVariant = value;
    const AverraFieldDefinition *field = fieldDefinition(normalizedKey);

    if (field != nullptr) {
        normalizedValueVariant = normalizeValue(*field, value);
    }

    if (m_values.value(normalizedKey) == normalizedValueVariant) {
        return;
    }

    m_values.insert(normalizedKey, normalizedValueVariant);
    updateValidation(false);
    emit valueChanged(normalizedKey, normalizedValueVariant);
    emit valuesChanged(m_values);
    emitDirtyIfNeeded(previousDirty);
    emit validationChanged(m_validationResult);
}

void AverraFormModel::loadObject(const QString &objectId, const QVariantMap &values)
{
    const bool previousDirty = isDirty();
    setObjectId(objectId);
    m_committedValues = mergedValues(values);
    m_values = m_committedValues;
    updateValidation(false);
    emit committedValuesChanged(m_committedValues);
    emit valuesChanged(m_values);
    emitDirtyIfNeeded(previousDirty);
    emit validationChanged(m_validationResult);
}

void AverraFormModel::commit()
{
    const bool previousDirty = isDirty();
    const QVariantMap normalizedValues = mergedValues(m_values);

    if (m_committedValues == normalizedValues) {
        updateValidation();
        return;
    }

    m_values = normalizedValues;
    m_committedValues = normalizedValues;
    updateValidation(false);
    emit committedValuesChanged(m_committedValues);
    emit valuesChanged(m_values);
    emitDirtyIfNeeded(previousDirty);
    emit validationChanged(m_validationResult);
}

void AverraFormModel::rollback()
{
    if (m_values == m_committedValues) {
        return;
    }

    const bool previousDirty = isDirty();
    m_values = m_committedValues;
    updateValidation(false);
    emit valuesChanged(m_values);
    emitDirtyIfNeeded(previousDirty);
    emit validationChanged(m_validationResult);
}

bool AverraFormModel::isDirty() const
{
    return m_values != m_committedValues;
}

AverraValidationResult AverraFormModel::validate() const
{
    return validateValues(m_values);
}

AverraValidationResult AverraFormModel::validationResult() const
{
    return m_validationResult;
}

void AverraFormModel::refreshValidation()
{
    updateValidation();
}

bool AverraFormModel::saveState(QSettings &settings, const QString &settingsKey) const
{
    const QString normalizedKey = settingsKey.trimmed();

    if (normalizedKey.isEmpty()) {
        return false;
    }

    settings.setValue(normalizedKey + QStringLiteral("/objectId"), m_objectId);
    settings.setValue(normalizedKey + QStringLiteral("/values"), m_values);
    return true;
}

bool AverraFormModel::restoreState(QSettings &settings, const QString &settingsKey)
{
    const QString normalizedKey = settingsKey.trimmed();

    if (normalizedKey.isEmpty()) {
        return false;
    }

    const QVariant valuesVariant = settings.value(normalizedKey + QStringLiteral("/values"));

    if (!valuesVariant.isValid()) {
        return false;
    }

    setObjectId(settings.value(normalizedKey + QStringLiteral("/objectId")).toString());
    setValues(valuesVariant.toMap());
    return true;
}

const AverraFieldDefinition *AverraFormModel::fieldDefinition(const QString &key) const
{
    for (int index = 0; index < m_fields.size(); ++index) {
        if (m_fields.at(index).key() == key) {
            return &m_fields.at(index);
        }
    }

    return nullptr;
}

QVariant AverraFormModel::normalizeValue(const AverraFieldDefinition &field, const QVariant &value) const
{
    if (!value.isValid() || value.isNull()) {
        return field.defaultValue();
    }

    switch (field.fieldType()) {
    case AverraFieldDefinition::BooleanField:
        return value.toBool();
    case AverraFieldDefinition::IntegerField: {
        int intValue = value.toInt();

        if (field.minimumValue() <= field.maximumValue()) {
            intValue = qBound(field.minimumValue(), intValue, field.maximumValue());
        }

        return intValue;
    }
    case AverraFieldDefinition::EnumField: {
        const QList<AverraFieldOption> options = field.options();

        if (options.isEmpty()) {
            return value;
        }

        for (int index = 0; index < options.size(); ++index) {
            if (options.at(index).value() == value || options.at(index).text() == value.toString()) {
                return options.at(index).value();
            }
        }

        if (field.defaultValue().isValid()) {
            return field.defaultValue();
        }

        return options.first().value();
    }
    case AverraFieldDefinition::ReadOnlyTextField:
    case AverraFieldDefinition::TextField:
    case AverraFieldDefinition::MultilineTextField:
        return value.toString();
    }

    return value;
}

QVariantMap AverraFormModel::mergedValues(const QVariantMap &values) const
{
    QVariantMap mergedValues = values;

    for (int index = 0; index < m_fields.size(); ++index) {
        const AverraFieldDefinition &field = m_fields.at(index);
        const QString key = field.key().trimmed();

        if (key.isEmpty()) {
            continue;
        }

        if (mergedValues.contains(key)) {
            mergedValues.insert(key, normalizeValue(field, mergedValues.value(key)));
            continue;
        }

        mergedValues.insert(key, normalizeValue(field, field.defaultValue()));
    }

    return mergedValues;
}

AverraValidationResult AverraFormModel::validateValues(const QVariantMap &values) const
{
    AverraValidationResult result;
    QStringList issueMessages;

    for (int index = 0; index < m_fields.size(); ++index) {
        const AverraFieldDefinition &field = m_fields.at(index);
        const QString key = field.key().trimmed();
        const QVariant currentValue = values.value(key, field.defaultValue());

        if (field.fieldType() == AverraFieldDefinition::EnumField && !field.options().isEmpty()) {
            bool found = false;
            const QList<AverraFieldOption> options = field.options();

            for (int optionIndex = 0; optionIndex < options.size(); ++optionIndex) {
                if (options.at(optionIndex).value() == currentValue) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                const QString message = QStringLiteral("%1取值无效。").arg(fieldLabel(field));
                result.addIssue(AverraValidationIssue(key, message));
                issueMessages.append(message);
                continue;
            }
        }

        const QList<AverraValidationRule> rules = field.validationRules();

        for (int ruleIndex = 0; ruleIndex < rules.size(); ++ruleIndex) {
            const AverraValidationRule &rule = rules.at(ruleIndex);

            if (rule.ruleType() == AverraValidationRule::RequiredRule) {
                if (isMissingValue(currentValue)) {
                    const QString message = rule.message().isEmpty() ? requiredMessageForField(field)
                                                                     : rule.message();
                    result.addIssue(AverraValidationIssue(key, message));
                    issueMessages.append(message);
                }

                continue;
            }

            if (rule.ruleType() == AverraValidationRule::MinimumRule) {
                bool ok = false;
                const double numberValue = currentValue.toDouble(&ok);

                if (ok && numberValue < rule.numericValue()) {
                    const QString message = rule.message().isEmpty() ? minimumMessageForField(field, rule.numericValue())
                                                                     : rule.message();
                    result.addIssue(AverraValidationIssue(key, message));
                    issueMessages.append(message);
                }

                continue;
            }

            if (rule.ruleType() == AverraValidationRule::MaximumRule) {
                bool ok = false;
                const double numberValue = currentValue.toDouble(&ok);

                if (ok && numberValue > rule.numericValue()) {
                    const QString message = rule.message().isEmpty() ? maximumMessageForField(field, rule.numericValue())
                                                                     : rule.message();
                    result.addIssue(AverraValidationIssue(key, message));
                    issueMessages.append(message);
                }

                continue;
            }

            if (rule.ruleType() == AverraValidationRule::RegularExpressionRule) {
                const QRegularExpression expression(rule.pattern());
                const QString currentText = currentValue.toString();

                if (!currentText.isEmpty() && expression.isValid() && !expression.match(currentText).hasMatch()) {
                    const QString message = rule.message().isEmpty() ? regularExpressionMessageForField(field)
                                                                     : rule.message();
                    result.addIssue(AverraValidationIssue(key, message));
                    issueMessages.append(message);
                }
            }
        }
    }

    if (result.hasIssues()) {
        result.setSummaryTitle(QStringLiteral("表单校验未通过"));
        result.setSummaryDescription(issueMessages.join(QStringLiteral("\n")));
    } else {
        result.setSummaryTitle(QStringLiteral("表单校验通过"));
        result.setSummaryDescription(QStringLiteral("当前字段值可以提交。"));
    }

    return result;
}

void AverraFormModel::updateValidation(bool emitSignal)
{
    m_validationResult = validateValues(m_values);

    if (emitSignal) {
        emit validationChanged(m_validationResult);
    }
}

void AverraFormModel::emitDirtyIfNeeded(bool previousDirty)
{
    const bool currentDirty = isDirty();

    if (previousDirty != currentDirty) {
        emit dirtyChanged(currentDirty);
    }
}
