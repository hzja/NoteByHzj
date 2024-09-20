来看看这个 Redis 的看门狗机制，毕竟现在还是有很多是会使用 Redis 来实现分布式锁的，现在看看这个 Redis 是怎么实现分布式锁的，然后再来分析这个 Redis 的看门狗机制，如果没有这个机制，很多使用 Redis 来做分布式锁的小伙伴们，经常给导致死锁。
<a name="aC0sO"></a>
## Redis 实现分布式锁
Redis实现分布式锁，最主要的就是这几个条件<br />**获取锁**

- 互斥：确保只能有一个线程获取锁
- 非阻塞：尝试一次，成功返回true，失败返回false

**释放锁**

- 手动释放
- 超时释放：获取锁时添加一个超时时间

上代码：
```java
@Resource
private RedisTemplate redisTemplate;

public static final String UNLOCK_LUA;

/**
 * 释放锁脚本，原子操作
 */
static {
    StringBuilder sb = new StringBuilder();
    sb.append("if redis.call(\"get\",KEYS[1]) == ARGV[1] ");
    sb.append("then ");
    sb.append("    return redis.call(\"del\",KEYS[1]) ");
    sb.append("else ");
    sb.append("    return 0 ");
    sb.append("end ");
    UNLOCK_LUA = sb.toString();
}


/**
 * 获取分布式锁，原子操作
 * @param lockKey
 * @param requestId 唯一ID, 可以使用UUID.randomUUID().toString();
 * @param expire
 * @param timeUnit
 * @return
 */
public boolean tryLock(String lockKey, String requestId, long expire, TimeUnit timeUnit) {
    try{
        RedisCallback<Boolean> callback = (connection) -> {
            return connection.set(lockKey.getBytes(Charset.forName("UTF-8")), requestId.getBytes(Charset.forName("UTF-8")), Expiration.seconds(timeUnit.toSeconds(expire)), RedisStringCommands.SetOption.SET_IF_ABSENT);
        };
        return (Boolean)redisTemplate.execute(callback);
    } catch (Exception e) {
        log.error("redis lock error.", e);
    }
    return false;
}

/**
 * 释放锁
 * @param lockKey
 * @param requestId 唯一ID
 * @return
 */
public boolean releaseLock(String lockKey, String requestId) {
    RedisCallback<Boolean> callback = (connection) -> {
        return connection.eval(UNLOCK_LUA.getBytes(), ReturnType.BOOLEAN ,1, lockKey.getBytes(Charset.forName("UTF-8")), requestId.getBytes(Charset.forName("UTF-8")));
    };
    return (Boolean)redisTemplate.execute(callback);
}

/**
 * 获取Redis锁的value值
 * @param lockKey
 * @return
 */
public String get(String lockKey) {
    try {
        RedisCallback<String> callback = (connection) -> {
            return new String(connection.get(lockKey.getBytes()), Charset.forName("UTF-8"));
        };
        return (String)redisTemplate.execute(callback);
    } catch (Exception e) {
        log.error("get redis occurred an exception", e);
    }
    return null;
}
```
这种实现方式就是相当于直接使用 Redis 来自己实现的分布式锁，但是也不是没有框架给我们来实现，那就是Redission。而看门狗机制是Redission提供的一种自动延期机制，这个机制使得Redission提供的分布式锁是可以自动续期的。
<a name="c5Od7"></a>
## 为什么需要看门狗机制
分布式锁是不能设置永不过期的，这是为了避免在分布式的情况下，一个节点获取锁之后宕机从而出现死锁的情况，所以需要个分布式锁设置一个过期时间。但是这样会导致一个线程拿到锁后，在锁的过期时间到达的时候程序还没运行完，导致锁超时释放了，那么其他线程就能获取锁进来，从而出现问题。<br />所以，看门狗机制的自动续期，就很好地解决了这一个问题。<br />Redisson已经实现了这个分布式锁，需要的就是调用，那么来看看 Redisson 的源码，他是如何来实现看门狗机制的。<br />**tryLock**<br />RedissonLock类下:
```java
public boolean tryLock(long waitTime, TimeUnit unit) throws InterruptedException {
    return tryLock(waitTime, -1, unit);
}
```

