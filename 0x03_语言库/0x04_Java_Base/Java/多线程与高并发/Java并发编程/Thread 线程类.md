JavaThread
<a name="ojlrh"></a>
## 一、简介
采用多线程的编程方式，能充分利用 CPU 资源，显著的提升程序的执行效率。<br />其中java.lang.Thread是 Java 实现多线程编程最核心的类，学习Thread类中的方法，是学习多线程的第一步。<br />下面就一起来看看，创建线程的几种方式以及Thread类中的常用方法。
<a name="ctFxN"></a>
## 二、创建线程的方式
在 JDK 1.8 版本中，创建线程总共有四种方式：

- 继承 Thread 类
- 实现 Runnable 接口
- 使用 Callable 和 Future 创建线程
- 使用 JDK 8 的 Lambda 创建线程
<a name="YYfSq"></a>
### 2.1、通过继承 Thread 创建线程
通过继承Thread类来创建线程是最简单的一种方法，继承类重写`run()`方法，然后通过线程对象实例去调用`start()`方法即可启动线程。
```java
public class MyThread extends Thread{

    @Override
    public void run() {
        System.out.println(Thread.currentThread().getName() + "在运行！");
    }
}
```
```java
MyThread thread = new MyThread();
thread.start();
```
<a name="GiStA"></a>
### 2.2、通过实现 `Runnable` 接口创建线程
通过实现`Runnable`接口来创建线程也是最简单的一种方法，同时也是最常用的一种方式。<br />开发者只需要实现`Runnable`接口，然后通过一个`Thread`类来启动。
```java
public class MyThread implements Runnable{

    @Override
    public void run() {
        System.out.println(Thread.currentThread().getName() + "在运行！");
    }
}
```
```java
Thread thread = new Thread(new MyThread());
thread.start();
```
<a name="SvsLU"></a>
###  2.3、使用 `Callable` 和 `Future` 创建线程
相比通过实现`Runnable`接口来创建线程，使用`Callable`和`Future`组合来创建线程可以实现获取子线程执行结果，弥补了调用线程没有返回值的情况，可以看做是`Runnable`的一个补充，`Callable`和`Future`是 JDK1.5 版本中加入的。
```java
public class MyThread implements Callable<String> {

    @Override
    public String call() throws Exception {
        System.out.println(Thread.currentThread().getName() + "在运行！");
        return Thread.currentThread().getName();
    }
}
```
```java
Callable<String> callable = new MyThread();
FutureTask<String> ft = new FutureTask<>(callable);
new Thread(ft).start();
// 通过阻塞方式获取线程执行结果
System.out.println(ft.get());
```
<a name="NiOtu"></a>
### 2.4、使用 JDK 8 的 Lambda 创建线程
Lambda 表达式，是从 JDK1.8 版本开始加入的，可以看作成通过实现`Runnable`接口创建线程的一种简写。
```java
new Thread(()-> System.out.println(Thread.currentThread().getName() + "在运行！")).start();
```
<a name="L60aX"></a>
### 2.5、创建线程几种方式的对比
以上四种方式都可以创建线程，使用继承`Thread`类的方式创建线程时，编写简单，如果需要访问当前线程，无需使用`Thread.currentThread()`方法，直接使用this即可获得当前线程。<br />采用实现`Runnable`、`Callable`接口的方式创建线程时，线程类只是实现了 `Runnable`或`Callable`接口，同时还可以继承其他类，最后通过`Thread`类来启动线程。它也是最常用的一种创建线程方式，通过接口方式来编程，可以实现代码更加统一。<br />其实通过继承`Thread`类创建线程的方式，本质上也可以看成实现了`Runnable`接口的一个实例，打开源码`Thread`，你会发现这一点。
```java
public class Thread implements Runnable {

    //省略...
}
```
**需要特别注意的地方是**：**真正启动线程的是**`**start()**`**方法而不是**`**run()**`**方法，单独调用**`**run()**`**方法和调用普通的成员方法一样，不能启动线程**。
<a name="WKcNx"></a>
## 三、Thread 常用方法介绍
Thread 类常用的方法主要有三大块：

