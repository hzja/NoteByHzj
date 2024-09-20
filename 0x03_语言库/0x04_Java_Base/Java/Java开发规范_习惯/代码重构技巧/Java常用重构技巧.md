Java
<a name="Evi59"></a>
## 基本原则
<a name="ERkae"></a>
### 1. 尽量在合适的场合使用单例
使用单例可以减轻加载的负担，缩短加载的时间，提高加载的效率，但并不是所有地方都适用于单例，简单来说，单例主要适用于以下三个方面：第一，控制资源的使用，通过线程同步来控制资源的并发访问；第二，控制实例的产生，以达到节约资源的目的；第三，控制数据共享，在不建立直接关联的条件下，让多个不相关的进程或线程之间实现通信。
<a name="g8Hli"></a>
### 2. 尽量避免随意使用静态变量
要知道，当某个对象被定义为`static`变量所引用，那么GC通常是不会回收这个对象所占有的内存，如
```java
public class A{ 
   private static B b = new B(); 
}
```
此时静态变量b的生命周期与A类同步，如果A类不会卸载，那么b对象会常驻内存，直到程序终止。
<a name="GLpGt"></a>
### 3. 尽量避免过多过常的创建Java对象
尽量避免在经常调用的方法，循环中`new`对象，由于系统不仅要花费时间来创建对象，而且还要花时间对这些对象进行垃圾回收和处理，在可以控制的范围内，最大限度的重用对象，最好能用基本的数据类型或数组来替代对象。
<a name="tF4RE"></a>
### 4. 尽量使用`final`修饰符
带有`final`修饰符的类是不可派生的。在JAVA核心API中，有许多应用`final`的例子，例如java.lang.String，为String类指定`final`防止了使用者覆盖`length()`方法。另外，如果一个类是`final`的，则该类所有方法都是`final`的。java编译器会寻找机会内联（`inline`）所有的`final`方法（这和具体的编译器实现有关）。此举能够使性能平均提高50%。如：让访问实例内变量的getter/setter方法变成”`final`：简单的getter/setter方法应该被置成`final`，这会告诉编译器，这个方法不会被重载，所以，可以变成“`inlined`”,例子：
```java
class MAF { 
     public void setSize (int size) { 
          _size = size; 
     } 
     private int _size; 
}
 
// 更正
 
class DAF_fixed { 
     final public void setSize (int size) { 
          _size = size; 
     } 
     private int _size; 
}
```
<a name="CR33t"></a>
### 5. 尽量使用局部变量
调用方法时传递的参数以及在调用中创建的临时变量都保存在栈（Stack）中，速度较快。其他变量，如静态变量，实例变量等，都在堆（Heap）中创建，速度较慢。
<a name="F0ZRc"></a>
### 6. 尽量处理好包装类型和基本类型两者的使用场所
虽然包装类型和基本类型在使用过程中是可以相互转换，但它们两者所产生的内存区域是完全不同的，基本类型数据产生和处理都在栈中处理，包装类型是对象，是在堆中产生实例。在集合类对象，有对象方面需要的处理适用包装类型，其他的处理提倡使用基本类型。
<a name="Le6mX"></a>
### 7. 慎用`synchronized`，尽量减小`synchronize`的方法
都知道，实现同步是要很大的系统开销作为代价的，甚至可能造成死锁，所以尽量避免无谓的同步控制。`synchronize`方法被调用时，直接会把当前对象锁 了，在方法执行完之前其他线程无法调用当前对象的其他方法。所以`synchronize`的方法尽量小，并且应尽量使用方法同步代替代码块同步。
<a name="pNT8P"></a>
### 8. 尽量使用`StringBuilder`和`StringBuffer`进行字符串连接
<a name="mPLW1"></a>
### 9. 尽量不要使用`finalize`方法
实际上，将资源清理放在`finalize`方法中完成是非常不好的选择，由于GC的工作量很大，尤其是回收Young代内存时，大都会引起应用程序暂停，所以再选择使用`finalize`方法进行资源清理，会导致GC负担更大，程序运行效率更差。另外，不推荐用`finalize`方法的根本原因在于，JVM的规范并不保证何时执行该方法，所以用这个方法来释放资源很不合适，有可能造成长时间资源得不到释放。
<a name="Bbhu5"></a>
### 10. 尽量使用基本数据类型代替对象
```java
String str = "hello";
```
上面这种方式会创建一个“hello”字符串，而且JVM的字符缓存池还会缓存这个字符串；
```java
String str = new String("hello");
```
此时程序除创建字符串外，str所引用的String对象底层还包含一个char[]数组，这个`char[]`数组依次存放了h,e,l,l,o
<a name="kGOYx"></a>
### 11. 多线程使用map、list原则
在未发生线程安全前提下应尽量使用`HashMap`、`ArrayList`，`HashTable`、`Vector`等使用了同步机制，降低了性能。
<a name="qTQze"></a>
### 12. 尽量合理的创建`HashMap`
当要创建一个比较大的hashMap时，充分利用这个构造函数
```java
public HashMap(int initialCapacity, float loadFactor); 
```
避免`HashMap`多次进行了hash重构,扩容是一件很耗费性能的事，在默认中`initialCapacity`只有16，而`loadFactor`是 0.75，需要多大的容量，最好能准确的估计所需要的最佳大小，同样的`Hashtable`，`Vectors`也是一样的道理。
<a name="BnETG"></a>
### 13. 尽量减少对变量的重复计算
如：
```java
for(int i=0;i<list.size();i++)
```
应该改为
```java
for(int i=0,len=list.size();i<len;i++)
```
并且在循环中应该避免使用复杂的表达式，在循环中，循环条件会被反复计算，如果不使用复杂表达式，而使循环条件值不变的话，程序将会运行的更快。
<a name="vBdpX"></a>
### 14. 尽量避免不必要的创建
如：
```java
A a = new A();

if(i==1){list.add(a);}
// 应该改为
if(i==1){
    A a = new A();
    list.add(a);
}
```
<a name="Q2Ocm"></a>
### 15. 尽量在`finally`块中释放资源
程序中使用到的资源应当被释放，以避免资源泄漏。这最好在`finally`块中去做。不管程序执行的结果如何，`finally`块总是会执行的，以确保资源的正确关闭。
<a name="gqyvO"></a>
### 16. 尽量确定`StringBuffer`的容量
`StringBuffer` 的构造器会创建一个默认大小（通常是16）的字符数组。在使用中，如果超出这个大小，就会重新分配内存，创建一个更大的数组，并将原先的数组复制过来，再 丢弃旧的数组。在大多数情况下，可以在创建 `StringBuffer`的时候指定大小，这样就避免了在容量不够的时候自动增长，以提高性能。如：`StringBuffer buffer = new StringBuffer(1000);`
<a name="rC7ar"></a>
### 17. 尽量早释放无用对象的引用
大部分时，方法局部引用变量所引用的对象 会随着方法结束而变成垃圾，因此，大部分时候程序无需将局部，引用变量显式设为null。例如：Java代码
```java
Public void test(){      
    Object obj = new Object();      
    …… 
    Obj=null;      
}
```
上面这个就没必要了，随着方法`test()`的执行完成，程序中obj引用变量的作用域就结束了。但是如果是改成下面：
```java
Public void test(){      
    Object obj = new Object();      
    ……      
    Obj=null;      
    //执行耗时，耗内存操作；或调用耗时，耗内存的方法     
    …… 
}
```
这时候就有必要将obj赋值为null，可以尽早的释放对Object对象的引用。
<a name="Xp26F"></a>
### 18. 尽量避免使用二维数组
二维数据占用的内存空间比一维数组多得多，大概10倍以上。
<a name="k49Xq"></a>
### 19. 尽量避免使用`split`
除非是必须的，否则应该避免使用`split`，`split`由于支持正则表达式，所以效率比较低，如果是频繁的几十，几百万的调用将会耗费大量资源，如果确实需要频繁的调用`split`，可以考虑使用apache的`StringUtils.split(string,char)`，频繁`split`的可以缓存结果。
<a name="WONbK"></a>
### 20. ArrayList & LinkedList
一个是线性表，一个是链表，一句话，随机查询尽量使用ArrayList，ArrayList优于LinkedList，LinkedList还要移动指针，添加删除的操作LinkedList优于ArrayList，ArrayList还要移动数据，不过这是理论性分析，事实未必如此，重要的是理解好2者得数据结构，对症下药。
<a name="e3vF1"></a>
### 21. 尽量使用`System.arraycopy()`代替通过来循环复制数组
`System.arraycopy()` 要比通过循环来复制数组快的多
<a name="rBwii"></a>
### 22. 尽量缓存经常使用的对象
尽可能将经常使用的对象进行缓存，可以使用数组，或HashMap的容器来进行缓存，但这种方式可能导致系统占用过多的缓存，性能下降，推荐可以使用一些第三方的开源工具，如EhCache，Oscache进行缓存，他们基本都实现了FIFO/FLU等缓存算法。
<a name="Aiu7q"></a>
### 23. 尽量避免非常大的内存分配
有时候问题不是由当时的堆状态造成的，而是因为分配失败造成的。分配的内存块都必须是连续的，而随着堆越来越满，找到较大的连续块越来越困难。
<a name="OLdmP"></a>
### 24. 慎用异常
当创建一个异常时，需要收集一个栈跟踪(stack track)，这个栈跟踪用于描述异常是在何处创建的。构建这些栈跟踪时需要为运行时栈做一份快照，正是这一部分开销很大。当需要创建一个 Exception 时，JVM 不得不说：先别动，我想就您现在的样子存一份快照，所以暂时停止入栈和出栈操作。栈跟踪不只包含运行时栈中的一两个元素，而是包含这个栈中的每一个元素。如果您创建一个 Exception ，就得付出代价。好在捕获异常开销不大，因此可以使用 try-catch 将核心内容包起来。从技术上讲，您甚至可以随意地抛出异常，而不用花费很大的代价。招致性能损失的并不是 throw 操作——尽管在没有预先创建异常的情况下就抛出异常是有点不寻常。真正要花代价的是创建异常。幸运的是，好的编程习惯已教会我们，不应该不管三七二十一就抛出异常。异常是为异常的情况而设计的，使用时也应该牢记这一原则。
<a name="fKAwD"></a>
### 25. 尽量重用对象
特别是String对象的使用中，出现字符串连接情况时应使用StringBuffer代替，由于系统不仅要花时间生成对象，以后可能还需要花时间对这些对象进行垃圾回收和处理。因此生成过多的对象将会给程序的性能带来很大的影响。
<a name="dRNLR"></a>
### 26. 不要重复初始化变量
默认情况下，调用类的构造函数时，java会把变量初始化成确定的值，所有的对象被设置成null，整数变量设置成0，float和double变量设置成0.0，逻辑值设置成false。当一个类从另一个类派生时，这一点尤其应该注意，因为用new关键字创建一个对象时，构造函数链中的所有构造函数都会被自动调用。这里有个注意，给成员变量设置初始值但需要调用其他方法的时候，最好放在一个方法比如`initXXX()`中，因为直接调用某方法赋值可能会因为类尚未初始化而抛空指针异常，如：`public int state = this.getState();`
<a name="eDoIB"></a>
### 27. 数据库语句尽量使用大写
在java+Oracle的应用系统开发中，java中内嵌的SQL语言应尽量使用大写形式，以减少Oracle解析器的解析负担。
<a name="skW6T"></a>
### 28. 及时关闭数据连接
在java编程过程中，进行数据库连接，I/O流操作，在使用完毕后，及时关闭以释放资源。因为对这些大对象的操作会造成系统大的开销。
<a name="Rdu5X"></a>
### 29. 不使用的对象及时设置为null
过分的创建对象会消耗系统的大量内存，严重时，会导致内存泄漏，因此，保证过期的对象的及时回收具有重要意义。JVM的GC并非十分智能，因此建议在对象使用完毕后，手动设置成null。大部分时，方法局部引用变量所引用的对象 会随着方法结束而变成垃圾，因此，大部分时候程序无需将局部、引用变量显式设为null。例如：
```java
Public void test(){ 
    Object obj = new Object(); 
    …… 
    Obj=null; 
} 
//上面这个就没必要了，随着方法test()的执行完成，程序中obj引用变量的作用域就结束了。但是如果是改成下面：
Public void test(){ 
    Object obj = new Object(); 
    …… 
    Obj=null; 
    //执行耗时，耗内存操作；或调用耗时，耗内存的方法 
    …… 
}
```
<a name="j71wv"></a>
### 30. 不要在循环中使用Try/Catch语句，应把Try/Catch放在循环最外层
Error是获取系统错误的类，或者说是虚拟机错误的类。不是所有的错误Exception都能获取到的，虚拟机报错Exception就获取不到，必须用Error获取。
<a name="QA8Bl"></a>
### 31. 通过`StringBuffer`的构造函数来设定他的初始化容量，可以明显提升性能
`StringBuffer`的默认容量为16，当`StringBuffer`的容量达到最大容量时，她会将自身容量增加到当前的2倍+2，也就是2*n+2。无论何时，只要`StringBuffer`到达她的最大容量，她就不得不创建一个新的对象数组，然后复制旧的对象数组，这会浪费很多时间。所以给`StringBuffer`设置一个合理的初始化容量值，是很有必要的
<a name="fP5lD"></a>
### 32. 合理使用java.util.Vector
Vector与`StringBuffer`类似，每次扩展容量时，所有现有元素都要赋值到新的存储空间中。`Vector`的默认存储能力为10个元素，扩容加倍。`vector.add(index,obj)` 这个方法可以将元素obj插入到index位置，但index以及之后的元素依次都要向下移动一个位置（将其索引加 1）。除非必要，否则对性能不利。同样规则适用于`remove(int index)`方法，移除此向量中指定位置的元素。将所有后续元素左移（将其索引减 1）。返回此向量中移除的元素。所以删除vector最后一个元素要比删除第1个元素开销低很多。删除所有元素最好用`removeAllElements()`方法。如果要删除vector里的一个元素可以使用 `vector.remove(obj)`；而不必自己检索元素位置，再删除，如
```java
int index = indexOf（obj）;vector.remove(index)；
```
<a name="MyGNT"></a>
### 33. 不用`new`关键字创建对象的实例
用`new`关键词创建类的实例时，构造函数链中的所有构造函数都会被自动调用。但如果一个对象实现了`Cloneable`接口，可以调用`clone()`方法。`clone()`方法不会调用任何类构造函数。下面是`Factory`模式的一个典型实现：
```java
public static Credit getNewCredit() 
{ 
    return new Credit(); 
} 
//改进后的代码使用clone()方法：
private static Credit BaseCredit = new Credit(); 
public static Credit getNewCredit() 
{ 
    return (Credit)BaseCredit.clone(); 
}
```
<a name="BloBk"></a>
### 34. `HaspMap`的遍历
```java
Map<String, String[]> paraMap = new HashMap<String, String[]>(); 
for( Entry<String, String[]> entry : paraMap.entrySet() ) 
{ 
    String appFieldDefId = entry.getKey(); 
    String[] values = entry.getValue(); 
}
```
利用散列值取出相应的`Entry`做比较得到结果，取得entry的值之后直接取key和value。
<a name="vRD8R"></a>
### 35. `array(数组)`和`ArrayList`的使用
array 数组效率最高，但容量固定，无法动态改变，ArrayList容量可以动态增长，但牺牲了效率。
<a name="CFfFg"></a>
### 36. `StringBuffer`,`StringBuilder`的区别
`java.lang.StringBuffer` 线程安全的可变字符序列。一个类似于`String`的字符串缓冲区，但不能修改。StringBuilder与该类相比，通常应该优先使用`StringBuilder`类，因为她支持所有相同的操作，但由于她不执行同步，所以速度更快。为了获得更好的性能，在构造`StringBuffer`或`StringBuilder`时应尽量指定她的容量。当然如果不超过16个字符时就不用了。相同情况下，使用`StringBuilder`比使用`StringBuffer`仅能获得10%~15%的性能提升，但却要冒多线程不安全的风险。综合考虑还是建议使用`StringBuffer`。
<a name="Fh8DX"></a>
### 37. 尽量使用基本数据类型代替对象。
<a name="i0V5E"></a>
### 38. 考虑使用静态方法
如果没有必要去访问对象的外部，那么就使方法成为静态方法。她会被更快地调用，因为她不需要一个虚拟函数导向表。这同时也是一个很好的实践，因为它告诉你如何区分方法的性质，调用这个方法不会改变对象的状态。
<a name="cJQi7"></a>
### 39. 应尽可能避免使用内在的GET,SET方法40. 避免在循环条件中使用复杂表达式
在不做编译优化的情况下，在循环中，循环条件会被反复计算，如果不使用复杂表达式，而使循环条件值不变的话，程序将会运行的更快。例子：
```java
import java.util.Vector; 
class CEL { 
    void method (Vector vector) { 
        for (int i = 0; i < vector.size (); i++)   // Violation 
            ; // ... 
    } 
} 

/* ---------------------更正：---------------------- */

class CEL_fixed { 
    void method (Vector vector) { 
        int size = vector.size () 
            for (int i = 0; i < size; i++) 
                ; // ... 
    } 
}
```
<a name="KTz1B"></a>
### 41. 为`Vectors`和`Hashtables`定义初始大小
JVM为`Vector`扩充大小的时候需要重新创建一个更大的数组，将原原先数组中的内容复制过来，最后，原先的数组再被回收。可见`Vector`容量的扩大是一个颇费时间的事。通常，默认的10个元素大小是不够的。最好能准确的估计所需要的最佳大小。例子：
```java
import java.util.Vector; 
public class DIC { 
     public void addObjects (Object[] o) { 
         // if length > 10, Vector needs to expand 
         for (int i = 0; i< o.length;i++) {    
             v.add(o);    // capacity before it can add more elements. 
         } 
     } 
     public Vector v = new Vector();   // no initialCapacity. 
}
```
更正：自己设定初始大小。
```java
public Vector v = new Vector(20);  
public Hashtable hash = new Hashtable(10);
```
<a name="OG0Qg"></a>
### 42. 在`finally`块中关闭`Stream`
程序中使用到的资源应当被释放，以避免资源泄漏。这最好在`finally`块中去做。不管程序执行的结果如何，`finally`块总是会执行的，以确保资源的正确关闭。
<a name="JxB9R"></a>
### 43. 对于常量字符串，用`String` 代替 `StringBuffer`
常量字符串并不需要动态改变长度。例子：
```java
public class USC { 
     String method () { 
         StringBuffer s = new StringBuffer ("Hello"); 
         String t = s + "World!"; 
         return t; 
     } 
}
```
更正：把`StringBuffer`换成`String`，如果确定这个`String`不会再变的话，这将会减少运行开销提高性能。
<a name="NWuYp"></a>
### 44. 在字符串相加的时候，使用 `''` 代替 `" "`
如果该字符串只有一个字符的话例子：
```java
public class STR { 
     public void method(String s) { 
         String string = s + "d"   // violation. 
         string = "abc" + "d"       // violation. 
     } 
}
```
更正：将一个字符的字符串替换成`''`
```java
public class STR { 
     public void method(String s) { 
         String string = s + 'd' 
         string = "abc" + 'd'   
     } 
}
```
<a name="hoK0E"></a>
## 类和方法优化
<a name="X2KDW"></a>
### No.1：重复代码的提炼
重复代码是重构收效最大的手法之一，进行这项重构的原因不需要多说。它有很多很明显的好处，比如总代码量大大减少，维护方便，代码条理更加清晰易读。它的重点就在于寻找代码当中完成某项子功能的重复代码，找到以后请毫不犹豫将它移动到合适的方法当中，并存放在合适的类当中。小实例
```java
class BadExample {

    public void someMethod1(){
        //code
        System.out.println("重复代码");/* 重复代码块 */
        //code
    }

    public void someMethod2(){
        //code
        System.out.println("重复代码");/* 重复代码块 */
        //code
    }    
}

/* ---------------------分割线---------------------- */

class GoodExample {

    public void someMethod1(){
        //code
        someMethod3();
        //code
    }

    public void someMethod2(){
        //code
        someMethod3();
        //code
    }

    public void someMethod3(){
        System.out.println("重复代码");/* 重复代码块 */
    }    
}
```
<a name="GkAr6"></a>
### No.2：冗长方法的分割
有关冗长方法的分割，其实有时候与重复代码的提炼是有着不可分割的关系的，往往在提炼重复代码的过程中，就不知不觉的完成了对某一个超长方法的分割。倘若在提炼了大部分的重复代码之后，某一些冗长方法依然留存，此时就要静下心来专门处理这些冗长方法了。这其中有一点是值得注意的，由于在分割一个大方法时，大部分都是针对其中的一些子功能分割，因此需要给每一个子功能起一个恰到好处的方法名，这很重要。小实例
```java
class BadExample {

    public void someMethod(){
        //function[1]
        //function[2]
        //function[3]
    }    
}

/* ---------------------分割线---------------------- */

class GoodExample {

    public void someMethod(){
        function1();
        function2();
        function3();
    }
    
    private void function1(){
        //function[1]
    }
    
    private void function2(){
        //function[2]
    }

    private void function3(){
        //function[3]
    }    
}
```
<a name="KvAZB"></a>
### No.3：嵌套条件分支的优化（1）
大量的嵌套条件分支是很容易让人望而却步的代码，应该极力避免这种代码的出现。尽管结构化原则一直在说一个函数只能有一个出口，但是在这么大量的嵌套条件分支下，忘了这所谓的规则吧。有一个专业名词叫卫语句，可以治疗这种恐怖的嵌套条件语句。它的核心思想是，将不满足某些条件的情况放在方法前面，并及时跳出方法，以免对后面的判断造成影响。经过这项手术的代码看起来会非常的清晰小实例
```java
class BadExample {

    public void someMethod(Object A,Object B){
        if (A != null) {
            if (B != null) {
                //code[1]
            }else {
                //code[3]
            }
        }else {
            //code[2]
        }
    }    
}

/* ---------------------分割线---------------------- */

class GoodExample {

    public void someMethod(Object A,Object B){
        if (A == null) {
            //code[2]
            return;
        }
        if (B == null) {
            //code[3]
            return;
        }
        //code[1]
    }    
}
```
<a name="Lyfd7"></a>
### No.4：嵌套条件分支的优化（2）
此处所说的嵌套条件分支与上面的有些许不同，它无法使用卫语句进行优化，而应该是将条件分支合并，以此来达到代码清晰的目的。由这两条也可以看出，嵌套条件分支在编码当中应当尽量避免，它会大大降低代码的可读性。小实例
```java
class BadExample {

    public void someMethod(Object A,Object B){
        if (A != null) {
            if (B != null) {
                //code
            }
        }
    }    
}

/* ---------------------分割线---------------------- */

class GoodExample {

    public void someMethod(Object A,Object B){
        if (A != null && B != null) {
            //code
        }
    }    
}
```
<a name="ZIikb"></a>
### No.5：去掉一次性的临时变量
生活当中经常用一次性筷子，这无疑是对树木的摧残。然而在程序当中，一次性的临时变量不仅是对性能上小小的摧残，更是对代码可读性的亵渎。因此有必要对一些一次性的临时变量进行手术。小实例
```java
class BadExample {
    
    private int i;

    public int someMethod(){
        int temp = getVariable();
        return temp * 100;
    }
    
    public int getVariable(){
        return i;
    }    
}

/* ---------------------分割线---------------------- */

class GoodExample {

    private int i;

    public int someMethod(){
        return getVariable() * 100;
    }
    
    public int getVariable(){
        return i;
    }    
}
```
<a name="D36TO"></a>
### No.6：消除过长参数列表
对于一些传递了大批参数的方法，对于追求代码整洁的程序猿来说，是无法接受的。可以尝试将这些参数封装成一个对象传递给方法，从而去除过长的参数列表。大部分情况下，尝试寻找这样一个对象的时候，它往往已经存在了，因此绝大多数情况下，并不需要做多余的工作。小实例
```java
class BadExample {
    
    public void someMethod(int i,int j,int k,int l,int m,int n){
        //code
    }    
}

/* ---------------------分割线---------------------- */

class GoodExample {

    public void someMethod(Data data){
        //code
    }    
}

class Data{
    
    private int i;
    private int j;
    private int k;
    private int l;
    private int m;
    private int n;
  //getter&&setter    
}
```
<a name="lBb7X"></a>
### No.7：提取类或继承体系中的常量
这项重构的目的是为了消除一些魔数或者是字符串常量等等，魔数所带来的弊端自不用说，它会让人对程序的意图产生迷惑。而对于字符串等类型的常量的消除，更多的好处在于维护时的方便。因为只需要修改一个常量，就可以完成对程序中所有使用该常量的代码的修改。顺便提一句，与此类情况类似并且最常见的，就是Action基类中，对于INPUT、LIST、SUCCESS等这些常量的提取。小实例
```java
class BadExample {
    
    public void someMethod1(){
        send("您的操作已成功！");
    }
    
    public void someMethod2(){
        send("您的操作已成功！");
    }
    
    public void someMethod3(){
        send("您的操作已成功！");
    }
    
    private void send(String message){
        //code
    }
}

/* ---------------------分割线---------------------- */

class GoodExample {
    
    protected static final String SUCCESS_MESSAGE = "您的操作已成功！";

    public void someMethod1(){
        send(SUCCESS_MESSAGE);
    }
    
    public void someMethod2(){
        send(SUCCESS_MESSAGE);
    }
    
    public void someMethod3(){
        send(SUCCESS_MESSAGE);
    }
    
    private void send(String message){
        //code
    }    
}
```
<a name="jSyD9"></a>
### No.8：让类提供应该提供的方法
很多时候，经常会操作一个类的大部分属性，从而得到一个最终想要的结果。这种时候，应该让这个类做它该做的事情，而不应该让我们替它做。而且大部分时候，这个过程最终会成为重复代码的根源。小实例
```java
class BadExample {
    
    public int someMethod(Data data){
        int i = data.getI();
        int j = data.getJ();
        int k = data.getK();
        return i * j * k;
    }
    
    public static class Data{
        
        private int i;
        private int j;
        private int k;
        
        public Data(int i, int j, int k) {
            super();
            this.i = i;
            this.j = j;
            this.k = k;
        }

        public int getI() {
            return i;
        }
        
        public int getJ() {
            return j;
        }
        
        public int getK() {
            return k;
        }        
    }    
}

/* ---------------------分割线---------------------- */

class GoodExample {
    
    public int someMethod(Data data){
        return data.getResult();
    }
    
    public static class Data{
        
        private int i;
        private int j;
        private int k;
        
        public Data(int i, int j, int k) {
            super();
            this.i = i;
            this.j = j;
            this.k = k;
        }

        public int getI() {
            return i;
        }
        
        public int getJ() {
            return j;
        }
        
        public int getK() {
            return k;
        }
        
        public int getResult(){
            return i * j * k;
        }        
    }    
}
```
<a name="xYZwb"></a>
### No.9：拆分冗长的类
这项技巧其实也是属于非常实用的一个技巧，只不过由于它的难度相对较高。针对这个技巧，大部分时候，拆分一个类的关注点应该主要集中在类的属性上面。拆分出来的两批属性应该在逻辑上是可以分离的，并且在代码当中，这两批属性的使用也都分别集中于某一些方法当中。如果实在有一些属性同时存在于拆分后的两批方法内部，那么可以通过参数传递的方式解决这种依赖。类的拆分是一个相对较大的工程，毕竟一个大类往往在程序中已经被很多类所使用着，因此这项重构的难度相当之大，一定要谨慎，并做好足够的测试。
<a name="qev6p"></a>
### No.10：提取继承体系中重复的属性与方法到父类
这项技巧大部分时候需要足够的判断力，很多时候，这其实是在向模板方法模式迈进的过程。往往这一类重构都不会是小工程，因此这一项重构与第九种类似，都需要足够的谨慎与测试。而且需要足够确认，这些提取到父类中的属性或方法，应该是子类的共性的时候，才可以使用这项技巧。
<a name="iSnfz"></a>
## 结束语
以上仅是Java方面编程时的性能优化，性能优化大部分都是在时间、效率、代码结构层次等方面的权衡，各有利弊，不要把上面内容当成教条，或许有些对实际工作适用，有些不适用，还望根据实际工作场景进行取舍吧，活学活用，变通为宜。
