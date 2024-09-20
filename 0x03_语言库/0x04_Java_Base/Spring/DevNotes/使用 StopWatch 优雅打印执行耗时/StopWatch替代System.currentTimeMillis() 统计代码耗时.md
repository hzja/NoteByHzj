Java<br />利用`StopWatch`监控Java代码运行时间和分析性能
<a name="klWrc"></a>
## 一、背景
有时在做开发的时候需要记录每个任务执行时间，或者记录一段代码执行时间，最简单的方法就是打印当前时间与执行完时间的差值，一般检测某段代码执行的时间，都是以如下方式来进行的：
```java
public static void main(String[] args) {
    Long startTime = System.currentTimeMillis();
    // 业务代码
    Long endTime = System.currentTimeMillis();
    Long elapsedTime = (endTime - startTime) / 1000;
    System.out.println("该段总共耗时：" + elapsedTime + "s");
}
```
事实上该方法通过获取执行完成时间与执行开始时间的差值得到程序的执行时间，简单直接有效，但想必写多了也是比较烦人的，尤其是碰到不可描述的代码时，会更加的让人忍不住多写几个bug聊表敬意，而且如果想对执行的时间做进一步控制，则需要在程序中很多地方修改。<br />此时会想是否有一个工具类，提供了这些方法，刚好可以满足这种场景？<br />可以利用已有的工具类中的秒表，常见的秒表工具类有 `org.springframework.util.StopWatch`、`org.apache.commons.lang.time.StopWatch`以及谷歌提供的guava中的秒表（这个没怎么用过）。<br />这里重点讲下基于Spring、Apache的使用。
<a name="KpRtN"></a>
## 二、Spring 用法
<a name="sCPe0"></a>
### 2.1 初遇
StopWatch 是位于 org.springframework.util 包下的一个工具类，通过它可方便的对程序部分代码进行计时(ms级别)，适用于同步单线程代码块。简单总结一句，Spring提供的计时器`StopWatch`对于秒、毫秒为单位方便计时的程序，尤其是单线程、顺序执行程序的时间特性的统计输出支持比较好。<br />也就是说假如手里面有几个在顺序上前后执行的几个任务，而且比较关心几个任务分别执行的时间占用状况，希望能够形成一个不太复杂的日志输出，StopWatch提供了这样的功能。而且Spring的`StopWatch`基本上也就是仅仅为了这样的功能而实现。<br />想要使用它，首先需要在 Maven 中引入 Spring 核心包，当然 Spring MVC 和 Spring Boot 都已经自动引入了该包：
```xml
<!-- https://mvnrepository.com/artifact/org.springframework/spring-core -->
<dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-core</artifactId>
  <version>${spring.version}</version>
</dependency>
```
对一切事物的认知，都是从使用开始，那就先来看看它的用法，会如下所示：
```java
public static void main(String[] args) throws InterruptedException {
    StopWatch stopWatch = new StopWatch();

    // 任务一模拟休眠3秒钟
    stopWatch.start("TaskOneName");
    Thread.sleep(1000 * 3);
    System.out.println("当前任务名称：" + stopWatch.currentTaskName());
    stopWatch.stop();

    // 任务一模拟休眠10秒钟
    stopWatch.start("TaskTwoName");
    Thread.sleep(1000 * 10);
    System.out.println("当前任务名称：" + stopWatch.currentTaskName());
    stopWatch.stop();

    // 任务一模拟休眠10秒钟
    stopWatch.start("TaskThreeName");
    Thread.sleep(1000 * 10);
    System.out.println("当前任务名称：" + stopWatch.currentTaskName());
    stopWatch.stop();

    // 打印出耗时
    System.out.println(stopWatch.prettyPrint());
    System.out.println(stopWatch.shortSummary());
    // stop后它的值为null
    System.out.println(stopWatch.currentTaskName()); 
    
    // 最后一个任务的相关信息
    System.out.println(stopWatch.getLastTaskName());
    System.out.println(stopWatch.getLastTaskInfo());
    
    // 任务总的耗时  如果想获取到每个任务详情（包括它的任务名、耗时等等）可使用
    System.out.println("所有任务总耗时：" + sw.getTotalTimeMillis());
    System.out.println("任务总数：" + sw.getTaskCount());
    System.out.println("所有任务详情：" + sw.getTaskInfo());
}
```
如图所示，`StopWatch` 不仅正确记录了上个任务的执行时间，并且在最后还可以给出精确的任务执行时间（纳秒级别）和耗时占比，这或许就会比自己输出要优雅那么一些。
<a name="aLU8m"></a>
### 2.2 源码
老规矩，由浅入深。看完用法，来看看源码。先看下组成 StopWatch 的属性
```java
public class StopWatch {
    /**
     * 本实例的唯一 Id，用于在日志或控制台输出时区分的。
     */
    private final String id;
    /**
     * 是否保持一个 taskList 链表
     * 每次停止计时时，会将当前任务放入这个链表，用以记录任务链路和计时分析
     */
    private boolean keepTaskList = true;
    /**
     * 任务链表
     * 用来存储每个task的信息, taskInfo由taskName 和 totoalTime组成
     */
    private final List<StopWatch.TaskInfo> taskList;
    /**
     * 当前任务的开始时间
     */
    private long startTimeMillis;
    /**
     *
     */
    private boolean running;
    /**
     * 当前任务名称
     */
    private String currentTaskName;
    /**
     * 最后一个任务的信息
     */
    private StopWatch.TaskInfo lastTaskInfo;
    /**
     * 任务总数
     */
    private int taskCount;
    /**
     * 程序执行时间
     */
    private long totalTimeMillis;
    ...
}
```
接下来，来看一下`StopWatch`类的构造器和一些关键方法<br /> 

