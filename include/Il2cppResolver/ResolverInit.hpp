// System lib
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

// User lib
#include "Logger.hpp"
#include "Il2cppApi/il2cppTabledefs.h"
#include "Il2cppApi/il2cppClass.h"

#define DO_API(r, n, p) r(*n) p
#include "Il2cppApi/il2cppApiFunctions.h"
#undef DO_API

static void *il2cpp_handle = nullptr;
static uint64_t il2cpp_base = 0;

void init_il2cpp_api(){
#define DO_API(r, n, p) n = (r(*) p)dlsym(il2cpp_handle, #n)
#include "Il2cppApi/il2cppApiFunctions.h"
#undef DO_API
}
