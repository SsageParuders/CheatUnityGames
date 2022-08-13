// user lib
#include "UnityCheat.hpp"
#include <jni.h>

static size_t OnWriteData(void *buffer, size_t size, size_t nmemb, void *lpVoid){
    std::string *str = dynamic_cast<std::string *>((std::string *)lpVoid);
    if (NULL == str || NULL == buffer){
        return -1;
    }
    char *pData = (char *)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}

int index = 0;
HOOK_DEF(int, Openat, int a, const char *b, int c) {

	LOGD("Openat: %s", b);

	// 改
	if (strcmp(b, "/data/app/~~Zm8hcJovWdVRxdUVe0mjfw==/com.xiaoyou.ToramOnline.aligames.ue-0NJGTbVIakbqF6bytuwt_g==/base.apk") == 0) {
		LOGD("Openat_change: %s", b);
		index++;
		LOGD("Openat_change: %d", index);
		if (index > 288) {
			return origOpenat(a, "/storage/emulated/0/MT2/apks/base.apk", c); // 改成新的apk 3是改
		}
		return origOpenat(a, b, c); // 原来的
	}

	// 原
	if (strcmp(b, "/data/app/~~sJy_xmXWdaYtehVtAoufNg==/com.xiaoyou.ToramOnline.aligames.uc-nx0zSJDPH4MaT9mljvf0wg==/base.apk") == 0) {
		LOGD("Openat_orig: %s", b);
		return origOpenat(a, "/storage/emulated/0/MT2/apks/base.apk", c);
	}

	return origOpenat(a, b, c);
}

// 修改 - 判断怪物状态时对怪物距离范围的判断 实现AOE技能全屏
HOOK_DEF(int, MobStatusMaster_get_Size, void *__this, void *methodInfo){
	if(myGameHack.isEnableCheckBox1){ // 开启了功能
		return 150;
	} else { // 未开启功能
		return origMobStatusMaster_get_Size(__this, methodInfo); // 返回原始值
	}
}

// 修改 - 判断怪物移动时的移动速度 实现怪物无法位移 定怪
HOOK_DEF(int, MobStatusMaster_get_MoveSpeed, void *__this, void *methodInfo){
	if(myGameHack.isEnableCheckBox2){ // 开启了功能
		return 0;
	} else { // 未开启功能
		return origMobStatusMaster_get_MoveSpeed(__this, methodInfo);
	}
}

