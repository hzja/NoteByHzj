JVM<br />这里介绍如何在容器环境下配置JVM堆参数大小。
<a name="lnFPR"></a>
## 背景信息
当业务是使用Java开发，且设置的JVM堆空间过小时，程序会出现系统内存不足OOM（Out of Memory）的问题。特别是在容器环境下，不合理的JVM堆参数设置会导致各种异常现象产生，例如应用堆大小还未到达设置阈值或规格限制，就因为OOM导致重启等。
<a name="Pj8EZ"></a>
## 通过`-XX:MaxRAMPercentage`限制堆大小（推荐）
在容器环境下，Java只能获取服务器的配置，无法感知容器内存限制。可以通过设置`-Xmx`来限制JVM堆大小，但该方式存在以下问题：

- **当规格大小调整后，需要重新设置堆大小参数。**
- **当参数设置不合理时，会出现应用堆大小未达到阈值但容器OOM被强制关闭的情况。**

**说明**<br />应用程序出现OOM问题时，会触发Linux内核的OOM Killer机制。该机制能够监控占用过大内存，尤其是瞬间消耗大量内存的进程，然后它会强制关闭某项进程以腾出内存留给系统，避免系统立刻崩溃。
<a name="x3qCt"></a>
### 推荐的JVM参数设置
```bash
-XX:+UseContainerSupport 
-XX:InitialRAMPercentage=70.0 
-XX:MaxRAMPercentage=70.0 
-XX:+PrintGCDetails 
-XX:+PrintGCDateStamps 
-Xloggc:/home/admin/nas/gc-${POD_IP}-$(date '+%s').log 
-XX:+HeapDumpOnOutOfMemoryError 
-XX:HeapDumpPath=/home/admin/nas/dump-${POD_IP}-$(date '+%s').hprof
```
参数说明如下:

| **参数** | **说明** |
| --- | --- |
| `-XX:+UseContainerSupport` | 使用容器内存。允许JVM从主机读取cgroup限制，例如可用的CPU和RAM，并进行相应的配置。当容器超过内存限制时，会抛出OOM异常，而不是强制关闭容器。 |
| `-XX:InitialRAMPercentage` | 设置JVM使用容器内存的初始百分比。建议与`-XX:MaxRAMPercentage`保持一致，推荐设置为70.0。 |
| `-XX:MaxRAMPercentage` | 设置JVM使用容器内存的最大百分比。由于存在系统组件开销，建议最大不超过75.0，推荐设置为70.0。 |
| `-XX:+PrintGCDetails` | 输出GC详细信息。 |
| `-XX:+PrintGCDateStamps` | 输出GC时间戳。日期形式，例如2019-12-24T21:53:59.234+0800。 |
| `-Xloggc:/home/admin/nas/gc-${POD_IP}-$(date '+%s').log` | GC日志文件路径。需保证Log文件所在容器路径已存在，建议您将该容器路径挂载到NAS目录，以便自动创建目录以及实现日志的持久化存储。 |
| `-XX:+HeapDumpOnOutOfMemoryError` | JVM发生OOM时，自动生成DUMP文件。 |
| `-XX:HeapDumpPath=/home/admin/nas/dump-${POD_IP}-$(date '+%s').hprof` | DUMP文件路径。需保证DUMP文件所在容器路径已存在，建议您将该容器路径挂载到NAS目录，以便自动创建目录以及实现日志的持久化存储。 |

**说明**

- **使用**`**-XX:+UseContainerSupport**`**参数需JDK 8u191+、JDK 10及以上版本。**
- **JDK 11版本下日志相关的参数**`**-XX:+PrintGCDetails**`**、**`**-XX:+PrintGCDateStamps**`**、**`**-Xloggc:$LOG_PATH/gc.log**`**参数已废弃，请使用参数**`**-Xlog:gc:$LOG_PATH/gc.log**`**代替。**
- **Dragonwell 11暂不支持**`**${POD_IP}**`** 变量。**
- **如果没有将/home/admin/nas容器路径挂载到NAS目录，则必须保证该目录在应用启动前已存在，否则将不会产生日志文件。**
<a name="ZBXfN"></a>
## 通过`-Xms` `-Xmx`限制堆大小
可以通过设置`-Xms`和`-Xmx`来限制堆大小，但该方式存在以下两个问题：

- **当规格大小调整后，需要重新设置堆大小参数。**
- **当参数设置不合理时，会出现应用堆大小未达到阈值但容器OOM被强制关闭的情况。**

