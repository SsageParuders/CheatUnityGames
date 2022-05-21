#!/bin/sh

# Created by GitHub@SsageParuders

# ==---------------==作用==---------------==
# 将 outputs/libs 目录下的编译产物
# 输出到设备的 /sdcard/outputs 目录下
# ==---------------==作用==---------------==

echo "\n\033[44;37mStart push build ...\033[0m"

# 判断是否存在目标文件夹, 不存在则创建, 存在则清空上一次推送产物, 进行本次推送
FILENAME_RESULT=$(adb shell ls /sdcard/ | tr -d '\015'|grep 'outputs')
echo "\033[44;37mStart looking for outputs ...\033[0m"
if [ -z "$FILENAME_RESULT" ]; then # 没找到 /sdcard/outputs
    echo "\033[44;37mNo outputs found\033[0m"
    echo "\033[44;37mStart mkdir outputs ...\033[0m"
    # 创建 /sdcard/outputs 目录
    adb shell mkdir /sdcard/outputs
    if [ $? -ne 0 ]; then
        echo "\n\033[41;37mError !! Mkdir outputs failed !!\033[0m\n"
    else
        echo "\033[42;35mMkdir outputs SUCCEED !!\033[0m"
    fi
else # 找到了 /sdcard/outputs
    echo "\033[42;35mFind outputs SUCCEED !!\033[0m"
    echo "\033[44;37mStart clean outputs On Android\033[0m"
    # 清空上一次推送产物
    adb shell find /data/local/tmp/libs -type f -delete
    if [ $? -ne 0 ]; then
        echo "\n\033[41;37mError !! Clean outputs On Android failed !!\033[0m\n"
    else
        echo "\033[42;35mClean outputs On Android SUCCEED !!\033[0m"
    fi
fi

# 推送最终生成产物 到设备的 /sdcard/outputs 目录下
adb push outputs/libs /data/local/tmp/

if [ $? -ne 0 ]; then
    echo "\033[41;37mError !! Push failed !!\033[0m\n"
else
    echo "\033[42;35mFinish push SUCCEED !!\033[0m\n"
fi