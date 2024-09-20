Linux
<a name="sT2S5"></a>
## Buffer 和 Cache 的定义：

- **Buffer**：是原始磁盘块的临时存储，即将缓存数据写入磁盘。它通常不会很大（大约  20MB）。这样，内核就可以将分散的写入集中起来，从而对磁盘写入进行统一优化。例如，多个小的写入可以合并为一个大的写入等。
- **Cache**：是用于从磁盘读取文件的页面缓存，用于缓存从文件中读取的数据。这样，下次访问这些文件数据时，可以直接从内存中快速取回，而无需再次访问缓慢的磁盘。

但是让我问你，由于 **Buffer** 只是将写入磁盘的数据的缓存。反过来，它还会缓存从磁盘读取的数据吗？或者 **Cache** 是从文件中读取数据的缓存，那么它是否也为写入文件缓存数据呢？
<a name="DDmZR"></a>
### free 命令
要检查系统内存使用情况，想到的第一个命令可能是 free ，例如：
```
$ free -h
              total        used        free      shared  buff/cache   available
Mem:           1.9G        1.0G        394M        2.6M        491M        728M
Swap:            0B          0B          0B
```
很明显，该输出包括了物理内存 Mem 和 Swap 的具体使用情况（如总内存、已用内存、缓存、可用内存等）。缓存是 **Buffer** 和 **Cache** 两部分的总和。<br />看一下 free 的手册页中的  **Buffer** 和 **Cache** 定义：
```
buffers
              Memory used by kernel buffers (Buffers in /proc/meminfo)
cache         Memory used by the page cache and slabs (Cached and SReclaimable in /proc/meminfo)
buff/cache
              Sum of buffers and cache
```
可以看到 free 命令的源数据实际上存储在 proc/meminfo 文件中。正如前面提到的，/proc 是 Linux 内核提供的一个特殊的文件系统，它就像一个用户与内核交互的接口。<br />/proc 文件系统也是许多性能工具的最终数据源。在 man proc 中，Buffers 和 Cached 的定义如下：
```
Buffers %lu
    Relatively temporary storage for raw disk blocks that shouldn't get tremendously large (20MB or so).
Cached %lu
   In-memory cache for files read from the disk (the page cache).  Doesn't include SwapCached.
...
SReclaimable %lu (since Linux 2.6.19)
    Part of Slab, that might be reclaimed, such as caches.
    
SUnreclaim %lu (since Linux 2.6.19)
    Part of Slab, that cannot be reclaimed on memory pressure.
```
至此，可能认为已经找到了问题的答案，“**Buffer**”只是用于将数据写入磁盘的缓存，“**Cache**”只是用于从文件中读取数据的缓存。但事实上，“**Buffer**”也可以用于读取，“**Cache**”也可以用于写入。
<a name="VmC6h"></a>
### 实验
在这里做两个实验，写缓存和读缓存。
<a name="uvynq"></a>
#### 写入 Cache
登录到 Linux 主机并准备好两个终端。在**终端 1**，先清理**缓存**：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652362730867-83bc509d-71f5-4e6c-8ff1-3d86636785ce.png#clientId=ua6465202-f66e-4&from=paste&id=u4ac94965&originHeight=172&originWidth=824&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf1dd1b32-0a41-41f0-a241-fec14c20cea&title=)<br />这里 /proc/sys/vm/drop_caches 是一个通过 proc 文件系统修改内核行为的例子。写入 3 意味着清理各种缓存，例如文件页、目录条目和 Inode。<br />仍然在**终端 1**，开启 vmstat 2 命令：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652362730904-799b45a1-218b-43cb-876e-c209c16f045e.png#clientId=ua6465202-f66e-4&from=paste&id=u8db0d71f&originHeight=137&originWidth=842&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u86b27dfa-4fba-4f2f-b297-3fd54641ccc&title=)

- buff 和 cache 就是前面看到的 **Buffer** 和 **Cache**，单位是 KB。
- bi 和 bo 分别表示块设备读取和写入的大小，以块/s 为单位。由于 Linux 中的块大小为 1KB，因此这个单位相当于 KB/s。

接下来，转移到**终端 2** 并运行以下命令：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652362730905-29fa5381-2c34-41e0-a97f-848198390133.png#clientId=ua6465202-f66e-4&from=paste&id=ub1e2ce4f&originHeight=80&originWidth=632&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uca6c9848-4e72-4bdd-8cab-8e66a7dbbf6&title=)<br />现在切换回**终端 1**，并观察 buff 和 cache 的变化：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652362730924-be0b0a71-e19c-499d-b7fa-2766fa72960e.png#clientId=ua6465202-f66e-4&from=paste&id=u787e305d&originHeight=239&originWidth=844&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6b92b83c-622d-4322-8395-994de2ad884&title=)<br />通过观察 vmstat 的输出，可以发现运行 dd 命令时，**Cache** 一直在增长，而 **Buffer** 基本没有变化。
<a name="Pw7Rz"></a>
#### 读取 Buffer
现在做第二个实验。再次清除**终端 1**中的**缓存**：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652362730907-affa6a4b-4244-49ce-ba4d-c94ebb91e915.png#clientId=ua6465202-f66e-4&from=paste&id=uecb293ad&originHeight=174&originWidth=818&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u04a9c982-594d-4440-a1c1-3fb857bf75f&title=)<br />同样在**终端 1** 中，再次启动 vmstat 2 命令：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652362731488-9da52de8-6c01-4ebd-8107-0f381d8afd2c.png#clientId=ua6465202-f66e-4&from=paste&id=ua5230cc5&originHeight=125&originWidth=856&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue1f49a11-43ee-4bb6-bed7-425d437e7f0&title=)<br />可以看到此时 buff 为 0。现在在**终端 2** 中，运行以下命令：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652362731537-b875ff68-600a-48f0-be10-25f2f498e991.png#clientId=ua6465202-f66e-4&from=paste&id=ub827ef0e&originHeight=81&originWidth=689&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u13b01440-510e-473a-848e-7e3fa4c1a07&title=)<br />然后，回到**终端 1** 观察：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652362731535-3edc16cb-e34f-4cc0-b116-c801682132df.png#clientId=ua6465202-f66e-4&from=paste&id=u5bc1c64a&originHeight=242&originWidth=847&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2595483c-958a-48bc-af6e-8e64559d308&title=)<br />观察 vmstat 的输出，会发现在读盘的时候（也就是 bi > 0 的时候），**Buffer** 和 **Cache** 都在增长，但是显然 **Buffer** 的增长要快很多。这意味着当从磁盘读取时，数据被缓存在 **Buffer** 中。<br />现在可以得出结论：<br />读取文件时数据会缓存在 **Cache** 中，读取磁盘时数据会缓存在 **Buffer** 中。
