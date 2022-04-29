// user lib
#include "UnityCheat.hpp"

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
			ImFontConfig font_cfg; // 创建imgui字体配置 (可选)
			font_cfg.SizePixels = 22.0f; // 设置字体大小
			io.Fonts->AddFontDefault(&font_cfg); // 加载字体
			ImGui::GetStyle().ScaleAllSizes(3.0f); // 设置imgui字体大小
			isGuiInit = true; // 设置初始化过imgui
			LOGD("[UnityCheat] Start imgui!");
		}
		if (isGuiInit) { // 判断是否初始化过imgui
			ImGuiIO &io = ImGui::GetIO(); // 获取imgui IO
			ImGui_ImplOpenGL3_NewFrame(); // 更新imgui opengl
			ImGui::NewFrame(); // 更新imgui
			ImGui::Begin("Unity Cheat"); // 开始imgui窗口
			ImGui::Text("Hello, world!"); // 显示文本
			ImGui::EndFrame(); // 结束imgui窗口
			ImGui::Render(); // 渲染imgui
			glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y); // 设置视口
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // 渲染imgui opengl
		}
	}
	// 原始函数调用
	return origEglSwapBuffers(dpy, surface);
}
HOOK_DEF(void, Input, void *thiz, void *a2, void *a3){
	// 原始函数调用
	origInput(thiz, a2, a3);
	LOGD("action:%d", AMotionEvent_getAction((AInputEvent *)thiz)); // 获取动作
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
		LOGI("eglSwapBuffers is %p", sym_eglSwapBuffers); // 打印eglSwapBuffers地址
		myHook((unsigned long)sym_eglSwapBuffers, (void *)myEglSwapBuffers, (void **)&origEglSwapBuffers, "eglSwapBuffers"); // 创建hook
	}

	void *sym_input = DobbySymbolResolver(NULL, "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"); // 获取input接收器
	if (NULL != sym_input){ // 判断是否获取到了input接收器
		LOGD("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE is %p", (unsigned long long)sym_input); // 打印input接收器地址
		myHook((unsigned long)sym_input, (void *)myInput, (void **)&origInput, "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"); // 创建hook
	}

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