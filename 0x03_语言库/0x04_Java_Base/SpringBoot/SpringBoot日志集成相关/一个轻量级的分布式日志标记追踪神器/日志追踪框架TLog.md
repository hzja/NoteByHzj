JavaSpringBootTLog
<a name="M7YyO"></a>
## 背景
随着微服务盛行，很多公司都把系统按照业务边界拆成了很多微服务，在排错查日志的时候，因为业务链路贯穿着很多微服务节点，导致定位某个请求的日志以及上下游业务的日志会变得有些困难。<br />这时候可能有的小伙伴就会想到使用SkyWalking，Pinpoint等分布式追踪系统来解决，并且这些系统通常都是无侵入性的，同时也会提供相对友好的管理界面来进行链路Span的查询，但是搭建分布式追踪系统还是需要一定的成本的，所以本文要说的并不是这些分布式追踪系统，而是一款简单、易用、几乎零侵入、适合中小型公司使用的日志追踪框架TLog。
<a name="A96tg"></a>
## TLog简介
TLog提供了一种最简单的方式来解决日志追踪问题，TLog会自动的对日志进行打标签，自动生成traceId贯穿微服务的一整条链路，在排查日志的时候，可以根据traceId来快速定位请求处理的链路。<br />TLog不收集日志，只在对原来打印的日志上增强，将请求链路信息traceId绑定到打印的日志上。当出现微服务中那么多节点的情况，官方推荐使用TLog+日志收集方案来解决。当然分布式追踪系统其实是链路追踪一个最终的解决方案，如果项目中已经上了分布式追踪系统，那TLog并不适用。<br />如下图，是ELK配合TLog，快速定位请求处理的链路的示例。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344056773-3e2dc6fc-2e84-4f4b-a955-7e30834cb8dd.png#averageHue=%23e9f1e9&clientId=u6f01a0de-7187-4&from=paste&id=ud2e539ab&originHeight=588&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6e737f01-ca07-4e3a-a174-1154757a13d&title=)
<a name="oCqCx"></a>
## TLog接入
<a name="KpFyp"></a>
### 1、接入步骤
<a name="dOwD4"></a>
#### 1.1、引入依赖
```
<dependency>  
    <groupId>com.yomahub</groupId>  
    <artifactId>tlog-all-spring-boot-starter</artifactId>  
    <version>1.5.0</version>  
</dependency>
```
<a name="vLmcT"></a>
#### 1.2、替换logback配置项
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344056765-e54eba19-0359-43a7-bacd-6c7d05864e16.png#averageHue=%23322e2d&clientId=u6f01a0de-7187-4&from=paste&id=u8d264fdc&originHeight=809&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9a33fded-021c-4c06-a5fb-cb9b5ddfb69&title=)<br />到这其实就已经完成了配置。
<a name="dXDFl"></a>
#### 1.3、测试
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344056762-01413f07-f022-4954-85b2-32daf8edbb96.png#averageHue=%232c2b2b&clientId=u6f01a0de-7187-4&from=paste&id=uc8eddfa5&originHeight=382&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u80c67d8e-8245-481e-bfd8-e8b9e4c162a&title=)<br />这里是通过slf4j的`LoggerFactory`获取Logger对象，因为logback适配了slf4j，最终会通过logback来输出日志。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344056854-6f4cf0dd-0d0a-4c51-a209-1b6cd9bfa06b.png#averageHue=%23a93b28&clientId=u6f01a0de-7187-4&from=paste&id=u96ea27ba&originHeight=214&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3b977e68-06f1-4f5a-9d3d-ff22c180e62&title=)<br />从这可以看出，11794076298070144 就是本次日志输出的时候生成的一个请求的traceId，在排查日志的时候就可以通过这个traceId去搜索出整个请求的链路日志。
<a name="x43lT"></a>
### 2、TLog接入方式
TLog总共提供了三种方式接入项目

- Javaagent接入方式
- 字节码注入方式
- 日志框架适配器方式

