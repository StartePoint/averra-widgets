#include "GalleryWindow.h"

#include <Averra/AverraBadgeButton.h>
#include <Averra/AverraButton.h>
#include <Averra/AverraCard.h>
#include <Averra/AverraCheckBox.h>
#include <Averra/AverraComboBox.h>
#include <Averra/AverraContextMenu.h>
#include <Averra/AverraDataTable.h>
#include <Averra/AverraDialog.h>
#include <Averra/AverraDivider.h>
#include <Averra/AverraDock.h>
#include <Averra/AverraDrawer.h>
#include <Averra/AverraEmptyState.h>
#include <Averra/AverraFieldDefinition.h>
#include <Averra/AverraFilterChipGroup.h>
#include <Averra/AverraFormModel.h>
#include <Averra/AverraFormSection.h>
#include <Averra/AverraIcon.h>
#include <Averra/AverraInfoBar.h>
#include <Averra/AverraInspectorPanel.h>
#include <Averra/AverraDateEdit.h>
#include <Averra/AverraDialogManager.h>
#include <Averra/AverraLineEdit.h>
#include <Averra/AverraLoadingRing.h>
#include <Averra/AverraNavigationPanel.h>
#include <Averra/AverraNumberBox.h>
#include <Averra/AverraNumberRangeBox.h>
#include <Averra/AverraPagination.h>
#include <Averra/AverraProgressBar.h>
#include <Averra/AverraPropertyGrid.h>
#include <Averra/AverraPropertyEditor.h>
#include <Averra/AverraPropertyItem.h>
#include <Averra/AverraRadioButton.h>
#include <Averra/AverraRibbonBar.h>
#include <Averra/AverraSearchBar.h>
#include <Averra/AverraSectionHeader.h>
#include <Averra/AverraSegmentedControl.h>
#include <Averra/AverraSettingsPage.h>
#include <Averra/AverraSkeleton.h>
#include <Averra/AverraSlider.h>
#include <Averra/AverraStatisticCard.h>
#include <Averra/AverraSwitch.h>
#include <Averra/AverraTag.h>
#include <Averra/AverraTabs.h>
#include <Averra/AverraTextArea.h>
#include <Averra/AverraToast.h>
#include <Averra/AverraToastManager.h>
#include <Averra/AverraTreeTable.h>
#include <Averra/AverraTreeView.h>
#include <Averra/AverraToolbar.h>
#include <Averra/AverraThemeManager.h>

#include <QButtonGroup>
#include <QCheckBox>
#include <QFrame>
#include <QHash>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QMap>
#include <QMainWindow>
#include <QItemSelectionModel>
#include <QPushButton>
#include <QSettings>
#include <QSharedPointer>
#include <QStringList>
#include <QTimer>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QStackedWidget>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
struct GalleryCategoryGuide
{
    QString title;
    QStringList keywords;
    QString summary;
};

QLabel *createGuideLabel(const QString &text, bool emphasis = false)
{
    QLabel *label = new QLabel(text);
    label->setWordWrap(true);
    label->setStyleSheet(
        QStringLiteral("color: %1; font-size: %2px; font-weight: %3; line-height: 1.45;")
            .arg(emphasis ? QStringLiteral("#1D2533") : QStringLiteral("#5B6472"),
                 emphasis ? QStringLiteral("14") : QStringLiteral("13"),
                 emphasis ? QStringLiteral("700") : QStringLiteral("500")));
    return label;
}

QWidget *createGuideTags(const QStringList &tags)
{
    QWidget *row = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(row);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    for (int index = 0; index < tags.size(); ++index) {
        AverraTag *tag = new AverraTag(tags.at(index));
        tag->setFilled(index == 0);
        if (index == 0) {
            tag->setTone(AverraTag::Accent);
        }
        layout->addWidget(tag);
    }

    layout->addStretch(1);
    return row;
}

QWidget *createDecisionCard(const QString &title,
                            const QString &subtitle,
                            const QStringList &lines,
                            const QStringList &tags = QStringList())
{
    AverraCard *card = new AverraCard(title);
    card->setSubtitle(subtitle);
    card->setAccentHeader(true);

    if (!tags.isEmpty()) {
        card->contentLayout()->addWidget(createGuideTags(tags));
    }

    for (int index = 0; index < lines.size(); ++index) {
        card->contentLayout()->addWidget(createGuideLabel(QStringLiteral("• %1").arg(lines.at(index))));
    }

    return card;
}

QWidget *createScenarioCard(const QString &title,
                            const QString &subtitle,
                            const QString &whenToUse,
                            const QString &placement,
                            const QString &avoid,
                            QWidget *demoWidget,
                            const QStringList &tags = QStringList())
{
    AverraCard *card = new AverraCard(title);
    card->setSubtitle(subtitle);

    if (!tags.isEmpty()) {
        card->contentLayout()->addWidget(createGuideTags(tags));
    }

    card->contentLayout()->addWidget(createGuideLabel(QStringLiteral("适用场景：%1").arg(whenToUse), true));
    card->contentLayout()->addWidget(createGuideLabel(QStringLiteral("推荐位置：%1").arg(placement)));

    if (!avoid.trimmed().isEmpty()) {
        card->contentLayout()->addWidget(createGuideLabel(QStringLiteral("尽量避免：%1").arg(avoid)));
    }

    if (demoWidget != nullptr) {
        card->contentLayout()->addSpacing(4);
        card->contentLayout()->addWidget(demoWidget);
    }

    return card;
}

QWidget *createVerticalDemoPanel(const QList<QWidget *> &widgets)
{
    QWidget *panel = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    for (int index = 0; index < widgets.size(); ++index) {
        if (widgets.at(index) != nullptr) {
            layout->addWidget(widgets.at(index));
        }
    }

    return panel;
}

QList<GalleryCategoryGuide> galleryCategoryGuides()
{
    QList<GalleryCategoryGuide> guides;

    guides.append({QStringLiteral("选型指南"),
                   QStringList()
                       << QStringLiteral("怎么选")
                       << QStringLiteral("选型")
                       << QStringLiteral("入口")
                       << QStringLiteral("不知道用哪个")
                       << QStringLiteral("guide"),
                   QStringLiteral("还没想清楚页面任务时，先来这里看整体选型规则。")});
    guides.append({QStringLiteral("页面头部与操作"),
                   QStringList()
                       << QStringLiteral("按钮")
                       << QStringLiteral("操作")
                       << QStringLiteral("头部")
                       << QStringLiteral("工具栏")
                       << QStringLiteral("主操作")
                       << QStringLiteral("toolbar"),
                   QStringLiteral("适合页面头部、命令区、主次操作和带计数入口。")});
    guides.append({QStringLiteral("表单录入与搜索"),
                   QStringList()
                       << QStringLiteral("输入")
                       << QStringLiteral("表单")
                       << QStringLiteral("搜索")
                       << QStringLiteral("日期")
                       << QStringLiteral("备注")
                       << QStringLiteral("field"),
                   QStringLiteral("适合结构化字段录入、搜索条件和长文本说明。")});
    guides.append({QStringLiteral("筛选切换与参数"),
                   QStringList()
                       << QStringLiteral("筛选")
                       << QStringLiteral("切换")
                       << QStringLiteral("分页")
                       << QStringLiteral("开关")
                       << QStringLiteral("参数")
                       << QStringLiteral("filter"),
                   QStringLiteral("适合列表筛选、视图切换、偏好开关和连续参数调节。")});
    guides.append({QStringLiteral("数据工作流"),
                   QStringList()
                       << QStringLiteral("表格")
                       << QStringLiteral("树")
                       << QStringLiteral("工作流")
                       << QStringLiteral("详情联动")
                       << QStringLiteral("tree")
                       << QStringLiteral("table"),
                   QStringLiteral("适合决定用 DataTable、TreeView 还是 TreeTable+InspectorPanel。")});
    guides.append({QStringLiteral("反馈提示与状态"),
                   QStringList()
                       << QStringLiteral("反馈")
                       << QStringLiteral("提示")
                       << QStringLiteral("toast")
                       << QStringLiteral("dialog")
                       << QStringLiteral("加载")
                       << QStringLiteral("空状态"),
                   QStringLiteral("适合区分即时提示、非阻塞通知、空状态和阻塞确认。")});
    guides.append({QStringLiteral("页面布局与容器"),
                   QStringList()
                       << QStringLiteral("布局")
                       << QStringLiteral("容器")
                       << QStringLiteral("设置页")
                       << QStringLiteral("检查器")
                       << QStringLiteral("工作台")
                       << QStringLiteral("layout"),
                   QStringLiteral("适合选设置页骨架、右侧检查器和桌面工作台容器。")});

    return guides;
}

QString normalizedGuideText(const QString &text)
{
    return text.trimmed().toLower();
}
}

GalleryWindow::GalleryWindow(QWidget *parent)
{
    if (parent != nullptr) {
        setParent(parent);
    }

    m_oceanButton = nullptr;
    m_forestButton = nullptr;
    m_sunsetButton = nullptr;
    m_orchidButton = nullptr;
    m_categorySearchBar = nullptr;
    m_categoryHintLabel = nullptr;
    m_categoryNavigation = nullptr;
    m_previewStack = nullptr;

    setupWindow();
    populateCategories();

    setWindowTitle(QStringLiteral("Averra Gallery"));
    setWindowIcon(AverraIcon::brandIcon());
    resize(1260, 860);
}

GalleryWindow::~GalleryWindow()
{
}

void GalleryWindow::applyOceanTheme()
{
    AverraThemeManager::instance()->setThemePreset(AverraThemeManager::OceanTheme);
}

void GalleryWindow::applyForestTheme()
{
    AverraThemeManager::instance()->setThemePreset(AverraThemeManager::ForestTheme);
}

void GalleryWindow::applySunsetTheme()
{
    AverraThemeManager::instance()->setThemePreset(AverraThemeManager::SunsetTheme);
}

void GalleryWindow::applyOrchidTheme()
{
    AverraThemeManager::instance()->setThemePreset(AverraThemeManager::OrchidTheme);
}

void GalleryWindow::switchCategory(int index)
{
    if (m_previewStack == nullptr) {
        return;
    }

    if (index < 0 || index >= m_previewStack->count()) {
        return;
    }

    m_previewStack->setCurrentIndex(index);

    const QList<GalleryCategoryGuide> guides = galleryCategoryGuides();
    if (m_categoryHintLabel != nullptr && index >= 0 && index < guides.size()) {
        m_categoryHintLabel->setText(guides.at(index).summary);
    }
}

void GalleryWindow::updateCategoryGuide(const QString &text)
{
    if (m_categoryHintLabel == nullptr) {
        return;
    }

    const QString pattern = normalizedGuideText(text);

    if (pattern.isEmpty()) {
        switchCategory(m_categoryNavigation != nullptr ? m_categoryNavigation->currentIndex() : 0);
        return;
    }

    const QList<GalleryCategoryGuide> guides = galleryCategoryGuides();
    int bestIndex = -1;
    int bestScore = -1;

    for (int index = 0; index < guides.size(); ++index) {
        int score = 0;
        const GalleryCategoryGuide &guide = guides.at(index);

        if (normalizedGuideText(guide.title).contains(pattern)) {
            score += 4;
        }

        for (int keywordIndex = 0; keywordIndex < guide.keywords.size(); ++keywordIndex) {
            const QString keyword = normalizedGuideText(guide.keywords.at(keywordIndex));

            if (keyword.contains(pattern) || pattern.contains(keyword)) {
                score += 3;
            }
        }

        if (score > bestScore) {
            bestScore = score;
            bestIndex = index;
        }
    }

    if (bestIndex >= 0 && bestScore > 0) {
        m_categoryNavigation->setCurrentIndex(bestIndex);
        m_categoryHintLabel->setText(QStringLiteral("推荐：%1。%2")
                                         .arg(guides.at(bestIndex).title, guides.at(bestIndex).summary));
        return;
    }

    m_categoryHintLabel->setText(QStringLiteral("没有直接匹配的分类，建议先看“选型指南”，再按页面任务选择。"));
}

