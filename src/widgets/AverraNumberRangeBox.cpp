#include <Averra/AverraNumberRangeBox.h>
#include <Averra/AverraNumberBox.h>
#include <Averra/AverraThemeManager.h>

#include "../core/AverraStyleHelper.h"

#include <QHBoxLayout>
#include <QLabel>

AverraNumberRangeBox::AverraNumberRangeBox(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    initialize();
}

int AverraNumberRangeBox::minimumValue() const
{
    return m_minimumBox->value();
}

void AverraNumberRangeBox::setMinimumValue(int value)
{
    m_minimumBox->setValue(value);
}

int AverraNumberRangeBox::maximumValue() const
{
    return m_maximumBox->value();
}

void AverraNumberRangeBox::setMaximumValue(int value)
{
    m_maximumBox->setValue(value);
}

bool AverraNumberRangeBox::accentFrame() const
{
    return m_accentFrame;
}

void AverraNumberRangeBox::setAccentFrame(bool accentFrame)
{
    if (m_accentFrame == accentFrame) {
        return;
    }

    m_accentFrame = accentFrame;
    m_minimumBox->setAccentFrame(m_accentFrame);
    m_maximumBox->setAccentFrame(m_accentFrame);
    refreshStyle();
    emit accentFrameChanged(m_accentFrame);
}

AverraNumberBox *AverraNumberRangeBox::minimumBox() const
{
    return m_minimumBox;
}

AverraNumberBox *AverraNumberRangeBox::maximumBox() const
{
    return m_maximumBox;
}

void AverraNumberRangeBox::initialize()
{
    m_accentFrame = false;
    m_rootFrame = new QWidget(this);
    m_minimumBox = new AverraNumberBox(m_rootFrame);
    m_maximumBox = new AverraNumberBox(m_rootFrame);
    m_separatorLabel = new QLabel(QStringLiteral("~"), m_rootFrame);
    m_separatorLabel->setObjectName(QStringLiteral("AverraNumberRangeBoxLabel"));

    QHBoxLayout *hostLayout = new QHBoxLayout(this);
    hostLayout->setContentsMargins(0, 0, 0, 0);
    hostLayout->addWidget(m_rootFrame);

    QHBoxLayout *rootLayout = new QHBoxLayout(m_rootFrame);
    rootLayout->setContentsMargins(10, 6, 10, 6);
    rootLayout->setSpacing(8);
    rootLayout->addWidget(m_minimumBox);
    rootLayout->addWidget(m_separatorLabel);
    rootLayout->addWidget(m_maximumBox);

    connect(m_minimumBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &AverraNumberRangeBox::handleValueChanged);
    connect(m_maximumBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &AverraNumberRangeBox::handleValueChanged);
    connect(AverraThemeManager::instance(),
            &AverraThemeManager::paletteChanged,
            this,
            &AverraNumberRangeBox::refreshStyle);

    refreshStyle();
}

void AverraNumberRangeBox::refreshStyle()
{
    const AverraThemePalette palette = AverraThemeManager::instance()->palette();
    m_rootFrame->setObjectName(QStringLiteral("AverraNumberRangeBoxRoot"));
    m_rootFrame->setStyleSheet(AverraStyleHelper::numberRangeBoxFrameStyleSheet(palette, m_accentFrame));
    m_separatorLabel->setStyleSheet(AverraStyleHelper::numberRangeBoxLabelStyleSheet(palette));
}

void AverraNumberRangeBox::handleValueChanged()
{
    emit rangeChanged(m_minimumBox->value(), m_maximumBox->value());
}

