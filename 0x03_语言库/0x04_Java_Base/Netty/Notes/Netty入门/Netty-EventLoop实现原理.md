JavaNetty
<a name="LX1sj"></a>
## 前言
EventLoop 是一种事件等待和处理的程序模型，可以解决多线程资源消耗高的问题。不仅占用资源低，而且能够支撑了大规模的流量访问。<br />下图展示了 EventLoop 通用的运行模式。每当事件发生时，应用程序都会将产生的事件放入事件队列当中，然后 EventLoop 会轮询从队列中取出事件执行或者将事件分发给相应的事件监听者执行。事件执行的方式通常分为立即执行、延后执行、定期执行几种。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694484470685-35c4381c-fa61-486f-854e-a751238c0ae6.png#averageHue=%23fcfefe&clientId=uae268e9d-9e35-4&from=paste&id=u860abb0f&originHeight=372&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uebe907ad-41f4-479e-847b-757c2fcd46f&title=)
<a name="SkWQW"></a>
## Netty 如何实现 EventLoop
在 Netty 中 EventLoop 可以理解为 Reactor 线程模型的事件处理引擎，每个 EventLoop 线程都维护一个 Selector 选择器和任务队列 TaskQueue。它主要负责处理 I/O 事件、普通任务和定时任务。<br />Netty 中推荐使用 NioEventLoop 作为实现类，那么 Netty 是如何实现 NioEventLoop 的呢？首先来看 NioEventLoop 最核心的 `run()` 方法源码，不会对源码做深入的分析，只是先了解 NioEventLoop 的实现结构。(后续会有详细的解读)
```java
protected void run() {
    for (;;) {
        try {
            try {
                switch (selectStrategy.calculateStrategy(selectNowSupplier, hasTasks())) {
                    case SelectStrategy.CONTINUE:
                        continue;

                    case SelectStrategy.BUSY_WAIT:
                        // fall-through to SELECT since the busy-wait is not supported with NIO

                    case SelectStrategy.SELECT:
                        // 轮询I/O事件
                        select(wakenUp.getAndSet(false)); 

                        if (wakenUp.get()) {
                            selector.wakeup();
                        }
                        // fall through
                    default:
                }
            } catch (IOException e) {
                // If we receive an IOException here its because the Selector is messed up. Let's rebuild
                // the selector and retry. https://github.com/netty/netty/issues/8566
                rebuildSelector0();
                handleLoopException(e);
                continue;
            }

            cancelledKeys = 0;
            needsToSelectAgain = false;
            final int ioRatio = this.ioRatio;
            if (ioRatio == 100) {
                try {
                    // 处理I/O任务
                    processSelectedKeys();
                } finally {
                    // Ensure we always run tasks.
                    // 处理所有的任务
                    runAllTasks();   
                }
            } else {
                final long ioStartTime = System.nanoTime();
                try {
                    // 处理I/O任务
                    processSelectedKeys();
                } finally {
                    // Ensure we always run tasks.
                    final long ioTime = System.nanoTime() - ioStartTime;
                    // 处理完I/O任务，在处理异步任务，处理的时间跟I/O处理完成的时间相同，通过 ioRatio来控制
                    runAllTasks(ioTime * (100 - ioRatio) / ioRatio);
                }
            }
        } catch (Throwable t) {
            handleLoopException(t);
        }
        // Always handle shutdown even if the loop processing threw an exception.
        try {
            if (isShuttingDown()) {
                closeAll();
                if (confirmShutdown()) {
                    return;
                }
            }
        } catch (Throwable t) {
            handleLoopException(t);
        }
    }
}
```
上述源码的结构比较清晰，NioEventLoop 每次循环的处理流程都包含事件轮询 Select、事件处理 `processSelectedKeys`、任务处理 `runAllTasks` 几个步骤，是典型的 Reactor 线程模型的运行机制。而且 Netty 提供了一个参数 ioRatio，可以调整 I/O 事件处理和任务处理的时间比例。
<a name="NNkAe"></a>
## 事件处理机制
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694484470764-a585ef73-45a2-49cf-af46-50d56fa0fcb9.png#averageHue=%23f6faf8&clientId=uae268e9d-9e35-4&from=paste&id=u27a58f94&originHeight=385&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8501cca9-d719-4a02-bdb0-2d7a4a363b2&title=)<br /> <br />NioEventLoop 的事件处理机制采用的是无锁串行化的设计思路。

- `BossEventLoopGroup` 和 `WorkerEventLoopGroup` 包含一个或者多个 `NioEventLoop`。`BossEventLoopGroup` 负责监听客户端的 Accept 事件，当事件触发时，将事件注册至 `WorkerEventLoopGroup` 中的一个 `NioEventLoop` 上。每新建一个 Channel， 只选择一个 `NioEventLoop` 与其绑定。所以说 Channel（图中 `NioSocketChannel`） 生命周期的所有事件处理都是线程独立的，不同的 `NioEventLoop` 线程之间不会发生任何交集。
- `NioEventLoop` 完成数据读取后，会调用绑定的 `ChannelPipeline` 进行事件传播，`ChannelPipeline` 也是线程安全的，数据会被传递到 `ChannelPipeline` 的第一个 `ChannelHandler` 中。数据处理完成后，将加工完成的数据再传递给下一个 `ChannelHandler`，整个过程是串行化执行，不会发生线程上下文切换的问题。

