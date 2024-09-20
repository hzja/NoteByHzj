Java
<a name="fB7MF"></a>
## 1、介绍
微服务的兴起以及现代软件架构对可扩展性、灵活性和可维护性的需求导致开发人员接受各种设计模式。<br />近年来备受关注的一种模式是命令查询职责分离 (CQRS) 模式。CQRS 特别适合命令（改变状态）和查询（读取状态）之间有明显区别的系统。在本文中将深入研究 CQRS 并了解如何使用 Spring 微服务来实现它。
<a name="H45B9"></a>
## 2、了解 CQRS
<a name="d4rV0"></a>
### 什么是 CQRS？
命令查询职责分离（CQRS）是一种架构模式，建议将数据修改操作（命令）与数据检索操作（查询）分离。这种分离允许开发专门的模型来查询和更新数据，从而增强应用程序的清晰度和可扩展性。<br />**CQRS 的核心目标是通过确保每个任务负责单个操作（命令或查询，但绝不会同时负责两者）来简化任务。**
<a name="fyrRN"></a>
### 起源与演变
CQRS 并不是一个全新的概念。它的根源可以追溯到 CQS（命令查询分离），这是 Eiffel 编程语言的创建者 Bertrand Meyer 推广的一项原则。虽然 CQS 主要是关于方法——声明方法应该执行命令或回答查询，但 CQRS 将这一原则扩展到应用程序的架构级别，建议不同的架构组件处理命令和查询。
<a name="wMfMb"></a>
### 为什么使用 CQRS？

- **可扩展性：** CQRS 允许水平可扩展性，因为可以根据需求部署命令或查询服务的多个实例。读取密集型和写入密集型操作可以独立扩展，从而优化资源利用率。
- **灵活性：** 命令和查询之间的明确区别意味着开发人员可以为每个命令和查询使用最合适的持久性机制、策略和优化。例如，虽然关系数据库可能用于事务命令操作，但非规范化视图存储甚至全文搜索引擎都可以提供查询服务。
- **可维护性：** 实施良好的 CQRS 模式简化了代码库。通过读取和写入操作的单独模型，开发人员可以专注于每个操作的细节，而不会分散不相关问题的注意力。这种隔离通常会产生更清晰的代码，更容易维护和扩展。
- **增强的安全性：** CQRS 本质上促进了更好的安全实践。通过分离命令和查询操作，可以更轻松地对写入操作实施严格的验证和授权检查，同时优化读取操作的性能。
<a name="nvHQ9"></a>
### 微服务中的 CQRS
微服务架构的兴起放大了 CQRS 的必要性。在分布式系统中，服务通常需要自治和高度解耦，CQRS 提供了一条清晰的路径。每个微服务都可以采用 CQRS 模式，确保其处理命令和查询的内部机制从其他服务中抽象出来。这也与领域驱动设计（DDD）非常吻合，其中领域事件可以触发不同微服务中的命令。
<a name="nGgMd"></a>
### 潜在的陷阱
虽然 CQRS 提供了许多好处，但它也面临着挑战：

