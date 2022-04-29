// System lib
#include <sys/mman.h>
#include <iostream>
#include <sys/system_properties.h>

// User lib
#include "Logger.hpp"

/*获取当前设备的Mac*/
const char *getAndroidMac();
/*获取当前设备的API版本*/
int getAndroidApiLevel();