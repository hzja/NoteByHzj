JavaMybatis
<a name="lD6MW"></a>
## 缓存是在哪起作用的？
Mybatis一级缓存和二级缓存并不是一个很好的设计，工作中基本上也不会使用一级缓存和二级缓存，因为一旦使用不当会造成很多问题，来看看到底会有什么问题？<br />Executor会调用`StatementHandler`执行sql，起一个承上启下的作用。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096614481-362c314c-a89e-4726-bfda-32581a57b102.png#clientId=ub889ccf5-c0b6-4&from=paste&id=ucdc390d7&originHeight=301&originWidth=620&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub7d9f254-ae6b-45c0-bde7-a535822ae3b&title=)<br />`Executor`的设计是一个典型的装饰者模式，`SimpleExecutor`，`ReuseExecutor`是具体实现类，而`CachingExecutor`是装饰器类。<br />可以看到具体组件实现类有一个父类`BaseExecutor`，而这个父类是一个模板模式的典型应用，操作一级缓存的操作都在这个类中，而具体的操作数据库的功能则让子类去实现。<br />**「二级缓存则是一个装饰器类，当开启二级缓存的时候，会使用**`**CachingExecutor**`**对具体实现类进行装饰，所以查询的时候一定是先查询二级缓存再查询一级缓存」**![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096614315-ed03e5b2-d213-414d-a4a7-07ede87e5a56.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u8fab50f7&originHeight=308&originWidth=663&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4888dd0f-18f8-4f27-85e2-4ac733308a7&title=)<br />**「那么一级缓存和二级缓存有什么区别呢？」**
<a name="BsUUw"></a>
## 一级缓存
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096614370-0b819d4f-c08f-44bd-ab81-da75b4ab45c4.png#clientId=ub889ccf5-c0b6-4&from=paste&id=ub5d58b7a&originHeight=498&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u28c4488e-cb01-43d8-89d2-d1da8107e2f&title=)
```java
// BaseExecutor
protected PerpetualCache localCache;
```
一级缓存是`BaseExecutor`中的一个成员变量localCache（对`HashMap`的一个简单封装），因此一级缓存的生命周期与`SqlSession`相同，如果对`SqlSession`不熟悉，可以把它类比为JDBC编程中的`Connection`，即数据库的一次会话。<br />**「一级缓存和二级缓存key的构建规则是一致的，都是一个CacheKey对象，因为Mybatis中涉及动态SQL等多方面的因素，缓存的key不能仅仅通过String来表示」**<br />当执行sql的如下4个条件都相等时，CacheKey才会相等

1. `mappedStatment`的id
2. 指定查询结构集的范围
3. 查询所使用SQL语句
4. 用户传递给SQL语句的实际参数值