- 构造方法
- 实例方法
- 静态方法
<a name="oFTrb"></a>
### 3.1、构造方法
在 JDK 中，Thread 类提供了如下几个常用的构造方法来创建线程。

| 方法 | 描述 |
| --- | --- |
| `Thread()` | 创建一个默认设置的线程实例，线程名称采用自增ID命名 |
| `Thread(Runnable target)` | 创建一个包含可执行对象的线程实例 |
| `Thread(Runnable target, String name)` | 创建一个包含可执行对象，指定名称的线程实例 |
| `Thread(String name)` | 创建一个指定名称的线程实例 |
| `Thread(ThreadGroup group, String name)` | 创建一个指定线程组，线程名称的线程实例 |
| `Thread(ThreadGroup group, Runnable target)` | 创建一个指定线程组，包含可执行对象的线程实例 |
| `Thread(ThreadGroup group, Runnable target, String name)` | 创建一个指定线程组，包含可执行对象，指定线程名称的线程实例 |
| `Thread(ThreadGroup group, Runnable target, String name, long stackSize)` | 创建一个指定线程组，包含可执行对象，指定名称以及堆栈大小的线程实例 |

其中`Thread(Runnable target)`构造方法最常见。
```java
Thread thread = new Thread(new Runnable() {

    @Override
    public void run() {
        System.out.println(Thread.currentThread().getName());
    }
});
thread.start();
```
其次`Thread(Runnable target, String name)`构造方法，可以指定线程名称。
```java
Thread thread = new Thread(new Runnable() {

    @Override
    public void run() {
        System.out.println(Thread.currentThread().getName());
    }
}, "thread-demo");
thread.start();
```
同时，还支持指定线程组来创建线程。
```java
// 创建一个线程组实例
ThreadGroup tg = new ThreadGroup("线程组1");
// 创建一个线程实例
Thread thread = new Thread(tg,new Runnable() {

    @Override
    public void run() {
        System.out.println(Thread.currentThread().getThreadGroup().getName() + "：" + Thread.currentThread().getName());
    }
}, "thread-demo");
thread.start();
```
如果不显式指定线程组，JVM 会将创建的线程归到当前线程所属的线程组中。
<a name="hcAds"></a>
###  3.2、实例方法
在 Java 中，实例方法只有实例对象才能调用，也就是`new`出来的对象或者反射出来的对象，类是无法直接调用的。<br />在 JDK 中，Thread 类提供了如下几个常用的实例方法来操作线程。

| 方法 | 描述 |
| --- | --- |
| `public void start()` | 启动线程 |
| `public void run()` | 线程进入可运行状态时，jvm 会调用该线程的 run 方法；单独调用 run 方法，不能启动线程 |
| `public final void setName(String name)` | 设置线程名称 |
| `public final void setPriority(int priority)` | 设置线程优先级，默认5，取值1-10 |
| `public final void setDaemon(boolean on)` | 设置线程为守护线程或用户线程，默认是用户线程 |
| `public final void join(long millisec)` | 挂起线程 xx 毫秒，参数可以不传 |
| `public void interrupt()` | 当线程受到阻塞时，调用此方法会抛出一个中断信号，让线程退出阻塞状态 |
| `public final boolean isAlive()` | 测试线程是否处于活动状态 |

下面依次来看看它们之间的用法。
<a name="hMWh2"></a>
#### 3.2.1、`start()`
`start()`方法，简单的说就是启动线程，至于什么时候能运行，需要等待获取 CPU 时间片，然后调用线程对象的`run()`方法，产生一个异步执行的效果。<br />样例代码如下：
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        for (int i = 0; i < 5; i++) {
            String time = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS").format(new Date());
            System.out.println(time + " 当前线程：" + Thread.currentThread().getName() + "，正在运行");
        }
    }
}
```
```java
public class ThreadB extends Thread {

