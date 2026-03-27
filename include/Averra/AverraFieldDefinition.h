#ifndef AVERRA_FIELD_DEFINITION_H
#define AVERRA_FIELD_DEFINITION_H

#include <Averra/AverraGlobal.h>

#include <QObject>
#include <QList>
#include <QString>
#include <QVariant>

class AVERRA_EXPORT AverraFieldOption
{
public:
    AverraFieldOption();
    AverraFieldOption(const QString &text,
                      const QVariant &value,
                      const QString &description = QString());

    QString text() const;
    void setText(const QString &text);

    QVariant value() const;
    void setValue(const QVariant &value);

    QString description() const;
    void setDescription(const QString &description);

private:
    QString m_text;
    QVariant m_value;
    QString m_description;
};

class AVERRA_EXPORT AverraValidationRule
{
    Q_GADGET

public:
    enum RuleType
    {
        RequiredRule,
        MinimumRule,
        MaximumRule,
        RegularExpressionRule
    };
    Q_ENUM(RuleType)

    AverraValidationRule();
    explicit AverraValidationRule(RuleType type);

    static AverraValidationRule required(const QString &message = QString());
    static AverraValidationRule minimum(double value, const QString &message = QString());
    static AverraValidationRule maximum(double value, const QString &message = QString());
    static AverraValidationRule regularExpression(const QString &pattern, const QString &message = QString());

    RuleType ruleType() const;
    void setRuleType(RuleType ruleType);

    double numericValue() const;
    void setNumericValue(double numericValue);

    QString pattern() const;
    void setPattern(const QString &pattern);

    QString message() const;
    void setMessage(const QString &message);

private:
    RuleType m_ruleType;
    double m_numericValue;
    QString m_pattern;
    QString m_message;
};

class AVERRA_EXPORT AverraFieldDefinition
{
    Q_GADGET

public:
    enum FieldType
    {
        ReadOnlyTextField,
        TextField,
        MultilineTextField,
        BooleanField,
        EnumField,
        IntegerField
    };
    Q_ENUM(FieldType)

    AverraFieldDefinition();
    explicit AverraFieldDefinition(const QString &key, FieldType fieldType = TextField);

    static AverraFieldDefinition createReadOnlyText(const QString &key,
                                                    const QString &title,
                                                    const QVariant &defaultValue = QVariant(),
                                                    const QString &description = QString());
    static AverraFieldDefinition createText(const QString &key,
                                            const QString &title,
                                            const QVariant &defaultValue = QVariant(),
                                            const QString &description = QString());
    static AverraFieldDefinition createMultilineText(const QString &key,
                                                     const QString &title,
                                                     const QVariant &defaultValue = QVariant(),
                                                     const QString &description = QString());
    static AverraFieldDefinition createBoolean(const QString &key,
                                               const QString &title,
                                               bool defaultValue = false,
                                               const QString &description = QString());
    static AverraFieldDefinition createEnum(const QString &key,
                                            const QString &title,
                                            const QList<AverraFieldOption> &options,
                                            const QVariant &defaultValue = QVariant(),
                                            const QString &description = QString());
    static AverraFieldDefinition createInteger(const QString &key,
                                               const QString &title,
                                               int minimumValue,
                                               int maximumValue,
                                               int defaultValue = 0,
                                               const QString &description = QString());

    QString key() const;
    void setKey(const QString &key);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    FieldType fieldType() const;
    void setFieldType(FieldType fieldType);

    QVariant defaultValue() const;
    void setDefaultValue(const QVariant &defaultValue);

    QString groupKey() const;
    void setGroupKey(const QString &groupKey);

    QString groupTitle() const;
    void setGroupTitle(const QString &groupTitle);

    QString groupDescription() const;
    void setGroupDescription(const QString &groupDescription);

    void setGroup(const QString &groupKey,
                  const QString &groupTitle = QString(),
                  const QString &groupDescription = QString());

    bool readOnly() const;
    void setReadOnly(bool readOnly);

    QList<AverraFieldOption> options() const;
    void setOptions(const QList<AverraFieldOption> &options);

    QList<AverraValidationRule> validationRules() const;
    void setValidationRules(const QList<AverraValidationRule> &validationRules);
    void addValidationRule(const AverraValidationRule &validationRule);

    int minimumValue() const;
    void setMinimumValue(int minimumValue);

    int maximumValue() const;
    void setMaximumValue(int maximumValue);

    void setRange(int minimumValue, int maximumValue);

    QString placeholderText() const;
    void setPlaceholderText(const QString &placeholderText);

private:
    QString m_key;
    QString m_title;
    QString m_description;
    FieldType m_fieldType;
    QVariant m_defaultValue;
    QString m_groupKey;
    QString m_groupTitle;
    QString m_groupDescription;
    bool m_readOnly;
    QList<AverraFieldOption> m_options;
    QList<AverraValidationRule> m_validationRules;
    int m_minimumValue;
    int m_maximumValue;
    QString m_placeholderText;
};

AVERRA_EXPORT bool operator==(const AverraFieldOption &left, const AverraFieldOption &right);
AVERRA_EXPORT bool operator!=(const AverraFieldOption &left, const AverraFieldOption &right);
AVERRA_EXPORT bool operator==(const AverraValidationRule &left, const AverraValidationRule &right);
AVERRA_EXPORT bool operator!=(const AverraValidationRule &left, const AverraValidationRule &right);
AVERRA_EXPORT bool operator==(const AverraFieldDefinition &left, const AverraFieldDefinition &right);
AVERRA_EXPORT bool operator!=(const AverraFieldDefinition &left, const AverraFieldDefinition &right);

Q_DECLARE_METATYPE(AverraFieldOption)
Q_DECLARE_METATYPE(AverraValidationRule)
Q_DECLARE_METATYPE(AverraFieldDefinition)
Q_DECLARE_METATYPE(QList<AverraFieldOption>)
Q_DECLARE_METATYPE(QList<AverraValidationRule>)
Q_DECLARE_METATYPE(QList<AverraFieldDefinition>)

#endif // AVERRA_FIELD_DEFINITION_H
