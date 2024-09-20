Java synchronized
<a name="zL3GI"></a>
## 什么是`synchronized`关键字？
在多线程的环境下，多个线程同时访问共享资源会出现一些问题，而`synchronized`关键字则是用来保证线程同步的。
<a name="TIoqm"></a>
## Java内存的可见性问题
在了解`synchronized`关键字的底层原理前，需要先简单了解下Java的内存模型，看看`synchronized`关键字是如何起作用的。<br />![2021-05-31-18-04-40-020216.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622469987289-820c57f2-d538-40e4-80e1-393ff9f8f1ee.png#clientId=u51719279-841e-4&from=ui&id=u1ce8d559&originHeight=381&originWidth=291&originalType=binary&size=12756&status=done&style=shadow&taskId=ub223e4ac-1465-45c2-8cf9-72bf981a594)<br />这里的本地内存并不是真实存在的，只是Java内存模型的一个抽象概念，它包含了控制器、运算器、缓存等。同时Java内存模型规定，线程对共享变量的操作必须在自己的本地内存中进行，不能直接在主内存中操作共享变量。这种内存模型会出现什么问题呢？，

1. 线程A获取到共享变量X的值，此时本地内存A中没有X的值，所以加载主内存中的X值并缓存到本地内存A中，线程A修改X的值为1，并将X的值刷新到主内存中，这时主内存及本地内存中的X的值都为1。
2. 线程B需要获取共享变量X的值，此时本地内存B中没有X的值，加载主内存中的X值并缓存到本地内存B中，此时X的值为1。线程B修改X的值为2，并刷新到主内存中，此时主内存及本地内存B中的X值为2，本地内存A中的X值为1。
3. 线程A再次获取共享变量X的值，此时本地内存中存在X的值，所以直接从本地内存中A获取到了X为1的值，但此时主内存中X的值为2，到此出现了所谓内存不可见的问题。

该问题Java内存模型是通过`synchronized`关键字和`volatile`关键字就可以解决，那么`synchronized`关键字是如何解决的呢，其实进入`synchronized`块就是把在`synchronized`块内使用到的变量从线程的本地内存中擦除，这样在`synchronized`块中再次使用到该变量就不能从本地内存中获取了，需要从主内存中获取，解决了内存不可见问题。
<a name="tin8P"></a>
## `synchronized`关键字三大特性是什么？
面试时经常拿`synchronized`关键字和`volatile`关键字的特性进行对比，`synchronized`关键字可以保证并发编程的三大特性：原子性、可见性、有序性，而`volatile`关键字只能保证可见性和有序性，不能保证原子性，也称为是轻量级的`synchronized`。

- 原子性：一个或多个操作要么全部执行成功，要么全部执行失败。`synchronized`关键字可以保证只有一个线程拿到锁，访问共享资源。
- 可见性：当一个线程对共享变量进行修改后，其他线程可以立刻看到。执行`synchronized`时，会对应执行 `lock`、`unlock`原子操作，保证可见性。
- 有序性：程序的执行顺序会按照代码的先后顺序执行。
<a name="awM58"></a>
## `synchronized`关键字可以实现什么类型的锁？

- 悲观锁：`synchronized`关键字实现的是悲观锁，每次访问共享资源时都会上锁。
- 非公平锁：`synchronized`关键字实现的是非公平锁，即线程获取锁的顺序并不一定是按照线程阻塞的顺序。
- 可重入锁：`synchronized`关键字实现的是可重入锁，即已经获取锁的线程可以再次获取锁。
- 独占锁或者排他锁：`synchronized`关键字实现的是独占锁，即该锁只能被一个线程所持有，其他线程均被阻塞。
<a name="Em3yz"></a>
## `synchronized`关键字的使用方式
`synchronized`主要有三种使用方式：修饰普通同步方法、修饰静态同步方法、修饰同步方法块。
<a name="NjOmB"></a>
### 修饰普通同步方法（实例方法）
```java
class syncTest implements Runnable {

    private static int i = 0;   //共享资源

    private synchronized void add() {
        i++;
    }

    @Override
    public void run() {
        for (int j = 0; j < 10000; j++) {
            add();
        }
    }

    public static void main(String[] args) throws Exception {

        syncTest syncTest = new syncTest();

        Thread t1 = new Thread(syncTest);
        Thread t2 = new Thread(syncTest);

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        System.out.println(i);
    }
}
```
这是一个非常经典的例子，多个线程操作`i++`会出现线程不安全问题，这段代码的结果很容易得到
```java
20000
```
可以再看看这段代码，猜一猜它的运行结果
```java
class syncTest implements Runnable {

    private static int i = 0;   //共享资源

    private synchronized void add() {
        i++;
    }

    @Override
    public void run() {
        for (int j = 0; j < 10000; j++) {
            add();
        }
    }

    public static void main(String[] args) throws Exception {

//        syncTest syncTest = new syncTest();

        Thread t1 = new Thread(new syncTest());
        Thread t2 = new Thread(new syncTest());

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        System.out.println(i);
    }
}
```
结果为
```java
18634
```
第二个示例中的`add()`方法虽然也使用`synchronized`关键字修饰了，但是因为两次`new syncTest()`操作建立的是两个不同的对象，也就是说存在两个不同的对象锁，线程t1和t2使用的是不同的对象锁，所以不能保证线程安全。那这种情况应该如何解决呢？因为每次创建的实例对象都是不同的，而类对象却只有一个，如果synchronized关键字作用于类对象，即用`synchronized`修饰静态方法，问题则迎刃而解。
<a name="H0dyv"></a>
### 修饰静态方法
只需要在`add()`方法前用`static`修饰即可，即当`synchronized`作用于静态方法，锁就是当前的class对象。
```java
class syncTest implements Runnable {

    private static int i = 0;   //共享资源

    private static synchronized void add() {
        i++;
    }

    @Override
    public void run() {
        for (int j = 0; j < 10000; j++) {
            add();
        }
    }

    public static void main(String[] args) throws Exception {

//        syncTest syncTest = new syncTest();

        Thread t1 = new Thread(new syncTest());
        Thread t2 = new Thread(new syncTest());

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        System.out.println(i);
    }
}
```
结果为
```java
20000
```
<a name="Uuncb"></a>
### 修饰同步代码代码块
如果某些情况下，整个方法体比较大，需要同步的代码只是一小部分，如果直接对整个方法体进行同步，会使得代码性能变差，这时只需要对一小部分代码进行同步即可。代码如下：
```java
class syncTest implements Runnable {

    static int i = 0;   //共享资源

    @Override
    public void run() {
        //其他操作.......
        synchronized (this){   //this表示当前对象实例，这里还可以使用syncTest.class，表示class对象锁
            for (int j = 0; j < 10000; j++) {
                i++;
            }
        }

    }

    public static void main(String[] args) throws Exception {

        syncTest syncTest = new syncTest();

        Thread t1 = new Thread(syncTest);
        Thread t2 = new Thread(syncTest);

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        System.out.println(i);
    }
}
```
输出结果：
```java
20000
```
<a name="CWL7u"></a>
## `synchronized`关键字的底层原理
这个问题也是面试比较高频的一个问题，也是比较难理解的，理解`synchronized`需要一定的Java虚拟机的知识。<br />在jdk1.6之前，`synchronized`被称为重量级锁，在jdk1.6中，为了减少获得锁和释放锁带来的性能开销，引入了偏向锁和轻量级锁。下面先介绍jdk1.6之前的`synchronized`原理。
<a name="YbbSJ"></a>
### 对象头
在HotSpot虚拟机中，Java对象在内存中的布局大致可以分为三部分：对象头、实例数据和填充对齐。因为`synchronized`用的锁是存在对象头里的，这里需要重点了解对象头。如果对象头是数组类型，则对象头由Mark Word、Class MetadataAddress和Array length组成，如果对象头非数组类型，对象头则由Mark Word和Class MetadataAddress组成。在32位虚拟机中，数组类型的Java对象头的组成如下表：

| 内容 | 说明 | 长度 |
| --- | --- | --- |
| Mark Word | 存储对象的hashCode、分代年龄和锁标记位 | 32bit |
| Class MetadataAddress | 存储到对象类型数据的指针 | 32bit |
| Array length | 数组的长度 | 32bit |

这里需要重点掌握的是Mark Word。
<a name="EXIJ4"></a>
### Mark Word
在运行期间，Mark Word中存储的数据会随着锁标志位的变化而变化，在32位虚拟机中，不同状态下的组成如下：<br />![2021-05-31-18-04-40-199736.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622470012786-8fd5ef72-19e6-413b-94fb-3b60c96eaf9f.png#clientId=u51719279-841e-4&from=ui&id=ud0bbe12c&originHeight=247&originWidth=1080&originalType=binary&size=104455&status=done&style=none&taskId=u90d51f2c-0d16-4069-b3e4-76a0e71c562)<br />其中线程ID表示持有偏向锁线程的ID，Epoch表示偏向锁的时间戳，偏向锁和轻量级锁是在jdk1.6中引入的。
<a name="A5TuL"></a>
### 重量级锁的底部实现原理：Monitor
在jdk1.6之前，`synchronized`只能实现重量级锁，Java虚拟机是基于Monitor对象来实现重量级锁的，所以首先来了解下`Monitor`，在`Hotspot`虚拟机中，`Monitor`是由`ObjectMonitor`实现的，其源码是用C++语言编写的，首先先下载Hotspot的源码，源码下载链接：[http://hg.openjdk.java.net/jdk8/jdk8/hotspot](http://hg.openjdk.java.net/jdk8/jdk8/hotspot)，找到ObjectMonitor.hpp文件，路径是src/share/vm/runtime/objectMonitor.hpp，这里只是简单介绍下其数据结构
```java
ObjectMonitor() {
    _header       = NULL;
    _count        = 0; //锁的计数器，获取锁时count数值加1，释放锁时count值减1，直到
    _waiters      = 0, //等待线程数
    _recursions   = 0; //锁的重入次数
    _object       = NULL; 
    _owner        = NULL; //指向持有ObjectMonitor对象的线程地址
    _WaitSet      = NULL; //处于wait状态的线程，会被加入到_WaitSet
    _WaitSetLock  = 0 ;
    _Responsible  = NULL ;
    _succ         = NULL ;
    _cxq          = NULL ; //阻塞在EntryList上的单向线程列表
    FreeNext      = NULL ;
    _EntryList    = NULL ; //处于等待锁block状态的线程，会被加入到该列表
    _SpinFreq     = 0 ;
    _SpinClock    = 0 ;
    OwnerIsThread = 0 ;
  }
```
其中 `_owner`、`_WaitSet`和`_EntryList` 字段比较重要，它们之间的转换关系如下图<br />![2021-05-31-18-04-40-337370.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622470034723-3b803cb6-d4c2-435c-be16-b6698ff0a2b7.png#clientId=u51719279-841e-4&from=ui&id=u7138dec7&originHeight=351&originWidth=621&originalType=binary&size=22997&status=done&style=none&taskId=uf955c371-d83a-41b5-a1a1-6fc8054e59f)<br />从上图可以总结获取`Monitor`和释放`Monitor`的流程如下：

1. 当多个线程同时访问同步代码块时，首先会进入到EntryList中，然后通过CAS的方式尝试将`Monitor`中的`owner`字段设置为当前线程，同时`count`加1，若发现之前的`owner`的值就是指向当前线程的，`recursions`也需要加1。如果CAS尝试获取锁失败，则进入到`EntryList`中。
2. 当获取锁的线程调用`wait()`方法，则会将`owner`设置为null，同时`count`减1，`recursions`减1，当前线程加入到`WaitSet`中，等待被唤醒。
3. 当前线程执行完同步代码块时，则会释放锁，`count`减1，`recursions`减1。当`recursions`的值为0时，说明线程已经释放了锁。

之前提到过一个常见面试题，为什么`wait()`、`notify()`等方法要在同步方法或同步代码块中来执行呢，这里就能找到原因，是因为`wait()`、`notify()`方法需要借助`ObjectMonitor`对象内部方法来完成。
<a name="x6cR1"></a>
### `synchronized`作用于同步代码块的实现原理
前面已经了解`Monitor`的实现细节，而Java虚拟机则是通过进入和退出`Monitor`对象来实现方法同步和代码块同步的。这里为了更方便看程序字节码执行指令，先在IDEA中安装了一个jclasslib Bytecode viewer插件。先来看这个`synchronized`作用于同步代码块的代码。
```java
public void run() {
    //其他操作.......
    synchronized (this){   //this表示当前对象实例，这里还可以使用syncTest.class，表示class对象锁
        for (int j = 0; j < 10000; j++) {
            i++;
        }
    }

}
```
查看代码字节码指令如下：
```java
 1 dup
 2 astore_1
 3 monitorenter     //进入同步代码块的指令
 4 iconst_0
 5 istore_2
 6 iload_2
 7 sipush 10000
10 if_icmpge 27 (+17)
13 getstatic #2 <com/company/syncTest.i>
16 iconst_1
17 iadd
18 putstatic #2 <com/company/syncTest.i>
21 iinc 2 by 1
24 goto 6 (-18)
27 aload_1
28 monitorexit     //结束同步代码块的指令
29 goto 37 (+8)
32 astore_3
33 aload_1
34 monitorexit     //遇到异常时执行的指令
35 aload_3
36 athrow
37 return
```
从上述字节码中可以看到同步代码块的实现是由`monitorenter `和`monitorexit`指令完成的，其中`monitorenter`指令所在的位置是同步代码块开始的位置，第一个`monitorexit`指令是用于正常结束同步代码块的指令，第二个`monitorexit`指令是用于异常结束时所执行的释放Monitor指令。
<a name="kjqnu"></a>
### synchronized作用于同步方法原理
```java
private synchronized void add() {
    i++;
}
```
查看字节码如下：
```java
0 getstatic #2 <com/company/syncTest.i>
3 iconst_1
4 iadd
5 putstatic #2 <com/company/syncTest.i>
8 return
```
发现这个没有`monitorenter` 和 `monitorexit` 这两个指令了，而在查看该方法的class文件的结构信息时发现了`Access flags`后边的`synchronized`标识，该标识表明了该方法是一个同步方法。Java虚拟机通过该标识可以来辨别一个方法是否为同步方法，如果有该标识，线程将持有`Monitor`，在执行方法，最后释放`Monitor`。<br />![2021-05-31-18-04-40-439097.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622470046115-867503ef-b349-4b54-970b-a935161515a7.png#clientId=u51719279-841e-4&from=ui&id=u2547961d&originHeight=152&originWidth=389&originalType=binary&size=3660&status=done&style=shadow&taskId=ub4786652-d6a1-42f4-8d87-b16a33aaba7)<br />原理大概就是这样，最后总结一下，面试中应该简洁地如何回答`synchroized`的底层原理这个问题。<br />答：Java虚拟机是通过进入和退出Monitor对象来实现代码块同步和方法同步的，代码块同步使用的是`monitorenter`和 `monitorexit` 指令实现的，而方法同步是通过`Access flags`后面的标识来确定该方法是否为同步方法。
<a name="KiIEC"></a>
## Jdk1.6为什么要对`synchronized`进行优化？
因为Java虚拟机是通过进入和退出Monitor对象来实现代码块同步和方法同步的，而Monitor是依靠底层操作系统的Mutex Lock来实现的，操作系统实现线程之间的切换需要从用户态转换到内核态，这个切换成本比较高，对性能影响较大。
<a name="KpUrB"></a>
## jDK1.6对`synchronized`做了哪些优化？
<a name="dZerY"></a>
### 锁的升级
在JDK1.6中，为了减少获得锁和释放锁带来的性能消耗，引入了偏向锁和轻量级锁，锁的状态变成了四种，如下图所示。锁的状态会随着竞争激烈逐渐升级，但通常情况下，锁的状态只能升级不能降级。这种只能升级不能降级的策略是为了提高获得锁和释放锁的效率。<br />![2021-05-31-18-04-40-536835.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622470056499-15d978ca-9dc1-4b11-a45d-c27d0e2bd9e3.png#clientId=u51719279-841e-4&from=ui&id=uaf9628df&originHeight=62&originWidth=682&originalType=binary&size=3829&status=done&style=shadow&taskId=u8ca35a28-f669-44cf-bfc2-e33deb5c8fd)
<a name="ebl1u"></a>
### 偏向锁
常见面试题：偏向锁的原理（或偏向锁的获取流程）、偏向锁的好处是什么（获取偏向锁的目的是什么）<br />引入偏向锁的目的：减少只有一个线程执行同步代码块时的性能消耗，即在没有其他线程竞争的情况下，一个线程获得了锁。<br />偏向锁的获取流程：

1. 检查对象头中Mark Word是否为可偏向状态，如果不是则直接升级为轻量级锁。
2. 如果是，判断Mark Work中的线程ID是否指向当前线程，如果是，则执行同步代码块。
3. 如果不是，则进行CAS操作竞争锁，如果竞争到锁，则将Mark Work中的线程ID设为当前线程ID，执行同步代码块。
4. 如果竞争失败，升级为轻量级锁。

偏向锁的获取流程如下图：<br />![2021-05-31-18-04-40-706381.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622470066697-6574d9ee-92ce-4adc-9730-98314124eca2.png#clientId=u51719279-841e-4&from=ui&id=u4a51acfc&originHeight=583&originWidth=502&originalType=binary&size=13718&status=done&style=shadow&taskId=uf261f887-0022-4390-85dc-35d566b1e47)<br />偏向锁的撤销：<br />只有等到竞争，持有偏向锁的线程才会撤销偏向锁。偏向锁撤销后会恢复到无锁或者轻量级锁的状态。

1. 偏向锁的撤销需要到达全局安全点，全局安全点表示一种状态，该状态下所有线程都处于暂停状态。
2. 判断锁对象是否处于无锁状态，即获得偏向锁的线程如果已经退出了临界区，表示同步代码已经执行完了。重新竞争锁的线程会进行CAS操作替代原来线程的ThreadID。
3. 如果获得偏向锁的线程还处于临界区之内，表示同步代码还未执行完，将获得偏向锁的线程升级为轻量级锁。

一句话简单总结偏向锁原理：使用CAS操作将当前线程的ID记录到对象的Mark Word中。
<a name="ydyBO"></a>
### 轻量级锁
引入轻量级锁的目的：在多线程交替执行同步代码块时（未发生竞争），避免使用互斥量（重量锁）带来的性能消耗。但多个线程同时进入临界区（发生竞争）则会使得轻量级锁膨胀为重量级锁。<br />轻量级锁的获取流程：

1. 首先判断当前对象是否处于一个无锁的状态，如果是，Java虚拟机将在当前线程的栈帧建立一个锁记录（Lock Record），用于存储对象目前的Mark  Word的拷贝，如图所示。

![2021-05-31-18-04-40-806115.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622470083928-c8f022c2-de7b-4299-b9b2-24d097625532.png#clientId=u51719279-841e-4&from=ui&id=ub5fe6c3d&originHeight=341&originWidth=481&originalType=binary&size=3949&status=done&style=shadow&taskId=u1706afb1-c71f-49c0-b902-4a399434bf4)

2. 将对象的Mark Word复制到栈帧中的Lock Record中，并将Lock Record中的owner指向当前对象，并使用CAS操作将对象的Mark Word更新为指向Lock Record的指针，如图所示。

![2021-05-31-18-04-40-933782.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622470094676-3bcc0659-1aae-4cbf-90db-4a9fc887418b.png#clientId=u51719279-841e-4&from=ui&id=uac55e823&originHeight=368&originWidth=481&originalType=binary&size=3897&status=done&style=shadow&taskId=u683fff59-3513-4384-8239-a133ea8510e)

3. 如果第二步执行成功，表示该线程获得了这个对象的锁，将对象Mark Word中锁的标志位设置为“00”，执行同步代码块。
4. 如果第二步未执行成功，需要先判断当前对象的Mark Word是否指向当前线程的栈帧，如果是，表示当前线程已经持有了当前对象的锁，这是一次重入，直接执行同步代码块。如果不是表示多个线程存在竞争，该线程通过自旋尝试获得锁，即重复步骤2，自旋超过一定次数，轻量级锁升级为重量级锁。

轻量级锁的解锁：<br />轻量级的解锁同样是通过CAS操作进行的，线程会通过CAS操作将Lock Record中的Mark Word（官方称为Displaced Mark Word）替换回来。如果成功表示没有竞争发生，成功释放锁，恢复到无锁的状态；如果失败，表示当前锁存在竞争，升级为重量级锁。<br />一句话总结轻量级锁的原理：将对象的Mark Word复制到当前线程的Lock Record中，并将对象的Mark Word更新为指向Lock  Record的指针。
<a name="bbR6B"></a>
### 自旋锁
Java锁的几种状态并不包括自旋锁，轻量级锁的竞争就是采用的自旋锁机制。<br />什么是自旋锁：当线程A已经获得锁时，线程B再来竞争锁，线程B不会直接被阻塞，而是在原地循环 等待，当线程A释放锁后，线程B可以马上获得锁。<br />引入自旋锁的原因：因为阻塞和唤起线程都会引起操作系统用户态和核心态的转变，对系统性能影响较大，而自旋等待可以避免线程切换的开销。<br />自旋锁的缺点：自旋等待虽然可以避免线程切花的开销，但它也会占用处理器的时间。如果持有锁的线程在较短的时间内释放了锁，自旋锁的效果就比较好，如果持有锁的线程很长时间都不释放锁，自旋的线程就会白白浪费资源，所以一般线程自旋的次数必须有一个限制，该次数可以通过参数-XX:PreBlockSpin调整，一般默认为10。<br />自适应自旋锁：JDK1.6引入了自适应自旋锁，自适应自旋锁的自旋次数不再固定，而是由上一次在同一个锁上的自旋时间及锁的拥有者的状态来决定的。如果对于某个锁对象，刚刚有线程自旋等待成功获取到锁，那么虚拟机将认为这次自旋等待的成功率也很高，会允许线程自旋等待的时间更长一些。如果对于某个锁对象，线程自旋等待很少成功获取到锁，那么虚拟机将会减少线程自旋等待的时间。
<a name="ZE4De"></a>
## 偏向锁、轻量级锁、重量级锁的对比
| 锁 | 优点 | 缺点 | 实用场景 |
| --- | --- | --- | --- |
| 偏向锁 | 加锁和解锁不需要额外的消耗，和执行非同步方法相比仅存在纳秒级的差距 | 如果线程间存在竞争，会额外带来锁撤销的消耗 | 适用于只有一个线程访问同步块场景 |
| 轻量级锁 | 竞争的线程不会阻塞，提高了程序的响应速度 | 如果始终得不到锁竞争的线程，使用自旋会消耗CPU | 追求响应时间，同步块执行速度非常快 |
| 重量级锁 | 线程竞争不使用自旋，不会消耗CPU | 线程阻塞，响应时间缓慢 | 追求吞吐量，同步块执行速度较慢 |

该表格出自《Java并发编程的艺术》
<a name="eSvQ8"></a>
## 了解锁消除吗？
锁消除是指Java虚拟机在即时编译时，通过对运行上下的扫描，消除那些不可能存在共享资源竞争的锁。锁消除可以节约无意义的请求锁时间。
<a name="qlfEr"></a>
## 了解锁粗化吗
一般情况下，为了提高性能，总是将同步块的作用范围限制到最小，这样可以使得需要同步的操作尽可能地少。但如果一系列连续的操作一直对某个对象反复加锁和解锁，频繁地进行互斥同步操作也会引起不必要的性能消耗。<br />如果虚拟机检测到有一系列操作都是对某个对象反复加锁和解锁，会将加锁同步的范围粗化到整个操作序列的外部。可以看下面这个经典案例。
```java
for(int i=0;i<n;i++){
    synchronized(lock){
    }
}
```
这段代码会导致频繁地加锁和解锁，锁粗化后
```java
synchronized(lock){
    for(int i=0;i<n;i++){
    }
}
```
<a name="Tnxnf"></a>
## 当线程1进入到一个对象的`synchronized`方法A后，线程2是否可以进入到此对象的`synchronized`方法B?
不能，线程2只能访问该对象的非同步方法。因为执行同步方法时需要获得对象的锁，而线程1在进入`sychronized`修饰的方A时已经获取到了锁，线程2只能等待，无法进入到`synchronized`修饰的方法B，但可以进入到其他非`synchronized`修饰的方法。
<a name="naVlX"></a>
## `synchronized`和`volatile`的区别？

- `volatile`主要是保证内存的可见性，即变量在寄存器中的内存是不确定的，需要从主存中读取。`synchronized`主要是解决多个线程访问资源的同步性。
- `volatile`作用于变量，`synchronized`作用于代码块或者方法。
- `volatile`仅可以保证数据的可见性，不能保证数据的原子性。`synchronized`可以保证数据的可见性和原子性。
- `volatile`不会造成线程的阻塞，`synchronized`会造成线程的阻塞。
<a name="KMabG"></a>
## `synchronized`和`Lock`的区别？

- `Lock`是显示锁，需要手动开启和关闭。`synchronized`是隐式锁，可以自动释放锁。
- `Lock`是一个接口，是JDK实现的。`synchronized`是一个关键字，是依赖JVM实现的。
- `Lock`是可中断锁，`synchronized`是不可中断锁，需要线程执行完才能释放锁。
- 发生异常时，`Lock`不会主动释放占有的锁，必须通过`unlock`进行手动释放，因此可能引发死锁。`synchronized`在发生异常时会自动释放占有的锁，不会出现死锁的情况。
- `Lock`可以判断锁的状态，`synchronized`不可以判断锁的状态。
- `Lock`实现锁的类型是可重入锁、公平锁。`synchronized`实现锁的类型是可重入锁，非公平锁。
- `Lock`适用于大量同步代码块的场景，`synchronized`适用于少量同步代码块的场景。
