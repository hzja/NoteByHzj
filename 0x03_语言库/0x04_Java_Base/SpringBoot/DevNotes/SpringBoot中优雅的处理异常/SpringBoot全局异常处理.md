SpringBoot 
<a name="AcdGG"></a>
## 背景
软件开发过程中，不可避免的是需要处理各种异常，所以代码中就会出现大量的`try {...} catch {...} finally {...}` 代码块，不仅有大量的冗余代码，而且还影响代码的可读性。比较下面两张图。<br />丑陋的 try catch 代码块<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623511726311-7252f5cf-b17d-452f-8c19-f78fdac16714.webp#averageHue=%23f4f4f4&height=897&id=uf5769345&originHeight=897&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)<br />优雅的Controlle<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623511726337-50f4bb64-1869-4b3d-b389-9f5fbb4425bd.webp#averageHue=%23f3f3f2&height=513&id=u7bbd0faa&originHeight=513&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=1080)<br />上面的示例，还只是在Controller层，如果是在Service层，可能会有更多的try catch代码块。这将会严重影响代码的可读性、“美观性”。<br />大家肯定偏向于第二种，可以把更多的精力放在业务代码的开发，同时代码也会变得更加简洁。<br />既然业务代码不显式地对异常进行捕获、处理，而异常肯定还是处理的，不然系统岂不是动不动就崩溃了，所以必须得有其他地方捕获并处理这些异常。<br />那么问题来了，如何优雅的处理各种异常？
<a name="NOCQg"></a>
## 什么是统一异常处理
Spring在3.2版本增加了一个注解`@ControllerAdvice`，可以与`@ExceptionHandler`、`@InitBinder`、`@ModelAttribute` 等注解注解配套使用，对于这几个注解的作用，这里不做过多赘述，若有不了解的，可以参考Spring3.2新注解`@ControllerAdvice`，先大概有个了解。<br />不过跟异常处理相关的只有注解`@ExceptionHandler`，从字面上看，就是 异常处理器的意思，其实际作用也是：若在某个`Controller`类定义一个异常处理方法，并在方法上添加该注解，那么当出现指定的异常时，会执行该处理异常的方法，其可以使用SpringMVC提供的数据绑定，比如注入`HttpServletRequest`等，还可以接受一个当前抛出的Throwable对象。<br />但是，这样一来，就必须在每一个`Controller`类都定义一套这样的异常处理方法，因为异常可以是各种各样。这样一来，就会造成大量的冗余代码，而且若需要新增一种异常的处理逻辑，就必须修改所有`Controller`类了，很不优雅。<br />那就定义个类似`BaseController`的基类，这样总行了吧。<br />这种做法虽然没错，但仍不尽善尽美，因为这样的代码有一定的侵入性和耦合性。简简单单的`Controller`，为啥非得继承这样一个类呢，万一已经继承其他基类了呢。大家都知道Java只能继承一个类。<br />那有没有一种方案，既不需要跟Controller耦合，也可以将定义的 异常处理器 应用到所有控制器呢？所以注解`@ControllerAdvice`出现了，简单的说，该注解可以把异常处理器应用到所有控制器，而不是单个控制器。借助该注解，可以实现：在独立的某个地方，比如单独一个类，定义一套对各种异常的处理机制，然后在类的签名加上注解`@ControllerAdvice`，统一对 不同阶段的、不同异常 进行处理。这就是统一异常处理的原理。<br />注意到上面对异常按阶段进行分类，大体可以分成：进入`Controller`前的异常 和 Service 层异常，具体可以参考下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623511726328-115afe38-9055-43bd-9791-dd6f4c64851b.webp#averageHue=%23f8f2ed&height=339&id=ub260a842&originHeight=339&originWidth=839&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=839)<br />不同阶段的异常
<a name="sH0Dy"></a>
## 目标
消灭95%以上的 try catch 代码块，以优雅的 `Assert`(断言) 方式来校验业务的异常情况，只关注业务逻辑，而不用花费大量精力写冗余的 try catch 代码块。
<a name="eSp2y"></a>
## 统一异常处理实战
在定义统一异常处理类之前，先来介绍一下如何优雅的判定异常情况并抛异常。
<a name="Qimoi"></a>
### 用 `Assert`(断言) 替换 throw exception
想必 `Assert`(断言) 大家都很熟悉，比如 Spring 家族的 `org.springframework.util.Assert`，在写测试用例的时候经常会用到，使用断言在编码的时候有一种非一般丝滑的感觉，比如：
```java
@Test
public void test1() {
    ...
        User user = userDao.selectById(userId);
    Assert.notNull(user, "用户不存在.");
    ...
    }

@Test
public void test2() {
    // 另一种写法
    User user = userDao.selectById(userId);
    if (user == null) {
        throw new IllegalArgumentException("用户不存在.");
    }
}
```
有没有感觉第一种判定非空的写法很优雅，第二种写法则是相对丑陋的 `if {...}` 代码块。那么 神奇的 `Assert.notNull()` 背后到底做了什么呢？下面是 `Assert` 的部分源码：
```java
public abstract class Assert {
    public Assert() {
    }

    public static void notNull(@Nullable Object object, String message) {
        if (object == null) {
            throw new IllegalArgumentException(message);
        }
    }
}
```
可以看到，`Assert` 其实就是把 `if {...}` 封装了一下，是不是很神奇。虽然很简单，但不可否认的是编码体验至少提升了一个档次。那么能不能模仿`org.springframework.util.Assert`，也写一个断言类，不过断言失败后抛出的异常不是`IllegalArgumentException` 这些内置异常，而是自己定义的异常。下面来尝试一下。
```java
Assert
public interface Assert {
    /**
     * 创建异常
     * @param args
     * @return
     */
    BaseException newException(Object... args);

    /**
     * 创建异常
     * @param t
     * @param args
     * @return
     */
    BaseException newException(Throwable t, Object... args);

    /**
     * <p>断言对象<code>obj</code>非空。如果对象<code>obj</code>为空，则抛出异常
     *
     * @param obj 待判断对象
     */
    default void assertNotNull(Object obj) {
        if (obj == null) {
            throw newException(obj);
        }
    }

    /**
     * <p>断言对象<code>obj</code>非空。如果对象<code>obj</code>为空，则抛出异常
     * <p>异常信息<code>message</code>支持传递参数方式，避免在判断之前进行字符串拼接操作
     *
     * @param obj 待判断对象
     * @param args message占位符对应的参数列表
     */
    default void assertNotNull(Object obj, Object... args) {
        if (obj == null) {
            throw newException(args);
        }
    }
}
```
上面的`Assert`断言方法是使用接口的默认方法定义的，然后有没有发现当断言失败后，抛出的异常不是具体的某个异常，而是交由2个`newException`接口方法提供。因为业务逻辑中出现的异常基本都是对应特定的场景，比如根据用户id获取用户信息，查询结果为`null`，此时抛出的异常可能为`UserNotFoundException`，并且有特定的异常码（比如7001）和异常信息“用户不存在”。所以具体抛出什么异常，有`Assert`的实现类决定。<br />看到这里，可能会有这样的疑问，按照上面的说法，那岂不是有多少异常情况，就得有定义等量的断言类和异常类，这显然是反人类的，这也没想象中高明。
<a name="uvTot"></a>
#### 善解人意的`Enum`
自定义异常`BaseException`有2个属性，即`code`、`message`，这样一对属性，有没有想到什么类一般也会定义这2个属性？没错，就是枚举类。将 `Enum` 和 `Assert`结合起来：
```java
public interface IResponseEnum {
    int getCode();
    String getMessage();
}
/**
 * <p>业务异常</p>
 * <p>业务处理时，出现异常，可以抛出该异常</p>
 */
public class BusinessException extends  BaseException {

    private static final long serialVersionUID = 1L;

    public BusinessException(IResponseEnum responseEnum, Object[] args, String message) {
        super(responseEnum, args, message);
    }

    public BusinessException(IResponseEnum responseEnum, Object[] args, String message, Throwable cause) {
        super(responseEnum, args, message, cause);
    }
}
public interface BusinessExceptionAssert extends IResponseEnum, Assert {

    @Override
    default BaseException newException(Object... args) {
        String msg = MessageFormat.format(this.getMessage(), args);

        return new BusinessException(this, args, msg);
    }

    @Override
    default BaseException newException(Throwable t, Object... args) {
        String msg = MessageFormat.format(this.getMessage(), args);

        return new BusinessException(this, args, msg, t);
    }

}
@Getter
@AllArgsConstructor
public enum ResponseEnum implements BusinessExceptionAssert {

    /**
     * Bad licence type
     */
    BAD_LICENCE_TYPE(7001, "Bad licence type."),
    /**
     * Licence not found
     */
    LICENCE_NOT_FOUND(7002, "Licence not found.")
    ;

    /**
     * 返回码
     */
    private int code;
    /**
     * 返回消息
     */
    private String message;
}
```
代码示例中定义了两个枚举实例：`BAD_LICENCE_TYPE`、`LICENCE_NOT_FOUND`，分别对应了`BadLicenceTypeException`、`LicenceNotFoundException`两种异常。以后每增加一种异常情况，只需增加一个枚举实例即可，再也不用每一种异常都定义一个异常类了。然后再来看下如何使用，假设`LicenceService`有校验`Licence`是否存在的方法，如下：
```java
/**
 * 校验{@link Licence}存在
 * @param licence
 */
private void checkNotNull(Licence licence) {
    ResponseEnum.LICENCE_NOT_FOUND.assertNotNull(licence);
}
```
若不使用断言，代码可能如下：
```java
private void checkNotNull(Licence licence) {
    if (licence == null) {
        throw new LicenceNotFoundException();
        // 或者这样
        throw new BusinessException(7001, "Bad licence type.");
    }
}
```
使用枚举类结合(继承)`Assert`，只需根据特定的异常情况定义不同的枚举实例，如上面的`BAD_LICENCE_TYPE`、`LICENCE_NOT_FOUND`，就能够针对不同情况抛出特定的异常（这里指携带特定的异常码和异常消息），这样既不用定义大量的异常类，同时还具备了断言的良好可读性，当然这种方案的好处远不止这些，请继续阅读后文，慢慢体会。<br />注：上面举的例子是针对特定的业务，而有部分异常情况是通用的，比如：服务器繁忙、网络异常、服务器异常、参数校验异常、404等，所以有`CommonResponseEnum`、`ArgumentResponseEnum`、`ServletResponseEnum`，其中 `ServletResponseEnum` 会在后文详细说明。
<a name="MQa5S"></a>
### 定义统一异常处理器类
```java
@Slf4j
@Component
@ControllerAdvice
@ConditionalOnWebApplication
@ConditionalOnMissingBean(UnifiedExceptionHandler.class)
public class UnifiedExceptionHandler {
    /**
     * 生产环境
     */
    private final static String ENV_PROD = "prod";

    @Autowired
    private UnifiedMessageSource unifiedMessageSource;

    /**
     * 当前环境
     */
    @Value("${spring.profiles.active}")
    private String profile;

    /**
     * 获取国际化消息
     *
     * @param e 异常
     * @return
     */
    public String getMessage(BaseException e) {
        String code = "response." + e.getResponseEnum().toString();
        String message = unifiedMessageSource.getMessage(code, e.getArgs());

        if (message == null || message.isEmpty()) {
            return e.getMessage();
        }

        return message;
    }

    /**
     * 业务异常
     *
     * @param e 异常
     * @return 异常结果
     */
    @ExceptionHandler(value = BusinessException.class)
    @ResponseBody
    public ErrorResponse handleBusinessException(BaseException e) {
        log.error(e.getMessage(), e);

        return new ErrorResponse(e.getResponseEnum().getCode(), getMessage(e));
    }

    /**
     * 自定义异常
     *
     * @param e 异常
     * @return 异常结果
     */
    @ExceptionHandler(value = BaseException.class)
    @ResponseBody
    public ErrorResponse handleBaseException(BaseException e) {
        log.error(e.getMessage(), e);

        return new ErrorResponse(e.getResponseEnum().getCode(), getMessage(e));
    }

    /**
     * Controller上一层相关异常
     *
     * @param e 异常
     * @return 异常结果
     */
    @ExceptionHandler({
        NoHandlerFoundException.class,
        HttpRequestMethodNotSupportedException.class,
        HttpMediaTypeNotSupportedException.class,
        MissingPathVariableException.class,
        MissingServletRequestParameterException.class,
        TypeMismatchException.class,
        HttpMessageNotReadableException.class,
        HttpMessageNotWritableException.class,
        // BindException.class,
        // MethodArgumentNotValidException.class
        HttpMediaTypeNotAcceptableException.class,
        ServletRequestBindingException.class,
        ConversionNotSupportedException.class,
        MissingServletRequestPartException.class,
        AsyncRequestTimeoutException.class
            })
    @ResponseBody
    public ErrorResponse handleServletException(Exception e) {
        log.error(e.getMessage(), e);
        int code = CommonResponseEnum.SERVER_ERROR.getCode();
        try {
            ServletResponseEnum servletExceptionEnum = ServletResponseEnum.valueOf(e.getClass().getSimpleName());
            code = servletExceptionEnum.getCode();
        } catch (IllegalArgumentException e1) {
            log.error("class [{}] not defined in enum {}", e.getClass().getName(), ServletResponseEnum.class.getName());
        }

        if (ENV_PROD.equals(profile)) {
            // 当为生产环境, 不适合把具体的异常信息展示给用户, 比如404.
            code = CommonResponseEnum.SERVER_ERROR.getCode();
            BaseException baseException = new BaseException(CommonResponseEnum.SERVER_ERROR);
            String message = getMessage(baseException);
            return new ErrorResponse(code, message);
        }

        return new ErrorResponse(code, e.getMessage());
    }


    /**
     * 参数绑定异常
     *
     * @param e 异常
     * @return 异常结果
     */
    @ExceptionHandler(value = BindException.class)
    @ResponseBody
    public ErrorResponse handleBindException(BindException e) {
        log.error("参数绑定校验异常", e);

        return wrapperBindingResult(e.getBindingResult());
    }

    /**
     * 参数校验异常，将校验失败的所有异常组合成一条错误信息
     *
     * @param e 异常
     * @return 异常结果
     */
    @ExceptionHandler(value = MethodArgumentNotValidException.class)
    @ResponseBody
    public ErrorResponse handleValidException(MethodArgumentNotValidException e) {
        log.error("参数绑定校验异常", e);

        return wrapperBindingResult(e.getBindingResult());
    }

    /**
     * 包装绑定异常结果
     *
     * @param bindingResult 绑定结果
     * @return 异常结果
     */
    private ErrorResponse wrapperBindingResult(BindingResult bindingResult) {
        StringBuilder msg = new StringBuilder();

        for (ObjectError error : bindingResult.getAllErrors()) {
            msg.append(", ");
            if (error instanceof FieldError) {
                msg.append(((FieldError) error).getField()).append(": ");
            }
            msg.append(error.getDefaultMessage() == null ? "" : error.getDefaultMessage());

        }

        return new ErrorResponse(ArgumentResponseEnum.VALID_ERROR.getCode(), msg.substring(2));
    }

    /**
     * 未定义异常
     *
     * @param e 异常
     * @return 异常结果
     */
    @ExceptionHandler(value = Exception.class)
    @ResponseBody
    public ErrorResponse handleException(Exception e) {
        log.error(e.getMessage(), e);

        if (ENV_PROD.equals(profile)) {
            // 当为生产环境, 不适合把具体的异常信息展示给用户, 比如数据库异常信息.
            int code = CommonResponseEnum.SERVER_ERROR.getCode();
            BaseException baseException = new BaseException(CommonResponseEnum.SERVER_ERROR);
            String message = getMessage(baseException);
            return new ErrorResponse(code, message);
        }
        return new ErrorResponse(CommonResponseEnum.SERVER_ERROR.getCode(), e.getMessage());
    }
}
```
可以看到，上面将异常分成几类，实际上只有两大类，一类是`ServletException`、`ServiceException`，还记得上文提到的 按阶段分类 吗，即对应 进入`Controller`前的异常 和 Service 层异常；然后 ServiceException 再分成自定义异常、未知异常。对应关系如下：

