# Changelog

本文件记录 `Averra Widgets` 的公开版本发布内容，便于发布、回滚和接入方核对差异。

## [1.0.1] - 2026-03-27

### Added

- 新增 `AverraStyleProfile` 的 `QVariantMap` 映射与 JSON 文件读写能力
- 新增 `docs/style-profile.example.json`，提供默认桌面风格参数示例
- 新增 GitHub Actions 工作流，支持持续集成与 `v*` tag 自动打包发布

### Changed

- 去掉内置 `Win11 / macOS` 风格预设，保留统一 `StyleProfile` 结构并默认使用当前桌面参数
- `AverraThemeManager` 改为以 `ThemePreset + StyleProfile` 为核心入口，不再暴露 `StylePreset`
- Gallery 和测试同步切换到 `StyleProfile` 自定义模式
- 发布文档、README、打包脚本与安装产物统一纳入 `style-profile.example.json` 和 GitHub 发布流程

### Fixed

- 修复 `InfoBar` 样式占位符警告
- 修复 `Switch` 尺寸测试受自定义样式状态污染的问题

## [1.0.0] - 2026-03-27

### Added

- 建立 `Qt Widgets` 组件库主干，提供统一主题、基础控件、复合控件、页面容器和 Gallery 示例
- 新增统一字段模型能力：`AverraFieldDefinition`、`AverraFormModel`、`AverraValidationResult`
- 新增 `TreeTable -> InspectorPanel -> 提交/回滚` 的完整工作流示例
- 新增 `AverraDateEdit` 自定义日期弹层，实现自布局、自绘头部和日期格

### Changed

- `AverraPropertyGrid` 升级为可直接消费字段模型的表单容器
- `AverraInspectorPanel` 升级为可直接绑定 `AverraFormModel` 的检查器面板
- `AverraTreeTable` 增加当前选中上下文输出和更完整的视图状态持久化
- README、ROADMAP 和 RELEASE 文档升级为面向接入与发布的说明

### Fixed

- 修复 `AverraDateEdit` 弹出日历与整套 Averra 视觉语言不一致的问题
- 修复高级组件联动测试和属性网格测试中的回归问题

### Packaging

- 新增 `CPack` 二进制包和源码包生成能力
- 新增 `qmake SDK` 归档生成能力
- 新增 `SHA256SUMS.txt` 校验输出
- 新增最小消费者工程自动验证脚本，用于检查 `find_package` 和 `AverraWidgets.pri` 接入路径

### Release

- 将 `AverraThemePalette`、`AverraThemeManager`、`AverraIcon`、`AverraFieldDefinition`、`AverraFormModel`、`AverraValidationResult`、`AverraPropertyGrid`、`AverraInspectorPanel`、`AverraTreeTable` 收口为 `1.0.1` 前的稳定基线
