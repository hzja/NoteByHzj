Java
<a name="MM3Lv"></a>
## 1、延迟队列
在 Java 中有类型众多的集合。那么听说过 DelayQueue 吗？它是一个特殊类型的 Java 集合，允许根据元素的延迟时间对其进行排序。坦白来讲，这是一个非常有意思的类。尽管 DelayQueue 类是 Java 集合的成员之一，但是它位于 java.util.concurrent 包中。它实现了 BlockingQueue 接口。只有当元素的时间过期时，才能从队列中取出。<br />要使用这个集合，首先，类需要实现 Delayed 接口的 `getDelay` 方法。当然，它不一定必须是类，也可以是 Java Record。
```java
public record DelayedEvent(long startTime, String msg) implements Delayed {

    public long getDelay(TimeUnit unit) {
        long diff = startTime - System.currentTimeMillis();
        return unit.convert(diff, TimeUnit.MILLISECONDS);
    }

    public int compareTo(Delayed o) {
        return (int) (this.startTime - ((DelayedEvent) o).startTime);
    }

}
```
假设想要把元素延迟 10 秒钟，那么只需要在 `DelayedEvent` 类上将时间设置成当前时间加上 10 秒钟即可。
```java
final DelayQueue<DelayedEvent> delayQueue = new DelayQueue<>();
final long timeFirst = System.currentTimeMillis() + 10000;
delayQueue.offer(new DelayedEvent(timeFirst, "1"));
log.info("Done");
log.info(delayQueue.take().msg());
```
对于上面的代码，能够看到什么输出呢？如下所示。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645625798099-7ec984a1-9d1f-4dbf-b621-48ffdd0555cd.png#clientId=u4cc2c151-f378-4&from=paste&id=u1bfb0d3f&originHeight=181&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua6d62e73-9183-43ef-a504-7dd6a5f570c&title=)
<a name="zychD"></a>
## 2、时间格式中支持显示一天中的时段
这个 Java 特性对于大多数人来讲并没有太大的用处，Java 8 对时间处理 API 做了很多的改进。从这个版本的 Java 开始，在大多数情况下，都不需要任何额外的库来处理时间了，比如 Joda Time。可能想象不到，从 Java 16 开始，甚至可以使用标准的格式化器来表达一天中的时段，也就是“in the morning”或者“in the afternoon”。这是一个新的格式模式，叫做 B。
```java
String s = DateTimeFormatter
  .ofPattern("B")
  .format(LocalDateTime.now());
System.out.println(s);
```
如下是运行的结果。当然，每个人的结果可能会因时间不同而有所差异。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645625798098-501e9838-9e80-4130-8b46-a16993709dbe.png#clientId=u4cc2c151-f378-4&from=paste&id=uf957535a&originHeight=102&originWidth=602&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2adb907e-3370-49d5-bc39-1bc9c3dff42&title=)<br />这个格式为什么叫做 B。事实上，对于这种类型的格式来讲，它不是最直观的名字。但也许下面的表格能够解决所有疑惑。它是 DateTimeFormatter 能够处理的模式字符和符号的片段。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645625798095-f3b14a87-0ed4-46a4-bf29-2d2fb6fbf61b.png#clientId=u4cc2c151-f378-4&from=paste&id=ub2582c6b&originHeight=903&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u79df8a50-04f6-435e-a88e-d248578fb9c&title=)
<a name="ADQos"></a>
## 3、StampedLock
Java Concurrent 是最有趣的 Java 包之一。同时，它也是一个不太为开发者所熟知的包，当开发人员主要使用 web 框架的时候更是如此。有多少人曾经在 Java 中使用过锁呢？锁是一种比 synchronized 块更灵活的线程同步机制。从 Java 8 开始，可以使用一种叫做 StampedLock 的新锁。StampedLock 是 ReadWriteLock 的一个替代方案。它允许对读操作进行乐观的锁定。而且，它的性能比 ReentrantReadWriteLock 更好。<br />假设有两个线程。第一个线程更新一个余额，而第二个线程则读取余额的当前值。为了更新余额，当然需要先读取其当前值。在这里，需要某种同步机制，假设第一个线程在同一时间内多次运行。第二个线程阐述了如何使用乐观锁来进行读取操作。
```java
StampedLock lock = new StampedLock();
Balance b = new Balance(10000);
Runnable w = () -> {
   long stamp = lock.writeLock();
   b.setAmount(b.getAmount() + 1000);
   System.out.println("Write: " + b.getAmount());
   lock.unlockWrite(stamp);
};
Runnable r = () -> {
   long stamp = lock.tryOptimisticRead();
   if (!lock.validate(stamp)) {
      stamp = lock.readLock();
      try {
         System.out.println("Read: " + b.getAmount());
      } finally {
         lock.unlockRead(stamp);
      }
   } else {
      System.out.println("Optimistic read fails");
   }
};
```
现在，同时运行这两个线程 50 次。它的结果应该是符合预期的，最终的余额是 60000。
```java
ExecutorService executor = Executors.newFixedThreadPool(10);
for (int i = 0; i < 50; i++) {
   executor.submit(w);
   executor.submit(r);
}
```
<a name="CAB9D"></a>
## 4、并发累加器
在 Java Concurrent 包中，有意思的并不仅仅有锁，另外一个很有意思的东西是并发累加器（concurrent accumulator）。也有并发的加法器（concurrent adder），但它们的功能非常类似。LongAccumulator（也有 DoubleAccumulator）会使用一个提供给它的函数更新一个值。在很多场景下，它能实现无锁的算法。当多个线程更新一个共同的值的时候，它通常会比 AtomicLong 更合适。<br />看一下它是如何运行的。要创建它，需要在构造函数中设置两个参数。第一个参数是一个用于计算累加结果的函数。通常情况下，会使用 sum 方法。第二个参数表示累积器的初始值。<br />现在创建一个初始值为 10000 的 LongAccumulator，然后从多个线程调用 `accumulate()` 方法。最后的结果是什么呢？如果回想一下的话，做的事情和上一节完全一样，但这一次没有任何锁。
```java
LongAccumulator balance = new LongAccumulator(Long::sum, 10000L);
Runnable w = () -> balance.accumulate(1000L);

ExecutorService executor = Executors.newFixedThreadPool(50);
for (int i = 0; i < 50; i++) {
   executor.submit(w);
}

executor.shutdown();
if (executor.awaitTermination(1000L, TimeUnit.MILLISECONDS))
   System.out.println("Balance: " + balance.get());
assert balance.get() == 60000L;
```
<a name="AoniO"></a>
## 5、十六进制格式
关于这个特性并没有什么大的故事。有时需要在十六进制的字符串、字节或字符之间进行转换。从 Java 17 开始，可以使用 HexFormat 类实现这一点。只要创建一个 HexFormat 的实例，然后就可以将输入的 byte 数组等格式化为十六进制字符串。还可以将输入的十六进制字符串解析为字节数组，如下所示。
```java
HexFormat format = HexFormat.of();

byte[] input = new byte[] {127, 0, -50, 105};
String hex = format.formatHex(input);
System.out.println(hex);

byte[] output = format.parseHex(hex);
assert Arrays.compare(input, output) == 0;
```
<a name="xDW35"></a>
## 6、数组的二分查找
假设想在排序的数组中插入一个新的元素。如果数组中已经包含该元素的话，`Arrays.binarySearch()` 会返回该搜索键的索引，否则，它返回一个插入点，可以用它来计算新键的索引：`-(insertion point)-1`。此外，在 Java 中，`binarySearch` 方法是在一个有序数组中查找元素的最简单和最有效的方法。<br />考虑下面的例子。有一个输入的数组，其中有四个元素，按升序排列。在这个数组中插入数字 3，下面的代码展示了如何计算插入点的索引。
```java
int[] t = new int[] {1, 2, 4, 5};
int x = Arrays.binarySearch(t, 3);

assert ~x == 2;
```
<a name="C5VWr"></a>
## 7、Bit Set
如果需要对 bit 数组进行一些操作该怎么办呢？是不是会使用 `boolean[]` 来实现呢？其实，有一种更有效、更节省内存的方法来实现。这就是 BitSet 类。BitSet 类允许存储和操作 bit 的数组。与 `boolean[]` 相比，它消耗的内存要少 8 倍。可以对数组进行逻辑操作，例如：and、or、xor。<br />比方说，有两个 bit 的数组，对它们执行 xor 操作。为了做到这一点，需要创建两个 BitSet 的实例，并在实例中插入样例元素，如下所示。最后，对其中一个 BitSet 实例调用 xor 方法，并将第二个 BitSet 实例作为参数。
```java
BitSet bs1 = new BitSet();
bs1.set(0);
bs1.set(2);
bs1.set(4);
System.out.println("bs1 : " + bs1);

BitSet bs2 = new BitSet();
bs2.set(1);
bs2.set(2);
bs2.set(3);
System.out.println("bs2 : " + bs2);

bs2.xor(bs1);
System.out.println("xor: " + bs2);
```
如下是运行上述代码的结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645625798101-577c014b-fbd9-49c7-a05d-a425583bc064.png#clientId=u4cc2c151-f378-4&from=paste&id=u89b94b1f&originHeight=206&originWidth=682&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc1708e02-ed1a-4d12-b58f-39caad3004e&title=)
<a name="kP6CS"></a>
## 8、Phaser
最后介绍本文最后一个有趣的 Java 特性。和其他一些样例一样，它也是 Java Concurrent 包的元素，被称为 Phaser。它与更知名的 CountDownLatch 相当相似。然而，它提供了一些额外的功能。它允许设置在继续执行之前需要等待的线程的动态数量。在 Phaser 中，已定义数量的线程需要在进入下一步执行之前在屏障上等待。得益于此，可以协调多个阶段的执行。<br />在下面的例子中，设置了一个具有 50 个线程的屏障，在进入下一个执行阶段之前，需要到达该屏障。然后，创建一个线程，在 Phaser 实例上调用 `arriveAndAwaitAdvance()` 方法。它会一直阻塞线程，直到所有的 50 个线程都到达屏障。然后，它进入 phase-1，同样会再次调用 `arriveAndAwaitAdvance()` 方法。
```java
Phaser phaser = new Phaser(50);
Runnable r = () -> {
   System.out.println("phase-0");
   phaser.arriveAndAwaitAdvance();
   System.out.println("phase-1");
   phaser.arriveAndAwaitAdvance();
   System.out.println("phase-2");
   phaser.arriveAndDeregister();
};

ExecutorService executor = Executors.newFixedThreadPool(50);
for (int i = 0; i < 50; i++) {
   executor.submit(r);
}
```
如下是执行上述代码的结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645625798078-6553a3d3-c658-4c6b-8e6b-6104b365e45d.png#clientId=u4cc2c151-f378-4&from=paste&id=u24b30ec4&originHeight=688&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7f623742-0876-4022-ac2b-49c076cf2b6&title=)
