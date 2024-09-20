JavaAtomicInteger<br />i++ 不是线程安全的操作，因为它不是一个原子性操作。<br />那么，如果想要达到类似 i++ 的这种效果，应该使用哪些集合或者说工具类呢？<br />在 JDK1.5 之前，为了确保在多线程下对某基本数据类型或者引用数据类型运算的原子性，必须依赖于外部关键字 `synchronized`，但是这种情况在 JDK1.5 之后发生了改观，当然依然可以使用 synchronized 来保证原子性，这里所说的一种线程安全的方式是原子性的工具类，比如 **「**`**AtomicInteger**`**、**`**AtomicBoolean**`**」** 等。这些原子类都是线程安全的工具类，他们同时也是 Lock-Free 的。下面就来一起认识一下这些工具类以及 Lock - Free 是个什么概念。
<a name="HiZ9t"></a>
## 了解 `AtomicInteger`
`AtomicInteger` 是 JDK1.5 新添加的工具类，首先来看一下它的继承关系<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512951974-a56b4fca-ff50-4cad-ac11-38c0697ea1c5.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u86327406&originHeight=57&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6584852f-7b6c-4371-9ea5-0ca83e182b8&title=)<br />与 int 的包装类 Integer 一样，都是继承于 `Number` 类的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512952072-66308888-29ad-4e60-a989-eb393a9ff7d0.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=ubafa3bbe&originHeight=69&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0b1a259e-fd22-4c9c-96bc-e9224864bc7&title=)<br />这个 Number 类是基本数据类型的包装类，一般和数据类型有关的对象都会继承于 Number 类。<br />它的继承体系很简单，下面来看一下它的基本属性和方法
<a name="mYfQM"></a>
### AtomicInteger 的基本属性
`AtomicInteger` 的基本属性有三个<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512951892-d3997de3-35ad-4661-83d6-b6439f3e6413.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=ud33a626c&originHeight=422&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u16a7751e-dfde-4394-a10e-2fe600ee2fc&title=)<br />`Unsafe` 是 sun.misc 包下面的类，`AtomicInteger` 主要是依赖于 sun.misc.Unsafe 提供的一些 `native` 方法保证操作的原子性。<br />Unsafe 的 `objectFieldOffset` 方法可以获取成员属性在内存中的地址相对于对象内存地址的偏移量。说得简单点就是找到这个变量在内存中的地址，便于后续通过内存地址直接进行操作，这个值就是 value。<br />value 就是 `AtomicIneger` 的值。
<a name="Yanoj"></a>
### `AtomicInteger` 的构造方法
继续往下看，`AtomicInteger` 的构造方法只有两个，一个是无参数的构造方法，无参数的构造方法默认的 value 初始值是 0 ，带参数的构造方法可以指定初始值。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512951888-b606540e-8d32-4661-aabf-39708ea4a9c7.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u4c58f28b&originHeight=305&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0bda99ad-c756-44ba-8517-c42e4787c36&title=)
<a name="QJCvL"></a>
### `AtomicInteger` 中的方法
下面就来聊一下 `AtomicInteger` 中的方法。
<a name="X2BTK"></a>
#### Get  和 Set
首先来看一下最简单的 get 、set 方法：<br />`get()` : 获取当前 `AtomicInteger` 的值<br />`set()` : 设置当前 `AtomicInteger` 的值<br />`get()` 可以原子性的读取 `AtomicInteger`  中的数据，`set()` 可以原子性的设置当前的值，因为 `get()` 和 `set()` 最终都是作用于 value 变量，而 value 是由 `volatile` 修饰的，所以 get 、set 相当于都是对内存进行读取和设置。如下图所示<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512951961-6b266860-b1fe-4bfa-9bca-78496b594b78.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u3fcc16f5&originHeight=612&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4ba4631d-9d1c-4320-80af-0c4e9302104&title=)<br />上面提到了 `i++` 和` i++` 的非原子性操作，可以使用 `AtomicInteger` 中的方法进行替换。
<a name="BkDmf"></a>
#### `Incremental` 操作
`AtomicInteger` 中的 `Incremental` 相关方法可以满足需求

