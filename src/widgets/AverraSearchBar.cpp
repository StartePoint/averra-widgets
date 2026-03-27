#include <Averra/AverraSearchBar.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

AverraSearchBar::AverraSearchBar(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QString AverraSearchBar::text() const
{
    return m_lineEdit->text();
}

void AverraSearchBar::setText(const QString &text)
{
    if (m_lineEdit->text() == text) {
        return;
    }

    m_lineEdit->setText(text);
}

QString AverraSearchBar::placeholderText() const
{
    return m_lineEdit->placeholderText();
}

void AverraSearchBar::setPlaceholderText(const QString &placeholderText)
{
    m_lineEdit->setPlaceholderText(placeholderText);
}

QString AverraSearchBar::buttonText() const
{
    return m_searchButton->text();
}

void AverraSearchBar::setButtonText(const QString &buttonText)
{
    m_searchButton->setText(buttonText);
}

bool AverraSearchBar::accentFrame() const
{
    return m_accentFrame;
}

void AverraSearchBar::setAccentFrame(bool accentFrame)
{
    if (m_accentFrame == accentFrame) {
        return;
    }

    m_accentFrame = accentFrame;
    refreshStyle();
    emit accentFrameChanged(m_accentFrame);
}

void AverraSearchBar::initialize()
{
    m_accentFrame = false;
    m_rootFrame = new QFrame(this);
    m_lineEdit = new QLineEdit(m_rootFrame);
    m_searchButton = new QPushButton(m_rootFrame);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(48);

    m_rootFrame->setObjectName(QStringLiteral("AverraSearchBarRoot"));
    m_lineEdit->setObjectName(QStringLiteral("AverraSearchBarLineEdit"));
    m_searchButton->setObjectName(QStringLiteral("AverraSearchBarButton"));

    m_lineEdit->setClearButtonEnabled(true);
    m_searchButton->setText(QStringLiteral("搜索"));

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QHBoxLayout *rootLayout = new QHBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(10, 8, 10, 8);
    rootLayout->setSpacing(8);
    rootLayout->addWidget(m_lineEdit, 1);
    rootLayout->addWidget(m_searchButton);

    connect(m_lineEdit,
            &QLineEdit::textChanged,
            this,
            &AverraSearchBar::textChanged);

    connect(m_lineEdit,
            &QLineEdit::returnPressed,
            this,
            &AverraSearchBar::emitSearchRequested);

    connect(m_searchButton,
            &QPushButton::clicked,
            this,
            &AverraSearchBar::emitSearchRequested);

    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraSearchBar::refreshStyle);

    refreshStyle();
}

void AverraSearchBar::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setStyleSheet(AverraStyleHelper::searchBarFrameStyleSheet(palette, m_accentFrame));
    m_lineEdit->setStyleSheet(AverraStyleHelper::searchBarLineEditStyleSheet(palette));
    m_searchButton->setStyleSheet(AverraStyleHelper::searchBarButtonStyleSheet(palette, m_accentFrame));
}

void AverraSearchBar::emitSearchRequested()
{
    emit searchRequested(m_lineEdit->text());
}
