#pragma once
// System lib
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <cinttypes>
#include <set>
#include <string_view>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <errno.h>
#include <malloc.h>

// user lib
#include <MapTools.h>
#include <dobby.h>
#include <Logger.h>

/*读取模块地址*/
long getModuleBase(const char *moduleName);
/*判断目标so是否加载*/
bool isLibraryLoaded(const char *libraryName);

/*------ memcpy直接读写内存 只在注入情况下可行 ------*/
/*读值的基础函数*/
void readin(unsigned long address, void *buffer, size_t size);
/*写值的基础函数*/
void writein(unsigned long address, void *buffer, size_t size);

/**
 * 读写封装为c++模版函数 这样调用更加灵活
 * 由于模版函数的一些特殊性质 需要将源代码放在头文件中
 * 否则会出现 error: undefined symbol
 */
/**
 * @deprecated // 数据类型对应表(以 GameGuardian 修改器的标准为例子)
 * long     --  Dword
 * float    --  Float
 * double   --  Double
 */

/*读值模板函数*/
template <typename T> 
T readValue(unsigned long address){ // long value = readValue<long>(address)
    T temp = 0;
    readin(address, &temp, sizeof(temp));
    return temp;
}

/*写值模板函数*/
template <typename T>
void writeValue(unsigned long address, T value){ // writeValue<long>(address, 10)
    writein(address, &value, sizeof(value));
}

/*内存call*/
template <typename Type, typename... Type2> // int level = callFunction<int>(reinterpret_cast<void *>(get_Level$$MobStatusMaster$$Address))
Type callFunction(void *ptr, Type2... args){ // 如果被call函数有参数的话 直接在后面跟就行了 int a; float b; callFunction<int>(reinterpret_cast<void *>(get_Level$$MobStatusMaster$$Address), a, b);
    if (ptr == nullptr){
        return Type();
    }
    return reinterpret_cast<Type(__cdecl *)(Type2...)>(ptr)(args...);
}

/**
 * @brief 基于Dobby框架的一个封装完善
 * hook目标地址的方法
 * 
 * @param address 
 * @param myFuntion 
 * @param origFuntion 
 * @param str 
 * @return true 
 * @return false 
 */
bool myHook(unsigned long address, void *myFuntion, void **origFuntion, const char *str);

/**
 * @brief 基于Dobby框架的一个封装完善
 * 取消目标地址的hook
 * 
 * @param address 
 * @param str 
 * @return true 
 * @return false 
 */
bool unHook(unsigned long address, const char *str);