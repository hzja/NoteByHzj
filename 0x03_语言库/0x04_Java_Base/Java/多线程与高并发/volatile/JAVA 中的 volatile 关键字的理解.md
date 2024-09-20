Java
<a name="Tbz7J"></a>
## 基本用法
JAVA 语言里的 volatile 关键字是用来修饰变量的，方式如下入所示。表示：该变量需要直接存储到主内存中。
```java
public class SharedClass {
    public volatile int counter = 0;
}
```
被 `volatile` 关键字修饰的 int counter 变量会直接存储到主内存中。并且所有关于该变量的读操作，都会直接从主内存中读取，而不是从 CPU 缓存。（关于主内存和CPU缓存的区别，如果不理解也不用担心，下面会详细介绍）<br />这么做解决什么问题呢？主要是两个问题：

1. 多线程间可见性的问题，
2. CPU 指令重排序的问题

注：为了描述方便，接下来会把 volatile 修饰的变量简称为“volatile 变量”，把没有用 volatile 修饰的变量建成为“non-volatile”变量。
<a name="IaNy8"></a>
## 理解 volatile 关键字
<a name="DUPKd"></a>
### 变量可见性问题（Variable Visibility Problem）
Volatile 可以保证变量变化在多线程间的可见性。<br />在一个多线程应用中，出于计算性能的考虑，每个线程默认是从主内存将该变量拷贝到线程所在CPU的缓存中，然后进行读写操作的。现在电脑基本都是多核CPU，不同的线程可能运行的不同的核上，而每个核都会有自己的缓存空间。如下图所示（图中的 CPU 1，CPU 2 大家可以直接理解成两个核）：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655882012084-929d8968-8746-47a3-98a0-4d2315ec5e9a.png#clientId=u959a8203-e0a8-4&from=paste&id=ucde3db89&originHeight=423&originWidth=502&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucdb54a45-3aff-4451-9965-cf76a510dc7&title=)<br />这里存在一个问题，JVM 既不会保证什么时候把 CPU 缓存里的数据写到主内存，也不会保证什么时候从主内存读数据到 CPU 缓存。也就是说，不同 CPU 上的线程，对同一个变量可能读取到的值是不一致的，这也就是通常说的：线程间的不可见问题。比如下图，Thread 1 修改的 counter = 7 只在 CPU 1 的缓存内可见，Thread 2 在自己所在的 CPU 2 缓存上读取 counter 变量时，得到的变量 counter 的值依然是 0。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655882012245-54989cea-ed96-4ee2-a1bd-f9a893420c05.png#clientId=u959a8203-e0a8-4&from=paste&id=u8667f56f&originHeight=405&originWidth=468&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3a2412ca-f0ba-42d2-87ee-23a167ffb26&title=)<br />而 volatile 出现的用意之一，就是要解决线程间不可见性，通过 volatile 修饰的变量，都会变得线程间可见。<br />其解决方式就是文章开头提到的：<br />通过 volatile 修饰的变量，所有关于该变量的读操作，都会直接从主内存中读取，而不是 CPU 自己的缓存。而所有该变量的写操都会写到主内存上。<br />因为主内存是所有 CPU 共享的，理所当然即使是不同 CPU 上的线程也能看到其他线程对该变量的修改了。
<a name="fAzSr"></a>
#### volatile 不仅仅只保证 volatile 变量的可见性
volatile 在可见性上所做的工作，实际上比保证 volatile 变量的可见性更多：

1. 当 Thread A 修改了某个被 volatile 变量 V，另一个 Thread B 立马去读该变量 V。一旦 Thread B 读取了变量 V 后，不仅仅是变量 V 对 Thread B 可见， 所有在 Thread A 修改变量 V 之前 Thread A 可见的变量，都将对 Thread B 可见。
2. 当 Thread A 读取一个 volatile 变量 V 时，所有对于 Thread A 可见的其他变量也都会从主内存中被读取。

