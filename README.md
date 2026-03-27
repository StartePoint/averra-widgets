# Averra Widgets

`Averra Widgets` 是一个面向 `Qt Widgets` 的第三方桌面控件库，目标是提供一套更适合商业桌面产品的 UI 基础设施：

- 统一的主题与视觉语言
- 可直接落业务页面的基础/复合控件
- 同时兼容 `CMake` 与 `qmake`
- 面向 `Qt 5.14.2` 的稳定开发体验

当前仓库已经具备 `1.0.0` 发布所需的可编译、可预览、可测试、可打包基础能力，并以文档中声明的核心接口作为稳定基线。

## 项目定位

`Averra Widgets` 不是简单的样式表集合，也不是单纯的演示仓库。它更接近一套面向桌面业务系统的控件基础库，适合这些场景：

- 后台管理桌面端
- 工业软件 / 配置工具 / 运维工具
- 设置页、列表页、统计页、表单页
- 需要统一视觉风格和可复用控件体系的 Qt Widgets 项目

当前不适合的场景：

- QML / Qt Quick 项目
- 依赖平台原生控件外观且不希望统一样式的系统
- 低于 Qt `5.14.2` 的老工程

## 特性

- 统一主题系统：颜色、状态色、表面色、强调色集中管理
- 同时支持 `CMake` 和 `qmake`
- 提供纯代码版 Gallery，便于控件分类预览
- 统一测试工程，便于集中回归验证
- 提供字段模型、校验结果和表单状态管理类型
- 支持 `TreeTable -> InspectorPanel -> 提交/回滚` 的完整工作流示例
- 基础控件、复合组件、页面容器逐步齐备

## 重要说明

- `1.0.0` 起，README 与发布文档中列出的核心接口作为稳定基线维护；扩展组件和示例仍会继续演进。
- 工程明确围绕 `Qt Widgets` 技术路线设计，所有能力、示例和测试都以 `QWidget` / `QDialog` / `QMainWindow` 场景为中心。
- 仓库同时维护 `CMake` 与 `qmake` 两套接入方式，其中 `CMake` 是安装导出和跨工程集成的主入口，`qmake` 用于兼容典型 Qt Widgets 工程。
- 构建产物统一收敛到固定目录 `build/`，其中 `qmake` 产物位于 `build/qmake/`，避免在仓库根目录和子目录中散落生成文件。

## 支持与验证范围

| 维度 | 当前状态 |
| --- | --- |
| 技术路线 | 仅面向 `Qt Widgets`，不覆盖 `QML / Qt Quick` |
| Qt 版本 | 以 `Qt 5.14.2` 为开发与验证基线 |
| 编译器 / 语言 | `C++17` |
| 构建系统 | `CMake 3.16+`、`qmake 3.1` |
| 测试方式 | 统一 `Qt Test` 工程 |
| 当前验证重点 | 组件库本体、Gallery 示例、统一测试工程 |
| 跨平台结论 | 仓库保留桌面工程接入路径，但当前 README 不提供完整的多平台验证矩阵，建议按目标环境实测 |

## 环境要求

当前库按以下环境开发与验证：

- Qt `5.14.2`
- CMake `3.16` 或更新版本
- QMake `3.1`
- C++17

依赖模块：

- `Qt5::Widgets`
- `Qt5::Test`

## 当前能力

当前仓库已经包含：

- 一个可安装的共享库目标 `Averra::Widgets`
- 一套 `qmake` 顶层工程与 `pri` 接入入口
- 一套主题管理能力
- 一套统一字段模型能力：`AverraFieldDefinition`、`AverraFormModel`、`AverraValidationResult`
- 一个纯代码版 Gallery 示例程序
- 一个统一的 `Qt Test` 测试工程

## 字段模型驱动工作流

当前仓库已经不只是“把控件摆出来”。围绕高级组件，已经补上了一条可以直接落业务页面的最小工作流：

- `AverraFieldDefinition` 负责描述字段 key、标题、类型、默认值、分组、只读状态、可选项和校验规则
- `AverraFormModel` 负责字段值、脏状态、提交前校验、提交后提交态基线和 `QSettings` 持久化
- `AverraPropertyGrid` 可以直接消费 `AverraFormModel`，自动生成标准编辑器、双向绑定数据并把校验错误反映到行级 UI
- `AverraInspectorPanel` 可以直接绑定 `AverraFormModel`，执行 `编辑 -> 校验 -> 提交 -> 成功/失败反馈 -> 失败回滚`
- `AverraTreeTable` 会输出当前选中节点的 `item path`、当前行数据和对象 `id`，可直接驱动右侧检查器
- Gallery 内置了左侧树表、中间概览、右侧检查器的完整联动示例