    @Override
    public void run() {
        for (int i = 0; i < 5; i++) {
            String time = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS").format(new Date());
            System.out.println(time + " 当前线程：" + Thread.currentThread().getName() + "，正在运行");
        }
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) {
        ThreadA threadA = new ThreadA();
        ThreadB threadB = new ThreadB();

        threadA.start();
        threadB.start();
    }
}
```
运行结果：
```
2023-08-30 15:51:43:331 当前线程：Thread-1，正在运行
2023-08-30 15:51:43:331 当前线程：Thread-1，正在运行
2023-08-30 15:51:43:332 当前线程：Thread-0，正在运行
2023-08-30 15:51:43:332 当前线程：Thread-1，正在运行
2023-08-30 15:51:43:332 当前线程：Thread-0，正在运行
2023-08-30 15:51:43:332 当前线程：Thread-1，正在运行
2023-08-30 15:51:43:332 当前线程：Thread-0，正在运行
2023-08-30 15:51:43:332 当前线程：Thread-1，正在运行
2023-08-30 15:51:43:333 当前线程：Thread-0，正在运行
2023-08-30 15:51:43:333 当前线程：Thread-0，正在运行
```
结果很明显，**CPU 什么时候执行线程的**`**run()**`**方法具有不确定，同时执行线程顺序也具有不确定性**，这是采用多线程异步执行程序的一个主要特征。
<a name="JEo0V"></a>
#### 3.2.2、`run()`
如果单独调用`run()`方法，不能启动线程，会像调用普通的成员方法一样，可以将上面例子中的`threadA.start()`改成`threadA.run()`，再看看结果如何。
```java
public class ThreadTest {

    public static void main(String[] args) {
        ThreadA threadA = new ThreadA();
        ThreadB threadB = new ThreadB();

        threadA.run();
        threadB.run();
    }
}
```
运行结果：
```
2023-08-30 16:14:50:983 当前线程：main，正在运行
2023-08-30 16:14:50:984 当前线程：main，正在运行
2023-08-30 16:14:50:985 当前线程：main，正在运行
2023-08-30 16:14:50:985 当前线程：main，正在运行
2023-08-30 16:14:50:985 当前线程：main，正在运行
2023-08-30 16:14:50:986 当前线程：main，正在运行
2023-08-30 16:14:50:986 当前线程：main，正在运行
2023-08-30 16:14:50:986 当前线程：main，正在运行
2023-08-30 16:14:50:987 当前线程：main，正在运行
2023-08-30 16:14:50:987 当前线程：main，正在运行
```
结果很明显，单独调用`Thread`类实例`run()`方法，是没有任何异步效果的，全部被主线程执行。
<a name="ICKqc"></a>
####  3.2.3、`setName()`
`setName()`方法，简而言之就是设置线程名称，如果不手动设置，创建线程的时候 JDK 会给一个默认的线程名称，从 0 开始依次自增。<br />开发者可以通过`getName()`方法获取线程名称，也可以通过`getId()`获取当前线程的唯一标记，这个值用户无法手动设置，由`Thread`类自动生成。<br />样例代码如下：
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        long threadId = Thread.currentThread().getId();
        String threadName = Thread.currentThread().getName();
        System.out.println("threadId:" + threadId + "，threadName:" + threadName);
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args)  {
        ThreadA threadA = new ThreadA();
        threadA.setName("thread-a");

        threadA.start();
    }
}
```
运行结果：
```
threadId:10，threadName:thread-a
```
<a name="GvaLS"></a>
#### 3.2.4、`setPriority()`
`setPriority()`方法的作用是设置线程的优先级，取值范围：1～ 10，与此对应的还有`getPriority()`方法，用于获取线程的优先级。优先级越高，拥有优先获取 CPU 执行的优势。<br />换句话说，当有两个线程在等待 CPU 执行时，优先级高的线程越容易被 CPU 选择执行。<br />样例代码如下：
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        String threadName = Thread.currentThread().getName();
        int priority = Thread.currentThread().getPriority();
        System.out.println("threadName:" + threadName + "，priority：" +  priority);
    }
}
```
```java
public class ThreadB extends Thread {

