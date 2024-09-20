JavaMybatis
<a name="DbJJ6"></a>
## 为什么不直接用 JDBC，而要 ORM 框架
直接使用 JDBC 操作比较繁琐：

1. 先创建 connection 连接数据库
2. 创建 statement 对象
3. 通过 statement 对象执行 sql 语句，得到 resultSet
4. 遍历 resultSet，获取结果，并将记录手动转为 JavaBean
5. 关闭 resultSet、statement 、connection，释放资源防止泄露

对于每一个 sql 来说，都需要重复执行上面的命令，重复代码非常多，且遍历结果集记录转换成 JavaBean的也很繁琐，简而言之，很多搬砖的无脑操作需要重复写。<br />而且生成上的项目还需要用连接池，不能用完了之后直接释放连接，重复建连开销大。<br />基于上面这些点，一个 ORM 框架就能解放双手，从重复无脑的搬砖行为中解脱出来，且简化代码，还提供缓存、连接管理、事务等操作。<br />提升开发效率和提高系统的可维护性。
<a name="nRWMC"></a>
## Hibernate
是一款完整形态的 ORM 框架，可以从纯面向对象的角度来操作数据库。<br />它不仅能完成对象模型和数据库关系模型的映射，还能屏蔽不同数据库不同 SQL 的差异，简单来说用 Hibernate 提供的 API 就能完成大部分 SQL 操作，不需要编写 SQL，Hibernate 会生成 SQL。<br />且也提供了 HQL（Hibernate Query Language），面向对象的查询语句，和 SQL 类似但是不太相同，最终 HQL 会被转化成 SQL 执行<br />还提供了 Criteria 接口，也是一样，是一套面向对象的 API，一些复杂的 SQL 可以利用 Criteria 来实现，非常灵活。<br />总而言之， Hibernate 通过面向对象的思维操作数据库，屏蔽不同数据库的差异，它会根据底层不同数据库转化成对应的 SQL。<br />缺点就是屏蔽的太多了，例如因为自动生成 SQL，所以无法控制具体生成的语句，无法直接决定它走哪个的索引，且又经过了一层转化 SQL 的操作，所以性能也会有所影响。<br />简而言之，Hibernate 很好，基本上 ORM 要的它都有，包括一级、二级缓存等，且屏蔽底层数据库，提高了程序的可移植行。但由于它封装的太好了，无法精细化的控制执行的 SQL。<br />在国外几乎都用 Hibernate ，在国内大家都用 Mybaits。<br />没有绝对的好坏，只有合适与不合适。
<a name="nAEis"></a>
## JPA
JPA（Java Persistence API）规范。<br />它的提出其实是为了规范，整合市面上的 ORM 框架，像 Hibernate 就符合 JPA 的规范。<br />不过规范仅仅只是规范，每个具体的 ORM 实现或多或少会有点修改，因此 Spring 搞了 Spring Data JPA，实现了 JPA 规范，且适配了底层各种 ORM 框架，可以认为 Spring Data JPA 是一个胶水层。<br />这样用 Spring Data JPA 的话就在底层切换各种实现 JPA 规范的 ORM 框架。
<a name="kRxlT"></a>
## Mybatis
Mybatis 相对于 Hibernate  称为半 ORM 框架，因为 Hibernate 不需要写 SQL ，而 Mybatis 需要写 SQL。<br />也因为这点 Mybatis 更加的灵活且轻量。<br />能针对 SQL 进行优化，非常灵活地根据条件动态拼接 SQL 等，极端情况下性能占优。<br />不过也因为这点  Mybatis 和底层的数据库是强绑的，如果更换底层数据库， 所有 SQL 需要重新过一遍。<br />比如之前本地开发是 MySQL，后来客户特殊情况不用 saas，需要在他们那边部署一套，他们用的是 oracle，所以改咯。<br />具体选择 Hibernate  还是 Mybatis ，是看情况的。<br />比方一些项目很简单， QPS 又不高，快速解决战斗就选 Hibernate  ，无脑上就完了。<br />而一些核心服务，QPS很高，需要对 SQL 进行精细化定制，那就  Mybatis ，就是追求极致用 Mybatis 。
<a name="gKqfx"></a>
## `#{}`和`${}`的区别
`#{}` 是参数占位符，在处理的时候会与预编译将这个占位符替换为 ？，可以有效的防止 sql 注入。<br />`${}` 是变量占位符，字符串替换，直接替换文本，有注入的风险，比如传入 ‘ or 1=1’，这样的玩意。<br />而 `#{}` 是预编译，sql 的关键字在参数替换之前，已经解析了，无法再解析 ‘ or 1=1’ 这样的操作。
<a name="KtQBk"></a>
## Mybatis的插件是如何实现的？
想要实现一个插件，就要实现 mybatis 提供的 Interceptor 接口。<br />Interceptor 上的 `@Signature` 注解表明拦截的目标方法，具体参数有 type、method、args。<br />例如：
```java
@Signature(type = StatementHandler.class, method = "query", args = {Statement.class, ResultHandler.class}
```
都知道 mybatis 底层是利用 Executor 类来执行 sql 的，再具体点会有 StatementHandler、ParameterHandler、ResultSetHandler 这三个 handler。<br />StatementHandler 执行 sql，ParameterHandler 设置参数，由 statement 对象将 sql 和实参传递给数据库执行，再由 ResultSetHandler 封装返回的结果，所以在这几道工序之间做拦截，就能实现切入的功能，也就是插件。<br />具体的做法是实现 Interceptor ，表明要拦截的方法，填充要切入的逻辑，然后将 Interceptor 注册到 mybatis 的配置文件（mybatis-config.xml）中。<br />mybatis 加载时会解析文件，得到一堆 Interceptor 组成 InterceptorChain 并保存着。<br />然后在创建 Executor、ParameterHandler、StatementHandler、ResultSetHandler 这几个类的对象时，就会从 InterceptorChain 得到相应的 Interceptor 通过 jdk 动态代理得到代理对象，如果没有合适的 Interceptor 则会返回原对象。<br />就这样，插件的逻辑就被织入了。
<a name="KUIL1"></a>
## Mybatis如何实现数据库类型和Java类型的转换的
主要是利用 TypeHandler 这个类型转换器。<br />mybait 内置了很多默认的 TypeHandler 实现：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909372-e6eb5b58-e05d-4a4b-a600-6259004ca2ec.png#clientId=u8f3942d8-c692-4&from=paste&id=u86dd7c82&originHeight=533&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u974fbec1-90c0-4304-adb3-0c29e90fa30&title=)这些 TypeHandler 会被注册到 TypeHandlerRegistry 中，实际底层就是用了很多 map 来存储类型和 TypeHandler 的对应关系：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909347-0a2acc77-c4f1-4375-b289-58895fbefd06.png#clientId=u8f3942d8-c692-4&from=paste&id=u49df85db&originHeight=252&originWidth=944&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u74f0e2de-786c-4b24-851d-2e59423cb4c&title=)<br />比如通过 JdbcType 找 TypeHandler ，通过 JavaType 找 TypeHandler 。<br />然后 TypeHandler 里面会实现类型的转化，拿 DateTypeHandler 来举例，很简单的：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909340-8a8f0939-7dd0-4cad-a645-07c31290688a.png#clientId=u8f3942d8-c692-4&from=paste&id=u550352f6&originHeight=412&originWidth=919&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc9e299b4-089d-4368-a980-4f9fce8f7ed&title=)<br />所以，如果要自定义类型转换器实现 TypeHandler 接口的这几个方法即可。<br />在 mybatis 加载的时候，就会把这些 TypeHandler 实现类注册了，然后有对应的类型转化的时候，就会调用对应的方法进行转化。
<a name="QiYKY"></a>
## Mybatis的数据源工厂
mybatis 提供了 DataSourceFactory 接口，具体有两个实现：

