Java<br />最近在分析一个应用中的某个接口的耗时情况时，发现一个看起来极其普通的对象创建操作，竟然每次需要消耗 8ms 左右时间，分析后发现这个对象可以通过对象池模式进行优化，优化后此步耗时仅有 0.01ms，这篇文章介绍对象池相关知识。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1659678138309-56893124-6807-4564-b798-a89451915c7e.jpeg#clientId=u74156a95-3443-4&from=paste&id=ue50d8137&originHeight=223&originWidth=1000&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3005a2fc-5d8b-41f4-bad4-29ce04e03a3&title=)
<a name="ChStu"></a>
## 1、什么是对象池
池化并不是什么新鲜的技术，它更像一种软件设计模式，主要功能是缓存一组已经初始化的对象，以供随时可以使用。对象池大多数场景下都是缓存着创建成本过高或者需要重复创建使用的对象，从池子中取对象的时间是可以预测的，但是新建一个对象的时间是不确定的。<br />当需要一个新对象时，就向池中借出一个，然后对象池标记当前对象正在使用，使用完毕后归还到对象池，以便再次借出。<br />常见的使用对象池化场景：

1. 对象创建成本过高。
2. 需要频繁的创建大量重复对象，会产生很多内存碎片。
3. 同时使用的对象不会太多。
4. 常见的具体场景如数据库连接池、线程池等。
<a name="QQI0V"></a>
## 2、为什么需要对象池
如果一个对象的**创建成本很高**，比如建立数据库的连接时耗时过长，在不使用池化技术的情况下，查询过程可能是这样的。<br />查询 1：建立数据库连接 -> 发起查询 -> 收到响应 -> 关闭连接<br />查询 2：建立数据库连接 -> 发起查询 -> 收到响应 -> 关闭连接<br />查询 3：建立数据库连接 -> 发起查询 -> 收到响应 -> 关闭连接<br />在这种模式下，每次查询都要重新建立关闭连接，因为建立连接是一个耗时的操作，所以这种模式会影响程序的总体性能。<br />那么使用池化思想是怎么样的呢？同样的过程会转变成下面的步骤。<br />初始化：建立 N 个数据库连接 -> 缓存起来<br />查询 1：从缓存借到数据库连接 -> 发起查询 -> 收到响应 -> 归还数据库连接对象到缓存<br />查询 2：从缓存借到数据库连接 -> 发起查询 -> 收到响应 -> 归还数据库连接对象到缓存<br />查询 3：从缓存借到数据库连接 -> 发起查询 -> 收到响应 -> 归还数据库连接对象到缓存<br />使用池化思想后，数据库连接并不会频繁的创建关闭，而是启动后就初始化了 N 个连接以供后续使用，使用完毕后归还对象，这样程序的总体性能得到提升。
<a name="fGVJL"></a>
## 3、对象池的实现
通过上面的例子也可以发现池化思想的几个关键步骤：**初始化、借出、归还**。上面没有展示**销毁**步骤， 某些场景下还需要对象的销毁这一过程，比如释放连接。<br />下面手动实现一个简陋的对象池，加深下对对象池的理解。主要是定一个对象池管理类，然后在里面实现对象的**初始化、借出、归还、销毁**等操作。
```java
import java.io.Closeable;
import java.io.IOException;
import java.util.HashSet;
import java.util.Stack;

public class MyObjectPool<T extends Closeable> {

    // 池子大小
    private Integer size = 5;
    // 对象池栈。后进先出
    private Stack<T> stackPool = new Stack<>();
    // 借出的对象的 hashCode 集合
    private HashSet<Integer> borrowHashCodeSet = new HashSet<>();

    /**
     * 增加一个对象
     *
     * @param t
     */
    public synchronized void addObj(T t) {
        if ((stackPool.size() + borrowHashCodeSet.size()) == size) {
            throw new RuntimeException("池中对象已经达到最大值");
        }
        stackPool.add(t);
        System.out.println("添加了对象:" + t.hashCode());
    }

    /**
     * 借出一个对象
     *
     * @return
     */
    public synchronized T borrowObj() {
        if (stackPool.isEmpty()) {
            System.out.println("没有可以被借出的对象");
            return null;
        }
        T pop = stackPool.pop();
        borrowHashCodeSet.add(pop.hashCode());
        System.out.println("借出了对象:" + pop.hashCode());
        return pop;
    }

    /**
     * 归还一个对象
     *
     * @param t
     */
    public synchronized void returnObj(T t) {
        if (borrowHashCodeSet.contains(t.hashCode())) {
            stackPool.add(t);
            borrowHashCodeSet.remove(t.hashCode());
            System.out.println("归还了对象:" + t.hashCode());
            return;
        }
        throw new RuntimeException("只能归还从池中借出的对象");
    }

    /**
     * 销毁池中对象
     */
    public synchronized void destory() {
        if (!borrowHashCodeSet.isEmpty()) {
            throw new RuntimeException("尚有未归还的对象，不能关闭所有对象");
        }
        while (!stackPool.isEmpty()) {
            T pop = stackPool.pop();
            try {
                pop.close();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
        System.out.println("已经销毁了所有对象");
    }
}
```
代码还是比较简单的，只是简单的示例，下面通过池化一个 Redis 连接对象 Jedis 来演示如何使用。<br />其实 Jedis 中已经有对应的 Jedis 池化管理对象了 JedisPool 了，不过里为了演示对象池的实现，就不使用官方提供的 JedisPool 了。<br />启动一个 Redis 服务这里不做介绍，假设已经有了一个 Redis 服务，下面引入 Java 中连接 Redis 需要用到的 Maven 依赖。
```xml
<dependency>
	<groupId>redis.clients</groupId>
	<artifactId>jedis</artifactId>
	<version>4.2.0</version>
</dependency>
```
正常情况下 Jedis 对象的使用方式：
```java
Jedis jedis = new Jedis("localhost", 6379);
String name = jedis.get("name");
System.out.println(name);
jedis.close();
```
如果使用上面的对象池，就可以像下面这样使用。
```java
import redis.clients.jedis.Jedis;

/**
* @author niulang
* @date 2022/07/02
*/
public class MyObjectPoolTest {
	
	public static void main(String[] args) {
		MyObjectPool<Jedis> objectPool = new MyObjectPool<>();
		// 增加一个 jedis 连接对象
		objectPool.addObj(new Jedis("127.0.0.1", 6379));
		objectPool.addObj(new Jedis("127.0.0.1", 6379));
		// 从对象池中借出一个 jedis 对象
		Jedis jedis = objectPool.borrowObj();
		// 一次 redis 查询
		String name = jedis.get("name");
		System.out.println(String.format("redis get:" + name));
		// 归还 redis 连接对象
		objectPool.returnObj(jedis);
		// 销毁对象池中的所有对象
		objectPool.destory();
		// 再次借用对象
		objectPool.borrowObj();
	}
}
```
输出日志：
```xml
添加了对象:1556956098
添加了对象:1252585652
借出了对象:1252585652
redis get:www.yuque.com
归还了对象:1252585652
已经销毁了所有对象
没有可以被借出的对象
```
如果使用 JMH 对使用对象池化进行 Redis 查询，和正常创建 Redis 连接然后查询关闭连接的方式进行性能对比，会发现两者的性能差异很大。下面是测试结果，可以发现使用对象池化后的性能是非池化方式的 5 倍左右。
```xml
Benchmark                   Mode  Cnt      Score       Error  Units
MyObjectPoolTest.test      thrpt   15   2612.689 ±   358.767  ops/s
MyObjectPoolTest.testPool  thrpt    9  12414.228 ± 11669.484  ops/s
```
<a name="myCSb"></a>
## 4、开源的对象池工具
上面自己实现的对象池总归有些简陋了，其实开源工具中已经有了非常好用的对象池的实现，如 Apache 的 commons-pool2 工具，很多开源工具中的对象池都是基于此工具实现，下面介绍这个工具的使用方式。<br />maven 依赖：
```xml
<dependency>
	<groupId>org.apache.commons</groupId>
	<artifactId>commons-pool2</artifactId>
	<version>2.11.1</version>
</dependency>
```
在 commons-pool2 对象池工具中有几个关键的类。

