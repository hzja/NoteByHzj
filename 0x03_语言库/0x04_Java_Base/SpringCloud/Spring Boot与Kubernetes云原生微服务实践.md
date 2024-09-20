Java SpringBoot Kubernetes 云原生 微服务
<a name="yY7va"></a>
## 案例需求介绍
<a name="fm3Q3"></a>
### staffjoy 公司背景

1. 硅谷初创公司(2015~2017)
2. 工时排班( Scheduling ) saaS服务
3. 开源
   1. [https://github.com/staffjoy/v2](https://github.com/staffjoy/v2)
   2. 原版Golang
   3. 课程教学版Java/Spring
<a name="tzdjI"></a>
## Dubbo、SpringCloud和K8s该如何选型？
<a name="THqAx"></a>
### 微服务公共关注点
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630970984346-49c3d82e-3b97-4208-bfcf-f9077de9876c.jpeg)
<a name="Sn9u1"></a>
### Dubbo、SpringCloud和K8s横向对比
| <br /> | **Dubbo** | **SpringCloud** | **K8s** |
| --- | --- | --- | --- |
| **服务发现和LB** | **ZK/Nacos+Client** | **Eureka+Ribbon** | **Service** |
| **API网关** | **NA** | **Zuul** | **Ingress** |
| **配置管理** | **Diamond** | **Spring Cloud Config** | **ConfigMaps/Secrets** |
| **容错限流** | **Sentinel** | **Hystrix** | **HealthCheck/Probe/ServiceMesh** |
| **日志监控** | **ELK** | **ELK** | **EFK** |
| **Metrics监控** | **Dubbo Admin/Monitor** | **Actuator/MicroMeter+Prometheus** | **Heapster+Prometheus** |
| **调用链监控** | **NA** | **SpringCloud Sleuth/Zipkin** | **Jaeger/Zipkin** |

| <br /> | **Dubbo** | **SpringCloud** | **K8s** |
| --- | --- | --- | --- |
| **应用打包** | **Jar/War** | **Uber Jar/War** | **Docker Image/Helm** |
| **服务框架** | **Dubbo RPC** | **Spring(Boot) REST** | **框架无关** |
| **发布和调度** | **NA** | **NA** | **Scheduler** |
| **自动伸缩和自愈** | **NA** | **NA** | **Scheduler/AutoScaler** |
| **进程隔离** | **NA** | **NA** | **Docker/Pod** |
| **环境管理** | **NA** | **NA** | **Namespace/Authorization** |
| **资源配额** | **NA** | **NA** | **CPU/Mem Limit,Namespace Quotas** |
| **流量治理** | **ZK+Client** | **NA** | **ServiceMesh** |

<a name="QfY1x"></a>
### 优劣比对
| <br /> | **Dubbo** | **SpringCloud** | **K8s** |
| --- | --- | --- | --- |
| **亮点** | **阿里背书**<br />**成熟稳定**<br />**RPC高性能**<br />**流量治理** | **Netflix/Pivotal背书**<br />**社区活跃**<br />**开发体验好**<br />**抽象组件化好** | **谷歌背书**<br />**平台抽象**<br />**全面覆盖微服务关注点（发布）**<br />**语言栈无关**<br />**社区活跃** |
| **不足** | **技术较老**<br />**耦合性高**<br />**JVM Only**<br />**国外社区小** | **JVM Only**<br />**运行耗资源** | **偏DevOps和运维**<br />**重量复杂**<br />**技术门槛高** |

