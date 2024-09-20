<a name="Zmo4B"></a>
## 前言
在电商领域，很多业务对象都是有状态的，且这些对象的状态又多又复杂。硬编码的方式已经不适合管理当前复杂业务对象的状态。为了适配复杂多变的业务，可以使用状态机来管理状态，统一定义业务对象状态和状态的流转。接下来，本文会重点介绍状态机相关的概念和使用场景。
<a name="L0RqA"></a>
## 定义
在介绍状态机之前，先介绍一个工作流（WorkFlow），初学者通常容易将两个概念混淆。工作流（WorkFlow），大体是指业务过程（整体或者部分）在计算机应用环境下的自动化，是对工作流程及其各操作步骤之间业务规则的描述。在计算机系统中，工作流属于计算机支持的协同工作（CSCW）的一部分。<br />状态机是工作流（WorkFlow）的一种类型，包括顺序工作流（Sequential）和状态机工作流（State Machine）。状态机是有限状态自动机的简称，是现实事物运行规则抽象而成的一个数学模型。<br />简单说明一下状态机和流程图这两个概念的区别。状态机用来描述一个特定对象的所有可能状态，以及由于各种事件的发生而引起的状态之间的转移。而流程图则用于表示完成某件事情中的各个活动过程，关键的是每一个步骤。

| 状态机（WorkFlow） | 工作流（State Machine） |
| --- | --- |
| 关注单个任务 | 关注状态流转 |
| 无循环 | 可以简单的实现循环 |
| 实现简单 | 比较麻烦，需要记录任务当前状态 |
| 串行表达，不是很灵活 | 表达更灵活 |
| 运行销量高 | 效率低，可并行 |

<a name="Dyz9h"></a>
## 状态机选型
流程引擎易滥用，但状态机却实用且使用广泛，主要有以下两个原因：

1. 实现。最简单、轻量的状态机用一个 Enum 就能实现，基本是零成本。
2. 使用状态机的 DSL 来表达状态的流转，语义会更加清晰，会增强代码的可读性和可维护性。

然而，当业务场景比较复杂时，还是会超出 Enum 仅支持线性状态流转的范畴，因此不得不考虑一下开源状态机。<br />着重看了两个状态机引擎的实现，一个是 Spring StateMachine，一个是 Squirrel StateMachine，他们的优点是功能很完备，缺点也是功能很完备。<br />就实际项目而言（其实大部分项目都是如此），实在不需要那么多状态机的高级玩法：比如状态的嵌套（Substate），状态的并行（Parallel，Fork，Join）、子状态机等等。<br />且开源状态机大多都是有状态的，使用分布式多线程来实现，无法做到线程安全，代码需要用到锁同步。每一次状态机在接收请求的时候，都不得不重新 Build 一个新的状态机实例，就导致开源状态机性能差。

| <br /> | 优点 | 缺点 |
| --- | --- | --- |
| Spring StateMachine | <br />1. 强大的生命周期管理<br />2. 易于集成<br />3. 良好的文档和社区支持<br /> | <br />1. 学习曲线较陡峭<br />2. 可能增加项目复杂性<br /> |
| Squirrel StateMachine | <br />1. 轻量级<br />2. 简单易用<br />3. 性能高效<br /> | <br />1. 功能相对有限<br /> |
| Cola-StateMachine | <br />1. 高度可扩展<br />2. 语义清晰、可读性强<br />3. 线程安全<br /> | <br />1. 文档和社区支持相对较少<br /> |

最终选用了一个开源的状态机引擎 Cola-StateMachine。
<a name="hcfvx"></a>
## Cola-StateMachine 简介
COLA 框架的状态机组件是一种小巧、无状态、简单、轻量、性能极高的状态机 DSL 实现，解决业务中的状态流转问题。<br />Cola-StateMachine 使用的是 Internal DSL 这样一种通用语言的特定用法，用这种 DSL 写出的程序有一种自定义语言的风格，与其所使用的宿主语言有所区别。Cola-StateMachine 使用 Java 实现，最简单，实现成本也最低，但是不支持“外部配置”。<br />Cola-StateMachine 中的核心概念：

1. State：状态
2. Event：事件，状态由事件触发，引起变化
3. Transtition：流转，表示从一个状态到另一个状态
4. External Transititon：外部流转，两个不同状态之间的流转
5. Interal Transtition：内部流转，同一个状态之间的流转
6. Condition：条件，标识是否允许到达某个状态
7. Action：动作，到达某个状态以后，可以做什么
8. StateMachine：状态

