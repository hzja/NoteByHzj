《Effective Java》
<a name="sYt0U"></a>
### 1、考虑用静态工厂方法替代构造函数
<a name="79jRk"></a>
##### 例子：
```java
Integer.valueOf("1")、Boolean.valueOf("true");
```
等。
<a name="Trv8A"></a>
##### 优势：

- 可读性高（方法名）
- 性能（不一定创建对象）
- 灵活性高

下面针对三个优势进行一些解读。
<a name="ModPn"></a>
#### **可读性高**
new Point(x,y)和Point.at(x,y)、Point.origin()。构造函数只能看出两个参数，不知其意，后者更易理解。
<a name="8N7Ii"></a>
#### **性能**
在某些情况下，可以事先进行实例化一些对象，调用时直接调用即可，不需要进行改变。比如，Boolean。
```java
public final class Boolean implements Serializable, Comparable<Boolean> {
    // 预先设置两个对象
    public static final Boolean TRUE = new Boolean(true);
    public static final Boolean FALSE = new Boolean(false);
    public Boolean(boolean var1) {
        this.value = var1;
    }
    public Boolean(String var1) {
        this(parseBoolean(var1));
    }
    // 工厂方法
    public static Boolean valueOf(boolean var0) {
        return var0?TRUE:FALSE; // 返回预先设置的对象，而不是创建对象
    }
    // 工厂方法
    public static Boolean valueOf(String var0) {
        return parseBoolean(var0)?TRUE:FALSE;
    }
    // ... other code
}
```
<a name="lBYjC"></a>
#### **灵活性高**
可根据具体情况，返回子类。相当于更强大的工厂。直接从父类获取到子类。尤其适用于工具类（提供各种API）。例子：Collections。
```java
public class Collections {
    // 私有，典型工厂
    private Collections() {
    }
    public static final List EMPTY_LIST = new EmptyList<>();
    // 工厂方法
    public static final <T> List<T> emptyList() {
        return (List<T>) EMPTY_LIST;
    }
    private static class EmptyList<E> extends AbstractList<E> implements RandomAccess, Serializable {
    // code
    }
    // 工厂方法
    public static <E> List<E> checkedList(List<E> list, Class<E> type) {
    // 根据具体情况，获取相应子类
        return (list instanceof RandomAccess ?
                new CheckedRandomAccessList<>(list, type) :
                new CheckedList<>(list, type));
    }
    // 子类1
    static class CheckedRandomAccessList<E> extends CheckedList<E> implements RandomAccess {
        CheckedRandomAccessList(List<E> list, Class<E> type) {
            super(list, type);
        }
        public List<E> subList(int fromIndex, int toIndex) {
            return new CheckedRandomAccessList<>(
                    list.subList(fromIndex, toIndex), type);
        }
    }
    // 子类2
    static class CheckedList<E> extends CheckedCollection<E> implements List<E> {
    // code
    }
}
```
<a name="276e4b74"></a>
### 2、多个构造函数时，考虑使用构造器
尤其在进行Android开发时，会碰到这种情况。通常是一个对象，具有多个成员变量可能需要初始化，常规方法，需要提供大量构造函数。例如：
```java
// 非Android中的AlertDialog，便于说明问题，举个例子
public class AlertDialog {
    private int width;
    private int height;
    private String title;
    private String confirmText;
    private String denyText;
    private AlertDialog(){}
    public AlertDialog(int width, int height){ // 空白的警告框
         AlertDialog(width,height,null);
    }
    // 带标题的警告框
    public AlertDialog(int width, int height, String title){ // 带标题的警告框
        AlertDialog(width, height, title, "确定");
    }
    // 带标题的警告框，有确定按钮
    public AlertDialog(int width, int height, String title, String confirm){
        AlertDialog(width, height, title, confirm, null);
    }
    // 带标题的警告框，有确定按钮，取消按钮
    public AlertDialog(int width, int height, String title, String confirm, String denyText){
        // set every thing.
    }
}
```
有多种样式的警告框，为了调用方便，必须提供多个构造函数。否则用户在调用时，只能使用完整构造函数，容易犯错且无法进行阅读。极不灵活。如果采用另外一种方式，则可以解决，但会花费很多经历处理并发的情况:
```java
// 非Android中的AlertDialog，便于说明问题，举个例子
public class AlertDialog {
    private int width;
    private int height;
    private String title;
    private String confirmText;
    private String denyText;
    public AlertDialog(){}// 空白的构造函数
   
    public void setWidth(int width){
        this.width = width;
    }
    // 其他set方法
}
```
调用时，通过调用各个参数的set方法进行设置。问题来了：

