Java
<a name="cRTVZ"></a>
## 前言
> "If you cannot measure it, you cannot improve it".

在日常开发中，对一些代码的调用或者工具的使用会存在多种选择方式，在不确定他们性能的时候，首先想要做的就是去测量它。大多数时候，会简单的采用多次计数的方式来测量，来看这个方法的总耗时。<br />但是，如果熟悉JVM类加载机制的话，应该知道JVM默认的执行模式是JIT编译与解释混合执行。JVM通过热点代码统计分析，识别高频方法的调用、循环体、公共模块等，基于JIT动态编译技术，会将热点代码转换成机器码，直接交给CPU执行。<br />![Fcant_2022-11-15_14-16-20.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668493028668-082e4eb5-9287-40ff-b684-f2afdffc4eb4.png#averageHue=%23f2ece9&clientId=u042dfbab-dc43-4&from=ui&id=u9e33af2a&originHeight=1772&originWidth=2404&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12802219&status=done&style=none&taskId=u397e028d-eeea-4216-9eab-f5a107caf11&title=)<br />也就是说，JVM会不断的进行编译优化，这就使得很难确定重复多少次才能得到一个稳定的测试结果？所以，很多有经验的同学会在测试代码前写一段预热的逻辑。<br />JMH，全称 Java Microbenchmark Harness (微基准测试框架），是专门用于Java代码微基准测试的一套测试工具API，是由 OpenJDK/Oracle 官方发布的工具。何谓 Micro Benchmark 呢？简单地说就是在 method 层面上的 benchmark，精度可以精确到微秒级。<br />Java的基准测试需要注意的几个点：

- 测试前需要预热。
- 防止无用代码进入测试方法中。
- 并发测试。
- 测试结果呈现。

JMH的使用场景：

1. 定量分析某个热点函数的优化效果
2. 想定量地知道某个函数需要执行多长时间，以及执行时间和输入变量的相关性
3. 对比一个函数的多种实现方式

这里主要是介绍JMH的DEMO演示，和常用的注解参数。
<a name="VprLb"></a>
## DEMO 演示
这里先演示一个DEMO，让不了解JMH的同学能够快速掌握这个工具的大概用法。
<a name="fJyoV"></a>
### 1、测试项目构建
JMH是内置Java9及之后的版本。这里是以Java8进行说明。<br />为了方便，这里直接介绍使用maven构建JMH测试项目的方式。<br />第一种是使用命令行构建，在指定目录下执行以下命令：
```bash
$ mvn archetype:generate \
          -DinteractiveMode=false \
          -DarchetypeGroupId=org.openjdk.jmh \
          -DarchetypeArtifactId=jmh-java-benchmark-archetype \
          -DgroupId=org.sample \
          -DartifactId=test \
          -Dversion=1.0
```
对应目录下会出现一个test项目，打开项目后会看到这样的项目结构。<br />![Fcant_2022-11-15_14-16-02.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668493027485-8d2517f2-aa6a-47e9-b989-b187e243ac62.png#averageHue=%23242e34&clientId=u042dfbab-dc43-4&from=ui&id=Pu4Lv&originHeight=1125&originWidth=3136&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10602357&status=done&style=none&taskId=uae8a5559-4bc5-47b5-9517-e3df6828748&title=)<br />第二种方式就是直接在现有的maven项目中添加jmh-core和jmh-generator-annprocess的依赖来集成JMH。
```xml
<dependency>
  <groupId>org.openjdk.jmh</groupId>
  <artifactId>jmh-core</artifactId>
  <version>${jmh.version}</version>
</dependency>
<dependency>
  <groupId>org.openjdk.jmh</groupId>
  <artifactId>jmh-generator-annprocess</artifactId>
  <version>${jmh.version}</version>
  <scope>provided</scope>
</dependency>
```
<a name="G3Sji"></a>
### 2、编写性能测试
这里以测试LinkedList 通过index 方式迭代和foreach 方式迭代的性能差距为例子，编写测试类，涉及到的注解在之后会讲解，
```java
@State(Scope.Benchmark)
@OutputTimeUnit(TimeUnit.SECONDS)
@Threads(Threads.MAX)
public class LinkedListIterationBenchMark {
 private static final int SIZE = 10000;

    private List<String> list = new LinkedList<>();
    
    @Setup
    public void setUp() {
        for (int i = 0; i < SIZE; i++) {
            list.add(String.valueOf(i));
        }
    }

    @Benchmark
    @BenchmarkMode(Mode.Throughput)
    public void forIndexIterate() {
        for (int i = 0; i < list.size(); i++) {
            list.get(i);
            System.out.print("");
        }
    }

    @Benchmark
    @BenchmarkMode(Mode.Throughput)
    public void forEachIterate() {
        for (String s : list) {
            System.out.print("");
        }
    }
}
```
<a name="yGKG4"></a>
### 3、执行测试
运行 **JMH** 基准测试有两种方式，一个是生产jar文件运行，另一个是直接写main函数或者放在单元测试中执行。<br />生成jar文件的形式主要是针对一些比较大的测试，可能对机器性能或者真实环境模拟有一些需求，需要将测试方法写好了放在linux环境执行。具体命令如下
```bash
$ mvn clean install
$ java -jar target/benchmarks.jar
```
日常中遇到的一般是一些小测试，比如上面写的例子，直接在IDE中跑就好了。启动方式如下：
```java
public static void main(String[] args) throws RunnerException {
    Options opt = new OptionsBuilder()
        .include(LinkedListIterationBenchMark.class.getSimpleName())
        .forks(1)
        .warmupIterations(2)
        .measurementIterations(2)
        .output("E:/Benchmark.log")
        .build();

    new Runner(opt).run();
}
```
<a name="Y7gU1"></a>
### 4、报告结果
输出结果如下，最后的结果：
```
Benchmark                                      Mode  Cnt     Score   Error  Units
LinkedListIterationBenchMark.forEachIterate   thrpt    2  1192.380          ops/s
LinkedListIterationBenchMark.forIndexIterate  thrpt    2   206.866          ops/s
```
整个过程：
```
# Detecting actual CPU count: 12 detected
# JMH version: 1.21
# VM version: JDK 1.8.0_131, Java HotSpot(TM) 64-Bit Server VM, 25.131-b11
# VM invoker: C:\Program Files\Java\jdk1.8.0_131\jre\bin\java.exe
# VM options: -javaagent:D:\Program Files\JetBrains\IntelliJ IDEA 2018.2.2\lib\idea_rt.jar=65175:D:\Program Files\JetBrains\IntelliJ IDEA 2018.2.2\bin -Dfile.encoding=UTF-8
# Warmup: 2 iterations, 10 s each
# Measurement: 2 iterations, 10 s each
# Timeout: 10 min per iteration
# Threads: 12 threads, will synchronize iterations
# Benchmark mode: Throughput, ops/time
# Benchmark: org.sample.jmh.LinkedListIterationBenchMark.forEachIterate

# Run progress: 0.00% complete, ETA 00:01:20
# Fork: 1 of 1
# Warmup Iteration   1: 1189.267 ops/s
# Warmup Iteration   2: 1197.321 ops/s
Iteration   1: 1193.062 ops/s
Iteration   2: 1191.698 ops/s


Result "org.sample.jmh.LinkedListIterationBenchMark.forEachIterate":
  1192.380 ops/s


# JMH version: 1.21
# VM version: JDK 1.8.0_131, Java HotSpot(TM) 64-Bit Server VM, 25.131-b11
# VM invoker: C:\Program Files\Java\jdk1.8.0_131\jre\bin\java.exe
# VM options: -javaagent:D:\Program Files\JetBrains\IntelliJ IDEA 2018.2.2\lib\idea_rt.jar=65175:D:\Program Files\JetBrains\IntelliJ IDEA 2018.2.2\bin -Dfile.encoding=UTF-8
# Warmup: 2 iterations, 10 s each
# Measurement: 2 iterations, 10 s each
# Timeout: 10 min per iteration
# Threads: 12 threads, will synchronize iterations
# Benchmark mode: Throughput, ops/time
# Benchmark: org.sample.jmh.LinkedListIterationBenchMark.forIndexIterate

# Run progress: 50.00% complete, ETA 00:00:40
# Fork: 1 of 1
# Warmup Iteration   1: 205.676 ops/s
# Warmup Iteration   2: 206.512 ops/s
Iteration   1: 206.542 ops/s
Iteration   2: 207.189 ops/s


Result "org.sample.jmh.LinkedListIterationBenchMark.forIndexIterate":
  206.866 ops/s


# Run complete. Total time: 00:01:21

REMEMBER: The numbers below are just data. To gain reusable insights, you need to follow up on
why the numbers are the way they are. Use profilers (see -prof, -lprof), design factorial
experiments, perform baseline and negative tests that provide experimental control, make sure
the benchmarking environment is safe on JVM/OS/HW level, ask for reviews from the domain experts.
Do not assume the numbers tell you what you want them to tell.

Benchmark                                      Mode  Cnt     Score   Error  Units
LinkedListIterationBenchMark.forEachIterate   thrpt    2  1192.380          ops/s
LinkedListIterationBenchMark.forIndexIterate  thrpt    2   206.866          ops/s
```
<a name="FH4Hp"></a>
## 注解介绍
下面来详细介绍一下相关的注解，
<a name="Vcs0D"></a>
### `@BenchmarkMode`
微基准测试类型。**JMH** 提供了以下几种类型进行支持：

| 类型 | 描述 |
| --- | --- |
| `Throughput` | 每段时间执行的次数，一般是秒 |
| `AverageTime` | 平均时间，每次操作的平均耗时 |
| `SampleTime` | 在测试中，随机进行采样执行的时间 |
| `SingleShotTime` | 在每次执行中计算耗时 |
| `All` | 所有模式 |

可以注释在方法级别，也可以注释在类级别，
```java
@BenchmarkMode(Mode.All)
public class LinkedListIterationBenchMark {
    ...
}
@Benchmark
@BenchmarkMode({Mode.Throughput, Mode.SingleShotTime})
public void m() {
    ...
}
```
<a name="Ezk3X"></a>
### `@Warmup`
这个单词的意思就是预热，`iterations = 3`就是指预热轮数。
```java
@Benchmark
@BenchmarkMode({Mode.Throughput, Mode.SingleShotTime})
@Warmup(iterations = 3)
public void m() {
	...
}
```
<a name="J2azm"></a>
### `@Measurement`
正式度量计算的轮数。

- iterations 进行测试的轮次
- time 每轮进行的时长
- timeUnit时长单位
```java
@Benchmark
@BenchmarkMode({Mode.Throughput, Mode.SingleShotTime})
@Measurement(iterations = 3)
public void m() {
 ...
}
```
<a name="wTrfi"></a>
### `@Threads`
每个进程中的测试线程。
```java
@Threads(Threads.MAX)
public class LinkedListIterationBenchMark {
 ...
}
```
<a name="gu9Hm"></a>
### `@Fork`
进行 fork 的次数。如果 fork 数是3的话，则 JMH 会 fork 出3个进程来进行测试。
```java
@Benchmark
@BenchmarkMode({Mode.Throughput, Mode.SingleShotTime})
@Fork(value = 3)
public void m() {
 ...
}
```
<a name="OyJhY"></a>
### `@OutputTimeUnit`
基准测试结果的时间类型。一般选择秒、毫秒、微秒。
```java
@OutputTimeUnit(TimeUnit.SECONDS)
public class LinkedListIterationBenchMark {
 ...
}
```
<a name="aR3mI"></a>
### `@Benchmark`
方法级注解，表示该方法是需要进行 benchmark 的对象，用法和 JUnit 的 `@Test` 类似。
<a name="oscSZ"></a>
### `@Param`
属性级注解，`@Param` 可以用来指定某项参数的多种情况。特别适合用来测试一个函数在不同的参数输入的情况下的性能。
<a name="BmVyG"></a>
### `@Setup`
方法级注解，这个注解的作用就是需要在测试之前进行一些**准备工作**，比如对一些数据的初始化之类的。
<a name="Iqwlu"></a>
### `@TearDown`
方法级注解，这个注解的作用就是需要在测试之后进行一些**结束工作**，比如关闭线程池，数据库连接等的，主要用于资源的回收等。
<a name="xceqz"></a>
### `@State`
当使用`@Setup`参数的时候，必须在类上加这个参数，不然会提示无法运行。<br />就比如上面的例子中，就必须设置`state`。<br />State 用于声明某个类是一个“状态”，然后接受一个 Scope 参数用来表示该状态的共享范围。因为很多 benchmark 会需要一些表示状态的类，JMH 允许把这些类以依赖注入的方式注入到 benchmark 函数里。Scope 主要分为三种。

1. Thread：该状态为每个线程独享。
2. Group：该状态为同一个组里面所有线程共享。
3. Benchmark：该状态在所有线程间共享。
<a name="eZ16j"></a>
### 启动方法
在启动方法中，可以直接指定上述说到的一些参数，并且能将测试结果输出到指定文件中，
```java
/**
 * 仅限于IDE中运行
 * 命令行模式 则是 build 然后 java -jar 启动
 *
 * 1. 这是benchmark 启动的入口
 * 2. 这里同时还完成了JMH测试的一些配置工作
 * 3. 默认场景下，JMH会去找寻标注了@Benchmark的方法，可以通过include和exclude两个方法来完成包含以及排除的语义
 */
public static void main(String[] args) throws RunnerException {
    Options opt = new OptionsBuilder()
            // 包含语义
            // 可以用方法名，也可以用XXX.class.getSimpleName()
            .include("Helloworld")
            // 排除语义
            .exclude("Pref")
            // 预热10轮
            .warmupIterations(10)
            // 代表正式计量测试做10轮，
            // 而每次都是先执行完预热再执行正式计量，
            // 内容都是调用标注了@Benchmark的代码。
            .measurementIterations(10)
            //  forks(3)指的是做3轮测试，
            // 因为一次测试无法有效的代表结果，
            // 所以通过3轮测试较为全面的测试，
            // 而每一轮都是先预热，再正式计量。
            .forks(3)
         .output("E:/Benchmark.log")
            .build();

    new Runner(opt).run();
}
```
<a name="DZ3gG"></a>
## 结语
基于JMH可以对很多工具和框架进行测试，比如日志框架性能对比、BeanCopy性能对比 等，更多的example可以参考官方给出的JMH samples ([https://hg.openjdk.java.net/code-tools/jmh/file/tip/jmh-samples/src/main/java/org/openjdk/jmh/samples/](https://hg.openjdk.java.net/code-tools/jmh/file/tip/jmh-samples/src/main/java/org/openjdk/jmh/samples/))