Cola-statemachine 工作模式![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697516987340-f7f541a8-1de2-4191-b900-9313b3cf68c2.png#averageHue=%23f8f7f7&clientId=uc764a0d2-3b18-4&from=paste&id=uc1969690&originHeight=503&originWidth=782&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub82f0ee5-53b5-4f20-8af4-118219b9b56&title=)<br />Cola-StateMachine 核心代码
```java
builder.externalTransition()
  .from(States.STATE1)
  .to(States.STATE2)
  .on(Events.EVENT1)
  .when(checkCondition())
  .perform(doAction());
```
<a name="ZM3Jd"></a>
## 实践使用
<a name="HFDDr"></a>
### 使用场景
在设计电商系统订单模块时，订单会涉及各种状态以及状态与状态之间的流转，可扩展性、可维护性会是开发者关注的重点。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697516987320-c110e538-2bed-4f93-be81-b92762980bff.png#averageHue=%231c1c1c&clientId=uc764a0d2-3b18-4&from=paste&id=u64f2e79d&originHeight=443&originWidth=824&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u79257daf-04f6-444b-af0c-11478cb1364&title=)<br />订单状态包括：初始化、待支付、待接单、待发货、待收货...<br />订单事件包括：创建订单、支付成功、接单、发货、取消订单...<br />需要考虑如下问题：

1. 当订单状态增加时，如何尽可能少的改动或改动对历史影响不大？
2. 如果在同一入口调用，每个事件的处理方法需要的入参都有所不同，如何处理？
3. 有可能某个事件，在不同平台(买家端、卖家后台、管理平台)的处理逻辑也有些不同，如何处理？

因为订单涉及到很多状态的流转，选择使用状态机引擎来表达状态流转。因为状态机 DSL 带来的表达能力，相比较于 if-else 的代码，要更优雅更容易理解。另一方面，状态机很简单，不像流程引擎那么华而不实。
<a name="sCICs"></a>
#### 1、引入依赖
第一步：引入依赖
```xml
<dependency>
  <groupId>com.alibaba.cola</groupId>
  <artifactId>cola-component-statemachine</artifactId>
  <version>4.2.1</version>
</dependency>
```
<a name="X1TYB"></a>
#### 2、订单基本配置
第二步：定义状态机中基础属性：状态、时间、状态机。在本次实践中使用 `OrderStatusEnum` 定义订单状态，使用 `OrderEventParam`定义订单事件，使用 `OrderStateMachineIdEnum` 定义状态机。
```java
//订单状态
public enum OrderStatusEnum {
    INIT("初始化", 0),
    PAY_ONLINE("待支付", 1),
    WAITING_FOR_RECEIVED("待接单", 2),
    WAITING_DELIVERY("待发货", 3),
    PART_DELIVERY("部分发货", 4),
    DELIVER_ALL("待收货", 5),
    RECEIVED("已收货", 6),
    DONE("已完成", 7),
    CANCEL("已关闭", 8);
}

//订单事件
public enum OrderEventEnum {
    CREATE_ORDER(1, "创建订单"),
    REPAY(2, "支付"),
    CANCEL_ORDER(3, "取消订单"),
    TAKE_ORDER(4, "接单"),
    REJECT_ORDER(5, "拒单"), 
    DELIVERY_PART(6, "部分发货"),
    DELIVERY_ALL(7, "全部发货"),
    CONFIRM_RECEIPT(8, "确认收货"),
    EXTEND_RECEIPT(9, "延长收货"),
    COMPLETE(10, "交易完成");
}

//订单状态机ID 枚举
public enum OrderStateMachineIdEnum {
    ORDER_OF_SALE("ORDER_OF_SALE",
                  OrderBizEnum.SALE_ORDER, "订单状态机");
}
```
<a name="x2lm2"></a>
#### 3、订单流转状态机
第三步：定义订单状态流转事件。具体使用 Builder 和 Fluent Interface 的方式，确保代码调用的顺序。比如，在 From 的后面只能调用To，从而保证了状态机构建的语义正确性和连贯性。使用状态机 Builder，确认状态流转模式（Transition），接收状态（From），定义动作（On），检查条件（When），执行事件（Perform），然后返回目标状态（To）。<br />当有新订单事件的增加时，在此状态机中增加相应事件即可，同时维护好订单事件与事件实现方法之间的关系。
```java
public class OrderOfSaleStateMachine implements OrderStateMachine {
    public void initialize() {

        //对状态机开始构建，并在StateMachineFactory中注册
        StateMachineBuilder<OrderStatusEnum, OrderEventEnum, OrderContext> builder
                = StateMachineBuilderFactory.create();

        /**
         * 创建订单: 初始化 -> 待支付
         * externalTransition : 用于一个流转的构建器
         */
        builder.externalTransition().from(OrderStatusEnum.INIT).to(OrderStatusEnum.PAY_ONLINE)
                .on(OrderEventEnum.CREATE_ORDER).when(checkOrder()).perform(createOrderAction);

        /**
         * 部分发货: 部分发货
         * internalTransition : 内部流转
         */
        builder.internalTransition()
                .within(OrderStatusEnum.PART_DELIVERY)
                .on(OrderEventEnum.DELIVERY_PART)
                .when(checkOrder())
                .perform(deliverOrderAction);

        /**
         * 取消订单: 待支付、待发货、待收货 -> 待支付
         * externalTransitions : 用于多个流转的构建器
         */
        builder.externalTransitions()
                .fromAmong(OrderStatusEnum.PAY_ONLINE,OrderStatusEnum.WAITING_FOR_RECEIVED,                                      OrderStatusEnum.WAITING_DELIVERY)
                .to(OrderStatusEnum.CANCEL)
                .on(OrderEventEnum.CANCEL_ORDER)
                .when(checkOrder())
                .perform(cancelOrderAction);


        // 说明：cola statemachine 构建后，会自动 StateMachineFactory 注册。
        // 构建成功后根据 StateMachineFactory.get(machineId) 获取状态机，不允许重复构建，重复构建会报错。
        builder.build(this.getStateMachineIdEnum().getMachineId());
    }

    @Override
    public OrderStateMachineIdEnum getStateMachineIdEnum() {
        return OrderStateMachineIdEnum.ORDER_OF_SALE;
    }
}
```
<a name="h6QgG"></a>
#### 4、单个起始状态-外部状态流转
```java
/*
   * 设置一个外部状态转义类型的builder,并设置from\to\on\when\perform
   */
builder.externalTransition()
.from(OrderStatusEnum.INIT)
.to(OrderStatusEnum.PAY_ONLINE)
.on(OrderEventEnum.CREATE_ORDER)
.when(checkOrder())
.perform(createOrderAction);
```
描述：订单起始状态为“初始化”，当发生“创建订单”事件执行状态转义，当满足 `CheckCondition` 时，执行 `CreateOrderAction`。
```java
public class CreateOrderAction implements Action<OrderStatusEnum, OrderEventEnum, OrderContext> {
    @Override
    public void execute(OrderStatusEnum from, OrderStatusEnum to, OrderEventEnum event, OrderContext context) {
        // ...
        // 1.获取状态机
        StateMachine<OrderStatusEnum, OrderEventEnum, OrderContext> stateMachine = getStateMachine();
        // 2.组状态机 messageContext
        OrderContext orderContext = buildContext();
        // 3.状态机事件触发
        stateMachine.fireEvent(OrderStatusEnum.INIT, OrderEventEnum.CREATE_ORDER, orderContext);
        // ...
    }
}
```
描述：`CreateOrderAction` 实现创建订单事件，通过 `StateMachine.FireEvent` 触发状态机事件。
<a name="B9yOB"></a>
#### 5、内部状态流转
```java
/**
 * 部分发货: 部分发货
 * internalTransition : 内部流转
 */
builder.internalTransition()
.within(OrderStatusEnum.PART_DELIVERY)
.on(OrderEventEnum.DELIVERY_PART)
.when(checkOrder())
.perform(deliverOrderAction);
```
描述：订单起始状态发生在部分发货状态下，当发生发货时执行状态流转，当满足 `CheckCondition`(订单部分发货条件)时，执行`DeliverOrderAction`，执行成功则返回状态：部分发货
<a name="DCT17"></a>
#### 6、多个起始状态-外部状态流转
```java
/**
 * 取消订单: 待支付、待发货、待收货 -> 待支付
 * externalTransitions : 用于多个流转的构建器
 */
builder.externalTransitions()
.fromAmong(OrderStatusEnum.PAY_ONLINE,OrderStatusEnum.WAITING_FOR_RECEIVED,OrderStatusEnum.WAITING_DELIVERY)
.to(OrderStatusEnum.CANCEL)
.on(OrderEventEnum.CANCEL_ORDER)
.when(checkOrder())
.perform(cancelOrderAction);
```
描述：订单起始状态为：待支付、待发货或待收货下，当发生取消订单事件时，当满足 `CheckCondition` 时，执行 `CancelOrderAction`，返回状态 `CANCEL_ORDER`。
<a name="hywvC"></a>
## 小结
Cola-StateMachine 作为阿里开源项目 COLA 中的轻量级状态机组件，最大的特点就是无状态、采用纯 Java 实现，用 Fluent Interface (连贯接口)定义状态和事件，可用于管理状态转换场景。比如：订单状态、支付状态等简单有限状态场景。
