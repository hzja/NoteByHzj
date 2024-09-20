Java CAS
<a name="isu9F"></a>
### 一、术语定义
| 术语 | 英文 | 解释 |
| --- | --- | --- |
| 缓存行 | Cache line | 缓存的最小操作单位 |
| 比较并交换 | Compare And Swap（CAS） | CAS操作需要输入两个数值，一个旧值（期望操作前的值）和一个新值，在操作期间先比较下在旧值有没有发生变化，如果没有发生变化，才交换成新值，发生了变化则不交换 |
| 内存顺序冲突 | Memory order violation | 内存顺序冲突一般是由假共享引起，假共享是指多个CPU同时修改同一个缓存行的不同部分而引起其中一个CPU的操作无效，当出现这个内存顺序冲突时，CPU必须清空流水线。 |

<a name="PufP2"></a>
### 二、CAS应用
CAS有3个操作数，内存值V，旧的预期值A，要修改的新值B。当且仅当预期值A和内存值V相同时，将内存值V修改为B，否则什么都不做。
<a name="gx5hE"></a>
#### 非阻塞算法 （nonblocking algorithms）
一个线程的失败或者挂起不应该影响其他线程的失败或挂起的算法。<br />现代的CPU提供了特殊的指令，可以自动更新共享数据，而且能够检测到其他线程的干扰，而 `compareAndSet()` 就用这些代替了锁定。<br />以`AtomicInteger`来研究在没有锁的情况下是如何做到数据正确性的。
```java
private volatile int value;
```
首先毫无疑问，在没有锁的机制下可能需要借助`volatile`原语，保证线程间的数据是可见的（共享的）。<br />这样才获取变量的值的时候才能直接读取。
```java
public final int get() {return value; }
```
然后来看看++1(自增)是怎么做到的:
```java
public final int incrementAndGet() {
    for (;;) {
        int current = get();
        int next = current + 1;
        if (compareAndSet(current, next))
            return next;
    }
}
```
在这里采用了CAS操作，每次从内存中读取数据然后将此数据和+1后的结果进行CAS操作，如果成功就返回结果，否则重试直到成功为止。<br />而`compareAndSet`利用JNI来完成CPU指令的操作。
```java
public final boolean compareAndSet(int expect, int update) {   
    return unsafe.compareAndSwapInt(this, valueOffset, expect, update);
}
```
整体的过程就是这样子的，利用CPU的CAS指令，同时借助JNI来完成Java的非阻塞算法。其它原子操作都是利用类似的特性完成的。

<a name="MW92f"></a>
### 三、CAS原理
CAS通过调用JNI的代码实现的。JNI:Java Native Interface为JAVA本地调用，允许java调用其他语言。<br />而`compareAndSwapInt`就是借助C来调用CPU底层指令实现的。下面从分析比较常用的CPU（intel x86）来解释CAS的实现原理。<br />下面是sun.misc.Unsafe类的`compareAndSwapInt()`方法的源代码：
```java
public final native boolean compareAndSwapInt(Object o, long offset, int expected, int x);
```
可以看到这是个本地方法调用。这个本地方法在openjdk中依次调用的c++代码为：unsafe.cpp，atomic.cpp和atomicwindowsx86.inline.hpp。这个本地方法的最终实现在openjdk的如下位置：openjdk-7-fcs-src-b147-27jun2011\openjdk\hotspot\src\oscpu\windowsx86\vm\ atomicwindowsx86.inline.hpp（对应于windows操作系统，X86处理器）。下面是对应于intel x86处理器的源代码的片段:
```cpp

// Adding a lock prefix to an instruction on MP machine 
// VC++ doesn't like the lock prefix to be on a single line 
// so we can't insert a label after the lock prefix. 
// By emitting a lock prefix, we can define a label after it. 
#define LOCK_IF_MP(mp) __asm cmp mp, 0  \ 
                     __asm je L0      \ 
                       __asm _emit 0xF0 \ 
                       __asm L0: 
inline jint Atomic::cmpxchg (jint exchange_value, volatile jint* dest, jint compare_value) {
  // alternative for InterlockedCompareExchange 
  int mp = os::is_MP();
  __asm {
      mov edx,
      dest mov ecx, 
      exchange_value mov eax,
      compare_value LOCK_IF_MP(mp) 
      cmpxchg dword ptr [edx], ecx
    } 
}

```
如上面源代码所示，程序会根据当前处理器的类型来决定是否为cmpxchg指令添加lock前缀。如果程序是在多处理器上运行，就为cmpxchg指令加上lock前缀（lock cmpxchg）。反之，如果程序是在单处理器上运行，就省略 lock 前缀（单处理器自身会维护单处理器内的顺序一致性，不需要lock前缀提供的内存屏障效果）。<br />intel 的手册对 lock 前缀的说明如下：

