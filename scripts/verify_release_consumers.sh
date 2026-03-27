#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DIST_DIR="${ROOT_DIR}/dist"
WORK_DIR="${ROOT_DIR}/build/verify-release-consumers"
VERIFY_CMAKE=1
VERIFY_QMAKE=1
KEEP_WORK_DIR=0

usage() {
    cat <<'EOF'
用法：
  scripts/verify_release_consumers.sh [选项]

选项：
  --dist-dir DIR      指定制品目录，默认 dist/
  --cmake-only        仅验证 CMake 消费者工程
  --qmake-only        仅验证 qmake 消费者工程
  --keep-workdir      保留验证过程中的临时目录
  -h, --help          显示帮助
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --dist-dir)
            shift
            DIST_DIR="${1:-}"
            ;;
        --cmake-only)
            VERIFY_CMAKE=1
            VERIFY_QMAKE=0
            ;;
        --qmake-only)
            VERIFY_CMAKE=0
            VERIFY_QMAKE=1
            ;;
        --keep-workdir)
            KEEP_WORK_DIR=1
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

if [[ ${VERIFY_CMAKE} -eq 0 && ${VERIFY_QMAKE} -eq 0 ]]; then
    echo "至少需要启用一种验证方式。" >&2
    exit 1
fi

VERSION="$(
    sed -n 's/^#define AVERRA_VERSION_STR "\(.*\)"$/\1/p' \
        "${ROOT_DIR}/include/Averra/AverraVersion.h"
)"

if [[ -z "${VERSION}" ]]; then
    echo "无法从 AverraVersion.h 读取版本号。" >&2
    exit 1
fi

QMAKE_BIN="${QMAKE_BIN:-$(command -v qmake || true)}"
if [[ ${VERIFY_QMAKE} -eq 1 && -z "${QMAKE_BIN}" ]]; then
    echo "未找到 qmake，可设置环境变量 QMAKE_BIN。" >&2
    exit 1
fi

QT_CMAKE_PREFIX="${QT_CMAKE_PREFIX:-}"
if [[ -z "${QT_CMAKE_PREFIX}" && -n "${QMAKE_BIN}" ]]; then
    QT_CMAKE_PREFIX="$("${QMAKE_BIN}" -query QT_INSTALL_PREFIX 2>/dev/null || true)"
fi

rm -rf "${WORK_DIR}"
mkdir -p "${WORK_DIR}/archives" "${WORK_DIR}/sources"

cleanup() {
    if [[ ${KEEP_WORK_DIR} -eq 0 ]]; then
        rm -rf "${WORK_DIR}"
    fi
}
trap cleanup EXIT

find_archive() {
    local pattern="$1"
    local archive

    archive="$(find "${DIST_DIR}" -maxdepth 3 -type f -name "${pattern}" | sort | head -n 1)"

    if [[ -z "${archive}" ]]; then
        echo "找不到制品：${pattern}" >&2
        exit 1
    fi

    printf '%s\n' "${archive}"
}

extract_archive() {
    local archive_path="$1"
    local output_dir="$2"

    rm -rf "${output_dir}"
    mkdir -p "${output_dir}"
    tar -xzf "${archive_path}" -C "${output_dir}"
    find "${output_dir}" -mindepth 1 -maxdepth 1 -type d | sort | head -n 1
}

write_cmake_consumer() {
    local source_dir="$1"

    mkdir -p "${source_dir}"

    cat > "${source_dir}/CMakeLists.txt" <<'EOF'
cmake_minimum_required(VERSION 3.16)
project(AverraConsumerCMake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(AverraWidgets CONFIG REQUIRED)

add_executable(AverraConsumerCMake main.cpp)
target_link_libraries(AverraConsumerCMake
    PRIVATE
        Averra::Widgets
)
EOF

    cat > "${source_dir}/main.cpp" <<'EOF'
#include <Averra/AverraButton.h>

#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AverraButton button(QStringLiteral("Consumer Smoke"));
    button.setAccent(true);
    button.resize(160, 42);
    button.show();

    QTimer::singleShot(0, &app, &QCoreApplication::quit);
    return app.exec();
}
EOF
}

