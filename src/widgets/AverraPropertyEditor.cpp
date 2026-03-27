#include <Averra/AverraPropertyEditor.h>
#include <Averra/AverraFormSection.h>
#include <Averra/AverraPropertyItem.h>
#include <Averra/AverraSearchBar.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QFrame>
#include <QLabel>
#include <QLayoutItem>
#include <QScrollArea>
#include <QVBoxLayout>

namespace
{
bool containsText(const QString &text, const QString &pattern)
{
    if (pattern.isEmpty()) {
        return true;
    }

    return text.contains(pattern, Qt::CaseInsensitive);
}
}

AverraPropertyEditor::AverraPropertyEditor(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

QString AverraPropertyEditor::title() const
{
    return m_title;
}

void AverraPropertyEditor::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }

    m_title = title;
    m_titleLabel->setText(m_title);
    updateTextVisibility();
    emit titleChanged(m_title);
}

QString AverraPropertyEditor::description() const
{
    return m_description;
}

void AverraPropertyEditor::setDescription(const QString &description)
{
    if (m_description == description) {
        return;
    }

    m_description = description;
    m_descriptionLabel->setText(m_description);
    updateTextVisibility();
    emit descriptionChanged(m_description);
}

QString AverraPropertyEditor::filterText() const
{
    return m_filterText;
}

void AverraPropertyEditor::setFilterText(const QString &filterText)
{
    if (m_filterText == filterText) {
        return;
    }

    m_filterText = filterText;

    if (m_searchBar->text() != m_filterText) {
        m_searchBar->setText(m_filterText);
    }

    applyFilter();
    emit filterTextChanged(m_filterText);
}

bool AverraPropertyEditor::searchEnabled() const
{
    return m_searchEnabled;
}

void AverraPropertyEditor::setSearchEnabled(bool searchEnabled)
{
    if (m_searchEnabled == searchEnabled) {
        return;
    }

    m_searchEnabled = searchEnabled;
    updateSearchVisibility();
    emit searchEnabledChanged(m_searchEnabled);
}

bool AverraPropertyEditor::readOnly() const
{
    return m_readOnly;
}

void AverraPropertyEditor::setReadOnly(bool readOnly)
{
    if (m_readOnly == readOnly) {
        return;
    }

    m_readOnly = readOnly;
    syncReadOnlyState();
    emit readOnlyChanged(m_readOnly);
}

AverraSearchBar *AverraPropertyEditor::searchBar() const
{
    return m_searchBar;
}

QVBoxLayout *AverraPropertyEditor::contentLayout() const
{
    return m_contentLayout;
}

void AverraPropertyEditor::addSection(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }

    widget->setParent(m_contentWidget);
    widget->setEnabled(!m_readOnly);
    m_contentLayout->addWidget(widget);
    applyFilter();
}

void AverraPropertyEditor::clearSections()
{
    while (m_contentLayout->count() > 0) {
        QLayoutItem *item = m_contentLayout->takeAt(0);

        if (item == nullptr) {
            continue;
        }

        if (item->widget() != nullptr) {
            delete item->widget();
        }

        delete item;
    }
}

void AverraPropertyEditor::initialize()
{
    m_title.clear();
    m_description.clear();
    m_filterText.clear();
    m_searchEnabled = true;
    m_readOnly = false;
    m_rootFrame = new QFrame(this);
    m_titleLabel = new QLabel(m_rootFrame);
    m_descriptionLabel = new QLabel(m_rootFrame);
    m_searchBar = new AverraSearchBar(m_rootFrame);
    m_scrollArea = new QScrollArea(m_rootFrame);
    m_contentWidget = new QWidget;
    m_contentLayout = new QVBoxLayout;

    m_rootFrame->setObjectName(QStringLiteral("AverraPropertyEditorRoot"));
    m_titleLabel->setObjectName(QStringLiteral("AverraPropertyEditorTitle"));
    m_descriptionLabel->setObjectName(QStringLiteral("AverraPropertyEditorDescription"));

    m_searchBar->setPlaceholderText(QStringLiteral("搜索属性、分组或说明"));
    m_searchBar->setButtonText(QStringLiteral("筛选"));

    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(14);

    m_titleLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);

    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(12);
    m_contentWidget->setLayout(m_contentLayout);
    m_scrollArea->setWidget(m_contentWidget);

    rootLayout->addWidget(m_titleLabel);
    rootLayout->addWidget(m_descriptionLabel);
    rootLayout->addWidget(m_searchBar);
    rootLayout->addWidget(m_scrollArea);

    connect(m_searchBar,
            &AverraSearchBar::textChanged,
            this,
            &AverraPropertyEditor::setFilterText);
    connect(m_searchBar,
            &AverraSearchBar::searchRequested,
            this,
            &AverraPropertyEditor::setFilterText);
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraPropertyEditor::refreshStyle);

    updateTextVisibility();
    updateSearchVisibility();
    refreshStyle();
}

