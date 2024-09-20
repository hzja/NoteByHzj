Java
<a name="uI01p"></a>
## 什么是COW？
在java里面说到集合容器一般首先会想到的是`HashMap`、`ArrayList`、`HasHSet`这几个容器也是平时开发中用的最多的。这几个都是非线程安全的，如果有特定业务需要使用线程的安全容器列，

- `HashMap`可以用`ConcurrentHashMap`代替。
- `ArrayList` 可以使用`Collections.synchronizedList()`方法(`list` 每个方法都用`synchronized`修饰) 或者使用`Vector`(现在基本也不用了，每个方法都用`synchronized`修饰) 或者使用`CopyOnWriteArrayList` 替代。
- HasHSet 可以使用 `Collections.synchronizedSet` 或者使用`CopyOnWriteArraySet`来代替。（CopyOnWriteArraySet为什么不叫CopyOnWriteHashSet因为`CopyOnWriteArraySet`底层是采用`CopyOnWriteArrayList`来实现的） 可以看到`CopyOnWriteArrayList`在线程安全的容器里面多次出现。首先来看看什么是`CopyOnWrite`？`Copy-On-Write`简称`COW`，是一种用于程序设计中的优化策略。
> CopyOnWrite容器即写时复制的容器。通俗的理解是往一个容器添加元素的时候，不直接往当前容器添加，而是先将当前容器进行Copy，复制出一个新的容器，然后新的容器里添加元素，添加完元素之后，再将原容器的引用指向新的容器。这样做的好处是可以对CopyOnWrite容器进行并发的读，而不需要加锁，因为当前容器不会添加任何元素。所以CopyOnWrite容器也是一种读写分离的思想，读和写不同的容器。

<a name="Nr0AF"></a>
## 为什么要引入COW
<a name="fMFf3"></a>
### 防止ConcurrentModificationException异常
在java里面如果采用不正确的循环姿势去遍历List时候，如果一边遍历一边修改抛出`java.util.ConcurrentModificationException`错误的。
```java
List<String> list = new ArrayList<>();
list.add("张三");
list.add("Fcant");
list.add("Fcant.cn");
Iterator<String> iterator = list.iterator();
while(iterator.hasNext()){
    String content = iterator.next();
    if("张三".equals(content)) {
        list.remove(content);
    }
}
```
上面这个栗子是会发生`java.util.ConcurrentModificationException`异常的，如果把`ArrayList`改为`CopyOnWriteArrayList` 是不会发生生异常的。
<a name="5ovVV"></a>
### 线程安全的容器
再看下面一个线程往List里面添加数据的例子，一个线程循环list读数据。
```java
List<String> list = new ArrayList<>();
list.add("张三");
list.add("Fcant");
list.add("Fcant.cn");
Thread t = new Thread(new Runnable() {
    int count = 0;
    @Override
    public void run() {
        while (true) {
            list.add(count++ + "");
        }
    }
});
t.start();
Thread.sleep(10000);
for (String s : list) {
    System.out.println(s);
}
```
运行上述代码也会发生`ConcurrentModificationException`异常，如果把`ArrayList`换成了`CopyOnWriteArrayList`就一切正常。
<a name="WAdsW"></a>
## CopyOnWriteArrayList的实现
通过上面两个栗子可以发现`CopyOnWriteArrayList`是线程安全的，下面就来一起看看`CopyOnWriteArrayList`是如何实现线程安全的。
```java
public class CopyOnWriteArrayList<E>
    implements List<E>, RandomAccess, Cloneable, java.io.Serializable {
    private static final long serialVersionUID = 8673264195747942595L;
    /** The lock protecting all mutators */
    final transient ReentrantLock lock = new ReentrantLock();
    /** The array, accessed only via getArray/setArray. */
    private transient volatile Object[] array;
```
从源码中可以知道`CopyOnWriteArrayList`这和`ArrayList`底层实现都是通过一个`Object`的数组来实现的，只不过 `CopyOnWriteArrayList`的数组是通过`volatile`来修饰的。
<a name="diubF"></a>
### add方法
```java
public boolean add(E e) {
    // 先获取锁
    final ReentrantLock lock = this.lock;
    lock.lock();
    try {
        Object[] elements = getArray();
        int len = elements.length;
        // 复制一个新的数组
        Object[] newElements = Arrays.copyOf(elements, len + 1);
        newElements[len] = e;
        // 把新数组的值 赋给原数组
        setArray(newElements);
        return true;
    } finally {
        // 释放锁
        lock.unlock();
    }
}
```
上述源码可以发现比较简单，有几个点需要稍微注意下

