Java 接口相信所有学过 Java 的人一定都知道，而且 99% 的人一定都背过这个面试题：Java 接口和抽象类的区别是什么？答案都是什么接口方法不能有实现，都是抽象的，接口的方法都默认为 public 修饰等等之类的，这些在 Java 8 之前是正确的，但是随着 Java 的发展，它不再是正确的了。<br />一起来探索 Java 接口的进化之路。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1700443993954-1542bf27-d062-4902-bcbc-794460570e94.jpeg)
<a name="ujjzQ"></a>
## 出生：Java 1.0 引入接口
Java 1.0 ，作为 Java 语言的最初版本，于 1996 年发布。在这个版本中，Java 引入了接口（`Interface`）这一概念，标志着面向对象编程（OOP）在 Java 中的深入应用。<br />在 Java 1.0 中，接口被定义为完全抽象的，它具备如下特点：

- **完全抽象**：接口被定义为完全抽象，意味着它不能包含任何方法的具体实现。
- **只能包含方法签名和常量**：接口中只能声明方法签名（没有方法体）和公共静态常量（`public static final`）。这意味着，在接口中定义的所有字段都会自动成为公共、静态、最终的，而所有方法都被视为公共的和抽象的。
- **没有构造函数**：接口不能被实例化，所以它们不能包含构造函数。
- **多重实现**：不同于类的继承，Java 允许一个类实现多个接口。由于 Java 只允许单继承，所以接口的出现，为 Java 的多重继承提供了实现的可能。
- **实现类必须实现所有方法**：任何一个实现接口的类都必须实现接口中声明的所有方法，除非该类被声明为抽象类。

这个时候定义接口只能定义为如下形式：
```java
public interface MyInterface {
    String SK_URL = "https://example.com/";

    String getSeriesUrl();
}
```
属性 SK_URL 默认为 `public static final`。<br />Java 1.1 引入内部类，内部类提供了一种强大的机制，它允许将一个类的定义放置在另一个类的定义内部，从而与外部类紧密地绑定在一起。内部类中的匿名内部类，可以用于实现接口，这对接口的使用有着特别的意义，它提高了代码的封装性和可维护性。
<a name="tR2M4"></a>
## 会爬了：Java 1.5 引入泛型
Java 5 于 2004 年发布，它引入了泛型，泛型的引入对接口的使用产生了深远的影响。<br />在引入泛型之前，Java 接口通常处理的是 Object 类型的数据，这意味着任何类型的对象都可以传递给这些接口。在实现类中我们需要将 Object 类型的数据转换为其它具体类型，这种转换可能会导致  ClassCastException。<br />引入泛型后，接口可以在定义时就指定具体的类型参数，这使得接口在编译时就能够检查和确认数据类型，从而大大增强了类型安全。同时，泛型允许接口定义通用的模板，例如`List<E>` 或 `Comparator<T>`，这些接口可以被不同类型的数据重用，而不需要为每种数据类型创建新的接口。<br />下面举一个简单的例子来说明，引入 泛型对接口好处。<br />在 Java 5 引入泛型之前，集合类如 List 接口存储的元素类型是不确定的，它们默认存储 Object 类型的对象。这使得集合可以存储任何类型的对象，但也带来了类型安全问题。
```java
List list = new ArrayList();
list.add("skjava.com");
list.add(1); // 这是合法的，因为集合可以存储任何类型的对象

// 获取时需要进行类型转换
String str = (String) list.get(0);
Integer num = (Integer) list.get(1);
```
list 可以同时存储 String 和 Integer。但是，在获取元素时，我们需要进行显式的类型转换。如果在运行时类型不匹配，会抛出 ClassCastException。<br />引入泛型后，可以在定义集合时指定存储的元素类型。这提高了类型安全，并减少了运行时的错误。
```java
List<String> list = new ArrayList<>();
list.add("skjava.com");
// list.add(1);       // 这行代码会导致编译错误，因为 list 只能存储字符串

// 获取时不需要进行类型转换
String str = list.get(0);
```
list 被定义为仅能存储 String 类型的 List，当尝试添加非 String 类型（如 Integer）到该 list 中会在编译时报错，而不是在运行时。同时，获取元素时也不需要显式的类型转换，因为编译器已经确保了所有元素都是 String 类型。
<a name="iJLW7"></a>
## 会走路了：Java 8 引入默认方法和静态方法
Java 8 是一个重大的里程碑，因为它为Java接口引入了两个重要的新特性：默认方法和静态方法，这两个特性让 Java 接口可以走路了。<br />在 Java 8 之前，接口中申明的方法必须是抽象的，实现该接口就需要实现该接口的所有方法。我们知道接口的设计是一项巨大的工作，因为如果我们需要在接口中新增一个方法，需要对它的所有实现类都进行修改，如果它的实现类比较少还可以接受，如果实现类比较多则工作量就比较大了。而且有些实现类根本不需要实现该方法，也由于 Java 接口的限制导致它强迫实现该方法。<br />为了解决这个问题，Java 8 引入了默认方法，默认方法允许在接口中添加具有默认实现的方法，它使得接口可以包含方法的实现，而不仅仅是抽象方法的定义。这一特性可以在不破坏现有实现的前提下向接口添加新方法，使得接口具备了向后兼容的能力。<br />用法如下：

