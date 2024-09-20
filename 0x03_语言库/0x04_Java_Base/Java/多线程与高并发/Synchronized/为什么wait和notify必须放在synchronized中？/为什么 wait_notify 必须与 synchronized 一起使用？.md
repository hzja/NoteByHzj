Java synchronized
<a name="YD65E"></a>
### 为什么 java wait/notify 必须与 synchronized 一起使用
这个问题就是书本上没怎么讲解，就是告诉这样处理，但没有解释为什么这么处理？
<a name="PP4Lp"></a>
### synchronized是什么
Java中提供了两种实现同步的基础语义：`synchronized`方法和`synchronized`块， 看个demo：
```cpp
public class SyncTest {

   // 1、synchronized方法
   
 public synchronized void syncMethod(){
        System.out.println("hello method");
    }
    
    // 2、synchronized块
    
    public void syncBlock(){
        synchronized (this){
            System.out.println("hello block");
        }
    }
}
```
具体还要区分：

- **修饰实例方法**，作用于当前实例加锁，进入同步代码前要获得当前实例的锁。不同实例对象的访问，是不会形成锁的。
- **修饰静态方法**，作用于当前类对象加锁，进入同步代码前要获得当前类对象的锁
- **修饰代码块**，指定加锁对象，对给定对象加锁，进入同步代码库前要获得给定对象的锁。

它具有的特性：

- 原子性
- 可见性
- 有序性
- 可重入性
<a name="eF8sf"></a>
### synchronized如何实现锁
这样看来`synchronized`实现的锁是基于class对象来实现的，来看看如何实现的，它其实是跟class对象的对象头一起起作用的，对象在内存中的布局分为三块区域：对象头、实例数据和对齐填充。<br />其中对象头中有一个Mark Word，这里主要存储对象的hashCode、锁信息或分代年龄或GC标志等信息，把可能的情况列出来大概如下：

| 锁状态 | 25bit |  | 4bit | 1bit | 2bit |
| --- | --- | --- | --- | --- | --- |
|  | 23bit | 2bit |  | 是否偏向锁 | 锁标志位 |
| 轻量级锁 | 指向栈中锁记录的指针 |  |  |  | 00 |
| 重量级锁 | 指向互斥量（重量级锁）的指针 |  |  |  | 10 |
| GC标记 | 空 |  |  |  | 11 |
| 偏向锁 | 线程ID | Epoch | 对象分代年龄 | 1 | 01 |

其中`synchronized`就与锁标志位一起作用实现锁。主要分析一下重量级锁也就是通常说synchronized的对象锁，锁标识位为10，其中指针指向的是monitor对象（也称为管程或监视器锁）的起始地址。<br />每个对象都存在着一个 monitor 与之关联，对象与其 monitor 之间的关系有存在多种实现方式，如monitor可以与对象一起创建销毁或当线程试图获取对象锁时自动生成，但当一个 monitor 被某个线程持有后，它便处于锁定状态。<br />在Java虚拟机(HotSpot)中，monitor是由`ObjectMonitor`实现的，其主要数据结构如下（位于HotSpot虚拟机源码ObjectMonitor.hpp文件，C++实现的）：
```cpp
ObjectMonitor() {
    _header       = NULL;
    _count        = 0; //记录个数
    _waiters      = 0,
    _recursions   = 0;
    _object       = NULL;
    _owner        = NULL;
    _WaitSet      = NULL; //处于wait状态的线程，会被加入到_WaitSet
    _WaitSetLock  = 0 ;
    _Responsible  = NULL ;
    _succ         = NULL ;
    _cxq          = NULL ;
    FreeNext      = NULL ;
    _EntryList    = NULL ; //处于等待锁block状态的线程，会被加入到该列表
    _SpinFreq     = 0 ;
    _SpinClock    = 0 ;
    OwnerIsThread = 0 ;
}
```
上面有2个字段很重要：