    @Override
    public void run() {
        String threadName = Thread.currentThread().getName();
        int priority = Thread.currentThread().getPriority();
        System.out.println("threadName:" + threadName + "，priority：" +  priority);
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) {
        for (int i = 0; i < 5; i++) {
            ThreadA threadA = new ThreadA();
            ThreadB threadB = new ThreadB();

            threadA.start();
            threadB.start();
        }
    }
}
```
运行结果：
```
threadName:Thread-0，priority：5
threadName:Thread-1，priority：5
threadName:Thread-2，priority：5
threadName:Thread-3，priority：5
threadName:Thread-4，priority：5
threadName:Thread-5，priority：5
threadName:Thread-6，priority：5
threadName:Thread-7，priority：5
threadName:Thread-8，priority：5
threadName:Thread-9，priority：5
```
**线程默认优先级为 5，如果不手动指定，那么线程优先级具有继承性**，比如线程 A 启动线程 B，那么线程 B 的优先级和线程 A 的优先级相同。<br />如果手动设置优先级，再看看结果如何。
```java
public class ThreadTest {

    public static void main(String[] args) {
        for (int i = 0; i < 5; i++) {
            ThreadA threadA = new ThreadA();
            ThreadB threadB = new ThreadB();

            threadA.setPriority(10);
            threadA.start();

            threadB.setPriority(1);
            threadB.start();
        }
    }
}
```
运行结果：
```
threadName:Thread-0，priority：10
threadName:Thread-1，priority：10
threadName:Thread-2，priority：10
threadName:Thread-3，priority：10
threadName:Thread-4，priority：1
threadName:Thread-5，priority：10
threadName:Thread-6，priority：1
threadName:Thread-7，priority：1
threadName:Thread-8，priority：1
threadName:Thread-9，priority：1
```
将线程实例threadB的优先级调整到最高，拥有优先被 CPU 执行的优势。<br />在实测过程中，可能有的同学感觉效果并不明显，如果你的电脑 CPU 是多核的，线程数量较少的情况，可能会被多个 CPU 并行执行，具体执行环境取决于 CPU 。<br />需要特别注意的是：**设置优先级只是很大程度上让某个线程尽可能获得比较多的执行机会，操作系统不能保证设置了优先级高的线程就一定会先运行或得到更多的 CPU 时间，具体执行哪一个线程，最终还是由 CPU 来决定**。<br />另外有些 Linux 操作系统是不区分优先级的，它把所有优先级都视为 5。<br />`setPriority()`方法在实际的开发中，使用的并不多见。
<a name="IVOC8"></a>
#### 3.2.5、`setDaemon()`
在 Java 中线程分为两种，一种是用户线程，一种是守护线程。<br />守护线程是一种特殊的线程，它的作用是为其他线程的运行提供便利的服务，比如垃圾回收线程，就是最典型的守护线程。<br />当 JVM 检测到应用程序中的所有线程都只有守护线程时，它将退出应用程序，因为没有存在的必要，服务的对象都没了，当然就需要销毁了。<br />开发者可以通过使用`setDaemon()`方法，传递`true`作为参数，使线程成为一个守护线程，同时可以使用`isDaemon()`方法来检查线程是否是守护线程。<br />样例代码如下：
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        try {
            while (true){
                String threadName = Thread.currentThread().getName();
                boolean isDaemon = Thread.currentThread().isDaemon();
                System.out.println("threadName:" + threadName + ",isDaemon:" + isDaemon);
                Thread.sleep(500);
            }
        } catch (Exception e){
            e.printStackTrace();
        }
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        threadA.setDaemon(true);
        threadA.start();

        Thread.sleep(3000);
        System.out.println("主线程方法执行完毕！");
    }
}
```
运行结果：
```
threadName:Thread-0,isDaemon:true
threadName:Thread-0,isDaemon:true
threadName:Thread-0,isDaemon:true
threadName:Thread-0,isDaemon:true
threadName:Thread-0,isDaemon:true
threadName:Thread-0,isDaemon:true
主线程方法执行完毕！
```
需要特别注意的是：创建守护线程时，`setDaemon(true)`方法必须在线程`start()`方法之前，否则会抛异常。
<a name="vMfUO"></a>
#### 3.2.6、`join()`
`join()`方法的作用是让调用此方法的主线程被阻塞，仅当该方法执行完成以后，才能继续运行。<br />从概念上感觉很抽象，看一下例子！
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        try {
            String time = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
            System.out.println(time + " 当前线程：" + Thread.currentThread().getName() + "，正在运行");
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        threadA.start();

        // 让执行这个方法的线程阻塞（指的是主线程，不是threadA线程）
        threadA.join();

        String time = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
        System.out.println(time + " 主线程方法执行完毕！");
    }
}
```
运行结果：
```
2023-08-31 12:46:06 当前线程：Thread-0，正在运行
2023-08-31 12:46:09 主线程方法执行完毕！
```
从运行结果可以得出一个结论，主线程`main`调用`threadA.join()`方法时，会进入阻塞状态，直到线程实例`threadA`的`run()`方法执行完毕，主线程main从阻塞状态变成可运行状态。<br />此例中主线程main会无限期阻塞直到`threadA.run()`方法执行完毕。<br />比如某个业务场景下，主线程main的执行时间是 1s，子线程的执行时间是 10s，同时主线程依赖子线程执行完的结果，此时让主线程执行`join()`方法进行适度阻塞，可以实现此目标。
<a name="OhvrP"></a>
#### 3.2.7、`interrupt()`
`interrupt()`方法的作用是当线程受到阻塞时，调用此方法会抛出一个中断信号，让线程退出阻塞状态，如果当前线程没有阻塞，是无法中断线程的。<br />与此对应的还有`isInterrupted()`方法，用于检查线程是否已经中断，但不清除状态标识。<br />先看一个例子！
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        for (int i = 0; i < 10000; i++) {
            String time = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS").format(new Date());
            System.out.println(time + " 当前线程：" + Thread.currentThread().getName() + "，count:" + i);
        }
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        threadA.start();

        Thread.sleep(50);

        // 检查线程是否中断，没有尝试终止线程
        if(!threadA.isInterrupted()){
            threadA.interrupt();
        }
    }
}
```
运行结果：
```
2023-08-31 14:46:55:053 当前线程：Thread-0，count:0
2023-08-31 14:46:55:054 当前线程：Thread-0，count:1
...
2023-08-31 14:46:55:839 当前线程：Thread-0，count:9999
```
如果当前线程没有阻塞，调用`interrupt()`起不到任何效果。<br />下面对ThreadA类在尝试改造一下，让它每执行一次停顿 1 秒，内容如下：
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        try {
            for (int i = 0; i < 10; i++) {
                String time = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS").format(new Date());
                System.out.println(time + " 当前线程：" + Thread.currentThread().getName() + "，count:" + i);
                Thread.sleep(1000);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        threadA.start();

        Thread.sleep(2000);

        // 检查线程是否中断，没有尝试终止线程
        if(!threadA.isInterrupted()){
            threadA.interrupt();
        }
    }
}
```
运行结果：
```
2023-08-31 14:51:19:792 当前线程：Thread-0，count:0
2023-08-31 14:51:20:798 当前线程：Thread-0，count:1
java.lang.InterruptedException: sleep interrupted
 at java.lang.Thread.sleep(Native Method)
 at com.example.thread.ThreadA.run(ThreadA.java:22)
