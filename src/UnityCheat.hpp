// System lib
#include <iostream>
#include <dlfcn.h>
#include <string>
// user lib
#include <Logger.hpp> // Log
#include <AndroidProp.hpp> // Property
#include <MapTools.hpp> // Maps
#include <MemoryTools.hpp> // Memory
#include <ResolverInit.hpp> // Il2cppUtils

// Var
static unsigned long il2cppAddress = 0; // 存储il2cpp.so基地址

// Method
int (*Screen$$get_height)(); // 预定义一个方法
int (*Screen$$get_width)();  // 预定义一个方法

void *main_thread(void *);

// Hook预定义模版
#define HOOK_DEF(ret, func, ...) \
    ret (*orig##func)(__VA_ARGS__); \
    ret my##func(__VA_ARGS__)