QWidget *GalleryWindow::createScrollableCategoryPage(const QString &title, const QString &subtitle, QLayout *contentLayout)
{
    QWidget *contentWidget = new QWidget;
    QVBoxLayout *contentRootLayout = new QVBoxLayout(contentWidget);
    contentRootLayout->setContentsMargins(24, 24, 24, 24);
    contentRootLayout->setSpacing(16);

    AverraSectionHeader *header = new AverraSectionHeader(title);
    header->setSubtitle(subtitle);
    header->setMetaText(QStringLiteral("Category"));

    QWidget *contentSection = new QWidget;
    contentSection->setLayout(contentLayout);

    contentRootLayout->addWidget(header);
    contentRootLayout->addWidget(contentSection);
    contentRootLayout->addStretch(1);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(contentWidget);

    return scrollArea;
}

QWidget *GalleryWindow::createGuidePage()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    layout->addWidget(
        createDecisionCard(QStringLiteral("先按任务选，再看控件名"),
                           QStringLiteral("如果你还不熟悉 Averra，先按页面任务决定进入哪个分类，再在分类页里看具体控件。"),
                           QStringList()
                               << QStringLiteral("要解决“页面头部、命令区、主次操作”时，先看“页面头部与操作”。")
                               << QStringLiteral("要解决“录入、搜索、日期、长文本”时，先看“表单录入与搜索”。")
                               << QStringLiteral("要解决“筛选、分页、视图切换、策略开关”时，先看“筛选切换与参数”。")
                               << QStringLiteral("要解决“表格、树、详情联动”时，先看“数据工作流”。")
                               << QStringLiteral("要解决“提示、加载、确认、空状态”时，先看“反馈提示与状态”。")
                               << QStringLiteral("要组织“设置页、工作台、右侧详情、主窗口骨架”时，先看“页面布局与容器”。"),
                           QStringList() << QStringLiteral("选型指南") << QStringLiteral("从任务出发")));

    layout->addWidget(
        createDecisionCard(QStringLiteral("控件选型的一条简单规则"),
                           QStringLiteral("同一层级里，尽量让控件承担单一职责，不要为了“看起来高级”而混用。"),
                           QStringList()
                               << QStringLiteral("只有一个主要动作时，用 `AverraButton`；需要页面级命令区时，再上 `AverraToolbar`。")
                               << QStringLiteral("只有简单字段时，用 `AverraPropertyGrid`；需要分组和右侧详情工作流时，再上 `AverraInspectorPanel`。")
                               << QStringLiteral("只有表格就用 `AverraDataTable`；只有层级就用 `AverraTreeView`；既有层级又有多列和右侧联动时，再用 `AverraTreeTable`。")
                               << QStringLiteral("即时提示优先 `AverraInfoBar / AverraToast`；需要用户停下来确认时，才用 `AverraDialog / AverraDrawer`。"),
                           QStringList() << QStringLiteral("别堆控件") << QStringLiteral("先解决任务")));

    return createScrollableCategoryPage(QStringLiteral("选型指南"),
                                        QStringLiteral("这是一个按业务场景浏览组件的 Gallery，而不是单纯的控件陈列室。"),
                                        layout);
}

QWidget *GalleryWindow::createActionsPage()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    AverraButton *primaryButton = new AverraButton(QStringLiteral("主操作按钮"));
    primaryButton->setAccent(true);

    AverraButton *secondaryButton = new AverraButton(QStringLiteral("次要操作按钮"));

    AverraBadgeButton *badgeButton = new AverraBadgeButton(QStringLiteral("通知中心"));
    badgeButton->setBadgeText(QStringLiteral("12"));
    badgeButton->setAccent(true);

    AverraToolbar *toolbar = new AverraToolbar;
    toolbar->setTitle(QStringLiteral("组件管理"));
    toolbar->setSubtitle(QStringLiteral("浏览、筛选并管理当前工作区中的组件"));
    toolbar->setAccent(true);

    AverraTag *toolbarTag = new AverraTag(QStringLiteral("Workspace"));
    toolbarTag->setFilled(false);
    toolbar->addLeadingWidget(toolbarTag);

    AverraSearchBar *toolbarSearchBar = new AverraSearchBar;
    toolbarSearchBar->setPlaceholderText(QStringLiteral("搜索组件"));
    toolbarSearchBar->setButtonText(QStringLiteral("查找"));
    toolbarSearchBar->setMinimumWidth(240);

    AverraBadgeButton *toolbarBadgeButton = new AverraBadgeButton(QStringLiteral("消息"));
    toolbarBadgeButton->setBadgeText(QStringLiteral("3"));

    AverraButton *toolbarActionButton = new AverraButton(QStringLiteral("新建组件"));
    toolbarActionButton->setAccent(true);

    toolbar->addTrailingWidget(toolbarSearchBar);
    toolbar->addTrailingWidget(toolbarBadgeButton);
    toolbar->addTrailingWidget(toolbarActionButton);

    QWidget *buttonDemo = new QWidget;
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonDemo);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(primaryButton);
    buttonLayout->addWidget(secondaryButton);
    buttonLayout->addStretch(1);

    QWidget *badgeDemo = new QWidget;
    QHBoxLayout *badgeLayout = new QHBoxLayout(badgeDemo);
    badgeLayout->setContentsMargins(0, 0, 0, 0);
    badgeLayout->setSpacing(10);
    badgeLayout->addWidget(badgeButton);
    badgeLayout->addStretch(1);

    layout->addWidget(
        createDecisionCard(QStringLiteral("什么时候看这一页"),
                           QStringLiteral("当你在做页面头部、卡片主操作、批量命令区或带计数入口时，先从这一页选。"),
                           QStringList()
                               << QStringLiteral("单个动作入口优先 `AverraButton`。")
                               << QStringLiteral("需要显示未读、数量或提醒时用 `AverraBadgeButton`。")
                               << QStringLiteral("页面标题、筛选和批量命令要放一起时用 `AverraToolbar`。"),
                           QStringList() << QStringLiteral("页面头部") << QStringLiteral("操作入口")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("主次操作按钮"),
                           QStringLiteral("适合卡片、对话框页脚、空状态入口和工具页顶部的明确动作。"),
                           QStringLiteral("页面里只有 1 个最重要动作，或者需要清晰区分主次操作。"),
                           QStringLiteral("卡片底部、弹窗页脚、列表页右上角。"),
                           QStringLiteral("不要在同一层级里放多个强调按钮竞争注意力。"),
                           buttonDemo,
                           QStringList() << QStringLiteral("AverraButton") << QStringLiteral("主次动作")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("带数量或提醒的入口"),
                           QStringLiteral("适合消息中心、任务中心、待办入口和有数量反馈的快捷入口。"),
                           QStringLiteral("按钮本身要承担“进入页面 + 告知数量”的双重职责。"),
                           QStringLiteral("页面头部右侧、导航区、工具条。"),
                           QStringLiteral("如果只是状态展示，不要拿它替代纯标签或统计卡片。"),
                           badgeDemo,
                           QStringList() << QStringLiteral("AverraBadgeButton") << QStringLiteral("计数入口")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("页面命令区与工具栏"),
                           QStringLiteral("适合列表页、资源页和管理后台，把标题、筛选、快捷操作放在同一条带里。"),
                           QStringLiteral("需要一眼看到“我在哪个页面 + 我现在能做什么”。"),
                           QStringLiteral("页面顶部、列表页头部、资源工作区顶栏。"),
                           QStringLiteral("不要把复杂表单控件堆进工具栏，工具栏更适合轻量筛选和快捷命令。"),
                           toolbar,
                           QStringList() << QStringLiteral("AverraToolbar") << QStringLiteral("页面顶栏")));

    return createScrollableCategoryPage(QStringLiteral("页面头部与操作"),
                                        QStringLiteral("先决定是单个动作、带计数入口，还是整条页面命令区。"),
                                        layout);
}

QWidget *GalleryWindow::createInputPage()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    AverraLineEdit *defaultLineEdit = new AverraLineEdit;
    defaultLineEdit->setPlaceholderText(QStringLiteral("请输入项目名称"));

    AverraLineEdit *accentLineEdit = new AverraLineEdit;
    accentLineEdit->setAccentFrame(true);
    accentLineEdit->setPlaceholderText(QStringLiteral("强调输入框，可用于搜索或主筛选条件"));

    AverraComboBox *defaultComboBox = new AverraComboBox;
    defaultComboBox->addItems(QStringList() << QStringLiteral("默认主题") << QStringLiteral("海洋蓝") << QStringLiteral("森林绿"));

    AverraComboBox *accentComboBox = new AverraComboBox;
    accentComboBox->setAccentFrame(true);
    accentComboBox->addItems(QStringList() << QStringLiteral("组件") << QStringLiteral("模板") << QStringLiteral("示例"));

    AverraNumberBox *numberBox = new AverraNumberBox;
    numberBox->setRange(0, 999);
    numberBox->setValue(12);

    AverraNumberRangeBox *numberRangeBox = new AverraNumberRangeBox;
    numberRangeBox->setMinimumValue(10);
    numberRangeBox->setMaximumValue(80);

    AverraDateEdit *dateEdit = new AverraDateEdit;
    dateEdit->setAccentFrame(true);

    AverraSearchBar *defaultSearchBar = new AverraSearchBar;
    defaultSearchBar->setPlaceholderText(QStringLiteral("搜索组件、主题或示例"));
    defaultSearchBar->setButtonText(QStringLiteral("搜索"));

    AverraSearchBar *accentSearchBar = new AverraSearchBar;
    accentSearchBar->setAccentFrame(true);
    accentSearchBar->setPlaceholderText(QStringLiteral("搜索更新日志、版本记录"));
    accentSearchBar->setButtonText(QStringLiteral("查找"));

    AverraTextArea *defaultTextArea = new AverraTextArea;
    defaultTextArea->setPlaceholderText(QStringLiteral("请输入发布说明、更新摘要或补充备注..."));
    defaultTextArea->setPlainText(QStringLiteral("Averra Widgets 0.2\n- 新增输入与状态控件\n- 优化基础主题变量\n- 补充页面编排组件"));

    AverraTextArea *accentTextArea = new AverraTextArea;
    accentTextArea->setAccentFrame(true);
    accentTextArea->setPlaceholderText(QStringLiteral("强调输入区域，可用于重点说明或审核意见"));

    layout->addWidget(
        createDecisionCard(QStringLiteral("录入页不要一股脑混用所有输入控件"),
                           QStringLiteral("先判断用户是在填字段、做搜索，还是在写说明。不同任务对应的输入控件不同。"),
                           QStringList()
                               << QStringLiteral("标识、名称、单值参数优先 `AverraLineEdit / AverraComboBox / AverraNumberBox`。")
                               << QStringLiteral("范围参数和日期字段优先 `AverraNumberRangeBox / AverraDateEdit`。")
                               << QStringLiteral("关键词检索优先 `AverraSearchBar`，长说明和审核意见优先 `AverraTextArea`。"),
                           QStringList() << QStringLiteral("表单录入") << QStringLiteral("搜索输入")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("基础字段录入"),
                           QStringLiteral("适合设置页、创建页和弹窗里的结构化字段。"),
                           QStringLiteral("用户在填写名称、主题、数值等明确字段。"),
                           QStringLiteral("设置页、创建向导、属性弹窗。"),
                           QStringLiteral("不要用多行文本或搜索栏去代替结构化字段输入。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << defaultLineEdit
                                                   << defaultComboBox
                                                   << numberBox),
                           QStringList() << QStringLiteral("AverraLineEdit")
                                         << QStringLiteral("AverraComboBox")
                                         << QStringLiteral("AverraNumberBox")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("范围、日期与筛选条件"),
                           QStringLiteral("适合任务查询、资源过滤和需要明确约束范围的参数输入。"),
                           QStringLiteral("用户要表达“开始到结束”“某一天”或主筛选条件。"),
                           QStringLiteral("筛选面板、报表页、查询条件区。"),
                           QStringLiteral("不要把它们放进密集工具栏里挤占操作空间。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << accentLineEdit
                                                   << numberRangeBox
                                                   << dateEdit),
                           QStringList() << QStringLiteral("AverraLineEdit")
                                         << QStringLiteral("AverraNumberRangeBox")
                                         << QStringLiteral("AverraDateEdit")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("搜索与长文本说明"),
                           QStringLiteral("适合资源搜索、日志检索、发布说明和审核意见。"),
                           QStringLiteral("一个控件既要承接关键词输入，又要保持页面语义清晰。"),
                           QStringLiteral("列表页顶部、抽屉面板、备注区。"),
                           QStringLiteral("不要用 `SearchBar` 承担正式表单字段，也不要让 `TextArea` 伪装成搜索框。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << defaultSearchBar
                                                   << accentSearchBar
                                                   << defaultTextArea
                                                   << accentTextArea
                                                   << accentComboBox),
                           QStringList() << QStringLiteral("AverraSearchBar")
                                         << QStringLiteral("AverraTextArea")));

    return createScrollableCategoryPage(QStringLiteral("表单录入与搜索"),
                                        QStringLiteral("先区分“结构化字段”“筛选条件”和“长文本说明”，再选输入控件。"),
                                        layout);
}