```
很明显，当线程处于阻塞状态时，调用`interrupt()`方法，可以让线程退出阻塞，起到终止线程的效果。
<a name="INp4R"></a>
#### 3.2.8、`isAlive()`
`isAlive()`方法的作用是检查线程是否处于活动状态，只要线程启动且没有终止，方法返回的就是`true`。<br />看一下例子！
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        System.out.println("当前线程：" + Thread.currentThread().getName() + "，isAlive：" + Thread.currentThread().isAlive());
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        System.out.println("begin == " + threadA.isAlive());

        threadA.start();

        Thread.sleep(1000);
        System.out.println("end == " + threadA.isAlive());
    }
}
```
运行结果：
```
begin == false
当前线程：Thread-0，isAlive：true
end == false
```
从运行结果上可以看出，线程启动前`isAlive=false`，线程运行中`isAlive=true`，线程运行完成`isAlive=false`。
<a name="aEvIR"></a>
### 3.3、静态方法
在 JDK 中，Thread 类还提供了如下几个常用的静态方法来操作线程。

| 方法 | 描述 |
| --- | --- |
| `public static Thread currentThread()` | 返回对当前正在执行的线程对象的引用 |
| `public static void yield()` | 暂停当前正在执行的线程对象，并执行其他线程 |
| `public static void sleep(long millisec)` | 在指定的毫秒数内让当前正在执行的线程休眠（暂停执行），此操作受到系统计时器和调度程序精度和准确性的影响 |
| `public static boolean holdsLock(Object x)` | 当且仅当当前线程在指定的对象上保持监视器锁时，才返回 true |
| `public static void dumpStack()` | 将当前线程的堆栈跟踪打印至标准错误流 |