上面案例的接入方式其实是属于日志框架适配器方式，并且是对于Logback框架的适配。TLog除了适配了Logback框架，还适配了Log4j框架和Log4j2框架，项目中可自行选择。<br />Javaagent接入方式和字节码注入方式相比与日志框架适配器方式对代码的入侵性更小，但是这两种方式仅仅只支持SpringBoot项目，并且相较于日志框架适配器的方式，MDC和异步日志功能并不支持，所以要想完整体验TLog的功能，还是建议选择日志框架适配器方式，日志框架适配器方式其实接入也很快，其实也就是修改一下配置文件的事。<br />![项目环境兼容对比](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344056780-96780a0c-7be4-4228-81d4-758ed738add7.png#averageHue=%23ececec&clientId=u6f01a0de-7187-4&from=paste&id=ue2f46153&originHeight=311&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u95907c8a-832c-4511-8aa3-20e49a013b9&title=%E9%A1%B9%E7%9B%AE%E7%8E%AF%E5%A2%83%E5%85%BC%E5%AE%B9%E5%AF%B9%E6%AF%94 "项目环境兼容对比")<br />![特性支持对比](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344057268-5be2bf81-ca6b-41c4-a4e3-061667b0af54.png#averageHue=%23ececec&clientId=u6f01a0de-7187-4&from=paste&id=ue881e01c&originHeight=399&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ufb35372a-559d-4024-b74a-13d9d6e0656&title=%E7%89%B9%E6%80%A7%E6%94%AF%E6%8C%81%E5%AF%B9%E6%AF%94 "特性支持对比")
<a name="ZhYi4"></a>
## TLog的基本原理
<a name="VT62g"></a>
### 1、日志标签
前面在介绍TLog的时候，提到TLog会自动的对日志进行打标签，这个标签就是日志标签，一个日志标签最多可以包含如下信息：

- preApp：接口调用方服务名
- preHost：接口调用方Host
- preIp：接口调用方ip
- currIp：当前服务ip
- traceId：链路id，调用方如果传递就是传递的值，不传递就会重新生成
- spanId：链路spanId

默认是按照如下`labelPattern`进行数据拼接生成日志标签，所以默认只打出spanId和traceId。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344057355-aababe17-0911-49ef-8271-998f56de7897.png#averageHue=%232d2c2b&clientId=u6f01a0de-7187-4&from=paste&id=ucdb55993&originHeight=387&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3616eeb4-2656-45d4-aa8c-ef7f8f30e04&title=)<br />这也就是上面为什么示例中会输出 <0><11794076298070144> 这种格式的原因，前面的0其实就是spanId。<br />如果想改变日志标签输出其它信息或者输出的顺序，只需要在SpringBoot配置文件中配置日志标签的生成样式就行。
```
tlog.pattern=[$preApp][$preIp][$spanId][$traceId]
```
<a name="aEywy"></a>
### 2、`TLogContext`
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344057314-ab041277-3989-4e88-8bcf-fb61306c43bc.png#averageHue=%232d2c2b&clientId=u6f01a0de-7187-4&from=paste&id=ubba6296f&originHeight=556&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud0710e6d-c13d-426d-a192-7f746f03dbb&title=)<br />`TLogContext`是TLog是一个核心的组件，这个组件内部是使用了`TransmittableThreadLocal`来传递traceId、preApp等信息。<br />当有一个请求过来的时候，会从解析出traceId、preApp等信息，然后设置到`TransmittableThreadLocal`中，之后就可以在整个调用链路中从`TLogContext`中获取到traceId等信息。
<a name="Hev7f"></a>
### 3、`TLogRPCHandler`
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344057303-a488fd81-5c48-4a74-a628-a80c3c201ac0.png#averageHue=%232c2c2c&clientId=u6f01a0de-7187-4&from=paste&id=ua7da94ee&originHeight=1003&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8a9ac0e5-edb1-4fb6-81c0-45f79e614fc&title=)<br />这个组件是用来处理调用方传递的traceId、preApp等信息，设置到TLogContext和MDC中，同时根据日志标签的格式生成日志标签。
<a name="DTWl6"></a>
## 第三方框架的适配
在实际项目中，一个请求处理过程可能会出现以下情况

- 异步线程处理
- 跨服务调用
- MQ调用

那么对于这些情况来说，traceId应该需要在异步线程、跨服务、MQ等中传递，以便更好地排查一个请求的处理链路。<br />而TLog对于以上可能出现的情况都做了大量的适配，保证traceId能够在异步线程、微服务间、MQ等中能够正确传递。
<a name="jwVDn"></a>
### 1、异步线程
<a name="x3WuJ"></a>
#### 1.1 一般异步线程
所谓的一般异步线程就是指直接通过`new Thread`的方法来创建异步线程，然后来执行，这种方式TLog是天然支持携带traceId的，如图。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344057461-bb7cdb54-2331-475a-aaff-bba3167a865c.png#averageHue=%232c2b2b&clientId=u6f01a0de-7187-4&from=paste&id=u1269670b&originHeight=399&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud5f00450-4110-47b5-8546-064e0e94325&title=)<br />执行结果<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344057675-a09529dd-d3a5-4db8-a483-6515ea423461.png#averageHue=%23302e2e&clientId=u6f01a0de-7187-4&from=paste&id=u53904666&originHeight=275&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u574881b5-6ee6-4d7d-aa37-11e6f9d6401&title=)<br />从这可以看出这种异步方式的确成功传递了traceId。
<a name="isMt0"></a>
#### 1.2 线程池
对于线程池来说，其实默认也是支持传递traceId，但是由于线程池中的线程是可以复用了，为了保证线程间的数据互不干扰，需要使用`TLogInheritableTask`将提交的任务进行包装。
```java
ThreadPoolExecutor pool =  
        new ThreadPoolExecutor(1, 2, 1, TimeUnit.SECONDS, new LinkedBlockingQueue<>(10));  
pool.execute(new TLogInheritableTask() {  
    @Override  
    public void runTask() {  
      logger.info("异步执行");  
    }  
});
```
上述代码的写法会有点耦合，每次提交任务都需要创建一个TLogInheritableTask，比较麻烦，可以按如下写法进行简化。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344057857-aef62e56-2aa2-4068-aa3a-dca922684de0.png#averageHue=%232c2b2b&clientId=u6f01a0de-7187-4&from=paste&id=u999fa6ef&originHeight=809&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf2a14cdb-12b7-4c27-b2ae-e8c581ff928&title=)<br />TLogThreadPoolExecutor<br />自己写个`TLogThreadPoolExecutor`继承`ThreadPoolExecutor`，重写`execute`方法(`submit`最终也会调用execute方法执行)，然后将提交的任务统一包装成`TLogInheritableTask`，这样需要使用线程池的地方直接创建`TLogThreadPoolExecutor`就可以了，就不需要在提交任务的时候创建`TLogInheritableTask`了。
```java
ThreadPoolExecutor pool =  
        new TLogThreadPoolExecutor(1, 2, 1, TimeUnit.SECONDS, new LinkedBlockingQueue<>(10));  
pool.execute(() -> logger.info("异步执行"));
```
<a name="wsUAU"></a>
### 2、对RPC框架的支持
除了对异步线程的支持，TLog也支持常见的Dubbo，Dubbox，OpenFeign三大RPC框架，在SpringBoot项目中不需要任何配置，只需要引入依赖就可以实现traceId在服务之间的传递
<a name="CanKd"></a>
#### 2.1 对Dubbo和Dubbox的支持
对于Dubbo和Dubbox的支持是基于Dubbo的Filter扩展点来的<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344057817-4cd1fd94-5108-4c1c-9c3d-eac1a2f77bd1.png#averageHue=%232f2c2b&clientId=u6f01a0de-7187-4&from=paste&id=uc20d8e1f&originHeight=794&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u460f5316-d7a4-48be-a818-e084575f042&title=)<br />TLog通过SPI机制扩展Filter，在消费者发送请求前从TLogContext获取到traceId，然后将traceId和其它调用者数据设置请求数据中，服务提供者在处理请求的时候，也会经过Filter，从请求中获取到traceId等信息，然后设置到TLogContext中，从而实现了traceId在dubbo的消费者和提供者之间的传递。
<a name="QRgOA"></a>
#### 2.2 对OpenFeign的支持
对于OpenFeign的支持其实也是通过Feign提供的扩展点`RequestInterceptor`来实现的<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344057950-eee2cb53-42e1-44fc-acd1-f7250d4d25e7.png#averageHue=%232c2b2b&clientId=u6f01a0de-7187-4&from=paste&id=ue5328cac&originHeight=625&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7778a71e-3412-4cbc-8576-52709cfd91e&title=)<br />发送请求之前，从TLogContext获取到traceId，将traceId等信息添加到请求头中，然后就可以通过Http请求将traceId等信息传递。<br />当被调用方接收到请求之后，会经过`TLogWebInterceptor`这个拦截器进行拦截，从请求头中获取到这些参数，设置到`TLogContext`中。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344059034-11deff63-7984-4de5-9303-1f47164bfef9.png#averageHue=%232e2c2b&clientId=u6f01a0de-7187-4&from=paste&id=uc3500161&originHeight=518&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0c210b48-fd59-416d-97ad-c14de7ebe7b&title=)
<a name="lL7ML"></a>
### 3、对常用Http框架的支持
除了一些RPC框架，TLog也对一些Http框架进行了适配，比如

- HttpClient
- Okhttp
- hutool-http
- RestTemplate
- forest

使用这些Http框架也可以实现traceId的传递<br />其实这些框架的适配跟Feign的适配都是大同小异，都是基于这些Http框架各自提供的扩展点进行适配的，将traceId等信息放到请求头中，这里都不举例了，具体的使用方法可以在官网查看。
<a name="rDIEd"></a>
### 4、对SpringCloud Gateway的支持
同样的，TLog也适配了SpringCloud Gateway<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344058152-9acce879-8523-4346-8527-e5529b2dca28.png#averageHue=%232f2c2b&clientId=u6f01a0de-7187-4&from=paste&id=uadcb24e1&originHeight=446&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueef53182-1cd9-48f3-9568-139f461b3d1&title=)<br />原理也是一样的，就是适配了Gateway的GlobalFilter，从请求头中获取traceId等信息。<br />除了适配了Gateway网关，TLog也适配了Soul网关。
<a name="YqzgL"></a>
### 5、对MQ的支持
对于MQ的支持跟异步线程差不多，需要发送的消息包装成`TLogMqWrapBean`对象<br />![TLogMqWrapBean](https://cdn.nlark.com/yuque/0/2022/png/396745/1671344058323-0599eb62-6830-4392-a797-27f45dd26a54.png#averageHue=%232c2c2b&clientId=u6f01a0de-7187-4&from=paste&id=d9nS8&originHeight=825&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u67a6b05b-2c1e-4004-b412-726cdd3f240&title=TLogMqWrapBean "TLogMqWrapBean")<br />TLogMqWrapBean<br />发送的时候直接发送`TLogMqWrapBean`对象过去
```java
TLogMqWrapBean<BizBean> tLogMqWrap = new TLogMqWrapBean(bizBean);  
mqClient.send(tLogMqWrap);
```
`TLogMqWrapBean`会将traceId等信息携带，消费者接受到`TLogMqWrapBean`，然后通过`TLogMqConsumerProcessor`处理业务消息。
```java
TLogMqConsumerProcessor.process(tLogMqWrapBean, new TLogMqRunner<BizBean>() {  
    @Override  
    public void mqConsume(BizBean o) {  
        //业务操作  
    }  
});
```
如此就实现了traceId通过MQ传递。<br />在实际使用中，根据不同的MQ的类型，可以将消息包装成TLogMqWrapBean对象的过程和处理消息的过程做统一的封装处理，以减少发送消息和处理消息对于TLog的耦合。
<a name="GmG4R"></a>
### 6、对任务框架的支持
TLog主要是支持一下四种任务框架

- JDK Timer
- Quartz框架
- spring-scheduled
- XXL-JOB框架

其中，spring-scheduled和XXL-JOB在SpringBoot环境底下是无需任务配置的，只需要引入依赖即可。<br />Timer在使用的时候需要将任务包装成TLogTimerTask，Quartz需要把QuartzJobBean替换成TLogQuartzJobBean就可以了。
<a name="ORx2k"></a>
### 小总结
其实从上面的各种适配可以看出，其实本质都是一样的，就是根据具体框架的扩展点，在发送请求之前从TLogContext获取到traceId，将traceId等调用者的信息在请求中携带，然后被调用方解析请求，取出traceId和调用者信息，设置到被调用方服务中的TLogContext中。<br />所以，如果一旦需要遇到官方还未适配的框架或者组件，可以参照上述适配过程进行适配即可。
<a name="QpY8S"></a>
## 最后
总的来说，TLog是一款非常优秀的日志追踪的框架，很适合中小公司使用。这里来总结一下TLog的特性

- 通过对日志打标签完成轻量级微服务日志追踪
- 提供三种接入方式：javaagent完全无侵入接入，字节码一行代码接入，基于配置文件的接入
- 对业务代码无侵入式设计，使用简单，10分钟即可接入
- 支持常见的log4j，log4j2，logback三大日志框架，并提供自动检测，完成适配
- 支持dubbo，dubbox，feign三大RPC框架
- 支持Spring Cloud Gateway和Soul网关
- 支持HttpClient和Okhttp等http调用框架标签传递
- 支持多种任务框架，JDK的TimerTask，Quartz，XXL-JOB，spring-scheduled
- 支持日志标签的自定义模板的配置，提供多个系统级埋点标签的选择
- 支持异步线程的追踪，包括线程池，多级异步线程等场景
- 几乎无性能损耗，快速稳定，经过压测，损耗在0.01%

由于本文篇幅有限，无法全面对TLog进行讲解，如果想深入了解该框架，可自行阅读官网或者源码。

- 官网：[https://tlog.yomahub.com](https://tlog.yomahub.com)
- Github地址：[https://github.com/dromara/TLog](https://github.com/dromara/TLog)