- `getAndIncrement()` :  原子性的增加当前的值，并把结果返回。相当于 `i++` 的操作。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512952409-8d8c513e-381a-4377-b5f8-444483168d2d.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u8cb45965&originHeight=144&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6624bbec-c6f6-46f7-af00-adb1ec79bd6&title=)<br />为了验证是不是线程安全的，用下面的例子进行测试
```java
public class TAtomicTest implements Runnable{

    AtomicInteger atomicInteger = new AtomicInteger();

    @Override
    public void run() {
        for(int i = 0;i < 10000;i++){
            System.out.println(atomicInteger.getAndIncrement());
        }
    }
    public static void main(String[] args) {

        TAtomicTest tAtomicTest = new TAtomicTest();

        Thread t1 = new Thread(tAtomicTest);
        Thread t2 = new Thread(tAtomicTest);
        t1.start();
        t2.start();
    }

}
```
通过输出结果会发现它是一个线程安全的操作，可以修改 i 的值，但是最后的结果仍然是 i - 1，因为先取值，然后再 + 1，它的示意图如下。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512952355-543abb1b-c474-4d17-a932-ec4f4b7ce979.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=ufcbbc204&originHeight=873&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud5f9f6b4-3231-4b33-b211-f614d47f3f5&title=)

- `incrementAndGet` 与此相反，首先执行 + 1 操作，然后返回自增后的结果，该操作方法能够确保对 value 的原子性操作。如下图所示

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512952441-1e918410-e6fd-4ab1-984a-8ad3b07bd31e.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u4d8bbb30&originHeight=841&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uacbbcea1-344f-4779-ae69-f132eb07a22&title=)
<a name="bUtQ5"></a>
#### `Decremental` 操作
与此相对，`x--` 或者 `x = x - 1` 这样的自减操作也是原子性的。仍然可以使用 `AtomicInteger` 中的方法来替换

- `getAndDecrement `: 返回当前类型的 int 值，然后对 value 的值进行自减运算。下面是测试代码
```java
class TAtomicTestDecrement implements Runnable{

    AtomicInteger atomicInteger = new AtomicInteger(20000);

    @Override
    public void run() {
        for(int i = 0;i < 10000 ;i++){
            System.out.println(atomicInteger.getAndDecrement());
        }
    }

    public static void main(String[] args) {

        TAtomicTestDecrement tAtomicTest = new TAtomicTestDecrement();

        Thread t1 = new Thread(tAtomicTest);
        Thread t2 = new Thread(tAtomicTest);
        t1.start();
        t2.start();

    }

}
```
下面是 `getAndDecrement` 的示意图<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512952589-7740dd4d-18b5-4b9d-a572-e574120810be.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=ucaa055d4&originHeight=891&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u27ebe917-acea-45e4-a2b7-eeb06efb71d&title=)

