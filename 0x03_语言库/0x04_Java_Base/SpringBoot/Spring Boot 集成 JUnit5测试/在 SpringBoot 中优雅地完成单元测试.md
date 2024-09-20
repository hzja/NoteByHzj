JavaSpringBoot
<a name="r7xZN"></a>
## 什么是单元测试
当一个测试满足下面任意一点时，测试就不是单元测试(by Michael Feathers in 2005)：

1. 与数据库交流
2. 与网络交流
3. 与文件系统交流
4. 不能与其他单元测试在同一时间运行
5. 不得不为运行它而作一些特别的事

如果一个测试做了上面的任何一条，那么它就是一个集成测试。
<a name="KU1SS"></a>
## 不要用Spring编写单元测试
```java
@SpringBootTest
class OrderServiceTests {
    @Autowired
    private OrderRepository orderRepository;
    @Autowired
    private OrderService orderService;

    @Test
    void payOrder() {
        Order order = new Order(1L, false);
        orderRepository.save(order);

        Payment payment = orderService.pay(1L, "4532756279624064");

        assertThat(payment.getOrder().isPaid()).isTrue();
        assertThat(payment.getCreditCardNumber()).isEqualTo("4532756279624064");
    }
}
```
这是一个单元测试吗？首先`@SpringBootTest`注解加载了整个应用上下文，而仅仅是为了注入两个Bean。<br />另一个问题是需要读取和写入订单到数据库，这也是集成测试的范畴。<br />Spring Framework文档对于单元测试的描述<br />真正的单元测试运行的非常快，因为不需要运行时去装配基础设施。强调将真正的单元测试作为开发方法的一部分可以提高生产力。
<a name="Blz4y"></a>
## 编写“可单元测试”的Service
Spring Framework文档对于单元测试的另一描述<br />依赖注入可以让代码减少依赖。POJO可以让应用可以通过`new`操作符在JUnit或TestNG上进行测试，不需要任何的Spring和其他容器<br />考虑如果编写这样的Service，它方便进行单元测试吗！？
```java
@Service
public class BookService {
    @Autowired
    private BookRepository repository;

    // ... service methods

}
```
不方便，因为`BookRepository`通过`@Autowired`被注入到Service中，并且repository是一个私有变量，这就限定了外界只能通过Spring或其它依赖注入容器（或反射）设置这个值，那么单元测试如果不想加载整个Spring容器，那么它就无法使用这个Service。<br />而如果这样写，使用构造方法注入，外界也可以通过`new`去自行传递`Repository`，这样即使没有Spring，外界也能进行快速的测试。这可能也是Spring不推荐属性注入的原因。
```java
@Service
public class BookService {
    private BookRepository repository;

    @Autowired
    public BookService(BookRepository repository) {
        this.repository = repository;
    }
}
```
<a name="j6scr"></a>
## 编写单元测试
<a name="w0TM0"></a>
### Mockito介绍
前面的知识表明，单元测试就是对一个系统中的某个最小单元的逻辑正确性的测试，通常是对一个方法来进行测试，因为只测试逻辑正确性，所以这个测试是独立的，不与任何外界环境相关，比如不需要连接数据库，不访问网络和文件系统，不依赖其他单元测试。但是现实的业务逻辑中往往有很多复杂错综的依赖关系，比如对Service进行单元测试，那么它要依赖一个数据库持久层的Repository对象，这时候就难办了，若创建了一个Repository便连接了数据库，连接了数据库便不是一个独立的单元测试。<br />Mockito是一个用来在单元测试中快速模拟那些需要与外界环境沟通的对象，以便快速的、方便的进行单元测试而不用启动整个系统。<br />下面的代码就是Mockito的一个基础使用，Mock意为伪造。
```java
// 通过mock方法伪造一个orderRepository的实现，这个实现目前什么都不会做
orderRepository = mock(OrderRepository.class);
// 通过mock方法伪造一个paymentRepository的实现，这个实现目前什么都不会做
paymentRepository = mock(PaymentRepository.class)

// 创建一个Order对象以便一会儿使用
Order order = new Order(1L, false);
// 使用when方法，定义当orderRepository.findById(1L)被调用时的行为，直接返回刚刚创建的order对象
when(orderRepository.findById(1L)).thenReturn(Optional.of(order));
// 使用when方法，定义当paymentRepository.save(任何参数)被调用时的行为，直接返回传入的参数。
when(paymentRepository.save(any())).then(returnsFirstArg());
```
<a name="ooB3p"></a>
### 编写单元测试
```java
class OrderServiceTests {
    private OrderRepository orderRepository;
    private PaymentRepository paymentRepository;
    private OrderService orderService;

    @BeforeEach
    void setupService() {
        orderRepository = mock(OrderRepository.class);
        paymentRepository = mock(PaymentRepository.class);
        orderService = new OrderService(orderRepository, paymentRepository);
    }

    @Test
    void payOrder() {
        Order order = new Order(1L, false);
        when(orderRepository.findById(1L)).thenReturn(Optional.of(order));
        when(paymentRepository.save(any())).then(returnsFirstArg());

        Payment payment = orderService.pay(1L, "4532756279624064");

        assertThat(payment.getOrder().isPaid()).isTrue();
        assertThat(payment.getCreditCardNumber()).isEqualTo("4532756279624064");
    }
}
```
现在即使不想连接数据库，也可以通过mock来给定一个Repository的其他实现，这样这个方法可以在毫秒内完成。<br />也可以使用Mockito
```java
@ExtendWith(MockitoExtension.class)
class OrderServiceTests {
    @Mock
    private OrderRepository orderRepository;
    @Mock
    private PaymentRepository paymentRepository;
    @InjectMocks
    private OrderService orderService;

    // ...
}
```
<a name="BU0aZ"></a>
## 单元测试足够了吗？
系统中进行单元测试就足够了吗？<br />最起码单元测试不能做到这些

