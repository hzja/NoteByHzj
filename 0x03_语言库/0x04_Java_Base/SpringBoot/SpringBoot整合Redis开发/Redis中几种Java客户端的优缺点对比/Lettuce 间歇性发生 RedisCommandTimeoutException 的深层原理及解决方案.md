Java
<a name="UHazK"></a>
## 0x00、起源
项目的一些微服务集成了 Spring Data Redis，而底层的 Redis 客户端是 lettuce，这也是默认的客户端。微服务在某些环境中运行很正常，但在另一些环境中运行就会间歇性的发生 `RedisCommandTimeoutException`：有时长时间没人使用（当然也不操作 Redis 了），例如一个晚上没人操作系统，第二天早上使用时就会发生这个异常。而且发生该异常之后，访问 Redis 就会一直抛这个异常，但过了一段时间后，又正常了。或者立即重启微服务，也会正常了。

- lettuce 版本：5.3.0
- Redis 版本：官方 docker 镜像， 5.0，默认配置
- Spring boot 版本：2.1.x

经过日志排查（lettuce 的日志级别需要开启 DEBUG 或 TRACE），发生`RedisCommandTimeoutException` 的原因时lettuce的 Connection 已经断了，发生异常后大约 15 分钟，lettuce 的 `ConnectionWatchdog`会进行自动重连。<br />那么为何 lettuce 的 Connection 为什么会断呢？而 `ConnectionWatchdog`为什么没有立即重连呢？又怎么解决这些问题呢？这些问题如果不弄清楚不解决，会严重影响系统的可用性，总不能让用户等十几分钟再用吧，也不能总重启应用吧。<br />网上也搜到了类似的问题，看来还是挺多人遇到相同的问题的。但大部分都没说清楚这个现象的原因，也没说真正的解决方法。网上几乎全部的解决方法都是将lettuce换成了jedis，回避了这个问题。
<a name="kAv4r"></a>
## 0x01、本质
换成jedis固然可以解决问题，但既然 lettuce能成为Spring默认的客户端，还是有先进的地方的。而且遇到问题不搞清楚，心里也痒痒的。下面会阐述这些问题的来龙去脉。
<a name="H1YUM"></a>
### 为什么 Redis 连接会断
其实这个问题并不是很重要，因为Socket连接断已经是事实，而且在分布式环境中，网络分区是必然的。在网络环境，Redis 服务器主动断掉连接是很正常的，lettuce 的作者也提及 lettuce 一天发生一两次重连是很正常的。<br />那么哪些情况会导致连接断呢：

- Linux 内核的 keepalive 功能可能会一直收不到客户端的回应；
- 收到与该连接相关的 ICMP 错误信息；
- 其他网络链路问题等等；

如果要需要真正查明原因，需要 tcp dump 进行抓包，但意义不大，除非断线的概率大，像一天一两次或者几天才一次不必花这么大力气去查这个。而最主要的问题是 lettuce 客户端能否及时检测到连接已断，并尽快重连。
<a name="oETxg"></a>
### 为何 lettuce 没有立刻重连
lettuce 重连机制的源码就不扯了，感谢兴趣的同学可以自行阅读 lettuce中`ConnectionWatchdog` 的源码。<br />根据ConnectionWatchdog重连的机制（收到netty的`ChannelInactived`事件后启动重连的线程不断进行连接）可以确定，连接是由 Redis 服务端断开的，因为如果是客户端主动断开连接，那么一定能收到`ChannelInactived`，因此，之所以lettuce要等 15 分钟后才重连，是因为没收到`ChanelInactived`事件。<br />那么为什么客户端没有到`ChannelInactived`事件呢？很多情况都会，例如：

- 客户端没收到服务端 FIN 包；
- 网络链路断了，例如拔网线，断电等等；

