// System lib
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

// User lib
#include "Logger.hpp" //log
#include "Il2cppApi/il2cppTabledefs.h" 
#include "Il2cppApi/il2cppClass.h" // 数据类型
#include "BasicStructs/BasicStructs.h" // BasicStructs

#define DO_API(r, n, p) r(*n) p
#include "Il2cppApi/il2cppApiFunctions.h"
#undef DO_API

static void *il2cppHandle = nullptr; // 存储il2cpp.so句柄

void init_il2cpp_api(){ // 初始化API
#define DO_API(r, n, p) n = (r(*) p)dlsym(il2cppHandle, #n)
#include "Il2cppApi/il2cppApiFunctions.h"
#undef DO_API
}

#define InitResolveFunc(x, y) *reinterpret_cast<void **>(&x) = (void *)il2cpp_resolve_icall(y)