Spring
<a name="jyhZp"></a>
### 一、获取Spring容器对象
<a name="QAQbn"></a>
#### 1.实现`BeanFactoryAware`接口
```java
@Service
public class PersonService implements BeanFactoryAware {
    private BeanFactory beanFactory;
    @Override
    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
        this.beanFactory = beanFactory;
    }
    public void add() {
        Person person = (Person) beanFactory.getBean("person");
    }
}
```
实现`BeanFactoryAware`接口，然后重写`setBeanFactory`方法，就能从该方法中获取到Spring容器对象。
<a name="1pm2o"></a>
#### 2.实现ApplicationContextAware接口
```java
@Service
public class PersonService2 implements ApplicationContextAware {
    private ApplicationContext applicationContext;
    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
    }
    public void add() {
        Person person = (Person) applicationContext.getBean("person");
    }
}
```
实现`ApplicationContextAware`接口，然后重写`setApplicationContext`方法，也能从该方法中获取到Spring容器对象。
<a name="UREe6"></a>
#### 3.实现ApplicationListener接口
```java
@Service
public class PersonService3 implements ApplicationListener<ContextRefreshedEvent> {
    private ApplicationContext applicationContext;
    @Override
    public void onApplicationEvent(ContextRefreshedEvent event) {
        applicationContext = event.getApplicationContext();
    }
    public void add() {
        Person person = (Person) applicationContext.getBean("person");
    }
}
```
实现`ApplicationListener`接口，需要注意的是该接口接收的泛型是`ContextRefreshedEvent`类，然后重写`onApplicationEvent`方法，也能从该方法中获取到Spring容器对象。<br />`Aware`接口其实是一个空接口，里面不包含任何方法。<br />它表示已感知的意思，通过这类接口可以获取指定对象，比如：

- 通过BeanFactoryAware获取BeanFactory
- 通过ApplicationContextAware获取ApplicationContext
- 通过BeanNameAware获取BeanName等

Aware接口是很常用的功能，目前包含如下功能：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1611839451626-e32b7e3c-c0f6-4ba4-aac3-99d9f78deb81.png#align=left&display=inline&height=1032&originHeight=1032&originWidth=722&size=0&status=done&style=shadow&width=722)
<a name="IE0mV"></a>
### 二、初始化bean
Spring中支持3种初始化bean的方法：

- xml中指定`init-method`方法
- 使用`@PostConstruct`注解
- 实现`InitializingBean`接口

第一种方法太古老了，现在用的人不多，具体用法就不作介绍了。
<a name="Z92HB"></a>
#### 1.使用`@PostConstruct`注解
```java
@Service
public class AService {
    @PostConstruct
    public void init() {
        System.out.println("===初始化===");
    }
}
```
在需要初始化的方法上增加`@PostConstruct`注解，这样就有初始化的能力。
<a name="XGA3l"></a>
#### 2.实现`InitializingBean`接口
```java
@Service
public class BService implements InitializingBean {
    @Override
    public void afterPropertiesSet() throws Exception {
        System.out.println("===初始化===");
    }
}
```
实现`InitializingBean`接口，重写`afterPropertiesSet`方法，该方法中可以完成初始化功能。<br />这里顺便抛出一个有趣的问题：`init-method`、`PostConstruct` 和 `InitializingBean` 的执行顺序是什么样的？<br />决定他们调用顺序的关键代码在`AbstractAutowireCapableBeanFactory`类的`initializeBean`方法中。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611840763740-768ddb83-7640-4d7f-a8b0-c5fcab27bec6.png#align=left&display=inline&height=508&originHeight=1525&originWidth=3432&size=396079&status=done&style=shadow&width=1144)<br />这段代码中会先调用`BeanPostProcessor`的`postProcessBeforeInitialization`方法，而`PostConstruct`是通过`InitDestroyAnnotationBeanPostProcessor`实现的，它就是一个`BeanPostProcessor`，所以`PostConstruct`先执行。<br />而`invokeInitMethods`方法中的代码：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611840910624-aa387137-1bc7-4c32-8884-cbea87b800e9.png#align=left&display=inline&height=508&originHeight=1523&originWidth=3432&size=458604&status=done&style=shadow&width=1144)<br />决定了先调用`InitializingBean`，再调用`init-method`。<br />所以得出结论，他们的调用顺序是：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1611839451597-04b71ca5-e905-4244-a947-d058c78e8c43.webp#align=left&display=inline&height=650&originHeight=650&originWidth=552&size=0&status=done&style=shadow&width=552)
<a name="iSrbS"></a>
### 三、自定义自己的Scope
Spring默认支持的`Scope`只有两种：