- `decrementAndGet`：同样的，`decrementAndGet` 方法就是先执行递减操作，然后再获取 value 的值，示意图如下

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512952628-2c57b633-6a4c-42ef-a7c5-a7bc4db73f4b.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u677d4285&originHeight=836&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc875415a-791c-40a3-8ae2-4d5bcd3eb39&title=)
<a name="OoZ5e"></a>
#### `LazySet` 方法
`volatile` 有内存屏障知道吗？<br />内存屏障是啥？<br />内存屏障，也称内存栅栏，内存栅障，屏障指令等， 是一类同步屏障指令，是 CPU 或编译器在对内存随机访问的操作中的一个同步点，使得此点之前的所有读写操作都执行后才可以开始执行此点之后的操作。也是一个让CPU 处理单元中的内存状态对其它处理单元可见的一项技术。<br />CPU 使用了很多优化，使用缓存、指令重排等，其最终的目的都是为了性能，也就是说，当一个程序执行时，只要最终的结果是一样的，指令是否被重排并不重要。所以指令的执行时序并不是顺序执行的，而是乱序执行的，这就会带来很多问题，这也促使着内存屏障的出现。<br />语义上，内存屏障之前的所有写操作都要写入内存；内存屏障之后的读操作都可以获得同步屏障之前的写操作的结果。因此，对于敏感的程序块，写操作之后、读操作之前可以插入内存屏障。<br />内存屏障的开销非常轻量级，但是再小也是有开销的，LazySet 的作用正是如此，它会以普通变量的形式来读写变量。<br />也可以说是：**「懒得设置屏障了」**
<a name="NqCuf"></a>
#### `GetAndSet` 方法
以原子方式设置为给定值并返回旧值。<br />它的源码就是调用了一下 unsafe 中的 `getAndSetInt` 方法，如下所示<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512952868-8cc4c15e-4dbe-4d21-89ab-6f780071b655.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u9b033c57&originHeight=333&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u07bb830a-c8cc-4664-9fd4-c0a5bc27a20&title=)<br />就是先进行循环，然后调用 `getIntVolatile` 方法，这个方法在 cpp 中没有找到。<br />循环直到 `compareAndSwapInt` 返回 false，这就说明使用 CAS 并没有更新为新的值，所以 var5 返回的就是最新的内存值。
<a name="SAbRV"></a>
#### CAS 方法
一直常说的 CAS 其实就是 `CompareAndSet` 方法，这个方法顾名思义，就是 **「比较并更新」** 的意思，当然这是字面理解，字面理解有点偏差，其实人家的意思是先比较，如果满足那么再进行更新。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512952925-5bbd3852-99e1-4759-9e47-89459c525efd.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u1a62433f&originHeight=133&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u85ca021e-4073-4fc8-81fa-d3bf4802fa0&title=)<br />上面给出了 CAS Java 层面的源码，JDK 官方给它的解释就是 **「如果当前值等于 expect 的值，那么就以原子性的方式将当前值设置为 update 给定值」**，这个方法会返回一个 boolean 类型，如果是 true 就表示比较并更新成功，否则表示失败。<br />CAS 同时也是一种无锁并发机制，也称为 Lock Free，所以 Lock Free 很高大上吗？并没有。<br />下面构建一个加锁解锁的 CASLock
```java
class CASLock {

    AtomicInteger atomicInteger = new AtomicInteger();
    Thread currentThread = null;

    public void tryLock() throws Exception{

        boolean isLock = atomicInteger.compareAndSet(0, 1);
        if(!isLock){
            throw new Exception("加锁失败");
        }

        currentThread = Thread.currentThread();
        System.out.println(currentThread + " tryLock");

    }

    public void unlock() {

        int lockValue = atomicInteger.get();
        if(lockValue == 0){
            return;
        }
        if(currentThread == Thread.currentThread()){
            atomicInteger.compareAndSet(1,0);
            System.out.println(currentThread + " unlock");
        }
    }

    public static void main(String[] args) {

        CASLock casLock = new CASLock();

        for(int i = 0;i < 5;i++){

            new Thread(() -> {
                try {
                    casLock.tryLock();
                    Thread.sleep(10000);
                } catch (Exception e) {
                    e.printStackTrace();
                }finally {
                    casLock.unlock();
                }
            }).start();
        }

    }
}
```
在上面的代码中，构建了一个 CASLock，在 `tryLock` 方法中，先使用 CAS 方法进行更新，如果更新不成功则抛出异常，并把当前线程设置为加锁线程。在 `unLock` 方法中，先判断当前值是否为 0 ，如果是 0 就是期望看到的结果，直接返回。否则是 1，则表示当前线程还在加锁，再来判断一下当前线程是否是加锁线程，如果是则执行解锁操作。<br />那么上面提到的 `compareAndSet`，它其实可以解析为如下操作
```java
// 伪代码

// 当前值
int v = 0;
int a = 0;
int b = 1;

if(compare(0,0) == true){
  set(0,1);
}
else{
  // 继续向下执行
}
```
也可以拿生活场景中的买票举例子，去景区旅游肯定要持票才能进，如果拿着是假票或者不符合景区的票肯定是能够被识别出来的，如果没有拿票肯定进不去景区。<br />`compareAndSet` 的示意图<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953135-5b146330-771e-4272-b7ae-a24226cdbee6.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=uf83833d2&originHeight=612&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u693aa102-c6eb-4973-b732-8a2cf8512a8&title=)