QWidget *GalleryWindow::createSelectionPage()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    AverraFilterChipGroup *filterChipGroup = new AverraFilterChipGroup;
    filterChipGroup->addChip(QStringLiteral("全部"));
    filterChipGroup->addChip(QStringLiteral("按钮"));
    filterChipGroup->addChip(QStringLiteral("表单"));
    filterChipGroup->addChip(QStringLiteral("数据展示"));
    filterChipGroup->setSelectedIndices(QList<int>() << 0);

    AverraPagination *pagination = new AverraPagination;
    pagination->setPageCount(12);
    pagination->setCurrentPage(3);

    QWidget *radioRow = new QWidget;
    QHBoxLayout *radioLayout = new QHBoxLayout(radioRow);
    radioLayout->setContentsMargins(0, 0, 0, 0);
    radioLayout->setSpacing(18);

    QButtonGroup *radioGroup = new QButtonGroup(radioRow);
    radioGroup->setExclusive(true);

    AverraRadioButton *gridRadio = new AverraRadioButton(QStringLiteral("网格视图"));
    AverraRadioButton *listRadio = new AverraRadioButton(QStringLiteral("列表视图"));
    listRadio->setChecked(true);
    radioGroup->addButton(gridRadio);
    radioGroup->addButton(listRadio);

    radioLayout->addWidget(gridRadio);
    radioLayout->addWidget(listRadio);
    radioLayout->addStretch(1);

    AverraSegmentedControl *segmentedControl = new AverraSegmentedControl;
    segmentedControl->addSegment(QStringLiteral("全部"));
    segmentedControl->addSegment(QStringLiteral("进行中"));
    segmentedControl->addSegment(QStringLiteral("已完成"));
    segmentedControl->setCurrentIndex(1);

    QWidget *switchRow = new QWidget;
    QHBoxLayout *switchLayout = new QHBoxLayout(switchRow);
    switchLayout->setContentsMargins(0, 0, 0, 0);
    switchLayout->setSpacing(12);

    AverraSwitch *primarySwitch = new AverraSwitch;
    primarySwitch->setChecked(true);
    QLabel *primarySwitchLabel = new QLabel(QStringLiteral("自动更新"));
    primarySwitchLabel->setStyleSheet(QStringLiteral("color: #1D2533; font-size: 13px; font-weight: 600;"));

    AverraSwitch *secondarySwitch = new AverraSwitch;
    QLabel *secondarySwitchLabel = new QLabel(QStringLiteral("实验特性"));
    secondarySwitchLabel->setStyleSheet(QStringLiteral("color: #1D2533; font-size: 13px; font-weight: 600;"));

    switchLayout->addWidget(primarySwitch);
    switchLayout->addWidget(primarySwitchLabel);
    switchLayout->addSpacing(16);
    switchLayout->addWidget(secondarySwitch);
    switchLayout->addWidget(secondarySwitchLabel);
    switchLayout->addStretch(1);

    QWidget *checkBoxRow = new QWidget;
    QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxRow);
    checkBoxLayout->setContentsMargins(0, 0, 0, 0);
    checkBoxLayout->setSpacing(18);

    AverraCheckBox *primaryCheckBox = new AverraCheckBox(QStringLiteral("记住窗口布局"));
    primaryCheckBox->setChecked(true);
    AverraCheckBox *secondaryCheckBox = new AverraCheckBox(QStringLiteral("启动时检查更新"));

    checkBoxLayout->addWidget(primaryCheckBox);
    checkBoxLayout->addWidget(secondaryCheckBox);
    checkBoxLayout->addStretch(1);

    AverraSlider *primarySlider = new AverraSlider;
    primarySlider->setValue(42);
    QLabel *sliderValueLabel = new QLabel(QStringLiteral("当前值：42"));
    sliderValueLabel->setStyleSheet(QStringLiteral("color: #5B6472; font-size: 13px;"));

    connect(primarySlider,
            &AverraSlider::valueChanged,
            this,
            [sliderValueLabel](int value) {
                sliderValueLabel->setText(QStringLiteral("当前值：%1").arg(value));
            });

    layout->addWidget(
        createDecisionCard(QStringLiteral("切换、勾选和筛选不要混成一类交互"),
                           QStringLiteral("这页的重点是帮助用户分清：用户是在切换视图、勾选策略，还是在缩小数据范围。"),
                           QStringList()
                               << QStringLiteral("页签式状态切换优先 `AverraSegmentedControl / AverraRadioButton`。")
                               << QStringLiteral("布尔策略开关优先 `AverraSwitch / AverraCheckBox`。")
                               << QStringLiteral("数据范围缩小和分页导航优先 `AverraFilterChipGroup / AverraPagination / AverraSlider`。"),
                           QStringList() << QStringLiteral("筛选切换") << QStringLiteral("参数选择")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("列表筛选与分页"),
                           QStringLiteral("适合资源列表、消息中心和报表页，把筛选条件与分页导航清晰拆开。"),
                           QStringLiteral("你需要让用户快速缩小结果集，并知道自己处于第几页。"),
                           QStringLiteral("列表页顶部和列表底部导航区。"),
                           QStringLiteral("不要用分页控件承载主筛选，也不要让 Chip 组承担视图切换。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << filterChipGroup
                                                   << pagination),
                           QStringList() << QStringLiteral("AverraFilterChipGroup")
                                         << QStringLiteral("AverraPagination")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("视图模式切换"),
                           QStringLiteral("适合在“列表 / 网格”“全部 / 进行中 / 已完成”这类互斥状态之间切换。"),
                           QStringLiteral("多个选项互斥，且用户需要马上理解当前模式。"),
                           QStringLiteral("页面头部、局部工具条、面板顶部。"),
                           QStringLiteral("如果允许多选，不要用单选按钮或 SegmentedControl。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << radioRow
                                                   << segmentedControl),
                           QStringList() << QStringLiteral("AverraRadioButton")
                                         << QStringLiteral("AverraSegmentedControl")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("策略开关与参数调节"),
                           QStringLiteral("适合偏好设置、实验特性开关和连续参数调节。"),
                           QStringLiteral("用户在表达“开 / 关”或调一个连续数值。"),
                           QStringLiteral("设置页、侧栏、实验配置面板。"),
                           QStringLiteral("不要让 `Switch` 触发不可逆行为，确认类行为应使用按钮或对话框。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << switchRow
                                                   << checkBoxRow
                                                   << primarySlider
                                                   << sliderValueLabel),
                           QStringList() << QStringLiteral("AverraSwitch")
                                         << QStringLiteral("AverraCheckBox")
                                         << QStringLiteral("AverraSlider")));

    return createScrollableCategoryPage(QStringLiteral("筛选切换与参数"),
                                        QStringLiteral("把“互斥切换”“布尔策略”“缩小结果集”三件事清楚拆开。"),
                                        layout);
}

