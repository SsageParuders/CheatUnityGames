# Example for Android Native Development by VsCode

> 网上大部分的`Android Native`开发都是依托于`Android Studio`
>
> 但是假设我们仅仅只是需要便捷的开发一些`Android Native`层产物
>
> 比如:动态库(*.so), 静态库(*.a), 可执行文件(elf) ...
>
> 这时`Android Studio`难免显得有些笨重/不便捷
>
> 因此, 本项目将大概介绍如何使用VsCode进行便捷的`Android Native`层开发

## Set Environment

- 将`Android Native`项目关联`ndk`

    关联`ndk`开发环境

    编写`.vscode`目录下的`c_cpp_properties.json`

    将`includePath`中的路径修改为自己系统中 `ndk` 的路径

    `cStandard`和`cppStandard` 是c/c++标准, 根据自己需求修改他

    `defines`主要是在编写代码时的预处理定义

***我们这样做的目的, 仅仅只是为了将项目关联ndk***

***这样头文件才不会缺失, VsCode才能给我们智能提示***

***编译的环节, 我们交给`ndk-build`脚本***

***`ndk-build`本质其实就是`make`构建体系***

## Compile Android.mk And Application.mk

> 相关语法, 有注释在脚本里

# Target: Inject so to Game And Trick Game On Android

> 注入的意思是将so注入到游戏内部, 这样我们就可以实现一些作弊手段
> 
> 常见的比如: 内存读写/hook/call 等等
> 
> 而这些手段如果要细分的话, 也有很多类别
>
> > 以内存读写为例:
> >
> >  在注入的情况下, 可以直接使用memcpy
> >
> >  在非注入情况下, 可以使用preadv/syscall
> >  
> >  更高级的, 可以刷入内核驱动, 从而在内核层直接读写物理内存
>
> 同时, 注入的手法也有很多种类
>
> 常见的比如: 修改dex/注入Zygote 等等
> 
> 这些手法在本项目中, 都会一一展示, 以供后来者研究/学习

# How to Use This Project

Later ... OK?

# TODO List

- [x] 完善注入情况下的内存读写

- [x] 测试hook问题

- [x] 改善项目结构

- [x] 完善内存段权限属性恢复

- [x] dlopen的hook适配安卓全版本

- [x] 添加CMake支持

- [ ] 实现基于MAC地址的授权验证

- [ ] 完善`Engilsh`版的README

- [ ] 完善README内容