- 并发
- 无法进行参数校验。

例如，只创建了对象，设置了标题，却没有尺寸，相当于创建了一个没有尺寸的警告框。在Android中，大量的控件都使用了构造器Builder。
```java
// 非Android中的AlertDialog，便于说明问题，举个例子
public class AlertDialog {
    private int width;
    private int height;
    private String title;
    private String confirmText;
    private String denyText;
    // private
    private AlertDialog(){}
    // Builder中使用
    protected AlertDialog(Builder b){
        width = b.width;
        height = b.height;
        // .....
        if(width==0||height==0) throws new Exception("size must be set");
    }
    // 构造器
    public static class Builder {
        private int width;
        private int height;
        private String title;
        private String confirmText;
        private String denyText;
        // 注意：返回的Builder。
        public Builder setTitle(String title) {
            this.title = title;
            return this;
        }
        // 其他set...
        
        public AlertDialog build(){
            return AlertDialog(this);
        }
    }
}
```
于是，可以根据相应需求，进行相应设置，并在AlertDialog真正构造时，进行参数校验。就像这样：<br />new AlertDialog.Builder().setTitle("提示").build();<br />上述例子，会成功抛出异常。
<a name="hhkua"></a>
### 3、用私有化构造器或者枚举型强化Singleton。
Singleton指最多会被实例化一次的类。通常情况下，以前的做法是没有问题的。但是在某些高级情况，通过使用反射的相关知识访问private的构造函数，破坏Singleton。
```java
public class Elvis{
    // 注意，公有final对象
    public static final Elvis INSTANCE = new Elvis();
    private Elvis(){}
}
```
另一种情况，在序列化的过程中，反序列化得到的对象已经不再是以前的对象（破坏了Singleton），这种情况下，可以通过单元素枚举型处理。
```java
public enum Elvis{
    INSTANCE;
    // some methods
}
```

<a name="gzaxC"></a>
### 4、通过私有化构造器强化不可实例化的能力
有一些工具类，仅仅是提供一些能力，自己本身不具备任何属性，所以，不适合提供构造函数。然而，缺失构造函数编译器会自动添加上一个无参的构造器。所以，需要提供一个私有化的构造函数。为了防止在类内部误用，再加上一个保护措施和注释。
```java
public class Util{
    private Util(){
        // 抛出异常，防止内部误调用
        throw new AssertionError();
    }
}
```
弊端是无法对该类进行继承（子类会调用super()）。
<a name="ton9t"></a>
### 5、避免创建不必要的对象

- 对象的重用
- 昂贵的对象，使用对象池
- 廉价的对象，慎用对象池。

现代JVM对廉价对象的创建和销毁非常快，此时不适于使用对象池。
<a name="5f323b2b"></a>
### 6、消除过期的对象引用
以下三种情况可能会造成内存泄露：