- **复杂性增加：** 引入 CQRS 会增加开销，尤其是在读取和写入之间的区别不明显的系统中。可能并不总是需要将每个读取和写入操作分开，这样做可能会导致不必要的复杂性增加。
- **一致性：** 鉴于写入存储和读取存储可能不同，确保它们之间的数据一致性可能具有挑战性，尤其是在分布式系统中。
<a name="lZIoq"></a>
## 3、CQRS 与 Spring 微服务
Spring 生态系统拥有丰富的工具和框架，非常适合在微服务环境中实现 CQRS 模式。
<a name="yxht6"></a>
### 设置 Spring 引导
第一步是建立一个基本的 Spring Boot 项目。如果是 Spring Boot 新手，可以使用Spring Initializr轻松初始化项目。基本依赖项包括 Spring Web、Spring Data JPA 以及喜欢的任何数据库连接器。
<a name="zoZtP"></a>
### 命令、命令处理程序和聚合
在基于 Spring 的 CQRS 系统中，命令表示更改某些状态的意图，命令处理程序处理这些命令。<br />示例命令：
```java
public class CreateUserCommand {
    private final String userId;
    private final String username;

    // Constructor, getters, and other methods...
}
```
对于每个命令，都定义了相应的命令处理程序。该处理程序包含处理命令的实际逻辑：
```java
@Service
public class CreateUserCommandHandler implements CommandHandler<CreateUserCommand> {

    @Autowired
    private UserRepository userRepository;

    @Override
    public void handle(CreateUserCommand command) {
        User user = new User(command.getUserId(), command.getUsername());
        userRepository.save(user);
    }
}
```
在领域驱动设计（DDD）的背景下，状态突变通常发生在聚合上。这些聚合可确保在保留任何更改之前遵守所有域规则。
<a name="dL4mS"></a>
### 查询和查询处理程序
类似地，查询表示读取某些状态的请求，查询处理程序处理这些请求。<br />查询示例：
```java
public class GetUserByIdQuery {
    private final String userId;

    // Constructor, getters, and other methods...
}
```
对应的查询处理程序：
```java
@Service
public class GetUserByIdQueryHandler implements QueryHandler<GetUserByIdQuery, User> {

    @Autowired
    private UserRepository userRepository;

    @Override
    public User handle(GetUserByIdQuery query) {
        return userRepository.findById(query.getUserId()).orElse(null);
    }
}
```
<a name="tNmvD"></a>
### 将事件溯源与 Axon 框架集成
虽然 CQRS 提供了隔离机制，但可以使用事件源来简化命令和查询之间状态的维护。Axon 框架是一种有助于使用 Spring 实现 CQRS 和事件溯源的流行框架。<br />对于 Axon，事件在命令处理后发布。这些事件可以被持久化，然后用于重新创建聚合的状态。它还有助于保持查询端与命令端同步。
<a name="OeqgZ"></a>
### 与 Apache Kafka 的异步通信
鉴于微服务的分布式特性，在服务之间实现异步通信通常是有益的。Apache Kafka 可以集成到 Spring 生态系统中，以实现强大的事件驱动架构，这在 CQRS 设置中特别有用。<br />命令端产生的事件可以推送到Kafka主题中，查询端可以使用这些事件来更新自己的数据存储。这确保了命令端和查询端之间的解耦，使系统更具弹性和可扩展性。
<a name="mkM7u"></a>
## 4、事件溯源和 CQRS
虽然 CQRS 专注于分离命令和查询职责，但事件溯源可确保应用程序状态的每次更改都被捕获在事件对象中，并按照它们应用于同一聚合的顺序存储。这种方法允许重建过去的状态，并且与 CQRS 结合使用时特别有利。
<a name="E9SlH"></a>
### 事件溯源的本质
事件溯源是关于持久化域事件而不是状态本身。这些事件捕获状态转换。通过重放它们，可以重建聚合的当前状态。<br />例如，将存储所有交易（例如存款和取款等事件），而不是存储银行帐户的当前余额。通过重播这些事件可以得出当前余额。
<a name="NYIiZ"></a>
### 事件溯源的好处

- **审计跟踪：** 事件溯源提供了自然的变更审计日志。这对于可追溯性和历史记录至关重要的领域至关重要。
- **临时查询：** 可以确定系统在任何时间点的状态。这对于调试和理解过去的状态非常有价值。
- **事件重播：** 通过重播事件，可以重新生成读取优化的视图。当要创建新的投影或重建损坏的投影时，这尤其有用。
<a name="h0M3n"></a>
### 将事件溯源与 CQRS 集成
CQRS 和事件溯源通过以下方式相互补充：

