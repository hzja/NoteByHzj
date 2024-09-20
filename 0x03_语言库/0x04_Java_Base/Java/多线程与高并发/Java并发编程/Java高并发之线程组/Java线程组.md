<a name="Q8xxu"></a>
## 一、简介
在Thread类上还有一层`ThreadGroup`类，也就是线程组。一起来简单的聊聊**线程组**相关的知识和用法。
<a name="FFfyJ"></a>
## 二、什么是线程组
线程组，简单来说就是多个线程的集合，它的出现主要是为了更方便的管理线程。<br />从结构角度看，线程组与线程之间其实是一个父子结构，一个线程组可以拥有几个线程，同时也可以拥有几个线程组。整个组织结构像一棵树一样，每个线程一定有一个线程组，线程组可能又有一个父线程组，追溯到根节点就是一个系统线程组。<br />线程组与线程之间的关系，可以用如下图来描述。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1702260173361-b3a00781-2ee1-421b-be1b-f23ea83a37b3.jpeg)<br />比如，通常创建的main方法，对应的是main线程，它所属的是main线程组，main线程组的父级是是system系统线程组。
```java
public static void main(String[] args) {
    Thread currentThread = Thread.currentThread();
    ThreadGroup currentThreadGroup = currentThread.getThreadGroup();
    ThreadGroup systemThreadGroup = currentThreadGroup.getParent();
    System.out.println("currentThread：" + currentThread.getName());
    System.out.println("currentThreadGroup：" + currentThreadGroup.getName());
    System.out.println("systemThreadGroup：" + systemThreadGroup.getName());
}
```
输出结果如下：
```
currentThread：main
currentThreadGroup：main
systemThreadGroup：system
```
其中system线程组就是根节点，再上一层就没有了，如果调用会抛空指针异常。<br />线程组最主要的作用是：可以实现批量管理线程或者线程组，有效的对线程或者线程组对象进行检查、尝试中断等操作。<br />下面就一起来看看`ThreadGroup`的常用方法和使用技巧。
<a name="HCKUY"></a>
## 三、线程组用法详解
<a name="ynihT"></a>
#### 3.1、构造方法介绍
ThreadGroup提供了两个构造方法，内容如下：

| 方法 | 描述 |
| --- | --- |
| `ThreadGroup(String name)` | 根据线程组名称创建线程组，其父线程组为main线程组 |
| `ThreadGroup(ThreadGroup parent, String name)` | 根据线程组名称创建线程组，其父线程组为指定的 parent 线程组 |

其中支持指定父级线程组的方法，在实际的使用中比较常见。<br />下面，演示一下这两个构造函数的用法：
```java
public static void main(String[] args) {
    ThreadGroup subThreadGroup1 = new ThreadGroup("sub1");
    ThreadGroup subThreadGroup2 = new ThreadGroup(subThreadGroup1, "sub2");
    System.out.println("sub1 parent thread group name：" + subThreadGroup1.getParent().getName());
    System.out.println("sub2 parent thread group name：" + subThreadGroup2.getParent().getName());
}
```
输出结果如下：
```
sub1 parent thread group name：main
sub2 parent thread group name：sub1
```
<a name="IYhmb"></a>
#### 3.2、核心方法介绍
ThreadGroup提供了很多有用的方法，下面整理了一些方法的简要介绍，内容如下：

| 方法 | 描述 |
| --- | --- |
| `public final String getName()` | 返回此线程组的名称 |
| `public final ThreadGroup getParent()` | 返回此线程组的父级 |
| `public final boolean parentOf(ThreadGroup g)` | 测试此线程组是线程组参数还是其父级线程组之一 |
| `public int activeCount()` | 返回此线程组及其子组中活动线程的数量的估计值，递归遍历该线程组中所有的子组，此方法主要用于调试和监视目的 |
| `public int activeGroupCount()` | 返回此线程组及其子组中活动组的数目的估计值。递归遍历该线程组中的所有子群，此方法主要用于调试和监视目的 |
| `public final void checkAccess()` | 确定当前运行的线程是否具有修改此线程组的权限 |
| `public int enumerate(Thread[] list)` | 将这个线程组复制到它所在的组及其子组中 |
| `public final void destroy()` | 销毁此线程组及其所有子组，当线程组还要子线程或者子线程组，会抛异常 |
| `public boolean isDestroyed()` | 测试此线程组是否已被销毁 |
| `public final int getMaxPriority()` | 返回此线程组的最大优先级 |
| `public final void setMaxPriority(int pri)` | 设置组的最大优先级。线程组中具有较高优先级的线程不会受到影响 |
| `public final boolean isDaemon()` | 测试此线程组是否是守护线程组 |
| `public final void setDaemon(boolean daemon)` | 修改此线程组的守护进程状态 |
| `public final void interrupt()` | 尝试中断此线程组中的所有线程 |
| `public void list()` | 将此线程组的信息打印到标准输出。此方法仅用于调试 |