## 公开 API 建议

当前建议外部项目优先依赖下面这批接口，它们已经具备比较明确的角色边界：

- 主题与基础设施：`AverraThemePalette`、`AverraThemeManager`、`AverraIcon`
- 字段模型与状态：`AverraFieldDefinition`、`AverraFormModel`、`AverraValidationResult`
- 高级业务组件：`AverraPropertyGrid`、`AverraInspectorPanel`、`AverraTreeTable`

像 `AverraStyleHelper` 这类内部样式辅助类型，仍然建议视为内部实现细节，不要在外部工程里直接依赖。

## 场景速查

如果你是按页面任务找控件，而不是按名字找，可以先看这张表：

| 页面任务 | 优先看哪些控件 | 典型落点 |
| --- | --- | --- |
| 页面头部、主次操作、命令区 | `AverraButton`、`AverraBadgeButton`、`AverraToolbar` | 页面顶栏、卡片页脚、列表页头部 |
| 字段录入、搜索条件、长文本说明 | `AverraLineEdit`、`AverraComboBox`、`AverraNumberBox`、`AverraDateEdit`、`AverraSearchBar`、`AverraTextArea` | 设置页、查询面板、备注区 |
| 筛选、分页、切换、参数调节 | `AverraFilterChipGroup`、`AverraPagination`、`AverraSegmentedControl`、`AverraSwitch`、`AverraSlider` | 列表筛选区、工具条、设置侧栏 |
| 纯表格、纯树、树表联动详情 | `AverraDataTable`、`AverraTreeView`、`AverraTreeTable`、`AverraInspectorPanel` | 资源页、工作台、右侧详情区 |
| 提示、空状态、加载态、确认流程 | `AverraInfoBar`、`AverraEmptyState`、`AverraToast`、`AverraDialog`、`AverraDrawer` | 页面主体、通知区、危险操作前 |
| 设置页骨架、工作台容器、概览摘要 | `AverraSettingsPage`、`AverraPropertyGrid`、`AverraRibbonBar`、`AverraDock`、`AverraCard` | 偏好页、主窗口、概览首页 |

更细的“适用场景 / 推荐位置 / 尽量避免”说明见 [docs/SCENARIO_GUIDE.md](docs/SCENARIO_GUIDE.md)。

## 组件总览

当前公开能力可以按下面方式快速理解：

| 分类 | 数量 | 代表组件 | 典型场景 |
| --- | ---: | --- | --- |
| 基础设施 | 3 | `AverraThemePalette`、`AverraThemeManager`、`AverraIcon` | 主题、品牌入口、全局样式驱动 |
| 操作与页面 | 7 | `AverraButton`、`AverraToolbar`、`AverraRibbonBar` | 页面头部、主操作区、列表操作区 |
| 输入与表单 | 8 | `AverraLineEdit`、`AverraSearchBar`、`AverraPropertyGrid` | 搜索、配置录入、业务表单 |
| 选择与切换 | 6 | `AverraSwitch`、`AverraTabs`、`AverraSegmentedControl` | 状态切换、视图切换、筛选 |
| 数据展示与反馈 | 10 | `AverraDataTable`、`AverraTreeView`、`AverraTreeTable` | 列表页、概览页、层级数据与提示 |
| 弹层与浮层 | 4 | `AverraDialog`、`AverraToast`、`AverraDock` | 对话框、通知、Dock 面板、侧边详情面板 |
| 页面结构与容器 | 9 | `AverraCard`、`AverraPropertyEditor`、`AverraInspectorPanel` | 设置页、属性面板、检查器与卡片化布局 |
| 管理器 | 2 | `AverraDialogManager`、`AverraToastManager` | 统一弹层调度 |

如果按“可直接拼业务页面”的角度看，当前库已经覆盖了设置页、属性编辑页、列表页、统计概览页和右侧详情编辑面板的基础组合能力。

## 组件矩阵

### 基础设施

