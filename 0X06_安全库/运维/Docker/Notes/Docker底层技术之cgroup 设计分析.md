Docker<br />cgroup 可能很多人都不了解，但提起 Docker 估计每个后端程序员都了解过。是的，Docker 已经成为程序员必须掌握的技术之一了。Docker 主要解决了传统虚拟机启动慢、占用大量资源的缺点。<br />当然，本文的重点并不是 Docker，而是 Docker 的底层支撑技术 cgroup。可以这样说，没有 cgroup 就没有 Docker。
<a name="lEEoQ"></a>
## 什么是 cgroup
cgroup 的全称为 control group，中文翻译为 控制组。主要用于控制进程组对某种资源的使用，这些资源包括但不限于：内存、CPU、I/O 和 网络 等。<br />如下图所示，使用 cgroup 来限制进程组对内存的使用：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660183235408-3c44e054-2507-410f-ba31-1950f3e812ac.jpeg#clientId=uef28c7f2-fad3-4&from=paste&id=u753258cb&originHeight=804&originWidth=1034&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc1827ca3-fe5c-48e7-9fe5-f9c4988e1ad&title=)<br />在上图中，创建了 2 个 cgroup（每个 cgroup 有 4 个进程），并且限制它们各自最多只能使用 2GB 的内存。如果使用超过 2GB 的内存，那么将会触发 OOM（Out Of Memory） 错误。<br />cgroup 通过把进程划分成控制组（一个控制组包含一个或多个进程），并且可以对控制组进行资源使用的控制，也就是说 cgroup 作用对象是控制组。<br />cgroup 提供了将进程组织成控制组的能力，然后通过使用 资源控制子系统（cgroup_subsys） 来对控制组进行资源使用的控制，cgroup 支持的 资源控制子系统 有以下几种：

- **cpu子系统：限制 CPU 的使用。**
- **memory子系统：限制内存使用。**
- **cpuset子系统：可以为进程组分配单独的 CPU 或者内存节点。**
- **cpuacct子系统：统计CPU group的使用情况。**
- **blkio子系统：限制I/O，一般用于磁盘。**
- **devices子系统：限制进程使用的设备。**
- **freezer子系统：可以挂起和恢复进程组。**
- **net_cls子系统：可以标记进程组的网络数据包，使用 tc 模块（traffic control）对数据包进行控制。**

