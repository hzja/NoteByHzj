CopyOnWriteArrayList
<a name="kgCaP"></a>
## 写入时复制（CopyOnWrite）思想
写入时复制（CopyOnWrite，简称COW）思想是计算机程序设计领域中的一种优化策略。其核心思想是，如果有多个调用者（Callers）同时要求相同的资源（如内存或者是磁盘上的数据存储），他们会共同获取相同的指针指向相同的资源，直到某个调用者视图修改资源内容时，系统才会真正复制一份专用副本（private copy）给该调用者，而其他调用者所见到的最初的资源仍然保持不变。这过程对其他的调用者都是透明的（transparently）。此做法主要的优点是如果调用者没有修改资源，就不会有副本（private copy）被创建，因此多个调用者只是读取操作时可以共享同一份资源。
<a name="Hrbll"></a>
## CopyOnWriteArrayList的实现原理
在使用CopyOnWriteArrayList之前，先阅读其源码了解下它是如何实现的。以下代码是向CopyOnWriteArrayList中add方法的实现（向CopyOnWriteArrayList里添加元素），可以发现在添加的时候是需要加锁的，否则多线程写的时候会Copy出N个副本出来。
```java
/**
 * Appends the specified element to the end of this list.
 *
 * @param e element to be appended to this list
 * @return <tt>true</tt> (as specified by {@link Collection#add})
 */
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
```
读的时候不需要加锁，如果读的时候有多个线程正在向CopyOnWriteArrayList添加数据，读还是会读到旧的数据，因为写的时候不会锁住旧的CopyOnWriteArrayList。
```java
public E get(int index) {
    return get(getArray(), index);
}
```
JDK中并没有提供CopyOnWriteMap，参考CopyOnWriteArrayList来实现，基本代码如下：
```java
import java.util.Collection;
import java.util.Map;
import java.util.Set;
 
public class CopyOnWriteMap<K, V> implements Map<K, V>, Cloneable {
    private volatile Map<K, V> internalMap;
 
    public CopyOnWriteMap() {
        internalMap = new HashMap<K, V>();
    }
 
    public V put(K key, V value) {
 
        synchronized (this) {
            Map<K, V> newMap = new HashMap<K, V>(internalMap);
            V val = newMap.put(key, value);
            internalMap = newMap;
            return val;
        }
    }
 
    public V get(Object key) {
        return internalMap.get(key);
    }
 
    public void putAll(Map<? extends K, ? extends V> newData) {
        synchronized (this) {
            Map<K, V> newMap = new HashMap<K, V>(internalMap);
            newMap.putAll(newData);
            internalMap = newMap;
        }
    }
}
```
实现很简单，只要了解了CopyOnWrite机制，可以实现各种CopyOnWrite容器，并且在不同的应用场景中使用。
<a name="YgSd5"></a>
## 几个要点

- 实现了List接口
- 内部持有一个ReentrantLock lock = new ReentrantLock();
- 底层是用volatile transient声明的数组 array
- 读写分离，写时复制出一个新的数组，完成插入、修改或者移除操作后将新数组赋值给array
:::tips
**「注：」**<br />volatile （挥发物、易变的）：变量修饰符，只能用来修饰变量。volatile修饰的成员变量在每次被线程访问时，都强迫从共享内存中重读该成员变量的值。而且，当成员变量发生变化时，强迫线程将变化值回写到共享内存。这样在任何时刻，两个不同的线程总是看到某个成员变量的同一个值。

