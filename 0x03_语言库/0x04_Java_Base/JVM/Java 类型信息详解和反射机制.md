Java 反射
<a name="npb3p"></a>
## 1、RTTI
RTTI（RunTime Type Information）运行时类型信息，能够在程序运行时发现和使用类型信息，从只能在编译期知晓类型信息并操作的局限中解脱出来<br />传统的多态机制正是 RTTI 的基本使用：假设有一个基类 Shape 和它的三个子类 Circle、Square、Triangle，现在要把 Circle、Square、Triangle 对象放入 `List<Shape>` 中，在运行时，先把放入其中的所有对象都当作 Object 对象来处理，再自动将类型转换为 Shape。所有类型转换的正确性检查都是在运行时进行的，这也正是 RTTI 的含义所在：在运行时，识别一个对象的类型<br />但这样的类型转换并不彻底，Object 只是被转型为 Shape，而不是更具体的 Circle、Square、Triangle，如果希望得到更具体的类型呢？比如说现在需要旋转所有图形，但是想跳过圆形（圆形旋转没有意义），这时可以使用 RTTI 查询某个 Shape 引用所指向对象的确切类型，然后选择进行合适的处理
<a name="9FLwX"></a>
## 2、Class 对象
众所周知，每当编写并编译了一个新类，就会产生一个 Class 对象，它包含了与类有关的信息。可以使用 Class 对象来实现 RTTI，一旦某个类的 Class 对象被载入内存，它就可以用来创建这个类的所有对象<br />Class 对象都属于 Class 类型，既然它也是对象，那就可以获取和操控它的引用。`forName()` 是 Class 类的一个静态方法，可以使用 `forName()` 根据目标类的全限定名（包含包名）得到该类的 Class 对象。使用 `forName()` 会有一个副作用，那就是如果这个类没有被加载就会加载它，而在加载的过程中，Gum 类的 static 初始块会被执行。当 `Class.forName()` 找不到要加载的类，就会抛出异常 ClassNotFoundException
```java
Class gumClass = Class.forName("Gum");
```
使用 `Class.forName()` 不需要先持有这个类型的对象，但如果已经拥有了目标类的对象，那就可以通过调用 `getClass()` 方法来获取 Class 引用，这个方法来自根类 Object，它将返回表示该对象实际类型的 Class 对象的引用
```java
Gum gum = new Gum();
Class gumClass = gum.getClass();
```
另外，还可以调用 `getSuperclass()` 方法来得到父类的 class 对象，再用父类的 Class 对象调用该方法，重复多次，就可以得到一个完整的类继承结构<br />Class 对象的 `newInstance()` 方法可以让你在不知道一个的确切类型的时候创建这个类的对象，使用 `newInstance()` 来创建的类，必须带有无参数的构造器
```java
Object obj = gumClass.newInstance();
```
当然，由于得到的是 Object 的引用，目前只能给它发送 Object 对象能接受的调用。如果想请求具体对象才有的调用，就得先获取该对象的更多类型信息，并执行转型<br />Java 还提供了另一种生成类对象的引用：类字面常量，这样做不仅更简单，而且更安全，因为它在编译时就会收到检查（不用放在 try 语句块中），而且根除了对 `forName()` 方法的调用，效率更高
```java
Class gumClass = Gum.class;
```
类字面常量不仅可以用于普通类，也可以用于接口、数组以及基本数据类型。对于基本数据类型的包装类，还有一个标准字段 Type，Type 字段是一个引用，指向对应基本数据类型的 Class 对象，例如 int.class 就等价于 `Integer.TYPE`。还有一点值得注意的是：使用 `.class` 语法来获得对类对象的引用不会触发初始化<br />到这里都知道了，Class 引用总是指向某个 Class 对象，而 Class 对象可以用于产生类的实例。不过自从 Java 引入泛型以后，就可以使用泛型对 Class 引用所指向的 Class 对象的类型进行限定，让它的类型变得更具体些
```java
Class intClass = int.class;
Class<Integer> genericIntClass = int.class;
intClass = genericIntClass; // 同一个东西
// genericIntClass = double.class 非法
```
好了，既然拿到了 Class 对象，就可以拿到这个类的类型信息，常用的方法如下：

