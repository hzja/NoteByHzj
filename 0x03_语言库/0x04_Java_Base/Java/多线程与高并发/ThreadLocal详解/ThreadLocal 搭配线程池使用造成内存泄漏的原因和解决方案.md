JavaThreadLocal
<a name="XAwiG"></a>
## ThreadLocal是什么
ThreadLocal是一个本地线程副本变量工具类。主要用于将私有线程和该线程存放的副本对象做一个映射，各个线程之间的变量互不干扰，在高并发场景下，可以实现无状态的调用，特别适用于各个线程依赖不通的变量值完成操作的场景。<br />下图为ThreadLocal的内部结构图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667781004680-0eb3f2c9-c717-4a01-bb40-2c0cb5e73cd1.png#averageHue=%23f8f8f7&clientId=u810a1a87-76d5-4&from=paste&id=u404dbc36&originHeight=837&originWidth=806&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u80ad1780-8eb6-4a56-bc6b-c07e73f09ef&title=)<br />从上面的结构图，已经窥见ThreadLocal的核心机制：

- 每个Thread线程内部都有一个Map。
- Map里面存储线程本地对象（key）和线程的变量副本（value）
- 但是，Thread内部的Map是由ThreadLocal维护的，由ThreadLocal负责向map获取和设置线程的变量值。

所以对于不同的线程，每次获取副本值时，别的线程并不能获取到当前线程的副本值，形成了副本的隔离，互不干扰。
<a name="lgL8K"></a>
## ThreadLocalMap
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1667781004608-4ca2b690-5149-4f69-8ffb-6b334119f7f4.png#averageHue=%23f3f3f3&clientId=u810a1a87-76d5-4&from=paste&id=u2bbb7005&originHeight=330&originWidth=576&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u060a9bd8-9a2c-4714-ab0d-77a2fe60881&title=)<br />`ThreadLocalMap`是`ThreadLocal`的内部类，没有实现Map接口，用独立的方式实现了Map的功能，其内部的Entry也独立实现。<br />和`HashMap`的最大的不同在于，`ThreadLocalMap`结构非常简单，没有next引用，也就是说`ThreadLocalMap`中解决Hash冲突的方式并非链表的方式，而是采用线性探测的方式。（`**ThreadLocalMap**`**如何解决冲突？**）<br />在`ThreadLocalMap`中，也是用Entry来保存K-V结构数据的。但是Entry中key只能是`ThreadLocal`对象，这点被Entry的构造方法已经限定死了。
```java
static class Entry extends WeakReference<ThreadLocal> {
    /** The value associated with this ThreadLocal. */
    Object value;

    Entry(ThreadLocal k, Object v) {
        super(k);
        value = v;
    }
}
```
注意了！！<br />Entry继承自WeakReference（弱引用，生命周期只能存活到下次GC前），但只有Key是弱引用类型的，Value并非弱引用。（问题马上就来了）<br />由于ThreadLocalMap的key是弱引用，而Value是强引用。这就导致了一个问题，ThreadLocal在没有外部对象强引用时，发生GC时弱引用Key会被回收，而Value不会回收。<br />当线程没有结束，但是ThreadLocal已经被回收，则可能导致线程中存在`ThreadLocalMap<null, Object>`的键值对，造成内存泄露。（ThreadLocal被回收，ThreadLocal关联的线程共享变量还存在）。
<a name="pOpib"></a>
## 如何避免泄漏
为了防止此类情况的出现，有两种手段。<br />1、使用完线程共享变量后，显示调用`ThreadLocalMap.remove`方法清除线程共享变量；<br />既然Key是弱引用，那么要做的事，就是在调用ThreadLocal的`get()`、`set()`方法时完成后再调用`remove`方法，将Entry节点和Map的引用关系移除，这样整个Entry对象在GC Roots分析后就变成不可达了，下次GC的时候就可以被回收。<br />2、JDK建议ThreadLocal定义为`private static`，这样ThreadLocal的弱引用问题则不存在了。