<a name="FlheF"></a>
## 技术中台
<a name="SvMgw"></a>
### 阿里巴巴中台体系
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630995135930-44ea9883-38fa-4f7b-b66c-93f740022742.jpeg)
<a name="W8Mym"></a>
### eBay中台架构
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630995243575-69a445c9-6201-4eaf-9957-f407671b116e.png#clientId=u8400404e-bd80-4&from=paste&height=318&id=ub87aa934&originHeight=954&originWidth=1256&originalType=binary&ratio=1&size=1468464&status=done&style=none&taskId=u6b86906f-34e8-486b-9f27-2f7de95babb&width=418.6666666666667)
<a name="dhYuc"></a>
### 拍拍贷中台架构
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630995335728-2305eb2e-d072-447b-91e7-4e6dba60c61d.png#clientId=u8400404e-bd80-4&from=paste&height=267&id=u8e8b6189&originHeight=800&originWidth=1669&originalType=binary&ratio=1&size=717093&status=done&style=shadow&taskId=u45d8ad95-b764-4be4-99c2-4f81e8bfa45&width=556.3333333333334)
<a name="Rzkf1"></a>
## 单体仓库（Mono-Repo）
<a name="TQS3u"></a>
### 多仓库和单体仓库对比
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1630997295738-3f4da810-ca41-4c20-86d0-eb7807ed71fc.jpeg)
<a name="qURwE"></a>
### 谁在用单体应用仓库
<a name="hLFfl"></a>
#### Google
[https://bazel.build/](https://bazel.build/)
<a name="JUwAP"></a>
#### FaceBook
[https://buck.build/](https://buck.build/)
<a name="fBrTE"></a>
## 项目架构设计
<a name="TONa3"></a>
### 接口参数校验
<a name="rCQGE"></a>
### 统一异常处理
```java
@RestControllerAdvice
public class GlobalExceptionTranslator {

    static final ILogger logger = SLoggerFactory.getLogger(GlobalExceptionTranslator.class);

    @ExceptionHandler(MissingServletRequestParameterException.class)
    public BaseResponse handleError(MissingServletRequestParameterException e) {
        logger.warn("Missing Request Parameter", e);
        String message = String.format("Missing Request Parameter: %s", e.getParameterName());
        return BaseResponse
                .builder()
                .code(ResultCode.PARAM_MISS)
                .message(message)
                .build();
    }

    @ExceptionHandler(MethodArgumentTypeMismatchException.class)
    public BaseResponse handleError(MethodArgumentTypeMismatchException e) {
        logger.warn("Method Argument Type Mismatch", e);
        String message = String.format("Method Argument Type Mismatch: %s", e.getName());
        return BaseResponse
                .builder()
                .code(ResultCode.PARAM_TYPE_ERROR)
                .message(message)
                .build();
    }

    @ExceptionHandler(MethodArgumentNotValidException.class)
    public BaseResponse handleError(MethodArgumentNotValidException e) {
        logger.warn("Method Argument Not Valid", e);
        BindingResult result = e.getBindingResult();
        FieldError error = result.getFieldError();
        String message = String.format("%s:%s", error.getField(), error.getDefaultMessage());
        return BaseResponse
                .builder()
                .code(ResultCode.PARAM_VALID_ERROR)
                .message(message)
                .build();
    }

    @ExceptionHandler(BindException.class)
    public BaseResponse handleError(BindException e) {
        logger.warn("Bind Exception", e);
        FieldError error = e.getFieldError();
        String message = String.format("%s:%s", error.getField(), error.getDefaultMessage());
        return BaseResponse
                .builder()
                .code(ResultCode.PARAM_BIND_ERROR)
                .message(message)
                .build();
    }

    @ExceptionHandler(ConstraintViolationException.class)
    public BaseResponse handleError(ConstraintViolationException e) {
        logger.warn("Constraint Violation", e);
        Set<ConstraintViolation<?>> violations = e.getConstraintViolations();
        ConstraintViolation<?> violation = violations.iterator().next();
        String path = ((PathImpl) violation.getPropertyPath()).getLeafNode().getName();
        String message = String.format("%s:%s", path, violation.getMessage());
        return BaseResponse
                .builder()
                .code(ResultCode.PARAM_VALID_ERROR)
                .message(message)
                .build();
    }

    @ExceptionHandler(NoHandlerFoundException.class)
    public BaseResponse handleError(NoHandlerFoundException e) {
        logger.error("404 Not Found", e);
        return BaseResponse
                .builder()
                .code(ResultCode.NOT_FOUND)
                .message(e.getMessage())
                .build();
    }

    @ExceptionHandler(HttpMessageNotReadableException.class)
    public BaseResponse handleError(HttpMessageNotReadableException e) {
        logger.error("Message Not Readable", e);
        return BaseResponse
                .builder()
                .code(ResultCode.MSG_NOT_READABLE)
                .message(e.getMessage())
                .build();
    }

    @ExceptionHandler(HttpRequestMethodNotSupportedException.class)
    public BaseResponse handleError(HttpRequestMethodNotSupportedException e) {
        logger.error("Request Method Not Supported", e);
        return BaseResponse
                .builder()
                .code(ResultCode.METHOD_NOT_SUPPORTED)
                .message(e.getMessage())
                .build();
    }

    @ExceptionHandler(HttpMediaTypeNotSupportedException.class)
    public BaseResponse handleError(HttpMediaTypeNotSupportedException e) {
        logger.error("Media Type Not Supported", e);
        return BaseResponse
                .builder()
                .code(ResultCode.MEDIA_TYPE_NOT_SUPPORTED)
                .message(e.getMessage())
                .build();
    }

    @ExceptionHandler(ServiceException.class)
    public BaseResponse handleError(ServiceException e) {
        logger.error("Service Exception", e);
        return BaseResponse
                .builder()
                .code(e.getResultCode())
                .message(e.getMessage())
                .build();
    }

    @ExceptionHandler(PermissionDeniedException.class)
    public BaseResponse handleError(PermissionDeniedException e) {
        logger.error("Permission Denied", e);
        return BaseResponse
                .builder()
                .code(e.getResultCode())
                .message(e.getMessage())
                .build();
    }

    @ExceptionHandler(Throwable.class)
    public BaseResponse handleError(Throwable e) {
        logger.error("Internal Server Error", e);
        return BaseResponse
                .builder()
                .code(ResultCode.INTERNAL_SERVER_ERROR)
                .message(e.getMessage())
                .build();
    }
}
```
<a name="LDNUa"></a>
### DTO和DMO互转
[https://github.com/modelmapper/modelmapper](https://github.com/modelmapper/modelmapper)
```java
private AccountDto convertToDto(Account account) {
    return modelMapper.map(account, AccountDto.class);
}

private Account convertToModel(AccountDto accountDto) {
    return modelMapper.map(accountDto, Account.class);
}
```
<a name="x77pC"></a>
### 设计基于Feign的强类型接口
<a name="LIrLK"></a>
#### Spring Feign
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631016773780-c94b8a21-185d-4beb-a08e-169afbd815a4.jpeg)
<a name="v6T4v"></a>
#### 强类型接口设计
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631017195838-2f415e97-4981-4712-a83d-a7e5f9ec6eb7.jpeg)
<a name="qXHJt"></a>
##### 强类型接口定义
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631017443400-4b54aa50-3c27-4537-961d-3e1978f318b4.png#clientId=u4c8be3c0-6428-4&from=paste&height=181&id=LfmbS&originHeight=543&originWidth=1303&originalType=binary&ratio=1&size=34818&status=done&style=shadow&taskId=ua4ad70b1-cce4-444b-81d0-9e68164d786&width=434.3333333333333)<br />设置基础类Response，做统一处理，然后其他的Response继承基础类Response
```java
@Data
@NoArgsConstructor
@AllArgsConstructor
@Builder
public class BaseResponse {
    private String message;
    @Builder.Default
    private ResultCode code = ResultCode.SUCCESS;

    public boolean isSuccess() {
        return code == ResultCode.SUCCESS;
    }
}
```
```java
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@ToString(callSuper = true)
@EqualsAndHashCode(callSuper = true)
public class GenericAccountResponse extends BaseResponse {
    private AccountDto account;
}
```
```java
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@ToString(callSuper = true)
@EqualsAndHashCode(callSuper = true)
public class ListAccountResponse extends BaseResponse {
    private AccountList accountList;
}
```
<a name="udQSz"></a>
##### 强类型接口的使用
```java
@FeignClient(name = AccountConstant.SERVICE_NAME, path = "/v1/account", url = "${staffjoy.account-service-endpoint}")
// TODO Client side validation can be enabled as needed
// @Validated
public interface AccountClient {

    @PostMapping(path = "/create")
    GenericAccountResponse createAccount(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestBody @Valid CreateAccountRequest request);

    @PostMapping(path = "/track_event")
    BaseResponse trackEvent(@RequestBody @Valid TrackEventRequest request);

    @PostMapping(path = "/sync_user")
    BaseResponse syncUser(@RequestBody @Valid SyncUserRequest request);

    @GetMapping(path = "/list")
    ListAccountResponse listAccounts(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestParam int offset, @RequestParam @Min(0) int limit);

    // GetOrCreate is for internal use by other APIs to match a user based on their phonenumber or email.
    @PostMapping(path= "/get_or_create")
    GenericAccountResponse getOrCreateAccount(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestBody @Valid GetOrCreateRequest request);

    @GetMapping(path = "/get")
    GenericAccountResponse getAccount(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestParam @NotBlank String userId);

    @PutMapping(path = "/update")
    GenericAccountResponse updateAccount(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestBody @Valid AccountDto newAccount);

    @GetMapping(path = "/get_account_by_phonenumber")
    GenericAccountResponse getAccountByPhonenumber(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestParam @PhoneNumber String phoneNumber);

    @PutMapping(path = "/update_password")
    BaseResponse updatePassword(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestBody @Valid UpdatePasswordRequest request);

    @PostMapping(path = "/verify_password")
    GenericAccountResponse verifyPassword(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestBody @Valid VerifyPasswordRequest request);

    // RequestPasswordReset sends an email to a user with a password reset link
    @PostMapping(path = "/request_password_reset")
    BaseResponse requestPasswordReset(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestBody @Valid PasswordResetRequest request);

    @PostMapping(path = "/request_email_change")
    BaseResponse requestEmailChange(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestBody @Valid EmailChangeRequest request);

    // ChangeEmail sets an account to active and updates its email. It is
    // used after a user clicks a confirmation link in their email.
    @PostMapping(path = "/change_email")
    BaseResponse changeEmail(@RequestHeader(AuthConstant.AUTHORIZATION_HEADER) String authz, @RequestBody @Valid EmailConfirmation request);
}
```
<a name="PAXtg"></a>
### 框架层考虑分环境配置
```java
public class EnvConstant {
    public static final String ENV_DEV = "dev";
    public static final String ENV_TEST = "test";
    public static final String ENV_UAT = "uat"; // similar to staging
    public static final String ENV_PROD = "prod";
}
```
```java
@Data
@Builder
public class EnvConfig {

    private String name;
    private boolean debug;
    private String externalApex;
    private String internalApex;
    private String scheme;

    @Getter(AccessLevel.NONE)
    @Setter(AccessLevel.NONE)
    private static Map<String, EnvConfig> map;

    static {
        map = new HashMap<String, EnvConfig>();
        EnvConfig envConfig = EnvConfig.builder().name(EnvConstant.ENV_DEV)
                .debug(true)
                .externalApex("staffjoy-v2.local")
                .internalApex(EnvConstant.ENV_DEV)
                .scheme("http")
                .build();
        map.put(EnvConstant.ENV_DEV, envConfig);

        envConfig = EnvConfig.builder().name(EnvConstant.ENV_TEST)
                .debug(true)
                .externalApex("staffjoy-v2.local")
                .internalApex(EnvConstant.ENV_DEV)
                .scheme("http")
                .build();
        map.put(EnvConstant.ENV_TEST, envConfig);

        // for aliyun k8s demo, enable debug and use http and staffjoy-uat.local
        // in real world, disable debug and use http and staffjoy-uat.xyz in UAT environment
        envConfig = EnvConfig.builder().name(EnvConstant.ENV_UAT)
                .debug(true)
                .externalApex("staffjoy-uat.local")
                .internalApex(EnvConstant.ENV_UAT)
                .scheme("http")
                .build();
        map.put(EnvConstant.ENV_UAT, envConfig);

//        envConfig = EnvConfig.builder().name(EnvConstant.ENV_UAT)
//                .debug(false)
//                .externalApex("staffjoy-uat.xyz")
//                .internalApex(EnvConstant.ENV_UAT)
//                .scheme("https")
//                .build();
//        map.put(EnvConstant.ENV_UAT, envConfig);

        envConfig = EnvConfig.builder().name(EnvConstant.ENV_PROD)
                .debug(false)
                .externalApex("staffjoy.com")
                .internalApex(EnvConstant.ENV_PROD)
                .scheme("https")
                .build();
        map.put(EnvConstant.ENV_PROD, envConfig);
    }

    public static EnvConfig getEnvConfg(String env) {
        EnvConfig envConfig = map.get(env);
        if (envConfig == null) {
            envConfig = map.get(EnvConstant.ENV_DEV);
        }
        return envConfig;
    }
}
```
然后在开发测试环境禁用Sentry异常日志
```java
@Aspect
@Slf4j
public class SentryClientAspect {

    @Autowired
    EnvConfig envConfig;

    @Around("execution(* io.sentry.SentryClient.send*(..))")
    public void around(ProceedingJoinPoint joinPoint) throws Throwable {
        // no sentry logging in debug mode
        if (envConfig.isDebug()) {
            log.debug("no sentry logging in debug mode");
            return;
        }
        joinPoint.proceed();
    }
}
```
<a name="D6Ua1"></a>
### 异步处理时复制上下文信息
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631018586078-e5fff43f-8a2e-4e6d-b7b3-a959c35279c2.jpeg)
<a name="peCUs"></a>
#### AsyncExecutor配置
```java
@Configuration
@EnableAsync
@Import(value = {StaffjoyRestConfig.class})
@SuppressWarnings(value = "Duplicates")
public class AppConfig {

    public static final String ASYNC_EXECUTOR_NAME = "asyncExecutor";

    @Bean(name=ASYNC_EXECUTOR_NAME)
    public Executor asyncExecutor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        // for passing in request scope context
        executor.setTaskDecorator(new ContextCopyingDecorator());
        executor.setCorePoolSize(3);
        executor.setMaxPoolSize(5);
        executor.setQueueCapacity(100);
        executor.setWaitForTasksToCompleteOnShutdown(true);
        executor.setThreadNamePrefix("AsyncThread-");
        executor.initialize();
        return executor;
    }

}
```
<a name="lI0Ha"></a>
#### 在异步的操作上面添加`@Async`标注
```java
@Async(AppConfig.ASYNC_EXECUTOR_NAME)
public void trackEventAsync(String userId, String eventName) {
    if (envConfig.isDebug()) {
        logger.debug("intercom disabled in dev & test environment");
        return;
    }

    Event event = new Event()
        .setUserID(userId)
        .setEventName("v2_" + eventName)
        .setCreatedAt(Instant.now().toEpochMilli());

    try {
        Event.create(event);
    } catch (Exception ex) {
        String errMsg = "fail to create event on Intercom";
        handleException(logger, ex, errMsg);
        throw new ServiceException(errMsg, ex);
    }

    logger.debug("updated intercom");
}
```
<a name="zErjp"></a>
#### 线程上下文拷贝
对于异常操作中线程切换，有时候需要的用户信息就没有了，所以需要处理线程上下文拷贝：
```java
// https://stackoverflow.com/questions/23732089/how-to-enable-request-scope-in-async-task-executor
public class ContextCopyingDecorator implements TaskDecorator {
    @Override
    public Runnable decorate(Runnable runnable) {
        RequestAttributes context = RequestContextHolder.currentRequestAttributes();
        return () -> {
            try {
                RequestContextHolder.setRequestAttributes(context);
                runnable.run();
            } finally {
                RequestContextHolder.resetRequestAttributes();
            }
        };
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631019124533-fe28685f-c936-4410-ba05-2d3ef509ffe0.png#clientId=u4c8be3c0-6428-4&from=paste&height=437&id=u48286534&originHeight=1312&originWidth=2774&originalType=binary&ratio=1&size=400131&status=done&style=shadow&taskId=u4a5a70c3-6bc0-48aa-99ca-22fac3059fb&width=924.6666666666666)<br />这样即便线程切换了，但是上下文信息在其他线程中依然可见。
<a name="u7ntU"></a>
### Swagger接口文档
[https://swagger.io/docs/specification/about/](https://swagger.io/docs/specification/about/)
<a name="vbxji"></a>
#### 引入pom依赖
```xml
<!-- Swagger -->
<dependency>
  <groupId>io.springfox</groupId>
  <artifactId>springfox-swagger2</artifactId>
  <version>2.9.2</version>
