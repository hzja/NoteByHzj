JavaSpringBoot<br />为了更好地实现对项目的管理，将一个项目迁移到MDP框架（基于Spring Boot），随后就发现系统会频繁报出Swap区域使用量过高的异常。查看原因，发现配置了4G堆内内存，但是实际使用的物理内存竟然高达7G，确实不正常。<br />JVM参数配置是：
```bash
-XX:MetaspaceSize=256M -XX:MaxMetaspaceSize=256M -XX:+AlwaysPreTouch -XX:ReservedCodeCacheSize=128m -XX:InitialCodeCacheSize=128m, -Xss512k -Xmx4g -Xms4g,-XX:+UseG1GC -XX:G1HeapRegionSize=4M
```
实际使用的物理内存如下图所示：<br />![top命令显示的内存情况](https://cdn.nlark.com/yuque/0/2023/png/396745/1699536557197-a739ca6f-13c5-40e5-9830-ba6ec97a950d.png#averageHue=%23105c19&clientId=u7029499a-d61d-4&from=paste&id=u25405251&originHeight=351&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u40a1a270-0b40-4525-b775-2ce4008fc17&title=top%E5%91%BD%E4%BB%A4%E6%98%BE%E7%A4%BA%E7%9A%84%E5%86%85%E5%AD%98%E6%83%85%E5%86%B5 "top命令显示的内存情况")
<a name="f72rC"></a>
## 排查过程
<a name="VuiSq"></a>
### 1、使用Java层面的工具定位内存区域（堆内内存、Code区域或者使用`unsafe.allocateMemory`和`DirectByteBuffer`申请的堆外内存）
在项目中添加`-XX:NativeMemoryTracking=detail` JVM参数重启项目，使用命令`jcmd pid VM.native_memory detail`查看到的内存分布如下：<br />![jcmd显示的内存情况](https://cdn.nlark.com/yuque/0/2023/png/396745/1699536557264-2328846d-7ca4-4010-bb8b-caa028ee1b38.png#averageHue=%23000000&clientId=u7029499a-d61d-4&from=paste&id=NpNwc&originHeight=1114&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uc0057f75-dd16-4de0-bb86-d9b5ab1b9a3&title=jcmd%E6%98%BE%E7%A4%BA%E7%9A%84%E5%86%85%E5%AD%98%E6%83%85%E5%86%B5 "jcmd显示的内存情况")<br />发现命令显示的committed的内存小于物理内存，因为jcmd命令显示的内存包含堆内内存、Code区域、通过`unsafe.allocateMemory`和`DirectByteBuffer`申请的内存，但是不包含其他Native Code（C代码）申请的堆外内存。所以猜测是使用Native Code申请内存所导致的问题。<br />为了防止误判，使用了pmap查看内存分布，发现大量的64M的地址；而这些地址空间不在jcmd命令所给出的地址空间里面，基本上就断定就是这些64M的内存所导致。
```c
pmap -x 15573 | sort -k 3 -n -r
```
![pmap显示的内存情况](https://cdn.nlark.com/yuque/0/2023/png/396745/1699536557227-c558f122-cdd4-4608-bb68-42008d1221dc.png#averageHue=%23070100&clientId=u7029499a-d61d-4&from=paste&id=u422bd53d&originHeight=1042&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u5039448d-643c-4172-a54d-21c7fe22840&title=pmap%E6%98%BE%E7%A4%BA%E7%9A%84%E5%86%85%E5%AD%98%E6%83%85%E5%86%B5 "pmap显示的内存情况")
<a name="Ie5FB"></a>
### 2、使用系统层面的工具定位堆外内存
因为已经基本上确定是Native Code所引起，而Java层面的工具不便于排查此类问题，只能使用系统层面的工具去定位问题。
<a name="PU11e"></a>
#### 首先，使用了gperftools去定位问题
gperftools的使用方法可以参考：[https://github.com/gperftools/gperftools](https://github.com/gperftools/gperftools)<br />**gperftools的监控如下：**<br />![gperftools监控](https://cdn.nlark.com/yuque/0/2023/png/396745/1699536557198-f75d7879-6905-4ebb-b690-5303b4af2d59.png#averageHue=%23000000&clientId=u7029499a-d61d-4&from=paste&id=ud9c0815a&originHeight=293&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u98bec9ec-1795-48ce-9e8d-4e1359c5660&title=gperftools%E7%9B%91%E6%8E%A7 "gperftools监控")<br />从上图可以看出：使用malloc申请的的内存最高到3G之后就释放了，之后始终维持在700M-800M。第一反应是：难道Native Code中没有使用malloc申请，直接使用mmap/brk申请的？（gperftools原理就使用动态链接的方式替换了操作系统默认的内存分配器（glibc）。）
<a name="L135q"></a>
#### 然后，使用strace去追踪系统调用
因为使用gperftools没有追踪到这些内存，于是直接使用命令“`strace -f -e"brk,mmap,munmap" -p pid`”追踪向OS申请内存请求，但是并没有发现有可疑内存申请。strace监控如下图所示:<br />![strace监控](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1699536557219-7f4e72ce-086e-46f1-891f-2a3f9da48aea.jpeg#averageHue=%230f4510&clientId=u7029499a-d61d-4&from=paste&id=u333c9e47&originHeight=290&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ue5e658e9-4cb1-4b8c-a646-bdc2a27663e&title=strace%E7%9B%91%E6%8E%A7 "strace监控")
<a name="A2SMa"></a>
#### 接着，使用GDB去dump可疑内存
因为使用strace没有追踪到可疑内存申请；于是想着看看内存中的情况。就是直接使用命令`gdp -pid pid`进入GDB之后，然后使用命令`dump memory mem.bin startAddress endAddress` dump内存，其中startAddress和endAddress可以从/proc/pid/smaps中查找。然后使用strings mem.bin查看dump的内容，如下：<br />![gperftools监控](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1699536557817-a661da21-6914-420a-a9ba-73797f284bb0.jpeg#averageHue=%23070b03&clientId=u7029499a-d61d-4&from=paste&id=ubcf4ff85&originHeight=325&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uf4cf2d62-1151-40ba-96bb-792006766ce&title=gperftools%E7%9B%91%E6%8E%A7 "gperftools监控")<br />从内容上来看，像是解压后的JAR包信息。读取JAR包信息应该是在项目启动的时候，那么在项目启动之后使用strace作用就不是很大了。所以应该在项目启动的时候使用strace，而不是启动完成之后。
<a name="VPK7S"></a>
#### 再次，项目启动时使用strace去追踪系统调用
项目启动使用strace追踪系统调用，发现确实申请了很多64M的内存空间，截图如下：<br />![strace监控](https://cdn.nlark.com/yuque/0/2023/png/396745/1699536557858-d4b3afe0-361d-4132-8a2d-5bc9e5203b32.png#averageHue=%231a0503&clientId=u7029499a-d61d-4&from=paste&id=uda44afcd&originHeight=116&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud8dbc1c5-7a3d-4300-bce4-bde674fa3ff&title=strace%E7%9B%91%E6%8E%A7 "strace监控")<br />使用该mmap申请的地址空间在pmap对应如下：<br />![strace申请内容对应的pmap地址空间](https://cdn.nlark.com/yuque/0/2023/png/396745/1699536558290-bdaaf056-808f-4003-b685-ecdea963c8a9.png#averageHue=%23070100&clientId=u7029499a-d61d-4&from=paste&id=u2cdfd66e&originHeight=582&originWidth=846&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uaefd5d15-17c5-4cc6-bc2c-a33826b5a65&title=strace%E7%94%B3%E8%AF%B7%E5%86%85%E5%AE%B9%E5%AF%B9%E5%BA%94%E7%9A%84pmap%E5%9C%B0%E5%9D%80%E7%A9%BA%E9%97%B4 "strace申请内容对应的pmap地址空间")
<a name="MpnPb"></a>
#### 最后，使用jstack去查看对应的线程
因为strace命令中已经显示申请内存的线程ID。直接使用命令`jstack pid`去查看线程栈，找到对应的线程栈（注意10进制和16进制转换）如下：<br />![strace申请空间的线程栈](https://cdn.nlark.com/yuque/0/2023/png/396745/1699536558557-560bd020-b8c1-45b4-8755-6200ec249257.png#averageHue=%23010000&clientId=u7029499a-d61d-4&from=paste&id=u025cf045&originHeight=396&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ueb8d526b-5597-491e-9f65-53861513bdb&title=strace%E7%94%B3%E8%AF%B7%E7%A9%BA%E9%97%B4%E7%9A%84%E7%BA%BF%E7%A8%8B%E6%A0%88 "strace申请空间的线程栈")<br />这里基本上就可以看出问题来了：MCC（美团统一配置中心）使用了Reflections进行扫包，底层使用了Spring Boot去加载JAR。因为解压JAR使用Inflater类，需要用到堆外内存，然后使用Btrace去追踪这个类，栈如下：<br />![btrace追踪栈](https://cdn.nlark.com/yuque/0/2023/png/396745/1699536558808-4f60fb27-8f60-4ffe-8cba-be8a1ac0c282.png#averageHue=%23020000&clientId=u7029499a-d61d-4&from=paste&id=ud433656b&originHeight=765&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ucbd684ab-4f58-4d12-a38f-4b0fd9ece26&title=btrace%E8%BF%BD%E8%B8%AA%E6%A0%88 "btrace追踪栈")<br />然后查看使用MCC的地方，发现没有配置扫包路径，默认是扫描所有的包。于是修改代码，配置扫包路径，发布上线后内存问题解决。
<a name="tsBGX"></a>
### 3、为什么堆外内存没有释放掉呢？
虽然问题已经解决了，但是有几个疑问：

- 为什么使用旧的框架没有问题？
- 为什么堆外内存没有释放？
- 为什么内存大小都是64M，JAR大小不可能这么大，而且都是一样大？
- 为什么gperftools最终显示使用的的内存大小是700M左右，解压包真的没有使用malloc申请内存吗？

带着疑问，直接看了一下Spring Boot Loader那一块的源码。发现Spring Boot对Java JDK的`InflaterInputStream`进行了包装并且使用了`Inflater`，而`Inflater`本身用于解压JAR包的需要用到堆外内存。<br />而包装之后的类`ZipInflaterInputStream`没有释放Inflater持有的堆外内存。以为找到了原因，立马向Spring Boot社区反馈了这个bug。但是反馈之后，就发现Inflater这个对象本身实现了`finalize`方法，在这个方法中有调用释放堆外内存的逻辑。也就是说Spring Boot依赖于GC释放堆外内存。<br />使用jmap查看堆内对象时，发现已经基本上没有`Inflater`这个对象了。于是就怀疑GC的时候，没有调用`finalize`。带着这样的怀疑，把Inflater进行包装在Spring Boot Loader里面替换成自己包装的`Inflater`，在`finalize`进行打点监控，结果finalize方法确实被调用了。于是又去看了Inflater对应的C代码，发现初始化的使用了`malloc`申请内存，end的时候也调用了free去释放内存。<br />此刻，只能怀疑free的时候没有真正释放内存，便把Spring Boot包装的`InflaterInputStream`替换成Java JDK自带的，发现替换之后，内存问题也得以解决了。<br />这时，再返过来看gperftools的内存分布情况，发现使用Spring Boot时，内存使用一直在增加，突然某个点内存使用下降了好多（使用量直接由3G降为700M左右）。这个点应该就是GC引起的，内存应该释放了，但是在操作系统层面并没有看到内存变化，那是不是没有释放到操作系统，被内存分配器持有了呢？<br />继续探究，发现系统默认的内存分配器（glibc 2.12版本）和使用gperftools内存地址分布差别很明显，2.5G地址使用smaps发现它是属于Native Stack。内存地址分布如下：<br />![gperftools显示的内存地址分布](https://cdn.nlark.com/yuque/0/2023/png/396745/1699536558997-3f8fca9c-1933-4425-a19a-e6e73c6320ee.png#averageHue=%23090101&clientId=u7029499a-d61d-4&from=paste&id=uba18dadb&originHeight=526&originWidth=888&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u6cc4a160-46c1-49d5-b33d-1f4d2b624e4&title=gperftools%E6%98%BE%E7%A4%BA%E7%9A%84%E5%86%85%E5%AD%98%E5%9C%B0%E5%9D%80%E5%88%86%E5%B8%83 "gperftools显示的内存地址分布")<br />到此，基本上可以确定是内存分配器在捣鬼；搜索了一下glibc 64M，发现glibc从2.11开始对每个线程引入内存池（64位机器大小就是64M内存），原文如下：<br />![glib内存池说明](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1699536558998-c8fdbcad-d3cb-489c-9238-3b5a1483a6d3.jpeg#averageHue=%23ebebeb&clientId=u7029499a-d61d-4&from=paste&id=u27c20624&originHeight=337&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ufddb8653-64a7-4657-be54-74da4d307dc&title=glib%E5%86%85%E5%AD%98%E6%B1%A0%E8%AF%B4%E6%98%8E "glib内存池说明")<br />按照文中所说去修改MALLOC_ARENA_MAX环境变量，发现没什么效果。查看tcmalloc（gperftools使用的内存分配器）也使用了内存池方式。<br />为了验证是内存池搞的鬼，就简单写个不带内存池的内存分配器。使用命令`gcc zjbmalloc.c -fPIC -shared -o zjbmalloc.so`生成动态库，然后使用`export LD_PRELOAD=zjbmalloc.so`替换掉glibc的内存分配器。<br />**其中代码Demo如下：**
```c
#include<sys/mman.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
//作者使用的64位机器，sizeof(size_t)也就是sizeof(long) 
void* malloc ( size_t size ){
    long* ptr = mmap( 0, size + sizeof(long), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );
    if (ptr == MAP_FAILED) {
        return NULL;
    }
    *ptr = size;                     // First 8 bytes contain length.
    return (void*)(&ptr[1]);        // Memory that is after length variable
}

void *calloc(size_t n, size_t size) {
    void* ptr = malloc(n * size);
    if (ptr == NULL) {
        return NULL;
    }
    memset(ptr, 0, n * size);
    return ptr;
}
void *realloc(void *ptr, size_t size){
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    if (ptr == NULL) {
        return malloc(size);
    }
    long *plen = (long*)ptr;
    plen--;                          // Reach top of memory
    long len = *plen;
    if (size <= len) {
        return ptr;
    }
    void* rptr = malloc(size);
    if (rptr == NULL) {
        free(ptr);
        return NULL;
    }
    rptr = memcpy(rptr, ptr, len);
    free(ptr);
    return rptr;
}

void free (void* ptr ){
    if (ptr == NULL) {
        return;
    }
    long *plen = (long*)ptr;
    plen--;                          // Reach top of memory
    long len = *plen;               // Read length
    munmap((void*)plen, len + sizeof(long));
}
```
通过在自定义分配器当中埋点可以发现其实程序启动之后应用实际申请的堆外内存始终在700M-800M之间，gperftools监控显示内存使用量也是在700M-800M左右。但是从操作系统角度来看进程占用的内存差别很大（这里只是监控堆外内存）。<br />做了一下测试，使用不同分配器进行不同程度的扫包，占用的内存如下：

| 内存分配器名称 | 没有扫全包 | 一次扫描全包 | 两次扫描全包 |
| --- | --- | --- | --- |
| glibc | 750M | 1.5G | 2.3G |
| tcmalloc | 900M | 2.0G | 2.77G |
| 自定义 | 1.7G | 1.7G | 1.7G |

内存测试对比
<a name="sPDqN"></a>
#### 为什么自定义的malloc申请800M，最终占用的物理内存在1.7G呢？
因为自定义内存分配器采用的是mmap分配内存，mmap分配内存按需向上取整到整数个页，所以存在着巨大的空间浪费。通过监控发现最终申请的页面数目在536k个左右，那实际上向系统申请的内存等于512k * 4k（pagesize） = 2G。为什么这个数据大于1.7G呢？<br />因为操作系统采取的是延迟分配的方式，通过mmap向系统申请内存的时候，系统仅仅返回内存地址并没有分配真实的物理内存。只有在真正使用的时候，系统产生一个缺页中断，然后再分配实际的物理Page。
<a name="P1vNW"></a>
## 总结
![流程图](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1699536559339-aae4fb5d-8b85-4ca3-be1b-f0ab5c7b765e.jpeg#averageHue=%23faf3ee&clientId=u7029499a-d61d-4&from=paste&id=u0c1e44aa&originHeight=264&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uc397c81e-009c-4356-831c-a820c7b11e4&title=%E6%B5%81%E7%A8%8B%E5%9B%BE "流程图")<br />整个内存分配的流程如上图所示。MCC扫包的默认配置是扫描所有的JAR包。在扫描包的时候，Spring Boot不会主动去释放堆外内存，导致在扫描阶段，堆外内存占用量一直持续飙升。当发生GC的时候，Spring Boot依赖于finalize机制去释放了堆外内存；但是glibc为了性能考虑，并没有真正把内存归返到操作系统，而是留下来放入内存池了，导致应用层以为发生了“内存泄漏”。<br />所以修改MCC的配置路径为特定的JAR包，问题解决。目前在Spring Boot的版本（2.0.5.RELEASE）已经做了修改，在`ZipInflaterInputStream`主动释放了堆外内存不再依赖GC；所以Spring Boot升级到最新版本，这个问题也可以得到解决。
