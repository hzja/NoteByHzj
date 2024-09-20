Java String<br />在 JAVA 语言中有8中基本类型和一种比较特殊的类型String。这些类型为了使他们在运行过程中速度更快，更节省内存，都提供了一种常量池的概念。常量池就类似一个JAVA系统级别提供的缓存。<br />8种基本类型的常量池都是系统协调的，String类型的常量池比较特殊。它的主要使用方法有两种：

- 直接使用双引号声明出来的String对象会直接存储在常量池中。
- 如果不是用双引号声明的String对象，可以使用String提供的intern方法。intern 方法会从字符串常量池中查询当前字符串是否存在，若不存在就会将当前字符串放入常量池中
<a name="HrQfg"></a>
## 1、常量池
<a name="WVO5J"></a>
### 1.1 常量池是什么？
![2021-07-23-14-49-13-046855.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627023061329-2a807c96-f346-4e8e-b71d-a24912411e9b.png#clientId=ue4c51358-7d88-4&from=ui&id=ua0e03019&originHeight=608&originWidth=1080&originalType=binary&ratio=1&size=1973792&status=done&style=shadow&taskId=u08202090-996e-4af7-bf5c-86b284780bf)<br />JVM常量池主要分为Class文件常量池、运行时常量池，全局字符串常量池，以及基本类型包装类对象常量池
<a name="yVpPO"></a>
#### 1.1.0 方法区
方法区的作用是存储Java类的结构信息，当创建对象后，对象的类型信息存储在方法区中，实例数据存放在堆中。类型信息是定义在Java代码中的常量、静态变量、以及类中声明的各种方法，方法字段等；实例数据则是在Java中创建的对象实例以及他们的值。<br />该区域进行内存回收的主要目的是对常量池的回收和对内存数据的卸载；一般说这个区域的内存回收率比起Java堆低得多。
<a name="iDGza"></a>
#### 1.1.1 Class文件常量池
class文件是一组以字节为单位的二进制数据流，在Java代码的编译期间，编写的Java文件就被编译为.class文件格式的二进制数据存放在磁盘中，其中就包括class文件常量池。<br />class文件常量池主要存放两大常量：字面量和符号引用。<br />字面量：字面量接近java语言层面的常量概念

- 文本字符串，也就是经常申明的：`public String s = "abc";`中的"abc"
- 用final修饰的成员变量，包括静态变量、实例变量和局部变量：`public final static int f = 0x101;,final int temp = 3;`
- 而对于基本类型数据(甚至是方法中的局部变量)，如`int value = 1`常量池中只保留了他的的字段描述符int和字段的名称value，他们的字面量不会存在于常量池。

符号引用：符号引用主要设涉及编译原理方面的概念

- 类和接口的全限定名，也就是java/lang/String;这样，将类名中原来的".“替换为”/"得到的，主要用于在运行时解析得到类的直接引用
- 字段的名称和描述符，字段也就是类或者接口中声明的变量，包括类级别变量和实例级的变量
- 方法中的名称和描述符，也即参数类型+返回值
<a name="PoYcN"></a>
#### 1.1.2 运行时常量池
当Java文件被编译成class文件之后，会生成上面的class文件常量池，JVM在执行某个类的时候，必须经过加载、链接（验证、准备、解析）、初始化的步鄹，运行时常量池则是在JVM将类加载到内存后，就会将class常量池中的内容存放到运行时常量池中，也就是class常量池被加载到内存之后的版本，是方法区的一部分。<br />在解析阶段，会把符号引用替换为直接引用，解析的过程会去查询字符串常量池，也就StringTable，以保证运行时常量池所引用的字符串与字符串常量池中是一致的。<br />运行时常量池相对于class常量池一大特征就是具有动态性，Java规范并不要求常量只能在运行时才产生，也就是说运行时常量池的内容并不全部来自class常量池，在运行时可以通过代码生成常量并将其放入运行时常量池中，这种特性被用的最多的就是`String.intern()`。
<a name="BE0ok"></a>
#### 1.1.3 字符串常量池
在JDK6.0及之前版本，字符串常量池存放在方法区中，在JDK7.0版本以后，字符串常量池被移到了堆中了。至于为什么移到堆内，大概是由于方法区的内存空间太小了。在HotSpot VM里实现的string pool功能的是一个StringTable类，它是一个Hash表，默认值大小长度是1009；这个StringTable在每个HotSpot VM的实例只有一份，被所有的类共享。字符串常量由一个一个字符组成，放在了StringTable上。<br />在JDK6.0中，StringTable的长度是固定的，长度就是1009，因此如果放入String Pool中的String非常多，就会造成hash冲突，导致链表过长，当调用String#intern()时会需要到链表上一个一个找，从而导致性能大幅度下降；在JDK7.0中，StringTable的长度可以通过参数指定。<br />字符串常量池设计思想：

- 字符串的分配，和其他的对象分配一样，耗费高昂的时间与空间代价，作为最基础的数据类型，大量频繁的创建字符串，极大程度地影响程序的性能
- JVM为了提高性能和减少内存开销，在实例化字符串常量的时候进行了一些优化
   - 为字符串开辟一个字符串常量池，类似于缓存区
   - 创建字符串常量时，首先查看字符串常量池是否存在该字符串
   - 存在该字符串，返回引用实例，不存在，实例化该字符串并放入池中
- 实现的基础
   - 实现该优化的基础是因为字符串是不可变的，可以不用担心数据冲突进行共享
   - 运行时实例创建的全局字符串常量池中有一个表，总是为池中每个唯一的字符串对象维护一个引用,这就意味着它们一直引用着字符串常量池中的对象，所以，在常量池中的这些字符串不会被垃圾收集器回收
<a name="KeUoL"></a>
## 2、`String.intern()`与字符串常量池
```java
/** 
 * Returns a canonical representation for the string object. 
 * <p> 
 * A pool of strings, initially empty, is maintained privately by the 
 * class <code>String</code>. 
 * <p> 
 * When the intern method is invoked, if the pool already contains a 
 * string equal to this <code>String</code> object as determined by 
 * the {@link #equals(Object)} method, then the string from the pool is 
 * returned. Otherwise, this <code>String</code> object is added to the 
 * pool and a reference to this <code>String</code> object is returned. 
 * <p> 
 * It follows that for any two strings <code>s</code> and <code>t</code>, 
 * <code>s.intern()&nbsp;==&nbsp;t.intern()</code> is <code>true</code> 
 * if and only if <code>s.equals(t)</code> is <code>true</code>. 
 * <p> 
 * All literal strings and string-valued constant expressions are 
 * interned. String literals are defined in section 3.10.5 of the 
 * <cite>The Java&trade; Language Specification</cite>. 
 * 
 * @return  a string that has the same contents as this string, but is 
 *          guaranteed to be from a pool of unique strings. 
 */  
public native String intern();   
```
字符串常量池的位置也是随着jdk版本的不同而位置不同。在jdk6中，常量池的位置在永久代（方法区）中，此时常量池中存储的是对象。在jdk7中，常量池的位置在堆中，此时，常量池存储的就是引用了。<br />在jdk8中，永久代（方法区）被元空间取代了。这里就引出了一个很常见很经典的问题，看下面这段代码。
```java
@Test
public void test(){
    String s = new String("2");
    s.intern();
    String s2 = "2";
    System.out.println(s == s2);


    String s3 = new String("3") + new String("3");
    s3.intern();
    String s4 = "33";
    System.out.println(s3 == s4);
}

//jdk6
//false
//false

//jdk7
//false
//true
```
这段代码在jdk6中输出是false false，但是在jdk7中输出的是false true。通过图来一行行解释。
<a name="rpUKT"></a>
### JDK1.6
![2021-07-23-14-49-13-249955.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627023087795-288df3af-7b62-4036-b291-743664b8d52e.png#clientId=ue4c51358-7d88-4&from=ui&id=u9521622a&originHeight=471&originWidth=665&originalType=binary&ratio=1&size=941708&status=done&style=shadow&taskId=u3ad35d86-fff3-431d-8913-e7d631f8c11)

- `String s = new String("2");`创建了两个对象，一个在堆中的`StringObject`对象，一个是在常量池中的“2”对象。
- `s.intern();`在常量池中寻找与s变量内容相同的对象，发现已经存在内容相同对象“2”，返回对象2的地址。
- `String s2 = "2";`使用字面量创建，在常量池寻找是否有相同内容的对象，发现有，返回对象"2"的地址。
- `System.out.println(s == s2);`从上面可以分析出，s变量和s2变量地址指向的是不同的对象，所以返回false

---

- `String s3 = new String("3") + new String("3");`创建了两个对象，一个在堆中的StringObject对象，一个是在常量池中的“3”对象。中间还有2个匿名的`new String("3")`不去讨论它们。
- `s3.intern();`在常量池中寻找与s3变量内容相同的对象，没有发现“33”对象，在常量池中创建“33”对象，返回“33”对象的地址。
- `String s4 = "33";`使用字面量创建，在常量池寻找是否有相同内容的对象，发现有，返回对象"33"的地址。
- `System.out.println(s3 == s4);`从上面可以分析出，s3变量和s4变量地址指向的是不同的对象，所以返回false
<a name="ipZvG"></a>
### JDK1.7
![2021-07-23-14-49-13-633922.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627023097324-b2735e54-90b1-40be-bdd4-3ba061ea9662.png#clientId=ue4c51358-7d88-4&from=ui&id=u3d39decd&originHeight=635&originWidth=792&originalType=binary&ratio=1&size=1511917&status=done&style=shadow&taskId=u08802f89-8663-4cbc-bec6-1a2c6e4e2ba)

- `String s = new String("2");`创建了两个对象，一个在堆中的StringObject对象，一个是在堆中的“2”对象，并在常量池中保存“2”对象的引用地址。
- `s.intern();`在常量池中寻找与s变量内容相同的对象，发现已经存在内容相同对象“2”，返回对象“2”的引用地址。
- `String s2 = "2";`使用字面量创建，在常量池寻找是否有相同内容的对象，发现有，返回对象“2”的引用地址。
- `System.out.println(s == s2);`从上面可以分析出，s变量和s2变量地址指向的是不同的对象，所以返回false

---

- `String s3 = new String("3") + new String("3");`创建了两个对象，一个在堆中的StringObject对象，一个是在堆中的“3”对象，并在常量池中保存“3”对象的引用地址。中间还有2个匿名的`new String("3")`不去讨论它们。
- `s3.intern();`在常量池中寻找与s3变量内容相同的对象，没有发现“33”对象，将s3对应的`StringObject`对象的地址保存到常量池中，返回StringObject对象的地址。
- `String s4 = "33";`使用字面量创建，在常量池寻找是否有相同内容的对象，发现有，返回其地址，也就是`StringObject`对象的引用地址。
- `System.out.println(s3 == s4);`从上面可以分析出，s3变量和s4变量地址指向的是相同的对象，所以返回true。
<a name="PYsfb"></a>
## 3、`String.intern()`的应用
在大量字符串读取赋值的情况下，使用`String.intern()`会大大的节省内存空间。
```java
static final int MAX = 1000 * 10000;
static final String[] arr = new String[MAX];

public static void main(String[] args) throws Exception {
    Integer[] DB_DATA = new Integer[10];
    Random random = new Random(10 * 10000);
    for (int i = 0; i < DB_DATA.length; i++) {
        DB_DATA[i] = random.nextInt();
    }
 long t = System.currentTimeMillis();
    for (int i = 0; i < MAX; i++) {
        //arr[i] = new String(String.valueOf(DB_DATA[i % DB_DATA.length]));
         arr[i] = new String(String.valueOf(DB_DATA[i % DB_DATA.length])).intern();
    }

 System.out.println((System.currentTimeMillis() - t) + "ms");
    System.gc();
}
```
运行的参数是：-Xmx2g -Xms2g -Xmn1500M 上述代码是一个演示代码，其中有两条语句不一样，一条是使用 intern，一条是未使用 intern。发现不使用 intern 的代码生成了1000w 个字符串，占用了大约640m 空间。<br />使用了 intern 的代码生成了1345个字符串，占用总空间 133k 左右。其实通过观察程序中只是用到了10个字符串，所以准确计算后应该是正好相差100w 倍。虽然例子有些极端，但确实能准确反应出 intern 使用后产生的巨大空间节省。<br />利用String的不变性，`String.intern()`方法本质就是维持了一个String的常量池，而且池里的String应该都是唯一的。这样，便可以利用这种唯一性，来做一些文章了。可以利用池里String的对象来做锁，实现对资源的控制。比如一个城市的某种资源同一时间只能一个线程访问，那就可以把城市名的String对象作为锁，放到常量池中去，同一时间只能一个线程获得。<br />不当的使用：fastjson 中对所有的 json 的 key 使用了 intern 方法，缓存到了字符串常量池中，这样每次读取的时候就会非常快，大大减少时间和空间，而且 json 的 key 通常都是不变的。但是这个地方没有考虑到大量的 json key 如果是变化的，那就会给字符串常量池带来很大的负担。
