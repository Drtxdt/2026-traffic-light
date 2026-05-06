#!/bin/bash

# ============================================================================
# Mars SDK Linux Build Script (Auto-Setup Version)
# ============================================================================

# ============================================================================
# 【开发者配置】工具链安装目录，可根据需要修改
# 工具链会安装到此目录，重新下载 SDK 后无需重复下载工具链
# 默认安装到用户主目录下的 mars-toolchain 文件夹
# ============================================================================
TOOLCHAIN_ROOT="$HOME/mars-toolchain"

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# 工具链目录（直接使用 TOOLCHAIN_ROOT）
TOOLCHAIN_DIR="$TOOLCHAIN_ROOT"

# ============================================================================
# 工具下载地址配置（可根据需要修改）
# ============================================================================
# RISC-V GCC 工具链 (Nuclei) - 解压后直接是 gcc/ 目录
GCC_URL="https://download.nucleisys.com/upload/files/toolchain/gcc/nuclei_riscv_newlibc_prebuilt_linux64_nuclei-2024.tar.bz2"
GCC_ARCHIVE="nuclei_riscv_newlibc_prebuilt_linux64_nuclei-2024.tar.bz2"

# CMake (如果系统没有安装)
CMAKE_NAME="cmake-3.28.0-linux-x86_64"
CMAKE_URL="https://listenai-firmware-delivery.oss-cn-beijing.aliyuncs.com/Mars/toolchain/cmake-3.28.0-linux-x86_64.tar.gz"

# Ninja (如果系统没有安装)
NINJA_NAME="ninja"
NINJA_URL="https://listenai-firmware-delivery.oss-cn-beijing.aliyuncs.com/Mars/toolchain/ninja-linux.zip"

# ============================================================================
# 设置环境变量
# ============================================================================
# GCC 工具链解压后直接是 gcc/ 目录，所以 GCC_DIR 就是 TOOLCHAIN_DIR
GCC_DIR="$TOOLCHAIN_DIR"
CMAKE_DIR="$TOOLCHAIN_DIR/$CMAKE_NAME"
NINJA_DIR="$TOOLCHAIN_DIR/$NINJA_NAME"

# 添加到 PATH
export PATH="$GCC_DIR/gcc/bin:$CMAKE_DIR/bin:$NINJA_DIR:$PATH"

# ============================================================================
# 默认参数
# ============================================================================
target=
verbose=
symstat=
genmake=Ninja
buildir=build
remove=0
gencmd=0
setup_only=0

# Kconfig 配置
export KCONFIG_CONFIG=.config
export KCONFIG_HEADER=include/autoconf.h

# ============================================================================
# 函数：显示帮助
# ============================================================================
function usage()
{
    echo ""
    echo "============================================================"
    echo "  Mars SDK Build Script - Auto Setup Version [Linux]"
    echo "============================================================"
    echo ""
    echo "Usage: $0 [options ...] [targets ...]"
    echo ""
    echo "Options:"
    echo "    -h, --help           Show this help message"
    echo "    --setup              Only download and setup toolchain, don't build"
    echo "    -v, --verbose        Enable verbose output when building"
    echo "    -r, --remove         Remove build directory before building"
    echo "    -R, --Remove         Remove build directory and config files"
    echo "    -G, --Genmake=[GEN]  Generator: 'Ninja'(*), empty for 'Unix Makefile'"
    echo "    -b, --buildir=<DIR>  Config building directory"
    echo "    -s, --symstat=<CFG>  Symbol static statistics"
    echo ""
    echo "Targets:"
    echo "    all                  Build all (default)"
    echo "    clean                Clean build files"
    echo "    help                 Show available CMake targets"
    echo "    menuconfig           Run menu configuration"
    echo "    defconfig            Generate default configuration"
    echo "    respack              Do resource packing"
    echo ""
    echo "Examples:"
    echo "    $0                   Auto-setup and build"
    echo "    $0 --setup           Only setup toolchain (no build)"
    echo "    $0 -r all            Clean and rebuild"
    echo "    $0 -R all            Full clean and rebuild"
    echo ""
    echo "Toolchain Directory: $TOOLCHAIN_DIR"
    echo ""
}