- singleton 单例，每次从Spring容器中获取到的bean都是同一个对象。
- prototype 多例，每次从Spring容器中获取到的bean都是不同的对象。

`spring web`又对`Scope`进行了扩展，增加了：

- RequestScope 同一次请求从spring容器中获取到的bean都是同一个对象。
- SessionScope 同一个会话从spring容器中获取到的bean都是同一个对象。

即便如此，有些场景还是无法满足要求。<br />比如，想在同一个线程中从spring容器获取到的bean都是同一个对象，该怎么办？<br />这就需要自定义`Scope`了。<br />第一步实现`Scope`接口：
```java
public class ThreadLocalScope implements Scope {
    private static final ThreadLocal THREAD_LOCAL_SCOPE = new ThreadLocal();
    @Override
    public Object get(String name, ObjectFactory<?> objectFactory) {
        Object value = THREAD_LOCAL_SCOPE.get();
        if (value != null) {
            return value;
        }
        Object object = objectFactory.getObject();
        THREAD_LOCAL_SCOPE.set(object);
        return object;
    }
    @Override
    public Object remove(String name) {
        THREAD_LOCAL_SCOPE.remove();
        return null;
    }
    @Override
    public void registerDestructionCallback(String name, Runnable callback) {
    }
    @Override
    public Object resolveContextualObject(String key) {
        return null;
    }
    @Override
    public String getConversationId() {
        return null;
    }
}
```
第二步将新定义的`Scope`注入到Spring容器中：
```java
@Component
public class ThreadLocalBeanFactoryPostProcessor implements BeanFactoryPostProcessor {
    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        beanFactory.registerScope("threadLocalScope", new ThreadLocalScope());
    }
}
```
第三步使用新定义的`Scope`：
```java
@Scope("threadLocalScope")
@Service
public class CService {
    public void add() {
    }
}
```
<a name="a9eds"></a>
### 四、FactoryBean
说起`FactoryBean`就不得不提`BeanFactory`，因为面试官老喜欢问它们的区别。

- BeanFactory：Spring容器的顶级接口，管理bean的工厂。
- FactoryBean：并非普通的工厂bean，它隐藏了实例化一些复杂Bean的细节，给上层应用带来了便利。

如果看Spring源码，会发现它有70多个地方在用FactoryBean接口。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1611839451621-5221b4da-d050-4642-9249-8da59dc7bced.webp#align=left&display=inline&height=1092&originHeight=1092&originWidth=1014&size=0&status=done&style=none&width=1014)<br />上面这张图足以说明该接口的重要性。<br />特别提一句：MyBatis的`SqlSessionFactory`对象就是通过`SqlSessionFactoryBean`类创建的。<br />定义自己的`FactoryBean`：
```java
@Component
public class MyFactoryBean implements FactoryBean {
    @Override
    public Object getObject() throws Exception {
        String data1 = buildData1();
        String data2 = buildData2();
        return buildData3(data1, data2);
    }
    private String buildData1() {
        return "data1";
    }
    private String buildData2() {
        return "data2";
    }
    private String buildData3(String data1, String data2) {
        return data1 + data2;
    }
    @Override
    public Class<?> getObjectType() {
        return null;
    }
}
```
获取`FactoryBean`实例对象：
```java
@Service
public class MyFactoryBeanService implements BeanFactoryAware {
    private BeanFactory beanFactory;
    @Override
    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
        this.beanFactory = beanFactory;
    }
    public void test() {
        Object myFactoryBean = beanFactory.getBean("myFactoryBean");
        System.out.println(myFactoryBean);
        Object myFactoryBean1 = beanFactory.getBean("&myFactoryBean");
        System.out.println(myFactoryBean1);
    }
}
```

- `getBean("myFactoryBean");`获取的是MyFactoryBeanService类中getObject方法返回的对象，
- `getBean("&myFactoryBean");`获取的才是MyFactoryBean对象。
<a name="GHUuC"></a>
### 五、自定义类型转换
Spring目前支持3中类型转换器：

- `Converter<S,T>`：将 S 类型对象转为 T 类型对象
- `ConverterFactory<S, R>`：将 S 类型对象转为 R 类型及子类对象
- `GenericConverter`：它支持多个source和目标类型的转化，同时还提供了source和目标类型的上下文，这个上下文能实现基于属性上的注解或信息来进行类型转换。

