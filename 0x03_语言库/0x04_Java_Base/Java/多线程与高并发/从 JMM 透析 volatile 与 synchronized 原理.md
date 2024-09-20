Java volatile synchronized<br />在并发编程、一些开源框架中总是会遇到 `volatile` 与 `synchronized` 。`synchronized` 如何保证并发安全？`volatile` 语义的内存可见性指的是什么？这其中又跟 JMM 有什么关系，在并发编程中 JMM 的作用是什么，为什么需要 JMM？与 JVM 内存结构有什么区别？<br />**从以下问题进行探析：**

1. JMM 与 JVM 内存结构有什么区别？
2. 到底什么是 **JMM (Java Memory Model)** 内存模型，JMM 的跟并发编程有什么关系？
3. 内存模型最重要的内容：**指令重排、原子性、内存可见性**。
4. **volatile** 内存可见性指的是什么？它的运用场景以及常见错误使用方式避坑指南。
5. 分析 synchronized 实现原理跟 monitor 的关系；
<a name="Yil7R"></a>
## JVM 内存与 JMM 内存模型
先分别认识 **JVM 内存结构**和 **JMM 内存模型**，总结下就是：

- **JVM 内存结构和 Java 虚拟机的运行时区域有关；**
- **Java 内存模型和 Java 的并发编程有关。**
<a name="P5qBk"></a>
### JVM 内存结构
Java 代码是运行在虚拟机上的，平时写的 .java 文件首先会被编译成 .class 文件，接着被 JVM 虚拟机加载，并且根据不同操作系统平台翻译成对应平台的机器码运行，如下如所示：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615370-bb6625bd-09f0-45a2-8684-5ea7014ff1df.webp#align=left&display=inline&height=474&originHeight=345&originWidth=478&size=0&status=done&style=shadow&width=657)<br />JVM跨平台<br />从图中可以看到，有了 JVM 这个抽象层之后，Java 就可以实现跨平台了。JVM 只需要保证能够正确加载 .class 文件，就可以运行在诸如 Linux、Windows、MacOS 等平台上了。<br />JVM 通过 Java 类加载器加载 javac 编译出来的 class 文件，通过执行引擎解释执行或者 JIT 即时编译调用才调用系统接口实现程序的运行。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615377-1fa47e5b-08cf-41da-a095-db5aad0f8ae0.webp#align=left&display=inline&height=484&originHeight=367&originWidth=498&size=0&status=done&style=shadow&width=657)<br />JVM加载<br />而虚拟机在运行程序的时候会把内存划分为不同的数据区域，不同区域负责不同功能，随着 Java 的发展，内存布局也在调整之中，如下是 Java 8 之后的布局情况，移除了永久代，使用 Mataspace 代替，所以 `-XX:PermSize -XX:MaxPermSize` 等参数变没有意义。<br />**JVM 内存结构**如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615450-b2d85f21-9f11-4492-a09a-a446bb48c2b5.webp#align=left&display=inline&height=592&originHeight=473&originWidth=525&size=0&status=done&style=shadow&width=657)<br />JVM内存布局<br />**执行字节码的模块叫做执行引擎，执行引擎依靠程序计数器恢复线程切换。本地内存包含元数据区域以及一些直接内存。**
<a name="BD3WS"></a>
#### 堆（Heap）
数据共享区域存储实例对象以及数组，通常是占用内存最大的一块也是数据共享的，比如 new Object() 就会生成一个实例；而数组也是保存在堆上面的，因为在 Java 中，数组也是对象。垃圾收集器的主要作用区域。<br />那一个对象创建的时候，到底是在堆上分配，还是在栈上分配呢？这和两个方面有关：对象的类型和在 Java 类中存在的位置。<br />Java 的对象可以分为基本数据类型和普通对象。<br />对于普通对象来说，JVM 会首先在堆上创建对象，然后在其他地方使用的其实是它的引用。比如，把这个引用保存在虚拟机栈的局部变量表中。<br />对于基本数据类型来说（byte、short、int、long、float、double、char)，有两种情况。<br />上面提到，每个线程拥有一个虚拟机栈。当在方法体内声明了基本数据类型的对象，它就会在栈上直接分配。其他情况，通常在在堆上分配，逃逸分析的情况下可能会在栈分配。<br />注意，像 int[] 数组这样的内容，是在堆上分配的。数组并不是基本数据类型。
<a name="n5CqN"></a>
#### 虚拟机栈（Java Virtual Machine Stacks）
Java 虚拟机栈基于线程，即使只有一个 main 方法，都是以线程的方式运行，在运行的生命周期中，参与计算的数据会出栈与入栈，而「虚拟机栈」里面的每条数据就是「栈帧」，在 Java 方法执行的时候则创建一个「栈帧」并入栈「虚拟机栈」。调用结束则「栈帧」出栈，随之对应的线程也结束。
```java
public int add() {
  int a = 1, b = 2;
  return a + b;
}
```
add 方法会被抽象成一个「栈帧」的结构，当方法执行过程中则对应着操作数 1 与 2 的操作数栈入栈，并且赋值给局部变量 a 、b ，遇到 add 指令则将操作数 1、2 出栈相加结果入栈。方法结束后「栈帧」出栈，返回结果结束。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604365615403-bdd52d30-147e-49c7-8907-a4238116d68a.png#align=left&display=inline&height=1295&originHeight=479&originWidth=243&size=0&status=done&style=shadow&width=657)<br />每个栈帧包含四个区域：

