// System lib
#include <iostream>
#include <dlfcn.h>
#include <string>
#include <EGL/egl.h> // EGL
#include <GLES2/gl2.h>
#include <android_native_app_glue.h>

// user lib
#include <Logger.hpp> // Log
#include <AndroidProp.hpp> // Property
#include <MapTools.hpp> // Maps
#include <MemoryTools.hpp> // Memory
#include <ResolverInit.hpp> // Il2cppUtils
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_android.h>
#include <font/Font.h>
// #include <CydiaSubstrate.h> // Substrate

// Var
static unsigned long il2cppAddress = 0; // 存储il2cpp.so基地址
static bool isGuiInit = false; // 是否初始化过imgui
struct gameHackStruct { // ImGUI的CheckBox回调事件处理
    bool isEnableCheckBox1 = false; // 是否启用CheckBox1
    bool isEnableCheckBox2 = false; // 是否启用CheckBox2
    bool isEnableCheckBox3 = false; // 是否启用CheckBox3
    bool isEnableCheckBox4 = false; // 是否启用CheckBox4
    bool isEnableCheckBox5 = false; // 是否启用CheckBox5
} myGameHack;
static int status = 0; // 判断作弊状态
// Method
int (*Screen$$get_height)(); // 预定义一个方法
int (*Screen$$get_width)();  // 预定义一个方法

void *main_thread(void *);

// Hook预定义模版
#define HOOK_DEF(ret, func, ...) \
    ret (*orig##func)(__VA_ARGS__); \
    ret my##func(__VA_ARGS__)