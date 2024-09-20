Java 类加载 ClassLoader
<a name="FZ2e4"></a>
## 类加载过程
当程序主动使用某个类时，如果该类还未被加载到内存中，则系统会通过如下三个步骤对该类进行初始化：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605159540730-9db24c6c-4f30-46cc-ad47-9dc9616594c7.png#height=227&id=pJufh&originHeight=681&originWidth=1819&originalType=binary&ratio=1&rotation=0&showTitle=false&size=134807&status=done&style=shadow&title=&width=606.3333333333334)

- 加载：将class文件字节码内容加载到内存，并将这些静态数据转换成方法区的运行时数据结构，然后生成一个代表这个类的 `java.lang.Class` 对象。
- 链接：将Java类的二进制代码合并到JVM的运行状态之中的过程。
- 验证：确保加载的类信息符合JVM规范，没有安全方面的问题
- 准备：正式为类变量(`static`)分配内存并设置类变量默认初始值的阶段，这些内存都将在方法区中进行分配。
- 解析：虚拟机常量池的符号引用(常量名)替换为直接引用(地址)的过程
- 初始化：
- 执行类构造器方法的过程，类构造器 方法是由编译期自动收集类中所有类变量的赋值动作和静态代码块中的语句合并产生的。（类构造器是构造类信息的，不是构造该类对象的构造器）
- 当初始化一个类的时候，如果发现其父类还没有初始化完成，则需要先触发其父类的初始化
- 虚拟机会保证一个类的方法在多相差环境中被正确的加锁和同步

下面一段代码，分别说明了static代码块，以及子类和父类构造方法的执行流程
```java
/**
 * 类加载流程
 *
 * @author: Fcant
 */
class SuperA {
    static {
        System.out.println("父类静态代码块初始化");
    }
    public SuperA() {
        System.out.println("父类构造函数初始化");
    }
}
class A extends SuperA{
    static {
        System.out.println("静态代码块初始化");
        m = 300;
    }
    static int m = 100;
    public A() {
        System.out.println("A类的无参构造方法");
    }
}
public class ClassLoaderDemo {
    public static void main(String[] args) {
        A a = new A();
        System.out.println(a.m);
    }
}
```
最后的结果为：
```
父类静态代码块初始化
静态代码块初始化
父类构造函数初始化
A类的无参构造方法
100
```
说明静态代码块都是执行的，并且父类优先
<a name="EdJvC"></a>
## 类加载步骤

- 加载到内存，会产生一个类对应Class对象
- 链接，链接结束 m = 0
- 初始化：
```
<clinit>() {
 syso("A类静态方法")
 m = 300;
 m = 100;
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605160062724-092f41ae-3c27-4313-bdcf-597a1aed2065.png#height=369&id=d7rcp&originHeight=1107&originWidth=2057&originalType=binary&ratio=1&rotation=0&showTitle=false&size=236090&status=done&style=shadow&title=&width=685.6666666666666)
<a name="KB8ly"></a>
## 什么时候发生类初始化
<a name="7s15r"></a>
### 类的主动引用（一定发生初始化）

- 当虚拟机启动，先初始化main方法所有在类
- new 一个类的对象
- 调用类的静态成员（除了 final常量）和静态方法
- 使用 java.lang.reflect包的方法对类进行反射调用
- 当初始化一个类，如果其父类没有被初始化，则会先初始化它的父类
<a name="YTguM"></a>
### 类的被动引用（不会发生初始化）

- 当访问一个静态域时，只有真正的申明这个域的类才会被初始化，如：当通过子类引用父类的静态变量，不会导致子类初始化
- 通过数组定义类引用，不会触发此类的初始化
- 引用常量不会触发此类的初始化（常量在链接阶段就存入调用类的常量池了）
<a name="8wyBz"></a>
## 类加载器的作用

- 类加载的作用：将class文件字节码内容加载到内存中，并将这些静态数据转换成方法区的运行时数据结构，然后在堆中生成了一个代表这个类的 `java.lang.Class`对象，作为方法区中类数据的访问入口。
- 类缓存：标准的JavaSE类加载器可以按要求查找类，但是一旦某个类被加载到类加载器中，它将维持加载（缓存）一段时间。不过JVM垃圾回收机制可以回收这些Class对象

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605160436406-19d6441e-9169-4d9c-a9cf-0d9587bbc84b.png#height=363&id=Etpu4&originHeight=1088&originWidth=1880&originalType=binary&ratio=1&rotation=0&showTitle=false&size=122326&status=done&style=shadow&title=&width=626.6666666666666)<br />类加载器作用是用来把类（Class）装载进内存的，JVM规范定义了如下类型的类的加载器<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605161018221-3a1d8bb0-4aaa-4723-8eda-66d6cd8225a3.png#height=312&id=Y9KO4&originHeight=937&originWidth=2170&originalType=binary&ratio=1&rotation=0&showTitle=false&size=250388&status=done&style=shadow&title=&width=723.3333333333334)<br />代码如下：
```java
/**
 * 类加载器的种类
 *
 * @author: Fcant
 */