1. 局部变量表：基本数据类型、对象引用、retuenAddress 指向字节码的指针；
2. 操作数栈
3. 动态连接
4. 返回地址

**这里有一个重要的地方**：

- 实际上有两层含义的栈，第一层是「栈帧」对应方法；第二层对应着方法的执行，对应着操作数栈。
- 所有的字节码指令，都会被抽象成对栈的入栈与出栈操作。执行引擎只需要傻瓜式的按顺序执行，就可以保证它的正确性。

**每个线程拥有一个「虚拟机栈」，每个「虚拟机栈」拥有多个「栈帧」，而栈帧则对应着一个方法。每个「栈帧」包含局部变量表、操作数栈、动态链接、方法返回地址。方法运行结束则意味着该「栈帧」出栈。**<br />如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604365615426-b2135970-ca4d-4c1b-a95b-c0bb8cd8334c.png#align=left&display=inline&height=383&originHeight=305&originWidth=523&size=0&status=done&style=shadow&width=657)<br />JVM虚拟机栈
<a name="GiHPG"></a>
#### 方法区（Method Area）元空间
存储每个 class 类的元数据信息，比如类的结构、运行时的常量池、字段、方法数据、方法构造函数以及接口初始化等特殊方法。
> 元空间是在堆上么？

答：不是在堆上分配的，而是在堆外空间分配，方法区就是在元空间中。
> 字符串常量池在那个区域中？

答：这个跟 JDK 不同版本不同区别，JDK 1.8 之前，元空间还没有出道成团，方法区被放在一个叫永久代的空间，而字符串常量就在此间。<br />JDK 1.7 之前，字符串常量池也放在叫作永久带的空间。JDK 1.7 之后，字符串常量池从永久代挪到了堆上凑。<br />所以，从 1.7 版本开始，字符串常量池就一直存在于堆上。
<a name="sfTfV"></a>
#### 本地方法栈（Native Method Stacks）
跟虚拟机栈类似，区别在于前者是为 Java 方法服务，而本地方法栈是为 native 方法服务。
<a name="WI5n8"></a>
#### 程序计数器（The PC Register）
保存当前正在执行的 JVM 指令地址。程序在线程切换中运行，那凭啥知道这个线程已经执行到什么地方呢？<br />程序计数器是一块较小的内存空间，它的作用可以看作是当前线程所执行的字节码的行号指示器。这里面存的，就是当前线程执行的进度。
<a name="cDnxP"></a>
### JMM（Java Memory Model，Java 内存模型）
**首先他不是“真实存在”，而是和多线程相关的一组“规范”，需要每个 JVM 的实现都要遵守这样的“规范”，有了 JMM 的规范保障，并发程序运行在不同的虚拟机得到出的程序结果才是安全可靠可信赖。**<br />如果没有 JMM 内存模型来规范，就可能会出现经过不同 JVM “翻译”之后，运行的结果都不相同也不正确。<br />JMM 与处理器、缓存、并发、编译器有关。**它解决了 CPU 多级缓存、处理器优化、指令重排**等导致的结果不可预期的问题数据，保证不同的并发语义关键字得到相应的并发安全的数据资源保护。<br />主要目的就是让 Java 程序员在各种平台下达到一致性访问效果。<br />**是 JUC 包工具类和并发关键字的原理保障**<br />`volatile、synchronized、Lock` 等，它们的实现原理都涉及 JMM。有了 JMM 的参与，才让各个同步工具和关键字能够发挥作用同步语义才能生效，这样才可以开发出并发安全的程序。<br />JMM 最重要的三点内容：**重排序、原子性、内存可见性**。
<a name="OBAxR"></a>
#### 指令重排序
实际上，编译器、JVM、甚至 CPU 都有可能出于优化性能的目的，并不能保证各个语句执行的先后顺序与输入的代码顺序一致，而是调整了顺序，这就是**指令重排序**。<br />**重排序优势**<br />可能有同学会疑问：为什么要指令重排序？有啥用？<br />如下图：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604365615449-0c8d2d18-a749-410c-bf4b-bd4528e5d74e.png#align=left&display=inline&height=438&originHeight=720&originWidth=1080&size=0&status=done&style=shadow&width=657)<br />经过重排序之后，情况如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615734-3bc0a78e-5a9b-4d9a-8e57-5c59910f2ece.webp#align=left&display=inline&height=386&originHeight=634&originWidth=1080&size=0&status=done&style=shadow&width=657)<br />重排序后，对 a 操作的指令发生了改变，**节省了一次 Load a 和一次 Store a**，减少了指令执行，提升了速度改变了运行，这就是重排序带来的好处。<br />**重排序的三种情况**