- `PooledObjectFactory` 类是一个工厂接口，用于实现想要池化对象的创建、验证、销毁等操作。
- `GenericObjectPool` 类是一个通用的对象池管理类，可以进行对象的借出、归还等操作。
- `GenericObjectPoolConfig` 类是对象池的配置类，可以进行对象的最大、最小等容量信息进行配置。

下面通过一个具体的示例演示 commons-pool2 工具类的使用，这里依旧选择 Redis 连接对象 Jedis 作为演示。<br />实现 `PooledObjectFactory` 工厂类，实现其中的对象创建和销毁方法。
```java
public class MyPooledObjectFactory implements PooledObjectFactory<Jedis> {

    @Override
    public void activateObject(PooledObject<Jedis> pooledObject) throws Exception {

    }

    @Override
    public void destroyObject(PooledObject<Jedis> pooledObject) throws Exception {
        Jedis jedis = pooledObject.getObject();
        jedis.close();
          System.out.println("释放连接");
    }

    @Override
    public PooledObject<Jedis> makeObject() throws Exception {
        return new DefaultPooledObject(new Jedis("localhost", 6379));
    }

    @Override
    public void passivateObject(PooledObject<Jedis> pooledObject) throws Exception {
    }

    @Override
    public boolean validateObject(PooledObject<Jedis> pooledObject) {
        return false;
    }
}
```
继承 `GenericObjectPool` 类，实现对对象的借出、归还等操作。
```java
public class MyGenericObjectPool extends GenericObjectPool<Jedis> {

    public MyGenericObjectPool(PooledObjectFactory factory) {
        super(factory);
    }

    public MyGenericObjectPool(PooledObjectFactory factory, GenericObjectPoolConfig config) {
        super(factory, config);
    }

    public MyGenericObjectPool(PooledObjectFactory factory, GenericObjectPoolConfig config,
        AbandonedConfig abandonedConfig) {
        super(factory, config, abandonedConfig);
    }
}
```
可以看到 `MyGenericObjectPool` 类的构造函数中的入参有 `GenericObjectPoolConfig` 对象，这是个对象池的配置对象，可以配置对象池的容量大小等信息，这里就不配置了，使用默认配置。<br />通过 `GenericObjectPoolConfig` 的源码可以看到**默认配置中，对象池的容量是 8 个**。
```java
public class GenericObjectPoolConfig<T> extends BaseObjectPoolConfig<T> {

	/**
     * The default value for the {@code maxTotal} configuration attribute.
     * @see GenericObjectPool#getMaxTotal()
     */
	public static final int DEFAULT_MAX_TOTAL = 8;

	/**
     * The default value for the {@code maxIdle} configuration attribute.
     * @see GenericObjectPool#getMaxIdle()
     */
	public static final int DEFAULT_MAX_IDLE = 8;
```
下面编写一个对象池使用测试类。
```java
public class ApachePool {

    public static void main(String[] args) throws Exception {
        MyGenericObjectPool objectMyObjectPool = new MyGenericObjectPool(new MyPooledObjectFactory());
        Jedis jedis = objectMyObjectPool.borrowObject();
        String name = jedis.get("name");
        System.out.println(name);
        objectMyObjectPool.returnObject(jedis);
        objectMyObjectPool.close();
    }

}
```
输出日志：
```java
redis get:www.yuque.com
释放连接
```
上面已经演示了 commons-pool2 工具中的对象池的使用方式，从上面的例子中可以发现这种对象池中只能存放同一种初始化条件的对象，如果这里的 Redis 需要存储一个本地连接和一个远程连接的两种 Jedis 对象，就不能满足了。那么怎么办呢？<br />其实 commons-pool2 工具已经考虑到了这种情况，通过增加一个 key 值可以在同一个对象池管理中进行区分，代码和上面类似，直接贴出完整的代码实现。
```java
import org.apache.commons.pool2.BaseKeyedPooledObjectFactory;
import org.apache.commons.pool2.KeyedPooledObjectFactory;
import org.apache.commons.pool2.PooledObject;
import org.apache.commons.pool2.impl.AbandonedConfig;
import org.apache.commons.pool2.impl.DefaultPooledObject;
import org.apache.commons.pool2.impl.GenericKeyedObjectPool;
import org.apache.commons.pool2.impl.GenericKeyedObjectPoolConfig;
import redis.clients.jedis.Jedis;

public class ApacheKeyedPool {

    public static void main(String[] args) throws Exception {
        String key = "local";
        MyGenericKeyedObjectPool objectMyObjectPool = new MyGenericKeyedObjectPool(new MyKeyedPooledObjectFactory());
        Jedis jedis = objectMyObjectPool.borrowObject(key);
        String name = jedis.get("name");
        System.out.println("redis get :" + name);
        objectMyObjectPool.returnObject(key, jedis);
    }
}

class MyKeyedPooledObjectFactory extends BaseKeyedPooledObjectFactory<String, Jedis> {

    @Override
    public Jedis create(String key) throws Exception {
        if ("local".equals(key)) {
            return new Jedis("localhost", 6379);
        }
        if ("remote".equals(key)) {
            return new Jedis("192.168.0.105", 6379);
        }
        return null;
    }

    @Override
    public PooledObject<Jedis> wrap(Jedis value) {
        return new DefaultPooledObject<>(value);
    }
}

class MyGenericKeyedObjectPool extends GenericKeyedObjectPool<String, Jedis> {

    public MyGenericKeyedObjectPool(KeyedPooledObjectFactory<String, Jedis> factory) {
        super(factory);
    }

    public MyGenericKeyedObjectPool(KeyedPooledObjectFactory<String, Jedis> factory,
        GenericKeyedObjectPoolConfig<Jedis> config) {
        super(factory, config);
    }

    public MyGenericKeyedObjectPool(KeyedPooledObjectFactory<String, Jedis> factory,
        GenericKeyedObjectPoolConfig<Jedis> config, AbandonedConfig abandonedConfig) {
        super(factory, config, abandonedConfig);
    }
}
```
输出日志：
```java
redis get :www.yuque.com
```
<a name="DSti0"></a>
## 5、JedisPool 对象池实现分析
这篇文章中的演示都使用了 Jedis 连接对象，其实在 Jedis SDK 中已经实现了相应的对象池，也就是常用的 JedisPool 类。那么这里的 JedisPool 是怎么实现的呢？先看一下 JedisPool 的使用方式。
```java
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

public class JedisPoolTest {

    public static void main(String[] args) {
        JedisPool jedisPool = new JedisPool("localhost", 6379);
        // 从对象池中借一个对象
        Jedis jedis = jedisPool.getResource();
        String name = jedis.get("name");
        System.out.println("redis get :" + name);
        jedis.close();
        // 彻底退出前，关闭 Redis 连接池
        jedisPool.close();
    }
}
```
代码中添加了注释，可以看到通过 `jedisPool.getResource()` 拿到了一个对象，这里和上面 commons-pool2 工具中的 `borrowObject` 十分相似，继续追踪它的代码实现可以看到下面的代码。
```java
// redis.clients.jedis.JedisPool
// public class JedisPool extends Pool<Jedis> {
public Jedis getResource() {
    Jedis jedis = (Jedis)super.getResource();
    jedis.setDataSource(this);
    return jedis;
}
// 继续追踪 super.getResource()
// redis.clients.jedis.util.Pool
public T getResource() {
    try {
        return super.borrowObject();
    } catch (JedisException var2) {
        throw var2;
    } catch (Exception var3) {
        throw new JedisException("Could not get a resource from the pool", var3);
    }
}
```
竟然看到了 `super.borrowObject()` ，多么熟悉的方法，继续分析代码可以发现 Jedis 对象池也是使用了 commons-pool2 工具作为实现。既然如此，那么 `jedis.close()` 方法的逻辑应该也可以猜到了，应该有一个归还的操作，查看代码发现果然如此。
```java
// redis.clients.jedis.JedisPool
// public class JedisPool extends Pool<Jedis> {
public void close() {
	if (this.dataSource != null) {
		Pool<Jedis> pool = this.dataSource;
		this.dataSource = null;
		if (this.isBroken()) {
			pool.returnBrokenResource(this);
		} else {
			pool.returnResource(this);
		}
	} else {
		this.connection.close();
	}
}
// 继续追踪 super.getResource()
// redis.clients.jedis.util.Pool
public void returnResource(T resource) {
	if (resource != null) {
		try {
			super.returnObject(resource);
		} catch (RuntimeException var3) {
			throw new JedisException("Could not return the resource to the pool", var3);
		}
	}
}
```
通过上面的分析，可见 Jedis 确实使用了 commons-pool2 工具进行对象池的管理，通过分析 JedisPool 类的继承关系图也可以发现。<br />![JedisPool 继承关系](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1659678138310-a1159d2b-db4c-4cc1-b8c4-5e24c4a49804.jpeg#clientId=u74156a95-3443-4&from=paste&id=u140687fb&originHeight=452&originWidth=1000&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u14502a90-7474-4ea7-9916-3e04199cf08&title=JedisPool%20%E7%BB%A7%E6%89%BF%E5%85%B3%E7%B3%BB "JedisPool 继承关系")
<a name="yCVi3"></a>
## 6、对象池总结
通过介绍，可以发现池化思想有几个明显的优势。

1. 可以显著的提高应用程序的性能。
2. 如果一个对象创建成本过高，那么使用池化非常有效。
3. 池化提供了一种对象的管理以及重复使用的方式，减少内存碎片。
4. 可以为对象的创建数量提供限制，对某些对象不能创建过多的场景提供保护。

但是使用对象池化也有一些需要注意的地方，比如归还对象时应确保对象已经被重置为可以重复使用的状态。同时也要注意，使用池化时要根据具体的场景**合理的设置池子的大小**，过小达不到想要的效果，过大会造成内存浪费。
