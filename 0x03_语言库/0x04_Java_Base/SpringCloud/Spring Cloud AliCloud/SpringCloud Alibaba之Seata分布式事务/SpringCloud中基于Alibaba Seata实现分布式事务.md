JavaSpringCloudSeata<br />本文主要包含三个方面内容：

- 讲解分布式事务的解决方案；
- 介绍 Alibaba Seata 分布式事务中间件；
- 分析 Seata 的 AT 模式实现原理。
<a name="vSgTA"></a>
## 分布式事务的解决方案
下面先聊一下为什么会产生分布式事务。举个例子，某线上商城会员在购买商品的同时产生相应的消费积分，消费积分在下一次购物时可以抵用现金。这个业务的逻辑如果放在以前的单点应用是很简单的，如下所示。
```
开启数据库事务
创建订单
会员积分增加
商品库存减少
提交数据库事务
```
在这个过程中，因为程序操作的是单点数据库，所以在一个数据库事务中便可完成所有操作，利用数据库事务自带的原子性保证了所有数据要么全部处理成功，要么全部回滚撤销。但是放在以微服务为代表的分布式架构下问题就没那么简单了，来看一下示意图。<br />![分布式架构下调用关系图](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017834043-a5e88970-8304-4f63-bb6c-31f6cdeb268a.png#averageHue=%23fcfbfb&clientId=uad0287e3-51cb-4&from=paste&id=u93f84360&originHeight=664&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u8f41714b-1198-43f4-8dc4-fbbf0deeb36&title=%E5%88%86%E5%B8%83%E5%BC%8F%E6%9E%B6%E6%9E%84%E4%B8%8B%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB%E5%9B%BE "分布式架构下调用关系图")<br />可以看到，商城应用作为业务的发起者分别向订单、会员、库存服务发起了调用，而这些服务又拥有自己独立的数据存储，因为在物理上各个数据库服务器都是独立的，每一个步骤的操作都会创建独立的事务，这就意味着在分布式处理时无法通过单点数据库利用一个事务保证数据的完整性，必须引入某种额外的机制来协调多个事务要么全部提交、要么全部回滚，以此保证数据的完整性，这便是“分布式事务”的由来。<br />在分布式架构中有两种经典的分布式事务解决方案：**二阶段提交（2PC**）与**三阶段提交（3PC）**。
<a name="xEkFs"></a>
## 二阶段提交
首先分析下二阶段提交的处理过程，下面是二阶段提交中的**第一个阶段：事务预处理阶段。**<br />![2PC 阶段一：事务预处理阶段](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017834062-693f4690-7030-410b-a9fd-e8e19aa0c6ba.png#averageHue=%23f8f8f8&clientId=uad0287e3-51cb-4&from=paste&id=ub456ff8f&originHeight=706&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u6c37b4de-65a2-4cce-8019-85e06935b9e&title=2PC%20%E9%98%B6%E6%AE%B5%E4%B8%80%EF%BC%9A%E4%BA%8B%E5%8A%A1%E9%A2%84%E5%A4%84%E7%90%86%E9%98%B6%E6%AE%B5 "2PC 阶段一：事务预处理阶段")<br />可以看到，相比单点事务，分布式事务中增加了一个新的角色：**事务协调者（Coordinator）**，它的职责就是协调各个分支事务的开启与提交、回滚的处理。<br />以上图为例，当商城应用订单创建后，首先事务协调者会向各服务下达“处理本地事务”的通知，所谓本地事务就是每个服务应该做的事情，如订单服务中负责创建新的订单记录；会员服务负责增加会员的积分；库存服务负责减少库存数量。在这个阶段，被操作的所有数据都处于未提交（uncommit）的状态，会被排它锁锁定。当本地事务都处理完成后，会通知事务协调者“本地事务处理完毕”。当事务协调者陆续收到订单、会员、库存服务的处理完毕通知后，便进入“**阶段二：提交阶段**”。<br />![2PC 阶段二：提交阶段](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017833986-c090ab46-8b26-44ce-a00b-4c53d0495cab.png#averageHue=%23fbfafa&clientId=uad0287e3-51cb-4&from=paste&id=u0d861b99&originHeight=749&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u5ccd09b7-bee6-4add-b7b7-8286d1a08e7&title=2PC%20%E9%98%B6%E6%AE%B5%E4%BA%8C%EF%BC%9A%E6%8F%90%E4%BA%A4%E9%98%B6%E6%AE%B5 "2PC 阶段二：提交阶段")<br />在提交阶段，事务协调者会向每一个服务下达提交命令，每个服务收到提交命令后在本地事务中对阶段一未提交的数据执行 Commit 提交以完成数据最终的写入，之后服务便向事务协调者上报“提交成功”的通知。当事务协调者收到所有服务“提交成功”的通知后，就意味着一次分布式事务处理已完成。<br />这便是二阶段提交的正常执行过程，但假设在阶段一有任何一个服务因某种原因向事务协调者上报“事务处理失败”，就意味着整体业务处理出现问题，阶段二的操作就自动改为回滚（Rollback）处理，将所有未提交的数据撤销，使数据还原以保证完整性。<br />对于二阶段提交来说，它有一个致命问题，当阶段二某个服务因为网络原因无法收到协调者下达的提交命令，则未提交的数据就会被长时间阻塞，可能导致系统崩溃。<br />![二阶段提交的缺陷](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017834195-9459e5a7-1060-4809-8103-3674edc8a7f6.png#averageHue=%23fafaf9&clientId=uad0287e3-51cb-4&from=paste&id=ufee8f012&originHeight=647&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u7581277d-cae6-4345-b710-ec0bd1dcb3a&title=%E4%BA%8C%E9%98%B6%E6%AE%B5%E6%8F%90%E4%BA%A4%E7%9A%84%E7%BC%BA%E9%99%B7 "二阶段提交的缺陷")<br />以上图为例，假如在提交阶段，库存服务实例与事务协调者之间断网。提交指令无法下达，这会导致库存中的“飞科剃须刀”商品库存记录会长期处于未提交的状态，因为这条记录被数据库排他锁长期独占，之后再有其他线程要访问“飞科剃须刀”库存数据，该线程就会长期处于阻塞状态，随着阻塞线程的不断增加，库存服务会面临崩溃的风险。<br />那这个问题要怎么解决呢？其实只要在服务这一侧增加超时机制，过一段时间被锁定的“飞科剃须刀”数据因超时自动执行提交操作，释放锁定资源。尽管这样做会导致数据不一致，但也比线程积压导致服务崩溃要好，出于此目的，三阶段提交（3PC）便应运而生。
<a name="x5Bsc"></a>
## 三阶段提交
三阶段提交实质是将二阶段中的提交阶段拆分为“**预提交阶段**”与“**提交阶段**”，同时在服务端都引入超时机制，保证数据库资源不会被长时间锁定。下面是三阶段提交的示意流程：<br />![3PC 阶段一：事务预处理阶段](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017834065-4b0eef12-e3b9-4de0-a777-b792fa01bc25.png#averageHue=%23f9f8f8&clientId=uad0287e3-51cb-4&from=paste&id=ua85c6da4&originHeight=687&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u72297d91-f55d-436d-9111-49736fb3140&title=3PC%20%E9%98%B6%E6%AE%B5%E4%B8%80%EF%BC%9A%E4%BA%8B%E5%8A%A1%E9%A2%84%E5%A4%84%E7%90%86%E9%98%B6%E6%AE%B5 "3PC 阶段一：事务预处理阶段")
<a name="Kv4n7"></a>
### 阶段一：事务预处理阶段。
3PC 的事务预处理阶段与 2PC 是一样的，用于处理本地事务，锁定数据库资源，当所有服务返回成功后，进入阶段二。<br />![3PC 阶段二：预提交阶段](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017834517-b86b56e0-c780-4fd3-98b4-d8de91142f9b.png#averageHue=%23fafaf9&clientId=uad0287e3-51cb-4&from=paste&id=z2nkY&originHeight=643&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u86208c71-a588-4089-b0bb-f99e2f11c9d&title=3PC%20%E9%98%B6%E6%AE%B5%E4%BA%8C%EF%BC%9A%E9%A2%84%E6%8F%90%E4%BA%A4%E9%98%B6%E6%AE%B5 "3PC 阶段二：预提交阶段")
<a name="Ux6a3"></a>
### 阶段二：预提交阶段。
预提交阶段只是一个询问机制，以确认所有服务都已准备好，同时在此阶段协调者和参与者都设置了超时时间以防止出现长时间资源锁定。当阶段二所有服务返回“可以提交”，进入阶段三“提交阶段”。
<a name="w7BC0"></a>
### 阶段三：提交阶段。
3PC 的提交阶段与 2PC 的提交阶段是一致的，在每一个数据库中执行提交实现数据的资源写入，如果协调者与服务通信中断导致无法提交，在服务端超时后在也会自动执行提交操作来保证资源释放。<br />通过对比可以发现，三阶段提交是二阶段提交的优化版本，主要通过加入预提交阶段引入了超时机制，让数据库资源不会被长期锁定，但这也会带来一个新问题，数据一致性也很可能因为超时后的强制提交被破坏，对于这个问题各大软件公司都在各显神通，常见的做法有：增加异步的数据补偿任务、日终跑批前的数据补偿、更完善的业务数据完整性的校验代码、引入数据监控及时通知人工补录这些都是不错的补救措施。<br />讲到这，相比你对 2PC 与 3PC 的分布式事务方案应该有了初步的了解，这里还是要强调下，**无论是 2PC 与 3PC 都是一种方案，是一种宏观的设计。**如果要落地就要依托具体的软件产品，在 Java 开源领域能够提供完善的分布式事务解决方案的产品并不多，比较有代表性的产品有 ByteTCC、TX-LCN、EasyTransaction、Alibaba Seata，其中无论从成熟度、厂商背景、更新频度、社区活跃度各维度比较，Alibaba Seata都是数一数二的分布式事务中间件产品，本讲后面的内容将围绕Alibaba Seata的AT模式展开，探讨Alibaba Seata是如何实现自动化的分布式事务处理的。
<a name="LiyAM"></a>
## Alibaba Seata 分布式事务中间件
Alibaba Seata 是一款开源的分布式事务解决方案，致力于在微服务架构下提供高性能和简单易用的分布式事务服务。它的官网是http://seata.io/,截止到目前 Seata 在 GitHub 已有 23147 star，最新版本已迭代到 1.6.0，阿里多年的技术沉淀让 Seata 的内部版本平稳渡过了多次双 11 的考验。2019 年 1 月为了打造更加完善的技术生态和普惠技术成果，Seata 正式宣布对外开源，未来 Seata 将以社区共建的形式帮助其技术更加可靠与完备，按官方的说法Seata目前已具备了在生产环境使用的条件。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017834567-f383d318-6231-45c0-93f8-340e40bed644.png#averageHue=%230a5ad1&clientId=uad0287e3-51cb-4&from=paste&id=u003c640e&originHeight=625&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua2a6aca2-1ce5-4c51-97e1-2bdae67164a&title=)<br />Seata 提供了多种分布式事务的解决方案，包含 AT 模式、TCC 模式、SAGA 模式以及 XA 模式。其中 AT 模式提供了最简单易用且无侵入的事务处理机制，通过自动生成反向 SQL 实现事务回滚。从 AT 模式入手使用，是理解分布式事务处理机制是非常好的学习办法。<br />![特色功能](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017834771-57fccfa5-e017-4bd3-883f-6d65fa06c919.png#averageHue=%23f7f8f8&clientId=uad0287e3-51cb-4&from=paste&id=u4b2df5d8&originHeight=466&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ue58bbbfc-eb04-407a-a4e6-c574f5e1506&title=%E7%89%B9%E8%89%B2%E5%8A%9F%E8%83%BD "特色功能")<br />AT 模式是 Seata 独创的模式，它是基于 2PC 的方案，核心理念是利用数据库 JDBC 加上 Oracle、MySQL 自带的事务方式来对分布式事务进行管理。说起来有点晦涩，下边就结合这张 AT 模式方案图给大家介绍，在 Seata 中关于分布式事务到底需要哪些组件，以及他们都起到了什么样的职能。<br />![Seata 组件图](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017834854-0bd0270c-8193-4e67-b59d-54b6e36e27d7.png#averageHue=%23f6e8d5&clientId=uad0287e3-51cb-4&from=paste&id=u6d5e176e&originHeight=638&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u53b9f767-4d77-48c2-9980-5b3dd06ff90&title=Seata%20%E7%BB%84%E4%BB%B6%E5%9B%BE "Seata 组件图")<br />通过Seata组件图可以看到三个组成部分：

- **第一个是事务协调者（TC）**，它的作用是维护全局和分支事务的状态，驱动全局事务提交或者回滚，这正是前面讲解 2PC 或者 3PC 方案时提到的事务协调者组件的具体实现，TC 由 SEATA 官方提供。
- **第二个是事务管理器（TM）**，事务管理器用于定义全局事务的范围，开始全局事务提交或者回滚全局事务都是由 TM 来决定。
- **第三个是资源管理器（RM）**，他用于管理分支事务处理的资源，并且报告分支事务的状态，并驱动分支事务提交或者回滚。

这些概念可能有些晦涩，通过商城会员采购积分的例子进行讲解。
<a name="p8vzE"></a>
### Seata AT 模式执行过程
![创建订单调用逻辑](https://cdn.nlark.com/yuque/0/2022/png/396745/1672017834939-92600cef-ba5b-4aa6-aa5f-b549b9a5b146.png#averageHue=%23fbfbfa&clientId=uad0287e3-51cb-4&from=paste&id=u6c25c60e&originHeight=641&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=uedc58889-3e63-4acb-b1f5-5f906da731e&title=%E5%88%9B%E5%BB%BA%E8%AE%A2%E5%8D%95%E8%B0%83%E7%94%A8%E9%80%BB%E8%BE%91 "创建订单调用逻辑")<br />这里先给出商城应用中会员采购业务的伪代码。
```
会员采购(){
    订单服务.创建订单();
    积分服务.增加积分();
    库存服务.减少库存();
}
```
在会员采购方法中，需要分别执行**创建订单、增加积分、减少库存**三个步骤完成业务，对于“会员采购”来说方法执行成功，则代表这个全局分布式事务需要提交，如果中间过程出错，则需要全局回滚，这个业务方法本身就决定了全局提交、回滚的时机以及决定了哪些服务需要参与业务处理，因此商城应用的会员采购方法就充当起事务管理器（TM）的角色。<br />而与之对应的在订单服务中创建订单、会员服务中增加积分、库存服务减少库存这些实际产生的数据处理的服务模块，则被称为资源管理器（RM)。<br />最后就是由Seata提供的Seata-Server中间件则提供事务协调者（TC）这个角色，实施全局事务1的提交、回滚命令下发。<br />为了方便理解，画了时序图介绍 Seata 的执行过程。<br />![执行过程](https://cdn.nlark.com/yuque/0/2022/png/396745/1672018148979-052caba4-9697-4b12-8774-f2f349a45f07.png#averageHue=%23faf9f9&clientId=uad0287e3-51cb-4&from=ui&id=ucf598b5a&originHeight=540&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=1753045&status=done&style=none&taskId=u0b93be69-3a8c-494d-878c-1d49129f7f4&title=%E6%89%A7%E8%A1%8C%E8%BF%87%E7%A8%8B "执行过程")<br />第一步，在商城应用（TM）与三个服务（RM）启动后自动向事务协调者Seata-Server（TC）进行注册，让 TC 知晓各个组件的详细信息。<br />第二步，当会员购物时会执行 TM 的“会员采购”方法，当进入方法前 Seata 为 TM 提供的客户端会自动生效，向 TC 发出开启全局事务的请求。<br />第三步，会员采购方法开始执行，会依次执行 3 个服务的新增订单、增加积分、减少库存，在请求送往新的 RM 时，都会向 TC 注册新的分支事务。这些分支事务在处理时不但向业务表写入数据，还会自动向 Seata 强制要求的 UNDO_LOG 回滚日志表写入回滚 SQL 日志。<br />以新增订单事务为例：新增订单时执行的 SQL 语句如下：
```sql
INSERT INTO order(id,...) values(1001,...)
```
与之对应的，Seata 的回滚日志是基于 SQL 反向生成，新增订单创建了 1001 订单，那 Seata会对 SQL 进行解析生成反向的回滚 SQL 日志保存在 UNDO_LOG 表，如下所示：
```sql
DELETE FROM order WHERE id = 1001
```
与之类似会员积分会生成加积分的业务 SQL 以及减积分的回滚 SQL。
```sql
#加积分
UPDATE FROM points SET point = 180 + 20 WHERE mid = 182
#UNDO_LOG表中的减积分SQL
UPDATE FROM points SET point = 200 - 20 WHERE mid = 182
```
第四步，当 RM 的分支事务执行成功后，会自动向 TC 上报分支事务处理成功。<br />第五步，当会员采购方法正确执行，所有 RM 也向 TC 上报分支事务处理成功，在“会员采购”方法退出前，TM 内置的 Seata 客户端会向 TC 自动发起“提交全局事务”请求。TC 收到“提交全局事务”请求，向所有 RM 下达提交分支事务的命令，每一个 RM 在收到提交命令后，会删除之前保存在 UNDO_LOG 表中的回滚日志。<br />但是事情总会有意外，假设某个 RM 分支事务处理失败，此时 TM 便不再向 TC 发起“提交全局事务”，转而发送“回滚全局事务”，TC 收到后，通知所有之前已处理成功的 RM 执行回滚 SQL 将数据回滚。<br />比如 1001 订单在第三步“减少库存”时发现库存不足导致库存服务预处理失败，那全局回滚时第一步订单服务会自动执行删除 1001 订单的回滚 SQL。
```sql
DELETE FROM order WHERE id = 1001
```
以及第二步积分服务会自动执行减少积分的回滚 SQL。
```sql
UPDATE FROM points SET point = 200 - 20 WHERE mid = 182
```
Seata AT模式就是通过执行反向 SQL 达到数据还原的目的，当反向 SQL 执行后便自动从 UNDO_LOG 表中删除。这便是 Seata AT 模式的大致执行过程，在这个过程中可以发现 Seata AT 模式设计的巧妙之处，Seata 为了能做到无侵入的自动实现全局事务提交与回滚，它在 TM端利用了类似于“Spring 声明式事务”的设计，在进入 TM 方法前通知 TC 开启全局事务，在成功执行后自动提交全局事务，执行失败后进行全局回滚。同时在 RM 端也巧妙的采用了 SQL 解析技术自动生成了反向的回滚 SQL 来实现数据还原。<br />在这也思考过，为什么 Seata 要生成反向 SQL，而不是利用数据库自带的排他锁机制处理呢？翻阅资料后理解到它的设计意图，如果采用排它锁机制会导致数据资源被锁死，可能会产生大量的数据资源阻塞，进而存在应用崩溃的风险。而生成反向 SQL 的方案则是在预处理阶段事务便已提交，不会出现长时间数据资源锁定的情况，这样能有效提高并发量。但这样做也有弊端，在研究时发现 Seata 是工作在“读未提交”的隔离级别，高并发环境下容易产生脏读、幻读的情况，这也是需要特别注意的地方。
