#!/bin/sh

# Created by GitHub@SsageParuders

# ==---------------==作用==---------------==
# 清除 outputs 目录下的编译产物
# ==---------------==作用==---------------==

echo "\n\033[44;37mStart Android Native Clean ...\033[0m"

ndk-build clean \
-C outputs \
NDK_PROJECT_PATH=. \
NDK_APPLICATION_MK=../Application.mk \
APP_BUILD_SCRIPT=../Android.mk

if [ $? -ne 0 ]; then
    echo "\033[41;37mError !! Clean failed !!\033[0m\n"
else
    echo "\033[42;35mFinish Android Native Clean SUCCEED !!\033[0m\n"
    clear
fi

# 详细解释 请看 build.sh
# ndk-build clean 清除所有编译出来的临时文件和目标文件