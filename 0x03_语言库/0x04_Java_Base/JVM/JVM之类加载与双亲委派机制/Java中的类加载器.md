Java 类加载器
<a name="By8Ek"></a>
### 前言
对于每个开发人员来说，java.lang.ClassNotFoundExcetpion这个异常几乎都遇到过，而追求其该异常的来源的话，就免不了谈一谈Java的类加载器了。
<a name="xYE60"></a>
### 类加载器简介
Java程序被编译器编译之后成为字节码文件（.class文件），当程序需要某个类时，虚拟机便会将对应的class文件进行加载，创建出对应的Class对象。而这个将class文件加载到虚拟机内存的过程，便是类加载。<br />![2021-05-04-10-50-57-627713.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620096799715-9c514167-3b1f-4ca1-9f3b-c86be40b61cc.jpeg#clientId=uaacdcacb-c3fa-4&from=ui&id=u0e5d0739&originHeight=437&originWidth=1080&originalType=binary&size=43249&status=done&style=shadow&taskId=ua60a35dd-513a-4238-8882-9a3a92e7e40)<br />类加载器负责在运行时将Java类动态加载到JVM（Java虚拟机），是JRE（Java运行时环境）的一部分。由于类加载器的存在，JVM无需了解底层文件或文件系统即可运行Java程序。<br />Java类不会一次全部加载到内存中，而是在应用程序需要时才会加载。此时，类加载器负责将类加载到内存中。
<a name="eJBAg"></a>
### 类加载的过程
类的生命周期通常包括：加载、链接、初始化、使用和卸载。上图中包含了类加载的三个阶段：加载阶段、链接阶段和初始化阶段。如果将这三个阶段再拆分细化包括：加载、验证、准备、解析和初始化。<br />![2021-05-04-10-50-58-016970.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620096807731-eaaabdad-f9be-4e52-9be5-31bbddd6698c.jpeg#clientId=uaacdcacb-c3fa-4&from=ui&id=ucc0ce2f0&originHeight=413&originWidth=1080&originalType=binary&size=46201&status=done&style=shadow&taskId=ucc9d2e3a-9bfe-4f20-a577-bba167ceb21)<br />关于这几个阶段的作用简单概况一下：

- 加载：通过一个类的完全限定查找类字节码文件，转化为方法区运行时的数据结构，创建一个代表该类的Class对象。
- 验证：确保Class文件的字节流中包含信息符合当前虚拟机要求，不会危害虚拟机自身安全。
- 准备：为类变量(即static修饰的字段变量)分配内存并且设置该类变量的初始值。不包含被final修饰的static变量，因为它在编译时已经分配了。
- 解析：将常量池内的符号引用转换为直接引用的过程。如果符号引用指向一个未被加载的类，或者未被加载类的字段或方法，那么解析将触发这个类的加载。
- 初始化：类加载最后阶段，若该类具有超类，则对其进行初始化，执行静态初始化器和静态初始化成员变量。

在上述类加载的过程中，虚拟机内部提供了三种类加载器：启动（Bootstrap）类加载器、扩展（Extension）类加载器、系统（System）类加载器（也称应用类加载器）。<br />下面就讨论不同类型的内置类加载器是如何工作，以及介绍如何自定义类加载器。
<a name="VrFlk"></a>
### 内置类加载器
先从一个简单的例子来看一下如何使用不同的加载器来加载不同的类：
```java
public void printClassLoaders() {

    System.out.println("Classloader of this class:"
        + PrintClassLoader.class.getClassLoader());

    System.out.println("Classloader of Logging:"
        + Logging.class.getClassLoader());

    System.out.println("Classloader of ArrayList:"
        + ArrayList.class.getClassLoader());
}
```
执行上述程序，打印如下内容：
```java
Classloader of this class:sun.misc.Launcher$AppClassLoader@18b4aac2
Classloader of Logging:sun.misc.Launcher$ExtClassLoader@2f0e140b
Classloader of ArrayList:null
```
上述三行输出分别对应三种不同的类加载器：系统（System）类加载器、扩展（Extension）类加载器和启动（Bootstrap）类加载器（显示为null）。<br />系统程序类加载器加载包含示例方法的类，也就是将自己的文件加载到类路径中。扩展类加载器加载Logging类，也就是加载作为标准核心Java类扩展的类。启动类加载器加载ArrayList类，是所有其他类的父级。<br />对于ArrayList的类加载器，输出为null。这是因为启动类加载器是用本机代码实现而不是Java，因此它不会显示为Java类。启动类加载器在操作在不同的JVM中会有所不同。<br />上述三种类加载器，外加自定义类加载器，它们之间的关系可用下图表示： <br />![2021-05-04-10-50-58-439668.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620096822741-4e089a6a-b0c3-4e64-beaf-66e35b28e1be.jpeg#clientId=uaacdcacb-c3fa-4&from=ui&id=u05c95e1c&originHeight=792&originWidth=1080&originalType=binary&size=78578&status=done&style=shadow&taskId=ubfdade57-746c-488f-a646-45fd520eba4)<br />现在来具体看一下这些类加载器。
<a name="jMN6C"></a>
#### Bootstrap类加载器
Java类由`java.lang.ClassLoader`的实例加载。但是，类加载器本身就是类。那么，谁来加载`java.lang.ClassLoader`？对，就是启动类加载器。<br />启动类加载器主要负责加载JDK内部类，通常是rt.jar和`$JAVA_HOME/jre/lib`目录中的其他核心库。此外，Bootstrap类加载器还充当所有其他`ClassLoader`实例的父类。<br />该启动程序类加载器是Java虚拟机的一部分，用本机代码编写（比如，C++），不同的平台的实现可能有所不同。<br />出于安全考虑，Bootstrap启动类加载器只加载包名为java、javax、sun等开头的类。
<a name="aj9Ps"></a>
#### Extension类加载器
扩展类加载器是启动类加载器的子类，Java语言编写，由`sun.misc.Launcher$ExtClassLoader`实现，父类加载器为启动类加载器，负责加载标准核心Java类的扩展。<br />扩展类加载器从JDK扩展目录（通常是$JAVA_HOME/lib/ext目录）或java.ext.dirs系统属性中指定的任何其他目录进行自动加载。
<a name="jo0wL"></a>
#### 系统类加载器
系统类加载器负责将所有应用程序级类加载到JVM中。它加载在类路径环境变量，`-classpath`或`-cp`命令行选项中找到的文件。它是扩展类加载器的子类。<br />系统类加载器，也称应用程序加载器是指 Sun公司实现的`sun.misc.Launcher$AppClassLoader`，负责加载系统类路径`-classpath`或`-D java.class.path`指定路径下的类库，也就是经常用到的`classpath`路径，开发者可以直接使用系统类加载器，一般情况下该类加载是程序中默认的类加载器，通过`ClassLoader#getSystemClassLoader()`方法可以获取到该类加载器。
<a name="TNv6B"></a>
### 类加载器是如何工作的
类加载器是Java运行时环境的一部分。当JVM请求一个类时，类加载器将尝试定位该类，并使用完全限定名将类定义装入运行时。<br />![2021-05-04-10-50-58-937989.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620096846935-4ae10800-0016-4c9d-9a8f-6c14c2b90a38.jpeg#clientId=uaacdcacb-c3fa-4&from=ui&id=u4597c800&originHeight=877&originWidth=1080&originalType=binary&size=95411&status=done&style=shadow&taskId=u166c452c-187d-4399-89d8-467bd5feaf0)<br />`java.lang.ClassLoader.loadClass()`方法负责将类定义加载到运行时，它尝试通过全限定名来加载类。如果未加载到该类，则它将请求委派给父类加载器。依次向上重复该过程。<br />最终，如果父类加载器找不到指定类，则子类将调用`java.net.URLClassLoader.findClass()`方法在文件系统本身中查找类。<br />如果最后一个子类加载器也无法加载该类，则它将抛出`java.lang.NoClassDefFoundError`或
```java
java.lang.ClassNotFoundException。抛出ClassNotFoundException时的输出示例:
java.lang.ClassNotFoundException: com.baeldung.classloader.SampleClassLoader    
    at java.net.URLClassLoader.findClass(URLClassLoader.java:381)    
    at java.lang.ClassLoader.loadClass(ClassLoader.java:424)    
    at java.lang.ClassLoader.loadClass(ClassLoader.java:357)    
    at java.lang.Class.forName0(Native Method)    
    at java.lang.Class.forName(Class.java:348)
```
上述过程，通常称作双亲委派机制。双亲委派机制要求除了顶层的启动类加载器外，其余的类加载器都应当有自己的父类加载器，请注意双亲委派机制中的父子关系并非通常所说的类继承关系，而是采用组合关系来复用父类加载器的相关代码。<br />除外，类加载器还具有三个重要功能：委派模型、类的唯一性和可见性。
<a name="ULhCB"></a>
#### 委派模型
类加载器遵循委派模型，在该模型中，根据请求查找类或资源，`ClassLoader`实例会将对类或资源的搜索委托给父类加载器。<br />假设有一个将应用程序类加载到JVM中的请求。系统类加载器首先将该类的加载委托给其父扩展类加载器，而父扩展类加载器又将其委托给引导类加载器。<br />仅当启动类加载器和扩展类加载器都未能成功加载类时，系统类加载器才会尝试加载类本身。
<a name="zeSLA"></a>
#### 类的唯一性
作为委托模型的结果，很容易确保类的唯一性，因为始终尝试向上委托。如果父类加载器无法找到该类，则只有当前实例自己会尝试进行查找和加载。
<a name="vgjVr"></a>
#### 可见性
此外，子类加载器对其父类加载器加载的类可见。例如，系统类加载器加载的类对扩展和Bootstrap类加载器加载的类具有可见性，反之亦然。<br />比如，通过系统类加载器加载类A，而通过扩展类加载器加载类B，则对系统类加载器加载的其他类而言，A和B类都是可见的。但对扩展类加载器加载的其他类而言，类B是唯一可见的类。
<a name="jyBLT"></a>
### 自定义类加载器
在大多数情况下，如果文件已经在文件系统中，则内置的类加载器就足够了。但是，在需要从本地硬盘驱动器或网络中加载类的情况下，可能需要使用自定义类加载器。下面介绍自定义类加载器的使用。
<a name="yOv5v"></a>
#### 自定义类加载器示例
自定义类加载器不仅对在运行时加载类有帮助，还有一些特殊的场景：

- 帮助修改现有的字节码，例如weaving agents；
- 动态创建适合用户需求的类。例如在JDBC中，通过动态类加载完成不同驱动程序实现之间的切换。
- 在为具有相同名称和程序包的类加载不同的字节码时，实现类版本控制机制。这可以通过URL类加载器（通过URL加载jar）或自定义类加载器来完成。

举一个更具体的例子，比如，浏览器使用自定义类加载器从网站加载可执行内容。浏览器可以使用单独的类加载器从不同的网页加载applet。用于运行`applet`的`applet`查看器包含一个`ClassLoader`，该类加载器可访问远程服务器上的网站，而无需查看本地文件系统。<br />然后通过HTTP加载原始字节码文件，并将其转换为JVM中的类。即使这些applet具有相同的名称，但如果由不同的类加载器加载，它们也被视为不同的组件。<br />现在，了解了为什么自定义类加载器是相关的，实现`ClassLoader`的子类来扩展和总结JVM如何加载类的。
<a name="VkKE9"></a>
#### 创建自定义类加载器
自定义类加载器通常通过继承`java.lang.ClassLoader`类，重写`findClass()`方法：
```java
public class CustomClassLoader extends ClassLoader {

    @Override
    public Class findClass(String name) throws ClassNotFoundException {
        byte[] b = loadClassFromFile(name);
        return defineClass(name, b, 0, b.length);
    }

    private byte[] loadClassFromFile(String fileName)  {
        InputStream inputStream = getClass().getClassLoader().getResourceAsStream(
            fileName.replace('.', File.separatorChar) + ".class");
        byte[] buffer;
        ByteArrayOutputStream byteStream = new ByteArrayOutputStream();
        int nextValue = 0;
        try {
            while ( (nextValue = inputStream.read()) != -1 ) {
                byteStream.write(nextValue);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        buffer = byteStream.toByteArray();
        return buffer;
    }
}
```
在上面的示例中，定义了一个自定义类加载器，该类加载器扩展了默认类加载器并从指定文件加载字节数组。如果没有太复杂的需求，可以直接继承`URLClassLoader`类，重写`loadClass`方法，具体可参考`AppClassLoader`和`ExtClassLoader`。
<a name="KmnOA"></a>
### 了解`java.lang.ClassLoader`
下面来看看`java.lang.ClassLoader`类中的一些基本方法，以更清楚地了解其工作方式。
<a name="fy1Nv"></a>
#### loadClass方法
```java
public Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
```
此方法负责加载给定名称参数的类。name参数为类的全限定名。<br />Java虚拟机调用`loadClass()`方法来解析类引用，并将resolve设置为true。但是，不一定总是要解析一个类。如果只需要确定该类是否存在，则将resolve参数设置为false。<br />此方法用作类加载器的入口。可以尝试从`java.lang.ClassLoader`的源代码中了解loadClass()方法的内部工作：
```java
protected Class<?> loadClass(String name, boolean resolve)
    throws ClassNotFoundException {

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
                c = findClass(name);
            }
        }
        if (resolve) {
            resolveClass(c);
        }
        return c;
    }
}
```
该方法的默认实现按以下顺序搜索类：

- 调用`findLoadedClass(String)`方法以查看是否已加载该类。
- 在父类加载器上调用`loadClass(String)`方法。
- 调用`findClass(String)`方法以查找类。
<a name="FUd6L"></a>
#### `defineClass`方法
```java
protected final Class<?> defineClass(
  String name, byte[] b, int off, int len) throws ClassFormatError
```
此方法负责将字节数组转换为类的实例。如果数据不包含有效的类，则会抛出`ClassFormatError`。另外，由于此方法被标记为`final`，因此无法覆盖此方法。
<a name="MeQ7f"></a>
#### `findClass`方法
```java
protected Class<?> findClass(
  String name) throws ClassNotFoundException
```
此方法查找以标准名称作为参数的类。需要在遵循委派模型加载类的自定义类加载器实现中重写此方法。<br />另外，如果父类加载器找不到请求的类，则`loadClass()`会调用此方法。如果没有任何类加载器的父类找到该类，则默认实现会抛出`ClassNotFoundException`异常。
<a name="geBdc"></a>
#### `getParent`方法
```java
public final ClassLoader getParent()
```
此方法返回父类加载器以进行委派。某些实现使用null来表示启动类加载器。
<a name="qeNMH"></a>
#### `getResource`方法
```java
public URL getResource(String name)
```
此方法尝试查找具有给定名称的资源。它将首先委托给资源的父类加载器，如果父级为null，则搜索虚拟机内置的类加载器的路径。如果失败，则该方法将调用`findResource(String)`来查找资源。<br />指定为输入的资源名称可以相对于类路径，也可以是相对于绝对路径。<br />它返回用于读取资源的URL对象；如果找不到资源或调用者没有足够的特权来返回资源，则返回null。<br />需要注意的是，Java是从类路径中加载资源。<br />最后，Java中的资源加载被认为是与位置无关的，因为只要设置了环境来查找资源，代码在何处运行都无关紧要。
<a name="YBzIB"></a>
### 上下文类加载器
通常，上下文类加载器为J2SE中引入的类加载委托方案提供了一种替代方法。JVM中的类加载器遵循分层模型，因此每个类加载器都有一个单独的父类，而启动类加载器除外。但是，有时当JVM核心类需要动态加载应用程序开发人员提供的类或资源时，可能会遇到问题。<br />例如，在JNDI中，核心功能由rt.jar中的引导程序类实现。但是这些JNDI类可能会加载由独立供应商实现的JNDI提供程序（部署在应用程序类路径中）。这种情况要求启动类加载器（父类加载器）加载对应程序加载器（子类加载器）可见的类。<br />线程上下文类加载器（context class loader）是从JDK 1.2开始引入的。Java.lang.Thread中的方法 `getContextClassLoader()`和`setContextClassLoader(ClassLoader cl)`用来获取和设置线程的上下文类加载器。如果没有通过`setContextClassLoader(ClassLoader cl)`方法进行设置的话，线程将继承其父线程的上下文类加载器。Java应用运行的初始线程的上下文类加载器是系统类加载器，在线程中运行的代码可以通过此类加载器来加载类和资源。<br />线程上下文类加载器从根本解决了一般应用不能违背双亲委派模式的问题，使得java类加载体系显得更灵活。上面所提到的问题正是线程上下文类加载器的拿手好菜。如果不做任何的设置，Java应用的线程上下文类加载器默认就是系统类加载器。因此，在SPI接口的代码中使用线程上下文类加载器，就可以成功的加载到SPI实现的类。
<a name="WtNDq"></a>
### 小结
类加载器对于执行Java程序是必不可少的。先学习了不同类型的类加载器，即Bootstrap类加载器、扩展类加载器和系统类加载器。Bootstrap类加载器充当所有类加载器的父级，负责加载JDK内部类。扩展类加载器和系统类加载器分别从Java扩展目录和类路径加载类。然后，学习了类加载器的工作原理、特性，以及如何创建自定义类加载器。