初次读这两句话可能会有些绕口，这里举个例子：
```java
public class MyClass {
    private int years;
    private int months
    private volatile int days;

    public void update(int years, int months, int days){
        this.years  = years;
        this.months = months;
        this.days   = days;
    }
}
```
这个 MyClass 类中有一个 update 方法，会更新该类的所有3个变量：years，months，days。其中仅 days 是 volatile 变量。当 this.days = days 执行时，也就是当 days 变量的修改被写到主内存时，所有该 Thread 可见的其他变量 years，months 也都会被写到主内存中。换句话说，当 days 被修改后，years 和 months 的修改也会被其他线程可见。<br />再看一个关于读的例子：
```java
public class MyClass {
    private int years;
    private int months
    private volatile int days;

    public int totalDays() {
        int total = this.days;
        total += months * 30;
        total += years * 365;
        return total;
    }

    public void update(int years, int months, int days){
        this.years  = years;
        this.months = months;
        this.days   = days;
    }
}
```
如果用另一个 Thread 调用同一个 MyClass 对象的 totalDays() ，在 int total = this.days 这一行被执行时，因为 days 是 volatile 变量，会从主内存中去读取 days 的值，同时，所有对于该 Thread 可见的其他变量 months 和 years 也都会从主内存中被读出。换句话说，该线程能够获取到最新的 days，months，years 的值。<br />以上就是关于 volatile 解决可见性问题的内容。
<a name="vueMy"></a>
### 指令重排序挑战
出于计算性能的考虑，JVM 和 CPU 允许在保证程序语义一致的范围类，对程序内的指令进行重排序。举个例子：
```java
int a = 1;
int b = 2;

a++;
b++;
```
该代码在经过重排序后可能会变成：
```java
int a = 1;
a++;

int b = 2;
b++;
```
这两段代码的只能顺序虽然不一样，但是语义是相同的——都是定义两个变量（int a = 1 和 int b = 2），然后分别 +1。乍一看，这种重排序没有任何问题，但其实如果咱们把其中一个变量定义为 volatile 变量，此时再结合前面提到的可见性的延伸问题来看，大家可能会发现端倪。<br />还是以可见性问题中的 MyClass 类为例：
```java
public class MyClass {
    private int years;
    private int months
    private volatile int days;

    public void update(int years, int months, int days){
        this.years  = years;
        this.months = months;
        this.days   = days;
    }
}
```
在可见性的部分说过，这里的 update() 方法中，执行到修改 days 这一行时，关于 years 和 months 的修改也会同时被写到主内存中。但如果 JVM 对此处的指令进行了重排序会发生什么？假设指令重排序后的 update() 执行过程如下：
```java
public void update(int years, int months, int days){
    this.days   = days;
    this.months = months;
    this.years  = years;
}
```
days 的修改被提到了最前面，此时，months 和 years 的修改还没有做，换句话说，此处 months 和 years 的修改并不能保证对其他线程可见。这么一来 volatile 关于可见性保证的延伸是不是就失效了？关于这一问题在实际使用 volatile 时并不会碰到，因为 JAVA 已经有解决方案：Happens-Before 规则。
<a name="vDYDX"></a>
#### Java volatile Happens-Before 规则
面对指令重排序对可见性的调整，volatile 采用 Happens-Before 规则解决：

1. 任何原始执行顺序中，在 volatile 变量写指令之前的其他变量读写指令，在重新排序后，不可以被放到 volatile 写指令之后。<br />所有原本就应该 volatile 变量写指令前发生的其他变量读写指令，必须依然在其之前发生（Happens-Before）。
2. 任何原始执行顺序中，在 volatile 变量读指令之后的其他变量读写指令，在重新排序后，不可以被放到 volatile 读指令之前。

有了以上两条 Happens-Before 规则，就避免了指令重排序对 volatile 可见性的影响。
<a name="dVwoc"></a>
## volatile 不能保证原子性
多线程并发中经常提到的“三性”：可见性，有序性，原子性。虽然 volatile 可以保证可见性，有序性，但其并不能保证原子性。<br />当两个线程 Thread 1 和 Thread 2 同时修改统一对象下的 volatile 变量 counter 时，比如同时执行 counter++。此时两个线程读取到的 counter 值可能都是 0，经过各线程的计算，他们认为 counter + 1 后的结果都是 1。最终虽然分别用两个线程对 counter 变量做了 + 1 操作，可最终结果不是 2 而是 1。因此说 volatile 并不能保证该变量读写操作的原子性。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655882012078-b198aa88-96d2-43e6-8197-5f359cd310f5.png#clientId=u959a8203-e0a8-4&from=paste&id=u645b4243&originHeight=404&originWidth=465&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uef5fa743-30fb-486e-959d-6e2384dd83f&title=)<br />如果希望避免该问题，需要使用 synchronized 关键字。用 synchronized 关键字来修饰对变量读写操作（counter++）的方法/代码块，保证该读写操作的原子性。<br />除了 synchronized 关键字，还可以直接只用 AtomicInterger 类型定义 counter 变量。AtomicInteger 提供了针对 Integer 的原子操作。类似的类还有 AtomicBoolean 和 AtomicLong。<br />synchronized 和 AtomicXXX 类都可以保证原子性，前者是基于锁的原理实现的原子性（悲观锁），而后者则是基于 CAS 原则（乐观锁）。<br />什么场景下只需要 volatile 就足够呢？比如：当某个变量只会被一个线程修改，其他并行线程只会执行读操作时，使用 volatile 就足以。
<a name="ksaw9"></a>
## 关于 Volatile 的性能问题
如果大家了解 CPU 的多级缓存机制，（不了解应该也能猜到），从主内存读取数据的效率一定比从 CPU 缓存中读取的效率低很多。包括指令重排序的目的也是为了提高计算效率，当重排序机制被限制时，计算效率也会相应收到影响。因此，应该只在需要保证变量可见性和有序性时，才使用 volatile 关键字。