- 使用`default`关键字标识方法，然后方法提供具体实现。
```java
public interface MyInterface {

    default void print() {
        System.out.println("我是大明哥");
    }
}
```
实现接口的类可以直接使用这些默认方法，或者根据需要重写它们：
```java
public class MyInterfaceImpl implements MyInterface{

    @Override
    public void print() {
        System.out.println("我是大明哥 2 号");
    }
}
```
Java 8 中还允许在接口中定义静态方法。这些方法与类中的静态方法一样，是属于接口的而不是接口的实例。静态方法主要用于提供辅助方法，例如根据传入参数构建接口的实现，或者提供通用的工具方法。<br />用法：

- 静态方法使用`static`关键字声明，并且必须提供实现。
- 是通过接口名直接调用静态方法，不需要接口的实例。
```java
public interface MyInterface {
    static int add(int a, int b) {
        return a + b;
    }
}
```
`add()` 是通过`MyInterface.add(5, 3)`直接调用，而不需要创建`MyInterface`的实例。<br />Java 引入默认方法和静态方法大大增强了 Java 接口的能力，提高了 Java 接口的灵活性和功能性。<br />更多关于接口默认方法和静态方法，阅读：Java 8 新特性—接口默认方法和静态方法
<a name="Yfgot"></a>
## 可以跑了：Java 9 引入私有方法
Java 8 为引入了默认方法和静态方法，提高了 Java 接口的灵活性和功能性。但是这些依然是 public 的，这在某些情况下会导致了代码重复和封装性不足的问题。为了解决这个问题，Java 9 引入私有方法。

- 私有方法只能在定义它们的接口内部被访问。这就意味着接口可以有自己的内部逻辑，而不必将所有逻辑都暴露给实现该接口的类。这样，接口的设计者可以更好地控制接口的行为，并防止实现类误用接口内部的代码，增强了接口的封装性。
- 由于私有方法只能在接口内部使用，无法在接口的实现类中被滥用，这有助于保持接口的一致性和约定。

在接口中使用私有方法有如下几个限制：

1. 私有方法不能是抽象的。
2. 私有方法只能在接口内部使用，无法被接口的实现类或外部类访问。
3. 私有方法不会继承给接口的子接口，每个接口都必须自己定义自己的私有方法。
4. 私有静态方法可以在其他静态和非静态接口方法中使用。
5. 私有非静态方法不能在私有静态方法内部使用。

下面是私有方法的简单使用：
```java
public interface MyInterface {
    private void print() {
        System.out.println("我是大明哥");
    }
}
```
<a name="LlJm3"></a>
## 总示例
下面用一个例子来演示上面所有功能。
```java
public interface MyInterface {
    String NAME = "MyInterface";

    /**
     * 抽象方法，所有实现类都要实现
     * @return
     */
    void abstractMethod();

    /**
     * 默认方法
     */
    default void defaultMethod() {
        //默认方法里可以调用私有方法
        privateMethod();

        // 默认方法可以调用静态私有方法
        staticPrivateMethod();

        System.out.println("默认方法-MyInterface-defaultMethod");
    }

    /**
     * 静态方法
     * @return
     */
    static void staticMethod() {
        // 静态方法只能调用静态私有方法，不能调用私有方法
        System.out.println("静态方法-MyInterface-staticMethod");
    }

    /**
     * 私有方法
     */
    private void privateMethod() {
        System.out.println("私有方法-MyInterface-privateMethod");
    }

    /**
     * 静态私有方法
     */
    private static void staticPrivateMethod() {
        System.out.println("静态私有方法-MyInterface-staticPrivateMethod");
    }
}
```
接口 MyInterface 里面定义了抽象方法，默认方法，静态方法，私有方法。这里要注意一下几点：

1. 私有方法为接口所有，只能在接口的默认方法中调用。
2. 静态私有方法既可以在静态方法中调用，也可以在默认方法中调用。
3. 默认方法也可以调用静态方法，但是静态方法不能调用默认方法。
- 两个实现类
```java
public class MyImplements1 implements MyInterface{

    @Override
    public void abstractMethod() {
        // 调用接口的静态方法
        MyInterface.staticMethod();

        System.out.println("抽象方法-MyImplements1-getName");
    }


    /**
     * 重写默认方法
     */
    @Override
    public void defaultMethod() {
        System.out.println("重写默认方法-MyImplements1-defaultMethod");
    }
}


public class MyImplements2 implements MyInterface{
    @Override
    public void abstractMethod() {
        // 调用接口的静态方法
        MyInterface.staticMethod();

        System.out.println("抽象方法-MyImplements1-getName");
    }
}
```
MyImplements1 重写了默认方法。所以当我们调用 MyImplements1 示例对象的 `defaultMethod()` 时，会调用到重写的这个方法。

- 测试
```java
public class InterfaceTest {
    public static void main(String[] args) {
        MyInterface implements1 = new MyImplements1();
        MyInterface implements2 = new MyImplements2();

        implements1.abstractMethod();
        implements1.defaultMethod();
        System.out.println("===========================");
        implements2.abstractMethod();
        implements2.defaultMethod();
    }
}
```

- 运行结果
```
静态方法-MyInterface-staticMethod
抽象方法-MyImplements1-getName
重写默认方法-MyImplements1-defaultMethod
===========================
静态方法-MyInterface-staticMethod
抽象方法-MyImplements1-getName
私有方法-MyInterface-privateMethod
静态私有方法-MyInterface-staticPrivateMethod
静态方法-MyInterface-staticMethod
默认方法-MyInterface-defaultMethod
```
