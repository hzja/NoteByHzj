Java 
<a name="g4VFs"></a>
## 前言
写时复制（Copy-on-write，简称COW）是一种计算机程序设计领域的优化策略。<br />其核心思想是，如果有多个调用者同时请求相同资源（如内存或磁盘上的数据存储），他们会共同获取相同的指针指向相同的资源，直到某个调用者试图修改资源的内容时，系统才会真正复制一份专用副本（private copy）给该调用者，而其他调用者所见到的最初的资源仍然保持不变。这个过程对其他的调用者是透明的（transparently）。<br />此作法的主要优点是如果调用者没有修改该资源，就不会有副本（private copy）被建立，因此多个调用者只是读取操作时可以共享同一份资源。<br />COW（奶牛）技术的应用场景很多，Linux通过Copy On Write技术极大地减少了Fork的开销。文件系统通过Copy On Write技术一定程度上保证数据的完整性。数据库服务器也一般采用了写时复制策略，为用户提供一份snapshot。<br />而JDK的`CopyOnWriteArrayList`/`CopyOnWriteArraySet`容器也采用了 COW思想，它是如何工作的是本文讨论的重点。
<a name="pFCM2"></a>
## `Vector`和`synchronizedList`
`ArrayList`是线程不安全的，而`Vector`是线程安全的容器。查看源码可以知道，`Vector`之所以线程安全，是因为它几乎在每个方法声明处都加了`synchronized`关键字来使整体方法原子化。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624027680401-bb75773f-8390-4638-a4c5-62fbe973d888.png#clientId=u70208647-93aa-4&from=paste&id=u0788b23e&originHeight=849&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=ub0e0c69a-d7fc-4761-84d8-60c544b525a)<br />另外，使用`Collections.synchronizedList(new ArrayList())`修饰后，新建出来的`ArrayList`也是安全的，它是如何实现的呢？查看源码发现，它也是几乎在每个方法都加上`synchronized`关键字使方法原子化，只不过它不是把`synchronized`加在方法的声明处，而是加在方法的内部。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624027680464-a0556e4f-594c-416c-9b4b-c66d38ce7a80.png#clientId=u70208647-93aa-4&from=paste&id=u511481ef&originHeight=555&originWidth=986&originalType=url&ratio=3&status=done&style=none&taskId=uc386fb49-b9a4-4699-a1c8-08445b12e97)<br />容器是线程安全的，并不意味着就可以在多线程环境下放心大胆地随便用了，来看下面这段使用`Vector`的代码。
```java
@Test
public void testVectorConcurrentReadWrite() {
    Vector<Integer> vector = new Vector<>();
    vector.add(1);
    vector.add(2);
    vector.add(3);
    vector.add(4);
    vector.add(5);
    for (Integer item : vector) {
        new Thread(vector::clear).start();
        System.out.println(item);
    }
}
```
运行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624027680427-38e0d691-0458-47f0-8cd0-63a09b83f5a7.png#clientId=u70208647-93aa-4&from=paste&id=u4cb3e1e6&originHeight=399&originWidth=685&originalType=url&ratio=3&status=done&style=none&taskId=uc591fe84-ef07-43bc-aad8-2b4ff40b7fd)<br />在一个线程中使用`Iterator`迭代器遍历`vector`，同时另一个线程对`vector`作修改时，会抛出`java.util.ConcurrentModificationException`异常。很多人不理解，因为`Vector`的所有方法都加了`synchronized`关键字来修饰，包括迭代器方法，理论上应该是线程安全的呀。
```java
public synchronized Iterator<E> iterator() {
 //Itr是AbstractList的私有内部类
    return new Itr();
}
```
看以上错误的堆栈指向`java.util.Vector$Itr.checkForComodification(Vector.java:1184)`，源码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624027680544-73b56030-9c3c-44f0-928b-8611c163dbc6.png#clientId=u70208647-93aa-4&from=paste&id=ua432a978&originHeight=464&originWidth=948&originalType=url&ratio=3&status=done&style=none&taskId=u73c2ee4a-e072-4601-a185-fd0a3e3ecd1)<br />两个关键变量：

- `expectedModCount`：表示对`List`修改次数的期望值，它的初始值与`modCount`相等
- `modCount`：表示`List`集合结构被修改次数，是`AbstractList`类中的一个成员变量，初始值为0