1. 确保对内存的读-改-写操作原子执行。在 Pentium 及 Pentium 之前的处理器中，带有lock 前缀的指令在执行期间会锁住总线，使得其他处理器暂时无法通过总线访问内存。很显然，这会带来昂贵的开销。从 Pentium 4，Intel Xeon 及 P6 处理器开始，intel 在原有总线锁的基础上做了一个很有意义的优化：如果要访问的内存区域（area of memory）在 lock前缀指令执行期间已经在处理器内部的缓存中被锁定（即包含该内存区域的缓存行当前处于独占或以修改状态），并且该内存区域被完全包含在单个缓存行（cache line）中，那么处理器将直接执行该指令。由于在指令执行期间该缓存行会一直被锁定，其它处理器无法读/写该指令要访问的内存区域，因此能保证指令执行的原子性。这个操作过程叫做缓存锁定（cache locking），缓存锁定将大大降低 lock 前缀指令的执行开销，但是当多处理器之间的竞争程度很高或者指令访问的内存地址未对齐时，仍然会锁住总线。
2. 禁止该指令与之前和之后的读和写指令重排序。
3. 把写缓冲区中的所有数据刷新到内存中。

上面的第2点和第3点所具有的内存屏障效果，足以同时实现 `volatile` 读和 `volatile` 写的内存语义。<br />经过上面的这些分析，现在终于能明白为什么 JDK 文档说 CAS 同时具有 `volatile` 读和`volatile`写的内存语义了。
<a name="Ju1iu"></a>
### 四、CAS缺点
CAS虽然很高效的解决原子操作，但是CAS仍然存在三大问题。ABA问题，循环时间长开销大和只能保证一个共享变量的原子操作

1. ABA问题。因为CAS需要在操作值的时候检查下值有没有发生变化，如果没有发生变化则更新，但是如果一个值原来是A，变成了B，又变成了A，那么使用CAS进行检查时会发现它的值没有发生变化，但是实际上却变化了。ABA问题的解决思路就是使用版本号。在变量前面追加上版本号，每次变量更新的时候把版本号加一，那么A－B－A 就会变成1A-2B－3A。

从Java1.5开始JDK的atomic包里提供了一个类`AtomicStampedReference`来解决ABA问题。这个类的`compareAndSet`方法作用是首先检查当前引用是否等于预期引用，并且当前标志是否等于预期标志，如果全部相等，则以原子方式将该引用和该标志的值设置为给定的更新值。

1. 循环时间长开销大。自旋CAS如果长时间不成功，会给CPU带来非常大的执行开销。如果JVM能支持处理器提供的pause指令那么效率会有一定的提升，pause指令有两个作用，第一它可以延迟流水线执行指令（de-pipeline）,使CPU不会消耗过多的执行资源，延迟的时间取决于具体实现的版本，在一些处理器上延迟时间是零。第二它可以避免在退出循环的时候因内存顺序冲突（memory order violation）而引起CPU流水线被清空（CPU pipeline flush），从而提高CPU的执行效率。
2. 只能保证一个共享变量的原子操作。当对一个共享变量执行操作时，可以使用循环CAS的方式来保证原子操作，但是对多个共享变量操作时，循环CAS就无法保证操作的原子性，这个时候就可以用锁，或者有一个取巧的办法，就是把多个共享变量合并成一个共享变量来操作。比如有两个共享变量`i＝2,j=a`，合并一下`ij=2a`，然后用CAS来操作ij。从Java1.5开始JDK提供了`AtomicReference`类来保证引用对象之间的原子性，可以把多个变量放在一个对象里来进行CAS操作。
<a name="cVHaS"></a>
### 五、`concurrent`包的实现
由于java的CAS同时具有 `volatile` 读和`volatile`写的内存语义，因此Java线程之间的通信现在有了下面四种方式：

1. A线程写`volatile`变量，随后B线程读这个volatile变量。
2. A线程写`volatile`变量，随后B线程用CAS更新这个`volatile`变量。
3. A线程用CAS更新一个`volatile`变量，随后B线程用CAS更新这个`volatile`变量。
4. A线程用CAS更新一个`volatile`变量，随后B线程读这个`volatile`变量。

Java的CAS会使用现代处理器上提供的高效机器级别原子指令，这些原子指令以原子方式对内存执行读-改-写操作，这是在多处理器中实现同步的关键（从本质上来说，能够支持原子性读-改-写指令的计算机器，是顺序计算图灵机的异步等价机器，因此任何现代的多处理器都会去支持某种能对内存执行原子性读-改-写操作的原子指令）。同时，`volatile`变量的读/写和CAS可以实现线程之间的通信。把这些特性整合在一起，就形成了整个`concurrent`包得以实现的基石。如果仔细分析`concurrent`包的源代码实现，会发现一个通用化的实现模式：

1. 首先，声明共享变量为`volatile`；
2. 然后，使用CAS的原子条件更新来实现线程之间的同步；
3. 同时，配合以`volatile`的读/写和CAS所具有的`volatile`读和写的内存语义来实现线程之间的通信。

AQS，非阻塞数据结构和原子变量类（java.util.concurrent.atomic包中的类），这些`concurrent`包中的基础类都是使用这种模式来实现的，而`concurrent`包中的高层类又是依赖于这些基础类来实现的。从整体来看，`concurrent`包的实现示意图如下：<br />![2021-05-18-08-16-04-111240.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621296972712-53bfb21f-3f72-435f-afa1-2acdb35789b4.jpeg#clientId=u995c323e-cc5b-4&from=ui&id=u5e6826d9&originHeight=445&originWidth=623&originalType=binary&size=25140&status=done&style=shadow&taskId=uac7e61c1-93e4-43de-a1b1-7935f18dcf1)
