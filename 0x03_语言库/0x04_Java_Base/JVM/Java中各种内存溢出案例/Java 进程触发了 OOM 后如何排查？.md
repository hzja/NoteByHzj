Java<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662553357062-7e54c95b-a7fc-4cb9-b2b7-f7a873cb5e64.png#averageHue=%23f2f3f6&clientId=u137816ab-8ac7-4&from=paste&id=ub0627f4f&originHeight=666&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udbd87bbe-d613-4ff3-9201-ab25a81a2d0&title=)
<a name="LkVbx"></a>
## 为什么会 OOM
OOM 全称 “Out Of Memory”，表示内存耗尽。当 JVM 因为没有足够的内存来为对象分配空间，并且垃圾回收器也已经没有空间可回收时，就会抛出这个错误。<br />为什么会出现 OOM，一般由这些问题引起：

1. 分配过少：JVM 初始化内存小，业务使用了大量内存；或者不同 JVM 区域分配内存不合理。
2. 代码漏洞：某一个对象被频繁申请，不用了之后却没有被释放，导致内存耗尽。

**内存泄漏**：申请使用完的内存没有释放，导致虚拟机不能再次使用该内存，此时这段内存就泄漏了。因为申请者不用了，而又不能被虚拟机分配给别人用。<br />**内存溢出**：申请的内存超出了 JVM 能提供的内存大小，此时称之为溢出。<br />内存泄漏持续存在，最后一定会溢出，两者是因果关系。<br />简言之，OOM 意味着程序存在着漏洞，可能是代码或者 JVM 参数配置引起的。
<a name="gbjSq"></a>
## 常见的 OOM
比较常见的 OOM 类型有以下几种：
<a name="CyF3h"></a>
### 1、java.lang.OutOfMemoryError: PermGen space
Java7 永久代（方法区）溢出，它用于存储已被虚拟机加载的类信息、常量、静态变量、即时编译器编译后的代码等数据。每当一个类初次加载的时候，元数据都会存放到永久代。<br />一般出现于大量 Class 对象或者 JSP 页面，或者采用 CgLib 动态代理技术导致。<br />可以通过 `-XX:PermSize` 和 `-XX:MaxPermSize` 修改方法区大小。<br />Java8 将永久代变更为元空间，报错：java.lang.OutOfMemoryError: Metadata space，元空间内存不足默认进行动态扩展。
<a name="bCpPV"></a>
### 2、java.lang.StackOverflowError
**虚拟机栈溢出**，一般是由于程序中存在**死循环或者深度递归调用**造成的。如果栈大小设置过小也会出现溢出，可以通过 `-Xss` 设置栈的大小。<br />虚拟机抛出栈溢出错误，可以在日志中定位到错误的类、方法。
<a name="FMU9I"></a>
### 3、java.lang.OutOfMemoryError: Java heap space
**Java 堆内存溢出**，溢出的原因一般由于 JVM 堆内存设置不合理或者内存泄漏导致。<br />如果是内存泄漏，可以通过工具查看泄漏对象到 GC Roots 的引用链。掌握了泄漏对象的类型信息以及 GC Roots 引用链信息，就可以精准地定位出泄漏代码的位置。<br />如果不存在内存泄漏，就是内存中的对象确实都还必须存活着，那就应该检查虚拟机的堆参数（-Xmx 与 -Xms），查看是否可以将虚拟机的内存调大些。<br />本文不对方法区和虚拟机栈的溢出场景进行过多讨论，下面主要讲解常见的 Java 堆空间的 OOM 排查思路。
<a name="h64wO"></a>
## 查看 JVM 内存分布
假设 Java 应用 PID 为 15162，输入命令查看 JVM 内存分布 `jmap -heap 15162`。
```bash
[xxx@xxx ~]# jmap -heap 15162
Attaching to process ID 15162, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.161-b12

using thread-local object allocation.
Mark Sweep Compact GC

Heap Configuration:
   MinHeapFreeRatio         = 40 # 最小堆使用比例
   MaxHeapFreeRatio         = 70 # 最大堆可用比例
   MaxHeapSize              = 482344960 (460.0MB) # 最大堆空间大小
   NewSize                  = 10485760 (10.0MB) # 新生代分配大小
   MaxNewSize               = 160759808 (153.3125MB) # 最大新生代可分配大小
   OldSize                  = 20971520 (20.0MB) # 老年代大小
   NewRatio                 = 2 # 新生代比例
   SurvivorRatio            = 8 # 新生代与 Survivor 比例
   MetaspaceSize            = 21807104 (20.796875MB) # 元空间大小
   CompressedClassSpaceSize = 1073741824 (1024.0MB) # Compressed Class Space 空间大小限制
   MaxMetaspaceSize         = 17592186044415 MB # 最大元空间大小
   G1HeapRegionSize         = 0 (0.0MB) # G1 单个 Region 大小

Heap Usage:  # 堆使用情况
New Generation (Eden + 1 Survivor Space): # 新生代
   capacity = 9502720 (9.0625MB) # 新生代总容量
   used     = 4995320 (4.763908386230469MB) # 新生代已使用
   free     = 4507400 (4.298591613769531MB) # 新生代剩余容量
   52.56726495150862% used # 新生代使用占比
Eden Space:  
   capacity = 8454144 (8.0625MB) # Eden 区总容量
   used     = 4029752 (3.8430709838867188MB) # Eden 区已使用
   free     = 4424392 (4.219429016113281MB) # Eden 区剩余容量
   47.665996699370154% used  # Eden 区使用占比
From Space: # 其中一个 Survivor 区的内存分布
   capacity = 1048576 (1.0MB)
   used     = 965568 (0.92083740234375MB)
   free     = 83008 (0.07916259765625MB)
   92.083740234375% used
To Space: # 另一个 Survivor 区的内存分布
   capacity = 1048576 (1.0MB)
   used     = 0 (0.0MB)
   free     = 1048576 (1.0MB)
   0.0% used
tenured generation: # 老年代
   capacity = 20971520 (20.0MB)
   used     = 10611384 (10.119804382324219MB)
   free     = 10360136 (9.880195617675781MB)
   50.599021911621094% used

10730 interned Strings occupying 906232 bytes.
```
通过查看 JVM 内存分配以及运行时使用情况，可以判断内存分配是否合理。<br />另外，可以在 JVM 运行时查看最耗费资源的对象，`jmap -histo:live 15162 | more`。<br />JVM 内存对象列表按照对象所占内存大小排序。

