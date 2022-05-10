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

// 初始化Il2cpp函数
#define InitResolveFunc(x, y) *reinterpret_cast<void **>(&x) = (void *)il2cpp_resolve_icall(y)

// 获取方法地址
unsigned long GetMethodAddress(const char *assName, const char *namespaceName, const char *className, const char *methodName, int num){
    auto *domain = il2cpp_domain_get(); // 获取domain
    il2cpp_thread_attach(domain); // 将domain附加到线程
    size_t ass_len = 0; // 初始化ass_len
    const Il2CppAssembly **assembly_list = il2cpp_domain_get_assemblies(domain, &ass_len); // 获取domain中的assembly
    while (strcmp((*assembly_list)->aname.name, assName) != 0){ // 所在程序集 (dll | assembly)
        // LOGI("Assembly name: %s", (*assembly_list)->aname.name); // 打印程序集名称
        assembly_list++;
    }
    const Il2CppImage *image = il2cpp_assembly_get_image((*assembly_list)); // 获取assembly中的image
    Il2CppClass *clazz = il2cpp_class_from_name(image, namespaceName, className);  // 第二个参数 命名空间 第三个参数 类名
    const MethodInfo *method = il2cpp_class_get_method_from_name(clazz, methodName, num); // 第二个参数 方法名 第三个参数 参数个数
    unsigned long methodAddress = (unsigned long)(method->methodPointer); // 获取方法地址
    return methodAddress;
}