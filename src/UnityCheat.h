// System lib
#include <iostream>
#include <dlfcn.h>
#include <string>
// user lib
#include <Logger.h> // Log
#include <AndroidProp.h> // Property
#include <MapTools.h> // Maps
#include <MemoryTools.h> // Memory
#include <ResolverInit.h> // Il2cppUtils

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