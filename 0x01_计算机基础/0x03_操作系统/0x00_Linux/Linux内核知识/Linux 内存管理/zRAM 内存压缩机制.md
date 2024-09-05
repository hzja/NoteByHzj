内存是计算机系统最重要的资源之一，当操作系统内存不足时，进程申请内存将会失败，从而导致其运行异常或者崩溃。<br />Linux 内核提供 swap 机制来解决内存不足的情况，其原理是：<br />当系统内存不足时，内核会将进程不常用的内存交换（写入）到磁盘中，然后将这些内存归还给系统，系统可以将这些内存继续分配给其他需要使用内存的进程。<br />通过 swap 机制，系统可以将内存分配给需求更迫切的进程。但由于 swap 机制需要进行 I/O 操作，所以一定程度上会影响系统性能。那么是否存在一种能够节省内存，而且对性能影响较少的机制呢？<br />在 Linux-3.14 引入了一种名为 zRAM 的技术，zRAM 的原理是：将进程不常用的内存压缩存储，从而达到节省内存的使用。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696557917429-1ec78376-2a89-4c8d-8329-6bbdd8c59277.png#averageHue=%23f6f4ea&clientId=uad79faed-73e4-4&from=paste&id=u7d08dcee&originHeight=510&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4748669a-76ac-4451-af83-ccac2c3073e&title=)<br />zRAM 机制建立在 swap 机制之上，swap 机制是将进程不常用的内存交换到磁盘中，而 zRAM 机制是将进程不常用的内存压缩存储在内存某个区域。所以 zRAM 机制并不会发生 I/O 操作，从而避免因 I/O 操作导致的性能下降。
<a name="zEVMn"></a>
## zRAM原理
由于 zRAM 机制是建立在 swap 机制之上，而 swap 机制需要配置 文件系统 或 块设备 来完成的。所以 zRAM 虚拟一个块设备，当系统内存不足时，swap 机制将内存写入到这个虚拟的块设备中。也就是说，zRAM 机制本质上只是一个虚拟块设备。<br />zRAM 的原理如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696557917359-09a3052c-c9bc-4f3e-8d66-88fb037ef66d.png#averageHue=%23fcf9f5&clientId=uad79faed-73e4-4&from=paste&id=uce99b0c2&originHeight=241&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4abc1523-5c1a-42cd-914f-6ebab8f3d00&title=)<br />从上图可以看出，在开启了 zRAM 机制的情况下，当系统内存不足时，内核会进行如下操作：

