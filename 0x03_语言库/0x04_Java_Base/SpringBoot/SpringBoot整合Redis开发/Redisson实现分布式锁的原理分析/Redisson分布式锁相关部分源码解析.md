JavaRedisson
<a name="YneNn"></a>
## 一、Redisson 简介
Redisson 是一个基于 Netty 通信框架的高性能 Redis 客户端， 实现了分布式和可扩展的 Java 数据结构，提供很多分布式相关操作服务以及大量便利的工具方法，让开发者可以把精力放在开发业务，避免重复造轮子。
<a name="Din79"></a>
## 二、Redisson 优点

1. 通信框架基于 Netty，使用多路复用。吞吐量高。
2. 兼容支持 Redis 集群模式，Reids 哨兵模式等，天然适配分布式服务。
3. 提供多种分布式对象的封装，如：Bloom Filter，Object Bucket，Bitset，AtomicLong， 和 HyperLogLog 等。
4. 提供分布式锁实现包括：

RedissonFairLock 公平锁，<br />RedissonLock 非公平锁，<br />RedissonRedLock 红锁（基于红锁算法， 当集群中大多数（ N/2 + 1 ）加锁成功了，则认为加锁成功，<br />目前已被弃用，Redisson 官方不再建议使用）。
<a name="z93e6"></a>
## 三、RedissonLock 分布式锁相关部分源码解析
RedissonLock 作为分布式锁，实现了可重入锁。阻塞锁，非阻塞锁。并且 Redisson 存在看门狗机制，可以对未手动设置超时时间的锁实现自动续期。
<a name="w1MRu"></a>
### 1、Trylock 加锁
加锁代码逻辑
```java
/**
 *
 * @param waitTime 获取锁的最大等待时间，默认 -1，
 * @param leaseTime 锁的过期时间，默认 -1
 * @param unit
 * @param threadId
 * @return
 */
private RFuture<Boolean> tryAcquireOnceAsync(long waitTime, long leaseTime, TimeUnit unit, long threadId) {
    RFuture<Boolean> acquiredFuture;
    if (leaseTime > 0) {
        //若手动设置了锁的过期时间，则加锁时以当前传入过期时间为准
        //执行Lua脚本，加锁
        acquiredFuture = tryLockInnerAsync(waitTime, leaseTime, unit, 
                                           threadId,RedisCommands.EVAL_NULL_BOOLEAN);                                                 
    } else {
        //若未手动设置，则默认过期时间等于配置的lockWatchdogTimeout，lockWatchdogTimeout默认为30s。
        //然后执行Lua脚本，加锁
        acquiredFuture = tryLockInnerAsync(waitTime, internalLockLeaseTime,
                                           TimeUnit.MILLISECONDS, threadId, RedisCommands.EVAL_NULL_BOOLEAN);
    }

    CompletionStage<Boolean> f = acquiredFuture.thenApply(acquired -> {
        //lock acquired
        //若锁成功获取到
        if (acquired) {
            if (leaseTime > 0) {
                internalLockLeaseTime = unit.toMillis(leaseTime);
            } else {
                //若未手动设置过期时间，则执行看门狗任务，自动续期
                scheduleExpirationRenewal(threadId);
            }
        }
        return acquired;
    });
    return new CompletableFutureWrapper<>(f);
}
```
加锁 Lua 脚本如下：
```lua
if (redis.call('exists', KEYS[1]) == 0) then " +
  "redis.call('hincrby', KEYS[1], ARGV[2], 1); " +
  "redis.call('pexpire', KEYS[1], ARGV[1]); " +
  "return nil; " +
  "end; " +
  "if (redis.call('hexists', KEYS[1], ARGV[2]) == 1) then " +
  "redis.call('hincrby', KEYS[1], ARGV[2], 1); " +
  "redis.call('pexpire', KEYS[1], ARGV[1]); " +
  "return nil; " +
  "end; " +
  "return redis.call('pttl', KEYS[1]);
```
其中 KEYS[1] 是锁逻辑名称，ARGV[1] 是 key 的过期时间，ARGV[2]是锁的线程级别名称（ uuid + 线程id ，uuid 是每个 Redisson 客户端创建时唯一生成的）。<br />由此可看出，锁利用 Hash 结构实现，其中 Hash 的 key 是锁的逻辑名称，field 是锁的线程级别名称，value 是锁的重入次数。<br />加锁 Lua 脚本的含义：

