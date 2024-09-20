JVM
<a name="eEiSh"></a>
## 一、Class类文件结构
<a name="ILL3O"></a>
### 1. JVM的平台无关性
与平台无关性是建立在操作系统上，虚拟机厂商提供了许多可以运行在各种不同平台的虚拟机，它们都可以载入和执行字节码，从而实现程序的一次编写，到处运行。<br />各种不同平台的虚拟机与所有平台都统一使用的程序存储格式——字节码（ByteCode）是构成平台无关性的基石，也是语言无关性的基础。Java 虚拟机不和包括 Java 在内的任何语言绑定，它只与“Class 文件”这种特定的二进制文件格式所关联，Class 文件中包含了 Java 虚拟机指令集和符号表以及若干其他辅助信息。
<a name="WUPlT"></a>
### 2. Class类文件

- Class文件是一组以8位字节为基础单位的二进制流。
- 类似于结构体的伪结构来存储数据。
- 只有两种数据类型：无符号数和表。
- 无符号数属于基本的数据类型，以u1、u2、u4、u8 。
- 表是由多个无符号数或者其他表作为数据项构成的复合数据类型。

其中值得注意的一个东西，class文件中有显示编译的版本号，使用notepad++等工具打开class文件。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669750489-a5675a06-bfeb-4a6f-b2bd-b1a3b7fa19f8.png#clientId=ud2f063f6-082e-4&from=paste&id=u5189ccc9&originHeight=98&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=udf76a543-cd39-4710-bca1-6c3a4a77bde)<br />图中标记前四个被称为魔数(唯一作用是确定这个文件是否为一个能被虚拟机接受的 Class 文件)。<br />后两个标记代表class文件的版本号，其中第4第500 00字节代表着jdk的次版本号，第6第7个字节00 34代表这jdk的主版本号，Java 的版本号是从 45 开始的，JDK 1.1 之后的每个 JDK 大版本发布主版本号向上加 1 高版本的 JDK 能向下兼容以前版本的 Class 文件，但不能运行以后版本的 Class 文件，即使文件格式并未发生任何变化，虚拟机也必须拒绝执行超过其版本号的 Class 文件。<br />34为16进制，转化为10进制就是52，所以 52-45+1 , 代表这个class文件的版本号为jdk1.8 。<br />当然class文件的结构详细说起来还有常量池、访问标志、父索引、接口索引、字段表集合、方法表集合、属性表集合，这些以后有时间再补上吧，概念性东西，对实际开发代码，优化代码帮助不大。
<a name="l5kWc"></a>
## 二、类的加载机制
类从被加载到虚拟机内存中开始，到卸载出内存为止，它的整个生命周期包括：加载（Loading）、验证（Verification）、准备（Preparation）、解析（Resolution）、初始化（Initialization）、使用（Using）和卸载（Unloading）7 个阶段。其中验证、准备、解析 3 个部分统称为连接（Linking）。
<a name="doiAD"></a>
### 1. 加载
虚拟机需要完成以下 3 件事情：<br />1、通过一个类的全限定名来获取定义此类的二进制字节流。<br />2、将这个字节流所代表的静态存储结构转化为方法区的运行时数据结构。<br />3、在内存中生成一个代表这个类的 java.lang.Class 对象，作为方法区这个类的各种数据的访问入口。
<a name="PVzKX"></a>
### 2. 验证
是连接阶段的第一步，这一阶段的目的是为了确保 Class 文件的字节流中包含的信息符合当前虚拟机的要求，并且不会危害虚拟机自身的安全。但从整体上看，验证阶段大致上会完成下面 4 个阶段的检验动作：文件格式验证、元数据验证、字节码验证、符号引用验证。
<a name="IluoF"></a>
### 3. 准备阶段
为类变量分配内存并设置类变量初始值(零值)的阶段。
<a name="evQd4"></a>
### 4. 解析阶段
是虚拟机将常量池内的符号引用替换为直接引用的过程。
<a name="N0NWc"></a>
### 5. 初始化阶段
是类加载过程的最后一步，前面的类加载过程中，除了在加载阶段用户应用程序可以通过自定义类加载器参与之外，其余动作完全由虚拟机主导和控制。<br />到了初始化阶段，才真正开始执行类中定义的 Java 程序代码在准备阶段，变量已经赋过一次系统要求的初始值，而在初始化阶段，则根据程序员通过程。<br />序制定的主观计划去初始化类变量和其他资源（即调用类构造器之类的）。<br />5种情况会对类立即进行初始化（了解即可）<br />1、遇到 `new`、`getstatic`、`putstatic` 或 `invokestatic` 这 4 条字节码指令时，如果类没有进行过初始化，则需要先触发其初始化。生成这 4 条指令的最常见的Java 代码场景是：使用 new 关键字实例化对象的时候、读取或设置一个类的静态字段（被 final 修饰、已在编译期把结果放入常量池的静态字段除外）的时候，以及调用一个类的静态方法的时候。<br />2、使用 java.lang.reflect 包的方法对类进行反射调用的时候，如果类没有进行过初始化，则需要先触发其初始化。<br />3、当初始化一个类的时候，如果发现其父类还没有进行过初始化，则需要先触发其父类的初始化。<br />4、当虚拟机启动时，用户需要指定一个要执行的主类（包含 `main()`方法的那个类），虚拟机会先初始化这个主类。<br />5、当使用 JDK 1.7 的动态语言支持时，如果一个 `java.lang.invoke.MethodHandle` 实例最后的解析结果 `REF_getStatic`、`REF_putStatic`、`REF_invokeStatic` 的方法句柄，并且这个方法句柄所对应的类没有进行过初始化，则需要先触发其初始化。
<a name="tY2vT"></a>
## 三、类加载器
对于任意一个类，都需要由 加载它的类加载器和这个类本身一同确立其在 Java 虚拟机中的唯一性 ，每一个类加载器，都拥有一个独立的类名称空间。这句话可以表达得更通俗一些：比较两个类是否“相等”，只有在这两个类是由同一个类加载器加载的前提下才有意义，否则，即使这两个类来源于同一个 Class 文件，被同一个虚拟机加载，只要加载它们的类加载器不同，那这两个类就必定不相等。这里所指的“相等”，包括代表类的 Class 对象的 `equals()`方法、`isAssignableFrom()`方法、`isInstance()`方法的返回结果，也包括使用 `instanceof` 关键字做对象所属关系判定等情况。
<a name="Ma1hS"></a>
### 1. 双亲委派模型

