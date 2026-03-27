#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DIST_DIR="${ROOT_DIR}/dist"
CMAKE_BUILD_DIR="${ROOT_DIR}/build/package-release"
QMAKE_BUILD_DIR="${ROOT_DIR}/build/package-qmake"
RUN_TESTS=1
PACKAGE_CMAKE=1
PACKAGE_QMAKE=1
VERIFY_CONSUMERS=1

usage() {
    cat <<'EOF'
用法：
  scripts/package_release.sh [选项]

选项：
  --skip-tests    打包前不运行测试
  --skip-consumer-verify  打包后不运行最小消费者工程验证
  --cmake-only    仅生成 CMake / CPack 包
  --qmake-only    仅生成 qmake SDK 归档
  --dist-dir DIR  指定输出目录
  -h, --help      显示帮助
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --skip-tests)
            RUN_TESTS=0
            ;;
        --skip-consumer-verify)
            VERIFY_CONSUMERS=0
            ;;
        --cmake-only)
            PACKAGE_CMAKE=1
            PACKAGE_QMAKE=0
            ;;
        --qmake-only)
            PACKAGE_CMAKE=0
            PACKAGE_QMAKE=1
            ;;
        --dist-dir)
            shift
            DIST_DIR="$1"
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

if [[ ${PACKAGE_CMAKE} -eq 0 && ${PACKAGE_QMAKE} -eq 0 ]]; then
    echo "至少需要启用一种打包方式。" >&2
    exit 1
fi

mkdir -p "${DIST_DIR}"

VERSION="$(
    sed -n 's/^#define AVERRA_VERSION_STR "\(.*\)"$/\1/p' \
        "${ROOT_DIR}/include/Averra/AverraVersion.h"
)"

if [[ -z "${VERSION}" ]]; then
    echo "无法从 AverraVersion.h 读取版本号。" >&2
    exit 1
fi

QMAKE_BIN="${QMAKE_BIN:-$(command -v qmake || true)}"

if [[ ${PACKAGE_QMAKE} -eq 1 && -z "${QMAKE_BIN}" ]]; then
    echo "未找到 qmake，可设置环境变量 QMAKE_BIN 指定 qmake 路径。" >&2
    exit 1
fi

echo "Averra Widgets 打包开始"
echo "版本：${VERSION}"
echo "输出目录：${DIST_DIR}"

package_cmake() {
    local install_root="${DIST_DIR}/staging/cmake/AverraWidgets-${VERSION}"
    local cpack_output_dir="${DIST_DIR}/cpack"

    echo
    echo "[1/2] 生成 CMake / CPack 包"

    cmake -S "${ROOT_DIR}" -B "${CMAKE_BUILD_DIR}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DAVERRA_BUILD_EXAMPLES=ON \
        -DAVERRA_BUILD_TESTS=ON \
        -DAVERRA_ENABLE_CPACK=ON

    cmake --build "${CMAKE_BUILD_DIR}" -j16

    if [[ ${RUN_TESTS} -eq 1 ]]; then
        ctest --test-dir "${CMAKE_BUILD_DIR}" --output-on-failure
    fi

    rm -rf "${install_root}"
    cmake --install "${CMAKE_BUILD_DIR}" --prefix "${install_root}"

    mkdir -p "${cpack_output_dir}"
    cpack --config "${CMAKE_BUILD_DIR}/CPackConfig.cmake" -G TGZ -B "${cpack_output_dir}"
    cpack --config "${CMAKE_BUILD_DIR}/CPackConfig.cmake" -G ZIP -B "${cpack_output_dir}"
    cpack --config "${CMAKE_BUILD_DIR}/CPackSourceConfig.cmake" -G TGZ -B "${cpack_output_dir}"
    cpack --config "${CMAKE_BUILD_DIR}/CPackSourceConfig.cmake" -G ZIP -B "${cpack_output_dir}"
}