- 通过 swap 机制从系统中查找一些进程不常用的内存。
- 将这些不常用的内存交换到 zRAM 块设备中，而 zRAM 块设备首先会对这些不常用的内存进行压缩，然后存储起来。
- 把不常用的内存压缩存储到 zRAM 块设备后，swap 机制会把这些不常用的内存归还给内核。
- 当进程访问到这些被交换到 zRAM 块设备的内存时，swap 机制将会通过 zRAM 块设备解压这些内存，并且重新建立与进程的地址映射关系。
<a name="WJzjo"></a>
## 启用zRAM
<a name="HjsQ1"></a>
### 1、创建 zRAM 块设备
要启用 zRAM，首先需要创建 zRAM 块设备。要创建 zRAM 块设备，可以使用以下命令：
```bash
modprobe zram num_devices=1
```
num_devices 参数可以指定创建 zRAM 块设备的个数，上面命令创建了一个 zRAM 块设备，可以通过路径 /dev/zram0 来访问这个块设备。
<a name="fIY2W"></a>
### 2、设置 zRAM 块设备的大小
创建完 zRAM 块设备后，可以通过以下命令来设置其空间大小：
```bash
echo 512M > /sys/block/zram0/disksize
```
上面命令设置了 zram0 的大小为 512MB，也就是说， zram0 能够存储 512MB 压缩后的数据。
<a name="g80J5"></a>
### 3、压缩算法选择
zRAM 机制支持多种压缩算法，不同的压缩算法有不同的压缩比率和压缩速度，用户可以按照自身的需求来选择不同的压缩算法。<br />要更改 zRAM 的压缩算法，可以使用下面命令：
```bash
echo lzo > /sys/block/zram0/comp_algorithm
```
上面命令将 zRAM 的压缩算法更改为 lzo，也可以通过下面命令来查看内核支持哪些压缩算法：
```bash
cat /sys/block/zram0/comp_algorithm
lzo [lz4]
```
从上面命令的输出可知，内核支持 lzo 和 lz4 两种压缩算法。
<a name="A7h4K"></a>
### 4、将 swap 交换设备设置为 zRAM
要将 swap 的交换设备设置为 zRAM 块设备，可以使用以下命令：
```bash
mkswap /dev/zram0
```
当执行完上面这条命令后，内核将会使用 zram0 作为 swap 的交换设备。
<a name="DcM3R"></a>
## zRAM实现
zRAM 块设备驱动的实现代码主要在 drivers/block/zram/zram_drv.c 文件中，下面主要围绕此文件进行分析。<br />本文并不会介绍块设备驱动的编写流程，只会分析 swap 机制在进行内存交换时，与 zRAM 块设备驱动的交互。
<a name="eq3Nc"></a>
### 压缩内存
当系统内存不足时，内核将会触发 swap 机制。swap 机制首先会从系统中选择一些进程不常用内存，然后将这些不常用的内存交换到 zRAM 块设备中（使用 zRAM 块设备作为交换设备的情况下）。<br />当 swap 机制将不常用的内存交换到 zRAM 块设备时，会调用 `zram_make_request()` 函数处理请求。而 `zram_make_request()` 最终会通过调用 `zram_bvec_write()` 函数来压缩内存，调用链如下：
```
zram_make_request()
 -> __zram_make_request()
     -> zram_bvec_rw()
         -> zram_bvec_write()
```
来分析一下 `zram_bvec_write()` 函数的实现，其代码如下：
```c
static int
zram_bvec_write(struct zram *zram, struct bio_vec *bvec, u32 index, int offset)
{
    ...
    // 1. 获取需要进行压缩的内存
    page = bvec->bv_page;
    ...
    user_mem = kmap_atomic(page);
    uncmem = user_mem;

    ...
    // 2. 对内存进行压缩
    ret = zcomp_compress(zram->comp, zstrm, uncmem, &clen);
    ...

    // 3. 获取压缩后的数据
    src = zstrm->buffer;
    ...

    // 4. 申请一个内存块保存压缩后的数据
    handle = zs_malloc(meta->mem_pool, clen);
    ...
    cmem = zs_map_object(meta->mem_pool, handle, ZS_MM_WO);

    // 5. 将压缩后的数据保存到新申请的内存块中
    memcpy(cmem, src, clen);
    ...

    // 6. 将压缩后的数据登记到 zRAM 块设备的表格中
    meta->table[index].handle = handle;
    ...
    return ret;
}
```
为了简化分析过程，对代码进行精简。从上面的代码可以看出，zRAM 机制对内存进行压缩的步骤如下：

- 获取需要进行压缩的内存，需要进行压缩的内存由 swap 机制提供。
- 通过 `zcomp_compress()` 函数对内存进行压缩，src 指针指向压缩后的内存地址。
- 通过 `zs_malloc()` 和 `zs_map_object()` 函数申请一块新的内存块，大小为压缩后数据的大小。
- 将压缩后的数据复制到新申请的内存块中。
- 将压缩后的数据记录到 zRAM 块设备的表格中。

由于 zRAM 块设备是建立在内存中的虚拟块设备，所以其并没有真实块设备的特性。真实块设备会将存储空间划分成一个个块，而 `zram_bvec_write()` 函数的 index 参数就是数据块的编号。此参数有 swap 机制提供，所以 zRAM 块设备驱动通过 index 参数作为原始内存数据的编号。<br />一图胜千言：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696557917398-c187b18c-282d-4d8f-9a33-7b5355b1055f.png#averageHue=%23e6d0d7&clientId=uad79faed-73e4-4&from=paste&id=u119c9c80&originHeight=570&originWidth=1018&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4ac48aaf-1391-467c-9796-984f86d0fd5&title=)<br />zRAM驱动有个数据块表，用来记录原始内存数据对应的压缩数据，此表的索引就是数据块的编号。swap 机制会维护此表格的使用情况，如哪个块是空闲的，哪个块被占用等。<br />当内存页被压缩后，swap 机制将会把原来的内存页释放掉，并且把所有映射到此内存页的进程解除映射，细节可以参考 swap 机制相关的资料。<br />对内存进行解压缩的过程与压缩过程相反，有兴趣的同学可以自行阅读代码，这里就不进行分析了。