- **解耦：** 正如命令和查询在 CQRS 中解耦一样，通过事件源，事件（表示状态更改）与实际状态解耦。这促进了松散耦合的架构。
- **可扩展性：** CQRS 中读取和写入的隔离性质非常适合事件驱动系统。命令模型处理命令并生成事件，而查询模型处理查询并可以通过侦听这些事件来更新。
- **弹性：** 通过重放事件的能力，可以在发生故障时重建系统状态，甚至可以迁移到全新的系统。
<a name="wcC3b"></a>
### 使用 Spring 和 Axon 框架实现
如前所述，Axon 框架提供了一种在 Spring 应用程序中实现 CQRS 和事件源的无缝方法：<br />**聚合和事件处理：** 在 Axon 中，聚合负责命令处理和事件生成。处理命令后，它们应用导致状态更改的事件。
```java
@Aggregate
public class Account {
    @AggregateIdentifier
    private String accountId;
    private int balance;

    @CommandHandler
    public void handle(WithdrawMoneyCommand cmd) {
        if (cmd.getAmount() > balance) {
            throw new InsufficientFundsException();
        }
        apply(new MoneyWithdrawnEvent(cmd.getAccountId(), cmd.getAmount()));
    }

    @EventSourcingHandler
    public void on(MoneyWithdrawnEvent evt) {
        this.balance -= evt.getAmount();
    }
}
```
**事件存储：** Axon 提供了一种存储和检索事件的机制。可以重播这些事件以重建聚合的状态。<br />**投影：** Axon 中的投影提供了 CQRS 的查询端。他们监听事件并更新读取优化视图。这样，查询模型始终会根据最新更改保持更新。
<a name="xbsFv"></a>
## 5、挑战和考虑因素
虽然 CQRS 和事件溯源可以提供巨大的好处，但它们也具有复杂性。意识到这些挑战将确保更明智、更顺利的实施。
<a name="Y08i2"></a>
### 复杂性开销
**架构复杂性：** CQRS 和事件源向系统引入了额外的层和组件，例如事件存储、命令和事件总线以及同步机制。<br />**学习曲线：** 对于刚接触这些模式的团队，将有一个学习阶段。从传统的基于 CRUD 的系统的概念转变可能具有挑战性。
<a name="eH8iO"></a>
### 数据一致性
**最终一致性：** 鉴于命令和查询模型的隔离性质，通常会为了最终一致性而牺牲即时一致性。这意味着在命令端所做的更改反映在查询端之前可能会有延迟。<br />**事件排序：** 确保事件按照生成的顺序进行处理，尤其是在分布式系统中，可能很棘手，但对于保持一致的状态至关重要。
<a name="u5EqM"></a>
### 事件版本控制
随着时间的推移，事件的结构或语义可能会发生变化，从而导致以下挑战：

- **版本不匹配：** 处理同一事件类型的不同版本可能会变得复杂。
- **事件升级：** 随着事件的发展，系统必须能够将旧事件升级为新版本，而不改变存储的事件。
<a name="KYFVs"></a>
### 数据存储和重放
**存储注意事项：** 由于存储所有事件，事件存储可能会快速增长，从而导致存储成本增加和潜在的性能问题。<br />**重播持续时间：** 通过重播长期历史事件来重建系统状态可能非常耗时，会影响系统恢复和初始化时间。
<a name="BaOge"></a>
### 与其他系统集成
将使用 CQRS 和事件源的系统与不遵循这些模式的外部系统集成可能具有挑战性，特别是在数据同步和事务管理方面。
<a name="OXclU"></a>
### 确定边界
**粒度决策：** 决定应用 CQRS 和事件溯源的粒度至关重要。在微观层面上实施它们可能会导致过于复杂化，而过于广泛地实施可能会削弱好处。<br />**域复杂性：** 这些模式对于简单域来说可能有点过分了。它们更适合复杂的领域，其好处超过了实施和维护成本。
<a name="WlhJo"></a>
### 工具和基础设施
虽然有像 Axon 和框架这样的工具支持 CQRS 和事件溯源，但它们可能并不总是适合所有场景。可能需要自定义实现，这会增加项目的复杂性和持续时间。
<a name="IMiTt"></a>
## 6、结论
CQRS 提供了一种独特的方式来扩展和组织微服务。当与 Spring 的生态系统结合时，它可以提供强大的工具包来构建健壮、可扩展和可维护的系统。然而，与所有架构决策一样，必须权衡利弊并确保它适合特定用例。
