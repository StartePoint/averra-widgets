# Averra Release Guide

## 目标

本文件用于约束 `Averra Widgets` 的对外发布节奏、兼容边界和发布前检查项，方便把当前仓库从“控件展示”继续推进到“可接入、可发布、可回滚”的组件库形态。

## 版本策略

- 主版本号：用于表示公开 API 或接入方式发生不兼容变化
- 次版本号：用于表示新增公开能力，例如新增稳定组件、字段模型能力、工作流示例或新的安装导出能力
- 修订号：用于表示缺陷修复、文档修正、样式和测试补齐，不应引入公开 API 破坏性变更

建议遵循以下原则：

- 如果修改了公开头文件且会破坏既有调用方编译或行为，提升主版本号
- 如果新增了公开类型但不破坏既有接口，提升次版本号
- 如果主要是修复实现、补测试、补文档和细化 Gallery，提升修订号

## 兼容说明

当前发布基线如下：

- 技术路线：仅支持 `Qt Widgets`
- Qt 版本：`Qt 5.14.2`
- 语言标准：`C++17`
- 构建系统：`CMake 3.16+` 与 `qmake 3.1`
- 安装导出：以 `CMake find_package(AverraWidgets CONFIG REQUIRED)` 为主入口

`1.0.0` 起对外承诺稳定的核心接口：

- `AverraThemePalette`
- `AverraThemeManager`
- `AverraIcon`
- `AverraFieldDefinition`
- `AverraFormModel`
- `AverraValidationResult`
- `AverraPropertyGrid`
- `AverraInspectorPanel`
- `AverraTreeTable`

下面这类内容仍应视为内部实现细节，不建议外部工程直接依赖：

- `src/` 下的内部辅助类
- `AverraStyleHelper`
- 组件内部对象名、样式细节和未在公开头文件中承诺的动态属性

## 最小接入示例

### CMake

```cmake
find_package(AverraWidgets CONFIG REQUIRED)

target_link_libraries(your_app
    PRIVATE
        Averra::Widgets
)
```

### qmake

```qmake
include(/path/to/averra-widgets/AverraWidgets.pri)
```

### 字段模型最小示例

```cpp
AverraFormModel *model = new AverraFormModel(this);
QList<AverraFieldDefinition> fields;

AverraFieldDefinition nameField = AverraFieldDefinition::createText(
    QStringLiteral("name"),
    QStringLiteral("节点名称"));
nameField.addValidationRule(AverraValidationRule::required());
fields.append(nameField);

model->setFields(fields);
model->loadObject(QStringLiteral("scene-indexer"),
                  QVariantMap{{QStringLiteral("name"), QStringLiteral("Scene Indexer")}});

AverraInspectorPanel *panel = new AverraInspectorPanel;
panel->setFormModel(model);
```

## 发布检查清单

- 确认版本号、`AverraVersion.h` 和发布说明一致
- 确认 `CMake` 与 `qmake` 的头文件和源文件清单同步
- 确认安装导出、`find_package` 和 `AverraWidgets.pri` 入口可用
- 确认 README 中的最小示例与当前公开 API 一致
- 确认 Gallery 至少包含一个完整工作流示例，而不只是散控件展示
- 确认字段模型、校验、双向绑定、提交/回滚、树表上下文和 `QSettings` 恢复测试都已更新
- 确认没有把内部辅助类型误暴露成稳定承诺接口
- 确认本次变更的兼容性结论已经记录在发布说明中

## 实际打包命令

当前仓库已经提供实际可执行的打包脚本：

```bash
./scripts/package_release.sh
```

默认行为：

- 使用 `CMake Release` 构建并运行测试
- 执行 `cmake --install` 生成安装树
- 通过 `CPack` 产出二进制包和源码包
- 使用 `qmake release` 构建库，并额外产出一个 `qmake SDK` 归档
- 自动解压并验证最小 `CMake` / `qmake` 消费者工程
- 在 `dist/` 下生成 `SHA256SUMS.txt`

常用选项：

```bash
./scripts/package_release.sh --skip-tests
./scripts/package_release.sh --skip-consumer-verify
./scripts/package_release.sh --cmake-only
./scripts/package_release.sh --qmake-only
./scripts/package_release.sh --dist-dir /custom/output
```

## 版本发布记录流程

仓库现在维护统一版本记录文件：

```text
CHANGELOG.md
```

准备一个正式版本时，先执行：

```bash
./scripts/prepare_release_record.sh --version 1.0.0 --date 2026-03-27
```

如果已经准备好了发布说明正文，也可以直接注入：

```bash
./scripts/prepare_release_record.sh --version 0.2.0 --notes-file /path/to/release-notes.md
```

这个脚本会同步更新以下版本元数据：

- `include/Averra/AverraVersion.h`
- `CMakeLists.txt`
- `src/AverraWidgets.pro`
- `CHANGELOG.md`

与控件选型相关的说明文档位于：

- `docs/SCENARIO_GUIDE.md`

## 最小消费者工程验证

仓库现在提供独立验证脚本：

```bash
./scripts/verify_release_consumers.sh
```

它会自动：

- 解压 `CPack` 二进制包
- 解压 `qmake SDK` 归档
- 生成最小 `CMake` 消费者工程，验证 `find_package(AverraWidgets CONFIG REQUIRED)`
- 生成最小 `qmake` 消费者工程，验证 `include(AverraWidgets.pri)`
- 编译并以 `offscreen` 方式实际运行这两个最小程序

## 产物结构

默认输出目录：

```text
dist/
```

典型产物包括：

- `dist/cpack/AverraWidgets-<version>-<system>-<arch>.tar.gz`
- `dist/cpack/AverraWidgets-<version>-<system>-<arch>.zip`
- `dist/cpack/AverraWidgets-<version>-src.tar.gz`
- `dist/cpack/AverraWidgets-<version>-src.zip`
- `dist/AverraWidgets-<version>-qmake-sdk.tar.gz`
- `dist/AverraWidgets-<version>-qmake-sdk.zip`
- `dist/SHA256SUMS.txt`

其中：

- `CPack` 包来自标准 `cmake --install` 安装树，适合 `find_package(AverraWidgets CONFIG REQUIRED)`
- `qmake SDK` 包包含 `include/`、`lib/`、`AverraWidgets.pri` 和发布文档，适合传统 `qmake` 工程直接解压接入

## 建议发布顺序

1. 更新版本号和 CHANGELOG/README/ROADMAP
2. 执行 `./scripts/prepare_release_record.sh --version <x.y.z>`
3. 运行构建与测试，确认 `CMake` 和 `qmake` 两条主流程都通过
4. 检查 Gallery 工作流示例是否能完整跑通
5. 执行 `./scripts/package_release.sh`
6. 如需单独复检，执行 `./scripts/verify_release_consumers.sh`
7. 核对 `dist/` 下的 `SHA256SUMS.txt`
8. 发布制品并附带兼容说明与回滚策略