- UnpooledDataSourceFactory
- PooledDataSourceFactory

从名字就能看出一个是创建非池化的数据源、一个是池化的。<br />工厂创建的数据源分别是：

- UnpooledDataSource
- PooledDataSource

都知道 mybatis 其实是基于 JDBC 的，所以实际通过数据源获取连接是依靠 JDBC 的能力。<br />先来看下 `UnpooledDataSource`，当从中想要获取一个连接的话，实际会调用 `doGetConnection`方法：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909392-362c2c69-9c92-48b0-8e65-b1002c775889.png#clientId=u8f3942d8-c692-4&from=paste&id=u40d497ab&originHeight=143&originWidth=760&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6cfb3449-b600-4e85-80e1-7de387fb1f0&title=)<br />对 JDBC 有一定了解的伙伴应该很熟悉，初始化 driver，然后通过 DriverManager 获取连接，这就是 JDBC 的功能，然后把连接配置下，设置下超时时间、是否自动提交事务，事务隔离级别。<br />通过调用这个方法就新建连了一条连接，所以这叫非池化，因为每次获取都是新连接。<br />然后调用 close 就直接把连接关闭了。<br />有了这个基础之后，再来看下 PooledDataSource 的实现。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909402-92f8ea06-d59d-473c-8ca3-629a82aa29d7.png#clientId=u8f3942d8-c692-4&from=paste&id=u5f0ddd08&originHeight=111&originWidth=899&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u20bad90a-1b85-41c4-b098-f68153a2cff&title=)<br />可以看到两个关键点，一个是 pop 这个名字，一个是返回的是 proxyConnection。<br />pop 说明是从一个地方得到连接，且 getProxyConnection 说明得到的是一个代理连接。<br />知晓了这两个前提之后，先来简单了解下连接池的原理：<br />连接池会保存一定数量的数据库连接，待获取连接的时候，直接从池里面拿连接，不需要重新建连，提高响应速度，预防突发流量，用完之后归还连接到连接池中，而不是关闭连接。<br />接下来详细看看 mybatis 如何实现的：<br />实际上 PooledDataSource 是通过一个 PoolState 对象来管理连接：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909806-64a86d6b-03a6-474d-9ed7-70ed7604dc20.png#clientId=u8f3942d8-c692-4&from=paste&id=uf1bfd329&originHeight=346&originWidth=871&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uddf3a97d-2938-4884-a5ed-ebbf94ab0b4&title=)<br />核心就是两个 list 存储了空闲的连接和活跃的连接，其他就是一些统计数据了。<br />然后可以看到，list 里面存储的是 `pooledConnectiton`，这玩意实现了代理逻辑，且保存了真正的 Connection 和代理的 Connection ：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909800-00aa7bab-e273-44b3-9f40-1c6661ff7576.png#clientId=u8f3942d8-c692-4&from=paste&id=u3e6a9d03&originHeight=603&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u440d7890-62f5-4e08-bde6-8cd73709ca6&title=)<br />而这个代理其实只是拦截了 CLOSE 方法，也就是连接关闭的方法，让它只是归还到连接池中，而不是真正的关闭。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909791-17f56f3f-34de-45fb-8020-38c85a3a61e3.png#clientId=u8f3942d8-c692-4&from=paste&id=ub53cf838&originHeight=287&originWidth=813&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u35207c33-4b17-4f5c-8da2-fafa7056adc&title=)
<a name="QQHer"></a>
### 现在回到之前提到的 popConnection 逻辑，用语言总结下：