- `AverraThemePalette`
- `AverraThemeManager`
- `AverraIcon`

### 操作与页面

- `AverraButton`
- `AverraBadgeButton`
- `AverraToolbar`
- `AverraRibbonBar`
- `AverraFilterChipGroup`
- `AverraPagination`
- `AverraContextMenu`

### 输入与表单

- `AverraLineEdit`
- `AverraComboBox`
- `AverraNumberBox`
- `AverraNumberRangeBox`
- `AverraDateEdit`
- `AverraSearchBar`
- `AverraPropertyGrid`
- `AverraTextArea`

### 选择与切换

- `AverraSwitch`
- `AverraRadioButton`
- `AverraCheckBox`
- `AverraSlider`
- `AverraSegmentedControl`
- `AverraTabs`

### 数据展示与反馈

- `AverraTag`
- `AverraProgressBar`
- `AverraDataTable`
- `AverraTreeView`
- `AverraTreeTable`
- `AverraEmptyState`
- `AverraStatisticCard`
- `AverraInfoBar`
- `AverraLoadingRing`
- `AverraSkeleton`

### 弹层与浮层

- `AverraDialog`
- `AverraToast`
- `AverraDock`
- `AverraDrawer`

### 页面结构与容器

- `AverraCard`
- `AverraSectionHeader`
- `AverraDivider`
- `AverraPropertyItem`
- `AverraPropertyEditor`
- `AverraInspectorPanel`
- `AverraFormSection`
- `AverraSettingsPage`
- `AverraNavigationPanel`

### 管理器

- `AverraDialogManager`
- `AverraToastManager`

## 构建

### CMake

```bash
cmake -S . -B build
cmake --build build -j16
```

### qmake

```bash
qmake AverraWidgets.pro
make -j16
```

说明：

- `CMake` 与 `qmake` 构建产物统一收敛到仓库下固定目录 `build/`
- `qmake` 产物位于 `build/qmake/`，不再散落到仓库根目录和各子模块隐藏目录

## 运行 Gallery

使用 CMake 构建时：

```bash
./build/examples/gallery/AverraGallery
```

使用 qmake 构建时：

```bash
./build/qmake/bin/AverraGallery
```

## 运行测试

使用 CMake 构建时：

```bash
ctest --test-dir build --output-on-failure
```

使用 qmake 构建时：

```bash
QT_QPA_PLATFORM=offscreen ./build/qmake/bin/AverraWidgetsTests
```

## 生成发行包

仓库现在提供了一键打包脚本，会同时生成：

- `CMake / CPack` 二进制包
- `CPack` 源码包
- `qmake SDK` 归档包
- `SHA256SUMS.txt` 校验文件
- 最小 `CMake` / `qmake` 消费者工程验证结果

直接执行：

```bash
./scripts/package_release.sh
```

常用选项：

```bash
./scripts/package_release.sh --skip-tests
./scripts/package_release.sh --skip-consumer-verify
./scripts/package_release.sh --cmake-only
./scripts/package_release.sh --qmake-only
./scripts/package_release.sh --dist-dir /custom/output
```

默认输出目录：

```text
dist/
```

更具体的发布说明和产物结构见 [docs/RELEASE.md](docs/RELEASE.md)。

## 发布记录

仓库现在维护公开版本记录文件：

```text
CHANGELOG.md
```

新增或准备一个版本记录时，可以执行：

```bash
./scripts/prepare_release_record.sh --version 1.0.0 --date 2026-03-27
```

如果已经准备好了本次版本说明，也可以通过文件注入正文：

```bash
./scripts/prepare_release_record.sh --version 0.2.0 --notes-file /path/to/release-notes.md
```

这个脚本会同步更新：

- [include/Averra/AverraVersion.h](/mnt/NewDisk/3rdselftdevlibrary/averra-widgets/include/Averra/AverraVersion.h)
- [CMakeLists.txt](/mnt/NewDisk/3rdselftdevlibrary/averra-widgets/CMakeLists.txt)
- [src/AverraWidgets.pro](/mnt/NewDisk/3rdselftdevlibrary/averra-widgets/src/AverraWidgets.pro)
- [CHANGELOG.md](/mnt/NewDisk/3rdselftdevlibrary/averra-widgets/CHANGELOG.md)

## 验证发行包接入