1. Controller中的`@PostMapping`和`@GetMapping`无法被处理，该如何知道Http请求带着`@PathVariable`中的参数正确的映射到对应的Controller方法中了呢？
2. 如果Controller方法被`@RequestBody`和`@ResponseBody`标注，如何检测输入被正确的反序列化，输出被正确的序列化？
3. 如果Controller参数被`@Vaild`标注，如何确定验证发生了？
4. Spring中的Controller异常时会走`ExceptionHandler`，只能对一个方法进行单元测试，如何保证`ExceptionHandler`被正确调用。
<a name="CEeDy"></a>
## 使用`@WebMvcTest`编写集成测试
上面的种种问题都说明仅仅是单元测试满足不了测试需求，它只能保证开发过程中快速的验证方法的逻辑正确性。现在需要编写集成测试了。<br />现在测试应用的Web层行为是否正确，并且，像前面说的，不想卷入数据库调用。<br />想要测试SpringMVC Controller，可以使用`@WebMvcTest`注解，它只扫描`@Controller`，`@ControllerAdvice`标注的Bean，还有一些其他和Web层相关的Bean。<br />这是一个基础的示例
```java
@WebMvcTest(BookController.class)
class BooklistApplicationTests {

    @Autowired
    private MockMvc mockMvc;

    @MockBean
    private BookRepository repository;

    @Test
    public void testReadingList() throws Exception { }
}
```
因为不想卷入数据库层的调用，所以把`BookRepository`标注了`@MockBean`注解，作为一个mock对象(伪造对象)。并且因为`@WebMvcTest`不扫描Controller之外的Bean，所以要提供所有Controller依赖的Bean对象。<br />如果不传递一个Controller类给`@WebMvcTest`，那么Spring将扫描所有的控制器，而且必须mock(伪造)所有的控制器依赖的bean。<br />由于`@WebMvcTest`标注了`@AutoConfigureMockMvc`，所以可以直接注入一个`MockMvc`对象，它用来伪造HTTP请求，无需真的启动一个服务器就能通过伪造的请求来进行Controller测试。
<a name="RCXA2"></a>
## 验证HTTP请求和返回正确性
```java
@Test
public void testReadingList() throws Exception {
    assertNotNull(mockMvc);
    List<Book> books = new ArrayList<>();
    when(repository.findBooksByReader(any())).thenReturn(books);

    mockMvc.perform(get("/readinglist/yulaoba"))
            .andExpect(status().isOk())
            .andExpect(view().name("readingList"))
            .andExpect(model().attributeExists("books"))
            .andExpect(model().attribute("books", is(empty())));
}
```
这里使用了perform发送了一个模拟的get请求，并且判断了请求的返回状态，视图名，模型参数。
<a name="cu48y"></a>
## 结果序列化的校验
```java
@Test
public void testReadingList2() throws Exception{
    assertNotNull(mockMvc);
    when(repository.findBooksByReader(any())).thenReturn(Arrays.asList(
            new Book(1l, "yulaoba", "123123123123123", "JAVA", "Smith", "asdfafd")
    ));
    mockMvc.perform(get("/readinglist/api/{reader}", "yulaoba"))
            .andExpect(status().isOk())
            .andExpect(content().contentType(MediaType.APPLICATION_JSON))
            .andExpect(content().json("[{'id': 1, 'reader': 'yulaoba', 'isbn': '123123123123123', 'title': 'JAVA', 'author': 'Smith', 'description': 'asdfafd'}]"))
            .andExpect(jsonPath("$[0]['title']").value("JAVA"));
}
```
jsonPath详细用法
<a name="xYfFi"></a>
## `ExceptionHandler`的测试
假设Controller中有这样的`@ExceptionHandler`
```java
@ExceptionHandler(RuntimeException.class)
@ResponseStatus(HttpStatus.METHOD_NOT_ALLOWED)
@ResponseBody
public String handleException() {
    return "METHOD_NOT_ALLOWED";
}
```
然后先假装让repository抛出异常，然后使用status对其返回状态码进行校验。
```java
@Test
public void testExceptionHandler() throws Exception {
    when(repository.findBooksByReader(any())).thenThrow(RuntimeException.class);

    mockMvc.perform(get("/readinglist/api/{reader}", "yulaoba"))
            .andExpect(status().isMethodNotAllowed());
}
```