- `weakCompareAndSet`：非常认真看了好几遍，发现 JDK1.8 的这个方法和 `compareAndSet` 方法完全一摸一样。

但是真的是这样么？并不是，JDK 源码很博大精深，才不会设计一个重复的方法， JDK 团队也不是会犯这种低级团队，但是原因是什么呢？<br />《Java 高并发详解》这本书给出了一个答案
> 通过源码我们不难发现这两个方法的实现完全一样，那么为什么要有这两个方法呢？其实在JDK 1.6版本以前双方的实现是存在差异的，compareAndSet 方法的底层主要是针对Intel x86 架构下的CPU指令 CAS:cmpxchg（sparc-TSO，ia64 的 CPU 架构也支持），但是 ARM CPU 架构下的类似指令为LL/SC:ldrex/strex（ARM架构下的 CPU 主要应用于当下的移动互联网设备，比如在智能手机终端设备中，高通骁龙、华为麒麟等一系列都是基于 ARM 架构和指令集下的 CPU 产品），或许在运行 Android 的 JVM 设备上这两个方法底层存在着。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953216-c8d41957-7ef7-4091-a115-894d7cde8e65.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u23361d80&originHeight=262&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u95b7cd99-adfe-4e9a-9d53-ade6f396456&title=)
<a name="sUfB1"></a>
#### `AddAndGet`
`AddAndGet` 和 `getAndIncrement`、`getAndAdd`、`incrementAndGet` 等等方法都是使用了 do ... while  + CAS 操作，其实也就相当于是一个自旋锁，如果 CAS 修改成功就会一直循环，修改失败才会返回。示意图如下<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953327-9fe72932-7bc8-4d84-8f74-3dc4afa1d988.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=ua9505fcd&originHeight=808&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub22b4818-da05-4dd3-a220-5326fe1d576&title=)
<a name="wpvgp"></a>
### 深入 `AtomicInteger`
上面探讨了 `AtomicInteger` 的具体使用，同时知道 `AtomicInteger` 是依靠 volatile 和 CAS 来保证原子性的，那么下面就来分析一下为什么 CAS 能够保证原子性，它的底层是什么？`AtomicInteger` 与乐观锁又有什么关系呢？
<a name="BVGoa"></a>
#### `AtomicInteger` 的底层实现原理
再来瞧瞧这个可爱的 `compareAndSetL(CAS)` 方法，为什么就这两行代码就保证原子性了？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953380-90d7f790-6e11-42f6-bc8f-8b10e230b81d.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u45ca5b55&originHeight=125&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaf62dfaa-d91f-4e90-a364-a6c62794bde&title=)<br />可以看到，这个 CAS 方法相当于是调用了 unsafe 中的 `compareAndSwapInt` 方法，进到 unsafe 方能发中看一下具体实现。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953556-0bb26b44-29bb-407e-8849-d661f2ffb8ad.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u2c4400d1&originHeight=58&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u01660090-9bde-4a27-93bb-370407b406c&title=)<br />`compareAndSwapInt` 是 sun.misc 中的方法，这个方法是一个 `native` 方法，它的底层是 C/C++ 实现的，所以需要看 C/C++ 的源码。<br />知道 C/C++ 的牛逼之处了么。使用 Java 就是玩应用和架构的，C/C++ 是玩服务器、底层的。<br />`compareAndSwapInt` 的源码在 jdk8u-dev/hotspot/src/share/vm/prims/unsafe.app 路径下，它的源码实现是<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953695-e913d016-b64f-4e01-997b-7f108348fcc9.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=uf366f521&originHeight=44&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud32dd54d-4534-4e1c-a6cd-460114f34a9&title=)<br />也就是 `Unsafe_CompareAndSwapInt` 方法，找到这个方法<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953829-1e00e843-402a-420b-8c35-cdba5f286a4b.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u494eb291&originHeight=151&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u84df3033-b006-4200-9b3f-d2320e31bd0&title=)<br />C/C++ 源码也看不懂，但是这不妨碍找到关键代码 `Atomic::cmpxchg` ，cmpxchg 是 x86 CPU 架构的汇编指令，它的主要作用就是比较并交换操作数。继续往下跟找一下这个指令的定义。<br />会发现对应不同的 os，其底层实现方式不一样<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953848-79a77c26-b2df-4298-a6b4-1df973f6a975.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u99a3ecc7&originHeight=421&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u229705cc-76aa-4155-b69b-c0e7cd3c0b1&title=)<br />找到 Windows 的实现方式如下<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953961-90a56d4a-142b-4b41-9eb5-9440cd1f9ce4.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u4c47ac0b&originHeight=583&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud0d3e41e-f927-4198-9c01-7c6c6043506&title=)<br />继续向下找，它其实定义的是第 216 行的代码，找进去<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512953945-edb0356d-3e16-424f-8cab-03d984ac35ab.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u80a3745f&originHeight=251&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uab306d7a-6c72-4f71-92a6-ad9da4369d6&title=)<br />此时就需要汇编指令和寄存器相关的知识了。<br />上面的 `os::is-MP()` 是多处理操作系统的接口，下面是 __asm ，它是 C/C++ 的关键字，用于调用内联汇编程序。<br />__asm 中的代码是汇编程序，大致来说就是把 dest、exchange_value 、compare_value 的值都放在寄存器中，下面的 LOCK_IF_MP 中代码的大致意思就是<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512954125-76347c27-e742-4f41-8631-f905eb168fad.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=uc7a6f622&originHeight=339&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2785a34c-4469-4f58-a7cc-06cb925f59e&title=)<br />如果是多处理器的话就会执行 lock，然后进行比较操作。其中的 cmp 表示比较，mp 表示的就是 MultiProcess，je 表示相等跳转，L0 表示的是标识位。<br />回到上面的汇编指令，可以看到，CAS 的底层就是 cmpxchg 指令。
<a name="YacDR"></a>
#### 乐观锁
有没有这个疑问，为什么 `AtomicInteger` 可以获取当前值，那为什么还会出现 `expectValue` 和 value 不一致的情况呢？<br />因为 `AtomicInteger` 只是一个原子性的工具类，它不具有排他性，它不像是 `synchronized` 或者是 lock 一样具有互斥和排他性，还记得 `AtomicInteger` 中有两个方法 get 和 set 吗？它们只是用 volatile 修饰了一下，而 volatile 不具有原子性，所以可能会存在 expectValue 和 value 的当前值不一致的情况，因此可能会出现重复修改。<br />针对上面这种情况的解决办法有两种，一种是使用 `synchronized` 和 lock 等类似的加锁机制，这种锁具有独占性，也就是说同一时刻只能有一个线程来进行修改，这种方式能够保证原子性，但是相对开销比较大，这种锁也叫做悲观锁。另外一种解决办法是使用版本号或者是 CAS 方法。<br />**「版本号」**<br />版本号机制是在数据表中加上一个 version 字段来实现的，表示数据被修改的次数，当执行写操作并且写入成功后，`version = version + 1`，当线程 A 要更新数据时，在读取数据的同时也会读取 version 值，在提交更新时，若刚才读取到的 version 值为当前数据库中的 version 值相等时才更新，否则重试更新操作，直到更新成功。<br />**「CAS 方法」**<br />还有一种方式就是 CAS 了，上面用了大量的篇幅来介绍 CAS 方法，那么认为现在已经对其运行机制有一定的了解了，就不再阐述它的运行机制了。<br />任何事情都是有利也有弊，软件行业没有完美的解决方案只有最优的解决方案，所以乐观锁也有它的弱点和缺陷，那就是 ABA 问题。
<a name="tV7TX"></a>
#### ABA 问题
ABA 问题说的是，如果一个变量第一次读取的值是 A，准备好需要对 A 进行写操作的时候，发现值还是 A，那么这种情况下，能认为 A 的值没有被改变过吗？可以是由 A -> B -> A 的这种情况，但是 `AtomicInteger` 却不会这么认为，它只相信它看到的，它看到的是什么就是什么。举个例子来说<br />假如现在有一个单链表，如下图所示<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512954372-bd4a08af-b5f3-40fd-a70d-679ffaee0925.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u947989fb&originHeight=310&originWidth=604&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc8fed02c-b681-4d3e-b516-f01ea9ec663&title=)<br />`A.next = B` ，`B.next = null`，此时有两个线程 T1 和 T2 分别从单链表中取出 A ，由于一些特殊原因，T2 把 A 改为 B ，然后又改为 A ，此时 T1 执行 CAS 方法，发现单链表仍然是 A ，就会执行 CAS 方法，虽然结果没错，但是这种操作会造成一些潜在的问题。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512954464-4bb71063-f7b8-47ac-87a9-cb63f039004b.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=uaf82e4fa&originHeight=671&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u34f185e2-07fa-4356-b6d7-f1ca90fe64d&title=)<br />此时还是一个单链表，两个线程 T1 和 T2 分别从单链表中取出 A ，然后 T1 把链表改为 ACD 如下图所示<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512954460-b0a3b5d9-4c23-441e-8d2e-2364a79436d9.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u451438ae&originHeight=826&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4babe008-fadf-405c-8d62-2747860121f&title=)<br />此时 T2，发现内存值还是 A ，就会把 A 的值尝试替换为 B ，因为 B 的引用是 null，此时就会造成 C、D 处于游离态<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512954655-9ae6324c-74ca-48b1-ad28-fcb09a2b14aa.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=uad4af0de&originHeight=405&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6e7b13fe-d637-44e2-97ca-34c19f641f6&title=)<br />JDK 1.5 以后的 `AtomicStampedReference`类就提供了此种能力，其中的 `compareAndSet` 方法就是首先检查当前值是否等于预期值，判断的标准就是当前引用和邮戳分别和预期引用和邮戳相等，如果全部相等，则以原子方式设置为给定的更新值。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512954686-fbbe939f-bd85-4f01-b13b-22d94b17db16.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u7e08c37b&originHeight=699&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua75928cd-c2a7-4731-b0ec-5b9e822d88c&title=)<br />好了，上面就是 Java 代码流程了，看到 native 知道又要撸 cpp 了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512954761-ed58aae9-32bf-4ef0-9690-453e97a6dfaf.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u925e2b10&originHeight=231&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6a8d8da8-af93-4e3c-bcb9-fb56952b37d&title=)<br />简单解释一下就是 `UnsafeWrapper` 就是包装器，换个名字而已。然后经过一些 JNI 的处理，因为 `compareAndSwapOject` 比较的是引用，所以需要经过 C++ 面向对象的转换。最主要的方法是 `atomic_compare_exchange_oop`<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657512955058-a49def7f-11da-4d78-84a4-391d2b65c5b7.png#clientId=u7a8a0a8c-9c5f-4&from=paste&id=u00f0e616&originHeight=637&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4fbe0579-292d-4554-aadf-b98b2e57f14&title=)<br />可以看到，又出现了熟悉的词汇 `cmpxchg`，也就是说 `compareAndSwapOject` 使用的还是 `cmpxchg` 原子性指令，只是它经过了一系列转换。
