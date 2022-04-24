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

// Il2CppClass *GetClassFromName(string _namespace, string _name){
//     init_il2cpp_api();

    
//     // DO_API(const Il2CppImage *, il2cpp_assembly_get_image, (const Il2CppAssembly *assembly));
//     // for (auto asmb : *Assembly$$GetAllAssemblies()){
//     //     TypeVector clases;
//     //     Image$$GetTypes(il2cpp_assembly_get_image(asmb), false, &clases);
//     //     for (auto cls : clases){
//     //         if (!cls)
//     //             continue;
//     //         Class$$Init(cls);
//     //         if (cls->name == _name && cls->namespaze == _namespace){
//     //             return (Il2CppClass *)cls;
//     //         }
//     //     }
//     // }
//     // return nullptr;
    
// }