QWidget *GalleryWindow::createDataPage()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    layout->addWidget(
        createDecisionCard(QStringLiteral("数据展示先判断层级和联动复杂度"),
                           QStringLiteral("用户看到数据页时，通常不是在挑一个控件，而是在决定要不要做树、表和详情联动。"),
                           QStringList()
                               << QStringLiteral("只有表格列表时，用 `AverraDataTable`。")
                               << QStringLiteral("只有层级结构浏览时，用 `AverraTreeView`。")
                               << QStringLiteral("需要“左侧树 + 中间概览 + 右侧检查器”时，用 `AverraTreeTable + AverraInspectorPanel`。"),
                           QStringList() << QStringLiteral("数据工作流") << QStringLiteral("先选结构")));

    AverraSectionHeader *dataHeader = new AverraSectionHeader(QStringLiteral("数据展示"));
    dataHeader->setSubtitle(QStringLiteral("树表、概览与检查器联动，演示一条真正可落业务的桌面工作流。"));
    dataHeader->setMetaText(QStringLiteral("Workflow"));

    AverraDataTable *quickTable = new AverraDataTable(QStringLiteral("纯表格列表"));
    quickTable->setSubtitle(QStringLiteral("没有层级关系、只需要排序筛选和批量操作时。"));
    quickTable->setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型") << QStringLiteral("状态"));
    quickTable->addRow(QStringList() << QStringLiteral("节点 A") << QStringLiteral("服务") << QStringLiteral("Live"));
    quickTable->addRow(QStringList() << QStringLiteral("节点 B") << QStringLiteral("任务") << QStringLiteral("Draft"));

    AverraTreeView *quickTreeView = new AverraTreeView(QStringLiteral("纯层级浏览"));
    quickTreeView->setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型"));
    QStandardItem *quickRoot = quickTreeView->addTopLevelItem(QStringList() << QStringLiteral("Workspace") << QStringLiteral("根节点"));
    quickTreeView->addChildItem(quickRoot, QStringList() << QStringLiteral("Scenes") << QStringLiteral("目录"));
    quickTreeView->addChildItem(quickRoot, QStringList() << QStringLiteral("Textures") << QStringLiteral("目录"));

    QWidget *quickChoiceRow = new QWidget;
    QHBoxLayout *quickChoiceLayout = new QHBoxLayout(quickChoiceRow);
    quickChoiceLayout->setContentsMargins(0, 0, 0, 0);
    quickChoiceLayout->setSpacing(12);
    quickChoiceLayout->addWidget(
        createScenarioCard(QStringLiteral("纯表格列表页"),
                           QStringLiteral("适合工单页、用户列表、发布记录和没有层级结构的资源列表。"),
                           QStringLiteral("关注的是列排序、批量操作和状态查看。"),
                           QStringLiteral("页面主体列表区。"),
                           QStringLiteral("如果用户还要展开目录层级，就不要继续硬塞 DataTable。"),
                           quickTable,
                           QStringList() << QStringLiteral("AverraDataTable")));
    quickChoiceLayout->addWidget(
        createScenarioCard(QStringLiteral("纯层级浏览页"),
                           QStringLiteral("适合项目树、资源导航树和对象层级总览。"),
                           QStringLiteral("主要诉求是看树结构，而不是同时比较多列状态。"),
                           QStringLiteral("左侧导航区或资源面板。"),
                           QStringLiteral("如果还要展示状态列和驱动右侧详情，应该升级到 TreeTable。"),
                           quickTreeView,
                           QStringList() << QStringLiteral("AverraTreeView")));

    QWidget *workflowWidget = new QWidget;
    QHBoxLayout *workflowLayout = new QHBoxLayout(workflowWidget);
    workflowLayout->setContentsMargins(0, 0, 0, 0);
    workflowLayout->setSpacing(14);

    AverraTreeTable *treeTable = new AverraTreeTable(QStringLiteral("资源树表"));
    treeTable->setSubtitle(QStringLiteral("左侧输出当前选中节点的 item path、对象 ID 和当前行数据。"));
    treeTable->setHeaders(QStringList() << QStringLiteral("名称") << QStringLiteral("类型") << QStringLiteral("状态"));
    treeTable->setActionText(QStringLiteral("定位"));
    treeTable->setColumnConfigVisible(true);
    treeTable->setSettingsKey(QStringLiteral("gallery/workflow/treeTable"));
    treeTable->setMinimumWidth(320);

    AverraCard *overviewCard = new AverraCard(QStringLiteral("当前节点概览"));
    overviewCard->setSubtitle(QStringLiteral("中间工作区实时展示路径、摘要、脏状态、校验结果和提交结果。"));

    QLabel *overviewPathLabel = new QLabel(QStringLiteral("路径：等待选择节点"));
    QLabel *overviewObjectIdLabel = new QLabel(QStringLiteral("对象 ID：-"));
    QLabel *overviewSummaryLabel = new QLabel(QStringLiteral("概览：左侧选择节点后，右侧检查器会自动装载对应字段模型。"));
    QLabel *overviewDirtyLabel = new QLabel(QStringLiteral("表单状态：未载入"));
    QLabel *overviewValidationLabel = new QLabel(QStringLiteral("校验状态：未开始"));
    QLabel *overviewSubmitLabel = new QLabel(QStringLiteral("提交状态：Idle"));

    overviewPathLabel->setWordWrap(true);
    overviewObjectIdLabel->setWordWrap(true);
    overviewSummaryLabel->setWordWrap(true);
    overviewDirtyLabel->setWordWrap(true);
    overviewValidationLabel->setWordWrap(true);
    overviewSubmitLabel->setWordWrap(true);

    overviewCard->contentLayout()->addWidget(overviewPathLabel);
    overviewCard->contentLayout()->addWidget(overviewObjectIdLabel);
    overviewCard->contentLayout()->addWidget(overviewSummaryLabel);
    overviewCard->contentLayout()->addWidget(overviewDirtyLabel);
    overviewCard->contentLayout()->addWidget(overviewValidationLabel);
    overviewCard->contentLayout()->addWidget(overviewSubmitLabel);

    QWidget *statisticRow = new QWidget;
    QHBoxLayout *statisticLayout = new QHBoxLayout(statisticRow);
    statisticLayout->setContentsMargins(0, 0, 0, 0);
    statisticLayout->setSpacing(12);

    AverraStatisticCard *selectedCard = new AverraStatisticCard(QStringLiteral("当前对象"));
    selectedCard->setValue(QStringLiteral("未选择"));
    selectedCard->setHelperText(QStringLiteral("左侧选中后自动切换"));

    AverraStatisticCard *themeCard = new AverraStatisticCard(QStringLiteral("主题 / 重试"));
    themeCard->setValue(QStringLiteral("-"));
    themeCard->setHelperText(QStringLiteral("来自字段模型"));

    AverraStatisticCard *submitCard = new AverraStatisticCard(QStringLiteral("提交流程"));
    submitCard->setValue(QStringLiteral("Idle"));
    submitCard->setHelperText(QStringLiteral("支持成功与失败回滚反馈"));

    statisticLayout->addWidget(selectedCard);
    statisticLayout->addWidget(themeCard);
    statisticLayout->addWidget(submitCard);

    QWidget *centerPane = new QWidget;
    QVBoxLayout *centerLayout = new QVBoxLayout(centerPane);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(12);
    centerLayout->addWidget(overviewCard);
    centerLayout->addWidget(statisticRow);
    centerLayout->addStretch(1);

    AverraInspectorPanel *inspectorPanel = new AverraInspectorPanel;
    inspectorPanel->setTitle(QStringLiteral("对象检查器"));
    inspectorPanel->setSubtitle(QStringLiteral("右侧直接消费统一字段模型，完成 编辑 -> 校验 -> 提交 -> 成功/失败反馈。"));
    inspectorPanel->setStatusText(QStringLiteral("Waiting"));
    inspectorPanel->setSubmitButtonText(QStringLiteral("提交更改"));
    inspectorPanel->setMinimumWidth(420);

    AverraButton *rollbackButton = new AverraButton(QStringLiteral("回滚"));
    inspectorPanel->addActionWidget(rollbackButton);

    AverraFormModel *formModel = new AverraFormModel(inspectorPanel);
    QList<AverraFieldDefinition> fields;

    AverraFieldDefinition nodeNameField = AverraFieldDefinition::createText(QStringLiteral("name"),
                                                                            QStringLiteral("节点名称"),
                                                                            QString(),
                                                                            QStringLiteral("用于树表、概览和提交结果展示。"));
    nodeNameField.setGroup(QStringLiteral("identity"),
                           QStringLiteral("基础信息"),
                           QStringLiteral("对象标识、摘要和接入信息。"));
    nodeNameField.setPlaceholderText(QStringLiteral("请输入节点名称"));
    nodeNameField.addValidationRule(AverraValidationRule::required());
    fields.append(nodeNameField);

    AverraFieldDefinition nodeIdField = AverraFieldDefinition::createReadOnlyText(QStringLiteral("nodeId"),
                                                                                  QStringLiteral("对象 ID"),
                                                                                  QString(),
                                                                                  QStringLiteral("由树表选中上下文输出，作为提交和回滚的稳定标识。"));
    nodeIdField.setGroup(QStringLiteral("identity"),
                         QStringLiteral("基础信息"),
                         QStringLiteral("对象标识、摘要和接入信息。"));
    fields.append(nodeIdField);

    AverraFieldDefinition endpointField = AverraFieldDefinition::createText(QStringLiteral("endpoint"),
                                                                            QStringLiteral("接入路径"),
                                                                            QString(),
                                                                            QStringLiteral("用于模拟业务系统里的接口路径。"));
    endpointField.setGroup(QStringLiteral("identity"),
                           QStringLiteral("基础信息"),
                           QStringLiteral("对象标识、摘要和接入信息。"));
    endpointField.setPlaceholderText(QStringLiteral("/workspace/scene-indexer"));
    endpointField.addValidationRule(AverraValidationRule::required());
    endpointField.addValidationRule(AverraValidationRule::regularExpression(QStringLiteral("^/[A-Za-z0-9/_-]+$"),
                                                                            QStringLiteral("接入路径必须以 / 开头，且只能包含字母、数字、下划线或短横线。")));
    fields.append(endpointField);

    QList<AverraFieldOption> themeOptions;
    themeOptions.append(AverraFieldOption(QStringLiteral("默认主题"), QStringLiteral("default")));
    themeOptions.append(AverraFieldOption(QStringLiteral("海洋蓝"), QStringLiteral("ocean")));
    themeOptions.append(AverraFieldOption(QStringLiteral("森林绿"), QStringLiteral("forest")));

    AverraFieldDefinition themeField = AverraFieldDefinition::createEnum(QStringLiteral("theme"),
                                                                         QStringLiteral("显示主题"),
                                                                         themeOptions,
                                                                         QStringLiteral("ocean"),
                                                                         QStringLiteral("会同步到概览区统计卡片。"));
    themeField.setGroup(QStringLiteral("display"),
                        QStringLiteral("行为与展示"),
                        QStringLiteral("控制主题、同步行为和提交流程。"));
    fields.append(themeField);

    AverraFieldDefinition autoSyncField = AverraFieldDefinition::createBoolean(QStringLiteral("autoSync"),
                                                                               QStringLiteral("自动同步"),
                                                                               true,
                                                                               QStringLiteral("开启后，成功提交会把左侧状态刷新为 Synced。"));
    autoSyncField.setGroup(QStringLiteral("display"),
                           QStringLiteral("行为与展示"),
                           QStringLiteral("控制主题、同步行为和提交流程。"));
    fields.append(autoSyncField);

    AverraFieldDefinition retryField = AverraFieldDefinition::createInteger(QStringLiteral("retryCount"),
                                                                            QStringLiteral("重试次数"),
                                                                            0,
                                                                            10,
                                                                            3,
                                                                            QStringLiteral("用于演示整数输入、校验和持久化。"));
    retryField.setGroup(QStringLiteral("display"),
                        QStringLiteral("行为与展示"),
                        QStringLiteral("控制主题、同步行为和提交流程。"));
    retryField.addValidationRule(AverraValidationRule::maximum(8,
                                                               QStringLiteral("演示环境中重试次数不能超过 8。")));
    fields.append(retryField);

    AverraFieldDefinition notesField = AverraFieldDefinition::createMultilineText(QStringLiteral("notes"),
                                                                                  QStringLiteral("发布备注"),
                                                                                  QString(),
                                                                                  QStringLiteral("用于演示多行备注、脏状态和 QSettings 持久化。"));
    notesField.setGroup(QStringLiteral("notes"),
                        QStringLiteral("提交说明"),
                        QStringLiteral("这些内容会按对象单独记住。"));
    notesField.setPlaceholderText(QStringLiteral("填写当前节点的发布说明或运维备注"));
    fields.append(notesField);

    formModel->setFields(fields);
    inspectorPanel->setFormModel(formModel);

    auto objectStore = QSharedPointer<QMap<QString, QVariantMap>>::create();
    auto itemStore = QSharedPointer<QHash<QString, QStandardItem *>>::create();

    QVariantMap workspaceValues;
    workspaceValues.insert(QStringLiteral("name"), QStringLiteral("Workspace"));
    workspaceValues.insert(QStringLiteral("nodeId"), QStringLiteral("workspace-root"));
    workspaceValues.insert(QStringLiteral("endpoint"), QStringLiteral("/workspace"));
    workspaceValues.insert(QStringLiteral("theme"), QStringLiteral("default"));
    workspaceValues.insert(QStringLiteral("autoSync"), true);
    workspaceValues.insert(QStringLiteral("retryCount"), 2);
    workspaceValues.insert(QStringLiteral("notes"), QStringLiteral("工作区根节点，负责组织资源树与发布流程。"));
    objectStore->insert(QStringLiteral("workspace-root"), workspaceValues);

    QVariantMap sceneValues;
    sceneValues.insert(QStringLiteral("name"), QStringLiteral("Scene Indexer"));
    sceneValues.insert(QStringLiteral("nodeId"), QStringLiteral("scene-indexer"));
    sceneValues.insert(QStringLiteral("endpoint"), QStringLiteral("/workspace/scene-indexer"));
    sceneValues.insert(QStringLiteral("theme"), QStringLiteral("ocean"));
    sceneValues.insert(QStringLiteral("autoSync"), true);
    sceneValues.insert(QStringLiteral("retryCount"), 4);
    sceneValues.insert(QStringLiteral("notes"), QStringLiteral("将名称改成包含 Rollback 可演示提交失败与自动回滚。"));
    objectStore->insert(QStringLiteral("scene-indexer"), sceneValues);

    QVariantMap textureValues;
    textureValues.insert(QStringLiteral("name"), QStringLiteral("Texture Cache"));
    textureValues.insert(QStringLiteral("nodeId"), QStringLiteral("texture-cache"));
    textureValues.insert(QStringLiteral("endpoint"), QStringLiteral("/workspace/texture-cache"));
    textureValues.insert(QStringLiteral("theme"), QStringLiteral("forest"));
    textureValues.insert(QStringLiteral("autoSync"), false);
    textureValues.insert(QStringLiteral("retryCount"), 1);
    textureValues.insert(QStringLiteral("notes"), QStringLiteral("缓存纹理和缩略图，适合体验开关字段与保存状态。"));
    objectStore->insert(QStringLiteral("texture-cache"), textureValues);

    QVariantMap publishValues;
    publishValues.insert(QStringLiteral("name"), QStringLiteral("Publish Queue"));
    publishValues.insert(QStringLiteral("nodeId"), QStringLiteral("publish-queue"));
    publishValues.insert(QStringLiteral("endpoint"), QStringLiteral("/workspace/publish-queue"));
    publishValues.insert(QStringLiteral("theme"), QStringLiteral("default"));
    publishValues.insert(QStringLiteral("autoSync"), true);
    publishValues.insert(QStringLiteral("retryCount"), 3);
    publishValues.insert(QStringLiteral("notes"), QStringLiteral("用于演示树表联动、字段模型驱动和 QSettings 恢复。"));
    objectStore->insert(QStringLiteral("publish-queue"), publishValues);

    QVariantMap workspaceContext;
    workspaceContext.insert(QStringLiteral("summary"), QStringLiteral("工作区根节点，聚合所有子节点。"));
    workspaceContext.insert(QStringLiteral("stateDescription"), QStringLiteral("根节点始终在线，适合演示整页概览。"));

    QVariantMap sceneContext;
    sceneContext.insert(QStringLiteral("summary"), QStringLiteral("场景索引服务，负责资源入库和树表同步。"));
    sceneContext.insert(QStringLiteral("stateDescription"), QStringLiteral("提交成功会直接刷新左侧名称和状态列。"));

    QVariantMap textureContext;
    textureContext.insert(QStringLiteral("summary"), QStringLiteral("纹理缓存节点，当前处于延迟同步模式。"));
    textureContext.insert(QStringLiteral("stateDescription"), QStringLiteral("适合体验开关、枚举和备注字段。"));

    QVariantMap publishContext;
    publishContext.insert(QStringLiteral("summary"), QStringLiteral("发布队列节点，适合演示提交链路。"));
    publishContext.insert(QStringLiteral("stateDescription"), QStringLiteral("把名称改成包含 Rollback 可触发失败并自动回滚。"));

    QStandardItem *workspaceRoot = treeTable->addTopLevelItem(QStringList()
                                                              << QStringLiteral("Workspace")
                                                              << QStringLiteral("工作区")
                                                              << QStringLiteral("Live"),
                                                              QStringLiteral("workspace-root"),
                                                              workspaceContext);
    itemStore->insert(QStringLiteral("workspace-root"), workspaceRoot);

    QStandardItem *sceneItem = treeTable->addChildItem(workspaceRoot,
                                                       QStringList()
                                                       << QStringLiteral("Scene Indexer")
                                                       << QStringLiteral("服务")
                                                       << QStringLiteral("Synced"),
                                                       QStringLiteral("scene-indexer"),
                                                       sceneContext);
    itemStore->insert(QStringLiteral("scene-indexer"), sceneItem);

    QStandardItem *textureItem = treeTable->addChildItem(workspaceRoot,
                                                         QStringList()
                                                         << QStringLiteral("Texture Cache")
                                                         << QStringLiteral("缓存")
                                                         << QStringLiteral("Draft"),
                                                         QStringLiteral("texture-cache"),
                                                         textureContext);
    itemStore->insert(QStringLiteral("texture-cache"), textureItem);

    QStandardItem *publishItem = treeTable->addChildItem(workspaceRoot,
                                                         QStringList()
                                                         << QStringLiteral("Publish Queue")
                                                         << QStringLiteral("队列")
                                                         << QStringLiteral("Ready"),
                                                         QStringLiteral("publish-queue"),
                                                         publishContext);
    itemStore->insert(QStringLiteral("publish-queue"), publishItem);

    treeTable->expandAllItems();

    auto refreshOverview = [treeTable,
                            formModel,
                            overviewPathLabel,
                            overviewObjectIdLabel,
                            overviewSummaryLabel,
                            overviewDirtyLabel,
                            overviewValidationLabel,
                            overviewSubmitLabel,
                            selectedCard,
                            themeCard,
                            submitCard,
                            inspectorPanel]() {
        const QVariantMap rowData = treeTable->currentRowData();
        const QVariantMap values = formModel->values();
        const QVariantMap contextData = treeTable->currentContextData();
        const AverraValidationResult validationResult = formModel->validationResult();

        overviewPathLabel->setText(QStringLiteral("路径：%1").arg(treeTable->currentItemPath().isEmpty()
                                                                     ? QStringLiteral("等待选择节点")
                                                                     : treeTable->currentItemPath()));
        overviewObjectIdLabel->setText(QStringLiteral("对象 ID：%1").arg(formModel->objectId().isEmpty()
                                                                            ? QStringLiteral("-")
                                                                            : formModel->objectId()));
        overviewSummaryLabel->setText(QStringLiteral("概览：%1").arg(contextData.value(QStringLiteral("summary")).toString().isEmpty()
                                                                       ? QStringLiteral("当前对象暂无额外摘要。")
                                                                       : contextData.value(QStringLiteral("summary")).toString()));
        overviewDirtyLabel->setText(QStringLiteral("表单状态：%1").arg(formModel->isDirty()
                                                                         ? QStringLiteral("存在未提交修改")
                                                                         : QStringLiteral("已与已保存状态一致")));
        overviewValidationLabel->setText(QStringLiteral("校验状态：%1").arg(validationResult.hasIssues()
                                                                              ? validationResult.summaryTitle()
                                                                              : QStringLiteral("校验通过，可提交")));
        overviewSubmitLabel->setText(QStringLiteral("提交状态：%1").arg(inspectorPanel->submitResultText().isEmpty()
                                                                          ? QStringLiteral("Idle")
                                                                          : inspectorPanel->submitResultText()));

        selectedCard->setValue(values.value(QStringLiteral("name")).toString().isEmpty()
                                   ? QStringLiteral("未选择")
                                   : values.value(QStringLiteral("name")).toString());
        selectedCard->setHelperText(treeTable->currentItemPath().isEmpty()
                                        ? QStringLiteral("左侧选中后自动切换")
                                        : treeTable->currentItemPath());
        themeCard->setValue(values.value(QStringLiteral("theme")).toString().isEmpty()
                                ? QStringLiteral("-")
                                : values.value(QStringLiteral("theme")).toString());
        themeCard->setHelperText(QStringLiteral("重试 %1 次").arg(QString::number(values.value(QStringLiteral("retryCount")).toInt())));

        if (!inspectorPanel->submitResultText().isEmpty()) {
            submitCard->setHelperText(inspectorPanel->submitResultText());
        } else if (!rowData.value(QStringLiteral("状态")).toString().isEmpty()) {
            submitCard->setHelperText(rowData.value(QStringLiteral("状态")).toString());
        }
    };

    connect(rollbackButton,
            &QPushButton::clicked,
            this,
            [formModel, inspectorPanel, refreshOverview]() {
                formModel->rollback();
                inspectorPanel->resetSubmitState();
                refreshOverview();
            });

    connect(formModel,
            &AverraFormModel::valuesChanged,
            this,
            [inspectorPanel, refreshOverview]() {
                QSettings settings;
                inspectorPanel->saveState(settings);
                refreshOverview();
            });
    connect(formModel,
            &AverraFormModel::dirtyChanged,
            this,
            [refreshOverview](bool) {
                refreshOverview();
            });
    connect(formModel,
            &AverraFormModel::validationChanged,
            this,
            [refreshOverview](const AverraValidationResult &) {
                refreshOverview();
            });

    connect(inspectorPanel,
            &AverraInspectorPanel::submitStateChanged,
            this,
            [submitCard, inspectorPanel, refreshOverview](AverraInspectorPanel::SubmitState state) {
                switch (state) {
                case AverraInspectorPanel::SubmitIdle:
                    submitCard->setValue(QStringLiteral("Idle"));
                    break;
                case AverraInspectorPanel::SubmitSubmitting:
                    submitCard->setValue(QStringLiteral("Submitting"));
                    break;
                case AverraInspectorPanel::SubmitSucceeded:
                    submitCard->setValue(QStringLiteral("Succeeded"));
                    break;
                case AverraInspectorPanel::SubmitFailed:
                    submitCard->setValue(QStringLiteral("Failed"));
                    break;
                }

                submitCard->setHelperText(inspectorPanel->submitResultText().isEmpty()
                                              ? QStringLiteral("支持成功与失败回滚反馈")
                                              : inspectorPanel->submitResultText());
                refreshOverview();
            });

    connect(inspectorPanel,
            &AverraInspectorPanel::formSubmitRequested,
            this,
            [objectStore, itemStore, inspectorPanel, treeTable, refreshOverview](const QString &objectId, const QVariantMap &values) {
                const QString name = values.value(QStringLiteral("name")).toString();

                if (name.contains(QStringLiteral("Rollback"), Qt::CaseInsensitive)) {
                    inspectorPanel->completeSubmitFailure(QStringLiteral("提交失败，已回滚"),
                                                          QStringLiteral("模拟后端拒绝"),
                                                          QStringLiteral("名称包含 “Rollback” 时会触发失败路径，用于演示自动回滚。"),
                                                          AverraInspectorPanel::ErrorLevel);
                    refreshOverview();
                    return;
                }

                objectStore->insert(objectId, values);

                if (itemStore->contains(objectId)) {
                    QStandardItem *item = itemStore->value(objectId);
                    item->setText(values.value(QStringLiteral("name")).toString());

                    QStandardItem *parentItem = item->parent();
                    QStandardItem *statusItem = parentItem != nullptr ? parentItem->child(item->row(), 2)
                                                                      : treeTable->model()->item(item->row(), 2);
                    if (statusItem != nullptr) {
                        statusItem->setText(values.value(QStringLiteral("autoSync")).toBool()
                                                ? QStringLiteral("Synced")
                                                : QStringLiteral("Draft"));
                    }

                    treeTable->treeView()->selectionModel()->clearCurrentIndex();
                    treeTable->treeView()->setCurrentIndex(item->index());
                }

                inspectorPanel->completeSubmitSuccess(QStringLiteral("保存成功，树表与概览已刷新"));
                QSettings settings;
                inspectorPanel->saveState(settings);
                treeTable->saveViewState(settings);
                refreshOverview();
            });

    connect(treeTable,
            &AverraTreeTable::selectionContextChanged,
            this,
            [treeTable, objectStore, inspectorPanel, refreshOverview](const QString &, const QVariantMap &, const QString &objectId) {
                if (objectId.isEmpty() || !objectStore->contains(objectId)) {
                    return;
                }

                inspectorPanel->setSettingsKey(QStringLiteral("gallery/workflow/inspector/%1").arg(objectId));
                inspectorPanel->setStatusText(treeTable->currentRowData().value(QStringLiteral("状态")).toString());
                inspectorPanel->loadObject(objectId, objectStore->value(objectId));

                QSettings settings;
                inspectorPanel->restoreState(settings);
                refreshOverview();
            });

    connect(treeTable,
            &AverraTreeTable::filterTextChanged,
            this,
            [treeTable](const QString &) {
                QSettings settings;
                treeTable->saveViewState(settings);
            });
    connect(treeTable->treeView(),
            &QTreeView::expanded,
            this,
            [treeTable](const QModelIndex &) {
                QSettings settings;
                treeTable->saveViewState(settings);
            });
    connect(treeTable->treeView(),
            &QTreeView::collapsed,
            this,
            [treeTable](const QModelIndex &) {
                QSettings settings;
                treeTable->saveViewState(settings);
            });
    connect(treeTable->treeView()->header(),
            &QHeaderView::sortIndicatorChanged,
            this,
            [treeTable](int, Qt::SortOrder) {
                QSettings settings;
                treeTable->saveViewState(settings);
            });
    connect(treeTable->treeView()->header(),
            &QHeaderView::sectionResized,
            this,
            [treeTable](int, int, int) {
                QSettings settings;
                treeTable->saveViewState(settings);
            });

    const QList<QCheckBox *> columnChecks = treeTable->columnConfigPanel()->findChildren<QCheckBox *>();
    for (int index = 0; index < columnChecks.size(); ++index) {
        connect(columnChecks.at(index),
                &QCheckBox::toggled,
                this,
                [treeTable](bool) {
                    QSettings settings;
                    treeTable->saveViewState(settings);
                });
    }

    {
        QSettings settings;
        treeTable->restoreViewState(settings);
    }

    if (treeTable->currentItemPath().isEmpty() && workspaceRoot != nullptr) {
        treeTable->treeView()->setCurrentIndex(workspaceRoot->index());
    }

    workflowLayout->addWidget(treeTable, 5);
    workflowLayout->addWidget(centerPane, 4);
    workflowLayout->addWidget(inspectorPanel, 5);

    layout->addWidget(quickChoiceRow);
    layout->addWidget(dataHeader);
    layout->addWidget(workflowWidget);

    return createScrollableCategoryPage(QStringLiteral("数据工作流"),
                                        QStringLiteral("先区分表格、纯树和完整树表工作流，再决定页面骨架。"),
                                        layout);
}