- instances：实例数
- bytes：单位 byte
- class name：类名

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662553358017-63b9b4de-d0bc-4537-91d5-49574d37efc5.png#averageHue=%23efeeee&clientId=u137816ab-8ac7-4&from=paste&id=u62590d8c&originHeight=436&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u042c0252-6e4b-49fa-8f41-8a9d8f19ab5&title=)<br />明显看到 CustomObjTest 对象实例以及占用内存过多。<br />可惜的是，方案存在局限性，因为它只能排查对象占用内存过高问题。<br />其中 "[" 代表数组，例如 "[C" 代表 Char 数组，"[B" 代表 Byte 数组。如果数组内存占用过多，不知道哪些对象持有它，所以就需要 Dump 内存进行离线分析。<br />`jmap -histo:live` 执行此命令，JVM 会先触发 GC，再统计信息。
<a name="AjEJJ"></a>
## Dump 文件分析
Dump 文件是 Java 进程的内存镜像，其中主要包括**系统信息**、**虚拟机属性**、**完整的线程 Dump**、**所有类和对象的状态**等信息。<br />当程序发生内存溢出或 GC 异常情况时，怀疑 JVM 发生了**内存泄漏**，这时就可以导出 Dump 文件分析。<br />JVM 启动参数配置添加以下参数：

