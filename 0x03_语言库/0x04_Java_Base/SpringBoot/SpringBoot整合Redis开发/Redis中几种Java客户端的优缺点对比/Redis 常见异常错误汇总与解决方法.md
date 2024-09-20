<a name="gTHwf"></a>
## 错误概览
- redis.clients.jedis.exceptions.JedisConnectionException: Could not get a resource from the pool
- redis.clients.jedis.exceptions.JedisConnectionException: Unexpected end of stream
- redis.clients.jedis.exceptions.JedisDataException: ERR illegal address
- redis.clients.jedis.exceptions.JedisDataException: ERR max number of clients reached
- redis.clients.jedis.exceptions.JedisConnectionException: java.net.SocketTimeoutException: Read timed out
- redis.clients.jedis.exceptions.JedisDataException: NOAUTH Authentication required
- redis.clients.jedis.exceptions.JedisDataException: EXECABORT Transaction discarded because of previous errors
- java.lang.ClassCastException: java.lang.Long cannot be cast to java.util.List
- redis.clients.jedis.exceptions.JedisDataException: WRONGTYPE Operation against a key holding the wrong kind of value
- redis.clients.jedis.exceptions.JedisDataException: OOM command not allowed when used memory > ‘maxmemory’
- redis.clients.jedis.exceptions.JedisDataException: LOADING Redis is loading the dataset in memory
- redis.clients.jedis.exceptions.JedisDataException: BUSY Redis is busy running a script. You can only call SCRIPT KILL or SHUTDOWN NOSAVE
- redis.clients.jedis.exceptions.JedisConnectionException: java.net.SocketTimeoutException: connect timed out
- UNKILLABLE Sorry the script already executed write commands against the dataset. You can either wait the script termination or kill the server in a hard way using the SHUTDOWN NOSAVE command
- java.lang.NoClassDefFoundError
- redis.clients.jedis.exceptions.JedisDataException: ERR unknown command
- redis.clients.jedis.exceptions.JedisDataException: Please close pipeline or multi block before calling this method
- redis.clients.jedis.exceptions.JedisDataException: ERR command role not support for normal user
- 其他问题
- 连接池参数
<a name="ejvYb"></a>
## 无法从连接池获取到Jedis连接
<a name="O8LGN"></a>
### 异常堆栈

- 当`blockWhenExhausted`连接池参数等于true（默认值）时，如果连接池没有可用的Jedis连接，则会等待一段时间，等待的时间由`maxWaitMillis`参数决定，单位为毫秒，如果依然没有获取到可用的Jedis连接，才会出现下列异常。
```
redis.clients.jedis.exceptions.JedisConnectionException: Could not get a resource from the pool
    …
Caused by: java.util.NoSuchElementException: Timeout waiting for idle object
    at org.apache.commons.pool2.impl.GenericObjectPool.borrowObject(GenericObjectPool.java:449)
```

