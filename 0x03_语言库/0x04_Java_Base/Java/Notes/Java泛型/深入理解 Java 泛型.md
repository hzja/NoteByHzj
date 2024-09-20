Java 泛型
<a name="XBBBZ"></a>
## 泛型的产生
泛型最开始是在 C++ 中提出的，实现为模块方法和模板类，主要为了解决与类型相关的算法的重用问题，比如对栈的描述：
```java
class stack {
     push(参数类型)  //入栈算法

     pop(参数类型)   //出栈算法
}
```
如果把上面的伪代码看作算法描述，没问题，因为算法与参数类型无关。但是如果把它写成可编译的源代码就必须指明是什么类型，否则是无法通过编译的，使用重载来解决这个问题，就要对N种不同的参数类型写 N 个<br />push 和 pop 算法，这样是很麻烦的，代码也无法通用。<br />若对上面的描述进行改造如下：首先指定一种通用类型 T ，不具体指明是哪一种类型。
```java
class stack<参数模板 T> {
     push(T)  //入栈算法

     pop(T)   //出栈算法
}
```
这里的参数模板T相当于一个占位符，当实例化类 stack 时， T 会被具体的数据类型替换掉。若定义对象 s 为 int 类型，在实例化s时若将 T 指定 int 型则：<br />这时候类s就成为：
```java
class s {
     push(int)  //入栈算法

     pop(int)   //出栈算法
}
```
这时可以称`class stack<参数模板 T>`是类的类，通过它可以生成具体参数类型不同的类<br />可以看出泛型是通过对算法中变化的类型的抽象，其它地方的代码相同，唯有类型是可变的，有了泛型后对于编码效率和代码通用有很大的好处。
<a name="rQMSL"></a>
## Java中的泛型
Java 中泛型的实现与 C++ 中不同，Java 中一个泛型类只会产生一份目标代码，而 C++ 则会根据具体的参数实例产生多份目标代码，是什么意思呢？举个例子 对于一个 `List<T>` 泛型类如果代码中有对应的 `List<String>` , `List<Integer>` 实例 Java 只会产生一个 List.class ，而 C++ 中就会产生用 String 、Integer 替换参数 T 的两份代码<br />这两种方式对应着编译器处理泛型的两种策略:

- Code specialization 在实例化一个泛型类或泛型方法时都产生一份新的目标代码（字节码or二进制代码）
- Code sharing 对每个泛型类只生成唯一的一份目标代码；该泛型类的所有实例都映射到这份目标代码上，在需要的时候执行类型检查和类型转换。
<a name="LKgZA"></a>
## Java的伪泛型，泛型擦除
由于向上兼容历史代码的原因 Java 采用了 Code sharing 的策略，使得泛型只存在于源码阶段，编译过后的 Class 文件并不存在泛型，虚拟机并不知道泛型的存在，所以说 Java 中的泛型是一种伪泛型，这种参数类型只存在于源码阶段在编译后并不存在的机制叫做泛型擦除，举个例子：
```java
public static void main(String[] args) {
      Class c1 = new ArrayList<String>().getClass();
      Class c2 = new ArrayList<Integer>().getClass();
      System.out.println(c1 == c2);
}
```
上面的代码有两个不同的 ArrayList : `ArrayList<Integer>` 和 `ArrayList<String>`。它们的参数化类型不同，一个保存整性，一个保存字符串。但是通过比较它们的 Class 对象，上面的代码输出是 true。这说明在 JVM 看来它们是同一个类。而在 C++、C# 这些支持真泛型的语言中，它们就是不同的类。
<a name="H6rUz"></a>
## Java编译器的类型转换和类型检查
<a name="gST5T"></a>
### 类型自动转换
既然上面所说Java会在编译时对泛型进行擦除，那么它要实现同一份泛型代码不同类型通用必然需要类型转换，事实上编译器也是这么干的，比如下面的这一段代码：
```java
public class Wrapper<T> {
    private T field;

    public void setField(T field) {
        this.field = field;
    }

    public T getField() {
        return field;
    }
}     
public void test() throws Exception {
    Wrapper<Integer> wrapper1 = new Wrapper<>();
    Wrapper<String> wrapper2 = new Wrapper<>();
    wrapper1.setField(1);
    int field1 = wrapper1.getField();
    wrapper2.setField("hello generics");
    String field2 = wrapper2.getField();
}
```
编译后：
```java
public void test() throws Exception {
    Wrapper wrapper1 = new Wrapper();
    Wrapper wrapper2 = new Wrapper();
    wrapper1.setField(Integer.valueOf(1));
    int field1 = ((Integer)wrapper1.getField()).intValue();
    wrapper2.setField("hello generics");
    String field2 = (String)wrapper2.getField();
}
```
可以看到编译后的代码增加了类型转换，编译器会自动添加类型转换的代码
<a name="FmzFg"></a>
### 类型检查
由于类型擦除可能导致的一些异常问题，编译器需要做类型检查来尽量确保程序在运行时不会抛出异常，在写泛型相关代码时，将泛型擦除考虑进去后再想这段代码在运行时会不会有异常，然后再看编译器报的错误也就理解了，比如：<br />泛型的方法签名
```java
private void fun(Wrapper<Integer> wrapper){

}

private void fun(Wrapper<String> wrapper){

}
```
编译器会报相同签名异常，因为他们的方法签名参数编译后都会被擦除掉
<a name="Tcy7S"></a>
#### 泛型的异常捕获
```java
public class Problem<T> extends Exception{......}
try{  

}catch(Problem<Integer> e1){  
    //...  
}catch(Problem<Number> e2){  
    //...  
}  
```
类型信息被擦除后，那么两个地方的 catch 都变为原始类型 Object，那么也就是说，这两个地方的 catch 变的一模一样，而对应 catch 的语句不一定相同，这就有冲突了。
<a name="zSSkg"></a>
#### 泛型的实例化
```java
public class Wrapper<T> {
    private final int SIZE = 10;
    private T field;

    public void setField(T field) {
        this.field = field;
    }

    public T getField() {
        return field;
    }

    private void create() {
        field = new T(); // Error
        if (field instanceof T) {
        } // Error
        T[] array = new T[SIZE]; // Error
        T[] array = (T) new Object[SIZE]; // Unchecked warning
    }
}
```
在上面的 create 中的几行代码都是编译器会报错误的，写这段代码本意上是为了实现泛型对象的创建比如：
```java
Wrapper<String> wrapper = new Wrapper();
wrapper.create();
```
对应：
```java
field = new String();
if(filed instanceof String){}
String[] array=new String[SIZE];
String[] array= String new Object[SIZE];
```
而由于泛型擦除 上面的参数 T 在编译后都会由它的第一个上界即 Object 代替，而编译器想通过去修改 Wrapper 类中的 create 方法达到输出本意代码是不可能的，因为泛型只有在运行时才知道具体的类型。<br />这里需要提一点的是编译器进行类型检查时是以声明的类型为基础依赖去检查的而不是具体创建的类型，什么意思呢？看下面这段比较常见的代码：
```java
ArrayList<String> arrayList1=new ArrayList();  
arrayList1.add("1");//编译通过  
arrayList1.add(1);//编译错误  
String str1=arrayList1.get(0);//返回类型就是String  

ArrayList arrayList2=new ArrayList<String>();  
arrayList2.add("1");//编译通过  
arrayList2.add(1);//编译通过  
Object object=arrayList2.get(0);//返回类型就是Object 
```
通过上面的例子，可以明白，类型检查就是针对声明的，变量声明是什么类型就用这个类型去调用泛型方法，就会对这个调用的方法进行类型检测，而无关它真正引用的对象。
<a name="paSaW"></a>
#### Java数组的“泛型化”
Java中数组相比于Java 类库中的容器类是比较特殊的，主要体现在三个方面：

