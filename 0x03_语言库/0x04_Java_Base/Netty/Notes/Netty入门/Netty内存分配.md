JavaNetty
<a name="cwfO9"></a>
## 背景
Netty 中的内存管理的实现并不是一蹴而就的，它也是参考了 Jemalloc 内存分配器。而 Jemalloc 又借鉴了 Tcmalloc（出身于 Google，通过红黑树来管理内存快和分页，带有线程缓存。对于小的对象来说，直接由线程的局部缓存来完成，大对象那就由自旋锁来减少多线程下的竞争）的设计思路，但是 Jemalloc 设计的更复杂，虽然也有线程缓存的特性，但是 Jemalloc 将内存分配的粒度划分为 Small、Large、Huge 三个分类，在空间的占用上比较多，但是在大内存分配的场景，内存碎片就略少 。<br />虽然有众多的内存分配器，但是它们的核心都是一致的：

- 高效大的内存分配和回收，提升单线程或者多线程场景下的性能；
- 减少内存碎片，包括内部碎片和外部碎片，提升内存的有效利用率。

这边有个内存碎片的概念，可以介绍下，Linux 中物理内存会被分成若干个 4k 大小的内存页 Page，物理内存的分配和回收都是基于 Page 完成的，内部碎片就是 Page 内部产生的碎片，外部碎片就是各个 Page 之间产生的碎片。<br />内部碎片：<br />即使只需要很小的内存，系统也至少会分配 4k 大小的 Page<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050537380-fc9dec32-6e2f-42e3-ae1d-1b041687265c.png#averageHue=%23f3e4e2&clientId=u927ab11c-b8c0-4&from=paste&id=ubb0802b9&originHeight=494&originWidth=956&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uacda28fb-2858-4488-92cb-a22c86e1f03&title=)<br />外部碎片：<br />当分配大内存的时候，此时一个 Page (4k) 显然不够，此时，系统会分配连续的 Page 才能够满足要求，但是，程序在不断的运行，这些 Page 会被频繁的回收，然后重新分配，难免这些 Page 之间会出现空闲的内存块，这就形成了外部碎片<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050537365-576c3deb-1111-4a1a-8390-97f50f4c2bbd.png#averageHue=%23eed6d4&clientId=u927ab11c-b8c0-4&from=paste&id=uf679ac6e&originHeight=1076&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1e6304b1-0ec0-4f7e-8a7d-4f31d7b8d34&title=)<br />对于内存分配的肯定有内存分配的一些算法，这里主要分析 Netty 的内存分配；
<a name="DWRha"></a>
## 基本概念
Netty 内存根据使用的内存位置（堆内 Heap 和堆外 Direct）和内存是否池化进行分类。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050537370-21906d51-1492-4e43-a002-0d5adbbb4bb5.png#averageHue=%233e4349&clientId=u927ab11c-b8c0-4&from=paste&id=u69190def&originHeight=920&originWidth=938&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u81512d87-97d5-4fc4-994a-c4006b46e05&title=)<br />对于每个线程而言，Netty 会为之分配一个内存 Cache；而在多个线程之间可共享一个 `Arena`。Arena 管理着相关内存，包含不同使用率的 `PoolChunkList`、`TinySubPagePools` 及 `SmallSubPagePools` 来更好地分配内存。<br />内存根据大小可分为 Huge、Normal、Small、Tiny。<br />Huge<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050537351-d61922ca-2a18-44db-bfbc-6a9632861f72.png#averageHue=%23f7f7f5&clientId=u927ab11c-b8c0-4&from=paste&id=ufac96bf4&originHeight=361&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0e1b6ea1-f973-4395-8760-81297f6d943&title=)<br />初次申请内存，都是按照 Chunk 来申请，但是为了更高效率的使用内存，在 Chunk 这个级别下，还定义了 Page 和 SubPage 的内存块。