- 编译器优化<br />比如当前唐伯虎爱慕 “秋香”，那就把对“秋香”的爱慕、约会放到一起执行效率就高得多。避免在撩“冬香”的时候又跑去约会“秋香”，减少了这部分的时间开销，此刻需要一定的顺序重排。不过重排序并不意味着可以任意排序，它需要需要保证重排序后，不改变单线程内的语义，不能把对“秋香”说的话传到“冬香”的耳朵里，否则能任意排序的话，后果不堪设想。
- CPU 重排序<br />这里的优化跟编译器类似，目的都是通过打乱顺序提高整体运行效率，这就是为了更快而执行的秘密武器。<br />
- 内存“重排序”<br />这里不是真正意义的重排序，但是结果跟重排序有类似的成绩。因为还是有区别所以加了双引号作为不一样的定义。<br />由于内存有缓存的存在，在 JMM 里表现为**主存**和**本地内存**，而主存和本地内存的内容可能不一致，所以这也会导致程序表现出乱序的行为。<br />每个线程只能够直接接触到工作内存，无法直接操作主内存，而工作内存中所保存的数据正是主内存的共享变量的副本，主内存和工作内存之间的通信是由 JMM 控制的。

举个例子：<br />线程 1 修改了 a 的值，但是修改后没有来得及把新结果写回主存或者线程 2 没来得及读到最新的值，所以线程 2 看不到刚才线程 1 对 a 的修改，此时线程 2 看到的 a 还是等于初始值。但是线程 2 却可能看到线程 1 修改 a 之后的代码执行效果，表面上看起来像是发生了重顺序。
<a name="uaxb8"></a>
#### 内存可见性
先来看为何会有内存可见性问题
```java
public class Visibility {
    int x = 0;
    public void write() {
        x = 1;
    }
    public void read() {
        int y = x;
    }
}
```
内存可见性问题：**当 x 的值已经被第一个线程修改了，但是其他线程却看不到被修改后的值。**<br />假设两个线程执行的上面的代码，第 1 个线程执行的是 write 方法，第 2 个线程执行的是 read 方法。下面来分析一下，代码在实际运行过程中的情景是怎么样的，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615437-7dbc5d7d-1b94-4959-944a-a07418252f50.webp#align=left&display=inline&height=332&originHeight=385&originWidth=761&size=0&status=done&style=shadow&width=657)<br />它们都可以从主内存中去获取到这个信息，对两个线程来说 x 都是 0。可是此时假设第 1 个线程先去执行 write 方法，它就把 x 的值从 0 改为了 1，但是它改动的动作并不是直接发生在主内存中的，而是会发生在第 1 个线程的工作内存中，如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615584-0899f7f4-02af-400f-a978-3cfb295b8ced.webp#align=left&display=inline&height=332&originHeight=385&originWidth=761&size=0&status=done&style=shadow&width=657)<br />那么，假设线程 1 的工作内存还未同步给主内存，此时假设线程 2 开始读取，那么它读到的 x 值不是 1，而是 0，也就是说虽然此时线程 1 已经把 x 的值改动了，但是对于第 2 个线程而言，根本感知不到 x 的这个变化，这就产生了可见性问题。<br />`volatile、synchronized、final、和锁` 都能保证可见性。要注意的是 volatile，每当变量的值改变的时候，都会立马刷新到主内存中，所以其他线程想要读取这个数据，则需要从主内存中刷新到工作内存上。<br />而锁和同步关键字就比较好理解一些，它是把更多个操作强制转化为原子化的过程。由于只有一把锁，变量的可见性就更容易保证。
<a name="JHdhk"></a>
#### 原子性
大致可以认为基本数据类型变量、引用类型变量、声明为 volatile 的任何类型变量的访问读写是具备原子性的（long 和 double 的非原子性协定：对于 64 位的数据，如 long 和 double，Java 内存模型规范允许虚拟机将没有被 volatile 修饰的 64 位数据的读写操作划分为两次 32 位的操作来进行，即允许虚拟机实现选择可以不保证 64 位数据类型的 load、store、read 和 write 这四个操作的原子性，即如果有多个线程共享一个并未声明为 volatile 的 long 或 double 类型的变量，并且同时对它们进行读取和修改操作，那么某些线程可能会读取到一个既非原值，也不是其他线程修改值的代表了“半个变量”的数值。<br />但由于目前各种平台下的商用虚拟机几乎都选择把 64 位数据的读写操作作为原子操作来对待，因此在编写代码时一般也不需要将用到的 long 和 double 变量专门声明为 volatile）。这些类型变量的读、写天然具有原子性，但类似于 “基本变量++” / “volatile++” 这种复合操作并没有原子性。比如 `i++;`
<a name="DnTOH"></a>
#### Java 内存模型解决的问题
JMM 最重要的的三点内容：**重排序、原子性、内存可见性**。那么 JMM 又是如何解决这些问题的呢？<br />JMM 抽象出主存储器（Main Memory）和工作存储器（Working Memory）两种。

