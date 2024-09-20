<a name="akLNK"></a>
# 一、事务
<a name="yjySf"></a>
## 事务
事务在一般情况下都是指代数据库事务，它是由系统对数据进行访问和更新的操作所组成的一个逻辑单元，具备原子性（ Atomicity ）、一致性（ Consistency ）、隔离性（ Isolation ）、持久性（ Durability ）四大特征，用来确保无论发生什么情况数据都处在一个合理的状态，在日常开发过程中能够不需要考虑网络波动、服务器宕机等问题。所以从某种角度来看，事务是用来服务应用层的。
<a name="lZwjn"></a>
## 为什么需要分布式事务
单体应用可以依赖数据库事务，将一系列操作限制在一个会话之中同时成功或失败，但是在分布式系统中，每个服务本身的数据库会话之间是隔离的，就无法单纯的依赖数据库事务，在这种场景下想要保证数据的一致性，就需要引入分布式事务。
<a name="GdCRe"></a>
# 二、Seata
<a name="nvVta"></a>
## Seata 的构成
<a name="S4dgd"></a>
### TC ( Transaction Coordinator ) - 事务协调者
维护全局和分支事务的状态，驱动全局事务提交或回滚。
<a name="j8XBD"></a>
### TM ( Transaction Manager ) - 事务管理器
定义全局事务的范围：开始全局事务、提交或回滚全局事务。
<a name="pmQyM"></a>
### RM ( Resource Manager ) - 资源管理器
管理分支事务处理的资源，与 TC 交谈以注册分支事务和报告分支事务的状态，并驱动分支事务提交或回滚。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693877257215-e5a86215-0e31-45f8-9a5f-d39d4b3c0a93.jpeg#averageHue=%23fbeec5&clientId=ue8faff2b-257b-4&from=paste&id=u93249653&originHeight=574&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf99674fa-7942-48fa-a77f-b62c7639bc4&title=)<br />Seata 保证分布式事务的基础是基于 2PC 协议，将事务拆分为准备阶段和提交阶段。在准备阶段，TM 给每个 RM 发送消息， RM 在本地执行事务，此时事务没有提交，在提交阶段， TM 收到了 RM 执行失败或超时则给每个 RM 发送回滚信息，否则则发送提交消息。RM 根据 TM 发送的指令进行提交或回滚消息。
<a name="ZNYKx"></a>
## Seata 的四种模式
<a name="eKgdR"></a>
### AT 模式
AT 模式是 Seata 的默认模式，保证最终一致性，没有业务入侵。<br />在一阶段中 Seate 会拦截业务 Sql ，解析 Sql 找到数据，在数据更新之前，保存 UndoLog 加行锁，二阶段负责业务的整体回滚和提交，如果一阶段中有本地的事务没有通过，就执行全局回滚，否则执行全局提交，回滚用到的就是一阶段记录的 UndoLog ，生成反向 Sql 。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693877257297-24ebfd6a-73fb-4097-a86d-2499e7aa349c.jpeg#averageHue=%23e9e9e4&clientId=ue8faff2b-257b-4&from=paste&id=ud57acdcf&originHeight=432&originWidth=687&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2770b609-8273-46fc-8c12-4ab64d5c45e&title=)
<a name="OH22n"></a>
#### 开启方式
开启 AT 模式只需要在方法上添加注解 `@GlobalTransactional` 即可
```java
@GlobalTransactional
public void method(PurchaseCommodityParam param) {
//......
}
```
<a name="jWURT"></a>
#### AT 模式的脏写问题
两个 Seata 管理的事务对同一条数据进行修改，事务 A 的 RM（ Commodity ）执行成功，本地事务提交，此时库存为 999 ，事务 B 减少 100 的库存，库存为 899 ，此时事务 A 的 RM（ Order ）执行失败，事务 A 整体回滚，库存回滚到 1000 ，那么事务 B 的的操作就被覆盖了。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693877257253-b5c6a8b4-e3a2-4ee5-bda0-839d33cfacf4.jpeg#averageHue=%23fafafa&clientId=ue8faff2b-257b-4&from=paste&id=uceec0308&originHeight=844&originWidth=836&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc7ec1927-29a6-4e42-ba70-b79740e34ed&title=)<br />对于这种情况 Seata 的解决办法是引入了全局锁的概念，一阶段本地事务提交前，需要确保先拿到全局锁 ，拿不到全局锁不能提交本地事务。<br />参考官网给出的案例，TX1、TX2 都是由 Seata 管理的事务，分别对 m 字段进行更新操作，m 的初始值 1000。TX1 先执行，获取数据库锁，对数据进行更新 m = 1000 - 100 = 900，在本地事务提交前，先获取由 Seata 管理的全局锁，本地事务提交释放数据库锁。此时 TX2 开始执行，获取数据库锁后对数据进行更新操作 m = 900 - 100 = 800，在提交前同样去获取 Seata 的全局锁，在 TX1 提交前，字段 m 的全局锁被 TX1 持有，TX2 无法对数据进行更新。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693877257217-e6358f3d-b43d-4abf-b6fd-130ec63fac3d.jpeg#averageHue=%23fefdfb&clientId=ue8faff2b-257b-4&from=paste&id=u4bca3a10&originHeight=649&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8b9375a3-101f-49bb-b537-c4550ec8633&title=)<br />Seata 通过在 'lock_table' 中记录数据库连接信息来实现全局锁，当 RM 提交或回滚后删除锁记录<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693877257250-13bb3721-e42f-4019-b009-a48dcf097e62.jpeg#averageHue=%23dbded8&clientId=ue8faff2b-257b-4&from=paste&id=u683ef08f&originHeight=165&originWidth=710&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue26c0a35-13f5-44c2-b552-a76103c0402&title=)<br />全局锁是基于事务都是 Seata 管理的事务，如果此时有非 Seata 管理的事务对数据改动， Seata 无法完成回滚，此时需要根据日志监控来人工介入。<br />同时在引入全局锁之后会可能会由于锁资源产生死锁的问题 ，TX1 如果在二阶段需要全局回滚，则 TX1 需要重新获取数据库锁回滚数据，才能释放全局锁，但是此时数据库锁被 TX2 所持有，TX2 需要获取全局锁才能提交本地事务，这里就产生了死锁，直到 TX2 全局锁等待超时，放弃获取全局锁并回滚本地事务，TX1 才可以回滚。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693877257636-0c6e353a-acc9-4878-a4c5-6badb4e78a8c.jpeg#averageHue=%23fcf9f5&clientId=ue8faff2b-257b-4&from=paste&id=uc6aed9a7&originHeight=723&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaaa90142-f10b-462c-9081-0e87d8fe438&title=)
<a name="FF3BQ"></a>
#### AT 模式的脏读问题
Seata 在一阶段执行过程中部分 RM 提交本地事务，二阶段事务发生回滚，在一阶段和二阶段之间如果有业务对数据进行查询读到的就是脏数据。Seata 的 AT 模式是保证分布式事务的最终一致性，在过程中无法保证数据的隔离，脏读是无法避免的问题。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693877257727-b3ad4f91-99b8-4cac-8f7c-57368910a715.jpeg#averageHue=%23fafafa&clientId=ue8faff2b-257b-4&from=paste&id=u29011dc8&originHeight=723&originWidth=812&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uefa695c5-8ed1-412e-bf1b-77c0be6ddc1&title=)
<a name="vhYOL"></a>
### XA 模式
Seata 的 XA 模式是基于数据库本身的特性来实现的，是一种强一致性的事务。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693877257814-92e5f53e-26da-4168-867f-52d124306e8e.jpeg#averageHue=%23efd695&clientId=ue8faff2b-257b-4&from=paste&id=ue7f4d61d&originHeight=697&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubf9c511e-da5a-47d0-9e91-2a4e3b4b971&title=)<br />在一阶段中 TC 向 RM 通知执行业务，但是不提交，将执行结果交给事务协调者，如果没有出现问题则进行第二阶段，在二阶段中 RM 一起提交事务。如果第一阶段其中一个事务出现了错误，那么其他的事务都进行回滚。XA 与 AT 模式相比在一阶段中 XA 不会提交本地事务，就可以避免保证只保证最终一致性所引发的脏读问题，同样的由于本地事务不提交 XA 模式也会占用更多的数据库资源。
<a name="XhP8D"></a>
#### 开启方式
可以在添加配置，设置为 XA 模式
```yaml
seata:
  data-source-proxy-mode: XA
```
也可以通过添加数据源代理的方式开启
```java
public class XADataSourceConfig {

    @Bean("dataSource")
    public DataSource dataSource(DruidDataSource druidDataSource) {
        return new DataSourceProxyXA(druidDataSource);
    }
}
```
<a name="polPP"></a>
#### XA 模式的死锁问题
RM 服务在执行过程中挂了， TC 收不到 RM 的结束命令，那么其他的RM都会持有数据库资源不释放，这里就产生了死锁，直到等待超时 TC 才会向 RM 下达回滚命令。
<a name="Ft7f2"></a>
### TCC 模式
TCC 与 AT 模式类似，AT 模式基于 支持本地 ACID 事务的关系型数据库：<br />一阶段 prepare 行为：在本地事务中，一并提交业务数据更新和相应回滚日志记录。<br />二阶段 commit 行为：马上成功结束，自动异步批量清理回滚日志。<br />二阶段 rollback 行为：通过回滚日志，自动生成补偿操作，完成数据回滚。<br />相应的，TCC 模式，不依赖于底层数据资源的事务支持：<br />一阶段 prepare 行为：调用自定义的 prepare 逻辑。<br />二阶段 commit 行为：调用自定义的 commit 逻辑。<br />二阶段 rollback 行为：调用自定义的 rollback 逻辑。<br />总的来说 TCC 相比于 AT 由于是根据业务来自己实现 Commit 和 Rollback 所以不受数据库类型的限制 ，AT模式支持的数据包括 MySQL、Oracle、PostgreSQL、TiDB、MariaDB，TCC 模式在 1.4.2 版本及之前是不依赖数据源的，从 1.5.1 版本开始增加了防悬挂措施 ， 就需要数据源的支持。XA 模式只支持实现了 XA 协议的数据库，包括 MySQL、Oracle、PostgreSQL 和 MariaDB 。
<a name="T2mGg"></a>
####  开启方式
```java
public interface TCCService {

    @TwoPhaseBusinessAction(name = "prepareMethod",commitMethod = "commit",rollbackMethod = "rollback")
    public void prepareMethod(@BusinessActionContextParameter(paramName = "param") MethodParam param);

    public boolean commit(BusinessActionContext businessActionContext);

    public boolean rollback(BusinessActionContext businessActionContext);

}
```
<a name="SSdem"></a>
#### TC 的空回滚、业务悬挂、超时重试
TM 下发命令给 RM 其中有部分 RM 执行阻塞，TC 此时感知异常发出回滚命令，执行过 Perpare 的 RM 回滚是没问题的，但是没有执行过 Perpare 的 RM 也要执行 Rollback 就造成了空回滚，空回滚之后 RM 再次执行 Perpare 就会成为业务悬挂。在 Commit Rollback 阶段 TC 没有收 RM 的执行响应需要进行 Perpare 的重试，就是超时重试。<br />空回滚、业务悬挂、超时重试的产生原因本质上就是 RM 没有对 TC 的消息顺序做校验以及幂等，在1.4.2版本及之前，需要自己在业务中处理这些问题，在 Seata 的 1.5.1 版本之后，Seata 增加了一张事务控制表来解决这个问题，通过全局 ID 以及状态来对 RM 的执行过程进行控制。
<a name="iQt5E"></a>
### Saga 模式
Seata 的 Saga 模式是基于状态机引擎实现的编排式分布式事务，状态机最小的执行单位是节点，对应一个服务调用，可以对其配置补偿节点，通过链路的串联编排出调用流程。Seata 的 Saga 模式与 XA 、 AT 、 TCC 模式相比较于复杂，可以参考 Seata 官网提供的 Demo 来进行学习。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1693877257864-dc32c30d-0e4b-4b51-8c5c-d941b02726c6.jpeg#averageHue=%23fcfbfb&clientId=ue8faff2b-257b-4&from=paste&id=ua6328439&originHeight=968&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5802037d-3869-4556-993a-3bf76872185&title=)
<a name="oaRmb"></a>
# 三、总结
在分布式系统中能否保证数据一致性是一项技术难题，Seata 在解决了分布式事务的场景同时也带来了编码复杂度的上升以及性能下降等等问题，也可能由于中间件本身引发一些问题。在开发生涯中不是每一个项目都适合引入分布式事务的解决方案，作为一名开发者需要在这之间有所取舍，本文更多是给大家提供一份解决问题的思路。
<a name="QqnJZ"></a>
# 参考资料

- Seata 官网 [https://seata.io/](https://seata.io/)