**「当查询的时候先从缓存中查询，如果查询不到的话再从数据库中查询」**<br />`org.apache.ibatis.executor.BaseExecutor#query`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096614327-eced4b19-2e2f-4b8b-8748-b2bdc8e34461.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u58aa1b97&originHeight=88&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9d1e965d-00c4-49a8-805f-880a8eed646&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096614345-dd9c6d93-3749-4a0d-9c31-849aa0fc7524.png#clientId=ub889ccf5-c0b6-4&from=paste&id=uc7265de7&originHeight=109&originWidth=680&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udabd6263-4eb7-401a-978f-9777ab86676&title=)<br />当使用同一个`SqlSession`执行更新操作时，会先清空一级缓存。因此一级缓存中内容被使用的概率也很低![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096614840-03eccc21-85f3-45f2-9ab1-1f816020a32c.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u0fd4bc1b&originHeight=164&originWidth=847&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua671970f-c892-438d-b1ea-076dea76cc2&title=)
<a name="tOx27"></a>
## 一级缓存测试
**「美团技术团队上关于一级缓存和二级缓存的一些测试写的挺不错的，就直接引用过来了」**<br />原文地址：[https://tech.meituan.com/2018/01/19/mybatis-cache.html](https://tech.meituan.com/2018/01/19/mybatis-cache.html) 测试代码github地址：[https://github.com/kailuncen/mybatis-cache-demo](https://github.com/kailuncen/mybatis-cache-demo)<br />接下来通过实验，了解MyBatis一级缓存的效果，每个单元测试后都请恢复被修改的数据。<br />首先是创建示例表student，创建对应的POJO类和增改的方法，具体可以在entity包和mapper包中查看。
```sql
CREATE TABLE `student` (
	`id` int(11) unsigned NOT NULL AUTO_INCREMENT,
	`name` varchar(200) COLLATE utf8_bin DEFAULT NULL,
	`age` tinyint(3) unsigned DEFAULT NULL,
	PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
```
在以下实验中，id为1的学生名称是凯伦
<a name="OOV9z"></a>
### 「实验1」
开启一级缓存，范围为会话级别，调用三次getStudentById，代码如下所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096614952-21863829-1062-40d1-9372-44bc14dfd2ce.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u5b316f02&originHeight=122&originWidth=585&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua5c24a09-5a3a-4874-bf2b-0782b262b80&title=)<br />执行结果：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615012-d158b8a5-323f-4dc6-9f97-1257ee459644.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u08d24c6d&originHeight=201&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue6465602-dec5-4bdc-8ff5-5d57d46585d&title=)可以看到，只有第一次真正查询了数据库，后续的查询使用了一级缓存。
<a name="elLck"></a>
### 「实验2」
增加了对数据库的修改操作，验证在一次数据库会话中，如果对数据库发生了修改操作，一级缓存是否会失效。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615016-9681446a-045c-4d2a-806c-909725e1dbfe.png#clientId=ub889ccf5-c0b6-4&from=paste&id=ube62326f&originHeight=155&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9ea58647-0c4a-4333-83b9-edaaa25fe15&title=)<br />执行结果：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615201-b423ce53-e606-413c-af4a-847cc46da666.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u62beecc8&originHeight=354&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0154f366-f98d-470b-aabd-39257ec0cbe&title=)可以看到，在修改操作后执行的相同查询，查询了数据库，一级缓存失效。
<a name="Xrvcn"></a>
### 「实验3」
开启两个SqlSession，在sqlSession1中查询数据，使一级缓存生效，在sqlSession2中更新数据库，验证一级缓存只在数据库会话内部共享。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615341-b0252815-5b5d-4e50-8db1-9ef380b28822.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u429c204f&originHeight=243&originWidth=795&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc5061b00-c26d-4c32-8608-227bcf00a54&title=)输出如下![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615529-d93d0691-67d5-4cb3-ab5d-2fe71fb93c65.png#clientId=ub889ccf5-c0b6-4&from=paste&id=uea6f5861&originHeight=313&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua63651d0-7029-4ca0-aa97-7f2df491dd8&title=)sqlSession1和sqlSession2读的时相同的数据，但是都查询了数据库，说明了**「一级缓存只在数据库会话层面共享」**<br />sqlSession2更新了id为1的学生的姓名，从凯伦改为了小岑，但sqlSession1之后的查询中，id为1的学生的名字还是凯伦，出现了脏数据，也证明了之前的设想，一级缓存只在数据库会话层面共享<br />**「MyBatis的一级缓存最大范围是**`**SqlSession**`**内部，有多个SqlSession或者分布式的环境下，数据库写操作会引起脏数据，建议设定缓存级别为**`**Statement**`**，即进行如下配置」**
```xml
<setting name="localCacheScope" value="STATEMENT"/>
```
原因也很简单，看`BaseExecutor`的`query()`方法，当配置成STATEMENT时，每次查询完都会清空缓存![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615581-6d1fe538-07ca-4e0c-b77b-2825f6d3ee40.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u9e11f5bb&originHeight=72&originWidth=572&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6f133469-cbaa-46ab-b1f7-22045a3ac5c&title=)<br />**「看到这可能会想，用Mybatis后没设置这个参数，好像也没发生脏读的问题，其实是因为和spring整合了」**<br />当Mybatis和spring整合后

1. 在未开启事务的情况之下，每次查询，spring都会关闭旧的sqlSession而创建新的sqlSession，因此此时的一级缓存是没有起作用的
2. 在开启事务的情况之下，spring使用threadLocal获取当前线程绑定的同一个sqlSession，因此此时一级缓存是有效的，当事务执行完毕，会关闭sqlSession

**「当Mybatis和spring整合后，未开启事务的情况下，不会有任何问题，因为一级缓存没有生效。当开启事务的情况下，可能会有问题，由于一级缓存的存在，在事务内的查询隔离级别是可重复读，即使数据库的隔离级别设置的是提交读」**
<a name="HSEzu"></a>
## 二级缓存
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615593-bc9c8adc-30b5-4525-a7b1-56d192186a8e.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u47c366bf&originHeight=592&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u735fb961-cacc-490e-8fa9-bb83d7f3bd9&title=)
```java
// Configuration
protected final Map<String, Cache> caches = new StrictMap<>("Caches collection");
```
**「而二级缓存是**`**Configuration**`**对象的成员变量，因此二级缓存的生命周期是整个应用级别的。并且是基于namespace构建的，一个namesapce构建一个缓存」**<br />**「二级缓存不像一级缓存那样查询完直接放入一级缓存，而是要等事务提交时才会将查询出来的数据放到二级缓存中。」**<br />因为如果事务1查出来直接放到二级缓存，此时事务2从二级缓存中拿到了事务1缓存的数据，但是事务1回滚了，此时事务2不就发生了脏读了吗？
<a name="jxUI0"></a>
### 「二级缓存的相关配置有如下3个」
<a name="ndINF"></a>
#### 「1.mybatis-config.xml」
```xml
<settings>
	<setting name="cacheEnabled" value="true"/>
</settings>
```
这个是二级缓存的总开关，只有当该配置项设置为true时，后面两项的配置才会有效果<br />从`Configuration`类的`newExecutor`方法可以看到，当`cacheEnabled`为`true`，就用缓存装饰器装饰一下具体组件实现类，从而让二级缓存生效![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615956-ca06251a-35e3-4638-a45d-3c4ccf904dc0.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u54287ead&originHeight=308&originWidth=663&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud422cded-648b-4d1f-82f5-84bedc1ac66&title=)
<a name="XBWkr"></a>
#### 「2.mapper映射文件中」
mapper映射文件中如果配置了`<cache>`和`<cache-ref>`中的任意一个标签，则表示开启了二级缓存功能，没有的话表示不开启
```xml
<cache type="" eviction="FIFO" size="512"></cache>
```
二级缓存的部分配置如上，type就是填写一个全类名，用来指定二级缓存的实现类，这个实现类需要实现Cache接口，默认是`PerpetualCache`（可以利用这个属性将mybatis二级缓存和Redis，`Memcached`等缓存组件整合在一起）<br />`org.apache.ibatis.builder.MapperBuilderAssistant#useNewCache`![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615919-45673598-7a37-49ac-9185-1c08a42d441c.png#clientId=ub889ccf5-c0b6-4&from=paste&id=uf0226b55&originHeight=157&originWidth=556&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u013d298f-d93a-4a07-84d2-b603932d225&title=)<br />这个`eviction`表示缓存清空策略，可填选项如下

| 选项 | 解释 | 装饰器类 |
| --- | --- | --- |
| LRU | 最近最少使用的:移除最长时间不被使用的对象 | LruCache |
| FIFO | 先进先出:按对象进入缓存的顺序来移除它们 | FifoCache |
| SOFT | 软引用:移除基于垃圾回收器状态和软引用规则的对象 | SoftCache |
| WEAK | 弱引用:更积极地移除基于垃圾收集器状态和弱引用规则的对象 | WeakCache |

典型的装饰者模式的实现，换缓存清空策略就是换装饰器。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096615970-acc6f215-558a-4b97-a078-34fa383969b2.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u7f5a939d&originHeight=203&originWidth=678&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1cdc5c2b-dc0d-4914-975e-c414772f4f7&title=)
<a name="QBM77"></a>
#### 「3.`<select>`节点中的`useCache`属性」
该属性表示查询产生的结果是否要保存的二级缓存中，useCache属性的默认值为true，这个配置可以将二级缓存细分到语句级别
<a name="XsoFf"></a>
### 测试二级缓存
二级缓存是基于namespace实现的，即一个mapper映射文件用一个缓存<br />在本实验中，id为1的学生名称初始化为点点。
<a name="tMO9r"></a>
#### 「实验1」
测试二级缓存效果，不提交事务，sqlSession1查询完数据后，sqlSession2相同的查询是否会从缓存中获取数据。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096616190-00b334b0-3668-47f6-a729-84954358f278.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u5e520994&originHeight=192&originWidth=648&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua9ee8c28-eb1f-4e1e-a4ff-fc9faca92c3&title=)<br />执行结果：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096616213-327608f0-1c0c-48fb-ae59-ec7b8889a9cd.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u7147d22c&originHeight=340&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2f51197e-6724-4dcb-a2f6-8b130f7d60e&title=)可以看到，当sqlsession没有调用`commit()`方法时，二级缓存并没有起到作用。
<a name="YIGCV"></a>
#### 「实验2」
测试二级缓存效果，当提交事务时，sqlSession1查询完数据后，sqlSession2相同的查询是否会从缓存中获取数据。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096616482-f1e7bd1a-3a0a-4059-854d-23fd77bee460.png#clientId=ub889ccf5-c0b6-4&from=paste&id=uc7b272bb&originHeight=208&originWidth=648&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u83fa949c-0429-4dee-9568-2c9bdb48c5b&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096616590-c05e299a-5f6f-4791-88d2-f7f8e0898593.png#clientId=ub889ccf5-c0b6-4&from=paste&id=ua186ee62&originHeight=244&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0847f850-287f-4fcc-85d1-bda4562287a&title=)从图上可知，sqlsession2的查询，使用了缓存，缓存的命中率是0.5。
<a name="OkV5B"></a>
#### 「实验3」
测试update操作是否会刷新该namespace下的二级缓存。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096616690-8db19773-f0e7-428f-93b0-7b1269bc67ae.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u5ff8d43b&originHeight=306&originWidth=657&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u60fc5684-4b2a-4e0b-9cb6-99b2db23268&title=)![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096616963-679869ae-3732-4109-a7de-6e1e92d02cec.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u3d2b614e&originHeight=331&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7886585d-9d0c-4a88-a66b-44b84b07168&title=)可以看到，在sqlSession3更新数据库，并提交事务后，sqlsession2的StudentMapper namespace下的查询走了数据库，没有走Cache。
<a name="SOD1V"></a>
#### 「实验4」
验证MyBatis的二级缓存不适应用于映射文件中存在多表查询的情况。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096617215-55e926f1-1cd7-4a7c-9a3d-01a33c535e88.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u52d67e63&originHeight=706&originWidth=592&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6e1e9c56-bfac-4ea7-aaf9-302b135bdaf&title=)<br />`getStudentByIdWithClassInfo`的定义如下<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096617361-54cebc70-b11c-4327-b08d-7dd638be7e04.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u85774e8c&originHeight=115&originWidth=677&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uebe49912-6418-4953-979a-de4e6a81d07&title=)<br />通常会为每个单表创建单独的映射文件，由于MyBatis的二级缓存是基于namespace的，多表查询语句所在的namspace无法感应到其他namespace中的语句对多表查询中涉及的表进行的修改，引发脏数据问题。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096617572-21ecb0a9-5e06-45ae-98d3-572f7efd3f77.png#clientId=ub889ccf5-c0b6-4&from=paste&id=uc2f46666&originHeight=305&originWidth=756&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf9e253ab-5d94-4370-a736-1595568aefe&title=)执行结果：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096617713-223a6016-0daf-449d-bb7c-cc34d17fe46a.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u9de5f0b5&originHeight=248&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub3c349b5-138c-4395-a1eb-7802c927cd6&title=)<br />在这个实验中，引入了两张新的表，一张class，一张classroom。class中保存了班级的id和班级名，classroom中保存了班级id和学生id。在StudentMapper中增加了一个查询方法`getStudentByIdWithClassInfo`，用于查询学生所在的班级，涉及到多表查询。在ClassMapper中添加了updateClassName，根据班级id更新班级名的操作。<br />当sqlsession1的studentmapper查询数据后，二级缓存生效。保存在`StudentMapper`的namespace下的cache中。当sqlSession3的classMapper的updateClassName方法对class表进行更新时，updateClassName不属于`StudentMapper`的namespace，所以`StudentMapper`下的cache没有感应到变化，没有刷新缓存。当`StudentMapper`中同样的查询再次发起时，从缓存中读取了脏数据。
<a name="yEkxP"></a>
#### 「实验5」
为了解决实验4的问题呢，可以使用Cache ref，让ClassMapper引用StudenMapper命名空间，这样两个映射文件对应的SQL操作都使用的是同一块缓存了。<br />mapper文件中的配置如下
```xml
<cache-ref namespace="mapper.StudentMapper"/>
```
执行结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660096618077-ddc46476-5eae-44f6-8c37-87f5a803a69d.png#clientId=ub889ccf5-c0b6-4&from=paste&id=u40990c00&originHeight=328&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1755899d-9c52-41c7-8883-0c754deb71b&title=)不过这样做的后果是，缓存的粒度变粗了，多个Mapper namespace下的所有操作都会对缓存使用造成影响。
<a name="cf21I"></a>
## 总结
Mybatis的一级缓存和二级缓存都是基于本地的，分布式环境下必然会出现脏读。<br />二级缓存可以通过实现Cache接口，来集中管理缓存，避免脏读，但是有一定的开发成本，并且在多表查询时，使用不当极有可能会出现脏数据<br />**「除非对性能要求特别高，否则一级缓存和二级缓存都不建议使用」**