# ============================================================================
# 函数：下载并解压文件
# ============================================================================
function download_and_extract()
{
    local url="$1"
    local filename="$2"
    local dest_dir="$3"
    local filepath="$TOOLCHAIN_DIR/$filename"

    echo "        URL: $url"
    echo "        Downloading..."

    # 使用 curl 或 wget 下载
    if command -v curl &> /dev/null; then
        curl -L -o "$filepath" "$url" --progress-bar
    elif command -v wget &> /dev/null; then
        wget -O "$filepath" "$url" --show-progress
    else
        echo "[ERROR] Neither curl nor wget is available!"
        return 1
    fi

    if [ $? -ne 0 ]; then
        echo "[ERROR] Download failed!"
        return 1
    fi

    echo "        Extracting to $dest_dir..."

    # 根据文件类型解压
    case "$filename" in
        *.tar.bz2)
            tar -xjf "$filepath" -C "$dest_dir"
            ;;
        *.tar.gz|*.tgz)
            tar -xzf "$filepath" -C "$dest_dir"
            ;;
        *.zip)
            unzip -q "$filepath" -d "$dest_dir"
            ;;
        *)
            echo "[ERROR] Unknown archive format: $filename"
            return 1
            ;;
    esac

    if [ $? -ne 0 ]; then
        echo "[ERROR] Extract failed!"
        return 1
    fi

    # 删除下载的压缩文件
    rm -f "$filepath"
    echo "        Done!"
    return 0
}

# ============================================================================
# 函数：检查并下载所需工具
# ============================================================================
function check_and_download_tools()
{
    echo "[CHECK] Checking required tools..."
    echo ""

    # 检查 GCC 工具链（解压后直接是 gcc/ 目录）
    if [ ! -f "$GCC_DIR/gcc/bin/riscv64-unknown-elf-gcc" ]; then
        echo "[DOWNLOAD] RISC-V GCC Toolchain not found, downloading..."
        download_and_extract "$GCC_URL" "$GCC_ARCHIVE" "$TOOLCHAIN_DIR"
        if [ $? -ne 0 ]; then return 1; fi
    else
        echo "[OK] RISC-V GCC Toolchain"
    fi

    # 检查 CMake（优先使用系统安装的）
    # if command -v cmake &> /dev/null; then
    #     echo "[OK] CMake (system)"
    # el
    if [ -f "$CMAKE_DIR/bin/cmake" ]; then
        echo "[OK] CMake (toolchain)"
    else
        echo "[DOWNLOAD] CMake not found, downloading..."
        download_and_extract "$CMAKE_URL" "${CMAKE_NAME}.tar.gz" "$TOOLCHAIN_DIR"
        if [ $? -ne 0 ]; then return 1; fi
    fi

    # 检查 Ninja（优先使用系统安装的）
    # if command -v ninja &> /dev/null; then
    #     echo "[OK] Ninja (system)"
    # el
    if [ -f "$NINJA_DIR/ninja" ]; then
        echo "[OK] Ninja (toolchain)"
    else
        echo "[DOWNLOAD] Ninja not found, downloading..."
        mkdir -p "$NINJA_DIR"
        download_and_extract "$NINJA_URL" "ninja-linux.zip" "$NINJA_DIR"
        if [ $? -ne 0 ]; then return 1; fi
        chmod +x "$NINJA_DIR/ninja"
    fi

    # 检查 Python3
    if command -v python3 &> /dev/null; then
        echo "[OK] Python3 (system)"
    else
        echo "[ERROR] Python3 is not installed! Please install it first."
        echo "        Ubuntu/Debian: sudo apt install python3 python3-pip"
        echo "        CentOS/RHEL:   sudo yum install python3 python3-pip"
        return 1
    fi

    # 检查 pyelftools
    python3 -c "import elftools" 2>/dev/null
    if [ $? -ne 0 ]; then
        echo "[INSTALL] Installing pyelftools..."
        pip3 install pyelftools -q 2>/dev/null || pip install pyelftools -q
    else
        echo "[OK] pyelftools"
    fi

    # 检查 kconfiglib
    python3 -c "import kconfiglib" 2>/dev/null
    if [ $? -ne 0 ]; then
        echo "[INSTALL] Installing kconfiglib..."
        pip3 install kconfiglib -q 2>/dev/null || pip install kconfiglib -q
    else
        echo "[OK] kconfiglib"
    fi

    echo ""
    echo "[CHECK] All tools are ready!"
    return 0
}

