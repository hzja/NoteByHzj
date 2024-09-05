Linux<br />主要以问答的方式来探索Linux内存系统的分配策略
<a name="IkOdY"></a>
## Linux 进程的内存分布长什么样？
在 Linux 操作系统中，虚拟地址空间的内部又被分为**内核空间和用户空间**两部分，不同位数的系统，地址空间的范围也不同。比如最常见的 32 位和 64 位系统，如下所示：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1674873882352-888620c3-b535-485f-bed6-54f1a20d945f.jpeg#averageHue=%23e9c19a&clientId=uf7fd76c1-a943-4&from=paste&id=u255e8b96&originHeight=415&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uca7f24a8-7d45-4a29-b0c2-79c8cc9d158&title=)<br />通过这里可以看出：

- 32 位系统的内核空间占用 1G，位于最高处，剩下的 3G 是用户空间；
- 64 位系统的内核空间和用户空间都是 128T，分别占据整个内存空间的最高和最低处，剩下的中间部分是未定义的。

再来说说，内核空间与用户空间的区别：

- 进程在用户态时，只能访问用户空间内存；
- 只有进入内核态后，才可以访问内核空间的内存；

虽然每个进程都各自有独立的虚拟内存，但是**每个虚拟内存中的内核地址，其实关联的都是相同的物理内存**。这样，进程切换到内核态后，就可以很方便地访问内核空间内存。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1674873882321-3d6978c6-1741-45ec-99de-5f76a8c43ca3.jpeg#averageHue=%23e5e8e5&clientId=uf7fd76c1-a943-4&from=paste&id=ua1acee45&originHeight=426&originWidth=950&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf0471e00-c092-47dc-86cf-db567fa8afd&title=)<br />接下来，进一步了解虚拟空间的划分情况，用户空间和内核空间划分的方式是不同的，内核空间的分布情况就不多说了。<br />看看用户空间分布的情况，以 32 位系统为例，画了一张图来表示它们的关系：<br />通过这张图可以看到，用户空间内存从**低到高**分别是 6 种不同的内存段：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873882381-4f1142ef-cd63-4061-805b-431ffaac51ea.png#averageHue=%23f4c392&clientId=uf7fd76c1-a943-4&from=paste&id=ua49877a6&originHeight=1324&originWidth=894&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8cbd5324-f2cc-42f2-8391-8601f839018&title=)

- 程序文件段，包括二进制可执行代码；
- 已初始化数据段，包括静态常量；
- 未初始化数据段，包括未初始化的静态变量；
- 堆段，包括动态分配的内存，从低地址开始向上增长；
- 文件映射段，包括动态库、共享内存等，从低地址开始向上增长（跟硬件和内核版本有关 ）；
- 栈段，包括局部变量和函数调用的上下文等。栈的大小是固定的，一般是 8 MB。当然系统也提供了参数，以便自定义大小；

在这 6 个内存段中，堆和文件映射段的内存是动态分配的。比如说，使用 C 标准库的 `malloc()` 或者 `mmap()` ，就可以分别在堆和文件映射段动态分配内存。
<a name="LGfkE"></a>
## `malloc` 是如何分配内存的？
实际上，`malloc()` 并不是系统调用，而是 C 库里的函数，用于动态分配内存。<br />malloc 申请内存的时候，会有两种方式向操作系统申请堆内存。

- 方式一：通过 `brk()` 系统调用从堆分配内存
- 方式二：通过 `mmap()` 系统调用在文件映射区域分配内存；

方式一实现的方式很简单，就是通过 `brk()` 函数将「堆顶」指针向高地址移动，获得新的内存空间。如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873882431-979f6453-55c6-4728-992f-4d19b562fb94.png#averageHue=%23f4f3f1&clientId=uf7fd76c1-a943-4&from=paste&id=ufa88051b&originHeight=702&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue9ce5341-4f7a-41e0-aa31-0025f379e79&title=)<br />方式二通过 `mmap()` 系统调用中「私有匿名映射」的方式，在文件映射区分配一块内存，也就是从文件映射区“偷”了一块内存。如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873882481-d5ce53a8-41c9-447c-a5dd-5f3507816bcf.png#averageHue=%23f3f3f1&clientId=uf7fd76c1-a943-4&from=paste&id=u16dee537&originHeight=676&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud18c9333-bc7b-498a-9b98-4475ba7eeaa&title=)<br />什么场景下 `malloc()` 会通过 `brk()` 分配内存？又是什么场景下通过 `mmap()` 分配内存？<br />`malloc()` 源码里默认定义了一个阈值：

- 如果用户分配的内存小于 128 KB，则通过 `brk()` 申请内存；
- 如果用户分配的内存大于 128 KB，则通过 `mmap()` 申请内存；
> 注意，不同的 glibc 版本定义的阈值也是不同的。