- 自己管理的内存（数组长度减小后，pop出的对象容易导致内存泄漏）
- 缓存
- 监听和回调
<a name="fP4ka"></a>
#### 自己管理的内存
对于自己管理的内存要小心，比如：
```java
public class Stack{
    private Object[] elements;
    private int size = 0;
    private static final int DEFAULT_INITIAL_CAPACITY = 16;
    
    public Stack(){
         elements = new Object[DEFAULT_INITIAL_CAPACITY];
    }
    public void push(Object e){
        ensureCapacity();
        elements[size++]=e; // allocate新的堆内存和栈内存
    }
    public Object pop(){
        if(size==0) throw new EmptyStackException();
        return element[--size]; // pop出element[size]，该对象不再有效。内存泄漏原因。
    }
    
    private void ensureCapacity(){
        if(elements.length==size)
            elements = Arrays.copyOf(elements, 2*size+1);
    }
}
```
弹出的对象不再有效，但JVM不知道，所以会一直保持该对象，造成内存泄露。解决：
```java
public Object pop(){
    if(size==0) throw new EmptyStackException();
    elements[size] = null; // 等待回收
    return element[--size];
}
```
<a name="45wWi"></a>
#### **缓存**
缓存的对象容易被程序员遗忘，需要设置机制来维护缓存，例如不定期回收不再使用的缓存（使用定时器）。某些情况下，使用WeakHashMap可以达到缓存回收的功效。注，只有缓存依赖于外部环境，而不是依赖于值时，WeakHashMap才有效。
<a name="jJ8YC"></a>
#### **监听或回调**
使用监听和回调要记住取消注册。确保回收的最好的实现是使用弱引用（weak reference），例如，只将他们保存成WeakHashMap的键。
<a name="a5M7V"></a>
### 7、避免显示调用GC
Java的GC有强大的回收机制，可以简单的记住：不要显示调用finalizer。可以这样理解：jvm是针对具体的硬件设计的，然而程序却不是针对具体硬件设计的，所以，java代码无法很好的解决gc问题（因为他具有平台差异化）。另外，finalizer的性能开销也非常大，从这个角度上考虑也不应该使用它。
<a name="a6N1d"></a>
### 8、覆盖equals方法请遵守通用约定

