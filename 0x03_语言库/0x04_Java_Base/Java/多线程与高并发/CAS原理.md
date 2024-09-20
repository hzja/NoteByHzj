Java CAS<br />在并发编程中都知道i++操作是非线程安全的，这是因为i++操作不是原子操作。<br />如何保证原子性呢？常用的方法就是加锁。在Java语言中可以使用`Synchronized`和CAS实现加锁效果。<br />`Synchronized`是悲观锁，线程开始执行第一步就是获取锁，一旦获得锁，其他的线程进入后就会阻塞等待锁。如果不好理解，举个生活中的例子：一个人进入厕所后首先把门锁上（获取锁），然后开始上厕所，这个时候有其他人来了只能在外面等（阻塞），就算再急也没用。上完厕所完事后把门打开（解锁），其他人就可以进入了。<br />CAS是乐观锁，线程执行的时候不会加锁，假设没有冲突去完成某项操作，如果因为冲突失败了就重试，最后直到成功为止。
<a name="VpaF6"></a>
## 什么是 CAS？
CAS（Compare-And-Swap）是比较并交换的意思，它是一条 CPU 并发原语，用于判断内存中某个值是否为预期值，如果是则更改为新的值，这个过程是原子的。下面用一个小示例解释一下。<br />CAS机制当中使用了3个基本操作数：内存地址V，旧的预期值A，计算后要修改后的新值B。<br />（1）初始状态：在内存地址V中存储着变量值为 1。<br />![2021-05-13-23-22-09-850067.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620919505482-f367a036-e8fd-4f3d-b6cd-0974e966cbd9.png#clientId=udf5ecb0c-c725-4&from=ui&id=ud47345c3&originHeight=276&originWidth=1080&originalType=binary&size=12891&status=done&style=shadow&taskId=ua3ea8892-9eae-41d3-a2b0-0e175e7359b)<br />（2）线程1想要把内存地址为 V 的变量值增加1。这个时候对线程1来说，旧的预期值A=1，要修改的新值B=2。<br />![2021-05-13-23-22-09-913897.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620919511271-19e4b59e-e8e3-450a-b14a-f6a6c6af8aef.png#clientId=udf5ecb0c-c725-4&from=ui&id=u2637e6f4&originHeight=398&originWidth=1080&originalType=binary&size=22269&status=done&style=shadow&taskId=u87f69eb3-1b0d-4ef0-aa8b-f1b75ebedf5)<br />（3）在线程1要提交更新之前，线程2捷足先登了，已经把内存地址V中的变量值率先更新成了2。<br />![2021-05-13-23-22-09-987701.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620919521035-983afed3-bd48-4572-ab9b-06421d58ce8b.png#clientId=udf5ecb0c-c725-4&from=ui&id=u16bb3d99&originHeight=371&originWidth=1080&originalType=binary&size=29658&status=done&style=shadow&taskId=udc80631c-c3af-49ff-abc8-587d4a7334b)<br />（4）线程1开始提交更新，首先将预期值A和内存地址V的实际值比较（Compare），发现A不等于V的实际值，提交失败。<br />![2021-05-13-23-22-10-190158.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620919530508-5c1cbb28-7c52-4328-a506-58effbf4be50.png#clientId=udf5ecb0c-c725-4&from=ui&id=ude9531fd&originHeight=382&originWidth=1080&originalType=binary&size=26465&status=done&style=shadow&taskId=u99d7e62b-dc51-44e2-a5fd-e3c9db311b8)<br />（5）线程1重新获取内存地址 V 的当前值，并重新计算想要修改的新值。此时对线程1来说，A=2，B=3。这个重新尝试的过程被称为自旋。如果多次失败会有多次自旋。<br />![2021-05-13-23-22-10-270941.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620919539050-2aaf3236-502a-428c-b51c-4b686f410306.png#clientId=udf5ecb0c-c725-4&from=ui&id=u2230011e&originHeight=374&originWidth=1080&originalType=binary&size=24745&status=done&style=shadow&taskId=u1e3f2a43-70ad-4e9a-b5e7-8c7e71b67b0)<br />（6）线程 1 再次提交更新，这一次没有其他线程改变地址 V 的值。线程1进行Compare，发现预期值 A 和内存地址 V的实际值是相等的，进行 Swap 操作，将内存地址 V 的实际值修改为 B。<br />![2021-05-13-23-22-13-444457.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620919548158-c807fce6-9aa9-448a-b51e-f3db6f929f03.png#clientId=udf5ecb0c-c725-4&from=ui&id=uc3a84ec5&originHeight=385&originWidth=1080&originalType=binary&size=27850&status=done&style=shadow&taskId=ub2622174-36c9-4e3c-800c-471eaf0723f)<br />总结：更新一个变量的时候，只有当变量的预期值 A 和内存地址 V 中的实际值相同时，才会将内存地址 V 对应的值修改为 B，这整个操作就是CAS。
<a name="ZUOFc"></a>
## CAS 基本原理
CAS 主要包括两个操作：Compare和Swap，有人可能要问了：两个操作能保证是原子性吗？可以的。<br />CAS 是一种系统原语，原语属于操作系统用语，原语由若干指令组成，用于完成某个功能的一个过程，并且原语的执行必须是连续的，在执行过程中不允许被中断，也就是说 CAS 是一条 CPU 的原子指令，由操作系统硬件来保证。<br />在 Intel 的 CPU 中，使用 cmpxchg 指令。<br />回到 Java 语言，JDK 是在 1.5 版本后才引入 CAS 操作，在sun.misc.Unsafe这个类中定义了 CAS 相关的方法。
```java
public final native boolean compareAndSwapObject(Object o, long offset, Object expected, Object x);

public final native boolean compareAndSwapInt(Object o, long offset, int expected, int x);

public final native boolean compareAndSwapLong(Object o, long offset, long expected, long x);
```
可以看到方法被声明为`native`，如果对 C++ 比较熟悉可以自行下载 OpenJDK 的源码查看 unsafe.cpp，这里不再展开分析。
<a name="LUbAH"></a>
## CAS 在 Java 语言中的应用
在 Java 编程中通常不会直接使用到 CAS，都是通过 JDK 封装好的并发工具类来间接使用的，这些并发工具类都在java.util.concurrent包中。<br />J.U.C 是java.util.concurrent的简称，也就是大家常说的 Java 并发编程工具包，面试常考，非常非常重要。<br />目前 CAS 在 JDK 中主要应用在 J.U.C 包下的 `Atomic` 相关类中。<br />![2021-05-13-23-22-13-543224.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620919560670-335d249d-8756-4795-a0c6-390ee695d5b7.png#clientId=udf5ecb0c-c725-4&from=ui&id=ue66f875a&originHeight=602&originWidth=582&originalType=binary&size=72671&status=done&style=none&taskId=ua6d68ed9-9763-4339-ba74-129dcb79866)<br />比如说 `AtomicInteger` 类就可以解决 `i++` 非原子性问题，通过查看源码可以发现主要是靠 `volatile` 关键字和 CAS 操作来实现。
<a name="rjayu"></a>
## CAS 的问题
CAS 不是万能的，也有很多问题。
<a name="EoaqK"></a>
### 典型 ABA 问题
ABA 是 CAS 操作的一个经典问题，假设有一个变量初始值为 A，修改为 B，然后又修改为 A，这个变量实际被修改过了，但是 CAS 操作可能无法感知到。<br />如果是整形还好，不会影响最终结果，但如果是对象的引用类型包含了多个变量，引用没有变实际上包含的变量已经被修改，这就会造成大问题。<br />如何解决？思路其实很简单，在变量前加版本号，每次变量更新了就把版本号加一，结果如下：<br />![2021-05-13-23-22-13-603035.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620919604609-ef3315b6-33e5-47e4-ba18-626680b59f25.png#clientId=uada191f2-018e-4&from=ui&id=u2bfbb03d&originHeight=424&originWidth=1080&originalType=binary&size=31132&status=done&style=shadow&taskId=u3d499b1b-f08f-45ae-b728-f06578609f5)<br />最终结果都是 A 但是版本号改变了。<br />从 JDK 1.5 开始提供了`AtomicStampedReference`类，这个类的`compareAndSwap`方法首先检查当前引用是否等于预期引用，并且当前标志是否等于预期标志，如果全部相等，则以原子方式将该引用和该标志的值设置为给定的更新值。
<a name="q2x1y"></a>
### 自旋开销问题
CAS 出现冲突后就会开始自旋操作，如果资源竞争非常激烈，自旋长时间不能成功就会给 CPU 带来非常大的开销。<br />解决方案：可以考虑限制自旋的次数，避免过度消耗 CPU；另外还可以考虑延迟执行。
<a name="IoBLb"></a>
### 只能保证单个变量的原子性
当对一个共享变量执行操作时，可以使用 CAS 来保证原子性，但是如果要对多个共享变量进行操作时，CAS 是无法保证原子性的，比如需要将 i 和 j 同时加 1：<br />`i++;`<br />`j++;`<br />这个时候可以使用 `synchronized` 进行加锁，有没有其他办法呢？有，将多个变量操作合成一个变量操作。从 JDK1.5 开始提供了`AtomicReference`类来保证引用对象之间的原子性，可以把多个变量放在一个对象里来进行CAS操作。
<a name="rKkNG"></a>
## 总结
CAS 是 Compare And Swap，是一条 CPU 原语，由操作系统保证原子性。<br />Java语言从 JDK1.5 版本开始引入 CAS ， 并且是 Java 并发编程J.U.C 包的基石，应用非常广泛。<br />当然 CAS 也不是万能的，也有很多问题：典型 ABA 问题、自旋开销问题、只能保证单个变量的原子性。