- `waitTime`：获取锁的最大等待时间（没有传默认为-1）
- `leaseTime`：锁自动释放的时间（没有传的话默认-1）
- `unit`：时间的单位（等待时间和锁自动释放的时间单位）
```java
@Override
public boolean tryLock(long waitTime, long leaseTime, TimeUnit unit) throws InterruptedException {
    long time = unit.toMillis(waitTime);
    long current = System.currentTimeMillis();
    long threadId = Thread.currentThread().getId();
    Long ttl = tryAcquire(waitTime, leaseTime, unit, threadId);
    // lock acquired
    if (ttl == null) {
        return true;
    }

    time -= System.currentTimeMillis() - current;
    if (time <= 0) {
        acquireFailed(waitTime, unit, threadId);
        return false;
    }

    current = System.currentTimeMillis();
    RFuture<RedissonLockEntry> subscribeFuture = subscribe(threadId);
    if (!subscribeFuture.await(time, TimeUnit.MILLISECONDS)) {
        if (!subscribeFuture.cancel(false)) {
            subscribeFuture.onComplete((res, e) -> {
                if (e == null) {
                    unsubscribe(subscribeFuture, threadId);
                }
            });
        }
        acquireFailed(waitTime, unit, threadId);
        return false;
    }

    try {
        time -= System.currentTimeMillis() - current;
        if (time <= 0) {
            acquireFailed(waitTime, unit, threadId);
            return false;
        }

        while (true) {
            long currentTime = System.currentTimeMillis();
            ttl = tryAcquire(waitTime, leaseTime, unit, threadId);
            // lock acquired
            if (ttl == null) {
                return true;
            }

            time -= System.currentTimeMillis() - currentTime;
            if (time <= 0) {
                acquireFailed(waitTime, unit, threadId);
                return false;
            }

            // waiting for message
            currentTime = System.currentTimeMillis();
            if (ttl >= 0 && ttl < time) {
                subscribeFuture.getNow().getLatch().tryAcquire(ttl, TimeUnit.MILLISECONDS);
            } else {
                subscribeFuture.getNow().getLatch().tryAcquire(time, TimeUnit.MILLISECONDS);
            }

            time -= System.currentTimeMillis() - currentTime;
            if (time <= 0) {
                acquireFailed(waitTime, unit, threadId);
                return false;
            }
        }
    } finally {
        unsubscribe(subscribeFuture, threadId);
    }
    //        return get(tryLockAsync(waitTime, leaseTime, unit));
}
```
上面这一段代码最主要的内容讲述看门狗机制的实际上应该算是 `**tryAcquire**`<br />最终落地为`**tryAcquireAsync**`
```java
//如果获取锁失败，返回的结果是这个key的剩余有效期
RFuture<Long> ttlRemainingFuture = this.tryLockInnerAsync(waitTime, this.commandExecutor.getConnectionManager().getCfg().getLockWatchdogTimeout(), TimeUnit.MILLISECONDS, threadId, RedisCommands.EVAL_LONG);
//上面获取锁回调成功之后，执行这代码块的内容
ttlRemainingFuture.onComplete((ttlRemaining, e) -> {
    //不存在异常
    if (e == null) {
        //剩余有效期为null
        if (ttlRemaining == null) {
            //这个函数是解决最长等待有效期的问题
            this.scheduleExpirationRenewal(threadId);
        }

    }
});
return ttlRemainingFuture;
```
调用`tryLockInnerAsync`，如果获取锁失败，返回的结果是这个key的剩余有效期，如果获取锁成功，则返回null。<br />获取锁成功后，如果检测不存在异常并且获取锁成功（ttlRemaining == null）。<br />那么则执行`this.scheduleExpirationRenewal(threadId);`来启动看门狗机制。<br />看门狗机制提供的默认超时时间是30*1000毫秒，也就是30秒<br />如果一个线程获取锁后，运行程序到释放锁所花费的时间大于锁自动释放时间（也就是看门狗机制提供的超时时间30s），那么Redission会自动给redis中的目标锁延长超时时间。<br />在Redission中想要启动看门狗机制，那么就不用获取锁的时候自己定义leaseTime(锁自动释放时间)。<br />但是 Redisson 和自己定义实现分布式锁不一样，如果自己定义了锁自动释放时间的话，无论是通过lock还是tryLock方法，都无法启用看门狗机制。<br />所以你了解分布式锁的看门狗机制了么？