- 主存储器是实例位置所在的区域，所有的实例都存在于主存储器内。比如，实例所拥有的字段即位于主存储器内，主存储器是所有的线程所共享的。
- 工作存储器是线程所拥有的作业区，每个线程都有其专用的工作存储器。工作存储器存有主存储器中必要部分的拷贝，称之为工作拷贝（Working Copy）。

线程是无法直接对主内存进行操作的，如下图所示，线程 A 想要和线程 B 通信，只能通过主存进行交换。<br />经历下面 2 个步骤：<br />1）线程 A 把本地内存 A 中更新过的共享变量刷新到主内存中去。<br />2）线程 B 到主内存中去读取线程 A 之前已更新过的共享变量。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615497-16a14c20-ee69-47ca-af55-84fed20b7921.webp#align=left&display=inline&height=565&originHeight=766&originWidth=891&size=0&status=done&style=shadow&width=657)<br />JMM内存模型<br />从抽象角度看，JMM 定义了线程与主内存之间的抽象关系：

1. 线程之间的共享变量存储在主内存（Main Memory）中；
2. 每个线程都有一个私有的本地内存（Local Memory），本地内存是 JMM 的一个抽象概念，并不真实存在，它涵盖了缓存、写缓冲区、寄存器以及其他的硬件和编译器优化。本地内存中存储了该线程以读/写共享变量的拷贝副本。
3. 从更低的层次来说，主内存就是硬件的内存，而为了获取更好的运行速度，虚拟机及硬件系统可能会让工作内存优先存储于寄存器和高速缓存中。
4. Java 内存模型中的线程的工作内存（working memory）是 cpu 的寄存器和高速缓存的抽象描述。而 JVM 的静态内存储模型（JVM 内存模型）只是一种对内存的物理划分而已，它只局限在内存，而且只局限在 JVM 的内存。

**八个操作**<br />为了支持 JMM，Java 定义了 8 种原子操作（Action），用来控制主存与工作内存之间的交互：

1. **read** 读取：作用于主内存，将共享变量从主内存传动到线程的工作内存中，供后面的 load 动作使用。
2. **load** 载入：作用于工作内存，把 read 读取的值放到工作内存中的副本变量中。
3. **store** 存储：作用于工作内存，把工作内存中的变量传送到主内存中，为随后的 write 操作使用。
4. **write** 写入：作用于主内存，把 store 传送值写到主内存的变量中。
5. **use** 使用：作用于工作内存，把工作内存的值传递给执行引擎，当虚拟机遇到一个需要使用这个变量的指令，就会执行这个动作。
6. **assign** 赋值：作用于工作内存，把执行引擎获取到的值赋值给工作内存中的变量，当虚拟机栈遇到给变量赋值的指令，执行该操作。比如 `int i = 1;`
7. **lock（锁定）** 作用于主内存，把变量标记为线程独占状态。
8. **unlock（解锁）** 作用于主内存，它将释放独占状态。

