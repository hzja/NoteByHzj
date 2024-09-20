Java
<a name="Ex5Nc"></a>
## **阻塞队列**
普通队列LinkedList或者是ArrayList这些，那普通的队列与阻塞队列有啥区别呢？最大的区别就是阻塞队列是有阻塞添加与阻塞删除的方法。<br />**阻塞添加：** 阻塞添加就是说当前这一个阻塞队列的元素已经是满的时候，队列这个时候会阻塞掉加入元素的线程，直到队列里的元素不满的时候才开始重新把线程唤醒执行元素的加入操作。<br />**阻塞删除：** 阻塞删除就是说当前这一队列里的元素为空的时候，具有删除操作队列元素的线程将会被阻塞，一直到队列里的元素不为空的时候再执行删除操作。
<a name="QH8BC"></a>
## `**BlockingQueue**`**接口**
在阻塞队列接口`BlockingQueue`是继承`Queue`接口，先来看看阻塞队列接口提供了什么方法。
```java
public interface BlockingQueue<E> extends Queue<E> {

//将的元素插入到当前队列的尾部，在成功时返回 true，如果此队列已满，则抛异常
boolean add(E e); 

//将指定的元素插入到当前队列的尾部，如果该队列已满，
//则在到达指定的等待时间之前等待可用的空间,该方法可中断
boolean offer(E e, long timeout, TimeUnit unit) throws InterruptedException; 

//将指定的元素插入当前队列的尾部，如果当前队列已满，则进行一直等待（阻塞情况）。 
void put(E e) throws InterruptedException; 

//获取并把队列头部移除，如果没有队列元素则进行等待（阻塞情况）， 
//一直到有元素将唤醒等待线程执行该操作 
E take() throws InterruptedException; 

//获取并把队列的头部移除，在指定的等待时间前一直等到获取元素， 超过时间的话方法会结束
E poll(long timeout, TimeUnit unit) throws InterruptedException; 

//从当前队列中移除指定元素的单个实例 
boolean remove(Object o); 
}

//除了上述方法还有继承自Queue接口的方法 
//获取但不移除此队列的头元素,没有则抛出异常 
E element(); 

//获取但不移除此队列的头；如果此队列为空，则返回 null。 
E peek(); 

//获取并移除此队列的头，如果此队列为空，则返回 null。 
E poll();
```
从上面方法可以进行归类<br />**插入方法：**

- `put (E e)`：将元素插入到当前队列的尾部，如果当前队列是满的话，则进行一直阻塞状态
- `add(E e)`：元素添加成功返回true，失败就报错lllegalstateException异常
- `offer(E e)`：元素添加成功返回true，如果当前队列是满的状态，则就返回了false

**检查方法：**

- `element()`：获取但是没有把队列的头元素给移除，如果没有元素的话就抛出异常
- `peek()`：获取但是没有移除掉队列的头，如果队列是空的话，返回null。

**删除方法：**

