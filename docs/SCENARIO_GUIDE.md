# Averra Scenario Guide

## 目标

这份文档按“页面任务”来组织控件，而不是按控件名字罗列。你可以先确定自己在做什么页面，再回到对应控件。

## 页面头部与操作

适合：

- 页面顶部主操作
- 列表页头部命令区
- 带数量提醒的快捷入口

推荐控件：

- `AverraButton`
- `AverraBadgeButton`
- `AverraToolbar`

不要误用：

- 不要在同一层级放多个强调按钮
- 不要把复杂表单堆进工具栏

## 表单录入与搜索

适合：

- 结构化字段录入
- 查询条件、日期、范围输入
- 发布说明、审核意见、备注

推荐控件：

- `AverraLineEdit`
- `AverraComboBox`
- `AverraNumberBox`
- `AverraNumberRangeBox`
- `AverraDateEdit`
- `AverraSearchBar`
- `AverraTextArea`

不要误用：

- 不要用 `SearchBar` 承担正式表单字段
- 不要用 `TextArea` 模拟搜索框

## 筛选切换与参数

适合：

- 列表筛选
- 视图切换
- 偏好策略开关
- 连续参数调节

推荐控件：

- `AverraFilterChipGroup`
- `AverraPagination`
- `AverraRadioButton`
- `AverraSegmentedControl`
- `AverraSwitch`
- `AverraCheckBox`
- `AverraSlider`

不要误用：

- 不要让分页承担筛选职责
- 不要让 `Switch` 承担危险确认动作

## 数据工作流

适合：

- 纯表格列表页
- 纯层级浏览
- 左侧树表联动右侧详情

推荐控件：

- `AverraDataTable`
- `AverraTreeView`
- `AverraTreeTable`
- `AverraInspectorPanel`

不要误用：

- 不要用纯表格硬塞树结构
- 不要用纯树硬扛状态列和提交流程

## 反馈提示与状态

适合：

- 页面内即时提示
- 非阻塞通知
- 空状态、加载态
- 阻塞确认与侧边流程

推荐控件：

- `AverraInfoBar`
- `AverraToast`
- `AverraEmptyState`
- `AverraLoadingRing`
- `AverraSkeleton`
- `AverraDialog`
- `AverraDrawer`

不要误用：

- 不要把确认流程塞进 Toast
- 不要把空状态和错误状态混成一类文案

## 页面布局与容器

适合：

- 设置页和属性编辑页
- 右侧详情检查器
- 主窗口工作台
- 概览卡片区

推荐控件：

- `AverraSettingsPage`
- `AverraPropertyGrid`
- `AverraPropertyEditor`
- `AverraInspectorPanel`
- `AverraRibbonBar`
- `AverraNavigationPanel`
- `AverraDock`
- `AverraCard`
- `AverraStatisticCard`

不要误用：

- 不要在轻量弹窗里硬塞 `RibbonBar` 或 `Dock`
- 不要把复杂表单直接堆进摘要卡片