也就是说，cgroup 通过把进程组织成 控制组，然后通过 资源控制子系统 来对 控制组 进行资源使用的限制，所以 cgroup 的分析可以分成两部分：cgroup框架 和 资源控制子系统。
<a name="nJilC"></a>
## cgroup 源码分析
cgroup 的设计还是比较复杂的，主要是因为 cgroup 涉及多种资源的控制，并且 cgroup 通过虚拟文件系统来组织进程控制组，所以导致 cgroup 的实现变得复杂难懂。<br />为了不会让大家陷入枯燥的概念和源码之中，本文主要通过以设计者的角度来分析 cgroup 的设计与实现。
<a name="MDTgu"></a>
### 1、设计一个简单的 cgroup
如果让你来设计一个限制进程组对内存使用的方案，你会怎么设计呢？<br />最简单的方法就是，创建一个内存使用的计数器，然后将进程组中所有的进程都指向这个计数器。当进程组的进程申请内存时，就增加计数器的值，如果计数器超过限制就触发错误。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660183235435-61e78e96-20d2-46c0-ba70-3fa6805b9667.jpeg#clientId=uef28c7f2-fad3-4&from=paste&id=ua8659240&originHeight=548&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5d8a11b0-9f72-4bbd-9571-3cd5a7d5ef5&title=)<br />上图中计数器的 limit 字段表示限制进程中使用的最大内存数，而 count 字段表示当前进程使用的内存数。每当进程组中的进程申请内存时，都需要增加计数器的 count 字段，并且比较 count 是否已经超出 limit 的限制。<br />数据结构可以这样设计：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660183235419-3502ed45-6dc6-43ec-bf68-949c68dc091f.jpeg#clientId=uef28c7f2-fad3-4&from=paste&id=u493dac2c&originHeight=602&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubb35441f-388d-41cb-9c81-d1d89f628a0&title=)<br />通过链表来将进程组织成进程组，并且在计数器中增加一个 task_group 字段，让其指向进程组。当进程组中的进程申请内存时，可以通过指针来找到对应的计数器，并且增加计数器的 count 字段。<br />就这样，设计了一个简单的 cgroup 功能。如果系统只有内存这种资源的话，的确可以这样设计。但是系统除了内存，还有CPU、硬盘和网络这些资源，所以 Linux 创建了一种比较通用的方式来组织进程组。
<a name="EdnaP"></a>
### 2、控制组
有了上面的雏形，cgroup 的很多概念就比较容易理解了，下面主要介绍一下 控制组 这个概念。<br />控制组 说白了就是一组进程（进程组），cgroup 就是用来限制 控制组 的资源使用。为了能够方便地向一个 控制组 添加或者移除进程（在命令行也能操作），内核使用了 虚拟文件系统 来进行管理 控制组。<br />可以把一个 控制组 当成是一个目录，由于目录有层级关系，所以 控制组 也有层级关系，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660183235443-16dd71aa-8753-43b4-9d1a-e8710b9be7bf.jpeg#clientId=uef28c7f2-fad3-4&from=paste&id=u18c9fc80&originHeight=559&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubf37b92b-e416-4d16-83e4-57595c7e863&title=)<br />如上图所示，控制组 是以目录树来组织的，每一个目录代表一个 控制组。在内核中，一个由 控制组 组成的目录树被称为 层级（hierarchy）。<br />每个控制组目录中，都有一个名为 tasks 的文件，用于保存当前 控制组 包含的进程列表。如果想向某个 控制组 添加一个进程时，可以把进程的 PID 写入到 tasks 文件中。例如：
```bash
$ cd /sys/fs/cgroup/memory/cgrp1    # 进入控制组cgrp1
$ echo 1029 > tasks                 # 将PID为1029的进程添加到cgrp1控制组中
```
也可以通过读取 tasks 文件来查看某个 控制组 中的进程列表，例如：
```bash
$ cat tasks
1
2
3
5
6
7
...
```
在内核中，控制组使用 cgroup 结构来表示，其定义如下：
```c
struct cgroup {
    ...
    // 下面3个字段把控制组连接成一个树结构
    struct list_head sibling;   // 兄弟节点
    struct list_head children;  // 子节点
    struct cgroup *parent;      // 父节点

    struct dentry *dentry;      // 当前控制组对应的目录对象

    // 当前控制组关联的子系统资源统计对象
    struct cgroup_subsys_state *subsys[CGROUP_SUBSYS_COUNT]; 
    ...
};
```
内核通过 cgroup 结构的 sibling、children 和 parent 这3个字段来将 控制组 组织成一棵树状结构。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660183235495-c47a44dd-91bb-4ca8-afef-58e2e075dce3.jpeg#clientId=uef28c7f2-fad3-4&from=paste&id=u536fdd63&originHeight=844&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uac4e0b50-2079-49dc-b702-8bf5b9930ba&title=)<br />另外，cgroup 结构的 subsys 字段表示当前控制组关联的子系统状态对象，下面介绍 资源控制子系统 时将会详细介绍。<br />在 Linux 内核中，可以存在多个 层级（控制组树），每个层级可以关联一个或多个 资源控制子系统，但同一个 资源控制子系统 不能关联到多个层级中（也就是说，同一种 资源控制子系统 只能关联到一个层级）。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660183236063-21fa3a99-1520-46e2-8b61-f8e99ff14caf.jpeg#clientId=uef28c7f2-fad3-4&from=paste&id=u1defda7b&originHeight=490&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9367072f-a571-4721-8679-fc7e802f1b3&title=)<br />在内核中，层级 的根结点使用 cgroupfs_root 结构来表示， 来看看其定义：
```c
struct cgroupfs_root {
    struct super_block *sb;            // 挂载点超级块对象（虚拟文件系统使用）
    unsigned long subsys_bits;         // 当前层级绑定的资源子系统位图（1表示已经绑定到当前层级）
    ...
    struct list_head subsys_list;      // 绑定到当前层级的资源子系统列表
    struct cgroup top_cgroup;          // 当前层级的根控制组
    int number_of_cgroups;             // 当前层级拥有的控制组数量
    ...
};
```
在 Linux 内核中，有个名为 rootnode 的根层级，在系统启动后，由内核自动创建并且初始化的层级。系统启动后，所有的资源控制子系统都关联到此层级。rootnode 的定义如下：
```c
// 定义在文件 ./kernel/cgroup.c 中

static struct cgroupfs_root rootnode;
```
如果用户想把资源控制子系统关联到其他层级，那么可以使用 mount 命令来进行挂载，如下命令所示：<br />$ mount -t cgroup -o memory memory /sys/fs/cgroup/memory