- 进入Controller前的异常: `handleServletException`、`handleBindException`、`handleValidException`
- 自定义异常: `handleBusinessException`、`handleBaseException`
- 未知异常: `handleException`

接下来分别对这几种异常处理器做详细说明。
<a name="wJdxl"></a>
#### 异常处理器说明
<a name="XH8IS"></a>
##### `handleServletException`
一个http请求，在到达Controller前，会对该请求的请求信息与目标控制器信息做一系列校验。这里简单说一下：<br />`NoHandlerFoundException`：首先根据请求Url查找有没有对应的控制器，若没有则会抛该异常，也就是大家非常熟悉的404异常；<br />`HttpRequestMethodNotSupportedException`：若匹配到了（匹配结果是一个列表，不同的是http方法不同，如：Get、Post等），则尝试将请求的http方法与列表的控制器做匹配，若没有对应http方法的控制器，则抛该异常；<br />`HttpMediaTypeNotSupportedException`：然后再对请求头与控制器支持的做比较，比如`content-type`请求头，若控制器的参数签名包含注解`@RequestBody`，但是请求的`content-type`请求头的值没有包含`application/json`，那么会抛该异常（当然，不止这种情况会抛这个异常）；<br />`MissingPathVariableException`：未检测到路径参数。比如url为：`/licence/{licenceId}`，参数签名包含`@PathVariable("licenceId")`，当请求的url为`/licence`，在没有明确定义url为`/licence`的情况下，会被判定为：缺少路径参数；<br />`MissingServletRequestParameterException`：缺少请求参数。比如定义了参数`@RequestParam("licenceId") String licenceId`，但发起请求时，未携带该参数，则会抛该异常；<br />`TypeMismatchException`: 参数类型匹配失败。比如：接收参数为Long型，但传入的值确是一个字符串，那么将会出现类型转换失败的情况，这时会抛该异常；<br />`HttpMessageNotReadableException`：与上面的`HttpMediaTypeNotSupportedException`举的例子完全相反，即请求头携带了`"content-type: application/json;charset=UTF-8"`，但接收参数却没有添加注解`@RequestBody`，或者请求体携带的 json 串反序列化成 pojo 的过程中失败了，也会抛该异常；<br />`HttpMessageNotWritableException`：返回的 pojo 在序列化成 json 过程失败了，那么抛该异常；
<a name="EaWru"></a>
##### `handleBindException`
参数校验异常。
<a name="i9FGL"></a>
##### `handleValidException`
参数校验异常。
<a name="gGD8I"></a>
##### `handleBusinessException`、`handleBaseException`
处理自定义的业务异常，只是`handleBaseException`处理的是除了 `BusinessException`意外的所有业务异常。就目前来看，这2个是可以合并成一个的。
<a name="x0vy0"></a>
##### `handleException`
处理所有未知的异常，比如操作数据库失败的异常。<br />注：上面的`handleServletException`、`handleException` 这两个处理器，返回的异常信息，不同环境返回的可能不一样，以为这些异常信息都是框架自带的异常信息，一般都是英文的，不太好直接展示给用户看，所以统一返回SERVER_ERROR代表的异常信息。
<a name="ME3H4"></a>
#### 异于常人的404
上文提到，当请求没有匹配到控制器的情况下，会抛出`NoHandlerFoundException`异常，但其实默认情况下不是这样，默认情况下会出现类似如下页面：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511726440-788756b8-ffee-4808-b2e2-e52f1b5fabc4.png#averageHue=%23e7e8e3&height=295&id=u9de6dd2f&originHeight=295&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=1080)<br />Whitelabel Error Page<br />这个页面是如何出现的呢？实际上，当出现404的时候，默认是不抛异常的，而是 forward跳转到/error控制器，Spring也提供了默认的error控制器，如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511726256-ca2d0735-3e5b-4f2f-9af5-340e01651c90.png#averageHue=%23d9e4e0&height=671&id=u9d5338a1&originHeight=671&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=1080)<br />那么，如何让404也抛出异常呢，只需在properties文件中加入如下配置即可：
```java
spring.mvc.throw-exception-if-no-handler-found=true
spring.resources.add-mappings=false
```
如此，就可以异常处理器中捕获它了，然后前端只要捕获到特定的状态码，立即跳转到404页面即可<br />捕获404对应的异常
<a name="ibl5S"></a>
#### 统一返回结果
在验证统一异常处理器之前，顺便说一下统一返回结果。说白了，其实是统一一下返回结果的数据结构。code、message 是所有返回结果中必有的字段，而当需要返回数据时，则需要另一个字段 data 来表示。<br />所以首先定义一个 `BaseResponse` 来作为所有返回结果的基类；<br />然后定义一个通用返回结果类`CommonResponse`，继承 `BaseResponse`，而且多了字段 data；<br />为了区分成功和失败返回结果，于是再定义一个 ErrorResponse<br />最后还有一种常见的返回结果，即返回的数据带有分页信息，因为这种接口比较常见，所以有必要单独定义一个返回结果类 `QueryDataResponse`，该类继承自 `CommonResponse`，只是把 data 字段的类型限制为 `QueryDdata`，`QueryDdata`中定义了分页信息相应的字段，即`totalCount`、`pageNo`、 `pageSize`、`records`。<br />其中比较常用的只有 `CommonResponse` 和 `QueryDataResponse`，但是名字又贼鬼死长，何不定义2个名字超简单的类来替代呢？于是 R 和 QR 诞生了，以后返回结果的时候只需这样写：new R<>(data)、new QR<>(queryData)。<br />所有的返回结果类的定义这里就不贴出来了
<a name="TPbyG"></a>
#### 验证统一异常处理
因为这一套统一异常处理可以说是通用的，所有可以设计成一个 common包，以后每一个新项目/模块只需引入该包即可。所以为了验证，需要新建一个项目，并引入该 common包。
<a name="VWOSR"></a>
##### 主要代码
下面是用于验证的主要源码：
```java
@Service
public class LicenceService extends ServiceImpl<LicenceMapper, Licence> {

    @Autowired
    private OrganizationClient organizationClient;

    /**
     * 查询{@link Licence} 详情
     * @param licenceId
     * @return
     */
    public LicenceDTO queryDetail(Long licenceId) {
        Licence licence = this.getById(licenceId);
        checkNotNull(licence);

        OrganizationDTO org = ClientUtil.execute(() -> organizationClient.getOrganization(licence.getOrganizationId()));
        return toLicenceDTO(licence, org);
    }

    /**
     * 分页获取
     * @param licenceParam 分页查询参数
     * @return
     */
    public QueryData<SimpleLicenceDTO> getLicences(LicenceParam licenceParam) {
        String licenceType = licenceParam.getLicenceType();
        LicenceTypeEnum licenceTypeEnum = LicenceTypeEnum.parseOfNullable(licenceType);
        // 断言, 非空
        ResponseEnum.BAD_LICENCE_TYPE.assertNotNull(licenceTypeEnum);

        LambdaQueryWrapper<Licence> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(Licence::getLicenceType, licenceType);
        IPage<Licence> page = this.page(new QueryPage<>(licenceParam), wrapper);
        return new QueryData<>(page, this::toSimpleLicenceDTO);
    }

    /**
     * 新增{@link Licence}
     * @param request 请求体
     * @return
     */
    @Transactional(rollbackFor = Throwable.class)
    public LicenceAddRespData addLicence(LicenceAddRequest request) {
        Licence licence = new Licence();
        licence.setOrganizationId(request.getOrganizationId());
        licence.setLicenceType(request.getLicenceType());
        licence.setProductName(request.getProductName());
        licence.setLicenceMax(request.getLicenceMax());
        licence.setLicenceAllocated(request.getLicenceAllocated());
        licence.setComment(request.getComment());
        this.save(licence);

        return new LicenceAddRespData(licence.getLicenceId());
    }

    /**
     * entity -> simple dto
     * @param licence {@link Licence} entity
     * @return {@link SimpleLicenceDTO}
     */
    private SimpleLicenceDTO toSimpleLicenceDTO(Licence licence) {
        // 省略
    }

    /**
     * entity -> dto
     * @param licence {@link Licence} entity
     * @param org {@link OrganizationDTO}
     * @return {@link LicenceDTO}
     */
    private LicenceDTO toLicenceDTO(Licence licence, OrganizationDTO org) {
        // 省略
    }

    /**
     * 校验{@link Licence}存在
     * @param licence
     */
    private void checkNotNull(Licence licence) {
        ResponseEnum.LICENCE_NOT_FOUND.assertNotNull(licence);
    }
}
```
PS: 这里使用的DAO框架是mybatis-plus。启动时，自动插入的数据为：
```sql
-- licence
INSERT INTO licence (licence_id, organization_id, licence_type, product_name, licence_max, licence_allocated)
VALUES (1, 1, 'user','CustomerPro', 100,5);
INSERT INTO licence (licence_id, organization_id, licence_type, product_name, licence_max, licence_allocated)
VALUES (2, 1, 'user','suitability-plus', 200,189);
INSERT INTO licence (licence_id, organization_id, licence_type, product_name, licence_max, licence_allocated)
VALUES (3, 2, 'user','HR-PowerSuite', 100,4);
INSERT INTO licence (licence_id, organization_id, licence_type, product_name, licence_max, licence_allocated)
VALUES (4, 2, 'core-prod','WildCat Application Gateway', 16,16);

-- organizations
INSERT INTO organization (id, name, contact_name, contact_email, contact_phone)
VALUES (1, 'customer-crm-co', 'Mark Balster', 'mark.balster@custcrmco.com', '823-555-1212');
INSERT INTO organization (id, name, contact_name, contact_email, contact_phone)
VALUES (2, 'HR-PowerSuite', 'Doug Drewry','doug.drewry@hr.com', '920-555-1212');
```
<a name="RTUCT"></a>
##### 开始验证
<a name="immMM"></a>
###### 捕获自定义异常

