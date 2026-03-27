#ifndef AVERRA_SEARCH_BAR_H
#define AVERRA_SEARCH_BAR_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QFrame;
class QLineEdit;
class QPushButton;

class AVERRA_EXPORT AverraSearchBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)
    Q_PROPERTY(QString buttonText READ buttonText WRITE setButtonText)
    Q_PROPERTY(bool accentFrame READ accentFrame WRITE setAccentFrame NOTIFY accentFrameChanged)

public:
    explicit AverraSearchBar(QWidget *parent = nullptr);

    QString text() const;
    void setText(const QString &text);

    QString placeholderText() const;
    void setPlaceholderText(const QString &placeholderText);

    QString buttonText() const;
    void setButtonText(const QString &buttonText);

    bool accentFrame() const;
    void setAccentFrame(bool accentFrame);

signals:
    void textChanged(const QString &text);
    void searchRequested(const QString &text);
    void accentFrameChanged(bool accentFrame);

private slots:
    void initialize();
    void refreshStyle();
    void emitSearchRequested();

private:
    bool m_accentFrame;
    QFrame *m_rootFrame;
    QLineEdit *m_lineEdit;
    QPushButton *m_searchButton;
};

#endif // AVERRA_SEARCH_BAR_H

