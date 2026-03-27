#include <Averra/AverraFieldDefinition.h>

#include <QtGlobal>

AverraFieldOption::AverraFieldOption()
{
}

AverraFieldOption::AverraFieldOption(const QString &text,
                                     const QVariant &value,
                                     const QString &description)
{
    m_text = text;
    m_value = value;
    m_description = description;
}

QString AverraFieldOption::text() const
{
    return m_text;
}

void AverraFieldOption::setText(const QString &text)
{
    m_text = text;
}

QVariant AverraFieldOption::value() const
{
    return m_value;
}

void AverraFieldOption::setValue(const QVariant &value)
{
    m_value = value;
}

QString AverraFieldOption::description() const
{
    return m_description;
}

void AverraFieldOption::setDescription(const QString &description)
{
    m_description = description;
}

AverraValidationRule::AverraValidationRule()
{
    m_ruleType = RequiredRule;
    m_numericValue = 0.0;
}

AverraValidationRule::AverraValidationRule(RuleType type)
{
    m_ruleType = type;
    m_numericValue = 0.0;
}

AverraValidationRule AverraValidationRule::required(const QString &message)
{
    AverraValidationRule rule(RequiredRule);
    rule.setMessage(message);
    return rule;
}

AverraValidationRule AverraValidationRule::minimum(double value, const QString &message)
{
    AverraValidationRule rule(MinimumRule);
    rule.setNumericValue(value);
    rule.setMessage(message);
    return rule;
}

AverraValidationRule AverraValidationRule::maximum(double value, const QString &message)
{
    AverraValidationRule rule(MaximumRule);
    rule.setNumericValue(value);
    rule.setMessage(message);
    return rule;
}

AverraValidationRule AverraValidationRule::regularExpression(const QString &pattern,
                                                             const QString &message)
{
    AverraValidationRule rule(RegularExpressionRule);
    rule.setPattern(pattern);
    rule.setMessage(message);
    return rule;
}

AverraValidationRule::RuleType AverraValidationRule::ruleType() const
{
    return m_ruleType;
}

void AverraValidationRule::setRuleType(RuleType ruleType)
{
    m_ruleType = ruleType;
}

double AverraValidationRule::numericValue() const
{
    return m_numericValue;
}

void AverraValidationRule::setNumericValue(double numericValue)
{
    m_numericValue = numericValue;
}

QString AverraValidationRule::pattern() const
{
    return m_pattern;
}

void AverraValidationRule::setPattern(const QString &pattern)
{
    m_pattern = pattern;
}

QString AverraValidationRule::message() const
{
    return m_message;
}

void AverraValidationRule::setMessage(const QString &message)
{
    m_message = message;
}

AverraFieldDefinition::AverraFieldDefinition()
{
    m_fieldType = TextField;
    m_readOnly = false;
    m_minimumValue = 0;
    m_maximumValue = 100;
}

AverraFieldDefinition::AverraFieldDefinition(const QString &key, FieldType fieldType)
{
    m_key = key;
    m_fieldType = fieldType;
    m_readOnly = false;
    m_minimumValue = 0;
    m_maximumValue = 100;
}

AverraFieldDefinition AverraFieldDefinition::createReadOnlyText(const QString &key,
                                                                const QString &title,
                                                                const QVariant &defaultValue,
                                                                const QString &description)
{
    AverraFieldDefinition field(key, ReadOnlyTextField);
    field.setTitle(title);
    field.setDefaultValue(defaultValue);
    field.setDescription(description);
    field.setReadOnly(true);
    return field;
}

AverraFieldDefinition AverraFieldDefinition::createText(const QString &key,
                                                        const QString &title,
                                                        const QVariant &defaultValue,
                                                        const QString &description)
{
    AverraFieldDefinition field(key, TextField);
    field.setTitle(title);
    field.setDefaultValue(defaultValue);
    field.setDescription(description);
    return field;
}

AverraFieldDefinition AverraFieldDefinition::createMultilineText(const QString &key,
                                                                 const QString &title,
                                                                 const QVariant &defaultValue,
                                                                 const QString &description)
{
    AverraFieldDefinition field(key, MultilineTextField);
    field.setTitle(title);
    field.setDefaultValue(defaultValue);
    field.setDescription(description);
    return field;
}

AverraFieldDefinition AverraFieldDefinition::createBoolean(const QString &key,
                                                           const QString &title,
                                                           bool defaultValue,
                                                           const QString &description)
{
    AverraFieldDefinition field(key, BooleanField);
    field.setTitle(title);
    field.setDefaultValue(defaultValue);
    field.setDescription(description);
    return field;
}

AverraFieldDefinition AverraFieldDefinition::createEnum(const QString &key,
                                                        const QString &title,
                                                        const QList<AverraFieldOption> &options,
                                                        const QVariant &defaultValue,
                                                        const QString &description)
{
    AverraFieldDefinition field(key, EnumField);
    field.setTitle(title);
    field.setOptions(options);
    field.setDescription(description);

    if (defaultValue.isValid()) {
        field.setDefaultValue(defaultValue);
    } else if (!options.isEmpty()) {
        field.setDefaultValue(options.first().value());
    }

    return field;
}