下面抽取几个比较常见的方法，进行演示介绍。
<a name="ojNRY"></a>
##### 3.2.1、`activeCount` 方法
`activeCount()`方法用于返回此线程组及其子组中活动线程的数量的估计值，因为线程的数量是动态发生变化的，返回的值只是一个估计值。<br />看一个简单的例子就知道了。
```java
public class MyThread extends Thread{

    public MyThread(ThreadGroup group, String name) {
        super(group, name);
    }

    @Override
    public void run() {
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```
```java
public class MyThreadMainTest {

    public static void main(String[] args) throws Exception {
        ThreadGroup tg = new ThreadGroup("group1");
        MyThread t1 = new MyThread (tg, "t1");
        MyThread t2 = new MyThread (tg, "t2");
        t1.start();
        t2.start();

        System.out.println("线程组的名称：" +  tg.getName() + "，活动的线程数：" +  tg.activeCount());
        Thread.sleep(1000);
        System.out.println("线程组的名称：" +  tg.getName() + "，活动的线程数：" +  tg.activeCount());
    }
}
```
输出结果如下：
```
线程组的名称：group1，活动的线程数：2
线程组的名称：group1，活动的线程数：0
```
第一次检查线程都处于运行状态，因此活动的线程数为 2；过 1 秒之后，线程运行结束，活动的线程数为 0。
<a name="AaQ3J"></a>
##### 3.2.2、`isDaemon` 方法
`setDaemon()`方法用于测试此线程组是否是守护线程组。<br />需要注意的是：后台线程组和后台线程是两个概念，后台线程组的特性是最后一个线程执行完或最后一个线程被销毁时，后台线程组自动销毁，线程组只是为了统一管理线程的一个方式，跟后台线程有区别！<br />例子如下：
```java
public class MyThread extends Thread{

    public MyThread(ThreadGroup group, String name) {
        super(group, name);
    }

    @Override
    public void run() {
        System.out.println("当前线程：" + Thread.currentThread().getName() + "，是否后台线程：" +  Thread.currentThread().isDaemon());
        System.out.println("当前线程组：" + Thread.currentThread().getThreadGroup().getName() + "，是否后台线程组：" +  Thread.currentThread().getThreadGroup().isDaemon());
    }
}
```
```java
public class MyThreadMainTest4 {

    public static void main(String[] args) throws Exception {
        ThreadGroup mainGroup = Thread.currentThread().getThreadGroup();
        new MyThread(mainGroup, "t1").start();

        Thread.sleep(100);

        // 设置守护线程组
        ThreadGroup tg = new ThreadGroup("group1");
        tg.setDaemon(true);
        new MyThread(tg,"t2").start();
    }
}
```
输出结果如下：
```
当前线程：t1，是否后台线程：false
当前线程组：main，是否后台线程组：false
当前线程：t2，是否后台线程：false
当前线程组：group1，是否后台线程组：true
```
<a name="nQW0V"></a>
##### 3.2.3、`interrupt` 方法
`interrupt()`方法用于尝试中断此线程组中的所有线程。如果正在运行的线程没有进入阻塞，是无法中断的。<br />例子如下：
```java
public class MyThreadA extends Thread{

    public MyThreadA(ThreadGroup group, String name) {
        super(group, name);
    }

    @Override
    public void run() {
        System.out.println("线程：" + Thread.currentThread().getName() + "，开始运行");
        String t;
        for (int i = 0; i < 1000000000; i++) {
            t = i + "";
        }
        System.out.println("线程：" + Thread.currentThread().getName() + "，停止运行");
    }
}
```
```java
public class MyThreadB extends Thread{

    public MyThreadB(ThreadGroup group, String name) {
        super(group, name);
    }

    @Override
    public void run() {
        System.out.println("线程：" + Thread.currentThread().getName() + "，开始运行");
        while (!Thread.interrupted()){
        }
        System.out.println("线程：" + Thread.currentThread().getName() + "，停止运行");
    }
}
```
```java
public class MyThreadC extends Thread{

    public MyThreadC(ThreadGroup group, String name) {
        super(group, name);
    }

    @Override
    public void run() {
        System.out.println("线程：" + Thread.currentThread().getName() + "，开始运行");
        try {
            Thread.sleep(1000);
        } catch (Exception e){
            //            e.printStackTrace();
        }
        System.out.println("线程：" + Thread.currentThread().getName() + "，停止运行");
    }
}
```
```java
public class MyThreadMainTest {

    public static void main(String[] args) throws Exception {
        ThreadGroup tg = new ThreadGroup("group1");
        new MyThreadA(tg,"t1").start();
        new MyThreadB(tg,"t2").start();
        new MyThreadC(tg,"t3").start();

        // 尝试中断线程组里面的线程
        tg.interrupt();
    }
}
```
输出结果如下：
```java
线程：t1，开始运行
线程：t2，开始运行
线程：t2，停止运行
线程：t3，开始运行
线程：t3，停止运行
```
线程t1只有等它运行结束，通过`interrupt()`不能中断程序！
<a name="HapZn"></a>
## 四、小结
本文主要围绕线程组的一些基本概念以及常用方法，并结合了一些简单示例进行介绍。<br />线程组的出现更多的是便于有组织的管理线程，比如 Java 的线程池就用到了线程组。