在这个情况，应该是没收到服务端的 FIN 包。<br />再来看另一个问题：日志显示发生`RedisCommandTimeoutException`后，15 分钟后收到`ChannelInactived`事件。那么，为什么会大约是 15 分钟而不是别的时间呢？<br />其实，这是与 Linux 底层 Socket的实现有关--这就是超时重传机制。也就是/proc/sys/net/ipv4/tcp_retries2 参数，关于这个重传机制，也不是本文的重点，大家自行多查一些资料。<br />根据重传机制，发生`RedisCommandTimeoutException`的命令会重传 `tcp_retries2`这么多次，刚刚好是 15 分钟左右。<br />小结：<br />问题的原因已经清楚了，这里需要对 lettuce的重连机制、netty的工作原理、Linux socket实现原理有一定的了解。既然问题的原因找到了，如何解决呢？显然无论是网上说的替换Jedis客户端，还是重启应用、还是等 15 分钟，都不是好办法。
<a name="c4iYI"></a>
## 0x02、解决方案
既然找到了问题原因所在，那么可以根据这些原因来解决。主要有三种解决的方案：
<a name="GpsfK"></a>
### 设置 Linux 的 TCP_RETRIES2 参数
针对等待 15 分钟，那么就可以猜想是不是可以设置 Linux 的 `TCP_RETRIES2` 参数小点来缩短等待时间呢？答案是肯定的；这个参数 Linux 的默认值是 15，而有些应用（如 Oracle）要求设置为 3。<br />其实，一般情况下，tcp数据包超时了，重发 3 次都不成功，重发再多几次也是枉然的。<br />但是这个方案有个缺点：<br />如果修改了这个参数，也会影响到其他应用，因为这个是全局的参数。那么能否单独针对某个应用程序设置 Socket Option呢？很遗憾的是，笔者在 netty里并没找到该选项的设置，无论是`EpollChannelOption` 还是 JDK 的`ExtendedSocketOptions`。<br />所幸的是：<br />netty提供另一个参数的设置：`TCP_USER_TIMEOUT`，这个参数就是为了针对单独设置某个应用程序的超时重传的设置。下面一小节讲述如何使用。
<a name="BTY1K"></a>
### 设置 Socket Option 的 TCP_USER_TIMEOUT 参数
在Spring Boot的auto-configuration中，`ClientResources`的初始化是默认的 `ClientResources`，因此，可以自定义一个 `ClientResources`。
```java
@Bean
public ClientResources clientResources(){
    return ClientResources clientResources = ClientResources.builder()
    .nettyCustomizer(new NettyCustomizer() {

        @Override
        public void afterBootstrapInitialized(Bootstrap bootstrap) {
            bootstrap.option(EpollChannelOption.TCP_USER_TIMEOUT, 10);
        }
    })
    .build();
}
```
<a name="nGE6l"></a>
### 定制 lettuce：增加心跳机制
上面两个方案，缩短了等待的时长，都是依赖操作系统底层的通知。如果不想依赖底层操作系统的通知，唯一的办法就是自己在应用层增加心跳机制。<br />如上述的方案，lettuce提供了NettyCustomizer进行扩展，熟悉netty的同学，应该听说过netty所提供的心跳机制--IdleStateHandler，结合这两者，就很容易在初始化netty时增加心跳机制：
```java
@Bean
public ClientResources clientResources(){

    NettyCustomizer nettyCustomizer = new NettyCustomizer() {

        @Override
        public void afterChannelInitialized(Channel channel) {
            channel.pipeline().addLast(
                new IdleStateHandler(readerIdleTimeSeconds, writerIdleTimeSeconds, allIdleTimeSeconds));
            channel.pipeline().addLast(new ChannelDuplexHandler() {
                @Override
                public void userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception {
                    if (evt instanceof IdleStateEvent) {
                        ctx.disconnect();
                    }
                }
            });
        }

        @Override
        public void afterBootstrapInitialized(Bootstrap bootstrap) {
        }

    };

    return ClientResources.builder().nettyCustomizer(nettyCustomizer ).build();
}
```
这里由客户端自己做心跳检测，一旦发现Channel死了，主动关闭`ctx.close()`，那么`ChannelInactived`事件一定会被触发了。但是这个方案有个缺点，增加了客户端的压力。
<a name="tUElR"></a>
## 0x03、总结
lettuce是一个优秀的开源软件，设计和代码都很优美。通过这次的问题排查和解决问题，加深了自己对netty，Linux Socket机制、TCP/IP 协议的理解。