`malloc()` 分配的是物理内存吗？<br />不是的，`**malloc()**`** 分配的是虚拟内存**。<br />如果分配后的虚拟内存没有被访问的话，虚拟内存是不会映射到物理内存的，这样就不会占用物理内存了。<br />只有在访问已分配的虚拟地址空间的时候，操作系统通过查找页表，发现虚拟内存对应的页没有在物理内存中，就会触发缺页中断，然后操作系统会建立虚拟内存和物理内存之间的映射关系。
<a name="Nvhu3"></a>
## `malloc(1)` 会分配多大的虚拟内存？
`malloc()` 在分配内存的时候，并不是老老实实按用户预期申请的字节数来分配内存空间大小，而是**会预分配更大的空间作为内存池**。<br />具体会预分配多大的空间，跟 malloc 使用的内存管理器有关系，就以 malloc 默认的内存管理器（Ptmalloc2）来分析。<br />接下里，做个实验，用下面这个代码，通过 malloc 申请 1 字节的内存时，看看操作系统实际分配了多大的内存空间。
```c
#include <stdio.h>
#include <malloc.h>

int main() {
  printf("使用cat /proc/%d/maps查看内存分配\n",getpid());
  
  //申请1字节的内存
  void *addr = malloc(1);
  printf("此1字节的内存起始地址：%x\n", addr);
  printf("使用cat /proc/%d/maps查看内存分配\n",getpid());
 
  //将程序阻塞，当输入任意字符时才往下执行
  getchar();

  //释放内存
  free(addr);
  printf("释放了1字节的内存，但heap堆并不会释放\n");
  
  getchar();
  return 0;
}
```
执行代码（**先提前说明，这里使用的 glibc 库的版本是 2.17**）：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873882950-689d832b-3e0a-4a9e-ab09-c2f7d9ad9b21.png#averageHue=%230e1018&clientId=uf7fd76c1-a943-4&from=paste&id=u6e25d6f4&originHeight=174&originWidth=854&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub1f89618-a789-4b1b-beb7-65e7ddaae45&title=)<br />可以通过 /proc//maps 文件查看进程的内存分布情况。在 maps 文件通过此 1 字节的内存起始地址过滤出了内存地址的范围。
```bash
[root@xiaolin ~]# cat /proc/3191/maps | grep d730
00d73000-00d94000 rw-p 00000000 00:00 0                                  [heap]
```
这个例子分配的内存小于 128 KB，所以是通过 `brk()` 系统调用向堆空间申请的内存，因此可以看到最右边有 [heap] 的标识。<br />可以看到，堆空间的内存地址范围是 00d73000-00d94000，这个范围大小是 132KB，也就说明了 `**malloc(1)**`** 实际上预分配 132K 字节的内存**。<br />可能有的同学注意到了，程序里打印的内存起始地址是 d73010，而 maps 文件显示堆内存空间的起始地址是 d73000，为什么会多出来 0x10 （16字节）呢？这个问题先放着，后面会说。<br />#free 释放内存，会归还给操作系统吗？<br />在上面的进程往下执行，看看通过 `free()` 函数释放内存后，堆内存还在吗？<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873883011-76fd4a0b-0d0a-4cb4-a330-92cb682a066e.png#averageHue=%230f1119&clientId=uf7fd76c1-a943-4&from=paste&id=uc6e75356&originHeight=254&originWidth=784&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u826bf115-7a11-4fd3-8521-da60a79b791&title=)<br />从下图可以看到，通过 free 释放内存后，堆内存还是存在的，并没有归还给操作系统。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873883093-a582066d-fa7b-4a31-babb-6fede343b431.png#averageHue=%23101119&clientId=uf7fd76c1-a943-4&from=paste&id=u26b73b50&originHeight=86&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u015ad7a6-d00f-407b-abfa-a5fdccc8ea9&title=)<br />这是因为与其把这 1 字节释放给操作系统，不如先缓存着放进 malloc 的内存池里，当进程再次申请 1 字节的内存时就可以直接复用，这样速度快了很多。<br />当然，当进程退出后，操作系统就会回收进程的所有资源。<br />上面说的 free 内存后堆内存还存在，是针对 malloc 通过 brk() 方式申请的内存的情况。<br />如果 malloc 通过 mmap 方式申请的内存，free 释放内存后就会归归还给操作系统。<br />做个实验验证下， 通过 malloc 申请 128 KB 字节的内存，来使得 malloc 通过 mmap 方式来分配内存。
```c
#include <stdio.h>
#include <malloc.h>

int main() {
  //申请1字节的内存
  void *addr = malloc(128*1024);
  printf("此128KB字节的内存起始地址：%x\n", addr);
  printf("使用cat /proc/%d/maps查看内存分配\n",getpid());

  //将程序阻塞，当输入任意字符时才往下执行
  getchar();

  //释放内存
  free(addr);
  printf("释放了128KB字节的内存，内存也归还给了操作系统\n");

  getchar();
  return 0;
}
```
执行代码：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873883209-1c7aee1c-abe5-4722-93fa-985d24f59177.png#averageHue=%230f1118&clientId=uf7fd76c1-a943-4&from=paste&id=u9b2e06bc&originHeight=166&originWidth=842&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3399e992-28fe-48a0-be12-2c8b9986c9d&title=)<br />查看进程的内存的分布情况，可以发现最右边没有 [head] 标志，说明是通过 mmap 以匿名映射的方式从文件映射区分配的匿名内存。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873883259-9a32082f-0766-4a72-b3bb-dd1582027fac.png#averageHue=%230f1119&clientId=uf7fd76c1-a943-4&from=paste&id=u7f5579c3&originHeight=94&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6c983eeb-2db6-4b1d-8f63-d8e0f35e240&title=)<br />然后释放掉这个内存看看：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873883600-d2716ef4-cea2-4ee9-8405-368e5f242596.png#averageHue=%230f1118&clientId=uf7fd76c1-a943-4&from=paste&id=u8c7619d8&originHeight=250&originWidth=856&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u69fb2732-7153-4a32-b33f-a86af3f6dee&title=)<br />再次查看该 128 KB 内存的起始地址，可以发现已经不存在了，说明归还给了操作系统。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873883686-ded6d95a-b088-436b-b609-160c19a2c935.png#averageHue=%230e1018&clientId=uf7fd76c1-a943-4&from=paste&id=ue391c373&originHeight=126&originWidth=934&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf52c8ce3-a3be-49c1-9c45-7a9df5fe02b&title=)<br />对于 「malloc 申请的内存，free 释放内存会归还给操作系统吗？」这个问题，可以做个总结了：