`NioEventLoop` 无锁串行化的设计不仅使系统吞吐量达到最大化，而且降低了用户开发业务逻辑的难度，不需要花太多精力关心线程安全问题。虽然单线程执行避免了线程切换，但是它的缺陷就是不能执行时间过长的 I/O 操作，一旦某个 I/O 事件发生阻塞，那么后续的所有 I/O 事件都无法执行，甚至造成事件积压。在使用 Netty 进行程序开发时，一定要对 `ChannelHandler` 的实现逻辑有充分的风险意识。<br />`NioEventLoop` 线程的可靠性至关重要，一旦 `NioEventLoop` 发生阻塞或者陷入空轮询，就会导致整个系统不可用。在 JDK 中， Epoll 的实现是存在漏洞的，即使 Selector 轮询的事件列表为空，NIO 线程一样可以被唤醒，导致 CPU 100% 占用。这就是臭名昭著的 JDK epoll 空轮询的 Bug。Netty 作为一个高性能、高可靠的网络框架，需要保证 I/O 线程的安全性。那么它是如何解决 JDK epoll 空轮询的 Bug 呢？实际上 Netty 并没有从根源上解决该问题，而是巧妙地规避了这个问题。
```java
long time = System.nanoTime();
if (time - TimeUnit.MILLISECONDS.toNanos(timeoutMillis) >= currentTimeNanos) {
    // timeoutMillis elapsed without anything selected.
    selectCnt = 1;
} else if (SELECTOR_AUTO_REBUILD_THRESHOLD > 0 &&
           selectCnt >= SELECTOR_AUTO_REBUILD_THRESHOLD) {
    // The code exists in an extra method to ensure the method is not too big to inline as this
    // branch is not very likely to get hit very frequently.
    selector = selectRebuildSelector(selectCnt);
    selectCnt = 1;
    break;
}
```
Netty 提供了一种检测机制判断线程是否可能陷入空轮询，具体的实现方式如下：

- 每次执行 Select 操作之前记录当前时间 `currentTimeNanos`。
- `time - TimeUnit.MILLISECONDS.toNanos(timeoutMillis) >= currentTimeNanos`，如果事件轮询的持续时间大于等于 timeoutMillis，那么说明是正常的，否则表明阻塞时间并未达到预期，可能触发了空轮询的 Bug。
- Netty 引入了计数变量 selectCnt。在正常情况下，`selectCnt` 会重置，否则会对 `selectCnt` 自增计数。当 selectCnt 达到 `SELECTOR_AUTO_REBUILD_THRESHOLD`（默认 512）阈值时，会触发重建 Selector 对象。

Netty 采用这种方法巧妙地规避了 JDK Bug。异常的 Selector 中所有的 SelectionKey 会重新注册到新建的 Selector 上，重建完成之后异常的 Selector 就可以废弃了。
<a name="ohZAq"></a>
## 任务处理机制
`NioEventLoop` 不仅负责处理 I/O 事件，还要兼顾执行任务队列中的任务。任务队列遵循 FIFO 规则，可以保证任务执行的公平性。`NioEventLoop` 处理的任务类型基本可以分为三类。

- 普通任务：通过 `NioEventLoop` 的 `execute()` 方法向任务队列 `TaskQueue` 中添加任务。例如 Netty 在写数据时会封装 `WriteAndFlushTask` 提交给 `TaskQueue`。`TaskQueue` 的实现类是多生产者单消费者队列 `MpscChunkedArrayQueue`，在多线程并发添加任务时，可以保证线程安全。
- 定时任务：通过调用 `NioEventLoop` 的 `schedule()` 方法向定时任务队列 `scheduledTaskQueue` 添加一个定时任务，用于周期性执行该任务。例如，心跳消息发送等。定时任务队列 `scheduledTaskQueue` 采用优先队列 `PriorityQueue` 实现。
- 尾部队列：`tailTasks` 相比于普通任务队列优先级较低，在每次执行完 `TaskQueue` 中任务后会去获取尾部队列中任务执行。尾部任务并不常用，主要用于做一些收尾工作，例如统计事件循环的执行时间、监控信息上报等。
```java
protected boolean runAllTasks(long timeoutNanos) {
    // 1.合并定时任务到普通任务队列
    fetchFromScheduledTaskQueue();
    
    // 2.从普通任务队列中取出任务
    Runnable task = pollTask();
    if (task == null) {
        afterRunningAllTasks();
        return false;
    }
    
    // 3.计算任务处理的超时时间
    final long deadline = ScheduledFutureTask.nanoTime() + timeoutNanos;
    long runTasks = 0;
    long lastExecutionTime;
    for (;;) {
        // 4.安全的执行任务
        safeExecute(task);
    
        runTasks ++;
    
        // Check timeout every 64 tasks because nanoTime() is relatively expensive.
        // XXX: Hard-coded value - will make it configurable if it is really a problem.
        // 5.每执行64个任务检查下是否超时
        if ((runTasks & 0x3F) == 0) {
            lastExecutionTime = ScheduledFutureTask.nanoTime();
            if (lastExecutionTime >= deadline) {
                break;
            }
        }
    
        task = pollTask();
        if (task == null) {
            lastExecutionTime = ScheduledFutureTask.nanoTime();
            break;
        }
    }
    
    // 6.收尾工作
    afterRunningAllTasks();
    this.lastExecutionTime = lastExecutionTime;
    return true;
}
```

