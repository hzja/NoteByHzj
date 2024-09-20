Java<br />对象怎么创建，这个太熟悉了，`new`一下(其实还有很多途径，比如反射、反序列化、`clone`等，这里拿最简单的`new`来讲)：
```java
Dog dog = new Dog();
```
总是习惯于固定语句的执行，却对于背后的实现过程缺乏认知，而理解这个过程对后面晦涩难懂的反射和代理其实会有很大帮助，所以请务必学好这块内容。
<a name="vJFr0"></a>
### JVM内存
先简单说下java虚拟机内存模型和存放内容的区别，两部分：

- 栈内存 存放基本类型数据和对象的引用变量，数据可以直接拿来访问，速度比堆快
- 堆内存 存放创建的对象和数组，会由java虚拟机的自动垃圾回收来管理（GC）,创建一个对象放入堆内的同时也会在栈中创建一个指向该对象堆内存中的地址引用变量，下面说的对象就是存在该内存中

下面就按照对象生成的过程来一一讲解参与其中过程的各个概念<br />首先有这么一个类，后面的初始化基于这个讲解：
```java
/**
 * @since 2021-04-18 11:01:41
 *
 * 执行顺序：（优先级从高到低。）静态代码块>构造代码块>构造方法>普通方法。
 * 其中静态代码块只执行一次。构造代码块在每次创建对象是都会执行。
 */
public class Dog {

    //默认狗狗的最大年龄是16岁
    private static int dog_max_age = 16;

    //狗狗的名字
    private String dog_name;
    
    {
        System.out.println("狗狗的构造代码块");
    }

    static {
        System.out.println("狗狗的静态代码块");
    }

    //无参构造器故意没设
    //有参构造器
    public Dog(String dog_name) {
        this.dog_name = dog_name;
    }

    public void getDogInfo(){
        System.out.println("名字是:"+dog_name + "  年龄：" + dog_max_age);
    }

    //狗叫
    public static void barking(){
        System.out.println("汪汪汪~~~");
    }
}
```
<a name="kHh64"></a>
### JVM生成.class文件
一个java文件会在编译期间被初始化生成.class字节码文件，字节码文件是专门给JVM阅读的，平时写的一行行代码最终都会被编译成机器能看懂的语句，这个文件后面会被类加载器加载到内存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651312909522-3061d96a-5873-4db6-92ec-ca12705c7742.png#clientId=u784b2c48-45d9-4&from=paste&id=u0ff1e887&originHeight=206&originWidth=462&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4792705c-63c6-433f-b9f8-efb9fbf5795&title=)
<a name="kUTYQ"></a>
### 类加载器加载.class文件
《深入理解Java的虚拟机》中大概有这么一句话：在虚拟机遇到一条new的指令时，会去检查一遍在静态常量池中能否定位到一个类的符号引用 （就这个类的路径+名字），并且检查这个符号引用代表的类是否已被加载、解析和初始化过。如果不是第一次使用，那必须先执行相应的类加载过程，这个过程由类加载器来完成。<br />类加载字面意思就可以理解成加载class文件，更准确点的说法就是会把class文件变成一个二进制流加载到内存中，即把类的描述信息加载到Metaspace，至于类加载器如何找到并把一个class文件转成IO流加载到内存中，后面会专门写一篇关于类加载器的文章，这里就只要理解创建对象中有这么一步就行了。不过这里面有很重要的概念不得不讲：Class对象。
<a name="VGWwO"></a>
#### 知识扩展：Class对象
划重点，这是个非常重要的概念，理解它对于理解后面的反射和代理会有很大的帮助<br />类加载器 ClassLoader 加载class文件时，会把类里的一些数值常量、方法、类信息等加载到内存中，称之为类的元数据，最终目的是为了生成一个Class对象用来描述类，这个对象会被保存在.class文件里，可能有新手看到这里会比较懵逼，class也有对象？<br />当然了，Class是个实实在在的类（用来描述类的类，比较拗口），有构造方法（ private ，意味着可以生成对象，但不能手动生成，由JVM自动创建Class对象）,类加载器会给每个java文件都创建一个Class对象，用来描述类，画个图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651312909433-eac63400-3744-4572-84c6-2b85c2ebd7ba.png#clientId=u784b2c48-45d9-4&from=paste&id=u218221d2&originHeight=656&originWidth=619&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u461bfe7c-ec94-44ad-93a9-768acd02817&title=)
```java
//以下操作只能由jvm完成，手动做不了
Class cls1 = new Class(Dog.class.getClassLoader());
Class cls2 = new Class(Cat.class.getClassLoader());
Class cls3 = new Class(People.class.getClassLoader());
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651312909527-f0f4c680-d537-4f99-8ff2-dd18482946c3.png#clientId=u784b2c48-45d9-4&from=paste&id=u073cfef0&originHeight=47&originWidth=417&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u422adab7-67ff-494c-93a4-205cb18a455&title=)<br />这个Class对象除了描述对应的类之外还有什么作用呢？也可以生成对象，就是java的反射概念（通过Class实例获取类信息） 上面说了，Class类是用来描述像People.Class类的类，那么它里面肯定包含了所有能够描述该class的所有属性，比如类名、方法、接口等，先到Class类源码中瞄一眼：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651312909466-0fc333c9-b5dd-4c6f-bd28-90f70eb588e3.png#clientId=u784b2c48-45d9-4&from=paste&id=uc3e28e70&originHeight=613&originWidth=454&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4eaf5684-b829-464e-b882-bef6d702429&title=)<br />这里面有个方法 `newInstance()`，即创建对象， 把源代码贴出来并简单解析下：
```java
@CallerSensitive
public T newInstance()
	throws InstantiationException, IllegalAccessException
    {

        if (System.getSecurityManager() != null) {
            checkMemberAccess(Member.PUBLIC, Reflection.getCallerClass(), false);
        }

        if (cachedConstructor == null) {
            if (this == Class.class) {
                throw new IllegalAccessException(
                    "Can not call newInstance() on the Class for java.lang.Class"
                );
            }
            try {
                Class<?>[] empty = {};
                //声明无参构造对象
                final Constructor<T> c = getConstructor0(empty, Member.DECLARED);
                // Disable accessibility checks on the constructor
                // since we have to do the security check here anyway
                // (the stack depth is wrong for the Constructor's
                // security check to work)
                java.security.AccessController.doPrivileged(
                    new java.security.PrivilegedAction<Void>() {
                        public Void run() {
                                c.setAccessible(true);
                                return null;
                            }
                        });
                cachedConstructor = c;
            } catch (NoSuchMethodException e) {
             //如果class中没有无参构造方法，那么抛InstantiationException错误
                throw (InstantiationException)
                    new InstantiationException(getName()).initCause(e);
            }
        }
        Constructor<T> tmpConstructor = cachedConstructor;
        // Security check (same as in java.lang.reflect.Constructor)
        int modifiers = tmpConstructor.getModifiers();
        if (!Reflection.quickCheckMemberAccess(this, modifiers)) {
            Class<?> caller = Reflection.getCallerClass();
            if (newInstanceCallerCache != caller) {
                Reflection.ensureMemberAccess(caller, this, null, modifiers);
                newInstanceCallerCache = caller;
            }
        }
        // Run constructor
        try {
         //最终还是调用了无参构造器对象的newInstance方法
            return tmpConstructor.newInstance((Object[])null);
        } catch (InvocationTargetException e) {
            Unsafe.getUnsafe().throwException(e.getTargetException());
            // Not reached
            return null;
        }
    }
}
```
**首先搞清楚 newInstance 两种方法区别：**<br />`Class.newInstance()` 只能够调用无参的构造函数，即默认的构造函数，在Class源码里也看到了其实最终还是调用了无参构造器对象 `Constructor` 的 `newInstance` 方法，举个栗子：`Dog.class` 中是没有无参构造方法，那么会直接抛出 `InstantiationException` 异常：
```java
//Dog类中只有一个dog_name的有参构造方法
Class c = Class.forName("com.service.ClassAnalysis.Dog");
Dog dog = (Dog) c.newInstance();//直接抛InstantiationException异常
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651312909457-3446efa9-c5df-45cf-bff6-123ba809aecb.png#clientId=u784b2c48-45d9-4&from=paste&id=ub6eedd58&originHeight=134&originWidth=743&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc6a54f59-64f6-495a-beaf-fbeb3733c24&title=)<br />`Constructor.newInstance()` 可以根据传入的参数，调用任意构造构造函数，也可以反射私有构造器（了解就行）
```java
//Dog类中只有一个dog_name的有参构造方法
Constructor cs = Dog.class.getConstructor(String.class);
Dog dog = (Dog) cs.newInstance("小黑");//执行没有问题
```
但是这里面的 `newInstance`跟这次要说的 new 方法存在区别，两者创建对象的方式不同，创建条件也不同：

