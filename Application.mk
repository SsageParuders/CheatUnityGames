APP_ABI := armeabi-v7a # arm64-v8a # x86 目标abi 也就是架构了
APP_PLATFORM := android-21 # 最低Android API标准
APP_STL := c++_static # 用于此应用的 C++ 标准库 参考: https://developer.android.com/ndk/guides/application_mk#app_stl
APP_OPTIM := release # 发行版
# 设置编译参数 下面分别解释这些参数
# -	支持 c++14 的特性
# -	禁用运行时类型信息
# -	禁用异常机制
# --------------
## 禁用这两个参数
## 一般只有对程序运行效率及资源占用比较看重的场合才会使用
## 禁用后可能带来一些不兼容问题
## 我这里是游戏相关 对运行效率和资源占用有所追求
# --------------
# -	关闭调试模式 用于取消 Releases 版本里的assert指令
# -	编译时 隐藏symbols
# -	不禁止长字节类型到短字节类型的转换
# -	允许declspec作为关键字
APP_CPPFLAGS := -std=c++17 -fno-rtti -fno-exceptions -DNDEBUG -fvisibility=hidden -Wno-narrowing -fdeclspec -pthread -w -s -fexceptions -Wall -O3

# 关于 Android.mk 和 Application.mk 的更多资料
# 请参阅 Google 官网: https://developer.android.com/ndk/guides/ndk-build