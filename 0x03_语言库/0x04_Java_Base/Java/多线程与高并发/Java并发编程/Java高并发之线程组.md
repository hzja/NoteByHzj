Java
<a name="CM5g6"></a>
## **线程组**
可以把线程归属到某个线程组中，线程组可以包含多个**线程**以及**线程组**，线程和线程组组成了父子关系，是个树形结构，如下图：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1691504044036-87f4db4a-b7c8-40ac-b76d-81e8df408ba1.png#averageHue=%23fafafa&clientId=uaca993a4-6485-4&from=paste&height=626&id=u4d147b44&originHeight=1564&originWidth=2119&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=280109&status=done&style=none&taskId=u1f430142-46aa-410e-91a8-bc8ab51d21d&title=&width=847.6)<br />使用线程组可以方便管理线程，线程组提供了一些方法方便方便管理线程。
<a name="DOAbJ"></a>
## **创建线程关联线程组**
创建线程的时候，可以给线程指定一个线程组，代码如下：
```java
import java.util.concurrent.TimeUnit;

public class Demo1 {
    public static class R1 implements Runnable {
        @Override
        public void run() {
            System.out.println("threadName:" + Thread.currentThread().getName());
            try {
                TimeUnit.SECONDS.sleep(3);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    public static void main(String[] args) throws InterruptedException {
        ThreadGroup threadGroup = new ThreadGroup("thread-group-1");
        Thread t1 = new Thread(threadGroup, new R1(), "t1");
        Thread t2 = new Thread(threadGroup, new R1(), "t2");
        t1.start();
        t2.start();
        TimeUnit.SECONDS.sleep(1);
        System.out.println("活动线程数:" + threadGroup.activeCount());
        System.out.println("活动线程组:" + threadGroup.activeGroupCount());
        System.out.println("线程组名称:" + threadGroup.getName());
    }
}
```
输出结果：
```
threadName:t1
threadName:t2
活动线程数:2
活动线程组:0
线程组名称:thread-group-1
```
`**activeCount()**`方法可以返回线程组中的所有活动线程数，包含下面的所有子孙节点的线程，由于线程组中的线程是动态变化的，这个值只能是一个估算值。
<a name="B1E3y"></a>
## **为线程组指定父线程组**
创建线程组的时候，可以给其指定一个父线程组，也可以不指定，如果不指定父线程组，则父线程组为当前线程的线程组，java api有2个常用的构造方法用来创建线程组：
```java
public ThreadGroup(String name) 

public ThreadGroup(ThreadGroup parent, String name)
```
第一个构造方法未指定父线程组，看一下内部的实现：
```java
public ThreadGroup(String name) {
    this(Thread.currentThread().getThreadGroup(), name);
}
```
系统自动获取当前线程的线程组作为默认父线程组。<br />上一段示例代码：
```java
import java.util.concurrent.TimeUnit;

public class Demo2 {
    public static class R1 implements Runnable {
        @Override
        public void run() {
            Thread thread = Thread.currentThread();
            System.out.println("所属线程组:" + thread.getThreadGroup().getName() + ",线程名称:" + thread.getName());
            try {
                TimeUnit.SECONDS.sleep(3);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    public static void main(String[] args) throws InterruptedException {
        ThreadGroup threadGroup1 = new ThreadGroup("thread-group-1");
        Thread t1 = new Thread(threadGroup1, new R1(), "t1");
        Thread t2 = new Thread(threadGroup1, new R1(), "t2");
        t1.start();
        t2.start();
        TimeUnit.SECONDS.sleep(1);
        System.out.println("threadGroup1活动线程数:" + threadGroup1.activeCount());
        System.out.println("threadGroup1活动线程组:" + threadGroup1.activeGroupCount());
        System.out.println("threadGroup1线程组名称:" + threadGroup1.getName());
        System.out.println("threadGroup1父线程组名称:" + threadGroup1.getParent().getName());
        System.out.println("----------------------");
        ThreadGroup threadGroup2 = new ThreadGroup(threadGroup1, "thread-group-2");
        Thread t3 = new Thread(threadGroup2, new R1(), "t3");
        Thread t4 = new Thread(threadGroup2, new R1(), "t4");
        t3.start();
        t4.start();
        TimeUnit.SECONDS.sleep(1);
        System.out.println("threadGroup2活动线程数:" + threadGroup2.activeCount());
        System.out.println("threadGroup2活动线程组:" + threadGroup2.activeGroupCount());
        System.out.println("threadGroup2线程组名称:" + threadGroup2.getName());
        System.out.println("threadGroup2父线程组名称:" + threadGroup2.getParent().getName());
        System.out.println("----------------------");
        System.out.println("threadGroup1活动线程数:" + threadGroup1.activeCount());
        System.out.println("threadGroup1活动线程组:" + threadGroup1.activeGroupCount());
        System.out.println("----------------------");
        threadGroup1.list();
    }
}
```
输出结果：
```
所属线程组:thread-group-1,线程名称:t1
所属线程组:thread-group-1,线程名称:t2
threadGroup1活动线程数:2
threadGroup1活动线程组:0
threadGroup1线程组名称:thread-group-1
threadGroup1父线程组名称:main
----------------------
所属线程组:thread-group-2,线程名称:t4
所属线程组:thread-group-2,线程名称:t3
threadGroup2活动线程数:2
threadGroup2活动线程组:0
threadGroup2线程组名称:thread-group-2
threadGroup2父线程组名称:thread-group-1
----------------------
threadGroup1活动线程数:4
threadGroup1活动线程组:1
----------------------
java.lang.ThreadGroup[name=thread-group-1,maxpri=10]
    Thread[t1,5,thread-group-1]
    Thread[t2,5,thread-group-1]
    java.lang.ThreadGroup[name=thread-group-2,maxpri=10]
        Thread[t3,5,thread-group-2]
        Thread[t4,5,thread-group-2]
```
代码解释：