# ============================================================================
# 函数：解析命令行参数
# ============================================================================
function argproc()
{
    base=$(basename $0)
    eval set -- $(getopt -n $0 -o hvrRG::b:s: -l help,verbose,remove,Remove,Genmake::,buildir:,symstat:,setup -- "$@")
    while [ -n "$1" ]; do
        case "$1" in
            -h|--help    ) usage $base;            exit 0 ;;
            --setup      ) setup_only=1                   ;;
            -v|--verbose ) verbose="-v"                   ;;
            -r|--remove  ) remove=1                       ;;
            -R|--Remove  ) remove=2                       ;;
            -G|--Genmake ) gencmd=1; genmake="$2"; shift  ;;
            -b|--buildir ) buildir="$2";           shift  ;;
            -s|--symstat ) symstat="$2";           shift  ;;
            --           )                  shift; break  ;;
            *            ) echo E=$1;              exit 1 ;;
        esac
        shift
    done
    target=$@
}

# ============================================================================
# 主程序
# ============================================================================

# 解析参数
if [[ -z "$@" ]]; then
    target=all
else
    argproc "$@"
fi

# ============================================================================
# Step 1: 检查并创建 toolchain 目录
# ============================================================================
echo ""
echo "============================================================"
echo "  Mars SDK Build System - Checking Environment [Linux]"
echo "============================================================"
echo ""

if [ ! -d "$TOOLCHAIN_DIR" ]; then
    echo "[INFO] Creating toolchain directory..."
    mkdir -p "$TOOLCHAIN_DIR"
fi

# ============================================================================
# Step 2: 检查并下载所需工具
# ============================================================================
check_and_download_tools
if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to setup toolchain!"
    exit 1
fi

# 如果只是 setup 模式，到此结束
if [ $setup_only -eq 1 ]; then
    echo ""
    echo "[INFO] Setup completed successfully!"
    exit 0
fi

# ============================================================================
# Step 3: 执行构建
# ============================================================================

# 删除构建目录
if [[ $remove -gt 0 ]]; then
    echo "[BUILD] Removing [$buildir] ..."
    rm -rf $buildir
    if [[ $remove -gt 1 ]]; then
        echo "[BUILD] Removing [$KCONFIG_CONFIG $KCONFIG_HEADER] ..."
        rm -f $KCONFIG_CONFIG* $KCONFIG_HEADER
    fi
fi

# 生成构建系统
if [[ ! -d $buildir ]] && ( [[ ! -z "$target" ]] || [[ $gencmd -eq 1 ]] ); then
    if [[ ! -e $KCONFIG_CONFIG ]]; then
        echo "[BUILD] Generating defconfig..."
        python3 tools/kconfpy/kconfig.py -KKconfig -Dboards/defconfig -H$KCONFIG_HEADER -C$KCONFIG_CONFIG
    fi

    if [[ ! -z "$genmake" ]]; then
        genmake_opt="-G$genmake"
    fi
    echo "[BUILD] Running CMake [$genmake_opt]..."
    cmake $genmake_opt -DPython3_EXECUTABLE="$(which python3)" -B $buildir
fi

# 执行构建
if [[ ! -z "$target" ]]; then
    echo "[BUILD] Building [$target] ..."
    cmake --build $buildir $verbose --parallel --target $target
fi

# 符号统计
if [[ ! -z $symstat ]]; then
    python3 tools/binutils/symstat.py $symstat
fi

# 检查构建结果
echo ""
if [ -f "build/bin/app.bin" ]; then
    echo "========================================"
    echo "  Mars SDK Build SUCCESS!"
    echo "========================================"
else
    echo "========================================"
    echo "  Mars SDK Build FAILED!"
    echo "========================================"
fi