**说明** 应用程序出现OOM问题时，会触发Linux内核的OOM Killer机制。该机制能够监控占用过大内存，尤其是瞬间消耗大量内存的进程，然后它会强制关闭某项进程以腾出内存留给系统，避免系统立刻崩溃。
<a name="C2Mkm"></a>
### 推荐的JVM参数设置
```bash
-Xms2048m -Xmx2048m -XX:+PrintGCDetails 
-XX:+PrintGCDateStamps 
-Xloggc:/home/admin/nas/gc-${POD_IP}-$(date '+%s').log 
-XX:+HeapDumpOnOutOfMemoryError 
-XX:HeapDumpPath=/home/admin/nas/dump-${POD_IP}-$(date '+%s').hprof
```
参数说明如下：

| **参数** | **说明** |
| --- | --- |
| `-Xms` | 设置JVM初始内存大小。建议与-Xmx相同，避免每次垃圾回收完成后JVM重新分配内存。 |
| `-Xmx` | 设置JVM最大可用内存大小。为避免容器OOM，请为系统预留足够的内存大小。 |
| `-XX:+PrintGCDetails` | 输出GC详细信息。 |
| `-XX:+PrintGCDateStamps` | 输出GC时间戳。日期形式，例如2019-12-24T21:53:59.234+0800。 |
| `-Xloggc:/home/admin/nas/gc-${POD_IP}-$(date '+%s').log` | GC日志文件路径。需保证Log文件所在容器路径已存在，建议您将该容器路径挂载到NAS目录以便自动创建目录以及实现日志的持久化存储。 |
| `-XX:+HeapDumpOnOutOfMemoryError` | JVM发生OOM时，自动生成DUMP文件。 |
| `-XX:HeapDumpPath=/home/admin/nas/dump-${POD_IP}-$(date '+%s').hprof` | DUMP文件路径。需保证DUMP文件所在容器路径已存在，建议您将该容器路径挂载到NAS目录，以便自动创建目录以及实现日志的持久化存储。 |

推荐的堆大小设置

| **内存规格大小** | **JVM堆大小** |
| --- | --- |
| 1 GB | 600 MB |
| 2 GB | 1434 MB |
| 4 GB | 2867 MB |
| 8 GB | 5734 MB |

<a name="Fk7ln"></a>
## 常见问题
<a name="Jhds1"></a>
### 容器出现137退出码的含义是什么？
当容器使用内存超过限制时，会出现容器OOM，导致容器被强制关闭。此时业务应用内存可能并未达到JVM堆大小上限，所以不会产生Dump日志。建议调小JVM堆大小的上限，为容器内其他系统组件预留足够多的内存空间。<br />![2022-08-25-08-39-37.041864800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661388016880-721210f5-b14e-40d8-9e72-03f80ea5e87d.png#clientId=ub298b645-b9bf-4&from=ui&id=u47a8d47b&originHeight=377&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1223914&status=done&style=none&taskId=u7bf52ea7-7c42-49ad-b00e-94f34851bca&title=)
<a name="T9Z4H"></a>
### 堆大小和规格内存的参数值可以相同吗？
不可以。因为系统自身组件存在内存开销，所以不能将JVM堆大小设置为和规格内存大小相同的数值，需要为这些系统组件预留足够的内存空间。
<a name="CNlI1"></a>
### 在JDK 8版本下设置`-XX:MaxRAMPercentage`值为整数时报错怎么处理？
这是JDK 8的一个Bug。具体信息，请参见Java Bug Database。<br />例如，在JDK 8u191版本下，设置`-XX:MaxRAMPercentage=70`，这时JVM会启动报错。<br />![2022-08-25-08-39-37.130331700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661388016787-becbfc2e-9c09-4f02-ac6a-897affe223a0.png#clientId=ub298b645-b9bf-4&from=ui&id=yGrTS&originHeight=187&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=607117&status=done&style=none&taskId=u93ed8d7d-d62b-4963-b8c5-0e42f68d413&title=)<br />解决方案如下：

- **方式一：设置**`**-XX:MaxRAMPercentage**`**为70.0。说明 如果使用了**`**-XX:InitialRAMPercentage**`**或**`**-XX:MinRAMPercentage**`**，参数值同样不可设置为整数，需按照方式一的形式来设置。**
- **方式二：升级JDK版本至JDK 10及以上版本。**
