Linux
<a name="k5zET"></a>
## **通过free命令看Linux内存**
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660744513719-8e56fd11-c24f-447b-8b63-aecfaabba345.png#clientId=u39b2e672-e5f8-4&from=paste&id=ua33324ba&originHeight=21&originWidth=746&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u601e0126-bebe-40f2-b08c-5bbc1d63ae5&title=)

- **total：总内存大小。**
- **used：已经使用的内存大小（这里面包含cached和buffers和shared部分）。**
- **free：空闲的内存大小。**
- **shared：进程间共享内存（一般不会用，可以忽略）。**
- **buffers：内存中写完的东西缓存起来，这样快速响应请求，后面数据再定期刷到磁盘上。**
- **cached：内存中读完缓存起来内容占的大小（这部分是为了下次查询时快速返回）。**
<a name="ZOe0q"></a>
### -/+ buffers/cache看做两部分

- **-buffers/cache：正在使用的内存大小（注意不是used部分，因为buffers和cached并不是正在使用的，组织和人民需要是它们是可以释放的），其值=used-buffers-cached。**
- **``+buffers/cache`：可用的内存大小（同理也不是free表示的部分），其值=free+buffers+cached。**
- **Swap：硬盘上交换分区的使用大小。**

设计的目的就是当上面提到的+buffers/cache表示的可用内存都已使用完，新的读写请求过来后，会把内存中的部分数据写入磁盘，从而把磁盘的部分空间当做虚拟内存来使用。
<a name="k5LrX"></a>
## **Buffer和Cache介绍**
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660744513729-bad13d05-7599-463a-b3df-20fc967db33c.png#clientId=u39b2e672-e5f8-4&from=paste&id=u78bc5bf8&originHeight=107&originWidth=589&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uda7f56e2-fd76-49e8-a556-f5e42ec093a&title=)<br />Cache（缓存），为了调高CPU和内存之间数据交换而设计，Buffer（缓冲）为了提高内存和硬盘（或其他I/O设备的数据交换而设计）。<br />Cache主要是针对读操作设计的，不过Cache概念可能容易混淆，理解为CPU本身就有Cache，包括一级缓存、二级缓存、三级缓存，CPU所有的指令操作对接的都是内存，而CPU的处理能力远高于内存速度，所以为了不让CPU资源闲置，Intel等公司在CPU内部集成了一些Cache，但毕竟不能放太多电路在里面，所以这部分Cache并不是很大，主要是用来存放一些常用的指令和常用数据，真正大部分Cache的数据应该是占用内存的空间来缓存请求过的数据，即上面的Cached部分（这部分纯属个人理解，正确与否有待考证）。<br />Buffer主要是针对写操作设计的，更细的说是针对内存和硬盘之间的写操作来设计的，目的是将写的操作集中起来进行，减少磁盘碎片和硬盘反复寻址过程，提高性能。<br />在Linux系统内部有一个守护进程会定期清空Buffer中的内容，将其写入硬盘内，当手动执行sync命令时也会触发上述操作。
<a name="iM0mv"></a>
## **常见症状**
症状一：在Linux中频繁存取文件，物理内存很快用光，而cached一直在增长。<br />解释：Linux会对每次请求过的数据缓存在cache里，好处就是CPU的处理速度远远高于内存，所以在CPU和内存通讯的时候可以快速从cache中命中结果返回。<br />症状二：Swap被占用。<br />解释：内存可能不够了，才会占Swap，所以Swap可以作为服务器监控的一项指标，引起注意。
<a name="CYBuC"></a>
## **手动清理Swap和buffers/cache**
<a name="Lexh3"></a>
### 清理Swap
```bash
swapoff -a && swapon -a
```
操作说明：如果已经使用了Swap，且当前清空下+buffers/cache还有空间，在执行  swapoff -a操作时，会触发把Swap中的内容交换到内存中，数据不会丢失。
<a name="tf9us"></a>
### 清理buffers/cache:
```bash
sync; sync; sync;&& echo 3 >/proc/sys/vm/drop_caches

sleep 2

echo 0 > /proc/sys/vm/drop_caches
```
操作说明：<br />`sync`-->将缓存的内存写回到硬盘中；<br />`echo 3 >/proc/sys/vm/drop_caches`-->修改drop_caches的值为3，默认为0，改为3系统会清理缓存的内容；<br />`sleep 2` --> 等一下，防止上一步没执行完；<br />`echo 0 >/proc/sys/vm/drop_caches` --> 改回默认值
<a name="uskjL"></a>
## **总结**
通过上面的分析可以知道，当空闲物理内存不多时，不一定表示系统运行状态很差，因为内存的cache及buffer部分可以随时被重用，在某种意义上，这两部分内存也可以看作是额外的空闲内存。<br />swap如果被频繁调用，bi，bo长时间不为0，则才是内存资源是否紧张的依据。通过free看资源时，实际主要关注-/+ buffers/cache的值就可以知道内存到底够不够了。