看过`ArrayList`的源码就知道，每次调用`add()`和`remove()`方法时就会对`modCount`进行加1操作。而上面的测试代码中调用了`Vector`类的`clear()`方法，这个方法中对`modCount`进行了加1，而迭代器中的`expectedModCount`依然等于0，两者不等，因此抛了异常。这就是集合中的fail-fast机制，fail-fast 机制用来防止在对集合进行遍历过程当中，出现意料之外的修改，会通过Unchecked异常暴力的反应出来。<br />虽然`Vector`的方法都采用了`synchronized`进行了同步，但是实际上通过`Iterator`访问的情况下，每个线程里面返回的是不同的`iterator`，也即是说`expectedModCount`变量是每个线程私有。如果此时有2个线程，线程1在进行遍历，线程2在进行修改，那么很有可能导致线程2修改后导致Vector中的modCount自增了，线程2的`expectedModCount`也自增了，但是线程1的`expectedModCount`没有自增，此时线程1遍历时就会出现`expectedModCount`不等于`modCount`的情况了。<br />同样地，`SynchronizedList`在使用迭代器遍历的时候同样会有问题的，源码中的注释已经提醒要手动加锁了。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624027681663-cbf0dd35-4c5b-4f11-8d0c-ff25addbe721.png#clientId=u70208647-93aa-4&from=paste&id=u8d4fcf07&originHeight=458&originWidth=981&originalType=url&ratio=3&status=done&style=none&taskId=u1b39163b-a6f5-4eea-9746-337ed169b1f)<br />`foreach`循环里不能调用集合的`remove`/`add`/`clear`方法这一条规约不仅对非线程安全的`ArrayList`/`LinkedList`适用，对于线程安全的`Vector`以及`synchronizedList`也同样适用。<br />因此，要想解决以上问题，只能在遍历前（无论用不用iterator）加锁。
```java
synchronized (vector) {
    for (int i = 0; i < vector.size(); i++) {
        System.out.println(vector.get(i));
}
//或者
synchronized (vector) {
    for (Integer item : vector) {
        System.out.println(item);
}
```
仅仅是遍历一下容器都要上锁，性能必然不好。<br />其实并非只有遍历前加锁这一种解决方法，使用并发容器`CopyOnWriteArrayList`也能避免以上问题。
<a name="GerHX"></a>
## `CopyOnWriteArrayList`介绍
一般来说会认为：`CopyOnWriteArrayList`是同步`List`的替代品，`CopyOnWriteArraySet`是同步Set的替代品。<br />无论是`Hashtable`–>`ConcurrentHashMap`，还是说`Vector`–>`CopyOnWriteArrayList`。JUC下支持并发的容器与老一代的线程安全类相比，总结起来就是加锁粒度的问题。

- `Hashtable`与`Vector`加锁的粒度大，直接在方法声明处使用`synchronized`
- `ConcurrentHashMap`、`CopyOnWriteArrayList`的加锁粒度小。用各种方式来实现线程安全，比如`ConcurrentHashMap`用了CAS、+ volatile等方式来实现线程安全
- JUC下的线程安全容器在遍历的时候不会抛出`ConcurrentModificationException`异常