QWidget *GalleryWindow::createFeedbackPage()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    QWidget *tagRow = new QWidget;
    QHBoxLayout *tagLayout = new QHBoxLayout(tagRow);
    tagLayout->setContentsMargins(0, 0, 0, 0);
    tagLayout->setSpacing(10);

    AverraTag *neutralTag = new AverraTag(QStringLiteral("Draft"));
    neutralTag->setFilled(false);
    AverraTag *accentTag = new AverraTag(QStringLiteral("New"));
    accentTag->setTone(AverraTag::Accent);
    accentTag->setFilled(true);
    AverraTag *successTag = new AverraTag(QStringLiteral("Stable"));
    successTag->setTone(AverraTag::Success);
    successTag->setFilled(true);

    tagLayout->addWidget(neutralTag);
    tagLayout->addWidget(accentTag);
    tagLayout->addWidget(successTag);
    tagLayout->addStretch(1);

    AverraProgressBar *primaryProgressBar = new AverraProgressBar;
    primaryProgressBar->setValue(68);

    AverraInfoBar *infoBar = new AverraInfoBar(QStringLiteral("新版本可用"));
    infoBar->setDescription(QStringLiteral("Averra 0.2 正在准备中，你可以先体验新的基础控件集。"));
    infoBar->setType(AverraInfoBar::Info);

    AverraEmptyState *emptyState = new AverraEmptyState(QStringLiteral("还没有任何组件"));
    emptyState->setDescription(QStringLiteral("当前列表为空。你可以先创建一个新组件，或者从模板开始初始化。"));
    emptyState->setIconText(QStringLiteral("A"));

    QWidget *loadingRow = new QWidget;
    QHBoxLayout *loadingLayout = new QHBoxLayout(loadingRow);
    loadingLayout->setContentsMargins(0, 0, 0, 0);
    loadingLayout->setSpacing(16);

    AverraLoadingRing *loadingRing = new AverraLoadingRing;
    loadingRing->start();
    QLabel *loadingLabel = new QLabel(QStringLiteral("正在同步组件元数据"));
    loadingLabel->setStyleSheet(QStringLiteral("color: #1D2533; font-size: 13px; font-weight: 600;"));

    loadingLayout->addWidget(loadingRing);
    loadingLayout->addWidget(loadingLabel);
    loadingLayout->addStretch(1);

    AverraSkeleton *skeleton = new AverraSkeleton;
    skeleton->start();

    AverraToast *toast = new AverraToast;
    toast->setTitle(QStringLiteral("保存成功"));
    toast->setDescription(QStringLiteral("你的组件配置已更新。"));
    toast->setType(AverraToast::Success);

    AverraDialog *dialog = new AverraDialog;
    dialog->setTitle(QStringLiteral("删除组件"));
    dialog->setDescription(QStringLiteral("删除后将从工作区移除当前组件，但不会影响历史版本。"));
    dialog->addContentWidget(new QLabel(QStringLiteral("这是一个统一风格对话框壳子示例。")));
    dialog->addFooterWidget(new AverraButton(QStringLiteral("取消")));
    AverraButton *confirmButton = new AverraButton(QStringLiteral("确认删除"));
    confirmButton->setAccent(true);
    dialog->addFooterWidget(confirmButton);

    QWidget *managerRow = new QWidget;
    QHBoxLayout *managerLayout = new QHBoxLayout(managerRow);
    managerLayout->setContentsMargins(0, 0, 0, 0);
    managerLayout->setSpacing(12);

    AverraButton *showToastButton = new AverraButton(QStringLiteral("通过管理器显示 Toast"));
    AverraButton *showDialogButton = new AverraButton(QStringLiteral("通过管理器显示 Dialog"));

    connect(showToastButton,
            &QPushButton::clicked,
            this,
            [this]() {
                AverraToastManager::instance()->showToast(this,
                                                          QStringLiteral("同步完成"),
                                                          QStringLiteral("组件列表已经更新。"),
                                                          AverraToast::Success);
            });

    connect(showDialogButton,
            &QPushButton::clicked,
            this,
            [this]() {
                AverraDialog *managedDialog = new AverraDialog(this);
                managedDialog->setAttribute(Qt::WA_DeleteOnClose, true);
                managedDialog->setTitle(QStringLiteral("通过管理器打开"));
                managedDialog->setDescription(QStringLiteral("这是由 DialogManager 统一调起的对话框。"));
                managedDialog->addContentWidget(new QLabel(QStringLiteral("后续可以继续接入更完整的堆叠和定位策略。")));
                AverraButton *closeDialogButton = new AverraButton(QStringLiteral("关闭"));
                connect(closeDialogButton,
                        &QPushButton::clicked,
                        managedDialog,
                        &QDialog::close);
                managedDialog->addFooterWidget(closeDialogButton);
                AverraDialogManager::instance()->showDialog(managedDialog);
            });

    managerLayout->addWidget(showToastButton);
    managerLayout->addWidget(showDialogButton);
    managerLayout->addStretch(1);

    AverraDrawer *drawer = new AverraDrawer;
    drawer->setTitle(QStringLiteral("批量编辑"));
    drawer->setDescription(QStringLiteral("用于承载右侧滑出的编辑面板或详情面板。"));
    drawer->addContentWidget(new QLabel(QStringLiteral("这里可以放表单、表格或详情信息。")));
    drawer->addFooterWidget(new AverraButton(QStringLiteral("取消")));
    AverraButton *applyDrawerButton = new AverraButton(QStringLiteral("应用"));
    applyDrawerButton->setAccent(true);
    drawer->addFooterWidget(applyDrawerButton);

    QWidget *menuRow = new QWidget;
    QHBoxLayout *menuLayout = new QHBoxLayout(menuRow);
    menuLayout->setContentsMargins(0, 0, 0, 0);
    menuLayout->setSpacing(10);

    AverraButton *contextMenuButton = new AverraButton(QStringLiteral("打开上下文菜单"));
    AverraContextMenu *contextMenu = new AverraContextMenu(contextMenuButton);
    contextMenu->addAction(QStringLiteral("重命名"));
    contextMenu->addAction(QStringLiteral("复制"));
    contextMenu->addAction(QStringLiteral("删除"));

    connect(contextMenuButton,
            &QPushButton::clicked,
            this,
            [contextMenuButton, contextMenu]() {
                contextMenu->popup(contextMenuButton->mapToGlobal(QPoint(0, contextMenuButton->height())));
            });

    menuLayout->addWidget(contextMenuButton);
    menuLayout->addStretch(1);

    layout->addWidget(
        createDecisionCard(QStringLiteral("反馈控件先看“是否阻塞用户”"),
                           QStringLiteral("这页最容易被误用。先判断提示是即时告诉用户、等待后台完成，还是必须停下来确认。"),
                           QStringList()
                               << QStringLiteral("页面内即时提示优先 `AverraInfoBar / AverraTag / AverraProgressBar`。")
                               << QStringLiteral("非阻塞通知优先 `AverraToast`，空数据和加载状态优先 `AverraEmptyState / AverraSkeleton / AverraLoadingRing`。")
                               << QStringLiteral("需要明确确认或承载更复杂流程时，再用 `AverraDialog / AverraDrawer`。"),
                           QStringList() << QStringLiteral("反馈提示") << QStringLiteral("先看阻塞级别")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("页面内即时状态"),
                           QStringLiteral("适合在页面中持续提示状态、进度或轻量提醒，而不打断当前流程。"),
                           QStringLiteral("用户还在当前页面继续操作，但需要知道“进展如何”“状态是什么”。"),
                           QStringLiteral("列表上方、卡片内部、任务详情页。"),
                           QStringLiteral("不要把需要用户确认的危险操作塞进这里。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << tagRow
                                                   << primaryProgressBar
                                                   << infoBar),
                           QStringList() << QStringLiteral("AverraTag")
                                         << QStringLiteral("AverraProgressBar")
                                         << QStringLiteral("AverraInfoBar")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("空状态与异步加载"),
                           QStringLiteral("适合数据未返回、列表为空或页面正在初始化时给出明确预期。"),
                           QStringLiteral("用户需要知道“现在没有数据”还是“数据还没来”。"),
                           QStringLiteral("页面主体、列表区域、卡片内容区。"),
                           QStringLiteral("不要让空状态和错误状态文案混淆。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << emptyState
                                                   << loadingRow
                                                   << skeleton),
                           QStringList() << QStringLiteral("AverraEmptyState")
                                         << QStringLiteral("AverraLoadingRing")
                                         << QStringLiteral("AverraSkeleton")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("非阻塞通知"),
                           QStringLiteral("适合保存成功、同步完成、轻量失败提示等短时通知。"),
                           QStringLiteral("用户无需停下来处理，只需要被短暂告知结果。"),
                           QStringLiteral("页面右上角或当前窗口顶部。"),
                           QStringLiteral("不要把多步骤确认流程放进 Toast。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << toast
                                                   << managerRow),
                           QStringList() << QStringLiteral("AverraToast")
                                         << QStringLiteral("AverraToastManager")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("阻塞式确认与侧边流程"),
                           QStringLiteral("适合删除确认、批量编辑、复杂确认流程和上下文动作。"),
                           QStringLiteral("用户必须先做决定，或者需要在弹层里完成一段连续操作。"),
                           QStringLiteral("危险操作前、批量编辑入口、详情侧栏。"),
                           QStringLiteral("不要用对话框承载可以在页面内直接完成的小提示。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << dialog
                                                   << drawer
                                                   << menuRow),
                           QStringList() << QStringLiteral("AverraDialog")
                                         << QStringLiteral("AverraDrawer")
                                         << QStringLiteral("AverraContextMenu")));

    return createScrollableCategoryPage(QStringLiteral("反馈提示与状态"),
                                        QStringLiteral("先判断这条反馈是不是要打断用户，再决定用哪类控件。"),
                                        layout);
}

