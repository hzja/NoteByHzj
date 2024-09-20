<a name="eVKE1"></a>
## **1、Redisson是什么**
Redisson是一个基于Redis的分布式Java框架。它提供了丰富的功能和工具，帮助开发者在分布式系统中解决数据共享、并发控制和任务调度等问题。通过使用Redisson，开发者可以轻松地操作Redis的分布式对象（如集合、映射、队列等），实现可靠的分布式锁机制，以及管理和调度分布式环境中的任务和服务。
<a name="y53nN"></a>
### Redisson提供的功能
**分布式对象：**

- 分布式集合（Set、SortedSet、List）
- 分布式映射（Map）
- 分布式队列（Queue、Deque）
- 分布式锁（Lock）
- 分布式计数器（AtomicLong）

**分布式限流：**

- 令牌桶算法（Rate Limiter）
- 漏桶算法（Rate Limiter）

**分布式发布订阅：**

- 发布订阅模式（Pub-Sub）
- 消息监听器容器（Message Listener Container）

**分布式锁和同步：**

- 可重入锁（ReentrantLock）
- 公平锁（FairLock）
- 联锁（MultiLock）
- 红锁（RedLock）
- 读写锁（ReadWriteLock）
- 信号量（Semaphore）
- 闭锁（CountDownLatch）
- 栅栏（CyclicBarrier）

**分布式服务和任务调度：**

- 远程服务（Remote Service）
- 分布式任务调度器（Task Scheduler）
- 分布式延迟队列（Delayed Queue）

**分布式地理空间索引（Geospatial Index）：**

- 地理位置存储
- 地理位置搜索

**分布式布隆过滤器（Bloom Filter）和可布隆过滤器（Bloom Filter）。**<br />**分布式缓存：**

- 对Redis进行本地缓存
- Spring缓存注解支持

**分布式连接池：**

- 支持连接池管理和维护

**Redis集群和哨兵支持：**

- 支持Redis集群模式
- 支持Redis哨兵模式
- 对于使用Redis集群部署的场景，Redisson可以自动识别和操作集群中的多个节点，保证数据的高可用性和扩展性。而对于使用Redis哨兵模式部署的场景，Redisson可以监控并切换到可用的主从节点，实现高可靠性和容错能力。

**Spring集成：**

- 与Spring框架的无缝集成
- 支持Spring缓存注解
<a name="QgSq1"></a>
## **2、Redisson分布式锁**
<a name="bjG0v"></a>
### Redisson的分布式锁的特点
线程安全：分布式锁可以确保在多线程和多进程环境下的数据一致性和可靠性。

- **可重入性：** 同一个线程可以多次获取同一个锁，避免死锁的问题。
- **锁超时：** 支持设置锁的有效期，防止锁被长时间占用而导致系统出现问题。
- **阻塞式获取锁：** 当某个线程尝试获取锁时，如果锁已经被其他线程占用，则该线程可以选择等待直到锁释放。
- **无阻塞式获取锁：** 当某个线程尝试获取锁时，如果锁已经被其他线程占用，则该线程不会等待，而是立即返回获取锁失败的信息。
<a name="MxtAx"></a>
### **Redisson的分布式锁的缺点**
单点故障： Redisson的分布式锁依赖于Redis集群，如果Redis集群出现故障或不可用，可能导致分布式锁的可靠性和可用性受到影响。因此，在使用Redisson分布式锁时，需要特别关注Redis集群的稳定性和高可用性。<br />**锁竞争：** 当多个线程同时请求获取分布式锁时，可能出现锁竞争的情况。如果锁竞争较为激烈，可能会导致性能下降和请求超时等问题。此外，由于Redisson分布式锁是基于Redis进行实现的，如果Redis节点的处理能力无法满足高并发的锁请求，可能会导致锁请求被延迟或阻塞。<br />**死锁风险：** 分布式环境下，由于网络通信、节点故障等因素，可能导致锁无法正常释放，从而引发死锁问题。需要合理设计和使用锁的超时时间、自动释放机制等来降低死锁风险。<br />**锁粒度管理：** 在分布式环境下，锁的粒度管理是一个重要的问题。过于细粒度的锁可能导致并发性能下降，而过于粗粒度的锁可能会影响系统的可伸缩性和并发性能。需要根据具体的业务场景和并发访问模式合理选择锁的粒度。<br />**数据一致性：** 使用分布式锁保证多个操作的原子性是很常见的应用场景之一。然而，分布式锁通常只能提供粗粒度的互斥访问，不能保证数据的完全一致性。在一些特定的应用场景中，可能需要额外的措施来确保数据的最终一致性。
<a name="n4zhw"></a>
### Redisson分布式锁源码分析
```java
public interface RLock extends Lock, RLockAsync {
    String getName();

    void lockInterruptibly(long var1, TimeUnit var3) throws InterruptedException;

    boolean tryLock(long var1, long var3, TimeUnit var5) throws InterruptedException;

    void lock(long var1, TimeUnit var3);

    boolean forceUnlock();

    boolean isLocked();

    boolean isHeldByThread(long var1);

    boolean isHeldByCurrentThread();

    int getHoldCount();

    long remainTimeToLive();
}
```
RLock接口主要继承了Lock接口，它是Redisson提供的用于分布式锁的核心接口，它定义了获取锁和释放锁等方法 ，并扩展了很多方法。<br />如：
```java
void lock(long leaseTime, TimeUnit unit)
```

- 功能：获取锁，并设置锁的自动释放时间。
- 参数：
   - leaseTime：锁的自动释放时间。
   - unit：时间单位。