- 数组创建后大小便固定，但效率更高
- 数组能追踪它内部保存的元素的具体类型，插入的元素类型会在编译期得到检查
- 数组可以持有原始类型 ( int，float等 )，不过有了自动装箱，容器类看上去也能持有原始类型了

由于Java中数组设计之初就是类型安全的，创建的时候必须知道内部元素的类型，而且一直都会记得这个类型信息。由于泛型不是一个具体的类型所以不能显式创建一个泛型数组如：
```java
Wrapper<String>[] wrapperArr1 = new Wrapper<String>[2]; //error
Wrapper<String>[] wrapperArr2 = new Wrapper[2];//Unchecked warning
```
而且Java 语言规范明确规定：数组内的元素必须是“物化”的。
> It is a compile-time error if the component type of the arra being initialized is not reifiable.

对“物化”的第一条定义就是不能是泛型：
>  A type is reifiable if and only if one of the following holds:<br />  - It refers to a non-generic class or interface type declaration.<br />  - ... ..

所以对于上面的代码第一行不可以，第二行可以，因为原生类 Wrapper 不是泛型
<a name="BA8rU"></a>
### 通配符
<a name="UcPRb"></a>
#### 通配符的产生
里式替换原则与协变<br />任何使用父类的地方可以被它的子类替换，在使用类和对象时经常会接触到里式替换原则，其实在数组中一样也符合这种原则，如：
```java
class Fruit {}
class Apple extends Fruit {}
class Jonathan extends Apple {}
class Orange extends Fruit {}

public class CovariantArrays {
    public  void main(String[] args) {
        Fruit[] fruit = new Apple[10];
        List<Fruit> fruits=new ArrayList<Apple>();//error
        fruit[0] = new Apple(); // OK
        fruit[1] = new Jonathan(); // OK
        // Runtime type is Apple[], not Fruit[] or Orange[]:
        try {
            // Compiler allows you to add Fruit:
            fruit[0] = new Fruit(); // ArrayStoreException
        } catch(Exception e) { System.out.println(e); }
        try {
            // Compiler allows you to add Oranges:
            fruit[0] = new Orange(); // ArrayStoreException
        } catch(Exception e) { System.out.println(e); }
    }
}
```
数组中的这种向上转变称为数组的协变，而泛型中是不支持协变的，如上面的
```java
List<Fruit> fruits=new ArrayList<Apple>();//error
```
会产生编译时错误，之所以这么设计是因为数组支持运行时检查而集合类不支持运行时检查。<br />Java的泛型的这种特性对于有需要向上转型的需求时就无能为力，所以 Java 为了满足这种需求设计出了通配符.
<a name="C00BO"></a>
#### 上边界限定通配符
利用 `<? extends Fruit>` 形式的通配符可以实现泛型的向上转型：
```java
List<？extends Fruit> fruits = new ArrayList<Apple>();
```
使用上通配符后编译器为了保证运行时的安全，会限定对其写的操作，开放读的操作，因为编译器只能保证 fruits 集合中存在的是 `Fruits` 及它的子类，并不知道具体的类型，所以对于下面的代码第二行会报错：
```java
ArrayList<? extends Fruit> fruits = new ArrayList<Apple>();
fruits.add(new Fruit());// Error 
Fruit apple = fruits.get(0);
```
<a name="Ux2HA"></a>
#### 下边界限定通配符
利用`<? super T>`形式的通配符可以实现泛型的向下转型
```java
void writeTo(List<? super Apple> apples) {
    apples.add(new Apple());
    apples.add(new Jonathan());

    // apples.add(new Fruit()); // Error
    // Apple apple=apples.get(0);// Error
}
```
与上通配符相反，下边界通配符通常限定读的操作，开放写的操作，对于上面的代码`writeTo` 方法的参数 apples 的类型是 `List<? super Apple>` ，它表示某种类型的 List，这个类型是 Apple 的基类型。也就是说，不知道实际类型是什么，但是这个类型肯定是 Apple 的父类型。因此，可以知道向这个 List 添加一个 Apple 或者其子类型的对象是安全的，这些对象都可以向上转型为 Apple。但是不知道加入 Fruit 对象是否安全，因为那样会使得这个 List 添加跟 Apple 无关的类型.
<a name="Vgb9M"></a>
#### 无边界通配符
还有一种通配符是无边界通配符，它的使用形式是一个单独的问号：`List<?>`，也就是没有任何限定。不做任何限制，跟不用类型参数的 List 有什么区别呢？<br />`List<?>` list 表示 list 是持有某种特定类型的 List，但是不知道具体是哪种类型。那么可以向其中添加对象吗？当然不可以，因为并不知道实际是哪种类型，所以不能添加任何类型，这是不安全的。而单独的 List list ，也就是没有传入泛型参数，表示这个 list 持有的元素的类型是 Object，因此可以添加任何类型的对象，只不过编译器会有警告信息。
<a name="DRuSY"></a>
## Kotlin的泛型
Kotlin中的泛型也是伪泛型，存在泛型擦除，因为它们都是JVM语言？Kotlin 相比于Java 类型使用更为安全，泛型数组不支持协变（Java中支持）避免了数组运行时可能导致的类型转换异常，Kotlin中集合类和数组的泛型是有特定关键字来达到“协变”和“逆变”的效果的，Kotlin 中相对于 Java 的通配符提出了一种新的定义：声明处型变（declaration-site variance）与类型投影（type projections）<br />这两种都是型变，不过一个是在声明处，一个是在使用处，通过 `out `、`in` 关键字来支持型变，类比于 Java 中的 `<? extends T>` `<? super T>`, `<*>` 类比于Java 中的 `<?>` , 例如：
<a name="N9GLB"></a>
### Java
```java
ArrayList<? extends Fruit> fruits = new ArrayList<Apple>();
Fruit apple = fruits.get(0);   
fruits.add(new Fruit());// Error 

void writeTo(List<? super Apple> apples) {
    apples.add(new Apple());
    apples.add(new Jonathan());

    // apples.add(new Fruit()); // Error
    // Apple apple=apples.get(0);// Error
}   
```
<a name="R2O27"></a>
### Kotlin
```kotlin
val fruits:ArrayList<out Fruit> = ArrayList()   
val apple:Fruit = fruits.get(0) 
fruits.add(Fruit()) // Error

void writeTo(apples : ArrayList<in Apple>) {
    apples.add(Apple());
    apples.add(Jonathan());

    // apples.add(Fruit()); // Error
    // val apple:Apple = apples.get(0);// Error
} 
```
另外 Kotlin 除了 `out` , `in` 关键字来支持型变外，它还有上边界泛型约束对应于Java中 `extends`
```kotlin
fun <T : Comparable<T>> sort(list: List<T>) {
    // ……
}
```
冒号之后指定的类型是上界：只有 `Comparable<T>` 的子类型可以替代 T。例如：
```kotlin
sort(listOf(1, 2, 3)) // OK。Int 是 Comparable<Int> 的子类型
sort(listOf(HashMap<Int, String>())) // 错误：HashMap<Int, String> 不是 Comparable<HashMap<Int, String>> 的子类型
```
默认的上界（如果没有声明）是 `Any?`。在尖括号中只能指定一个上界。如果同一类型参数需要多个上界，需要一个单独的 where子句：
```kotlin
fun <T> cloneWhenGreater(list: List<T>, threshold: T): List<T>
    where T : Comparable<T>,
        T : Cloneable {
return list.filter { it > threshold }.map { it.clone() }
```
