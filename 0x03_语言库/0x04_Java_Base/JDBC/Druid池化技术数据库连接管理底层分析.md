Druid 池华技术
<a name="isfMR"></a>
## 零、类图&流程预览
通过`getConnection`作为入口，探索在`druid`里，一个连接的生命周期。大体流程被划分成了以下几个主流程：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005578-e1ffe10d-375e-4fc3-88a2-0688d115d26a.png#align=left&display=inline&height=315&originHeight=315&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="DqvvE"></a>
## 一、主流程1：获取连接流程
首先从入口来看看它在获取连接时做了哪些操作：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005548-ca2d9649-f478-4d16-b45b-b606d2ec7b7a.png#align=left&display=inline&height=984&originHeight=984&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />上述为获取连接时的流程图，首先会调用`init`进行连接池的初始化，然后运行责任链上的每一个`filter`，最终执行`getConnectionDirect`获取真正的连接对象，如果开启了`testOnBorrow`，则每次都会去测试连接是否可用（这也是官方不建议设置`testOnBorrow`为`true`的原因，影响性能，这里的测试是指测试MySQL服务端的长连接是否断开，一般MySQL服务端长连保活时间是`8h`，被使用一次则刷新一次使用时间，若一个连接距离上次被使用超过了保活时间，那么再次使用时将无法与MySQL服务端通信）。<br />如果`testOnBorrow`没有被置为`true`，则会进行`testWhileIdle`的检查（这一项官方建议设置为true，缺省值也是true），检查时会判断当前连接对象距离上次被使用的时间是否超过规定检查的时间，若超过，则进行检查一次，这个检查时间通过`timeBetweenEvictionRunsMillis`来控制，默认60s。<br />每个连接对象会记录下上次被使用的时间，用当前时间减去上一次的使用时间得出闲置时间，闲置时间再跟`timeBetweenEvictionRunsMillis`比较，超过这个时间就做一次连接可用性检查，这个相比`testOnBorrow`每次都检查来说，性能会提升很多，用的时候无需关注该值，因为缺省值是`true`，经测试如果将该值设置为false，`testOnBorrow`也设置为`false`，数据库服务端长连保活时间改为60s，60s内不使用连接，超过60s后使用将会报连接错误。<br />若使用`testConnectionInternal`方法测试长连接结果为`false`，则证明该连接已被服务端断开或者有其他的网络原因导致该连接不可用，则会触发`discardConnection`进行连接回收（对应流程1.4，因为丢弃了一个连接，因此该方法会唤醒主流程3进行检查是否需要新建连接）。整个流程运行在一个死循环内，直到取到可用连接或者超过重试上限报错退出（在连接没有超过连接池上限的话，最多重试一次（重试次数默认重试1次，可以通过`notFullTimeoutRetryCount`属性来控制），所以取连接这里一旦发生等待，在连接池没有满的情况下，最大等待 `2 × maxWait` 的时间 ←这个有待验证）。
<a name="5rIvT"></a>
### 特别说明①
为了保证性能，不建议将`testOnBorrow`设置为`true`，或者说牵扯到长连接可用检测的那几项配置使用druid默认的配置就可以保证性能是最好的，如上所说，默认长连接检查是60s一次，所以不启用`testOnBorrow`的情况下要想保证万无一失，自己要确认下所连的那个MySQL服务端的长连接保活时间（虽然默认是8h，但是dba可能给测试环境设置的时间远小于这个时间，所以如果这个时间小于60s，就需要手动设置`timeBetweenEvictionRunsMillis`了，如果MySQL服务端长连接时间是8h或者更长，则用默认值即可。
<a name="18EeD"></a>
### 特别说明②
为了防止不必要的扩容，在MySQL服务端长连接够用的情况下，对于一些qps较高的服务、网关业务，建议把池子的最小闲置连接数`minIdle`和最大连接数`maxActive`设置成一样的，且按照需要调大，且开启`keepAlive`进行连接活性检查（参考流程4.1），这样就不会后期发生动态新建连接的情况（建连还是个比较重的操作，所以不如一开始就申请好所有需要的连接，个人意见，仅供参考），但是像管理后台这种，长期qps非常低，但是有的时候需要用管理后台做一些巨大的操作（比如导数据什么的）导致需要的连接暴增，且管理后台不会特别要求性能，就适合将`minIdle`的值设置的比`maxActive`小，这样不会造成不必要的连接浪费，也不会在需要暴增连接的时候无法动态扩增连接。
<a name="apRxA"></a>
## 二、主流程2：初始化连接池
通过上面的流程图可以看到，在获取一个连接的时候首先会检查连接池是否已经初始化完毕（通过`inited`来控制，bool类型，未初始化为`flase`，初始化完毕为`true`，这个判断过程在`init`方法内完成），若没有初始化，则调用`init`进行初始化（图主流程1中的紫色部分），下面来看看`init`方法里又做了哪些操作：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005529-052ad4f9-e96f-4211-bfb0-1b35ac293967.png#align=left&display=inline&height=1239&originHeight=1239&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />可以看到，实例化的时候会初始化全局的重入锁`lock`，在初始化过程中包括后续的连接池操作都会利用该锁保证线程安全，初始化连接池的时候首先会进行双重检查是否已经初始化过，若没有，则进行连接池的初始化，这时候还会通过SPI机制额外加载责任链上的filter。<br />但是这类`filter`需要在类上加上`@AutoLoad`注解。然后初始化了三个数组，容积都为`maxActive`，首先`connections`就是用来存放池子里连接对象的，`evictConnections`用来存放每次检查需要抛弃的连接（结合流程4.1理解），`keepAliveConnections`用于存放需要连接检查的存活连接（同样结合流程4.1理解），然后生成初始化数（`initialSize`）个连接，放进`connections`，然后生成两个必须的守护线程，用来添加连接进池以及从池子里摘除不需要的连接，这俩过程较复杂，因此拆出来单说（主流程3和主流程4）。
<a name="XjMUo"></a>
### 特别说明①
从流程上看如果一开始实例化的时候不对连接池进行初始化（这个初始化是指对池子本身的初始化，并非单纯的指druid对象属性的初始化），那么在第一次调用`getConnection`时就会走上图那么多逻辑，尤其是耗时较久的建立连接操作，被重复执行了很多次，导致第一次`getConnection`时耗时过久，如果程序并发量很大，那么第一次获取连接时就会因为初始化流程而发生排队，所以建议在实例化连接池后对其进行预热，通过调用`init`方法或者`getConnection`方法都可以。
<a name="bfia0"></a>
### 特别说明②
在构建全局重入锁的时候，利用lock对象生成了俩Condition，对这俩Condition解释如下：
> 当连接池连接够用时，利用empty阻塞添加连接的守护线程（主流程3），当连接池连接不够用时，获取连接的那个线程（这里记为业务线程A）就会阻塞在notEmpty上，且唤起阻塞在empty上的添加连接的守护线程，走完添加连接的流程，走完后会重新唤起阻塞在notEmpty上的业务线程A，业务线程A就会继续尝试获取连接。