write_qmake_consumer() {
    local source_dir="$1"
    local sdk_root="$2"

    mkdir -p "${source_dir}"

    cat > "${source_dir}/AverraConsumerQmake.pro" <<'EOF'
TEMPLATE = app
TARGET = AverraConsumerQmake

QT += widgets
CONFIG += c++17 console

AVERRA_SDK_DIR = __SDK_ROOT__
include($$AVERRA_SDK_DIR/AverraWidgets.pri)

SOURCES += $$PWD/main.cpp
EOF

    perl -0pi -e "s|__SDK_ROOT__|${sdk_root}|g" "${source_dir}/AverraConsumerQmake.pro"

    cat > "${source_dir}/main.cpp" <<'EOF'
#include <Averra/AverraButton.h>

#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AverraButton button(QStringLiteral("Consumer Smoke"));
    button.setAccent(true);
    button.resize(160, 42);
    button.show();

    QTimer::singleShot(0, &app, &QCoreApplication::quit);
    return app.exec();
}
EOF
}

verify_cmake_consumer() {
    local archive_path extracted_root source_dir build_dir

    archive_path="$(find "${DIST_DIR}/cpack" -maxdepth 1 -type f -name "AverraWidgets-${VERSION}-*.tar.gz" ! -name "*-src.tar.gz" | sort | head -n 1)"

    if [[ -z "${archive_path}" ]]; then
        echo "找不到 CMake 二进制包。" >&2
        exit 1
    fi

    extracted_root="$(extract_archive "${archive_path}" "${WORK_DIR}/archives/cmake")"
    source_dir="${WORK_DIR}/sources/cmake-consumer"
    build_dir="${WORK_DIR}/build-cmake-consumer"

    write_cmake_consumer "${source_dir}"

    local cmake_prefix_path="${extracted_root}"
    if [[ -n "${QT_CMAKE_PREFIX}" ]]; then
        cmake_prefix_path="${extracted_root};${QT_CMAKE_PREFIX}"
    fi

    cmake -S "${source_dir}" -B "${build_dir}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_PREFIX_PATH="${cmake_prefix_path}"
    cmake --build "${build_dir}" -j16
    if [[ -n "${QT_CMAKE_PREFIX}" ]]; then
        LD_LIBRARY_PATH="${QT_CMAKE_PREFIX}/lib:${LD_LIBRARY_PATH:-}" \
            QT_QPA_PLATFORM=offscreen \
            "${build_dir}/AverraConsumerCMake"
    else
        QT_QPA_PLATFORM=offscreen "${build_dir}/AverraConsumerCMake"
    fi
}

verify_qmake_consumer() {
    local archive_path extracted_root source_dir

    archive_path="$(find_archive "AverraWidgets-${VERSION}-qmake-sdk.tar.gz")"
    extracted_root="$(extract_archive "${archive_path}" "${WORK_DIR}/archives/qmake")"
    source_dir="${WORK_DIR}/sources/qmake-consumer"

    write_qmake_consumer "${source_dir}" "${extracted_root}"

    (
        cd "${source_dir}"
        "${QMAKE_BIN}" AverraConsumerQmake.pro
        make -j16
        if [[ -n "${QT_CMAKE_PREFIX}" ]]; then
            LD_LIBRARY_PATH="${QT_CMAKE_PREFIX}/lib:${LD_LIBRARY_PATH:-}" \
                QT_QPA_PLATFORM=offscreen \
                ./AverraConsumerQmake
        else
            QT_QPA_PLATFORM=offscreen ./AverraConsumerQmake
        fi
    )
}

echo "开始验证最小消费者工程"
echo "版本：${VERSION}"
echo "制品目录：${DIST_DIR}"

if [[ ${VERIFY_CMAKE} -eq 1 ]]; then
    echo
    echo "[1/2] 验证 CMake find_package 接入"
    verify_cmake_consumer
fi

if [[ ${VERIFY_QMAKE} -eq 1 ]]; then
    echo
    echo "[2/2] 验证 qmake AverraWidgets.pri 接入"
    verify_qmake_consumer
fi

echo
echo "最小消费者工程验证通过"
