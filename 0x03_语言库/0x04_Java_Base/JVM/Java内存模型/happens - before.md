JVM JMM<br />happens - before 不像是什么 Java 并发工具类能够浅显易懂，容易上手。happens - before 重在理解。<br />happens - before 和 JMM 也就是 Java 内存模型有关，所以需要先从 JMM 入手，才能更好的理解 happens - before 原则。
<a name="a6hg0"></a>
## JMM 的设计
JMM 是 JVM 的基础，因为 JVM 中的堆区、方法区、栈区都是建立在 JMM 基础上的，可能还是不理解这是怎么回事，没关系，先来看一下 JMM 的模型。<br />![2021-08-31-15-33-02-079036.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630395384841-92ce05ca-341a-41d2-8337-1a70fb2be33e.jpeg#clientId=ud12f4382-6bb3-4&from=ui&id=u9e69626c&originHeight=411&originWidth=1080&originalType=binary&ratio=1&size=28805&status=done&style=shadow&taskId=u8e307558-3d11-40df-82d3-6fa62de8347)<br />JVM 的划分想必大家应该了然于胸，这里就不再赘述了，主要说一下 JVM 各个区域在 JMM 中的分布。JVM 中的栈区包括局部变量和操作数栈，局部变量在各个线程之间都是独立存在的，即各个线程之间不会互相干扰，变量的值只会受到当前线程的影响，这在《Java 并发编程实战》中被称为**线程封闭**。<br />然而，线程之间的共享变量却存储在主内存（Main Memory）中，共享变量是 JVM 堆区的重要组成部分。<br />那么，共享变量是如何被影响的呢？<br />这里其实有操作系统层面解决进程通信的一种方式：共享内存，主内存其实就是共享内存。<br />之所以说共享变量能够被影响，是由于每个 Java 线程在执行代码的过程中，都会把主内存中的共享变量 load 一份副本到工作内存中。<br />![2021-08-31-15-33-02-213036.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630395384741-159c1f3a-f314-4aca-8d5b-a47b27467b04.jpeg#clientId=ud12f4382-6bb3-4&from=ui&id=cUKxI&originHeight=594&originWidth=1080&originalType=binary&ratio=1&size=44064&status=done&style=shadow&taskId=u7743eece-1715-4010-9920-28a141c2822)<br />当每个 Java 线程修改工作内存中的共享变量副本后，会再把共享变量 store 到主存中，由于不同线程对共享变量的修改不一样，而且每个线程对共享变量的修改彼此不可见，所以最后覆盖内存中共享变量的值的时候可能会出现重复覆盖的现象，这也是共享变量不安全的因素。<br />![2021-08-31-15-33-02-298034.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630395384740-51859a95-cf28-4979-936b-9a1ed5369948.jpeg#clientId=ud12f4382-6bb3-4&from=ui&id=l2HHM&originHeight=444&originWidth=1080&originalType=binary&ratio=1&size=40803&status=done&style=shadow&taskId=ue4876afd-96cf-4871-9591-9d5cd39dc34)<br />由于 JMM 的这种设计，导致出现了经常说的可见性和有序性问题。<br />关于可见性和 Java 并发编程中如何解决可见性问题，实际上，在 volatile 解决可见性问题的同时，也是遵循了 happens - before 原则的。
<a name="Wwiqa"></a>
## happens - before 原则
JSR-133 使用 happens - before 原则来指定两个操作之间的执行顺序。这两个操作可以在同一个线程内，也可以在不同线程之间。同一个线程内是可以使用 as-if-serial 语义来保证可见性的，所以 happens - before 原则更多的是用来解决不同线程之间的可见性。<br />JSR - 133 对 happens - before 关系有下面这几条定义，分别来解释下。
<a name="EI7DC"></a>
### 程序顺序规则
> Each action in a thread happens-before every subsequent action in that thread.

每个线程在执行指令的过程中都相当于是一条顺序执行流程：取指令，执行，指向下一条指令，取指令，执行。<br />![2021-08-31-15-33-02-396053.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630395410968-b946bb6f-85a9-432e-9bc7-255bf546e22f.png#clientId=ud12f4382-6bb3-4&from=ui&id=ue023734c&originHeight=440&originWidth=552&originalType=binary&ratio=1&size=27100&status=done&style=shadow&taskId=u4e3fe6d5-d1c6-4eac-941d-81abc5e2f2a)<br />而程序顺序规则说的就是在同一个顺序执行流中，会按照程序代码的编写顺序执行代码，编写在前面的代码操作要 happens - before 编写在后面的代码操作。
:::tips
这里需要特别注意⚠️的一点就是：这些操作的顺序都是对于同一个线程来说的。
:::
<a name="KEIyY"></a>
### monitor 规则
> An unlock on a monitor happens-before every subsequent lock on that monitor.

这是一条对 monitor 监视器的规则，主要是面向 lock 和 unlock 也就是加锁和解锁来说明的。这条规则是对于同一个 monitor 来说，这个 monitor 的解锁（unlock）要 happens - before 后面对这个监视器的加锁（lock）。<br />![2021-08-31-15-33-02-477036.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630395410922-21195a76-0f6e-4205-957e-f5a8dfb45aa7.jpeg#clientId=ud12f4382-6bb3-4&from=ui&id=Ti7qg&originHeight=241&originWidth=1080&originalType=binary&ratio=1&size=17592&status=done&style=shadow&taskId=u97f51063-f8b2-4212-8325-cdf51e71af1)<br />比如下面这段代码
```java
class monitorLock {
    private int value = 0;
  
    public synchronized int getValue() {
        return value;
    }
    
    public synchronized void setValue(int value) {
        this.value = value;
    }
}
```
在这段代码中，`getValue` 和 `setValue` 这两个方法使用了同一个 `monitor` 锁，假设 A 线程正在执行 `getValue` 方法，B 线程正在执行 `setValue` 方法。`monitor` 的原则会规定线程 B 对 value 值的修改，能够直接对线程 A 可见。如果 `getValue` 和 `setValue` 没有 `synchronized` 关键字进行修饰的话，则不能保证线程 B 对 value 值的修改，能够对线程 A 可见。
:::tips
monitor 的规则对于 `synchronized` 语义和 `ReentrantLock` 中的 `lock` 和 `unlock` 的语义是一样的。
:::
<a name="Bc7Oq"></a>
### `volatile` 规则
> A write to a volatile ﬁeld happens-before every subsequent read of that volatile.

这是一条对 `volatile` 的规则，它说的是对一个 `volatile` 变量的写操作 happens - before 后续任意对这个变量的读操作。<br />嗯，这条规则其实就是在说 `volatile` 语义的规则，因为对 `volatile` 的写和读之间会增加 memory barrier ，也就是内存屏障。<br />内存屏障也叫做栅栏，它是一种底层原语。它使得 CPU 或编译器在对内存进行操作的时候, 要严格按照一定的顺序来执行, 也就是说在 memory barrier 之前的指令和 memory barrier 之后的指令不会由于系统优化等原因而导致乱序。
<a name="uaw3Q"></a>
### 线程 start 规则
> A call to `start()` on a thread happens-before any actions in the started thread.

这条规则也是适用于同一个线程，对于相同线程来说，调用线程 `start` 方法之前的操作都 happens - before start 方法之后的任意操作。
:::tips
这条原则也可以这样去理解：调用 `start` 方法时，会将 `start` 方法之前所有操作的结果同步到主内存中，新线程创建好后，需要从主内存获取数据。这样在 `start` 方法调用之前的所有操作结果对于新创建的线程都是可见的。
:::
画幅图。<br />![2021-08-31-15-33-02-565035.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630395411056-65406870-398b-4746-82bc-8b1fa98a44c5.jpeg#clientId=ud12f4382-6bb3-4&from=ui&id=UeVAL&originHeight=474&originWidth=723&originalType=binary&ratio=1&size=24621&status=done&style=shadow&taskId=u762e0ced-ac4a-4a1e-af1b-6c901a251d0)<br />可以看到，线程 A 在执行 `ThreadB.start` 方法之前会对共享变量进行修改，修改之后的共享变量会直接刷新到内存中，然后线程 A 执行 `ThreadB.start` 方法，紧接着线程 B 会从内存中读取共享变量。
<a name="oCx0i"></a>
### 线程 join 规则
> All actions in a thread happen-before any other thread successfully returns from a join() on that thread.

这条规则是对多条线程来说的：如果线程 A 执行操作 `ThreadB.join()` 并成功返回，那么线程 B 中的任意操作都 happens - before 于线程 A 从 `ThreadB.join` 操作成功返回。<br />假设有两个线程 s、t，在线程 s 中调用 `t.join()` 方法。则线程 s 会被挂起，等待 t 线程运行结束才能恢复执行。当`t.join()` 成功返回时，s 线程就知道 t 线程已经结束了。所以根据本条原则，在 t 线程中对共享变量的修改，对 s 线程都是可见的。类似的还有 Thread.isAlive 方法也可以检测到一个线程是否结束。
<a name="PINC2"></a>
### 线程传递规则
> If an action a happens-before an action b, and b happens before an action c, then a happensbefore c.

这是 happens - before 的最后一个规则，它主要说的是操作之间的传递性，也就是说，如果 A happens-before B，且 B happens-before C，那么 A happens-before C。<br />线程传递规则不像上面其他规则有单独的用法，它主要是和 `volatile` 规则、`start` 规则和 `join` 规则一起使用。
<a name="eLynu"></a>
#### 和 `volatile` 规则一起使用
比如现在有四个操作：普通写、`volatile` 写、`volatile` 读、普通读，线程 A 执行普通写和 `volatile` 写，线程B 执行`volatile` 读和普通读，根据程序的顺序性可知，普通写 happens - before volatile 写，`volatile` 读 happens - before 普通读，根据 `volatile` 规则可知，线程的 `volatile` 写 happens - before volatile 读和普通读，然后根据线程传递规则可知，普通写也 happens - before 普通读。<br />![2021-08-31-15-33-02-664040.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630395437686-93b4f712-82e9-447f-8d03-4f6f6b077436.png#clientId=ud12f4382-6bb3-4&from=ui&id=uab8dd9a5&originHeight=471&originWidth=797&originalType=binary&ratio=1&size=75775&status=done&style=shadow&taskId=u5cb4f1aa-10f8-47bb-9673-c0c64c703bd)
<a name="Bcsvj"></a>
#### 和 `start()` 规则一起使用
和 start 规则一起使用，其实在上面描述 start 规则的时候已经描述了，只不过上面那幅图少画了一条线，也就是 `ThreadB.start` happens - before 线程 B 读共享变量，由于 `ThreadB.start` 要 happens - before 线程 B 开始执行，然而从程序定义的顺序来说，线程 B 的执行 happens - before 线程 B 读共享变量，所以根据线程传递规则来说，线程 A 修改共享变量 happens - before 线程 B 读共享变量，如下图所示。<br />![2021-08-31-15-33-02-780040.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630395437707-ebcae199-4ea4-4014-869a-9cc16dff84d4.png#clientId=ud12f4382-6bb3-4&from=ui&id=UfR7I&originHeight=475&originWidth=780&originalType=binary&ratio=1&size=97710&status=done&style=shadow&taskId=u08ff2ab2-002a-4e5d-91d1-88dedb62ea0)
<a name="p1Lys"></a>
#### 和 `join()` 规则一起使用
假设线程 A 在执行的过程中，通过执行 `ThreadB.join` 来等待线程 B 终止。同时，假设线程 B 在终止之前修改了一些共享变量，线程 A 从 `ThreadB.join` 返回后会读这些共享变量。<br />![2021-08-31-15-33-02-874036.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630395437686-9c7e2108-b37d-4a3f-86d0-3fbc285910aa.png#clientId=ud12f4382-6bb3-4&from=ui&id=SIRXA&originHeight=550&originWidth=762&originalType=binary&ratio=1&size=73364&status=done&style=shadow&taskId=uab749360-3266-495e-8940-60c976c070f)<br />在上图中，2 happens - before 4 由 join 规则来产生，4 happens - before 5 是程序顺序规则，所以根据线程传递规则，将会有 2 happens - before 5，这也意味着，线程 A 执行操作 `ThreadB.join` 并成功返回后，线程 B 中的任意操作将对线程 A 可见。