- 使用 `newInstance` 时必须要保证这类已经加载并且已经建立连接，就是已经被类记载器加载完毕，而 new 不需要
- class对象的 `newInstance` 方法只能用无参构造，上面已经提到了，而 new 不需要
- 前者使用的是类加载机制，是一种方法，后者是创建一个新类，一种关键字

这个不能说`newInstance` 不方便，相反它在反射、工厂设计模式、代理中发挥了重要作用，后续也会写下代理和反射，因为理解起来确实有点绕。还有一点需要注意，不管以哪种方式创建对象，对应的Class对象都是同一个
```java
Dog dog1 = new Dog("旺财");
Dog dog2 = new Dog("小黑");
Class c = Class.forName("com.service.classload.Dog");//为了测试，加了无参构造
Dog dog3 = (Dog) c.newInstance();
System.out.println(dog1.getClass() == dog2.getClass());
System.out.println(dog1.getClass() == dog3.getClass());
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651312912479-85287cd2-f8e6-4ebe-916f-ae0a8dada7e0.png#clientId=u784b2c48-45d9-4&from=paste&id=u46192524&originHeight=126&originWidth=454&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6be345bc-0f5b-4b70-8654-3d3f18b54ae&title=)
<a name="hp6QZ"></a>
### 连接和初始化
在此阶段首先为静态`static`变量内存中分配存储空间，设立初始值（还未被初始化）比如：
```java
public static int i = 666;//被类加载器加载到内存时会执行，赋予一个初始值
public static Integer ii = new Integer(666);//也被赋值一个初始值
```
但请注意，实际上i 的初始值是0，不是666，其他基本数据类型比如boolean的初始值就是false，以此类推。如果是引用类型的成员变量 ii 那么初始值就是null。
```java
Dog dog = new Dog("旺财");//在这里打个断点
```
执行，首先会执行静态成员变量初始化，默认值是0：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651312912724-5b5f8601-dbc6-490e-afd4-579a65b128e4.png#clientId=u784b2c48-45d9-4&from=paste&id=ub6436e75&originHeight=260&originWidth=804&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucfd0f2b7-2ede-44bf-b69f-f1d9aace20b&title=)<br />但有例外，如果加上了 final 修饰词那么初始值就是设定的值。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651312913554-2ffff504-1f49-4042-a1cd-eefd8c868759.png#clientId=u784b2c48-45d9-4&from=paste&id=u28344c04&originHeight=186&originWidth=782&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6cba5c67-dd3a-4efa-847f-205ba1b784a&title=)<br />接着对已经分配存储空间的静态变量真正赋值，比如为上面的dog_max_age 赋值16，还有执行静态代码块，也就是类似下面的代码：
```java
static {
    System.out.println("狗狗的静态代码块");
}
```
到这为止，类的加载过程才算完成。
<a name="mjbEQ"></a>
### 创建实例
在加载类完毕后，对象的所需大小根据类信息就可以确认了，具体创建的步骤如下：

- 先给对象分配内存（包括本类和父类的所有实例变量，不包括上面的静态变量），并设置默认值，如果有引用对象那么会在栈内存中申请一个空间用来指向的实际对象。
- 执行初始化代码实例化，先初始化父类再初始化子类，赋予给定值（尊重长辈是java的传统美德）
- 对象实例化完毕后如果存在引用对象的话还需要**把第一步的栈对象指向到堆内存中的实际对象，**这样一个真正可用的对象才被创建出来。

说了这么多估计很多人都没概念，懵逼状态中，其实很简单，只要记住new的创建对象就两步：初始化和实例化，再看一张图:可以简单理解②③④为初始化⑤实例化<br />![2022-04-30-18-02-13-019116.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651313072778-bcb8f15a-d444-49ef-bd64-57fe1be95997.png#clientId=u784b2c48-45d9-4&from=ui&id=ua13b7ffd&originHeight=772&originWidth=892&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2070067&status=done&style=shadow&taskId=ua970969f-b570-42af-8e3e-297aaa70034&title=)<br />弄懂这些概念比起单单知道对象创建的过程有意义的多：

- 类加载器，可以找找网上的资料，蛮多的，这块内容做个了解就行
- Class类和Class对象的概念，请重点掌握，不然理解反射和动态代理很费劲，spring的源码也会难以理解
- 栈内存和堆内存以及对应的基本类型和引用类型，也很重要，争取能基本理解