- 当`blockWhenExhausted`连接池参数等于false时，如果连接池没有可用的Jedis连接，则会立即出现下列异常。
```
redis.clients.jedis.exceptions.JedisConnectionException: Could not get a resource from the pool
    …
Caused by: java.util.NoSuchElementException: Pool exhausted
    at org.apache.commons.pool2.impl.GenericObjectPool.borrowObject(GenericObjectPool.java:464)
```
<a name="eRO46"></a>
## 异常描述
上述异常是客户端没有从连接池获得可用的Jedis连接造成，Jedis资源最大数量由maxTotal值决定，可能有下列几种原因。
<a name="FRhg4"></a>
## 连接泄露
JedisPool默认的`maxTotal`值为8，从下列代码得知，从JedisPool中获取了8个Jedis资源，但是没有归还资源。因此，当第9次尝试获取Jedis资源的时候，则无法调用`jedisPool.getResource().ping()`。
```java
GenericObjectPoolConfig poolConfig = new GenericObjectPoolConfig();
JedisPool jedisPool = new JedisPool(poolConfig, "127.0.0.1", 6379);
//向JedisPool借用8次连接，但是没有执行归还操作。
for (int i = 0; i < 8; i++) {
    Jedis jedis = null;
    try {
        jedis = jedisPool.getResource();
        jedis.ping();
    } catch (Exception e) {
        logger.error(e.getMessage(), e);
    }
}
jedisPool.getResource().ping();
```
推荐使用下列规范代码。
```java
Jedis jedis = null;
try {
    jedis = jedisPool.getResource();
    //具体的命令
    jedis.executeCommand()
} catch (Exception e) {
    //如果命令有key最好把key也在错误日志打印出来，对于集群版来说通过key可以帮助定位到具体节点。
    logger.error(e.getMessage(), e);
} finally {
    //注意这里不是关闭连接，在JedisPool模式下，Jedis会被归还给资源池。
    if (jedis != null) 
        jedis.close();
}
```
<a name="chNVZ"></a>
### 业务并发量大（`maxTotal`值设置得过小）
业务并发量大导致出现异常的示例：一次命令运行时间（borrow|return resource + Jedis执行命令 + 网络时间）的平均耗时约为1ms，一个连接的QPS大约是1000。业务期望的QPS是50000。那么理论上需要的资源池大小为50000除以1000等于50个。因此用户需要根据实际情况对maxTotal值进行微调。
<a name="JtzEl"></a>
### Jedis连接阻塞
例如Redis发生了阻塞（例如慢查询等原因），所有连接会在超时时间范围内等待，当并发量较大时，会造成连接池资源不足。
<a name="eUeTj"></a>
### Jedis连接被拒绝
从连接池中获取连接时，由于没有空闲连接，需要重新生成一个Jedis连接，但是连接被拒绝。
```
redis.clients.jedis.exceptions.JedisConnectionException: Could not get a resource from the pool
    at redis.clients.util.Pool.getResource(Pool.java:50)
    at redis.clients.jedis.JedisPool.getResource(JedisPool.java:99)
    at TestAdmin.main(TestAdmin.java:14)
Caused by: redis.clients.jedis.exceptions.JedisConnectionException: java.net.ConnectException: Connection refused
    at redis.clients.jedis.Connection.connect(Connection.java:164)
    at redis.clients.jedis.BinaryClient.connect(BinaryClient.java:80)
    at redis.clients.jedis.BinaryJedis.connect(BinaryJedis.java:1676)
    at redis.clients.jedis.JedisFactory.makeObject(JedisFactory.java:87)
    at org.apache.commons.pool2.impl.GenericObjectPool.create(GenericObjectPool.java:861)
    at org.apache.commons.pool2.impl.GenericObjectPool.borrowObject(GenericObjectPool.java:435)
    at org.apache.commons.pool2.impl.GenericObjectPool.borrowObject(GenericObjectPool.java:363)
    at redis.clients.util.Pool.getResource(Pool.java:48)
    ... 2 more
Caused by: java.net.ConnectException: Connection refused
    at java.net.PlainSocketImpl.socketConnect(Native Method)
    at java.net.AbstractPlainSocketImpl.doConnect(AbstractPlainSocketImpl.java:339)
    at java.net.AbstractPlainSocketImpl.connectToAddress(AbstractPlainSocketImpl.java:200)
    at java.net.AbstractPlainSocketImpl.connect(AbstractPlainSocketImpl.java:182)
    at java.net.SocksSocketImpl.connect(SocksSocketImpl.java:392)
    at java.net.Socket.connect(Socket.java:579)
    at redis.clients.jedis.Connection.connect(Connection.java:158)
    ... 9 more
```
可以从at redis.clients.jedis.Connection.connect(Connection.java:158)中看到实际是一个Socket连接。
```
socket.setSoLinger(true, 0); // Control calls close () method,
        // the underlying socket is closed
        // immediately
        // <-@wjw_add
158:  socket.connect(new InetSocketAddress(host, port), connectionTimeout);
```
**「提示」**：一般这类报错需要检查Redis的域名配置是否正确，排查该段时间网络是否正常。
<a name="qQgtw"></a>
### 其他问题
丢包、DNS、客户端TCP参数配置等，可以提交工单获取帮助。
<a name="fmv1u"></a>
### 解决方法
从上述分析，可以看出这个问题的原因比较复杂，不能简单地认为连接池不够就盲目加大`maxTotal`值，需要具体问题具体分析。
<a name="sVSP4"></a>
## 客户端缓冲区异常
<a name="a49I7"></a>
### 异常堆栈
异常堆栈如下。
```
redis.clients.jedis.exceptions.JedisConnectionException: Unexpected end of stream. at redis.clients.util.RedisInputStream.ensureFill(RedisInputStream.java:199) at redis.clients.util.RedisInputStream.readByte(RedisInputStream.java:40) at redis.clients.jedis.Protocol.process(Protocol.java:151) ......
```
<a name="ZB9ba"></a>
### 异常描述
这个异常描述是客户端缓冲区异常，产生这个问题可能有下列三个原因。
<a name="NXJLR"></a>
### 多个线程使用一个Jedis连接
正常的情况是一个线程使用一个Jedis连接，可以使用JedisPool管理Jedis连接，实现线程安全，避免出现这种情况。例如，下面代码就是两个线程共用了一个Jedis连接。
```java
new Thread(new Runnable() {
    public void run() {
        for (int i = 0; i < 100; i++) {
            jedis.get("hello");
        }
    }
}).start();
new Thread(new Runnable() {
    public void run() {
        for (int i = 0; i < 100; i++) {
            jedis.hget("haskey", "f");
        }
    }
}).start();
```
<a name="tNtxO"></a>
### 客户端缓冲区满了
Redis有下列三种客户端缓冲区。

