JVM 类加载 双亲委派机制
<a name="ca2nM"></a>
## 类的生命周期
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603636231229-483a739d-13da-43e1-9b5c-765263744a4a.png#align=left&display=inline&height=212&originHeight=636&originWidth=1573&size=59813&status=done&style=shadow&width=524.3333333333334)<br />类的生命周期包含了如上的7个阶段，其中**验证**、**准备**、**解析**统称为**连接 ，类的加载主要是前五个阶段，每个阶段基本上保持如上顺序开始**（仅仅是开始，实际上执行是交叉混合的），只有**解析**阶段不一定，在**初始化后**也有可能才开始执行解析，这是为了支持动态语言。
<a name="gN6vx"></a>
### 加载
加载就是将字节码的二进制流转化为**方法区**的运行时数据结构，并生成类所对象的Class对象，字节码二进制流可以是编译后的class文件，也可以从网络中获取，或者运行时动态生成（动态代理）等等。<br />那什么时候会触发类加载呢？这个在虚拟机规范中没有明确定义，只是规定了何时需要执行初始化（稍后详细分析）。
<a name="hcKSy"></a>
### 验证
这个阶段很好理解，就是进行必要的校验，确保加载到内存中的字节码是符合要求的，主要包含以下四个校验步骤（了解即可）：

- 文件格式校验：这个阶段要校验的东西非常多，主要的有下面这些（实际上远远不止）
   1. 是否以魔数0xCAFEBABE开头。
   2. 主、次版本号是否在当前Java虚拟机接受范围之内。
   3. 常量池的常量中是否有不被支持的常量类型（检查常量tag标志）。
   4. 指向常量的各种索引值中是否有指向不存在的常量或不符合类型的常量。
   5. CONSTANT_Utf8_info型的常量中是否有不符合UTF-8编码的数据。
   6. Class文件中各个部分及文件本身是否有被删除的或附加的其他信息。
- 元数据校验：对字节码描述信息进行语义分析。
   1. 这个类是否有父类（除了java.lang.Object之外，所有的类都应当有父类）。
   2. 这个类的父类是否继承了不允许被继承的类（被final修饰的类）。
   3. 如果这个类不是抽象类，是否实现了其父类或接口之中要求实现的所有方法。
   4. 类中的字段、方法是否与父类产生矛盾（例如覆盖了父类的final字段，或者出现不符合规则的方法重载，例如方法参数都一致，但返回值类型却不同等）。
- 字节码校验：确保程序没有语法和逻辑错误，这是整个验证阶段最复杂的一个步骤。
   1. 保证任意时刻操作数栈的数据类型与指令代码序列都能配合工作，例如不会出现类似于“在操作栈放置了一个 int 类型的数据，使用时却按 long 类型来加载入本地变量表中”这样的情况。
   2. 保证任何跳转指令都不会跳转到方法体以外的字节码指令上。
   3. 保证方法体中的类型转换总是有效的，例如可以把-个子类对象赋值给父类数据类型，这是安全的，但是把父类对象赋值给子类数据类型，甚至把对象赋值给与它毫无继承关系、完全不相干的一个数据类型，则是危险和不合法的。
- 符号引用验证：这个阶段发生在符号引用转为直接引用的时候，即实际上是在**解析**阶段中进行的。
   1. 符号引用中通过字符串描述的全限定名是否能找到对应的类。
   2. 在指定类中是否存在符合方法的字段描述符及简单名称所描述的方法和字段。
   3. 符号引用中的类、字段、方法的可访问性( private、 protected、public)。
   4.  是否可被当前类访问。
<a name="uzpgC"></a>
### 准备
该阶段是为**类变量**（static）分配内存并设置零值，即类只要经过准备阶段其中的静态变量就是可使用的了，但此时类变量的值还不是想要的值，需要经过初始化阶段才会将希望的值赋值给对应的静态变量。
<a name="Sl0Fd"></a>
### 解析
**解析**就是将常量池中的**符号引用**替换为**直接引用**的过程。**符号引用**就是一个代号，比如名字，而这里可以理解为就是类的**完全限定名**；**直接引用**则是对应的具体的人、物，这里就是指目标的**内存地址**。为什么需要符号引用呢？因为类在加载到内存之前还没有分配内存地址，因此必然需要一个东西指代它。这个阶段包含了**类或接口的解析**、**字段解析**、**类方法解析**、**接口方法解析**，在解析的过程中可能会抛出以下异常：