1. 利用 PoolState 加锁，同步执行以下流程
2. 判断空闲连接列表是否有空闲连接，如果有则获得这个连接。
3. 如果没有空闲连接，则看看活跃的连接是否超过限定值，不超过则通过 JDBC 获取新的连接，再由 PooledConnection 封装，获得这个连接
4. 如果活跃连接超过最大限定值，则获得最老的连接（PooledConnection ），看看连接是否超时，如果超时则进行一些统计，把上面一些未提交的事务进行回滚，然后获得底层 JDBC 的连接，再新建 PooledConnection 封装这个底层连接，**并把之前老连接PooledConnection 对象设置为无效**
5. 如果没有超时连接，则 wait 等待，默认等待 20 s。

看到这里肯定有同学会对 PooledConnection 的结构有点疑惑，这玩意除了拦截 close 方法还有什么用了？<br />PooledConnection 里面有个关键的 vaild 属性，这个很有用处，因为归还连接到连接池之后，不能保证外面是否持有连接的引用，所以 PooledConnection 里加了个 valid 属性，在归还的之后把代理连接置为无效，**这样即使外面有这个引用，也无法使用这个连接**。<br />所以上面才会有把已经超时的 PooledConnection 代理的底层的 JDBC 连接拿出来，然后新建一个 PooledConnection 再封装底层 JDBC 连接的操作，因为老的 PooledConnection 已经被设为无效了。
<a name="FiHCd"></a>
### 再看看归还连接的流程 pushConnection，同样还是用语言总结下：