- 普通客户端缓冲区(normal)：用于接受普通的命令，例如get、set、mset、hgetall、zrange等。
- slave客户端缓冲区(slave)：用于同步master节点的写命令，完成复制。
- 发布订阅缓冲区(pubsub)：pubsub不是普通的命令，因此有单独的缓冲区。

Redis客户端缓冲区配置的格式如下。
```
client-output-buffer-limit [$Class] [$Hard_Limit] [$Soft_Limit] [$Soft_Seconds]
```

- [$Class]：客户端类型，可选值为normal、slave和pubsub。
- [$Hard_Limit]：如果客户端使用的输出缓冲区大于hard limit，客户端会被立即关闭，单位为秒。
- [SoftLimit]和[Soft_Limit]和[SoftLimit]和[Soft_Seconds]：如果客户端使用的输出缓冲区超过了soft limit，并且持续了soft limit秒，客户端会被立即关闭，单位为秒。

例如下面是一份Redis缓冲区的配置，所以当条件满足时，客户端连接会被关闭，就会出现Unexpected end of stream报错。
```
redis> config get client-output-buffer-limit
1) "client-output-buffer-limit"
2) "normal 524288000 0 0 slave 2147483648 536870912 480 pubsub 33554432 8388608 60"
```
<a name="NAVIF"></a>
### 长时间闲置连接
长时间闲置连接会被服务端主动断开，可以查询timeout配置的设置以及自身连接池配置确定是否需要做空闲检测。
<a name="JSzjD"></a>
### 解决方法和处理途径

