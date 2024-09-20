JavaThreadLocal<br />每个线程都有自己的一个 `ThreadLocalMap`，`ThreadLocal` 持有的数据就是存在这个 Map 里的（`Thread.ThreadLocalMap threadLocals`），所以能够实现线程隔离，毕竟每个线程的 ThreadLocalMap 都是不一样的<br />那**如果子线程想要拿到父线程的中的 ThreadLocal 值怎么办呢**？<br />比如会有以下的这种代码的实现。在子线程中调用 get 时，拿到的 Thread 对象是当前子线程对象，对吧，每个线程都有自己独立的 `ThreadLocal`，那么当前子线程的 `ThreadLocalMap` 是 null 的（而父线程，也就是 main 线程中的 `ThreadLocalMap` 是有数据的），所以得到的 value 也是 null
```java
public class ThreadLocalTest {
    private static ThreadLocal<String> threadLocal = new ThreadLocal<String>();

    public static void main(String[] args) throws  Exception{
        threadLocal.set("飞天小牛肉");
        System.out.println("父线程的值："+ threadLocal.get());
        new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("子线程的值："+ threadLocal.get());
            }
        }).start();

        Thread.sleep(2000);
    }
}
```
结果输出如下：<br />要如何解决这个问题呢？<br />先来从 Thread 类中找找思路：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668430948118-e7cdabde-6434-4607-9a90-01cbe403e6f1.png#averageHue=%2327333a&clientId=uedf9e51d-ba23-4&from=paste&id=u7131234e&originHeight=1064&originWidth=863&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u370cc570-92aa-48ce-b104-9d3a0190626&title=)<br />可以发现，在 `ThreadLocalMap threadLocals` 的下方，还有一个 ThreadLocalMap 变量 `inherittableThreadLocals`，`inherit` 翻译为**继承**<br />先看下这个变量的注释：_InheritableThreadLocal values pertaining to this thread. This map is maintained by the InheritableThreadLocal class._<br />这里出现了一个渣渣辉都从未体验过的传新类：`InheritableThreadLocal`<br />翻译一下注释，大概就是，如果使用 `InheritableThreadLocal`，那么保存的数据都已经不在原来的 `ThreadLocal.ThreadLocalMap threadLocals` 里面了，而是在一个新的 `ThreadLocal.ThreadLocalMap inheritableThreadLocals` 变量中了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668430947989-a4716a8e-265c-4c2a-ab80-a41ad789b3d0.png#averageHue=%2327343a&clientId=uedf9e51d-ba23-4&from=paste&id=uba0b0fb7&originHeight=894&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0dd4446e-a0ef-4984-ac87-8619ff201d8&title=)<br />所以，如果想让上面那段代码中，子线程能够拿到父线程的 `ThreadLocal` 值，只需要把 ThreadLocal 声明改为 `InheritableThreadLocal` 就可以了<br />下面具体来看下 `InheritableThreadLocal` 是怎么做到父子线程传值的。<br />首先看下 `new Thread` 的时候线程都做了些什么 `Thread#init()`
```java
private void init(ThreadGroup g, Runnable target, String name, long stackSize, AccessControlContext acc) {
    // 省略部分代码
    Thread parent = currentThread();

    if (inheritThreadLocals && parent.inheritableThreadLocals != null)
        // copy父线程的 map，创建一个新的 map 赋值给当前线程的inheritableThreadLocals
        this.inheritableThreadLocals =
            ThreadLocal.createInheritedMap(parent.inheritableThreadLocals);

    // 省略部分代码
}
```
核心其实就是上面几句代码，如果设置了 `inheritableThreadLocals` 变量，那么 Thread 就会**把父线程 **`**ThreadLocal threadLocals**`** 中的所有数据都 copy 到子线程的 **`**InheritableThreadLocal**`** **`**inheritableThreadLocals**`** 中**。<br />而且，copy 调用的 `createInheritedMap` 方法其实是一个浅拷贝函数，key 和 value 都是原来的引用地址，这里所谓的 copy 其实就是把一个 Map 中的数据复制到另一个 Map 中：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668430948107-39f5214b-abb9-42c0-a1d7-c3f4c2518f54.png#averageHue=%2327343b&clientId=uedf9e51d-ba23-4&from=paste&id=u6fd2fd67&originHeight=950&originWidth=991&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u66f60527-5f48-4d21-9578-bd91aecdd5d&title=)<br />至此，大致的解释了 `InheritableThreadLocal` 为什么能解决父子线程传递 `Threadlcoal` 值的问题了，总结下：

1. 在创建 `InheritableThreadLocal` 对象的时候赋值给线程的 `t.inheritableThreadLocals` 变量
2. 在创建新线程的时候会 check 父线程中 `t.inheritableThreadLocals` 变量是否为 null，如果不为 null 则 copy 一份数据到子线程的 `t.inheritableThreadLocals` 成员变量中去
3. `InheritableThreadLocal` 重写了 `getMap(Thread)` 方法，所以 get 的时候，就会从 `t.inheritableThreadLocals` 中拿到 `ThreadLocalMap` 对象，从而实现了可以拿到父线程 ThreadLocal 中的值