下面依次来看看它们之间的用法。
<a name="ZQCjz"></a>
#### 3.3.1、`currentThread()`
`currentThread()`方法的作用是返回当前正在执行线程对象的引用，在上文中有所介绍。<br />下面再来看看几个例子！
```java
public class ThreadA extends Thread {

    static {
        System.out.println("静态块打印的线程名称：" + Thread.currentThread().getName());
    }

    public ThreadA() {
        System.out.println("构造方法打印的线程名称：" + Thread.currentThread().getName());
    }

    @Override
    public void run() {
        System.out.println("run()方法打印的线程名称：" + Thread.currentThread().getName());
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        threadA.start();
    }
}
```
运行结果：
```
静态块打印的线程名称：main
构造方法打印的线程名称：main
run()方法打印的线程名称：Thread-0
```
从运行结果可以看出，线程类的构造方法、静态块是被主线程main调用的，而线程类的`run()`方法才是用户线程自己调用的。<br />再来看看另一个例子！
```java
public class ThreadA extends Thread {

    public ThreadA() {
        System.out.println("构造方法打印 Begin...");
        System.out.println("Thread.currentThread打印的线程名称：" + Thread.currentThread().getName());
        System.out.println("this.getName打印的线程名称：" + this.getName());
        System.out.println("构造方法打印 end...");
    }

    @Override
    public void run() {
        System.out.println("run()方法打印 Begin...");
        System.out.println("Thread.currentThread打印的线程名称：" + Thread.currentThread().getName());
        System.out.println("this.getName打印的线程名称：" + this.getName());
        System.out.println("run()方法打印 end...");
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        System.out.println("===============");
        threadA.start();
    }
}
```
运行结果如下：
```
构造方法打印 Begin...
Thread.currentThread打印的线程名称：main
this.getName打印的线程名称：Thread-0
构造方法打印 end...
===============
run()方法打印 Begin...
Thread.currentThread打印的线程名称：Thread-0
this.getName打印的线程名称：Thread-0
run()方法打印 end...
```
从运行结果可以看出，`Thread.currentThread`方法返回的未必是Thread本身，而是当前正在执行线程对象的引用，这和通过`this.XXX()`返回的对象是有区别的。
<a name="w5FT3"></a>
####  3.3.2、`yield()`
`yield()`方法的作用是暂停当前执行的线程对象，并执行其他线程。这个暂停会放弃 CPU 资源，并且放弃 CPU 的时间不确定，有可能刚放弃，就获得 CPU 资源了，也有可能放弃好一会儿，才会被 CPU 执行。<br />相关例子如下。
```java
public class ThreadA extends Thread {

    private String name;

    public ThreadA(String name) {
        this.name = name;
    }

    @Override
    public void run() {
        for (int i = 0; i < 5; i++) {
            System.out.println(name  + ":" + i);
            if ("t1".equals(name)) {
                System.out.println(name  + ":" + i +"......yield.............");
                Thread.yield();
            }
        }
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA1 = new ThreadA("t1");
        ThreadA threadA2 = new ThreadA("t2");

        threadA1.start();
        threadA2.start();
    }
}
```
运行结果：
```
t2:0
t1:0
t2:1
t2:2
t2:3
t2:4
t1:0......yield.............
t1:1
t1:1......yield.............
t1:2
t1:2......yield.............
t1:3
t1:3......yield.............
t1:4
t1:4......yield.............
```
从运行结果上可以看出，调用`yield()`方法可以让线程放弃 CPU 资源，循环次数越多，越明显。
<a name="pw5nS"></a>
#### 3.3.3、`sleep()`
`sleep()`方法的作用是在指定的毫秒数内让当前正在执行的线程休眠（暂停执行），此操作受到系统计时器和调度程序精度和准确性的影响。这个**正在执行的线程**指的是`Thread.currentThread()`返回的线程。<br />根据 JDK API 的说法，该线程不丢失任何监视器的所属权，换句话说就是不会释放锁，如果`sleep()`代码上下文被加锁了，锁依然在，只是 CPU 资源会让出给其他线程。<br />相关例子如下。
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        try {
            String begin = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS").format(new Date());
            System.out.println(begin + " 当前线程：" + Thread.currentThread().getName());

            Thread.sleep(3000);

            String end = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS").format(new Date());
            System.out.println(end + " 当前线程：" + Thread.currentThread().getName());

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        threadA.start();
    }
}
```
运行结果如下：
```
2023-08-31 18:06:41:459 当前线程：Thread-0
2023-08-31 18:06:44:464 当前线程：Thread-0
```
<a name="m6e23"></a>
#### 3.3.4、`holdsLock()`
`holdsLock()`方法表示当且仅当**当前线程在指定的对象上保持监视器锁时，才返回 true**，简单的说就是检测一个线程是否拥有锁。<br />相关例子如下。
```java
public class ThreadA extends Thread {