</dependency>
<dependency>
  <groupId>io.springfox</groupId>
  <artifactId>springfox-swagger-ui</artifactId>
  <version>2.9.2</version>
</dependency>
```
<a name="RJhD1"></a>
#### 配置Swagger的JavaConfig
```java
@Configuration
@EnableSwagger2
public class SwaggerConfig {
    @Bean
    public Docket api() {
        return new Docket(DocumentationType.SWAGGER_2)
            .select()
            .apis(RequestHandlerSelectors.basePackage("xyz.staffjoy.account.controller"))
            .paths(PathSelectors.any())
            .build()
            .apiInfo(apiEndPointsInfo())
            .useDefaultResponseMessages(false);
    }

    private ApiInfo apiEndPointsInfo() {
        return new ApiInfoBuilder().title("Account REST API")
            .description("Staffjoy Account REST API")
            .contact(new Contact("bobo", "https://github.com/jskillcloud", "bobo@jskillcloud.com"))
            .license("The MIT License")
            .licenseUrl("https://opensource.org/licenses/MIT")
            .version("V2")
            .build();
    }
}
```
<a name="Jfy0W"></a>
#### Swagger JSON Doc
[https://editor.swagger.io](https://editor.swagger.io)
<a name="CGbHJ"></a>
## 主流的服务框架对比
|  | 支持公司 | 编程风格 | 编程模型 | 支持语言 | 亮点 |
| --- | --- | --- | --- | --- | --- |
| Spring（Boot） | Pivotal | REST | 代码优先 | Java | 社区生态好 |
| Dubbo | 阿里 | RPC/REST | 代码优先 | Java | 阿里背书+服务治理 |
| Motan | 新浪 | RPC | 代码优先 | Java为主 | 轻量版Dubbo |
| gRpc | 谷歌 | RPC | 契约优先 | 跨语言 | 谷歌背书+多语言支持+HTTP2支持 |

