Java
<a name="GBA6f"></a>
## 主要内容

1. 需要解决的问题
2. 介绍`ThreadLocal`
3. 介绍`InheritableThreadLocal`
<a name="GEUGK"></a>
## 需要解决的问题
还是以解决问题的方式来引出`ThreadLocal`、`InheritableThreadLocal`，这样印象会深刻一些。<br />目前java开发web系统一般有3层，Controller、Service、Dao，请求到达Controller，Controller调用Service，Service调用Dao，然后进行处理。<br />写一个简单的例子，有3个方法分别模拟Controller、Service、Dao。代码如下：
```java
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class Demo1 {

    static AtomicInteger threadIndex = new AtomicInteger(1);
    //创建处理请求的线程池子
    static ThreadPoolExecutor disposeRequestExecutor = new ThreadPoolExecutor(3,
            3,
            60,
            TimeUnit.SECONDS,
            new LinkedBlockingDeque<>(),
            r -> {
                Thread thread = new Thread(r);
                thread.setName("disposeRequestThread-" + threadIndex.getAndIncrement());
                return thread;
            });

    //记录日志
    public static void log(String msg) {
        StackTraceElement stack[] = (new Throwable()).getStackTrace();
        System.out.println("****" + System.currentTimeMillis() + ",[线程:" + Thread.currentThread().getName() + "]," + stack[1] + ":" + msg);
    }

    //模拟controller
    public static void controller(List<String> dataList) {
        log("接受请求");
        service(dataList);
    }

    //模拟service
    public static void service(List<String> dataList) {
        log("执行业务");
        dao(dataList);
    }

    //模拟dao
    public static void dao(List<String> dataList) {
        log("执行数据库操作");
        //模拟插入数据
        for (String s : dataList) {
            log("插入数据" + s + "成功");
        }
    }

    public static void main(String[] args) {
        //需要插入的数据
        List<String> dataList = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            dataList.add("数据" + i);
        }

        //模拟5个请求
        int requestCount = 5;
        for (int i = 0; i < requestCount; i++) {
            disposeRequestExecutor.execute(() -> {
                controller(dataList);
            });
        }

        disposeRequestExecutor.shutdown();
    }
}
```
运行结果：
```
****1565338891286,[线程:disposeRequestThread-2],com.example.chat24.Demo1.controller(Demo1.java:36):接受请求
****1565338891286,[线程:disposeRequestThread-1],com.example.chat24.Demo1.controller(Demo1.java:36):接受请求
****1565338891287,[线程:disposeRequestThread-2],com.example.chat24.Demo1.service(Demo1.java:42):执行业务
****1565338891287,[线程:disposeRequestThread-1],com.example.chat24.Demo1.service(Demo1.java:42):执行业务
****1565338891287,[线程:disposeRequestThread-3],com.example.chat24.Demo1.controller(Demo1.java:36):接受请求
****1565338891287,[线程:disposeRequestThread-1],com.example.chat24.Demo1.dao(Demo1.java:48):执行数据库操作
****1565338891287,[线程:disposeRequestThread-1],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据0成功
****1565338891287,[线程:disposeRequestThread-1],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据1成功
****1565338891287,[线程:disposeRequestThread-2],com.example.chat24.Demo1.dao(Demo1.java:48):执行数据库操作
****1565338891287,[线程:disposeRequestThread-1],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据2成功
****1565338891287,[线程:disposeRequestThread-3],com.example.chat24.Demo1.service(Demo1.java:42):执行业务
****1565338891288,[线程:disposeRequestThread-1],com.example.chat24.Demo1.controller(Demo1.java:36):接受请求
****1565338891287,[线程:disposeRequestThread-2],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据0成功
****1565338891288,[线程:disposeRequestThread-1],com.example.chat24.Demo1.service(Demo1.java:42):执行业务
****1565338891288,[线程:disposeRequestThread-3],com.example.chat24.Demo1.dao(Demo1.java:48):执行数据库操作
****1565338891288,[线程:disposeRequestThread-1],com.example.chat24.Demo1.dao(Demo1.java:48):执行数据库操作
****1565338891288,[线程:disposeRequestThread-2],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据1成功
****1565338891288,[线程:disposeRequestThread-1],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据0成功
****1565338891288,[线程:disposeRequestThread-3],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据0成功
****1565338891288,[线程:disposeRequestThread-1],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据1成功
****1565338891288,[线程:disposeRequestThread-2],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据2成功
****1565338891288,[线程:disposeRequestThread-1],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据2成功
****1565338891288,[线程:disposeRequestThread-3],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据1成功
****1565338891288,[线程:disposeRequestThread-2],com.example.chat24.Demo1.controller(Demo1.java:36):接受请求
****1565338891288,[线程:disposeRequestThread-3],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据2成功
****1565338891288,[线程:disposeRequestThread-2],com.example.chat24.Demo1.service(Demo1.java:42):执行业务
****1565338891289,[线程:disposeRequestThread-2],com.example.chat24.Demo1.dao(Demo1.java:48):执行数据库操作
****1565338891289,[线程:disposeRequestThread-2],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据0成功
****1565338891289,[线程:disposeRequestThread-2],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据1成功
****1565338891289,[线程:disposeRequestThread-2],com.example.chat24.Demo1.dao(Demo1.java:51):插入数据数据2成功
```
代码中调用Controller、Service、Dao 3个方法时，来模拟处理一个请求。main方法中循环了5次模拟发起5次请求，然后交给线程池去处理请求，Dao中模拟循环插入传入的dataList数据。<br />**问题来了：开发者想看一下哪些地方耗时比较多，想通过日志来分析耗时情况，想追踪某个请求的完整日志，怎么搞？**<br />上面的请求采用线程池的方式处理的，多个请求可能会被一个线程处理，通过日志很难看出那些日志是同一个请求，能不能给请求加一个唯一标志，日志中输出这个唯一标志，当然可以。<br />如果代码就只有上面示例这么简单，还是很容易的，上面就3个方法，给每个方法加个traceId参数，log方法也加个traceId参数，就解决了，代码如下：
```java
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class Demo2 {

    static AtomicInteger threadIndex = new AtomicInteger(1);
    //创建处理请求的线程池子
    static ThreadPoolExecutor disposeRequestExecutor = new ThreadPoolExecutor(3,
            3,
            60,
            TimeUnit.SECONDS,
            new LinkedBlockingDeque<>(),
            r -> {
                Thread thread = new Thread(r);
                thread.setName("disposeRequestThread-" + threadIndex.getAndIncrement());
                return thread;
            });

    //记录日志
    public static void log(String msg, String traceId) {
        StackTraceElement stack[] = (new Throwable()).getStackTrace();
        System.out.println("****" + System.currentTimeMillis() + "[traceId:" + traceId + "],[线程:" + Thread.currentThread().getName() + "]," + stack[1] + ":" + msg);
    }

    //模拟controller
    public static void controller(List<String> dataList, String traceId) {
        log("接受请求", traceId);
        service(dataList, traceId);
    }

    //模拟service
    public static void service(List<String> dataList, String traceId) {
        log("执行业务", traceId);
        dao(dataList, traceId);
    }

    //模拟dao
    public static void dao(List<String> dataList, String traceId) {
        log("执行数据库操作", traceId);
        //模拟插入数据
        for (String s : dataList) {
            log("插入数据" + s + "成功", traceId);
        }
    }

    public static void main(String[] args) {
        //需要插入的数据
        List<String> dataList = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            dataList.add("数据" + i);
        }

        //模拟5个请求
        int requestCount = 5;
        for (int i = 0; i < requestCount; i++) {
            String traceId = String.valueOf(i);
            disposeRequestExecutor.execute(() -> {
                controller(dataList, traceId);
            });
        }

        disposeRequestExecutor.shutdown();
    }
}
```
输出：
```
****1565339559773[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo2.controller(Demo2.java:36):接受请求
****1565339559773[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo2.controller(Demo2.java:36):接受请求
****1565339559773[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo2.controller(Demo2.java:36):接受请求
****1565339559774[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo2.service(Demo2.java:42):执行业务
****1565339559774[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo2.service(Demo2.java:42):执行业务
****1565339559774[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo2.dao(Demo2.java:48):执行数据库操作
****1565339559774[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo2.service(Demo2.java:42):执行业务
****1565339559774[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据0成功
****1565339559774[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo2.dao(Demo2.java:48):执行数据库操作
****1565339559774[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据1成功
****1565339559774[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo2.dao(Demo2.java:48):执行数据库操作
****1565339559774[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据2成功
****1565339559774[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据0成功
****1565339559775[traceId:3],[线程:disposeRequestThread-2],com.example.chat24.Demo2.controller(Demo2.java:36):接受请求
****1565339559775[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据0成功
****1565339559775[traceId:3],[线程:disposeRequestThread-2],com.example.chat24.Demo2.service(Demo2.java:42):执行业务
****1565339559775[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据1成功
****1565339559775[traceId:3],[线程:disposeRequestThread-2],com.example.chat24.Demo2.dao(Demo2.java:48):执行数据库操作
****1565339559775[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据1成功
****1565339559775[traceId:3],[线程:disposeRequestThread-2],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据0成功
****1565339559775[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据2成功
****1565339559775[traceId:3],[线程:disposeRequestThread-2],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据1成功
****1565339559775[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据2成功
****1565339559775[traceId:3],[线程:disposeRequestThread-2],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据2成功
****1565339559775[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo2.controller(Demo2.java:36):接受请求
****1565339559776[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo2.service(Demo2.java:42):执行业务
****1565339559776[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo2.dao(Demo2.java:48):执行数据库操作
****1565339559776[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据0成功
****1565339559776[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据1成功
****1565339559776[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo2.dao(Demo2.java:51):插入数据数据2成功
```
上面通过修改代码的方式，把问题解决了，但前提是系统都像上面这么简单，功能很少，需要改的代码很少，可以这么去改。但事与愿违，系统一般功能都是比较多的，如果都一个个去改，岂不是要疯掉，改代码还涉及到重新测试，风险也不可控。那有什么好办法么？
<a name="V9kmH"></a>
## ThreadLocal
还是拿上面的问题，来分析一下，每个请求都是由一个线程处理的，线程就相当于一个人一样，每个请求相当于一个任务，任务来了，人来处理，处理完毕之后，再处理下一个请求任务。人身上是不是有很多口袋，人刚开始准备处理任务的时候，把任务的编号放在处理者的口袋中，然后处理中一路携带者，处理过程中如果需要用到这个编号，直接从口袋中获取就可以了。那么刚好java中线程设计的时候也考虑到了这些问题，Thread对象中就有很多口袋，用来放东西。Thread类中有这么一个变量：
```java
ThreadLocal.ThreadLocalMap threadLocals = null;
```
这个就是用来操作Thread中所有口袋的东西，`ThreadLocalMap`源码中有一个数组（有兴趣的可以去看一下源码），对应处理者身上很多口袋一样，数组中的每个元素对应一个口袋。<br />如何来操作Thread中的这些口袋呢，java提供了一个类`ThreadLocal`，`ThreadLocal`对象用来操作Thread中的某一个口袋，可以向这个口袋中放东西、获取里面的东西、清除里面的东西，这个口袋一次性只能放一个东西，重复放东西会将里面已经存在的东西覆盖掉。<br />常用的3个方法：
```java
//向Thread中某个口袋中放东西
public void set(T value);
//获取这个口袋中目前放的东西
public T get();
//清空这个口袋中放的东西
public void remove()
```
使用ThreadLocal来改造一下上面的代码，如下：
```java
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class Demo3 {

    //创建一个操作Thread中存放请求任务追踪id口袋的对象
    static ThreadLocal<String> traceIdKD = new ThreadLocal<>();

    static AtomicInteger threadIndex = new AtomicInteger(1);
    //创建处理请求的线程池子
    static ThreadPoolExecutor disposeRequestExecutor = new ThreadPoolExecutor(3,
            3,
            60,
            TimeUnit.SECONDS,
            new LinkedBlockingDeque<>(),
            r -> {
                Thread thread = new Thread(r);
                thread.setName("disposeRequestThread-" + threadIndex.getAndIncrement());
                return thread;
            });

    //记录日志
    public static void log(String msg) {
        StackTraceElement stack[] = (new Throwable()).getStackTrace();
        //获取当前线程存放tranceId口袋中的内容
        String traceId = traceIdKD.get();
        System.out.println("****" + System.currentTimeMillis() + "[traceId:" + traceId + "],[线程:" + Thread.currentThread().getName() + "]," + stack[1] + ":" + msg);
    }

    //模拟controller
    public static void controller(List<String> dataList) {
        log("接受请求");
        service(dataList);
    }

    //模拟service
    public static void service(List<String> dataList) {
        log("执行业务");
        dao(dataList);
    }

    //模拟dao
    public static void dao(List<String> dataList) {
        log("执行数据库操作");
        //模拟插入数据
        for (String s : dataList) {
            log("插入数据" + s + "成功");
        }
    }

    public static void main(String[] args) {
        //需要插入的数据
        List<String> dataList = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            dataList.add("数据" + i);
        }

        //模拟5个请求
        int requestCount = 5;
        for (int i = 0; i < requestCount; i++) {
            String traceId = String.valueOf(i);
            disposeRequestExecutor.execute(() -> {
                //把traceId放入口袋中
                traceIdKD.set(traceId);
                try {
                    controller(dataList);
                } finally {
                    //将tranceId从口袋中移除
                    traceIdKD.remove();
                }
            });
        }

        disposeRequestExecutor.shutdown();
    }
}
```
输出：
```
****1565339644214[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo3.controller(Demo3.java:41):接受请求
****1565339644214[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo3.controller(Demo3.java:41):接受请求
****1565339644214[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo3.controller(Demo3.java:41):接受请求
****1565339644214[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo3.service(Demo3.java:47):执行业务
****1565339644214[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo3.service(Demo3.java:47):执行业务
****1565339644214[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo3.dao(Demo3.java:53):执行数据库操作
****1565339644214[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo3.service(Demo3.java:47):执行业务
****1565339644214[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据0成功
****1565339644214[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo3.dao(Demo3.java:53):执行数据库操作
****1565339644214[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo3.dao(Demo3.java:53):执行数据库操作
****1565339644215[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据0成功
****1565339644215[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据1成功
****1565339644215[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据1成功
****1565339644215[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据0成功
****1565339644215[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据2成功
****1565339644215[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据2成功
****1565339644215[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据1成功
****1565339644215[traceId:4],[线程:disposeRequestThread-3],com.example.chat24.Demo3.controller(Demo3.java:41):接受请求
****1565339644215[traceId:3],[线程:disposeRequestThread-1],com.example.chat24.Demo3.controller(Demo3.java:41):接受请求
****1565339644215[traceId:4],[线程:disposeRequestThread-3],com.example.chat24.Demo3.service(Demo3.java:47):执行业务
****1565339644215[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据2成功
****1565339644215[traceId:4],[线程:disposeRequestThread-3],com.example.chat24.Demo3.dao(Demo3.java:53):执行数据库操作
****1565339644215[traceId:3],[线程:disposeRequestThread-1],com.example.chat24.Demo3.service(Demo3.java:47):执行业务
****1565339644215[traceId:4],[线程:disposeRequestThread-3],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据0成功
****1565339644215[traceId:3],[线程:disposeRequestThread-1],com.example.chat24.Demo3.dao(Demo3.java:53):执行数据库操作
****1565339644215[traceId:4],[线程:disposeRequestThread-3],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据1成功
****1565339644215[traceId:3],[线程:disposeRequestThread-1],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据0成功
****1565339644215[traceId:4],[线程:disposeRequestThread-3],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据2成功
****1565339644215[traceId:3],[线程:disposeRequestThread-1],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据1成功
****1565339644215[traceId:3],[线程:disposeRequestThread-1],com.example.chat24.Demo3.dao(Demo3.java:56):插入数据数据2成功
```
可以看出输出和刚才使用traceId参数的方式结果一致，但是却简单了很多。不用去修改Controller、Service、Dao代码了，风险也减少了很多。<br />代码中创建了一个ThreadLocal traceIdKD，这个对象用来操作Thread中一个口袋，用这个口袋来存放tranceId。在main方法中通过`traceIdKD.set(traceId)`方法将traceId放入口袋，log方法中通`traceIdKD.get()`获取口袋中的traceId，最后任务处理完之后，main中的`finally`中调用`traceIdKD.remove();`将口袋中的traceId清除。<br />**ThreadLocal的官方API解释为：**<br />“该类提供了线程局部 (thread-local) 变量。这些变量不同于它们的普通对应物，因为访问某个变量（通过其 get 或 set 方法）的每个线程都有自己的局部变量，它独立于变量的初始化副本。ThreadLocal 实例通常是类中的 private static 字段，它们希望将状态与某一个线程（例如，用户 ID 或事务 ID）相关联。”
<a name="bCMrH"></a>
## `InheritableThreadLocal`
继续上面的实例，Dao中循环处理dataList的内容，假如dataList处理比较耗时，想加快处理速度有什么办法么？大家已经想到了，用多线程并行处理dataList，那么把代码改一下，如下：
```java
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class Demo4 {

    //创建一个操作Thread中存放请求任务追踪id口袋的对象
    static ThreadLocal<String> traceIdKD = new ThreadLocal<>();

    static AtomicInteger threadIndex = new AtomicInteger(1);
    //创建处理请求的线程池子
    static ThreadPoolExecutor disposeRequestExecutor = new ThreadPoolExecutor(3,
            3,
            60,
            TimeUnit.SECONDS,
            new LinkedBlockingDeque<>(),
            r -> {
                Thread thread = new Thread(r);
                thread.setName("disposeRequestThread-" + threadIndex.getAndIncrement());
                return thread;
            });

    //记录日志
    public static void log(String msg) {
        StackTraceElement stack[] = (new Throwable()).getStackTrace();
        //获取当前线程存放tranceId口袋中的内容
        String traceId = traceIdKD.get();
        System.out.println("****" + System.currentTimeMillis() + "[traceId:" + traceId + "],[线程:" + Thread.currentThread().getName() + "]," + stack[1] + ":" + msg);
    }

    //模拟controller
    public static void controller(List<String> dataList) {
        log("接受请求");
        service(dataList);
    }

    //模拟service
    public static void service(List<String> dataList) {
        log("执行业务");
        dao(dataList);
    }

    //模拟dao
    public static void dao(List<String> dataList) {
        CountDownLatch countDownLatch = new CountDownLatch(dataList.size());

        log("执行数据库操作");
        String threadName = Thread.currentThread().getName();
        //模拟插入数据
        for (String s : dataList) {
            new Thread(() -> {
                try {
                    //模拟数据库操作耗时100毫秒
                    TimeUnit.MILLISECONDS.sleep(100);
                    log("插入数据" + s + "成功,主线程：" + threadName);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    countDownLatch.countDown();
                }
            }).start();
        }
        //等待上面的dataList处理完毕
        try {
            countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        //需要插入的数据
        List<String> dataList = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            dataList.add("数据" + i);
        }

        //模拟5个请求
        int requestCount = 5;
        for (int i = 0; i < requestCount; i++) {
            String traceId = String.valueOf(i);
            disposeRequestExecutor.execute(() -> {
                //把traceId放入口袋中
                traceIdKD.set(traceId);
                try {
                    controller(dataList);
                } finally {
                    //将tranceId从口袋中移除
                    traceIdKD.remove();
                }
            });
        }

        disposeRequestExecutor.shutdown();
    }
}
```
输出：
```
****1565339904279[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565339904279[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565339904279[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565339904279[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565339904279[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565339904279[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565339904279[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565339904279[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565339904279[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565339904281[traceId:null],[线程:Thread-3],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据0成功,主线程：disposeRequestThread-1
****1565339904281[traceId:null],[线程:Thread-5],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据0成功,主线程：disposeRequestThread-2
****1565339904281[traceId:null],[线程:Thread-4],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据0成功,主线程：disposeRequestThread-3
****1565339904281[traceId:null],[线程:Thread-6],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据1成功,主线程：disposeRequestThread-3
****1565339904281[traceId:null],[线程:Thread-9],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据2成功,主线程：disposeRequestThread-3
****1565339904282[traceId:null],[线程:Thread-8],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据1成功,主线程：disposeRequestThread-1
****1565339904282[traceId:null],[线程:Thread-10],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据2成功,主线程：disposeRequestThread-1
****1565339904282[traceId:3],[线程:disposeRequestThread-3],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565339904282[traceId:null],[线程:Thread-7],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据1成功,主线程：disposeRequestThread-2
****1565339904282[traceId:null],[线程:Thread-11],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据2成功,主线程：disposeRequestThread-2
****1565339904282[traceId:3],[线程:disposeRequestThread-3],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565339904282[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565339904283[traceId:3],[线程:disposeRequestThread-3],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565339904283[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565339904283[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565339904283[traceId:null],[线程:Thread-12],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据0成功,主线程：disposeRequestThread-3
****1565339904283[traceId:null],[线程:Thread-13],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据1成功,主线程：disposeRequestThread-3
****1565339904283[traceId:null],[线程:Thread-14],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据0成功,主线程：disposeRequestThread-1
****1565339904284[traceId:null],[线程:Thread-15],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据2成功,主线程：disposeRequestThread-3
****1565339904284[traceId:null],[线程:Thread-17],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据2成功,主线程：disposeRequestThread-1
****1565339904284[traceId:null],[线程:Thread-16],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:62):插入数据数据1成功,主线程：disposeRequestThread-1
```
看一下上面的输出，有些traceId为null，这是为什么呢？这是因为Dao中为了提升处理速度，创建了子线程来并行处理，子线程调用log的时候，去自己的存放traceId的口袋中拿去东西，肯定是空的了。<br />那有什么办法么？可不可以这样？<br />父线程相当于主管，子线程相当于干活的小弟，主管让小弟们干活的时候，将自己兜里面的东西复制一份给小弟们使用，主管兜里面可能有很多牛逼的工具，为了提升小弟们的工作效率，给小弟们都复制一个，丢到小弟们的兜里，然后小弟就可以从自己的兜里拿去这些东西使用了，也可以清空自己兜里面的东西。<br />Thread对象中有个`inheritableThreadLocals`变量，代码如下：
```java
ThreadLocal.ThreadLocalMap inheritableThreadLocals = null;
```
`inheritableThreadLocals`相当于线程中另外一种兜，这种兜有什么特征呢，当创建子线程的时候，子线程会将父线程这种类型兜的东西全部复制一份放到自己的`inheritableThreadLocals`兜中，使用`InheritableThreadLocal`对象可以操作线程中的`inheritableThreadLocals`兜。<br />`InheritableThreadLocal`常用的方法也有3个：
```java
//向Thread中某个口袋中放东西
public void set(T value);
//获取这个口袋中目前放的东西
public T get();
//清空这个口袋中放的东西
public void remove()
```
使用`InheritableThreadLocal`解决上面子线程中无法输出traceId的问题，只需要将上一个示例代码中的`ThreadLocal`替换成`InheritableThreadLocal`即可，代码如下：
```java
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.LinkedBlockingDeque;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class Demo4 {

    //创建一个操作Thread中存放请求任务追踪id口袋的对象,子线程可以继承父线程中内容
    static InheritableThreadLocal<String> traceIdKD = new InheritableThreadLocal<>();

    static AtomicInteger threadIndex = new AtomicInteger(1);
    //创建处理请求的线程池子
    static ThreadPoolExecutor disposeRequestExecutor = new ThreadPoolExecutor(3,
            3,
            60,
            TimeUnit.SECONDS,
            new LinkedBlockingDeque<>(),
            r -> {
                Thread thread = new Thread(r);
                thread.setName("disposeRequestThread-" + threadIndex.getAndIncrement());
                return thread;
            });

    //记录日志
    public static void log(String msg) {
        StackTraceElement stack[] = (new Throwable()).getStackTrace();
        //获取当前线程存放tranceId口袋中的内容
        String traceId = traceIdKD.get();
        System.out.println("****" + System.currentTimeMillis() + "[traceId:" + traceId + "],[线程:" + Thread.currentThread().getName() + "]," + stack[1] + ":" + msg);
    }

    //模拟controller
    public static void controller(List<String> dataList) {
        log("接受请求");
        service(dataList);
    }

    //模拟service
    public static void service(List<String> dataList) {
        log("执行业务");
        dao(dataList);
    }

    //模拟dao
    public static void dao(List<String> dataList) {
        CountDownLatch countDownLatch = new CountDownLatch(dataList.size());

        log("执行数据库操作");
        String threadName = Thread.currentThread().getName();
        //模拟插入数据
        for (String s : dataList) {
            new Thread(() -> {
                try {
                    //模拟数据库操作耗时100毫秒
                    TimeUnit.MILLISECONDS.sleep(100);
                    log("插入数据" + s + "成功,主线程：" + threadName);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    countDownLatch.countDown();
                }
            }).start();
        }
        //等待上面的dataList处理完毕
        try {
            countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        //需要插入的数据
        List<String> dataList = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            dataList.add("数据" + i);
        }

        //模拟5个请求
        int requestCount = 5;
        for (int i = 0; i < requestCount; i++) {
            String traceId = String.valueOf(i);
            disposeRequestExecutor.execute(() -> {
                //把traceId放入口袋中
                traceIdKD.set(traceId);
                try {
                    controller(dataList);
                } finally {
                    //将tranceId从口袋中移除
                    traceIdKD.remove();
                }
            });
        }

        disposeRequestExecutor.shutdown();
    }
}
```
输出：
```
****1565341611454[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565341611454[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565341611454[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565341611454[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565341611454[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565341611454[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565341611454[traceId:2],[线程:disposeRequestThread-3],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565341611454[traceId:1],[线程:disposeRequestThread-2],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565341611454[traceId:0],[线程:disposeRequestThread-1],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565341611557[traceId:2],[线程:Thread-5],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据0成功,主线程：disposeRequestThread-3
****1565341611557[traceId:0],[线程:Thread-4],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据0成功,主线程：disposeRequestThread-1
****1565341611557[traceId:1],[线程:Thread-11],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据2成功,主线程：disposeRequestThread-2
****1565341611557[traceId:1],[线程:Thread-3],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据0成功,主线程：disposeRequestThread-2
****1565341611557[traceId:1],[线程:Thread-8],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据1成功,主线程：disposeRequestThread-2
****1565341611557[traceId:0],[线程:Thread-6],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据1成功,主线程：disposeRequestThread-1
****1565341611557[traceId:0],[线程:Thread-10],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据2成功,主线程：disposeRequestThread-1
****1565341611557[traceId:3],[线程:disposeRequestThread-2],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565341611557[traceId:2],[线程:Thread-9],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据2成功,主线程：disposeRequestThread-3
****1565341611558[traceId:2],[线程:Thread-7],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据1成功,主线程：disposeRequestThread-3
****1565341611557[traceId:3],[线程:disposeRequestThread-2],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565341611557[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo4.controller(Demo4.java:42):接受请求
****1565341611558[traceId:3],[线程:disposeRequestThread-2],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565341611558[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo4.service(Demo4.java:48):执行业务
****1565341611558[traceId:4],[线程:disposeRequestThread-1],com.example.chat24.Demo4.dao(Demo4.java:56):执行数据库操作
****1565341611659[traceId:3],[线程:Thread-15],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据2成功,主线程：disposeRequestThread-2
****1565341611659[traceId:4],[线程:Thread-14],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据0成功,主线程：disposeRequestThread-1
****1565341611659[traceId:3],[线程:Thread-13],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据1成功,主线程：disposeRequestThread-2
****1565341611659[traceId:3],[线程:Thread-12],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据0成功,主线程：disposeRequestThread-2
****1565341611660[traceId:4],[线程:Thread-16],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据1成功,主线程：disposeRequestThread-1
****1565341611660[traceId:4],[线程:Thread-17],com.example.chat24.Demo4.lambda$dao$1(Demo4.java:64):插入数据数据2成功,主线程：disposeRequestThread-1
```
输出中都有traceId了，和期望的结果一致。