```java
RFuture<Void> lockAsync(long leaseTime, TimeUnit unit)
```

- 功能：异步方式获取锁，并设置锁的自动释放时间。
- 参数：
   - leaseTime：锁的自动释放时间。
   - unit：时间单位。
- 返回值：一个RFuture对象，表示异步操作的结果。
```java
boolean tryLock(long waitTime, long leaseTime, TimeUnit unit) throws InterruptedException
```

- 功能：尝试在指定的等待时间内获取锁，并设置锁的自动释放时间。
- 参数：
   - waitTime：等待获取锁的最大时间量。
   - leaseTime：锁的自动释放时间。
   - unit：时间单位。
- 返回值：如果在等待时间内成功获取锁，则返回true；否则返回false。
- 异常：如果在等待获取锁的过程中被中断，则抛出InterruptedException。

通过上述方法，RLock接口提供了更多对lock()方法的拓展，使得在获取锁时可以设置自动释放时间或进行异步操作。这样可以更加灵活地控制锁的行为，适应不同场景下的需求。<br />除了上述拓展，RLock接口还提供了其他方法来支持可重入锁、公平锁、红锁、读写锁等特性，以便满足更为复杂的分布式锁需求。
<a name="V8IpC"></a>
## **3、Spring Boot 整合 Redisson 分布式锁**
<a name="GDN1O"></a>
### 添加Maven依赖
```xml
<!-- Redisson依赖 -->
<dependency>
  <groupId>org.redisson</groupId>
  <artifactId>redisson-spring-boot-starter</artifactId>
  <version>3.16.2</version>
</dependency>

<!-- Spring Data Redis依赖 -->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```
<a name="g7rYs"></a>
### 配置Redisson连接application.yml
```yaml
spring:
  redis:
    cluster:
      nodes:
        - 127.0.0.1:7000
        - 127.0.0.1:7001
        - 127.0.0.1:7002
    password: yourRedisPassword
```
<a name="joEva"></a>
### 创建Redisson客户端
```java
import org.redisson.Redisson;
import org.redisson.api.RedissonClient;
import org.redisson.config.Config;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
 
@Configuration
public class RedissonConfig {
 
    @Value("${spring.redis.cluster.nodes}")
    private String clusterNodes;
 
    @Value("${spring.redis.password}")
    private String password;
 
    @Bean
    public RedissonClient redissonClient() {
        Config config = new Config();
        config.useClusterServers()
                .addNodeAddress(clusterNodes.split(","))
                .setPassword(password);
 
        return Redisson.create(config);
    }
}
```
<a name="WlUfs"></a>
### 使用分布式锁
在需要使用分布式锁的地方注入`RedissonClient`实例，并使用getLock方法创建一个分布式锁对象（RLock）。
```java
import org.redisson.api.RLock;
import org.redisson.api.RedissonClient;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class SomeService {

    @Autowired
    private RedissonClient redissonClient;

    public void doSomething() {
        String lockKey = "myLock"; // 锁的key

        RLock lock = redissonClient.getLock(lockKey);
        try {
            lock.lock(); // 获取锁
            // 在这里执行需要加锁保护的代码
        } finally {
            lock.unlock(); // 释放锁
        }
    }
}
```
<a name="rQsMJ"></a>
###  `RLock.lock()`
使用`Rlock.lock()` 方法时 ，如果当前没有其他线程或进程持有该锁，那么调用线程将立即获得锁定，并继续执行后续的代码。如果其他线程或进程已经持有了该锁，那么调用线程将被阻塞，直到该锁被释放为止。<br />此外，`Rlock.lock()` 方法还具有以下特点：

- **可重入性：** 同一个线程可以多次调用 `Rlock.lock()` 方法而不会造成死锁，只需确保每次 `lock()` 调用都有相应的 `unlock()` 调用与之匹配。
- **超时机制：** 可以通过 `lock()` 方法中的参数设置等待锁的超时时间，避免因为无法获得锁而一直等待。
- **自动续期：** 当线程持有锁的时间超过设置的锁的过期时间时，Redisson 会自动延长锁的有效期，避免因为业务执行时间过长而导致锁过期。
- **防止死锁：** Redisson 通过唯一标识锁的 ID 来区分不同的锁，防止发生死锁。

`**lock()**`** 方法加锁流程**<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698823113382-f8d6b13f-808f-4036-89b4-35ddf9a60441.jpeg)
<a name="l5080"></a>
### `RLock.unlock()`
`RLock.unlock()`方法用于释放由Redission分布式锁所保护的资源。它允许持有锁的线程主动释放锁，从而允许其他线程获取该锁并访问共享资源。<br />**注意事项：**<br />`RLock.unlock()`方法应该在保护的临界区代码执行完毕后进行调用，以确保锁的及时释放。<br />在多线程环境下，释放锁的顺序应该与获取锁的顺序相对应，以避免死锁或资源争用的问题。<br />如果当前线程没有持有锁，调用`RLock.unlock()`方法不会抛出异常，也不会影响其他线程。<br />如果Redisson客户端刚加锁成功，并且未指定leaseTime，后台会启动一个定时任务watchdog每隔10s检查key，key如果存在就为它⾃动续命到30s；在watchdog定时任务存在的情况下，如果不是主动释放锁，那么key将会⼀直的被watchdog这个定时任务维持加锁。但是如果客户端宕机了，定时任务watchdog也就没了，也就没有锁续约机制了，那么过完30s之后，key会⾃动被删除、key对应的锁也自动被释放了。<br />**unlock()方法解锁流程**<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698823589767-72af7b6a-8412-47c8-b16f-56fdfc3be7a0.jpeg)