这3种类型转换器使用的场景不一样，以`Converter<S,T>`为例。假如：接口中接收参数的实体对象中，有个字段的类型是Date，但是实际传参的是字符串类型：2021-01-03 10:20:15，要如何处理呢？<br />第一步，定义一个实体`User`：
```java
@Data
public class User {
    private Long id;
    private String name;
    private Date registerDate;
}
```
第二步，实现`Converter`接口：
```java
public class DateConverter implements Converter<String, Date> {
    private SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    @Override
    public Date convert(String source) {
        if (source != null && !"".equals(source)) {
            try {
                simpleDateFormat.parse(source);
            } catch (ParseException e) {
                e.printStackTrace();
            }
        }
        return null;
    }
}
```
第三步，将新定义的类型转换器注入到Spring容器中：
```java
@Configuration
public class WebConfig extends WebMvcConfigurerAdapter {
    @Override
    public void addFormatters(FormatterRegistry registry) {
        registry.addConverter(new DateConverter());
    }
}
```
第四步，调用接口
```java
@RequestMapping("/user")
@RestController
public class UserController {
    @RequestMapping("/save")
    public String save(@RequestBody User user) {
        return "success";
    }
}
```
请求接口时`User`对象中`registerDate`字段会被自动转换成`Date`类型。
<a name="mQDcz"></a>
### 六、SpringMVC拦截器
SpringMVC拦截器根Spring拦截器相比，它里面能够获取`HttpServletRequest`和`HttpServletResponse` 等web对象实例。<br />SpringMVC拦截器的顶层接口是：`HandlerInterceptor`，包含三个方法：

- `preHandle` 目标方法执行前执行
- `postHandle` 目标方法执行后执行
- `afterCompletion` 请求完成时执行