QWidget *GalleryWindow::createLayoutPage()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(14);

    AverraPropertyItem *propertyItem = new AverraPropertyItem(QStringLiteral("自动更新"));
    propertyItem->setDescription(QStringLiteral("启动应用时自动检查新版本并提示安装。"));
    AverraSwitch *propertySwitch = new AverraSwitch;
    propertySwitch->setChecked(true);
    propertyItem->setAccessoryWidget(propertySwitch);

    AverraFormSection *formSection = new AverraFormSection(QStringLiteral("更新与同步"));
    formSection->setDescription(QStringLiteral("控制自动更新、同步检查和组件索引刷新。"));
    formSection->setMetaText(QStringLiteral("General"));
    formSection->addRow(propertyItem);

    AverraSettingsPage *settingsPage = new AverraSettingsPage;
    settingsPage->setTitle(QStringLiteral("通用设置"));
    settingsPage->setDescription(QStringLiteral("统一组织属性项、搜索栏和筛选组件，作为设置页或偏好页的基础容器。"));
    settingsPage->addSection(formSection);

    AverraPropertyEditor *propertyEditor = new AverraPropertyEditor;
    propertyEditor->setTitle(QStringLiteral("节点属性"));
    propertyEditor->setDescription(QStringLiteral("适合在右侧详情面板中统一展示、筛选和切换只读状态。"));

    AverraRibbonBar *ribbonBar = new AverraRibbonBar;
    ribbonBar->setTitle(QStringLiteral("项目工具区"));
    ribbonBar->setSubtitle(QStringLiteral("适合桌面主窗口顶部的命令分组与页签切换。"));

    const int homeRibbonPage = ribbonBar->addPage(QStringLiteral("开始"));
    const int homeClipboardGroup = ribbonBar->addGroup(homeRibbonPage, QStringLiteral("剪贴板"));
    ribbonBar->addGroupWidget(homeRibbonPage, homeClipboardGroup, new AverraButton(QStringLiteral("粘贴")));
    ribbonBar->addGroupWidget(homeRibbonPage, homeClipboardGroup, new AverraButton(QStringLiteral("复制")));

    const int homeArrangeGroup = ribbonBar->addGroup(homeRibbonPage, QStringLiteral("布局"));
    ribbonBar->addGroupWidget(homeRibbonPage, homeArrangeGroup, new AverraButton(QStringLiteral("对齐")));
    ribbonBar->addGroupWidget(homeRibbonPage, homeArrangeGroup, new AverraButton(QStringLiteral("分布")));

    const int viewRibbonPage = ribbonBar->addPage(QStringLiteral("视图"));
    const int viewDisplayGroup = ribbonBar->addGroup(viewRibbonPage, QStringLiteral("显示"));
    ribbonBar->addGroupWidget(viewRibbonPage, viewDisplayGroup, new AverraButton(QStringLiteral("放大")));
    ribbonBar->addGroupWidget(viewRibbonPage, viewDisplayGroup, new AverraButton(QStringLiteral("缩小")));

    AverraPropertyGrid *propertyGrid = new AverraPropertyGrid;
    propertyGrid->setTitle(QStringLiteral("属性网格"));
    propertyGrid->setDescription(QStringLiteral("适合平铺展示一组紧凑属性，常用于基础检查和快速编辑。"));
    const int gridBaseGroup = propertyGrid->addGroup(QStringLiteral("基础属性"), QStringLiteral("常用平铺属性与标准编辑器工厂。"));
    const int versionRow = propertyGrid->addReadOnlyRowToGroup(gridBaseGroup,
                                                               QStringLiteral("版本"),
                                                               QStringLiteral("0.2"),
                                                               QStringLiteral("只读信息展示。"));
    const int autoUpdateRow = propertyGrid->addBooleanRowToGroup(gridBaseGroup,
                                                                 QStringLiteral("自动更新"),
                                                                 true,
                                                                 QStringLiteral("布尔开关编辑器。"));
    const int themeRow = propertyGrid->addEnumRowToGroup(gridBaseGroup,
                                                         QStringLiteral("显示主题"),
                                                         QStringList() << QStringLiteral("默认主题")
                                                                       << QStringLiteral("海洋蓝")
                                                                       << QStringLiteral("森林绿"),
                                                         1,
                                                         QStringLiteral("枚举下拉编辑器。"));
    const int retryRow = propertyGrid->addNumberRowToGroup(gridBaseGroup,
                                                           QStringLiteral("重试次数"),
                                                           0,
                                                           10,
                                                           3,
                                                           QStringLiteral("数值编辑器。"));
    propertyGrid->bindRow(gridBaseGroup, versionRow, QStringLiteral("version"));
    propertyGrid->bindRow(gridBaseGroup, autoUpdateRow, QStringLiteral("autoUpdate"));
    propertyGrid->bindRow(gridBaseGroup, themeRow, QStringLiteral("themeIndex"));
    propertyGrid->bindRow(gridBaseGroup, retryRow, QStringLiteral("retryCount"));
    QVariantMap propertyGridModel;
    propertyGridModel.insert(QStringLiteral("version"), QStringLiteral("0.3-preview"));
    propertyGridModel.insert(QStringLiteral("autoUpdate"), true);
    propertyGridModel.insert(QStringLiteral("themeIndex"), 2);
    propertyGridModel.insert(QStringLiteral("retryCount"), 4);
    propertyGrid->setDataModel(propertyGridModel);

    AverraFormSection *identitySection = new AverraFormSection(QStringLiteral("基础信息"));
    identitySection->setDescription(QStringLiteral("节点标识和基础展示信息。"));

    AverraPropertyItem *nameEditorItem = new AverraPropertyItem(QStringLiteral("节点名称"));
    nameEditorItem->setDescription(QStringLiteral("用于树和列表中的主要显示名称。"));
    AverraLineEdit *nameEditorLineEdit = new AverraLineEdit;
    nameEditorLineEdit->setPlaceholderText(QStringLiteral("请输入节点名称"));
    nameEditorLineEdit->setText(QStringLiteral("Core Node A"));
    nameEditorItem->setAccessoryWidget(nameEditorLineEdit);

    AverraPropertyItem *idEditorItem = new AverraPropertyItem(QStringLiteral("节点标识"));
    idEditorItem->setDescription(QStringLiteral("用于接口和配置文件中的唯一标识。"));
    AverraLineEdit *idEditorLineEdit = new AverraLineEdit;
    idEditorLineEdit->setPlaceholderText(QStringLiteral("请输入节点 ID"));
    idEditorLineEdit->setText(QStringLiteral("core-node-a"));
    idEditorItem->setAccessoryWidget(idEditorLineEdit);

    identitySection->addRow(nameEditorItem);
    identitySection->addRow(idEditorItem);

    AverraFormSection *behaviorSection = new AverraFormSection(QStringLiteral("行为与显示"));
    behaviorSection->setDescription(QStringLiteral("控制主题、自动更新和详情页展示行为。"));

    AverraPropertyItem *themeEditorItem = new AverraPropertyItem(QStringLiteral("显示主题"));
    themeEditorItem->setDescription(QStringLiteral("用于当前节点详情页的颜色风格。"));
    AverraComboBox *themeEditorComboBox = new AverraComboBox;
    themeEditorComboBox->addItems(QStringList()
                                  << QStringLiteral("默认主题")
                                  << QStringLiteral("海洋蓝")
                                  << QStringLiteral("森林绿"));
    themeEditorItem->setAccessoryWidget(themeEditorComboBox);

    AverraPropertyItem *autoSyncEditorItem = new AverraPropertyItem(QStringLiteral("自动同步"));
    autoSyncEditorItem->setDescription(QStringLiteral("保存后自动将更改同步到目标节点。"));
    AverraSwitch *autoSyncEditorSwitch = new AverraSwitch;
    autoSyncEditorSwitch->setChecked(true);
    autoSyncEditorItem->setAccessoryWidget(autoSyncEditorSwitch);

    behaviorSection->addRow(themeEditorItem);
    behaviorSection->addRow(autoSyncEditorItem);

    propertyEditor->addSection(identitySection);
    propertyEditor->addSection(behaviorSection);

    AverraInspectorPanel *inspectorPanel = new AverraInspectorPanel;
    inspectorPanel->setTitle(QStringLiteral("检查器"));
    inspectorPanel->setSubtitle(QStringLiteral("适合右侧详情检查、状态展示和动作入口集中组织。"));
    inspectorPanel->setStatusText(QStringLiteral("Live"));
    inspectorPanel->addActionWidget(new AverraButton(QStringLiteral("应用")));
    inspectorPanel->setValidationLevel(AverraInspectorPanel::WarningLevel);
    inspectorPanel->setValidationTitle(QStringLiteral("存在待确认项"));
    inspectorPanel->setValidationDescription(QStringLiteral("请检查同步策略和主题设置后再提交。"));
    inspectorPanel->setValidationVisible(true);
    inspectorPanel->setSubmitButtonText(QStringLiteral("提交更改"));
    inspectorPanel->setSubmitResultText(QStringLiteral("等待提交"));
    inspectorPanel->setSubmitState(AverraInspectorPanel::SubmitIdle);

    AverraFormSection *inspectorSection = new AverraFormSection(QStringLiteral("运行状态"));
    inspectorSection->setDescription(QStringLiteral("用于展示当前节点的运行选项和状态开关。"));
    AverraPropertyItem *inspectorSyncItem = new AverraPropertyItem(QStringLiteral("自动同步"));
    inspectorSyncItem->setDescription(QStringLiteral("保存后自动同步到目标节点。"));
    AverraSwitch *inspectorSyncSwitch = new AverraSwitch;
    inspectorSyncSwitch->setChecked(true);
    inspectorSyncItem->setAccessoryWidget(inspectorSyncSwitch);
    inspectorSection->addRow(inspectorSyncItem);
    inspectorPanel->addSection(inspectorSection);
    inspectorPanel->addFooterWidget(new AverraButton(QStringLiteral("关闭")));
    connect(inspectorPanel,
            &AverraInspectorPanel::submitRequested,
            this,
            [inspectorPanel]() {
                inspectorPanel->setSubmitResultText(QStringLiteral("正在提交..."));
                QTimer::singleShot(0,
                                   inspectorPanel,
                                   [inspectorPanel]() {
                                       inspectorPanel->completeSubmitSuccess(QStringLiteral("保存成功"));
                                   });
            });

    QWidget *statisticRow = new QWidget;
    QHBoxLayout *statisticLayout = new QHBoxLayout(statisticRow);
    statisticLayout->setContentsMargins(0, 0, 0, 0);
    statisticLayout->setSpacing(12);

    AverraStatisticCard *primaryStatisticCard = new AverraStatisticCard(QStringLiteral("在线节点"));
    primaryStatisticCard->setValue(QStringLiteral("24"));
    primaryStatisticCard->setHelperText(QStringLiteral("较昨日 +3"));
    primaryStatisticCard->setBadgeText(QStringLiteral("Live"));
    primaryStatisticCard->setAccent(true);

    AverraStatisticCard *secondaryStatisticCard = new AverraStatisticCard(QStringLiteral("待处理工单"));
    secondaryStatisticCard->setValue(QStringLiteral("7"));
    secondaryStatisticCard->setHelperText(QStringLiteral("2 个高优先级"));
    secondaryStatisticCard->setBadgeText(QStringLiteral("Queue"));

    statisticLayout->addWidget(primaryStatisticCard);
    statisticLayout->addWidget(secondaryStatisticCard);

    AverraCard *primaryCard = new AverraCard(QStringLiteral("Averra Pro"));
    primaryCard->setSubtitle(QStringLiteral("适合展示关键状态、产品摘要和操作入口。"));
    primaryCard->setAccentHeader(true);
    QLabel *primaryCardText = new QLabel(QStringLiteral("一个适合承载标题、说明、数据摘要和主操作按钮的基础卡片容器。"));
    primaryCardText->setWordWrap(true);
    primaryCard->contentLayout()->addWidget(primaryCardText);

    AverraNavigationPanel *navigationPanel = new AverraNavigationPanel(QStringLiteral("工作区"));
    navigationPanel->addItem(QStringLiteral("概览"));
    navigationPanel->addItem(QStringLiteral("组件"));
    navigationPanel->addItem(QStringLiteral("主题"));
    navigationPanel->addItem(QStringLiteral("发布"));
    navigationPanel->setCurrentIndex(1);

    QMainWindow *dockPreviewWindow = new QMainWindow;
    dockPreviewWindow->setMinimumHeight(280);

    QWidget *dockCentralWidget = new QWidget(dockPreviewWindow);
    QVBoxLayout *dockCentralLayout = new QVBoxLayout(dockCentralWidget);
    dockCentralLayout->setContentsMargins(16, 16, 16, 16);
    dockCentralLayout->setSpacing(10);

    QLabel *dockCentralLabel = new QLabel(QStringLiteral("这里模拟主工作区内容。"));
    dockCentralLabel->setWordWrap(true);
    dockCentralLayout->addWidget(dockCentralLabel);
    dockCentralLayout->addStretch(1);

    dockPreviewWindow->setCentralWidget(dockCentralWidget);

    AverraDock *dockPreview = new AverraDock(QStringLiteral("资源检查"));
    dockPreview->setDescription(QStringLiteral("适合挂载到主窗口左右两侧的工具、属性或检查面板。"));
    dockPreview->addContentWidget(new QLabel(QStringLiteral("这里可以放属性编辑器、资源树或日志过滤器。")));
    dockPreview->addContentWidget(new AverraButton(QStringLiteral("刷新检查结果")));
    dockPreviewWindow->addDockWidget(Qt::RightDockWidgetArea, dockPreview);

    AverraDivider *horizontalDivider = new AverraDivider;

    layout->addWidget(
        createDecisionCard(QStringLiteral("容器类控件先按页面骨架来选"),
                           QStringLiteral("这一页不是“哪个更高级”，而是“你的页面现在缺的是设置页骨架、详情面板，还是工作台框架”。"),
                           QStringList()
                               << QStringLiteral("设置与属性编辑优先 `AverraSettingsPage / AverraPropertyGrid / AverraPropertyEditor`。")
                               << QStringLiteral("右侧详情检查与提交流程优先 `AverraInspectorPanel`。")
                               << QStringLiteral("主窗口框架、工作台导航和命令区优先 `AverraRibbonBar / AverraNavigationPanel / AverraDock`。"),
                           QStringList() << QStringLiteral("页面布局") << QStringLiteral("先选骨架")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("设置页与属性编辑"),
                           QStringLiteral("适合偏好设置页、设备配置页和结构化参数编辑页。"),
                           QStringLiteral("你需要让用户在同一块区域里稳定地录入和检查字段。"),
                           QStringLiteral("设置页主体、右侧属性栏、偏好面板。"),
                           QStringLiteral("不要在这里只放一个按钮或纯展示统计，那会让容器显得过重。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << settingsPage
                                                   << propertyGrid
                                                   << propertyEditor),
                           QStringList() << QStringLiteral("AverraSettingsPage")
                                         << QStringLiteral("AverraPropertyGrid")
                                         << QStringLiteral("AverraPropertyEditor")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("检查器与提交面板"),
                           QStringLiteral("适合资源详情、右侧属性检查、校验提示和提交确认。"),
                           QStringLiteral("页面需要一个专门的区域来承担“查看详情 + 编辑 + 校验 + 提交”。"),
                           QStringLiteral("工作台右侧、Drawer 内容区、Dock 面板。"),
                           QStringLiteral("不要把它当成简单卡片使用，它更适合完整的详情流。"),
                           inspectorPanel,
                           QStringList() << QStringLiteral("AverraInspectorPanel")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("主窗口框架与工作台骨架"),
                           QStringLiteral("适合桌面主窗口、资源管理器和多面板工作台。"),
                           QStringLiteral("你在设计一个完整工作区，而不是单张表单或卡片。"),
                           QStringLiteral("桌面主窗口、资源工作台、管理后台外壳。"),
                           QStringLiteral("不要在轻量弹窗里硬塞 Ribbon 或 Dock。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << ribbonBar
                                                   << navigationPanel
                                                   << dockPreviewWindow),
                           QStringList() << QStringLiteral("AverraRibbonBar")
                                         << QStringLiteral("AverraNavigationPanel")
                                         << QStringLiteral("AverraDock")));

    layout->addWidget(
        createScenarioCard(QStringLiteral("概览卡片与摘要区"),
                           QStringLiteral("适合首页概览、统计摘要和分组信息展示。"),
                           QStringLiteral("你要在一个块里概括状态、说明和少量操作。"),
                           QStringLiteral("首页卡片区、列表页顶部、概览仪表盘。"),
                           QStringLiteral("不要把复杂表单放进摘要卡片里。"),
                           createVerticalDemoPanel(QList<QWidget *>()
                                                   << statisticRow
                                                   << horizontalDivider
                                                   << primaryCard),
                           QStringList() << QStringLiteral("AverraStatisticCard")
                                         << QStringLiteral("AverraCard")
                                         << QStringLiteral("AverraDivider")));

    return createScrollableCategoryPage(QStringLiteral("页面布局与容器"),
                                        QStringLiteral("先选页面骨架，再决定属性编辑、检查器还是工作台容器。"),
                                        layout);
}