- 排查自身代码是否使用JedisPool管理Jedis连接，是否存在并发操作Jedis的情况。
- 排查是否是上述客户端缓冲区满了或长时间闲置连接原因。云数据库Redis版默认的**「timeout」**值为0，目前不支持修改。**「client-output-buffer-limit」**默认值为500MB，为阿里云优化后的合理值。如果超过该值，说明用户返回的值过多，出于性能和稳定性考虑，建议优化应用程序。
<a name="PCWdp"></a>
## 非法的客户端地址
**「提示」**：阿里云Redis提供客户端白名单功能。
<a name="bJG06"></a>
### 异常堆栈
异常堆栈如下。
```
Caused by: redis.clients.jedis.exceptions.JedisDataException: ERR illegal address
    at redis.clients.jedis.Protocol.processError(Protocol.java:117)
    at redis.clients.jedis.Protocol.process(Protocol.java:151)
    at redis.clients.jedis.Protocol.read(Protocol.java:205)
    ......
```
向*添加一个成员时，例如使用SET TEST "Helloworld"命令，出现下列报错，也可能是白名单问题。
```
Error: Insert the diskette for drive %1.
```
<a name="flXvw"></a>
### 异常描述
Redis实例配置了白名单，但当前访问Redis的客户端（IP）不在白名单中。
<a name="kgeJw"></a>
### 解决方法
添加该客户端（IP）的白名单，关于如何添加白名单，请参考设置IP白名单
<a name="rgEmh"></a>
## 客户端连接数达到最大值
<a name="MAgKh"></a>
### 异常堆栈
异常堆栈如下。
```
redis.clients.jedis.exceptions.JedisDataException: ERR max number of clients reached
```
<a name="pJjd6"></a>
### 异常描述
客户端连接数超过了Redis实例配置的最大maxclients。
<a name="sRaSA"></a>
### 解决方法

- 提交工单联系阿里云技术支持临时调整最大连接数，协助定位问题。
- 定位自身问题，可以定位连接最多的客户端，找到问题原因，例如连接池配置等，然后进行处理。
<a name="SdqFK"></a>
## 客户端读写超时
<a name="qDZuw"></a>
### 异常堆栈
```
redis.clients.jedis.exceptions.JedisConnectionException: java.net.SocketTimeoutException: Read timed out
```
<a name="AW8UN"></a>
### 异常描述
问题原因可能有下列几种。

- 读写超时设置的过短。
- 有慢查询或者Redis发生阻塞。
- 网络不稳定。
<a name="qoCBe"></a>
### 解决方法
用户提供读写超时时间，提交工单定位相关原因并解决。
<a name="pVRVW"></a>
## 密码相关的异常
<a name="Gjkg8"></a>
### 异常堆栈

- Redis设置了密码鉴权，客户端请求没有提供密码。
```
Exception in thread "main" redis.clients.jedis.exceptions.JedisDataException: NOAUTH Authentication required.
     at redis.clients.jedis.Protocol.processError(Protocol.java:127)
     at redis.clients.jedis.Protocol.process(Protocol.java:161)
      at redis.clients.jedis.Protocol.read(Protocol.java:215)
```

- Redis没有设置密码鉴权，客户端请求中包含了密码。
```
Exception in thread "main" redis.clients.jedis.exceptions.JedisDataException: ERR Client sent AUTH, but no password is set
     at redis.clients.jedis.Protocol.processError(Protocol.java:127)
     at redis.clients.jedis.Protocol.process(Protocol.java:161)
      at redis.clients.jedis.Protocol.read(Protocol.java:215)
```