| 方法 | 用途 |
| --- | --- |
| `asSubclass(Class clazz)` | 把传递的类的对象转换成代表其子类的对象 |
| `Cast` | 把对象转换成代表类或是接口的对象 |
| `getClassLoader()` | 获得类的加载器 |
| `getClasses()` | 返回一个数组，数组中包含该类中所有公共类和接口类的对象 |
| `getDeclaredClasses()` | 返回一个数组，数组中包含该类中所有类和接口类的对象 |
| `forName(String className)` | 根据类名返回类的对象 |
| `getName()` | 获得类的完整路径名字 |
| `newInstance()` | 创建类的实例 |
| `getPackage()` | 获得类的包 |
| `getSimpleName()` | 获得类的名字 |
| `getSuperclass()` | 获得当前类继承的父类的名字 |
| `getInterfaces()` | 获得当前类实现的类或是接口 |

<a name="wFXzU"></a>
## 3、类型转换检测
到目前为止，已知的 RTTI 类型包括：

1. 传统的类型转换，如多态
2. 代表对象类型的 Class 对象

RTTI 在 Java 中还有第三种形式，那就是关键字 `instanceof`，它返回一个布尔值，告诉我们对象是不是某个特定类型的实例，可以用提问的方式使用它
```java
if(x instanceof Dog) {
    ((Dog)x).bark();
}
```
Java 还提供了 `Class.isInstance()` 方法动态检测对象类型，例如
```java
0 instance of String // 编译报错
String.class.isInstance(0) // 可以通过编译
```
<a name="7jDt2"></a>
## 4、反射
如果不知道对象的确切类型，RTTI 会告诉你，但是有一个限制：必须在编译时知道类型，才能使用 RTTI 检测它。换句话说，编译器必须知道你使用的所有类<br />看上去这并不是什么特别大的限制，但假设你引用了一个不在程序空间中的对象，比如从磁盘文件或网络连接中获得大量的字节，并被告知这些字节代表一个类，那该怎么办呢？<br />类 Class 支持反射的概念，java.lang.reflect 库中支持类 Field、Method、Constructor（每一个都实现了 Member 接口），这些类型的对象由 JVM 运行时创建，以表示未知类中的对应成员。通常不会直接使用反射，但反射可以用来支持其他 Java 特性，例如对象序列化等<br />Field 代表类的成员变量（成员变量也称为类的属性），Class 类中定义了如下方法用来获取 Field 对象

| 方法 | 用途 |
| --- | --- |
| `getField(String name)` | 获得某个公有的属性对象 |
| `getFields()` | 获得所有公有的属性对象 |
| `getDeclaredField(String name)` | 获得某个属性对象 |
| `getDeclaredFields()` | 获得所有属性对象 |

Field 类定义了如下方法设置成员变量的信息

| 方法 | 用途 |
| --- | --- |
| `equals(Object obj)` | 属性与 obj 相等则返回 true |
| `get(Object obj)` | 获得 obj 中对应的属性值 |
| `set(Object obj, Object value)` | 设置 obj 中对应属性值 |

Method 代表类的方法，Class 类中定义了如下方法用来获取 Method 对象

| 方法 | 用途 |
| --- | --- |
| `getMethod(String name, Class...<?> parameterTypes)` | 获得该类某个公有的方法 |
| `getMethods()` | 获得该类所有公有的方法 |
| `getDeclaredMethod(String name, Class...<?> parameterTypes)` | 获得该类某个方法 |
| `getDeclaredMethods()` | 获得该类所有方法 |

Method 类定义了如下方法对方法进行调用

| 方法 | 用途 |
| --- | --- |
| `invoke(Object obj, Object... args)` | 传递 object 对象及参数调用该对象对应的方法 |

Constructor 代表类的构造器，Class 类中定义了如下方法用来获取 Constructor 对象