public class ClassLoaderTypeDemo {
    public static void main(String[] args) {
        //当前类是哪个加载器
        ClassLoader loader = ClassLoaderTypeDemo.class.getClassLoader();
        System.out.println(loader);
        // 获取系统类加载器
        ClassLoader classLoader = ClassLoader.getSystemClassLoader();
        System.out.println(classLoader);
        // 获取系统类加载器的父类加载器 -> 扩展类加载器
        ClassLoader parentClassLoader = classLoader.getParent();
        System.out.println(parentClassLoader);
        // 获取扩展类加载器的父类加载器 -> 根加载器（C、C++）
        ClassLoader superParentClassLoader = parentClassLoader.getParent();
        System.out.println(superParentClassLoader);
        // 测试JDK内置类是谁加载的
        ClassLoader loader2 = Object.class.getClassLoader();
        System.out.println(loader2);
    }
}
```
运行结果：可以发现，根加载器无法获取到
```
sun.misc.Launcher$AppClassLoader@18b4aac2
sun.misc.Launcher$AppClassLoader@18b4aac2
sun.misc.Launcher$ExtClassLoader@45ee12a7
null
null
```
获取类加载器能够加载的路径
```
// 如何获取类加载器可以加载的路径
System.out.println(System.getProperty("java.class.path"));
```
最后输出结果为：
```java
// 获取类加载器可以加载的路径
        System.out.println(System.getProperty("java.class.path"));
        /*
        E:\Software\JDK1.8\Java\jre\lib\charsets.jar;
        E:\Software\JDK1.8\Java\jre\lib\deploy.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\access-bridge-64.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\cldrdata.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\dnsns.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\jaccess.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\jfxrt.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\localedata.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\nashorn.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\sunec.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\sunjce_provider.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\sunmscapi.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\sunpkcs11.jar;
        E:\Software\JDK1.8\Java\jre\lib\ext\zipfs.jar;
        E:\Software\JDK1.8\Java\jre\lib\javaws.jar;
        E:\Software\JDK1.8\Java\jre\lib\jce.jar;
        E:\Software\JDK1.8\Java\jre\lib\jfr.jar;
        E:\Software\JDK1.8\Java\jre\lib\jfxswt.jar;
        E:\Software\JDK1.8\Java\jre\lib\jsse.jar;
        E:\Software\JDK1.8\Java\jre\lib\management-agent.jar;
        E:\Software\JDK1.8\Java\jre\lib\plugin.jar;
        E:\Software\JDK1.8\Java\jre\lib\resources.jar;
        E:\Software\JDK1.8\Java\jre\lib\rt.jar;
        C:\Users\Administrator\Desktop\LearningNotes\校招面试\JUC\Code\target\classes;
        C:\Users\Administrator\.m2\repository\org\projectlombok\lombok\1.18.10\lombok-1.18.10.jar;
        C:\Users\Administrator\.m2\repository\cglib\cglib\3.3.0\cglib-3.3.0.jar;
        C:\Users\Administrator\.m2\repository\org\ow2\asm\asm\7.1\asm-7.1.jar;
        E:\Software\IntelliJ IDEA\IntelliJ IDEA 2019.1.2\lib\idea_rt.jar
         */
```
能够发现，类在加载的时候，都是有自己的加载区域的，而不是任何地方的类都能够被加载。
<a name="yi3QI"></a>
## 获取运行时候类的完整结构
通过反射能够获取运行时类的完整结构

- 实现的全部接口
- 所继承的父类
- 全部的构造器
- 全部的方法
- 全部的Field
- 注解
```java
/**
 * 获取运行时类信息
 * @author: Fcant
 */
