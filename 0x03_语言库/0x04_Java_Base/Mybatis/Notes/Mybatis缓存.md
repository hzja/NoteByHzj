Mybatis 缓存
<a name="Gwauy"></a>
## Mybatis是什么？
Mybatis是一个半自动 ORM（对象关系映射）框架，它内部封装了JDBC，加载驱动、创建连接、创建 statement 等繁杂的过程，开发的时候只需要关注如何编写 SQL 语句，而不用关心其他的。<br />为什么说 Mybatis 是一个半自动 ORM 的框架呢？<br />ORM，是Object和Relation之间的映射，而Mybatis 在查询关联对象或关联集合对象时，需要手动编写 sql 来完成，所以，称之为半自动 ORM 框架，而Hibernate 属于全自动 ORM 映射工具，使用 Hibernate 查询关联对象或者关联集合对象时，可以根据对象关系模型直接获取，所以它是全自动的。<br />这也是为什么有些面试官在面试初级程序员的时候，很喜欢说，Mybatis，和 Hibernate 都有什么优缺点，为啥选择使用的 Mybatis 而不选择使用 Hibernate 呢？
<a name="XW9iA"></a>
## Mybatis的一级、二级缓存
<a name="Qv1n9"></a>
### Mybatis 的一级缓存
先说 Mybatis 的一级缓存，因为这是如果不手动配置，他是自己默认开启的一级缓存，一级缓存只是相对于同一个 SqlSession 而言，参数和SQL完全一样的情况下，使用同一个SqlSession对象调用一个Mapper方法，往往只执行一次SQL，因为使用SelSession第一次查询后，MyBatis会将其放在缓存中，以后再查询的时候，如果没有声明需要刷新，并且缓存没有超时的情况下，SqlSession都会取出当前缓存的数据，而不会再次发送SQL到数据库。<br />来画个图表示一下一级缓存<br />![2021-05-31-15-32-44-989511.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622446679184-9514bf6b-d910-4f7a-90ea-74950821bc06.png#clientId=u541c90c5-bf34-4&from=ui&id=u9fb1d643&originHeight=430&originWidth=1075&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6761&status=done&style=shadow&taskId=ub5118806-0a4d-4913-9e66-bd0931c0e8f&title=)<br />那面试官肯定会说，直接从数据库查不就行了，为啥要一级缓存呢？<br />当使用MyBatis开启一次和数据库的会话时，MyBatis 会创建出一个 SqlSession 对象表示一次与数据库之间的信息传递，在执行 SQL 语句的过程中，可能会反复执行完全相同的查询语句，如果不采取一些措施，每一次查询都会查询一次数据库，而如果在极短的时间内做了很多次相同的查询操作，那么这些查询返回的结果很可能相同。<br />也就是说，如果在短时间内，频繁的去执行一条 SQL，查询返回的结果本来应该是改变了，但是查询出来的时候，会出现结果一致的情况，正是为了解决这种问题，也为了减轻数据库的开销，所以 Mybatis 默认开启了一级缓存。
<a name="mn71s"></a>
### Mybatis 的二级缓存
Mybatis 的二级缓存一般如果不对他进行设置，他是不会开启的，而二级缓存是什么呢？Mybatis 中的二级缓存实际上就是 mapper 级别的缓存，而这时候肯定会有人说，那么不同之间的 Mapper 是同一个缓存么？<br />答案是否定的，他不是一个，Mapper 级别的缓存实际上就是相同的 Mapper 使用的是一个二级缓存，但是在二级缓存中，又有多个不同的 SqlSession，而不同的 Mapper 之间的二级缓存也就是互相不会影响的。<br />就类似下面的图<br />![2021-05-31-15-32-45-066274.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622446689829-2382aa2e-fa45-4694-874f-973b7236fc9e.png#clientId=u541c90c5-bf34-4&from=ui&id=u14f57e39&originHeight=612&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=47263&status=done&style=shadow&taskId=u236deb68-5e41-4e6f-8c48-c0016f0c125&title=)<br />这二级缓存是不是就看起来有点意思了？
<a name="kK5Ul"></a>
#### 开启二级缓存

1. MyBatis 配置文件
```xml
<settings>
 <setting name = "cacheEnabled" value = "true" />
</settings>
```

2. MyBatis 要求返回的 POJO 必须是可序列化的
3. Mapper 的 xml 配置文件中加入 标签

既然想要了解这个二级缓存，那么必然，还得知道它里面的配置都有哪些含义。<br />先从标签看起，然后从源码里面看都有哪些配置信息提供给开发者使用：<br />![2021-05-31-15-32-45-121129.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622446699630-7c13e79e-246b-40db-8ca6-9cdae2b6c3c4.png#clientId=u541c90c5-bf34-4&from=ui&id=u3fbeef9a&originHeight=220&originWidth=513&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12798&status=done&style=none&taskId=uc763167f-7dc4-4a7f-a270-c4180318009&title=)<br />`blocking`：直译就是调度，而在 Mybatis 中，如果缓存中找不到对应的 key ，是否会一直 blocking，直到有对应的数据进入缓存。<br />`eviction`：缓存回收策略
<a name="tcCNH"></a>
#### 缓存回收策略
而缓存回收策略，在源码中是有直接体现的，那么分别都对应了什么形式呢？
```java
typeAliasRegistry.registerAlias("PERPETUAL", PerpetualCache.class);
typeAliasRegistry.registerAlias("FIFO", FifoCache.class);
typeAliasRegistry.registerAlias("LRU", LruCache.class);
typeAliasRegistry.registerAlias("SOFT", SoftCache.class);
typeAliasRegistry.registerAlias("WEAK", WeakCache.class);
```

- PERPETUAL：选择 PERPETUAL 来命名缓存，暗示这是一个最底层的缓存，数据一旦存储进来，永不清除。好像这种缓存不怎么受待见。
- FIFO：先进先出：按对象进入缓存的顺序来移除它们
- LRU：最近最少使用的：移除最长时间不被使用的对象。
- SOFT：软引用：移除基于垃圾回收器状态和软引用规则的对象。
- WEAK：弱引用：更积极地移除基于垃圾收集器状态和弱引用规则的对象。

虽然看着 PERPETUAL 排在了第一位，但是它可不是默认的，在 Mybatis 的缓存策略里面，默认的是 LRU 。<br />PERPETUAL：<br />源代码如下：
```java
public class PerpetualCache implements Cache {
    private final String id;
    private Map<Object, Object> cache = new HashMap<>();
    public PerpetualCache(String id) {
        this.id = id;
    }
}
```
看着是不是有点眼熟，它怎么就只是包装了 HashMap ? <br />既然使用 HashMap，那么必然就会有Key，那么他们的Key是怎么设计的?<br />CacheKey：
```java
public class CacheKey implements Cloneable, Serializable {
    private static final long serialVersionUID = 1146682552656046210L;
    public static final CacheKey NULL_CACHE_KEY = new NullCacheKey();
    private static final int DEFAULT_MULTIPLYER = 37;
    private static final int DEFAULT_HASHCODE = 17;
    private final int multiplier;
    private int hashcode; //用于表示CacheKey的哈希码
    private long checksum; //总和校验，当出现复合key的时候，分布计算每个key的哈希码，然后求总和
    private int count;//当出现复合key的时候，计算key的总个数
    // 8/21/2017 - Sonarlint flags this as needing to be marked transient.  While true if content is not serializable, this is not always true and thus should not be marked transient.
    private List<Object> updateList;//当出现复合key的时候，保存每个key
}
```
至于内部如何初始化，如何进行操作，有兴趣的可以去阅读一下源码，导入个源码包，打开自己看一下。
<a name="RSTmk"></a>
##### FIFO：先进先出缓冲淘汰策略
```java
public class FifoCache implements Cache {

    private final Cache delegate; //被装饰的Cache对象
    private final Deque<Object> keyList;//用于记录key 进入缓存的先后顺序
    private int size;//记录了缓存页的上限，超过该值需要清理缓存（FIFO）

    public FifoCache(Cache delegate) {
        this.delegate = delegate;
        this.keyList = new LinkedList<>();
        this.size = 1024;
    }
}
```
在 FIFO 淘汰策略中使用了 Java 中的 Deque，而 Deque 一种常用的数据结构，可以将队列看做是一种特殊的线性表，该结构遵循的先进先出原则。Java中，LinkedList实现了Queue接口，因为LinkedList进行插入、删除操作效率较高。<br />看完这个源码的时候，是不是就感觉源码其实也没有那么难看懂，里面都是已经掌握好的知识，只不过中间做了一些操作，进行了一些封装。
<a name="LbuKs"></a>
##### LRU：最近最少使用的缓存策略
需要看的源码则是在 Mybatis 中的源码，
```java
public class LruCache implements Cache {

    private final Cache delegate;
    private Map<Object, Object> keyMap;
    private Object eldestKey;//记录最少被使用的缓存项key

    public LruCache(Cache delegate) {
        this.delegate = delegate;
        setSize(1024);//重新设置缓存的大小，会重置KeyMap 字段 如果到达上限 则更新eldestKey
    }
    public void putObject(Object key, Object value) {
        delegate.putObject(key, value);
        // 删除最近未使用的key
        cycleKeyList(key);
    }
}
```
<a name="uR39n"></a>
##### SOFT：基于垃圾回收器状态和软引用规则的对象
```java
public class SoftCache implements Cache {
    //在SoftCache 中，最近使用的一部分缓存项不会被GC回收，这就是通过将其value添加到
    private final Deque<Object> hardLinksToAvoidGarbageCollection;
    //引用队列，用于记录GC回收的缓存项所对应的SoftEntry对象
    private final ReferenceQueue<Object> queueOfGarbageCollectedEntries;
    //底层被修饰的Cache 对象
    private final Cache delegate;
    //连接的个数，默认是256
    private int numberOfHardLinks;

    public SoftCache(Cache delegate) {
        this.delegate = delegate;
        this.numberOfHardLinks = 256;
        this.hardLinksToAvoidGarbageCollection = new LinkedList<>();
        this.queueOfGarbageCollectedEntries = new ReferenceQueue<>();
    }

    public void putObject(Object key, Object value) {
        // 清除被GC回收的缓存项
        removeGarbageCollectedItems();
        // 向缓存中添加缓存项
        delegate.putObject(key, new SoftEntry(key, value, queueOfGarbageCollectedEntries));
    }
    public Object getObject(Object key) {
        Object result = null;
        // 查找对应的缓存项
        @SuppressWarnings("unchecked") // assumed delegate cache is totally managed by this cache
        SoftReference<Object> softReference = (SoftReference<Object>) delegate.getObject(key);
        if (softReference != null) {
            result = softReference.get();
            // 已经被GC 回收
            if (result == null) {
                // 从缓存中清除对应的缓存项
                delegate.removeObject(key);
            } else {
                // See #586 (and #335) modifications need more than a read lock 
                synchronized (hardLinksToAvoidGarbageCollection) {
                    hardLinksToAvoidGarbageCollection.addFirst(result);
                    if (hardLinksToAvoidGarbageCollection.size() > numberOfHardLinks) {
                        hardLinksToAvoidGarbageCollection.removeLast();
                    }
                }
            }
        }
        return result;
    }
    public void clear() {
        synchronized (hardLinksToAvoidGarbageCollection) {
            // 清理强引用集合
            hardLinksToAvoidGarbageCollection.clear();
        }
        // 清理被GC回收的缓存项
        removeGarbageCollectedItems();
        delegate.clear();
    }
    //其中指向key的引用是强引用，而指向value的引用是弱引用
    private static class SoftEntry extends SoftReference<Object> {
        private final Object key;

        SoftEntry(Object key, Object value, ReferenceQueue<Object> garbageCollectionQueue) {
            super(value, garbageCollectionQueue);
            this.key = key;
        }
    }
}
```
<a name="hOVlR"></a>
##### WEAK：基于垃圾收集器状态和弱引用规则的对象
```java
public class WeakCache implements Cache {
    private final Deque<Object> hardLinksToAvoidGarbageCollection;
    private final ReferenceQueue<Object> queueOfGarbageCollectedEntries;
    private final Cache delegate;
    private int numberOfHardLinks;

    public WeakCache(Cache delegate) {
        this.delegate = delegate;
        this.numberOfHardLinks = 256;
        this.hardLinksToAvoidGarbageCollection = new LinkedList<>();
        this.queueOfGarbageCollectedEntries = new ReferenceQueue<>();
    }
}
```
WeakCache在实现上与SoftCache几乎相同，只是把引用对象由SoftReference软引用换成了WeakReference弱引用。
