Java JVM 类加载机制
<a name="KMEhm"></a>
## 类加载机制
类是在运行期间第一次使用时动态加载的，而不是一次性加载所有类。因为如果一次性加载，那么会占用很多的内存。
<a name="oROJv"></a>
### 类的生命周期
![2021-05-05-20-48-49-930569.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620219164972-5acc85e7-0dc4-44cb-8234-fe11a1e15e4a.png#clientId=u31d60699-382e-4&from=ui&id=u6fa0a19d&originHeight=256&originWidth=1080&originalType=binary&size=831117&status=done&style=shadow&taskId=ub0355266-ca9e-496b-a36b-181a2738ec8)<br />包括以下 7 个阶段：

- 「加载（Loading）」
- 「验证（Verification）」
- 「准备（Preparation）」
- 「解析（Resolution）」
- 「初始化（Initialization）」
- 使用（Using）
- 卸载（Unloading）
<a name="BhNKp"></a>
### 类加载过程 --- new 一个对象的过程
包含加载、验证、准备、解析和初始化这 5 个阶段。

1. 加载加载过程完成以下三件事：其中二进制字节流可以从以下方式中获取：
   - 从 ZIP 包读取，成为 JAR、EAR、WAR 格式的基础。
   - 从网络中获取，最典型的应用是 Applet。
   - 运行时计算生成，例如动态代理技术，在 `java.lang.reflect.Proxy` 使用 `ProxyGenerator.generateProxyClass` 的代理类的二进制字节流。
   - 由其他文件生成，例如由 JSP 文件生成对应的 Class 类。
   - 通过类的完全限定名称获取定义该类的二进制字节流。
   - 将该字节流表示的静态存储结构转换为方法区的运行时存储结构。
   - 在内存中生成一个代表该类的 Class 对象，作为方法区中该类各种数据的访问入口。
2. 验证格式验证：验证是否符合class文件规范 语义验证：检查一个被标记为`final`的类型是否包含子类；检查一个类中的`final`方法是否被子类进行重写；确保父类和子类之间没有不兼容的一些方法声明（比如方法签名相同，但方法的返回值不同） 操作验证：在操作数栈中的数据必须进行正确的操作，对常量池中的各种符号引用执行验证（通常在解析阶段执行，检查是否可以通过符号引用中描述的全限定名定位到指定类型上，以及类成员信息的访问修饰符是否允许访问等）
3. 准备类变量是被 `static` 修饰的变量，准备阶段为类变量分配内存并设置初始值，使用的是方法区的内存。
```java
public static int value = 123;
```
如果类变量是常量，那么它将初始化为表达式所定义的值而不是 0。例如下面的常量 value 被初始化为 123 而不是 0。
```java
public static final int value = 123;
```
实例变量不会在这阶段分配内存，它会在对象实例化时随着对象一起被分配在堆中。

4. 解析将常量池中的符号引用转为直接引用（得到类或者字段、方法在内存中的指针或者偏移量，以便直接调用该方法），这个可以在初始化之后再执行，可以支持 Java 的动态绑定。
:::info
以上2、3、4三个阶段又合称为链接阶段，链接阶段要做的是将加载到JVM中的二进制字节流的类数据信息合并到JVM的运行时状态中。
:::

5. 初始化初始化阶段是虚拟机执行类构造器 `<clinit>()` 方法的过程，是真正开始执行类中定义的 Java 程序代码。在前面的准备阶段，类变量已经赋过一次系统要求的初始值，而在初始化阶段，主要根据程序员通过程序制定的主观计划去初始化类变量和其它资源。`<clinit>()` 是由编译器自动收集类中所有类变量的赋值动作和静态语句块中的语句合并产生的，编译器收集的顺序由语句在源文件中出现的顺序决定。特别注意的是，静态语句块只能访问到定义在它之前的类变量，定义在它之后的类变量只能赋值，不能访问。虚拟机会保证一个类的 `<clinit>()` 方法在多线程环境下被正确的加锁和同步，如果多个线程同时初始化一个类，只会有一个线程执行这个类的\ `<clinit>()` 方法，其它线程都会阻塞等待，直到活动线程执行\ `<clinit>()` 方法完毕。如果在一个类的 `<clinit>()` 方法中有耗时的操作，就可能造成多个线程阻塞，在实际过程中此种阻塞很隐蔽。

上述步骤简单来说就是分为以下两步：

   - 类变量的赋值操作
   - 执行`static`代码块。`static`代码块只有jvm能够调用。如果是多线程需要同时初始化一个类，仅仅只能允许其中一个线程对其执行初始化操作，其余线程必须等待，只有在活动线程执行完对类的初始化操作之后，才会通知正在等待的其他线程。

最终，方法区会存储当前类的类信息，包括类的静态变量、类初始化代码（定义静态变量时的赋值语句和静态初始化代码块）、实例变量定义、实例初始化代码（定义实例变量时的赋值语句实例代码块和构造方法）和实例方法，还有父类的类信息引用。
<a name="Dg9bk"></a>
### 创建对象
假设是第一次使用一个类的话，那么需要经过上述的类加载的过程，之后才是创建对象。<br />**「1、在堆区分配对象需要的内存」**<br />分配的内存包括本类和父类的所有实例变量，但不包括任何静态变量<br />**「2、对所有实例变量赋默认值」**<br />将方法区内对实例变量的定义拷贝一份到堆区，然后赋默认值<br />**「3、执行实例初始化代码」**<br />初始化顺序是先初始化父类再初始化子类，初始化时先执行实例代码块然后是构造方法。（第一执行类中的静态代码，包括静态成 员变量的初始化和静态语句块的执行；第二执行类中的非静态代码，包括非静态成员变量的初始化和非静态语句块的执行，最后执 行构造函数。在继承的情况下，会首先执行父类的静态代码，然后执行子类的静态代码；之后执行父类的非静态代码和构造函数； 最后执行子类的非静态代码和构造函数）<br />**「4、如果有类似于**`**Child c = new Child()**`**形式的c引用的话，在栈区定义Child类型引用变量c，然后将堆区对象的地址赋值给它」**<br />需要注意的是，「每个子类对象持有父类对象的引用」，可在内部通过super关键字来调用父类对象，但在外部不可访问。<br />存在继承的情况下，初始化顺序为：

- 父类（静态变量、静态语句块）
- 子类（静态变量、静态语句块）
- 父类（实例变量、普通语句块）
- 父类（构造函数）
- 子类（实例变量、普通语句块）
- 子类（构造函数）
<a name="aLlm4"></a>
### 类初始化的情况

- 主动引用

虚拟机规范中并没有强制约束何时进行加载，但是规范严格规定了有且只有下列五种情况必须对类进行初始化（加载、验证、准备都会随之发生）：

   - 遇到 `new`、`getstatic`、`putstatic`、`invokestatic` 这四条字节码指令时，如果类没有进行过初始化，则必须先触发其初始化。最常见的生成这 4 条指令的场景是：使用 `new` 关键字实例化对象的时候；读取或设置一个类的静态字段（被 final 修饰、已在编译期把结果放入常量池的静态字段除外）的时候；以及调用一个类的静态方法的时候。
   - 使用 `java.lang.reflect` 包的方法对类进行反射调用的时候，如果类没有进行初始化，则需要先触发其初始化。
   - 当初始化一个类的时候，如果发现其父类还没有进行过初始化，则需要先触发其父类的初始化。
   - 当虚拟机启动时，用户需要指定一个要执行的主类（包含 `main()` 方法的那个类），虚拟机会先初始化这个主类。
- 被动引用

以上的行为称为对一个类进行主动引用。除此之外，所有引用类的方式都不会触发初始化，称为被动引用。被动引用的常见例子包括：

   - 通过子类引用父类的静态字段，不会导致子类初始化。
```java
System.out.println(SubClass.value);  // value 字段在 SuperClass 中定义
```

   - 通过数组定义来引用类，不会触发此类的初始化。该过程会对数组类进行初始化，数组类是一个由虚拟机自动生成的、直接继承自 Object 的子类，其中包含了数组的属性和方法。
```java
SuperClass[] sca = new SuperClass[10];
```

   - 常量在编译阶段会存入调用类的常量池中，本质上并没有直接引用到定义常量的类，因此不会触发定义常量的类的初始化。
```java
System.out.println(ConstClass.HELLOWORLD);
```
<a name="lGuF1"></a>
### 类与类加载器
两个类相等，需要类本身相等，并且使用同一个类加载器进行加载。这是因为每一个类加载器都拥有一个独立的类名称空间。那么最终的相等包括了类的 `Class` 对象的 `equals()` 方法、`isAssignableFrom()` 方法、`isInstance()` 方法的返回结果为 true，也包括使用 `instanceof` 关键字做对象所属关系判定结果为 true。<br />从 Java 虚拟机的角度来讲，只存在以下两种不同的类加载器：

- 启动类加载器（`Bootstrap ClassLoader`），使用 C++ 实现，是虚拟机自身的一部分；
- 所有其它类的加载器，使用 Java 实现，独立于虚拟机，继承自抽象类 `java.lang.ClassLoader`。

那么上述又可以分为以下三种类加载器：`BootstrapClassLoader`、`ExtensionClassLoader`、`AppClassLoader`

- 启动类加载器（`BootstrapClassLoader`）是嵌在JVM内核中的加载器，该加载器是用C++语言写的，主要负载加载JAVA_HOME/lib下的类库，或者被 `-Xbootclasspath` 参数所指定的路径中的，并且是虚拟机识别的（仅按照文件名识别，如 rt.jar，名字不符合的类库即使放在 lib 目录中也不会被加载）。启动类加载器无法被 Java 程序直接引用，用户在编写自定义类加载器时，如果需要把加载请求委派给启动类加载器，直接使用 null 代替即可。
- 扩展类加载器（`ExtensionClassLoader`）是用JAVA编写，由 `ExtClassLoader`（`sun.misc.Launcher$ExtClassLoader`）实现的。它负责将 <JAVA_HOME>/lib/ext 或者被 java.ext.dir 系统变量所指定路径中的所有类库加载到内存中，开发者可以直接使用扩展类加载器。它的父类加载器是Bootstrap。
- 应用程序类加载器（`Application ClassLoader`）这个类加载器是由 `AppClassLoader`（`sun.misc.Launcher$AppClassLoader`）实现的。一般负责加载应用程序classpath目录下的所有jar和class文件。开发者可以直接使用这个类加载器，如果应用程序中没有自定义过自己的类加载器，一般情况下这个就是程序中默认的类加载器。它的父加载器为`ExteClassLoader`。
<a name="PSlNN"></a>
### 双亲委派模型
应用程序是由三种类加载器互相配合从而实现类加载，除此之外还可以加入自己定义的类加载器。下图展示了类加载器之间的层次关系，称为双亲委派模型（Parents Delegation Model）。这里的父子关系一般通过委托来实现，而不是继承关系（Inheritance）。<br />![2021-05-05-20-48-50-463956.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620219140620-eb549ab9-06d4-43d7-9149-a941a893fa09.png#clientId=u31d60699-382e-4&from=ui&id=u98ba823f&originHeight=771&originWidth=817&originalType=binary&size=1893621&status=done&style=shadow&taskId=ue4f9e98f-4853-4191-8f21-25ed8972ef6)

- 工作流程如果一个类加载器收到了一个类加载请求，它不会自己去尝试加载这个类，而是把这个请求转交给父类加载器去完成。每一个层次的类加载器都是如此。因此所有的类加载请求都应该传递到最顶层的启动类加载器中，只有到父类加载器反馈自己无法完成这个加载请求（在它的搜索范围没有找到这个类）时，子类加载器才会尝试自己去加载。
- 好处使得 Java 类随着它的类加载器一起具有一种带有优先级的层次关系，从而使得基础类得到统一。例如 `java.lang.Object` 存放在 rt.jar 中，如果编写另外一个 `java.lang.Object` 并放到 `ClassPath` 中，程序可以编译通过。由于双亲委派模型的存在，所以在 rt.jar 中的 Object 比在 `ClassPath` 中的 `Object` 优先级更高，这是因为 rt.jar 中的 `Object` 使用的是启动类加载器，而 `ClassPath` 中的 `Object` 使用的是应用程序类加载器。rt.jar 中的 `Object` 优先级更高，那么程序中所有的 `Object` 都是这个 `Object`。
- demo以下是抽象类 `java.lang.ClassLoader` 的代码片段，其中的 `loadClass()` 方法运行过程如下：先检查类是否已经加载过，如果没有则让父类加载器去加载。当父类加载器加载失败时抛出 `ClassNotFoundException`，此时尝试自己去加载。
```java
public abstract class ClassLoader {
    // The parent class loader for delegation
    private final ClassLoader parent;

    public Class<?> loadClass(String name) throws ClassNotFoundException {
        return loadClass(name, false);
    }

    protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
        synchronized (getClassLoadingLock(name)) {
            // First, check if the class has already been loaded
            Class<?> c = findLoadedClass(name);
            if (c == null) {
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
                    c = findClass(name);
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
}
```

- 自定义类加载器实现以下代码中的 `FileSystemClassLoader` 是自定义类加载器，继承自 `java.lang.ClassLoader`，用于加载文件系统上的类。它首先根据类的全名在文件系统上查找类的字节代码文件（.class 文件），然后读取该文件内容，最后通过 `defineClass()` 方法来把这些字节代码转换成 `java.lang.Class` 类的实例。`java.lang.ClassLoader` 的 `loadClass()` 实现了双亲委派模型的逻辑，自定义类加载器一般不去重写它，但是需要重写 `findClass()` 方法。
```java
public class FileSystemClassLoader extends ClassLoader {

    private String rootDir;

    public FileSystemClassLoader(String rootDir) {
        this.rootDir = rootDir;
    }

    protected Class<?> findClass(String name) throws ClassNotFoundException {
        byte[] classData = getClassData(name);
        if (classData == null) {
            throw new ClassNotFoundException();
        } else {
            return defineClass(name, classData, 0, classData.length);
        }
    }

    private byte[] getClassData(String className) {
        String path = classNameToPath(className);
        try {
            InputStream ins = new FileInputStream(path);
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            int bufferSize = 4096;
            byte[] buffer = new byte[bufferSize];
            int bytesNumRead;
            while ((bytesNumRead = ins.read(buffer)) != -1) {
                baos.write(buffer, 0, bytesNumRead);
            }
            return baos.toByteArray();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    private String classNameToPath(String className) {
        return rootDir + File.separatorChar
                + className.replace('.', File.separatorChar) + ".class";
    }
}
```
