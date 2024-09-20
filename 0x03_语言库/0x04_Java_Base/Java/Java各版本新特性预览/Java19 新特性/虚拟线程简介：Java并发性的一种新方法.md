Java<br />Java19影响最深远的更新之一是引入了虚拟线程。虚拟线程是Project Loom的一部分，可以在Java19预览版中使用。
<a name="Vrqba"></a>
## 虚拟线程如何工作
虚拟线程在操作系统进程和应用程序级并发之间引入了一个抽象层。换句话说，虚拟线程可用于调度Java虚拟机编排的任务，因此JVM在操作系统和程序之间起到中介作用。图1展示了虚拟线程的架构。<br />![图1.Java中虚拟线程的架构](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1669037277974-f52d3d08-b24e-43a8-9868-4b1327af2528.jpeg#averageHue=%23ebedd3&clientId=ueedbdb6c-2c2a-4&from=paste&id=uf3e4010d&originHeight=655&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u88e4faa3-0714-44d3-8d81-c383847e1f8&title=%E5%9B%BE1.Java%E4%B8%AD%E8%99%9A%E6%8B%9F%E7%BA%BF%E7%A8%8B%E7%9A%84%E6%9E%B6%E6%9E%84 "图1.Java中虚拟线程的架构")<br />在这种架构中，应用程序实例化虚拟线程，并由JVM分配处理虚拟线程的计算资源。与此相比，常规线程直接映射到操作系统(OS)进程。对于常规线程，应用程序代码负责提供和分配操作系统资源。而使用虚拟线程，应用程序可以实例化虚拟线程，从而表达并发性的需求。但正是JVM从操作系统获取和释放资源。<br />Java中的虚拟线程类似于Go语言中的goroutine。在使用虚拟线程时，JVM只能在应用程序的虚拟线程被驻留时分配计算资源，这意味着它们处于空闲状态并等待新的事件。这种空闲在大多数服务器中是常见的：它们将一个线程分配给一个请求，然后处于空闲状态，并等待一个新的事件，例如来自数据存储的响应或来自网络的进一步输入。<br />使用传统Java线程，当服务器在处理请求时处于空闲状态时，操作系统线程也处于空闲状态，这严重限制了服务器的可扩展性。正如Nicolai Parlog所解释的那样，“操作系统无法提高平台线程的效率，但JDK通过切断其线程与操作系统线程之间的一对一关系，可以更好地利用它们。” <br />以前为缓解与传统Java线程相关的性能和可扩展性问题所做的努力包括异步、响应式库(如JavaRX)。虚拟线程的不同之处在于它们是在JVM级别实现的，但是它们适合Java中现有的编程结构。
<a name="q2TQ3"></a>
## 使用Java虚拟线程：演示
在这个演示中，创建了一个使用Maven原型的简单Java应用程序。为此还做了一些更改，以便在Java19预览版中启用虚拟线程。一旦虚拟线程被升级到预览之外，就不需要做这些更改了。<br />清单1显示了对Maven原型的POM文件所做的更改。需要注意的是，还将编译器设置为使用Java19，并在.mvn/jvm.config中添加了一行（例如清单2所示）。
<a name="tHxRo"></a>
### 演示应用程序的pom.xml
```xml
<properties>
  <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  <maven.compiler.source>19</maven.compiler.source>
  <maven.compiler.target>19</maven.compiler.target>
</properties>
<plugin>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-compiler-plugin</artifactId>
  <version>3.10.1</version>
  <configuration>
    <compilerArgs>
      <arg>--add-modules=jdk.incubator.concurrent</arg>
      <arg>--enable-preview</arg>
    </compilerArgs>
  </configuration>
</plugin>
```
要使exec：java在启用预览的情况下工作，必须使用enable-preview开关。它使用所需的开关启动Maven进程。 
<a name="cN7zD"></a>
### 将enable preview添加到.mvn/jvm.config
```bash
--enable-preview
```
现在，可以使用`mvn compile exec:java`执行该程序，虚拟线程特性将被编译和执行。 
<a name="pkt1g"></a>
## 使用虚拟线程的两种方法
现在考虑在代码中实际使用虚拟线程的两种主要方式。虽然虚拟线程对JVM的工作方式产生了巨大的变化，但其代码实际上与传统Java线程非常相似。设计上的相似性使得重构现有的应用程序和服务器相对容易。这种兼容性还意味着用于监视和观察JVM中的线程的现有工具将与虚拟线程一起工作。
```java
Thread.startVirtualThread(Runnable r)
```
使用虚拟线程的最基本方法是使用`Thread.startVirtualThread(Runnable r)`。这是实例化线程和调用`thread.start()`的替代方法。查看清单3中的示例代码。
<a name="fOTIm"></a>
### 实例化一个新线程 
```java
package com.infoworld;
import java.util.Random;
public class App {
    public static void main( String[] args ) {
        boolean vThreads = args.length > 0;
        System.out.println( "Using vThreads: " + vThreads);
        long start = System.currentTimeMillis();
        Random random = new Random();
        Runnable runnable = () -> { double i = random.nextDouble(1000) % random.nextDouble(1000);  }; 
        for (int i = 0; i < 50000; i++){
            if (vThreads){
                Thread.startVirtualThread(runnable);
            } else {
                Thread t = new Thread(runnable);
                t.start();
            }
        }
        long finish = System.currentTimeMillis();
        long timeElapsed = finish - start;
        System.out.println("Run time: " + timeElapsed);
    }
}
```
当带有参数运行时，清单3中的代码将使用一个虚拟线程，否则将使用常规线程。无论选择哪种线程类型，该程序都会生成5万次迭代。然后，它用随机数做一些简单的数学运算，并跟踪执行所需的时间。<br />要使用虚拟线程运行代码，需要键入：`mvn-compile-exec:java-Dexec.args="true"`。要使用标准线程运行，需要键入：`mvn-compile-exec:java`。为此进行了一个快速的性能测试，得到如下结果:

- 带有虚拟线程：Runtime: 174
- 使用常规线程：Runtime: 5450

这些结果是不科学的，但是运行时的差异是巨大的。<br />还有其他使用Thread生成虚拟线程的方法，例如`Thread.ofVirtual().start(runnable)`。 
<a name="HYbdi"></a>
### 使用执行器
启动虚拟线程的另一种主要方法是使用执行器。执行器在处理线程时很常见，它提供了一种协调许多任务和线程池的标准方法。 <br />虚拟线程不需要使用线程池，因为创建和处理它们的成本很低，因此没有必要使用线程池。与其相反，可以将JVM看作是管理线程池。但是，许多程序确实使用执行器，因此Java19在执行器中包含了一个新的预览方法，使重构虚拟线程变得容易。清单4展示了新方法和旧方法。
<a name="TNS4z"></a>
### 新的执行器方法
```java
ExecutorService executor = Executors.newVirtualThreadPerTaskExecutor(); // New method
ExecutorService executor = Executors.newFixedThreadPool(Integer poolSize); // Old method
```
此外，Java19引入了`Executors.newThreadPerTaskExecutor(ThreadFactory threadFactory)`方法，它可以采用构建虚拟线程的`ThreadFactory`。这样的线程工厂可以通过`Thread.ofVirtual().factory().`获得。
<a name="CcDNk"></a>
## 虚拟线程的优秀实践
一般来说，因为虚拟线程实现了线程类，所以它们可以在标准线程所在的任何地方使用。但是，在如何使用虚拟线程以获得最佳效果方面存在差异。一个例子是在访问数据存储等资源时使用信号量来控制线程数量，而不是使用有限制的线程池。<br />另一个重要注意事项是，虚拟线程始终守护线程，这意味着它们将使包含它们的JVM进程保持活动状态，直到它们完成。此外，不能更改它们的优先级。更改优先级和守护进程状态的方法为无操作（no-ops）。
<a name="yuhhl"></a>
## 使用虚拟线程重构
虚拟线程在本质上是一个很大的改变，但它们很容易应用到现有的代码库中。虚拟线程将对Tomcat和GlassFish等服务器产生最大、最直接的影响。这样的服务器应该能够以最小的努力采用虚拟线程。在这些服务器上运行的应用程序将获得可扩展性的收益，而无需对代码进行任何更改，这可能对大规模应用程序产生巨大影响。考虑一个运行在多个服务器和核心上的Java应用程序，突然之间它将能够处理一个数量级的并发请求，当然这完全取决于请求处理配置文件。<br />像Tomcat这样的服务器允许带配置参数的虚拟线程可能只是时间问题。与此同时，如果对将服务器迁移到虚拟线程感到好奇，可以阅读Cay Horstmann撰写的一篇博客文章，他在文章中展示了为虚拟线程配置Tomcat的过程。他启用了虚拟线程预览功能，并将Executor替换为只差一行的自定义实现。可扩展性的好处是显著的，正如他在文章中所说:“通过这种更改，200个请求只需3秒，而Tomcat可以轻松处理10,000个请求。”
<a name="qHpPY"></a>
## 结论
虚拟线程是JVM的一个主要变化。对于应用程序程序员来说，它们代表了异步风格编码(如使用回调)的另一种选择。总之，在处理Java并发性时，可以将虚拟线程看作是一个摆向Java中同步编程范式的钟摆。这在编程风格上大致类似于JavaScript引入的async/await(尽管在实现上完全不同)。简而言之，使用简单的同步语法编写正确的异步行为变得相当容易，至少在线程花费大量时间空闲的应用程序中是这样。