- java.lang.NoSuchFieldError：找不到字段
- java.lang.IllegalAccessError：不具有访问权限
- java.lang.NoSuchMethodError：找不到方法
<a name="9t36b"></a>
### 初始化
这是类加载过程中的最后一个步骤，主要是收集类的**静态变量的赋值动作**和**static块中的语句**合成<cinit>方法，通过该方法根据我们的意愿为**静态变量赋值**以及**执行static块**，该方法会被加锁，确保多线程情况下只有一个线程能初始化成功，利用该特性可以实现**单例模式**。虚拟机规定了**有且只有**遇到以下情况时必须先确保对应类的初始化完成（加载、准备必然在此之前）：

- 遇到new、getstatic、putstatic或invokestatic这四条字节码指令时。能够生成这四条指令的典型Java代码场景有：
   1. 使用new关键字实例化对象的时候。
   2. 读取或设置一个类型的静态字段（被final修饰、已在编译期把结果放入常量池的静态字段除外）的时候。
   3. 调用一个类型的静态方法的时候。
- 反射调用类时。
- 当初始化类的时候，如果发现其父类还没有进行过初始化，则需要先触发其父类的初始化。
- 当虚拟机启动时，用户需要指定一个要执行的主类（包含main()方法的那个类），虚拟机会先初始化这个主类。
- 当使用JDK 7新加入的动态语言支持时，如果一个java.lang.invoke.MethodHandle实例最后的解析结果为REF_getStatic、REF_putStatic、REF_invokeStatic、REF_newInvokeSpecial四种类型的方法句柄，并且这个方法句柄对应的类没有进行过初始化，则需要先触发其初始化。
- 当一个接口中定义了JDK 8新加入的默认方法（被default关键字修饰的接口方法）时，如果有这个接口的实现类发生了初始化，那该接口要在其之前被初始化。

