Java 关键字 native<br />`native` 关键字在 JDK 源码中很多类中都有，在 Object.java类中，其 `getClass()` 方法、`hashCode()`方法、`clone()` 方法等等都是用 `native` 关键字修饰的。
```java
public final native Class<?> getClass();
public native int hashCode();
protected native Object clone() throws CloneNotSupportedException;
```
那么为什么要用 `native` 来修饰方法，这样做有什么用？
<a name="KnbfX"></a>
### 1、JNI：Java Native Interface
在介绍 `native` 之前，先了解什么是 JNI。<br />一般情况下，完全可以使用 Java 语言编写程序，但某些情况下，Java 可能会不满足应用程序的需求，或者是不能更好的满足需求，比如：<br />①、标准的 Java 类库不支持应用程序平台所需的平台相关功能。<br />②、已经用另一种语言编写了一个类库，如何用Java代码调用？<br />③、某些运行次数特别多的方法代码，为了加快性能，需要用更接近硬件的语言（比如汇编）编写。<br />上面这三种需求，其实说到底就是如何用 Java 代码调用不同语言编写的代码。那么 JNI 应运而生了。<br />从Java 1.1开始，Java Native Interface (JNI)标准就成为java平台的一部分，它允许Java代码和其他语言写的代码进行交互。JNI一开始是为了本地已编译语言，尤其是C和C++而设计 的，但是它并不妨碍使用其他语言，只要调用约定受支持就可以了。使用java与本地已编译的代码交互，通常会丧失平台可移植性。但是，有些情况下这样做是可以接受的，甚至是必须的，比如，使用一些旧的库，与硬件、操作系统进行交互，或者为了提高程序的性能。JNI标准至少保证本地代码能工作在任何Java 虚拟机实现下。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680281364-c33a95b4-1faa-4bb2-ade8-d4d2a11ddb91.png#clientId=ucdd3db9a-b375-4&from=paste&id=u060beb08&originHeight=136&originWidth=326&originalType=url&ratio=1&status=done&style=shadow&taskId=u4163e0d2-22fe-415b-8213-17c0df81c57)<br />通过 JNI，就可以通过 Java 程序（代码）调用到操作系统相关的技术实现的库函数，从而与其他技术和系统交互，使用其他技术实现的系统的功能；同时其他技术和系统也可以通过 JNI 提供的相应原生接口开调用 Java 应用系统内部实现的功能。<br />在windows系统上，一般可执行的应用程序都是基于 native 的PE结构，windows上的 JVM 也是基于native结构实现的。Java应用体系都是构建于 JVM 之上。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680281441-b1bfb631-19a5-498d-99a5-134a455b75c4.png#clientId=ucdd3db9a-b375-4&from=paste&id=ueb02981d&originHeight=311&originWidth=511&originalType=url&ratio=1&status=done&style=shadow&taskId=u112f5960-db1b-4f81-918b-5466b2d9b77)<br />可能有人会问，Java不是跨平台的吗？如果用 JNI，那么程序不就将失去跨平台的优点?确实是这样的。<br />**JNI 的缺点：**<br />①、程序不再跨平台。要想跨平台，必须在不同的系统环境下重新编译本地语言部分。<br />②、程序不再是绝对安全的，本地代码的不当使用可能导致整个程序崩溃。一个通用规则是，应该让本地方法集中在少数几个类当中。这样就降低了JAVA和C之间的耦合性。<br />目前来讲使用 JNI 的缺点相对于优点还是可以接受的，可能后面随着 Java 的技术发展，不在需要 JNI，但是目前 JDK 还是一直提供对 JNI 标准的支持。
<a name="LGQt6"></a>
### 2、用C语言编写程序本地方法
上面讲解了什么是 JNI，那么接下来就写个例子，如何用 Java 代码调用本地的 C 程序。<br />官方文档如下：[https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/jniTOC.html](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/jniTOC.html)<br />步骤如下：<br />①、编写带有 **native** 声明的方法的java类，生成.java文件；**(注意这里出现了 native 声明的方法关键字）**<br />②、使用 **javac** 命令编译所编写的java类，生成.class文件；<br />③、使用** **`**javah -jni  java类名**` 生成扩展名为 h 的头文件，也即生成.h文件；<br />④、使用C/C++（或者其他编程想语言）实现本地方法，创建.h文件的实现，也就是创建.cpp文件实现.h文件中的方法；<br />⑤、将C/C++编写的文件生成动态连接库，生成dll文件；<br />下面通过一个 HelloWorld 程序的调用来完成这几个步骤。<br />注意：下面所有操作都是在所有操作都是在目录：D:\JNI 下进行的。
<a name="alukv"></a>
#### 一、编写带有 `native` 声明的方法的java类
```java
public class HelloJNI {
    //native 关键字告诉 JVM 调用的是该方法在外部定义
    private native void helloJNI();

    static{
        System.loadLibrary("helloJNI");//载入本地库
    }
    public static void main(String[] args) {
        HelloJNI jni = new HelloJNI();
        jni.helloJNI();
    }

}
```
用 native 声明的方法表示告知 JVM 调用，该方法在外部定义，也就是会用 C 语言去实现。<br />`System.loadLibrary("helloJNI");`加载动态库，参数 helloJNI 是动态库的名字。可以这样理解：程序中的方法 helloJNI() 在程序中没有实现，但是下面要调用这个方法，怎么办呢？就需要对这个方法进行初始化，所以用 static 代码块进行初始化。<br />这时候如果直接运行该程序，会报“A Java Exception has occurred”错误：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680281361-fcae31ab-0399-47b6-821b-f73c0cb796da.png#clientId=ucdd3db9a-b375-4&from=paste&id=uedd4d97a&originHeight=155&originWidth=1293&originalType=url&ratio=1&status=done&style=shadow&taskId=uf410fe4e-eb72-4629-b190-b8b5f63b4bd)
<a name="vZ4rc"></a>
#### 二、使用 `javac` 命令编译所编写的java类，生成.class文件
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680281474-6eeb8f88-3e7e-431a-8d63-fe37986e75e0.png#clientId=ucdd3db9a-b375-4&from=paste&id=ub4b0d758&originHeight=47&originWidth=301&originalType=url&ratio=1&status=done&style=none&taskId=u2abd7729-6700-4165-847d-a1bf797516f)<br />执行上述命令后，生成 HelloJNI.class 文件：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680281603-b96bae8e-ac5d-4124-905c-07f62f4c1069.png#clientId=ucdd3db9a-b375-4&from=paste&id=u4f1230b8&originHeight=117&originWidth=771&originalType=url&ratio=1&status=done&style=shadow&taskId=u2a672021-f18b-4c1c-9b5e-80d8e90e867)
<a name="uxPJ5"></a>
#### 三、使用 `javah -jni  java类名` 生成扩展名为 h 的头文件
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680282121-8594bf2e-97fa-4c91-92ff-dce2c7a069d4.png#clientId=ucdd3db9a-b375-4&from=paste&id=u3ead4274&originHeight=41&originWidth=284&originalType=url&ratio=1&status=done&style=none&taskId=ufe6c44ee-6234-40a8-846b-6f1a89b652d)<br />执行上述命令后，在 D:/JNI 目录下多出了个 HelloJNI.h 文件：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680282026-01e47890-32c7-4574-b405-fc9f191d1184.png#clientId=ucdd3db9a-b375-4&from=paste&id=u84c3e216&originHeight=173&originWidth=775&originalType=url&ratio=1&status=done&style=shadow&taskId=u7b18b6aa-1841-4cf5-8c23-f4122ff56f8)
<a name="szqq5"></a>
#### 四、使用C语言实现本地方法
需要在 windows平台安装 gcc。<br />注意安装版本的选择，根据系统是32位还是64位来选择。[64位点击下载](https://sourceforge.net/projects/mingw-w64/files/latest/download?source=files)。<br />安装完成之后注意配置环境变量，在 cmd 中输入 `g++ -v`，如果出现如下信息，则安装配置完成：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680282204-d942ee06-0aaa-4c5c-b2be-43f5fff989f9.png#clientId=ucdd3db9a-b375-4&from=paste&id=uec0ae506&originHeight=268&originWidth=838&originalType=url&ratio=1&status=done&style=none&taskId=u7af72c18-a62d-44e7-ae59-f20915fc2d0)<br />接着输入如下命令：
```bash
gcc -m64  -Wl,--add-stdcall-alias -I"C:\Program Files\Java\jdk1.8.0_152\include" -I"C:\Program Files\Java\jdk1.8.0_152\include\include\win32" -shared -o helloJNI.dll helloJNI.c
```
`-m64`表示生成dll库是64位的。后面的路径表示本机安装的JDK路径。生成之后多了一个helloJNI.dll 文件<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680282103-388d9d45-fd9d-4b59-87c5-b0fc1516ac17.png#clientId=ucdd3db9a-b375-4&from=paste&id=ua4cafe44&originHeight=212&originWidth=777&originalType=url&ratio=1&status=done&style=shadow&taskId=ube5355b9-cbc2-4457-a9a6-fc1f69bb83a)<br />最后运行 HelloJNI：输出 Hello JNI! **大功告成。**<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680282504-70b9ed39-7c7f-4cb8-a113-ec14324f3bbb.png#clientId=ucdd3db9a-b375-4&from=paste&id=ub1a797a0&originHeight=167&originWidth=1198&originalType=url&ratio=1&status=done&style=none&taskId=u90aaaa7b-279e-42d3-b316-8b3206f53b8)
<a name="bJMvN"></a>
### 3、JNI调用C的流程图
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1633680282804-12cc8638-b52d-45cd-8d97-f166d666d48e.png#clientId=ucdd3db9a-b375-4&from=paste&id=udbbaf9aa&originHeight=789&originWidth=660&originalType=url&ratio=1&status=done&style=shadow&taskId=u96a22b15-e5c2-4c1a-a03f-c2b3f39c8fa)
<a name="K3KLm"></a>
### 4、`native`关键字
`native` 用来修饰方法，用 `native` 声明的方法表示告知 JVM 调用，该方法在外部定义，可以用任何语言去实现它。 简单地讲，一个`native` Method就是一个 Java 调用非 Java 代码的接口。<br />`native` 语法：<br />①、修饰方法的位置必须在返回类型之前，和其余的方法控制符前后关系不受限制。<br />②、不能用 `abstract` 修饰，也没有方法体，也没有左右大括号。<br />③、返回值可以是任意类型<br />在日常编程中看到`native`修饰的方法，只需要知道这个方法的作用是什么，至于别的就不用管了，操作系统会实现。