- 自反性。   `x.equals(x) == true`
- 对称性。  当前仅当`y.equals(x)==true`时，`x.equals(y)==true`
- 传递性。   `if(x.equals(y)&&y.equals(z))`，`y.equals(z)==true`
- 一致性。
- 非空性。  `x.equals(null)==false`
<a name="fQEVy"></a>
### 9、覆盖equals方法时总要覆盖hashCode
为了保证基于散列的集合使用该类（HashMap、HashSet、HashTable），同时，也是Object.hashCode的通用约定，覆盖equals方法时，必须覆盖hashCode。
<a name="R0jad"></a>
### 10、始终覆盖toString
Object的toString方法的通用约定是该对象的描述。注意覆盖时，如果有格式，请备注或者严格按照格式返回。
<a name="de5abfa7"></a>
### 11、谨慎覆盖clone
<a name="1AV9b"></a>
### 12、考虑实现`Comparable`接口
<a name="3d51a9b6"></a>
### 13、使类和成员的可访问性最小化
目的是解耦。简单来讲，使用修饰符的优先级从大到小，private>protected>default(缺省)>public。如果在设计之初，设计为private修饰符后，在之后的编码过程如果不得不扩大其作用于，应该先检查是否设计的确如此。子类覆盖超类，不允许访问级别低于超类的访问级别。（超类的protected，子类覆盖后不能改为default）。成员变量决不允许是公有的。一旦设置为公有，则放弃了对他处理的能力。这种类并不是线程安全的。即使是final的，也不允许。除非希望通过public static final来暴露常量。成员变量总是需要使用setter和getter来维护。有一个例外：长度非零的数组。这是安全漏洞的一个根源。
```java
public Object pop(){
    if(size==0) throw new EmptyStackException();
    elements[size] = null; // 等待回收
    return element[--size];
}
```
改进：
```java
private static final Thing[] PRIVATE_VALUES = {...}
// 此时获取到的才是“常量”
public static final List<Thing> VALUS =
    Collections.unmodifiableList(Arrays.asList(PRIVATE_VALUES))
```
另一种：
```java
private static final Thing[] PRIVATE_VALUES = {...}
// 此时获取到的才是“常量”
public static final Thing[] values(){
    return PRIVATE_VALUES.clone();
}
```
<a name="yyGGb"></a>
### 14、在公有类中使用访问方法而非公有成员变量（类似13）
<a name="b17c511e"></a>
### 15、使可变性最小化
<a name="c5a5227d"></a>
### 16、复合优先于继承
继承有利于代码复用，但是尽可能不要进行跨包的继承。包内的继承是优秀的设计方式，一个包里的文件处在同一个程序员的控制之下。但是继承有其局限性：子类依赖于超类。超类一旦发生更改，将可能破坏子类。并且，如果超类是有缺陷的，子类也会得“遗传病”。复合，即不扩展已有的类，而是在的类中新增一个现有类的。相当于现有类作为一个组建存在于新类中。如此，将只会用到需要用到的东西，而不表现现有类所有的方法和成员变量。新类也可以称为“包装类”，也就是设计模式中的Decorate模式。
<a name="603035b0"></a>
### 17、要么就为继承而设计，并提供文档说明，要么就禁止继承
<a name="4fde58fa"></a>
### 18、接口优于抽象类
<a name="YlYZi"></a>
### 19、接口只用于定义类型
<a name="L8Jfz"></a>
### 20、类层次优先于标签类
<a name="b58b6314"></a>
### 21、用函数对象表示策略
函数参数可以传入类似listener的对象，目的是使用listener中的方法。如果使用匿名的参数，每一次调用会创建新的对象。可以将listener声明为成员变量，每次都复用同一个对象，并且可以使用静态域（static变量）。比如String类的`CASE_INSENSITIVE_ORDER`域。
<a name="e1YvA"></a>
### 22、优先考虑静态类成员
嵌套类的目的应该只是为了他的外围类提供服务，如果以后还可能用于其他环境中，则应该设计为顶层类。静态类相当于一个普通的外部类，只是恰好声明在了一个类内部。通常的用户是：`Calculator.Operation.PLUS`等。和普通类的区别只是，在PLUS前，有了2个前缀，来表明其含义。而非静态类必须存在于外部类对象中。不要手动在外部创建一个内部非静态类对象，创建的过程是：`instance.New MemberClass()`。这非常奇怪。如果成员类不需要访问外围类，则需要添加static，是他成为静态成员类，否则每个实例都将包含一个额外指向外围对象的引用。将会影响垃圾回收机制。
<a name="cKlPu"></a>
### 23、应指定泛型的具体类型，而不是直接使用原生类型。
例如，应该指定List<E>，而不建议直接使用List。
<a name="cxv7t"></a>
### 24、消除非首检警告
在使用IDE进行编码时，强大的IDE都会在编码过程中提示warning，需要尽可能的消除warning，至少，应该小心这些warning。慎用SuppresWarning，如果IDE提示可以通过添加该注解解决掉warning，请不要那么做。如果实在要使用，请添加注释说明原因。
<a name="qQtOu"></a>
### 25、列表优先于数组
类比泛型，数组是有一定缺陷的。`List<SuperClass>`和`List<SubClass>`是没有关系的，而`Sub[]`是`Super[]`的子类。
```java
// Fails at runtime
Object[] objectArray = new Long[1];
objectArray[0] = "I don't fit in"; // throw exception
// won't compile
List<Object> ol = new ArrayList<Long>(); // Incompatible types
ol.add("I don't fit in");
```
从代码中可以看到，使用泛型，会提前发现错误。
<a name="a60607fa"></a>
### 26、优先考虑泛型
<a name="7ed47de1"></a>
### 27、优先考虑泛型方法
<a name="151b5620"></a>
### 28、利用有限制通配符来提升API的灵活性
PECS，producer-extends，consumer-super。
```java
//public class Stack<E>{
// public Stack();
// public void push(E e);
// public E pop();
// public boolean isEmpty();
//}
public void pushAll(Iterator<? extends E> src){
    for(E e : src)
        push(e);
}
public void popAll(Collection<? super E> dst){
    while(!isEmpty()){
        dst.add(pop());
    }
}
// Get and Put Principle
```
所有comparable和comparator都是消费者（Consumer）。
<a name="OzhXz"></a>
### 29、优先考虑类型安全的异构容器
<a name="87fdeeb6"></a>
### 30、用enum代替int常量
```java
public enum Apple { FUJI, PIPPIN, GRANNY_SMITH }
public enum Orange { NAVEL, TEMPLE, BLOOD }
```
枚举型在java中非常强大，当需要一组固定常量时，使用enum比int好很多。比如代码可读性，安全性等。
<a name="6fc44365"></a>
### 31、enum用实例域代替序数
```java
// bad solution
public enum Ensemble {
    SOLO, DUET, TRIO, QUARTET, QUINTET,
    SEXTET, SEPTET, OCTET, NONET, DECTET;
    public int numberOfMusicians() { return ordinal() + 1; }
}
//
// improvement
public enum Ensemble {
    SOLO(1), DUET(2), TRIO(3), QUARTET(4), QUINTET(5),
    SEXTET(6), SEPTET(7), OCTET(8), NONET(9), DECTET(10), TRIPLE_QUARTET(12);
    private final int numberOfMusicians;
    Ensemble(int size) { this.numberOfMusicians = size; }
    public int numberOfMusicians() { return numberOfMusicians; }
}
```
永远不要像第一种的方式，利用序数访问enum，需要在构造函数中使用参数来初始化。
<a name="333cbdd2"></a>
### 32、用EnumSet代替位域
```java
public class Text{
    public static final int STYLE_BOLD = 1 << 0; // 1
    public static final int STYLE_ITALIC = 1 << 1; // 2
    public static final int STYLE_UNDERLINE = 1 << 2; // 4
    public static final int STYLE_STRIKETHROUGH = 1 << 3; // 8
    public void applyStyles(int styles){
        // ...
    }
}
//
text.applyStyles(STYLE_BOLD | STYLE_ITALIC);
```
以上叫做位图法，但是有更好的方案来传递多组常量——EnumSet。
```java
public class Text{
    public enum Style { BOLD, ITALIC, UNDERLINE, STRIKETHROUGH }
    // 注意此处，使用的是Set而不是EnumSet
    public void applyStyles(Set<Style> styles){
        // ...
    }
}
//
text.applyStyles(EnumSet.of(Style.BOLD, Style.ITALIC));
```
<a name="vGSw7"></a>
### 33、用EnumMap代替序数索引
任何时候都不要使用enum的ordinal()方法。
<a name="Qb1O5"></a>
### 34、用接口模拟可伸缩的枚举
<a name="57a3970e"></a>
### 35、注解优先于命名模式
<a name="eqUL8"></a>
### 36、坚持使用Override注解
<a name="5ff12d61"></a>
### 38、检查参数的有效性
公有方法检查参数，参数异常需要跑出Exception。私有方法利用断言assertion检查参数。
<a name="89d3bc00"></a>
### 39、必要时进行保护性拷贝
假设类的客户端会尽其所能来破坏这个类的约束条件，因此必须保护性的设计程序。以下是一个不可变类的设计。
```java
public Period(Date start, Date end){
    this.start = new Date(start); // 使用了值的拷贝，没有使用原对象（指针）
    this.end = new Date(end);
    if(this.start.compareTo(this.end)>0)
        throw new IllegalArgumentException(start + " after " + end)
}
```
注意：保护性拷贝是在检查参数之前进行的，防止多线程的影响。不要使用clone方法进行保护性拷贝。以上方法防御了传入参数的修改，但是对于get方法获取到的对象，仍然可以被修改，通过以下方法可以防止这种攻击。
```java
public Date start(){
    return new Date(start);
}
public Date end(){
    return new Date(end);
}
```
<a name="51eNw"></a>
### 40、谨慎设计方法签名
<a name="d032bba4"></a>
### 41、慎用重载
<a name="dc06d56b"></a>
### 42、慎用可变参数
<a name="148908c8"></a>
### 43、返回0长度的数组或者集合，而不是null
null一般用于表示没有被初始化或处理，如果方法返回了null，则需要在上层做更多的处理，以防止NPE。
<a name="f3c36db1"></a>
### 44、为所有导出的API元素编写文档注释
正确的javadoc文档，需要每个被导出的类、接口、构造器、方法和域之前增加文档注释。注释应该是对实现透明的，只需要简洁的描述它和客户端之间的约定。并且，还应该附上该方法的副作用。
<a name="c3382454"></a>
### 45、将局部变量的作用域最小化
<a name="8e72107a"></a>
### 46、for-each优先于for循环
for-each规避掉了for循环的index变量的引用，通常来说它是不必要的——会增加引入错误的风险，并且风险一旦发生，很难被发现。不过有三种情况下，无法使用for-each（注：在jdk1.8中已经很好的解决了这些问题）。

