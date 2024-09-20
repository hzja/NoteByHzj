在 Spring Boot 依赖项注入的上下文中，存在关于注入依赖项最佳实践的争论：字段注入、Setter注入和构造函数注入。
<a name="P734W"></a>
## 什么是字段注入？
字段注入涉及直接用 `@Autowired` 注释类的私有字段。这是一个例子：
```java
@Component
public class OrderService {

    @Autowired
    private OrderRepository orderRepository;

    public Order findOrderById(Long id) {
        return orderRepository.findById(id);
    }
}
```
<a name="GJSus"></a>
## 为什么应该停止使用字段注入
<a name="iEinb"></a>
### 1、可测试性
**字段注入使组件的单元测试变得复杂。** 由于依赖项直接注入到字段中，因此无法在 Spring 上下文之外轻松提供模拟或替代实现。<br />以 sameOrderService 类为例。如果希望对 OrderService 进行单元测试，那么在模拟 OrderRepository 时会遇到困难，因为它是一个私有字段。下面是对 OrderService 进行单元测试的方法:
```java
@RunWith(SpringJUnit4ClassRunner.class)
public class OrderServiceTest {

    private OrderService orderService;

    @Mock
    private OrderRepository orderRepository;

    @Before
    public void setUp() throws Exception {
        orderService = new OrderService();

        // This will set the mock orderRepository into orderService's private field
        ReflectionTestUtils.setField(orderService, "orderRepository", orderRepository);
    }

    ...
}
```
尽管可以实现，但使用反射来替换私有字段并不是一个很好的设计。它违背了面向对象的设计原则，使测试难以阅读和维护。<br />但是，如果使用构造函数注入：
```java
@Component
public class OrderService {
    private final OrderRepository orderRepository;

    public OrderService(OrderRepository orderRepository) {
        this.orderRepository = orderRepository;
    }
}
```
可以在测试期间轻松提供模拟 OrderRepository：
```java
OrderRepository mockRepo = mock(OrderRepository.class);
OrderService orderService = new OrderService(mockRepo);
```
<a name="CAZGK"></a>
### 2、不变性
字段注入使 Bean 在构建后可变。而通过构造函数注入，一旦构造了一个对象，它的依赖关系就会保持不变。<br />举例来说：<br />**字段注入类：**
```java
@Component
public class UserService {
    @Autowired
    private UserRepository userRepository;
}
```
这里，userRepository 在创建对象后可以重新分配引用，这就打破了不变性原则。<br />如果使用构造函数注入：
```java
@Component
public class UserService {
    private final UserRepository userRepository;

    public UserService(UserRepository userRepository) {
        this.userRepository = userRepository;
    }
}
```
该 userRepository 字段可以声明为最终字段，在构造完成后，就会一直保持不变。
<a name="PTEFS"></a>
### 3、与Spring更紧密的耦合
字段注入使类与 Spring 耦合更紧密，因为它直接在字段上使用 Spring 特定的注释 (`@Autowired`)。这可能会在以下场景中出现问题：

- **「不使用 Spring 的情况」**：假设正在构建一个不使用 Spring 的轻量级命令行应用程序，但仍然想利用 UserService 的逻辑。在这种情况下，`@Autowired` 注释没有任何意义，不能用于注入依赖项。就必须重构该类或实现繁琐的解决方法才能重用UserService.
- **「切换到另一个 DI 框架」**：如果决定切换到另一个依赖注入框架，比如 Google Guice，Spring 特定的框架 `@Autowired` 就会成为一个障碍。那时必须重构使用 Spring 特定注释的每一个地方，这会是十分繁琐的。
- **「可读性和理解性」**：对于不熟悉 Spring 的开发人员来说，遇到 `@Autowired` 注解可能会感到困惑。他们可能想知道如何解决依赖关系，从而增加学习成本（ps：虽然不熟悉 Spring 开发的Java程序员可能很少了）。
<a name="nUeHP"></a>
### 4、空指针异常
当类利用字段注入并通过其默认构造函数实例化时，依赖字段保持未初始化。<br />举例来讲：
```java
@Component
public class PaymentGateway {
    @Autowired
    private PaymentQueue paymentQueue;

    public void initiate (PaymentRequest request){
        paymentQueue.add(request);
        ...
    }
}

public class PaymentService {
    public void process (PaymentRequest request) {
        PaymentGateway gateway = new PaymentGateway();
        gateway.initiate(request);
    }   
}
```
通过上面的代码，不难看出，如果在运行时以这种状态访问PaymentGateway，则会发生 NullPointerException。在Spring上下文之外手动初始化这些字段的唯一方法是使用反射，反射机制的语法比较繁琐且易错，在程序可读性方面存在一定问题，所以不建议这样做。
<a name="d1QdT"></a>
### 5、循环依赖
字段注入可能会掩盖循环依赖问题，使它们在开发过程中更难被发现。<br />举例来讲：考虑两个相互依赖的服务AService和BService:
```java
@Service
public class AService {
    @Autowired
    private BService bService;
}

@Service
public class BService {
    @Autowired
    private AService aService;
}
```
以上可能会导致应用程序中出现意想不到的问题。<br />使用构造函数注入，Spring会在启动期间立即抛出 `BeanCurrentlyInCreationException`，让我们意识到循环依赖。不过，要解决循环依赖问题，可以使用`@Lazy`延迟加载其中一个依赖项。
<a name="DyNMI"></a>
### 结论
虽然字段注入可能看起来更简洁，但它的缺点远远超过了它的简洁性。构造函数注入在应用程序的可测试性、不变性和整体稳健性方面提供了明显的优势。它与 SOLID 原则非常一致，确保 Spring Boot 应用程序可维护且不易出错。<br />所以，建议大家停止在 Spring Boot 中使用字段注入！
