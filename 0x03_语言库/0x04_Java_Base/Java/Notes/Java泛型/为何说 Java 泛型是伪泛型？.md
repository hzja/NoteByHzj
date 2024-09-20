Java泛型<br />在 JDK1.5 之前，Java 还没有出现泛型时，只能通过 Object 来实现类似功能。比如在 1.5 版本之前，HashMap 的 `get()` 方法返回一个 Object 类型的对象。由于 Object 类是一切类的父类，那 Object 转为任何对象都是被允许的。但因为 Object 类在向下转型时有无限的可能性，编译器并不知道这个 Object 的转型是否成功，这是一个潜在的稳定性风险。<br />Java 有一个兄弟语言 C#，无论语法还是生态都和 Java 非常相似。同样地，C# 中也有泛型，而且语法和 Java 相同。但这两者在泛型的具体实现上完全不一样，C# 中的泛型是真实存在的，例如，List与 List就是两个不同的类型，它们在系统运行期生成，这种实现称为类型膨胀，基于这种方法实现的泛型称为真泛型。此时，如果将 List和 List的尖括号去掉变成 ListInteger 和 ListString，那么 ListInteger 和 ListString 就是 2 个独立的类。<br />与 C# 的真泛型对应，Java 中的泛型称为伪泛型，其实就是一个语法糖，起到了让代码更简洁易读的作用。它只在源码中存在，在编译后的字节码中不存在。虚拟机层面就像完全没实现泛型一样，因为从虚拟机视角来说，List与 List都是 List，没有区别。<br />无论哪种实现原理，使用泛型编写的程序具有更好的安全性和可读性。泛型对于集合类来说尤其重要。<br />接下来就来讲一讲 Java 泛型的原理以及泛型的 PECS 法则。
<a name="JzIZA"></a>
### 类型擦除：为何说 Java 泛型是伪泛型
在上面已经说到，Java 的泛型是伪泛型。在对 Java 源文件编译后，字节码中没有任何泛型信息，所有的泛型信息都会被“擦除”掉，这个过程就叫做类型擦除。<br />比如 List 和 List类型，在编译后都是 List。JVM 看到的只是 List，因为泛型对 JVM 来说是不可见的。编译器在编译时会去找到可能出错的地方，但是无法保证 100% 运行时一定不会发生类型转换异常。<br />类型擦除也是 Java 的泛型实现与 C++ 的泛型实现之间的重要区别。<br />通过对下面代码运行结果的分析，来尝试证明一下类型擦除机制：
```java
ArrayList<String> l1=Lists.newArrayList("a");
ArrayList<Integer> l2=Lists.newArrayList(1);
System.out.println(l1.getClass()==l2.getClass());
```
在上述代 类型，另一个是 ArrayList泛型类型。通过l1对象和l2对象的 getClass 方法，可以获取这两个对象所属的类的信息，对得到的这两个信息进行是否相等的判断，最后发现输出的结果为 true。说明泛型 String 和 Integer 都被擦除了，只剩下了原始类型。<br />换个更复杂的例子来验证一下：
```java
ArrayList<Integer> l3=Lists.newArrayList(1);
l3.getClass().getMethod("add", Object.class).invoke(l3, "a");
```
定义了一个 ArrayList类型，然后调用 add 方法，但这时，ArrayList类型只能存储 Integer 的数据。但是当利用反射调用 add 方法的时候，ArrayList类型却可以存储 String。这说明了 Integer 在编译之后被擦除了，只保留了原始类型。<br />在很多类似 diamond 的配置平台上都存在这样的问题，比如在代码中声明了 List，而在 diamond 配置的时候写的是数字，将被默认转换成 Integer，最终导致程序异常。<br />类型擦除以后，ArrayList 后面的泛型变量默认使用 Object，这也就倒退到 JDK1.5 以前没有泛型的实现方式。所以说泛型的实现都是假的，JVM 层什么也没有改变。<br />当然，类型擦除之后泛型变量并不一定是 Object，比如下面的代码的格式中，泛型变量使用的是 Serializable：
```java
public class Test<T extends Serializable>
```
需要注意的是，在调用泛型方法时，可以声明泛型类型，也可以不声明。在不声明的情况下，泛型类型为该方法中的几个类型的最小公倍数。可能第一次听过类还有最小公倍数的概念，其实这是为了方便大家理解，比如 Integer 类和 Float 的最小公倍数就是 Number。比如有下面的方法：
```java
public <T> T add(T x,T y){
     return y;
}
```
看看传入不同的参数会发生什么：
```java
Integer i=add(1, 2)); 
Number n=add(1, 1.2);
```
首先传入相同的类型的变量 1 和 2，因为都是 Integer 类型，所以最小公倍数也是 Integer 类型，而后面传入变量 1 和 1.2，一个是 Integer 类型、另一个是 Float 类型，所以最小公倍数是 Number。<br />类型擦除欺骗了 IDEA，IDEA 在编写代码的时候就会给出类型转换提示。那么编译器或者 IDE 是如何进行类型检查的呢？答案是根据类的声明，比如对比下面的两种写法：
```java
ArrayList<String> l1=new ArrayList(); 
ArrayList l2=new ArrayList<String>();
```
对于上述代码中的变量 l1 和 l2，哪种类型限制会成功呢？答案是 l1。因为，类型检查是编译时完成的，编译完成后类型也就擦除了。而 `new ArrayList()` 是在内存中开辟一个空间，这是运行时发生的事情了。类型检查的是它的类声明，这是当代码写完以后就可以静态分析的内容。<br />看到这里是否好奇，虽然 Java 实现泛型的时候类型被擦除了，但在获取对象的时候，好像并不需要进行强制类型转换。<br />比如 ArrayList 可以直接进行 get 操作，而不需要显示地去转换，那 ArrayList 中的 get 是如何做的呢？看下 ArrayList 的 get 方法源码：
```java
public E get(int index) {
	RangeCheck(index);
	return (E) elementData[index];
}
```
能从代码中看到，return 语句原来已经转化好了，框架已经做好了转换。
<a name="jsKQi"></a>
### PECS 原则
讲完了类型擦除，应该对泛型的实现原理有所了解了，下面聊聊更实用的 PECS 原则，PECS 全称 Producer Extends, Consumer Super，先不解释，借助个例子来理解一下：
```java
List<? extends Object> list = new ArrayList<>();
list.add(1);      //错误
```
上述代码声明了一个 list，这个 list 里面放的是 Object 的子类，但是可以发现 Integer 无法加入 list。因为编译器只知道 list 是 Object 的某个子类的 list，但并不确定究竟是哪个子类，稳妥起见，这个时候任何类型都无法加入。好比一个大楼的保安，领导说咱们只允许技术大牛进大楼，但是领导没有告知“技术大牛”的定义，是 Java 大牛还是 Python 大牛？到底怎么样算牛？保安为了保住自己的职位，稳妥起见，一个人也没让进。<br />但是，由于编译器知道 list 里面存放的总是 Object 的子类型，因此总可以从中读取出 Object 对象，就像下面这样的代码是被允许的：
```java
list.get(0);
```
再看看下面的例子：
```java
List<? super Integer> list = new ArrayList<>();
list.add(1);      //成功
list.add(1.2);      //失败
```
声明了一个 list，这个 list 里面放的是 Integer 的父类型，这里同样有很多情况，稳妥起见，只能把它当成 Integer 来看。这种情况放入一个 Integer 当然没问题，虽然 Number 类型的 1.2 也满足 super Integer 这个条件，但是不能保证一定是这样的，除了 Number 别的类型也可以。所以只能放入 Integer 和它的子类型。<br />从上面的例子可以看出 PECS 原则如下：<br />从集合中读取类型 T 的数据，且不能写入，用 `extends(Producer Extends)`；<br />从集合中写入类型 T 的数据，且不需要读取，用 `super(Consumer Super)`。<br />这个法则称做“读 E 写 S”法则，更便于记忆和理解。
<a name="prxNU"></a>
### 总结
复习并深度详解了 Java 基础语法中的泛型，PECS 在一些开源项目源码中多次出现，在代码中也可以使用 PECS 来更优雅的约束方法。如果有很多只需要的读的场景或只需要写的场景，那么 PECS 就可以发挥它的优势了。此外知道了 Java 泛型的类型擦除是在编译器层面实现的，对JVM来说并没有感知，其实 Java 很多语法糖也是编译器层面实现的，甚至一些基于 JVM 的编程语言如 Groovy、Scala 也是在编译器层面玩的花样，而没有改造 JVM 的指令