如果你只想验证已生成的制品，而不重新打包，可以直接运行：

```bash
./scripts/verify_release_consumers.sh
```

它会自动解压已生成的发布包，并实际编译、运行两个最小消费者工程：

- 一个 `CMake + find_package(AverraWidgets CONFIG REQUIRED)`
- 一个 `qmake + include(AverraWidgets.pri)`

## 工程接入

### 在 CMake 工程中接入

```cmake
find_package(AverraWidgets CONFIG REQUIRED)

target_link_libraries(your_app
    PRIVATE
        Averra::Widgets
)
```

### 在 qmake 工程中接入

```qmake
include(/path/to/averra-widgets/AverraWidgets.pri)
```

默认行为：

- 自动加入 `include/` 头文件目录
- 默认链接仓库固定输出目录 `build/qmake/lib/` 下的 `AverraWidgets` 动态库

如需自定义库目录：

```qmake
AVERRA_LIB_DIR = /custom/path/to/lib
include(/path/to/averra-widgets/AverraWidgets.pri)
```

发布策略、兼容说明和发布检查清单见 [docs/RELEASE.md](docs/RELEASE.md)。

## 快速开始

### 最小按钮示例

```cpp
#include <Averra/AverraButton.h>

#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);

    AverraButton *button = new AverraButton(QStringLiteral("主操作按钮"));
    button->setAccent(true);

    layout->addWidget(button);
    window.show();

    return app.exec();
}
```

### 字段模型驱动示例

```cpp
#include <Averra/AverraFieldDefinition.h>
#include <Averra/AverraFormModel.h>
#include <Averra/AverraInspectorPanel.h>

AverraFormModel *model = new AverraFormModel(this);
QList<AverraFieldDefinition> fields;

AverraFieldDefinition nameField = AverraFieldDefinition::createText(
    QStringLiteral("name"),
    QStringLiteral("节点名称"));
nameField.addValidationRule(AverraValidationRule::required());
fields.append(nameField);

fields.append(AverraFieldDefinition::createBoolean(
    QStringLiteral("autoSync"),
    QStringLiteral("自动同步"),
    true));

model->setFields(fields);
model->loadObject(QStringLiteral("scene-indexer"),
                  QVariantMap{
                      {QStringLiteral("name"), QStringLiteral("Scene Indexer")},
                      {QStringLiteral("autoSync"), true}
                  });

AverraInspectorPanel *panel = new AverraInspectorPanel;
panel->setFormModel(model);
panel->setSubmitButtonText(QStringLiteral("提交更改"));
```

### 树表 + 检查器联动示例

典型页面组合如下：

- 左侧 `AverraTreeTable` 负责输出当前选中节点的路径、对象 `id` 和当前行数据
- 中间概览区负责展示摘要、脏状态、校验结果和提交状态
- 右侧 `AverraInspectorPanel` 绑定 `AverraFormModel`，按对象 `id` 装载数据并提交
- 树表视图状态和表单编辑状态都可以通过 `QSettings` 持久化恢复

### 搜索栏示例

```cpp
#include <Averra/AverraSearchBar.h>

AverraSearchBar *searchBar = new AverraSearchBar;
searchBar->setPlaceholderText(QStringLiteral("搜索组件"));
searchBar->setButtonText(QStringLiteral("查找"));

QObject::connect(searchBar,
                 &AverraSearchBar::searchRequested,
                 searchBar,
                 [](const QString &text) {
                     qDebug() << "search:" << text;
                 });
```

### 数据表格示例

```cpp
#include <Averra/AverraDataTable.h>

AverraDataTable *table = new AverraDataTable(QStringLiteral("组件列表"));
table->setSubtitle(QStringLiteral("当前工作区中的所有组件"));
table->setHeaders(QStringList()
                  << QStringLiteral("名称")
                  << QStringLiteral("类型")
                  << QStringLiteral("状态"));

table->addRow(QStringList()
              << QStringLiteral("AverraButton")
              << QStringLiteral("输入")
              << QStringLiteral("Stable"));

QObject::connect(table,
                 &AverraDataTable::rowActionTriggered,
                 table,
                 [](int row) {
                     qDebug() << "action row:" << row;
                 });
```

### 设置页示例