- Chunk : 是 Netty 向操作系统申请内存的单位，所有的内存分配操作都是基于 Chunk 完成的，默认大小是16M。在分配大小超过 8K的内存，会从 `PoolChunkList` 中分配内存，或新增 Chunk。一个 Chunk 会被分成 2048 个 Page，是一个完全二叉树。一般每层节点有一个标识，标识当前节点及以下节点是否还有可用节点。
- Page：是 Chunk 用于管理内存的单位，Netty 中的 Page 的大小为 8k，假如需要分配 64K 的内存，需要在 Chunk 中选取4个 Page 进行分配。
- SubPage：负责 Page 内的内存分配，假如分配的内存大小远小于 Page（8K），直接分配一个 Page 会造成严重的内存浪费，所以需要将 Page 划分为多个相同的子块来进行分配，这里的子块就相当于 SubPage。SubPage 也分为两种不同的规格，在 Tiny 场景下，最小的划分为 16B，然后按 16B 依次递增；在 Small 场景下，就分为 4 种规格，512B、1024B、2048B、4096B。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050537385-10f816f2-3ebe-4ee0-b096-f2beb8799348.png#averageHue=%23f7f7f7&clientId=u927ab11c-b8c0-4&from=paste&id=u43bc757e&originHeight=819&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udb2c4cc2-e991-48cc-916b-9c363196575&title=)
<a name="ztrGs"></a>
### `PoolArena`
Netty 借鉴了 Jemalloc 中的 Arena 的设计思想，采用固定数量的多个 Arena 进行内存分配，Arena 的默认数量与 CPU 的核数有关，通过创建多个 Arena 来缓解资源竞争的问题，提高了内存分配的效率。线程在首次申请分配内存时，会轮询 Arena 数量，选择一个固定的Arena，在线程的生命周期内只与该 Arena 打交道，所以每个线程都保存了 Arena 的信息，从而提高访问的效率。<br />`PoolArena` 的数据结构包含了两个 `PoolSubPage` 数组，和六个 `PoolChunkList`，这两个 `PoolSubPage` 数组分别存放 Tiny 和 Small 类型的内存块，六个 `PoolChunkList` 分别存储不同利用率的 Chunk，构成一个双向链表。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050538033-7f585955-e5a4-435b-a0a4-475fda49378f.png#averageHue=%23f8f8f8&clientId=u927ab11c-b8c0-4&from=paste&id=u65e0b18b&originHeight=243&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u39f61eec-541e-42f2-884f-7d4c8a72ef7&title=)
```java
// 内存使用率为100%的Chunk  
q100 = new PoolChunkList<T>(this, null, 100, Integer.MAX_VALUE, chunkSize);

// 内存使用率为75～100%的Chunk
q075 = new PoolChunkList<T>(this, q100, 75, 100, chunkSize);

// 内存使用率为50～100%的Chunk
q050 = new PoolChunkList<T>(this, q075, 50, 100, chunkSize);

// 内存使用率为25～75%的Chunk
q025 = new PoolChunkList<T>(this, q050, 25, 75, chunkSize);

// 内存使用率为1～50%的Chunk
q000 = new PoolChunkList<T>(this, q025, 1, 50, chunkSize);

// 内存使用率为0～25%的Chunk
qInit = new PoolChunkList<T>(this, q000, Integer.MIN_VALUE, 25, chunkSize);

q100.prevList(q075);
q075.prevList(q050);
q050.prevList(q025);
q025.prevList(q000);
q000.prevList(null);
qInit.prevList(qInit);
```
六种类型的 `PoolChunkList` 除了 qInit，它们都形成了双向链表.<br />qInit 用于存储初始化分配的 `PoolChunk`，在第一次内存分配时，`PoolChunkList` 中并没有可用的 `PoolChunk`，所以需要新创建一个 `PoolChunk` 并添加到 qInit 列表中。qInit 中的 `PoolChunk` 即使内存被完全释放也不会被回收，避免了 `PoolChunk` 的重复初始化工作。<br />内存池的初始阶段线程是没有内存缓存的，所以最开始的内存分配都需要在全局分配区进行分配<br />无论是 `TinySubpagePools` 还是 `SmallSubpagePools` 成员在内存池初始化时是不会预置内存的，所以最开始内存分配阶段都会进入 `PoolArena` 的 `allocateNormal` 方法
```java
private void allocateNormal(PooledByteBuf<T> buf, int reqCapacity, int normCapacity) {

    // 1.尝试从现有的 Chunk 进行分配
    if (q050.allocate(buf, reqCapacity, normCapacity) 
        || q025.allocate(buf, reqCapacity, normCapacity) 
        || q000.allocate(buf, reqCapacity, normCapacity) 
        || qInit.allocate(buf, reqCapacity, normCapacity) 
        || q075.allocate(buf, reqCapacity, normCapacity)) {
        return;
    }

    // Add a new chunk 2.尝试创建一个 Chuank 进行内存分配
    PoolChunk<T> c = newChunk(pageSize, maxOrder, pageShifts, chunkSize);
    boolean success = c.allocate(buf, reqCapacity, normCapacity);
    assert success; 

    // 4.将 PoolChunk 添加到 PoolChunkList 中
    qInit.add(c);
}


boolean allocate(PooledByteBuf<T> buf, int reqCapacity, int normCapacity) {

    final long handle;

    // >= pageSize 通过位运算是否大于 512k
    if ((normCapacity & subpageOverflowMask) != 0) { 
        handle =  allocateRun(normCapacity);
    } else {
        handle = allocateSubpage(normCapacity);
    }

    if (handle < 0) {
        return false;
    }

    ByteBuffer nioBuffer = cachedNioBuffers != null ? cachedNioBuffers.pollLast() : null;

    // 3.初始化 PooledByteBuf
    initBuf(buf, nioBuffer, handle, reqCapacity);

    return true;
}
```
分配内存时为什么选择从 q050 开始<br />1、qinit 的 Chunk 利用率低，但不会被回收。 2、q075 和 q100 由于内存利用率太高，导致内存分配的成功率大大降低，因此放到最后。 3、q050 保存的是内存利用率 50%~100% 的 Chunk，这应该是个折中的选择。这样能保证 Chunk 的利用率都会保持在一个较高水平提高整个应用的内存利用率，并且内存利用率在 50%~100% 的 Chunk 内存分配的成功率有保障。 4、当应用在实际运行过程中碰到访问高峰，这时需要分配的内存是平时的好几倍需要创建好几倍的 Chunk，如果先从 q000 开始，这些在高峰期创建的 Chunk 被回收的概率会大大降低，延缓了内存的回收进度，造成内存使用的浪费。
<a name="JHdFF"></a>
### `PoolChunkList`
`PoolChunkList` 负责管理多个 `PoolChunk` 的生命周期，同一个 `PoolChunkList` 中存放了内存相近的 `PoolChunk`，通过双向链表的形式链接在一起，因为 `PoolChunk` 经常要从 `PoolChunkList` 中删除，而且需要在不同的 `PoolChunkList` 中移动，所以双向链表是管理 `PoolChunk` 时间复杂度较低的数据结构。
```java
final class PoolChunkList<T> implements PoolChunkListMetric {
    private static final Iterator<PoolChunkMetric> EMPTY_METRICS = Collections.<PoolChunkMetric>emptyList().iterator();
    private final PoolArena<T> arena;

    // 下一个PoolChunkList（使用率更高的）
    private final PoolChunkList<T> nextList;

    // 最低使用率，低于该值，会移除该chunk，放到preList中
    private final int minUsage;

    // 最高使用率，高于该值，会移除该chunk，放到nextList中
    private final int maxUsage;

    // 最大可分配的内存大小，就是用minUsage计算的
    private final int maxCapacity;
    private PoolChunk<T> head;

    // This is only update once when create the linked 
    // like list of PoolChunkList in PoolArena constructor.
    // 前一个PoolChunkList（使用率更低的）
    private PoolChunkList<T> prevList;
```
每个 `PoolChunkList` 都有内存使用率的上下限：`minUsage` 和 `maxUsage`，当 `PoolChunk` 进行内存分配后，如果使用率超过 `maxUsage`，那么 `PoolChunk` 会从当前 `PoolChunkList` 中删除，并移动到下一个 `PoolChunkList`；同理，`PoolChunk` 中的内存发生释放后，使用率小于 `minUsage`，那么 `PoolChunk` 会从当前 `PoolChunkList` 中移除，移动到前一个 PoolChunk List。<br />再细看下上面的各个部分的内存使用率会有交叉重叠的部分，这样设计的原因是，因为 `PoolChunk` 需要在 `PoolChunkList` 中不断的移动，如果每个 `PoolChunkList` 的内存使用率的临界值都是恰好衔接的，例如 1%～50%，50%（51%）～70%，如果 PoolChunk 的使用率在 45%~55% 之间不停徘徊的话，那么就会导致 `PoolChunk` 在两个 `PoolChunkList` 不断移动，造成性能损耗。
<a name="vnueQ"></a>
### `PoolChunk`
Netty 内存的分配和回收都是基于 `PoolChunk` 完成的，`PoolChunk` 是真正存储内存数据的地方，每个 `PoolChunk` 的默认大小为 16M
```java
final class PoolChunk<T> implements PoolChunkMetric {

    final PoolArena<T> arena;

    // 存储的数据
    final T memory; 

    // 满二叉树中的节点是否被分配，数组大小为 4096
    private final byte[] memoryMap; 

    // 满二叉树中的节点高度，数组大小为 4096
    private final byte[] depthMap; 

    // PoolChunk 中管理的 2048 个 8K 内存块
    private final PoolSubpage<T>[] subpages; 

    // 剩余的内存大小
    private int freeBytes; 

    PoolChunkList<T> parent;

    PoolChunk<T> prev;

    PoolChunk<T> next;

    // 省略其他代码

}
```
`PoolChunk` 可以理解为 Page(8K) 的集合 ，Page 只是一种抽象的概念，实际在 Netty 中 Page 指的是 `PoolChunk` 所管理的子内存块，每个子内存块采用 `PoolSubpage` 表示<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050537972-ebd4a558-8eb0-4d87-a8ed-bc2575241dc1.png#averageHue=%23f9f9f7&clientId=u927ab11c-b8c0-4&from=paste&id=u298de24a&originHeight=431&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u468c69f1-2eb8-44e5-b074-5d244814346&title=)
```java
maxOrder = 11;
maxSubpageAllocs = 1 << maxOrder; 

// Generate the memory map.
memoryMap = new byte[maxSubpageAllocs << 1];
depthMap = new byte[memoryMap.length];
int memoryMapIndex = 1;

// move down the tree one level at a time
for (int d = 0; d <= maxOrder; ++ d) { 
    int depth = 1 << d;
    for (int p = 0; p < depth; ++ p) {
        // in each level traverse left to right and set value to the depth of subtree
        memoryMap[memoryMapIndex] = (byte) d;
        depthMap[memoryMapIndex] = (byte) d;
        memoryMapIndex ++;
    }
}
```
deptMap 用于存放节点所对应的高度。例如第 2048 个节点 `depthMap[1025] = 10`<br />`memoryMap` 用于记录二叉树节点分配的信息，初始值和 deptMap 是一样的，随着节点被分配，不仅节点的值会改变，而且会递归遍历更新其父节点的值，父节点的值取两个子节点中的最小值。<br />subpages 对应上图中 `PoolChunk` 内部的 Page0，Page1 等。Netty 中没有 Page 的定义，直接使用 `PoolSubPage` 表示。当分配的内存小于 8k 是，`PoolChunk` 中的每个 Page 节点会被划分成为更小的粒度的内存进行管理，小内存块同样以 `PoolSubPage` 管理。
```java
private long allocateSubpage(int normCapacity) {
    // Obtain the head of the PoolSubPage pool that is owned by the PoolArena and synchronize on it.
    // This is need as we may add it back and so alter the linked-list structure.
    PoolSubpage<T> head = arena.findSubpagePoolHead(normCapacity);
    // subpages are only be allocated from pages i.e., leaves
    int d = maxOrder; 
    synchronized (head) {
        int id = allocateNode(d);
        if (id < 0) {
            return id;
        }
    
        final PoolSubpage<T>[] subpages = this.subpages;
        final int pageSize = this.pageSize;
    
        freeBytes -= pageSize;
    
        int subpageIdx = subpageIdx(id);
        PoolSubpage<T> subpage = subpages[subpageIdx];
        if (subpage == null) {
            subpage = new PoolSubpage<T>(head, this, id, runOffset(id), pageSize, normCapacity);
            subpages[subpageIdx] = subpage;
        } else {
            subpage.init(head, normCapacity);
        }
        return subpage.allocate();
    }
}

PoolSubpage<T> findSubpagePoolHead(int elemSize) {
    int tableIdx;
    PoolSubpage<T>[] table;
    // < 512
    if (isTiny(elemSize)) { 
        tableIdx = elemSize >>> 4;
        table = tinySubpagePools;
    } else {
        tableIdx = 0;
        elemSize >>>= 10;
        while (elemSize != 0) {
            elemSize >>>= 1;
            tableIdx ++;
        }
        table = smallSubpagePools;
    }
    
    return table[tableIdx];
}
```
根据代码可以看出，小内存分配的场景下，会首先找到对应的 `PoolArena`，然后根据计算出对应的<br />`TinySubpagePools` 或者 `SmallSubpagePools` 数组对应的下标，如果对应数组元素所包含的 `PoolSubpage` 链表不存在任何节点，那么将创建新的 `PoolSubpage` 加入链表中
<a name="sx2PX"></a>
### `PoolSubpage`
```java
final class PoolSubpage<T> implements PoolSubpageMetric {

    final PoolChunk<T> chunk;

    // 对应满二叉树节点的下标
    private final int memoryMapIdx; 

    // PoolSubpage 在 PoolChunk 中 memory 的偏移量
    private final int runOffset; 

    // 记录每个小内存块的状态
    private final long[] bitmap; 

    // 与 PoolArena 中 tinySubpagePools 或 smallSubpagePools 中元素连接成双向链表
    PoolSubpage<T> prev;

    PoolSubpage<T> next;

    // 每个小内存块的大小
    int elemSize; 

    // 最多可以存放多少小内存块：8K/elemSize
    private int maxNumElems; 

    // 可用于分配的内存块个数
    private int numAvail; 

    // 省略其他代码

}
```
`PoolSubpage` 是通过位图 bitmap 来记录子内存是否已经被使用，bit 的取值为 0 或 1<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050538109-c856e43b-3c39-48c7-a922-b475ad74fe0c.png#averageHue=%23f7f7f7&clientId=u927ab11c-b8c0-4&from=paste&id=u898ab24a&originHeight=958&originWidth=980&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3b11e274-84ab-495c-b05e-ba61cfc53e2&title=)<br />那 `PoolSubPage` 和 `PoolArea` 怎样联系起来的呢：<br />`PoolArea` 在创建是会初始化 `TinySubpagePools` 和 `SmallSubpagePools` 两个 `PoolSubpage` 数组，数组的大小分别是 32 和 4 加入分配 20B 大小的内存，会向上取整到 32B，从满二叉树的第 11 层找到一个 `PoolSubpage` 节点，并把它分为 8KB/32B = 256 个小内存块，然后找到这个 `PoolSubpage` 节点对应的 `PoolArena`，然后将 `PoolSubPage` 节点与 `tinySubpagePools[1]` 对应的 head 节点链接成双向链表。<br />如果后续再有 32B 规格的内存分配时，直接从 `PoolArena` 中 `tinySubpagePools[1]` 元素的next节点是否存在可用的 `PoolSubpage`，如果存在直接使用该 `PoolSubpage` 执行内存分配，提高内存分配的使用效率。<br />当内存释放时，Netty 并没有将缓存归还到 `PoolChunk` 中，而是使用 `PoolThreadCache` (本地线程缓存)，当下次有同样规格的内存分配时，如果缓存有，直接从缓存里面取出当前符合规格的内存。
<a name="Vyk8Q"></a>
## 内存的分配策略
<a name="RuEhQ"></a>
### 分配内存大于 8k，`PoolChunk` 中采用的 Page 级别的内存分配策略
假设依次申请了 8k、16k、8k 的内存

