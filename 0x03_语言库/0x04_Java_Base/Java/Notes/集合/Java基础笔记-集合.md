Java 集合
<a name="Cd5xk"></a>
# ①、集合相关思维导图
![Java集合.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566866493104-ccd49cc2-a975-4aa9-9fea-a02c9ae58dcd.png#height=5912&id=A7VSo&originHeight=5912&originWidth=3618&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2153399&status=done&style=none&title=&width=3618)
<a name="NLQQA"></a>
# ②、集合
<a name="UuEpI"></a>
## 1、Collection接口
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566781967203-c390e97f-c101-4f54-a2fd-ec11222789e3.png#height=532&id=zmC5Y&originHeight=1463&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=275446&status=done&style=shadow&title=&width=1381.090909090909)<br />[Collection_1.png](https://www.yuque.com/attachments/yuque/0/2019/png/396745/1566746699440-272afef7-cd22-44a5-b2c9-ad1357bfc6be.png?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fpng%2F396745%2F1566746699440-272afef7-cd22-44a5-b2c9-ad1357bfc6be.png%22%2C%22name%22%3A%22Collection_1.png%22%2C%22size%22%3A119653%2C%22type%22%3A%22image%2Fpng%22%2C%22ext%22%3A%22png%22%2C%22status%22%3A%22done%22%2C%22uid%22%3A%22rc-upload-1566746569678-7%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22id%22%3A%22GMinE%22%2C%22card%22%3A%22file%22%7D)<br />[AbstractCollection.png](https://www.yuque.com/attachments/yuque/0/2019/png/396745/1566783276726-ed098494-73d4-420a-9c56-d84fcdc1da11.png?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fpng%2F396745%2F1566783276726-ed098494-73d4-420a-9c56-d84fcdc1da11.png%22%2C%22name%22%3A%22AbstractCollection.png%22%2C%22size%22%3A415183%2C%22type%22%3A%22image%2Fpng%22%2C%22ext%22%3A%22png%22%2C%22status%22%3A%22done%22%2C%22uid%22%3A%22rc-upload-1566782207310-7%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22id%22%3A%22TBxHR%22%2C%22card%22%3A%22file%22%7D)

![Collection.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566746793051-b8b68aa8-a6e6-4770-ab04-64cc124afdec.png#height=15882&id=uN05p&originHeight=15882&originWidth=11105&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4060107&status=done&style=shadow&title=&width=11105)<br />[Collection.png](https://www.yuque.com/attachments/yuque/0/2019/png/396745/1566746809864-1884288d-b62c-4a8e-b0e3-3e27b3a1dca4.png?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fpng%2F396745%2F1566746809864-1884288d-b62c-4a8e-b0e3-3e27b3a1dca4.png%22%2C%22name%22%3A%22Collection.png%22%2C%22size%22%3A4060107%2C%22type%22%3A%22image%2Fpng%22%2C%22ext%22%3A%22png%22%2C%22status%22%3A%22done%22%2C%22uid%22%3A%22rc-upload-1566746569678-17%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22id%22%3A%22w552D%22%2C%22card%22%3A%22file%22%7D)
<a name="jVarV"></a>
### A.List集合
List 集合存储的是有序的数据集合，其数据结构特点是：读取快，修改慢，适合于读取多、写入修改少的场景。
<a name="W3HWt"></a>
#### 1、list集合的类继承结构如下：
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566782789106-85df4b31-d107-466f-a99d-7c7cb94bd86c.png#height=470&id=V9vou&originHeight=1293&originWidth=2196&originalType=binary&ratio=1&rotation=0&showTitle=false&size=141951&status=done&style=shadow&title=&width=798.5454545454545)
<a name="n16NS"></a>
#### 2、List列表实现
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566785712491-e1bed2d3-5790-49cb-976b-36d9b15e66d3.png#height=444&id=ThUew&originHeight=1220&originWidth=2081&originalType=binary&ratio=1&rotation=0&showTitle=false&size=341325&status=done&style=shadow&title=&width=756.7272727272727)<br />ArrayList 类是很常用的 List 实现，其底层是用数组实现的。其读取元素的时间复杂度是 O(1)，修改写入元素的时间复杂度是 O(N)。
<a name="jfgUY"></a>
#### 3、List列表安全实现
Vector 类也是很常用的 List 实现，其数据结构与 ArrayList 非常类似。但其与 ArrayList 的一个最大的不同是：Vector 是线程安全的，而 ArrayList 则不是线程安全的。<br />Stack 类则是在 Vector 的基础上，又实现了一个双向队列。所以其除了是线程安全的之外，其还是一个先进后出的 List 实现。<br />List 集合最为关键的几个实现类是：

- ArrayList：列表集合经典实现。
- Vector：列表集合经典实现，线程安全，与 ArrayList 对应。
- Stack：栈结构的经典实现，先进后出的数据结构。继承了 Vector，线程安全。
- LinkedList：链表结构的经典实现。
<a name="bCDkd"></a>
#### 4、List链表实现
LinkedList 是一个经典的链表实现。LinkedList 继承了 AbstractSequentialList 抽象类。AbstractSequentialList 抽象类从字面上理解是抽象连续列表。这里的重点是 sequential 这个词，表示其数据结构是连续的（链表）。
<a name="EStmk"></a>
### B.Set集合
Set 集合中存储的元素是不重复的，但是其存储顺序是无序的。
<a name="vUTmz"></a>
#### 1、Set集合的类继承结构图
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566783057076-1a6af039-faeb-454a-ad97-76c60e1fbeb0.png#height=551&id=BJqIO&originHeight=1516&originWidth=2298&originalType=binary&ratio=1&rotation=0&showTitle=false&size=177892&status=done&style=shadow&title=&width=835.6363636363636)
<a name="ZinWP"></a>
#### 2、Set集合的实现
Set 集合的实现可以分为两大块，一块是 Set 集合的有序实现（红色部分），另一块是 Set 集合的哈希实现（蓝色部分）<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566786895422-8c2cbd85-cf5e-4295-b92a-3c23ac82b066.png#height=492&id=aiuJc&originHeight=1353&originWidth=2145&originalType=binary&ratio=1&rotation=0&showTitle=false&size=374877&status=done&style=shadow&title=&width=780)
<a name="vXCiH"></a>
#### 3、有序实现(TreeSet)
SortedSet 接口继承了 Set 接口，TreeSet 实现了 SortedSet。<br />Set 集合中的元素是无序的，而 SortedSet 接口则是定义了有序 Set 集合的接口。而 TreeSet 则是 SortedSet 的具体实现。
<a name="lBS3X"></a>
#### 4、哈希实现(HashSet、LinkedHashSet)
HashSet 是 Set 接口的经典哈希实现。但 Set 集合中的元素是无序的，为了维护 Set 集合的插入顺序，出现了 LinkedHashSet。LinkedHashSet 是在 HashSet 的基础上用链表维护元素的插入顺序。<br />Set 集合的所有实现：

- TreeSet：Set 集合的有序实现。
- HashSet：Set 集合的哈希实现。
- LinkedHashSet：Set 集合的哈希实现，维护了元素插入顺序。
<a name="nVPNR"></a>
### C.Queue集合
队列是一个特殊的线性表，其数据结构特点是先进先出。
<a name="zd1rd"></a>
#### 1、Queue类结构体系如下
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566787765284-14185e72-d567-4ce5-b871-dd221e1b8bde.png#height=570&id=zDdmD&originHeight=1568&originWidth=2729&originalType=binary&ratio=1&rotation=0&showTitle=false&size=207402&status=done&style=shadow&title=&width=992.3636363636364)
<a name="M3zvo"></a>
#### 2、Queue的具体实现
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566788049574-8d162445-b85e-4321-9808-c6a0c844c970.png#height=571&id=qF7af&originHeight=1571&originWidth=2766&originalType=binary&ratio=1&rotation=0&showTitle=false&size=229256&status=done&style=shadow&title=&width=1005.8181818181819)<br />Queue 接口继承了 Collection 接口。Queue 接口在拥有基本集合操作的基础上，定义了队列这种数据结构的基本操作。offer、poll 等方法都是队列独有的操作。<br />AbstractQueue 是对 Queue 接口的抽象实现。针对队列这种数据结构，其添加、删除元素的动作都不一样。在 AbstractQueue 抽象类里将队列的基本操作都实现了一遍。例如 AbstractQueue 中的 add 方法就和 AbstractList 中的 add 方法有着不同的实现。
<a name="VJKPV"></a>
#### 3、有序实现
PriorityQueue 是 AbstractQueue 抽象类的具体实现。<br />PriorityQueue 表示优先级队列，其按照队列元素的大小进行重新排序。当调用 peek() 或 pool() 方法取出队列中头部的元素时，并不是取出最先进入队列的元素，而是取出队列的最小元素。
<a name="rPt4r"></a>
#### 4、双向实现

1. Deque（double ended queue）是双向队列的意思，它能在头部或尾部进行元素操作。
2. LinkedList 是一个链表，但它还是一个双向队列。因此 LinkedList 具有 List 和 Queue 的双重特性。ArrayDeque 是一个双向循环队列，其底层是用数组实现。
<a name="w8hcc"></a>
#### 5、Queue 体系常见实现类：

- PriorityQueue：优先级队列
- LinkedList：双向队列实现
- ArrayDeque：双向循环队列实现
<a name="mhNVA"></a>
### D.Map集合
Map 集合与 List、Set、Queue 有较大不同，其实类似于 key/value 的数据结构。<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566782048921-396fee4f-7b9e-4747-a371-0adc2fb42dbd.png#height=499&id=lsLoa&originHeight=1372&originWidth=3033&originalType=binary&ratio=1&rotation=0&showTitle=false&size=186543&status=done&style=shadow&title=&width=1102.909090909091)

![Map.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566747423667-5c0588e5-ff09-4879-9cf5-131d67bbd367.png#height=11517&id=wrOYr&originHeight=11517&originWidth=20325&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4204582&status=done&style=shadow&title=&width=20325)<br />[Map.png](https://www.yuque.com/attachments/yuque/0/2019/png/396745/1566747442702-3e6419da-97f1-43c2-9ee8-04879dac1078.png?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fpng%2F396745%2F1566747442702-3e6419da-97f1-43c2-9ee8-04879dac1078.png%22%2C%22name%22%3A%22Map.png%22%2C%22size%22%3A4204582%2C%22type%22%3A%22image%2Fpng%22%2C%22ext%22%3A%22png%22%2C%22status%22%3A%22done%22%2C%22uid%22%3A%22rc-upload-1566746569678-27%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22id%22%3A%22Xvzgh%22%2C%22card%22%3A%22file%22%7D)
<a name="75s4j"></a>
#### 1、Map集合的实现

- 首先，Map 接口是最顶层的接口。

与 List、Set、Queue 类似，Map 接口定义的是哈希表数据结构的操作。例如常用的 put、get、keySet 等。

- 接着，有 AbstractMap 抽象类。

和 List 等类似，AbstractMap 是 Map 接口的抽象实现。如下图所示，Map 集合的整个类结构可以分为红、黄、棕三块。<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566789312574-d2b95b63-1751-43ff-89e3-140738a8ab06.png#height=465&id=aqCOy&originHeight=1279&originWidth=2745&originalType=binary&ratio=1&rotation=0&showTitle=false&size=181701&status=done&style=shadow&title=&width=998.1818181818181)
<a name="TBW3p"></a>
#### 2、哈希实现

- AbstractMap 有具体的实现类 HashMap。

HashMap 是 AbstractMap 基于哈希算法的具体实现。

- LinkedHashMap 和 WeakedHashMap 继承了 HashMap。

LinkedHashMap 是 HashMap 的进一步实现，其用链表保存了插入 HashMap 中的元素顺序。WeakedHashMap 是 HashMap 的进一步实现，与 HashMap不同的是：WeakedHashMap 中的引用是弱引用，如果太久没用，则会被自动回收。
<a name="9u2BZ"></a>
#### 3、有序实现

- SortedMap 接口继承了 Map 接口。

与 Set 一样，Map 中的元素是没有顺序的，SortedMap 就是有序 Map 的接口定义。

- NavigableMap 继承了 SortedMap 接口。

NavigableMap 接口定义了一些查找逻辑，方便后续实现。

- TreeMap 则是 NavigableMap 接口的具体实现。

其实 TreeMap 是基于红黑树的 Map 实现。
<a name="QVIx4"></a>
#### 4、Map集合的所有实现类

- HashMap：Map 集合的经典哈希实现。
- LinkedHashMap：在 HashMap的基础上，增加了对插入元素的链表维护。
- WeakedHashMap：在 HashMap的基础上，使强引用变为弱引用。
- TreeMap：Map 集合的有序实现。
<a name="1W51V"></a>
### E.工具类
工具类(Iterator 迭代器、Enumeration 枚举类、Arrays 和 Collections)
<a name="boNaF"></a>
#### 1.Iterator迭代器
Iterator 迭代器是一个用来遍历并选择序列中的对象。Java 的 Iterator 只能单向移动。可以看到在 ArrayList、WeakHashMap 等集合类都实现了该接口，从而实现不同类型集合的遍历。
<a name="SPT63"></a>
#### 2.ListIterator迭代器
ListIterator 继承了 Iterator 接口，所以其有更强大的功能，即它能够实现双向移动。但从其名字也可以看出，其只能适用于 List 集合的遍历。
<a name="Ze7Xi"></a>
#### 3.Enumeration枚举类
Enumeration 枚举类是 JDK 1.0引入的接口。作用和Iterator一样，也是遍历集合。但是Enumeration的功能要比Iterator少。Enumeration只能在Hashtable, Vector, Stack中使用。这种传统接口已被迭代器取代，虽然 Enumeration 还未被遗弃，但在代码中已经被很少使用了。
<a name="8I0kq"></a>
#### 4.Arrays
Java.util.Arrays类能方便地操作数组，它提供的所有方法都是静态的。
<a name="4jca5"></a>
#### 5.Collections
Collections 是一个包含各种有关集合操作的静态多态方法的工具类。
<a name="2zJuu"></a>
## 2、同步容器
<a name="CgLVy"></a>
### Java中同步容器主要包括2类

- 1、Vector、Stack、HashTable
- 2、Collections类中提供的静态工厂方法创建的类
<a name="fYoLI"></a>
### 同步容器存在的问题
同步容器直接保证单个线程的操作安全性，但是无法保证复合操作的线程安全，遇到这种情况时，必须要通过主动加锁的方式来实现。而且，除此之外，同步容易由于对其所有方法都加了锁，这就导致多个线程访问同一个容器的时候，只能进行顺序访问，即使是不同的操作，也要排队，如get和add要排队执行。这就大大的降低了容器的并发能力。
<a name="IqOIs"></a>
## 3、并发容器
java.util.concurent包下，提供了大量支持高效并发的访问的集合类，称之为并发容器<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566784571418-1fd09ad5-61a2-4f00-8af2-567a18cd9439.png#height=193&id=nnu8k&originHeight=530&originWidth=513&originalType=binary&ratio=1&rotation=0&showTitle=false&size=58091&status=done&style=none&title=&width=186.54545454545453)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566784935334-fc03b283-4a6b-4319-84c1-f50b191e13e1.png#height=357&id=FLNK5&originHeight=981&originWidth=3594&originalType=binary&ratio=1&rotation=0&showTitle=false&size=152845&status=done&style=shadow&title=&width=1306.909090909091)<br />同步容器的复合操作的问题，一般在Map中发生的比较多，所以在`ConcurrentHashMap`中增加了对常用复合操作的支持，比如`putIfAbsent()`、`replace()`，这2个操作都是原子操作，可以保证线程安全。<br />另外，并发包中的`CopyOnWriteArrayList`和`CopyOnWriteArraySet`是Copy-On-Write的两种实现。<br />Copy-On-Write容器即写时复制的容器。通俗的理解是当往一个容器添加元素的时候，不直接往当前容器添加，而是先将当前容器进行Copy，复制出一个新的容器，然后新的容器里添加元素，添加完元素之后，再将原容器的引用指向新的容器。<br />`CopyOnWriteArrayList`中`add`/`remove`等写方法是需要加锁的，而读方法是没有加锁的。<br />这样做的好处是可以对`CopyOnWrite`容器进行并发的读，当然，这里读到的数据可能不是最新的。因为写时复制的思想是通过延时更新的策略来实现数据的最终一致性的，并非强一致性。<br />但是，作为代替`Vector`的`CopyOnWriteArrayList`并没有解决同步容器的复合操作的线程安全性问题。
