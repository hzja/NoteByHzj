虚拟线程是轻量级线程（类似于 Go 中的 “协程[3]（Goroutine）”），可以减少编写、维护和调度高吞吐量并发应用程序的工作量。<br />线程是可供调度的最小处理单元，它与其他类似的处理单元并发运行，并且在很大程度上是独立运行的。线程（java.lang.Thread）有两种，平台线程和虚拟线程。
<a name="VfPQt"></a>
## **平台线程**
平台线程也就是之前的普通线程 java.lang.Thread 的实例，它被实现为对操作系统线程的简单包装，它通常以 1:1 的比例映射到由操作系统调度的内核线程中。它在其底层操作系统线程上运行 Java 代码，并且在它的整个生命周期内捕获着其映射的操作系统线程。因此，可用平台线程的数量局限于对应操作系统线程的数量。<br />平台线程通常有一个大的堆栈和其他由操作系统维护的资源，它适合运行所有类型的任务，但可供使用的资源可能有限。<br />平台线程可被指定为守护线程或非守护线程，除了守护线程状态之外，平台线程还具有线程优先级，并且是线程组的成员。默认情况下，平台线程会获得自动生成的线程名称。<br />与此同时，关于线程还有一些需要特别提到的变更，并值得注意：如果先前有通过直接 `new Thread(...)` 手工创建单个平台线程并使用（尽管此做法在大多数情况下是不推荐的）的话，请记住 Java 21 中的 `suspend()`、 `resume()`、`stop()` 和 `countStackFrames()` 等弃用方法将会直接抛出 UnsupportedOperationException 异常，可能会影响到之前的业务处理逻辑！
<a name="v9NdR"></a>
## **虚拟线程**
与平台线程一样，虚拟线程同样是 java.lang.Thread 的实例，但是，虚拟线程并不与特定的操作系统线程绑定。它与操作系统线程的映射关系比例也不是 1:1，而是 m:n。虚拟线程通常是由 Java 运行时来调度的，而不是操作系统。虚拟线程仍然是在操作系统线程上运行 Java 代码，但是，当在虚拟线程中运行的代码调用阻塞的 I/O 操作时，Java 运行时会将虚拟线程挂起，直到其可以恢复为止。此时与挂起的虚拟线程相关联的操作系统线程便可以自由地为其他虚拟线程来执行操作。<br />与平台线程不同，虚拟线程通常有一个浅层调用栈，它只需要很少的资源，单个 Java 虚拟机可能支持数百万个虚拟线程（也正因为如此，尽管虚拟线程支持使用 ThreadLocal 或 InheritableThreadLocal 等线程局部变量，也应该仔细考虑是否需要使用它们）。虚拟线程适合执行大部分时间被阻塞的任务，这些任务通常需要等待 I/O 操作完成，它不适合用于长时间运行的 CPU 密集型操作。<br />虚拟线程通常使用一小组平台线程作为载体线程（Carrier Thread），在虚拟线程中运行的代码不知道其底层的载体线程。<br />虚拟线程是守护线程，具有固定的线程优先级，不能更改。默认情况下，虚拟线程没有线程名称，如果未设置线程名称，则获取当前线程名称时将会返回空字符串。<br />那么，为什么要使用虚拟线程呢？<br />在高吞吐量并发应用程序中使用虚拟线程，尤其是那些包含由大量并发任务组成的应用程序，这些任务需要花费大量时间等待。例如服务器应用程序，因为它们通常处理许多执行阻塞 I/O 操作（例如获取资源）的客户端请求。<br />虚拟线程并不是更快的线程，它们运行代码的速度并不会比平台线程更快。它们的存在是为了提高扩展性（更高的吞吐量，而吞吐量意味着系统在给定时间内可以处理多少个信息单元），而不是速度（更低的延迟）。
<a name="im6N4"></a>
## **创建和运行虚拟线程**
<a name="sPwpJ"></a>
### **1、**`**Thread.ofVirtual()**`** 创建和运行虚拟线程**
```python
Thread thread = Thread.ofVirtual().start(() -> System.out.println("Hello"));
thread.join();   // 等待虚拟线程终止
```
`Thread.startVirtualThread(task)` 可以快捷地创建并启动虚拟线程，它与 `Thread.ofVirtual().start(task)` 是等价的。
<a name="VykUi"></a>
### **2、**`**Thread.Builder**`** 创建和运行虚拟线程**
`Thread.Builder` 接口允许创建具有通用的线程属性（例如线程名称）的线程，`Thread.Builder.OfPlatform` 子接口创建平台线程，而 `Thread.Builder.OfVirtual` 子接口则创建虚拟线程。
```python
Thread.Builder builder = Thread.ofVirtual().name("MyThread");  // 虚拟线程的名称是 MyThread
Runnable task = () -> System.out.println("Running thread");
Thread t = builder.start(task);
System.out.println("Thread t name: " + t.getName());      // 控制台打印：Thread t name: MyThread
t.join();
```
下面的示例代码创建了 2 个虚拟线程，名称分别是 worker-0 和 worker-1（这个是由 name() 中的两个参数 prefix 和 start 指定的）：
```java

Thread.Builder builder = Thread.ofVirtual().name("worker-", 0);
Runnable task = () -> System.out.println("Thread ID: " + Thread.currentThread().threadId());

// 虚拟线程 1，名称为 worker-0
Thread t1 = builder.start(task);
t1.join();
System.out.println(t1.getName() + " terminated");

// 虚拟线程 2，名称为 worker-1
Thread t2 = builder.start(task);
t2.join();  
System.out.println(t2.getName() + " terminated");
```
以上示例代码运行结果，在控制台中打印内容如下：
```java
Thread ID: 21
worker-0 terminated
Thread ID: 24
worker-1 terminated
```
<a name="TJa26"></a>
### **3、**`**Executors.newVirtualThreadPerTaskExecutor()**`** 创建和运行虚拟线程**
Executor 允许将线程管理和创建与应用程序的其余部分分开：
```java
// Java 21 中 ExecutorService 接口继承了 AutoCloseable 接口，
// 所以可以使用 try-with-resources 语法使 Executor 在最后被自动地 close()
try (ExecutorService myExecutor = Executors.newVirtualThreadPerTaskExecutor()) {
    // 每次 submit() 调用向 Executor 提交任务时都会创建和启动一个新的虚拟线程
    Future<?> future = myExecutor.submit(() -> System.out.println("Running thread"));
    future.get();  // 等待线程任务执行完成
    System.out.println("Task completed");
} catch (ExecutionException | InterruptedException ignore) {}
```
<a name="KJEcF"></a>
### **4、一个多线程的回显客户端服务器示例**
EchoServer 为回显服务器程序，监听本地 8080 端口并为每个客户端连接创建并启动一个新的虚拟线程：
```java
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer {
    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(8080)) {
            while (true) {
                try {
                    // 接受传入的客户端连接
                    Socket clientSocket = serverSocket.accept();
                    // 启动服务线程，处理这个客户端连接传输的数据并回显。可以通过虚拟线程同时服务多个客户端，每个客户端连接一个线程。
                    Thread.ofVirtual().start(() -> {
                        try (PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
                             BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()))) {
                            String inputLine;
                            while ((inputLine = in.readLine()) != null) {
                                System.out.println(inputLine);
                                out.println(inputLine);
                            }
                        } catch (IOException ignore) {}
                    });
                } catch (Throwable unknown) {
                    break;
                }
            }
        } catch (IOException e) {
            System.err.println("Exception caught when trying to listen on port 8080 or listening for a connection: " + e.getMessage());
            System.exit(1);
        }
    }
}
```
EchoClient 为回显客户端程序，它连接到本地的服务器并发送在命令行输入的文本消息：
```java
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class EchoClient {
    public static void main(String[] args) {
        try (Socket echoSocket = new Socket("127.0.0.1", 8080);
             PrintWriter out = new PrintWriter(echoSocket.getOutputStream(), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(echoSocket.getInputStream()))) {
            BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
            String userInput;
            while ((userInput = stdIn.readLine()) != null) {
                out.println(userInput);
                System.out.println("echo: " + in.readLine());
                if (userInput.equals("bye")) {
                    break;
                }
            }
        } catch (Exception e) {
            System.err.println("Couldn't get I/O for the connection to 127.0.0.1:8080: " + e.getMessage());
            System.exit(1);
        }
    }
}
```
在上面的示例程序代码中，可以看到 EchoServer 的 `while (true) {...}` 无限循环体内每次接受到一个新的客户端连接时，都创建和启动一个新的虚拟线程，并且没有用到虚拟线程池。请不要担心，事实上以上不管哪种创建和运行虚拟线程的方式，其背后都有一个线程池 ForkJoinPool（Carrier Thread 载体线程的池，这些载体线程是平台线程）。ForkJoinPool 的默认的调度参数：`parallelism` 并行度为计算机处理器的可用核心数、maxPoolSize 池的最大线程数为 256 和 `parallelism` 的最大值、`minRunnable` 允许的不被 join 或阻塞的最小核心线程数为 1 和 parallelism /2 的最大值，它们可以通过系统属性启动参数 `jdk.virtualThreadScheduler.parallelism`、`jdk.virtualThreadScheduler.maxPoolSize`、`jdk.virtualThreadScheduler.minRunnable` 自定义修改。
<a name="fR1lp"></a>
### **5、**`**CompletableFuture**`** 应当如何适应虚拟线程？**
`CompletableFuture` 平常用得比较多，在有虚拟线程以前，它一个惯常的使用方法如下：
```java
long startMills = System.currentTimeMillis();
ExecutorService executor = Executors.newFixedThreadPool(256);
List<CompletableFuture<Void>> futures = new ArrayList<>();
IntStream.range(0, 10000).forEach(i -> {
    // 如果 runAsync 不指定 Executor，则会使用默认的线程池（除非系统不支持并行，否则会使用一个通用的 ForkJoinPool.commonPool 线程池）
    CompletableFuture<Void> f = CompletableFuture.runAsync(() -> {
        try {
            Thread.sleep(1000L);
        } catch (InterruptedException ignore) {
            Thread.currentThread().interrupt();
        }
    }, executor);
    futures.add(f);
});
CompletableFuture.allOf(futures.toArray(new CompletableFuture[0])).join();
executor.shutdown();
System.out.println("【线程池】任务执行时间：" + (System.currentTimeMillis() - startMills) / 1000 + " 秒！");
```
以上示例代码运行结果，在控制台中打印内容如下：
```
【线程池】任务执行时间：40 秒！
```
在有虚拟线程后，其实改动非常少，只需要将平台线程池的 executor 替换为虚拟线程的 executor 即可：
```java
long startMills = System.currentTimeMillis();
List<CompletableFuture<Void>> futures = new ArrayList<>();
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    IntStream.range(0, 10000).forEach(i -> {
        // 如果 runAsync 不指定 Executor，则会使用默认的线程池（除非系统不支持并行，否则会使用一个通用的 ForkJoinPool.commonPool 线程池）
        CompletableFuture<Void> f = CompletableFuture.runAsync(() -> {
            try {
                Thread.sleep(1000L);
            } catch (InterruptedException ignore) {
                Thread.currentThread().interrupt();
            }
        }, executor);
        futures.add(f);
    });
    CompletableFuture.allOf(futures.toArray(new CompletableFuture[0])).join();
}
System.out.println("【虚拟线程】任务执行时间：" + (System.currentTimeMillis() - startMills) / 1000 + " 秒！");
```
以上示例代码运行结果，在控制台中打印内容如下：
```
【虚拟线程】任务执行时间：1 秒！
```
同时，也可以看到在这个示例代码的场景下，虚拟线程相比平台线程池的方案在性能上提升了约 40 倍！
<a name="PUJVP"></a>
## **调度和固定虚拟线程**
平台线程由操作系统来调度并决定何时运行，但是虚拟线程是由 Java 运行时来调度并决定何时运行的。当 Java 运行时调度虚拟线程时，它在平台线程上分配或挂载虚拟线程，然后操作系统像往常一样调度该平台线程，这个平台线程称为载体（Carrier）。运行一些代码后，虚拟线程可以从它的载体卸载，这通常发生在虚拟线程执行阻塞 I/O 操作时。虚拟线程从它的载体上卸载后，载体是空闲的，这意味着 Java 运行时调度器可以在其上挂载不同的虚拟线程。<br />在阻塞操作期间，当虚拟线程被固定到它的载体上时，它不能被卸载。虚拟线程在以下情况下会被固定（pinning）：