所以一般来说，都会使用JUC包下提供的线程安全容器，而不是使用老一代的线程安全容器。<br />下面来看看`CopyOnWriteArrayList`是怎么实现的，为什么使用迭代器遍历的时候就不用额外加锁，也不会抛出`ConcurrentModificationException`异常。
<a name="zF3kN"></a>
## 实现原理
Copy-on-write是解决并发的的一种思路，指的是实行读写分离，如果执行的是写操作，则复制一个新集合，在新集合内添加或者删除元素。待一切修改完成之后，再将原集合的引用指向新的集合。<br />这样的好处就是，可以高并发地对COW进行读和遍历操作，而不需要加锁，因为当前集合不会添加任何元素。<br />写时复制（copy-on-write）的这种思想，这种机制，并不是始于Java集合之中，在Linux、Redis、文件系统中都有相应思想的设计，是一种计算机程序设计领域的优化策略。<br />`CopyOnWriteArrayList`的核心理念就是读写分离，写操作在一个复制的数组上进行，读操作还是在原始数组上进行，读写分离，互不影响。写操作需要加锁，防止并发写入时导致数据丢失。写操作结束之后需要让数组指针指向新的复制数组。<br />看一下`CopyOnWriteArrayList`基本的结构。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624027685803-f4e15840-9f4b-4a07-8bcc-a86eea30e91a.png#clientId=u70208647-93aa-4&from=paste&id=ubd428aa5&originHeight=347&originWidth=732&originalType=url&ratio=3&status=done&style=none&taskId=u637cfbb0-5d26-4993-a793-94a312081b4)<br />看一下其读写的源码，写操作加锁，防止并发写入时导致数据丢失，并复制一个新数组，增加操作在新数组上完成，将array指向到新数组中，最后解锁。至于读操作，则是直接读取array数组中的元素。
```java
public boolean add(E e) {
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        Object[] elements = getArray();
        int len = elements.length;
        Object[] newElements = Arrays.copyOf(elements, len + 1);
        newElements[len] = e;
        setArray(newElements);
        return true;
    } finally {
        lock.unlock();
    }
}

final void setArray(Object[] a) {
    array = a;
}

public E get(int index) {
    return get(getArray(), index);
}

final Object[] getArray() {
    return array;
}
```
<a name="qPqNu"></a>
## 遍历 - `COWIterator`
到现在，还是没有解释为什么`CopyOnWriteArrayList`在遍历时，对其进行修改而不抛出异常。<br />不管是`foreach`循环还是直接写`Iterator`来遍历，实际上都是使用`Iterator`遍历。那么就直接来看下`CopyOnWriteArrayList`的`iterator()`方法。
```java
public Iterator<E> iterator() {
    return new COWIterator<E>(getArray(), 0);
}
```
来看一下迭代器`COWIterator`的实现源码。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624027681812-599fa271-c4e7-4557-a16f-8b210668e943.png#clientId=u70208647-93aa-4&from=paste&id=u0ccfd3fc&originHeight=627&originWidth=732&originalType=url&ratio=3&status=done&style=none&taskId=ubd72df7b-84aa-4fbd-b1b6-6b528d62b02)<br />可以发现的是，迭代器所有的操作都基于`snapshot`数组，而`snapshot`是传递进来的array数组。<br />也就是说在使用`COWIterator`进行遍历的时候，如果修改了集合，集合内部的array就指向了新的一个数组对象，而`COWIterator`内部的那个`snapshot`还是指向初始化时传进来的旧数组，所以不会抛异常，因为旧数组永远没变过，旧数组读操作永远可靠且安全。
<a name="G3npv"></a>
## `CopyOnWriteArrayList`与`synchronizedList`性能测试
写单元测试来对`CopyOnWriteArrayList`与`synchronizedList`的并发写性能作测试，由于`CopyOnWriteArrayList`写时直接复制新数组，可以预想到其写操作性能不高，会劣于`synchronizedList`。
```java
@Test
public void testThreadSafeListWrite() {
    List<Integer> copyOnWriteArrayList = new CopyOnWriteArrayList<>();
    List<Integer> synchronizedList = Collections.synchronizedList(new ArrayList<>());
    StopWatch stopWatch = new StopWatch();
    int loopCount = 10000;
    stopWatch.start();
    /**
     * ThreadLocalRandom:是JDK 7之后提供并发产生随机数，能够解决多个线程发生的竞争争夺。
     * ThreadLocalRandom不是直接用new实例化，而是第一次使用其静态方法current()。
     * 从Math.random()改变到ThreadLocalRandom有如下好处：我们不再有从多个线程访问同一个随机数生成器实例的争夺。
     */
    IntStream.rangeClosed(1, loopCount).parallel().forEach(
            item -> copyOnWriteArrayList.add(ThreadLocalRandom.current().nextInt(loopCount)));
    stopWatch.stop();
    System.out.println(
            "Write:copyOnWriteList: " + stopWatch.getTime() + "，copyOnWriteList.size()=" + copyOnWriteArrayList
                    .size());

    stopWatch.reset();
    stopWatch.start();
    /**
     * parallelStream特点：基于服务器内核的限制，如果你是八核
     * 每次线程只能起八个，不能自定义线程池
     */
    IntStream.rangeClosed(1, loopCount).parallel().forEach(
            item -> synchronizedList.add(ThreadLocalRandom.current().nextInt(loopCount)));
    stopWatch.stop();
    System.out.println(
            "Write:synchronizedList: " + stopWatch.getTime() + "，synchronizedList.size()=" + synchronizedList
                    .size());
}
```
运行结果如下，可以看到同样条件下的写耗时，`CopyOnWriteArrayList`是`synchronizedList`的30多倍。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624027682496-cff98d02-4763-4f13-be32-e194d29cb665.png#clientId=u70208647-93aa-4&from=paste&id=u4f07a004&originHeight=459&originWidth=623&originalType=url&ratio=3&status=done&style=none&taskId=udbdcc9b6-68f0-4d45-a2ed-7edcdc16fe9)<br />同样地，写单元测试来对`CopyOnWriteArrayList`与`synchronizedList`的并发读性能作测试，由于`CopyOnWriteArrayList`读操作不加锁，可以预想到其读操作性能明显会优于`synchronizedList`。
```java
@Test
public void testThreadSafeListRead() {
    List<Integer> copyOnWriteArrayList = new CopyOnWriteArrayList<>();
    List<Integer> synchronizedList = Collections.synchronizedList(new ArrayList<>());
    copyOnWriteArrayList.addAll(IntStream.rangeClosed(1, 1000000).boxed().collect(Collectors.toList()));
    synchronizedList.addAll(IntStream.rangeClosed(1, 1000000).boxed().collect(Collectors.toList()));

    int copyOnWriteArrayListSize = copyOnWriteArrayList.size();
    StopWatch stopWatch = new StopWatch();
    int loopCount = 1000000;
    stopWatch.start();
    /**
     * ThreadLocalRandom:是JDK 7之后提供并发产生随机数，能够解决多个线程发生的竞争争夺。
     * ThreadLocalRandom不是直接用new实例化，而是第一次使用其静态方法current()。
     * 从Math.random()改变到ThreadLocalRandom有如下好处：我们不再有从多个线程访问同一个随机数生成器实例的争夺。
     */
    IntStream.rangeClosed(1, loopCount).parallel().forEach(
            item -> copyOnWriteArrayList.get(ThreadLocalRandom.current().nextInt(copyOnWriteArrayListSize)));
    stopWatch.stop();
    System.out.println("Read:copyOnWriteList: " + stopWatch.getTime());

    stopWatch.reset();
    stopWatch.start();
    int synchronizedListSize = synchronizedList.size();
    /**
     * parallelStream特点：基于服务器内核的限制，如果你是八核
     * 每次线程只能起八个，不能自定义线程池
     */
    IntStream.rangeClosed(1, loopCount).parallel().forEach(
            item -> synchronizedList.get(ThreadLocalRandom.current().nextInt(synchronizedListSize)));
    stopWatch.stop();
    System.out.println("Read:synchronizedList: " + stopWatch.getTime());
}
```
运行结果如下，同等条件下的读耗时，`CopyOnWriteArrayList`只有`synchronizedList`的一半。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624027682507-96ae64bd-e0e1-4fec-bfa1-366bfbf593a2.png#clientId=u70208647-93aa-4&from=paste&id=uc9bea588&originHeight=470&originWidth=658&originalType=url&ratio=3&status=done&style=none&taskId=u8f9f279e-d534-411d-813f-cbdffcd4998)
<a name="WG0Km"></a>
## `CopyOnWriteArrayList`优缺点总结
<a name="rSRiW"></a>
#### 优点：

- 对于一些读多写少的数据，写入时复制的做法就很不错，例如配置、黑名单、物流地址等变化非常少的数据，这是一种无锁的实现。可以实现程序更高的并发。
- `CopyOnWriteArrayList`并发安全且性能比`Vector`好。`Vector`是增删改查方法都加了`synchronized `来保证同步，但是每个方法执行的时候都要去获得锁，性能就会大大下降，而`CopyOnWriteArrayList`只是在增删改上加锁，但是读不加锁，在读方面的性能就好于`Vector`。
<a name="Ql9PK"></a>
#### 缺点：

- 数据一致性问题。`CopyOnWrite`容器只能保证数据的最终一致性，不能保证数据的实时一致性。比如线程A在迭代`CopyOnWriteArrayList`容器的数据。线程B在线程A迭代的间隙中将`CopyOnWriteArrayList`部分的数据修改了，但是线程A迭代出来的是旧数据。
- 内存占用问题。如果`CopyOnWriteArrayList`经常要增删改里面的数据，并且对象比较大，频繁地写会消耗内存，从而引发Java的GC问题，这个时候，应该考虑其他的容器，例如`ConcurrentHashMap`。