- 增加数据的时候是通过`ReentrantLock`加锁操作来（在`jdk11`的时候采用了`synchronized`来替换`ReentrantLock`)保证多线程写的时候只有一个线程进行数组的复制，否则的话内存中会有多份被复制的数据，导致数据错乱。
- 数组是通过`volatile` 修饰的，根据 `volatile` 的 `happens-before` 规则，写线程对数组引用的修改是可以立即对读线程是可见的。
- 通过写时复制来保证读写在两个不同的数据容器中进行操作。
<a name="FNkze"></a>
## 自己实现一个COW容器
Java并发包里提供了两个使用`CopyOnWrite`机制实现的并发容器,它们是`CopyOnWriteArrayList`和`CopyOnWriteArraySet`，但是并没有`CopyOnWriteHashMap`可以按照他的思路自己来实现一个`CopyOnWriteHashMap`
```java
public class CopyOnWriteHashMap<K, V> implements Map<K, V>, Cloneable {
    final transient ReentrantLock lock = new ReentrantLock();
    private volatile Map<K, V> map;
    public CopyOnWriteHashMap() {
        map = new HashMap<>();
    }
    @Override
    public V put(K key, V value) {
        final ReentrantLock lock = this.lock;
        lock.lock();
        try {
            Map<K, V> newMap = new HashMap<K, V>(map);
            V val = newMap.put(key, value);
            map = newMap;
            return val;
        } finally {
            lock.unlock();
        }
    }
    @Override
    public V get(Object key) {
        return map.get(key);
    }
    @Override
    public V remove(Object key) {
        final ReentrantLock lock = this.lock;
        lock.lock();
        try {
            Map<K, V> newMap = new HashMap<K, V>(map);
            if (!newMap.containsKey(key)) {
                return null;
            }
            V v = newMap.get(key);
            newMap.remove(key);
            map = newMap;
            return v;
        }finally {
            lock.unlock();
        }
    }
```
上述实现了一个简单的`CopyOnWriteHashMap`，只实现了`add、remove、get`方法其他剩余的方法可以自行去实现，涉及到只要数据变化的就要加锁，读无需加锁。
<a name="rlGem"></a>
## 应用场景
`CopyOnWrite`并发容器适用于读多写少的并发场景，比如黑白名单、国家城市等基础数据缓存、系统配置等。这些基本都是只要想项目启动的时候初始化一次，变更频率非常的低。如果这种读多写少的场景采用 `Vector,Collections`包装的这些方式是不合理的，因为尽管多个读线程从同一个数据容器中读取数据，但是读线程对数据容器的数据并不会发生发生修改，所以并不需要读也加锁。
<a name="EIBZH"></a>
## CopyOnWrite缺点
CopyOnWriteArrayList虽然是一个线程安全版的ArrayList，但其每次修改数据时都会复制一份数据出来，所以CopyOnWriteArrayList只适用读多写少或无锁读场景。如果在实际业务中使用CopyOnWriteArrayList，一定是因为这个场景适合而非是为了炫技。
<a name="Oohan"></a>
### 内存占用问题
因为CopyOnWrite的写时复制机制每次进行写操作的时候都会有两个数组对象的内存，如果这个数组对象占用的内存较大的话，如果频繁的进行写入就会造成频繁的Yong GC和Full GC。
<a name="5UZyF"></a>
### 数据一致性问题
CopyOnWrite容器只能保证数据的最终一致性，不能保证数据的实时一致性。读操作的线程可能不会立即读取到新修改的数据，因为修改操作发生在副本上。但最终修改操作会完成并更新容器所以这是最终一致性。
<a name="9MkeW"></a>
## CopyOnWriteArrayList和`Collections.synchronizedList()`
简单的测试了下CopyOnWriteArrayList  和  `Collections.synchronizedList()`的读和写发现：

- 在高并发的写时CopyOnWriteArray比同步Collections.synchronizedList慢百倍
- 在高并发的读性能时CopyOnWriteArray比同步Collections.synchronizedList快几十倍。
- 高并发写时，CopyOnWriteArrayList为何这么慢呢？因为其每次add时，都用Arrays.copyOf创建新数组，频繁add时内存申请释放性能消耗大。
- 高并发读的时候CopyOnWriteArray无锁，Collections.synchronizedList有锁所以读的效率比较低下。
<a name="iDuGh"></a>
## 总结
选择CopyOnWriteArrayList的时候一定是读远大于写。如果读写都差不多的话建议选择Collections.synchronizedList。
