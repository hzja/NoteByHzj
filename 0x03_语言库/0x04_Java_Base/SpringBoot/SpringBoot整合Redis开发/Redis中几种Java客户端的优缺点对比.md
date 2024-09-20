JavaRedis
<a name="v8xDv"></a>
### 一、摘要
相对于其他的分布式中间件，Redis 支持的客户端种类非常繁多，涵盖更加全面，除了支持比较流行的 c、c++、Java、C#、php、Python 等语言以外，还支持 Objective-C、Swift、Node.js 等等。<br />站在 Java 开发语言的角度，虽然可供选择的客户端非常多，但是官方主要推荐使用以下三种客户端，原因大概是它们对 redis 提供的支持更加全面、api 操作更佳丰富。

- Jedis
- Lettuce
- Redisson

![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1673224421797-4aef29b1-f44d-4bdb-84ea-b4b97e8785f4.png#averageHue=%23e2cca6&clientId=u9919ed30-23b6-4&from=paste&height=713&id=uab7b507a&originHeight=1783&originWidth=3796&originalType=binary&ratio=1&rotation=0&showTitle=false&size=728589&status=done&style=none&taskId=uc04eaee2-45ca-4caf-b322-7dd526496ab&title=&width=1518.4)<br />来看看这三个客户端的优缺点，以及使用建议！
<a name="fGFJ3"></a>
### 二、客户端对比
<a name="gZoOn"></a>
#### 2.1、Jedis
Jedis 是一款老牌 Redis 的 Java 客户端，提供了比较全面的 Redis 命令的操作支持，也是目前使用最广泛的客户端。<br />项目访问地址：[https://github.com/redis/jedis](https://github.com/redis/jedis)<br />优点如下：

- Jedis 的 API 提供了比较全面的 Redis 命令的支持
- Jedis 中的 Java 方法基本和 Redis 的 API 保持着一致，也就是说了解 Redis 的API，可以熟练的使用 Jedis
- 支持 pipelining、事务、LUA Scripting、Redis Sentinel、Redis Cluster等等 redis 提供的高级特性
- 客户端轻量，简洁，便于集成和改造
- 使用广泛，开发人员易上手

Jedis 有优点也有劣势，缺点如下：

- 使用阻塞的 I/O 操作，且其方法调用都是同步的，程序流需要等到 sockets 处理完 I/O 才能执行，不支持异步
- Jedis 在实现上是直接连接的 redis server，如果在多线程环境下是非线程安全的，这个时候可以使用连接池来管理 Jedis，已解决 Jedis 客户端实例存在非线程安全的问题
- 不支持读写分离，需要自己实现
- 技术文档差，可以说几乎没有

整体来说，Jedis 是一款经典的 Redis 客户端（Java 语言方向），能满足绝大部分项目中的业务开发需求，虽然有些瑕疵，但是可以通过其它方式来弥补，可用性、安全性方面都有保证，总体评价是操作简单，易上手！
<a name="IPFkX"></a>
#### 2.2、Lettuce
Lettuce 是一种可扩展的、线程安全的 Redis 高级客户端。支持异步模式。<br />如果避免阻塞和事务操作，如BLPOP和MULTI/EXEC，多个线程就可以共享一个连接。<br />lettuce 底层基于 Netty，支持高级的 Redis 特性，比如哨兵，集群，管道，自动重新连接和Redis数据模型。<br />lettuce能够支持redis4，需要Java8及以上。<br />lettuce是基于netty实现的与redis进行同步和异步的通信。<br />从 Spring Boot 2.x 开始， Lettuce 已取代 Jedis 成为SpringBoot 默认的 Redis 客户端<br />项目访问地址：[https://lettuce.io/](https://lettuce.io/)<br />lettuce git项目地址：[https://github.com/lettuce-io/lettuce-core](https://github.com/lettuce-io/lettuce-core)<br />优点如下：

- 相比于 Jedis，Lettuce 属于后起之秀，对 Redis 更加全面，并且解决了 Jedis 客户端实例存在非线程安全的问题
- 支持同步编程，异步编程，响应式编程，自动重新连接，主从模式，集群模块，哨兵模式，管道和编码器等等高级的 Redis 特性
- Lettuce 底层基于 Netty 框架的事件驱动与 redis 通信，采用了非阻塞的 I/O 操作，可异步调用，相比 Jedis，性能高
- Lettuce 的 API 是线程安全的，如果不是执行阻塞和事务操作，如 BLPOP 和MULTI/EXEC 等命令，多个线程就可以共享一个连接，性能方面不会衰减

缺点如下：

- API 更加抽象，学习使用成本高
<a name="LlhYq"></a>
#### 2.3、Redisson
Redisson 是一个在 Redis 的功能基础上实现的 Java 驻内存数据网格客户端。实现了分布式和可扩展的 Java 数据结构，提供很多分布式相关操作服务，例如分布式锁，分布式集合，可通过 Redis 支持延迟队列。<br />其中包括：BitSet, Set, Multimap, SortedSet, Map, List, Queue, BlockingQueue, Deque, BlockingDeque, Semaphore, Lock, AtomicLong, CountDownLatch, Publish / Subscribe, Bloom filter, Remote service, Spring cache, Executor service, Live Object service, Scheduler service)<br />Redisson 的宗旨是促进使用者对Redis的关注分离（Separation of Concern），从而让使用者能够将精力更集中地放在处理业务逻辑上。<br />Redisson 官网地址：[https://redisson.org/](https://redisson.org/)<br />项目访问地址：[https://github.com/redisson/redisson](https://github.com/redisson/redisson)<br />优点如下：

- 实现了分布式特性和可扩展的 Java 数据结构，例如分布式锁，分布式集合，分布式对象，分布式远程调度等等高级功能，适合分布式开发
- 与 Lettuce 一样，基于 Netty 框架的事件驱动与 redis 通信，支持异步调用，性能高
- Redisson 的 API 是线程安全的，所以可以使用单个 Redisson 连接来完成各种操作。
- 支持读写分离，支持读负载均衡，在主从复制和 Redis Cluster 架构下都可以使用
- 内建 Tomcat Session Manager，为 Tomcat 6/7/8 提供了会话共享功能，可以与 Spring Session 集成，实现基于 Redis 的会话共享
- 相比于 Jedis、Lettuce 等基于 redis 命令封装的客户端，Redisson 提供的功能更加高端和抽象，Redisson 可以类比 Spring 框架，这些框架搭建了应用程序的基础框架和功能，可以显著提升开发效率，让开发者有更多的时间来关注业务逻辑
- 文档较丰富，有中文文档

缺点如下：

- 和 Jedis、Lettuce 客户端相比，功能较为简单，对字符串的支持比较差，不支持排序、事务、管道、分区等 Redis 特性
- API 更加抽象，学习使用成本高
<a name="fN5Xt"></a>
### 三、小结
Jedis使直接连接redis server，如果在多线程环境下是非线程安全的，这个时候只有使用连接池，为每个jedis实例增加物理连接；Jedis 的性能比较差，所以如果不需要使用 Redis 的高级功能的话，优先推荐使用 lettuce。<br />lettuce的连接是基于Netty的，连接实例（StatefulRedisConnection）可以在多个线程间并发访问，StatefulRedisConnection是线程安全的，所以一个连接实例可以满足多线程环境下的并发访问，当然这也是可伸缩的设计，一个连接实例不够的情况也可以按需增加连接实例。<br />Jedis 和 lettuce 是比较纯粹的 Redis 客户端，几乎没提供什么高级功能。<br />Jedis 和 Lettuce 两者相比，Jedis 的性能比较差，其他方面并没有太明显的区别，所以如果不需要使用 Redis 的高级功能的话，优先推荐使用 Lettuce。<br />Redisson实现了分布式和可扩展的Java数据结构，和Jedis相比，功能较为简单，不支持字符串操作，不支持排序、事务、管道、分区等Redis特性。<br />Redisson的宗旨是促进使用者对Redis的关注分离，从而让使用者能够将精力更集中地放在处理业务逻辑上。<br />如果需要分布式锁，分布式集合等分布式的高级特性，添加Redisson结合使用，因为Redisson本身对字符串的操作支持很差。<br />相比于 Jedis、Lettuce 等基于 redis 命令封装的客户端，Redisson 提供的功能更加高端和抽象，逼格高！<br />Redisson 的优势是提供了很多开箱即用的 Redis 高级功能，如果应用中需要使用到 Redis 的高级功能，比如分布式锁，分布式对象，分布式会话共享等等，建议使用 Redisson。<br />总结下来：

- 如果项目中对分布式功能的需求场景不多，优先推荐使用 Lettuce，基本上够用，当然使用 Jedis 也没用问题，api 操作方面会更加简单。
- 如果项目中除了对基本的数据缓存操作需求以外，还需要用到分布式锁，分布式对象，分布式集合等功能，优先推荐采用Lettuce + Redisson组合方式使用。
