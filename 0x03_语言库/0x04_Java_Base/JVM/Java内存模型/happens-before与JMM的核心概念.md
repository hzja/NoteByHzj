JMM
<a name="XHby6"></a>
## 什么是内存模型
为了保证共享内存的正确性也就是可见性、有序性、原子性这些，内存模型定义了共享内存里多线程读写操作的规范。通过了这些规则来对内存的读写操作具有规范性，保证指令的正确性。它是与处理器、缓存、并发、编译器是有关的。它解决了CPU的多级缓存，处理器优化、指令重排导致的内存访问问题的出现。保证了在并发的场景下一致性，原子性和有序性。
<a name="PZJ0v"></a>
## 什么是Java内存模型
Java的程序是需要运行在Java的虚拟机上的，Java的内存模型就是符合内存模型规范的，屏蔽了各种硬件和操作系统导致访问差异的，保证了Java程序在各个平台下对内存的访问都能保证了效果一致的机制以及规范。
<a name="mYqJM"></a>
#### 可见性问题：
在现在的多核时代，每一颗CPU都有自己的缓存，这个时候CPU缓存跟内存数据的一致性就没有那么简单的解决，当多个线程在不同的CPU上执行的时候，这一些线程的操作是不同CPU缓存的。如图所示，线程A是CPU1的缓存，线程B是CPU2缓存，如果这个时候线程A对变量A进行操作的话，那么对于线程B就是不具备可见性了。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643605641157-0e1bc31f-b196-4ad7-bb44-88b9ac6f984e.webp#clientId=u847b2a33-d8b3-4&from=paste&id=ub15b4fda&originHeight=538&originWidth=618&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u73821c81-3477-44cf-8968-45f45e80f13&title=)
<a name="hn6vg"></a>
#### 原子性问题
很早的时候，操作系统是基于进程来对CPU进行调度的，不同的进程之间是不共享内存空间的，所以进程要想做任务切换的话就要切换内存的映射地址了，而进程创建所有的线程都是共享，所以的话使用线程来做任务切换的话成本就很低很低了。操作系统做任务切换，可以在任何一条CPU指令执行完成的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1643605641491-2eed933a-38d9-4301-8d12-e428896d258d.png#clientId=u847b2a33-d8b3-4&from=paste&id=u6f9fa714&originHeight=657&originWidth=649&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u441a7d28-f7f5-4e4d-991b-f8acbcec0df&title=)<br />假设count=0，线程A在指令1执行完之后做线程切换，线程A和线程B按照上图的序列执行，那么两个线程都执行了count+1=1的操作，没有得到期望值2，而是1。
<a name="SuKE8"></a>
#### 有序性问题：
编译器为了对性能有所优化，有时候会改变了程序语句的先后顺序，比如程序“a=1，b=2”经过编译器优化后可能变成b=2,a=1，虽然这样不影响程序的最终执行结果，但是编译器以及解释器的优化可能会导致意想不到的bug（双重检查创建单例对象案例就是一个经典例子）。<br />Java从源代码到最后实际执行的指令序列呢，会分别的经历下面的三种重排序：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643605641448-e75b4bfa-1861-4717-8ef4-2a6fafc119d9.webp#clientId=u847b2a33-d8b3-4&from=paste&id=u673a9fa6&originHeight=407&originWidth=749&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud4ddf9b6-b438-4d96-bfe8-38dc5feee70&title=)<br />这些重排序可能会让多线程的程序出现内存可见性问题，对于编译器的话，JMM的编译器重排序的规则会禁止掉特定类型的编译器重排序。
<a name="CiOmy"></a>
## JMM的设计
要学习happens-before的时候需要介绍一下JMM。JMM在设计的时候定义了两种策略，第一种对于会改变程序执行结果的进行重排序，当然JMM要求编译器和处理器必须禁止掉这种重排序。第二种就是对于不会改变程序的执行结果重排序，JMM对于编译器和处理器不会做要求（JMM也允许使用这种重排序）。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643605641463-e8f9dbb3-deb6-4473-b138-e912966bc3bd.webp#clientId=u847b2a33-d8b3-4&from=paste&id=ueacd3a6a&originHeight=650&originWidth=819&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u144f9e98-67de-4068-9ebb-f69b96402fe&title=)<br />从上面的图可以看出来，JMM向可以提供了足够强的内存可见性的保证，在不会影响程序的执行结果的情况，有些可见性保证并一定存在的。这就引出来另一方面，JMM为了满足编译器和处理器的约束尽量可能的少，它的规则就是只要不改变程序的执行结果的话，编译器和处理器想怎么优化就怎么优化。
<a name="cqhCB"></a>
## 先行发生原则 happens-before
如何来对happens-before的规则了解呢，happens-before表达的并不是说前面一个操作发生在后面一个操作的前面，它表达的是前一个操作的结果读后续操作时可见的。因为JMM是为程序员提供的视角是顺序执行的，满足了一个happens-before于另一个操作，这样的话第一个操作的执行结果将对第二个的执行结果是可见的，并且的话第一个的执行的顺序在第二个顺序的前面。<br />1、**「程序次序规则：」** 同一个线程内，按照代码出现的顺序，前面的代码先行于后面的代码，准确的说是控制流顺序，因为要考虑到分支和循环结构。<br />2、**「管程锁定规则：」** 一个unlock操作先行发生于后面（时间上）对同一个锁的`lock`操作。<br />3、**「volatile变量规则：」** 对一个volatile变量的写操作先行发生于后面（时间上）对这个变量的读操作<br />4、**「线程启动规则：」** Thread的`start()`方法先行发生于这个线程的每一个操作。<br />5、**「线程终止规则：」** 线程的所有操作都先行于此线程的终止检测。可以通过`Thread.join()`方法结束、Thread.isAlive( )的返回值等手段检测线程的终止。<br />6、**「线程中断规则：」** 对线程`interrupt()`方法的调用先行发生于被中断线程的代码检测到中断事件的发生，可以通过Thread.interrupt( )方法检测线程是否中断。<br />7、**「对象终结规则：」** 一个对象的初始化完成先行于发生它的`finalize()`方法的开始。<br />8、**「传递性：」** 如果操作A先行于操作B，操作B先行于操作C，那么操作A先行于操作C。
<a name="iVzG5"></a>
#### **「详细看看上面的八条规则」**
**「程序的次序规则：」** 在一段代码里的单线程里的执行结果是有序的，这里注意看是执行结果，因为的话虚拟机与处理器会对指令来进行重排序。虽然重排序了，但是呢不会影响程序的执行结果，所以的话程序的执行结果与顺序执行出来的结果是一致的。从而这个规则只对单线程有效，在多线程的环境下没有办法保证正确性。<br />**「锁定规则：」** 这个规则的话比较简单理解，通俗来说就是无论是在单线程还是多线程的环境下，一个锁处于被锁定的状态的话，那么就必须要执行`unlock`的操作后面才能进行lock操作。<br />**「valatile变量规则：」** 这一条规则它说明了`valatile`是保证了线程的可见性。<br />**「传递规则：」** 该规则体现了happens-before原则具有传递性，就是A happens-before B, B happens-before C，那么的话A happens-before C。<br />**「线程启动规则：」** 假如A线程在执行的过程里，通过ThreadB来启动 线程B，那么就是线程A对共享变量的修改在后面的线程B开始执行就确保了对线程B是可见的。<br />**「线程中终结规则：」** 假如A线程在执行的过程里，通过`join`等待B线程终止的话，那么B线程就是在终止前对共享变量的修改在A线程等待返回后可见的。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643605641561-dcaa6a97-995d-4700-8919-5341467afdaf.webp#clientId=u847b2a33-d8b3-4&from=paste&id=ud128aa76&originHeight=766&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9ff62d20-3a7e-4580-8fe0-a73f44859ee&title=)