- malloc 通过 `**brk()**` 方式申请的内存，free 释放内存的时候，**并不会把内存归还给操作系统，而是缓存在 malloc 的内存池中，待下次使用**；
- malloc 通过 `**mmap()**` 方式申请的内存，free 释放内存的时候，**会把内存归还给操作系统，内存得到真正的释放**。
<a name="Gpmqp"></a>
## 为什么不全部使用 mmap 来分配内存？
因为向操作系统申请内存，是要通过系统调用的，执行系统调用是要进入内核态的，然后在回到用户态，运行态的切换会耗费不少时间。<br />所以，申请内存的操作应该避免频繁的系统调用，如果都用 mmap 来分配内存，等于每次都要执行系统调用。<br />另外，因为 mmap 分配的内存每次释放的时候，都会归还给操作系统，于是每次 mmap 分配的虚拟地址都是缺页状态的，然后在第一次访问该虚拟地址的时候，就会触发缺页中断。<br />也就是说，**频繁通过 mmap 分配的内存话，不仅每次都会发生运行态的切换，还会发生缺页中断（在第一次访问虚拟地址后），这样会导致 CPU 消耗较大**。<br />为了改进这两个问题，malloc 通过 brk() 系统调用在堆空间申请内存的时候，由于堆空间是连续的，所以直接预分配更大的内存来作为内存池，当内存释放的时候，就缓存在内存池中。<br />**等下次在申请内存的时候，就直接从内存池取出对应的内存块就行了，而且可能这个内存块的虚拟地址与物理地址的映射关系还存在，这样不仅减少了系统调用的次数，也减少了缺页中断的次数，这将大大降低 CPU 的消耗**。
<a name="fBbqW"></a>
## 既然 brk 那么牛逼，为什么不全部使用 brk 来分配？
前面提到通过 brk 从堆空间分配的内存，并不会归还给操作系统，那么考虑这样一个场景。<br />如果连续申请了 10k，20k，30k 这三片内存，如果 10k 和 20k 这两片释放了，变为了空闲内存空间，如果下次申请的内存小于 30k，那么就可以重用这个空闲内存空间。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873883770-4757162e-cfca-46fd-9a3f-8e35e1d5cd41.png#averageHue=%23f4f2ef&clientId=uf7fd76c1-a943-4&from=paste&id=u2a30e977&originHeight=692&originWidth=804&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1cee01a6-f075-47c5-93cc-f717b620566&title=)<br />但是如果下次申请的内存大于 30k，没有可用的空闲内存空间，必须向 OS 申请，实际使用内存继续增大。<br />因此，随着系统频繁地 malloc 和 free ，尤其对于小块内存，堆内将产生越来越多不可用的碎片，导致“内存泄露”。而这种“泄露”现象使用 valgrind 是无法检测出来的。<br />所以，malloc 实现中，充分考虑了 brk 和 mmap 行为上的差异及优缺点，默认分配大块内存 (128KB) 才使用 mmap 分配内存空间。
<a name="cw6Ck"></a>
## `free()` 函数只传入一个内存地址，为什么能知道要释放多大的内存？
还记得前面提到， malloc 返回给用户态的内存起始地址比进程的堆空间起始地址多了 16 字节吗？<br />这个多出来的 16 字节就是保存了该内存块的描述信息，比如有该内存块的大小。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1674873883737-4942577c-7831-405e-8472-51d8a00ffc5d.png#averageHue=%23f3e9e9&clientId=uf7fd76c1-a943-4&from=paste&id=u0920af1d&originHeight=272&originWidth=527&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5976c1df-60dc-4135-8241-29874e9f68f&title=)<br />这样当执行 `free()` 函数时，free 会对传入进来的内存地址向左偏移 16 字节，然后从这个 16 字节的分析出当前的内存块的大小，自然就知道要释放多大的内存了。