![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603246446500-3e753cea-c557-4c72-8f59-bce1259704b4.webp#align=left&display=inline&height=450&originHeight=450&originWidth=892&size=0&status=done&style=shadow&width=892)<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1603246446537-e7a623e2-adb6-4ea3-b967-0a3dbd31d1aa.webp#align=left&display=inline&height=396&originHeight=396&originWidth=908&size=0&status=done&style=shadow&width=908)
:::
<a name="qRWqe"></a>
## 增删改查
<a name="50eOy"></a>
### 1）增
```java
public boolean add(E e) {
    final ReentrantLock lock = this.lock;
    //获得锁
    lock.lock();
    try {
        Object[] elements = getArray();
        int len = elements.length;
        //复制一个新的数组
        Object[] newElements = Arrays.copyOf(elements, len + 1);
        //插入新值
        newElements[len] = e;
        //将新的数组指向原来的引用
        setArray(newElements);
        return true;
    } finally {
        //释放锁
        lock.unlock();
    }
}
   
public void add(int index, E element) {
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        Object[] elements = getArray();
        int len = elements.length;
        if (index > len || index < 0)
            throw new IndexOutOfBoundsException("Index: "+index+
                                                ", Size: "+len);
        Object[] newElements;
        int numMoved = len - index;
        if (numMoved == 0)
            newElements = Arrays.copyOf(elements, len + 1);
        else {
            newElements = new Object[len + 1];
            System.arraycopy(elements, 0, newElements, 0, index);
            System.arraycopy(elements, index, newElements, index + 1,
                             numMoved);
        }
        newElements[index] = element;
        setArray(newElements);
    } finally {
        lock.unlock();
    }
}
```
<a name="9RWAo"></a>
### 2）删
```java
public E remove(int index) {
    final ReentrantLock lock = this.lock;
    //获得锁
    lock.lock();
    try {
        Object[] elements = getArray();
        int len = elements.length;
        E oldValue = get(elements, index);
        int numMoved = len - index - 1;
        if (numMoved == 0)
            //如果删除的元素是最后一个，直接复制该元素前的所有元素到新的数组
            setArray(Arrays.copyOf(elements, len - 1));
        else {
            //创建新的数组
            Object[] newElements = new Object[len - 1];
            //将index+1至最后一个元素向前移动一格
            System.arraycopy(elements, 0, newElements, 0, index);
            System.arraycopy(elements, index + 1, newElements, index,
                             numMoved);
            setArray(newElements);
        }
        return oldValue;
    } finally {
        lock.unlock();
    }
}
```
<a name="72aaadf1"></a>
### 3）改
```java
public E set(int index, E element) {
    final ReentrantLock lock = this.lock;
    //获得锁
    lock.lock();
    try {
        Object[] elements = getArray();
        E oldValue = get(elements, index);
        if (oldValue != element) {
            int len = elements.length;
            //创建新数组
            Object[] newElements = Arrays.copyOf(elements, len);
            //替换元素
            newElements[index] = element;
            //将新数组指向原来的引用
            setArray(newElements);
        } else {
            // Not quite a no-op; ensures volatile write semantics
            setArray(elements);
        }
        return oldValue;
    } finally {
        //释放锁
        lock.unlock();
    }
}
```
<a name="aCWv2"></a>
### 4）查
```java
//直接获取index对应的元素
public E get(int index) {return get(getArray(), index);}
private E get(Object[] a, int index) {return (E) a[index];}
```
<a name="CAvzh"></a>
## CopyOnWrite的应用场景
CopyOnWrite并发容器用于读多写少的并发场景。比如白名单，黑名单，商品类目的访问和更新场景，假如有一个搜索网站，用户在这个网站的搜索框中，输入关键字搜索内容，但是某些关键字不允许被搜索。这些不能被搜索的关键字会被放在一个黑名单当中，黑名单每天晚上更新一次。当用户搜索时，会检查当前关键字在不在黑名单当中，如果在，则提示不能搜索。实现代码如下：
```java
import java.util.Map;
import com.ifeve.book.forkjoin.CopyOnWriteMap;
 
/**
 * 黑名单服务
 *
 * @author fcant
 *
 */
public class BlackListServiceImpl {
    private static CopyOnWriteMap<String, Boolean> blackListMap = new CopyOnWriteMap<String, Boolean>(
            1000);
 
    public static boolean isBlackList(String id) {
        return blackListMap.get(id) == null ? false : true;
    }
 
    public static void addBlackList(String id) {
        blackListMap.put(id, Boolean.TRUE);
    }
 
    /**
     * 批量添加黑名单
     *
     * @param ids
     */
    public static void addBlackList(Map<String,Boolean> ids) {
        blackListMap.putAll(ids);
    }
}
```
代码很简单，但是使用CopyOnWriteMap需要注意两件事情：<br />1. 减少扩容开销。根据实际需要，初始化CopyOnWriteMap的大小，避免写时CopyOnWriteMap扩容的开销。<br />2. 使用批量添加。因为每次添加，容器每次都会进行复制，所以减少添加次数，可以减少容器的复制次数。如使用上面代码里的addBlackList方法。
<a name="U6yD5"></a>
## CopyOnWrite的缺点
CopyOnWrite容器有很多优点，但是同时也存在两个问题，即内存占用问题和数据一致性问题。所以在开发的时候需要注意一下。
<a name="tvgh2"></a>
### 「内存占用问题」
因为CopyOnWrite的写时复制机制，所以在进行写操作的时候，内存里会同时驻扎两个对象的内存，旧的对象和新写入的对象（注意:在复制的时候只是复制容器里的引用，只是在写的时候会创建新对象添加到新容器里，而旧容器的对象还在使用，所以有两份对象内存）。如果这些对象占用的内存比较大，比如说200M左右，那么再写入100M数据进去，内存就会占用300M，那么这个时候很有可能造成频繁的Yong GC和Full GC。之前系统中使用了一个服务由于每晚使用CopyOnWrite机制更新大对象，造成了每晚15秒的Full GC，应用响应时间也随之变长。
<a name="9az3t"></a>
### 「针对内存占用问题」
可以通过压缩容器中的元素的方法来减少大对象的内存消耗，比如，如果元素全是10进制的数字，可以考虑把它压缩成36进制或64进制。或者不使用CopyOnWrite容器，而使用其他的并发容器，如ConcurrentHashMap。
<a name="hEdrj"></a>
### 「数据一致性问题」
CopyOnWrite容器只能保证数据的最终一致性，不能保证数据的实时一致性。所以如果希望写入的的数据，马上能读到，请不要使用CopyOnWrite容器。
<a name="jUoMV"></a>
## CopyOnWriteArrayList为什么并发安全且性能比Vector好
Vector是增删改查方法都加了synchronized，保证同步，但是每个方法执行的时候都要去获得锁，性能就会大大下降，而CopyOnWriteArrayList 只是在增删改上加锁，但是读不加锁，在读方面的性能就好于Vector，CopyOnWriteArrayList支持读多写少的并发情况。
