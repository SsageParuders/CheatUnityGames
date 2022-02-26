#pragma once
/**
 * @file Il2cppUtils.h
 * @author SsageParuders
 * @brief Il2cppUtils
 * @version 0.1
 * @date 2022-02-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// 获取导出函数
#define InitResolveFunc(x, y) *reinterpret_cast<void **>(&x) = (void *)il2cpp_resolve_icall(y)