<a name="rDlO2"></a>
## 三、流程1.1：责任链
WARN：这块东西结合源码看更容易理解<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005564-48840e6f-0910-42ff-806f-3498b1e04fcf.png#align=left&display=inline&height=930&originHeight=930&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />这里对应流程1里获取连接时需要执行的责任链，每个`DruidAbstractDataSource`里都包含`filters`属性，`filters`是对`Druid`里`Filters`接口的实现，里面有很多对应着连接池里的映射方法，比如例子中`dataSource`的`getConnection`方法在触发的时候就会利用`FilterChain`把每个`filter`里的`dataSource_getConnection`给执行一遍，这里也要说明下`FilterChain`，通过流程1.1可以看出来，`datasource`是利用`FilterChain`来触发各个`filter`的执行的，`FilterChain`里也有一堆`datasource`里的映射方法，比如上图里的`dataSource_connect`，这个方法会把datasource里的`filters`全部执行一遍直到`nextFilter`取不到值，才会触发`dataSource.getConnectionDirect`，这个结合代码会比较容易理解。
<a name="3Iex7"></a>
## 四、流程1.2：从池中获取连接的流程
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005582-6f413963-64b9-43cb-9aee-2ff280663615.png#align=left&display=inline&height=595&originHeight=595&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />通过`getConnectionInternal`方法从池子里获取真正的连接对象，druid支持两种方式新增连接，一种是通过开启不同的守护线程通过`await`、`signal`通信实现（本文启用的方式，也是默认的方式），另一种是直接通过线程池异步新增，这个方式通过在初始化druid时传入`asyncInit=true`，再把一个线程池对象赋值给`createScheduler`，就成功启用了这种模式，没仔细研究这种方式，所以本文的流程图和代码块都会规避这个模式。<br />上面的流程很简单，连接足够时就直接`poolingCount-1`，数组取值，返回，`activeCount+1`，整体复杂度为O(1)，关键还是看取不到连接时的做法，取不到连接时，druid会先唤起新增连接的守护线程新增连接，然后陷入等待状态，然后唤醒该等待的点有两处，一个是用完了连接recycle（主流程5）进池子后触发，另外一个就是新增连接的守护线程成功新增了一个连接后触发，await被唤起后继续加入锁竞争，然后往下走如果发现池子里的连接数仍然是0（说明在唤醒后参与锁竞争里刚被放进来的连接又被别的线程拿去了），则继续下一次的await，这里采用的是`awaitNanos`方法，初始值是`maxWait`，然后下次被刷新后就是`maxWait`减去上次阻塞花费的实际时间，每次await的时间会逐步减少，直到归零，整体时间是约等于maxWait的，但实际比`maxActive`要大，因为程序本身存在耗时以及被唤醒后又要参与锁竞争导致也存在一定的耗时。<br />如果最终都没办法拿到连接则返回null出去，紧接着触发主流程1中的重试逻辑。
<a name="ip544"></a>
### druid如何防止在获取不到连接时阻塞过多的业务线程？
通过上面的流程图和流程描述，如果非常极端的情况，池子里的连接完全不够用时，会阻塞过多的业务线程，甚至会阻塞超过`maxWait`这么久，有没有一种措施是可以在连接不够用的时候控制阻塞线程的个数，超过这个限制后直接报错，而不是陷入等待呢？<br />druid其实支持这种策略的，在`maxWaitThreadCount`属性为默认值（-1）的情况下不启用，如果`maxWaitThreadCount`配置大于0，表示启用，这是druid做的一种丢弃措施，如果不希望在池子里的连接完全不够用导阻塞的业务线程过多，就可以考虑配置该项，这个属性的意思是说在连接不够用时最多让多少个业务线程发生阻塞，流程1.2的图里没有体现这个开关的用途，可以在代码里查看，每次在`pollLast`方法里陷入等待前会把属性`notEmptyWaitThreadCount`进行累加，阻塞结束后会递减，由此可见`notEmptyWaitThreadCount`就是表示当前等待可用连接时阻塞的业务线程的总个数，而`getConnectionInternal`在每次调用`pollLast`前都会判断这样一段代码：
```java
if (maxWaitThreadCount > 0 && notEmptyWaitThreadCount >= maxWaitThreadCount) {
    connectErrorCountUpdater.incrementAndGet(this);
    throw new SQLException("maxWaitThreadCount " + maxWaitThreadCount + ", current wait Thread count "
                           + lock.getQueueLength()); //直接抛异常，而不是陷入等待状态阻塞业务线程
}
```
可以看到，如果配置了`maxWaitThreadCount`所限制的等待线程个数，那么会直接判断当前陷入等待的业务线程是否超过了`maxWaitThreadCount`，一旦超过甚至不触发`pollLast`的调用（防止新增等待线程），直接抛错。<br />一般情况下不需要启用该项，一定要启用建议考虑好`maxWaitThreadCount`的取值，一般来说发生大量等待说明代码里存在不合理的地方：比如典型的连接池基本配置不合理，高qps的系统里`maxActive`配置过小；比如借出去的连接没有及时`close`归还；比如存在慢查询或者慢事务导致连接借出时间过久。这些要比配置`maxWaitThreadCount`更值得优先考虑，当然配置这个做一个极限保护也是没问题的，只是要结合实际情况考虑好取值。
<a name="psDlC"></a>
## 五、流程1.3：连接可用性测试
<a name="78Gsg"></a>
### ①init-checker
先了解下如何初始化检测连接用的checker，整个流程参考下图：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005653-a532ccd6-e8d8-4f33-9451-61e7c462c431.png#align=left&display=inline&height=606&originHeight=606&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />初始化`checker`发生在`init`阶段（没有在主流程2（init阶段）里体现出来，只需要记住初始化checker也是发生在`init`阶段就好），druid支持多种数据库的连接源，所以checker针对不同的驱动程序都做了适配，所以才看到图中checker有不同的实现，我们根据加载到的驱动类名匹配不同的数据库checker，上图匹配至MySQL的checker，checker的初始化里做了一件事情，就是判断驱动内是否有ping方法（jdbc4开始支持，mysql-connector-java早在3.x的版本就有ping方法的实现了），如果有，则把`usePingMethod`置为`true`，用于后续启用checker时做判断用（这里置为`true`，则通过反射的方式调用驱动程序的ping方法，如果为false，则触发普通的`SELECT 1`查询检测，SELECT 1就是我们非常熟悉的那个东西啦，新建statement，然后执行`SELECT 1`，然后再判断连接是否可用）。
<a name="vJawo"></a>
### ②testConnectionInternal
然后回到本节探讨的方法：流程1.3对应的testConnectionInternal<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005577-bec48b25-9c81-44ef-aff1-2b252deed875.png#align=left&display=inline&height=511&originHeight=511&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />这个方法会利用主流程2（init阶段）里初始化好的checker对象（流程参考init-checker）里的`isValidConnection`方法，如果启用ping，则该方法会利用invoke触发驱动程序里的ping方法，如果不启用ping，就采用`SELECT 1`方式（从init-checker里可以看出启不启用取决于加载到的驱动程序里是否存在相应的方法）。
<a name="BIAY7"></a>
## 六、流程1.4：抛弃连接
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005657-22874cca-a11b-4eb1-92c3-07752c0bac87.png#align=left&display=inline&height=902&originHeight=902&originWidth=990&size=0&status=done&style=shadow&width=990)<br />经过流程1.3返回的测试结果，如果发现连接不可用，则直接触发抛弃连接逻辑，这个过程非常简单，如上图所示，由流程1.2获取到该连接时累加上去的`activeCount`，在本流程里会再次减一，表示被取出来的连接不可用，并不能active状态。其次这里的close是拿着驱动那个连接对象进行close，正常情况下一个连接对象会被druid封装成`DruidPooledConnection`对象，内部持有的conn就是真正的驱动`Connection`对象，上图中的关闭连接就是获取的该对象进行close，如果使用包装类`DruidPooledConnection`进行close，则代表回收连接对象（`recycle`，参考主流程5）。
<a name="iiuP4"></a>
## 七、主流程3：添加连接的守护线程
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005669-bf028ade-23c2-4759-aa8f-b2377544a149.png#align=left&display=inline&height=793&originHeight=793&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />在主流程2（init初始化阶段）时就开启了该流程，该流程独立运行，大部分时间处于等待状态，不会抢占cpu，但是当连接不够用时，就会被唤起追加连接，成功创建连接后将会唤醒其他正在等待获取可用连接的线程，比如：
> 结合流程1.2来看，当连接不够用时，会通过`empty.signal`唤醒该线程进行补充连接（阻塞在empty上的线程只有主流程3的单线程），然后通过`notEmpty`阻塞自己，当该线程补充连接成功后，又会对阻塞在`notEmpty`上的线程进行唤醒，让其进入锁竞争状态，简单理解就是一个生产-消费模型。这里有一些细节，比如池子里的连接使用中（activeCount）加上池子里剩余连接数（poolingCount）就是指当前一共生成了多少个连接，这个数不能比`maxActive`还大，如果比maxActive还大，则再次陷入等待。而在往池子里put连接时，则判断`poolingCount`是否大于`maxActive`来决定最终是否入池。

