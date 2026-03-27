#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CHANGELOG_FILE="${ROOT_DIR}/CHANGELOG.md"
VERSION=""
RELEASE_DATE="$(date +%F)"
NOTES_FILE=""

usage() {
    cat <<'EOF'
用法：
  scripts/prepare_release_record.sh --version X.Y.Z [选项]

选项：
  --version X.Y.Z    目标版本号，必填
  --date YYYY-MM-DD  发布日期，默认使用今天
  --notes-file FILE  使用指定文件作为 CHANGELOG 条目正文
  -h, --help         显示帮助
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --version)
            shift
            VERSION="${1:-}"
            ;;
        --date)
            shift
            RELEASE_DATE="${1:-}"
            ;;
        --notes-file)
            shift
            NOTES_FILE="${1:-}"
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "未知参数：$1" >&2
            usage >&2
            exit 1
            ;;
    esac
    shift
done

if [[ -z "${VERSION}" ]]; then
    echo "必须通过 --version 指定版本号。" >&2
    exit 1
fi

if [[ ! "${VERSION}" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    echo "版本号格式无效：${VERSION}" >&2
    exit 1
fi

if [[ -n "${NOTES_FILE}" && ! -f "${NOTES_FILE}" ]]; then
    echo "notes 文件不存在：${NOTES_FILE}" >&2
    exit 1
fi

IFS='.' read -r VERSION_MAJOR VERSION_MINOR VERSION_PATCH <<< "${VERSION}"

perl -0pi -e "s/project\\(AverraWidgets VERSION [0-9]+\\.[0-9]+\\.[0-9]+ LANGUAGES CXX\\)/project(AverraWidgets VERSION ${VERSION} LANGUAGES CXX)/" \
    "${ROOT_DIR}/CMakeLists.txt"
perl -0pi -e "s/^VERSION = .*\$/VERSION = ${VERSION}/m" \
    "${ROOT_DIR}/src/AverraWidgets.pro"
perl -0pi -e "s/^#define AVERRA_VERSION_MAJOR .*\$/#define AVERRA_VERSION_MAJOR ${VERSION_MAJOR}/m; s/^#define AVERRA_VERSION_MINOR .*\$/#define AVERRA_VERSION_MINOR ${VERSION_MINOR}/m; s/^#define AVERRA_VERSION_PATCH .*\$/#define AVERRA_VERSION_PATCH ${VERSION_PATCH}/m; s/^#define AVERRA_VERSION_STR \".*\"\$/#define AVERRA_VERSION_STR \"${VERSION}\"/m" \
    "${ROOT_DIR}/include/Averra/AverraVersion.h"

if [[ -n "${NOTES_FILE}" ]]; then
    ENTRY_BODY="$(cat "${NOTES_FILE}")"
else
    ENTRY_BODY="$(cat <<'EOF'
### Added

- 填写本版本新增能力

### Changed

- 填写本版本行为或接口变更

### Fixed

- 填写本版本问题修复

### Packaging

- 填写本版本发布包或接入流程变化
EOF
)"
fi

ENTRY_HEADER="## [${VERSION}] - ${RELEASE_DATE}"

if [[ ! -f "${CHANGELOG_FILE}" ]]; then
    cat > "${CHANGELOG_FILE}" <<EOF
# Changelog

本文件记录 \`Averra Widgets\` 的公开版本发布内容，便于发布、回滚和接入方核对差异。

${ENTRY_HEADER}

${ENTRY_BODY}
EOF
    echo "已创建 CHANGELOG：${CHANGELOG_FILE}"
    exit 0
fi

if grep -Fq "${ENTRY_HEADER}" "${CHANGELOG_FILE}"; then
    echo "CHANGELOG 已包含版本记录：${ENTRY_HEADER}"
    exit 0
fi

TEMP_FILE="$(mktemp)"
{
    if grep -Fq "# Changelog" "${CHANGELOG_FILE}"; then
        sed -n '1,3p' "${CHANGELOG_FILE}"
        echo
        echo "${ENTRY_HEADER}"
        echo
        printf '%s\n' "${ENTRY_BODY}"
        echo
        tail -n +4 "${CHANGELOG_FILE}"
    else
        echo "# Changelog"
        echo
        echo "本文件记录 \`Averra Widgets\` 的公开版本发布内容，便于发布、回滚和接入方核对差异。"
        echo
        echo "${ENTRY_HEADER}"
        echo
        printf '%s\n' "${ENTRY_BODY}"
        echo
        cat "${CHANGELOG_FILE}"
    fi
} > "${TEMP_FILE}"

mv "${TEMP_FILE}" "${CHANGELOG_FILE}"

echo "已更新版本号和 CHANGELOG：${VERSION} (${RELEASE_DATE})"
