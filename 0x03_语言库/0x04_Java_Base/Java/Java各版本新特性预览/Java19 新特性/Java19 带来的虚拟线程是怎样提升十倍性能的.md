Java<br />都知道一个 Java 服务其实是一个进程，当这个服务遇到高并发场景的时候往往会考虑使用线程池来提高性能，这里来看一下 Java19 中提到的虚拟线程 virtual threads。
<a name="GszLS"></a>
## 基本概念
都知道 Java 中的线程跟操作系统的内核线程是一对一的，Java 线程的调度其实是依赖操作系统的内核线程的，这就导致了线程切换和运行就需要进行上下文切换以及消耗大量的系统资源，同时也知道机器的资源是昂贵的并且也是有限的，不能也无法肆无忌惮的创建线程，因此线程往往会成为系统的瓶颈。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1665819954391-f1713dac-b4d3-4a81-9b73-5e7c64ba2c16.jpeg#clientId=u4490c175-6f9b-4&from=paste&id=ubef6ef6c&originHeight=253&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua4a00c1d-be78-4000-a11c-1d1d7295390&title=)<br />为了解决这个问题，Java19 中提出了一种虚拟线程的概念，为了区别，之前的线程被称为平台线程。要注意虚拟线程并不是用来直接取代平台线程的，虚拟线程是建议在平台线程之上的，一个平台线程可以对应多个虚拟线程，同时一个平台线程还是一一对应内核线程，因此上面的架构就变成了如下，一个 VT 代表一个虚拟线程。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1665819954435-bd5fe0fa-a3f0-4ef5-9f4f-0fbed9718001.jpeg#clientId=u4490c175-6f9b-4&from=paste&id=u37974701&originHeight=406&originWidth=962&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u595b2bd2-1f46-43f0-b21b-e6fca974408&title=)<br />如果有小伙伴对 GO 语言比较熟悉的话，就会想到 Java 中的虚拟线程跟 GO 中的 Goroutines 是很类似的，确实是这样，所以说语言都是相通的。
<a name="BGuuG"></a>
## 举个栗子
这里通过分别使用平台线程以及虚拟线程来测试一个 case 看看两者的耗时和性能是怎样的，测试分如下几步，依次来看一下。**注意下面的测试代码都是在 Java19 的版本中运行的**。
<a name="rJB8h"></a>
### 平台线程方式
通过 JDK 自带的线程池 `Executors.newCachedThreadPool()` 来创建线程池，并执行一定数据任务，任务的数量通过入参来控制，方便后续通过主函数调用。
```java
public static void platformThread(int size) {
    long l = System.currentTimeMillis();
    try(var executor = Executors.newCachedThreadPool()) {
        IntStream.range(0, size).forEach(i -> {
            executor.submit(() -> {
                Thread.sleep(Duration.ofSeconds(1));
                //System.out.println(i);
                return i;
            });
        });
    }
    System.out.printf("elapsed time: %dms\n", System.currentTimeMillis() - l);
}
```
<a name="a7BDw"></a>
### 虚拟线程的方式
虚拟线程的代码跟上面的代码十分相似，代码如下。可以看到，在代码层面上跟上面唯一的区别就是 `Executors.newCachedThreadPool()`  这一行变成了 `Executors.newVirtualThreadPerTaskExecutor()` 即代表创建的虚拟线程。
```java
public static void virThread(int size) {
    long l = System.currentTimeMillis();
    try(var executor = Executors.newVirtualThreadPerTaskExecutor()) {
        IntStream.range(0, size).forEach(i -> {
            executor.submit(() -> {
                Thread.sleep(Duration.ofSeconds(1));
                //System.out.println(i);
                return i;
            });
        });
    }
    System.out.printf("elapsed time: %dms\n", System.currentTimeMillis() - l);
}
```
<a name="jXGhp"></a>
### 监控运行的线程
上面的两个方法都是都是创建线程池用来提交任务的，但是位于具体创建了多少个线程是不知道的，所以还需要通过下面的代码来监控。
```java
public static void main(String[] args) {
    ScheduledExecutorService scheduledExecutorService = Executors.newScheduledThreadPool(1);
    scheduledExecutorService.scheduleAtFixedRate(() -> {
        ThreadMXBean threadBean = ManagementFactory.getThreadMXBean();
        ThreadInfo[] threadInfo = threadBean.dumpAllThreads(false, false);
        long count = Arrays.stream(threadInfo).count();
        System.out.println(count + " os thread");
    }, 1, 1, TimeUnit.SECONDS);

    int size = 100000;
    //    platformThread(size);
    virThread(size);
}
```
通过另一个线程池开启一个线程信息监控的线程，每秒钟输出一次当前的运行线程数。这里注意，如果上面的代码在 IDEA 中提示报错，找不到类，如下所示，可以将鼠标放上去进行修复。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1665819954556-d398458f-3eed-4db3-8d75-55b6c106eb5f.jpeg#clientId=u4490c175-6f9b-4&from=paste&id=u9d691c6a&originHeight=289&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u83e98193-218d-4609-b93e-8dd00caa363&title=)<br />也可以手动在设置中的编译器》Java 编译器这里给自己的模块增加一个编译参数 `-parameters --add-modules java.management --enable-preview` 。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1665819954417-2e2bbb43-2d1a-4cb3-ae50-d59376dba3d7.jpeg#clientId=u4490c175-6f9b-4&from=paste&id=u035f65f7&originHeight=701&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u12408690-e365-4a1b-9b59-22d2a5dca81&title=)
<a name="XfhvQ"></a>
#### 运行
上面的三段组合在一起就是一个完整的 case，如果这个时候如果上面的代码都正常，在运行的时候不出意外会出现下面的错误，<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1665819954415-7211d87a-623e-47f6-91c9-2d5ff2aa355c.jpeg#clientId=u4490c175-6f9b-4&from=paste&id=u8539f509&originHeight=420&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u10bf5692-5e33-48f5-b90d-5933f0fcda6&title=)<br />这里是因为当前 Java19 中的虚拟线程特性还处于预览阶段，不能直接使用，需要在启动参数上面配置 `--enable-preview` 参数，才能正常测试，如下所示，不同版本的 IDEA 可能显示的位置不一样，但是都是配置 VM 参数，找一下就好了。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1665819954702-611bd7b6-27c5-4a95-a2df-103e9093bfd7.jpeg#clientId=u4490c175-6f9b-4&from=paste&id=uedbbf21a&originHeight=664&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub1673be7-dfb1-4fa0-8bec-3c1570445f4&title=)<br />配置好了过后再次运行就可以得到如下的结果，可以看到在 size 大小为 100000 的情况下，虚拟线程只创建了 12 个平台线程，并且只在 2523 ms 就完成了整个任务。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1665819954738-beb61cf7-b45c-4423-a70f-a4b1fd6a6579.jpeg#clientId=u4490c175-6f9b-4&from=paste&id=u150257df&originHeight=605&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5d3ffdda-c40a-4265-b558-64744e1db02&title=)<br />但是当运行平台线程的方法的时候会发现，同样的 size 的情况下，平台线程创建了好几千个，而且还会触发 OOM，因为操作系统的资源已经被耗尽了，由此可见虚拟线程的性能要远远高于平台线程。YYDS！<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1665819954805-5a84d8ea-754d-49ad-8b26-9526095ff8ce.jpeg#clientId=u4490c175-6f9b-4&from=paste&id=u832d4c7b&originHeight=676&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u65b4e900-e112-45c1-b865-864e4d43270&title=)

- [ ] 为了避免 OOM 也可以将代码中的  `Executors.newCachedThreadPool()` 方法，改成 `Executors.newFixedThreadPool(xxx)`，这样虽然可以避免大量创建线程导致 OOM，但是任务执行的时长就会消耗更长，这边测试在 size 为 10000 的情况下，配置 500 个线程的时候，总共花费了 20276 ms，在数据量小十倍的情况下耗时却增长十倍。性能可想而知，感兴趣的小伙伴可以自己尝试一下。

![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1665819954874-ecc9a588-f9e5-4508-8057-71631842d6c7.jpeg#clientId=u4490c175-6f9b-4&from=paste&id=ud5a1b830&originHeight=771&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8aded17f-75c0-4858-81cb-86143b7a0f1&title=)