```cpp
#include <Averra/AverraFormSection.h>
#include <Averra/AverraPropertyEditor.h>
#include <Averra/AverraPropertyItem.h>
#include <Averra/AverraSettingsPage.h>
#include <Averra/AverraSwitch.h>

AverraSettingsPage *page = new AverraSettingsPage;
page->setTitle(QStringLiteral("通用设置"));
page->setDescription(QStringLiteral("应用级别偏好项"));

AverraFormSection *section = new AverraFormSection(QStringLiteral("更新与同步"));
section->setDescription(QStringLiteral("控制自动更新行为"));

AverraPropertyItem *item = new AverraPropertyItem(QStringLiteral("自动更新"));
item->setDescription(QStringLiteral("启动时自动检查新版本"));

AverraSwitch *sw = new AverraSwitch;
sw->setChecked(true);
item->setAccessoryWidget(sw);

section->addRow(item);
page->addSection(section);
```

### 属性编辑器示例

```cpp
#include <Averra/AverraFormSection.h>
#include <Averra/AverraLineEdit.h>
#include <Averra/AverraPropertyEditor.h>
#include <Averra/AverraPropertyItem.h>

AverraPropertyEditor *editor = new AverraPropertyEditor;
editor->setTitle(QStringLiteral("节点属性"));
editor->setDescription(QStringLiteral("适合右侧详情编辑面板"));

AverraFormSection *section = new AverraFormSection(QStringLiteral("基础信息"));
AverraPropertyItem *nameItem = new AverraPropertyItem(QStringLiteral("节点名称"));

AverraLineEdit *nameEdit = new AverraLineEdit;
nameEdit->setText(QStringLiteral("Core Node A"));
nameItem->setAccessoryWidget(nameEdit);

section->addRow(nameItem);
editor->addSection(section);
```

### 属性网格示例

```cpp
#include <Averra/AverraPropertyGrid.h>

AverraPropertyGrid *grid = new AverraPropertyGrid;
grid->setTitle(QStringLiteral("基础属性"));
const int baseGroup = grid->addGroup(QStringLiteral("基础属性"));
const int versionRow = grid->addReadOnlyRowToGroup(baseGroup, QStringLiteral("版本"), QStringLiteral("0.2"));
const int autoUpdateRow = grid->addBooleanRowToGroup(baseGroup, QStringLiteral("自动更新"), true);
grid->bindRow(baseGroup, versionRow, QStringLiteral("version"));
grid->bindRow(baseGroup, autoUpdateRow, QStringLiteral("autoUpdate"));

QVariantMap model;
model.insert(QStringLiteral("version"), QStringLiteral("0.3-preview"));
model.insert(QStringLiteral("autoUpdate"), false);
grid->setDataModel(model);
```

### TreeTable 示例

```cpp
#include <Averra/AverraTreeTable.h>

#include <QSettings>

AverraTreeTable *treeTable = new AverraTreeTable(QStringLiteral("资源树表"));
treeTable->setSettingsKey(QStringLiteral("workspace/treeTable"));
treeTable->setHeaders(QStringList()
                      << QStringLiteral("名称")
                      << QStringLiteral("类型")
                      << QStringLiteral("状态"));
treeTable->setColumnConfigVisible(true);

QSettings settings(QStringLiteral("Averra"), QStringLiteral("Gallery"));
treeTable->saveViewState(settings);
treeTable->restoreViewState(settings);
```

### InspectorPanel 示例

```cpp
#include <Averra/AverraFormSection.h>
#include <Averra/AverraInspectorPanel.h>

AverraInspectorPanel *panel = new AverraInspectorPanel;
panel->setTitle(QStringLiteral("检查器"));
panel->setStatusText(QStringLiteral("Live"));
panel->setValidationLevel(AverraInspectorPanel::WarningLevel);
panel->setValidationTitle(QStringLiteral("配置不完整"));
panel->setValidationVisible(true);
panel->setSubmitButtonText(QStringLiteral("提交更改"));

AverraFormSection *section = new AverraFormSection(QStringLiteral("运行状态"));
panel->addSection(section);

QObject::connect(panel,
                 &AverraInspectorPanel::submitRequested,
                 panel,
                 [panel]() {
                     panel->completeSubmitSuccess(QStringLiteral("保存成功"));
                 });

QObject::connect(panel,
                 &AverraInspectorPanel::retryRequested,
                 panel,
                 [panel]() {
                     panel->triggerSubmit();
                 });
```

