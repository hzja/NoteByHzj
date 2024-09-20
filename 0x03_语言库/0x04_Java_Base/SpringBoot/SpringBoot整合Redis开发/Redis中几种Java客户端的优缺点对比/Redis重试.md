受网络和运行环境影响，应用程序可能遇到暂时性故障，如瞬时网络抖动、服务暂时不可用、服务繁忙导致超时等。<br />自动重试机制可大幅避免此类故障，保障操作成功执行。
<a name="B9sPV"></a>
## 1、引发暂时性故障的原因
<a name="eAeon"></a>
### 1.1 故障触发了高可用机制
云 Redis 支持节点健康状态监测，当监测到实例中的主节点不可用时，会自动触发主备切换，例如将主节点和从节点进行互换，保障实例的高可用性。此时，客户端可能会遇到下列暂时性故障：秒级的连接闪断。30 秒内的只读状态（用于避免主备切换引起潜在的数据丢失风险和双写）。
<a name="nUj5n"></a>
### 1.2 慢查询引起了请求堵塞
执行时间复杂度为 O (N) 的操作，引发慢查询和请求的堵塞，此时，客户端发起的其他请求可能出现暂时性失败。
<a name="FcmKP"></a>
### 1.3 复杂的网络环境
由于客户端与 Redis 服务器之间复杂网络环境引起，可能出现偶发的网络抖动、数据重传等问题，此时，客户端发起的请求可能会出现暂时性失败。
<a name="oolNF"></a>
## 2、推荐的重试准则
<a name="yrO1A"></a>
### 2.1 仅重试幂等的操作
由于超时可能发生在下述任一阶段：该命令由客户端发送成功，但尚未到达 Redis。命令到达 Redis，但执行超时。命令在 Redis 中执行结束，但结果返回给客户端时发生超时。如果执行重试可能导致某个操作在 Redis 中被重复执行，因此不是所有操作均适合设计重试机制。通常推荐仅重试幂等的操作，例如 **SET** 操作，即多次执行 **SET a b** 命令，那么 a 的值只可能是 b 或执行失败；如果执行 **LPUSH mylist a** 则不是幂等的，可能导致 mylist 中包含多个 a 元素。
<a name="mh11d"></a>
### 2.2 适当的重试次数与间隔
根据业务需求和实际场景调整适当的重试次数与间隔，否则可能引发下述问题：如果重试次数不足或间隔太长，应用程序可能无法完成操作而导致失败。如果重试次数过大或间隔过短，应用程序可能会占用过多的系统资源，且可能因请求过多而堵塞在服务器上无法恢复。常见的重试间隔方式包括立即重试、固定时间重试、指数增加时间重试、随机时间重试等。
<a name="cp26z"></a>
### 2.3 避免重试嵌套
避免重试嵌套，否则可能会导致重复的重试且无法停止。
<a name="NLcN9"></a>
### 2.4 记录重试异常并打印失败报告
在重试过程中，建议在 WARN 级别上打印重试错误日志，同时，仅在重试失败时打印异常信息。
<a name="uCZMI"></a>
## 3、Jedis
建议使用 Jedis 4.0.0 及以上版本，推荐使用最新的 Jedis 版本，以下代码为 Jedis 5.0.0 的重试示例。
<a name="vuSqF"></a>
### 3.1 添加 Jedis 的 Pom 依赖
```xml
<dependency>
  <groupId>redis.clients</groupId>
  <artifactId>jedis</artifactId>
  <version>5.0.0</version>
</dependency>
```
<a name="spIKq"></a>
### 3.2 重试实战
<a name="vzh4D"></a>
#### ① 标准架构实例或集群架构代理（Proxy）模式
使用 JedisPool 模式。<br />该示例会将 SET 命令自动重试 5 次，且总重试时间不超过 10s，每次重试之间等待类指数间隔的时间，如果最终不成功，则抛出异常。
```java
PooledConnectionProvider provider = new PooledConnectionProvider(HostAndPort.from("127.0.0.1:6379"));
int maxAttempts = 5; // 最大重试次数
Duration maxTotalRetriesDuration = Duration.ofSeconds(10); // 最大的重试时间
UnifiedJedis jedis = new UnifiedJedis(provider, maxAttempts, maxTotalRetriesDuration);
try {
    System.out.println("set key: " + jedis.set("key", "value"));
} catch (Exception e) {
    // 表示尝试maxAttempts次或到达了最大查询时间maxTotalRetriesDuration仍旧没有访问成功。
    e.printStackTrace();
}
```
<a name="xB9AM"></a>
#### ② 集群架构直连模式
使用 JedisCluster 模式。<br />可以通过配置 maxAttempts 参数来定义失败情况下的重试次数，默认值为 5，如果最终不成功，则抛出异常。
```java
HostAndPort hostAndPort = HostAndPort.from("127.0.0.1:30001");
int connectionTimeout = 5000;
int soTimeout = 2000;
int maxAttempts = 5;
ConnectionPoolConfig config = new ConnectionPoolConfig();
JedisCluster jedisCluster = new JedisCluster(hostAndPort, connectionTimeout, soTimeout, maxAttempts, config);
try {
    System.out.println("set key: " + jedisCluster.set("key", "value"));
} catch (Exception e) {
    // 表示尝试maxAttempts之后仍旧没有访问成功。
    e.printStackTrace();
}
```
<a name="hF2Pf"></a>
## 4、Redisson
Redisson 客户端提供了两个参数来控制重试逻辑：

- **retryAttempts**：重试次数，默认为 3。
- **retryInterval**：重试间隔，默认为 1,500 毫秒。

重试示例如下：
```java
Config config = new Config();
config.useSingleServer()
    .setTimeout(1000)
    .setRetryAttempts(3)
    .setRetryInterval(1500) //ms
    .setAddress("redis://127.0.0.1:6379");
RedissonClient connect = Redisson.create(config);
```
<a name="n7wnH"></a>
## 5、S`tackExchange.Redis`
`StackExchang.Redis` 客户端目前仅支持重试时连接，重试示例如下：
```java
var conn = ConnectionMultiplexer.Connect("redis0:6380,redis1:6380,connectRetry=3");
```
**说明**<br />如需实现 API 级别的重试策略，请参见 Polly。
<a name="BaFhh"></a>
## 6、Lettuce
Lettuce 客户端未提供在命令超时后重试的参数，但是可以通过下述参数来实现命令重试策略：

- at-most-once execution：命令最多执行 1 次，即 0 次或 1 次，如果连接断开并重新连接，命令可能会丢失。
- at-least-once execution（默认）：最少成功执行 1 次，即可能会在执行时进行多次尝试，保障最少成功执行 1 次。使用此策略时，如果 Tair 实例发生了主备切换，此时客户端可能累积了较多的重试命令，主备切换完成后可能会引发 Tair 实例的 CPU 使用率激增。

**说明**<br />更多信息，请参见 Client-Options（[https://github.com/lettuce-io/lettuce-core/wiki/Client-Options](https://github.com/lettuce-io/lettuce-core/wiki/Client-Options)） 和 Command execution reliability（[https://github.com/lettuce-io/lettuce-core/wiki/Command-execution-reliability](https://github.com/lettuce-io/lettuce-core/wiki/Command-execution-reliability)）。<br />重试示例：
```java
clientOptions.isAutoReconnect() ? Reliability.AT_LEAST_ONCE : Reliability.AT_MOST_ONCE;
```
