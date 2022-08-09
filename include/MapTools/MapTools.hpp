#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <cinttypes>
#include <set>
#include <string_view>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <errno.h>
#include <malloc.h>
#include <map>
#include <vector>
// User lib
#include <Logger.hpp>

struct MapsInfo{
    uintptr_t start; // 地址
    uintptr_t end; // 地址
    char perms[5]; // r-x
    uintptr_t useless; // 不需要的垃圾信息
    unsigned long inode; // inode
    uintptr_t size; // 段大小 == 结束地址 - 开始地址
    char name[512]; // 段名称
};

std::vector<MapsInfo> getMapsInfo();
/*从地址判断所属内存段的属性*/
int getMemPermission(unsigned long address);
/*修改内存段保护属性*/// PROT_READ | PROT_WRITE | PROT_EXEC  ---- 读|写|执行
bool editMemProt(unsigned long address, int prot); // 第二个参数其实就是个int值 可以通过或运算表达多种属性