- `fetchFromScheduledTaskQueue` 函数：将定时任务从 `scheduledTaskQueue` 中取出，聚合放入普通任务队列 `TaskQueue` 中，只有定时任务的截止时间小于当前时间才可以被合并。
- 从普通任务队列 `TaskQueue` 中取出任务。
- 计算任务执行的最大超时时间。
- `safeExecute` 函数：安全执行任务，实际直接调用的 Runnable 的 `run()` 方法。
- 每执行 64 个任务进行超时时间的检查，如果执行时间大于最大超时时间，则立即停止执行任务，避免影响下一轮的 I/O 事件的处理。
- 最后获取尾部队列中的任务执行。
<a name="YTkyN"></a>
## EventLoop 最佳实践

- 网络连接建立过程中三次握手、安全认证的过程会消耗不少时间。这里建议采用 Boss 和 Worker 两个 `EventLoopGroup`，有助于分担 Reactor 线程的压力。
- 由于 Reactor 线程模式适合处理耗时短的任务场景，对于耗时较长的 `ChannelHandler` 可以考虑维护一个业务线程池，将编解码后的数据封装成 Task 进行异步处理，避免 `ChannelHandler` 阻塞而造成 EventLoop 不可用。
- 如果业务逻辑执行时间较短，建议直接在 `ChannelHandler` 中执行。例如编解码操作，这样可以避免过度设计而造成架构的复杂性。
- 不宜设计过多的 `ChannelHandler`。对于系统性能和可维护性都会存在问题，在设计业务架构的时候，需要明确业务分层和 Netty 分层之间的界限。不要一味地将业务逻辑都添加到 `ChannelHandler` 中。
<a name="Gt1OX"></a>
## 总结
学习了 Netty Reactor 线程模型的核心处理引擎 EventLoop，熟悉了 EventLoop 的来龙去脉。结合 Reactor 主从多线程模型，对 Netty EventLoop 的功能用处做一个简单的归纳总结。

- `MainReactor` 线程：处理客户端请求接入。
- `SubReactor` 线程：数据读取、I/O 事件的分发与执行。
- 任务处理线程：用于执行普通任务或者定时任务，如空闲连接检测、心跳上报等。
<a name="YKKVm"></a>
## 问答
**1、Eventloop 负责事件轮询和事件执行，任务执行，那事件分发是谁负责，还有 **`**NioServerSocketChannel**`** 和 **`**NioSocketChannel**`** 到底啥区别，服务端有两个通道吗，**`**NioEventLoop**`** 完成数据读取，是指从哪个通道完成数据读取?**<br />回复：

1. 事件分发和执行可以统一理解，都是 `EventLoop` 负责的，Reactor 主从多线程模型中，主 Reactor 负责接收客户端连接，并负责分发到从 Reactor 中。
2. Netty 服务端 Channel 的类型是 `NioServerSocketChannel`，而客户端 Channel 的类型是 `NioSocketChannel`。
3. 数据是从客户端 Channel 读取的。

**2、如果客户端连接过多，而 **`**WorkerEventLoop**`** 不够的时候，是怎么处理的？例如 4 核 CPU 默认初始化的 NioEventLoop 是 8 个，有 20 个客户端连接了，这时候是不是会创建 20 个 **`**NioSocketChannel**`**，然后绑定到这 8 个 EventLoop 上，那必然会有 Channel 共享 EventLoop 吧？**<br />回复：对的，海量 Channel 共享一定数量的 EventLoop。<br />**3、在某些 **`**ChannelHandler**`** 中把 task 丢到自定义的线程池中么，那什么时候拿到对应的结果输出到 outbound 中进行返回？每一个 Eventloop 都有关联的 TaskQueue，类似于一个线程一个任务队列，和线程池多个线程一个任务队列的设计不太一样，那么 boss 应该也有分配任务给各个 Eventloop 的分配策略？**<br />回复：

1. 当异步线程处理完后，可以主动触发数据回写，例如调用 `writeAndFlush`，只要你持有 ctx 的引用即可。
2. Boss 会为客户端 Channel 选择一个 Worker，Boss 和 Worker 都是操作各自的任务队列。
