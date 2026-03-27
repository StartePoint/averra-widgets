#ifndef AVERRA_VALIDATION_RESULT_H
#define AVERRA_VALIDATION_RESULT_H

#include <Averra/AverraGlobal.h>

#include <QObject>
#include <QList>
#include <QString>

class AVERRA_EXPORT AverraValidationIssue
{
    Q_GADGET

public:
    enum Severity
    {
        InfoSeverity,
        SuccessSeverity,
        WarningSeverity,
        ErrorSeverity
    };
    Q_ENUM(Severity)

    AverraValidationIssue();
    AverraValidationIssue(const QString &fieldKey,
                          const QString &message,
                          Severity severity = ErrorSeverity,
                          const QString &description = QString());

    QString fieldKey() const;
    void setFieldKey(const QString &fieldKey);

    QString message() const;
    void setMessage(const QString &message);

    QString description() const;
    void setDescription(const QString &description);

    Severity severity() const;
    void setSeverity(Severity severity);

private:
    QString m_fieldKey;
    QString m_message;
    QString m_description;
    Severity m_severity;
};

class AVERRA_EXPORT AverraValidationResult
{
public:
    AverraValidationResult();

    QList<AverraValidationIssue> issues() const;
    void setIssues(const QList<AverraValidationIssue> &issues);
    void addIssue(const AverraValidationIssue &issue);
    void clearIssues();

    QString summaryTitle() const;
    void setSummaryTitle(const QString &summaryTitle);

    QString summaryDescription() const;
    void setSummaryDescription(const QString &summaryDescription);

    bool hasIssues() const;
    bool hasErrors() const;
    bool hasWarnings() const;
    bool isValid() const;

    AverraValidationIssue::Severity highestSeverity() const;
    QList<AverraValidationIssue> fieldIssues(const QString &fieldKey) const;
    QString fieldMessage(const QString &fieldKey) const;
    QString fieldDescription(const QString &fieldKey) const;

private:
    QList<AverraValidationIssue> m_issues;
    QString m_summaryTitle;
    QString m_summaryDescription;
};

AVERRA_EXPORT bool operator==(const AverraValidationIssue &left, const AverraValidationIssue &right);
AVERRA_EXPORT bool operator!=(const AverraValidationIssue &left, const AverraValidationIssue &right);
AVERRA_EXPORT bool operator==(const AverraValidationResult &left, const AverraValidationResult &right);
AVERRA_EXPORT bool operator!=(const AverraValidationResult &left, const AverraValidationResult &right);

Q_DECLARE_METATYPE(AverraValidationIssue)
Q_DECLARE_METATYPE(AverraValidationResult)
Q_DECLARE_METATYPE(QList<AverraValidationIssue>)

#endif // AVERRA_VALIDATION_RESULT_H