- 客户端传输了错误的密码。
```
redis.clients.jedis.exceptions.JedisDataException: ERR invalid password
    at redis.clients.jedis.Protocol.processError(Protocol.java:117)
    at redis.clients.jedis.Protocol.process(Protocol.java:151)
    at redis.clients.jedis.Protocol.read(Protocol.java:205)
```
<a name="eFrra"></a>
### 解决方法
确认有没有设置密码鉴权，是否提供了正确的密码。
<a name="NZMaK"></a>
## 事务异常
<a name="loczC"></a>
### 异常堆栈
异常堆栈如下。
```
redis.clients.jedis.exceptions.JedisDataException: EXECABORT Transaction discarded because of previous errors
```
<a name="NePNa"></a>
### 异常描述
这个是Redis的事务异常，事务中包含了错误的命令，例如，下列sett是个不存在的命令。
```
127.0.0.1:6379> multi
OK
127.0.0.1:6379> sett key world
(error) ERR unknown command 'sett'
127.0.0.1:6379> incr counter
QUEUED
127.0.0.1:6379> exec
(error) EXECABORT Transaction discarded because of previous errors.
```
<a name="Ofoq4"></a>
### 解决方法
查看自身代码逻辑，修复代码错误。
<a name="TImjo"></a>
## 类转换错误
<a name="E5Fc2"></a>
### 异常堆栈
异常堆栈如下。
```
java.lang.ClassCastException: java.lang.Long cannot be cast to java.util.List
         at redis.clients.jedis.Connection.getBinaryMultiBulkReply(Connection.java:199)
         at redis.clients.jedis.Jedis.hgetAll(Jedis.java:851)
         at redis.clients.jedis.ShardedJedis.hgetAll(ShardedJedis.java:198)
java.lang.ClassCastException: java.util.ArrayList cannot be cast to [B
         at redis.clients.jedis.Connection.getBinaryBulkReply(Connection.java:182)
         at redis.clients.jedis.Connection.getBulkReply(Connection.java:171)
         at redis.clients.jedis.Jedis.rpop(Jedis.java:1109)
         at redis.clients.jedis.ShardedJedis.rpop(ShardedJedis.java:258)
.......
```
<a name="hWfVm"></a>
### 异常描述
Jedis正确的使用方法是，一个线程操作一个Jedis，如果多个线程操作同一个Jedis连接就会发生此类错误。使用JedisPool可避免此类问题。例如下列代码在两个线程并发使用了一个Jedis（get、hgetAll返回不同的类型）。
```java
new Thread(new Runnable() {
    public void run() {
        for (int i = 0; i < 100; i++) {
            jedis.set("hello", "world");
            jedis.get("hello");
        }
    }
}).start();
new Thread(new Runnable() {
    public void run() {
        for (int i = 0; i < 100; i++) {
            jedis.hset("hashkey", "f", "v");
            jedis.hgetAll("hashkey");
        }
    }
}).start();
```
<a name="p21iZ"></a>
### 解决方法和处理途径
用户排查自身代码是否存在问题。
<a name="f1vIk"></a>
## 命令使用错误
<a name="DKJ67"></a>
### 异常堆栈
异常堆栈如下。
```
Exception in thread "main" redis.clients.jedis.exceptions.JedisDataException: WRONGTYPE Operation against a key holding the wrong kind of value
    at redis.clients.jedis.Protocol.processError(Protocol.java:127)
    at redis.clients.jedis.Protocol.process(Protocol.java:161)
    at redis.clients.jedis.Protocol.read(Protocol.java:215)
.....
```
<a name="sIReU"></a>
### 异常描述
例如key="hello"是字符串类型的键，而hgetAll返回哈希类型的键，所以出现了错误。
```java
jedis.set("hello","world");
jedis.hgetAll("hello");
```
<a name="FGPpp"></a>
### 解决方法和处理途径
请用户修改自身代码错误。
<a name="mLDPj"></a>
## Redis使用的内存超过maxmemory配置
<a name="IbiJn"></a>
### 异常堆栈
Redis节点（如果是集群，则是其中一个节点）使用内存大于该实例的内存规格（maxmemory配置）。异常堆栈如下。
```
redis.clients.jedis.exceptions.JedisDataException: OOM command not allowed when used memory > 'maxmemory'.
```
<a name="kjA8x"></a>
### 问题原因
原因可能有下列几种：

- 业务数据正常增加。
- 客户端缓冲区异常，例如monitor、pub/sub使用不当等等。
- 纯缓存使用场景，但是maxmemory-policy配置有误（例如没有设置过期键的业务配置volatile-lru）。
<a name="DSh5k"></a>
### 解决方法