AverraFieldDefinition AverraFieldDefinition::createInteger(const QString &key,
                                                           const QString &title,
                                                           int minimumValue,
                                                           int maximumValue,
                                                           int defaultValue,
                                                           const QString &description)
{
    AverraFieldDefinition field(key, IntegerField);
    field.setTitle(title);
    field.setRange(minimumValue, maximumValue);
    field.setDefaultValue(defaultValue);
    field.setDescription(description);
    return field;
}

QString AverraFieldDefinition::key() const
{
    return m_key;
}

void AverraFieldDefinition::setKey(const QString &key)
{
    m_key = key;
}

QString AverraFieldDefinition::title() const
{
    return m_title;
}

void AverraFieldDefinition::setTitle(const QString &title)
{
    m_title = title;
}

QString AverraFieldDefinition::description() const
{
    return m_description;
}

void AverraFieldDefinition::setDescription(const QString &description)
{
    m_description = description;
}

AverraFieldDefinition::FieldType AverraFieldDefinition::fieldType() const
{
    return m_fieldType;
}

void AverraFieldDefinition::setFieldType(FieldType fieldType)
{
    m_fieldType = fieldType;
}

QVariant AverraFieldDefinition::defaultValue() const
{
    return m_defaultValue;
}

void AverraFieldDefinition::setDefaultValue(const QVariant &defaultValue)
{
    m_defaultValue = defaultValue;
}

QString AverraFieldDefinition::groupKey() const
{
    return m_groupKey;
}

void AverraFieldDefinition::setGroupKey(const QString &groupKey)
{
    m_groupKey = groupKey;
}

QString AverraFieldDefinition::groupTitle() const
{
    return m_groupTitle;
}

void AverraFieldDefinition::setGroupTitle(const QString &groupTitle)
{
    m_groupTitle = groupTitle;
}

QString AverraFieldDefinition::groupDescription() const
{
    return m_groupDescription;
}

void AverraFieldDefinition::setGroupDescription(const QString &groupDescription)
{
    m_groupDescription = groupDescription;
}

void AverraFieldDefinition::setGroup(const QString &groupKey,
                                     const QString &groupTitle,
                                     const QString &groupDescription)
{
    m_groupKey = groupKey;
    m_groupTitle = groupTitle;
    m_groupDescription = groupDescription;
}

bool AverraFieldDefinition::readOnly() const
{
    return m_readOnly;
}

void AverraFieldDefinition::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
}

QList<AverraFieldOption> AverraFieldDefinition::options() const
{
    return m_options;
}

void AverraFieldDefinition::setOptions(const QList<AverraFieldOption> &options)
{
    m_options = options;
}

QList<AverraValidationRule> AverraFieldDefinition::validationRules() const
{
    return m_validationRules;
}

void AverraFieldDefinition::setValidationRules(const QList<AverraValidationRule> &validationRules)
{
    m_validationRules = validationRules;
}

void AverraFieldDefinition::addValidationRule(const AverraValidationRule &validationRule)
{
    m_validationRules.append(validationRule);
}

int AverraFieldDefinition::minimumValue() const
{
    return m_minimumValue;
}

void AverraFieldDefinition::setMinimumValue(int minimumValue)
{
    m_minimumValue = minimumValue;
}

int AverraFieldDefinition::maximumValue() const
{
    return m_maximumValue;
}

void AverraFieldDefinition::setMaximumValue(int maximumValue)
{
    m_maximumValue = maximumValue;
}

void AverraFieldDefinition::setRange(int minimumValue, int maximumValue)
{
    m_minimumValue = qMin(minimumValue, maximumValue);
    m_maximumValue = qMax(minimumValue, maximumValue);
}

QString AverraFieldDefinition::placeholderText() const
{
    return m_placeholderText;
}

void AverraFieldDefinition::setPlaceholderText(const QString &placeholderText)
{
    m_placeholderText = placeholderText;
}

bool operator==(const AverraFieldOption &left, const AverraFieldOption &right)
{
    return left.text() == right.text()
           && left.value() == right.value()
           && left.description() == right.description();
}

bool operator!=(const AverraFieldOption &left, const AverraFieldOption &right)
{
    return !(left == right);
}

bool operator==(const AverraValidationRule &left, const AverraValidationRule &right)
{
    return left.ruleType() == right.ruleType()
           && qFuzzyCompare(left.numericValue() + 1.0, right.numericValue() + 1.0)
           && left.pattern() == right.pattern()
           && left.message() == right.message();
}

bool operator!=(const AverraValidationRule &left, const AverraValidationRule &right)
{
    return !(left == right);
}

bool operator==(const AverraFieldDefinition &left, const AverraFieldDefinition &right)
{
    return left.key() == right.key()
           && left.title() == right.title()
           && left.description() == right.description()
           && left.fieldType() == right.fieldType()
           && left.defaultValue() == right.defaultValue()
           && left.groupKey() == right.groupKey()
           && left.groupTitle() == right.groupTitle()
           && left.groupDescription() == right.groupDescription()
           && left.readOnly() == right.readOnly()
           && left.options() == right.options()
           && left.validationRules() == right.validationRules()
           && left.minimumValue() == right.minimumValue()
           && left.maximumValue() == right.maximumValue()
           && left.placeholderText() == right.placeholderText();
}

bool operator!=(const AverraFieldDefinition &left, const AverraFieldDefinition &right)
{
    return !(left == right);
}