- 虚拟线程在 synchronized 同步块或方法中运行代码；
- 虚拟线程运行本地方法（native method）或外部函数（foreign function）。

固定不会使应用程序出错，但可能会影响其扩展性。尝试通过修改频繁运行的 synchronized 同步块或方法，并使用`java.util.concurrent.locks.ReentrantLock` 来保护可能长时间执行的 I/O 操作，以避免频繁和长时间的虚拟线程固定。
<a name="QNlyY"></a>
## **调试虚拟线程**
虚拟线程仍然是线程，调试器可以像平台线程那样对它们进行步进。Java Flight Recorder (JFR) 和 jcmd 工具具有额外的特性功能可以帮助观察应用程序中的虚拟线程。
<a name="W4ern"></a>
### **1、用于虚拟线程的 JFR 事件**
Java Flight Recorder (JFR) 可以发出以下与虚拟线程相关的事件：

- `jdk.VirtualThreadStart` 和 `jdk.VirtualThreadEnd` 虚拟线程的开始和结束的时间，这些事件在默认情况下是禁用的；
- `jdk.VirtualThreadPinned` 表示一个虚拟线程被固定（并且它的载体线程没有被释放）的超过阈值的持续时间，缺省情况下启用该事件，阈值为 20 毫秒；
- `jdk.VirtualThreadSubmitFailed` 表示启动或取消挂起（unpark）虚拟线程失败，可能是由于资源问题。挂起（park）一个虚拟线程释放底层的载体线程去做其他工作，取消挂起（unpark）一个虚拟线程以被调度它继续，该事件默认开启。

