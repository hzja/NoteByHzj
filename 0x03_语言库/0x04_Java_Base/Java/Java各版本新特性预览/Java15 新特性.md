Java <br />JEP 339：EdDSA 数字签名算法 <br />JEP 360：密封类（预览） <br />JEP 371：隐藏类 <br />JEP 372：删除 Nashorn JavaScript 引擎 <br />JEP 373：重新实现 Legacy DatagramSocket API <br />JEP 374：重新实现 DatagramSocket API <br />JEP 375：实例模式匹配（第二次预览） <br />JEP 377：ZGC：一个可扩展的低延迟垃圾收集器 <br />JEP 378：文本块 <br />JEP 379：低暂停时间垃圾收集器 <br />JEP 381：移除 Solaris 和 SPARC 端口 <br />JEP 383：外部存储器访问 API（第二个内置程序）<br />JEP 384：Records（第二次预览） <br />JEP 385：不推荐的 RMI 激活去除<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602516766626-51253892-7f13-4fab-ac59-7cbfecbaae3e.png#averageHue=%23f4f4f4&height=391&id=HAUWc&originHeight=1174&originWidth=1644&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1289864&status=done&style=shadow&title=&width=548)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602516778199-57c2f779-fcaf-429b-aa4d-1d78b07b838c.png#averageHue=%23f7f7f6&height=201&id=qw9Pe&originHeight=603&originWidth=1644&originalType=binary&ratio=1&rotation=0&showTitle=false&size=207127&status=done&style=shadow&title=&width=548)
<a name="tSbj3"></a>
### 1、文本块Text Blocks（正式版）
文本块，是一个多行字符串，它可以避免使用大多数转义符号，自动以可预测的方式格式化字符串，并让开发人员在需要时可以控制格式。<br />Text Blocks首次是在JDK 13中以预览功能出现的，然后在JDK 14中又预览了一次，终于在JDK 15中被确定下来，可放心使用了。
```java
public static void main(String[] args) {
    String query = """
           SELECT * from USER \
           WHERE `id` = 1 \
           ORDER BY `id`, `name`;\
           """;
    System.out.println(query);
}
```
运行程序，输出（可以看到展示为一行了）：
```sql
SELECT * from USER WHERE `id` = 1 ORDER BY `id`, `name`;
```
<a name="wIulp"></a>
### 2、ZGC可伸缩低延迟垃圾收集器转正（正式版）
ZGC是Java 11引入的新的垃圾收集器（JDK9以后默认的垃圾回收器是G1），经过了多个实验阶段，自此终于成为正式特性。<br />ZGC是一个重新设计的并发的垃圾回收器，可以极大的提升GC的性能。支持任意堆大小而保持稳定的低延迟（10ms以内），性能非常可观。<br />之前需要通过`-XX:+UnlockExperimentalVMOptions -XX:+UseZGC`来启用ZGC，现在只需要`-XX:+UseZGC`就可以。<br />以下是相关介绍：<br />ZGC 是一个可伸缩的、低延迟的垃圾收集器，主要为了满足如下目标进行设计：

- GC 停顿时间不超过 10ms
- 即能处理几百 MB 的小堆，也能处理几个 TB 的大堆
- 应用吞吐能力不会下降超过 15%（与 G1 回收算法相比）
- 方便在此基础上引入新的 GC 特性和利用 colord
- 针以及 Load barriers 优化奠定基础
- 当前只支持 Linux/x64 位平台 停顿时间在 10ms 以下，10ms 其实是一个很保守的数据，即便是 10ms 这个数据，也是 GC 调优几乎达不到的极值。根据 SPECjbb 2015 的基准测试，128G 的大堆下最大停顿时间才 1.68ms，远低于 10ms，和 G1 算法相比，改进非常明显。

