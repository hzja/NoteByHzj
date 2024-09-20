Java字符串
<a name="kccuQ"></a>
## 1、String Pool
String 的字符串常量池（String Pool）是一个固定大小的 `HashTable`（数组+链表的数据结构），故不存在两个相同的字符串。也叫 `StringTable`。`StringTable`是放在本地内存的，是C++写的，里面放的是字符串对象的引用，真实的字符串对象是在堆里。
<a name="CqIf9"></a>
### 1.1字符串常量池产生时间
![2022-09-29-10-31-39.929116000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664418748351-559675ff-c496-41d3-a323-fdd993824bfd.png#clientId=u9cfed0ed-269d-4&from=ui&id=u6898f33e&originHeight=765&originWidth=662&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1522589&status=done&style=shadow&taskId=u57d929f5-a2c3-4262-b566-f3c0f80d4d8&title=)<br />像这些静态的、未加载的.class文件的数据被称为静态常量池，但经过jvm把.class文件装入内存、加载到方法区后，常量池就会变为运行时常量池。![2022-09-29-09-31-47.149282700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664415462776-07735a83-6765-4b8f-8eac-dfcf019287a4.png#clientId=ub2810953-1058-4&errorMessage=unknown%20error&from=ui&id=nx3QQ&originHeight=503&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1632940&status=error&style=shadow&taskId=u3cf42d16-18b6-4ab1-a0c7-abf6b049f60&title=)当类加载到内存中后，jvm就会将class常量池中的内容存放到运行时常量池中，运行时常量池存在于内存中，也就是class常量池被加载到内存之后的版本。<br />**不同之处是：它的字面量可以动态的添加(**`**String#intern()**`**)，符号引用可以被解析为直接引用。**<br />简单来说，HotSpot VM里StringTable是个哈希表，里面存的是驻留字符串的引用（而不是驻留字符串实例自身）。也就是说某些普通的字符串实例被这个StringTable引用之后就等同被赋予了“驻留字符串”的身份。这个StringTable在每个HotSpot VM的实例里只有一份，被所有的类共享。类的运行时常量池里的CONSTANT_String类型的常量，经过解析（resolve）之后，同样存的是字符串的引用；解析的过程会去查询StringTable，以保证运行时常量池所引用的字符串与StringTable所引用的是一致的。

| 字符串常量池 |
| --- |
| 本质就是一个哈希表 |
| 存储的是字符串实例的引用 |
| 在被整个JVM共享 |
| 在解析运行时常量池中的符号引用时，会去查询字符串常量池，确保运行时常量池中解析后的直接引用跟字符串常量池中的引用是一致的 |

<a name="lPGae"></a>
### 1.2字符串常量池的位置
以下图表示的是运行时数据区<br />![2022-09-29-09-31-47.508144800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664415494761-16da2cf2-485a-4ffa-b0b7-945a3b1d39ce.png#clientId=ub2810953-1058-4&errorMessage=unknown%20error&from=ui&id=ua01e8122&originHeight=468&originWidth=694&originalType=binary&ratio=1&rotation=0&showTitle=false&size=976506&status=error&style=shadow&taskId=u95b1a286-67de-40d6-8555-c05fba648d2&title=)<br />![2022-09-29-10-31-40.622192600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664418978790-86b40352-6090-4560-a56c-f9b88cccc8b2.png#clientId=u9cfed0ed-269d-4&from=ui&id=u5c078796&originHeight=537&originWidth=942&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1520633&status=done&style=shadow&taskId=u02dcbfd2-7057-4c9a-a9d9-be75990603d&title=)![2022-09-29-10-31-40.947820400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664418979101-69c484db-dabb-417f-ba73-69cb3f2065a5.png#clientId=u9cfed0ed-269d-4&from=ui&id=ude154bab&originHeight=525&originWidth=1010&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1593927&status=done&style=shadow&taskId=u431424a3-de9a-4fb4-b60b-db1727d827c&title=)
<a name="W7Qug"></a>
### 1.3字符串常量池的优点
为了避免频繁的创建和销毁对象而影响系统性能，实现了对象的共享。 例如字符串常量池，在编译阶段就把所有的字符串文字放到一个常量池中。 节省内存空间：常量池中所有相同的字符串常量被合并，只占用一个空间。 节省运行时间：比较字符串时，`==`比`equals()`快。对于两个引用变量，只用`==`判断引用是否相等，也就可以判断实际值是否相等。
<a name="W2rm7"></a>
## 2、String
<a name="vgjuL"></a>
### 2.1不可变性
```java
public final class String
implements java.io.Serializable, Comparable<String>, CharSequence {
    /** The value is used for character storage. */
    private final char value[];
}
```
<a name="Vsq7s"></a>
### 2.2定义方式
<a name="rmpK7"></a>
#### 字面量的方式
```java
String s1 = "hello";
```
此时会有如下过程。 会去解析的符号引用，ldc指令，会先到字符串常量池中查找是否存在对应字符串实例的引用，如果有的话，那么直接返回这个字符串实例的引用，如果没有的话，会创建一个字符串实例，那么将其添加到字符串常量池中（实际上是将其引用放入到一个哈希表中），之后再返回这个字符串实例对象的引用。<br />ldc：将int、float、或者一个类、方法类型或方法句柄的符号引用、还可能是String型常量值从常量池中推送至栈顶，在执行ldc指令时会触发对它的符号引用进行解析。<br />![2022-09-29-10-31-41.377200500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664418862343-b5006656-3876-485b-a612-785b9184f236.png#clientId=u9cfed0ed-269d-4&from=ui&id=uca97ad9c&originHeight=538&originWidth=833&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1347257&status=done&style=shadow&taskId=u6dba6033-0bb7-4b8d-90be-a6964deede0&title=)
<a name="RqkLf"></a>
#### new的方式
**问题：以下方式会创建几个对象？怎么证明？**
```
String str = new String("hello");
```
2个对象或者1个对象<br />①如果字符串常量池中已经有“hello”，则创建了一个对象。如下图。
```java
public class Main {
    public static void main(String[] args) {
        String s1 = "hello";
        String s2 = new String("hello");
        System.out.println(s1==s2);
    }
}
//输出：false，由图容易看出
```
![2022-09-29-09-31-49.284982600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664415546025-701d9361-0d16-4b95-af87-84790d770486.png#clientId=ub2810953-1058-4&errorMessage=unknown%20error&from=ui&id=imBr3&originHeight=510&originWidth=960&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1471762&status=error&style=shadow&taskId=ud75d5714-1e83-470a-8b45-786fbd5135b&title=)②如果字符串常量池中不存在“hello”，则创建了两个对象。<br />一个对象是：new关键字在堆空间创建的 另一个对象是：另外一个是在解析常量池的时候JVM自动创建的，如下图。<br />![2022-09-29-09-31-49.415196200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1664415546121-92d22227-f108-4d07-ba05-bd93ddfee212.png#clientId=ub2810953-1058-4&errorMessage=unknown%20error&from=ui&id=JyKic&originHeight=533&originWidth=990&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1586183&status=error&style=shadow&taskId=ufdca1ee7-2c89-4d12-a79e-08ae32c4413&title=)
<a name="Tum8h"></a>
#### `intern()`的方式
如果不是用字面量的方式定义的String对象，可以使用String提供的`intern`方法：`intern`方法会从字符串常量池中查询当前字符串是否存在，若存在则返回其引用；若不存在就会将当前字符串放入常量池中，并返回其引用。**只需牢记返回的是字符串常量池的引用（即哈希表中的值）即可。**
```java
public class Main {
    public static void main(String[] args) {
        String s1 = new String("1");
        String s2=s1.intern();
        String s3 = "1";
        System.out.println(s2 == s3);
    }
}
```
分析：true<br />第一行，创建了两个对象实例，其引用一个在字符串常量池中，一个返回给s，如上图。<br />第二行，intern()方法会会从字符串常量池中查询当前字符串是否存在，发现存在，返回的是字符串常量池的引用（地址）。<br />第三行，s3是赋值为字符串常量池的引用。 故 s2和s3地址一样。
<a name="SJDqA"></a>
### 2.3字符串拼接
```java
String s =new String("1")+new String("1");
```
分析：true<br />执行完成后，堆区多了两个匿名对象，另外堆区还多了一个字面量为11的字符串实例，并且栈中存在一个引用指向这个实例。当在进行字符串拼接时，编译器默认会创建一个`StringBuilder`对象并调用其`append`方法来进行拼接，最后再调用其`toString`方法来转换成一个字符串，`StringBuilder`的`toString`方法其实就是`new`一个字符串。
```java
//StringBuilder的toString方法
@Override
public String toString() {
    // Create a copy, don't share the array
    return new String(value, 0, count);
}
```
