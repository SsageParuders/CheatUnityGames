// System lib
#include <iostream>
#include <dlfcn.h>
#include <string>
#include <EGL/egl.h> // EGL
#include <GLES2/gl2.h>
#include <android_native_app_glue.h>
#include <memory>
#include <string>
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
#include <curl.h> // curl
#include <json.hpp> // json

// Var
static unsigned long il2cppAddress = 0; // 存储il2cpp.so基地址
static bool isGuiInit = false; // 是否初始化过imgui
struct UnityArrayIl2CppObject{
    uint32_t *klass;
    uint32_t *monitor;
};
struct UnityIl2CppArray : public UnityArrayIl2CppObject{
    /* bounds is NULL for szarrays */
    uint32_t *bounds;
    /* total number of elements of the array */
    il2cpp_array_size_t max_length;
};
struct UnityArrayBytes : public UnityIl2CppArray{
public:
    __attribute__((aligned(8))) uint8_t m_Items[1];

    uint8_t GetAt(int index) const{
        return m_Items[index];
    }
    void SetAt(int index, uint8_t value){
        m_Items[index] = value;
    }
};
struct gameHackStruct { // ImGUI的CheckBox回调事件处理
    bool isEnableCheckBox0 = false; // 是否启用CheckBox0
    bool isEnableCheckBox1 = false; // 是否启用CheckBox1
    bool isEnableCheckBox2 = false; // 是否启用CheckBox2
    bool isEnableCheckBox3 = false; // 是否启用CheckBox3
    bool isEnableCheckBox4 = false; // 是否启用CheckBox4
    bool isEnableCheckBox5 = false; // 是否启用CheckBox5
} myGameHack;
struct gameFuncInit {
    bool isFuncInit0 = false;
    bool isFuncInit1 = false;
    bool isFuncInit2 = false;
    bool isFuncInit3 = false;
    bool isFuncInit4 = false;
    bool isFuncInit5 = false;
} myGameFunc;
static bool isAllowed = false; // 是否允许游戏
static int connectCount = 0; // 连接次数
static int updateCount = 0; // 更新次数
const MethodInfo *AccountManager_AsobimoId; // 获取游戏ID实例
const MethodInfo *AccountManager_get_AsobimoId; // 获取游戏ID的get方法
const MethodInfo *Encoding_UTF8; // UTF8编码实例
const MethodInfo *Encoding_GetBytes; // 获取字节数组实例
static Il2CppObject *utf_8; // utf8对象实例
static Il2CppObject *ret_AsobimoId_obj;
static Il2CppObject *ret_AsobimoId;
static string AsobimoId;
// Method
int (*Screen$$get_height)(); // 预定义一个方法
int (*Screen$$get_width)();  // 预定义一个方法
void *main_thread(void *);
// Hook预定义模版
#define HOOK_DEF(ret, func, ...) \
    ret (*orig##func)(__VA_ARGS__); \
    ret my##func(__VA_ARGS__)