1. 获取不存在的 licence 详情：http://localhost:10000/licence/5。成功响应的请求：licenceId=1

检验非空<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511727040-29575225-6a7a-4d1a-87a7-9966b6f6d0fe.png#averageHue=%238ba0c0&height=224&id=ufe0b5d58&originHeight=224&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=1080)<br />捕获 Licence not found 异常<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511727453-378304fc-a2fd-49ea-8ea6-fe992162cc23.png#averageHue=%23e2ece9&height=441&id=uf3a774f8&originHeight=441&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=1080)<br />Licence not found<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511727465-0a220f8b-9c3e-4835-bd0f-6dcde552a411.png#averageHue=%23f0f0f0&height=164&id=uc192407e&originHeight=164&originWidth=604&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=604)

2. 根据不存在的 licence type 获取 licence 列表：http://localhost:10000/licence/list?licenceType=ddd。可选的 licence type 为：user、core-prod 。

校验非空<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511727394-07967825-9cf0-48fb-9b79-25837d158e2c.png#averageHue=%23c9d5d7&height=249&id=udc7e0264&originHeight=249&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)<br />捕获 Bad licence type 异常<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511727816-c483ca5c-5124-4584-bfbf-956e0df49ec2.png#averageHue=%23e6eeec&height=357&id=u2b62f658&originHeight=357&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=1080)<br />Bad licence type<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511727766-d8414e6e-cbef-4d9b-b4ba-97f9661796e9.png#averageHue=%23ededed&height=164&id=u28053e9f&originHeight=164&originWidth=608&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=608)
<a name="rjHHW"></a>
###### 捕获进入 Controller 前的异常

