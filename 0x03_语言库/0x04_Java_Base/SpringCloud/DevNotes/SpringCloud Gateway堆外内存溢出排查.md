Java SpringCloudGateWay
<a name="UzRn9"></a>
## 问题描述
<a name="llmas"></a>
### 报错详情
网关模块偶现 OutOfDirectMemoryError 错误，两次问题出现相隔大概 3 个月。两次发生的时机都是正在大批量接收数据 (大约 500w)，TPS 60 左右，网关服务波动不大，完全能扛住，按理不应该出现此错误。<br />详细报错信息如下：
```java
2021-05-06 13:44:18|WARN |[reactor-http-epoll-5]|[AbstractChannelHandlerContext.java : 311]|An exception 'io.netty.util.internal.OutOfDirectMemoryError: failed to allocate 16384 byte(s) of direct memory (used: 8568993562, max: 8589934592)' [enable DEBUG level for full stacktrace] was thrown by a user handler's exceptionCaught() method while handling the following exception:
io.netty.util.internal.OutOfDirectMemoryError: failed to allocate 16384 byte(s) of direct memory (used: 8568993562, max: 8589934592)
        at io.netty.util.internal.PlatformDependent.incrementMemoryCounter(PlatformDependent.java:754)
        at io.netty.util.internal.PlatformDependent.allocateDirectNoCleaner(PlatformDependent.java:709)
        at io.netty.buffer.UnpooledUnsafeNoCleanerDirectByteBuf.allocateDirect(UnpooledUnsafeNoCleanerDirectByteBuf.java:30)
        at io.netty.buffer.UnpooledDirectByteBuf.<init>(UnpooledDirectByteBuf.java:64)
        at io.netty.buffer.UnpooledUnsafeDirectByteBuf.<init>(UnpooledUnsafeDirectByteBuf.java:41)
        at io.netty.buffer.UnpooledUnsafeNoCleanerDirectByteBuf.<init>(UnpooledUnsafeNoCleanerDirectByteBuf.java:25)
        at io.netty.buffer.UnsafeByteBufUtil.newUnsafeDirectByteBuf(UnsafeByteBufUtil.java:625)
        at io.netty.buffer.PooledByteBufAllocator.newDirectBuffer(PooledByteBufAllocator.java:359)
        at io.netty.buffer.AbstractByteBufAllocator.directBuffer(AbstractByteBufAllocator.java:187)
        at io.netty.buffer.AbstractByteBufAllocator.directBuffer(AbstractByteBufAllocator.java:178)
        at io.netty.channel.unix.PreferredDirectByteBufAllocator.ioBuffer(PreferredDirectByteBufAllocator.java:53)
        at io.netty.channel.DefaultMaxMessagesRecvByteBufAllocator$MaxMessageHandle.allocate(DefaultMaxMessagesRecvByteBufAllocator.java:114)
        at io.netty.channel.epoll.EpollRecvByteAllocatorHandle.allocate(EpollRecvByteAllocatorHandle.java:75)
        at io.netty.channel.epoll.AbstractEpollStreamChannel$EpollStreamUnsafe.epollInReady(AbstractEpollStreamChannel.java:777)
        at io.netty.channel.epoll.EpollEventLoop.processReady(EpollEventLoop.java:475)
        at io.netty.channel.epoll.EpollEventLoop.run(EpollEventLoop.java:378)
        at io.netty.util.concurrent.SingleThreadEventExecutor$4.run(SingleThreadEventExecutor.java:989)
        at io.netty.util.internal.ThreadExecutorMap$2.run(ThreadExecutorMap.java:74)
        at io.netty.util.concurrent.FastThreadLocalRunnable.run(FastThreadLocalRunnable.java:30)
        at java.lang.Thread.run(Thread.java:748)
```
<a name="YtR4T"></a>
### JVM 配置
```bash
-server -Xmx8g -Xms8g -Xmn1024m 
-XX:PermSize=512m -Xss256k 
-XX:+DisableExplicitGC -XX:+UseConcMarkSweepGC -XX:+CMSParallelRemarkEnabled 
-XX:+UseCMSCompactAtFullCollection -XX:LargePageSizeInBytes=128m 
-XX:+UseFastAccessorMethods -XX:+UseCMSInitiatingOccupancyOnly 
-XX:CMSInitiatingOccupancyFraction=70 -Djava.awt.headless=true 
-Djava.net.preferIPv4Stack=true
```
<a name="je1Ji"></a>
### 版本信息
Spring cloud : Hoxton.SR5<br />Spring cloud starter gateway : 2.2.3.RELEASE<br />Spring boot starter : 2.3.0.RELEASE<br />Netty : 4.1.54.Final<br />Reactor-netty: 0.9.7.RELEASE
<a name="o9tSb"></a>
## 山重水复疑无路
JVM 参数详解：[https://docs.oracle.com/javase/8/docs/technotes/tools/unix/java.html](https://docs.oracle.com/javase/8/docs/technotes/tools/unix/java.html)<br />报错的信息是 OutOfDirectMemoryError，即堆外内存不足。

1. 堆外内存是在 NIO 中使用的；
2. 堆外内存通过 `-XX:MaxDirectMemorySize` 参数控制大小，注意和 `-XX:+DisableExplicitGC` 参数的搭配使用；
3. JDK8 中堆外内存默认和堆内存一样大（-Xmx）；
4. JDK8 如果配置 `-XX:MaxDirectMemorySize` 参数，则堆外内存大小以设置的参数为准；

SpringCloudGateway 是基于 WebFlux 框架实现的，而 WebFlux 框架底层则使用了高性能的 Reactor 模式通信框架 Netty。<br />网上查阅相关资料，有些场景是因为堆外内存没有手动 release 导致，于是简单查看了网关模块的相关代码发现并无此问题，关键的地方也都调用了相关方法释放内存。堆外内存通过操作堆的命令无法看到，只能监控实例总内存走势判断。
```java
// 释放内存方法
DataBufferUtils.release(dataBuffer);
```
Dump 堆内存下来也没有发现有什么问题：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648259855912-c351843e-ebc9-451a-88ca-4bb06063e439.png#clientId=u84ec3684-3631-4&from=paste&id=ub10e16d6&originHeight=563&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9deb644c-18aa-4b69-a898-50d603ac708&title=)
<a name="RGwYn"></a>
## 柳暗花明又一村
抱着试一试的想法到 SpringCloudGateway 官方仓库 issue 搜索有没有人遇到相同的问题，果不其然，有人提了类似的 issue。[https://github.com/spring-cloud/spring-cloud-gateway/issues/1704](https://github.com/spring-cloud/spring-cloud-gateway/issues/1704)<br />在 issue 中开发人员也给出了回应，**确实是 SpringCloudGateway 的 BUG！此问题已在 2.2.6.RELEASE 版本中修复。而项目中使用版本为 2.2.3.RELEASE，所以就会出现这个问题。**<br />原因是：**包装原生的 pool 后没有释放内存。**<br />[https://github.com/spring-cloud/spring-cloud-gateway/milestone/42?closed=1](https://github.com/spring-cloud/spring-cloud-gateway/milestone/42?closed=1)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648261437607-d7f8628e-95ac-4af9-9a41-f771bb8ea5e0.png#clientId=u84ec3684-3631-4&from=paste&height=713&id=u438f909c&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=352524&status=done&style=none&taskId=u487a2239-80cb-44d4-96ef-fd8eca9834a&title=&width=1536)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648261562148-43df9085-dded-4fa0-b78a-e0f6452c5c75.png#clientId=u84ec3684-3631-4&from=paste&height=713&id=uef1c343e&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=415894&status=done&style=none&taskId=u63d4b5f3-bd30-4b8c-a49f-e115de22c9c&title=&width=1536)
<a name="AMso2"></a>
## 出乎意料
问题原因已经找到，想着在测试环境复现后升级版本再验证即可。可结果却出乎意料。

1. 测试环境将堆内存调小尝试进行复现生产问题，在压测将近 1 个小时后出现了同样的问题，复现成功。
2. 升级 SpringCloudGateway 的版本至 2.2.6.RELEASE。
3. **重新压测，问题再次出现。**

没看错，问题再次出现，且报错信息一模一样。很快又陷入了沉思。
<a name="o5R6X"></a>
## 深究原因
排除了组件的问题，剩下的就是代码的问题了，最有可能的就是程序中没有显式调用释放内存导致。<br />网关模块共定义了三个过滤器，一个全局过滤器 `RequestGatewayFilter implements GlobalFilter`。两个自定义过滤器 `RequestDecryptGatewayFilterFactory extends AbstractGatewayFilterFactory` 和 `ResponseEncryptGatewayFilterFactory extends AbstractGatewayFilterFactory`。<br />依次仔细排查相关逻辑，在全局过滤器 `RequestGatewayFilter` 中有一块代码引起了注意：
```java
// 伪代码
@Override
public Mono<Void> filter(ServerWebExchange exchange, GatewayFilterChain chain) {
	HttpHeaders headers = request.getHeaders();
	return DataBufferUtils.join(exchange.getRequest().getBody())
		.flatMap(dataBuffer -> {
			DataBufferUtils.retain(dataBuffer);
			Flux<DataBuffer> cachedFlux = Flux.defer(() -> Flux.just(dataBuffer.slice(0, dataBuffer.readableByteCount())));
			
			ServerHttpRequest mutatedRequest = new ServerHttpRequestDecorator(exchange.getRequest()) {
				@Override
				public Flux<DataBuffer> getBody() {
					return cachedFlux;
				}
				
				@Override
				public HttpHeaders getHeaders() {
					return headers;
				}
			};
			return chain.filter(exchange.mutate().request(mutatedRequest).build());
		});
}
```
Request 的 Body 是只能读取一次的，如果直接通过在 Filter 中读取，而不封装回去回导致后面的服务无法读取数据。<br />此全局过滤器的目的就是把原有的 request 请求中的 body 内容读出来，并且使用`ServerHttpRequestDecorator` 这个请求装饰器对 request 进行包装，重写 `getBody` 方法，并把包装后的请求放到过滤器链中传递下去。这样后面的过滤器中再使用 `exchange.getRequest().getBody()` 来获取 body 时，实际上就是调用的重载后的 `getBody()` 方法，获取的最先已经缓存了的 body 数据。这样就能够实现 body 的多次读取了。<br />但是将 DataBuffer 读取出来后并没有手动释内存，会导致堆外内存持续增长。于是添加了一行代码手动释放堆外内存：
```java
DataBufferUtils.release(dataBuffer);
```
```java
// 伪代码
@Override
public Mono<Void> filter(ServerWebExchange exchange, GatewayFilterChain chain) {
	HttpHeaders headers = request.getHeaders();
	return DataBufferUtils.join(exchange.getRequest().getBody())
		.flatMap(dataBuffer -> {
			byte[] bytes = new byte[dataBuffer.readableByteCount()];
			dataBuffer.read(bytes);
			// 释放堆外内存
			DataBufferUtils.release(dataBuffer);
			ServerHttpRequest mutatedRequest = new ServerHttpRequestDecorator(exchange.getRequest()) {
				@Override
				public Flux<DataBuffer> getBody() {
					return Flux.defer(() -> {
						DataBuffer buffer = exchange.getResponse().bufferFactory().wrap(bytes);
						DataBufferUtils.retain(buffer);
						return Mono.just(buffer);
					});
				}
				
				@Override
				public HttpHeaders getHeaders() {
					return headers;
				}
			};
			return chain.filter(exchange.mutate().request(mutatedRequest).build());
		});
}
```
再次压测未出现堆外内存溢出问题。在网络上查询到了类似的案例：<br />[https://github.com/reactor/reactor-netty/issues/788](https://github.com/reactor/reactor-netty/issues/788)
