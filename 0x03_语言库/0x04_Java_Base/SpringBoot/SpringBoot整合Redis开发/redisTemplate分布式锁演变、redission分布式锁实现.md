Java Redis
<a name="S4V2s"></a>
## 基本原理
可以同时去一个地方“占坑”，如果占到，就执行逻辑。否则就必须等待，直到释放锁。“占坑”可以去redis，可以去数据库，可以去任何大家都能访问的地方。等待可以自旋的方式。
<a name="MBQFT"></a>
## 阶段一
![2021-07-15-23-25-29-308517.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1626362863223-79d3710e-5cf1-462a-9ff1-99fb17b8e6d4.png#clientId=u90dbd1a9-2795-4&from=ui&id=u7252b540&originHeight=781&originWidth=1080&originalType=binary&ratio=1&size=2535398&status=done&style=shadow&taskId=u33556db8-d0ce-4e78-a72f-bc34306ab2a)
```java
public Map<String, List<Catalog2Vo>> getCatalogJsonDbWithRedisLock() {
    //阶段一
    Boolean lock = stringRedisTemplate.opsForValue().setIfAbsent("lock", "111");
    //获取到锁，执行业务
    if (lock) {
        Map<String, List<Catalog2Vo>> categoriesDb = getCategoryMap();
        //删除锁，如果在此之前报错或宕机会造成死锁
        stringRedisTemplate.delete("lock");
        return categoriesDb;
    }else {
        //没获取到锁，等待100ms重试
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return getCatalogJsonDbWithRedisLock();
    }
}

public Map<String, List<Catalog2Vo>> getCategoryMap() {
    ValueOperations<String, String> ops = stringRedisTemplate.opsForValue();
    String catalogJson = ops.get("catalogJson");
    if (StringUtils.isEmpty(catalogJson)) {
        System.out.println("缓存不命中，准备查询数据库。。。");
        Map<String, List<Catalog2Vo>> categoriesDb= getCategoriesDb();
        String toJSONString = JSON.toJSONString(categoriesDb);
        ops.set("catalogJson", toJSONString);
        return categoriesDb;
    }
    System.out.println("缓存命中。。。。");
    Map<String, List<Catalog2Vo>> listMap = JSON.parseObject(catalogJson, new TypeReference<Map<String, List<Catalog2Vo>>>() {});
    return listMap;
}
```
问题： setnx占好了位，业务代码异常或者程序在页面过程中宕机。没有执行删除锁逻辑，这就造成了死锁<br />解决： 设置锁的自动过期，即使没有删除，会自动删除
<a name="GxAiJ"></a>
## 阶段二
![2021-07-15-23-25-29-458515.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1626362874806-3c53a991-fb00-4b65-a3c8-3469ffe5b93f.png#clientId=u90dbd1a9-2795-4&from=ui&id=u8e0d603d&originHeight=683&originWidth=1080&originalType=binary&ratio=1&size=2217262&status=done&style=shadow&taskId=u5825bca3-701a-48ba-9177-0f48b26c256)
```java
public Map<String, List<Catalog2Vo>> getCatalogJsonDbWithRedisLock() {
    Boolean lock = stringRedisTemplate.opsForValue().setIfAbsent("lock", "111");
    if (lock) {
        //设置过期时间
        stringRedisTemplate.expire("lock", 30, TimeUnit.SECONDS);
        Map<String, List<Catalog2Vo>> categoriesDb = getCategoryMap();
        stringRedisTemplate.delete("lock");
        return categoriesDb;
    }else {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return getCatalogJsonDbWithRedisLock();
    }
}
```
问题： setnx设置好，正要去设置过期时间，宕机。又死锁了。<br />解决： 设置过期时间和占位必须是原子的。[redis支持使用setnx ex命令](http://mp.weixin.qq.com/s?__biz=MzU2MTI4MjI0MQ==&mid=2247501932&idx=3&sn=b106c21ba2596b49189c7d1ad27a0936&chksm=fc79a5c2cb0e2cd425ef9353030d606d71c0d0d9b9e520b9216c59247612c76b0cd3211a8c4a&scene=21#wechat_redirect)
<a name="l64Oc"></a>
## 阶段三
![2021-07-15-23-25-29-633523.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1626362889568-9af5f24d-0b83-4d97-9325-248fe055b30f.png#clientId=u90dbd1a9-2795-4&from=ui&id=u0b74674e&originHeight=872&originWidth=1080&originalType=binary&ratio=1&size=2830806&status=done&style=shadow&taskId=ue2c21bd2-66cb-411c-adbf-e33262dba8d)
```java
public Map<String, List<Catalog2Vo>> getCatalogJsonDbWithRedisLock() {
    //加锁的同时设置过期时间，二者是原子性操作
    Boolean lock = stringRedisTemplate.opsForValue().setIfAbsent("lock", "1111",5, TimeUnit.SECONDS);
    if (lock) {
        Map<String, List<Catalog2Vo>> categoriesDb = getCategoryMap();
        //模拟超长的业务执行时间
        try {
            Thread.sleep(6000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        stringRedisTemplate.delete("lock");
        return categoriesDb;
    }else {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return getCatalogJsonDbWithRedisLock();
    }
}
```
问题： 删除锁直接删除？？？如果由于业务时间很长，锁自己过期了，直接删除，有可能把别人正在持有的锁删除了。<br />解决： 占锁的时候，值指定为uuid，每个人匹配是自己的锁才删除。
<a name="WIUZ6"></a>
## 阶段四
![2021-07-15-23-25-29-816064.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1626362900207-78234bea-9c0a-4a61-95ba-6f5afba0aa1d.png#clientId=u90dbd1a9-2795-4&from=ui&id=u0266e001&originHeight=757&originWidth=1080&originalType=binary&ratio=1&size=2457479&status=done&style=shadow&taskId=u4b05fca0-db75-45af-b160-8d5efae9ba0)
```java
public Map<String, List<Catalog2Vo>> getCatalogJsonDbWithRedisLock() {
    String uuid = UUID.randomUUID().toString();
    ValueOperations<String, String> ops = stringRedisTemplate.opsForValue();
    //为当前锁设置唯一的uuid，只有当uuid相同时才会进行删除锁的操作
    Boolean lock = ops.setIfAbsent("lock", uuid,5, TimeUnit.SECONDS);
    if (lock) {
        Map<String, List<Catalog2Vo>> categoriesDb = getCategoryMap();
        String lockValue = ops.get("lock");
        if (lockValue.equals(uuid)) {
            try {
                Thread.sleep(6000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            stringRedisTemplate.delete("lock");
        }
        return categoriesDb;
    }else {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return getCatalogJsonDbWithRedisLock();
    }
}
```
问题： 如果正好判断是当前值，正要删除锁的时候，锁已经过期，别人已经设置到了新的值。那么删除的是别人的锁<br />解决： 删除锁必须保证原子性。使用redis+Lua脚本完成
<a name="WfVdJ"></a>
## 阶段五-最终形态
![2021-07-15-23-25-29-992612.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1626362910053-4024859f-3893-4cf7-8384-a12aeaf71823.png#clientId=u90dbd1a9-2795-4&from=ui&id=u14430e30&originHeight=581&originWidth=1080&originalType=binary&ratio=1&size=1886138&status=done&style=shadow&taskId=u8562b700-8a26-45c6-867c-eb9fe661e1d)
```java
public Map<String, List<Catalog2Vo>> getCatalogJsonDbWithRedisLock() {
    String uuid = UUID.randomUUID().toString();
    ValueOperations<String, String> ops = stringRedisTemplate.opsForValue();
    Boolean lock = ops.setIfAbsent("lock", uuid,5, TimeUnit.SECONDS);
    if (lock) {
        Map<String, List<Catalog2Vo>> categoriesDb = getCategoryMap();
        String lockValue = ops.get("lock");
        String script = "if redis.call(\"get\",KEYS[1]) == ARGV[1] then\n" +
            "    return redis.call(\"del\",KEYS[1])\n" +
            "else\n" +
            "    return 0\n" +
            "end";
        stringRedisTemplate.execute(new DefaultRedisScript<Long>(script, Long.class), Arrays.asList("lock"), lockValue);
        return categoriesDb;
    }else {
        try {
            Thread.sleep(100);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return getCatalogJsonDbWithRedisLock();
    }
}
```
保证加锁【占位+过期时间】和删除锁【判断+删除】的原子性。更难的事情，锁的自动续期
<a name="q2ICr"></a>
## Redisson
Redisson是一个在Redis的基础上实现的Java驻内存数据网格（In-Memory Data Grid）。它不仅提供了一系列的分布式的Java常用对象，还提供了许多分布式服务。
> 其中包括(BitSet, Set, Multimap, SortedSet, Map, List, Queue, BlockingQueue, Deque, BlockingDeque, Semaphore, Lock, AtomicLong, CountDownLatch, Publish / Subscribe, Bloom filter, Remote service, Spring cache, Executor service, Live Object service, Scheduler service)

Redisson提供了使用Redis的最简单和最便捷的方法。Redisson的宗旨是促进使用者对Redis的关注分离（Separation of Concern），从而让使用者能够将精力更集中地放在处理业务逻辑上。