- `remove(Object o)`：移除指定的元素，如果移除成功返回true，失败的话返回false
- `poll()`：获取并把队列的头元素移除，如果队列为空的话，就返回null
- `take()`：获取并把队列的头元素移除，如果没有元素的话则就是一直阻塞的状态。
<a name="eryjq"></a>
## `**ArrayBlockingQueue**`**原理**
`ArrayBlockingQueue`的内部是通过可重入锁与两个条件对象来实现阻塞的，先看源码。
```java
public class ArrayBlockingQueue<E> extends AbstractQueue<E>
        implements BlockingQueue<E>, java.io.Serializable {

   //存储数据数组
    final Object[] items;

    //获取数据索引，用在take,poll,peek,remove
    int takeIndex;

    //添加数据的索引，用在 put, offer, or add 方法
    int putIndex;

    // 队列元素的个数 
    int count;


    // 控制并非访问的锁
    final ReentrantLock lock;

    /**notEmpty条件对象，用于通知take方法队列已有元素，可执行获取操作 */
    private final Condition notEmpty;

    /**notFull条件对象，用于通知put方法队列未满，可执行添加操作 */
    private final Condition notFull;

    //迭代器     
    transient Itrs itrs = null;

}
```
从上面的成员变量可以看到，ArrayBlockingQueue内部是通过数组对象来进行数据存储的，还可以看到ArrayBlockingQueue是通过ReentrantLock来控制添加和删除线程的并非访问。对于notEmpty的话是存放线程进行等待或者是唤醒调用take的线程，来告知他们队列里面已经有元素，可以进行执行获取操作。notFull就是用于等待或者是唤醒调用put的线程，告诉它们队列没有满，可以进行添加元素。takeindex代表是下一个方法被调用获取数组元素的索引，putindex是代表下一个方法被调用的时候元素添加到数组中的索引。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646972871805-ce98f0b9-734a-4b08-8594-f971bc4c9b21.png#clientId=uc795119f-d5dc-4&from=paste&id=ubf0fbe6f&originHeight=322&originWidth=999&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7b1cbbe7-c29b-44be-88e2-fdff60d590e&title=)
<a name="hxrBk"></a>
### `ArrayBlockingQueue`的阻塞添加原理
```java
//add方法，间接调用了offer(e)
public boolean add(E e) {
        if (offer(e))
            return true;
        else
            throw new IllegalStateException("Queue full");
    }

//offer方法
public boolean offer(E e) {
     checkNotNull(e);//检查元素是否为空
     final ReentrantLock lock = this.lock;
     lock.lock();//进行加锁
     try {
         if (count == items.length)//判断队列是否满的状态
             return false;
         else {
             enqueue(e);//把元素添加到队列
             return true;
         }
     } finally {
         lock.unlock();
     }
 }

//入队操作
private void enqueue(E x) {
    //获取当前数组
    final Object[] items = this.items;
    //通过putIndex索引对数组进行赋值
    items[putIndex] = x;
    //索引自增，如果已是最后一个位置，重新设置 putIndex = 0;
    if (++putIndex == items.length)
        putIndex = 0;
    count++;//队列中元素数量加1
    //唤醒调用take()方法的线程，执行元素获取操作。
    notEmpty.signal();
}
```
可以看到这里的add和offer这两个方法的实现思路相对来说比较简单，更需要注意是`enqueue(E x)`这个方法，方法里的putindex直接把元素添加到数组当中。这里分为两个场景，如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646972871724-bc0452a9-ca9c-425a-a6d4-219e9a82ee21.png#clientId=uc795119f-d5dc-4&from=paste&id=u852f21e8&originHeight=723&originWidth=1041&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u44aa9b22-3609-45d0-833f-343eea80fd2&title=)
<a name="BhIut"></a>
### `ArrayBlockingQueque`的阻塞移除原理
```java
//从队列头部删除，队列没有元素就阻塞，可中断
public E take() throws InterruptedException {
final ReentrantLock lock = this.lock;
  lock.lockInterruptibly();//中断
  try {
      //如果队列没有元素
      while (count == 0)
          //执行阻塞操作
          notEmpty.await();
      return dequeue();//如果队列有元素执行删除操作
  } finally {
      lock.unlock();
  }
}
```
可以看出`take`方法实现的思想很简单，如果有的话就进行删除，没有的话就进行阻塞。而且要注意的是，这个阻塞是可进行中断的。队列里没有数据的话那么就进行队列等待，反之有数据就直接删除。如果这时候有新的`put`线程添加了数据的话，那么`put`操作会把`take`线程进行唤醒，然后执行take操作。
<a name="QNEdA"></a>
## `**LinkedBlockingQueue**`**的实现原理**
`LinkedBlockingQueue`是链表的阻塞队列，内部维持了一个链表的数据队列，按照惯例，先看成员变量。
```java
public class LinkedBlockingQueue<E> extends AbstractQueue<E>
        implements BlockingQueue<E>, java.io.Serializable {

    //
     用于存储数据
    static class Node<E> {
        E item;


        Node<E> next;

        Node(E x) { item = x; }
    }

    // 阻塞队列的大小，默认为Integer.MAX_VALUE 
    private final int capacity;

    // 当前阻塞队列中的元素个数
    private final AtomicInteger count = new AtomicInteger();

    
    
     //阻塞队列的头结点
    transient Node<E> head;

    //阻塞队列的尾节点 
    private transient Node<E> last;

    //获取并移除元素时使用的锁
    private final ReentrantLock takeLock = new ReentrantLock();

    // notEmpty条件对象，当队列没有数据时用于挂起执行删除的线程
    private final Condition notEmpty = takeLock.newCondition();

    //添加元素时使用的锁如 put, offer, etc 
    private final ReentrantLock putLock = new ReentrantLock();

    //notFull条件对象，当队列数据已满时用于挂起执行添加的线程
    private final Condition notFull = putLock.newCondition();

}
```
可以看到，当每一个添加`LinkedBlockingQueue`的数据都会被封装成一个Node节点，然后添加到链表队列当中去，然后head和last分别是指向了队列的头和尾。它与`ArrayBlockingQueue`不同的是，它的内部使用了`takeLock`与`putLock`来对并发进行控制，说明添加和删除并不是互斥的操作，可以进行同时执行，提高吞吐量。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646972871723-5833d7a5-037a-42ea-a174-2daebd696206.png#clientId=uc795119f-d5dc-4&from=paste&id=ue21c6ad0&originHeight=505&originWidth=991&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u86bb456c-1e80-43c2-a6a0-f275f93b17a&title=)
<a name="ZI5ka"></a>
### `LinkedBlockingQueue`阻塞添加原理
```java
public boolean add(E e) {
     if (offer(e))
         return true;
     else
         throw new IllegalStateException("Queue full");
}
```
添加方法相对来说比较简单，成功就返回true，失败就抛出异常。接着看看`offer`方法。
```java
public boolean offer(E e) {
     //添加元素为null直接抛出异常
     if (e == null) throw new NullPointerException();
      //获取队列的个数
      final AtomicInteger count = this.count;
      //判断队列是否已满
      if (count.get() == capacity)
          return false;
      int c = -1;
      //构建节点
      Node<E> node = new Node<E>(e);
      final ReentrantLock putLock = this.putLock;
      putLock.lock();
      try {
          //再次判断队列是否已满，考虑并发情况
          if (count.get() < capacity) {
              enqueue(node);//添加元素
              c = count.getAndIncrement();//拿到当前未添加新元素时的队列长度
              //如果容量还没满
              if (c + 1 < capacity)
                  notFull.signal();//唤醒下一个添加线程，执行添加操作
          }
      } finally {
          putLock.unlock();
      }
      // 由于存在添加锁和消费锁，而消费锁和添加锁都会持续唤醒等到线程。
    
      if (c == 0) 
        signalNotEmpty();//如果还存在数据那么就唤醒消费锁
    return c >= 0; // 添加成功返回true，否则返回false
  }

//入队操作
private void enqueue(Node<E> node) {
     //队列尾节点指向新的node节点
     last = last.next = node;
}

//signalNotEmpty方法
private void signalNotEmpty() {
      final ReentrantLock takeLock = this.takeLock;
      takeLock.lock();
          //唤醒获取并删除元素的线程
          notEmpty.signal();
      } finally {
          takeLock.unlock();
      }
  }
```
这里的`Offer()`方法做了两件事，第一件事是判断队列是否满的状态，如果是满的话就直接把锁释放，没满就将节点封装成Node入队，然后再次判断队列添加完成后是否已满状态，不满就继续唤醒等到在条件对象`notFull`上的添加线程。第二件事是，判断是否需要唤醒等到在`notEmpty`条件对象上的消费线程。
<a name="dOxOj"></a>
### `LinkedBlockingQueue`阻塞删除原理
```java
public E poll() {
	//获取当前队列的大小
	final AtomicInteger count = this.count;
	if (count.get() == 0)//如果没有元素数据返回null
		return null;
	E x = null;
	int c = -1;
	final ReentrantLock takeLock = this.takeLock;
	takeLock.lock();
	try {
		//判断队列里是否有数据
		if (count.get() > 0) {
			//有，直接删除并获取元素值
			x = dequeue();
			
			c = count.getAndDecrement();
			//如果队列不是空的话，继续唤醒等待在条件对象notEmpty上的消费线程
			if (c > 1)
				notEmpty.signal();
		}
	} finally {
		takeLock.unlock();
	}
	//判断c是否等于capacity，如果满可能存在等待添加的线程
	if (c == capacity)
		signalNotFull();
	return x;
}

private E dequeue() {
	Node<E> h = head;//获取头结点
	Node<E> first = h.next; 获取头结的下一个节点（要删除的节点）
		h.next = h; // help GC//自己next指向自己
	head = first;//更新头结点
	E x = first.item;//获取删除节点的值
	first.item = null;//清空数据
	return x;
}
```
队列没有数据就返回null，如果队列有数据的话，那么取走，如果队列还有数据那么唤醒等待在条件对象notEmpty上的消费线程。然后进行判断。因为只有可能队列满了，才可能存在等待的添加线程。
<a name="yM8gy"></a>
## `LinkedBlockingQueue`和`ArrayBlockingQueue`的不同之处

- 队列大小的不一样
- 数据存储的容器不一样
- 数组容器存储的，在插入和删除不会销毁额外的实例，而链表的会生成一个node对象。
- 锁的不一样，数组实现的队列的锁是不分离的，链表队列的锁是分离的。