要打印这些事件，请运行以下命令，其中 recording.jfr 是记录的文件名：
```bash
jfr print --events jdk.VirtualThreadStart,jdk.VirtualThreadEnd,jdk.VirtualThreadPinned,jdk.VirtualThreadSubmitFailed recording.jfr
```
<a name="KWVmN"></a>
### **2、查看 jcmd 线程转储中的虚拟线程**
可以创建纯文本或 JSON 格式的线程转储：
```bash
jcmd <PID> Thread.dump_to_file -format=text <file>
jcmd <PID> Thread.dump_to_file -format=json <file>
```
jcmd 线程转储列出在网络 I/O 操作中阻塞的虚拟线程和由 ExecutorService 接口创建的虚拟线程。它不包括对象地址、锁、JNI 统计信息、堆统计信息和其他出现在传统线程转储中的信息。
<a name="LosXO"></a>
## **总结：虚拟线程采用指南**
虚拟线程是由 Java 运行时而不是操作系统实现的 Java 线程。虚拟线程和传统线程（现在称之为平台线程）之间的主要区别在于，可以很容易地在同一个 Java 进程中运行大量活动的虚拟线程，甚至数百万个。大量的虚拟线程赋予了它们强大的功能：通过允许服务器并发处理更多的请求，它们可以更有效地运行以每个请求一个线程的方式编写的服务器应用程序，从而实现更高的吞吐量和更少的硬件浪费。<br />由于虚拟线程是 java.lang.Thread 的实现，并且遵循自 Java SE 1.0 以来指定的 java.lang.Thread 的相同规则，因此开发人员不需要学习使用它们的新概念。然而，由于无法生成非常多的平台线程（多年来 Java 中唯一可用的线程实现），因此产生了旨在应对其高成本的实践做法。当这些做法应用于虚拟线程时会适得其反，必须摒弃。此外，成本上的巨大差异提示了一种考虑线程的新方式，这些线程一开始可能是外来的。
<a name="dnLIo"></a>
### **1、编写简单、同步的代码，采用单请求单线程风格的阻塞 I/O API**
虚拟线程可以显著提高以单请求单线程（Thread-Per-Request）的方式编写的服务器应用程序的吞吐量（而不是延迟）。在这种风格中，服务器在整个持续时间内专用一个线程来处理每个传入请求。它至少专用一个线程，因为在处理单个请求时，可能希望使用更多的线程来并发地执行一些任务。<br />阻塞平台线程的代价很高，因为它占用了系统线程（相对稀缺的资源），而它并没有做多少有意义的工作。因为虚拟线程可能很多，所以阻塞它们的成本很低，而且应该得到提倡。因此，应该以直接的同步风格编写代码，并使用阻塞 I/O API。<br />以下这种以非阻塞、异步风格编写的代码不会从虚拟线程中获得太多好处：
```java
CompletableFuture.supplyAsync(info::getUrl, pool)
        .thenCompose(url -> getBodyAsync(url, HttpResponse.BodyHandlers.ofString()))
        .thenApply(info::findImage)
        .thenCompose(url -> getBodyAsync(url, HttpResponse.BodyHandlers.ofByteArray()))
        .thenApply(info::setImageData)
        .thenAccept(this::process)
        .exceptionally(ignore -> null);
```
但是下面这种以同步风格编写并使用简单阻塞 I/O 的代码却将受益匪浅：
```java
try {
    String page = getBody(info.getUrl(), HttpResponse.BodyHandlers.ofString());
    String imageUrl = info.findImage(page);
    byte[] data = getBody(imageUrl, HttpResponse.BodyHandlers.ofByteArray());
    info.setImageData(data);
    process(info);
} catch (Exception ignore) {}
```
这样的代码也更容易在调试器中进行调试，在分析器中进行概要分析，或者使用线程转储进行观察。为了观察虚拟线程，使用 jcmd 命令创建一个线程转储：
```bash
jcmd <pid> Thread.dump_to_file -format=json <file>
```
以这种风格编写的堆栈越多，虚拟线程的性能和可观察性就越好。用其他风格编写的程序或框架，如果没有为每个任务指定一个线程，就不应该期望从虚拟线程中获得显著的好处。避免将同步、阻塞代码与异步框架混在一起。
<a name="sHkGr"></a>
### **2、将每个并发任务表示为一个虚拟线程，不要池化虚拟线程**
关于虚拟线程，最难内化的是，虽然它们具有与平台线程相同的行为，但它们不应该表示相同的程序概念。<br />平台线程是稀缺的，因此是一种宝贵的资源。需要管理宝贵的资源，管理平台线程的最常用方法是使用线程池。接下来需要回答的问题是，池中应该有多少线程？<br />但是虚拟线程非常多，因此每个线程不应该代表一些共享的、池化的资源，而应该代表一个任务。线程从托管资源转变为应用程序域对象。应该有多少个虚拟线程的问题变得很明显，就像应该使用多少个字符串在内存中存储一组用户名的问题一样：虚拟线程的数量总是等于应用程序中并发任务的数量。<br />将 n 个平台线程转换为 n 个虚拟线程不会产生什么好处；相反，需要转换的是任务。<br />为了将每个应用程序任务表示为一个线程，不要像下面的例子那样使用共享线程池执行器：
```java
Future<ResultA> f1 = sharedThreadPoolExecutor.submit(task1);
Future<ResultB> f2 = sharedThreadPoolExecutor.submit(task2);
// ... 使用 f1、f2
```
相反地，应该使用虚拟线程执行器，如下例所示：
```java
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {  // 注意这里实际上并没有将虚拟线程进行池化
    Future<ResultA> f1 = executor.submit(task1);
    Future<ResultB> f2 = executor.submit(task2);
    // ... 使用 f1、f2
}
```
代码仍然使用 ExecutorService，但是从 `Executors.newVirtualThreadPerTaskExecutor()` 返回的那个没有使用线程池。相反，它为每个提交的任务创建一个新的虚拟线程。<br />此外，ExecutorService 本身是轻量级的，可以创建一个新的，就像处理任何简单的对象一样。这允许依赖于新添加的`ExecutorService.close()` 方法和 try-with-resources 语句。在 try 块结束时隐式调用的 close 方法将自动等待提交给ExecutorService 的所有任务（即由 ExecutorService 生成的所有虚拟线程）终止。<br />对于 fanout 场景，这是一个特别有用的模式，在这种场景中，希望并发地向不同的服务执行多个传出调用，如下面的示例所示：
```java
void handle(Request request, Response response) {
    var url1 = ...
    var url2 = ...

    try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
        var future1 = executor.submit(() -> fetchURL(url1));
        var future2 = executor.submit(() -> fetchURL(url2));
        response.send(future1.get() + future2.get());
    } catch (ExecutionException | InterruptedException e) {
        response.fail(e);
    }
}

String fetchURL(URL url) throws IOException {
    try (var in = url.openStream()) {
        return new String(in.readAllBytes(), StandardCharsets.UTF_8);
    }
}
```
应该创建一个新的虚拟线程，如上例所示，即使是小型的、短暂的并发任务也是如此。<br />为了在编写 fanout 模式和其他常见并发模式时获得更多帮助，并且具有更好的可观察性，请使用结构化并发。<br />根据经验，如果应用程序从来没有 10000 个或更多的虚拟线程，那么它不太可能从虚拟线程中获益。要么它的负载太轻，不需要更好的吞吐量，要么没有向虚拟线程表示有足够多的任务。
<a name="qLjf6"></a>
### **3、使用信号量限制并发**
有时需要限制某个确定操作的并发性。例如，某些外部服务可能无法处理 10 个以上的并发请求。由于平台线程是通常在池中管理的宝贵资源，因此线程池已经变得如此普遍，以至于它们被用于限制并发性的目的，如下例所示：
```java
ExecutorService es = Executors.newFixedThreadPool(10);  // 固定线程池的核心及最大线程数量为 10
...
Result foo() {
    try {
        var fut = es.submit(() -> callLimitedService());
        return f.get();
    } catch (...){ ...}
}
```
此示例确保对有限的服务最多有 10 个并发请求。<br />但是限制并发性只是线程池操作的副作用。池被设计为共享稀缺资源，而虚拟线程并不稀缺，因此永远不应该被池化！<br />在使用虚拟线程时，如果希望限制访问某些服务的并发性，则应该使用专门为此目的设计的构造：Semaphore 类。如下示例：
```java
Semaphore sem = new Semaphore(10);  // 初始化一个信号量，拥有 10 个许可
...
Result foo() {
    sem.acquire();          // 申请许可，如果当前没有许可了，则阻塞直至其他线程 release 以释放许可
    try {
        return callLimitedService(); // 只有申请并获得了许可的线程，才能进入此处执行业务逻辑，从而控制了并发性
    } finally {
        sem.release();        // 释放许可，以供其他线程使用
    }
}
```
简单地用信号量阻塞一些虚拟线程可能看起来与将任务提交到一个固定线程池有很大的不同，但事实上并非如此。将任务提交到线程池会将它们排队等待以供稍后执行，但是信号量内部（或任何其他类似的阻塞同步构造）会创建一个阻塞在它上面的线程队列，这些线程被阻塞在其上，与等待池化的平台线程来执行它们的任务队列相对应。因为虚拟线程即是任务，所以其结果结构是等价的：<br />![图示：对比线程池和信号量](https://cdn.nlark.com/yuque/0/2023/png/396745/1698935642836-d03ad11c-6f23-4073-a468-4603cf9cf554.png#averageHue=%232e2923&clientId=u1ff6f909-f490-4&from=paste&id=uce28a185&originHeight=528&originWidth=631&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u2eb5c468-01b5-4e04-b116-5db2e473f24&title=%E5%9B%BE%E7%A4%BA%EF%BC%9A%E5%AF%B9%E6%AF%94%E7%BA%BF%E7%A8%8B%E6%B1%A0%E5%92%8C%E4%BF%A1%E5%8F%B7%E9%87%8F "图示：对比线程池和信号量")<br />即使可以将平台线程池视为从队列中提取任务并处理它们的工作线程，而将虚拟线程视为等待继续执行的任务本身，但在计算机中的基础表示实际上几乎相同。认识到排队的任务和被阻塞的线程之间的等效性将有助于充分利用虚拟线程。
<a name="Cu2gH"></a>
### **4、不要在线程局部变量中缓存昂贵的可重用对象**
虚拟线程与平台线程一样支持线程局部变量。通常，线程局部变量用于将某些与当前运行的代码相关的上下文特定信息关联起来，例如当前的事务和用户 ID。在虚拟线程中，使用线程本地变量来实现这种用途是完全合理的。但是，考虑使用更安全和更高效的作用域值（java.lang.ScopedValue，当前为预览特性）。<br />还有一种使用线程局部变量的方式与虚拟线程存在根本性冲突：缓存可重复使用的对象。这些对象通常创建昂贵（并消耗大量内存），是可变的，并且不是线程安全的。它们被缓存在线程局部变量中，以减少它们被实例化的次数和内存中的实例数量，但它们会被在不同时间运行在线程上的多个任务重复使用。<br />例如，SimpleDateFormat 的实例创建昂贵且不是线程安全的。一种常见的做法是将这样的实例缓存在 ThreadLocal 中，如下例所示：
```java
static final ThreadLocal<SimpleDateFormat> cachedFormatter = ThreadLocal.withInitial(SimpleDateFormat::new);

void foo() {
    ...
    cachedFormatter.get().format(...);
    ...
}
```
这种类型的缓存仅在线程（因此在线程局部缓存的昂贵对象）被多个任务共享和重复使用时才有帮助，就像在平台线程池中的池化线程时的情况一样。在线程池中运行时，许多任务可能会调用 foo，但由于池中只包含一些线程，该对象只会被实例化几次 - 每个池线程一次 - 然后被缓存和重复使用。<br />然而，虚拟线程从不被池化，也不会被不相关的任务重复使用。因为每个任务都有自己的虚拟线程，来自不同任务的每次对 foo 的调用都会触发新的  SimpleDateFormat 实例的实例化。而且，由于可能有大量虚拟线程同时运行，昂贵的对象可能会消耗大量内存。这与线程局部缓存的预期成果完全相反。<br />没有单一的通用替代方案，但在 SimpleDateFormat 的情况下，应该将其替换为 `DateTimeFormatter`。`DateTimeFormatter` 是不可变的，因此可以由所有线程共享单个实例：
```java
static final DateTimeFormatter formatter = DateTimeFormatter….;

void foo() {
    ...
    formatter.format(...);
    ...
}
```
请注意，有时候，使用线程局部变量来缓存共享的昂贵对象是由异步框架在幕后完成的，这是它们的隐式假设，认为它们会被一个非常小的线程池中的线程使用。这就是为什么混合使用虚拟线程和异步框架不是一个好主意的原因之一：调用一个方法可能会导致在本应缓存和共享的线程本地变量中实例化昂贵的对象。
<a name="kQlWa"></a>
### **5、避免长时间和频繁的固定**
目前虚拟线程的实现存在一个限制，即在 synchronized 同步块或方法内执行阻塞操作会导致 JDK 的虚拟线程调度器阻塞一个宝贵的操作系统线程，而如果阻塞操作在 synchronized 同步块或方法之外执行，就不会出现这种情况。称这种情况为 “pinning”（固定）。如果阻塞操作既长时间存在又频繁发生，pinning 可能会对服务器的吞吐量产生不利影响。使用 synchronized 同步块或方法保护短时操作（例如内存操作）或不频繁的操作应该不会产生不利影响。<br />为了检测可能有害的 pinning 情况，JDK Flight Recorder（JFR）在阻塞操作被固定时会发出 `jdk.VirtualThreadPinned` 线程事件；默认情况下，当操作持续时间超过 20 毫秒时，此事件被启用。<br />或者，可以使用系统属性 `jdk.tracePinnedThreads`，在线程被固定时发出堆栈跟踪。使用选项 `-Djdk.tracePinnedThreads=full` 时，当线程被固定时会打印完整的堆栈跟踪，突出显示本机帧和持有监视器的帧。使用选项 `-Djdk.tracePinnedThreads=short` 时，输出将限制为仅包括有问题的帧。<br />如果这些机制检测到 pinning 在某些地方既长时间存在又频繁发生，那么在那些特定地方使用 ReentrantLock 替代 synchronized（再次强调，**不需要替代用于保护短时操作或不频繁操作的 synchronized**）。以下是一个长时间存在且频繁使用同步块的示例：
```java
synchronized(lockObj) {
    frequentIO();
}
```
可以将其替换为：
```java
lock.lock();
try {
    frequentIO();
} finally {
    lock.unlock();
}
```
<a name="AVeQn"></a>
## 引用链接
JEP 虚拟线程: [https://openjdk.org/jeps/444](https://openjdk.org/jeps/444)<br />JEP 结构化并发: [https://openjdk.org/jeps/453](https://openjdk.org/jeps/453)<br />Go 协程（Goroutine）: [https://go.dev/doc/effective_go#goroutines](https://go.dev/doc/effective_go#goroutines)<br />Java SE 21 核心库文档：14. 并发 - 虚拟线程: [https://docs.oracle.com/en/java/javase/21/core/virtual-threads.html](https://docs.oracle.com/en/java/javase/21/core/virtual-threads.html)