- 过滤
- 转换
- 平行迭代
<a name="YVFxO"></a>
### 48、如果需要精确的答案，请避免使用float和double
float和double是执行的二进制浮点运算，目的是在广泛数值范围上使用精确的快速近似计算而设计的。然而他们并没有提供完全精确的计算（实际应用中，经常会碰到出现x.99999等结果）。尤其是，在进行货币计算时，他们并不适用。比如：<br />System.out.println(1.03-.42);<br />得到的结果将是：0.610000000001。为了解决这个问题，需要使用BigDecimal。然而这也有一些问题，相对于普通的运算，它显得更加麻烦，而且也更慢。通常来说后一个缺点可以忽略，但是前者可能会让人很不舒服。有一种做法是将需要处理的数值*10(或更多），使用int进行计算，不过需要自己处理四舍五入等操作。
<a name="4JUvX"></a>
### 49、基本类型优先于装箱基本类型
基本类型只有值，装箱类具有与他们值不同的同一性。<br />基本类型只有功能完备的值，装箱类还具有非功能值：<br />null。<br />所以可能会碰到NPE<br />基本类型省空间省时间
<a name="ZJ7zq"></a>
### 50、如果有更精确的类型，请避免使用字符串,字符串不适合代替其他值的类型。
例如：int，boolean等

- 不适合代替枚举类型（第30条）
- 不适合聚集类型
<a name="3KeP7"></a>
### 51、当心字符串连接的性能
操作符“+”可以将多个字符串进行连接。但是在大规模使用“+”的情况下，连接n个字符串的开销是n的平房级时间。这是由于字符串的不可变性导致的。在这种情况下请使用StringBuilder进行连接。
<a name="057dcd09"></a>
### 52、通过接口引用对象
<a name="7efb08ad"></a>
### 53、接口优先于反射机制
使用反射机制会带来以下的问题：

- 丧失了编译期类型检查
- 代码笨拙冗长
- 性能损失

反射基本上只适合用在编写组件时、代码分析器、RPC等场景下使用。在使用反射机制时，如果可能，尽可能只通过反射机制实例化对象，而访问方法时，使用已知的接口或者超类。
<a name="aGA3X"></a>
### 54、谨慎使用JNI
<a name="FDFh8"></a>
### 55、谨慎进行优化
> _很多计算上的过失都被归咎于效率（没有必要达到的效率），而不是任何其他原因——甚至包括盲目的做傻事。                _——William A. Wulf_不要去计较效率上的一些小小的得失，在97%的情况下，不成熟的优化才是一切问题的根源。_——Donald E. Knuth_在优化方面，我们应该遵守两条规则：_
> _规则1:不要进行优化。_
> _规则2（仅针对专家）：还是不要进行优化——也就是说，在还没有绝对清晰的优化方案前，请不要进行优化。_——M. A. Jackson

这些格言比java的出现还要早20年。他们讲述了一个关于优化的深刻事实：优化的弊大于利。<br />要努力编写好的程序，而不是快的程序。低耦合的重要性远远大于性能。当程序编写得足够低耦合后，通过工具发现了性能瓶颈的代码块，才可以保证对其的修改不影响任何外部环境。
<a name="3bd00e08"></a>
### 56、遵守普遍的命名规则
<a name="d6d6ce74"></a>
### 57、只针对异常情况才使用异常
不要尝试通过异常机制来做正常代码应该做的事情，比如，检查数组下标。jvm很少对异常进行优化，因为它只用于不正常的情况。并且，如果将代码放入try-catch代码块，jvm就丧失了本来可以对它进行的优化。
<a name="cb43f1fc"></a>
### 58、对于可恢复的情况使用受检异常，对于编程错误的情况使用运行时异常
如果期望调用者适当的恢复，则需要使用受检异常，强迫调用者食用try-catch代码块，或者将他们抛出去<br />当调用发生前提违例——违反约定的情况时，使用运行时异常，这个时候程序已经无法再执行下去了。<br />例如调用数组的-1索引。
