Java 队列<br />`LinkedBlockingQueue`和`ArrayBlockingQueue`比较简单，不进行讲解了。下面只介绍`PriorityBlockingQueue`和`DelayQueue`。
<a name="bu8c0"></a>
## PriorityBlockingQueue
`PriorityBlockingQueue`是带优先级的无界阻塞队列，每次出队都返回优先级最高或最低的元素。内部使用二叉堆实现。
<a name="mmKwK"></a>
### 类图结构
![2021-05-14-23-52-12-539602.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621007651726-23ce94c4-e534-48d6-a36f-8901447e168c.png#clientId=u238323de-e525-4&from=ui&id=ubebdd53c&originHeight=880&originWidth=996&originalType=binary&size=44176&status=done&style=shadow&taskId=ue667873b-e861-4e98-a5ab-a8ae9fc3c80)<br />`PriorityBlockingQueue`内部有一个数组`queue`，用来存放队列元素。`allocationSpinLock`是个自旋锁，通过CAS操作来保证同时只有一个线程可以扩容队列，状态为0或1。<br />由于这是一个优先队列，所以有一个`comparator`用来比较元素大小。<br />下面为构造函数：
```java
private static final int DEFAULT_INITIAL_CAPACITY = 11;

public PriorityBlockingQueue() {
    this(DEFAULT_INITIAL_CAPACITY, null);
}

public PriorityBlockingQueue(int initialCapacity) {
    this(initialCapacity, null);
}
```
可知默认队列容量为11，默认比较器为null，也就是使用元素的`compareTo`方法进行比较来确定元素的优先级，这意味着队列元素必须实现`Comparable`接口。
<a name="p3etd"></a>
### 原理讲解
<a name="kZuUs"></a>
#### `boolean offer()`
```java
public boolean offer(E e) {
    if (e == null)
        throw new NullPointerException();
    // 获取独占锁
    final ReentrantLock lock = this.lock;
    lock.lock();
    int n, cap;
    Object[] array;
    // 扩容
    while ((n = size) >= (cap = (array = queue).length))
        tryGrow(array, cap);
    try {
        Comparator<? super E> cmp = comparator;
        if (cmp == null)
            // 通过对二叉堆的上浮操作保证最大或最小的元素总在根节点
            siftUpComparable(n, e, array);
        else
            // 使用了自定义比较器
            siftUpUsingComparator(n, e, array, cmp);
        size = n + 1;
        // 激活因调用take()方法被阻塞的线程
        notEmpty.signal();
    } finally {
        // 释放锁
        lock.unlock();
    }
    return true;
}
```
流程比较简单，下面主要看扩容和建堆操作。<br />先看扩容。
```java
private void tryGrow(Object[] array, int oldCap) {
    // 由前面的代码可知，调用tryGrow函数前先获取了独占锁，
    // 由于扩容比较费时，此处先释放锁，
    // 让其他线程可以继续操作（如果满足可操作的条件的话），
    // 以提升并发性能
    lock.unlock();
    Object[] newArray = null;
    // 通过allocationSpinLock保证同时最多只有一个线程进行扩容操作。
    if (allocationSpinLock == 0 &&
        UNSAFE.compareAndSwapInt(this, allocationSpinLockOffset,0, 1)) {
        try {
            // 当容量比较小时，一次只增加2容量
            // 比较大时增加一倍
            int newCap = oldCap + ((oldCap < 64) ?(oldCap + 2) : (oldCap >> 1));
            // 溢出检测
            if (newCap - MAX_ARRAY_SIZE > 0) {
                int minCap = oldCap + 1;
                if (minCap < 0 || minCap > MAX_ARRAY_SIZE)
                    throw new OutOfMemoryError();
                newCap = MAX_ARRAY_SIZE;
            }
            if (newCap > oldCap && queue == array)
                newArray = new Object[newCap];
        } finally {
            // 释放锁，没用CAS是因为同时最多有一个线程操作allocationSpinLock
            allocationSpinLock = 0;
        }
    }
    // 如果当前线程发现有其他线程正在对队列进行扩容，
    // 则调用yield方法尝试让出CPU资源促使扩容操作尽快完成
    if (newArray == null)
        Thread.yield();
    lock.lock();
    if (newArray != null && queue == array) {
        queue = newArray;
        System.arraycopy(array, 0, newArray, 0, oldCap);
    }
}
```
下面来看建堆算法
```java
private static <T> void siftUpComparable(int k, T x, Object[] array) {
    Comparable<? super T> key = (Comparable<? super T>) x;
    while (k > 0) {
        // 获取父节点，设子节点索引为k，
        // 则由二叉堆的性质可知，父节点的索引总为(k - 1) >>> 1
        int parent = (k - 1) >>> 1;
        // 获取父节点对应的值
        Object e = array[parent];
        // 只有子节点的值小于父节点的值时才上浮
        if (key.compareTo((T) e) >= 0)
            break;
        array[k] = e;
        k = parent;
    }
    array[k] = key;
}
```
如果了解二叉堆的话，此处代码是十分容易理解的。关于二叉堆，可参看《数据结构之二叉堆》。
<a name="TyLoo"></a>
#### `E poll()`
```java
public E poll() {
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        // 出队
        return dequeue();
    } finally {
        lock.unlock();
    }
}

private E dequeue() {
    int n = size - 1;
    if (n < 0)
        return null;
    else {
        Object[] array = queue;
        E result = (E) array[0];
        // 获取尾节点，在实现对二叉堆的下沉操作时要用到
        E x = (E) array[n];
        array[n] = null;
        Comparator<? super E> cmp = comparator;
        if (cmp == null)
            // 下沉操作，保证取走最小的节点（根节点）后，新的根节点仍时最小的，二叉堆的性质依然满足
            siftDownComparable(0, x, array, n);
        else
            // 使用自定义比较器
            siftDownUsingComparator(0, x, array, n, cmp);
        size = n;
        return result;
    }
}
```
`poll`方法通过调用`dequeue`方法使最大或最小的节点出队并将其返回。<br />下面来看二叉堆的下沉操作。
```java
private static <T> void siftDownComparable(int k, T x, Object[] array, int n) {
    if (n > 0) {
        Comparable<? super T> key = (Comparable<? super T>)x;
        int half = n >>> 1;
        while (k < half) {
            // child为两个子节点（如果有的话）中较小的那个对应的索引
            int child = (k << 1) + 1;
            Object c = array[child];
            int right = child + 1;
            // 通过比较保证child对应的为较小值的索引
            if (right < n &&
                ((Comparable<? super T>) c).compareTo((T) array[right]) > 0)
                c = array[child = right];
            if (key.compareTo((T) c) <= 0)
                break;
            // 下沉，将较小的子节点换到父节点位置
            array[k] = c;
            k = child;
        }
        array[k] = key;
    }
}
```
同上，对下沉操作有疑问的话可参考上述文章。
<a name="dVcm3"></a>
#### `void put(E e)`
调用了offer
```java
public void put(E e){
    offer(e);
}
```
<a name="wHfO7"></a>
#### `E take()`
take操作的作用是获取二叉堆的根节点元素，如果队列为空则阻塞。
```java
public E take() throws InterruptedException {
    final ReentrantLock lock = this.lock;
    // 阻塞可被中断
    lock.lockInterruptibly();
    E result;
    try {
        // 队列为空就将当前线程放入notEmpty条件队列
        // 使用while循环判断是为了避免虚假唤醒
        while ( (result = dequeue()) == null)
            notEmpty.await();
    } finally {
        lock.unlock();
    }
    return result;
}
```
<a name="FMMsy"></a>
## DelayQueue
DelayQueue并发队列是一个无界阻塞延迟队列，队列中的每一个元素都有一个过期时间，当从队列中获取元素时只有过期元素才会出列。队列头元素是最快要过期的元素。
<a name="tkq4F"></a>
### 类图结构
![2021-05-14-23-52-15-897562.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621007714248-d9b32f8e-9c56-4524-8550-ad2ae0aa2fc5.png#clientId=u238323de-e525-4&from=ui&id=u951cc9e3&originHeight=815&originWidth=931&originalType=binary&size=40365&status=done&style=shadow&taskId=uf1e31bb1-8a6c-4de5-91bc-8dea6bdbfdd)<br />`DelayQueue`内部使用`PriorityQueue`存放数据，使用`ReentrantLock`实现线程同步。队列里的元素要实现Delayed接口（`Delayed`接口继承了`Comparable`接口），用以得到过期时间并进行过期时间的比较。
```java
public interface Delayed extends Comparable<Delayed> {
    long getDelay(TimeUnit unit);
}
```
`available`是由`lock`生成的条件变量，用以实现线程间的同步。<br />`leader`是leader-follower模式的变体，用于减少不必要的线程等待。当一个线程调用队列的`take`方法变为`leader`线程后，它会调用条件变量`available.waitNanos(delay)`等待`delay`时间，但是其他线程（follower）则会调用`available.await()`进行无限等待。`leader`线程延迟时间过期后，会退出`take`方法，并通过调用`available.signal()`方法唤醒一个`follower`线程，被唤醒的线程会被选举为新的`leader`线程。
<a name="S3i2x"></a>
### 原理讲解
<a name="biJQz"></a>
#### `boolean offer(E e)`
```java
public boolean offer(E e) {
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        // 添加新元素
        q.offer(e);
        // 查看新添加的元素是否为最先过期的
        if (q.peek() == e) {
            leader = null;
            available.signal();
        }
        return true;
    } finally {
        lock.unlock();
    }
}
```
上述代码首先获取独占锁，然后添加元素到优先级队列，由于`q`是优先级队列，所以添加元素后，调用`q.peek()`方法返回的并不一定是当前添加的元素。当如果`q.peek() == e`，说明当前元素是最先要过期的，那么重置`leader`线程为`null`并激活`available`条件队列里的一个线程，告诉它队列里面有元素了。
<a name="K2SPu"></a>
#### `E take()`
获取并移除队列里面过期的元素，如果队列里面没有过期元素则等待。
```java
public E take() throws InterruptedException {
    final ReentrantLock lock = this.lock;
    // 可中断
    lock.lockInterruptibly();
    try {
        for (;;) {
            E first = q.peek();
            // 为空则等待
            if (first == null)
                available.await();
            else {
                long delay = first.getDelay(NANOSECONDS);
                // 过期则成功获取
                if (delay <= 0)
                    return q.poll();
                // 执行到此处，说明头元素未过期    
                first = null; // don't retain ref while waiting
                // follower无限等待，直到被唤醒
                if (leader != null)
                    available.await();
                else {
                    Thread thisThread = Thread.currentThread();
                    leader = thisThread;
                    try {
                        // leader等待lelay时间，则头元素必定已经过期
                        available.awaitNanos(delay);
                    } finally {
                        // 重置leader，给follower称为leader的机会
                        if (leader == thisThread)
                            leader = null;
                    }
                }
            }
        }
    } finally {
        if (leader == null && q.peek() != null)
            // 唤醒一个follower线程
            available.signal();
        lock.unlock();
    }
}
```
一个线程调用`take`方法时，会首先查看头元素是否为空，为空则直接等待，否则判断是否过期。若头元素已经过期，则直接通过`poll`获取并移除，否则判断是否有`leader`线程。若有`leader`线程则一直等待，否则自己成为`leader`并等待头元素过期。
<a name="bIqWw"></a>
#### `E poll()`
获取并移除头过期元素，如果没有过期元素则返回`null`。
```java
public E poll() {
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        E first = q.peek();
        // 若队列为空或没有元素过期则直接返回null
        if (first == null || first.getDelay(NANOSECONDS) > 0)
            return null;
        else
            return q.poll();
    } finally {
        lock.unlock();
    }
}
```
<a name="hsJkh"></a>
#### `int size()`
计算队列元素个数，包含过期的和未过期的。
```java
public int size() {
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        return q.size();
    } finally {
        lock.unlock();
    }
}
```