1. 首先根据分配内存大小计算二叉树所在节点的高度，然后查找对应高度中是否存在可用节点，如果分配成功则减去已经分配的内存大小得到剩余的可用空间，核心代码如下：
```java
private long allocateRun(int normCapacity) {
    
    // 根据分配内存大小计算树对应的节点高度 maxOrder 为二叉树的最大高度 11. , pageShifts 默认为13
    int d = maxOrder - (log2(normCapacity) - pageShifts);
    
    // 查找对应高度中是否存在可用节点
    int id = allocateNode(d);
    
    if (id < 0) {
        return id;
    }
    
    // 减去以分配的内存大小
    freeBytes -= runLength(id);
    return id;
}
```
第一次在分配 8k 大小的内存时，计算得到二叉树所在节点高度为 11，8k= 2^13。然后从第 11 层查找可用的 Page，下标为 2048 的节点可以被用于分配内存，即 page[0] 被分配使用，此时赋值 memoryMap[2048] =12，表示该节点已经不可用，然后递归更新父节点的值，父节点的值取两个子节点的最小值，即 memoryMap[1024]=11,memory[512]=10。<br />第二次分配 16k 内存时，计算得到的节点高度是 10，此时 1024 节点已经分配了一个 8K 的内存，不满足条件，继续寻找 1025 节点，此节点并未使用过，满足分配的条件，就将 1025 的两个子节点分配出去，赋值，memoryMap[2050]=12,memoryMap[2051] = 12，然后在递归更新父节点的值。<br />第三次分配 8k 大小的内存时，依然从第 11 层开始查找，发现 2048 已经使用，2049 可以分配，赋值 memoryMap[2049] =12，然后递归更新父节点。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050538112-07a813d3-66a8-431e-bbb4-5dd1620021b8.png#averageHue=%23faf9f9&clientId=u927ab11c-b8c0-4&from=paste&id=ua4b7339a&originHeight=321&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9abda922-7a80-4bf4-ac7f-08b5b7f8dfd&title=)
<a name="nLFx9"></a>
### 分配内存小于 8k，由 `PoolSubpage` 负责管理的内存分配策略
`PoolChunk` 不在分配单独的 Page，而是将 Page 划分为更小的内存块，由 `PoolSubpage` 进行管理
```java
private long allocateSubpage(int normCapacity) {
    // Obtain the head of the PoolSubPage pool 
    // that is owned by the PoolArena and synchronize on it.
    // This is need as we may add it back and so alter the linked-list structure.
    
    // 根据内存大小找到PoolArena中Subpage数组对应的头节点
    PoolSubpage<T> head = arena.findSubpagePoolHead(normCapacity);
    // 从最底层开始查找
    // subpages are only be allocated from pages i.e., leaves
    int d = maxOrder; 
    synchronized (head) {
        //找到一个可用的节点
        int id = allocateNode(d);
        if (id < 0) {
            return id;
        }
        //把转化为Subpage的Page给记录下来
        final PoolSubpage<T>[] subpages = this.subpages;
        final int pageSize = this.pageSize;
    
        freeBytes -= pageSize;
        //pageId 到subpageId的转化，pageId=2048 subpageId = 0
        int subpageIdx = subpageIdx(id);
        PoolSubpage<T> subpage = subpages[subpageIdx];
        if (subpage == null) {
            //创建PoolSubPage，并切分为相同大小的子内存块，然后加入PoolArena对应的双向链表中
            subpage = new PoolSubpage<T>(head, this, id, runOffset(id), pageSize, normCapacity);
            subpages[subpageIdx] = subpage;
        } else {
            subpage.init(head, normCapacity);
        }
        //执行内存分配并返回内存地址
        return subpage.allocate();
    }
}
```
如果分配 20B 大小的内存，20B 属于 Tiny 场景，按照内存规格的分类，20B 需要向上取整到 32B。在满二叉树中寻找可用的节点用于内存分配，假如 2049 节点时可用的，那么返回的 ID=2049，然后将 pageId 转换成了 subpageIdx， 2049 对应 1 ，如果 `PoolChunk` 中 `subpages` 数组的 `subpageIdx` 下标对应的 `PoolSubpage` 不存在，那么就新创建一个 `PoolSubpage`，并将 `PoolSubpage` 切分为相同大小的子内存块，这边对应的子内存块是32B，然后找到 `PoolArena` 中 `tinySubpagePools` 数组对应的头节点，32B 对应的`tinySubpagePools[1]` 的 head 节点连接成双向链表，最后执行内存分配返回内存地址。<br />`PoolSubpage` 通过位图 bitmap 记录每个内存块是否已经被使用。在上述的示例中，8K/32B = 256，因为每个 long 有 64 位，所以需要 256/64 = 4 个 long 类型的即可描述全部的内存块分配状态，因此 bitmap 数组的长度为 4，从 bitmap[0] 开始记录，每分配一个内存块，就会移动到 bitmap[0] 中的下一个二进制位，直至 bitmap[0] 的所有二进制位都赋值为 1，然后继续分配 bitmap[1]。
<a name="MRfo4"></a>
### 分配内存小于 8k，为了提高内存分配效率，由 `PoolThreadCache` 本地线程缓存提供的内存分配
假如现在需要分配 32B 大小的堆外内存，会从 `MemoryRegionCache` 数组 `tinySubPageDirectCaches[1]` 中取出对应的 `MemoryRegionCache` 节点，尝试从 `MemoryRegionCache` 的队列中取出可用的内存块。
<a name="cJ4d6"></a>
## 内存回收
```java
// 默认执行 8192 次 allocate()，就会调用 trim() 进行内存整理
boolean allocated = cache.allocate(buf, reqCapacity);
if (++ allocations >= freeSweepAllocationThreshold) {
    allocations = 0;
    trim();
}

void trim() {
    trim(tinySubPageDirectCaches);
    trim(smallSubPageDirectCaches);
    trim(normalDirectCaches);
    trim(tinySubPageHeapCaches);
    trim(smallSubPageHeapCaches);
    trim(normalHeapCaches);
}

private static void trim(MemoryRegionCache<?>[] caches) {
    if (caches == null) {
        return;
    }
    for (MemoryRegionCache<?> c: caches) {
        trim(c);
    }
}

public final void trim() {
    /** 
   * 通过 size - allocations 衡量内存分配执行的频繁程度，
    * 其中 size 为该 MemoryRegionCache 对应的内存规格大小，size 为固定值，
    * 例如 Tiny 类型默认为 512。
    * allocations 表示 MemoryRegionCache 距离上一次内存整理已经发生了多少次 allocate 调用，
    * 当调用次数小于 size 时，
    * 表示 MemoryRegionCache 中缓存的内存块并不常用，从队列中取出内存块依次释放。
    */
    int free = size - allocations;
    allocations = 0;

    // We not even allocated all the number that are
    if (free > 0) {
        free(free, false);
    }
}

// 最终会执行native方法  这是一个native方法
public native void freeMemory(long var1);

// 详见源码。PoolThreadCache  # allocate
```
<a name="aaxB4"></a>
## 总结
主要是介绍了内存的分配的工作，以及其他的额外的特性；对内存管理有一定程度的认识；里面的内存释放，涉及到的操作细节非常多，例如内存合并操作；以及内存真正释放的时机；更多细节还是需要通过源码了解；这里列一下关键的 Netty 中的几个类： `ServerChannelRecvByteBufAllocator`：分配缓存大小的策略对象 `PooledByteBufAllocator`：字节缓存池分配器 `PoolThreadCache`：线程缓存对象 。<br />有兴趣的可以一起深入研究。
<a name="fLtGp"></a>
## 参考资料
[https://netty.io/](https://netty.io/)
