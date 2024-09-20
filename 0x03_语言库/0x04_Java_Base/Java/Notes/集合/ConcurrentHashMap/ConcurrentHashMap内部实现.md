Java ConcurrentHashMap内部实现<br />`ConcurrentHashMap`可以说是目前使用最多的并发数据结构之一，作为如此核心的基本组件，不仅仅要满足功能的需求，更要满足性能的需求。而实现一个高性能的线程安全的`HashMap`也绝非易事。<br />`ConcurrentHashMap`作为JDK8的内部实现，一个成功的典范，有着诸多可以学习和致敬的地方。
<a name="ARBHh"></a>
## `ConcurrentHashMap`的内部数据结构
在JDK8中， `ConcurrentHashMap`的内部实现发生了天翻地覆的变化。这里依据JDK8，来介绍一下`ConcurrentHashMap`的内部实现。<br />从静态数据结构上说，`ConcurrentHashMap`包含以下内容：
<a name="mwbwj"></a>
### `int sizeCtl`
这是一个多功能的字段，可以用来记录参与Map扩展的线程数量，也用来记录新的table的扩容阈值
<a name="FMkv6"></a>
#### `CounterCell[] counterCells`
用来记录元素的个数，这是一个数组，使用数组来记录，是因为避免多线程竞争时，可能产生的冲突。使用了数组，那么多个线程同时修改数量时，极有可能实际操作数组中不同的单元，从而减少竞争。
<a name="FcpK8"></a>
#### `Node<K,V>[] table`
实际存放`Map`内容的地方，一个`map`实际上就是一个`Node`数组，每个`Node`里包含了`key`和`value`的信息。
<a name="Y1bvo"></a>
### `Node<K,V>[] nextTable`
当`table`需要扩充时，会把新的数据填充到`nextTable`中，也就是说`nextTable`是扩充后的Map。<br />以上就是`ConcurrentHashMap`的核心元素，其中最值得注意的便是`Node`，`Node`并非想象中如此简单，下面的图展示了`Node`的类族结构：<br />![2021-05-01-16-44-34-420684.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619858861352-aab1a256-4704-4060-b337-9aa624e5a82b.jpeg#clientId=ue95488f0-72f8-4&from=ui&id=ud2028df9&originHeight=385&originWidth=521&originalType=binary&size=34991&status=done&style=shadow&taskId=uc075a14f-07fc-4523-a670-3550cc5e9a5)<br />可以看到，在Map中的`Node`并非简单的Node对象，实际上，它有可能是`Node`对象，也有可能是一个`Treebin`或者`ForwardingNode`。<br />那什么时候是`Node`，什么时候是`TreeBin`，什么时候又是一个`ForwardingNode`呢？<br />其实在绝大部分场景中，使用的依然是`Node`，从`Node`数据结构中，不难看出，`Node`其实是一个链表，也就是说，一个正常的`Map`可能是长这样的：<br />![2021-05-01-16-44-34-619147.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619858872597-e0d38d9d-d262-49b4-b79a-5ea9757164a5.jpeg#clientId=ue95488f0-72f8-4&from=ui&id=u0591d217&originHeight=424&originWidth=815&originalType=binary&size=29606&status=done&style=shadow&taskId=u6a8cfc2a-9261-49c9-a712-2cd2be7b1bf)<br />上图中，绿色部分表示`Node`数组，里面的元素是`Node`，也就是链表的头部，当两个元素在数据中的位置发生冲突时，就将它们通过链表的形式，放在一个槽位中。<br />当数组槽位对应的是一个链表时，在一个链表中查找`key`只能使用简单的遍历，这在数据不多时，还是可以接受的，当冲突数据比较多少，这种简单的遍历就有点慢了。<br />因此，在具体实现中，当链表的长度大于等于8时，会将链表树状化，也就是变成一颗红黑树。如下图所示，其中一个槽位就变成了一颗树，这就是`TreeBin`(在`TreeBin`中使用`TreeNode`构造整科树)。<br />![2021-05-01-16-44-34-825140.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619858892987-99990c33-d677-40bc-81de-fb93edc52422.jpeg#clientId=ue95488f0-72f8-4&from=ui&id=udfcb591f&originHeight=473&originWidth=816&originalType=binary&size=45992&status=done&style=shadow&taskId=u667de09f-f2cc-43d9-b5df-8336333038c)<br />当数组容量快满时，即超过75%的容量时，数组还需要进行扩容，在扩容过程中，如果老的数组已经完成了复制，那么就会将老数组中的元素使用`ForwardingNode`对象替代，表示当前槽位的数据已经处理了，不需要再处理了，这样，当有多个线程同时参与扩容时，就不会冲突。
<a name="kYyQP"></a>
#### `put()`方法的实现
现在来看一下作为一个HashMap最为重要的方法`put()`：<br />`public V put(K key, V value)`<br />它负责将给定的key和value对存入`HashMap`，它的工作主要有以下几个步骤：

1. 如果没有初始化数组，则尝试初始化数组
2. 如果当前正在扩容，则参与帮助扩容(调用`helpTransfer()`方法)
3. 将给定的`key,value` 放入对应的槽位
4. 统计元素总数
5. 触发扩容操作

根据以上主要4个步骤，来依次详细说明一下：
<a name="Q9UtM"></a>
##### 如果没有初始化数组，则尝试初始化数组
初始化数据会生成一个`Node`数组:
```java
Node<K,V>[] nt = (Node<K,V>[])new Node<?,?>[n];
```
默认情况下，n为16。同时设置`sizeCtl`为·`n - (n >>> 2)`; 这意味着`sizeCtl`为n的75%，表示`Map`的`size`，也就是说`ConcurrentHashMap`的负载因子是0.75。（为了避免冲突，Map的容量是数组的75%，超过这个阈值，就会扩容）
<a name="G8Cel"></a>
##### 如果当前正在扩容，则参与帮助扩容
```java
else if ((fh = f.hash) == MOVED)
    tab = helpTransfer(tab, f);
```
如果一个节点的`hash`是`MOVE`，则表示这是一个`ForwardingNode`，也就是当前正在扩容中，为了尽快完成扩容，当前线程就会参与到扩容的工作中，而不是等待扩容操作完成，如此紧密细致的操作，恰恰是ConcurrentHashMap高性能的原因。<br />而代码中的`f.hash==MOVE`语义上等同于`f instanceof ForwardingNode`，但是使用整数相等的判断的效率要远远高于`instanceof`，所以，这里也是一处对性能的极限优化。
<a name="umxKM"></a>
##### 将给定的`key,value` 放入对应的槽位
在大部分情况下，应该会走到这一步，也就是将key和value放入数组中。在这个操作中会使用大概如下操作：
```java
Node<K,V> f;
synchronized (f) {
     if(所在槽位是一个链表)
         插入链表
     else if(所在槽位是红黑树)
         插入树
     if(链表长度大于8[TREEIFY_THRESHOLD])
         将链表树状化
}
```
可以看到，这使用了`synchronized`关键字，锁住了`Node`对象。由于在绝大部分情况下，不同线程大概率会操作不同的`Node`，因此这里的竞争应该不会太大。<br />并且随着数组规模越来越大，竞争的概率会越来越小，因此`ConcurrentHashMap`有了极好的并行性。
<a name="AjMki"></a>
### 统计元素总数
为了有一个高性能的`size()`方法，`ConcurrentHashMap`使用了单独的方法来统计元素总数，元素数量统计在`CounterCell`数组中：
```java
CounterCell[] counterCells;
@sun.misc.Contended static final class CounterCell {
    volatile long value;
    CounterCell(long x) { value = x; }
}
```
`CounterCell`使用伪共享优化，具有很高的读写性能。`counterCells`中所有的成员的`value`相加，就是整个`Map`的大小。这里使用数组，也是为了防止冲突。<br />如果简单使用一个变量，那么多线程累加一个计数器时，难免要有竞争，现在分散到一个数组中，这种竞争就小了很多，对并发就更加友好了。<br />累加的主要逻辑如下：
```java
if (as == null || (m = as.length - 1) < 0 ||
    //不同线程映射到不同的数组元素，防止冲突
    (a = as[ThreadLocalRandom.getProbe() & m]) == null ||
    //使用CAS直接增加对应的数据
    !(uncontended =
      U.compareAndSwapLong(a, CELLVALUE, v = a.value, v + x)))
    //如果有竞争，在这里会重试，如果竞争严重还会将CounterCell[]数组扩容，以减少竞争
```
<a name="zBcua"></a>
### 触发扩容操作
最后，`ConcurrentHashMap`还会检查是否需要扩容，它会检查当前`Map`的大小是否超过了阈值，如果超过了，还会进行扩容。<br />`ConcurrentHashMap`的扩容过程非常巧妙，它并没有完全打乱当前已有的元素位置，而是在数组扩容2倍后，将一半的元素移动到新的空间中。<br />所有的元素根据高位是否为1分为`low`节点和`high`节点：
```java
//n是数组长度，数组长度是2的幂次方，因此一定是100 1000 10000 100000这种二进制数字
//这里将low节点串一起， high节点串一起
if ((ph & n) == 0)
    ln = new Node<K,V>(ph, pk, pv, ln);
else
    hn = new Node<K,V>(ph, pk, pv, hn);
```
接着，重新放置这些元素的位置：
```java
//low节点留在当前位置
setTabAt(nextTab, i, ln);
//high节点放到扩容后的新位置，新位置距离老位置n
setTabAt(nextTab, i + n, hn);
//扩容完成，用ForwardingNode填充
setTabAt(tab, i, fwd);
```
下图显示了 从8扩充到16时的可能得一种扩容情况，注意，新的位置总是在老位置的后面n个槽位（n为原数组大小）<br />![2021-05-01-16-44-35-045551.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1619858907305-3d39cb49-389c-4ee1-bfaa-b61fb92d1054.jpeg#clientId=ue95488f0-72f8-4&from=ui&id=uddafcb3c&originHeight=801&originWidth=818&originalType=binary&size=67937&status=done&style=shadow&taskId=u89f84b6a-ab88-47c9-ac52-cf899aff2c5)<br />这样做的好处是，每个元素的位置不需要重新计算，进行查找时，由于总是会对n-1(一定是一个类似于1111 11111 111111这样的二进制数)按位与，因此，high类的节点自然就会出现在+n的位置上。
<a name="iPfgG"></a>
#### `get()`方法的实现
与`put()`方法相比，`get()`方法就比较简单了。步骤如下：

1. 根据`hash`值 得到对应的槽位 `(n - 1) & h`
2. 如果当前槽位第一个元素key就和请求的一样，直接返回
3. 否则调用Node的`find()`方法查找
   1. 对于`ForwardingNode` 使用的是 `ForwardingNode.find()`
   2. 对于红黑树 使用的是`TreeBin.find()`
4. 对于链表型的槽位，依次顺序查找对应的`key`
<a name="ayP9d"></a>
## 总结
`ConcurrentHashMap`可以说是并发设计的典范，在JDK8中，`ConcurrentHashMap`可以说是再一次脱胎换骨，全新的架构和实现带来了飞一般的体验（JDK7中的`ConcurrentHashMap`还是采用比较骨板的`segment`实现的），细细品读，还是有不少的收获。<br />他和`HashMap`的区别，优劣势对比，这也是常考的考点，所以不管是为了了解、工作还是面试，都应该好好的熟悉一下。
