Javasynchronized<br />想了解 synchronized 是如何运行的？就要先搞清楚 synchronized 是如何实现？synchronized 同步锁是通过 JVM 内置的 Monitor 监视器实现的，而监视器又是依赖操作系统的互斥锁 Mutex 实现的，那接下来先来了解一下监视器。
<a name="E8ehH"></a>
## 监视器
**监视器是一个概念或者说是一个机制，它用来保障在任何时候，只有一个线程能够执行指定区域的代码。**
> 一个监视器像是一个建筑，建筑里有一个特殊的房间，这个房间同一时刻只能被一个线程所占有。一个线程从进入该房间到离开该房间，可以全程独占该房间的所有数据。进入该建筑叫做进入监视器（entering the monitor），进入该房间叫做获得监视器（acquiring the monitor），独自占有该房间叫做拥有监视器（owning the monitor），离开该房间叫做释放监视器（releasing the monitor），离开该建筑叫做退出监视器（exiting the monitor）。

严格意义来说监视器和锁的概念是不同的，但很多地方也把二者相互指代。
<a name="ZbBLy"></a>
## 底层实现
下面在代码中添加一个 synchronized 代码块，来观察一下它在字节码层面是如何实现的？示例代码如下：
```java
public class SynchronizedToMonitorExample {
    public static void main(String[] args) {
        int count = 0;
        synchronized (SynchronizedToMonitorExample.class) {
            for (int i = 0; i < 10; i++) {
                count++;
            }
        }
        System.out.println(count);
    }
}
```
将上述代码编译成字节码之后，得到的结果是这样的：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653318936296-feaa1511-bb91-4ddf-8796-fcbe3f968a36.png#clientId=ubcdce0d5-7353-4&from=paste&id=u147c3272&originHeight=1076&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud3a4b5bf-24e6-4769-b1e7-b7dad1069ba&title=)从上述结果可以看出，在 main 方法中多了一对 monitorenter 和 monitorexit 的指令，它们的含义是：

- `monitorenter`：表示进入监视器。
- `monitorexit`：表示退出监视器。

由此可知 `synchronized` 是依赖 Monitor 监视器实现的。
<a name="wI9Aj"></a>
## 执行流程
在 Java 中，`**synchronized**`** 是非公平锁，也是可以重入锁**。所谓的非公平锁是指，线程获取锁的顺序不是按照访问的顺序先来先到的，而是由线程自己竞争，随机获取到锁。可重入锁指的是，一个线程获取到锁之后，可以重复得到该锁。这些内容是理解接下来内容的前置知识。在 HotSpot 虚拟机中，Monitor 底层是由 C++实现的，它的实现对象是 `ObjectMonitor`，`ObjectMonitor` 结构体的实现如下：
```java
ObjectMonitor::ObjectMonitor() {  
	_header       = NULL;  
	_count       = 0;  
	_waiters      = 0,  
	_recursions   = 0;       //线程的重入次数
	_object       = NULL;  
	_owner        = NULL;    //标识拥有该monitor的线程
	_WaitSet      = NULL;    //等待线程组成的双向循环链表，_WaitSet是第一个节点
	_WaitSetLock  = 0 ;  
	_Responsible  = NULL ;  
	_succ         = NULL ;  
	_cxq          = NULL ;    //多线程竞争锁进入时的单向链表
	FreeNext      = NULL ;  
	_EntryList    = NULL ;    //_owner从该双向循环链表中唤醒线程结点，_EntryList是第一个节点
	_SpinFreq     = 0 ;  
	_SpinClock    = 0 ;  
	OwnerIsThread = 0 ;  
} 
```
在以上代码中有几个关键的属性：

- `_count`：记录该线程获取锁的次数（也就是前前后后，这个线程一共获取此锁多少次）。
- `_recursions`：锁的重入次数。
- `_owner`：The Owner 拥有者，是持有该 ObjectMonitor（监视器）对象的线程；
- `_EntryList`：EntryList 监控集合，存放的是处于阻塞状态的线程队列，在多线程下，竞争失败的线程会进入 EntryList 队列。
- `_WaitSet`：WaitSet 待授权集合，存放的是处于 wait 状态的线程队列，当线程执行了 wait() 方法之后，会进入 WaitSet 队列。

监视器执行的流程如下：

1. 线程通过 CAS（对比并替换）尝试获取锁，如果获取成功，就将 _owner 字段设置为当前线程，说明当前线程已经持有锁，并将 _recursions 重入次数的属性 +1。如果获取失败则先通过自旋 CAS 尝试获取锁，如果还是失败则将当前线程放入到 EntryList 监控队列（阻塞）。
2. 当拥有锁的线程执行了 wait 方法之后，线程释放锁，将 owner 变量恢复为 null 状态，同时将该线程放入 WaitSet 待授权队列中等待被唤醒。
3. 当调用 notify 方法时，随机唤醒 WaitSet 队列中的某一个线程，当调用 `notifyAll` 时唤醒所有的 WaitSet 中的线程尝试获取锁。
4. 线程执行完释放了锁之后，会唤醒 EntryList 中的所有线程尝试获取锁。

以上就是监视器的执行流程，执行流程如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653318936300-792dece1-ff7c-46cc-911d-e58172392772.png#clientId=ubcdce0d5-7353-4&from=paste&id=u3df8085d&originHeight=702&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u44cb198a-14fd-4057-b865-3f78ddc2ac2&title=)
<a name="CYKEK"></a>
## 总结
synchronized 同步锁是通过 JVM 内置的 Monitor 监视器实现的，而监视器又是依赖操作系统的互斥锁 Mutex 实现的。JVM 监视器的执行流程是：线程先通过自旋 CAS 的方式尝试获取锁，如果获取失败就进入 EntrySet 集合，如果获取成功就拥有该锁。当调用 wait() 方法时，线程释放锁并进入 WaitSet 集合，等其他线程调用 `notify` 或 `notifyAll` 方法时再尝试获取锁。锁使用完之后就会通知 EntrySet 集合中的线程，让它们尝试获取锁。