下面分析几个案例代码，可以先思考后再运行代码看看和自己想的是否一样。
<a name="qrIF9"></a>
#### 案例一
先定义如下两个类：
```java
public class SuperClazz {
    static  {
        System.out.println("SuperClass init！");
    }
    public static int value=123;
    public static final String HELLOWORLD="hello world";
    public static final int WHAT = value;
}
public class SubClaszz extends SuperClazz {
    static{
        System.out.println("SubClass init！");
    }
}
```
然后进行下面的调用：
```java
public class Initialization {
    public static void main(String[]args){
        Initialization initialization = new Initialization();
        initialization.M1();
    }
    public void M1(){
        System.out.println(SubClaszz.value);
    }
}
```
第一个案例是通过子类去引用父类中的**静态变量**，两个类都会加载和初始化么？打印结果看看：
```
SuperClass init！
123
```
可以看到只有父类初始化了，那么父类必然是加载了的，问题就在于子类有没有被加载呢？可以加上参数：-XX:+TraceClassLoading再执行（该参数的作用就是打印被加载了的类），可以看到子类是被加载了的。所以**通过子类引用父类静态变量，父子类都会被加载，但只有父类会进行初始化**。<br />为什么呢？反编译后可以看到生成了如下指令：
```
0: getstatic     #5                  // Field java/lang/System.out:Ljava/io/PrintStream;
3: getstatic     #6                  // Field ex7/init/SubClaszz.value:I
6: invokevirtual #7                  // Method java/io/PrintStream.println:(I)V
9: return
```
关键就是**getstatic**指令就会触发类的初始化，但是为什么子类不会初始化呢？因为这个变量是来自于父类的，为了提高效率，所以虚拟机进行了优化，这种情况只需要初始化父类就行了。
<a name="UMFTW"></a>
#### 案例二
调用下面的方法：
```java
public void M2(){
     SubClaszz[]sca = new SubClaszz[10];
}
```
执行后可以发现，**使用数组，不会触发初始化，但父子类都会被加载**。
<a name="NKnnh"></a>
#### 案例三
```java
public void M3(){
    System.out.println(SuperClazz.HELLOWORLD);
}
```
**引用常量不会触发类的加载和初始化**，因为常量在编译后就已经存在当前class的常量池。
<a name="lr0E1"></a>
#### 案例四
```java
public void M4(){
    System.out.println(SubClaszz.WHAT);
}
```
通过常量去引用其它的静态变量会发生什么呢？这个和案例一结果是一样的。
<a name="PtMJh"></a>
## 类加载器
<a name="4EWpG"></a>
### 类加载器和双亲委派模型
在平时开发中，确定一个类需要通过**完全限定名**，而不能简单的通过名字，因为在不同的路径下是可以定义同名的类的。那么在虚拟机中又是怎么区分类的呢？在虚拟机中需要**类加载器**+**完全限定名**一起来指定一个类的唯一性，即相同限定名的类若由两个不同的类加载器加载，那虚拟机就不会把它们当做一个类。从这里可以看出**类加载器**一定是有多个的，那么不同的类加载器是怎么组织的？它们又分别需要加载哪些类呢？![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603636303517-098c96bb-4e4e-4104-8bb5-7d33cc29cfc1.png#align=left&display=inline&height=365&originHeight=1096&originWidth=1987&size=144023&status=done&style=shadow&width=662.3333333333334)<br />从虚拟角度看，只有两种类型的类加载器：**启动类加载器（BootstrapClassLoader）**和**非启动类加载器**。前者是C++实现，属于虚拟机的一部分，后者则是由Java实现的，独立于虚拟机的外部，并且全部继承自抽象类java.lang.ClassLoader。<br />但从Java本身来看，一直保持着**三层类加载器**、**双亲委派**的结构，当然除了Java本身提供的三层类加载器，还可以自定义实现类加载器。如上图，上面三个就是原生的类加载器，每一个都是下一个类加载器的**父加载器**，注意这里都是采用**组合**而非**继承**。当开始加载类时，首先交给父加载器加载，父加载器加载了子加载器就不用再加载了，而若是父加载器加载不了，就会交给子加载器加载，这就是**双亲委派机制**。这就好比工作中遇到了无法处理的事，会去请示直接领导，直接领导处理不了，再找上层领导，然后上层领导觉得这是个小事，不用他亲自动手，就让直接领导去做，接着他又交给你去做等等。下面来看看每个类加载器的具体作用：

- BootstrapClassLoader：启动类加载器，顾名思义，这个类加载器主要负责加载JDK lib包，以及-Xbootclasspath参数指定的目录，并且虚拟机对文件名进行了限定，也就是说即使自己写个jar放入到上述目录，也不会被加载。由于该类加载器是C++使用，所以Java程序中无法直接引用，调用java.lang.ClassLoader.getClassLoader()方法时默认返回的是null。
- ExtClassLoader：扩展类加载器，主要负责加载JDK lib/ext包，以及被系统变量java.ext.dirs指向的所有类库，这个类库可以存放自己写的通用jar。
- AppClassLoader：应用程序类加载器，负责加载用户classpath上的所有类。它是java.lang.ClassLoader.getSystemClassLoader()的返回值，也是程序的默认类加载器（如果没有自定义类加载器的话）。

通过这三个类加载以及双亲委派机制，一个显而易见的好处就是，不同的类随它的类加载器天然具有了加载优先级，像Object、String等等这些核心类库自然就会在应用程序类之前被加载，使得程序更安全，不会出现错误，Spring的父子容器也是这样的一个设计。通过下面这段代码可以看到每个类所对应的类加载器：
```java
public class ClassLoader {
    public static void main(String[] args) {
        System.out.println(String.class.getClassLoader()); //启动类加载器
        System.out.println(sun.net.spi.nameservice.dns.DNSNameService.class.getClassLoader());//拓展类加载器
        System.out.println(ClassLoader.class.getClassLoader());//应用程序类加载器
    }
}
```
输出：
```
null
sun.misc.Launcher$ExtClassLoader@4b67cf4d
sun.misc.Launcher$AppClassLoader@14dad5dc
```
<a name="RJCK4"></a>
### 破坏双亲委派模型
刚刚举了工作中的一个例子来说明双亲委派机制，但现实中不需要事事都去请示领导，同样类加载器也不是完全遵循双亲委派机制，在必要的时候是可以打破这个规则的。下面列举四个破坏的情况，在此之前需要先了解下双亲 委派的代码实现原理，在java.lang.ClassLoader类中有一个**loadClass**以及**findClass**方法：
```java
protected Class<?> loadClass(String name, boolean resolve)
    throws ClassNotFoundException
    {
        synchronized (getClassLoadingLock(name)) {
            // First, check if the class has already been loaded
            Class<?> c = findLoadedClass(name);
            if (c == null) {
                long t0 = System.nanoTime();
                try {
                    if (parent != null) {
                        c = parent.loadClass(name, false);
                    } else {
                        c = findBootstrapClassOrNull(name);
                    }
                } catch (ClassNotFoundException e) {
                    // ClassNotFoundException thrown if class not found
                    // from the non-null parent class loader
                }
                if (c == null) {
                    // If still not found, then invoke findClass in order
                    // to find the class.
                    long t1 = System.nanoTime();
                    c = findClass(name);
                    // this is the defining class loader; record the stats
                    sun.misc.PerfCounter.getParentDelegationTime().addTime(t1 - t0);
                    sun.misc.PerfCounter.getFindClassTime().addElapsedTimeFrom(t1);
                    sun.misc.PerfCounter.getFindClasses().increment();
                }
            }
            if (resolve) {
                resolveClass(c);
            }
            return c;
        }
    }
    protected Class<?> findClass(String name) throws ClassNotFoundException {
        throw new ClassNotFoundException(name);
    }
```
从上面可以看到首先是调用**parent**去加载类，没有加载到才调用自身的**findClass**方法去加载。也就是说用户在实现自定义类加载器的时候需要覆盖的是**fiindClass**而不是**loadClass**，这样才能满足**双亲委派模型**。<br />下面具体来看看破坏双亲委派的几个场景。
<a name="Dt1xe"></a>
#### 第一次
第一次破坏是在双亲委派模型出现之前， 因为该模型是在JDK1.2之后才引入的，那么在此之前，抽象类java.lang.ClassLoader就已经存在了，用户自定义的类加载器都会去覆盖该类中的**loadClass**方法，所以双亲委派模型出现后，就无法避免用户覆盖该方法，因此新增了**findClass**引导用户去覆盖该方法实现自己的类加载逻辑。
<a name="SPI"></a>
#### SPI
第二次破坏是由于这个模型本身缺陷导致的，因为该模型保证了类的加载优先级，但是有些接口是Java定义在核心类库中，但具体的服务实现是由用户提供的，这时候就不得不破坏该模型才能实现，典型的就是Java中的SPI机制。JDBC的驱动加载就是SPI实现的，所以直接看到**java.sql.DriverManager**类，该类中有一个静态初始化块：
```java
static {
    loadInitialDrivers();
    println("JDBC DriverManager initialized");
}
private static void loadInitialDrivers() {
    String drivers;
    try {
        drivers = AccessController.doPrivileged(new PrivilegedAction<String>() {
            public String run() {
                return System.getProperty("jdbc.drivers");
            }
        });
    } catch (Exception ex) {
        drivers = null;
    }
    AccessController.doPrivileged(new PrivilegedAction<Void>() {
        public Void run() {
            ServiceLoader<Driver> loadedDrivers = ServiceLoader.load(Driver.class);
            Iterator<Driver> driversIterator = loadedDrivers.iterator();
            try{
                while(driversIterator.hasNext()) {
                    driversIterator.next();
                }
            } catch(Throwable t) {
                // Do nothing
            }
            return null;
        }
    });
    println("DriverManager.initialize: jdbc.drivers = " + drivers);
    if (drivers == null || drivers.equals("")) {
        return;
    }
    String[] driversList = drivers.split(":");
    println("number of Drivers:" + driversList.length);
    for (String aDriver : driversList) {
        try {
            println("DriverManager.Initialize: loading " + aDriver);
            Class.forName(aDriver, true,
                          ClassLoader.getSystemClassLoader());
        } catch (Exception ex) {
            println("DriverManager.Initialize: load failed: " + ex);
        }
    }
}
```
主要看**ServiceLoader.load**方法，这个就是通过SPI去加载引入**java.sql.Driver**实现类（比如引入mysql的驱动包就是com.mysql.cj.jdbc.Driver）：
```java
public static <S> ServiceLoader<S> load(Class<S> service) {
    ClassLoader cl = Thread.currentThread().getContextClassLoader();
    return ServiceLoader.load(service, cl);
}
```
这个方法主要是从当前线程中获取类加载器，然后通过这个类加载器去加载**驱动实现类**（这个叫**线程上下文类加载器**，也可以使用这个技巧去打破双亲委派），那这里会获取到哪一个类加载器呢？具体的设置是在**sun.misc.Launcher**类的构造器中：
```java
public Launcher() {
    Launcher.ExtClassLoader var1;
    try {
        var1 = Launcher.ExtClassLoader.getExtClassLoader();
    } catch (IOException var10) {
        throw new InternalError("Could not create extension class loader", var10);
    }
    try {
        this.loader = Launcher.AppClassLoader.getAppClassLoader(var1);
    } catch (IOException var9) {
        throw new InternalError("Could not create application class loader", var9);
    }
    Thread.currentThread().setContextClassLoader(this.loader);
    String var2 = System.getProperty("java.security.manager");
    if (var2 != null) {
        SecurityManager var3 = null;
        if (!"".equals(var2) && !"default".equals(var2)) {
            try {
                var3 = (SecurityManager)this.loader.loadClass(var2).newInstance();
            } catch (IllegalAccessException var5) {
            } catch (InstantiationException var6) {
            } catch (ClassNotFoundException var7) {
            } catch (ClassCastException var8) {
            }
        } else {
            var3 = new SecurityManager();
        }
        if (var3 == null) {
            throw new InternalError("Could not create SecurityManager: " + var2);
        }
        System.setSecurityManager(var3);
    }
}
```
可以看到设置的就是**AppClassLoader**。可能会有点疑惑，这个类加载器加载类的时候不也是先调用父类加载器加载么，怎么就打破双亲委派了呢？其实打破双亲委派指的就是类的层次结构，延伸意思就是类的加载优先级，这里本应该是在加载核心类库的时候却提前将应用程序中的类库给加载到虚拟机中来了。
<a name="mjrsB"></a>
#### Tomcat
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1603636348253-af6e0fb0-5758-49d3-b662-eb97cb21ea8c.png#align=left&display=inline&height=584&originHeight=1753&originWidth=1546&size=138258&status=done&style=shadow&width=515.3333333333334)<br />上图是Tomcat类加载的类图，前面三个不用说，**CommonClassLoader**、**CatalinaClassLoader**、**SharedClassLoader**、**WebAppClassLoader**、**JspClassLoader**则是Tomcat自己实现的类加载器，分别加载**common包**、**server包**、**shared包**、**WebApp/WEB-INF/lib包**以及**JSP文件**，前面三个在tomcat 6之后已经合并到根目录下的lib目录下。而**WebAppClassLoader**则是每一个应用程序对应一个，**JspClassLoader**是每一个JSP文件都会对应一个，并且这两个类加载器都没有父类加载器，这也就违背了双亲委派模型。<br />为什么每个应用程序需要单独的**WebAppClassLoader**实例？因为每个应用程序需要彼此隔离，假如在两个应用中定义了一样的类（完全限定名），如果遵循双亲委派那就只会存在一份了，另外不同的应用还有可能依赖**同一个类库的不同版本**，这也需要隔离，所以每一个应用程序都会对应一个**WebAppClassLoader**，它们共享的类库可以让**SharedClassLoader**加载，另外这些类加载加载的类对Tomcat本身来说也是隔离的（CatalinaClassLoader加载的）。<br />为什么每个JSP文件需要对应单独的一个**JspClassLoader**实例？这是由于JSP是支持运行时修改的，修改后会丢弃掉之前编译生成的class，并重新生成一个**JspClassLoader**实例去加载新的class。<br />以上就是Tomcat为什么要打破双亲委派模型的原因。
<a name="dWkjf"></a>
#### OSGI
OSGI是用于实现模块热部署，像Eclipse的插件系统就是利用OSGI实现的，这个技术非常复杂同时使用的也越来越少了，感兴趣的可自行查阅资料学习。
<a name="kum03"></a>
## 总结
类加载的过程让我们了解到一个类是如何被加载到内存中，需要经过哪些阶段；而类加载器和双亲委派模型则是告诉我们应该怎么去加载类、类的加载优先级是怎样的，其中的设计思想也可以学习借鉴；最后需要深刻理解的是为什么需要打破双亲委派，在遇到相应的场景时应该怎么做。