1. 访问不存在的接口：http://localhost:10000/licence/list/ddd

捕获404异常<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511728099-1ae13e3e-cbda-4b0c-b875-3328e8f68e86.png#averageHue=%23d0dfde&height=621&id=u53535ad4&originHeight=621&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)

2. http 方法不支持：http://localhost:10000/licence

PostMapping<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511728561-06b2e6f7-cf83-4ef1-8f87-2859f15acd62.png#averageHue=%23eaf0ec&height=382&id=ue0fee7fc&originHeight=382&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)<br />捕获 Request method not supported 异常<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511728589-06c117a2-73ca-43d8-b7cc-d5f06a27e8af.png#averageHue=%23c7d4d8&height=757&id=u93a0e31e&originHeight=757&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)<br />Request method not supported<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511728806-56b7c023-a4c6-48c9-b107-52a7236604c3.png#averageHue=%23f0f0f0&height=156&id=u0b511d75&originHeight=156&originWidth=808&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=808)

3. 校验异常1：http://localhost:10000/licence/list?licenceType=

getLicences<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511729085-9aba6552-af49-492b-9098-3582d9ae0267.png#averageHue=%23d1d7c7&height=134&id=u4f904d6d&originHeight=134&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)<br />LicenceParam<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511729166-12a06ceb-4f6b-4deb-8040-fab93e55a2be.png#averageHue=%23dbe6d9&height=285&id=u1b36c5a0&originHeight=285&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)
<a name="owx5n"></a>
###### 捕获参数绑定校验异常
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511729393-2baa467f-cb02-4950-a642-808454f5ee54.png#averageHue=%2394accf&height=597&id=ubac92cdc&originHeight=597&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=1080)<br />licence type cannot be empty<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511729600-689f7389-9eaf-4e63-b689-47ceb11ac8ad.png#averageHue=%23d9e0d0&height=160&id=u0c2d44f0&originHeight=160&originWidth=976&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=976)

