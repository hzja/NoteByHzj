Java<br />一个大型的Java项目也许从开发到测试结束并可能不能未发现一些重大的问题，但是在生产环境中还是会出现一些非常棘手的问题，如内存泄漏直接导致服务宕机，遇到这样的问题对于一个经验尚浅的开发人员来说难度非常大，好的一点是JVM 能够记录下问题发生时系统的部分运行状态，并将其存储在堆转储 (Heap Dump) 文件中，从而为分析和诊断问题提供了重要的依据。要生成Heap Dump文件的前提是需要在服务的启动脚本添加一些jvm参数。<br />接下来将说明如何生成Heap Dump文件到如何使用Memory Analyzer Tool分析Heap Dump文件。Memory Analyzer Tool是一款“傻瓜式“的堆转储文件分析工具，通过该工具可以生成一个专业的分析报告，从而准确的定位到问题的所在位置。
<a name="YdUHV"></a>
## 1、安装 Memory Analyzer Tool
<a name="IEfsU"></a>
### 方式一：直接从官网下载程序包 [http://www.eclipse.org/mat/downloads.php](http://www.eclipse.org/mat/downloads.php)，解压可直接使用。
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970053498-2dc1484d-e4a3-4911-b90e-eaddf7e58e8b.webp#clientId=uc7c51607-de6b-4&from=paste&id=u3968930f&originHeight=545&originWidth=833&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u59eb5900-5270-4df5-a2c1-3329c4716ac&title=)
<a name="YMOmX"></a>
### 方式二：在Eclipse中通过 Help -> Install New Software，输入地址 [http://download.eclipse.org/mat/1.9.0/update-site/](http://download.eclipse.org/mat/1.9.0/update-site/)进行安装
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970053864-630b3ac5-fc08-435b-8f4f-f285588f2e24.webp#clientId=uc7c51607-de6b-4&from=paste&id=u7c184269&originHeight=676&originWidth=970&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue8edd089-b1df-4100-b12d-9c67f5cf823&title=)
<a name="Nl1f8"></a>
### 方式三：Eclipse 的可插拔式插件安装方式
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970053792-873422d6-a7a5-4805-bbff-2e62f1ac4df3.webp#clientId=uc7c51607-de6b-4&from=paste&id=uf9cc7ecc&originHeight=367&originWidth=683&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3106389f-4a76-4c83-baa8-85d3c3379d6&title=)
<a name="hJpL7"></a>
## 2、配置环境参数
为了更有效率的使用Memory Analyzer Tool，还需要做一些配置工作。因为通常而言，分析一个堆转储文件需要消耗很多的堆空间，为了保证分析的效率和性能，在有条件的情况下，建议分配给 Memory Analyzer Tool尽可能多的内存资源。可以采用如下两种方式来分配内存更多的内存资源给 Memory Analyzer Tool 。<br />方式一：修改启动参数 `MemoryAnalyzer.exe -vmargs -Xmx4g`<br />方式二：编辑文件 MemoryAnalyzer.ini，在里面添加类似信息 `-vmargs – Xmx4g`
<a name="jDUpn"></a>
## 3、如何获得堆转储文件
方式一：在Eclipse中配置JVM启动参数 `-XX:+HeapDumpOnOutOfMemoryError`<br />方式二：通过JDK自带的工具`jmap`，`jconsole`来获得一个堆转储文件<br />这里使用方式一来获得
<a name="okcIL"></a>
### Java中OutOfMemoryError(内存溢出)的三种情况及解决办法
在解决java内存溢出问题之前，需要对jvm（java虚拟机）的内存管理有一定的认识。jvm管理的内存大致包括三种不同类型的内存区域：PermanentGeneration space（永久保存区域）、Heap space(堆区域)、JavaStacks(Java栈）。其中永久保存区域主要存放Class（类）和Meta的信息，Class第一次被Load的时候被放入PermGenspace区域，Class需要存储的内容主要包括方法和静态属性。堆区域用来存放Class的实例（即对象），对象需要存储的内容主要是非静态属性。每次用new创建一个对象实例后，对象实例存储在堆区域中，这部分空间也被jvm的垃圾回收机制管理。而Java栈跟大多数编程语言包括汇编语言的栈功能相似，主要基本类型变量以及方法的输入输出参数。Java程序的每个线程中都有一个独立的堆栈。容易发生内存溢出问题的内存空间包括：PermanentGeneration space和Heap space。
<a name="br8Q4"></a>
#### 第一种OutOfMemoryError：PermGenspace
发生这种问题的原意是程序中使用了大量的jar或class，使java虚拟机装载类的空间不够，与PermanentGeneration space有关。解决这类问题有以下两种办法：<br />1、增加java虚拟机中的XX:PermSize和XX:MaxPermSize参数的大小，其中XX:PermSize是初始永久保存区域大小，XX:MaxPermSize是最大永久保存区域大小。如针对tomcat，在catalina.sh或catalina.bat文件中一系列环境变量名说明结束处（大约在70行左右） 增加一行：<br />`JAVA_OPTS=" -XX:PermSize=64M -XX:MaxPermSize=128m"`
<a name="wNWwn"></a>
#### 第二种OutOfMemoryError：Java heap space
发生这种问题的原因是java虚拟机创建的对象太多，在进行垃圾回收之间，虚拟机分配的到堆内存空间已经用满了，与Heapspace有关。解决这类问题有两种思路：<br />1、检查程序，看是否有死循环或不必要地重复创建大量对象。找到原因后，修改程序和算法。<br />2、增加Java虚拟机中Xms（初始堆大小）和Xmx（最大堆大小）参数的大小。如：set JAVA_OPTS= -Xms256m-Xmx1024m
<a name="tJRYO"></a>
#### 第三种OutOfMemoryError：unable to create new nativethread
这种错误在Java线程个数很多的情况下容易发生
<a name="ljJeP"></a>
## 4、实例分析
<a name="J45Bb"></a>
### 编写例子获取Heap Dump文件
```java
import java.util.ArrayList;
import java.util.List;

public class HeapDumpMain {

    static class OOMHeapDumpObject{
        String str ="1234567890";
    }

    public static void main(String[] args) {
        List<OOMHeapDumpObject> ooms = new ArrayList<OOMHeapDumpObject>();
        while (true) {
            ooms.add(new OOMHeapDumpObject());
        }
    }

}
```
<a name="YWFdN"></a>
### Eclipse中VM参数配置如下
`-Xms20m -Xmx20m -XX:+HeapDumpOnOutOfMemoryError`<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970053475-2888a90e-b7b5-4194-9d58-83739ff8cb41.webp#clientId=uc7c51607-de6b-4&from=paste&id=ud8ce31c6&originHeight=639&originWidth=830&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6f9f605e-c1b7-48d3-9812-359d2538a30&title=)
<a name="fMq6e"></a>
### 运行示例代码即可生成Heap Dump文件
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970053841-4040012c-e375-497a-8306-8cb1a6f392d9.webp#clientId=uc7c51607-de6b-4&from=paste&id=u9426a053&originHeight=399&originWidth=808&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uce8308a4-77f4-4f5b-baed-0dd340957cf&title=)
<a name="NrmJ2"></a>
### 一般在项目的当目前目录下
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970054023-4f72d06b-5366-4a1b-a61f-c3e49e115ab8.webp#clientId=uc7c51607-de6b-4&from=paste&id=u3583a98c&originHeight=203&originWidth=360&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u20324419-93f3-4ca2-a2d6-5c367774032&title=)
<a name="G4mBf"></a>
### 运行Memory Analyzer Tool（一般选择独立运行的Memory Analyzer Tool）
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970054221-8a9a3bb3-bcf2-416c-9ac7-f0e8601d90ec.webp#clientId=uc7c51607-de6b-4&from=paste&id=u0c056c42&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8e63cf48-a2fb-4965-b472-9c2598c7a3d&title=)
<a name="ojhnC"></a>
### 报告分析
<a name="d4dv4"></a>
#### 内存使用整体情况
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970054846-8267ee7d-bc7b-4012-80e4-7ecd7aaba990.webp#clientId=uc7c51607-de6b-4&from=paste&id=uf941bef7&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u538a589a-8a2f-4c19-ba8a-42a1992430f&title=)<br />直接点击下方的 Reports->Leak Suspects 链接来生成报告，查看导致内存泄露的罪魁祸首<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970054567-622995fd-872d-4c0b-ab3c-9bf96788a1f5.webp#clientId=uc7c51607-de6b-4&from=paste&id=u6611e6bd&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6333fbee-51c6-4708-a58d-2f05cabb824&title=)<br />从图上可以清晰地看到一个可疑对象消耗了系统近 98% 的内存。再往下看饼图下方文字简短描述了大量的内存是由属于Object实例的对象所消耗的，system class loader 负责加载这个对象。也许从这里还不能找出内存泄漏的具体原因，接着往下看。点击下图标志的地方<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970054587-2d3f116c-7c50-4341-8d67-45228dee1231.webp#clientId=uc7c51607-de6b-4&from=paste&id=u99c192a8&originHeight=549&originWidth=714&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6a1e6943-5959-4d48-a2a9-6fe1dd9157b&title=)<br />可以看到如下<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643970054612-94870d33-78be-4db0-a24e-fafcf942c120.webp#clientId=uc7c51607-de6b-4&from=paste&id=ue4818e69&originHeight=559&originWidth=781&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1564bf12-c890-4507-8dfa-2e6c8e31a69&title=)<br />备注：Shallow Heap 为对象自身占用的内存大小，不包括它引用的对象。<br />Retained Heap 为当前对象大小 + 当前对象可直接或间接引用到的对象的大小总和<br />在这张图上可以清楚的看到，这个对象集合中保存了大量 OOMHeapDumpObject 对象的引用，就是它导致的内存泄露。这时就需要排查程序中为什么会创建这么多OOMHeapDumpObject 对象，然后解决问题。
