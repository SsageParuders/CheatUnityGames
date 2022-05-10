// user lib
#include "UnityCheat.hpp"

// 修改 - 判断怪物状态时对怪物距离范围的判断 实现AOE技能全屏
HOOK_DEF(int, MobStatusMaster_get_Size, void *__this, void *methodInfo){
	if(myGameHack.isEnableCheckBox1){ // 开启了功能
		return 150;
	} else { // 未开启功能
		return origMobStatusMaster_get_Size(__this, methodInfo); // 返回原始值
	}
}

// 修改 - 判断怪物移动时的移动速度 实现怪物无法位移
HOOK_DEF(int, MobStatusMaster_get_MoveSpeed, void *__this, void *methodInfo){
	if(myGameHack.isEnableCheckBox2){ // 开启了功能
		return 0;
	} else { // 未开启功能
		return origMobStatusMaster_get_MoveSpeed(__this, methodInfo);
	}
}

// 实现hook不崩溃
HOOK_DEF(EGLBoolean, EglSwapBuffers, EGLDisplay dpy, EGLSurface surface){
	// 判断是否是主屏幕
	if (Screen$$get_width() == Screen$$get_width()) {
		if (!isGuiInit) { // 判断是否初始化过imgui
			LOGD("[UnityCheat] Main screen detected!");
			IMGUI_CHECKVERSION(); // 初始化imgui
			LOGD("[UnityCheat] Init imgui!");
			ImGui::CreateContext(); // 创建imgui上下文
			ImGuiIO &io = ImGui::GetIO(); // 获取imgui IO
			io.DisplaySize = ImVec2(Screen$$get_width(), Screen$$get_height() ); // 设置屏幕尺寸
			io.IniFilename = nullptr; // 关闭imgui配置文件
			ImGui::StyleColorsDark(); // 设置imgui默认颜色
			ImGui_ImplOpenGL3_Init("#version 300 es"); // 初始化imgui opengl
			ImFontConfig font_cfg; // 创建字体配置
			ImFont *myFont; // 设置字体
			font_cfg.FontDataOwnedByAtlas = false; // 设置字体数据是否在图集中
			myFont = io.Fonts->AddFontFromMemoryTTF((void *)OPPOSans_H, OPPOSans_H_size, 32.0f, &font_cfg, io.Fonts->GetGlyphRangesChineseFull()); // 加载字体 - 字体用OPPO开源的
			isGuiInit = true; // 设置初始化过imgui
			LOGD("[UnityCheat] Start imgui!");
		}
		// TODO: 菜单和回调得要封装一下
		if (isGuiInit) { // 判断是否初始化过imgui
			ImGuiIO &io = ImGui::GetIO(); // 获取imgui IO
			ImGui_ImplOpenGL3_NewFrame(); // 更新imgui opengl
			ImGui::NewFrame(); // 更新imgui
			/* 菜单绘制开始 */
			ImGui::Begin("托拉姆物语_九游_MOD版"); // 开始imgui窗口
			ImGui::Checkbox("技能全屏", &myGameHack.isEnableCheckBox1); // 显示checkbox
			ImGui::SameLine(); // 同一行
			ImGui::Checkbox("定怪", &myGameHack.isEnableCheckBox2); // 显示checkbox
			/* 菜单绘制结束 */
			ImGui::EndFrame(); // 结束imgui窗口
			ImGui::Render(); // 渲染imgui
			glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y); // 设置视口
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // 渲染imgui opengl
		}
	}
	// 原始函数调用
	return origEglSwapBuffers(dpy, surface);
}
HOOK_DEF(void, Input, void *thiz, void *a2, void *a3){ // 触摸设置
	// 原始函数调用
	origInput(thiz, a2, a3);
	// LOGD("action:%d", AMotionEvent_getAction((AInputEvent *)thiz)); // 获取动作
	ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz); // 处理imgui输入
	return;
}

// hack start
void* main_thread(void*){
	
	LOGD("Start Cheat");

	// initilize il2cpp api
	do {// 等待目标动态库初始化完成
        sleep(1);
		LOGD("Waiting for il2cpp.so has been loaded ...");
	} while (!isLibraryLoaded("libil2cpp.so"));// 动态库已经完全加载

	while (il2cppAddress == 0 || il2cppHandle == nullptr) {// 动态库已经完全加载
		il2cppAddress = getModuleBase("libil2cpp.so");
		il2cppHandle = dlopen("libil2cpp.so", RTLD_LAZY);
		if (il2cppAddress != 0 && il2cppHandle != nullptr){
			LOGD("libil2cpp.so BaseAddress: %lx",il2cppAddress);
			LOGD("libil2cpp.so Handle: %p",il2cppHandle);
			break;
		}
	} // 获取 il2cpp 基地址和句柄

	init_il2cpp_api(); // 初始化API

	LOGD("Mac is: %s", getAndroidMac()); // 获取mac地址
	LOGD("Api level: %d", getAndroidApiLevel()); // 打印Api版本

	InitResolveFunc(Screen$$get_height, "UnityEngine.Screen::get_height");
	InitResolveFunc(Screen$$get_width, "UnityEngine.Screen::get_width");
	// 使用Unity游戏内的导出方法 获取屏幕宽高
	if (Screen$$get_height && Screen$$get_width){
		LOGI("Screen height is %d \nScreen width is %d", Screen$$get_height(), Screen$$get_width());
	}

	void *sym_eglSwapBuffers = DobbySymbolResolver(NULL, "eglSwapBuffers"); // 获取eglSwapBuffers地址
	if (NULL != sym_eglSwapBuffers){ // 判断是否获取到了eglSwapBuffers
		LOGD("eglSwapBuffers is %p", (unsigned long)sym_eglSwapBuffers); // 打印eglSwapBuffers地址
		myHook((unsigned long)sym_eglSwapBuffers, (void *)myEglSwapBuffers, (void **)&origEglSwapBuffers, "eglSwapBuffers"); // 启动hook
	}

	void *sym_input = DobbySymbolResolver("/system/lib/libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"); // 获取input接收器
	if (NULL != sym_input){ // 判断是否获取到了input接收器
		LOGD("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE is %p", (unsigned long)sym_input); // 打印input接收器地址
		myHook((unsigned long)sym_input, (void *)myInput, (void **)&origInput, "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"); // 创建hook
	}

	unsigned long MobStatusMaster$$get_Size = GetMethodAddress("Assembly-CSharp", "", "MobStatusMaster", "get_Size", 0); // 修改距离判断 实现技能全屏
	LOGD("MobStatusMaster$$get_Size addr: %p", MobStatusMaster$$get_Size);
	myHook(MobStatusMaster$$get_Size, (void *)myMobStatusMaster_get_Size, (void **)&origMobStatusMaster_get_Size, "MobStatusMaster$$get_Size");

	unsigned long MobStatusMaster$$get_MoveSpeed = GetMethodAddress("Assembly-CSharp", "", "MobStatusMaster", "get_MoveSpeed", 0); // 修改怪物移动速度 实现定怪
	LOGD("MobStatusMaster$$get_MoveSpeed addr: %p", MobStatusMaster$$get_MoveSpeed);
	myHook(MobStatusMaster$$get_MoveSpeed, (void *)myMobStatusMaster_get_MoveSpeed, (void **)&origMobStatusMaster_get_MoveSpeed, "MobStatusMaster$$get_MoveSpeed");

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