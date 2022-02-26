// user lib
#include "UnityCheat.h"
const MethodInfo *m = NULL;
Il2CppString *txt = NULL;
void *params[2] = {};

// OnGUI
HOOK_DEF(void, Test, void *__this, void *method){
	LOGD("Test Hook");
	origTest(__this, method);
	// il2cpp_runtime_invoke(m, nullptr, params, nullptr);
}

// hack start
void* main_thread(void*){
	
	LOGD("Start Cheat");

	LOGD("Mac is: %s", getAndroidMac());
	int apiLevel = getAndroidApiLevel(); // 确认系统Api版本
	LOGD("Api level: %d", apiLevel);

	do {// 等待目标动态库初始化完成
        sleep(1);
		LOGD("Waiting for il2cpp.so has been loaded ...");
	} while (!isLibraryLoaded("libil2cpp.so"));// 动态库已经完全加载

	while (il2cppAddress == 0){
		il2cppAddress = getModuleBase("libil2cpp.so");
		if (il2cppAddress != 0){
			LOGD("libil2cpp.so BaseAddress: %lx",il2cppAddress);
			break;
		}
	} // 获取 il2cpp 基地址
	
	
	/**
	 * @brief Demo Util
	 * 
	 */
	init_il2cpp_api();

	InitResolveFunc(Screen$$get_height, "UnityEngine.Screen::get_height");
	InitResolveFunc(Screen$$get_width, "UnityEngine.Screen::get_width");
	// 使用Unity游戏内的导出方法 获取屏幕宽高
	if (Screen$$get_height && Screen$$get_width){
		LOGI("Screen height is %d \nScreen width is %d", Screen$$get_height(), Screen$$get_width());
	}
	/**
	 * @brief 下面是一些我们自己定义的基本数据类型
	 */
	Rect r1 = {100,200,300,400};

	Il2CppDomain *domain = il2cpp_domain_get();
	LOGD("Il2CppDomain:%p", domain);

	const Il2CppAssembly *assembly = il2cpp_domain_assembly_open(domain, "Assembly-CSharp");
	LOGD("Il2CppAssembly:%p", assembly);

	const Il2CppImage *image = il2cpp_assembly_get_image(assembly);
	LOGD("Il2CppImage:%p", image);

	Il2CppClass *typeInfo = il2cpp_class_from_name(image, "", "NGUIDebug");
	LOGD("TypeInfo:%p", typeInfo);

	const Il2CppType *type = il2cpp_class_get_type(typeInfo);

	LOGD("Il2CppType:%p", type);

	Il2CppObject *refType = il2cpp_type_get_object(type);
	LOGD("refType:%p", refType);



	UnityObject* (*UnityEngine$GameObject$$Internal_AddComponentWithType)(void* __this, Il2CppObject* typeInfo);
	UnityObject* (*UnityEngine$GameObject$$GetComponent)(void* __this, Il2CppObject* type);
	UnityObject* (*UnityEngine$GameObject$$Find)(Il2CppString* string);

	InitResolveFunc(UnityEngine$GameObject$$Internal_AddComponentWithType, "UnityEngine.GameObject::Internal_AddComponentWithType(System.Type)");
	InitResolveFunc(UnityEngine$GameObject$$GetComponent, "UnityEngine.GameObject::GetComponent(System.Type)");
	InitResolveFunc(UnityEngine$GameObject$$Find, "UnityEngine.GameObject::Find(System.String)");

	UnityObject *target = UnityEngine$GameObject$$Find(il2cpp_string_new("InitGameSystem"));
	LOGD("target:%p", target);

	UnityObject *guiComponent = UnityEngine$GameObject$$Internal_AddComponentWithType(target, refType);
	LOGD("guiComponent:%p", guiComponent);

	txt = il2cpp_string_new("Testsssssssssssssssssssssss");

	const Il2CppAssembly *assembly2 = il2cpp_domain_assembly_open(domain, "UnityEngine.IMGUIModule");
	LOGD("Il2CppAssembly:%p", assembly2);

	const Il2CppImage *image2 = il2cpp_assembly_get_image(assembly2);
	LOGD("Il2CppImage:%p", image2);
	typeInfo = il2cpp_class_from_name(image2, "UnityEngine", "GUI");
	LOGD("typeInfo:%p", typeInfo);
	m = il2cpp_class_get_method_from_name(typeInfo, "Button", 2);
	// params[0] = r1;
	// params[1] = txt;

	auto *domain2 = il2cpp_domain_get();
	LOGD("Il2CppDomain:%p", domain2);
	il2cpp_thread_attach(domain2);
	size_t ass_len = 0;

	const Il2CppAssembly **assembly_list = il2cpp_domain_get_assemblies(domain2, &ass_len);
	while (strcmp((*assembly_list)->aname.name, "Assembly-CSharp") != 0){ // 所在程序集 (dll | assembly)
		LOGI("Assembly name: %s", (*assembly_list)->aname.name);
		assembly_list++;
	}

	const Il2CppImage *image3 = il2cpp_assembly_get_image(*assembly_list); // 从中取出image
	LOGD("Il2CppImage Address: %p", image3);
	LOGD("Il2CppImage Name: %s", image->name);

	Il2CppClass *clazzGUI = il2cpp_class_from_name(image3, "", "NGUIDebug"); // 第二个参数 命名空间 第三个参数 类名
	LOGD("Il2CppClass Address: %p", clazzGUI);
	LOGD("Il2CppClass Name: %s", clazzGUI->name);

	const MethodInfo *OnGUI = il2cpp_class_get_method_from_name(clazzGUI, "OnGUI", 0); // 第二个参数 方法名 第三个参数 参数个数
	unsigned long OnGUI$$Address = (unsigned long)(OnGUI->methodPointer);
	LOGD("Method Address is at %lx", OnGUI$$Address);
	LOGD("Method Name is %s", OnGUI->name);
	myHook(OnGUI$$Address, (void *)myTest, (void **)&origTest, "OnGUI");

	LOGD("Finish Cheat");

	// 线程退出
	pthread_exit(nullptr);
}

// 先atmain函数执行的 最高执行权重函数
__unused __attribute__((constructor)) void constructor_main() {
	LOGD("ToramCheat started");

	pthread_t ptid;
	// 另开线程 以免阻塞游戏主线程
	pthread_create(&ptid, nullptr, main_thread, nullptr);

	LOGD("ToramCheat Finished");
}

// 主函数
int main() {// nothing to do
    return 0;
}