4. 校验异常2：post 请求，这里使用postman模拟。

addLicence<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511729771-358bc9ca-c357-4333-ac77-3a5375c8f74b.png#averageHue=%23f1f5f2&height=108&id=u397039fe&originHeight=108&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)<br />LicenceAddRequest<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511729897-a38c0a54-93e9-436c-b95c-9fe3f8191be2.png#averageHue=%23dfe4e6&height=704&id=u7dedcbe5&originHeight=704&originWidth=852&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=852)<br />请求url即结果<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511729897-27a7731d-be45-4d4b-b533-2a9be4c22059.png#averageHue=%23f0f7f3&height=647&id=uc37abb10&originHeight=647&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)
<a name="KUvEN"></a>
###### 捕获参数绑定校验异常
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511730342-f437ae03-3142-477d-b2b3-8bff08e436cd.png#averageHue=%23e1e9e4&height=609&id=ud96839c9&originHeight=609&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)<br />注：因为参数绑定校验异常的异常信息的获取方式与其它异常不一样，所以才把这2种情况的异常从 进入 Controller 前的异常 单独拆出来，下面是异常信息的收集逻辑：<br />异常信息的收集<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511730404-1481804d-d94c-4729-b9a4-17e9dd601756.png#averageHue=%23d9e3e0&height=405&id=u96480f07&originHeight=405&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)
<a name="YOA8C"></a>
###### 捕获未知异常
假设现在随便对 Licence 新增一个字段 test，但不修改数据库表结构，然后访问：http://localhost:10000/licence/1。<br />增加test字段<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623511730508-103390ca-5dfb-44ac-bd06-d78dc3039f1e.webp#averageHue=%23f0f4f3&height=444&id=ude984200&originHeight=444&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)
<a name="RUnLK"></a>
###### 捕获数据库异常
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511730812-4211b10d-29e0-47d8-82b0-dd804c0d7e9a.png#averageHue=%23d9e4e4&height=475&id=u8a20e7b4&originHeight=475&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)<br />Error querying database<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511731326-d406d6af-58a3-432a-8b0f-980eb7d5f379.png#averageHue=%23e1e6dc&height=118&id=u9e7c9c8a&originHeight=118&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=1080)
<a name="cr2ko"></a>
#### 小结
可以看到，测试的异常都能够被捕获，然后以 code、message 的形式返回。每一个项目/模块，在定义业务异常的时候，只需定义一个枚举类，然后实现接口 `BusinessExceptionAssert`，最后为每一种业务异常定义对应的枚举实例即可，而不用定义许多异常类。使用的时候也很方便，用法类似断言。
<a name="V9KmA"></a>
### 扩展
在生产环境，若捕获到 未知异常 或者 `ServletException`，因为都是一长串的异常信息，若直接展示给用户看，显得不够专业，于是，可以这样做：当检测到当前环境是生产环境，那么直接返回 "网络异常"。<br />生产环境返回“网络异常”<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511731070-1eec43eb-4f03-4ab1-aa78-71b76c635bf9.png#averageHue=%23f2f2f2&height=148&id=u3a0ebeee&originHeight=148&originWidth=436&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=436)<br />可以通过以下方式修改当前环境：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511731239-f880bec4-84ee-4c99-9e25-fa8789a33ae5.png#averageHue=%23f4f8f5&height=288&id=u17449dff&originHeight=288&originWidth=668&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=668)<br />修改当前环境为生产环境
<a name="A9N4B"></a>
## 总结
使用 断言 和 枚举类 相结合的方式，再配合统一异常处理，基本大部分的异常都能够被捕获。为什么说大部分异常，因为当引入 spring cloud security 后，还会有认证/授权异常，网关的服务降级异常、跨模块调用异常、远程调用第三方服务异常等，这些异常的捕获方式与本文介绍的不太一样。<br />另外，当需要考虑国际化的时候，捕获异常后的异常信息一般不能直接返回，需要转换成对应的语言，不过本文已考虑到了这个，获取消息的时候已经做了国际化映射，逻辑如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623511731468-eec608bf-c820-49eb-82eb-184f7356b61b.png#averageHue=%23d7e2d8&height=470&id=u53fbe8cd&originHeight=470&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)<br />获取国际化消息<br />最后总结，全局异常属于老生长谈的话题，可以参考以上内容根据实际情况自行修改。<br />也可以采用以下的jsonResult对象的方式进行处理，也贴出来代码.
```java
@Slf4j
@RestControllerAdvice
public class GlobalExceptionHandler {

    /**
     * 没有登录
     * @param request
     * @param response
     * @param e
     * @return
     */
    @ExceptionHandler(NoLoginException.class)
    public Object noLoginExceptionHandler(HttpServletRequest request,HttpServletResponse response,Exception e)
    {
        log.error("[GlobalExceptionHandler][noLoginExceptionHandler] exception",e);
        JsonResult jsonResult = new JsonResult();
        jsonResult.setCode(JsonResultCode.NO_LOGIN);
        jsonResult.setMessage("用户登录失效或者登录超时,请先登录");
        return jsonResult;
    }

    /**
     * 业务异常
     * @param request
     * @param response
     * @param e
     * @return
     */
    @ExceptionHandler(ServiceException.class)
    public Object businessExceptionHandler(HttpServletRequest request,HttpServletResponse response,Exception e)
    {
        log.error("[GlobalExceptionHandler][businessExceptionHandler] exception",e);
        JsonResult jsonResult = new JsonResult();
        jsonResult.setCode(JsonResultCode.FAILURE);
        jsonResult.setMessage("业务异常,请联系管理员");
        return jsonResult;
    }

    /**
     * 全局异常处理
     * @param request
     * @param response
     * @param e
     * @return
     */
    @ExceptionHandler(Exception.class)
    public Object exceptionHandler(HttpServletRequest request,HttpServletResponse response,Exception e)
    {
        log.error("[GlobalExceptionHandler][exceptionHandler] exception",e);
        JsonResult jsonResult = new JsonResult();
        jsonResult.setCode(JsonResultCode.FAILURE);
        jsonResult.setMessage("系统错误,请联系管理员");
        return jsonResult;
    }
}
```
