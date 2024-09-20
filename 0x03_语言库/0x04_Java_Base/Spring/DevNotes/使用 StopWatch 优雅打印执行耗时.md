Java Spring 耗时
<a name="I3eaq"></a>
## 1、背景
有时在做开发的时候需要记录每个任务执行时间，或者记录一段代码执行时间，最简单的方法就是打印当前时间与执行完时间的差值，然后这样如果执行大量测试的话就很麻烦，并且不直观，如果想对执行的时间做进一步控制，则需要在程序中很多地方修改，目前spring-framework提供了一个StopWatch类可以做类似任务执行时间控制，也就是封装了一个对开始时间，结束时间记录工具
<a name="Fdquu"></a>
### 示例
先来看几个示例
<a name="cTzmt"></a>
#### 统计输出总耗时
```java
import org.springframework.util.StopWatch;

public class SpringStopWatchExample {

    public static void main (String[] args) throws InterruptedException {
        StopWatch sw = new StopWatch();
        sw.start();
        //long task simulation
        Thread.sleep(1000);
        sw.stop();
        System.out.println(sw.getTotalTimeMillis());
    }
}
```
输出
```
1013
```
<a name="QIPLb"></a>
#### 输出最后一个任务的耗时
```java
public class SpringStopWatchExample2 {

    public static void main (String[] args) throws InterruptedException {
        StopWatch sw = new StopWatch();
        sw.start("A");//setting a task name
        //long task simulation
        Thread.sleep(1000);
        sw.stop();
        System.out.println(sw.getLastTaskTimeMillis());
    }
}
```
输出
```
1009
```
<a name="LWSrY"></a>
#### 以优雅的格式打出所有任务的耗时以及占比
```java
import org.springframework.util.StopWatch;

public class SpringStopWatchExample3 {

    public static void main (String[] args) throws InterruptedException {
        StopWatch sw = new StopWatch();
        sw.start("A");
        Thread.sleep(500);
        sw.stop();
        sw.start("B");
        Thread.sleep(300);
        sw.stop();
        sw.start("C");
        Thread.sleep(200);
        sw.stop();
        System.out.println(sw.prettyPrint());
    }
}
```
输出
```java
StopWatch '': running time (millis) = 1031
-----------------------------------------
ms     %     Task name
-----------------------------------------
00514  050%  A
00302  029%  B
00215  021%  C:
```
<a name="oKQwh"></a>
#### 序列服务输出耗时信息
```java
@Override
public long nextSeq(String name) {
    StopWatch watch = new StopWatch();
    watch.start("单序列获取总消耗");
    long sequence = generator.generateId(name);
    watch.stop();
    logger.info(watch.prettyPrint());
    return sequence;
}
```
<a name="xW2y0"></a>
## 2、更多用法
不同的打印结果

- `getTotalTimeSeconds()` 获取总耗时秒，同时也有获取毫秒的方法
- `prettyPrint()` 优雅的格式打印结果，表格形式
- `shortSummary()` 返回简短的总耗时描述
- `getTaskCount()` 返回统计时间任务的数量
- `getLastTaskInfo().getTaskName()` 返回最后一个任务TaskInfo对象的名称

更多查看文档<br />[https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/util/StopWatch.html](https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/util/StopWatch.html)
<a name="XFvaY"></a>
## 3、总结
以后统计代码执行效率建议大家都使用这个工具来进行输出，不需要在`starttime`、`endtime`再相减计算，用优雅的方式来完成这件事情。
