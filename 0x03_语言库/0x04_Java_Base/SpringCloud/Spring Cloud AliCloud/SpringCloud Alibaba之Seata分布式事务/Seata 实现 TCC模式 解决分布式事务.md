Java SpringCloudAlibaba Seata 分布式事务<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947990854-6092c5d3-9fbc-4825-a844-e21edd3ce968.webp#clientId=u31b694f2-dec2-4&from=paste&id=ua0cf2da6&originHeight=498&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u91b117a0-e170-4d0b-8b01-a430cb46cca&title=)
<a name="DNx2V"></a>
## 什么是TCC模式？
TCC（Try Confirm Cancel）方案是一种应用层面侵入业务的两阶段提交。是目前最火的一种柔性事务方案，其核心思想是：针对每个操作，都要注册一个与其对应的确认和补偿（撤销）操作。<br />TCC分为两个阶段，分别如下：

- 第一阶段：Try（尝试），主要是对业务系统做检测及资源预留 (加锁，锁住资源)
- 第二阶段：本阶段根据第一阶段的结果，决定是执行confirm还是cancel
   1. Confirm（确认）：执行真正的业务（执行业务，释放锁）
   2. Cancle（取消）：是预留资源的取消（出问题，释放锁）

![TCC](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947991204-be5aed3a-8a1e-4882-8082-4719e7956d0b.webp#clientId=u31b694f2-dec2-4&from=paste&id=ub2584542&originHeight=491&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=udef106a5-be7c-4614-b220-ac1d564f43a&title=TCC "TCC")<br />为了方便理解，下面以电商下单为例进行方案解析，这里把整个过程简单分为扣减库存，订单创建 2 个步骤，库存服务和订单服务分别在不同的服务器节点上。<br />假设商品库存为 100，购买数量为 2，这里检查和更新库存的同时，冻结用户购买数量的库存，同时创建订单，订单状态为待确认。
<a name="W6bqn"></a>
#### ①Try 阶段
TCC 机制中的 Try 仅是一个初步操作，它和后续的确认一起才能真正构成一个完整的业务逻辑，这个阶段主要完成：

- 完成所有业务检查( 一致性 ) 。
- 预留必须业务资源( 准隔离性 ) 。
- Try 尝试执行业务。

![Try阶段](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947990868-e08d3f98-02af-4ea5-8a4a-5f82907937b2.webp#clientId=u31b694f2-dec2-4&from=paste&id=u22dac362&originHeight=381&originWidth=556&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u9d89e01e-b32a-45d8-a0cf-aa888877222&title=Try%E9%98%B6%E6%AE%B5 "Try阶段")
<a name="NzSPs"></a>
#### ②Confirm / Cancel 阶段
根据 Try 阶段服务是否全部正常执行，继续执行确认操作（Confirm）或取消操作（Cancel）。<br />Confirm 和 Cancel 操作满足幂等性，如果 Confirm 或 Cancel 操作执行失败，将会不断重试直到执行完成。<br />Confirm：当 Try 阶段服务全部正常执行， 执行确认业务逻辑操作，业务如下图：<br />![Try->Confirm](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947990858-62d39f9c-6303-4e34-b02d-e136e9d8a613.webp#clientId=u31b694f2-dec2-4&from=paste&id=u8d474777&originHeight=384&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua78ce253-303f-404c-9d33-5f50e8c7c71&title=Try-%3EConfirm "Try->Confirm")<br />这里使用的资源一定是 Try 阶段预留的业务资源。在 TCC 事务机制中认为，如果在 Try 阶段能正常的预留资源，那 Confirm 一定能完整正确的提交。<br />Confirm 阶段也可以看成是对 Try 阶段的一个补充，Try+Confirm 一起组成了一个完整的业务逻辑。<br />Cancel：当 Try 阶段存在服务执行失败， 进入 Cancel 阶段，业务如下图：<br />![Try-Cancel](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947990830-964fb371-4fd4-4efa-8a29-853486a35f56.webp#clientId=u31b694f2-dec2-4&from=paste&id=u061d35ec&originHeight=384&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u4089bcd9-46bb-45e9-991d-13fc84ea358&title=Try-Cancel "Try-Cancel")<br />Cancel 取消执行，释放 Try 阶段预留的业务资源，上面的例子中，Cancel 操作会把冻结的库存释放，并更新订单状态为取消。
<a name="Rg7oa"></a>
## TCC模式的三种类型？
业内实际生产中对TCC模式进行了扩展，总结出了如下三种类型，其实从官方的定义中无此说法，不过是企业生产中根据实际的需求衍生出来的三种方案。
<a name="rGN76"></a>
### 1、通用型 TCC 解决方案
通用型TCC解决方案是最经典的TCC事务模型的实现，正如第一节介绍的模型，所有的从业务都参与到主业务的决策中。<br />![通用型TCC](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947991213-c1d0b524-d107-4500-9519-323c7b2a887d.webp#clientId=u31b694f2-dec2-4&from=paste&id=u85ddec80&originHeight=554&originWidth=654&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u3b961419-1903-475a-98d0-0e3042948c1&title=%E9%80%9A%E7%94%A8%E5%9E%8BTCC "通用型TCC")<br />适用场景：<br />由于从业务服务是同步调用，其结果会影响到主业务服务的决策，因此通用型 TCC 分布式事务解决方案适用于执行时间确定且较短的业务，比如电商系统的三个核心服务：订单服务、账户服务、库存服务。<br />这个三个服务要么同时成功，要么同时失败。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947991346-3fe181f6-6299-480d-ba06-c47cf5a582d2.webp#clientId=u31b694f2-dec2-4&from=paste&id=ub48d143e&originHeight=382&originWidth=1018&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucf23cbd2-9290-4b8f-a49c-0c034ccc685&title=)<br />当库存服务、账户服务的第二阶段调用完成后，整个分布式事务完成。
<a name="JS1C8"></a>
### 2、异步确保型 TCC 解决方案
异步确保型 TCC 解决方案的直接从业务服务是可靠消息服务，而真正的从业务服务则通过消息服务解耦，作为消息服务的消费端，异步地执行。<br />![异步确保型](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947991683-c29b289f-9ce2-490f-8540-18a13be806ee.webp#clientId=u31b694f2-dec2-4&from=paste&id=ud5b5239c&originHeight=555&originWidth=1004&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u843d5603-ecc6-4ff8-8b3a-a3e47a33962&title=%E5%BC%82%E6%AD%A5%E7%A1%AE%E4%BF%9D%E5%9E%8B "异步确保型")<br />可靠消息服务需要提供 Try，Confirm，Cancel 三个接口。Try 接口预发送，只负责持久化存储消息数据；Confirm 接口确认发送，这时才开始真正的投递消息；Cancel 接口取消发送，删除消息数据。<br />消息服务的消息数据独立存储，独立伸缩，降低从业务服务与消息系统间的耦合，在消息服务可靠的前提下，实现分布式事务的最终一致性。<br />此解决方案虽然增加了消息服务的维护成本，但由于消息服务代替从业务服务实现了 TCC 接口，从业务服务不需要任何改造，接入成本非常低。<br />适用场景：<br />由于从业务服务消费消息是一个异步的过程，执行时间不确定，可能会导致不一致时间窗口增加。因此，异步确保性 TCC 分布式事务解决方案只适用于对最终一致性时间敏感度较低的一些被动型业务（从业务服务的处理结果不影响主业务服务的决策，只被动的接收主业务服务的决策结果）。比如会员注册服务和邮件发送服务：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947991492-5ba13e8e-4df7-4f28-af60-94b181bec19e.webp#clientId=u31b694f2-dec2-4&from=paste&id=uc4c1c747&originHeight=382&originWidth=1018&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua6f0f0e4-46cc-4582-b01e-f8fe9fa9578&title=)
<a name="FKtJo"></a>
### 3、补偿型 TCC 解决方案
补偿型 TCC 解决方案与通用型 TCC 解决方案的结构相似，其从业务服务也需要参与到主业务服务的活动决策当中。但不一样的是，前者的从业务服务只需要提供 Do 和 Compensate 两个接口，而后者需要提供三个接口。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947991829-f84d65be-3b38-47b2-8f6a-732bda30d655.webp#clientId=u31b694f2-dec2-4&from=paste&id=u176b5384&originHeight=554&originWidth=654&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucd69f54e-d250-4c9e-bc47-b2623aec191&title=)<br />Do 接口直接执行真正的完整业务逻辑，完成业务处理，业务执行结果外部可见；Compensate 操作用于业务补偿，抵消或部分抵消正向业务操作的业务结果，Compensate操作需满足幂等性。<br />与通用型解决方案相比，补偿型解决方案的从业务服务不需要改造原有业务逻辑，只需要额外增加一个补偿回滚逻辑即可，业务改造量较小。但要注意的是，业务在一阶段就执行完整个业务逻辑，无法做到有效的事务隔离，当需要回滚时，可能存在补偿失败的情况，还需要额外的异常处理机制，比如人工介入。<br />适用场景：<br />由于存在回滚补偿失败的情况，补偿型 TCC 分布式事务解决方案只适用于一些并发冲突较少或者需要与外部交互的业务，这些外部业务不属于被动型业务，其执行结果会影响主业务服务的决策。<br />以上部分内容参考自：[https://seata.io/zh-cn/blog/tcc-mode-applicable-scenario-analysis.html?utm_source=gold_browser_extension](https://seata.io/zh-cn/blog/tcc-mode-applicable-scenario-analysis.html?utm_source=gold_browser_extension)
<a name="yOwtr"></a>
## TCC事务模式的落地实现
当然Seata支持的事务模式不局限于AT模式，还有TCC模式、SAGA模式、XA模式，下面整合一下TCC模式。
<a name="clBJJ"></a>
### 1、演示场景
就以电商系统中下订单为例，为了演示，直接去掉账户服务，以订单服务、库存服务为例介绍。<br />具体的逻辑如下：

1. 客户端调用下订单接口
2. 扣库存
3. 创建订单
4. 请求完成

根据上面的逻辑可知，订单服务肯定是主业务服务，事务的发起方，库存服务是从业务服务，参与事务的决策。<br />Seata的AT模式解决方案伪代码如下：
```java
@GlobalTransactional
public Result<Void> createOrder(Long productId,Long num,.....){
    //1、扣库存
    reduceStorage();
    //2、创建订单
    saveOrder();
}
```
`@GlobalTransactional`这个注解用于发起一个全局事务。<br />但是AT模式有局限性，如下：

- 性能低，锁定资源时间太长
- 无法解决跨应用的事务

因此对于要求性能的下单接口，可以考虑使用TCC模式进行拆分成两阶段执行，这样整个流程锁定资源的时间将会变短，性能也能提高。<br />此时的TCC模式的拆分如下：<br />1、一阶段的Try操作<br />TCC模式中的Try阶段其实就是预留资源，在这个过程中可以将需要的商品数量的库存冻结，这样就要在库存表中维护一个冻结的库存这个字段。<br />伪代码如下：
```java
@Transactional
public boolean try(){
  //冻结库存
  frozenStorage();
  //生成订单，状态为待确认
  saveOrder();
}
```
注意：`@Transactional`开启了本地事务，只要出现了异常，本地事务将会回滚，同时执行第二阶段的`cancel`操作。<br />2、二阶段的`confirm`操作<br />`confirm`操作在一阶段`try`操作成功之后提交事务，涉及到的操作如下：

1. 释放try操作冻结的库存（冻结库存-购买数量）
2. 生成订单

伪代码如下：
```java
@Transactional
public boolean confirm(){
    //释放掉try操作预留的库存
    cleanFrozen();
    //修改订单，状态为已完成
    updateOrder();
    return true;
}
```
注意：这里如果返回false，遵循TCC规范，应该要不断重试，直到confirm完成。<br />3、二阶段的cancel操作<br />cancel操作在一阶段try操作出现异常之后执行，用于回滚资源，涉及到的操作如下：

1. 恢复冻结的库存（冻结库存-购买数量、库存+购买数量）
2. 删除订单

伪代码如下：
```java
@Transactional
public boolean cancel(){
    //释放掉try操作预留的库存
    rollbackFrozen();
    //修改订单，状态为已完成
    delOrder();
    return true;
}
```
注意：这里如果返回false，遵循TCC规范，应该要不断重试，直到`cancel`完成。
<a name="SzsNd"></a>
### 2、TCC事务模型的三个异常
实现TCC事务模型涉及到的三个异常是不可避免的，实际生产中必须要规避这三大异常。
<a name="zQ0w6"></a>
#### 1、空回滚
定义：在未调用`try`方法或`try`方法未执行成功的情况下，就执行了`cancel`方法进行了回滚。<br />怎么理解呢？未调用`try`方法就执行了`cancel`方法，这个很容易理解，既然没有预留资源，那么肯定是不能回滚。<br />try方法未执行成功是什么意思？<br />可以看上节中的第一阶段try方法的伪代码，由于try方法开启了本地事务，一旦try方法执行过程中出现了异常，将会导致try方法的本地事务回滚（注意这里不是cancel方法回滚，而是try方法的本地事务回滚），这样其实try方法中的所有操作都将会回滚，也就没有必要调用cancel方法。<br />但是实际上一旦try方法抛出了异常，那么必定是要调用cancel方法进行回滚，这样就导致了空回滚。<br />解决方案：<br />解决逻辑很简单：在cancel方法执行操作之前，必须要知道try方法是否执行成功。
<a name="WUsDX"></a>
#### 2、幂等性
TCC模式定义中提到：如果confirm或者cancel方法执行失败，要一直重试直到成功。<br />这里就涉及了幂等性，confirm和cancel方法必须保证同一个全局事务中的幂等性。<br />解决方案：<br />解决逻辑很简单：对付幂等，自然是要利用幂等标识进行防重操作。
<a name="gXPSm"></a>
#### 3、悬挂
事务协调器在调用 TCC 服务的一阶段 Try 操作时，可能会出现因网络拥堵而导致的超时，此时事务管理器会触发二阶段回滚，调用 TCC 服务的 Cancel 操作，Cancel 调用未超时；<br />在此之后，拥堵在网络上的一阶段 Try 数据包被 TCC 服务收到，出现了二阶段 Cancel 请求比一阶段 Try 请求先执行的情况，此 TCC 服务在执行晚到的 Try 之后，将永远不会再收到二阶段的 Confirm 或者 Cancel ，造成 TCC 服务悬挂。<br />解决方案：<br />解决逻辑很简单：在执行try方法操作资源之前判断cancel方法是否已经执行；同样的在cancel方法执行后要记录执行的状态。
<a name="KdIMw"></a>
#### 4、总结
针对以上三个异常，落地的解决方案很多，比如维护一个事务状态表，每个事务的执行阶段全部记录下来。

- 幂等：在执行confirm或者cancel之前根据事务状态表查询当前全局事务是否已经执行过confirm或者cancel方法
- 空回滚：在执行cancel之前才能根据事务状态表查询当前全局事务是否已经执行成功try方法
- 悬挂：在执行try方法之前，根据事务状态表查询当前全局事务是否已经执行过cancel方法
<a name="rRTSE"></a>
## Seata整合TCC实现
这里只介绍关键代码。<br />源码目录如下：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947991723-7e7dc852-2cfd-4eac-a1e6-522eb42c385a.webp#clientId=u31b694f2-dec2-4&from=paste&id=ud201a987&originHeight=524&originWidth=688&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub0a36848-b93c-48b2-8981-b9b65765dfc&title=)<br />项目启动所需要的相关文件如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947991852-5a6fd92f-80b7-4b39-9b1d-5d1b29064b54.webp#clientId=u31b694f2-dec2-4&from=paste&id=uf2bfe938&originHeight=305&originWidth=571&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u22c27528-e252-46ad-8eae-53a3b229d46&title=)<br />nacos目录中的SEATA_GROUP是Seata事务服务端和客户端所需要的相关配置，直接导入nacos即可。<br />seata目录中的conf是1.3.0版本服务端的配置<br />SQL目录是相关的几个数据库。
<a name="kLTHt"></a>
### 1、TCC接口定义
在order-boot模块创建OrderTccService，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947991928-e09eb8c2-7e88-4f17-ba15-49db6f6b117a.webp#clientId=u31b694f2-dec2-4&from=paste&id=u1b3a01d9&originHeight=681&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucea80859-2fc5-43ba-b16f-f241d71bbe5&title=)<br />代码中注释已经很完整了，下面挑几个重点介绍一下：

1. `@LocalTCC`：该注解开启TCC事务
2. `@TwoPhaseBusinessAction`：该注解标注在try方法上，其中的三个属性如下：
   1. `name`：TCC事务的名称，必须是唯一的
   2. `commitMethod`：confirm方法的名称，默认是commit
   3. `rollbackMethod`：cancel方法的名称，，默认是rollback
3. confirm和cancel的返回值尤为重要，返回false则会不断的重试。
<a name="fZ6DM"></a>
### 2、TCC接口实现
定义有了，总要实现，如下：
<a name="UAQqE"></a>
#### 1、try方法
![try方法](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947992146-dd4c9229-f05e-41f0-9499-cae9b90f7d34.webp#clientId=u31b694f2-dec2-4&from=paste&id=u2cb68465&originHeight=661&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u2a28877d-7078-43d5-8fc9-c2a45113ffe&title=try%E6%96%B9%E6%B3%95 "try方法")<br />①处的代码是为了防止悬挂异常，从事务日志表中获取全局事务ID的状态，如果是cancel状态则不执行。<br />②处的代码冻结库存<br />③处的代码生成订单，状态为待确认<br />④处的代码向幂等工具类中添加一个标记，key为当前类和全局事务ID，value为当前时间戳。<br />注意：必须要开启本地事务，如上代码使用`@Transactional`开启本地事务
<a name="jPQgN"></a>
#### 2、confirm方法
![confirm方法](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947992579-2a52056d-aee2-46a0-a711-e8b66e6f04b0.webp#clientId=u31b694f2-dec2-4&from=paste&id=u8d7a9bdb&originHeight=640&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ua6c5b350-5663-415f-96bb-eb5c644ca97&title=confirm%E6%96%B9%E6%B3%95 "confirm方法")<br />①处的代码从幂等工具类中根据当前类和全局事务ID获取值，由于try阶段执行成功会向其中添加值，confirm方法执行成功会移出这个值，因此在confirm开头判断这个值是否存在就起到了幂等效果，防止重试的效果。<br />⑥处的代码从幂等工具类中移出try方法中添加的值。<br />②处的代码是从BusinessActionContext中获取try方法中的入参。<br />③处的代码是释放掉冻结的库存<br />④处的代码是修改订单的状态为已完成。<br />注意：1. 开启本地事务  2. 注意返回值，返回false时将会重试
<a name="lWOhd"></a>
#### 3、cancel方法
![cancel方法](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947992575-6479eda6-d37c-4391-a6ef-fe02f3e1a734.webp#clientId=u31b694f2-dec2-4&from=paste&id=u39f338a5&originHeight=722&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u13836243-d9d3-4c64-bf3d-0406f80c96e&title=cancel%E6%96%B9%E6%B3%95 "cancel方法")<br />①处的代码是向事务日志记录表中插入一条数据，标记当前事务进入cancel方法，用来防止悬挂，这个和try方法中的①处的代码相呼应。<br />②处的代码是为了防止幂等和空回滚，因为只有当try方法中执行成功幂等工具类中对应的当前类和全局事务ID才会存储该值。这样既防止了幂等，也防止了空回滚。<br />③处的代码恢复冻结的库存。<br />④处的代码删除这笔订单<br />⑤处的代码是移出幂等工具类当前类和全局事务ID对应的值。
<a name="rV5RV"></a>
### 3、如何防止TCC模型的三个异常？
实现方法有很多，有些案例是全部使用事务日志表记录当前的状态，这样完美的解决了幂等、空回滚、悬挂的问题。<br />这里为了方便，使用了两种方案，如下：
<a name="KfPQM"></a>
#### 1、幂等、空回滚
使用了一个幂等工具类，其中是个Map，key为当前类和全局事务ID，value是时间戳。<br />代码如下：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947992312-fc2d254b-abd9-4f31-b215-2cb82f8cc790.webp#clientId=u31b694f2-dec2-4&from=paste&id=ud32e7f0b&originHeight=389&originWidth=1044&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u497ca97f-f0f9-45fd-aae7-c08d281cc73&title=)<br />思路如下：

1. 在try方法最后使用幂等工具类中的add方法添加值
2. 在confirm、cancel方法中使用幂等工具类中的remove方法移出值
3. 在confirm、cancel方法中使用幂等工具类中get方法获取值，如果为空，则表示已经执行过了，直接返回true，这样既防止了幂等，也防止了空回滚。
<a name="ycv20"></a>
#### 2、悬挂
悬挂的实现依靠的是事务日志表，表结构如下：
```sql
CREATE TABLE `transactional_record` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT,
  `xid` varchar(100) NOT NULL,
  `status` int(1) DEFAULT NULL COMMENT '1. try  2 commit 3 cancel ',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```
其中的xid是全局事务ID，status是事务的状态。<br />其他的字段自己可以扩展<br />解决悬挂问题的逻辑如下：

1. cancel方法中将当前全局事务ID记录到事务日志表中，状态为cancel
2. try方法执行资源操作前检查事务日志表中当前全局事务ID是否已经是cancel状态
<a name="qBO17"></a>
### 4、创建订单的业务方法
上面只是完成了TCC的三个方法，主业务事务发起方还未提供，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641947992445-9b9dd373-2ce6-4b1e-8197-40e1cdca3c58.webp#clientId=u31b694f2-dec2-4&from=paste&id=u7887d9b1&originHeight=521&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u096fcd7f-7042-452f-b323-60cc5a3e83f&title=)<br />`@GlobalTransactional`这个注解开启了全局事务，是事务的发起方。<br />内部直接调用的TCC的try方法。
<a name="ZWLmh"></a>
### 5、其他的配置
以上只是列出了关键的步骤，剩余其他的配置自己根据案例源码完善，如下：

1. 接口测试
2. 整合nacos
3. 整合feign
4. 整合seata，TCC模式中的配置和AT模式的Seata配置相同

注意：一定要配置Seata的事务组`tx-service-group`。
