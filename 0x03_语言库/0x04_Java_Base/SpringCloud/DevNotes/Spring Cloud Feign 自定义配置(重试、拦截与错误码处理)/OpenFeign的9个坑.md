Java OpenFeign<br />OpenFeign是SpringCloud中的重要组件，它是一种声明式的HTTP客户端。使用OpenFeign调用远程服务就像调用本地方法一样，但是如果使用不当，很容易踩到坑。
<a name="KWi00"></a>
## 坑一：用对Http Client
<a name="Z18vF"></a>
### 1.1 feign中http client
如果不做特殊配置，OpenFeign默认使用jdk自带的`HttpURLConnection`，都知道`HttpURLConnection`没有连接池、性能和效率比较低，如果采用默认，很可能会遇到性能问题导致系统故障。<br />可以采用Apache HttpClient，properties文件中增加下面配置：
```
feign.httpclient.enabled=true
```
pom文件中增加依赖：
```xml
<dependency>
    <groupId>io.github.openfeign</groupId>
    <artifactId>feign-httpclient</artifactId>
    <version>9.3.1</version>
</dependency>
```
也可以采用OkHttpClient，properties文件中增加下面配置：
```
feign.okhttp.enabled=true
```
pom文件中增加依赖：
```xml
<dependency>
    <groupId>io.github.openfeign</groupId>
    <artifactId>feign-okhttp</artifactId>
    <version>10.2.0</version>
</dependency>
```
<a name="wiXmz"></a>
### 1.2 ribbon中的Http Client
通过OpenFeign作为注册中心的客户端时，默认使用Ribbon做负载均衡，Ribbon默认也是用jdk自带的`HttpURLConnection`，需要给Ribbon也设置一个Http client，比如使用okhttp，在properties文件中增加下面配置：
```
ribbon.okhttp.enabled=true
```
<a name="Lxar6"></a>
## 坑二：全局超时时间
OpenFeign可以设置超时时间，简单粗暴，设置一个全局的超时时间，如下：
```
feign.client.config.default.connectTimeout=2000
feign.client.config.default.readTimeout=60000
```
如果不配置超时时间，默认是连接超时10s，读超时60s，在源码`feign.Request`的内部类`Options`中定义。<br />这个接口设置了最大的`readTimeout`是60s，这个时间必须大于调用的所有外部接口的readTimeout，否则处理时间大于`readTimeout`的接口就会调用失败。<br />如下图，在一个系统中使用OpenFeign调用外部三个服务，每个服务提供两个接口，其中serviceC的一个接口需要60才能返回，那上面的`readTimeout`必须设置成60s。<br />![2021-09-28-12-28-10-603004.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632803328964-8231d826-1f0a-450f-8b40-b1136580587d.png#clientId=ue348ce06-771d-4&from=ui&id=ubad66286&originHeight=284&originWidth=473&originalType=binary&ratio=1&size=29032&status=done&style=shadow&taskId=u8582feac-0c3d-48d6-9aef-6ebe666fb1a)<br />但是如果serviceA出故障了，表现是接口1超过60s才能返回，这样OpenFeign只能等到读超时，如果调用这个接口的并发量很高，会大量占用连接资源直到资源耗尽系统奔溃。要防止这样的故障发生，就必须保证接口1能fail-fast。最好的做法就是给serviceC单独设置超时时间。
<a name="OEjUM"></a>
## 坑三：单服务设置超时时间
从上一节的讲解可以看到，需要对serviceC单独设置一个超时时间，代码如下：
```
feign.client.config.serviceC.connectTimeout=2000
feign.client.config.serviceC.readTimeout=60000
```
这个时间会覆盖第一节中默认的超时时间。但是问题又来了，serviceC中又掉了serviceD，因为serviceD的故障导致接口6发生了读超时的情况，为了不让系统奔溃，不得不对serviceC的接口5单独设置超时时间。如下图：<br />![2021-09-28-12-28-10-766006.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632803328889-3db35ac8-0966-4578-9d56-334128c15b3e.png#clientId=ue348ce06-771d-4&from=ui&id=rL9Fu&originHeight=324&originWidth=572&originalType=binary&ratio=1&size=37255&status=done&style=shadow&taskId=u10843bb8-59bf-4b42-a08f-b4041fe643b)
<a name="AOmGR"></a>
## 坑四：熔断超时时间
怎样给单个接口设置超时时间，查看网上资料，必须开启熔断，配置如下：
```
feign.hystrix.enabled=true
```
开启熔断后，就可以给单个接口配置超时了。如果调用serviceC的接口5的声明如下：
```java
@FeignClient(value = "serviceC"configuration = FeignMultipartSupportConfig.class)
public interface ServiceCClient {
    @GetMapping("/interface5")
    String interface5(String param);
}
```
根据上面interface5接口的声明，在properties文件中增加如下配置：
```
hystrix.command.ServiceCClient#interface5(param).execution.isolation.thread.timeoutInMilliseconds=60000
```
网上资料说的并不准确，这个超时时间并没有起作用。为什么不生效呢？
<a name="cJIX6"></a>
### 4.1 使用feign超时
最终使用的超时时间来自于Options类。如果配置了feign的超时时间，会选择使用feign超时时间，下面代码在`FeignClientFactoryBean`类的`configureUsingProperties`方法：
```java
if (config.getConnectTimeout() != null && config.getReadTimeout() != null) {
    builder.options(new Request.Options(config.getConnectTimeout(), config.getReadTimeout()));
}
```
<a name="HbN6s"></a>
### 4.2 使用ribbon超时
如果没有配置feign，但是配置了ribbon的超时时间，会使用ribbon的超时时间。看下这段源代码，`FeignLoadBalancer`里面的`execute`方法，
```java
public RibbonResponse execute(RibbonRequest request, IClientConfig configOverride)
    throws IOException {
    Request.Options options;
    if (configOverride != null) {
        RibbonProperties override = RibbonProperties.from(configOverride);
        options = new Request.Options(
            override.connectTimeout(this.connectTimeout),
            override.readTimeout(this.readTimeout));
    }
    else {
        options = new Request.Options(this.connectTimeout, this.readTimeout);
    }
    //这个request里面的client就是OkHttpClient
    Response response = request.client().execute(request.toRequest(), options);
    return new RibbonResponse(request.getUri(), response);
}
```
<a name="wqYaF"></a>
### 4.3 使用自定义Options
对于单个接口怎么配置超时时间，这里给出一个方案。使用`RestTemplate`来调这个接口，单独配置超时时间，配置代码如下，这里使用OkHttpClient：
```java
public class RestTemplateConfiguration {

    @Bean
    public OkHttp3ClientHttpRequestFactory okHttp3RequestFactory(){
        OkHttp3ClientHttpRequestFactory requestFactory = new OkHttp3ClientHttpRequestFactory();
        requestFactory.setConnectTimeout(2000);
        requestFactory.setReadTimeout(60000);
        return requestFactory;
    }

    @Bean
    @LoadBalanced
    public RestTemplate restTemplate(OkHttp3ClientHttpRequestFactory okHttp3RequestFactory){
        return new RestTemplate(okHttp3RequestFactory);
    }
}
```
为了使用ribbon负载均衡，上面加了`@LoadBalanced`<br />如果使用`RestTemplate`，就会使用`OkHttp3ClientHttpRequestFactory`中配置的时间。
<a name="VKINi"></a>
## 坑五：ribbon超时时间
作为负载均衡，ribbon超时时间也是可以配置的，可以在properties增加下面配置：
```
ribbon.ConnectTimeout=2000
ribbon.ReadTimeout=11000
```
有文章讲ribbon配置的超时时间必须要满足接口响应时间，其实不然，配置feign的超时时间就足够了，因为它可以覆盖掉ribbon的超时时间。
<a name="mvWqS"></a>
## 坑六：重试默认不开启
OpenFeign默认是不支持重试的，可以在源代码`FeignClientsConfiguration`中`feignRetryer`中看出。
```java
@Bean
@ConditionalOnMissingBean
public Retryer feignRetryer() {
    return Retryer.NEVER_RETRY;
}
```
要开启重试，可以自定义`Retryer`，比如下面这行代码：
```java
Retryer retryer = new Retryer.Default(100, 1000, 2);
```
表示每间隔100ms，最大间隔1000ms重试一次，最大重试次数是1，因为第三个参数包含了第一次请求。
<a name="GKAEF"></a>
## 坑七：Ribbon重试
<a name="A4Uck"></a>
### 7.1 拉取服务列表
Ribbon默认从服务端拉取列表的时间间隔是30s，这个对优雅发布很不友好，一般会把这个时间改短，如下改成3s：
```
serviceC.ribbon.ServerListRefreshInterval=3
```
<a name="PJu2E"></a>
### 7.2 重试
Ribbon重试有不少需要注意的地方，这里分享4个。

1. 同一实例最大重试次数，不包括首次调用，配置如下：
```
serviceC.ribbon.MaxAutoRetries=1
```
这个次数不包括首次调用，配置了1，重试策略会先尝试在失败的实例上重试一次，如果失败，请求下一个实例。

2. 同一个服务其他实例的最大重试次数，这里不包括第一次调用的实例。默认值为1：
```
serviceC.ribbon.MaxAutoRetriesNextServer=1
```

3. 是否对所有操作都重试，如果改为true，则对所有操作请求都进行重试，包括post，建议采用默认配置false。
```
serviceC.ribbon.OkToRetryOnAllOperations=false
```

4. 对指定的http状态码进行重试
```
serviceC.retryableStatusCodes=404,408,502,500
```
<a name="a2azD"></a>
## 坑八：hystrix超时
如下图：<br />![2021-09-28-12-28-10-936003.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632803328900-c2f16127-9efa-440b-a2d1-5ffc6a3b7502.png#clientId=ue348ce06-771d-4&from=ui&id=u0XdS&originHeight=167&originWidth=608&originalType=binary&ratio=1&size=14035&status=done&style=shadow&taskId=u4a3e5821-1e12-451f-9d4d-b10b6baa0de)<br />hystrix默认不开启，但是如果开启了hystrix，因为hystrix是在Ribbon外面，所以超时时间需要符合下面规则：hystrix超时 >= (MaxAutoRetries + 1) * (ribbon ConnectTimeout + ribbon ReadTimeout)<br />如果Ribbon不重试，`MaxAutoRetries=0`<br />根据上面公式，假如配置熔断超时时间如下：
```java
hystrix.command.ServiceCClient#interface5(param).execution.isolation.thread.timeoutInMilliseconds=15000
ribbon.ReadTimeout=8000
```
这个配置是不会重试一次的。serviceA调用serviceB时，hystrix会等待Ribbon返回的结果，如果Ribbon配置了重试，hystrix会一直等待直到超时。上面的配置，因为第一次请求已经耗去了8s，剩下时间7s不够请求一次了，所以是不会进行重试的。
<a name="qWWmQ"></a>
## 坑九：使用OpenFeign做http客户端
即使不用注册中心，使用OpenFeign做普通http客户端也是很方便的，但是有三点需要注意：

- 不用配置ribbon相关参数
- 使用`RestTemplate`调用时，不考虑负载均衡
- 使用过程中OpenFeign要组装出自己的一套请求，跟直接使用http客户端比，会有一定开销

使用OpenFeign有很多配置上的坑，对于没有注册中心的情况，建议直接使用http客户端。
