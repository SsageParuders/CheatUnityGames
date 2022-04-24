#pragma once
// system lib
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>
#include <dlfcn.h>
#include <dlfcn.h>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <assert.h>
#include <sstream>
#include <vector>
#include <any>
#include <setjmp.h>
#include <assert.h>
#include <utility>
#include <memory>
#include <algorithm>
#include <limits>
#include <vector>
#include <android/log.h>

using namespace std;

// UnityVersion
#define UnityVersion 2018.3.8f1 // Unity版本号 <-- 自行适配
/* UnityVersion Compatible list
// 5.3.0f4     | 5.3.0f4 - 5.3.1f1         | v16
// 5.3.2f1     | 5.3.2f1                   | v19
// 5.3.3f1     | 5.3.3f1 - 5.3.4f1         | v20
// 5.3.5f1     | 5.3.5f1                   | v21
// 5.3.6f1     | 5.3.6f1                   | v21
// 5.3.7f1     | 5.3.7f1 - 5.3.8f2         | v21
// 5.4.0f3     | 5.4.0f3                   | v21
// 5.4.1f1     | 5.4.1f1 - 5.4.3f1         | v21
// 5.4.4f1     | 5.4.4f1 - 5.4.6f3         | v21
// 5.5.0f3     | 5.5.0f3                   | v22
// 5.5.1f1     | 5.5.1f1 - 5.5.6f1         | v22
// 5.6.0f3     | 5.6.0f3 - 5.6.7f1         | v23
// 2017.1.0f3  | 2017.1.0f3 - 2017.1.2f1   | v24
// 2017.1.3f1  | 2017.1.3f1 - 2017.1.5f1   | v24
// 2017.2.0f3  | 2017.2.0f3                | v24
// 2017.2.1f1  | 2017.2.1f1 - 2017.4.40f1  | v24
// 2018.1.0f2  | 2018.1.0f2 - 2018.1.9f2   | v24
// 2018.2.0f2  | 2018.2.0f2 - 2018.2.21f1  | v24
// 2018.3.0f2  | 2018.3.0f2 - 2018.3.7f1   | v24.1
// 2018.3.8f1  | 2018.3.8f1 - 2018.4.36f1  | v24.1
// 2019.1.0f2  | 2019.1.0f2 - 2019.2.21f1  | v24.2
// 2019.3.0f6  | 2019.3.0f6 - 2019.3.6f1   | v24.2
// 2019.3.7f1  | 2019.3.7f1 - 2019.4.14f1  | v24.3
// 2019.4.15f1 | 2019.4.15f1 - 2019.4.20f1 | v24.4
// 2019.4.21f1 | 2019.4.21f1 - 2019.4.29f1 | v24.5
// 2020.1.0f1  | 2020.1.0f1 - 2020.1.10f1  | v24.3
// 2020.1.11f1 | 2020.1.11f1 - 2020.1.17f1 | v24.4
// 2020.2.0f1  | 2020.2.0f1 - 2020.2.3f1   | v27
// 2020.2.4f1  | 2020.2.4f1 - 2020.3.15f2  | v27.1
// 2021.1.0f1  | 2021.1.0f1 - 2021.1.16f1  | v27.2
*/
// Unity Path
#define STR(x) #x
#define STRINGIFY_MACRO(x) STR(x)
#define EXPAND(x) x
#define IL2CPPHEADER(a, b, c) STRINGIFY_MACRO(EXPAND(a)EXPAND(b)EXPAND(c))
#define IL2CPPAPIDIR Il2cppApi/
#define IL2CPPCLASS IL2CPPHEADER(IL2CPPAPIDIR,UnityVersion,/il2cpp-class.h) // 基础数据类型
#define IL2CPPAPI IL2CPPHEADER(IL2CPPAPIDIR,UnityVersion,/il2cpp-api-functions.h) // API

// Unity lib
#include IL2CPPCLASS // Unity基础数据类型
#define DO_API(r, n, p) r(*n) p // 对 DO_API的预定义
#include IL2CPPAPI // Unity的API
#undef DO_API // API头文件处理结束

// Method
void init_il2cpp_api(){ // 初始化Unity的API
#define DO_API(r, n, p) n = (r(*) p)dlsym(dlopen("libil2cpp.so", RTLD_LAZY), #n) // 本质就是利用句柄dlsym出il2cpp的API auto
#include IL2CPPAPI // Unity的API
#undef DO_API // 清除 以防止初始化冲突
} // 运行任何Unity自身API时 切记调用此方法初始化

// Util lib
#include "BasicStructs/BasicStructs.h" // BasicStructs
#include "Il2cppUtils.h" // Utils
