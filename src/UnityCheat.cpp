// user lib
#include "UnityCheat.h"

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
	init_il2cpp_api(); // 初始化API

	InitResolveFunc(Screen$$get_height, "UnityEngine.Screen::get_height");
	InitResolveFunc(Screen$$get_width, "UnityEngine.Screen::get_width");
	// 使用Unity游戏内的导出方法 获取屏幕宽高
	if (Screen$$get_height && Screen$$get_width){
		LOGI("Screen height is %d \nScreen width is %d", Screen$$get_height(), Screen$$get_width());
	}
	/**
	 * @brief 下面是一些我们自己定义的基本数据类型
	 */
	// Rect r1 = {100,200,300,400};

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