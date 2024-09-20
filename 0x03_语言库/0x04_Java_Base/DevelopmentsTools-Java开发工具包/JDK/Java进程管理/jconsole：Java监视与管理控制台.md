Java
<a name="D1ADW"></a>
## jconsole介绍
JConsole（java monitoring and management console）是一款基于JMX的可视化监视和管理工具。
<a name="FcyvI"></a>
## 启动JConsole

1. 点击JDK/bin 目录下面的“jconsole.exe”即可启动
2. 然后会自动自动搜索本机运行的所有虚拟机进程
3. 选择其中一个进程可开始进行监控

![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692064993304-48438a6b-c88f-4969-8ba7-b57512d00a34.png#averageHue=%2399c0c4&clientId=u5f9a79f9-0e24-4&from=paste&height=721&id=ufe9fb1cd&originHeight=1802&originWidth=2143&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=467500&status=done&style=none&taskId=u3b8f9b18-03df-45a7-a762-66440c57fbd&title=&width=857.2)
<a name="GKmMK"></a>
## JConsole基本介绍
JConsole 基本包括以下基本功能：**概述、内存、线程、类、VM概要、MBean**<br />运行下面的程序、然后使用JConsole进行监控;注意设置虚拟机参数
```java
package com.jvm.jconsole;

import java.util.ArrayList;
import java.util.List;

/**
 * 设置虚拟机参数：-Xms100M -Xms100m -XX:+UseSerialGC -XX:+PrintGCDetails
 */
public class Demo1 {
    static class OOMObject {
        public byte[] placeholder = new byte[64 * 1024];
    }

    public static void fillHeap(int num) throws InterruptedException {
        Thread.sleep(20000); //先运行程序，在执行监控
        List<OOMObject> list = new ArrayList<OOMObject>();
        for (int i = 0; i < num; i++) {
            // 稍作延时，令监视曲线的变化更加明显
            Thread.sleep(50);
            list.add(new OOMObject());
        }
        System.gc();
    }

    public static void main(String[] args) throws Exception {
        fillHeap(1000);
        while (true) {
            //让其一直运行着
        }
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692065121755-d97c55db-09fe-4c2c-adef-b52dde1e5728.png#averageHue=%23eceae9&clientId=u5f9a79f9-0e24-4&from=paste&height=567&id=u67cd0a2a&originHeight=1418&originWidth=2146&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=598911&status=done&style=none&taskId=u51300ed6-433d-4870-9d3d-42c947b8b5a&title=&width=858.4)<br />打开JConsole查看上面程序<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692065302437-beb7821b-4818-45f3-804a-c2676701a007.png#averageHue=%239ac1c6&clientId=u5f9a79f9-0e24-4&from=paste&height=718&id=uaa0aba08&originHeight=1796&originWidth=2140&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=505502&status=done&style=none&taskId=u0e5e4d22-5ef3-4306-953f-2f02fd40957&title=&width=856)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692065197894-ccb88505-6763-4d6b-89ca-d4bacdadc4df.png#averageHue=%23f8f7f7&clientId=u5f9a79f9-0e24-4&from=paste&height=718&id=u1559bf34&originHeight=1796&originWidth=2140&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=417594&status=done&style=none&taskId=u6e3baf7c-378f-49f9-8353-ba9bc94c70e&title=&width=856)<br />可以切换顶部的选项卡查看各种指标信息。
<a name="tlxqe"></a>
## 内存监控
“内存”页签相当于可视化的jstat 命令，用于监视受收集器管理的虚拟机内存的变换趋势。<br />还是上面的程序：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692065409551-e1d77d74-e6a2-4035-a886-3289f5a0dd8a.png#averageHue=%23f6f6f5&clientId=u5f9a79f9-0e24-4&from=paste&height=717&id=u085b894d&originHeight=1793&originWidth=2146&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=477248&status=done&style=none&taskId=uf330db7c-7fd3-4543-9e50-ccc2e412ef2&title=&width=858.4)<br />代码运行，控制台也会输出gc日志：
```
[GC (Allocation Failure) [DefNew: 27328K->3392K(30720K), 0.0112139 secs] 27328K->19901K(99008K), 0.0112664 secs] [Times: user=0.00 sys=0.01, real=0.01 secs] 
[GC (Allocation Failure) [DefNew: 30720K->3392K(30720K), 0.0133413 secs] 47229K->40117K(99008K), 0.0133708 secs] [Times: user=0.01 sys=0.00, real=0.01 secs] 
[GC (Allocation Failure) [DefNew: 30664K->3374K(30720K), 0.0140975 secs] 67389K->65091K(99008K), 0.0141239 secs] [Times: user=0.00 sys=0.02, real=0.01 secs] 
[Full GC (System.gc()) [Tenured: 61716K->66636K(68288K), 0.0098835 secs] 66919K->66636K(99008K), [Metaspace: 9482K->9482K(1058816K)], 0.0100578 secs] [Times: user=0.00 sys=0.00, real=0.01 secs]
```
<a name="lGJTg"></a>
## 线程监控
如果上面的“内存”页签相当于可视化的jstat命令的话，“线程”页签的功能相当于可视化的jstack命令，遇到线程停顿时可以使用这个页签进行监控分析。线程长时间停顿的主要原因主要有：等待外部资源（数据库连接、网络资源、设备资源等）、死循环、锁等待（活锁和死锁）<br />下面三个方法分别等待控制台输入、死循环演示、线程锁等待演示<br />**第一步**：运行如下代码：
```java
package com.jvm.jconsole;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Demo2 {
    public static void main(String[] args) throws IOException {
        waitRerouceConnection();
        createBusyThread();
        createLockThread(new Object());
    }

    /**
     * 等待控制台输入
     *
     * @throws IOException
     */
    public static void waitRerouceConnection() throws IOException {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
                try {
                    br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }, "waitRerouceConnection");
        thread.start();

    }

    /**
     * 线程死循环演示
     */
    public static void createBusyThread() {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    ;
                }
            }
        }, "testBusyThread");
        thread.start();
    }

    /**
     * 线程锁等待演示
     */
    public static void createLockThread(final Object lock) {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                synchronized (lock) {
                    try {
                        lock.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }, "testLockThread");
        thread.start();
    }
}
```
**第二步**：打开jconsole中查看上面程序运行情况，可以查看到3个目标线程<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692065522157-972a303e-9c53-47b9-99e0-f5628b473e47.png#averageHue=%23f9f7f6&clientId=u5f9a79f9-0e24-4&from=paste&height=709&id=u84978b96&originHeight=1772&originWidth=2144&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=664170&status=done&style=none&taskId=u913056fc-e733-48aa-bba9-dd32c860c8e&title=&width=857.6)<br />**第三步**：查看目标线程信息<br />`waitRerouceConnection`线程处于读取数据状态，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692065785901-eac01034-942a-4b3d-a65b-751a5b0836c8.png#averageHue=%23faf8f7&clientId=u5f9a79f9-0e24-4&from=paste&height=686&id=u9f574b35&originHeight=1716&originWidth=2140&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1037145&status=done&style=none&taskId=ua8096755-c24c-4202-a254-aedc4dd8685&title=&width=856)<br />`testBusyThread`线程位于代码45行，处于运行状态，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692067419491-f5e14977-03da-4ca3-87cc-769d6d944252.png#averageHue=%23fbf9f9&clientId=u5f9a79f9-0e24-4&from=paste&height=319&id=udbad7213&originHeight=797&originWidth=2142&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=371854&status=done&style=none&taskId=u4d126e5e-5fdb-492f-bc39-428f9f23679&title=&width=856.8)<br />`testLockThread`处于活锁等待状态，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692067538323-fb505cd5-c260-46af-9813-b46b3b4a881d.png#averageHue=%23fbf7f7&clientId=u5f9a79f9-0e24-4&from=paste&height=315&id=u76ffb113&originHeight=788&originWidth=2131&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=447571&status=done&style=none&taskId=u9a80ea2e-93a1-455c-84fb-3715aac6f19&title=&width=852.4)<br />只要lock对象的`notify()`或`notifyAll()`方法被调用，这个线程便可能激活以继续执行<br />通过“线程”这个窗口可以很方便查询虚拟机中的线程堆栈信息，对发现系统中的一些问题非常有帮助。
<a name="x0QUo"></a>
## 线程死锁演示
**第一步**：运行下面代码：
```java
package com.jvm.jconsole;

