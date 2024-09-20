Redis Client Jedis lettuce Redisson<br />Redis 支持多种语言的客户端，下面列举了部分 Redis 支持的客户端语言，可以通过官网查看 Redis 支持的客户端详情。

- C语言<br />
- C++<br />
- C#<br />
- Java<br />
- Python<br />
- Node.js<br />
- PHP<br />

Redis 是用单线程来处理多个客户端的访问，因此作为 Redis 的开发和运维人员需要了解 Redis 服务端和客户端的通信协议，以及主流编程语言的 Redis 客户端使用方法，同时还需要了解客户端管理的相应 API 以及开发运维中可能遇到的问题。
<a name="RakI7"></a>
## Redis 客户端通信协议
Redis制定了RESP（Redis Serialization Protocol，Redis序列化协议）实现客户端与服务端的正常交互，这种协议简单高效，既能够被机器解析，又容易被人类识别。<br />`RESP`可以序列化不同的数据类型，如整型、字符串、数组还有一种特殊的`Error`类型。需要执行的`Redis`命令会封装为类似于字符串数组的请求然后通过`Redis`客户端发送到`Redis`服务端。`Redis`服务端会基于特定的命令类型选择对应的一种数据类型进行回复。<br />1. RESP 发送命令格式<br />在`RESP`中，发送的数据类型取决于数据报的第一个字节：

- 单行字符串的第一个字节为`+`。<br />
- 错误消息的第一个字节为`-`。<br />
- 整型数字的第一个字节为`:`。<br />
- 定长字符串的第一个字节为`$`。<br />
- `RESP`数组的第一个字节为`*`。<br />
| 数据类型 | 翻译名称 | 基本特征 | 例子 |
| :--- | :--- | :--- | :--- |
| `Simple String` | 单行字符串 | 第一个字节是`+`，最后两个字节是`\\r\\n`，其他字节是字符串内容 | `+OK\r\n` |
| `Error` | 错误消息 | 第一个字节是`-`，最后两个字节是`\\r\\n`，其他字节是异常消息的文本内容 | `-ERR\r\n` |
| `Integer` | 整型数字 | 第一个字节是`:`，最后两个字节是`\\r\\n`，其他字节是数字的文本内容 | `:100\r\n` |
| `Bulk String` | 定长字符串 | 第一个字节是`$`，紧接着的字节是`内容字符串长度\\r\\n`，最后两个字节是`\\r\\n`，其他字节是字符串内容 | `$4\r\ndoge\r\n` |
| `Array` | `RESP`数组 | 第一个字节是`*`，紧接着的字节是`元素个数\\r\\n`，最后两个字节是`\\r\\n`，其他字节是各个元素的内容，每个元素可以是任意一种数据类型 | `*2\r\n:100\r\n$4\r\ndoge\r\n` |

