Linux<br />CPU、IO、磁盘、内存可以说是影响计算机性能关键因素，来探究下内存的那些事儿。<br />![2022-02-23-12-35-32-022258.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595958182-78099a50-8667-4380-a590-c58063a3d034.png#clientId=u3794eab8-6c7d-4&from=ui&id=u620067ae&originHeight=908&originWidth=1010&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2756679&status=done&style=none&taskId=ub20f5a54-8469-4a39-b995-98473dfa2b9&title=)<br />内存为进程的运行提供物理空间，同时作为快速CPU和慢速磁盘之间的适配器，可以说是个非常重要的角色。<br />![2022-02-23-12-35-32-169348.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595957903-10dba064-0cd0-405b-b4e1-6dc1e4f8f1ec.png#clientId=u3794eab8-6c7d-4&from=ui&id=Rb86X&originHeight=517&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1678384&status=done&style=none&taskId=ubcf2ec46-5cc2-4f14-b082-39f994d3328&title=)<br />通过本文了解到以下内容：<br />![2022-02-23-12-35-32-291579.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595957926-c4befe15-67fa-462c-a9d0-591a73959545.png#clientId=u3794eab8-6c7d-4&from=ui&id=UR1B0&originHeight=698&originWidth=934&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1959729&status=done&style=none&taskId=ucf011fc1-ad9d-487b-8b17-ef59ed309e6&title=)<br />围绕Linux操作系统展开！
<a name="S1Bjs"></a>
## 虚拟内存机制
当要学习一个新知识点时，比较好的过程是先理解出现这个技术点的背景原因，同期其他解决方案，新技术点解决了什么问题以及它存在哪些不足和改进之处，这样整个学习过程是闭环的。
<a name="iMgcj"></a>
### 内存为什么需要管理
老子的著名观点是无为而治，简单说就是不过多干预而充分依靠自觉就可以有条不紊地运作，理想是美好的，现实是残酷的。<br />Linux系统如果以一种原始简单的方式管理内存是存在一些问题:
<a name="UFuaM"></a>
#### 进程空间隔离问题
假如现在有ABC三个进程运行在Linux的内存空间，设定OS给进程A分配的地址空间是0-20M，进程B地址空间30-80M，进程C地址空间90-120M。<br />虽然分配给每个进程的空间是无交集的，但是仍然无法避免进程在某些情况下出现访问异常的情况，如图：<br />![2022-02-23-12-35-32-394710.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645596128274-35c0fd30-ae5b-4730-9166-4dab861b84e1.png#clientId=u3794eab8-6c7d-4&from=ui&id=u665f1409&originHeight=412&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57598&status=done&style=none&taskId=u213fef65-e954-4292-9e53-362c894c4f1&title=)<br />比如进程A访问了属于进程B的空间，进程B访问了属于进程C的空间，甚至修改了空间的值，这样就会造成混乱和错误，实际中是不允许发生的。<br />所以需要的是每个进程有独立且隔离的安全空间。
<a name="mb1Wm"></a>
#### 内存效率低下问题
机器的内存是有限资源，而进程数量是动态且无法确定的，这样就会出现几个必须要考虑的问题：

- 如果已经启动的进程们占据了几乎所有内存空间，没有新内存可分配了，此时新进程将无法启动。
- 已经启动的进程有时候是在睡大觉，也就是给了内存也不用，占着茅坑不拉屎。
- 连续内存实在是很珍贵，大部分时候都无法给进程分配它想要的连续内存，离散化内存才是需要面对的现实。

![2022-02-23-12-35-32-573104.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595892368-3613d311-115f-4412-9e6d-2717324ecf8e.png#clientId=u3794eab8-6c7d-4&from=ui&id=ue8f46aa7&originHeight=489&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1587489&status=done&style=none&taskId=u64946451-e193-40b8-b8ff-bbe93f218c5&title=)
<a name="GC3iV"></a>
#### 定位调试和编译运行问题
由于程序运行时的位置是不确定的，在定位问题、调试代码、编译执行时都会存在很多问题。<br />希望每个进程有一致且完整的地址空间，同样的起始位置放置了堆、栈以及代码段等，从而简化编译和执行过程中的链接器、加载器的使用。<br />换句话说，如果所有进程的空间地址分配都是一样的，那么Linux在设计编译和调试工具时就非常简单了，否则每个进程都可能是定制化的。![2022-02-23-12-35-32-959081.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595892693-6fbe6611-4251-4f03-9932-decf347e19eb.png#clientId=u3794eab8-6c7d-4&from=ui&id=hgha5&originHeight=756&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2454238&status=done&style=none&taskId=ud7368156-58bf-4c76-a0b0-a893bde0f27&title=)<br />综上，面对众多问题，需要一套内存管理机制。
<a name="v5ge2"></a>
### 中间层的引入
大家一定听过这句计算机谚语：<br />Any problem in computer science can be solved by another layer of indirection.<br />计算机科学领域的任何问题都可以通过增加一个中间层来解决，解决内存问题也不例外。<br />![2022-02-23-12-35-34-221024.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595892887-140b0f08-2b50-416e-91d4-3f3d59c2b0bd.png#clientId=u3794eab8-6c7d-4&from=ui&id=iauFO&originHeight=896&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2908708&status=done&style=none&taskId=ua40e9e54-028c-43aa-9944-bdd0ece74ef&title=)<br />Linux的虚拟内存机制简单来说就是在物理内存和进程之间请了个管家，内存管家上任之后做了以下几件事情：

- 给每个进程分配完全独立的虚拟空间，每个进程终于有只属于自己的活动场地了
- 进程使用的虚拟空间最终还要落到物理内存上，因此设置了一套完善的虚拟地址和物理地址的映射机制
- 引入缺页异常机制实现内存的惰性分配，啥时候用啥时候再给
- 引入swap机制把不活跃的数据换到磁盘上，让每块内存都用在刀刃上
- 引入OOM机制在内存紧张的情况下干掉那些内存杀手
- ......
<a name="Flh44"></a>
#### 虚拟内存下数据读写问题
引入虚拟机制后，进程在获取CPU资源读取数据时的流程也发生了一些变化。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595283573-05065881-92b4-4da6-9121-aa34d57aad15.png#clientId=u3794eab8-6c7d-4&from=paste&id=uaf5e3668&originHeight=388&originWidth=677&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud9560dd4-a3cd-4723-863e-962eeb4c7f8&title=)<br />CPU并不再直接和物理内存打交道，而是把地址转换的活外包给了MMU，MMU是一种硬件电路，其速度很快，主要工作是进行内存管理，地址转换只是它承接的业务之一。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595283731-7380bcad-1775-49c1-86fb-8b5d27797f64.png#clientId=u3794eab8-6c7d-4&from=paste&id=u2f29fd87&originHeight=175&originWidth=692&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf83f35cc-c20b-4419-ac8b-1f07dcdd737&title=)
<a name="zm0YA"></a>
#### 页表的存储和检索问题
每个进程都会有自己的页表Page Table，页表存储了进程中虚拟地址到物理地址的映射关系，所以就相当于一张地图，MMU收到CPU的虚拟地址之后开始查询页表，确定是否存在映射以及读写权限是否正常，如图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595283725-a613c7e1-c418-40a7-bd5c-f0010b89c647.png#clientId=u3794eab8-6c7d-4&from=paste&id=u49a0a5ed&originHeight=519&originWidth=644&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufde12bb3-bc15-4873-8900-668f0f746e6&title=)<br />当机器的物理内存越来越大，页表这个地图也将非常大，于是问题出现了：

- 对于4GB的虚拟地址且大小为4KB页，一级页表将有2^20个表项，页表占有连续内存并且存储空间大
- 多级页表可以有效降低页表的存储空间以及内存连续性要求，但是多级页表同时也带来了查询效率问题

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595284166-d1e31e3a-ea66-4601-8c6f-6008694edc8e.png#clientId=u3794eab8-6c7d-4&from=paste&id=u4f5b4b2e&originHeight=298&originWidth=523&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u526a75b3-dcce-4411-97dc-d731e072778&title=)<br />以2级页表为例，MMU要先进行两次页表查询确定物理地址，在确认了权限等问题后，MMU再将这个物理地址发送到总线，内存收到之后开始读取对应地址的数据并返回。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595284245-f41f8809-0295-432a-9063-55647d49c07d.png#clientId=u3794eab8-6c7d-4&from=paste&id=u585db8ca&originHeight=607&originWidth=864&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u48060bd8-0ad0-435a-b8e2-c3280fec34f&title=)<br />MMU在2级页表的情况下进行了2次检索和1次读写，那么当页表变为N级时，就变成了N次检索+1次读写。<br />可见，页表级数越多查询的步骤越多，对于CPU来说等待时间越长，效率越低，这个问题还需要优化才行。
<a name="CCHim"></a>
##### 本段小结 敲黑板 划重点
页表存在于进程的内存之中，MMU收到虚拟地址之后查询Page Table来获取物理地址。<br />单级页表对连续内存要求高，于是引入了多级页表。<br />多级页表也是一把双刃剑，在减少连续存储要求且减少存储空间的同时降低了查询效率。
<a name="wkoMy"></a>
#### MMU和TLB这对黄金搭档
CPU觉得MMU干活虽然卖力气，但是效率有点低，不太想继续外包给它了，这一下子把MMU急坏了。<br />MMU于是找来了一些精通统计的朋友，经过一番研究之后发现CPU用的数据经常是一小搓，但是每次MMU都还要重复之前的步骤来检索，害，就知道埋头干活了，也得讲究方式方法呀！<br />找到瓶颈之后，MMU引入了新武器，江湖人称快表的TLB，别看TLB容量小，但是正式上岗之后干活还真是不含糊。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595284280-3b3d32c6-e9d8-4190-a1d7-5d015ace1cca.png#clientId=u3794eab8-6c7d-4&from=paste&id=u7ba431b2&originHeight=281&originWidth=411&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u100101cb-9016-4376-aea9-705b618faf0&title=)<br />当CPU给MMU传新虚拟地址之后，MMU先去问TLB那边有没有，如果有就直接拿到物理地址发到总线给内存，齐活。<br />TLB容量比较小，难免发生Cache Miss，这时候MMU还有保底的老武器页表 Page Table，在页表中找到之后MMU除了把地址发到总线传给内存，还把这条映射关系给到TLB，让它记录一下刷新缓存。<br />TLB容量不满的时候就直接把新记录存储了，当满了的时候就开启了淘汰大法把旧记录清除掉，来保存新记录，仿佛完美解决了问题。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595284490-ced8f370-3a8a-4e6d-9260-1a1e2dcdd7db.png#clientId=u3794eab8-6c7d-4&from=paste&id=u686bbe56&originHeight=507&originWidth=738&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u62d38dd1-3773-47ea-91c9-62e6159e961&title=)
<a name="WJgcR"></a>
##### 本段小结 敲黑板 划重点
**MMU也是个聪明的家伙，集成了TLB来存储CPU最近常用的页表项来加速寻址，TLB找不到再去全量页表寻址，可以认为TLB是MMU的缓存。**
<a name="U4Cwm"></a>
#### 缺页异常来了
假如目标内存页在物理内存中没有对应的页帧或者存在但无对应权限，CPU 就无法获取数据，这种情况下CPU就会报告一个缺页错误。<br />由于CPU没有数据就无法进行计算，CPU罢工了用户进程也就出现了缺页中断，进程会从用户态切换到内核态，并将缺页中断交给内核的 Page Fault Handler 处理。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595284713-2fe67512-d9a7-4dd2-bab0-0dc7a5f0420f.png#clientId=u3794eab8-6c7d-4&from=paste&id=ubcd3e3ea&originHeight=491&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue4ee4292-9636-4ec4-88cf-04d340cd3ff&title=)<br />缺页中断会交给PageFaultHandler处理，其根据缺页中断的不同类型会进行不同的处理：

- **Hard Page Fault**<br />也被称为Major Page Fault，翻译为硬缺页错误/主要缺页错误，这时物理内存中没有对应的页帧，需要CPU打开磁盘设备读取到物理内存中，再让MMU建立VA和PA的映射。
- **Soft Page Fault**<br />也被称为Minor Page Fault，翻译为软缺页错误/次要缺页错误，这时物理内存中是存在对应页帧的，只不过可能是其他进程调入的，发出缺页异常的进程不知道而已，此时MMU只需要建立映射即可，无需从磁盘读取写入内存，一般出现在多进程共享内存区域。
- **Invalid Page Fault**<br />翻译为无效缺页错误，比如进程访问的内存地址越界访问，又比如对空指针解引用内核就会报segment fault错误中断进程直接挂掉。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595284866-feb5f860-9cae-446b-9c7f-187d245c3e6a.png#clientId=u3794eab8-6c7d-4&from=paste&id=ue6b47789&originHeight=610&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub68e5ef9-fe3d-441e-b1d6-779183b4d0a&title=)<br />不同类型的Page Fault出现的原因也不一样，常见的几种原因包括：

- **非法操作访问越界**<br />这种情况产生的影响也是最大的，也是Coredump的重要来源，比如空指针解引用或者权限问题等都会出现缺页错误。
- **使用malloc新申请内存**<br />malloc机制是延时分配内存，当使用malloc申请内存时并未真实分配物理内存，等到真正开始使用malloc申请的物理内存时发现没有才会启动申请，期间就会出现Page Fault。
- **访问数据被swap换出**<br />物理内存是有限资源，当运行很多进程时并不是每个进程都活跃，对此OS会启动内存页面置换将长时间未使用的物理内存页帧放到swap分区来腾空资源给其他进程，当存在于swap分区的页面被访问时就会触发Page Fault从而再置换回物理内存。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595284801-b519c1e2-2ac9-4765-a8e1-787eb8ca25d2.png#clientId=u3794eab8-6c7d-4&from=paste&id=ud36bb2fb&originHeight=558&originWidth=749&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3857068f-9c4d-4fc8-9fd6-1c72944be44&title=)
<a name="chctD"></a>
##### 本段小结 敲黑板 划重点
**缺页异常在虚拟机制下是必然会出现的，原因非常多，没什么大不了的，在缺页异常的配合下合法的内存访问才能得到响应。**<br />基本弄清楚了为什么需要内存管理、虚拟内存机制主要做什么、虚拟机制下数据的读写流程等等。<br />![2022-02-23-12-35-35-915275.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595758926-65060a0b-86fb-44e7-aeab-437cc2381eaa.png#clientId=u3794eab8-6c7d-4&from=ui&id=u7e15f840&originHeight=526&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1707606&status=done&style=none&taskId=ub4b8bc47-0123-4e24-be59-c829bf324f6&title=)
<a name="Ea7tQ"></a>
## 内存分配
虚拟机制下每个进程都有独立的地址空间，并且地址空间被划分为了很多部分，如图为32位系统中虚拟地址空间分配：<br />![2022-02-23-12-35-36-114914.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595760060-9cfb2aa2-b7b3-40a9-88ce-20568dd17183.png#clientId=u3794eab8-6c7d-4&from=ui&id=B55qq&originHeight=1344&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4363020&status=done&style=none&taskId=u71fe030e-c396-40e8-b3a3-3a20c90219c&title=)<br />64位系统也是类似的，只不过对应的空间都扩大为128TB。<br />来看看各个段各自特点和相互联系：

- text段包含了当前运行进程的二进制代码，所以又被称为代码段，在32位和64位系统中代码段的起始地址都是确定的，并且大小也是确定的。
- data段存储已初始化的全局变量，和text段紧挨着，中间没有空隙，因此起始地址也是固定的，大小也是确定的。
- bss段存储未初始化的全局变量，和data段紧挨着，中间没有空隙，因此起始地址也是固定的，大小也是确定的。
- heap段和bss段并不是紧挨着的，中间会有一个随机的偏移量，**heap段的起始地址也被称为start_brk**，由于heap段是动态的，顶部位置称为program break brk。
- 在heap段上方是内存映射段，该段是mmap系统调用映射出来的，该段的大小也是不确定的，并且夹在heap段和stack段中间，该段的起始地址也是不确定的。
- stack段算是用户空间地址最高的一部分了，它也并没有和内核地址空间紧挨着，中间有随机偏移量，同时**一般stack段会设置最大值RLIMIT_STACK(比如8MB)**，在之下再加上一个随机偏移量就是内存映射段的起始地址了。

抓住几点：

- **进程虚拟空间的各个段，并非紧挨着，也就是有的段的起始地址并不确定，大小也并不确定**
- **随机的地址是为了防止黑客的攻击，因为固定的地址被攻击难度低很多**

把heap段、stack段、mmap段再细化一张图：<br />![2022-02-23-12-35-36-424349.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595710314-74c05642-33c1-41d3-8ce7-edf9a0db7ff1.png#clientId=u3794eab8-6c7d-4&from=ui&id=ubcb6d694&originHeight=858&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2785350&status=done&style=none&taskId=u13b8cf62-779b-4f40-af51-5480a396ec3&title=)<br />从图上可以看到各个段的布局关系和随机偏移量的使用，多看几遍就清楚啦！
<a name="HqAfD"></a>
### 内存区域的组织
从前面可以看到进程虚拟空间就是一块块不同区域的集合，这些区域就是上面的段，每个区域在Linux系统中使用vm_area_struct这个数据结构来表示的。<br />内核为每个进程维护了一个单独的任务结构task_strcut，该结构中包含了进程运行时所需的全部信息，其中有一个内存管理(memory manage)相关的成员结构mm_struct：
```c
struct mm_struct  *mm;
struct mm_struct  *active_mm;
```
结构mm_strcut的成员非常多，其中gpd和mmap是需要关注的：

- pgd指向第一级页表的基地址，是实现虚拟地址和物理地址的重要部分
- mmap指向一个双向链表，链表节点是vm_area_struct结构体，vm_area_struct描述了虚拟空间中的一个区域
- mm_rb指向一个红黑树的根结点，节点结构也是vm_area_struct

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595285437-9f0c3abc-8fb1-48dd-8ba2-b91c52a07011.png#clientId=u3794eab8-6c7d-4&from=paste&id=ud0ad6496&originHeight=478&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucb244971-4273-4e8b-bd4e-f19d08fa72a&title=)<br />看下vm_area_struct的结构体定义，后面要用到，注意看哈：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595285487-8623c385-0f92-4197-9c7c-1499d31ba654.png#clientId=u3794eab8-6c7d-4&from=paste&id=ued0514cb&originHeight=895&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1aa1ae00-1fb9-419e-8538-b27562bb15e&title=)<br />vm_area_start作为链表节点串联在一起，每个vm_area_struct表示一个虚拟内存区域，由其中的vm_start和vm_end指向了该区域的起始地址和结束地址，这样多个vm_area_struct就将进程的多个段组合在一起了。<br />![2022-02-23-12-35-37-504641.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595648671-6c35a373-1fa3-406b-b0ce-6e97921e59c0.png#clientId=u3794eab8-6c7d-4&from=ui&id=u6c0efa16&originHeight=819&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2658756&status=done&style=none&taskId=uaf75f908-66d3-45a7-9dbf-44d7d402010&title=)<br />同时注意到vm_area_struct的结构体定义中有rb_node的相关成员，不过有的版本内核是AVL-Tree，这样就和mm_struct对应起来了：<br />![2022-02-23-12-35-37-825650.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595647924-e06c35fe-fd04-4588-9b6c-37abe9fca075.png#clientId=u3794eab8-6c7d-4&from=ui&id=FvrRb&originHeight=398&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1292081&status=done&style=none&taskId=u5612b010-e74a-41f2-bae8-93e73d2d787&title=)<br />这样vm_area_struct通过双向链表和红黑树两种数据结构串联起来，实现了两种不同效率的查找，双向链表用于遍历vm_area_struct，红黑树用于快速查找符合条件的vm_area_struct。
<a name="SiKjd"></a>
### 内存分配器概述
有内存分配和回收的地方就可能有内存分配器。<br />以glibc为例，先捋一下：

- 在用户态层面，进程使用库函数malloc分配的是虚拟内存，并且系统是延迟分配物理内存的，由缺页中断来完成分配
- 在内核态层面，内核也需要物理内存，并且使用了另外一套不同于用户态的分配机制和系统调用函数

从而就引出了，今天的主线图：<br />![2022-02-23-12-35-38-423324.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595648778-e9db0c05-ff37-4411-985d-09302da8e524.png#clientId=u3794eab8-6c7d-4&from=ui&id=AwUrq&originHeight=900&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2921701&status=done&style=none&taskId=uac425b1c-bba1-407e-8fb9-dd7f290f842&title=)<br />从图中来阐述几个重点：

- 伙伴系统和slab属于内核级别的内存分配器，同时为内核层面内存分配和用户侧面内存分配提供服务，算是终极boss的赶脚
- 内核有自己单独的内存分配函数kmalloc/vmalloc，和用户态的不一样，毕竟是中枢机构嘛
- 用户态的进程通过库函数malloc来玩转内存，malloc调用了brk/mmap这两个系统调用，最终触达到伙伴系统实现内存分配
- 内存分配器分为两大类：用户态和内核态，用户态分配和释放内存最终还是通过内核态来实现的，用户态分配器更加贴合进程需求，有种社区居委会的感觉
<a name="ZQdDS"></a>
### 常见用户态内存分配器
进程的内存分配器工作于内核和用户程序之间，主要是为了实现用户态的内存管理。<br />**分配器响应进程的内存分配请求，向操作系统申请内存，找到合适的内存后返回给用户程序，当进程非常多或者频繁内存分配释放时，每次都找内核老大哥要内存/归还内存，可以说十分麻烦。**<br />总麻烦大哥，也不是个事儿，于是分配器决定自己搞管理！

- 分配器一般都会预先分配一块大于用户请求的内存，然后管理这块内存
- 进程释放的内存并不会立即返回给操作系统，分配器会管理这些释放掉的内存从而快速响应后续的请求

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595286107-ba8e4608-9552-4e09-bacf-06db3dc17455.png#clientId=u3794eab8-6c7d-4&from=paste&id=udd3aa2b1&originHeight=457&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u64aad564-1a95-49c1-b549-c54fcb54b6f&title=)<br />说到管理能力，每个人每个国家都有很大差别，分配器也不例外，要想管好这块内存也挺难的，场景很多要求很多，于是就出现了很多分配器：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595286282-762e3918-849c-47a8-9e57-873bf53fd2d2.png#clientId=u3794eab8-6c7d-4&from=paste&id=u524eaa64&originHeight=660&originWidth=846&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9ff07f5f-f165-45d1-9f60-7482d997f87&title=)
<a name="JgLYC"></a>
#### dlmalloc
dlmalloc是一个著名的内存分配器，最早由Doug Lea在1980s年代编写，由于早期C库的内置分配器在某种程度上的缺陷，dlmalloc出现后立即获得了广泛应用，后面很多优秀分配器中都能看到dlmalloc的影子，可以说是鼻祖了。<br />[http://gee.cs.oswego.edu/dl/html/malloc.html](http://gee.cs.oswego.edu/dl/html/malloc.html)
<a name="LcS9g"></a>
#### ptmalloc2
ptmalloc是在dlmalloc的基础上进行了多线程改造，认为是dlmalloc的扩展版本，它也是目前glibc中使用的默认分配器，不过后续各自都有不同的修改，因此ptmalloc2和glibc中默认分配器也并非完全一样。
<a name="XJRSw"></a>
#### tcmalloc
tcmalloc 出身于 Google，全称是 thread-caching malloc，所以 tcmalloc 最大的特点是带有线程缓存，tcmalloc 非常出名，目前在 Chrome、Safari 等知名产品中都有所应有。<br />tcmalloc 为每个线程分配了一个局部缓存，对于小对象的分配，可以直接由线程局部缓存来完成，对于大对象的分配场景，tcmalloc 尝试采用自旋锁来减少多线程的锁竞争问题。
<a name="uU3jy"></a>
#### jemalloc
jemalloc 是由 Jason Evans 在 FreeBSD 项目中引入的新一代内存分配器。<br />它是一个通用的 malloc 实现，侧重于减少内存碎片和提升高并发场景下内存的分配效率，其目标是能够替代 malloc。<br />jemalloc 应用十分广泛，在 Firefox、Redis、Rust、Netty 等出名的产品或者编程语言中都有大量使用。<br />具体细节可以参考 Jason Evans 发表的论文 《A Scalable Concurrent malloc Implementation for FreeBSD》<br />论文链接：[https://www.bsdcan.org/2006/papers/jemalloc.pdf](https://www.bsdcan.org/2006/papers/jemalloc.pdf)
<a name="smdm3"></a>
#### glibc malloc原理分析
在使用malloc进行内存分配，malloc只是glibc提供的库函数，它仍然会调用其他函数从而最终触达到物理内存，所以是个很长的链路。<br />先看下malloc的特点：

- malloc 申请分配指定size个字节的内存空间，返回类型是 void* 类型，但是此时的内存只是虚拟空间内的连续内存，无法保证物理内存连续
- malloc 并不关心进程用申请的内存来存储什么类型的数据，void* 类型可以强制转换为任何其它类型的指针，从而做到通用性
```c
/* malloc example */
#include <stdio.h>
#include <stdlib.h>

int main ()
{
  int i,n;
  char * buffer;
  scanf ("%d", &i);

  buffer = (char*) malloc (i+1);
  if (buffer==NULL) exit (1);

  for (n=0; n<i; n++)
    buffer[n]=rand()%26+'a';
  buffer[i]='\0';
  free (buffer);
  return 0;
}
```
上面是malloc作为库函数和用户交互的部分，如果不深究原理，掌握上面这些就可以使用malloc了，但是对于这些追求极致的人来说，还远远不够。<br />继续看下 malloc是如何触达到物理内存的：
```c
#include <unistd.h>
int brk(void *addr);
void *sbrk(intptr_t increment);
```

- brk函数将break指针直接设置为某个地址，相当于绝对值
- sbrk将break指针从当前位置移动increment所指定的增量，相当于相对值
- 本质上brk和sbrk作用是一样的都是移动break指针的位置来扩展内存
```c
#include <sys/mman.h>
void *mmap(void *addr, size\_t length, int prot, int flags, int fd, off\_t offset);
int munmap(void *addr, size_t length);
```

- mmap和munmap是一对函数，一个负责申请，一个负责释放
- mmap有两个功能：实现文件映射到内存区域 和 分配匿名内存区域，在malloc中使用的就是匿名内存分配，从而为程序存放数据开辟空间

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595286692-bdca2818-b2e0-4c72-af94-6fdbf51fd0e3.png#clientId=u3794eab8-6c7d-4&from=paste&id=u7b0f0ab9&originHeight=452&originWidth=990&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u67583494-6834-4c40-a91d-e806f696e67&title=)
<a name="rrKnG"></a>
#### malloc底层数据结构
malloc的核心工作就是组织管理内存，高效响应进程的内存使用需求，同时保证内存的使用率，降低内存碎片化。<br />那么malloc是如何解决这些问题呢？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595286639-0b65690b-a7d4-45fe-a9c7-3f7f11ada4c4.png#clientId=u3794eab8-6c7d-4&from=paste&id=u8b645a42&originHeight=398&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uacd825e6-759b-430c-b4ff-d04578440ae&title=)<br />malloc为了解决这些问题，采用了多种数据结构和策略来实现内存分配，这就是接下来研究的事情：

- 什么样的数据结构
- 什么样的组织策略

事情没有一蹴而就，很难理解内存分配器设计者面临的复杂问题，因此当看到malloc底层复杂的设计逻辑时难免没有头绪，所以要忽略细节抓住主线多看几遍。<br />**malloc将内存分成了大小不同的chunk，malloc将相似大小的chunk用双向链表链接起来，这样一个链表被称为一个bin。**<br />这些空闲的不同大小的内存块chunk通过bin来组织起来，换句话说bin是空闲内存块chunk的容器。<br />malloc一共维护了128个bin，并使用一个数组来存储这些bin。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595286809-6ab9716f-91f9-44f7-a2c3-149c9ccec988.png#clientId=u3794eab8-6c7d-4&from=paste&id=u6942ef6e&originHeight=1098&originWidth=1070&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u779f80d5-8010-4a31-ab57-4991fd15833&title=)<br />malloc中128个bin的bins数组存储的chunk情况如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595286764-46f1e19e-fd6b-4db8-9fda-9b31b1a64eb3.png#clientId=u3794eab8-6c7d-4&from=paste&id=u3037d826&originHeight=740&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u25ea2c1b-a47d-4cbb-aa23-73b85d7b87c&title=)

- bins[0]目前没有使用
- bins[1]的链表称为unsorted_list，用于维护free释放的chunk。
- bins[2,63]总计长度为62的区间称为small_bins，用于维护＜512B的内存块，其中每个bin中对应的链表中的chunk大小相同，相邻bin的大小相差8字节，范围为16字节到504字节。

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595286889-de153277-0af5-4d88-96e7-2bef8995ed8d.png#clientId=u3794eab8-6c7d-4&from=paste&id=u9f728e69&originHeight=792&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4f7f130c-5bb5-47a8-8ceb-bae4f8a2459&title=)

- bins[64,126]总计长度为63的区间称为large_bins，用于维护大于等于512字节的内存块，每个元素对应的链表中的chunk大小不同，数组下标越大链表中chunk的内存越大，large bins中的每一个bin分别包含了一个给定范围内的chunk，其中的chunk按大小递减排序，最后一组的largebin链中的chunk大小无限制，该bins的使用频率低于small bins。

malloc有两种特殊类型的bin：
<a name="DDHu5"></a>
##### fast bin
malloc对于释放的内存并不会立刻进行合并，如何将刚释放的两个相邻小chunk合并为1个大chunk，此时进程分配仍然是小chunk则可能还需要分割大chunk，来来回回确实很低效，于是出现了fast bin。<br />fast bin存储在fastbinY数组中，一共有10个，每个fast bin都是一个单链表，每个单链表中的chunk大小是一样的，多个链表的chunk大小不同，这样在找特定大小的chunk的时候就不用挨个找，只需要计算出对应链表的索引即可，提高了效率。
```c
// http://gee.cs.oswego.edu/pub/misc/malloc-2.7.2.c
/* The maximum fastbin request size we support */
#define MAX_FAST_SIZE     80
#define NFASTBINS  (fastbin_index(request2size(MAX_FAST_SIZE))+1)
```
多个fast bin链表存储的chunk大小有16, 24, 32, 40, 48, 56, 64, 72, 80, 88字节总计10种大小。<br />fast bin是除tcache外优先级最高的，如果fastbin中有满足需求的chunk就不需要再到small bin和large bin中寻找。当在fast bin中找到需要的chunk后还将与该chunk大小相同的所有chunk放入tcache，目的就是利用局部性原理提高下一次内存分配的效率。<br />对于不超过max_fast的chunk被释放后，首先会被放到 fast bin中，当给用户分配的 chunk 小于或等于 max_fast 时，malloc 首先会在 fast bin 中查找相应的空闲块，找不到再去找别的bin。
<a name="ShLkz"></a>
##### unsorted bin
当小块或大块内存被释放时，它们会被添加到 unsorted bin 里，相当于malloc给了最近被释放的内存被快速二次利用的机会，在内存分配的速度上有所提升。<br />当用户释放的内存大于max_fast或者fast bins合并后的chunk都会首先进入unsorted bin上，unsorted bin中的chunk大小没有限制。<br />在进行 malloc 操作的时候，如果在 fast bins 中没有找到合适的 chunk，则malloc 会先在 unsorted bin 中查找合适的空闲 chunk。<br />unsorted bin里面的chunk是最近回收的，但是并不能全部再被快速利用，因此在遍历unsorted bins的过程中会把不同大小的chunk再分配到small bins或者large bins。<br />malloc在chunk和bin的结构之上，还有两种特殊的chunk：
<a name="jkAYE"></a>
##### top chunk
top chunk不属于任何bin，它是始终位于堆内存的顶部。<br />当所有的bin里的chunk都无法满足分配要求时，malloc会从top chunk分配内存，如果大小不合适会进行分割，剩余部分形成新的top chunk。<br />如果top chunk也无法满足用户的请求，malloc只能向系统申请更多的堆空间，所以top chunk可以认为是各种bin的后备力量，尤其在分配大内存时，large bins也无法满足时大哥就得顶上了。
<a name="JYKnV"></a>
##### last remainder chunk
当unsorted bin只有1个chunk，并且这个chunk是上次刚刚被使用过的内存块，那么它就是last remainder chunk。<br />当进程分配一个small chunk，在small bins中找不到合适的chunk，这时last remainder chunk就上场了。

- 如果last remainder chunk大于所需的small chunk大小，它会被分裂成两个chunk，其中一个chunk返回给用户，另一个chunk变成新的last remainder chunk。

这种特殊chunk主要用于分配内存非常小的情况下，当fast bin和small bin都无法满足时，还会再次从last remainder chunk进行分配，这样就很好地利用了程序局部性原理。
<a name="e58K9"></a>
#### malloc内存分配流程
前面了解到malloc为了实现内存的分配，采用了一些数据结构和组织策略，接着来看看实际的内存分配流程以及这些数据结构之间的关系。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595287322-3e25ffd1-ef86-4d74-9586-6c2ef66510ed.png#clientId=u3794eab8-6c7d-4&from=paste&id=u456c68b4&originHeight=642&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u89ca3ffa-38b0-46ab-a51e-12b67b13640&title=)<br />在上图中有几个点需要说明：

- 内存释放后，size小于max_fast则放到fast bin中，size大于max_fast则放到unsorted bin中，fast bin和unsorted bin可以看作是刚释放内存的容器，目的是给这些释放内存二次被利用的机会。
- fast bin中的fast chunk被设置为不可合并，但是如果一直不合并也就爆了，因此会定期合并fast chunk到unsorted bin中。
- unsorted bin很特殊，可以认为是个中间过渡bin，在large bin分割chunk时也会将下脚料chunk放到unsorted bin中等待后续合并以及再分配到small bin和large bin中。
- 由于small bin和large bin链表很多并且大小各不相同，遍历查找合适chunk过程是很耗时的，为此引入binmap结构来加速查找，binmap记录了bins的是否为空等情况，可以提高效率。

当用户申请的内存比较小时，分配过程会比较复杂，再尝试梳理下该情况下的分配流程：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595287351-854fc9fb-fb48-4b59-9176-8afe94d23579.png#clientId=u3794eab8-6c7d-4&from=paste&id=u9a624966&originHeight=807&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u64b201ae-7642-41ec-8560-49036b77e95&title=)<br />查找合适空闲内存块的过程涉及循环过程，因此把各个步骤标记顺序来表述过程。

1. 将进程需要分配的内存转换为对应空闲内存块的大小，记做chunk_size。
2. 当chunk_size小于等于max_fast，则在fast bin中搜索合适的chunk，找到则返回给用户，否则跳到第3步。
3. 当chunk_size<=512字节，那么可能在small bin的范围内有合适的chunk，找到合适的则返回，否则跳到第4步。
4. 在fast bin和small bin都没有合适的chunk，那么就对fast bin中的相邻chunk进行合并，合并后的更大的chunk放到unsorted bin中，跳转到第5步。
5. 如果chunk_size属于small bins，unsorted bin 中只有一个 chunk，并且该 chunk 大于等于需要分配的大小，此时将该 chunk 进行切割，一部分返回给用户，另外一部分形成新的last remainder chunk分配结束，否则将 unsorted bin 中的 chunk 放入 small bins 或者 large bins，进入第6步。
6. 现在看chunk_size属于比较大的，因此在large bins进行搜索，满足要求则返回，否则跳到第7步。
7. 至此fast bin和另外三组bin都无法满足要求，就轮到top chunk了，在top chunk满足则返回，否则跳到第8步。
8. 如果chunk_size大于等于mmap分配阈值，使用mmap向内核伙伴系统申请内存，chunk_size小于mmap阈值则使用brk来扩展top chunk满足要求。

特别地，搜索合适chunk的过程中，fast bins 和small bins需要大小精确匹配，而在large bins中遵循“smallest-first，best-fit”的原则，不需要精确匹配，因此也会出现较多的碎片。
<a name="u63TG"></a>
## 内存回收
内存回收的必要性显而易见，试想一直分配不回收，当进程们需要新大块内存时肯定就没内存可用了，为此内存回收必须要搞起来。
<a name="g4viu"></a>
### 页面回收
内存回收就是释放掉比如缓存和缓冲区的内存，通常他们被称为文件页page cache，对于通过mmap生成的用于存放程序数据而非文件数据的内存页称为匿名页。

- **文件页** 有外部的文件介质形成映射关系
- **匿名页** 没有外部的文件形成映射关系

这两种物理页面在某些情况下是可以回收的，但是处理方式并不同。
<a name="aoM0H"></a>
#### 文件页回收
page cache常被用于缓冲磁盘文件的数据，让磁盘数据放到内存中来实现CPU的快速访问。<br />page cache中有非常多page frame，要回收这些page frame需要确定这些物理页是否还在用，为了解决这个问题出现了**反向映射技术**。<br />正向映射是通过虚拟地址根据页表找到物理内存，**反向映射就是通过物理地址找到哪些虚拟地址使用它，也就是当在决定page frame是否可以回收时，需要使用反向映射来查看哪些进程被映射到这块物理页了，进一步判断是否可以回收**。<br />反向映射技术最早并没有在内核中出现，从诞生到被广泛推广也经历了很多波折，并且细节很多，这里找了一篇关于反向映射很棒的文章：<br />[https://cclinuxer.github.io/2020/11/Linux%E5%8F%8D%E5%90%91%E6%98%A0%E5%B0%84%E6%9C%BA%E5%88%B6/](https://cclinuxer.github.io/2020/11/Linux%E5%8F%8D%E5%90%91%E6%98%A0%E5%B0%84%E6%9C%BA%E5%88%B6/)<br />找到可以回收的page frame之后内核使用LRU算法进行回收，Linux采用的方法是维护2个双向链表，一个是包含了最近使用页面的active list，另一个是包含了最近不使用页面的inactive list。

- **active_list** 活跃内存页链表，这里存放的是最近被访问过的内存页，属于安全区。
- **inactive_list** 不活跃内存页链表，这里存放的是很少被访问的内存页，属于毒区。
<a name="GSfJr"></a>
#### 匿名页回收
**匿名页没有对应的文件形成映射，因此也就没有像磁盘那样的低速备份。**<br />**在回收匿名页的时候，需要先保存匿名页上的内容到特定区域，这样才能避免数据丢失保证后续的访问。**<br />匿名页在进程中是非常普遍的，动态分配的堆内存都可以说是匿名页，Linux为回收匿名页，特地开辟了swap space来存储内存上的数据，关于swap机制的文章太多了，这算是个常识的东西了，所以本文不啰嗦啦！<br />内核倾向于回收page cache中的物理页面，只有当内存很紧张并且内核配置允许swap机制时，才会选择回收匿名页。<br />回收匿名页意味着将数据放到了低速设备，一旦被访问性能损耗也很大，因此现在大内存的物理机器经常关闭swap来提高性能。
<a name="AbYW9"></a>
### kswapd线程和waterMark
NUMA架构下每个CPU都有自己的本地内存来加速访问避免总线拥挤，在本地内存不足时又可以访问其他Node的内存，但是访问速度会下降。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595287452-3c590bc4-24be-42cc-99e1-34129940c173.png#clientId=u3794eab8-6c7d-4&from=paste&id=uf9284127&originHeight=244&originWidth=586&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u440fa6b0-85ff-42a4-9449-8d8ae1fdca8&title=)<br />**每个CPU加本地内存被称作Node，一个node又被划分为多个zone，每个zone有自己一套内存水位标记，来记录本zone的内存水平，同时每个node有一个kswapd内核线程来回收内存。**<br />Linux内核中有一个非常重要的内核线程kswapd，负责在内存不足的情况下回收页面，系统初始化时，会为每一个NUMA内存节点创建一个名为kswapd的内核线程。<br />在内存不足时内核通过wakeup_kswapd()函数唤醒kswapd内核线程来回收页面，以便释放一些内存，kswapd的回收方式又被称为background reclaim。<br />Linux内核使用水位标记（watermark）的概念来描述这个压力情况。<br />Linux为内存的使用设置了三种内存水位标记，high、low、min，当内存处于不同阶段会触发不同的内存回收机制，来保证内存的供应，如图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595287441-71a59ec6-bea8-48ad-8ed3-1a061c1ab00a.png#clientId=u3794eab8-6c7d-4&from=paste&id=u8ec4e15e&originHeight=575&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u08ffa2e6-d8f3-4e6c-a233-6bdea8df0d9&title=)<br />他们所标记的分别含义为：

- 水位线在high以上表示内存剩余较多，目前内存使用压力不大，kswapd处于休眠状态
- 水位线在high-low的范围表示目前虽然还有剩余内存但是有点紧张，kswapd开始工作进行内存回收
- 水位线在low-min表示剩余可用内存不多了压力山大，min是最小的水位标记，当剩余内存达到这个状态时，就说明内存面临很大压力。
- 水位线低于min这部分内存，就会触发直接回收内存。
<a name="c39JR"></a>
### OOM机制
OOM(Out Of Memory)是Linux内核在可用内存较少或者某个进程瞬间申请并使用超额的内存，此时空闲的物理内存是远远不够的，此时就会触发OOM。<br />为了保证其他进程兄弟们能正常跑，内核会让OOM Killer根据设置参数和策略选择认为最值得被杀死的进程，杀掉它然后释放内存来保证大盘的稳定。<br />OOM Killer这个杀手很多时候不够智慧，经常会遇到进程A是个重要程序，正在欢快稳定的跑着，此时杀出来个进程B，瞬间要申请大量内存，Linux发现满足不了这个程咬金，于是就祭出大招OOM Killer，但是结果却是把进程A给杀了。<br />在oom的源码中可以看到，作者关于如何选择最优进程的一些说明：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595287511-2a2ebe24-fca8-4817-8848-194eebdc4fc6.png#clientId=u3794eab8-6c7d-4&from=paste&id=u0ac5cea0&originHeight=357&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u80221e10-aa76-4a40-b10b-8edf75673fc&title=)<br />[https://github.com/torvalds/linux/blob/master/mm/oom_kill.c](https://github.com/torvalds/linux/blob/master/mm/oom_kill.c)<br />**oom_killer在选择最优进程时决策并不完美**，只是做到了"还行"，根据策略对进程打分，选择分数最高的进程杀掉。<br />具体的计算在oom_badness函数中进行的，如下为分数的计算：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595287944-cc22a655-f533-4745-9429-1791e3403809.png#clientId=u3794eab8-6c7d-4&from=paste&id=uae0341df&originHeight=249&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua14fd41e-5107-46f9-9bba-b5ff912a3ca&title=)<br />其中涉及进程正在使用的物理内存RSS+swap分区+页面缓冲，再对比总内存大小，同时还有一些配置来避免杀死最重要的进程。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1645595288042-05b82670-2812-4279-984d-9effd667c23a.png#clientId=u3794eab8-6c7d-4&from=paste&id=u0636b45b&originHeight=478&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udffdacc8-8c73-4959-aabc-6bc0bdf98ee&title=)<br />进程设置OOM_SCORE_ADJ_MIN时，说明该进程为不可被杀死，返回的得分就非常低，从而被oom killer豁免。
