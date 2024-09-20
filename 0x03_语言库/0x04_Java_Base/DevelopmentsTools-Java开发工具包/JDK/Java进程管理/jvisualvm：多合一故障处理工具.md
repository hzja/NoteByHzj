<a name="qOh2E"></a>
## VisualVM介绍
VisualVM 是一款免费的，集成了多个 JDK 命令行工具的可视化工具，它能提供强大的分析能力，对 Java 应用程序做性能分析和调优。这些功能包括生成和分析海量数据、跟踪内存泄漏、监控垃圾回收器、执行内存和 CPU 分析，同时它还支持在 MBeans 上进行浏览和操作。本文主要介绍如何使用 VisualVM 进行性能分析及调优。<br />VisualVM位于{JAVA_HOME}/bin目录中。<br />点击运行，效果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692074852593-5431fce2-c29c-44f8-ae55-0780a93eb5ed.png#averageHue=%23f5f4f3&clientId=u809040a8-cde3-4&from=paste&height=582&id=u6fabee59&originHeight=1454&originWidth=2148&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=637004&status=done&style=none&taskId=u1b63bc9e-11d3-4719-b04c-0dbdfb2338b&title=&width=859.2)<br />下面来看visualvm的各种功能
<a name="HkYCv"></a>
## 查看jvm配置信息
**第一步**：点击左边窗口显示正在运行的java进程<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692078503370-8515c34e-fca3-4604-99cb-4be9e932451e.png#averageHue=%23f8f7f5&clientId=u809040a8-cde3-4&from=paste&height=411&id=u314827d6&originHeight=1027&originWidth=895&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=285009&status=done&style=none&taskId=ub018774d-27a6-4b04-8d65-140e0c9035e&title=&width=358)<br />**第二步**：点击右侧窗口“概述”，可以查看各种配置信息<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692078512939-885ed6e1-83a2-4611-8b95-10058c2ead2a.png#averageHue=%23f9f8f8&clientId=u809040a8-cde3-4&from=paste&height=285&id=uaa7baf21&originHeight=712&originWidth=2148&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=249058&status=done&style=none&taskId=u6ee65f6a-cbb7-4e46-9f92-298b988daee&title=&width=859.2)<br />通过jdk提供的jinfo命令工具也可以查看上面的信息。
<a name="eUZ0N"></a>
## 查看cpu、内存、类、线程监控信息
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692078558999-249bae2b-c34a-44d3-b880-6349c75beb08.png#averageHue=%23f5f2ee&clientId=u809040a8-cde3-4&from=paste&height=497&id=u6c487598&originHeight=1243&originWidth=2150&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=349310&status=done&style=none&taskId=u83724500-5dcb-47c1-b004-4d25a177abf&title=&width=860)
<a name="vv69d"></a>
## 查看堆的变化
**步骤一**：运行下面的代码<br />每隔3秒，堆内存使用新增100M
```java
package com.jvm.visualvm;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import java.util.concurrent.TimeUnit;

public class Demo1 {
    public static final int _1M = 1024 * 1024;

    public static void main(String[] args) throws InterruptedException {
        List<Object> list = new ArrayList<>();
        for (int i = 0; i < 1000; i++) {
            list.add(new byte[100 * _1M]);
            TimeUnit.SECONDS.sleep(3);
            System.out.println(i);
        }
    }
}
```
**步骤二**：在VisualVM可以很清晰的看到堆内存变化信息。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692078742092-cd390034-0eb2-42cd-b517-849fbe607338.png#averageHue=%23ecd1ad&clientId=u809040a8-cde3-4&from=paste&height=576&id=wgaAz&originHeight=1441&originWidth=2151&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=541297&status=done&style=none&taskId=u2d38c396-4bef-4faa-b714-473b824635b&title=&width=860.4)
<a name="O0ocq"></a>
## 查看堆快照
**步骤一**：点击“监视”->”堆(dump)”可以生产堆快照信息.<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692078883634-b0b2d613-d32c-4912-82d2-d8c35405a39b.png#averageHue=%23f3d5ad&clientId=u809040a8-cde3-4&from=paste&height=686&id=P0JML&originHeight=1715&originWidth=2141&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=563374&status=done&style=none&taskId=ufc4fd00f-936a-4715-8036-a9682ed617e&title=&width=856.4)<br />生成了以heapdump开头的一个选项卡，内容如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692080168939-93f373e9-fc78-43a4-8c40-9f79862c86f6.png#averageHue=%23f9f8f7&clientId=u809040a8-cde3-4&from=paste&height=517&id=u02f8d6b6&originHeight=1292&originWidth=2142&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=472724&status=done&style=none&taskId=u50eb48e3-824c-4ad0-8108-316e92c5cae&title=&width=856.8)<br />对于“堆 dump”来说，在远程监控jvm的时候，VisualVM是没有这个功能的，只有本地监控的时候才有。
<a name="ZLfBu"></a>
## 导出堆快照文件
**步骤一**：查看堆快照，此步骤可以参考上面的“查看堆快照”功能<br />**步骤二**：右键点击另存为，即可导出hprof堆快照文件，可以发给其他同事分析使用<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692080525414-98c7bc6c-628f-40aa-90e8-a7b677043166.png#averageHue=%23f6f4f3&clientId=u809040a8-cde3-4&from=paste&height=350&id=u912044d6&originHeight=874&originWidth=996&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=240151&status=done&style=none&taskId=u67464164-64eb-4f6d-9656-d6e4a6084a7&title=&width=398.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692080544980-fe4c3f2c-2105-43e1-b2dc-fe18ed6cb15b.png#averageHue=%23f4f3f2&clientId=u809040a8-cde3-4&from=paste&height=431&id=u1134be0d&originHeight=1077&originWidth=1402&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=299615&status=done&style=none&taskId=u31ffc465-ea8b-459c-8008-65d5516a356&title=&width=560.8)
<a name="gP4Lo"></a>
## 查看class对象加载信息
其次来看下永久保留区域PermGen使用情况<br />**步骤一**：运行一段类加载的程序，代码如下：
```java
package com.jvm.visualvm;

import java.io.File;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

public class Demo2 {

    private static List<Object> insList = new ArrayList<Object>();

    public static void main(String[] args) throws Exception {
        permLeak();
    }

    private static void permLeak() throws Exception {
        for (int i = 0; i < 2000; i++) {
            URL[] urls = getURLS();
            URLClassLoader urlClassloader = new URLClassLoader(urls, null);
            Class<?> logfClass = Class.forName("org.apache.commons.logging.LogFactory", true, urlClassloader);
            Method getLog = logfClass.getMethod("getLog", String.class);
            Object result = getLog.invoke(logfClass, "TestPermGen");
            insList.add(result);
            System.out.println(i + ": " + result);
            if (i % 100 == 0) {
                TimeUnit.SECONDS.sleep(1);
            }
        }
    }

    private static URL[] getURLS() throws MalformedURLException {
        File libDir = new File("D:\\installsoft\\maven\\.m2\\repository3.3.9_0\\commons-logging\\commons-logging\\1.1.1");
        File[] subFiles = libDir.listFiles();
        int count = subFiles.length;
        URL[] urls = new URL[count];
        for (int i = 0; i < count; i++) {
            urls[i] = subFiles[i].toURI().toURL();
        }
        return urls;
    }

}
```
**步骤二**：打开visualvm查看，metaspace<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692080714097-0e91615b-755a-4661-bf59-c5d96fe046c4.png#averageHue=%23f5f2f0&clientId=u809040a8-cde3-4&from=paste&height=660&id=ufb626f99&originHeight=1651&originWidth=2147&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=648553&status=done&style=none&taskId=u4913ff4e-7141-4cb0-9ff4-02a6c3a5ee1&title=&width=858.8)
<a name="bVeiG"></a>
## CPU分析：发现cpu使用率最高的方法
CPU 性能分析的主要目的是统计函数的调用情况及执行时间，或者更简单的情况就是统计应用程序的 CPU 使用情况。<br />没有程序运行时的 CPU 使用情况如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692081563946-65532c8d-4417-4ce1-b931-b1bfdf4640a4.png#averageHue=%23f4f3f3&clientId=u809040a8-cde3-4&from=paste&height=776&id=u91947b09&originHeight=1939&originWidth=2147&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=556973&status=done&style=none&taskId=u78285d32-d083-4eaa-9243-d4d4ed9f613&title=&width=858.8)<br />下面写一个cpu占用率比较高的程序。<br />**步骤一**：运行下列程序：
```java
package com.jvm.visualvm;

public class Demo3 {
    public static void main(String[] args) throws InterruptedException {
        cpuFix();
    }

    /**
     * cpu 运行固定百分比
     *
     * @throws InterruptedException
     */
    public static void cpuFix() throws InterruptedException {
        // 80%的占有率
        int busyTime = 8;
        // 20%的占有率
        int idelTime = 2;
        // 开始时间
        long startTime = 0;
        while (true) {
            // 开始时间
            startTime = System.currentTimeMillis();
            /*
             * 运行时间
             */
            while (System.currentTimeMillis() - startTime < busyTime) {
                ;
            }
            // 休息时间
            Thread.sleep(idelTime);
        }
    }
}
```
**步骤二**：打开visualvm查看cpu使用情况，电脑是8核的，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692081686094-5741be11-4fb1-4df1-b3fe-323794fc5207.png#averageHue=%23f5f3f2&clientId=u809040a8-cde3-4&from=paste&height=664&id=u5e10ecea&originHeight=1659&originWidth=2150&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=553843&status=done&style=none&taskId=ua24e282a-3a95-46df-823e-45eec1d41aa&title=&width=860)<br />过高的 CPU 使用率可能是程序代码性能有问题导致的。可以切换到“抽样器”对cpu进行采样，可以看到那个方法占用的cpu最高，然后进行优化。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692081729361-d8f2405e-9235-4201-98f5-2595fd34c092.png#averageHue=%23f8f6f5&clientId=u809040a8-cde3-4&from=paste&height=482&id=u1953ee73&originHeight=1205&originWidth=2145&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=468373&status=done&style=none&taskId=u200af84b-90c4-407d-b348-fd29ef92625&title=&width=858)<br />从图中可以看出cpuFix方法使用cpu最多，然后就可以进行响应的优化了。
<a name="ga0AS"></a>
## 查看线程快照：发现死锁问题
Java 语言能够很好的实现多线程应用程序。当对一个多线程应用程序进行调试或者开发后期做性能调优的时候，往往需要了解当前程序中所有线程的运行状态，是否有死锁、热锁等情况的发生，从而分析系统可能存在的问题。<br />在 VisualVM 的监视标签内，可以查看当前应用程序中所有活动线程（Live threads）和守护线程（Daemon threads）的数量等实时信息。<br />可以查看线程快照，发现系统的死锁问题。<br />下面将通过visualvm来排查一个死锁问题。<br />**步骤一**：运行下面的代码：
```java
package com.jvm.visualvm;

public class Demo4 {

    public static void main(String[] args) {
        Obj1 obj1 = new Obj1();
        Obj2 obj2 = new Obj2();
        Thread thread1 = new Thread(new SynAddRunalbe(obj1, obj2, 1, 2, true));
        thread1.setName("thread1");
        thread1.start();
        Thread thread2 = new Thread(new SynAddRunalbe(obj1, obj2, 2, 1, false));
        thread2.setName("thread2");
        thread2.start();
    }

    /**
     * 线程死锁等待演示
     */
    public static class SynAddRunalbe implements Runnable {
        Obj1 obj1;
        Obj2 obj2;
        int a, b;
        boolean flag;

        public SynAddRunalbe(Obj1 obj1, Obj2 obj2, int a, int b, boolean flag) {
            this.obj1 = obj1;
            this.obj2 = obj2;
            this.a = a;
            this.b = b;
            this.flag = flag;
        }

        @Override
        public void run() {
            try {
                if (flag) {
                    synchronized (obj1) {
                        Thread.sleep(100);
                        synchronized (obj2) {
                            System.out.println(a + b);
                        }
                    }
                } else {
                    synchronized (obj2) {
                        Thread.sleep(100);
                        synchronized (obj1) {
                            System.out.println(a + b);
                        }
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static class Obj1 {
    }

    public static class Obj2 {
    }
}
```
程序中：thread1持有obj1的锁，thread2持有obj2的锁，thread1等待获取obj2的锁，thread2等待获取obj1的锁，相互需要获取的锁都被对方持有者，造成了死锁。程序中出现了死锁的情况，是比较难以发现的。需要依靠工具解决。<br />**步骤二**：打开visualvm查看堆栈信息：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692081987785-d53e03d0-faaf-48c8-bde0-b52e90b6bff3.png#averageHue=%23f7f3ee&clientId=u809040a8-cde3-4&from=paste&height=682&id=ub526b8f6&originHeight=1705&originWidth=2151&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=883341&status=done&style=none&taskId=ub1e9162e-de31-485d-a848-b9019a322df&title=&width=860.4)<br />点击dump，生成线程堆栈信息：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692082505392-c261b3ba-c358-46a6-93f4-5407eabb6b65.png#averageHue=%23fbf7f5&clientId=u809040a8-cde3-4&from=paste&height=850&id=u935453ec&originHeight=2126&originWidth=1743&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=990204&status=done&style=none&taskId=ufc9a1e95-e912-4487-8c74-8aefbcd365b&title=&width=697.2)<br />可以看到“**Found one Java-level deadlock**”，包含了导致死锁的代码。
```
"thread2":
    at com.jvm.visualvm.Demo4$SynAddRunalbe.run(Demo4.java:50)
    - waiting to lock <0x00000007173d40f0> (a com.jvm.visualvm.Demo4$Obj1)
    - locked <0x00000007173d6310> (a com.jvm.visualvm.Demo4$Obj2)
    at java.lang.Thread.run(Thread.java:745)
"thread1":
    at com.jvm.visualvm.Demo4$SynAddRunalbe.run(Demo4.java:43)
    - waiting to lock <0x00000007173d6310> (a com.jvm.visualvm.Demo4$Obj2)
    - locked <0x00000007173d40f0> (a com.jvm.visualvm.Demo4$Obj1)
    at java.lang.Thread.run(Thread.java:745)
```
上面这段信息可以看出，thread1持有Obj1对象的锁，等待获取Obj2的锁，thread2持有Obj2的锁，等待获取Obj1的锁，导致了死锁。
<a name="nm9cZ"></a>
## 总结
本文介绍了jdk提供的一款非常强大的分析问题的一个工具VisualVM，通过他可以做以下事情：

1. 查看应用jvm配置信息
2. 查看cpu、内存、类、线程监控信息
3. 查看堆的变化
4. 查看堆快照
5. 导出堆快照文件
6. 查看class对象加载信息
7. CPU分析：发现cpu使用率最高的方法
8. 分析死锁问题，找到死锁的代码