上面的命令用于将内存子系统重新关联到 /sys/fs/cgroup/memory 这个层级。
<a name="YaDau"></a>
### 3、资源控制子系统
继续来介绍 资源控制子系统 (下面简称子系统) 这个重要的概念。<br />在 **设计一个简单的 cgroup** 例子中，主要以内存资源作为分析对象。但我们知道，计算机不单止只有内存资源，还有譬如 CPU、硬盘和网络等资源。所以，cgroup 不单止要控制内存资源的使用，还要控制 CPU、硬盘和网络等资源的使用。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660183236560-05b20dd1-882c-4d74-91bd-3ae1af0990f8.jpeg#clientId=uef28c7f2-fad3-4&from=paste&id=ua4f70563&originHeight=302&originWidth=948&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u001e2239-1a76-4127-ba7b-0b5b626483f&title=)<br />在上面的实例中，使用一个计数器来统计进程组对内存资源的使用情况，每个 控制组 都需要一个这样的计数器来统计和限制进程组对内存资源的使用。<br />在 Linux 内核中也有类似的 “计数器“，使用 cgroup_subsys_state 结构来表示（称它为 资源统计对象），其定义如下：
```c
struct cgroup_subsys_state {
    struct cgroup *cgroup; // 指向控制组对象
    atomic_t refcnt;       // 引用计数器
    unsigned long flags;   // 标志位
};
```
cgroup_subsys_state 结构看起来非常简单，这只是表面现象。内核为了将所有的 资源统计对象 抽象化（也就是都能用 cgroup_subsys_state 指针来指向所有类型的 资源统计对象），才定义出这个通用的部分，实际上的 资源统计对象 是比较复杂的。<br />例如内存的 资源统计对象 定义如下：
```c
struct mem_cgroup {
    // 资源统计对象通用部分
    struct cgroup_subsys_state css;
    
    // 资源统计对象私有部分
    struct res_counter res;  // 用于统计进程组的内存使用情况
    struct mem_cgroup_lru_info info;
    int prev_priority;
    struct mem_cgroup_stat stat;
};
```
mem_cgroup 结构与 cgroup_subsys_state 结构的关系如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660183236698-438375a0-cc6e-4a28-8c3f-6bd1387d2e02.jpeg#clientId=uef28c7f2-fad3-4&from=paste&id=u87592afa&originHeight=996&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u88551dc7-0575-4ce1-8b19-038fd129fc3&title=)<br />资源统计对象 必须与 控制组 绑定，才能实现限制 控制组 对资源的使用。前面了解到 cgroup 结构中有个名为 subsys 的字段，如下代码所示：
```c
struct cgroup {
    ...
    // 当前控制组关联的资源统计对象
    struct cgroup_subsys_state *subsys[CGROUP_SUBSYS_COUNT]; 
    ...
};
```
可以看出，subsys 字段是一个 cgroup_subsys_state 结构的数组，数组的大小为系统支持的 资源控制子系统 数（也就是说，数组上的每个槽位对应着一个子系统资统计对象）。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660183236724-c40ffc53-8510-4b03-a134-3e78791551aa.jpeg#clientId=uef28c7f2-fad3-4&from=paste&id=u4421ca4b&originHeight=705&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaa87b69f-ecf5-4d6a-9868-d6e3fa8d24f&title=)<br />在 Linux 内核中，一个进程可以属于多个 控制组，而每个 控制组 又关联着一个或多个 资源统计对象。所以，一个进程所关联的 资源统计对象 是其所在 控制组 关联的 资源统计对象 的集合。这句话有点难懂，用一幅图来说明：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660183237005-ebec6efa-1984-4f31-91a3-e1e3d60bf514.png#clientId=uef28c7f2-fad3-4&from=paste&id=u8578a7fe&originHeight=979&originWidth=961&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uff692ec2-c837-4680-b669-7ba78fa31b8&title=)<br />如上图所示：