- _WaitSet队列处于wait状态的线程，会被加入到`_WaitSet`。
- _EntryList队列处于等待锁block状态的线程，会被加入到该列表。
- _owner_owner指向持有`ObjectMonitor`对象的线程

**来模拟一下进入锁的流程：**<br />1、当多个线程同时访问一段同步代码时，首先会进入 `_EntryList` 集合<br />2、当线程获取到对象的monitor 后进入 `_Owner` 区域，并把monitor中的owner变量设置为当前线程同时monitor中的计数器count加1<br />3、若线程调用 `wait()` 方法，将释放当前持有的monitor，owner变量恢复为null，count自减1，同时该线程进入 WaitSet集合中等待被唤醒。<br />4、若当前线程执行完毕也将释放`monitor(锁)`并复位变量的值，以便其他线程进入获取`monitor(锁)`
<a name="LvccC"></a>
### wait/notify
这两个是Java对象都有的属性，表示这个对象的一个等待和通知机制，
<a name="sBBVh"></a>
### 不用synchronized 会怎么样
来看看不使用synchronized会怎么样，假设有2个线程，分别做2件事情，T1线程代码逻辑：
```java
while(!条件满足) // line 1  
{  
    obj.wait(); // line 2  
}  
doSomething();
```
T2线程的代码逻辑：
```java
更改条件为满足; // line 1  
obj.notify(); // line 2 
```
多线程环境下没有`synchronized`，没有锁的情况下可能会出现如下执行顺序情况：

- T1 line1 满足while 条件
- T2 line1 执行
- T2 line2 执行，notify发出去了
- T1 line2 执行，wait再执行

这样的执行顺序导致了notify通知发出去了，但没有用，已经wait是在之后执行，所以有人说没有保证原子性，就是line1 和line2 是一起执行结束，这个也被称作lost wake up问题。解决方法就是可以利用synchronized来加锁，于是有人就写了这样的代码：
```java
synchronized(lock)  {  
    while(!条件满足) {  
        obj.wait();  
    }  
    doSomething();  
}
```
```java
synchronized(lock)  {  
   更改条件为满足;  
   obj.notify();  
}
```
这样靠锁来做达到目的。但这代码会造成死锁，因为先`T1 wait()`，再`T2 notify();`而问题在于T1持有`lock`后`block`住了，T2一直无法获得lock，从而永无可能`notify()`并将T1的block状态解除，就与T1形成了死锁。<br />所以JVM在实现`wait()`方法时，一定需要先隐式的释放`lock`，再`block`，并且被`notify()`后从`wait()`方法返回前，隐式的重新获得了`lock`后才能继续user code的执行。要做到这点，就需要提供`lock`引用给`obj.wait()`方法，否则`obj.wait()`不知道该隐形释放哪个`lock`，于是调整之后的结果如下：
```java
synchronized(lock)  {  
    while(!条件满足)  {  
        obj.wait(lock);  
        // obj.wait(lock)伪实现  
        //   [1] unlock(lock)  
        //   [2] block住自己，等待notify()  
        //   [3] 已被notify()，重新lock(lock)  
        //   [4] obj.wait(lock)方法成功返回  
    }  
    doSomething();  
}
```
<a name="m8nmt"></a>
### [最终形态] 把lock和obj合一
其它线程API如PThread提供`wait()`函数的签名是类似`cond_wait(obj, lock)`的，因为同一个lock可以管多个obj条件队列。而Java内置的锁与条件队列的关系是1:1，所以就直接把obj当成lock来用了。因此此处就不需要额外提供lock，而直接使用obj即可，代码也更简洁：
```java
synchronized(obj)  {  
    while(!条件满足)  {  
        obj.wait();  
    }  
    doSomething();  
} 
```
```java
synchronized(lock)  {  
    更改条件为满足;  
    obj.notify();  
}
```
<a name="ex2fN"></a>
### lost wake up
wait/notify 如果不跟synchronized结合就会造成lost wake up，难以唤醒`wait`的线程，所以单独使用会有问题。
