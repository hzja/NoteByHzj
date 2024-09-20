# C/C++

<a name="ptEZW"></a>

## lib 和 dll 的区别
首先介绍一下静态库（静态链接库）、动态库（动态链接库）的概念，首先两者都是代码共享的方式。<br />静态库：在链接步骤中，连接器将从库文件取得所需的代码，复制到生成的可执行文件中，这种库称为静态库，其特点是可执行文件中包含了库代码的一份完整拷贝；缺点就是被多次使用就会有多份冗余拷贝。即静态库中的指令都全部被直接包含在最终生成的 EXE 文件中了。在vs中新建生成静态库的工程，编译生成成功后，只产生一个.lib文件。

动态库：动态链接库是一个包含可由多个程序同时使用的代码和数据的库，DLL不是可执行文件。动态链接提供了一种方法，使进程可以调用不属于其可执行代码的函数。函数的可执行代码位于一个 DLL 中，该 DLL 包含一个或多个已被编译、链接并与使用它们的进程分开存储的函数。在vs中新建生成动态库的工程，编译成功后，产生一个.lib文件和一个.dll文件。

那么上述静态库和动态库中的lib有什么区别呢？<br />静态库中的lib：该LIB包含函数代码本身（即包括函数的索引，也包括实现），在编译时直接将代码加入程序当中。<br />动态库中的lib：该LIB包含了函数所在的DLL文件和文件中函数位置的信息（索引），函数实现代码由运行时加载在进程空间中的DLL提供。

总之，lib是编译时用到的，dll是运行时用到的。如果要完成源代码的编译，只需要lib；如果要使动态链接的程序运行起来，只需要dll。

以下例子均在vs2010上测试。


## 生成和使用动态库
### 生成动态库
新建项目--win32项目--填写项目名--确定--下一步--应用程序类型：选择dll--附加选项：选择导出符号--完成。<br />可以看到生成了一个dllmain.cpp 文件，这是dll应用程序的入口，注意它和普通工程的入口main函数不同，这个文件不需要修改。<br />在这个动态库中举例导出一个变量，一个类，一个函数，头文件dll.h如下：
```cpp
//新建生成dll的工程时，vs默认定义了宏DLL_EXPORT,因此，DLL_API 是 __declspec(dllexport)，用来导出
//在静态调用dll时，包含该头文件，由于没有定义DLL_EXPORT，所以DLL_API是
//__declspec(dllimport),用来导入
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

// 导出类
class DLL_API Cdll {
    public:
    Cdll(void);
    // TODO: 在此添加您的方法。
};

//导出变量，变量在.cpp文件中定义
extern DLL_API int ndll;

//导出函数，加extern "C",是为了保证编译时生成的函数名不变，这样动态调用dll时才能
//正确获取函数的地址
extern "C" DLL_API int fndll(void);
```
dll.cpp 文件如下：
```cpp
#include "dll.h"

// 这是导出变量的一个示例
DLL_API int ndll=6;

// 这是导出函数的一个示例。
DLL_API int fndll(void){
    return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 dll.h
Cdll::Cdll(){
    return;
}
```
<a name="HwFjF"></a>
### 调用动态库
有两种方法调用动态库，一种隐式链接，一种显示链接。
### 调用动态库：隐式链接
隐式链接 需要.h文件，dll文件，lib文件

（1）将dll放到工程的工作目录

（2）设置项目属性--vc++目录--库目录为lib所在的路径

（3）将lib添加到项目属性--链接器--输入--附加依赖项（或者直接在源代码中加入`#pragma comment(lib, "**.lib")`）

（4）在源文件中添加.h头文件

然后就像平常一样调用普通函数、类、变量。


### 调用动态库：显示链接
显示链接只需要.dll文件，但是这种调用方式不能调用dll中的变量或者类（其实可以调用类，但是相当麻烦）<br />显示调用主要使用WIN32 API函数LoadLibrary、GetProcAddress，举例如下：
```cpp
typedef int (*dllfun)(void);//定义指向dll中函数的函数指针
HINSTANCE hlib = LoadLibrary(".\\dll.dll");
if(!hlib){
    std::cout<<"load dll error\n";
    return -1;
}
dllfun fun = (dllfun)GetProcAddress(hlib,"fndll");
if(!fun){
    std::cout<<"load fun error\n";
    return -1;
}
fun();
```
<a name="k4mXX"></a>
## 生成和使用静态库
### 生成静态库
新建项目--win32项目--填写项目名--确定--下一步--应用程序类型：选择静态库。

静态库项目没有main函数，也没有像dll项目中的dllmain。

创建项目后添加.h文件，添加相应的导出函数、变量或类，如下所示

```cpp
#ifndef _MYLIB_H_
#define _MYLIB_H_

void fun(int a);

extern int k;

class testclass{
    public:
    testclass();
    void print();
};

#endif
```
.cpp文件如下
```cpp

#include "stdafx.h"
#include "lib.h"
#include <iostream>

void fun(int a){
    std::cout<<a<<"lib gen\n";
}

int k = 222;

testclass::testclass(){
    std::cout<<"123\n";
}

void testclass::print(){
    std::cout<<"this is testcalss\n";
}
```
编译工程后就会生成一个.lib文件

### 使用静态库
需要.h文件，lib文件

（1）设置项目属性--vc++目录--库目录为lib所在的路径

（2）将lib添加到项目属性--链接器--输入--附加依赖项（或者直接在源代码中加入`#pragma comment(lib, "**.lib")`）

（3）在源文件中添加.h头文件

然后就像平常一样调用普通函数、类、变量，举例如下：

```cpp
#include <iostream>
#include "lib.h"

#pragma comment(lib, "lib.lib")

int main(){
    fun(4);
    std::cout<<k<<std::endl;
    testclass tc; 
    tc.print();
    return 0;
}
```