    private String lock = "lock";

    @Override
    public void run() {
        System.out.println("当前线程：" + Thread.currentThread().getName() + "，Holds Lock = " + Thread.holdsLock(lock));

        synchronized (lock){
            System.out.println("当前线程：" + Thread.currentThread().getName() + "，Holds Lock = " + Thread.holdsLock(lock));
        }
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        threadA.start();
    }
}
```
运行结果如下：
```
当前线程：Thread-0，Holds Lock = false
当前线程：Thread-0，Holds Lock = true
```
<a name="TD3Lp"></a>
#### 3.3.5、`dumpStack()`
`dumpStack()`方法的作用是将当前线程的堆栈跟踪打印至标准错误流。此方法仅用于调试。<br />相关例子如下。
```java
public class ThreadA extends Thread {

    @Override
    public void run() {
        System.out.println("当前线程：" + Thread.currentThread().getName());
        Thread.dumpStack();
    }
}
```
```java
public class ThreadTest {

    public static void main(String[] args) throws InterruptedException {
        ThreadA threadA = new ThreadA();
        threadA.start();
    }
}
```
运行结果如下：
```
当前线程：Thread-0
java.lang.Exception: Stack trace
 at java.lang.Thread.dumpStack(Thread.java:1336)
 at com.example.thread.ThreadA.run(ThreadA.java:16)
```
`Thread.dumpStack`会将当前线程的堆栈跟踪信息打印出控制台。
<a name="Nb8Q4"></a>
## 四、小结
主要围绕线程类Thread相关的常用方法进行详解。