### Ribbon 示例

```cpp
#include <Averra/AverraButton.h>
#include <Averra/AverraRibbonBar.h>

AverraRibbonBar *ribbon = new AverraRibbonBar;
ribbon->setTitle(QStringLiteral("项目工具区"));
ribbon->setSubtitle(QStringLiteral("适合组织桌面主窗口顶部命令"));

const int homePage = ribbon->addPage(QStringLiteral("开始"));
const int clipboardGroup = ribbon->addGroup(homePage, QStringLiteral("剪贴板"));
ribbon->addGroupWidget(homePage, clipboardGroup, new AverraButton(QStringLiteral("粘贴")));
ribbon->addGroupWidget(homePage, clipboardGroup, new AverraButton(QStringLiteral("复制")));
```

### Dock 示例

```cpp
#include <Averra/AverraDock.h>

#include <QLabel>
#include <QMainWindow>

QMainWindow *window = new QMainWindow;
AverraDock *dock = new AverraDock(QStringLiteral("属性面板"), window);
dock->setDescription(QStringLiteral("用于承载编辑器、资源树或检查结果。"));
dock->addContentWidget(new QLabel(QStringLiteral("Dock 内容区域")));

window->addDockWidget(Qt::RightDockWidgetArea, dock);
```

## 主题系统

当前主题入口：

- `AverraThemePalette`
- `AverraThemeManager`

### 获取当前主题

```cpp
AverraThemePalette palette = AverraThemeManager::instance()->palette();
```

### 整体替换主题

```cpp
AverraThemePalette palette = AverraThemePalette::createLightPalette();
palette.setAccentColor(QColor(QStringLiteral("#2E6BE6")));
palette.setAccentSurfaceColor(QColor(QStringLiteral("#EDF3FF")));

AverraThemeManager::instance()->setPalette(palette);
```

### 只改强调色

```cpp
AverraThemeManager::instance()->setAccentColor(QColor(QStringLiteral("#2E8B57")));
```

### 只改交互状态色

```cpp
AverraThemeManager::instance()->setSurfaceHoverColor(QColor(QStringLiteral("#F3F6FA")));
AverraThemeManager::instance()->setSurfacePressedColor(QColor(QStringLiteral("#E6EDF6")));
AverraThemeManager::instance()->setAccentPressedColor(QColor(QStringLiteral("#1F58D0")));
```

当前主题变量包括：

- `accentColor`
- `surfaceColor`
- `surfaceRaisedColor`
- `surfaceHoverColor`
- `surfacePressedColor`
- `surfaceDisabledColor`
- `borderColor`
- `borderHoverColor`
- `textPrimaryColor`
- `textSecondaryColor`
- `textDisabledColor`
- `accentSurfaceColor`
- `accentHoverColor`
- `accentPressedColor`
- `accentDisabledColor`
- `successColor`
- `successSurfaceColor`
- `warningColor`
- `warningSurfaceColor`
- `errorColor`
- `errorSurfaceColor`

## Gallery 说明

当前 `AverraGallery` 已改为纯代码界面，不再依赖 `.ui` 文件。

展示方式采用：

- 左侧分类导航
- 右侧场景页面切换
- 按任务搜索并推荐最合适的分类入口

当前分类包括：

- 选型指南
- 页面头部与操作
- 表单录入与搜索
- 筛选切换与参数
- 数据工作流
- 反馈提示与状态
- 页面布局与容器

这种结构更适合让接入者先按页面任务选型，再回到具体控件实现。

## 测试策略

当前仓库使用统一测试工程，而不是一个组件一个测试项目。

测试入口：

- `tests/widgets/AverraWidgetsTests.pro`
- `tests/widgets/CMakeLists.txt`
- `tests/widgets/test_main.cpp`

这意味着：

- 所有组件测试集中构建
- 便于统一回归
- 更适合后续 CI 集成

## 目录结构

- `include/Averra/`：公共头文件
- `src/`：库实现
- `resources/`：内置资源
- `examples/gallery/`：纯代码控件预览程序
- `tests/`：统一 `Qt Test` 单元测试
- `docs/`：发布、路线和场景选型说明
- `AverraWidgets.pro`：qmake 顶层子工程入口
- `AverraWidgets.pri`：外部 qmake 工程接入入口
