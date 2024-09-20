Java ThreadLocal 内存泄露
<a name="CyKGS"></a>
### ThreadLocal 解决什么问题
ThreadLocal是为了解决对象不能被多线程共享访问的问题，通过 threadLocal.set() 方法将对象实例保存在每个线程自己所拥有的 threadLocalMap 中，这样的话每个线程都使用自己的对象实例，彼此不会影响从而达到了隔离的作用，这样就解决了对象在被共享访问时带来的线程安全问题<br />先把 Thread， ThreadLocal， ThreadLocalMap 的关系捋一捋：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1599726835281-40885285-2ba0-4ca3-8529-3adb55fb39f7.webp#align=left&display=inline&height=384&originHeight=384&originWidth=903&size=0&status=done&style=shadow&width=903)<br />可以看到，在 Thread 中持有一个 ThreadLocalMap ， ThreadLocalMap 又是由 Entry 来组成的，在 Entry 里面有 ThreadLocal 和 value
<a name="Rqc8B"></a>
### ThreadLocal 为什么会内存泄漏
首先是因为 ThreadLocal 是基于 ThreadLocalMap 实现的，其中 ThreadLocalMap 的 Entry 继承了 WeakReference ，而 Entry 对象中的 key 使用了 WeakReference 封装，也就是说， Entry 中的 key 是一个弱引用类型，对于弱引用来说，它只能存活到下次 GC 之前。<br />如果此时一个线程调用了 ThreadLocalMap 的 set 设置变量，当前的 ThreadLocalMap 就会新增一条记录，但由于发生了一次垃圾回收，这样就会造成一个结果: key 值被回收掉了，但是 value 值还在内存中，而且如果线程一直存在的话，那么它的 value 值就会一直存在<br />这样被垃圾回收掉的 key 就会一直存在一条引用链: Thread -> ThreadLocalMap -> Entry -> Value :<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1599726835296-002af335-8c32-42b8-aa9f-46fb27c280d0.webp#align=left&display=inline&height=476&originHeight=476&originWidth=1035&size=0&status=done&style=shadow&width=1035)<br />就是因为这条引用链的存在，就会导致如果 Thread 还在运行，那么 Entry 不会被回收，进而 value 也不会被回收掉，但是 Entry 里面的 key 值已经被回收掉了<br />这只是一个线程，如果再来一个线程，又来一个线程…多了之后就会造成内存泄漏<br />知道是怎么造成内存泄漏之后，接下来要做的事情就好说了，因为 value 值没有被回收掉所以才会导致内存泄露。<br />所以在使用完 key 值之后，将 value 值通过 remove 方法 remove 掉，这样的话内存中就不会有 value 值了，也就防止了内存泄漏。
<a name="lS2CE"></a>
### ThreadLocal 是基于 ThreadLocalMap 实现的
在源码中能够看到下面这几行代码：
```java
public class ThreadLocal<T> {
    static class ThreadLocalMap {
        static class Entry extends WeakReference<ThreadLocal<?>> {
            /** The value associated with this ThreadLocal. */
            Object value;
            Entry(ThreadLocal<?> k, Object v) {
                super(k);
                value = v;
            }
        }
    }
}
```
代码中说的很清楚，在 ThreadLocal 内部维护着 ThreadLocalMap ，而它的 Entry 则继承自 WeakReference 的 ThreadLocal<?> ，其中 Entry 的 k 为 ThreadLocal ， v 为 Object ，在调用 super(k) 时就将 ThreadLocal 实例包装成了一个 WeakReference

| 引用类型 | 功能特点 |
| :---: | :---: |
| 强引用 ( Strong Reference ) | 被强引用关联的对象永远不会被垃圾回收器回收掉 |
| 软引用( Soft Reference ) | 软引用关联的对象，只有当系统将要发生内存溢出时，才会去回收软引用引用的对象 |
| 弱引用 ( Weak Reference ) | 只被弱引用关联的对象，只要发生垃圾收集事件，就会被回收 |
| 虚引用 ( Phantom Reference ) | 被虚引用关联的对象的唯一作用是能在这个对象被回收器回收时收到一个系统通知 |

从表格中应该能够看出来，弱引用的对象只要发生垃圾收集事件，就会被回收。所以弱引用的存活时间也就是下次 GC 之前了。为什么 ThreadLocal 采用弱引用，而不是强引用？<br />在 ThreadLocalMap 上面有些注释，在这里摘录一部分，或许可以从中窥探一二：
> To help deal with very large and long-lived usages， the hash table entries use WeakReferences for keys

