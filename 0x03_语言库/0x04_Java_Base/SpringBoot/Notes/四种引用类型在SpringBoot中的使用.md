Java SpringBoot
<a name="uFLHL"></a>
## 概念介绍
不同的引用类型，主要体现的是对象不同的可达性（reachable）状态和对垃圾收集的影响。
<a name="inLSn"></a>
### 1.  强引用
这个就是创建的普通对象了~  当该对象被显示地赋值为 null 时，或者没有被其他存活的对象继续引用时，它就会成为垃圾收集器的目标，等待被收回
<a name="y8pk9"></a>
### 2.  软引用
软引用( SoftReference )，当内存不足 时会被回收<br />比如<br />![2021-08-11-17-05-48-427928.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628672837743-fb8ef2bf-6096-41ba-b746-7d3d7df1d0d4.png#clientId=u347f44f7-1d98-4&from=ui&id=ubda7d523&originHeight=258&originWidth=794&originalType=binary&ratio=1&size=29388&status=done&style=none&taskId=u21191689-5754-42bd-9801-c4299263e7c)<br />被回收后，这里会打印 null
<a name="k4idq"></a>
### 3.  弱引用
弱引用( WeakReference ) , 当 垃圾回收器 进行垃圾回收时，无论内存足与否，它都会被垃圾回收器回收<br />比如<br />![2021-08-11-17-05-48-788068.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628672837770-24566333-59a9-4707-a12d-b08d9844d46a.png#clientId=u347f44f7-1d98-4&from=ui&id=jyzt6&originHeight=299&originWidth=831&originalType=binary&ratio=1&size=31136&status=done&style=none&taskId=u9e2b3a11-c41f-4325-a00f-e9228b0f423)<br />被回收后，这里也是会打印 null
<a name="uaHVI"></a>
### 4.  虚引用
虚引用( `ReferenceQueue`) , 这个也是随时会被回收，不过它的作用更像一个标记，当对象被回收时，它不为 null ，但是要注意，无论什么时候去调用 虚引用的 `get` 方法，都只能获取到一个 null 值。<br />为一个对象设置虚引用关联的唯一目的就是能在这个对象被收集器回收时收到一个系统通知 —— <<深入理解Java虚拟机>>
```java
User user = new User(1, "Java4ye");
ReferenceQueue<User> userReferenceQueue = new ReferenceQueue<>();
// 创建User对象的虚引用
PhantomReference<User> phantomReference = new PhantomReference<>(user, userReferenceQueue);
// 去掉强引用
user = null;
System.out.println(phantomReference.get());
// 手动触发GC
System.gc();
System.out.println("GC: " + phantomReference.get());
Reference<? extends User> reference = null;
try {
    reference = userReferenceQueue.remove(1000);
} catch (InterruptedException e) {
    e.printStackTrace();
}
if (reference != null) {
    System.out.println("对象User被回收了:");
}
```
<a name="cdCD6"></a>
## 对象可达性
那么 简单介绍完上面的 4 种引用后，再来看看它的可达性~<br />如图~<br />![2021-08-11-17-05-49-102073.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628673207276-84b65110-691b-40c2-81b6-f4fd25f9e143.png#clientId=u347f44f7-1d98-4&from=ui&id=u4e79ab75&originHeight=510&originWidth=687&originalType=binary&ratio=1&size=1053400&status=done&style=none&taskId=u39eba3dc-4400-440e-be94-13783993af8)

1. 强可达：比如 创建一个对象时，创建它的线程对该对象就是强可达
2. 软可达：只能通过软引用访问
3. 弱可达：只能通过弱引用访问
4. 虚可达：当对象没有 强，软，弱 引用关联时，并且 `finalize` 过，就会进入该状态
5. 不可达：意味着该对象可以被清除了。

通过最开始的代码例子和上面的图（双向箭头）还可以发现，软引用和弱引用和强引用这三者间可以进行转换( 通过 `Reference` 的 `get()` 可获取到原对象)，这意味着:<br />对于软引用、弱引用之类，垃圾收集器可能会存在二次确认的问题，以保证处于弱引用状态的对象，没有改变为强引用。<br />在 JDK8 中，还可以通过 指定参数打印引用的相关信息
```java
-XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintReferenceGC
```
在 JDK8 中使用 ParrallelGC 收集的垃圾回收日志
```java
0.403: [GC (Allocation Failure) 0.871: [SoftReference, 0 refs, 0.0000393 secs]0.871: [WeakReference, 8 refs, 0.0000138 secs]0.871: [FinalReference, 4 refs, 0.0000094 secs]0.871:
[PhantomReference, 0 refs, 0 refs, 0.0000085 secs]0.871: JNI Weak Reference, 0.0000071 secs 128286K->128422K(316928K), 0.4683919 secs] [Times:
user=1.17 sys=0.03, real=0.47 secs]
```
再记录下这个点👇<br />通过底层API来达到强引用👍<br />![2021-08-11-17-05-49-341077.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628673098004-5fe99a14-eb08-4a8b-82ef-cd1e3d991f0b.png#clientId=u347f44f7-1d98-4&from=ui&id=ue7fd3240&originHeight=169&originWidth=1080&originalType=binary&ratio=1&size=89558&status=done&style=none&taskId=uf8415789-12c4-4d1f-a0ab-b3368d6a395)
<a name="X830U"></a>
## SpringBoot源码中的使用
在 SpringBoot 源码中看到这个`ConcurrentReferenceHashMap`。那么这个 `ConcurrentReferenceHashMap`  到底有什么作用呢？`ConcurrentReferenceHashMap`能指定所存放对象的引用级别<br />默认情况下是 软引用级别<br />![2021-08-11-17-05-49-658106.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628673107674-fc65a494-07f9-4356-916b-1bf18ce078b1.png#clientId=u347f44f7-1d98-4&from=ui&id=ud51fffa5&originHeight=319&originWidth=930&originalType=binary&ratio=1&size=44979&status=done&style=none&taskId=u33f0243d-deb9-44c7-9f3e-41c2f0843ce)<br />比如 在 SpringBoot自动装配原理探索中提到的 SpringBoot SPI 机制 其中的主角：`SpringFactoriesLoader`<br />源码如下：<br />![2021-08-11-17-05-49-983072.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628673120998-d45622a0-8897-4b60-af3b-d50bd8aa4fbb.png#clientId=u347f44f7-1d98-4&from=ui&id=uc593668e&originHeight=303&originWidth=923&originalType=binary&ratio=1&size=42486&status=done&style=none&taskId=u6508a7fd-1fa2-4f99-8fd2-9377e574104)<br />还有自动配置过程中的注解扫描`AnnotationsScanner`<br />![2021-08-11-17-05-50-597063.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628673120991-3c2283d3-978e-457b-95bc-2a4ffbaab492.png#clientId=u347f44f7-1d98-4&from=ui&id=Ntha2&originHeight=277&originWidth=839&originalType=binary&ratio=1&size=44454&status=done&style=none&taskId=ub98e7f1b-a541-4f1b-8daa-3b21acf2bda)<br />异步任务线程池：`ThreadPoolTaskExecutor`  源码如下： （可以看到这里指明了是 弱引用级别）<br />![2021-08-11-17-05-50-813819.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1628673121030-98d74ce7-6641-4f89-a2e3-bd6792034f0f.png#clientId=u347f44f7-1d98-4&from=ui&id=nOZ91&originHeight=426&originWidth=1006&originalType=binary&ratio=1&size=66519&status=done&style=none&taskId=ue409add9-ac65-47db-8ebd-491d2f22a04)
<a name="PlsoT"></a>
## 总结

1. 看完上面的例子，可以模仿下 SpringBoot 的 `ConcurrentReferenceHashMap`  ，对对象进行一个合理的存储，间接地优化jvm ，提高垃圾回收的效率。
2. 这两个别记错：软引用，内存不足时回收；弱引用，在进行垃圾回收时，不管内存足与否，都会被回收，