![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615644-0ba839c2-284d-41ce-8476-94c2ce9ee289.webp#align=left&display=inline&height=388&originHeight=638&originWidth=1080&size=0&status=done&style=shadow&width=657)<br />如上图所示，把一个变量数据从主内存复制到工作内存，要顺序执行 read 和 load；而把变量数据从工作内存同步回主内存，就要顺序执行 store 和 write 操作。<br />由于**重排序、原子性、内存可见性**，带来的不一致问题，JMM 通过 八个原子动作，内存屏障保证了并发语义关键字的代码能够实现对应的安全并发访问。<br />**原子性保障**<br />JMM 保证了 read、load、assign、use、store 和 write 六个操作具有原子性，可以认为除了 long 和 double 类型以外，对其他基本数据类型所对应的内存单元的访问读写都是原子的。<br />但是当想要更大范围的的原子性保证就需要使用 ，就可以使用 lock 和 unlock 这两个操作。<br />**内存屏障：内存可见性与指令重排序**<br />那 JMM 如何保障指令重排序排序，内存可见性带来并发访问问题？<br />内存屏障（Memory Barrier）用于控制在特定条件下的重排序和内存可见性问题。JMM 内存屏障可分为读屏障和写屏障，Java 的内存屏障实际上也是上述两种的组合，完成一系列的屏障和数据同步功能。**Java 编译器在生成字节码时，会在执行指令序列的适当位置插入内存屏障来限制处理器的重排序**。<br />组合如下：

- Load-Load Barriers：load1 的加载优先于 load2 以及所有后续的加载指令，在指令前插入 Load Barrier，使得高速缓存中的数据失效，强制重新从驻内存中加载数据。<br />
- Load-Store Barriers：确保 load1 数据的加载先于 store2 以及之后的存储指令刷新到内存。<br />
- Store-Store Barriers：确保 store1 数据对其他处理器可见，并且先于 store2 以及所有后续的存储指令。在 Store Barrie 指令后插入 Store Barrie 会把写入缓存的最新数据刷新到主内存，使得其他线程可见。<br />
- Store-Load Barriers：在 Load2 及后续所有读取操作执行前，保证 Store1 的写入对所有处理器可见。这条内存屏障指令是一个全能型的屏障，它同时具有其他 3 条屏障的效果，而且它的开销也是四种屏障中最大的一个。<br />
<a name="5Q8No"></a>
## JMM 总结
JMM 是一个抽象概念，由于 CPU 多核多级缓存、为了优化代码会发生指令重排的原因，JMM 为了屏蔽细节，定义了一套规范，保证最终的并发安全。它抽象出了工作内存于主内存的概念，并且**通过八个原子操作以及内存屏障保证了原子性、内存可见性、防止指令重排**，使得 volatile 能保证内存可见性并防止指令重排、synchronised 保证了内存可见性、原子性、防止指令重排导致的线程安全问题，JMM 是并发编程的基础。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615486-369711c4-e71b-4124-8545-f34bd01cce89.webp#align=left&display=inline&height=624&originHeight=842&originWidth=887&size=0&status=done&style=shadow&width=657)<br />并且 JMM 为程序中所有的操作定义了一个关系，称之为 「Happens-Before」原则，要保证执行操作 B 的线程看到操作 A 的结果，那么 A、B 之间必须满足「Happens-Before」关系，如果这两个操作缺乏这个关系，那么 JVM 可以任意重排序。<br />**Happens-Before**

- 程序顺序原则：如果程序操作 A 在操作 B 之前，那么多线程中的操作依然是 A 在 B 之前执行。
- 监视器锁原则：在监视器锁上的解锁操作必须在同一个监视器上的加锁操作之前执行。
- volatile 变量原则：对 volatile 修饰的变量写入操作必须在该变量的读操作之前执行。
- 线程启动原则：在线程对 Tread.start 调用必须在该线程执行任何操作之前执行。
- 线程结束原则：线程的任何操作必须在其他线程检测到该线程结束前执行，或者从 Thread.join 中成功返回，或者在调用 Thread.isAlive 返回 false。
- 中断原则：当一个线程在另一个线程上调用 interrupt 时，必须在被中断线程检测到 interrupt 调用之前执行。
- 终结器规则：对象的构造方法必须在启动对象的终结器之前完成。
- 传递性：如果操作 A 在操作 B 之前执行，并且操作 B 在操作 C 之前执行，那么操作 A 必须在操作 C 之前执行。
<a name="HMTCn"></a>
## volatile
它是 Java 中的一个关键字，当一个变量是共享变量，同时被 `volatile` 修饰当值被更改的时候，其他线程再读取该变量的时候可以保证能获取到修改后的值，通过 JMM 屏蔽掉各种硬件和操作系统的内存访问差异 以及 CPU 多级缓存等导致的数据不一致问题。<br />需要注意的是，volatile 修饰的变量对所有线程是立即可见的，关键字本身就包含了禁止指令重排的语意，但是在非原子操作的并发读写中是不安全的，比如 i++ 操作一共分三步操作。<br />相比 `synchronised` `Lock` `volatile` 更加轻量级，不会发生上下文切换等开销，接着来分析下他的适用场景，以及错误使用场景。<br />**volatile 的作用**

- 保证可见性：Happens-before 关系中对于 volatile 是这样描述的：对一个 volatile 变量的写操作 happen-before 后面对该变量的读操作。<br />这就代表了如果变量被 volatile 修饰，那么每次修改之后，接下来在读取这个变量的时候一定能读取到该变量最新的值。<br />
- 禁止指令重排：先介绍一下 as-if-serial 语义：不管怎么重排序，（单线程）程序的执行结果不会改变。在满足 as-if-serial 语义的前提下，由于编译器或 CPU 的优化，代码的实际执行顺序可能与编写的顺序是不同的，这在单线程的情况下是没问题的，但是一旦引入多线程，这种乱序就可能会导致严重的线程安全问题。用了 volatile 关键字就可以在一定程度上禁止这种重排序。
<a name="mo9WW"></a>
### volatile 正确用法
**boolean 标志位**<br />共享变量只有被赋值和读取，没有其他的多个复合操作（比如先读数据再修改的复合运算 i++），就可以使用 volatile 代替 synchronized 或者代替原子类，因为赋值操作是原子性操作，而 volatile 同时保证了 可见性，所以是线程安全的。<br />如下经典场景 `volatile boolean flag`，一旦 flag 发生变化，所有的线程立即可见。
```java
volatile boolean shutdownRequested;
...
public void shutdown() {
    shutdownRequested = true;
}
public void doWork() {
    while (!shutdownRequested) {
        // do stuff
    }
}
```
线程 1 执行 doWork() 的过程中，可能有另外的线程 2 调用了 shutdown，线程 1 里吗读区到修改的值并停止执行。<br />这种类型的状态标记的一个公共特性是：**通常只有一种状态转换**；`shutdownRequested` 标志从`false` 转换为`true`，然后程序停止。<br />**双重检查（单例模式）**
```java
class Singleton{
    private volatile static Singleton instance = null;
    private Singleton() {
    }
    public static Singleton getInstance() {
        if(instance==null) { // 1
            synchronized (Singleton.class) {
                if(instance==null)
                    instance = new Singleton();  //2
            }
        }
        return instance;
    }
}
```
**在双重检查锁模式中为什么需要使用 volatile 关键字？**<br />假如 Instance 类变量是没有用 volatile 关键字修饰的，会导致这样一个问题：<br />在线程执行到第 1 行的时候，代码读取到 instance 不为 null 时，instance 引用的对象有可能还没有完成初始化。<br />造成这种现象主要的原因是创建对象不是原子操作以及指令重排序。<br />第二行代码可以分解成以下几步：
```java
memory = allocate();  // 1：分配对象的内存空间
ctorInstance(memory); // 2：初始化对象
instance = memory;  // 3：设置instance指向刚分配的内存地址
```
根源在于代码中的 2 和 3 之间，可能会被重排序。例如：
```java
memory = allocate();  // 1：分配对象的内存空间
instance = memory;  // 3：设置instance指向刚分配的内存地址
// 注意，此时对象还没有被初始化！
ctorInstance(memory); // 2：初始化对象
```
这种重排序可能就会导致一个线程拿到的 instance 是非空的但是还没初始化完全。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615714-6fbce359-b1dc-46e9-9812-81b61a444aaa.webp#align=left&display=inline&height=419&originHeight=688&originWidth=1080&size=0&status=done&style=shadow&width=657)<br />**面试官可能会问，“为什么要 double-check？去掉任何一次的 check 行不行？”**<br />先来看第二次的 check，这时需要考虑这样一种情况，有两个线程同时调用 getInstance 方法，由于 singleton 是空的 ，因此两个线程都可以通过第一重的 if 判断；然后由于锁机制的存在，会有一个线程先进入同步语句，并进入第二重 if 判断 ，而另外的一个线程就会在外面等待。<br />不过，当第一个线程执行完 new Singleton() 语句后，就会退出 synchronized 保护的区域，这时如果没有第二重 if (singleton == null) 判断的话，那么第二个线程也会创建一个实例，此时就破坏了单例，这肯定是不行的。<br />而对于第一个 check 而言，如果去掉它，那么所有线程都会串行执行，效率低下，所以两个 check 都是需要保留的。
<a name="2APL4"></a>
### volatile 错误用法
volatile 不适合运用于需要保证原子性的场景，比如更新的时候需要依赖原来的值，而最典型的就是 a++ 的场景，仅靠 volatile 是不能保证 a++ 的线程安全的。代码如下所示：
```java
public class DontVolatile implements Runnable {
    volatile int a;
    public static void main(String[] args) throws InterruptedException {
        Runnable r =  new DontVolatile();
        Thread thread1 = new Thread(r);
        Thread thread2 = new Thread(r);
        thread1.start();
        thread2.start();
        thread1.join();
        thread2.join();
        System.out.println(((DontVolatile) r).a);
    }
    @Override
    public void run() {
        for (int i = 0; i < 1000; i++) {
            a++;
        }
    }
}
```
最终的结果 a < 2000。
<a name="gPkNs"></a>
## synchronized
互斥同步是常见的并发正确性保障方式。同步就好像在公司上班，厕所只有一个，现在一帮人同时想去「带薪拉屎」占用厕所，为了保证厕所同一时刻只能一个员工使用，通过排队互斥实现。<br />互斥是实现同步的一种手段，临界区、互斥量（Mutex）和信号量（Semaphore）都是主要互斥方式。互斥是因，同步是果。<br />监视器锁（Monitor 另一个名字叫管程）本质是依赖于底层的操作系统的 Mutex Lock（互斥锁）来实现的。每个对象都存在着一个 monitor 与之关联，对象与其 monitor 之间的关系有存在多种实现方式，如 monitor 可以与对象一起创建销毁或当线程试图获取对象锁时自动生成，但当一个 monitor 被某个线程持有后，它便处于锁定状态。<br />**mutex 的工作方式**<br />在 Java 虚拟机 (HotSpot) 中，Monitor 是基于 C++ 实现的，由 ObjectMonitor 实现的, 几个关键属性：

- _owner：指向持有 ObjectMonitor 对象的线程
- _WaitSet：存放处于 wait 状态的线程队列
- _EntryList：存放处于等待锁 block 状态的线程队列
- _recursions：锁的重入次数
- count：用来记录该线程获取锁的次数

![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615493-7aef2d91-da43-4ea8-bde6-501a56af38f9.webp#align=left&display=inline&height=393&originHeight=319&originWidth=533&size=0&status=done&style=shadow&width=657)<br />ObjectMonitor 中有两个队列，_WaitSet 和 _EntryList，用来保存 ObjectWaiter 对象列表( 每个等待锁的线程都会被封装成 ObjectWaiter 对象)，_owner 指向持有 ObjectMonitor 对象的线程，当多个线程同时访问一段同步代码时，首先会进入 _EntryList 集合，当线程获取到对象的 monitor 后进入 _Owner 区域并把 monitor 中的 owner 变量设置为当前线程同时 monitor 中的计数器 count 加 1。<br />若线程调用 wait() 方法，将释放当前持有的 monitor，owner 变量恢复为 null，count 自减 1，同时该线程进入 WaitSet 集合中等待被唤醒。若当前线程执行完毕也将释放 monitor(锁)并复位变量的值，以便其他线程进入获取 monitor(锁)。<br />在 Java 中，最基本的互斥同步手段就是 synchronised，经过编译之后会在同步块前后分别插入 `monitorenter`, `monitorexit` 这两个字节码指令，而这两个字节码指令都需要提供一个 reference 类型的参数来指定要锁定和解锁的对象，具体表现如下所示：

- 在普通同步方法，reference 关联和锁定的是当前方法示例对象；
- 对于静态同步方法，reference 关联和锁定的是当前类的 class 对象；
- 在同步方法块中，reference 关联和锁定的是括号里制定的对象；
<a name="Fx7YU"></a>
### Java 对象头
synchronized 用的锁也存在 Java 对象头里，在 JVM 中，对象在内存的布局分为三块区域：对象头、实例数据、对其填充。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615522-35fdf3cb-e7e5-420d-9d29-9828b4247a9d.webp#align=left&display=inline&height=506&originHeight=408&originWidth=530&size=0&status=done&style=shadow&width=657)<br />对象头

- 对象头：MarkWord 和 metadata，也就是图中对象标记和元数据指针；
- 实例对象：存放类的属性数据，包括父类的属性信息，如果是数组的实例部分还包括数组的长度，这部分内存按 4 字节对齐；
- 填充数据：由于虚拟机要求对象起始地址必须是 8 字节的整数倍。填充数据不是必须存在的，仅仅是为了字节对齐；

对象头是 synchronized 实现的关键，使用的锁对象是存储在 Java 对象头里的，jvm 中采用 2 个字宽（一个字宽代表 4 个字节，一个字节 8bit）来存储对象头(如果对象是数组则会分配 3 个字宽，多出来的 1 个字宽记录的是数组长度)。其主要结构是由 **Mark Word 和 Class Metadata Address** 组成。<br />**Mark word 记录了对象和锁有关的信息，当某个对象被 synchronized 关键字当成同步锁时，那么围绕这个锁的一系列操作都和 Mark word 有关系。**

| 虚拟机位数 | 对象结构 | 说明 |
| :---: | :---: | :---: |
| 32/64bit | Mark Word | 存储对象的 hashCode、锁信息或分代年龄或 GC 标志等信息 |
| 32/64bit | Class Metadata Address | 类型指针指向对象的类元数据，JVM 通过这个指针确定该对象是哪个类的实例。 |
| 32/64bit | Array length | 数组的长度（如果当前对象是数组） |

其中 Mark Word 在默认情况下存储着对象的 HashCode、分代年龄、锁标记位等。Mark Word 在不同的锁状态下存储的内容不同，在 32 位 JVM 中默认状态为下：

| 锁状态 | 25 bit | 4 bit | 1 bit 是否是偏向锁 | 2 bit 锁标志位 |
| :---: | :---: | :---: | :---: | :---: |
| 无锁 | 对象 HashCode | 对象分代年龄 | 0 | 01 |

在运行过程中，Mark Word 存储的数据会随着锁标志位的变化而变化，可能出现如下 4 种数据：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604365615730-8afc9cba-8f29-4f68-ab48-9ed701af6be7.webp#align=left&display=inline&height=134&originHeight=221&originWidth=1080&size=0&status=done&style=shadow&width=657)<br />锁标志位的表示意义：

1. 锁标识 lock=00 表示轻量级锁
2. 锁标识 lock=10 表示重量级锁
3. 偏向锁标识 biased_lock=1 表示偏向锁
4. 偏向锁标识 biased_lock=0 且锁标识=01 表示无锁状态

**到目前为止，再总结一下前面的内容，synchronized(lock) 中的 lock 可以用 Java 中任何一个对象来表示，而锁标识的存储实际上就是在 lock 这个对象中的对象头内。**<br />Monitor（监视器锁）本质是依赖于底层的操作系统的 Mutex Lock（互斥锁）来实现的。Mutex Lock 的切换需要从用户态转换到核心态中，因此状态转换需要耗费很多的处理器时间。所以 synchronized 是 Java 语言中的一个重量级操作。<br />**为什么任意一个 Java 对象都能成为锁对象呢？**<br />Java 中的每个对象都派生自 Object 类，而每个 Java Object 在 JVM 内部都有一个 native 的 C++对象 oop/oopDesc 进行对应。其次，线程在获取锁的时候，实际上就是获得一个监视器对象(monitor) ,monitor 可以认为是一个同步对象，所有的 Java 对象是天生携带 monitor。<br />多个线程访问同步代码块时，相当于去争抢对象监视器修改对象中的锁标识, ObjectMonitor 这个对象和线程争抢锁的逻辑有密切的关系。
<a name="Km9rM"></a>
## 总结讨论
<a name="cb53c645"></a>
### JMM 总结
JVM 内存结构和 Java 虚拟机的运行时区域有关；<br />**Java 内存模型和 Java 的并发编程有关。JMM 是并发编程的基础，它屏蔽了硬件和系统造成的内存访问差异，保证了 一致性、原子性、并禁止指令重排保证了安全访问。通过总线嗅探机制使得缓存数据失效， 保证 volatile 内存可见性。**<br />JMM 是一个抽象概念，由于 CPU 多核多级缓存、为了优化代码会发生指令重排的原因，JMM 为了屏蔽细节，定义了一套规范，保证最终的并发安全。它抽象出了工作内存于主内存的概念，并且**通过八个原子操作以及内存屏障保证了原子性、内存可见性、防止指令重排**，使得 volatile 能保证内存可见性并防止指令重排、synchronised 保证了内存可见性、原子性、防止指令重排导致的线程安全问题，JMM 是并发编程的基础。
<a name="DApzP"></a>
### synchronized 原理
提到了锁的几个概念，偏向锁、轻量级锁、重量级锁。在 JDK1.6 之前，synchronized 是一个重量级锁，性能比较差。从 JDK1.6 开始，为了减少获得锁和释放锁带来的性能消耗，synchronized 进行了优化，引入了偏向锁和轻量级锁的概念。<br />所以从 JDK1.6 开始，锁一共会有四种状态，锁的状态根据竞争激烈程度从低到高分别是: **无锁状态->偏向锁状态->轻量级锁状态->重量级锁状态**。这几个状态会随着锁竞争的情况逐步升级。为了提高获得锁和释放锁的效率，锁可以升级但是不能降级。<br />同时为了提升性能，还带来了锁消除、锁粗化、自旋锁和自适应自旋锁…...
