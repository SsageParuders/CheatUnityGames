cmake_minimum_required(VERSION 3.2.2) # 最低CMake版本要求

##################### ⬇Android设置⬇ #####################
set(CMAKE_SYSTEM_NAME ANDROID) # 设置目标编译平台参数 Android
set(CMAKE_SYSTEM_VERSION 28) # 系统版本
set(ANDROID_PLATFORM 28) # 平台版本
set(ANDROID_ABI armeabi-v7a) # 设置目标构架 armeabi-v7a
set(ANDROID_NDK /Users/hongqing/Library/Android/sdk/ndk/23.1.7779620) # 设置ndk路径
set(CMAKE_TOOLCHAIN_FILE /Users/hongqing/Library/Android/sdk/ndk/23.1.7779620/build/cmake/android.toolchain.cmake) # 设置交叉编译链的cmake配置
##################### ⬆Android设置⬆ #####################

project(UnityCheat VERSION 0.1.0) # 工程名称 + 版本

##################### ⬇项目相关参数设置⬇ #####################
set(CMAKE_CXX_STANDARD 17) # c++ 标准
set(CMAKE_CXX_FLAGS 
    "-fno-rtti -fno-exceptions -DNDEBUG -fvisibility=hidden -Wno-narrowing -fdeclspec -pthread -w -s -fexceptions -Wall -O3 -Wno-error=format-security -fpermissive"
) # 参数
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY   ${PROJECT_SOURCE_DIR}/outputs/libs/${CMAKE_ANDROID_ARCH_ABI}/) # 重定向输出产物(动态库)
##################### ⬆项目相关参数设置⬆ #####################

##################### ⬇CMake头文件设置⬇ #####################
FILE(GLOB_RECURSE FILE_INCLUDES # 遍历子目录下所有符合情况的头文件
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*
)
include_directories( # 设置全局头文件目录 使其他源码文件可在任意目录<头文件.h>
    ${CMAKE_CURRENT_SOURCE_DIR}/include/AndroidLog/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/AndroidProp/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MemoryTools/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MapTools/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Dobby/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ImGUI/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Il2cppResolver/
)
##################### ⬆CMake头文件设置⬆ #####################

##################### ⬇CMake源文件设置⬇ #####################
FILE(GLOB_RECURSE FILE_SOURCES # 遍历子目录下所有符合情况的源文件
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c*
)
##################### ⬆CMake源文件设置⬆ #####################

##################### ⬇设置三方库文件目录⬇ #####################
link_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/dobby/android/${CMAKE_ANDROID_ARCH_ABI}/
)
##################### ⬆设置三方库文件目录⬆ #####################

##################### ⬇添加产物⬇ #####################
add_library(${PROJECT_NAME} SHARED # 生成动态库
    ${FILE_INCLUDES} # 头文件
    ${FILE_SOURCES} # 源文件
)
##################### ⬆添加产物⬆ #####################

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue
)

##################### ⬇连接库文件⬇ #####################
target_link_libraries(${PROJECT_NAME} PRIVATE 
    m dl log android EGL GLESv2 dobby
)
##################### ⬆连接库文件⬆ #####################