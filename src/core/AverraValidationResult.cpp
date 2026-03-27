#include <Averra/AverraValidationResult.h>

AverraValidationIssue::AverraValidationIssue()
{
    m_severity = ErrorSeverity;
}

AverraValidationIssue::AverraValidationIssue(const QString &fieldKey,
                                             const QString &message,
                                             Severity severity,
                                             const QString &description)
{
    m_fieldKey = fieldKey;
    m_message = message;
    m_description = description;
    m_severity = severity;
}

QString AverraValidationIssue::fieldKey() const
{
    return m_fieldKey;
}

void AverraValidationIssue::setFieldKey(const QString &fieldKey)
{
    m_fieldKey = fieldKey;
}

QString AverraValidationIssue::message() const
{
    return m_message;
}

void AverraValidationIssue::setMessage(const QString &message)
{
    m_message = message;
}

QString AverraValidationIssue::description() const
{
    return m_description;
}

void AverraValidationIssue::setDescription(const QString &description)
{
    m_description = description;
}

AverraValidationIssue::Severity AverraValidationIssue::severity() const
{
    return m_severity;
}

void AverraValidationIssue::setSeverity(Severity severity)
{
    m_severity = severity;
}

AverraValidationResult::AverraValidationResult()
{
}

QList<AverraValidationIssue> AverraValidationResult::issues() const
{
    return m_issues;
}

void AverraValidationResult::setIssues(const QList<AverraValidationIssue> &issues)
{
    m_issues = issues;
}

void AverraValidationResult::addIssue(const AverraValidationIssue &issue)
{
    m_issues.append(issue);
}

void AverraValidationResult::clearIssues()
{
    m_issues.clear();
}

QString AverraValidationResult::summaryTitle() const
{
    return m_summaryTitle;
}

void AverraValidationResult::setSummaryTitle(const QString &summaryTitle)
{
    m_summaryTitle = summaryTitle;
}

QString AverraValidationResult::summaryDescription() const
{
    return m_summaryDescription;
}

void AverraValidationResult::setSummaryDescription(const QString &summaryDescription)
{
    m_summaryDescription = summaryDescription;
}

bool AverraValidationResult::hasIssues() const
{
    return !m_issues.isEmpty();
}

bool AverraValidationResult::hasErrors() const
{
    for (int index = 0; index < m_issues.size(); ++index) {
        if (m_issues.at(index).severity() == AverraValidationIssue::ErrorSeverity) {
            return true;
        }
    }

    return false;
}

bool AverraValidationResult::hasWarnings() const
{
    for (int index = 0; index < m_issues.size(); ++index) {
        if (m_issues.at(index).severity() == AverraValidationIssue::WarningSeverity) {
            return true;
        }
    }

    return false;
}

bool AverraValidationResult::isValid() const
{
    return !hasErrors();
}

AverraValidationIssue::Severity AverraValidationResult::highestSeverity() const
{
    AverraValidationIssue::Severity severity = AverraValidationIssue::SuccessSeverity;

    for (int index = 0; index < m_issues.size(); ++index) {
        if (m_issues.at(index).severity() > severity) {
            severity = m_issues.at(index).severity();
        }
    }

    return severity;
}

QList<AverraValidationIssue> AverraValidationResult::fieldIssues(const QString &fieldKey) const
{
    QList<AverraValidationIssue> matches;

    for (int index = 0; index < m_issues.size(); ++index) {
        const AverraValidationIssue &issue = m_issues.at(index);

        if (issue.fieldKey() == fieldKey) {
            matches.append(issue);
        }
    }

    return matches;
}

QString AverraValidationResult::fieldMessage(const QString &fieldKey) const
{
    const QList<AverraValidationIssue> matches = fieldIssues(fieldKey);

    if (matches.isEmpty()) {
        return QString();
    }

    return matches.first().message();
}

QString AverraValidationResult::fieldDescription(const QString &fieldKey) const
{
    const QList<AverraValidationIssue> matches = fieldIssues(fieldKey);

    if (matches.isEmpty()) {
        return QString();
    }

    return matches.first().description();
}

bool operator==(const AverraValidationIssue &left, const AverraValidationIssue &right)
{
    return left.fieldKey() == right.fieldKey()
           && left.message() == right.message()
           && left.description() == right.description()
           && left.severity() == right.severity();
}

bool operator!=(const AverraValidationIssue &left, const AverraValidationIssue &right)
{
    return !(left == right);
}

bool operator==(const AverraValidationResult &left, const AverraValidationResult &right)
{
    return left.issues() == right.issues()
           && left.summaryTitle() == right.summaryTitle()
           && left.summaryDescription() == right.summaryDescription();
}

bool operator!=(const AverraValidationResult &left, const AverraValidationResult &right)
{
    return !(left == right);
}
