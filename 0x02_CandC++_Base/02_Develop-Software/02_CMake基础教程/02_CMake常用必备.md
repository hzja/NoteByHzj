# CMake

## 目录

1. 设置编译器flags
2. 设置源文件属性
3. 链接器标志
4. Debug与Release包

分享一些工作中经常用到的一些CMake命令。
<a name="Si5qc"></a>
## 1、设置编译器flags
例如：设置C++标准，显示所有警告。
```cmake
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")
add_compile_options("$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-std=c++11>")
```
还可以设置一些优化flags<br />官方资料：[https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)<br />例如：
```
-O0, -O1, -O2, -O3
```
如果使用llvm，则可以参考这里：<br />[https://clang.llvm.org/docs/DiagnosticsReference.html](https://clang.llvm.org/docs/DiagnosticsReference.html)
<a name="xoat2"></a>
## 2、设置源文件属性
这是CMake的一个复杂属性，如果存在多个目标，可能需要更改一个目标的某些行为。例如，如果要使用C++11构建main.cpp，而只构建库，则可能希望将其构建为C++14。在这种情况下，可以使用`set_source_files_properties`命令来配置特定源文件的属性，如下所示：
```cmake
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/*.cpp PROPERTIES COMPILE_FLAGS "-std=c++11")
```
可以从以下手册中查看大量的属性，每个属性都可以用于满足特定需求。

- CMake 属性

[https://cmake.org/cmake/help/v3.3/manual/cmake-properties.7.html#source-file-properties](https://cmake.org/cmake/help/v3.3/manual/cmake-properties.7.html#source-file-properties)
<a name="j3cJf"></a>
## 3、链接器标志
以下是GNU GCC链接器的链接器标志列表。<br />GNU GCC 链接选项：[https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html](https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)<br />GCC链接器默认情况下可以在环境变量中定义的目录（/usr/lib;/usr/local/lib）中搜索，然后默认链接标准库。<br />最常见的标志之一是`-l`，用于链接所需的库，如`-lzlib`、`-lboost`等。<br />还有其他附加标志，可更改可执行文件的链接选项行为。<br />以下是可以添加链接器标志的变量：

- `CMAKE_EXE_LINKER_FLAGS`：在创建可执行文件时由链接器使用的标志
- `CMAKE_EXE_LINKER_FLAGS_RELEASE`：在创建发布版本可执行文件时由链接器使用的标志
- `CMAKE_EXE_LINKER_FLAGS_DEBUG`：在创建调试版本可执行文件时由链接器使用的标志
- `CMAKE_STATIC_LINKER_FLAGS`：在创建静态库（.a、.lib）时由链接器使用的标志
- `CMAKE_SHARED_LINKER_FLAGS`：在创建共享库（.so、.dll）时由链接器使用的标志
- `CMAKE_MODULE_LINKER_FLAGS`：在创建模块库时由链接器使用的标志

例如，可以使用以下代码添加链接器标志：
```cmake
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl")
```
了解如何使用这些标志可以更好地控制可执行文件的链接行为。
<a name="i2UwB"></a>
## 4、Debug与Release包
在CMakeLists.txt文件内部，可以访问CMAKE_BUILD_TYPE。可以在CMakeLists.txt中轻松检查构建类型：
```cmake
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    message("Debug Build")
elseif(${CMAKE_BUILD_TYPE} MATCHES Release)
    message("Release Build")
endif()
```
还可以使用上一节中所示的配置变量，分别为不同的构建类型设置编译器和链接器标志：

- CMAKE_EXE_LINKER_FLAGS_RELEASE：用于创建发布版本可执行文件的标志
- CMAKE_EXE_LINKER_FLAGS_DEBUG：用于创建调试版本可执行文件的标志
- CMAKE_CXX_FLAGS_RELEASE
- CMAKE_CXX_FLAGS_DEBUG
