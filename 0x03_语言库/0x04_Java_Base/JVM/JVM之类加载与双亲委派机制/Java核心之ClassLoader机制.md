Java ClassLoader
<a name="ccQ9w"></a>
## 类加载机制
Java中的源码.java后缀文件会在运行前被编译成.class后缀文件，文件内的字节码的本质就是一个字节数组 ，它有特定的复杂的内部格式，Java类初始化的时候会调用`java.lang.ClassLoader`加载字节码，.class文件中保存着Java代码经转换后的虚拟机指令，当需要使用某个类时，虚拟机将会加载它的.class文件，并创建对应的class对象，将class文件加载到虚拟机的内存，而在JVM中类的查找与装载就是由`ClassLoader`完成的，而程序在启动的时候，并不会一次性加载程序所要用的所有class文件，而是根据程序的需要，来动态加载某个class文件到内存当中的，从而只有class文件被载入到了内存之后，才能被其它class所引用。所以`ClassLoader`就是用来动态加载class文件到内存当中用的。
<a name="sGMJV"></a>
## 类加载方式
Java类加载方式分为显式和隐式<br />显式：利用反射或`ClassLoader`来动态加载一个类<br />隐式：通过new 一个类或者 类名.方法名返回一个类<br />示例代码
```java
@Test
public void loadClassTest() throws Exception {
    //1、反射加载
    Class<?> aClass = Class.forName("java.lang.Runtime");
    System.out.println(aClass.getName());

    //2、ClassLoader加载
    Class<?> aClass1 = ClassLoader.getSystemClassLoader().loadClass("java.lang.ProcessBuilder");
    System.out.println(aClass1.getName());

}
```
那也就是其实可以通过`ClassLoader.loadClass()`代替`Class.forName()`来获取某个类的class对象。
<a name="MwVn8"></a>
## `ClassLoader`
`ClassLoader`（类加载器）主要作用就是将class文件读入内存，并为之生成对应的java.lang.Class对象<br />JVM中存在3个内置`ClassLoader`：