1. 请查看Redis实例是否负载过高，如果是集群版，需要查看每个分片是否负载过高。如果存在大Key，分析获取大Key。
2. 确认内存增大的原因，根据自身业务场景解决问题。
3. 需要紧急处理时，可以临时调整maxmeory，后续咨询是否需要升配或者调整配置。
<a name="OOpmO"></a>
## Redis正在加载持久化文件
<a name="hZLrD"></a>
### 异常堆栈
异常堆栈如下。
```
redis.clients.jedis.exceptions.JedisDataException: LOADING Redis is loading the dataset in memory
```
<a name="RSYer"></a>
### 异常描述
Jedis调用Redis时，如果Redis正在加载持久化文件，无法进行正常的读写。
<a name="mWpgZ"></a>
### 解决方法
正常情况下，阿里云Redis不会出现这种情况，如果出现，则提交工单。
<a name="M986G"></a>
## Lua脚本超时
<a name="frUTz"></a>
### 异常堆栈
异常堆栈如下。
```
redis.clients.jedis.exceptions.JedisDataException: BUSY Redis is busy running a script. You can only call SCRIPT KILL or SHUTDOWN NOSAVE.
```
<a name="REqRF"></a>
### 异常描述
如果Redis当前正在执行Lua脚本，并且超过了lua-time-limit，此时Jedis调用Redis时，会收到上述异常。
<a name="flFk2"></a>
### 解决方法
按照异常提示You can only call SCRIPT KILL or SHUTDOWN NOSAVE.，使用kill命令终止Lua脚本。
<a name="OFi26"></a>
## 连接超时
<a name="TnUiC"></a>
### 异常堆栈
```
redis.clients.jedis.exceptions.JedisConnectionException: java.net.SocketTimeoutException: connect timed out
```
<a name="oJt5B"></a>
### 异常原因
可能原因有下列几种。

- 连接超时设置的过短。
- tcp-backlog满，造成新的连接失败。
- 客户端与服务端网络故障。
<a name="NQYQB"></a>
### 解决方法
用户提供连接超时时间，提交工单定位相关原因。
<a name="EVc53"></a>
## Lua脚本写超时
<a name="YaxUo"></a>
### 异常堆栈
异常堆栈如下。
```
(error) UNKILLABLE Sorry the script already executed write commands against the dataset. You can either wait the script termination or kill the server in a hard way using the SHUTDOWN NOSAVE command.
```
<a name="SgjWw"></a>
### 异常描述
如果Redis当前正在执行Lua脚本，并且超过了lua-time-limit，并且已经执行过写命令，此时Jedis调用Redis时，会收到上述异常。
<a name="w6jwO"></a>
### 解决方法
提交工单紧急处理，管理员需要重启或者切换Redis节点。
<a name="qyCwa"></a>
## 类加载错误
<a name="gPtkc"></a>
### 异常堆栈
找不到类和方法的异常堆栈如下。
```
Exception in thread "commons-pool-EvictionTimer" java.lang.NoClassDefFoundError: redis/clients/util/IOUtils
    at redis.clients.jedis.Connection.disconnect(Connection.java:226)
    at redis.clients.jedis.BinaryClient.disconnect(BinaryClient.java:941)
    at redis.clients.jedis.BinaryJedis.disconnect(BinaryJedis.java:1771)
    at redis.clients.jedis.JedisFactory.destroyObject(JedisFactory.java:91)
    at         org.apache.commons.pool2.impl.GenericObjectPool.destroy(GenericObjectPool.java:897)
    at org.apache.commons.pool2.impl.GenericObjectPool.evict(GenericObjectPool.java:793)
    at org.apache.commons.pool2.impl.BaseGenericObjectPool$Evictor.run(BaseGenericObjectPool.java:1036)
    at java.util.TimerThread.mainLoop(Timer.java:555)
    at java.util.TimerThread.run(Timer.java:505)
Caused by: java.lang.ClassNotFoundException: redis.clients.util.IOUtils
......
```
<a name="VScHY"></a>
### 异常描述
运行时，Jedis执行命令，抛出异常，提示某个类找不到。此类问题一般都是由于加载多个jedis版本（例如jedis 2.9.0和jedis 2.6），在编译期间代码未出现问题，但类加载器在运行时加载了低版本的Jedis，造成运行时找不到类。
<a name="A6obi"></a>
### 解决方法
通常此类问题，可以将重复的Jedis排除掉，例如利用Maven的依赖树，把无用的依赖去掉或者exclusion掉。
<a name="EQ7Sc"></a>
## 服务端命令不支持
<a name="wgxpF"></a>
### 异常堆栈
例如客户端执行了geoadd命令，但是服务端返回不支持此命令。
```
redis.clients.jedis.exceptions.JedisDataException: ERR unknown command 'GEOADD'
```
<a name="SyCcG"></a>
### 异常描述
该命令不能被Redis端识别，可能有下列两个原因。

