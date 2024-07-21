Linux<br />Linux 内核包含4个IO调度器，分别是 Noop IO scheduler、Anticipatory IO scheduler、Deadline IO scheduler 与 CFQ IO scheduler。<br />anticipatory，预期的；提早发生的；期待着的<br />通常磁盘的读写影响是由磁头到柱面移动造成了延迟，解决这种延迟内核主要采用两种策略：缓存和IO调度算法来进行弥补.
<a name="pYrZI"></a>
## 调度算法概念

1. **当向设备写入数据块或是从设备读出数据块时，请求都被安置在一个队列中等待完成.**
2. **每个块设备都有它自己的队列.**
3. **I/O调度程序负责维护这些队列的顺序，以更有效地利用介质.I/O调度程序将无序的I/O操作变为有序的I/O操作.**
4. **内核必须首先确定队列中一共有多少个请求，然后才开始进行调度.**

![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661995216631-60cae16f-646e-4ae9-b082-3d328f24a405.jpeg#clientId=ub50255dc-1212-4&from=paste&id=u704816c1&originHeight=1586&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0d128b19-62f7-424f-af2c-38259526551&title=)
<a name="Q99dh"></a>
## IO调度器（IO Scheduler）
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661995216525-b89e8d49-f7ff-4813-93e2-266fd1a28f16.png#clientId=ub50255dc-1212-4&from=paste&height=826&id=ub3fc30c9&originHeight=1652&originWidth=944&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubc838625-3812-4860-a7a4-8a24563ac20&title=&width=472)<br />IO调度器（IO Scheduler）是操作系统用来决定块设备上IO操作提交顺序的方法。存在的目的有两个，一是提高IO吞吐量，二是降低IO响应时间。<br />然而IO吞吐量和IO响应时间往往是矛盾的，为了尽量平衡这两者，IO调度器提供了多种调度算法来适应不同的IO请求场景。其中，对数据库这种随机读写的场景最有利的算法是DEANLINE。<br />IO调度器在内核栈中所处位置如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661995216606-4a518234-d771-4da9-82e4-4bdd848ac238.png#clientId=ub50255dc-1212-4&from=paste&id=u4aa75e3a&originHeight=540&originWidth=960&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua05d3acc-da84-4cf6-a77b-0a2e426e827&title=)![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661995216536-9c9c78ef-4170-4e41-ad07-294f427142ac.jpeg#clientId=ub50255dc-1212-4&from=paste&id=u36f6e16c&originHeight=752&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2a34a1e6-890d-4fd2-b9e3-90f3bc07129&title=)<br />块设备最悲剧的地方就是磁盘转动，这个过程会很耗时间。每个块设备或者块设备的分区，都对应有自身的请求队列(request_queue)，而每个请求队列都可以选择一个I/O调度器来协调所递交的request。<br />I/O调度器的基本目的是将请求按照它们对应在块设备上的扇区号进行排列，以减少磁头的移动，提高效率。每个设备的请求队列里的请求将按顺序被响应。<br />实际上，除了这个队列，每个调度器自身都维护有不同数量的队列，用来对递交上来的request进行处理，而排在队列最前面的request将适时被移动到请求队列中等待响应。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661995216514-340ad3dd-970f-4444-9162-945e96a3ad19.jpeg#clientId=ub50255dc-1212-4&from=paste&height=406&id=u216b4e27&originHeight=811&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue71e5441-28a7-4f32-945c-74eb0ca6719&title=&width=540)<br />IO scheduler 的作用主要是为了减少磁盘转动的需求。主要通过2种方式实现：

1. **合并**
2. ** 排序**

每个设备都会自己对应请求队列，所有的请求在被处理之前都会在请求队列上。在新来一个请求的时候如果发现这个请求和前面的某个请求请求的位置是相邻的，那么就可以合并为一个请求。<br />如果不能找到合并的，就会按照磁盘的转动方向进行排序。通常IO scheduler 的作用就是为了在进行合并和排序的同时，也不会太影响单个请求的处理时间。
<a name="CLPLP"></a>
### 1、NOOP
![FIFO](https://cdn.nlark.com/yuque/0/2022/png/396745/1661995216912-09543219-6502-4d6b-b4d7-f5a44bbab5f3.png#clientId=ub50255dc-1212-4&from=paste&id=u0bcd26fb&originHeight=316&originWidth=894&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ueb21eef0-d3c8-49b4-93a5-fcdbbc6a99f&title=FIFO "FIFO")

1. **noop是什么? noop是一种输入输出调度算法 . NOOP，No Operation. 什么都不做，请求来一个处理一个。这种方式事实起来简单，也更有效。问题就是disk seek 太多，对于传统磁盘，这是不能接受的。但对于SSD 磁盘就可以，因为SSD 磁盘不需要转动。**
2. **noop的别称 又称为电梯调度算法.**
3. **noop原理是怎样的?**

将输入输出请求放到一个FIFO队列中，然后按次序执行队列中的输入输出请求:当来一个新请求时:

1. **如果能合并就合并**
2. **如果不能合并，就会尝试排序。如果队列上的请求都已经很老了，这个新的请求就不能插队，只能放到最后面。否则就插到合适的位置**
3. **如果既不能合并，又没有合适的位置插入，就放到请求队列的最后。**
4. **适用场景4.1 在不希望修改输入输出请求先后顺序的场景下；4.2 在输入输出之下具有更加智能调度算法的设备，如NAS存储设备；4.3 上层应用程序已经精心优化过的输入输出请求；4.4 非旋转磁头式的磁盘设备，如SSD磁盘**
<a name="Xk0Vo"></a>
### 2、CFQ(Completely Fair Queuing，完全公平排队)
CFQ（Completely Fair Queuing）算法，顾名思义，绝对公平算法。它试图为竞争块设备使用权的所有进程分配一个请求队列和一个时间片，在调度器分配给进程的时间片内，进程可以将其读写请求发送给底层块设备，当进程的时间片消耗完，进程的请求队列将被挂起，等待调度。<br />每个进程的时间片和每个进程的队列长度取决于进程的IO优先级，每个进程都会有一个IO优先级，CFQ调度器将会将其作为考虑的因素之一，来确定该进程的请求队列何时可以获取块设备的使用权。<br />IO优先级从高到低可以分为三大类:<br />RT(real time)<br />BE(best try)<br />IDLE(idle)<br />其中RT和BE又可以再划分为8个子优先级。可以通过ionice 去查看和修改。优先级越高，被处理得越早，用于这个进程的时间片也越多，一次处理的请求数也会越多。<br />实际上，我们已经知道CFQ调度器的公平是针对于进程而言的，而只有同步请求(read或syn write)才是针对进程而存在的，他们会放入进程自身的请求队列，而所有同优先级的异步请求，无论来自于哪个进程，都会被放入公共的队列，异步请求的队列总共有8(RT)+8(BE)+1(IDLE)=17个。<br />从Linux 2.6.18起，CFQ作为默认的IO调度算法。对于通用的服务器来说，CFQ是较好的选择。具体使用哪种调度算法还是要根据具体的业务场景去做足benchmark来选择，不能仅靠别人的文字来决定。
<a name="JguB4"></a>
### 3、DEADLINE
DEADLINE在CFQ的基础上，解决了IO请求饿死的极端情况。<br />除了CFQ本身具有的IO排序队列之外，DEADLINE额外分别为读IO和写IO提供了FIFO队列。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661995217071-b3698b0a-4c4a-4310-b70f-4dfe703d80c1.png#clientId=ub50255dc-1212-4&from=paste&id=uc57a4805&originHeight=768&originWidth=834&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3d72255e-2131-4e59-9951-ca459ff870b&title=)<br />读FIFO队列的最大等待时间为500ms，写FIFO队列的最大等待时间为5s（当然这些参数都是可以手动设置的）。<br />FIFO队列内的IO请求优先级要比CFQ队列中的高，而读FIFO队列的优先级又比写FIFO队列的优先级高。优先级可以表示如下：<br />FIFO(Read) > FIFO(Write) > CFQ<br />deadline 算法保证对于既定的 IO 请求以最小的延迟时间，从这一点理解，对于 DSS 应用应该会是很适合的。<br />deadline 实际上是对Elevator 的一种改进: 

1. 避免有些请求太长时间不能被处理。
2. 区分对待读操作和写操作。

deadline IO 维护3个队列。第一个队列和Elevator 一样， 尽量按照物理位置排序。第二个队列和第三个队列都是按照时间排序，不同的是一个是读操作一个是写操作。<br />deadline IO 之所以区分读和写是因为设计者认为如果应用程序发了一个读请求，一般就会阻塞到那里，一直等到的结果返回。而写请求则不是通常是应用请求写到内存即可，由后台进程再写回磁盘。应用程序一般不等写完成就继续往下走。所以读请求应该比写请求有更高的优先级。<br />在这种设计下，每个新增请求都会先放到第一个队列，算法和Elevator的方式一样，同时也会增加到读或者写队列的尾端。这样首先处理一些第一队列的请求，同时检测第二/三队列前几个请求是否等了太长时间，如果已经超过一个阀值，就会去处理一下。这个阀值对于读请求时 5ms， 对于写请求时5s.<br />个人认为对于记录数据库变更日志的分区，例如oracle 的online log， mysql 的binlog 等等，最好不要使用这种分区。因为这类写请求通常是调用fsync 的。如果写完不成，也会很影响应用性能的。
<a name="s0VW5"></a>
### 4、ANTICIPATORY
CFQ和DEADLINE考虑的焦点在于满足零散IO请求上。对于连续的IO请求，比如顺序读，并没有做优化。<br />为了满足随机IO和顺序IO混合的场景，Linux还支持ANTICIPATORY调度算法。ANTICIPATORY的在DEADLINE的基础上，为每个读IO都设置了6ms的等待时间窗口。如果在这6ms内OS收到了相邻位置的读IO请求，就可以立即满足。
<a name="HnQZ4"></a>
## 小结
IO调度器算法的选择，既取决于硬件特征，也取决于应用场景。<br />在传统的SAS盘上，CFQ、DEADLINE、ANTICIPATORY都是不错的选择；对于专属的数据库服务器，DEADLINE的吞吐量和响应时间都表现良好。<br />然而在新兴的固态硬盘比如SSD、Fusion IO上，最简单的NOOP反而可能是最好的算法，因为其他三个算法的优化是基于缩短寻道时间的，而固态硬盘没有所谓的寻道时间且IO响应时间非常短。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661995217025-21982b84-50d1-448f-ba60-8595f0dc37cc.jpeg#clientId=ub50255dc-1212-4&from=paste&id=u1fd579ca&originHeight=1412&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue118bcd0-efbd-4851-9cbe-800196564d7&title=)