<a name="UTX5Y"></a>
## 八、主流程4：抛弃连接的守护线程
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005661-a3c4f10c-8605-40b1-8521-7e52bd635d16.png#align=left&display=inline&height=915&originHeight=915&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="R0LdM"></a>
### 流程4.1：连接池瘦身，检查连接是否可用以及丢弃多余连接
整个过程如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005715-a15c9f1a-d721-472b-9f7d-af16aaea545e.png#align=left&display=inline&height=2046&originHeight=2046&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />整个流程分成图中主要的几步，首先利用`poolingCount`减去`minIdle`计算出需要做丢弃检查的连接对象区间，意味着这个区间的对象有被丢弃的可能，具体要不要放进丢弃队列`evictConnections`，要判断两个属性：<br />`minEvictableIdleTimeMillis`：最小检查间隙，缺省值30min，官方解释：一个连接在池中最小生存的时间（结合检查区间来看，闲置时间超过这个时间，才会被丢弃）。<br />`maxEvictableIdleTimeMillis`：最大检查间隙，缺省值7h，官方解释：一个连接在池中最大生存的时间（无视检查区间，只要闲置时间超过这个时间，就一定会被丢弃）。<br />如果当前连接对象闲置时间超过`minEvictableIdleTimeMillis`且下标在`evictCheck`区间内，则加入丢弃队列`evictConnections`，如果闲置时间超过`maxEvictableIdleTimeMillis`，则直接放入`evictConnections`（一般情况下会命中第一个判断条件，除非一个连接不在检查区间，且闲置时间超过`maxEvictableIdleTimeMillis`）。<br />如果连接对象不在evictCheck区间内，且keepAlive属性为true，则判断该对象闲置时间是否超出`keepAliveBetweenTimeMillis`（缺省值60s），若超出，则意味着该连接需要进行连接可用性检查，则将该对象放入`keepAliveConnections`队列。<br />两个队列赋值完成后，则池子会进行一次压缩，没有涉及到的连接对象会被压缩到队首。<br />然后就是处理`evictConnections`和`keepAliveConnections`两个队列了，`evictConnections`里的对象会被`close`最后释放掉，`keepAliveConnections`里面的对象将会其进行检测（流程参考流程1.3的`isValidConnection`），碰到不可用的连接会调用`discard`（流程1.4）抛弃掉，可用的连接会再次被放进连接池。<br />整个流程可以看出，连接闲置后，也并非一下子就减少到`minIdle`的，如果之前产生一堆的连接（不超过`maxActive`），突然闲置了下来，则至少需要花`minEvictableIdleTimeMillis`的时间才可以被移出连接池，如果一个连接闲置时间超过`maxEvictableIdleTimeMillis`则必定被回收，所以极端情况下（比如一个连接池从初始化后就没有再被使用过），连接池里并不会一直保持`minIdle`个连接，而是一个都没有，生产环境下这是非常不常见的，默认的`maxEvictableIdleTimeMillis`都有7h，除非是极度冷门的系统才会出现这种情况，而开启keepAlive也不会推翻这个规则，`keepAlive`的优先级是低于`maxEvictableIdleTimeMillis`的，`keepAlive`只是保证了那些检查中不需要被移出连接池的连接在指定检测时间内去检测其连接活性，从而决定是否放入池子或者直接`discard`。
<a name="CEiFK"></a>
### 流程4.2：主动回收连接，防止内存泄漏
过程如下：![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005670-a162600c-459d-4a48-bd20-f06a2d8a2b2c.png#align=left&display=inline&height=1866&originHeight=1866&originWidth=990&size=0&status=done&style=shadow&width=990)<br />这个流程在`removeAbandoned`设置为`true`的情况下才会触发，用于回收那些拿出去的使用长期未归还（归还：调用`close`方法触发主流程5）的连接。<br />先来看看`activeConnections`是什么，`activeConnections`用来保存当前从池子里被借出去的连接，这个可以通过主流程1看出来，每次调用getConnection时，如果开启`removeAbandoned`，则会把连接对象放到`activeConnections`，然后如果长期不调用`close`，那么这个被借出去的连接将永远无法被重新放回池子，这是一件很麻烦的事情，这将存在内存泄漏的风险，因为不`close`，意味着池子会不断产生新的连接放进`connections`，不符合连接池预期（连接池出发点是尽可能少的创建连接），然后之前被借出去的连接对象还有一直无法被回收的风险，存在内存泄漏的风险，因此为了解决这个问题，就有了这个流程，流程整体很简单，就是将现在借出去还没有归还的连接，做一次判断，符合条件的将会被放进`abandonedList`进行连接回收（这个list里的连接对象里的`abandoned`将会被置为`true`，标记已被该流程处理过，防止主流程5再次处理）。<br />这个如果在实践中能保证每次都可以正常`close`，完全不用设置`removeAbandoned=true`，目前如果使用了类似mybatis、spring等开源框架，框架内部是一定会`close`的，所以此项是不建议设置的，视情况而定。
<a name="6N2fY"></a>
## 九、主流程5：回收连接
这个流程通常是靠连接包装类`DruidPooledConnection`的`close`方法触发的，目标方法为`recycle`，流程图如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617420005655-928198ac-dc22-47ea-aa95-4c6818ed2563.png#align=left&display=inline&height=993&originHeight=993&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />这也是非常重要的一个流程，连接用完要归还，就是利用该流程完成归还的动作，利用druid对外包装的`Connecion`包装类`DruidPooledConnection`的`close`方法触发，该方法会通过自己内部的`close`或者`syncClose`方法来间接触发`dataSource`对象的`recycle`方法，从而达到回收的目的。<br />最终的`recycle`方法：<br />①如果`removeAbandoned`被设置为`true`，则通过`traceEnable`判断是否需要从`activeConnections`移除该连接对象，防止流程4.2再次检测到该连接对象，当然如果是流程4.2主动触发的该流程，那么意味着流程4.2里已经`remove`过该对象了，`traceEnable`会被置为`false`，本流程就不再触发`remove`了（这个流程都是在`removeAbandoned=true`的情况下进行的，在主流程1里连接被放进`activeConnections`时`traceEnable`被置为`true`，而在`removeAbandoned=false`的情况下`traceEnable`恒等于`false`）。<br />②如果回收过程中发现存在有未处理完的事务，则触发回滚（比较有可能触发这一条的是流程4.2里强制归还连接，也有可能是单纯使用连接，开启事务却没有提交事务就直接`close`的情况），然后利用`holder.reset`进行恢复连接对象里一些属性的默认值，除此之外，`holder`对象还会把由它产生的`statement`对象放到自己的一个`arraylist`里面，`reset`方法会循环着关闭内部未关闭的`statement`对象，最后清空`list`，当然，`statement`对象自己也会记录下其产生的所有的`resultSet`对象，然后关闭`statement`时同样也会循环关闭内部未关闭的`resultSet`对象，这是连接池做的一种保护措施，防止用户拿着连接对象做完一些操作没有对打开的资源关闭。<br />③判断是否开启`testOnReturn`，这个跟`testOnBorrow`一样，官方默认不开启，也不建议开启，影响性能，理由参考主流程1里针对`testOnBorrow`的解释。<br />④直接放回池子（当前`connections`的尾部），然后需要注意的是`putLast`方法和`put`方法的不同之处，`putLast`会把`lastActiveTimeMillis`置为当前时间，也就是说不管一个连接被借出去过久，只要归还了，最后活跃时间就是当前时间，这就会有造成某种特殊异常情况的发生（非常极端，几乎不会触发，可以选择不看）：
> 如果不开启`testOnBorrow`和`testOnReturn`，并且`keepAlive`设置为`false`，那么长连接可用测试的间隔依据就是利用当前时间减去上次活跃时间（`lastActiveTimeMillis`）得出闲置时间，然后再利用闲置时间跟`timeBetweenEvictionRunsMillis`（默认60s）进行对比，超过才进行长连接可用测试。
> 那么如果一个MySQL服务端的长连接保活时间被人为调整为60s，然后`timeBetweenEvictionRunsMillis`被设置为59s，这个设置是非常合理的，保证了测试间隔小于长连接实际保活时间，然后如果这时一个连接被拿出去后一直过了61s才被`close`回收，该连接对象的`lastActiveTimeMillis`被刷为当前时间，如果在59s内再次拿到该连接对象，就会绕过连接检查直接报连接不可用的错误。

<a name="Ah7uK"></a>
## 十、结束
到这里针对druid连接池的初始化以及其内部一个连接从生产到消亡的整个流程就已经整理完了，主要是列出其运行流程以及一些主要的监控数据都是如何产生的，没有涉及到的是一个sql的执行，因为这个基本上就跟使用原生驱动程序差不多，只是druid又包装了一层`Statement`等，用于完成一些自己的操作。