1. 先判断当前逻辑锁名称的 key 是否存在，
2. 若不存在，在 Hash 结构中设置这个锁，锁重入次数加 1，然后给 key 设置一个过期时间，最后返回 null。
3. 若存在，并且已经被当前线程持有，就锁可重入次数加 1，并且重新设置 key 的过期时间,最后返回 null，
4. 若当前锁被其他线程持有，返回 key 剩余过期时间。
<a name="aI0EA"></a>
### 2、Lock 阻塞锁
Lock 阻塞锁与 Trylock 底层调用代码基本一致。多了一个等待锁被其他线程释放后，重新尝试加锁的过程。<br />代码如下：
```java
private void lock(long leaseTime, TimeUnit unit, boolean interruptibly) throws InterruptedException {
    long threadId = Thread.currentThread().getId();
    Long ttl = tryAcquire(-1, leaseTime, unit, threadId);
    // lock acquired
    if (ttl == null) {
        return;
    }
    //订阅释放锁消息
    CompletableFuture<RedissonLockEntry> future = subscribe(threadId);
    pubSub.timeout(future);
    RedissonLockEntry entry;
    if (interruptibly) {
        entry = commandExecutor.getInterrupted(future);
    } else {
        entry = commandExecutor.get(future);
    }

    try {
        while (true) {
            //重新尝试取锁
            ttl = tryAcquire(-1, leaseTime, unit, threadId);
            // lock acquired
            if (ttl == null) {
                break;
            }
            // waiting for message，
            if (ttl >= 0) {
                try {
                    //当锁仍然被其他线程占有时，调用
                    //java.util.concurrent.Semaphore#tryAcquire方法进行信号量阻塞，
                    //当线程阻塞等待时间超过最大超时时间（ttl即锁的key的剩余存活时间）
                    //或者 监听到锁释放消息后，信号量被释放后，线程不再阻塞
                    entry.getLatch().tryAcquire(ttl, TimeUnit.MILLISECONDS);
                } catch (InterruptedException e) {
                    if (interruptibly) {
                        throw e;
                    }
                    entry.getLatch().tryAcquire(ttl, TimeUnit.MILLISECONDS);
                }
            } else {
                if (interruptibly) {
                    //尝试从信号量获取一个许可
                    entry.getLatch().acquire();
                } else {
                    entry.getLatch().acquireUninterruptibly();
                }
            }
        }
    } finally {
        //取消订阅锁释放消息
        unsubscribe(entry, threadId);
    }
}
```
大致流程如下：

1. 先获取锁，若获取锁成功，直接返回。
2. 若获取失败，订阅释放锁消息。
3. 进入 while 循环，重新尝试获取锁。若获取锁成功，则跳出循环，并不再订阅释放锁消息。
4. 若重新获取锁失败，进行信号量阻塞，直到锁被其他占有线程释放（监听锁释放消息的监听器中，有唤醒信号量的逻辑）或者到达阻塞超时时间，然后继续这个 while 循环。
<a name="GLvwq"></a>
### 3、Unlock 解锁
代码如下
```java
public RFuture<Void> unlockAsync(long threadId) {
    //执行解锁lua脚本
    RFuture<Boolean> future = unlockInnerAsync(threadId);
    
    CompletionStage<Void> f = future.handle((opStatus, e) -> {
        //取消看门狗任务
        cancelExpirationRenewal(threadId);
    
        if (e != null) {
            throw new CompletionException(e);
        }
        if (opStatus == null) {
            IllegalMonitorStateException cause = new IllegalMonitorStateException
            ("attempt to unlock lock, not locked by current thread by node id: "
             + id + " thread-id: " + threadId);
            throw new CompletionException(cause);
        }
        return null;
    });
    
    return new CompletableFutureWrapper<>(f);
}
```

