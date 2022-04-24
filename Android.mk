LOCAL_PATH := $(call my-dir) # 定义 local path 只需一次即可

#################################################

### 以下这段用于预构建我们将要连接的已存在的静态库或动态库 ###

include $(CLEAR_VARS) # 声明 CLEAR_VARS 变量

LOCAL_MODULE  := dobby # 预构建库名称
LOCAL_SRC_FILES := libs/dobby/android/$(TARGET_ARCH_ABI)/libdobby.a # 动态/静态库位置
include $(PREBUILT_STATIC_LIBRARY) # 生成预构建库

# 这里要注意的是，对于一次预构建只能预构建动态库，要么是静态库，两者不能共存

### 预构建静态库或动态库完毕 ###

#################################################

### 以下内容用于描述编译当前工程的源代码 ###

include $(CLEAR_VARS) # 声明 CLEAR_VARS 变量

LOCAL_MODULE := UnityCheat # 设定模块名称

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/AndroidProp # 添加头文件库路径
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/AndroidLog
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Dobby
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/MapTools
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/MemoryTools
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Il2cppResolver

# 列举源文件
FILE_LIST := $(wildcard $(LOCAL_PATH)/src/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/utils/MemoryTools/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/utils/MapTools/*.c*)
FILE_LIST += $(wildcard $(LOCAL_PATH)/utils/AndroidProp/*.c*)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

# C++ 编译参数 ollvm-指令替换 -- 对每个函数混淆三次 默认一次
LOCAL_CPPFLAGS += -mllvm -sub -mllvm -sub_loop=2
# C++ 编译参数 ollvm-字符串混淆 -- 指定随机数种子生成器
LOCAL_CPPFLAGS += -mllvm -sobf -mllvm -aesSeed=0xada46ab5da824b96a18409c49dc91dc3
# C++ 编译参数 ollvm-控制流扁平化 -- 启用block切分 提升平展程度 -- 对每个block混淆三次 默认一次
LOCAL_CPPFLAGS += -mllvm -fla -mllvm -split -mllvm -split_num=2
# C++ 编译参数 ollvm-虚假控制流 -- 对一个函数混淆三次 默认一次 -- 代码块被混淆的概率是40% 默认30% | --> 这参数有毛病，遇到imgui就会卡很长时间 <--
# LOCAL_CPPFLAGS += -mllvm -bcf # -mllvm -bcf_loop=2 -mllvm -bcf_prob=40

LOCAL_LDLIBS := -lm -ldl -llog -landroid -lEGL -lGLESv2 # 链接系统库

## 也可以使用如下方法链接动态/静态库 ndk会报WARNING 但是没什么影响
# LOCAL_LDFLAGS := $(LOCAL_PATH)/libs/dobby/android/$(TARGET_ARCH_ABI)/libdobby.a
## 我强迫症 我用预构建 =.=

LOCAL_STATIC_LIBRARIES += dobby # 链接前面声明好的静态库

include $(BUILD_SHARED_LIBRARY) # 最终目标为: 构建动态库 so文件

### 当前工程的源代码编译完毕 ###