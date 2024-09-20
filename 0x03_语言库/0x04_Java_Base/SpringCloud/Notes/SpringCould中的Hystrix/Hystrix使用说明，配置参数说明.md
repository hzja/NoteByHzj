Java SpringCloud Hystrix
<a name="yjMp3"></a>
## 1、概述
Hystrix使用Archaius作为配置属性的默认实现。官方配置文档：[https://github.com/Netflix/Hystrix](https://github.com/Netflix/Hystrix)<br />每个属性有四个优先级，依次增大：

- 1：代码的全局默认值
- 2：动态全局默认属性<br />可以使用全局属性文件来更改全局默认值。
- 3：代码实例默认<br />定义特定于实例的默认值，比如在`HystrixCommand`构造函数中设置的值
- 4：动态实例属性<br />可以动态设置实例特定的值，从而覆盖前面三个默认级别，格式是：<br />`hystrix.command.命令key.属性名称=值`
<a name="eRYDY"></a>
## 2、请求上下文
<a name="dXN7p"></a>
### 1：`requestCache.enabled`
设置是否开启请求的缓存功能，默认`true`
<a name="O6RmF"></a>
### 2：`requestLog.enabled`
设置是否开启请求的日志功能，默认`true`
<a name="AwBey"></a>
## 3、命令执行
<a name="igB4p"></a>
### `execution.isolation.strategy`
指示`HystrixCommand.run()`执行哪个隔离策略，选项：<br />1：THREAD - 它在单独的线程上执行，并发请求受线程池中线程数的限制<br />2：SEMAPHORE - 它在调用线程上执行，并发请求受信号计数的限制<br />3：官方推荐使用线程隔离策略，默认也是按照线程隔离进行处理。<br />4：信号量隔离的方式是限制了总的并发数，每一次请求过来，请求线程和调用依赖服务的线程是同一个线程，那么如果不涉及远程RPC调用（没有网络开销）则使用信号量来隔离，更为轻量，开销更小。<br />5：信号量的大小可以动态调整, 线程池大小不可以动态调整。<br />6：配置示例：
```java
@HystrixCommand(fallbackMethod = "error", commandProperties = { @HystrixProperty(name="execution.isolation.strategy", value = "THREAD") })
```
<a name="KcCyp"></a>
### `execution.isolation.thread.timeoutInMilliseconds`
表示请求线程总超时时间，如果超过这个设置的时间hystrix就会调用`fallback`方法。value的参数为毫秒，默认值为1000ms。
```java
@HystrixCommand(fallbackMethod = "error", commandProperties = { @HystrixProperty(name = "execution.isolation.thread.timeoutInMilliseconds", value = "4000") })
```
<a name="X9er3"></a>
### `execution.timeout.enabled`
这个超时开关表示，当超时后是否触发`fallback`方法，默认为true。
<a name="a8Hn7"></a>
### `execution.isolation.thread.interruptOnTimeout`
表示`HystrixCommand.run()`在发生超时时是否应该中断执行，默认true
<a name="rCnxw"></a>
### `execution.isolation.thread.interruptOnCance`
表示`HystrixCommand.run()`在发生取消时是否应该中断执行，默认false
<a name="GHaiV"></a>
### `execution.isolation.semaphore.maxConcurrentRequests`
当隔离策略使用`SEMAPHORE`时，最大的并发请求量，如果请求超过这个最大值将拒绝后续的请求，默认值为10
<a name="t7pXh"></a>
## 4、回退
<a name="agbXk"></a>
### `fallback.isolation.semaphore.maxConcurrentRequests`
设置`HystrixCommand.getFallback()`方法允许从调用线程进行请求的最大数量，默认10<br />如果达到最大并发限制，则随后的请求将被拒绝，并抛出异常。
<a name="xPudD"></a>
### `fallback.enabled`
开启`fallback`功能，默认true
<a name="O9hy0"></a>
## 5、断路器
<a name="qdo8Q"></a>
### `circuitBreaker.enabled`
设置是否将使用断路器来跟踪健康状况，并且如果断路器跳闸则将其短路。默认true
<a name="ZfpIK"></a>
### `circuitBreaker.requestVolumeThreshold`
设置滚动窗口中将使电路跳闸的最小请求数量，默认20。<br />熔断器在整个统计时间内是否开启的阀值，每个熔断器默认维护10个bucket,每秒一个bucket,每个bucket记录成功,失败,超时,拒绝的状态，该阈值默认20次。也就是一个统计窗口时间内（10秒钟）至少请求20次，熔断器才启动。
<a name="cbfoK"></a>
### `circuitBreaker.sleepWindowInMilliseconds`
熔断器默认工作时间,默认值为5秒，熔断器中断请求5秒后会进入半打开状态,放部分流量过去重试，如果重试成功则会恢复正常请求。
<a name="Hh5EK"></a>
### `circuitBreaker.errorThresholdPercentage`
熔断器错误阈值，默认为50%。当在一个时间窗口内出错率超过50%后熔断器自动启动。熔断器启动后会自动转发到配置的fallbackMethod，进行降级处理。
<a name="ZL8GQ"></a>
### `circuitBreaker.forceOpen`
断路器强制开关，如果设置为true则表示强制打开熔断器，所有请求都会拒绝，默认false
<a name="HW3U5"></a>
### `circuitBreaker.forceClosed`
断路器强制开关，如果设置为true则表示强制关闭熔断器，所有请求都会允许，默认false
<a name="RdoQq"></a>
## 6、度量指标
<a name="CRxmJ"></a>
### `metrics.rollingStats.timeInMilliseconds`
设置统计滚动窗口的持续时间，以毫秒为单位。默认10秒
<a name="O6vmr"></a>
### `metrics.rollingStats.numBuckets`
该属性设置滚动统计窗口分成的桶的数量。默认10
<a name="ipwqA"></a>
### `metrics.rollingPercentile.enabled`
表示执行延迟是否应该跟踪和计算为百分比。如果被禁用，则所有汇总统计返回为-1。默认true
<a name="UVswA"></a>
### `metrics.rollingPercentile.timeInMilliseconds`
设置滚动窗口的持续时间，在该窗口中保留执行时间以允许百分数计算（单位毫秒），默认1分钟。
<a name="MFpdr"></a>
### `metrics.rollingPercentile.numBuckets`
设置`rollingPercentile`窗口将被分成的桶的数量，默认6
<a name="WgMn0"></a>
### `metrics.rollingPercentile.bucketSize`
设置每个存储桶的最大执行次数，如果在执行多次，将在桶的开头重写。默认100<br />例如，如果存储区大小设置为100，并表示10秒的存储区窗口，但在此期间发生500次执行，则只有最后100次执行将保留在该10秒存储区中
<a name="x3rvI"></a>
### `metrics.healthSnapshot.intervalInMilliseconds`
设置允许执行健康快照之间等待的时间（以毫秒为单位）。默认500
<a name="GJ5A9"></a>
## 7、ThreadPool配置
<a name="oYnMW"></a>
### `coreSize`
线程池核心线程数，默认值为10。
<a name="IEuzS"></a>
### `MAXIMUMSIZE`
设置线程池大小，默认10
<a name="JWntU"></a>
### `maxQueueSize`
配置线程池任务队列的大小，默认值为 -1。当使用 -1 时，`SynchronousQueue`将被使用，即意味着其实这个队列只是一个交换器，任务将被直接交给工作线程处理。如果工作线程不足，那任务将被拒绝；如果使用任何正整数，`LinkedBlockingQueue`将被使用
<a name="LzPsA"></a>
### `queueSizeRejectionThreshold`
表示等待队列超过阈值后开始拒绝线程请求，默认值为5，如果`maxQueueSize`为-1，则该属性失效。
<a name="gfYpK"></a>
### `keepAliveTimeMinutes`
设置活动保持时间，以分钟为单位，默认1分钟
<a name="qBpuq"></a>
### `allowMaximumSizeToDivergeFromCoreSize`
配置`maximumSize`生效。这个值可以等于或高于`coreSize`，默认`false`
