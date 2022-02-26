#!/bin/sh

# Created by GitHub@SsageParuders

# ==---------------==作用==---------------==
# 进行 adb logcat 调试
# ==---------------==作用==---------------==

adb logcat -c "TORAMCHEAT"
if [ $? -ne 0 ]; then
    echo "\033[41;37mError !! Clean Log failed !!\033[0m\n"
else
    echo "\033[42;35mFinish Clean Log SUCCEED !!\033[0m\n"
fi
echo "\n\033[44;37mStart ADB logcat ...\033[0m"
adb logcat -s "TORAMCHEAT"

# 用来调试