package_qmake() {
    local sdk_root="${DIST_DIR}/staging/qmake/AverraWidgets-${VERSION}"
    local sdk_parent
    sdk_parent="$(dirname "${sdk_root}")"
    local archive_tgz="${DIST_DIR}/AverraWidgets-${VERSION}-qmake-sdk.tar.gz"
    local archive_zip="${DIST_DIR}/AverraWidgets-${VERSION}-qmake-sdk.zip"

    echo
    echo "[2/2] 生成 qmake SDK 归档"

    mkdir -p "${QMAKE_BUILD_DIR}"

    (
        cd "${QMAKE_BUILD_DIR}"
        "${QMAKE_BIN}" "${ROOT_DIR}/src/AverraWidgets.pro" CONFIG+=release
        make -j16
    )

    rm -rf "${sdk_root}"
    mkdir -p "${sdk_root}/include" "${sdk_root}/lib" "${sdk_root}/docs"

    cmake -E copy_directory "${ROOT_DIR}/include/Averra" "${sdk_root}/include/Averra"
    cmake -E copy_if_different "${ROOT_DIR}/AverraWidgets.pri" "${sdk_root}/AverraWidgets.pri"
    cmake -E copy_if_different "${ROOT_DIR}/CHANGELOG.md" "${sdk_root}/docs/CHANGELOG.md"
    cmake -E copy_if_different "${ROOT_DIR}/README.md" "${sdk_root}/docs/README.md"
    cmake -E copy_if_different "${ROOT_DIR}/docs/SCENARIO_GUIDE.md" "${sdk_root}/docs/SCENARIO_GUIDE.md"
    cmake -E copy_if_different "${ROOT_DIR}/docs/ROADMAP.md" "${sdk_root}/docs/ROADMAP.md"
    cmake -E copy_if_different "${ROOT_DIR}/docs/RELEASE.md" "${sdk_root}/docs/RELEASE.md"
    cmake -E copy_if_different "${ROOT_DIR}/docs/style-profile.example.json" "${sdk_root}/docs/style-profile.example.json"

    find "${ROOT_DIR}/build/qmake/lib" -maxdepth 1 -type f \
        \( -name 'libAverraWidgets.so*' -o -name 'AverraWidgets.dll' -o -name 'AverraWidgets.lib' -o -name 'libAverraWidgets.dylib' \) \
        -exec cp -a {} "${sdk_root}/lib/" \;

    find "${ROOT_DIR}/build/qmake/lib" -maxdepth 1 -type l -name 'libAverraWidgets.so*' \
        -exec cp -a {} "${sdk_root}/lib/" \;

    (
        cd "${sdk_parent}"
        rm -f "${archive_tgz}" "${archive_zip}"
        cmake -E tar czf "${archive_tgz}" "AverraWidgets-${VERSION}"
        cmake -E tar cf "${archive_zip}" --format=zip "AverraWidgets-${VERSION}"
    )
}

if [[ ${PACKAGE_CMAKE} -eq 1 ]]; then
    package_cmake
fi

if [[ ${PACKAGE_QMAKE} -eq 1 ]]; then
    package_qmake
fi

if [[ ${VERIFY_CONSUMERS} -eq 1 ]]; then
    echo
    echo "[3/3] 验证最小消费者工程"

    VERIFY_ARGS=(--dist-dir "${DIST_DIR}")
    if [[ ${PACKAGE_CMAKE} -eq 1 && ${PACKAGE_QMAKE} -eq 0 ]]; then
        VERIFY_ARGS+=(--cmake-only)
    elif [[ ${PACKAGE_CMAKE} -eq 0 && ${PACKAGE_QMAKE} -eq 1 ]]; then
        VERIFY_ARGS+=(--qmake-only)
    fi

    "${ROOT_DIR}/scripts/verify_release_consumers.sh" "${VERIFY_ARGS[@]}"
fi

(
    cd "${DIST_DIR}"
    find . -maxdepth 3 -type f \
        \( -name '*.tar.gz' -o -name '*.zip' \) \
        -print0 | xargs -0 -r sha256sum > SHA256SUMS.txt
)

echo
echo "打包完成，制品位于：${DIST_DIR}"