1. 其中解锁 Lua 脚本如下：
```lua
if (redis.call('hexists', KEYS[1], ARGV[3]) == 0) then " +
  "return nil;" +
  "end; " +
  "local counter = redis.call('hincrby', KEYS[1], ARGV[3], -1); " +
  "if (counter > 0) then " +
  "redis.call('pexpire', KEYS[1], ARGV[2]); " +
  "return 0; " +
  "else " +
  "redis.call('del', KEYS[1]); " +
  "redis.call('publish', KEYS[2], ARGV[1]); " +
  "return 1; " +
  "end; " +
  "return nil;
```
其中 KEYS[1] 为锁的逻辑名称，KEYS[2] 为通道名称，ARGV[1] 为 0， ARGV[2] 为锁的过期时间，默认 30s，ARGV[3] 为锁的线程级别名称。<br />解锁 Lua 脚本含义：

- 解锁时，先判断当前锁是否被当前线程持有，
- 若不是，则返回 null。
- 若是，锁的可重入次数 减1。
- 然后继续判断锁的可重入次数是否大于 0，若大于 0，继续给这个锁 key 续期 30s，并且最后返回 0。
- 若不大于 0，删除这个锁的 key，并向指定通道发布这个解锁消息，并且返回 1。
2. 如果这个锁有看门狗任务在定时续期，当解锁成功时会取消这个定时续期任务。
<a name="HyaBd"></a>
### 4、看门狗机制
当某个锁内的任务的执行时间不可预估时，可能执行时间很长，也可能很短。此时若直接设置一个固定的锁过期时间，可能会导致任务执行时间远远大于锁的过期时间，导致任务还未执行完成，但是锁已经过期了。那其他线程又可以获取到锁，然后执行该任务了，最终导致线程安全问题。<br />为应对这种情况，定期给锁续期的看门狗机制出现了。<br />代码：
```java
//真正看门狗续期任务
private void renewExpiration() {
    ExpirationEntry ee = EXPIRATION_RENEWAL_MAP.get(getEntryName());
    if (ee == null) {
        return;
    }
    //创建一个延时任务，底层实现是netty时间轮。当每过了lockWatchdogTimeout/3的时间，执行该任务
    Timeout task = commandExecutor.getConnectionManager().newTimeout(new TimerTask() {
        @Override
        public void run(Timeout timeout) throws Exception {
            ExpirationEntry ent = EXPIRATION_RENEWAL_MAP.get(getEntryName());
            if (ent == null) {
                return;
            }
            Long threadId = ent.getFirstThreadId();
            //若当前锁已经被当前线程释放，则锁不再续期
            if (threadId == null) {
                return;
            }
            //调用Lua脚本，判断当前锁是否被当前线程占有，若是则返回true,
            //并且重新设置key的过期时间，默认30s
            CompletionStage<Boolean> future = renewExpirationAsync(threadId);
            future.whenComplete((res, e) -> {
                if (e != null) {
                    log.error("Can't update lock " + getRawName() + " expiration", e);
                    EXPIRATION_RENEWAL_MAP.remove(getEntryName());
                    return;
                }
                //当锁仍然被当前线程占有，说明业务代码还在执行，则递归调用续期任务
                if (res) {
                    // reschedule itself
                    log.info("续期任务执行"+ "threadId：" +threadId);
                    renewExpiration();
                } else {
                    //否则移除该续期任务，直接在EXPIRATION_RENEWAL_MAP移除ExpirationEntry
                    cancelExpirationRenewal(null);
                }
            });
        }
    }, internalLockLeaseTime / 3, TimeUnit.MILLISECONDS);

    ee.setTimeout(task);
}
```
当没有显式指定锁过期时间时候，就默认 key 过期时间 30s，然后定时任务每 10 秒（ lockWatchdogTimeout/3 ）进行一次调用，执行锁续期动作，若这个线程还持有这个锁，就对这个线程持有的锁进行续期操作（通过 pexpire 续期 key 30s），若途中持有锁的线程 手动被 unlock 或者机器宕机才会取消这个任务。否则会一直续期。
<a name="H04mP"></a>
# 四.总结
Redisson 作为一个 Redis 客户端，基于 Redis、Lua 和 Netty 建立起了一套完善的分布式解决方案，比如分布式锁的实现，分布式对象的操作等。本文主要简单讲述了在 Redisson 中分布式锁的实现。其实在 Redisson 中还有很多值得深挖的点。比如：Redisson 中使用了大量 Netty 的特性。
