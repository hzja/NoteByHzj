JavaSpringBoot
<a name="ym58m"></a>
## 项目简介
OSHI 是一个基于 JNA（本地）的免费的操作系统和硬件信息库，专为 Java 设计。它可以跨平台的获取系统信息以及硬件信息，如操作系统版本、进程、内存和 CPU 使用情况、磁盘和分区、设备、传感器等。它不需要安装任何额外的本地库，因此接入起来很方便。
<a name="r0JOM"></a>
## 项目使用
<a name="erwlG"></a>
### Maven 依赖
通过配置 Maven 依赖来引入 OSHI，如下所示：
```xml
<dependency>
  <groupId>com.github.oshi</groupId>
  <artifactId>oshi-core</artifactId>
  <version>6.4.0</version>
</dependency>
```
这里注意的是，不同版本的 JDK 环境，需要引用不同版本的包：

- JDK8: oshi-core-6.4.0
- JPMS: oshi-core-java11-6.4.0
- JDK6: oshi-core-3.14.0
<a name="AootN"></a>
### 简单的例子
在这里写一个简单的例子，包括了一些常用硬件参数的获取，注释已经写的很详细了：
```java
public static void main(String[] args) {
    // 系统信息类
    SystemInfo systemInfo = new SystemInfo();
    // 获取硬件信息
    HardwareAbstractionLayer hardware = systemInfo.getHardware();

    // CPU信息
    CentralProcessor centralProcessor = hardware.getProcessor();
    // 获取CPU名称
    String name = centralProcessor.getProcessorIdentifier().getName();
    // 获取CPU核心数
    int physicalProcessorCount = centralProcessor.getPhysicalProcessorCount();
    // 获取当前CPU每个核心的频率
    long[] currentFreq = centralProcessor.getCurrentFreq();
    System.out.println("CPU名称：" + name);
    System.out.println("CPU核心数：" + physicalProcessorCount);

    // 内存信息
    GlobalMemory memory = hardware.getMemory();
    // 内存总大小
    long totalOfG = memory.getTotal() / 1024 / 1024 / 1024;
    // 可用大小
    long availableOfG = memory.getAvailable() / 1024 / 1024 / 1024;
    System.out.println("内存总大小：" + totalOfG + "G");
    System.out.println("内存可用大小：" + availableOfG + "G");

    // 磁盘信息
    List<HWDiskStore> diskStores = hardware.getDiskStores();
    // 获取磁盘总大小
    long total = diskStores.stream().mapToLong(HWDiskStore::getSize).sum();
    System.out.println("磁盘总大小：" + total / 1024 / 1024 / 1024 + "G");

    // 系统运行时间
    long uptimeInSeconds = systemInfo.getOperatingSystem().getSystemUptime();
    System.out.println("系统运行时间：" + uptimeInSeconds / 3600 + "小时");
}
```
在本地运行之后输出如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679217304213-9b898254-c319-49cf-b84c-772e68492a6c.png#averageHue=%23404040&clientId=u7b538848-0ccc-4&from=paste&id=ub3cdd3c1&originHeight=184&originWidth=308&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9def1e86-0fe8-4bcd-a911-b7b33f7f9dd&title=)
<a name="QXsZn"></a>
## 一个漂亮的仪表盘
看过上面的例子，你会发现 OSHI 其实很好用吧。但是只有一个例子的展示大家应该不太满足，所以特意找了一个已经开发好的监控工具——Ward。Ward 是一款简单、极简主义的服务器监控工具。它用漂亮的仪表盘的形式来展示系统的硬件信息。如图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1679217304300-bc2d2e28-8fd7-4077-b2ce-521854499bfe.png#averageHue=%23eaece6&clientId=u7b538848-0ccc-4&from=paste&id=uc83d5dbc&originHeight=567&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u20244e38-ea3e-410e-8fe4-9766bf16d13&title=)<br />其实 Ward 是一个很简单的 springboot 项目，借助于该项目，简单阅读下源码就能更好的理解 OSHI 的使用啦。
<a name="b8EyN"></a>
## 总结
介绍了 OSHI 的使用方式，以及另一个使用 OSHI 的 Web 服务。感兴趣的朋友们可以亲自体验一下，项目地址都放到下面了：<br />OSHI：[https://github.com/oshi/oshi](https://github.com/oshi/oshi)<br />Ward：[https://github.com/Rudolf-Barbu/Ward](https://github.com/Rudolf-Barbu/Ward)