1. `BootstrapClassLoader` 启动类加载器 负责加载 JVM 运行时核心类，这些类位于 JAVA_HOME/lib/rt.jar 文件中，常用内置库 java.xxx.* 都在里面，比如 java.util.`、`java.io.、java.nio.`、`java.lang. 等等。
2. `ExtensionClassLoader` 扩展类加载器 负责加载 JVM 扩展类，比如 swing 系列、内置的 js 引擎、xml 解析器 等等，这些库名通常以 javax 开头，它们的 jar 包位于 JAVA_HOME/lib/ext/*.jar 中
3. `AppClassLoader` 系统类加载器 才是直接面向用户的加载器，它会加载 Classpath 环境变量里定义的路径中的 jar 包和目录。自己编写的代码以及使用的第三方 jar 包通常都是由它来加载的。

除了Java自带的`ClassLoader`外，还可以自定义`ClassLoader`，自定义的`ClassLoader`都必须继承自`java.lang.ClassLoader`类，也包括Java提供的另外二个`ClassLoader`（Extension ClassLoader和App `ClassLoader`）在内，但是Bootstrap ClassLoader不继承自`ClassLoader`，因为它不是一个普通的Java类，底层由C++编写，已嵌入到了JVM内核当中，当JVM启动后，Bootstrap ClassLoader也随着启动，负责加载完核心类库后，并构造Extension ClassLoader和App ClassLoader类加载器。
<a name="Fa55k"></a>
## 类加载流程
类加载指的是在.java文件编译成.class字节码文件后，当需要使用某个类时，虚拟机将会加载它的.class文件，将.class文件读入内存，并在内存中为之创建一个java.lang.Class对象。但是实现步骤看起来会比较空洞和概念化，暂时不去深入研究，理解类加载是做什么的并了解加载过程即可。后续有刚需再去深入。<br />类加载大致分为三个步骤：加载、连接、初始化。
<a name="We7Yc"></a>
### 1、加载
类加载指的是将class文件读入内存，并为之创建一个java.lang.Class对象，即程序中使用任何类时，也就是任何类在加载进内存时，系统都会为之建立一个java.lang.Class对象，这个Class对象包含了该类的所有信息，如Filed，Method等，系统中所有的类都是java.lang.Class的实例。<br />类的加载由类加载器完成，JVM提供的类加载器叫做系统类加载器，此外还可以通过自定义类加载器加载。<br />通常可以用如下几种方式加载类的二进制数据：<br />从本地文件系统加载class文件。<br />从JAR包中加载class文件，如JAR包的数据库启驱动类。<br />通过网络加载class文件。<br />把一个Java源文件动态编译并执行加载。
<a name="xxeFM"></a>
### 2、链接
链接阶段负责把类的二进制数据合并到JRE中，其又可分为如下三个阶段：

1. 验证：确保加载的类信息符合JVM规范，无安全方面的问题。
2. 准备：为类的静态Field分配内存，并设置初始值。
3. 解析：将类的二进制数据中的符号引用替换成直接引用。
<a name="KEEhZ"></a>
### 3、初始化
类加载最后阶段，若该类具有超类，则对其进行初始化，执行静态初始化器和静态初始化成员变量(如前面只初始化了默认值的static变量将会在这个阶段赋值，成员变量也将被初始化
<a name="ELkdM"></a>
## 双亲委派机制
<a name="MJ98C"></a>
### 基本概念
前面提到了Java自带3个`ClassLoader`，包括实现自定义`ClassLoader`完成类加载，但是具体某个类的加载用的是哪个`ClassLoader`呢。这里涉及到一个双亲委派机制<br />这个图基本概述了双亲委派机制（先走蓝色箭头再走红色箭头）<br />![2022-01-05-08-59-57-693355.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1641344594003-2235e8e6-d3ed-452e-8a44-cd85ce67d6b5.png#clientId=u56b76aa9-4ef3-4&from=ui&id=u0dcb0b5d&originHeight=1030&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3343703&status=done&style=shadow&taskId=u9d590ea8-2ec0-4cbe-b699-69a5e4d69fd&title=)<br />双亲委派简单理解：向上委派，向下加载<br />当一个.class文件要被加载时。不考虑自定义类加载器，首先会在`AppClassLoader`中检查是否加载过，如果有那就无需再加载了。如果没有，那么会拿到父加载器，然后调用父加载器的`loadClass`方法。父类中同理也会先检查自己是否已经加载过，如果没有再往上。注意这个类似递归的过程，直到到达Bootstrap classLoader之前，都是在检查是否加载过，并不会选择自己去加载。直到BootstrapClassLoader，已经没有父加载器了，这时候开始考虑自己是否能加载了（向上委派）；如果自己无法加载，会下沉到子加载器去加载，一直到最底层（向下加载）。如果没有任何加载器能加载，就会抛出ClassNotFoundException异常。
<a name="JIzpi"></a>
### 为什么？
那么为什么加载类的时候需要双亲委派机制呢？<br />采用双亲委派模式的是好处是Java类随着它的类加载器一起具备了一种带有优先级的层次关系，通过这种层级关可以避免类的重复加载，当父亲已经加载了该类时，就没有必要子`ClassLoader`再加载一次。<br />其次是，如果有人想替换系统级别的类：String.java。篡改它的实现，在这种机制下这些系统的类已经被Bootstrap classLoader加载过了（为什么？因为当一个类需要加载的时候，最先去尝试加载的就是BootstrapClassLoader），所以其他类加载器并没有机会再去加载，从一定程度上防止了危险代码的植入。
<a name="K3n3n"></a>
## 自定义ClassLoader
先看下`ClassLoader`这个类中的核心方法
<a name="Pqmkz"></a>
### `ClassLoader`核心方法

1. `loadClass(加载指定的Java类)`一般实现这个方法的步骤是：执行`findLoadedClass(String)`去检测这个class是不是已经加载过了。<br />执行父加载器的`loadClass`方法。如果父加载器为null则jvm内置的加载器去替代，也就是Bootstrap ClassLoader。这也解释了`ExtClassLoader`的parent为null，但仍然说`Bootstrap ClassLoader`是它的父加载器。如果向上委托父加载器没有加载成功；则通过`findClass(String)`查找。<br />如果class在上面的步骤中找到了，参数resolve又是true的话那么`loadClass()`又会调用`resolveClass(Class)`这个方法来生成最终的Class对象。
2. `findClass(查找指定的Java类)`
3. `findLoadedClass(查找JVM已经加载过的类)`
4. `defineClass(定义一个Java类)`
5. `resolveClass(链接指定的Java类)`
<a name="VxIHm"></a>
### 编写自定义`ClassLoader`步骤
1、编写一个类继承`ClassLoader`抽象类；<br />2、重写`findClass()`方法；<br />3、在`findClass()`方法中调用`defineClass()`方法即可实现自定义`ClassLoader`；
<a name="O7Kem"></a>
#### 1、编写测试类
```java
package classloader;

public class test {
    public String hello(){
        return "hello, CoLoo!";
    }
}
```
<a name="a92jE"></a>
#### 2、编译为`.class`文件
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641344381446-632846ff-b247-4049-8ce9-f2bbb20213de.webp#clientId=u56b76aa9-4ef3-4&from=paste&id=u0f4aac8c&originHeight=328&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8302eda2-a67d-4b97-890d-e3e36e74d18&title=)
<a name="EvxD2"></a>
#### 3、`.class`转换bytes
```java
public class ByteClass {
    public static void main(String[] args) throws IOException {
        FileInputStream fis = new FileInputStream("test.class");
        byte[] classBytes = IOUtils.readFully(fis, -1, false);
        System.out.println(Arrays.toString(classBytes));
    }
}
```
Output：
```java
[-54, -2, -70, -66, 0, 0, 0, 52, 0, 17, 10, 0, 4, 0, 13, 8, 0, 14, 7, 0, 15, 7, 0, 16, 1, 0, 6, 60, 105, 110, 105, 116, 62, 1, 0, 3, 40, 41, 86, 1, 0, 4, 67, 111, 100, 101, 1, 0, 15, 76, 105, 110, 101, 78, 117, 109, 98, 101, 114, 84, 97, 98, 108, 101, 1, 0, 5, 104, 101, 108, 108, 111, 1, 0, 20, 40, 41, 76, 106, 97, 118, 97, 47, 108, 97, 110, 103, 47, 83, 116, 114, 105, 110, 103, 59, 1, 0, 10, 83, 111, 117, 114, 99, 101, 70, 105, 108, 101, 1, 0, 9, 116, 101, 115, 116, 46, 106, 97, 118, 97, 12, 0, 5, 0, 6, 1, 0, 13, 104, 101, 108, 108, 111, 44, 32, 67, 111, 76, 111, 111, 33, 1, 0, 16, 99, 108, 97, 115, 115, 108, 111, 97, 100, 101, 114, 47, 116, 101, 115, 116, 1, 0, 16, 106, 97, 118, 97, 47, 108, 97, 110, 103, 47, 79, 98, 106, 101, 99, 116, 0, 33, 0, 3, 0, 4, 0, 0, 0, 0, 0, 2, 0, 1, 0, 5, 0, 6, 0, 1, 0, 7, 0, 0, 0, 29, 0, 1, 0, 1, 0, 0, 0, 5, 42, -73, 0, 1, -79, 0, 0, 0, 1, 0, 8, 0, 0, 0, 6, 0, 1, 0, 0, 0, 3, 0, 1, 0, 9, 0, 10, 0, 1, 0, 7, 0, 0, 0, 27, 0, 1, 0, 1, 0, 0, 0, 3, 18, 2, -80, 0, 0, 0, 1, 0, 8, 0, 0, 0, 6, 0, 1, 0, 0, 0, 5, 0, 1, 0, 11, 0, 0, 0, 2, 0, 12]
```
<a name="MYV1Y"></a>
#### 4、自定义`ClassLoader`
```java
package classloader;

import java.lang.reflect.Method;

public class ClassLoaderTest extends ClassLoader {

    private static String className = "classloader.test";
    //转换byte后的字节码
    private static byte[] classBytes = new byte[]{54, -2, -70, -66, 0, 0, 0, 52, 0, 17, 10, 0, 4, 0, 13, 8, 0, 14, 7, 0, 15, 7, 0, 16, 1, 0, 6, 60, 105, 110, 105, 116, 62, 1, 0, 3, 40, 41, 86, 1, 0, 4, 67, 111, 100, 101, 1, 0, 15, 76, 105, 110, 101, 78, 117, 109, 98, 101, 114, 84, 97, 98, 108, 101, 1, 0, 5, 104, 101, 108, 108, 111, 1, 0, 20, 40, 41, 76, 106, 97, 118, 97, 47, 108, 97, 110, 103, 47, 83, 116, 114, 105, 110, 103, 59, 1, 0, 10, 83, 111, 117, 114, 99, 101, 70, 105, 108, 101, 1, 0, 9, 116, 101, 115, 116, 46, 106, 97, 118, 97, 12, 0, 5, 0, 6, 1, 0, 13, 104, 101, 108, 108, 111, 44, 32, 67, 111, 76, 111, 111, 33, 1, 0, 16, 99, 108, 97, 115, 115, 108, 111, 97, 100, 101, 114, 47, 116, 101, 115, 116, 1, 0, 16, 106, 97, 118, 97, 47, 108, 97, 110, 103, 47, 79, 98, 106, 101, 99, 116, 0, 33, 0, 3, 0, 4, 0, 0, 0, 0, 0, 2, 0, 1, 0, 5, 0, 6, 0, 1, 0, 7, 0, 0, 0, 29, 0, 1, 0, 1, 0, 0, 0, 5, 42, -73, 0, 1, -79, 0, 0, 0, 1, 0, 8, 0, 0, 0, 6, 0, 1, 0, 0, 0, 3, 0, 1, 0, 9, 0, 10, 0, 1, 0, 7, 0, 0, 0, 27, 0, 1, 0, 1, 0, 0, 0, 3, 18, 2, -80, 0, 0, 0, 1, 0, 8, 0, 0, 0, 6, 0, 1, 0, 0, 0, 5, 0, 1, 0, 11, 0, 0, 0, 2, 0, 12};

    @Override
    protected Class<?> findClass(String name) throws ClassNotFoundException {
        //只处理classloader.test类
        if (name.equals(className)) {
            //调用definClass将一个字节流定义为一个类。
            return defineClass(className, classBytes, 0, classBytes.length);
        }
        return super.findClass(name);
    }

    public static void main(String[] args) throws Exception {
        //创建加载器
        ClassLoaderTest clt = new ClassLoaderTest();
        //使用我们自定义的类去加载className
        Class clazz = clt.loadClass(className);
        //反射创建test类对象
        Object test = clazz.newInstance();
        //反射获取方法
        Method method = test.getClass().getMethod("hello");
        //反射去调用执行方法
        String str = (String) method.invoke(test);
        System.out.println(str);

    }
}
```
执行了test类的hello方法<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641344381461-3aeaad87-a745-40f1-82f1-7c57fd4715c8.webp#clientId=u56b76aa9-4ef3-4&from=paste&id=ua45d3c20&originHeight=878&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ube077826-7b10-450f-a10b-9346adcd267&title=)
<a name="qf9l9"></a>
## 总结
<a name="BuOsv"></a>
### 自定义`ClassLoader`流程小结

1. 准备自定义类，编译为.class文件
2. 将.class文件内容专为bytes数组
3. 自定义`ClassLoader`，继承`ClassLoader`类，重写`findClass()`方法，在方法中定义对指定类的处理流程
4. 主函数创建自定义`ClassLoader`对象并`loadClass()`指定类，如果自定义的`ClassLoader`完成了加载则会获得该类的class对象，后续可通过反射来深入利用（如执行某个方法）。
<a name="nCYZd"></a>
### `ClassLoader`对于安全来说能做什么？
1、这里目前想到的是代替`Class.forName()`，通过`ClassLoader.loadClass()`获取class对象
```java
@Test
public void classLoaderRuntime() throws Exception {
    Class<?> aClass = ClassLoader.getSystemClassLoader().loadClass("java.lang.Runtime");
    Runtime runtime = (Runtime) aClass.getMethod("getRuntime").invoke(aClass);
    runtime.exec("open -a Calculator");

}
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641344381692-a0e711f9-28bc-4e33-a5fd-357ebc8e6c7f.webp#clientId=u56b76aa9-4ef3-4&from=paste&id=ua8803df4&originHeight=554&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud5c743ea-ece8-4f9b-899a-b6d55ff1301&title=)<br />2、加载恶意类<br />如webshell中（之前有次攻防捕捉到一个webshell里面用到了classloader）或者内存马中应该也可以用到，待研究。<br />3、… 应该还有很多。
