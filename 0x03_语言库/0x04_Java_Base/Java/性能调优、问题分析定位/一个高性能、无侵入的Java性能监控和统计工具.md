Java
<a name="r5qzQ"></a>
## 背景
随着所在公司的发展，应用服务的规模不断扩大，原有的垂直应用架构已无法满足产品的发展，几十个工程师在一个项目里并行开发不同的功能，开发效率不断降低。<br />于是公司开始全面推进服务化进程，把团队内的大部分工程师主要精力全部都集中到服务化中。服务化可以让每个工程师仅在自己负责的子项目中进行开发，提高了开发的效率，但是服务化同时也带来了其他问题：

- 无法知道每个服务的运行情况，例如，某一台服务它目前的 QPS 是多少？它的平均延迟是多少，99% 的延迟是多少，99.9% 的延迟又是多少？
- 某一个接口响应时间慢，如何定位是哪个方法引起的？
- 每个服务的负载是否均衡？
- 当服务出现抖动时，如何判断是 DB、Cache 还是下游服务引起的？
- DB 和 Cache 响应延迟是多少？
- 如何评估服务的容量，随着服务的调用量越来越大，这个服务需要多少机器来支撑？什么时候应该加机器？

针对以上开发中的烦恼，介绍一个针对高并发、低延迟应用设计的高性能 Java 性能监控和统计工具——**MyPerf4J**。
<a name="VzGER"></a>
## 使用场景

- 在开发环境中快速定位 Java 应用程序的性能瓶颈
- 在生产环境中长期监控 Java 应用程序的性能指标
<a name="z4npO"></a>
## 特性

- **高性能：**单线程支持每秒 1000万次 响应时间的记录，每次记录只花费 73纳秒
- **无侵入：**采用 JavaAgent 方式，对应用程序完全无侵入，无需修改应用代码
- **低内存：**采用内存复用的方式，整个生命周期只产生极少的临时对象，不影响应用程序的GC
- **高精度：**采用纳秒来计算响应时间
- **高实时：**支持秒级监控，最低 1 秒!
<a name="nSTQS"></a>
## 监控指标
MyPerf4J 为每个应用收集数十个监控指标，所有的监控指标都是实时采集和展现的。<br />下面是 MyPerf4J 目前支持的监控指标列表：
<a name="AsAS0"></a>
### Method
RPS，Count，Avg，Min，Max，StdDev，TP50, TP90, TP95, TP99, TP999, TP9999, TP99999, TP100<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1671272330230-7bc65112-8e58-4abe-a017-69fe8114fc8f.gif#averageHue=%2317181a&clientId=u25e9797b-d00c-4&from=paste&id=uec6cc517&originHeight=569&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u63b19a0e-5a74-44b0-90dd-4c9cb868f9c&title=)
<a name="P9xyZ"></a>
### Memory
HeapInit，HeapUsed，HeapCommitted，HeapMax，NonHeapInit，NonHeapUsed，NonHeapCommitted，NonHeapMax<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1671272330342-8446a7f8-a24d-4635-92b9-6cd248327656.gif#averageHue=%2318191b&clientId=u25e9797b-d00c-4&from=paste&id=u3b60ae07&originHeight=569&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf2cf2d0f-d165-48da-9cd3-e9879d63fde&title=)
<a name="E1A6R"></a>
### JVM GC
CollectCount，CollectTime<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1671272330312-5a81101e-0879-4744-82bc-89ac3f1f9bb9.gif#averageHue=%2317181a&clientId=u25e9797b-d00c-4&from=paste&id=u67f4e124&originHeight=569&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u43531462-a7d9-4d3b-a679-be4c9deed11&title=)
<a name="qJjqx"></a>
### JVM Class
Total，Loaded，Unloaded<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1671272330253-5d81caf6-cd8d-462a-9321-a809ba18da19.gif#averageHue=%2317181a&clientId=u25e9797b-d00c-4&from=paste&id=u74b9573b&originHeight=569&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue2090c87-b17e-4df9-9cae-54324d190d6&title=)
<a name="HMtNw"></a>
## 快速启动
MyPerf4J 采用 JavaAgent 配置方式，透明化接入应用，对应用代码完全没有侵入。
<a name="G5iCH"></a>
### 下载

- 下载并解压 MyPerf4J-ASM.zip
- 阅读解压出的 README 文件
- 修改解压出的 MyPerf4J.properties 配置文件中 app_name、metrics.log.xxx 和 filter.packages.include 的配置值
- **MyPerf4J-ASM.zip包**：[https://github.com/LinShunKang/Objects/blob/master/zips/CN/MyPerf4J-ASM-3.3.0-SNAPSHOT.zip?raw=true](https://github.com/LinShunKang/Objects/blob/master/zips/CN/MyPerf4J-ASM-3.3.0-SNAPSHOT.zip?raw=true)
<a name="PM4rA"></a>
### 配置
在 JVM 启动参数里加上以下两个参数
```bash
-javaagent:/path/to/MyPerf4J-ASM.jar
-DMyPerf4JPropFile=/path/to/MyPerf4J.properties
```
![2022-12-17-18-17-53.411750900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1671272444638-37093c62-32ca-4296-8601-9d32391741c0.png#averageHue=%23f5f5f4&clientId=u25e9797b-d00c-4&from=ui&id=u8de8bf68&originHeight=274&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=889549&status=done&style=none&taskId=u4f8fa7b9-7e96-416f-a634-829036ade6c&title=)
<a name="ZnfDH"></a>
### 运行
启动应用，监控日志输出到 /path/to/log/method_metrics.log：
```
MyPerf4J Method Metrics [2020-01-01 12:49:57, 2020-01-01 12:49:58]
Method[6]                            Type        Level  TimePercent      RPS  Avg(ms)  Min(ms)  Max(ms)    StdDev    Count     TP50     TP90     TP95     TP99    TP999   TP9999
DemoServiceImpl.getId2(long)      General      Service      322.50%     6524     0.49        0        1     0.50      6524        0        1        1        1        1        1
DemoServiceImpl.getId3(long)      General      Service      296.10%     4350     0.68        0        1     0.47      4350        1        1        1        1        1        1
DemoServiceImpl.getId4(long)      General      Service      164.60%     2176     0.76        0        1     0.43      2176        1        1        1        1        1        1
DemoServiceImpl.getId1(long)      General      Service        0.00%     8704     0.00        0        0     0.00      8704        0        0        0        0        0        0
DemoDAO.getId1(long)         DynamicProxy          DAO        0.00%     2176     0.00        0        0     0.00      2176        0        0        0        0        0        0
DemoDAO.getId2()             DynamicProxy          DAO        0.00%     2176     0.00        0        0     0.00      2176        0        0        0        0        0        0
```
<a name="UJqoU"></a>
### 卸载
在 JVM 启动参数中去掉以下两个参数，重启即可卸载此工具。
```bash
-javaagent:/path/to/MyPerf4J-ASM.jar
-DMyPerf4JPropFile=/path/to/MyPerf4J.properties
```
<a name="BnVTL"></a>
## 开源地址

- [https://github.com/LinShunKang/MyPerf4J](https://github.com/LinShunKang/MyPerf4J)
