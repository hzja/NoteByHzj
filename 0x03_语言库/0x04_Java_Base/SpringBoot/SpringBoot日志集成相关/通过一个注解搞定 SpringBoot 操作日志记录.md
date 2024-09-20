Java SpringBoot 日志<br />此组件解决的问题是：「谁」在「什么时间」对「什么」做了「什么事」<br />此组件目前针对 Spring-boot 做了 Autoconfig，如果是 SpringMVC，也可自己在 xml 初始化 bean
<a name="dY9qh"></a>
## 项目源码
[https://github.com/mouzt/mzt-biz-log](https://github.com/mouzt/mzt-biz-log)
<a name="ewSzo"></a>
## 基本使用
<a name="vsZQF"></a>
### Maven依赖添加SDK依赖
```xml
<dependency>
  <groupId>io.github.mouzt</groupId>
  <artifactId>bizlog-sdk</artifactId>
  <version>1.0.1</version>
</dependency>
```
<a name="OcqKl"></a>
### SpringBoot入口打开开关，添加 `@EnableLogRecord` 注解
tenant是代表租户的标识，一般一个服务或者一个业务下的多个服务都写死一个 tenant 就可以
```java
@SpringBootApplication(exclude = DataSourceAutoConfiguration.class)
@EnableTransactionManagement
@EnableLogRecord(tenant = "com.mzt.test")
public class Main {
    public static void main(String[] args) {
        SpringApplication.run(Main.class, args);
    }
}
```
<a name="npGqp"></a>
### 日志埋点
<a name="y2WL6"></a>
#### 1. 普通的记录日志

- pefix：是拼接在 bizNo 上作为 log 的一个标识。避免 bizNo 都为整数 ID 的时候和其他的业务中的 ID 重复。比如订单 ID、用户 ID 等
- bizNo：就是业务的 ID，比如订单ID，查询的时候可以根据 bizNo 查询和它相关的操作日志
- success：方法调用成功后把 success 记录在日志的内容中
- SpEL 表达式：其中用双大括号包围起来的（例如：{{#order.purchaseName}}）#order.purchaseName 是 SpEL表达式。Spring中支持的它都支持的。比如调用静态方法，三目表达式。SpEL 可以使用方法中的任何参数
```java
@LogRecordAnnotation(success = "{{#order.purchaseName}}下了一个订单,购买商品「{{#order.productName}}」,下单结果:{{#_ret}}",
                     prefix = LogRecordType.ORDER, bizNo = "{{#order.orderNo}}")
public boolean createOrder(Order order) {
    log.info("【创建订单】orderNo={}", order.getOrderNo());
    // db insert order
    return true;
}
```
此时会打印操作日志 “张三下了一个订单,购买商品「超值优惠红烧肉套餐」,下单结果:true”
<a name="MH8yE"></a>
#### 2. 期望记录失败的日志, 如果抛出异常则记录fail的日志，没有抛出记录 success 的日志
```java
@LogRecordAnnotation(
    fail = "创建订单失败，失败原因：「{{#_errorMsg}}」",
    success = "{{#order.purchaseName}}下了一个订单,购买商品「{{#order.productName}}」,下单结果:{{#_ret}}",
    prefix = LogRecordType.ORDER, bizNo = "{{#order.orderNo}}")
public boolean createOrder(Order order) {
    log.info("【创建订单】orderNo={}", order.getOrderNo());
    // db insert order
    return true;
}
```
其中的 `#_errorMsg` 是取的方法抛出异常后的异常的 errorMessage。
<a name="MyhDb"></a>
#### 3. 日志支持种类
比如一个订单的操作日志，有些操作日志是用户自己操作的，有些操作是系统运营人员做了修改产生的操作日志，系统不希望把运营的操作日志暴露给用户看到， 但是运营期望可以看到用户的日志以及运营自己操作的日志，这些操作日志的bizNo都是订单号，所以为了扩展添加了类型字段，主要是为了对日志做分类，查询方便，支持更多的业务。
```java
@LogRecordAnnotation(
    fail = "创建订单失败，失败原因：「{{#_errorMsg}}」",
    category = "MANAGER",
    success = "{{#order.purchaseName}}下了一个订单,购买商品「{{#order.productName}}」,下单结果:{{#_ret}}",
    prefix = LogRecordType.ORDER, bizNo = "{{#order.orderNo}}")
public boolean createOrder(Order order) {
    log.info("【创建订单】orderNo={}", order.getOrderNo());
    // db insert order
    return true;
}
```
<a name="KlScp"></a>
#### 4. 支持记录操作的详情或者额外信息
如果一个操作修改了很多字段，但是success的日志模版里面防止过长不能把修改详情全部展示出来，这时候需要把修改的详情保存到 detail 字段， detail 是一个 String ，需要自己序列化。这里的 `#order.toString()` 是调用了 Order 的 `toString()` 方法。如果保存 JSON，自己重写一下 Order 的 `toString()` 方法就可以。
```java
@LogRecordAnnotation(
    fail = "创建订单失败，失败原因：「{{#_errorMsg}}」",
    category = "MANAGER_VIEW",
    detail = "{{#order.toString()}}",
    success = "{{#order.purchaseName}}下了一个订单,购买商品「{{#order.productName}}」,下单结果:{{#_ret}}",
    prefix = LogRecordType.ORDER, bizNo = "{{#order.orderNo}}")
public boolean createOrder(Order order) {
    log.info("【创建订单】orderNo={}", order.getOrderNo());
    // db insert order
    return true;
}
```
<a name="zH3wE"></a>
#### 5. 指定操作日志的操作人是什么？框架提供了两种方法
第一种：手工在LogRecord的注解上指定。这种需要方法参数上有operator
```java
@LogRecordAnnotation(
    fail = "创建订单失败，失败原因：「{{#_errorMsg}}」",
    category = "MANAGER_VIEW",
    detail = "{{#order.toString()}}",
    operator = "{{#currentUser}}",
    success = "{{#order.purchaseName}}下了一个订单,购买商品「{{#order.productName}}」,下单结果:{{#_ret}}",
    prefix = LogRecordType.ORDER, bizNo = "{{#order.orderNo}}")
public boolean createOrder(Order order, String currentUser) {
    log.info("【创建订单】orderNo={}", order.getOrderNo());
    // db insert order
    return true;
}
```
这种方法手工指定，需要方法参数上有 operator 参数，或者通过 SpEL 调用静态方法获取当前用户。<br />第二种：通过默认实现类来自动的获取操作人，由于在大部分web应用中当前的用户都是保存在一个线程上下文中的，所以每个注解都加一个operator获取操作人显得有些重复劳动，所以提供了一个扩展接口来获取操作人 框架提供了一个扩展接口，使用框架的业务可以 implements 这个接口自己实现获取当前用户的逻辑， 对于使用 Springboot 的只需要实现 IOperatorGetService 接口，然后把这个 Service 作为一个单例放到 Spring 的上下文中。使用 SpringMVC的就需要自己手工装配这些 bean 了。
```java
@Configuration
public class LogRecordConfiguration {
    @Bean
    public IOperatorGetService operatorGetService() {
        return () -> Optional.of(OrgUserUtils.getCurrentUser())
            .map(a -> new OperatorDO(a.getMisId()))
            .orElseThrow(() -> new IllegalArgumentException("user is null"));
    }
}
//也可以这么搞：
@Service
public class DefaultOperatorGetServiceImpl implements IOperatorGetService {
    @Override
    public OperatorDO getUser() {
        OperatorDO operatorDO = new OperatorDO();
        operatorDO.setOperatorId("SYSTEM");
        return operatorDO;
    }
}
```
<a name="RVehn"></a>
#### 6. 日志文案调整
对于更新等方法，方法的参数上大部分都是订单ID、或者产品ID等， 比如下面的例子：日志记录的success内容是：“更新了订单{{#orderId}},更新内容为…”，这种对于运营或者产品来说难以理解，所以引入了自定义函数的功能。使用方法是在原来的变量的两个大括号之间加一个函数名称 例如 “`{ORDER{#orderId}}`” 其中 ORDER 是一个函数名称。只有一个函数名称是不够的,需要添加这个函数的定义和实现。可以看下面例子 自定义的函数需要实现框架里面的IParseFunction的接口，需要实现两个方法：

- `functionName()` 方法就返回注解上面的函数名；
- apply()函数参数是 "`{ORDER{#orderId}}`"中SpEL解析的#orderId的值，这里是一个数字1223110，接下来只需要在实现的类中把 ID 转换为可读懂的字符串就可以了， 一般为了方便排查问题需要把名称和ID都展示出来，例如："订单名称（ID）"的形式。
> 这里有个问题：加了自定义函数后，框架怎么能调用到呢？
> 答：对于SpringBoot应用很简单，只需要把它暴露在Spring的上下文中就可以了，可以加上Spring的 `@Component` 或者 `@Service` 很方便😄。SpringMVC 应用需要自己装配 Bean。

```java
// 没有使用自定义函数
@LogRecordAnnotation(success = "更新了订单{{#orderId}},更新内容为....",
                     prefix = LogRecordType.ORDER, bizNo = "{{#order.orderNo}}",
                     detail = "{{#order.toString()}}")
public boolean update(Long orderId, Order order) {
    return false;
}
//使用了自定义函数，主要是在 {{#orderId}} 的大括号中间加了 functionName
@LogRecordAnnotation(success = "更新了订单ORDER{#orderId}},更新内容为...",
                     prefix = LogRecordType.ORDER, bizNo = "{{#order.orderNo}}",
                     detail = "{{#order.toString()}}")
public boolean update(Long orderId, Order order) {
    return false;
}
// 还需要加上函数的实现
@Component
public class OrderParseFunction implements IParseFunction {
    @Resource
    @Lazy //为了避免类加载顺序的问题 最好为Lazy，没有问题也可以不加
    private OrderQueryService orderQueryService;
    @Override
    public String functionName() {
        //  函数名称为 ORDER
        return "ORDER";
    }
    @Override
    //这里的 value 可以吧 Order 的JSON对象的传递过来，然后反解析拼接一个定制的操作日志内容
    public String apply(String value) {
        if(StringUtils.isEmpty(value)){
            return value;
        }
        Order order = orderQueryService.queryOrder(Long.parseLong(value));
        //把订单产品名称加上便于理解，加上 ID 便于查问题
        return order.getProductName().concat("(").concat(value).concat(")");
    }
}
```
<a name="iXaLu"></a>
#### 7. 日志文案调整 使用 SpEL 三目表达式
```java
@LogRecordAnnotation(prefix = LogRecordTypeConstant.CUSTOM_ATTRIBUTE, bizNo = "{{#businessLineId}}",
                     success = "{{#disable ? '停用' : '启用'}}了自定义属性{ATTRIBUTE{#attributeId}}")
public CustomAttributeVO disableAttribute(Long businessLineId, Long attributeId, boolean disable) {
    return xxx;
}
```
<a name="hCenU"></a>
### 框架的扩展点
<a name="WvpRG"></a>
#### 重写OperatorGetServiceImpl通过上下文获取用户的扩展
例子如下
```java
@Service
public class DefaultOperatorGetServiceImpl implements IOperatorGetService {
    @Override
    public Operator getUser() {
        return Optional.ofNullable(UserUtils.getUser())
            .map(a -> new Operator(a.getName(), a.getLogin()))
            .orElseThrow(()->new IllegalArgumentException("user is null"));
    }
}
```
ILogRecordService 保存/查询日志的例子,使用者可以根据数据量保存到合适的存储介质上，比如保存在数据库/或者ES。自己实现保存和删除就可以了
> 也可以只实现查询的接口，毕竟已经保存在业务的存储上了，查询业务可以自己实现，不走 ILogRecordService 这个接口，毕竟会有一些千奇百怪的查询需求。

```java
@Service
public class DbLogRecordServiceImpl implements ILogRecordService {
    @Resource
    private LogRecordMapper logRecordMapper;
    @Override
    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void record(LogRecord logRecord) {
        log.info("【logRecord】log={}", logRecord);
        LogRecordPO logRecordPO = LogRecordPO.toPo(logRecord);
        logRecordMapper.insert(logRecordPO);
    }
    @Override
    public List<LogRecord> queryLog(String bizKey, Collection<String> types) {
        return Lists.newArrayList();
    }
    @Override
    public PageDO<LogRecord> queryLogByBizNo(String bizNo, Collection<String> types, PageRequestDO pageRequestDO) {
        return logRecordMapper.selectByBizNoAndCategory(bizNo, types, pageRequestDO);
    }
}
```
<a name="ivgk2"></a>
#### IParseFunction 自定义转换函数的接口
可以实现IParseFunction 实现对LogRecord注解中使用的函数扩展 例子：
```java
@Component
public class UserParseFunction implements IParseFunction {
    private final Splitter splitter = Splitter.on(",").trimResults();
    @Resource
    @Lazy
    private UserQueryService userQueryService;
    @Override
    public String functionName() {
        return "USER";
    }
    @Override
    // 11,12 返回 11(小明)，12(张三)
    public String apply(String value) {
        if (StringUtils.isEmpty(value)) {
            return value;
        }
        List<String> userIds = Lists.newArrayList(splitter.split(value));
        List<User> misDOList = userQueryService.getUserList(userIds);
        Map<String, User> userMap = StreamUtil.extractMap(misDOList, User::getId);
        StringBuilder stringBuilder = new StringBuilder();
        for (String userId : userIds) {
            stringBuilder.append(userId);
            if (userMap.get(userId) != null) {
                stringBuilder.append("(").append(userMap.get(userId).getUsername()).append(")");
            }
            stringBuilder.append(",");
        }
        return stringBuilder.toString().replaceAll(",$", "");
    }
}
```
<a name="hFue6"></a>
### 变量相关
> LogRecordAnnotation 可以使用的变量出了参数也可以使用返回值#_ret变量，以及异常的错误信息#_errorMsg，也可以通过SpEL的 T 方式调用静态方法噢

<a name="lNSth"></a>
## 待扩展
实现一个 Log的 Context，可以解决方法参数中没有的变量但是想使用的问题，作者的初步想法是可以通过在方法中 add 变量的形式实现，很快就可以实现了。
<a name="H7vzq"></a>
## 注意点
⚠️ 整体日志拦截是在方法执行之后记录的，所以对于方法内部修改了方法参数之后，LogRecordAnnotation 的注解上的 SpEL 对变量的取值是修改后的值。