1. poolstate 加锁
2. 直接从连接活跃链表里面移除当前的连接
3. 判断连接是否有效（vaild） ，无效则记录下 badConnectionCount 数量，直接返回不做其他处理
4. 有效则判断连接空闲列表数量是否够了，不够则判断当前连接是否有未提交事务，有的话回滚，然后新建 PooledConnection 封装当前 PooledConnection 底层的 JDBC 连接，并加入连接空闲列表中，并把当前的 PooledConnection 置为无效，**并唤醒等待的连接的线程**。
5. 如果连接空闲列表数量数量够了，同样则判断当前连接是否有未提交事务，有的话回滚，然后直接关闭底层 JDBC 的连接，，并把当前的 PooledConnection 置为无效。

好了，获取和归还连接的逻辑就如上所示，如果想看源码的话，代码在 `PooledDataSource#popConnection` 和 `PooledDataSource#pushConnection` 里。<br />这么看下来后，想来对连接池的设计已经心知肚明了吧~
<a name="zODTW"></a>
## 说说 MyBatis 的缓存机制
一级缓存默认开启，二级缓存默认关闭，可以为每个 namespace单独设置二级缓存，也可通过在多个 namespace 中共享一个二级缓存。<br />一级缓存是会话级缓存。即创建一个 SqlSession 对象就是一个会话，一次会话可能会执行多次相同的查询，这样缓存了之后就能重复利用查询结果，提高性能，不过 commit、rollback、update 都会清除缓存。<br />不过要注意，不同 SqlSession 之间的修改不会影响双方，比如 SqlSession1 读了数据 A， SqlSession2 将数据改为 B，此时 SqlSession1 再读还是得到 A ，这就出现了脏数据的问题。<br />所以，如果是多 SqlSession 或者分布式环境下，就可能有脏数据的情况发生。<br />二级缓存是命名空间级别的共享的，二级缓存只有在事务提交的时候，才会真正添加到底层的 cache 对象中，这样防止出现脏读的情况。<br />但是二级缓存也会有脏数据的情况，比如多个命名空间进行多表查询，各命名空间之间数据是不共享的，所以存在脏数据的情况，这时候如果利用共享 cache 就能解决。<br />开启二级缓存之后，会先从二级缓存查找，找不到再去一级缓存查找，找不到再去数据库查询。<br />二级缓存主要是利用 CachingExecutor 这个装饰器拦了一道，来看下 CachingExecutor#query 方法：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909790-b51067b2-603c-4ecf-8a71-b554511888da.png#clientId=u8f3942d8-c692-4&from=paste&id=u40ec9daf&originHeight=378&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud106a693-f1fe-4ab5-8e26-e4143b25060&title=)<br />而 MyBatis 的缓存本质上就是在本地利用 map 来存储数据。<br />基础实现类是 PerpetualCache ，并且使用了装饰器模式，提供了各种各样的 cache 进行功能的扩展，比如：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653874909815-0d776668-4e7c-48eb-a1e8-5c49f812d86e.png#clientId=u8f3942d8-c692-4&from=paste&id=u82d681f2&originHeight=387&originWidth=932&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubadbe018-1540-4562-ac12-028eedc1420&title=)<br />像 BlockingCache 可以提供阻塞，FifoCache、LruCache 等等。<br />**建议生产上关闭缓存，直接通过 aop 利用 redis 等分布式缓存来实现**。
