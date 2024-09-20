Java Copy-On-Write
<a name="4966e5a6"></a>
### 1、什么是写时复制(Copy-On-Write)容器？
写时复制是指：在并发访问的情景下，当需要修改JAVA中Containers的元素时，不直接修改该容器，而是先复制一份副本，在副本上进行修改。修改完成之后，将指向原来容器的引用指向新的容器(副本容器)。
<a name="674ab125"></a>
### 2、写时复制带来的影响

- 由于不会修改原始容器，只修改副本容器。因此，可以对原始容器进行并发地读。其次，实现了读操作与写操作的分离，读操作发生在原始容器上，写操作发生在副本容器上。<br />
- 数据一致性问题：读操作的线程可能不会立即读取到新修改的数据，因为修改操作发生在副本上。但最终修改操作会完成并更新容器，因此这是最终一致性。<br />
<a name="bYu4r"></a>
### 3、写时复制容器的实现
在JDK中提供了CopyOnWriteArrayList类和CopyOnWriteArraySet类，但是并没有提供CopyOnWriteMap的实现。因此，可以参考CopyOnWriteArrayList自己实现一个CopyOnWriteHashMap<br />主要是在写操作时，如何保证线程安全即可。
```java
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
public class CopyOnWriteMap<K, V> implements Map<K, V>, Cloneable{
    private volatile Map<K, V> internalMap;
    public CopyOnWriteMap() {
        internalMap = new HashMap<K, V>(100);//初始大小应根据实际应用来指定
    }
    @Override
    public V put(K key, V value) {
        synchronized (this) {
            Map<K, V> newMap = new HashMap<K, V>(internalMap);//复制出一个新HashMap
            V val = newMap.put(key, value);//在新HashMap中执行写操作
            internalMap = newMap;//将原来的Map引用指向新Map
            return val;
        }
    }
    @Override
    public void putAll(Map<? extends K, ? extends V> m) {
        synchronized (this) {
            Map<K, V> newMap = new HashMap<K, V>(internalMap);
            newMap.putAll(m);
            internalMap = newMap;
        }
    }
    @Override
    public V get(Object key) {
        V result = internalMap.get(key);
        return result;
    }
    ......//other methods inherit from interface Map
}
```
从上可以看出，对于`put()` 和 `putAll()` 而言，需要加锁。而读操作则不需要，如`get(Object key)`。这样，当一个线程需要put一个新元素时，它先锁住当前`CopyOnWriteMap`对象，并复制一个新`HashMap`，而其他的读线程因为不需要加锁，则可继续访问原来的`HashMap`。<br />这里加锁使用 synchronized 关键字；其实也可以使用 ReentrantLock 对象。官方对synchronized的优化，已经使得synchronized不在一直是重量级；而是满足某种情况才升级为重量级锁。
<a name="Ks2ho"></a>
### 4、应用场景
CopyOnWrite容器适用于读多写少的场景。因为写操作时，需要复制一个容器，造成内存开销很大，也需要根据实际应用把握初始容器的大小。<br />不适合于数据的强一致性场合。若要求数据修改之后立即能被读到，则不能用写时复制技术。因为它是最终一致性。<br />总结：写时复制技术是一种很好的提高并发性的手段。
<a name="r9qGt"></a>
### 5、Copy-On-Write的作用
集合类(ArrayList、HashMap)上的常用操作是：向集合中添加元素、删除元素、遍历集合中的元素然后进行某种操作。当多个线程并发地对一个集合对象执行这些操作时就会引发ConcurrentModificationException，比如线程A在for-each中遍历ArrayList，而线程B同时又在删除ArrayList中的元素，就可能会抛出ConcurrentModificationException，可以在线程A遍历ArrayList时加锁，但由于遍历操作是一种常见的操作，加锁之后会影响程序的性能，因此for-each遍历选择了不对ArrayList加锁而是当有多个线程修改ArrayList时抛出ConcurrentModificationException，因此，这是一种设计上的权衡。<br />为了应对多线程并发修改这种情况，一种策略就是这里的主题“写时复制”机制；另一种策略是：线程安全的容器类：
```java
ArrayList--->CopyOnWriteArrayList
HashMap--->ConcurrentHashMap
```
而ConcurrentHashMap并不是从“复制”这个角度来应对多线程并发修改，而是引入了分段锁(JDK7)；CAS、锁(JDK11)解决多线程并发修改的问题。