public class GetClassInfo {
    public static void main(String[] args) throws ClassNotFoundException, NoSuchFieldException, NoSuchMethodException {
        Class clazz = Class.forName("com.moxi.interview.study.annotation.User");
        // 获取类名字
        System.out.println(clazz.getName()); // 包名 + 类名
        System.out.println(clazz.getSimpleName()); // 类名
        // 获取类属性
        System.out.println("================");
        // 只能找到public属性
        Field [] fields = clazz.getFields();
        // 找到全部的属性
        Field [] fieldAll = clazz.getDeclaredFields();
        for (int i = 0; i < fieldAll.length; i++) {
            System.out.println(fieldAll[i]);
        }
        // 获取指定属性的值
        Field name = clazz.getDeclaredField("name");
        // 获取方法
        Method [] methods = clazz.getDeclaredMethods(); // 获取本类和父类的所有public方法
        Method [] methods2 = clazz.getMethods(); // 获取本类所有方法
        // 获得指定方法
        Method method = clazz.getDeclaredMethod("getName", null);
        // 获取方法的时候，可以把参数也丢进去，这样因为避免方法重载，而造成不知道加载那个方法
        Method method2 = clazz.getDeclaredMethod("setName", String.class);
    }
}
```
<a name="ee5ea972"></a>
## 双亲委派机制
如果想定义一个：java.lang.string 包，可以发现无法创建<br />因为类在加载的时候，会逐级往上<br />也就是说当前的系统加载器，不会马上的创建该类，而是将该类委派给扩展类加载器，扩展类加载器在委派为根加载器，然后引导类加载器去看这个类在不在能访问的路径下，发现 sring包已经存在了，所以就无法进行，也就是无法使用自己自定义的string类，而是使用初始化的stirng类<br />当一个类收到了类加载请求，他首先不会尝试自己去加载这个类，而是把这个请求委派给父类去完成，每一个层次类加载器都是如此，因此所有的加载请求都应该传送到启动类加载其中，只有当父类加载器反馈自己无法完成这个请求的时候（在它的加载路径下没有找到所需加载的Class），子类加载器才会尝试自己去加载。<br />采用双亲委派的一个好处是比如加载位于rt.jar 包中的类java.lang.Object，不管是哪个加载器加载这个类，最终都是委托给顶层的启动类加载器进行加载，这样就保证了使用不同的类加载器最终得到的都是同样一个Object 对象<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605162527098-93548389-1d83-4a8f-b815-8814663cca8e.png#height=406&id=M4VuE&originHeight=1218&originWidth=2229&originalType=binary&ratio=1&rotation=0&showTitle=false&size=326460&status=done&style=shadow&title=&width=743)
<a name="fHsVk"></a>
## 有了Class对象能够做什么？
创建类的对象：通过调用Class对象的`newInstance()`方法

- 类必须有一个无参数的构造器
- 类的构造器的权限需要足够

如果没有无参构造器就不能创建对象？<br />只要在操作的时候明确的调用类中的构造器，并将参数传递进去之后，才可以实例化操作。<br />步骤如下：

- 通过Class类的getDeclaredConstructor(Class … parameterTypes)取得本类的指定形参类型的构造器
- 向构造器的形参中，传递一个对象数组进去，里面包含了构造器中所需的各个参数
- 通过Constructor实例化对象
<a name="XOYS6"></a>
### 调用指定方法
通过反射，调用类中的方法，通过Method类完成。

- 通过Class类的`getMethod`方法取得一个Method对象，并设置此方法操作是所需要的参数类型
- 之后使用Object invoke进行调用，并向方法中传递要设置的obj对象的参数信息
<a name="ZonCd"></a>
### `Invoke`方法

- `Object invoke(Object obj， Object … args)`
- Object对应原方法的返回值，若原方法无返回值，此时返回null
- 若原方法为静态方法，此时形参Object 可以为null
- 若原方法形参列表为空，则`Object[] args` 为 null
- 若原方法声明private，则需要在调用此invoke() 方法前，显示调用方法对象的`setAccessible(true)`方法，将可访问private的方法
<a name="COHoC"></a>
### `setAccessible`方法

- Method和`Field`、`Constructor`对象都有`setAccessible()`方法
- `setAccessible`作用是启动和禁用访问安全检查的开关
- 参数值为true则指示反射对象再使用时应该取消Java语言访问检查
- 提高反射效率，如果代码中必须使用反射，而这句代码需要频繁被嗲用，那么设置成true
- 使得原本无法访问的私有成员也可以访问
- 参数值为false则指示反射的对象应该实行Java语言访问检查

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605161532102-f8025f85-b0fc-459f-86c4-9db024414073.png#height=247&id=obieg&originHeight=742&originWidth=1881&originalType=binary&ratio=1&rotation=0&showTitle=false&size=116137&status=done&style=shadow&title=&width=627)<br />完整代码：
```java
/**
 * 通过反射获取对象
 *
 * @author: Fcant
 */
public class GetObjectByReflectionDemo {
    public static void main(String[] args) throws ClassNotFoundException, IllegalAccessException, InstantiationException, NoSuchMethodException, InvocationTargetException, NoSuchFieldException {
        // 获取Class
        Class clazz = Class.forName("com.moxi.interview.study.annotation.User");
        // 构造一个对象，newInstance调用的是无参构造器，如果没有无参构造器的话，本方法会出错
//        User user = (User)clazz.newInstance();
        // 获取class的有参构造器
        Constructor constructor = clazz.getDeclaredConstructor(String.class, int.class, int.class);
        User user2 = (User) constructor.newInstance("小溪", 10, 10);
        System.out.println(user2);
        // 通过反射调用普通构造方法
        User user3 = (User)clazz.newInstance();
        // 获取setName 方法
        Method setName = clazz.getDeclaredMethod("setName", String.class);
        // 执行setName方法，传入对象 和 参数
        setName.invoke(user3, "小白");
        System.out.println(user3);
        System.out.println("============");
        Field age = clazz.getDeclaredField("age");
        // 关闭权限检测,这样才能直接修改字段，因为 set方法不能直接操作私有变量
        age.setAccessible(true);
        age.set(user3, 10);
        System.out.println(user3);
    }
}
```
运行结果
```
User{name='小溪', id=10, age=10}
User{name='小白', id=0, age=0}
============
User{name='小白', id=0, age=10}
```