public class Demo3 {
    public static void main(String[] args) {
        User u1 = new User("u1");
        User u2 = new User("u2");
        Thread thread1 = new Thread(new SynAddRunalbe(u1, u2, 1, 2, true));
        thread1.setName("thread1");
        thread1.start();
        Thread thread2 = new Thread(new SynAddRunalbe(u1, u2, 2, 1, false));
        thread2.setName("thread2");
        thread2.start();
    }

    /**
     * 线程死锁等待演示
     */
    public static class SynAddRunalbe implements Runnable {
        User u1, u2;
        int a, b;
        boolean flag;

        public SynAddRunalbe(User u1, User u2, int a, int b, boolean flag) {
            this.u1 = u1;
            this.u2 = u2;
            this.a = a;
            this.b = b;
            this.flag = flag;
        }

        @Override
        public void run() {
            try {
                if (flag) {
                    synchronized (u1) {
                        Thread.sleep(100);
                        synchronized (u2) {
                            System.out.println(a + b);
                        }
                    }
                } else {
                    synchronized (u2) {
                        Thread.sleep(100);
                        synchronized (u1) {
                            System.out.println(a + b);
                        }
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static class User {
        private String name;

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public User(String name) {
            this.name = name;
        }

        @Override
        public String toString() {
            return "User{" +
            "name='" + name + '\'' +
            '}';
        }
    }
}
```
thread1持有u1的锁，thread2持有u2的锁，thread1等待获取u2的锁，thread2等待获取u1的锁，相互需要获取的锁都被对方持有者，造成了死锁。程序中出现了死锁的情况，是比较难以发现的。需要依靠工具解决。刚好jconsole就是这个美妙的工具。<br />**第二步：**在jconsole中打开上面程序的监控信息：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692067794259-3976c777-ef19-4471-8d2c-7b7816b9027f.png#averageHue=%23f9f7f6&clientId=u5f9a79f9-0e24-4&from=paste&height=722&id=ub1df2992&originHeight=1804&originWidth=2149&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=523114&status=done&style=none&taskId=u82808335-7e3b-4e64-b372-51ff3cd1235&title=&width=859.6)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692068091678-4a0fe455-4fc3-4aa8-a87d-3c6611b3edb0.png#averageHue=%23fcf8f8&clientId=u5f9a79f9-0e24-4&from=paste&height=333&id=ufa2a7786&originHeight=833&originWidth=2152&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=255671&status=done&style=none&taskId=ubeaa7e43-f629-4cef-b832-0c6a23c68a7&title=&width=860.8)<br />从上面可以看出代码43行和50行处导致了死锁。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1692068005417-70769dc5-2840-4970-8266-05ac48b070d4.png#averageHue=%23f3efee&clientId=u5f9a79f9-0e24-4&from=paste&height=726&id=u809657bc&originHeight=1814&originWidth=1843&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=966342&status=done&style=none&taskId=ub9607d34-92af-4d89-8dfc-6c0caeb015a&title=&width=737.2)<br />关于程序死锁的，还可以使用命令行工具**jstack**来查看java线程堆栈信息，也可以发现死锁。
