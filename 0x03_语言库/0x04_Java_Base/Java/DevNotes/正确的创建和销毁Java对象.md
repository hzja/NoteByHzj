Java
<a name="SBv8A"></a>
### 1、实例构造（Instance Construction）
Java是面向对象的编程语言，所以新实例（objects）的创建可能是它最重要的概念之一。在新的类实例中构造器（Constructors）扮演了非常核心的角色，Java对于构造器（Constructors）的定义提供了很多方案。
<a name="m3W8s"></a>
#### 1.1 隐式（implicitly）构造器
Java允许定义无任何构造器的类，但是这并不意味着此类没有构造器。比如说，看一下下面这个类。
```java
public class NoConstructor {
}
```
此类没有构造器，但是Java编译器会隐式地（implicitly）生成一个构造器并且在使用new关键字创建新的类实例时会被调用。
```java
final NoConstructor noConstructorInstance = new NoConstructor();
```
<a name="jieor"></a>
#### 1.2 无参构造器（Constructors without Arguments）
无参构造器是显式执行Java编译器工作的最简单的方法。
```java
public class NoArgConstructor {
    public NoArgConstructor() {
        // Constructor body here
    }
}
```
在使用`new`关键字创建此类的新实例时会此构造器将会被调用。
```java
final NoArgConstructor noArgConstructor = new NoArgConstructor();
```
<a name="tjV3u"></a>
#### 1.3 有参构造器（Constructors with Arguments）
有参构造器是参数化创建类实例的一个非常有意思和有用的方法。下面这个类定义了一个具有两个参数的构造器。
```java
public class ConstructorWithArguments {
    public ConstructorWithArguments(final String arg1,final String arg2) {
        // Constructor body here
    }
}
```
在这种情况下，当使用`new`关键字创建类实例时，两个构造参数都必须提供。
```java
final ConstructorWithArguments constructorWithArguments = new ConstructorWithArguments( "arg1", "arg2" );
```
非常有意思的是，使用`this`关键字，构造器之间可以相互调用。这种连接构造函数的方式在作为减少代码重复方面是一个非常好的实践，并且从跟本上说这样做可以让一个类只有一个初始化入口点。接上例，添加一个只有一个参数的构造器。
```java
public ConstructorWithArguments(final String arg1) {
this(arg1, null);
}
```
<a name="YBz12"></a>
#### 1.4 初始化块（Initialization Blocks）
Java也提供了另外一种使用初始化块的方式实现初始化逻辑。这个特性很少使用但是非常有必要了解一下它的存在。
```java
public class InitializationBlock {
    // initialization code here
}
```
在某些情况下，初始化块可以弥补匿名无参构造器的缺陷。有一些特殊的类可能会有很多个初始化块并且他们会依次按照他们在代码中定义的顺序被调用，比如：
```java
public class InitializationBlocks {
    {
        // initialization code here
    } {
        // initialization code here
    }
}
```
初始化块并不是替代构造器并且他们可以独立于构造器而存在。但是需要提及的最重要的一点就是初始化块会在任何构造器被调用之前被执行。
```java
public class InitializationBlockAndConstructor {
    {
        // initialization code here
    }
    public InitializationBlockAndConstructor() {
    }
}
```
<a name="ghZkG"></a>
#### 1.5 构造保障（Construction guarantee）
Java提供了一些开发者所依赖的初始化保障，未初始化的实例和类参数会自动初始化为它们的默认值。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622870939966-0d24140c-be85-4ba4-9209-db5d370b46d7.webp#clientId=u63eb5956-5d6f-4&from=paste&id=u56b6c2e2&originHeight=432&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u785c7961-515a-4b7c-8641-77a984f37f0)<br />让使用下面的例子来确认一下这些默认值。
```java
public class InitializationWithDefaults {
    private boolean booleanMember;
    private byte byteMember;
    private short shortMember;
    private int intMember;
    private long longMember;
    private char charMember;
    private float floatMember;
    private double doubleMember;
    private Object referenceMember;

    public InitializationWithDefaults() {
        System.out.println( "booleanMember = " + booleanMember );
        System.out.println( "byteMember = " + byteMember );
        System.out.println( "shortMember = " + shortMember );
        System.out.println( "intMember = " + intMember );
        System.out.println( "longMember = " + longMember );
        System.out.println( "charMember = " +
        Character.codePointAt( new char[] { charMember }, 0 ) );
        System.out.println( "floatMember = " + floatMember );
        System.out.println( "doubleMember = " + doubleMember );
        System.out.println( "referenceMember = " + referenceMember );
    }
}
```
一旦使用new关键字实例化：
```java
inal InitializationWithDefaults initializationWithDefaults = new InitializationWithDefaults();
```
将会在控制台输出如下结果：
```java
booleanMember = false
byteMember = 0
shortMember = 0
intMember = 0
longMember = 0
charMember = 0
floatMember = 0.0
doubleMember = 0.0
referenceMember = null
```
<a name="UazmX"></a>
#### 1.6 可见性（Visibility）
构造器受Java可见性规则约束并且可以拥有访问控制修饰符来决定是否其他类可以调用特定的构造函数。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1622870939970-1c90e6e2-4d27-45b9-9861-e47a1050b5cb.png#clientId=u63eb5956-5d6f-4&from=paste&id=u50f6d8bf&originHeight=215&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u54746aa6-9330-4771-a7ac-77214ee0add)
<a name="DkVcm"></a>
#### 1.7 垃圾回收（Garbage collection）
Java（特别是JVM）使用自动垃圾回收机制。简而言之，当新对象被创建，JVM就会自动为这些新创建的对象分配内存。于是，当这些对象没有任何引用的时候，他们就会被销毁并且他们所占用的内存就会被回收。<br />Java垃圾回收是分代的，基于这种假设（分代假设）大多数的对象在很年轻的时候就已经不可达（在他们被创建之后的很短的时间内就没有任何引用并且被安全销毁）。大多数开发者曾经相信在Java中创建对象是很慢的并且应该尽可能地避免新对象的实例化。<br />实际上，这并不成立：在Java中创建对象的开销非常的小并且很快。虽然如此，但是没有必要创建生命周期比较长的对象，因为创建过多的长寿命对象最终可能会填满老年代空间从而引发stop-the-world的垃圾回收，这样的话开销就会比较大。
<a name="XNFei"></a>
#### 1.8 终结器（Finalizers）
到目前为止，已经谈到了构造函数和对象初始化，但实际上并没有提到任何关于对象销毁的内容。这是因为Java使用垃圾收集器去管理对象的生命周期，并且垃圾收集器的责任就是去销毁无用对象并回收这些对象占用的内存。<br />然而，在Java中有一个被称为终结器（Finalizers）的特殊特性，它有点类似于析构函数，但是在执行资源清理时它所解决的是不同的意图。终结器（Finalizers）是被考虑用来解决一些危险的特征（比如会导致无数的副作用和性能问题的问题）。<br />一般来说，他们是没有必要的，应该避免（除了非常罕见的情况下，主要是有关本地对象）。Java 7语言引入了一种名为try-with-resources的更好的替代方法和AutoCloseable接口，它允许像如下的方式这样干净的写代码：
```java
try ( final InputStream in = Files.newInputStream( path ) ) {
    // code here
}
```
<a name="HmO6r"></a>
### 2、静态初始化（Static initialization）
到目前为止，，已经谈到了构造函数和对象初始化。但是Java也支持类级别的初始化构造，称之为静态初始化（Static initialization）。<br />静态初始化（Static initialization）有点类似于初始化块，除了需要添加`static`关键字之外。注意静态初始化在每次类加载的时候它只执行一次。比如：
```java
public class StaticInitializationBlock {
    static {
        // static initialization code here
    }
}
```
和初始化块类似，在类定义时可以包含任意数量的初始化块，它们会根据在类代码中出现的顺序依次执行，比如：
```java
public class StaticInitializationBlocks {
    static {
        // static initialization code here
    }
    static {
        // static initialization code here
    }
}
```
因为静态初始化（Static initialization）块可以从多个并行线程中触发（第一次类加载发生），Java运行时保证在线程安全的前提下仅仅被执行一次。
<a name="xdRiN"></a>
### 3、构造模式（Construction Patterns）
过去这几年很多易于理解和广泛应用的构造模式在Java社区出现。介绍几个比较常用的：单例模式（singleton）、帮助器（helpers）、工厂模式（factory）、依赖注入（dependency injection ）——大家熟知的控制反转（inversion of control）。
<a name="qXFe3"></a>
#### 3.1 单例模式（Singleton）
单例模式是软件开发者社区中最老也是最具争议性的模式之一。基本来说，它的主要思想就是确保在任何时候类仅仅只有一个实例被创建。思想就是如此简单，然而单例模式引发了很多关于如何使之正确的讨论，特别是线程安全的讨论。下面是单例模式原生版本的例子：
```java
public class NaiveSingleton {
    private static NaiveSingleton instance;

    private NaiveSingleton() {
    }

    public static NaiveSingleton getInstance() {
        if( instance == null ) {
            instance = new NaiveSingleton();
        }
        return instance;
    }
}
```
这段代码至少有一个问题就是如果多个线程同时调用，那么此类就能够创建多个实例。设计合适的单例模式的方法之一是使用类的 `static` `final`属性。<br />final property of the class.
```java
public class EagerSingleton {
    private static final EagerSingleton instance = new EagerSingleton();

    private EagerSingleton() {
    }

    public static EagerSingleton getInstance() {
        return instance;
    }
}
```
如果不想浪费资源并且希望在单例对象真正需要的时候才被延迟创建的话，这就要求显示同步了（explicit synchronization），这就有可能导致多线程环境中的并发性降低。
```java
public class LazySingleton {
private static LazySingleton instance;
    private LazySingleton() {
    }

    public static synchronized LazySingleton getInstance() {
        if( instance == null ) {
            instance = new LazySingleton();
        }
        return instance;
    }
}
```
如今，在大多数的案例中单例模式并不被考虑作为一个很好的选择，主要是因为单例模式将会导致代码很难测试。依赖注入模式让单例模式变得没有必要。
<a name="Udvxg"></a>
#### 3.2 Utility/Helper类
utility或者helper类是被许多开发者所使用的相当流行的一种模式。基本来说，它所代表的是无实例（ non-instantiable）类（构造器被定义成private），仅仅可以选择将方法定义成final（后续会介绍如何定义类）或者static。比如;
```java
public final class HelperClass {
    private HelperClass() {
    }

    public static void helperMethod1() {
        // Method body here
    }

    public static void helperMethod2() {
        // Method body here
    }
}
```
站在开发者的角度，helpers类经常所扮演的是一个容器的角色，这个容器中放了很多在其他地方找不到但是其他类需要相互共享和使用的互相不相关的方法。这种设计决定了在很多情况下要避免使用：总能找到另一种重用所需功能的方式，保持代码的简洁和清晰。
<a name="jYLq6"></a>
#### 3.3 工厂模式（Factory）
工厂模式被证明是软件开发人员手中非常有用的技术。因此，Java有几种风格工厂模式，从工厂方法到抽象工厂。工厂模式最简单的例子是返回特定类的新实例的静态方法（工厂方法）。例如：
```java
public class Book {
    private Book( final String title) {
    }
    public static Book newBook( final String title ) {
        return new Book( title );
    }
}
```
有人可能会争辩说，介绍newBook工厂方法并没有什么意义，但是使用这种模式通常会使代码更具可读性。工厂模式的另一个变化涉及接口或抽象类（抽象工厂）。例如，定义一个工厂接口：
```java
public interface BookFactory {
    Book newBook();
}
```
依赖库类型，完成几种不同的实现：
```java
public class Library implements BookFactory {
    @Override
    public Book newBook() {
        return new PaperBook();
    }
}

public class KindleLibrary implements BookFactory {
@Override
    public Book newBook() {
        return new KindleBook();
    }
}
```
现在，Book的特定类被隐藏在BookFactory接口实现之后，BookFactory仍然提供创建book的通用方式。
<a name="KTdap"></a>
#### 3.4 依赖注入（Dependency Injection）
依赖注入（一说控制反转）被类设计者认为是一个很好的做法：如果某些类的实例依赖其他类的实例，被依赖的实例应该通过构造（比如通过设置器——setters，或者策略——strategies模式等）的思想提供给依赖的实例，而不是依赖的实例自行创建。看一下下面这种情况：
```java
import java.text.DateFormat;
import java.util.Date;
public class Dependant {
    private final DateFormat format = DateFormat.getDateInstance();

    public String format( final Date date ) {
        return format.format( date );
    }
}
```
类Dependant需要一个DateFormat的实例，并且它仅仅只是在构造时通过调用DateFormat.getDateInstance() 创建。最好的设计方案应该是通过构造器参数的形式去完成相同的事情。
```java
import java.text.DateFormat;
import java.util.Date;
public class Dependant {
    private final DateFormat format;

    public Dependant( final DateFormat format ) {
        this.format = format;
    }


    public String format( final Date date ) {
        return format.format( date );
    }
}
```
按这种方案实现的话，类的所有依赖都是通过外部提供，这样就很容易的修改date format和为类写测试用例。