QFrame *GalleryWindow::createSidebarFrame()
{
    QFrame *sidebarFrame = new QFrame;
    sidebarFrame->setFrameShape(QFrame::StyledPanel);
    sidebarFrame->setMinimumWidth(300);
    sidebarFrame->setMaximumWidth(320);

    QVBoxLayout *layout = new QVBoxLayout(sidebarFrame);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(14);

    QLabel *titleLabel = new QLabel(QStringLiteral("Averra Widgets"));
    titleLabel->setStyleSheet(QStringLiteral("font-size: 28px; font-weight: 700; color: #1D2533;"));

    QLabel *subtitleLabel = new QLabel(QStringLiteral("按业务场景选控件，而不是按控件名字盲猜。先看“选型指南”，再进入具体分类。"));
    subtitleLabel->setWordWrap(true);
    subtitleLabel->setStyleSheet(QStringLiteral("font-size: 14px; color: #5B6472;"));

    QWidget *hintWidget = new QWidget;
    QVBoxLayout *hintLayout = new QVBoxLayout(hintWidget);
    hintLayout->setContentsMargins(0, 0, 0, 0);
    hintLayout->setSpacing(8);
    hintLayout->addWidget(createGuideLabel(QStringLiteral("1. 先看页面任务：这是头部操作、表单录入、筛选切换，还是工作台骨架？")));
    hintLayout->addWidget(createGuideLabel(QStringLiteral("2. 进入对应分类页，先看“适用场景”和“尽量避免”。")));
    hintLayout->addWidget(createGuideLabel(QStringLiteral("3. 最后再把 live demo 当成落地参考。")));

    AverraSectionHeader *themeHeader = new AverraSectionHeader(QStringLiteral("主题切换"));
    themeHeader->setSubtitle(QStringLiteral("主题负责颜色气质；样式结构默认使用桌面参数，并可通过 StyleProfile JSON 自定义。"));
    themeHeader->setMetaText(QStringLiteral("Theme"));

    AverraSectionHeader *searchHeader = new AverraSectionHeader(QStringLiteral("按任务搜索"));
    searchHeader->setSubtitle(QStringLiteral("比如输入：设置、搜索、树表、提示、对话框。"));
    searchHeader->setMetaText(QStringLiteral("Guide"));

    m_categorySearchBar = new AverraSearchBar;
    m_categorySearchBar->setPlaceholderText(QStringLiteral("搜索任务、页面或控件名称"));
    m_categorySearchBar->setButtonText(QStringLiteral("推荐"));

    m_categoryHintLabel = new QLabel(QStringLiteral("先从“选型指南”开始，然后再按任务进入具体分类。"));
    m_categoryHintLabel->setWordWrap(true);
    m_categoryHintLabel->setStyleSheet(QStringLiteral("font-size: 13px; color: #5B6472; line-height: 1.45;"));

    m_oceanButton = new AverraButton(QStringLiteral("Ocean 蓝"));
    m_forestButton = new AverraButton(QStringLiteral("Forest 绿"));
    m_sunsetButton = new AverraButton(QStringLiteral("Sunset 橙"));
    m_orchidButton = new AverraButton(QStringLiteral("Orchid 紫"));

    QWidget *themeButtonRow = new QWidget;
    QHBoxLayout *themeButtonLayout = new QHBoxLayout(themeButtonRow);
    themeButtonLayout->setContentsMargins(0, 0, 0, 0);
    themeButtonLayout->setSpacing(8);
    themeButtonLayout->addWidget(m_oceanButton);
    themeButtonLayout->addWidget(m_forestButton);

    QWidget *themeButtonRow2 = new QWidget;
    QHBoxLayout *themeButtonLayout2 = new QHBoxLayout(themeButtonRow2);
    themeButtonLayout2->setContentsMargins(0, 0, 0, 0);
    themeButtonLayout2->setSpacing(8);
    themeButtonLayout2->addWidget(m_sunsetButton);
    themeButtonLayout2->addWidget(m_orchidButton);

    m_categoryNavigation = new AverraNavigationPanel(QStringLiteral("组件分类"));

    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addWidget(hintWidget);
    layout->addWidget(searchHeader);
    layout->addWidget(m_categorySearchBar);
    layout->addWidget(m_categoryHintLabel);
    layout->addWidget(themeHeader);
    layout->addWidget(themeButtonRow);
    layout->addWidget(themeButtonRow2);
    layout->addWidget(m_categoryNavigation);
    layout->addStretch(1);

    connect(m_oceanButton, &QPushButton::clicked, this, &GalleryWindow::applyOceanTheme);
    connect(m_forestButton, &QPushButton::clicked, this, &GalleryWindow::applyForestTheme);
    connect(m_sunsetButton, &QPushButton::clicked, this, &GalleryWindow::applySunsetTheme);
    connect(m_orchidButton, &QPushButton::clicked, this, &GalleryWindow::applyOrchidTheme);
    connect(m_categorySearchBar,
            &AverraSearchBar::textChanged,
            this,
            &GalleryWindow::updateCategoryGuide);
    connect(m_categorySearchBar,
            &AverraSearchBar::searchRequested,
            this,
            &GalleryWindow::updateCategoryGuide);
    connect(m_categoryNavigation,
            &AverraNavigationPanel::currentIndexChanged,
            this,
            &GalleryWindow::switchCategory);

    return sidebarFrame;
}