- **进程A 属于控制组 /sys/fs/cgroup/memory/cgrp1/cgrp3 和控制组 /sys/fs/cgroup/cpu/cgrp2/cgrp3，所以 进程A 就关联了 mem_group A 和 task_group A 这两个资源统计对象。**
- **进程B 属于控制组 /sys/fs/cgroup/memory/cgrp1/cgrp4 和控制组 /sys/fs/cgroup/cpu/cgrp2/cgrp3，所以 进程B 就关联了 mem_group B 和 task_group A 这两个资源统计对象。**

进程通过 css_set 结构来收集不同控制组的 资源统计对象，其定义如下：
```c
struct css_set {
    ...
    // 用于收集不同控制组的资源统计对象
    struct cgroup_subsys_state *subsys[CGROUP_SUBSYS_COUNT];
};
```
在 进程描述符结构（task_struct） 中有个指向 css_set 结构的指针，如下所示：
```cpp
struct task_struct {
    ...
    struct css_set *cgroups;
    ...
};
```
所以，当把一个进程添加到一个 控制组 时，将会把 控制组 关联的 资源统计对象 添加到进程的 cgroups 字段中，从而使进程受到这些 资源统计对象 的限制，结合图10就比较容易理解了。<br />另外，资源子系统必须关联到某个层级才能起到限制 控制组 使用的目的。每种资源子系统都由一个名为 cgroup_subsys 的结构来描述，其定义如下：
```c
struct cgroup_subsys {
    struct cgroup_subsys_state *(*create)(struct cgroup_subsys *ss, 
                                          struct cgroup *cgrp);
    ...
    void (*attach)(struct cgroup_subsys *ss, struct cgroup *cgrp,
                   struct cgroup *old_cgrp, struct task_struct *tsk);
    void (*fork)(struct cgroup_subsys *ss, struct task_struct *task);
    void (*exit)(struct cgroup_subsys *ss, struct task_struct *task);
    ...
    int subsys_id;
    int active;
    int disabled;
    int early_init;
    const char *name;            // 子系统名字
    struct cgroupfs_root *root;  // 关联的层级根节点
    struct list_head sibling;
    void *private;
};
```
从 cgroup_subsys 结构的定义可以看出，其主要定义了一些方法和关联的层级。比如：create 方法主要用于当新建一个 控制组 时，创建一个新的 资源统计对象 与其关联；而 root 字段指向关联的层级根节点。<br />如内存子系统的定义如下：
```c
// 定义在文件：./mm/memcontrol.c

struct cgroup_subsys mem_cgroup_subsys = {
    .name        = "memory",
    .subsys_id   = mem_cgroup_subsys_id,
    .create      = mem_cgroup_create,
    .pre_destroy = mem_cgroup_pre_destroy,
    .destroy     = mem_cgroup_destroy,
    .populate    = mem_cgroup_populate,
    .attach      = mem_cgroup_move_task,
    .early_init  = 0,
};
```
<a name="AJoPZ"></a>
## 总结
主要分析了 cgroup 的设计与源码实现，不过聪明的读者可能发现本文并没有分析 cgroup 的逻辑代码。<br />是的，本文并没有分析具体的逻辑代码实现。不过按照本文的设计分析，相信读者能够很容易看到 cgroup 的逻辑代码实现，有兴趣的读者可以自行阅读源代码。