- 社区版的一些命令，阿里云Redis不支持，或者只在某些小版本上支持，例如geoadd是Redis 3.2添加的地理信息API。
- 命令本身是错误的，不过对于Jedis来说不能识别，Jedis不支持直接使用命令，每个API都有固定的函数。
<a name="VMyr3"></a>
### 解决方法
咨询是否有Redis版本支持该命令，如支持可以让客户做小版本升级。
<a name="npgmI"></a>
## pipeline错误使用
<a name="dHXBd"></a>
### 异常堆栈
异常堆栈如下。
```
redis.clients.jedis.exceptions.JedisDataException: Please close pipeline or multi block before calling this method.
```
<a name="lU51b"></a>
### 异常描述

- 在
```java
pipeline.sync()
```
执行之前，通过
```java
response.get()
```
获取值，在
```java
pipeline.sync()
```
执行前，命令没有执行（可以通过monitor做验证），下面代码就会引起上述异常。
```java
Jedis jedis = new Jedis("127.0.0.1", 6379);
Pipeline pipeline = jedis.pipelined();
pipeline.set("hello", "world"); 
pipeline.set("java", "jedis");
Response<String> pipeString = pipeline.get("java");
//这个get必须在sync之后，如果是批量获取值建议直接用List<Object> objectList = pipeline.syncAndReturnAll();
System.out.println(pipeString.get());
//命令此时真正执行
pipeline.sync();
复制代码
```

- Jedis中Reponse的`get()`方法，判断代码如下，如果`set=false`就会报错，而response中的set初始化为false。
```java
public T get() {
    // if response has dependency response and dependency is not built,
    // build it first and no more!!
    if (dependency != null && dependency.set && !dependency.built) {
        dependency.build();
    }
    if (!set) {
        throw new JedisDataException(
            "Please close pipeline or multi block before calling this method.");
    }
    if (!built) {
        build();
    }
    if (exception != null) {
        throw exception;
    }
    return response;
}
```
`pipeline.sync()`代码会将每个运行结果设置`set=true`，如下所示。
```java
public void sync() {
    if (getPipelinedResponseLength() > 0) {
        List<Object> unformatted = client.getAll();
        for (Object o : unformatted) {
            generateResponse(o);
        }
    }
}
```

- 其中
```
generateResponse(o)
```
代码如下。
```java
protected Response<?> generateResponse(Object data) {
    Response<?> response = pipelinedResponses.poll();
    if (response != null) {
        response.set(data);
    }
    return response;
}
```