发送的命令格式如下，CRLF代表"\r\n":
```bash
*<参数数量> CRLF
$<参数1的字节数量> CRLF
<参数1> CRLF
...
$<参数N的字节数量> CRLF
<参数N> CRLF
```
以`set hello world`这个命令为例，发送的内容就是这样的：
```
*3
$3
SET
$5
hello
$5
world
```
第一行*3表示有3个参数，$3表示接下来的一个参数有3个字节，接下来是参数，$5表示下一个参数有5个字节，接下来是参数，$5表示下一个参数有5个字节，接下来是参数。<br />所以set hello world最终发送给redis服务器的命令是：
```
*3\r\n$3\r\nSET\r\n$5\r\nhello\r\n$5\r\nworld\r\n
```
2. RESP 响应内容
```
Redis的返回结果类型分为以下五种：
        正确回复：在RESP中第一个字节为"+"
        错误回复：在RESP中第一个字节为"-"
        整数回复：在RESP中第一个字节为":"
        字符串回复：在RESP中第一个字节为"$"
        多条字符串回复：在RESP中第一个字节为"*"
(+) 表示一个正确的状态信息，具体信息是当前行+后面的字符。
(-)  表示一个错误信息，具体信息是当前行－后面的字符。
(*) 表示消息体总共有多少行，不包括当前行,*后面是具体的行数。
($) 表示下一行数据长度，不包括换行符长度\r\n,$后面则是对应的长度的数据。
(:) 表示返回一个数值，：后面是相应的数字节符。
```
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1602947314626-ff046169-53f9-44d5-aaf3-02a2abda25f4.webp#align=left&display=inline&height=258&originHeight=258&originWidth=654&size=0&status=done&style=shadow&width=654)<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1602947314596-f8d347af-96ec-45e0-bf90-1a332877342a.webp#align=left&display=inline&height=409&originHeight=409&originWidth=723&size=0&status=done&style=shadow&width=723)<br />有了这个协议，就可以编写程序来和 Redis 服务端进行通信。由于 Redis 的流行，已经存在了很多流行的开源客户端。主要选择 Java 领域 Redis 官方推荐的客户端进行介绍。
<a name="boy5S"></a>
## Redis 的 Java 客户端
Redis 官方推荐的 Java 客户端有Jedis、lettuce 和 Redisson。
<a name="f1636c2a"></a>
### 1. Jedis
Jedis 是老牌的 Redis 的 Java 实现客户端，提供了比较全面的 Redis 命令的支持，其官方网址是：[http://tool.oschina.net/uploads/apidocs/redis/clients/jedis/Jedis.html](http://tool.oschina.net/uploads/apidocs/redis/clients/jedis/Jedis.html)。
<a name="QEBNJ"></a>
#### 优点：

- 支持全面的 Redis 操作特性（可以理解为API比较全面）。<br />
<a name="myj0S"></a>
#### 缺点：

- 使用阻塞的 I/O，且其方法调用都是同步的，程序流需要等到 sockets 处理完 I/O 才能执行，不支持异步；<br />
- Jedis 客户端实例不是线程安全的，所以需要通过连接池来使用 Jedis。<br />
<a name="GfQd9"></a>
### 2. lettuce
lettuce （[ˈletɪs]），是一种可扩展的线程安全的 Redis 客户端，支持异步模式。如果避免阻塞和事务操作，如BLPOP和MULTI/EXEC，多个线程就可以共享一个连接。lettuce 底层基于 Netty，支持高级的 Redis 特性，比如哨兵，集群，管道，自动重新连接和Redis数据模型。lettuce 的官网地址是：[https://lettuce.io/](https://lettuce.io/)
<a name="ZyxB7"></a>
#### 优点：

- 支持同步异步通信模式；<br />
- Lettuce 的 API 是线程安全的，如果不是执行阻塞和事务操作，如BLPOP和MULTI/EXEC，多个线程就可以共享一个连接。<br />
<a name="c9170577"></a>
### 3. Redisson
Redisson 是一个在 Redis 的基础上实现的 Java 驻内存数据网格（In-Memory Data Grid）。它不仅提供了一系列的分布式的 Java 常用对象，还提供了许多分布式服务。其中包括( BitSet, Set, Multimap, SortedSet, Map, List, Queue, BlockingQueue, Deque, BlockingDeque, Semaphore, Lock, AtomicLong, CountDownLatch, Publish / Subscribe, Bloom filter, Remote service, Spring cache, Executor service, Live Object service, Scheduler service) Redisson 提供了使用Redis 的最简单和最便捷的方法。Redisson 的宗旨是促进使用者对Redis的关注分离（Separation of Concern），从而让使用者能够将精力更集中地放在处理业务逻辑上。Redisson的官方网址是：[https://redisson.org/](https://redisson.org/)
<a name="Kknr3"></a>
#### 优点：

- 使用者对 Redis 的关注分离，可以类比 Spring 框架，这些框架搭建了应用程序的基础框架和功能，提升开发效率，让开发者有更多的时间来关注业务逻辑；<br />
- 提供很多分布式相关操作服务，例如，分布式锁，分布式集合，可通过Redis支持延迟队列等。<br />
<a name="ykqBi"></a>
#### 缺点：

- Redisson 对字符串的操作支持比较差。<br />
<a name="yTA8P"></a>
### 4. 使用建议
结论：lettuce + Redisson<br />Jedis 和 lettuce 是比较纯粹的 Redis 客户端，几乎没提供什么高级功能。Jedis 的性能比较差，所以如果不需要使用 Redis 的高级功能的话，优先推荐使用 lettuce。<br />Redisson 的优势是提供了很多开箱即用的 Redis 高级功能，如果项目中需要使用到 Redis 的高级功能，建议使用 Redisson。具体 Redisson 的高级功能可以参考：[https://redisson.org/](https://redisson.org/)
