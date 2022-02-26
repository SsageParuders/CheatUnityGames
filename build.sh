#!/bin/sh

# Created by GitHub@SsageParuders

# ==---------------==作用==---------------==
# 编译源文件 输出到 outputs/libs 目录下
# ==---------------==作用==---------------==

echo "\n\033[44;37mStart Android Native Build ...\033[0m"

ndk-build -B \
-C outputs \
NDK_PROJECT_PATH=. \
NDK_APPLICATION_MK=../Application.mk \
APP_BUILD_SCRIPT=../Android.mk

# 判断上一条shell是否执行成功
if [ $? -ne 0 ]; then
    echo "\033[41;37mError !! Build failed !!\033[0m\n"
else
    echo "\033[42;35mFinish Android Native Build SUCCEED !!\033[0m\n"
fi

# 注意事项:
#   每个编译参数后面的 \ 为继续符 表示本行与下一行连接起来
#   后面不可添加任何东西 所以该shell脚本的注释写在最底部

# 以下正式开始注释说明:
# -B 强制重新编译已经编译完成的代码
# -C 到指定目录编译native代码
# NDK_PROJECT_PATH 指定NDK编译的代码路径为当前目录, 如果不配置, 则必须把工程代码放到Android工程的jni目录下
# NDK_APP_APPLICATION_MK 指定NDK编译使用的application.mk文件
# APP_BUILD_SCRIPT 指定NDK编译使用的Android.mk文件
# ../ 意思是上级目录 因为我们指定到 outputs 目录进行编译 Android.mk 和 Application.mk 相对于 outputs 为上级目录

# 结果:
# 最终在 outputs 目录下
# libs 目录下为最终产物
# obj 目录下为中间文件