1. **threadGroup1未指定父线程组，系统获取了主线程的线程组作为threadGroup1的父线程组，输出结果中是：main**
2. **threadGroup1为threadGroup2的父线程组**
3. **threadGroup1活动线程数为4，包含了threadGroup1线程组中的t1、t2，以及子线程组threadGroup2中的t3、t4**
4. **线程组的list()方法，将线程组中的所有子孙节点信息输出到控制台，用于调试使用**
<a name="AMZRb"></a>
## **根线程组**
获取根线程组
```java
public class Demo3 {
    public static void main(String[] args) {
        System.out.println(Thread.currentThread());
        System.out.println(Thread.currentThread().getThreadGroup());
        System.out.println(Thread.currentThread().getThreadGroup().getParent());
        System.out.println(Thread.currentThread().getThreadGroup().getParent().getParent());
    }
}
```
运行上面代码，输出：
```
Thread[main,5,main]
java.lang.ThreadGroup[name=main,maxpri=10]
java.lang.ThreadGroup[name=system,maxpri=10]
null
```
从上面代码可以看出：

1. **主线程的线程组为main**
2. **根线程组为system**

看一下ThreadGroup的源码：
```java
private ThreadGroup() {     // called from C code
    this.name = "system";
    this.maxPriority = Thread.MAX_PRIORITY;
    this.parent = null;
}
```
发现ThreadGroup默认构造方法是private的，是由c调用的，创建的正是system线程组。
<a name="BrE2T"></a>
## **批量停止线程**
调用线程组`**interrupt()**`，会将线程组树下的所有子孙线程中断标志置为true，可以用来批量中断线程。<br />示例代码：
```java
import java.util.concurrent.TimeUnit;

public class Demo4 {
    public static class R1 implements Runnable {
        @Override
        public void run() {
            Thread thread = Thread.currentThread();
            System.out.println("所属线程组:" + thread.getThreadGroup().getName() + ",线程名称:" + thread.getName());
            while (!thread.isInterrupted()) {
                ;
            }
            System.out.println("线程:" + thread.getName() + "停止了！");
        }
    }
    public static void main(String[] args) throws InterruptedException {
        ThreadGroup threadGroup1 = new ThreadGroup("thread-group-1");
        Thread t1 = new Thread(threadGroup1, new R1(), "t1");
        Thread t2 = new Thread(threadGroup1, new R1(), "t2");
        t1.start();
        t2.start();
        ThreadGroup threadGroup2 = new ThreadGroup(threadGroup1, "thread-group-2");
        Thread t3 = new Thread(threadGroup2, new R1(), "t3");
        Thread t4 = new Thread(threadGroup2, new R1(), "t4");
        t3.start();
        t4.start();
        TimeUnit.SECONDS.sleep(1);
        System.out.println("-----------threadGroup1信息-----------");
        threadGroup1.list();
        System.out.println("----------------------");
        System.out.println("停止线程组：" + threadGroup1.getName() + "中的所有子孙线程");
        threadGroup1.interrupt();
        TimeUnit.SECONDS.sleep(2);
        System.out.println("----------threadGroup1停止后，输出信息------------");
        threadGroup1.list();
    }
}
```
输出：
```
所属线程组:thread-group-1,线程名称:t1
所属线程组:thread-group-1,线程名称:t2
所属线程组:thread-group-2,线程名称:t3
所属线程组:thread-group-2,线程名称:t4
-----------threadGroup1信息-----------
java.lang.ThreadGroup[name=thread-group-1,maxpri=10]
    Thread[t1,5,thread-group-1]
    Thread[t2,5,thread-group-1]
    java.lang.ThreadGroup[name=thread-group-2,maxpri=10]
        Thread[t3,5,thread-group-2]
        Thread[t4,5,thread-group-2]
----------------------
停止线程组：thread-group-1中的所有子孙线程
线程:t4停止了！
线程:t2停止了！
线程:t1停止了！
线程:t3停止了！
----------threadGroup1停止后，输出信息------------
java.lang.ThreadGroup[name=thread-group-1,maxpri=10]
    java.lang.ThreadGroup[name=thread-group-2,maxpri=10]
```
停止线程之后，通过`**list()**`方法可以看出输出的信息中不包含已结束的线程了。<br />多说几句，建议大家再创建线程或者线程组的时候，给他们取一个有意义的名字，对于计算机来说，可能名字并不重要，但是在系统出问题的时候，可能会去查看线程堆栈信息，如果看到的都是t1、t2、t3，估计自己也比较崩溃，如果看到的是httpAccpHandler、dubboHandler类似的名字，应该会好很多。