void AverraPropertyEditor::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    const AverraStyleProfile styleProfile = AverraThemeManager::instance()->styleProfile();
    m_rootFrame->setStyleSheet(
        QStringLiteral(
            "QFrame#AverraPropertyEditorRoot {"
            "background-color: %1;"
            "border: 1px solid %2;"
            "border-radius: %3px;"
            "}")
            .arg(palette.surfaceColor().name(),
                 palette.borderColor().name(),
                 QString::number(styleProfile.pageRadius())));
    m_titleLabel->setStyleSheet(
        QStringLiteral(
            "QLabel#AverraPropertyEditorTitle {"
            "color: %1;"
            "font-size: %2px;"
            "font-weight: 700;"
            "background: transparent;"
            "}")
            .arg(palette.textPrimaryColor().name(),
                 QString::number(styleProfile.pageTitleFontSize())));
    m_descriptionLabel->setStyleSheet(
        QStringLiteral(
            "QLabel#AverraPropertyEditorDescription {"
            "color: %1;"
            "font-size: %2px;"
            "font-weight: 500;"
            "background: transparent;"
            "}")
            .arg(palette.textSecondaryColor().name(),
                 QString::number(styleProfile.bodyFontSize())));
    m_contentWidget->setStyleSheet(QStringLiteral("background: transparent;"));
}

void AverraPropertyEditor::applyFilter()
{
    const QString pattern = m_filterText.trimmed();

    for (int index = 0; index < m_contentLayout->count(); ++index) {
        QLayoutItem *item = m_contentLayout->itemAt(index);

        if (item == nullptr || item->widget() == nullptr) {
            continue;
        }

        applyFilterToSection(item->widget(), pattern);
    }
}

bool AverraPropertyEditor::applyFilterToSection(QWidget *widget, const QString &pattern)
{
    if (widget == nullptr) {
        return false;
    }

    if (pattern.isEmpty()) {
        widget->setVisible(true);

        AverraFormSection *formSection = qobject_cast<AverraFormSection *>(widget);
        if (formSection != nullptr) {
            for (int index = 0; index < formSection->contentLayout()->count(); ++index) {
                QLayoutItem *item = formSection->contentLayout()->itemAt(index);

                if (item != nullptr && item->widget() != nullptr) {
                    item->widget()->setVisible(true);
                }
            }
        }

        return true;
    }

    AverraFormSection *formSection = qobject_cast<AverraFormSection *>(widget);
    if (formSection == nullptr) {
        const bool visible = widgetMatches(widget, pattern);
        widget->setVisible(visible);
        return visible;
    }

    const bool sectionMatches = widgetMatches(formSection, pattern);
    bool hasVisibleRows = false;

    for (int index = 0; index < formSection->contentLayout()->count(); ++index) {
        QLayoutItem *item = formSection->contentLayout()->itemAt(index);

        if (item == nullptr || item->widget() == nullptr) {
            continue;
        }

        const bool rowVisible = sectionMatches || widgetMatches(item->widget(), pattern);
        item->widget()->setVisible(rowVisible);
        hasVisibleRows = hasVisibleRows || rowVisible;
    }

    const bool sectionVisible = sectionMatches || hasVisibleRows;
    formSection->setVisible(sectionVisible);
    return sectionVisible;
}

bool AverraPropertyEditor::widgetMatches(QWidget *widget, const QString &pattern) const
{
    if (widget == nullptr) {
        return false;
    }

    if (pattern.isEmpty()) {
        return true;
    }

    const AverraPropertyItem *propertyItem = qobject_cast<AverraPropertyItem *>(widget);
    if (propertyItem != nullptr) {
        return containsText(propertyItem->title(), pattern)
               || containsText(propertyItem->description(), pattern);
    }

    const AverraFormSection *formSection = qobject_cast<AverraFormSection *>(widget);
    if (formSection != nullptr) {
        return containsText(formSection->title(), pattern)
               || containsText(formSection->description(), pattern)
               || containsText(formSection->metaText(), pattern);
    }

    return containsText(widget->windowTitle(), pattern)
           || containsText(widget->accessibleName(), pattern)
           || containsText(widget->toolTip(), pattern)
           || containsText(widget->objectName(), pattern);
}

void AverraPropertyEditor::syncReadOnlyState()
{
    for (int index = 0; index < m_contentLayout->count(); ++index) {
        QLayoutItem *item = m_contentLayout->itemAt(index);

        if (item == nullptr || item->widget() == nullptr) {
            continue;
        }

        item->widget()->setEnabled(!m_readOnly);
    }
}

void AverraPropertyEditor::updateSearchVisibility()
{
    m_searchBar->setVisible(m_searchEnabled);
}

void AverraPropertyEditor::updateTextVisibility()
{
    m_titleLabel->setVisible(!m_title.isEmpty());
    m_descriptionLabel->setVisible(!m_description.isEmpty());
}