| 方法 | 用途 |
| --- | --- |
| `getConstructor(Class...<?> parameterTypes)` | 获得该类中与参数类型匹配的公有构造方法 |
| `getConstructors()` | 获得该类的所有公有构造方法 |
| `getDeclaredConstructor(Class...<?> parameterTypes)` | 获得该类中与参数类型匹配的构造方法 |
| `getDeclaredConstructors()` | 获得该类所有构造方法 |

Constructor 代表类的构造方法

| 方法 | 用途 |
| --- | --- |
| `newInstance(Object... initargs)` | 根据传递的参数创建类的对象 |

除了成员变量、方法和构造器以外，反射还能获取其他更多的信息，例如注解等，具体可查阅 Java API<br />反射的强大威力大家已经看到了，通过反射甚至可以获取到一些“本不应该获取”的信息，例如程序员为了降低耦合，往往会使用接口来隔离组件，但反射却可以轻易破解
```java
public interface A {
    void f();
}
class B implements A {
    public void f() {}
    public void g() {}
}
public class InterfaceViolation {
    public static void main(String[] args) {
        A a = new B();
        a.f();
        // a.g(); // 编译错误
        if (a instanceof B) {
            B b = (B) a;
            b.g();
        }
    }
}
```
通过使用 RTTI，可以发现 a 是用 B 实现的，只要将其转型为 B，就可以调用不在 A 中的方法。如果不希望客户端开发者这样做，那该如何解决呢？一种解决方案是直接声明为实际类型，另一种则是让实现类只具有包访问权限，这样包外部的客户端就看不到实现类了<br />除了这个以外，通过反射可以获得所有成员信息，包括 `private` 的，通常这种违反访问权限的操作并不是十恶不赦的，也许还可以解决某些特定类型的问题
<a name="Z9PMd"></a>
## 5、动态代理
代理是基本的设计模式之一，一个对象封装真实对象，代替真实对象提供其他不同的操作，这些操作通常涉及到与真实对象的通信，因此代理通常充当中间对象。下面是一个简单的静态代理的示例：
```java
interface Interface {    
    void doSomething();
}
class RealObject implements Interface {
    @Override
    public void doSomething() {
        System.out.println("doSomething");
    }
}
class SimpleProxy implements Interface {
    private Interface proxied;
    SimpleProxy(Interface proxied) {
        this.proxied = proxied;
    }
    @Override
    public void doSomething() {
        System.out.println("SimpleProxy doSomething");
        proxied.doSomething();
    }
}
class SimpleProxyDemo {
    public static void consumer(Interface iface) {
        iface.doSomething();
    }
    public static void main(String[] args) {
        consumer(new RealObject());
        consumer(new SimpleProxy(new RealObject()));
    }
}
```
当你希望将额外的操作与真实对象做分离时，代理可能会有所帮助，而 Java 的动态代理更进一步，不仅动态创建代理对象，而且可以动态地处理对代理方法的调用。在动态代理上进行的所有调用都会重定向到一个**调用处理程序**，该程序负责发现调用的内容并决定如何处理，下面是一个简单示例：
```java
class DynamicProxyHandler implements InvocationHandler {
    private Object proxied;
    DynamicProxyHandler(Object proxied) {
        this.proxied = proxied;
    }
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        return method.invoke(proxied, args);
    }
}
class SimpleDynamicProxy {
    public static void consumer(Interface iface) {
        iface.doSomething();
    }
    public static void main(String[] args) {
        RealObject real = new RealObject();
        Interface proxy = (Interface) Proxy.newProxyInstance(
                Interface.class.getClassLoader(),
                new Class[]{Interface.class},
                new DynamicProxyHandler(real));
        consumer(proxy);
    }
}
```
通过调用静态方法 `Proxy.newProxyInstance()` 来创建动态代理，该方法需要三个参数：类加载器、希望代理实现的接口列表、以及接口 InvocationHandler 的一个实现。InvocationHandler 正是所说的调用处理程序，动态代理的所有调用会被重定向到调用处理程序，因此通常为调用处理程序的构造函数提供一个真实对象的引用，以便执行中间操作后可以转发请求。