- 启动类加载器 : `BootstrapClassLoader`，负责加载存放在JDK\jre\lib(JDK代表JDK的安装目录，下同)下，或被 `-Xbootclasspath`参数指定的路径中的，并且能被虚拟机识别的类库（如rt.jar，所有的java.开头的类均被 `BootstrapClassLoader`加载）。启动类加载器是无法被Java程序直接引用的。
- 扩展类加载器 : `ExtensionClassLoader`，该加载器由`sun.misc.Launcher$ExtClassLoader`实现，它负责加载 JDK\jre\lib\ext目录中，或者由java.ext.dirs系统变量指定的路径中的所有类库（如javax.开头的类），开发者可以直接使用扩展类加载器。
- 应用类加载器 : `ApplicationClassLoader`，该类加载器由 `sun.misc.Launcher$AppClassLoader`来实现，它负责加载用户类路径（ClassPath）所指定的类，开发者可以直接使用该类加载器，如果应用程序中没有自定义过自己的类加载器，一般情况下这个就是程序中默认的类加载器。
- 自定义`ClassLoader` ：在自定义 `ClassLoader` 的子类时候，常见的会有两种做法，一种是重写 `loadClass` 方法，另一种是重写 `findClass` 方法。
   - 其实这两种方法本质上差不多毕竟 loadClass 也会调用 `findClass`，但是从逻辑上讲最好不要直接修改 `loadClass` 的内部逻辑。
   - 建议的做法是只在 `findClass` 里重写自定义类的加载方法。`loadClass` 这个方法是实现双亲委托模型逻辑的地方，擅自修改这个方法会导致模型被破坏，容易造成问题。
   - 因此最好是在双亲委托模型框架内进行小范围的改动，不破坏原有的稳定结构。同时，也避免了自己重写 `loadClass` 方法的过程中必须写双亲委托的重复代码，从代码的复用性来看，不直接修改这个方法始终是比较好的选择。。

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669751532-9fd11ea7-3675-4b66-8a87-d99be9eac7dc.png#clientId=ud2f063f6-082e-4&from=paste&id=u70531ab5&originHeight=549&originWidth=614&originalType=url&ratio=3&status=done&style=none&taskId=u1b487c2a-6817-49a6-8696-86bb61e9e1f)<br />双亲委派模型的过程<br />1、当 `AppClassLoader`加载一个class时,它首先不会自己去尝试加载这个类,而是把类加载请求委派给父类加载器`ExtClassLoader`去完成。<br />2、当 `ExtClassLoader`加载一个class时,它首先也不会自己去尝试加载这个类,而是把类加载请求委派给`BootStrapClassLoader`去完成。<br />3、如果 `BootStrapClassLoader`加载失败（例如在 `$JAVA_HOME/jre/lib`里未查找到该class）,会使用 `ExtClassLoader`来尝试加载。<br />4、若`ExtClassLoader`也加载失败,则会使用 `AppClassLoader`来加载,如果 `AppClassLoader`也加载失败,则会报出异常 `ClassNotFoundException`。<br />双亲委派模型的好处<br />Java类随着它的类加载器一起具备了带有优先级的层次关系，保证java程序稳定运行。
<a name="JPq5c"></a>
### 2. Tomcat是怎么保证两个应用相同名称类的隔离性
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623669751821-1fa5d811-48e6-4436-afc0-92e7b8982a6c.png#clientId=ud2f063f6-082e-4&from=paste&id=uca6b8c09&originHeight=466&originWidth=446&originalType=url&ratio=3&status=done&style=shadow&taskId=u3c0bf5cf-c794-4fcf-95f9-204588c239f)<br />tomcat下可能会同时部署多个应用，那么tomcat是怎么保证多个应用相同类（比如 SysUserService 类）呢？

- Tomcat 本身也是一个 java 项目，因此其也需要被 JDK 的类加载机制加载，也就必然存在启动类加载器、扩展类加载器和应用系统类加载器。
- 为了保证多应用相同类的唯一性，tomcat在一定程度上打破了双亲委派模型，每启动一个项目都会创建一个唯一的WebApp类加载器，分别用来加载不同的应用，所以就保证了类的唯一性。