为了方便一般情况会用`HandlerInterceptor`接口的实现类`HandlerInterceptorAdapter`类。<br />假如有权限认证、日志、统计的场景，可以使用该拦截器。<br />第一步，继承`HandlerInterceptorAdapter`类定义拦截器：
```java
public class AuthInterceptor extends HandlerInterceptorAdapter {
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
            throws Exception {
        String requestUrl = request.getRequestURI();
        if (checkAuth(requestUrl)) {
            return true;
        }
        return false;
    }
    private boolean checkAuth(String requestUrl) {
        System.out.println("===权限校验===");
        return true;
    }
}
```
第二步，将该拦截器注册到Spring容器：
```java
@Configuration
public class WebAuthConfig extends WebMvcConfigurerAdapter {
 
    @Bean
    public AuthInterceptor getAuthInterceptor() {
        return new AuthInterceptor();
    }
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(new AuthInterceptor());
    }
}
```
第三步，在请求接口时SpringMVC通过该拦截器，能够自动拦截该接口，并且校验权限。<br />该拦截器其实相对来说，比较简单，可以在`DispatcherServlet`类的`doDispatch`方法中看到调用过程：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611841405380-7ea153ef-69fa-4dca-bf4c-b90002d83e25.png#align=left&display=inline&height=422&originHeight=1265&originWidth=3393&size=364241&status=done&style=shadow&width=1131)
<a name="UBnjM"></a>
### 七、Enable开关
`Enable`开头的注解，比如：`EnableAsync`、`EnableCaching`、`EnableAspectJAutoProxy`等，这类注解就像开关一样，只要在`@Configuration`定义的配置类上加上这类注解，就能开启相关的功能。<br />实现一个自己的开关：<br />第一步，定义一个LogFilter：
```java
public class LogFilter implements Filter {
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
    }
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        System.out.println("记录请求日志");
        chain.doFilter(request, response);
        System.out.println("记录响应日志");
    }
    @Override
    public void destroy() {
        
    }
}
```
第二步，注册LogFilter：
```java
@ConditionalOnWebApplication
public class LogFilterWebConfig {
    @Bean
    public LogFilter timeFilter() {
        return new LogFilter();
    }
}
```
注意，这里用了`@ConditionalOnWebApplication`注解，没有直接使用`@Configuration`注解。<br />第三步，定义开关`@EnableLog`注解：
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Import(LogFilterWebConfig.class)
public @interface EnableLog {
}
```
第四步，只需在SpringBoot启动类加上`@EnableLog`注解即可开启LogFilter记录请求和响应日志的功能。
<a name="xLMDy"></a>
### 八、RestTemplate拦截器
使用`RestTemplate`调用远程接口时，有时需要在`header`中传递信息，比如：traceId，source等，便于在查询日志时能够串联一次完整的请求链路，快速定位问题。<br />这种业务场景就能通过`ClientHttpRequestInterceptor`接口实现，具体做法如下：<br />第一步，实现`ClientHttpRequestInterceptor`接口：
```java
public class RestTemplateInterceptor implements ClientHttpRequestInterceptor {
    @Override
    public ClientHttpResponse intercept(HttpRequest request, byte[] body, ClientHttpRequestExecution execution) throws IOException {
        request.getHeaders().set("traceId", MdcUtil.get());
        return execution.execute(request, body);
    }
}
```
第二步，定义配置类：
```java
@Configuration
public class RestTemplateConfiguration {
    @Bean
    public RestTemplate restTemplate() {
        RestTemplate restTemplate = new RestTemplate();
        restTemplate.setInterceptors(Collections.singletonList(restTemplateInterceptor()));
        return restTemplate;
    }
    @Bean
    public RestTemplateInterceptor restTemplateInterceptor() {
        return new RestTemplateInterceptor();
    }
}
```
其中MdcUtil其实是利用`MDC`工具在`ThreadLocal`中存储和获取traceId
```java
public class MdcUtil {
    private static final String TRACE_ID = "TRACE_ID";
    public static String get() {
        return MDC.get(TRACE_ID);
    }
    public static void add(String value) {
        MDC.put(TRACE_ID, value);
    }
}
```
当然，这个例子中没有演示MdcUtil类的add方法具体调的地方，可以在filter中执行接口方法之前，生成traceId，调用MdcUtil类的add方法添加到`MDC`中，然后在同一个请求的其他地方就能通过MdcUtil类的get方法获取到该traceId。
<a name="A8N9T"></a>
### 九、统一异常处理
在开发接口时，如果出现异常，为了给用户一个更友好的提示，例如：
```java
@RequestMapping("/test")
@RestController
public class TestController {
    @GetMapping("/add")
    public String add() {
        int a = 10 / 0;
        return "成功";
    }
}
```
如果不做任何处理请求add接口结果直接报错：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1611839451648-9254e011-7efb-4053-8c63-d76e4d541bc8.png#align=left&display=inline&height=306&originHeight=306&originWidth=1080&size=0&status=done&style=shadow&width=1080)<br />用户能直接看到错误信息，这种交互方式给用户的体验非常差，为了解决这个问题，通常会在接口中捕获异常：
```java
@GetMapping("/add")
public String add() {
    String result = "成功";
    try {
        int a = 10 / 0;
    } catch (Exception e) {
        result = "数据异常";
    }
    return result;
}
```
接口改造后，出现异常时会提示：“数据异常”，对用户来说更友好。这样看起来挺不错的，但还是有问题的。<br />如果只是一个接口还好，但是如果项目中有成百上千个接口，都要加上异常捕获代码吗？<br />答案是否定的，这时全局异常处理就派上用场了：`RestControllerAdvice`。
```java
@RestControllerAdvice
public class GlobalExceptionHandler {
    @ExceptionHandler(Exception.class)
    public String handleException(Exception e) {
        if (e instanceof ArithmeticException) {
            return "数据异常";
        }
        if (e instanceof Exception) {
            return "服务器内部异常";
        }
        retur nnull;
    }
}
```
只需在`handleException`方法中处理异常情况，业务接口中可以放心使用，不再需要捕获异常（统一处理了）。
<a name="xO3fR"></a>
### 十、异步也可以这么优雅
以前在使用异步功能时，通常情况下有三种方式：

- 继承Thread类
- 实现Runable接口
- 使用线程池

回顾一下：
<a name="gghy9"></a>
#### 1. 继承Thread类
```java
public class MyThread extends Thread {
    @Override
    public void run() {
        System.out.println("===call MyThread===");
    }
    public static void main(String[] args) {
        new MyThread().start();
    }
}
```
<a name="kQAM7"></a>
#### 2. 实现Runable接口
```java
public class MyWork implements Runnable {
    @Override
    public void run() {
        System.out.println("===call MyWork===");
    }
    public static void main(String[] args) {
        new Thread(new MyWork()).start();
    }
}
```
<a name="PlbC7"></a>
#### 3. 使用线程池
```java
public class MyThreadPool {
    private static ExecutorService executorService = new ThreadPoolExecutor(1, 5, 60, TimeUnit.SECONDS, new ArrayBlockingQueue<>(200));
    static class Work implements Runnable {
        @Override
        public void run() {
            System.out.println("===call work===");
        }
    }
    public static void main(String[] args) {
        try {
            executorService.submit(new MyThreadPool.Work());
        } finally {
            executorService.shutdown();
        }
    }
}
```
这三种实现异步的方法不能说不好，但是Spring已经抽取了一些公共的地方，无需再继承`Thread`类或实现`Runable`接口，它都搞定了。<br />使用Spring的异步功能：<br />第一步，SpringBoot项目启动类上加`@EnableAsync`注解。
```java
@EnableAsync
@SpringBootApplication
public class Application {
    public static void main(String[] args) {
        new SpringApplicationBuilder(Application.class).web(WebApplicationType.SERVLET).run(args);
    }
}
```
第二步，在需要使用异步的方法上加上`@Async`注解：
```java
@Service
public class PersonService {
    @Async
    public String get() {
        System.out.println("===add==");
        return "data";
    }
}
```
然后在使用的地方调用一下：p`ersonService.get();`就拥有了异步功能。<br />默认情况下，Spring会为异步方法创建一个线程去执行，如果该方法被调用次数非常多的话，需要创建大量的线程，会导致资源浪费。<br />这时，可以定义一个线程池，异步方法将会被自动提交到线程池中执行。
```java
@Configuration
public class ThreadPoolConfig {
    @Value("${thread.pool.corePoolSize:5}")
    private int corePoolSize;
    @Value("${thread.pool.maxPoolSize:10}")
    private int maxPoolSize;
    @Value("${thread.pool.queueCapacity:200}")
    private int queueCapacity;
    @Value("${thread.pool.keepAliveSeconds:30}")
    private int keepAliveSeconds;
    @Value("${thread.pool.threadNamePrefix:ASYNC_}")
    private String threadNamePrefix;
    @Bean
    public Executor MessageExecutor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(corePoolSize);
        executor.setMaxPoolSize(maxPoolSize);
        executor.setQueueCapacity(queueCapacity);
        executor.setKeepAliveSeconds(keepAliveSeconds);
        executor.setThreadNamePrefix(threadNamePrefix);
        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        executor.initialize();
        return executor;
    }
}
```
Spring异步的核心方法：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1611839451818-e054d7a1-1694-4031-b95f-12b03b884b4f.png#align=left&display=inline&height=607&originHeight=607&originWidth=1080&size=0&status=done&style=none&width=1080)<br />根据返回值不同，处理情况也不太一样，具体分为如下情况：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1611839451684-aeffcb49-35a5-4376-8ce3-df6913dad4f7.png#align=left&display=inline&height=369&originHeight=369&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="wVbpk"></a>
### 十一、缓存
Spring Cache架构图：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1611839451682-e3cbe646-3ef9-4ef8-b094-21711954f595.png#align=left&display=inline&height=610&originHeight=610&originWidth=870&size=0&status=done&style=shadow&width=870)<br />它目前支持多种缓存：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1611839451780-b73b3edf-0811-48f8-a3d2-54ca09e55703.png#align=left&display=inline&height=342&originHeight=342&originWidth=1080&size=0&status=done&style=none&width=1080)<br />在这里以`caffeine`为例，它是Spring官方推荐的。<br />第一步，引入`caffeine`的相关jar包
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-cache</artifactId>
</dependency>
<dependency>
    <groupId>com.github.ben-manes.caffeine</groupId>
    <artifactId>caffeine</artifactId>
    <version>2.6.0</version>
</dependency>
```
第二步，配置`CacheManager`，开启`EnableCaching`
```java
@Configuration
@EnableCaching
public class CacheConfig {
    @Bean
    public CacheManager cacheManager(){
        CaffeineCacheManager cacheManager = new CaffeineCacheManager();
        //Caffeine配置
        Caffeine<Object, Object> caffeine = Caffeine.newBuilder()
                //最后一次写入后经过固定时间过期
                .expireAfterWrite(10, TimeUnit.SECONDS)
                //缓存的最大条数
                .maximumSize(1000);
        cacheManager.setCaffeine(caffeine);
        return cacheManager;
    }
}
```
第三步，使用`Cacheable`注解获取数据
```java
@Service
public class CategoryService {
   
   //category是缓存名称,#type是具体的key，可支持el表达式
   @Cacheable(value = "category", key = "#type")
   public CategoryModel getCategory(Integer type) {
       return getCategoryByType(type);
   }
   private CategoryModel getCategoryByType(Integer type) {
       System.out.println("根据不同的type:" + type + "获取不同的分类数据");
       CategoryModel categoryModel = new CategoryModel();
       categoryModel.setId(1L);
       categoryModel.setParentId(0L);
       categoryModel.setName("电器");
       categoryModel.setLevel(3);
       return categoryModel;
   }
}
```
调用`categoryService.getCategory()`方法时，先从`caffine`缓存中获取数据，如果能够获取到数据则直接返回该数据，不会进入方法体。如果不能获取到数据，则直接方法体中的代码获取到数据，然后放到`caffine`缓存中。