- `-XX:+HeapDumpOnOutOfMemoryError`
- `-XX:HeapDumpPath=./`（参数为 Dump 文件生成路径）

当 JVM 发生 OOM 异常自动导出 Dump 文件，文件名称默认格式：java_pid{pid}.hprof。<br />上面配置是在应用抛出 OOM 后自动导出 Dump，或者可以在 JVM 运行时导出 Dump 文件。
```bash
jmap -dump:file=[文件路径] [pid]

# 示例
jmap -dump:file=./jvmdump.hprof 15162
```
在本地写一个测试代码，验证下 OOM 以及分析 Dump 文件。<br />设置 VM 参数：`-Xms3m -Xmx3m -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=./`
```java
public static void main(String[] args) {
    List<Object> oomList = Lists.newArrayList();
   // 无限循环创建对象
    while (true) {
        oomList.add(new Object());
    }
}
```
通过报错信息得知，`java heap space` 表示 OOM 发生在堆区，并生成了 hprof 二进制文件在当前文件夹下。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662553356961-e9e3c67d-c487-47fb-95c8-41ffc48ef160.png#averageHue=%233a3333&clientId=u137816ab-8ac7-4&from=paste&id=ua2d6a317&originHeight=266&originWidth=659&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue41a4c0b-247b-4aa6-9816-f0a71b423f0&title=)
<a name="TAYXi"></a>
### JvisualVM 分析
Dump 分析工具有很多，相对而言，**JvisualVM**、**JProfiler**、**Eclipse Mat**，使用人群更多一些。下面以 JvisualVM 举例分析 Dump 文件。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662553356911-3c5b4071-9427-4614-81b0-87ed33d9ea35.png#averageHue=%23f0f0f0&clientId=u137816ab-8ac7-4&from=paste&id=u5c57d8b2&originHeight=652&originWidth=1068&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u12175513-00a6-445e-9b4f-5c37d5694ca&title=)<br />列举两个常用的功能。<br />第一个是能看到触发 OOM 的线程堆栈，清晰得知程序溢出的原因。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662553357058-e8b9731d-8588-43f2-bf2b-e24dafb79344.png#averageHue=%23ececec&clientId=u137816ab-8ac7-4&from=paste&id=u44474cd9&originHeight=307&originWidth=616&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u74405bd0-d0bf-4b88-b0fc-f90fdb123b2&title=)<br />第二个是可以查看 JVM 内存里保留大小最大的对象，可以自由选择排查个数。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662553357450-ce008b41-94f7-45fb-9ca3-ada629850f77.png#averageHue=%23f1f1f1&clientId=u137816ab-8ac7-4&from=paste&id=ua00e0bed&originHeight=250&originWidth=373&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u783c8c36-40f2-489f-99af-d5321b7665c&title=)<br />点击对象还可以跳转具体的对象引用详情页面。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662553357464-ecf44777-b567-4c9f-b44e-aa9576b85f18.png#averageHue=%23efefef&clientId=u137816ab-8ac7-4&from=paste&id=u94cae519&originHeight=601&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4c0de6b7-22a9-4bf3-83cd-7b2c9787732&title=)<br />文中 Dump 文件较为简单，而正式环境出错的原因五花八门，所以不对该 Dump 文件做深度解析。<br />**注意**：JvisualVM 如果分析大 Dump 文件，可能会因为内存不足打不开，需要调整默认的内存。
<a name="DpSQ9"></a>
## 总结回顾
线上如遇到 JVM 内存溢出，可以分以下几步排查：

1. `jmap -heap` 查看是否内存分配过小；
2. `jmap -histo` 查看是否有明显的对象分配过多且没有释放情况；
3. `jmap -dump` 导出 JVM 当前内存快照，使用 JDK 自带或 MAT 等工具分析快照。

如果上面还不能定位问题，那么需要排查应用是否在不断创建资源，比如网络连接或者线程，都可能会导致系统资源耗尽。