HOOK_DEF(void, _GMWatchManager_Update, void *instance){
	if(instance != NULL){

		// call获取游戏ID
		AccountManager_AsobimoId = GetMethod_for_Property_Get("Assembly-CSharp", "", "AccountManager", "AsobimoId");
		ret_AsobimoId_obj = il2cpp_runtime_invoke(AccountManager_AsobimoId, NULL, NULL, NULL); // call 获取对象实例
		AccountManager_get_AsobimoId = GetMethod("Assembly-CSharp", "", "AccountManager", "get_AsobimoId", 0);
		void *param[] = {};
		ret_AsobimoId = il2cpp_runtime_invoke(AccountManager_get_AsobimoId, ret_AsobimoId_obj, param, NULL); // call获取到游戏ID
		/*
			进行字符串转化
		*/
		Encoding_UTF8 = GetMethod_for_Property_Get("mscorlib", "System.Text", "Encoding", "UTF8");
		utf_8 = il2cpp_runtime_invoke(Encoding_UTF8, NULL, NULL, NULL); // call 获取对象实例
		Encoding_GetBytes = GetMethod("mscorlib", "System.Text", "Encoding", "GetBytes", 1);
		void *params[] = {(Il2CppString *)ret_AsobimoId};
		UnityArrayBytes *ret_of_ID = (UnityArrayBytes *) il2cpp_runtime_invoke(Encoding_GetBytes, utf_8, params, NULL);
		AsobimoId = (char *)ret_of_ID->m_Items;

		// 使用资格验证
		if (!isAllowed && connectCount <=15 && !AsobimoId.empty()){ // 还没有进行授权验证过 或者授权验证已经10+次 并且游戏ID不为空
			connectCount++;
			CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
			if (CURLE_OK != res){
				LOGD("curl init failed");
			}
			CURL *curl;
			curl = curl_easy_init();
			if (NULL == curl){
				LOGD("Init CURL failed...");
			}
			char key[80];
			sprintf(key, "account=%s&password=%s", AsobimoId.substr(0, 10).c_str(), "QWEasdzxc77");
			const std::string strPost = key;
			const std::string strUrl = "http://138.2.123.80/api.php?act=user_logon&app=10000";
			const std::string strResponse;
			curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			nlohmann::json j1 = nlohmann::json::parse(strResponse);
			int code;
			j1.at("code").get_to(code);
			if (code == 200){
				isAllowed = true;
				LOGD("Login success!");
				isAllowed = true; // 登陆成功
			} else {
				LOGD("Login failed!");
			}
		}

		if(myGameHack.isEnableCheckBox1 && isAllowed){ // 技能全屏
			if(!myGameFunc.isFuncInit1){
				unsigned long MobStatusMaster$$get_Size = GetMethodAddress("Assembly-CSharp", "", "MobStatusMaster", "get_Size", 0); // 修改距离判断 实现技能全屏
				LOGD("MobStatusMaster$$get_Size addr: %p", MobStatusMaster$$get_Size);
				myHook(MobStatusMaster$$get_Size, (void *)myMobStatusMaster_get_Size, (void **)&origMobStatusMaster_get_Size, "MobStatusMaster$$get_Size");
				myGameFunc.isFuncInit1 = true;
			}
		} else {
			myGameHack.isEnableCheckBox1 = false;
		}

		if(myGameHack.isEnableCheckBox2 && isAllowed){ // 定怪
			if(!myGameFunc.isFuncInit2){
				unsigned long MobStatusMaster$$get_MoveSpeed = GetMethodAddress("Assembly-CSharp", "", "MobStatusMaster", "get_MoveSpeed", 0); // 修改怪物移动速度 实现定怪
				LOGD("MobStatusMaster$$get_MoveSpeed addr: %p", MobStatusMaster$$get_MoveSpeed);
				myHook(MobStatusMaster$$get_MoveSpeed, (void *)myMobStatusMaster_get_MoveSpeed, (void **)&origMobStatusMaster_get_MoveSpeed, "MobStatusMaster$$get_MoveSpeed");
				myGameFunc.isFuncInit2 = true;
			}
		} else {
			myGameHack.isEnableCheckBox2 = false;
		}

		if(myGameHack.isEnableCheckBox3 && isAllowed){ // 开启了无敌按钮 并且 授权验证成功
			if(!myGameFunc.isFuncInit3){
				if (aim_add != 0){ // aim_add被赋值过 有了固定的地址
					writeValue<int>(aim_add, 0); // 写值 实现无敌
					myGameFunc.isFuncInit3 = true; // 已经写过值了
				} else { // 没有初始化过 则需要搜索内存
					// 00 00 00 05 ; 00 00 00 00 ; 3F 00 00 00 ; 3F 80 00 00 ; 40 00 00 00 ; 40 80 00 00 ; 40 C0 00 00
					unsigned char pattern[] = {// 5D;0D;0.5F;1.0F;2F;4F;6F // 特征码
						0x05, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x3F,
						0x00, 0x00, 0x80, 0x3F,
						0x00, 0x00, 0x00, 0x40,
						0x00, 0x00, 0x80, 0x40,
						0x00, 0x00, 0xC0, 0x40};
					std::vector<MapsInfo> maps;
					MapsInfo *map = new MapsInfo;
					maps = getMapsInfo();
					for(int i = 0; i < maps.size(); i++){
						map = &maps[i];
						unsigned char *ptr = (unsigned char *)map->start;
						unsigned char *end = (unsigned char *)map->end;
						while (ptr < end - sizeof(pattern)){
							if (memcmp(ptr, pattern, sizeof(pattern)) == 0){
								LOGD("Memory: %lx-%lx\t Perms: %4s\t Size: %lx\t Name: %s", map->start, map->end, map->perms, map->size, map->name);
								LOGD("\033[42;37mFound Pattern at %lx\033[0m", ptr);
								aim_add = (unsigned long)ptr;
								LOGD("aim_add: %lx", aim_add);
								break;
							}
							ptr++;
						}
						memset(map, 0, sizeof(MapsInfo)); // 清空内存
					}
				}
			}
		} else {
			if (aim_add != 0){ // aim_add被赋值过 有了固定的地址
				writeValue<int>(aim_add, 5); // 写值 取消无敌
				myGameFunc.isFuncInit3 = false; // 取消写过值了
			}
			myGameHack.isEnableCheckBox3 = false;
		}

		if (myGameHack.isEnableCheckBox4 && isAllowed){ // 开启了无敌按钮 并且 授权验证成功
			if(!myGameFunc.isFuncInit4){
				myGameFunc.isFuncInit3 = true;
			}
		} else {
			isIoReLoad = false;
			myGameHack.isEnableCheckBox4 = false;
		}

	}
	orig_GMWatchManager_Update(instance);
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

		if (isGuiInit) { // 判断是否初始化过imgui
			ImGuiIO &io = ImGui::GetIO(); // 获取imgui IO
			ImGui_ImplOpenGL3_NewFrame(); // 更新imgui opengl
			ImGui::NewFrame(); // 更新imgui
			/* 菜单绘制开始 */
			ImGui::Begin("托拉姆物语_MOD"); // 开始imgui窗口
			ImGui::Text("Account ID: %s\t\t", AsobimoId.substr(0,10).c_str()); // 显示游戏状态
			if(isAllowed){
				ImGui::Text("Login success!!");
			} else {
				ImGui::Text("Login failed!!");
			}
			ImGui::Checkbox("技能全屏", &myGameHack.isEnableCheckBox1); // 显示checkbox
			ImGui::SameLine(); // 同一行
			ImGui::Checkbox("定怪", &myGameHack.isEnableCheckBox2); // 显示checkbox
			ImGui::SameLine(); // 同一行
			ImGui::Checkbox("无敌", &myGameHack.isEnableCheckBox3); // 显示checkbox
			// 新一行
			ImGui::Checkbox("过检测", &myGameHack.isEnableCheckBox4); // 显示checkbox
			ImGui::Text("注意事项:");
			ImGui::Text("1. 本辅助自动验证游戏账号是否具有使用权");
			ImGui::Text("2. 无敌需要被攻击一次后开启方可生效");
			/* 菜单绘制结束 */
			ImGui::EndFrame(); // 结束imgui窗口
			ImGui::Render(); // 渲染imgui
			glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y); // 设置视口
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // 渲染imgui opengl
		}

		if (!myGameHack.isEnableCheckBox0 && updateCount >= 5){
			myGameHack.isEnableCheckBox0 = true;
			unsigned long addrUpdate = GetMethodAddress("Assembly-CSharp", "", "GMWatchManager", "Update", 0);
			LOGD("GMWatchManager$$Update is %p", addrUpdate);
			if (addrUpdate != 0){
				myHook(addrUpdate, (void *)&my_GMWatchManager_Update, (void **)&orig_GMWatchManager_Update, "GMWatchManager.Update");
			}
		} else if (updateCount <= 30){
			LOGD("updateCount is %d", updateCount);
			updateCount++;
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

void dlopen_process(const char *name, void *handle) {
    // LOGD("dlopen: %s", name);
    if (!il2cppHandle) {
        if (strstr(name, "libil2cpp.so")) {
			il2cppHandle = handle;
			LOGI("Got il2cpp handle!");
        }
    }
}

HOOK_DEF(void*, __loader_dlopen, const char *filename, int flags, const void *caller_addr) {
    void *handle = orig__loader_dlopen(filename, flags, caller_addr);
    dlopen_process(filename, handle);
    return handle;
}

HOOK_DEF(void *, _do_dlopen_V24, const char *name, int flags, const void *extinfo, void *caller_addr){
	void *handle = orig_do_dlopen_V24(name, flags, extinfo, caller_addr);
	dlopen_process(name, handle);
	return handle;
}

HOOK_DEF(void *, _do_dlopen_V19, const char *name, int flags, const void *extinfo){
	void *handle = orig_do_dlopen_V19(name, flags, extinfo);
	dlopen_process(name, handle);
	return handle;
}

HOOK_DEF(const char *, Md5, JNIEnv *env, char *c){
	LOGD("Md5_orig is : %s\n", c);

	char *con = (char *)"xxxxxxxx";
	for (int i = 0; i < strlen(c); i++){
		c[i] = con[i];
	}

	LOGD("Md5_changed is : %s\n", c);

	return origMd5(env, c);
}

HOOK_DEF(void *, GetApk, int64_t a, int64_t b, int64_t c,int64_t d){
	LOGD("a is %d", a);
	LOGD("d is %d", d);
	return origGetApk(a, b, c, d);
}

HOOK_DEF(jobject, GetX509, JNIEnv *env, jobject a2, jstring a3){
	LOGD("aaaa");
	return origGetX509(env, a2, a3);
}

HOOK_DEF(jobjectArray, GetSig, JNIEnv *env, int64_t thiz){

	jobjectArray array = origGetSig(env, thiz);
	int len = env->GetArrayLength(array);

	for (int i = 0; i < len; i++){
		jstring val = (jstring)env->GetObjectArrayElement(array, i);
		LOGD("arr[%d]_orig is :%s\n", i, env->GetStringUTFChars(val, NULL));
	}

	// 原
	// env->SetObjectArrayElement(array, 0, env->NewStringUTF("O=xiaoyougame"));
	// env->SetObjectArrayElement(array, 1, env->NewStringUTF("O=xiaoyougame"));
	// env->SetObjectArrayElement(array, 2, env->NewStringUTF("F21CFED1A7212D85CAC2AAB8175A32BE"));
	// env->SetObjectArrayElement(array, 3, env->NewStringUTF("3BD6CBBD1BFEFD308B3AF2D9A04DE1F5312D4530"));

	// 改
	env->SetObjectArrayElement(array, 0, env->NewStringUTF("xxxxx"));
	env->SetObjectArrayElement(array, 1, env->NewStringUTF("xxxxx"));
	env->SetObjectArrayElement(array, 2, env->NewStringUTF("xxxxx"));
	env->SetObjectArrayElement(array, 3, env->NewStringUTF("xxxxx"));

	for (int i = 0; i < len; i++){
		jstring val = (jstring)env->GetObjectArrayElement(array, i);
		LOGD("arr[%d]_changed is :%s\n", i, env->GetStringUTFChars(val, NULL));
	}

	return origGetSig(env, thiz);
}

HOOK_DEF(void, Exit, int a){
	LOGD("\033[42;35mExit: %d\033[0m", a);
	uint64_t aa = 0;
	__asm__ __volatile__(
		"mov %0, x30\n"
		:"=r"(aa)
	);
	LOGD("x30 is %p", aa);
	LOGD("Exit_1 called  %p", __builtin_return_address(0));
	LOGD("Exit_2 called  %p", __builtin_return_address(1));
	LOGD("Exit_3 called  %p", __builtin_return_address(2));
	return ;
}

// hack start
void* main_thread(void*){
	
	LOGD("Start Cheat");

	// 适配性问题
	int apiLevel = getAndroidApiLevel();
	void *addr;
	if (apiLevel >= 30) { // 真机
		void *libdl_handle = dlopen("libdl.so", RTLD_LAZY);
		addr = dlsym(libdl_handle, "__loader_dlopen");
		LOGI("__loader_dlopen at: %p", addr);
		DobbyHook(addr, (void *)my__loader_dlopen, (void **)&orig__loader_dlopen);
	} else if (apiLevel >= 24) { // android 7.0 虚拟机适配
		addr = DobbySymbolResolver(nullptr, "__dl__Z9do_dlopenPKciPK17android_dlextinfoPv"); // 通过hook掉dlopen获取il2cpp的句柄
		LOGD("__dl__Z9do_dlopenPKciPK17android_dlextinfoPv addr: %p", addr);
		if (addr){
			DobbyHook(addr, (void *)my_do_dlopen_V24, (void **)&orig_do_dlopen_V24);
		}
	} else {
		addr = DobbySymbolResolver(nullptr, "__dl__Z9do_dlopenPKciPK17android_dlextinfo");
		LOGD("__dl__Z9do_dlopenPKciPK17android_dlextinfo addr: %p", addr);
		if (addr){
			DobbyHook(addr, (void *)my_do_dlopen_V19, (void **)&orig_do_dlopen_V19);
		}
	}

	// initilize il2cpp api
	do{ // 等待目标动态库初始化完成
		sleep(1);
		LOGD("Waiting for il2cpp.so has been loaded ...");
	} while (!isLibraryLoaded("libil2cpp.so")); // 动态库已经完全加载

	LOGD("Mac is: %s", getAndroidMac()); // 获取mac地址
	LOGD("Api level: %d", getAndroidApiLevel()); // 打印Api版本

	// TODO:优化dlopen处理
	while (il2cppAddress == 0 || il2cppHandle == nullptr) {// 动态库已经完全加载
		il2cppAddress = getModuleBase("libil2cpp.so");
		if (il2cppAddress != 0 && il2cppHandle != nullptr){
			LOGD("libil2cpp.so BaseAddress: %lx",il2cppAddress);
			LOGD("libil2cpp.so Handle: %p",il2cppHandle);
			break;
		}
	} // 获取 il2cpp 基地址和句柄
	DobbyDestroy(addr); // 销毁hook

	init_il2cpp_api(); // 初始化API

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

	void *sym_exit = DobbySymbolResolver(NULL, "exit"); // 获取exit地址
	if (NULL != sym_exit){ // 判断是否获取到了exit地址
		LOGD("exit is %p", (unsigned long)sym_exit); // 打印exit地址
		myHook((unsigned long)sym_exit, (void *)myExit, (void **)&origExit, "exit"); // 启动hook
	}
	// _Z18getCertificateInfoP7_JNIEnvP8_jobject
	void *sym_getSig = DobbySymbolResolver("libqkcheck.so", "_Z18getCertificateInfoP7_JNIEnvP8_jobject"); // 获取getSig地址
	if (NULL != sym_getSig){ // 判断是否获取到了getSig地址
		LOGD("_Z18getCertificateInfoP7_JNIEnvP8_jobject is %p", (unsigned long)sym_getSig); // 打印getSig地址
		myHook((unsigned long)sym_getSig, (void *)myGetSig, (void **)&origGetSig, "_Z18getCertificateInfoP7_JNIEnvP8_jobject"); // 启动hook
	}

	void *sym_getApk = DobbySymbolResolver("libqkcheck.so", "Java_com_game_apkverify_Check_getAPKCharacteristic"); // 获取getApk地址
	if (NULL != sym_getApk){ // 判断是否获取到了getApk地址
		LOGD("Java_com_game_apkverify_Check_getAPKCharacteristic is %p", (unsigned long)sym_getApk);	 // 打印getApk地址
		myHook((unsigned long)sym_getApk, (void *)myGetApk, (void **)&origGetApk, "Java_com_game_apkverify_Check_getAPKCharacteristic"); // 启动hook
	}

	void *sym_md5 = DobbySymbolResolver("libqkcheck.so", "_Z7get_md5P7_JNIEnvPc"); // 获取md5地址
	if (NULL != sym_md5){ // 判断是否获取到了md5地址
		LOGD("_Z7get_md5P7_JNIEnvPc is %p", (unsigned long)sym_md5); // 打印md5地址
		myHook((unsigned long)sym_md5, (void *)myMd5, (void **)&origMd5, "_Z7get_md5P7_JNIEnvPc"); // 启动hook
	}

	void *sym_getX509 = DobbySymbolResolver("libqkcheck.so", "_Z18getX509CertificateP7_JNIEnvP8_jobject"); // 获取getX509地址
	if (NULL != sym_getX509){ // 判断是否获取到了getX509地址
		LOGD("_Z18getX509CertificateP7_JNIEnvP8_jobject is %p", (unsigned long)sym_getX509); // 打印getX509地址
		myHook((unsigned long)sym_getX509, (void *)myGetX509, (void **)&origGetX509, "_Z18getX509CertificateP7_JNIEnvP8_jobject"); // 启动hook
	}

	// sleep(3); // 等待hook完成

	// void *sym_openat = DobbySymbolResolver("libc.so", "__openat"); // 获取open地址
	// if (NULL != sym_openat){ // 判断是否获取到了open地址
	// 	LOGD("__openat is %p", (unsigned long)sym_openat); // 打印openat地址
	// 	myHook((unsigned long)sym_openat, (void *)myOpenat, (void **)&origOpenat, "__openat"); // 启动hook
	// }

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