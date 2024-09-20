Java
<a name="Mootx"></a>
## 1、JNA
<a name="QNoXa"></a>
### JNA介绍
JNA（Java Native Access ）提供一组Java工具类用于在运行期动态访问系统本地库（native library：如Window的dll）而不需要编写任何Native/JNI代码。开发人员只要在一个java接口中描述目标native library的函数与结构，JNA将自动实现Java接口到native function的映射。<br />[https://github.com/java-native-access/jna](https://github.com/java-native-access/jna) <br />[https://java-native-access.github.io/jna/5.3.0/javadoc/](https://java-native-access.github.io/jna/5.3.0/javadoc/)

- 优点

JNA可以像调用一般java方法一样直接调用本地方法。就和直接执行本地方法差不多，而且调用本地方法还不用额外的其他处理或者配置什么的，也不需要多余的引用或者编码，使用很方便。
<a name="B7cbZ"></a>
### JNA描述
JNA类库使用一个很小的本地类库sub动态的调用本地代码。程序员只需要使用一个特定的java接口描述一下将要调用的本地代码的方法的结构和一些基本属性。这样就省了为了适配多个平台而大量的配置和编译代码。因为调用的都是JNA提供的公用jar 包中的接口。

- 缺点

JNA是建立在JNI技术基础之上的一个Java类库，原来使用JNI，必须手工用C写一个动态链接库，在C语言中映射Java的数据类型。JNA中，它提供了一个动态的C语言编写的转发器，可以自动实现Java和C的数据类型映射。不再需要编写C动态链接库。当然，这也意味着，使用JNA技术比使用JNI技术调用动态链接库会有些微的性能损失。可能速度会降低几倍。但影响不大。

- 关于jna-platform

其实很多情况下，jna.jar就完全满足一般项目开发的需要了，比如数据 类型的映射和常用的方法等等，这些C/C++中基础的映射已经可以实现，包括一些基本的平台方法，但是，真实涉及到比较深入的平台方法的时候，就需要platform.jar的帮助了，platform.jar是依赖于jna.jar实现的，包括了FileMonitor、FileUtils、KeyboardUtils、WindowUtil等Win32和平台相关的简化动态访问功能类中的大部分常用方法，为开发者开发自己的跨平台映射方法提供参考。<br />所以platform.jar对于jna.jar是一种补充和扩展，jna.jar相当于核，platfrorm.jar相当于增量插件。
<a name="iKRuP"></a>
## 2、JNA使用
<a name="YnYdo"></a>
### pom.xml 引入
```xml
<dependency>
  <groupId>net.java.dev.jna</groupId>
  <artifactId>jna</artifactId>
  <version>5.13.0</version>
</dependency>
<dependency>
  <groupId>net.java.dev.jna</groupId>
  <artifactId>jna-platform</artifactId>
  <version>5.13.0</version>
</dependency>
```
使用的函数必须与链接库中的函数原型保持一致，这是JNA甚至所有跨平台调用的难点，因为C/C++的类型与Java的类型是不一样的，必须转换成java对应类型让它们保持一致，这就是类型映射（Type Mappings），JNA官方给出的默认类型映射表如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678806114517-048024fb-294c-4bb9-b997-a933bdfeb2cf.png#averageHue=%23f9f8f8&clientId=u9461f475-f051-4&from=paste&id=u402a4ab7&originHeight=821&originWidth=561&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaa704bcb-3416-4dc2-aa24-b7cd51b62ff&title=)<br />其中类型映射的难点在于结构体、指针和函数回调。
<a name="P9vDK"></a>
### 创建头文件 TestJna.h
```c
#ifndef _TestJna_h
#define _TestJna_h

#ifdef __cplusplus
extern "C" {
#endif

    int add(int a, int b);

    int sub(int a, int b);

#ifdef __cplusplus
}
#endif
#endif
```
<a name="n3M8n"></a>
### 创建头文件 TestJna.cpp
```cpp
#include "TestJna.h"

extern "C" int add(int a, int b){
    return a+b;
}

extern "C" int sub(int a, int b){
    return a-b;
}
```
**注意：**一定要加`extern "C"`否则生成的方法名跟cpp文件定义的方法名不一致。<br />查看so方法名
```bash
nm -D libTestJnaEx.so | grep add
nm -D libTestJnaEx.so | grep sub
```
<a name="vsm6G"></a>
### 生成动态连接库so文件
```bash
g++ TestJna.cpp -fPIC -shared -o libTestJnaEx.so
```
<a name="OCkjY"></a>
### 拷贝so文件到/usr/lib目录
```bash
cp libTestJnaEx.so /usr/lib
```
<a name="gRsBf"></a>
### 创建TestJna.java文件
```java
import com.sun.jna.Library;
import com.sun.jna.Native;

public class TestJnaEx {

    public interface CLibrary extends Library {
        CLibrary INSTANCE = (CLibrary) Native.load("TestJnaEx", CLibrary.class);

        int add(int a, int b);

        int sub(int a, int b);
    }

    public static void main(String[] args) {
        int sum = CLibrary.INSTANCE.add(2, 3);
        System.out.println(sum);

        int diff = CLibrary.INSTANCE.sub(5, 3);
        System.out.println(diff);
    }

}
```
导出可执行jar文件TestJnaEx.jar
<a name="sy7mh"></a>
### 执行TestJnaEx.jar
```bash
java -jar TestJnaEx.jar
```
结果![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678806114632-8b5e62fd-8d27-4ad3-b419-fc5904e6bc62.png#averageHue=%23070504&clientId=u9461f475-f051-4&from=paste&id=u721f8071&originHeight=105&originWidth=786&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u232f9f44-27f0-4c97-96ca-e6358d0010f&title=)