QFrame *GalleryWindow::createPreviewFrame()
{
    QFrame *previewFrame = new QFrame;
    previewFrame->setFrameShape(QFrame::StyledPanel);

    QVBoxLayout *layout = new QVBoxLayout(previewFrame);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(16);

    QLabel *titleLabel = new QLabel(QStringLiteral("场景导览"));
    titleLabel->setStyleSheet(QStringLiteral("font-size: 20px; font-weight: 700; color: #1D2533;"));

    QLabel *subtitleLabel = new QLabel(QStringLiteral("每一页都会先告诉你“什么时候用”，再展示控件的 live demo。"));
    subtitleLabel->setWordWrap(true);
    subtitleLabel->setStyleSheet(QStringLiteral("font-size: 14px; color: #5B6472;"));

    m_previewStack = new QStackedWidget;

    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addWidget(m_previewStack, 1);

    return previewFrame;
}

void GalleryWindow::setupWindow()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *layout = new QHBoxLayout(central);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(20);

    layout->addWidget(createSidebarFrame());
    layout->addWidget(createPreviewFrame(), 1);
}

void GalleryWindow::populateCategories()
{
    m_categoryNavigation->addItem(QStringLiteral("选型指南"));
    m_categoryNavigation->addItem(QStringLiteral("页面头部与操作"));
    m_categoryNavigation->addItem(QStringLiteral("表单录入与搜索"));
    m_categoryNavigation->addItem(QStringLiteral("筛选切换与参数"));
    m_categoryNavigation->addItem(QStringLiteral("数据工作流"));
    m_categoryNavigation->addItem(QStringLiteral("反馈提示与状态"));
    m_categoryNavigation->addItem(QStringLiteral("页面布局与容器"));

    m_previewStack->addWidget(createGuidePage());
    m_previewStack->addWidget(createActionsPage());
    m_previewStack->addWidget(createInputPage());
    m_previewStack->addWidget(createSelectionPage());
    m_previewStack->addWidget(createDataPage());
    m_previewStack->addWidget(createFeedbackPage());
    m_previewStack->addWidget(createLayoutPage());

    m_categoryNavigation->setCurrentIndex(0);
    m_previewStack->setCurrentIndex(0);
}