翻译一下就是:<br />为了解决非常大且长期使用的问题，哈希表使用了弱引用的 key。<br />假设， ThreadLocal 使用的是强引用，会怎样呢？<br />如果是强引用的话，在表格中也能够看出来，被强引用关联的对象，永远都不会被垃圾回收器回收掉<br />如果引用的 ThreadLocal 对象被回收了，但是 ThreadLocalMap 还持有对 ThreadLocal 的强引用，如果没有 remove 的话， 在 GC 时进行可达性分析， ThreadLocal 依然可达，这样就不会对 ThreadLocal 进行回收，但是期望的是引用的 ThreadLocal 对象被回收，这样就不能达不到期望的目的了。<br />使用弱引用的话，虽然会出现内存泄漏的问题，但是在 ThreadLocal 生命周期里面，都有对 key 值为 null 时进行回收的处理操作。所以，使用弱引用的话，可以在 ThreadLocal 生命周期中尽可能保证不出现内存泄漏的问题。在 ThreadLcoal 生命周期里面，都有对 key 值为 null 时进行回收的处理操作。
<a name="nAgPM"></a>
### 源码分析
<a name="Ljz2q"></a>
#### ThreadLocal.set()
```java
private void set(ThreadLocal<?> key, Object value) {

    // We don't use a fast path as with get() because it is at
    // least as common to use set() to create new entries as
    // it is to replace existing ones, in which case, a fast
    // path would fail more often than not.

    Entry[] tab = table;
    int len = tab.length;
    int i = key.threadLocalHashCode & (len-1);

    for (Entry e = tab[i];
        // 如果 e 不为空,说明 hash 冲突,需要向后查找
        e != null;
        // 从这里可以看出, ThreadLocalMap 采用的是开放地址法解决的 hash 冲突
        // 是最经典的 线性探测法 --> 我觉得之所以选择这种方法解决冲突时因为数据量不大
        e = tab[i = nextIndex(i, len)]) {
        ThreadLocal<?> k = e.get();

        // 要查找的 ThreadLocal 对象找到了,直接设置需要设置的值,然后 return
        if (k == key) {
            e.value = value;
            return;
        }

        // 如果 k 为 null ,说明有 value 没有及时回收,此时通过 replaceStaleEntry 进行处理
        // replaceStaleEntry 具体内容等下分析
        if (k == null) {
            replaceStaleEntry(key, value, i);
            return;
        }
    }

    // 如果 tab[i] == null ,则直接创建新的 entry 即可
    tab[i] = new Entry(key, value);
    int sz = ++size;
    // 在创建之后调用 cleanSomeSlots 方法检查是否有 value 值没有及时回收
    // 如果 sz >= threshold ,则需要扩容,重新 hash 即, rehash();
    if (!cleanSomeSlots(i, sz) && sz >= threshold)
        rehash();
}
```
通过源码可以看到，在 set 方法中，主要是通过 `replaceStaleEntry` 方法和 `cleanSomeSlots` 方法去做的检测和处理
<a name="aGv6J"></a>
#### replaceStaleEntry
```java
private void replaceStaleEntry(ThreadLocal<?> key, Object value,
                                int staleSlot) {
    Entry[] tab = table;
    int len = tab.length;
    Entry e;

    // 从当前 staleSlot 位置开始向前遍历
    int slotToExpunge = staleSlot;
    for (int i = prevIndex(staleSlot, len);
        (e = tab[i]) != null;
        i = prevIndex(i, len))
        if (e.get() == null)
            // 当 e.get() == null 时, slotToExpunge 记录下此时的 i 值
            // 即 slotToExpunge 记录的是 staleSlot 左手边第一个空的 Entry
            slotToExpunge = i;

    // 接下来从当前 staleSlot 位置向后遍历
    // 这两个遍历是为了清理在左边遇到的第一个空的 entry 到右边的第一个空的 entry 之间所有过期的对象
    // 但是如果在向后遍历过程中,找到了需要设置值的 key ,就开始清理,不会再继续向下遍历
    for (int i = nextIndex(staleSlot, len);
        (e = tab[i]) != null;
        i = nextIndex(i, len)) {
        ThreadLocal<?> k = e.get();

        // 如果 k == key 说明在插入之前就已经有相同的 key 值存在,所以需要替换旧的值
        // 同时和前面过期的对象进行交换位置
        if (k == key) {
            e.value = value;

            tab[i] = tab[staleSlot];
            tab[staleSlot] = e;

            // 如果 slotToExpunge == staleSlot 说明向前遍历时没有找到过期的
            if (slotToExpunge == staleSlot)
                slotToExpunge = i;
            // 进行清理过期数据
            cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);
            return;
        }

        // 如果在向后遍历时,没有找到 value 被回收的 Entry 对象
        // 且刚开始 staleSlot 的 key 为空,那么它本身就是需要设置 value 的 Entry 对象
        // 此时不涉及到清理
        if (k == null && slotToExpunge == staleSlot)
            slotToExpunge = i;
    }

    // 如果 key 在数组中找不到,那就好说了,直接创建一个新的就可以了
    tab[staleSlot].value = null;
    tab[staleSlot] = new Entry(key, value);

    // 如果 slotToExpunge != staleSlot 说明存在过期的对象,就需要进行清理
    if (slotToExpunge != staleSlot)
        cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);
}
```
在 `replaceStaleEntry` 方法中,需要注意一下刚开始的两个 for 循环中内容（在这里再贴一下）：
```java
if (e.get() == null)
    // 当 e.get() == null 时, slotToExpunge 记录下此时的 i 值
    // 即 slotToExpunge 记录的是 staleSlot 左手边第一个空的 Entry
    slotToExpunge = i;

if (k == key) {
    e.value = value;

    tab[i] = tab[staleSlot];
    tab[staleSlot] = e;
                                        
    // 如果 slotToExpunge == staleSlot 说明向前遍历时没有找到过期的
    if (slotToExpunge == staleSlot)
        slotToExpunge = i;
        // 进行清理过期数据
        cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);
        return;
}
```
这两个 for 循环中的 if 到底是在做什么？<br />看第一个 if ，当 `e.get() == null` 时，此时将 i 的值给 slotToExpunge<br />第二个 if ，当 `k ==key` 时，此时将 i 给了 staleSlot 来进行交换<br />为什么要对 `staleSlot` 进行交换呢？画图说明一下<br />如下图，假设此时表长为 10 ，其中下标为 3 和 5 的 key 已经被回收（ key 被回收掉的就是 null ），因为采用的开放地址法，所以 15 mod 10 应该是 5 ，但是因为位置被占，所以在 6 的位置，同样 25 mod 10 也应该是 5 ，但是因为位置被占，下个位置也被占，所以就在第 7 号的位置上了<br />按照上面的分析，此时 `slotToExpunge` 值为 3 ， `staleSlot` 值为 5 ， i 为 6<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1599793854228-809512f0-1a4d-4319-a843-011cc865eb16.webp#align=left&display=inline&height=210&originHeight=210&originWidth=704&size=0&status=done&style=shadow&width=704)<br />假设，假设这个时候如果不进行交换，而是直接回收的话，此时位置为 5 的数据就被回收掉，然后接下来要插入一个 key 为 15 的数据，此时 15 mod 10 算出来是 5 ，正好这个时候位置为 5 的被回收完毕，这个位置就被空出来了，那么此时就会这样：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1599793854173-0949cad7-70ff-42cd-9d52-3ba746d70139.webp#align=left&display=inline&height=222&originHeight=222&originWidth=738&size=0&status=done&style=shadow&width=738)<br />同样的 key 值竟然出现了两次？！<br />这肯定是不希望看到的结果，所以一定要进行数据交换<br />在上面代码中有一行代码 `cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);` ，说明接下来的处理是交给了 `expungeStaleEntry` ，接下来去分析一下 `expungeStaleEntry`
<a name="expungeStaleEntry"></a>
#### expungeStaleEntry
```java
private int expungeStaleEntry(int staleSlot) {
    Entry[] tab = table;
    int len = tab.length;

    // expunge entry at staleSlot
    tab[staleSlot].value = null;
    tab[staleSlot] = null;
    size--;

    // Rehash until we encounter null
    Entry e;
    int i;
    for (i = nextIndex(staleSlot, len);
        (e = tab[i]) != null;
        i = nextIndex(i, len)) {
        ThreadLocal<?> k = e.get();
        // 如果 k == null ,说明 value 就应该被回收掉
        if (k == null) {
            // 此时直接将 e.value 置为 null 
            // 这样就将 thread -> threadLocalMap -> value 这条引用链给打破
            // 方便了 GC
            e.value = null;
            tab[i] = null;
            size--;
        } else {
            // 这个时候要重新 hash ,因为采用的是开放地址法,所以可以理解为就是将后面的元素向前移动
            int h = k.threadLocalHashCode & (len - 1);
            if (h != i) {
                tab[i] = null;

                // Unlike Knuth 6.4 Algorithm R, we must scan until
                // null because multiple entries could have been stale.
                while (tab[h] != null)
                    h = nextIndex(h, len);
                tab[h] = e;
            }
        }
    }
    return i;
}
```
因为是在 `replaceStaleEntry` 方法中调用的此方法，传进来的值是 `staleSlot` ，继续上图，经过 `replaceStaleEntry` 之后，它的数据结构是这样：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1599794443383-3349d9fe-c478-4e20-a523-929b2fa97225.webp#align=left&display=inline&height=224&originHeight=224&originWidth=698&size=0&status=done&style=shadow&width=698)<br />此时传进来的 `staleSlot` 值为 6 ，因为此时的 key 为 null ，所以接下来会走 `e.value = null` ，这一步结束之后，就成了：<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1599794368656-65a1132d-5edb-4678-9ae5-9b33a5cbcb62.webp#align=left&display=inline&height=238&originHeight=238&originWidth=706&size=0&status=done&style=shadow&width=706)<br />接下来 i 为 7 ，此时的 key 不为 null ，那么就会重新 hash : `int h = k.threadLocalHashCode & (len - 1);` ，得到的 h 应该是 5 ，但是实际上 i 为 7 ，说明出现了 hash 冲突，就会继续向下走，最终的结果是这样：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599794561994-325b1ea9-0e1d-49ed-933b-d2baae47e11f.png#align=left&display=inline&height=179&originHeight=538&originWidth=1644&size=275122&status=done&style=shadow&width=548)<br />可以看到，原来的 key 为 null ，值为 V5 的已经被回收掉了。所以回收掉之后，还要再次进行重新 hash ，就是为了防止 key 值重复插入情况的发生<br />假设 key 为 25 的并没有进行向前移动，也就是它还在位置 7 ，位置 6 是空的，再插入一个 key 为 25 ，经过 hash 应该在位置 5 ，但是有数据了，那就向下走，到了位置 6 ，诶，竟然是空的，赶紧插进去，这不就又造成了上面说到的问题，同样的一个 key 竟然出现了两次？！<br />而且经过 `expungeStaleEntry` 之后，将 key 为 null 的值，也设置为了 null ，这样就方便 GC<br />分析到这里应该就比较明确了，在 `expungeStaleEntry` 中，有些地方是帮助 GC 的，而通过源码能够发现， set 方法调用了该方法进行了 GC 处理， get 方法也有，看下面的源码：
<a name="bbpYn"></a>
#### ThreadLocal.get()
```java
private Entry getEntry(ThreadLocal<?> key) {
    int i = key.threadLocalHashCode & (table.length - 1);
    Entry e = table[i];
    // 如果能够找到寻找的值,直接 return 即可
    if (e != null && e.get() == key)
        return e;
    else
        // 如果找不到,则调用 getEntryAfterMiss 方法去处理
        return getEntryAfterMiss(key, i, e);
}

private Entry getEntryAfterMiss(ThreadLocal<?> key, int i, Entry e) {
    Entry[] tab = table;
    int len = tab.length;

    // 一直探测寻找下一个元素,直到找到的元素是要找的
    while (e != null) {
        ThreadLocal<?> k = e.get();
        if (k == key)
            return e;
        if (k == null)
            // 如果 k == null 说明有 value 没有及时回收
            // 调用 expungeStaleEntry 方法去处理,帮助 GC
            expungeStaleEntry(i);
        else
            i = nextIndex(i, len);
        e = tab[i];
    }
    return null;
}
```
get 和 set 方法都有进行帮助 GC ，所以正常情况下是不会有内存溢出的，但是如果创建了之后一直没有调用 get 或者 set 方法，还是有可能会内存溢出。所以最保险的方法就是，使用完之后就及时 remove 一下，加快垃圾回收，就完美的避免了内存溢出。