![2021-09-29-19-31-37-707242.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632919594373-9b8536b5-bf98-48fc-aa47-8e1b683f5e9d.png#averageHue=%236a93a4&clientId=uac4915c7-e94f-4&from=ui&id=u09613c40&originHeight=525&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=154466&status=done&style=shadow&taskId=u756630b4-bb04-40bc-b409-1795b087678&title=)<br />图片引用自：The Z Garbage Collector – An Introduction<br />不过目前 ZGC 还处于实验阶段，目前只在 Linux/x64 上可用，如果有足够的需求，将来可能会增加对其他平台的支持。同时作为实验性功能的 ZGC 将不会出现在 JDK 构建中，除非在编译时使用 configure 参数：`--with-jvm-features=zgc` 显式启用。<br />在实验阶段，编译完成之后，已经迫不及待的想试试 ZGC，需要配置以下 JVM 参数，才能使用 ZGC，具体启动 ZGC 参数如下：
```java
-XX：+ UnlockExperimentalVMOptions -XX：+ UseZGC -Xmx10g
```
其中参数：-Xmx 是 ZGC 收集器中最重要的调优选项，大大解决了程序员在 JVM 参数调优上的困扰。ZGC 是一个并发收集器，必须要设置一个最大堆的大小，应用需要多大的堆，主要有下面几个考量：

- 对象的分配速率，要保证在 GC 的时候，堆中有足够的内存分配新对象。
- 一般来说，给 ZGC 的内存越多越好，但是也不能浪费内存，所以要找到一个平衡。
<a name="8Qask"></a>
### 3、Shenandoah转正（正式版）
Shenandoah垃圾回收算法终于从实验特性转变为产品特性，这是一个从 JDK 12 引入的回收算法，该算法通过与正在运行的 Java 线程同时进行疏散工作来减少 GC 暂停时间。Shenandoah 的暂停时间与堆大小无关，无论堆栈是 200 MB 还是 200 GB，都具有相同的一致暂停时间。<br />怎么形容Shenandoah和ZGC的关系呢？异同点大概如下：

- 相同点：性能几乎可认为是相同的
- 「不同点」：ZGC是Oracle JDK的，根正苗红。而Shenandoah只存在于OpenJDK中，因此使用时需注意JDK版本
- 打开方式：使用`-XX:+UseShenandoahGC`命令行参数打开。

Shenandoah在JDK12被作为experimental引入，在JDK15变为Production；之前需要通过`-XX:+UnlockExperimentalVMOptions -XX:+UseShenandoahGC`来启用，现在只需要`-XX:+UseShenandoahGC`即可启用
<a name="46Kgr"></a>
### 4、CharSequence新增`isEmpty`默认方法
看源码
```java
@since 15
default boolean isEmpty() {
    return this.length() == 0;
}
```
String实现了`CharSequence`接口的
<a name="GIjyy"></a>
### 5、EdDSA 数字签名算法
新加入了一个安全性和性能都更强的基于 Edwards-Curve Digital Signature Algorithm （EdDSA）实现的数字签名算法。<br />Edwards-Curve 数字签名算法（EdDSA），一种根据 RFC 8032 规范所描述的 Edwards-Curve 数字签名算法（EdDSA）实现加密签名，实现了一种 RFC 8032 标准化方案，但它不能代替 ECDSA。<br />与 JDK 中的现有签名方案相比，EdDSA 具有更高的安全性和性能，因此备受关注。它已经在OpenSSL和BoringSSL等加密库中得到支持，在区块链领域用的比较多。<br />EdDSA是一种现代的椭圆曲线方案，具有JDK中现有签名方案的优点。EdDSA将只在SunEC提供商中实现。<br />虽然其性能优于现有的 ECDSA 实现，不过，它并不会完全取代 JDK 中现有的椭圆曲线数字签名算法( ECDSA)。
```java
// example: generate a key pair and sign
KeyPairGenerator kpg = KeyPairGenerator.getInstance("Ed25519");
KeyPair kp = kpg.generateKeyPair();

byte[] msg = "test_string".getBytes(StandardCharsets.UTF_8);

Signature sig = Signature.getInstance("Ed25519");
sig.initSign(kp.getPrivate());
sig.update(msg);
byte[] s = sig.sign();

String encodedString = Base64.getEncoder().encodeToString(s);
System.out.println(encodedString);

// example: use KeyFactory to contruct a public key
KeyFactory kf = KeyFactory.getInstance("EdDSA");
boolean xOdd = ...
BigInteger y = ...
NamedParameterSpec paramSpec = new NamedParameterSpec("Ed25519");
EdECPublicKeySpec pubSpec = new EdECPublicKeySpec(paramSpec, new EdPoint(xOdd, y));
PublicKey pubKey = kf.generatePublic(pubSpec);
```
输出：
```java
0Hc0lxxASZNvS52WsvnncJOH/mlFhnA8Tc6D/k5DtAX5BSsNVjtPF4R4+yMWXVjrvB2mxVXmChIbki6goFBgAg==
```

- 使用 Edwards-Curve 数字签名算法（EdDSA）实现加密签名。
- 与其它签名方案相比，EdDSA 具有更高的安全性和性能。
- 得到许多其它加密库（如 OpenSSL、BoringSSL）的支持。
<a name="r6kls"></a>
### 6、Sealed Classes（封闭类，预览）
JAVA 的继承目前只能选择允许继承和不允许继承（`final` 修饰），现在新增了一个封闭（Sealed ）类的特性，可以指定某些类才可以继承。<br />封闭类（预览特性），可以是封闭类和或者封闭接口，用来增强 Java 编程语言，防止其他类或接口扩展或实现它们。<br />因为引入了`sealed class`或`interfaces`，这些`class`或者`interfaces`只允许被指定的类或者`interface`进行扩展和实现。<br />使用修饰符`sealed`，可以将一个类声明为密封类。密封的类使用`reserved`关键字`permits`列出可以直接扩展它的类。子类可以是最终的，非密封的或密封的。<br />之前代码是这样的。
```java
public class Person { } //人
 
class Teacher extends Person { }//教师
class Worker extends Person { }  //工人
class Student extends Person{ } //学生
```
但是现在要限制 Person类 只能被这三个类继承，不能被其他类继承，需要这么做。
```java
// 添加sealed修饰符，permits后面跟上只能被继承的子类名称
public sealed class Person permits Teacher, Worker, Student{ } //人
 
// 子类可以被修饰为 final
final class Teacher extends Person { }//教师
 
// 子类可以被修饰为 non-sealed，此时 Worker类就成了普通类，谁都可以继承它
non-sealed class Worker extends Person { }  //工人
// 任何类都可以继承Worker
class AnyClass extends Worker{}
 
//子类可以被修饰为 sealed,同上
sealed class Student extends Person permits MiddleSchoolStudent,GraduateStudent{ } //学生
 
final class MiddleSchoolStudent extends Student { }  //中学生
 
final class GraduateStudent extends Student { }  //研究生
```
很强很实用的一个特性，可以限制类的层次结构。
```java
public sealed interface Service permits Car, Truck {
    int getMaxServiceIntervalInMonths();
    default int getMaxDistanceBetweenServicesInKilometers() {
        return 100000;
    }
}
```
封闭类，可以是封闭类、封闭接口，防止其他类或接口扩展或实现它们。
```java
public abstract sealed class Singer
    permits Jay, Eason{
    ...
}
```
类Singer被`sealed` 修饰，是封闭类，只能被2个指定子类（Jay, Eason）继承。
<a name="558eed13"></a>
### 7、Hidden Classes（隐藏类）
隐藏类是为框架（frameworks）所设计的，隐藏类不能直接被其他类的字节码使用，只能在运行时生成类并通过反射间接使用它们。<br />该提案通过启用标准 API 来定义 无法发现 且 具有有限生命周期 的隐藏类，从而提高 JVM 上所有语言的效率。JDK内部和外部的框架将能够动态生成类，而这些类可以定义隐藏类。通常来说基于JVM的很多语言都有动态生成类的机制，这样可以提高语言的灵活性和效率。

- 隐藏类天生为框架设计的，在运行时生成内部的class。
- 隐藏类只能通过反射访问，不能直接被其他类的字节码访问。
- 隐藏类可以独立于其他类加载、卸载，这可以减少框架的内存占用。
<a name="O9uBd"></a>
#### Hidden Classes是什么呢？
Hidden Classes就是不能直接被其他class的二进制代码使用的class。Hidden Classes主要被一些框架用来生成运行时类，但是这些类不是被用来直接使用的，而是通过反射机制来调用。<br />比如在JDK8中引入的lambda表达式，JVM并不会在编译的时候将lambda表达式转换成为专门的类，而是在运行时将相应的字节码动态生成相应的类对象。<br />另外使用动态代理也可以为某些类生成新的动态类。
<a name="thOcX"></a>
#### 希望这些动态生成的类需要具有什么特性呢？

- 不可发现性。为某些静态的类动态生成的动态类，所以希望把这个动态生成的类看做是静态类的一部分。所以不希望除了该静态类之外的其他机制发现。
- 访问控制。希望在访问控制静态类的同时，也能控制到动态生成的类。
- 生命周期。动态生成类的生命周期一般都比较短，并不需要将其保存和静态类的生命周期一致。
<a name="lhLmE"></a>
#### API的支持
所以需要一些API来定义无法发现的且具有有限生命周期的隐藏类。这将提高所有基于JVM的语言实现的效率。<br />比如：
```java
java.lang.reflect.Proxy // 可以定义隐藏类作为实现代理接口的代理类。 
java.lang.invoke.StringConcatFactory // 可以生成隐藏类来保存常量连接方法； 
java.lang.invoke.LambdaMetaFactory //可以生成隐藏的nestmate类，以容纳访问封闭变量的lambda主体；
```
普通类是通过调用`ClassLoader::defineClass`创建的，而隐藏类是通过调用`Lookup::defineHiddenClass`创建的。这使JVM从提供的字节中派生一个隐藏类，链接该隐藏类，并返回提供对隐藏类的反射访问的查找对象。调用程序可以通过返回的查找对象来获取隐藏类的Class对象。
<a name="wOMOP"></a>
### 8、Remove the Nashorn JavaScript Engine

- Nashorn太难维护了，移除 Nashorn JavaScript引擎成为一种必然
- 其实早在JDK 11 中就已经被标记为 `deprecated` 了。
<a name="PRW4c"></a>
#### Nashorn引擎是什么？
Nashorn 是 JDK 1.8 引入的一个 JavaScript 脚本引擎，用来取代 Rhino 脚本引擎。Nashorn 是 ECMAScript-262 5.1 的完整实现，增强了 Java 和 JavaScript 的兼容性，并且大大提升了性能。
<a name="mqNNR"></a>
#### 为什么要移除？
官方的描述是，随着 ECMAScript 脚本语言的结构、API 的改编速度越来越快，维护 Nashorn 太有挑战性了，所以……。
<a name="ve9FI"></a>
#### 重新实现 DatagramSocket API
重新实现了老的 `DatagramSocket` API 接口，更改了 `java.net.DatagramSocket` 和 `java.net.MulticastSocket` 为更加简单、现代化的底层实现，更易于维护和调试。<br />`java.net.datagram.Socket`和`java.net.MulticastSocket`的当前实现可以追溯到JDK 1.0，那时IPv6还在开发中。因此，当前的多播套接字实现尝试调和IPv4和IPv6难以维护的方式。

- 通过替换 java.net.datagram 的基础实现，重新实现旧版 DatagramSocket API。
- 更改`java.net.DatagramSocket` 和 `java.net.MulticastSocket` 为更加简单、现代化的底层实现。提高了 JDK 的可维护性和稳定性。
- 通过将`java.net.datagram.Socket`和`java.net.MulticastSocket` API的底层实现替换为更简单、更现代的实现来重新实现遗留的DatagramSocket API。

新的实现：

- 易于调试和维护;
- 与Project Loom中正在探索的虚拟线程协同。

在JDK11中取以代之的是GraalVM。GraalVM是一个运行时平台，它支持Java和其他基于Java字节码的语言，但也支持其他语言，如JavaScript，Ruby，Python或LLVM。「性能是Nashorn的2倍以上」。
<a name="t1Ype"></a>
### 9、Reimplement the Legacy DatagramSocket API（重新实现DatagramSocket API）

- 重新实现老的DatagramSocket API
- 更改java.net.DatagramSocket 和 java.net.MulticastSocket 为更加简单、现代化的底层实现。
<a name="NeInd"></a>
### 10、TreeMap
TreeMap 新引入了下面这些方法：

- `putIfAbsent()`
- `computeIfAbsent()`
- `computeIfPresent()`
- `compute()`
- `merge()`
<a name="o85j4"></a>
### 11、`record` 关键字（预览新特性）
Java 15 对 Java 14 中引入的预览新特性进行了增强，主要是引入了一个新的概念密封类（Sealed Classes）。<br />密封类可以对继承或者实现它们的类进行限制。<br />比如抽象类 `Person` 只允许 `Employee` 和 `Manager` 继承。
```java
public abstract sealed class Person
    permits Employee, Manager {

    //...
}
```
另外，任何扩展密封类的类本身都必须声明为 `sealed`、`non-sealed` 或 `final`。
```java
public final class Employee extends Person {
}

public non-sealed class Manager extends Person {
}
```
![2021-09-06-16-57-05-989708.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630931064202-76311e92-817c-43b5-a184-fedc074dbe28.png#averageHue=%232d323b&clientId=u83455751-6f41-4&from=ui&id=u56b45b1d&originHeight=163&originWidth=644&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29165&status=done&style=none&taskId=u49e0469a-9b49-4b44-9ef5-133046875ed&title=)<br />在 java.lang.Class 增加了两个公共方法用于获取 Record 类信息：

1. `RecordComponent[] getRecordComponents()`
2. `boolean isRecord()`
<a name="XBJ8P"></a>
### 12、`instanceof` 模式匹配（预览新特性）
Java 15 并没有对此特性进行调整，继续预览特性，主要用于接受更多的使用反馈。<br />在未来的 Java 版本中，Java 的目标是继续完善 `instanceof` 模式匹配新特性。<br />Java 14 之前：
```java
if (object instanceof Kid) {
    Kid kid = (Kid) object;
    // ...
} else if (object instanceof Kiddle) {
    Kid kid = (Kid) object;
    // ...
}
```
Java 14+：
```java
if (object instanceof Kid kid) {
    // ...
} else if (object instanceof Kiddle kiddle) {
    // ...
}
```
Java 15 并没有对此特性进行调整，继续预览特性，只是为了收集更多的用户反馈，可能还不成熟吧。
<a name="UaLEi"></a>
### 13、禁用偏向锁定
准备禁用和废除偏向锁，在 JDK 15 中，默认情况下禁用偏向锁，并弃用所有相关的命令行选项。<br />在默认情况下禁用偏向锁定，并弃用所有相关命令行选项。目标是确定是否需要继续支持偏置锁定的 高维护成本 的遗留同步优化， HotSpot虚拟机使用该优化来减少非竞争锁定的开销。尽管某些Java应用程序在禁用偏向锁后可能会出现性能下降，但偏向锁的性能提高通常不像以前那么明显。<br />该特性默认禁用了`biased locking(-XX:+UseBiasedLocking`)，并且废弃了所有相关的命令行选型(`BiasedLockingStartupDelay`, `BiasedLockingBulkRebiasThreshold`, `BiasedLockingBulkRevokeThreshold`, `BiasedLockingDecayTime`, `UseOptoBiasInlining`, `PrintBiasedLockingStatistics` and `PrintPreciseBiasedLockingStatistics`)
<a name="IWSDE"></a>
### 14、移除了 Solaris 和 SPARC 端口。
移除了 Solaris/SPARC、Solaris/x64 和 Linux/SPARC 端口的源代码及构建支持。这些端口在 JDK 14 中就已经被标记为 deprecated 了，JDK 15 被移除也不奇怪。<br />删除对Solaris/SPARC、Solaris/x64和Linux/SPARC端口的源代码和构建支持，在JDK 14中被标记为废弃，在JDK15版本正式移除。许多正在开发的项目和功能（如Valhalla、Loom和Panama）需要进行重大更改以适应CPU架构和操作系统特定代码。<br />近年来，Solaris 和 SPARC 都已被 Linux 操作系统和英特尔处理器取代。放弃对 Solaris 和 SPARC 端口的支持将使 OpenJDK 社区的贡献者能够加速开发新功能，从而推动平台向前发展。
<a name="PsCeW"></a>
### 15、外部存储器访问 API（孵化器版）
外存访问 API（二次孵化），可以允许 Java 应用程序安全有效地访问 Java 堆之外的外部内存。<br />目的是引入一个 API，以允许 Java 程序安全、有效地访问 Java 堆之外的外部存储器。如本机、持久和托管堆。<br />有许多Java程序是访问外部内存的，比如Ignite和MapDB。该API将有助于避免与垃圾收集相关的成本以及与跨进程共享内存以及通过将文件映射到内存来序列化和反序列化内存内容相关的不可预测性 。该Java API目前没有为访问外部内存提供令人满意的解决方案。但是在新的提议中，API不应该破坏JVM的安全性。<br />Foreign-Memory Access API在JDK14被作为incubating API引入，在JDK15处于Second Incubator，提供了改进。
<a name="ZbxdN"></a>
### 16、废除 RMI 激活
RMI Activation被标记为Deprecate,将会在未来的版本中删除。RMI激活机制是RMI中一个过时的部分，自Java 8以来一直是可选的而非必选项。RMI激活机制增加了持续的维护负担。RMI的其他部分暂时不会被弃用。<br />RMI jdk1.2引入，EJB在RMI系统中，使用延迟激活。延迟激活将激活对象推迟到客户第一次使用（即第一次方法调用）之前。既然RMI Activation这么好用，为什么要废弃呢？<br />因为对于现代应用程序来说，分布式系统大部分都是基于Web的，web服务器已经解决了穿越防火墙，过滤请求，身份验证和安全性的问题，并且也提供了很多延迟加载的技术。<br />所以在现代应用程序中，RMI Activation已经很少被使用到了。并且在各种开源的代码库中，也基本上找不到RMI Activation的使用代码了。为了减少RMI Activation的维护成本，在JDK8中，RMI Activation被置为可选的。现在在JDK15中，终于可以废弃了。
<a name="v5hAA"></a>
### 17、其他

- Disable and Deprecate Biased Locking（准备禁用偏向锁）
- instanceof 自动匹配模式（预览）
- ZGC，一个可伸缩、低延迟的垃圾回收器。（转正）
- Text Blocks，文本功能转正（JDK 13和14预览，14终于转正）
- Remove the Solaris and SPARC Ports（删除 Solaris 和 SPARC 端口）
- 外部存储器访问 API（允许Java 应用程序安全有效地访问 Java 堆之外的外部内存。）
- Record类型二次预览（在Java 14就预览过）
- Nashorn JavaScript 引擎彻底移除 ：Nashorn 从 Java8 开始引入的 JavaScript 引擎，Java9 对 Nashorn 做了些增强，实现了一些 ES6 的新特性。在 Java 11 中就已经被弃用，到了 Java 15 就彻底被删除了。
- DatagramSocket API 重构
- 禁用和废弃偏向锁（Biased Locking） ：偏向锁的引入增加了 JVM 的复杂性大于其带来的性能提升。不过仍然可以使用 `-XX:+UseBiasedLocking` 启用偏向锁定，但它会提示这是一个已弃用的 API。