- 其中
```java
response.set(data)
```
代码如下。
```java
public void set(Object data) {
    this.data = data;
    set = true;
}
```
<a name="ytbLi"></a>
### 解决方法
对于批量结果的解析，建议使用`pipeline.syncAndReturnAll()`来实现，下面操作模拟了批量hgetAll。
```java
/**
 * pipeline模拟批量hgetAll
 * @param keyList
 * @return
 */
public Map<String, Map<String, String>> mHgetAll(List<String> keyList) {
    // 1.生成pipeline对象
    Pipeline pipeline = jedis.pipelined();
    // 2.pipeline执行命令，注意此时命令并未真正执行
    for (String key : keyList) {
        pipeline.hgetAll(key);
    }
    // 3.执行命令 syncAndReturnAll()返回结果
    List<Object> objectList = pipeline.syncAndReturnAll();
    if (objectList == null || objectList.isEmpty()) {
        return Collections.emptyMap();
    }
    // 4.解析结果
    Map<String,Map<String, String>> resultMap = new HashMap<String, Map<String,String>>();
    for (int i = 0; i < objectList.size(); i++) {
        Object object = objectList.get(i);
        Map<String, String> map = (Map<String, String>) object;
        String key = keyList.get(i);
        resultMap.put(key, map);
    }
    return resultMap;
}
```
<a name="uAZXV"></a>
### 处理途径
检查并修改业务代码。
<a name="WGj6a"></a>
## 管理员命令普通用户不能执行
<a name="CO8qq"></a>
### 异常堆栈
命令role不能被普通用户执行。
```
redis.clients.jedis.exceptions.JedisDataException: ERR command role not support for normal user
```
<a name="awiP9"></a>
### 异常描述
该命令尚未开放。
<a name="i6TT2"></a>
### 解决方法
不能使用该命令，如果有需求或者疑问可以提交工单。
<a name="o6bVe"></a>
## 连接池参数
<a name="inXhH"></a>
### 资源设置和使用
资源设置和使用如下。

| 序号 | 参数名 | 含义 | 默认值 | 使用建议 |
| --- | --- | --- | --- | --- |
| 1 | maxTotal | 资源池中最大连接数 | 8 | - |
| 2 | maxIdle | 资源池允许最大空闲的连接数 | 8 | - |
| 3 | minIdle | 资源池确保最少空闲的连接数 | 0 | - |
| 4 | blockWhenExhausted | 当资源池用尽后，调用者是否要等待。只有当为true时，下面的maxWaitMillis才会生效 | true | 建议使用默认值 |
| 5 | maxWaitMillis | 当资源池连接用尽后，调用者的最大等待时间（单位为毫秒） | -1：表示永不超时 | 不建议使用默认值 |
| 6 | testOnBorrow | 向资源池借用连接时是否做连接有效性检测（ping），无效连接会被移除 | false | 业务量很大时候建议设置为false（设置为true会多一次ping的开销） |
| 7 | testOnReturn | 向资源池归还连接时是否做连接有效性检测（ping），无效连接会被移除 | false | 业务量很大时候建议设置为false（设置为true会多一次ping的开销） |
| 8 | jmxEnabled | 是否开启jmx监控，可用于监控 | true | 建议开启，但应用本身也要开启 |

<a name="FkIOX"></a>
### 空闲资源监测
空闲Jedis对象检测，下面四个参数组合来完成，`testWhileIdle`是该功能的开关。

| 序号 | 参数名 | 含义 | 默认值 | 使用建议 |
| --- | --- | --- | --- | --- |
| 1 | testWhileIdle | 是否开启空闲资源监测 | false | 设置为true |
| 2 | timeBetweenEvictionRunsMillis | 空闲资源的检测周期（单位为毫秒） | -1：不检测 | 建议设置，周期自行选择，也可以默认也可以使用下面JedisPoolConfig中的配置 |
| 3 | minEvictableIdleTimeMillis | 资源池中资源最小空闲时间（单位为毫秒），达到此值后空闲资源将被移除 | 1000 * 60 * 30 = 30分钟 | 可根据自身业务决定，大部分默认值即可，也可以考虑使用下面JeidsPoolConfig中的配置 |
| 4 | numTestsPerEvictionRun | 做空闲资源检测时，每次的采样数 | 3 | 可根据自身应用连接数进行微调，如果设置为-1，就是对所有连接做空闲监测 |