| **方法** | **说明** |
| --- | --- |
| `**new StopWatch()**` | 构建一个新的秒表，不开始任何任务。 |
| `**new StopWatch(String id)**` | 创建一个指定了id的StopWatch |
| `**String getId()**` | 返回此秒表的ID |
| `**void start(String taskName)**` | 不传入参数，开始一个无名称的任务的计时。传入String类型的参数来开始指定任务名的任务计时 |
| `**void stop()**` | 停止当前任务的计时 |
| `**boolean isRunning()**` | 是否正在计时某任务 |
| `**String currentTaskName()**` | 当前正在运行的任务的名称（如果有） |
| `**long getTotalTimeMillis()**` | 所有任务的总体执行时间(毫秒单位) |
| `**double getTotalTimeSeconds()**` | 所有任务的总时间（以秒为单位） |
| `**String getLastTaskName()**` | 上一个任务的名称 |
| `**long getLastTaskTimeMillis()**` | 上一个任务的耗时(毫秒单位) |
| `**int getTaskCount()**` | 定时任务的数量 |
| `**String shortSummary()**` | 总运行时间的简短描述 |
| `**String prettyPrint()**` | 优美地打印所有任务的详细耗时情况 |

<a name="bUfxL"></a>
### 2.3 注意事项

- `**StopWatch**`**对象不是设计为线程安全的，并且不使用同步。**
- **一个**`**StopWatch**`**实例一次只能开启一个task，不能同时start多个task**
- **在该task还没stop之前不能start一个新的task，必须在该task stop之后才能开启新的task**
- **若要一次开启多个，需要new不同的**`**StopWatch**`**实例**
<a name="hF2DP"></a>
## 三、Apache 用法
`StopWath`是 apache commons lang3 包下的一个任务执行时间监视器，与平时常用的秒表的行为比较类似，先看一下其中的一些重要方法：

| **方法** | **说明** |
| --- | --- |
| `**new StopWatch()**` | 构建一个新的秒表，不开始任何任务。 |
| `**static StopWatch createStarted()**` | <br /> |
| `**void start()**` | 开始计时 |
| `**void stop()**` | 停止当前任务的计时 |
| `**void reset()**` | 重置计时 |
| `**void split()**` | 设置split点 |
| `**void unsplit()**` | <br /> |
| `**void suspend()**` | 暂停计时, 直到调用resume()后才恢复计时 |
| `**void resume()**` | 恢复计时 |
| `**long getTime()**` | 统计从start到现在的计时 |
| `**long getTime(final TimeUnit timeUnit)**` | <br /> |
| `**long getNanoTime()**` | <br /> |
| `**long getSplitTime()**` | 获取从start 到 最后一次split的时间 |
| `**long getSplitNanoTime()**` | <br /> |
| `**long getStartTime()**` | <br /> |
| `**boolean isStarted()**` | <br /> |
| `**boolean isSuspended()**` | <br /> |
| `**boolean isStopped()**` | <br /> |

```xml
<!-- https://mvnrepository.com/artifact/org.apache.commons/commons-lang3 -->
<dependency>
  <groupId>org.apache.commons</groupId>
  <artifactId>commons-lang3</artifactId>
  <version>3.6</version>
</dependency>
```
Apache提供的这个任务执行监视器功能丰富强大，灵活性强，如下经典实用案例：
```java
public static void main(String[] args) throws InterruptedException {
    //创建后立即start，常用
    StopWatch watch = StopWatch.createStarted();

    // StopWatch watch = new StopWatch();
    // watch.start();

    Thread.sleep(1000);
    System.out.println(watch.getTime());
    System.out.println("统计从开始到现在运行时间：" + watch.getTime() + "ms");

    Thread.sleep(1000);
    watch.split();
    System.out.println("从start到此刻为止的时间：" + watch.getTime());
    System.out.println("从开始到第一个切入点运行时间：" + watch.getSplitTime());


    Thread.sleep(1000);
    watch.split();
    System.out.println("从开始到第二个切入点运行时间：" + watch.getSplitTime());

    // 复位后, 重新计时
    watch.reset();
    watch.start();
    Thread.sleep(1000);
    System.out.println("重新开始后到当前运行时间是：" + watch.getTime());

    // 暂停 与 恢复
    watch.suspend();
    System.out.println("暂停2秒钟");
    Thread.sleep(2000);

    // 上面suspend，这里要想重新统计，需要恢复一下
    watch.resume();
    System.out.println("恢复后执行的时间是：" + watch.getTime());

    Thread.sleep(1000);
    watch.stop();

    System.out.println("花费的时间》》" + watch.getTime() + "ms");
    // 直接转成s
    System.out.println("花费的时间》》" + watch.getTime(TimeUnit.SECONDS) + "s");
}
```
<a name="pXCZb"></a>
## 四、最后
很多时候，写代码也是一种艺术，而借助这种实用工具艺术感更强些。希望能有追求更加美好事物的心，这点对于接纳新知识特别重要。此处推荐这个监视器来代替之前的的使用，能让小伙伴们更加灵活的分析代码~
