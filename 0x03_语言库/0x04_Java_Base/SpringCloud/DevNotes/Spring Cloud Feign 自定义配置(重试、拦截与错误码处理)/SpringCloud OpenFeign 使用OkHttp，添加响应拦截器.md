JavaSpringCloudOpenFeignOkHttp
<a name="sxWe7"></a>
## 前言
SpringCloud微服务之间的请求一般使用OpenFeign，有时候需要在请求或者响应的时候做一些额外的操作。比如请求的时候添加请求头，响应的时候判断token是否过期等等。这时候拦截器就派上用场了！<br />接下来就说一下怎么添加请求和响应拦截器。
<a name="jQmew"></a>
## 一、修改OpenFeign的http客户端
OpenFeign默认的http客户端是`javax.net.ssl.HttpsURLConnection`，详细信息见feign-core：feign.Client，该http客户端不支持添加拦截器和连接池。所以需要添加第三方http客户端，可选的http客户端有httpClient和okHttp，对应的依赖如下：
```xml
<dependency>
  <groupId>io.github.openfeign</groupId>
  <artifactId>feign-httpclient</artifactId>
  <version>9.7.0</version>
</dependency>
<dependency>
  <groupId>io.github.openfeign</groupId>
  <artifactId>feign-okhttp</artifactId>
  <version>9.7.0</version>
</dependency>
```
OpenFeign默认启用的是HttpClient，但是使用的是OkHttp，故添加以下配置
```
feign.httpclient.enabled=false
feign.okhttp.enabled=true
```
添加以上配置后，OpenFeign的http客户端就自动切换为OkHttp了，详细过程看源码就清楚了，org.springframework.cloud.openfeign.FeignAutoConfiguration，org.springframework.cloud.openfeign.ribbon.OkHttpFeignLoadBalancedConfiguration。在此就不赘述了。
<a name="p4PX4"></a>
## 二、添加请求拦截器
OpenFeign官方自带请求拦截器的接口`feign.RequestInterceptor`，只要实现该接口就可以了。然后就可以做想做的任何事情了
```java
@Component
public class CustomerFeignRequestInterceptor implements RequestInterceptor {
 
    @Override
    public void apply(RequestTemplate template) {
        ServletRequestAttributes attributes = (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
 
        if (attributes != null) {
            HttpServletRequest request = attributes.getRequest();
            template.header("sessionId", request.getHeader("sessionId"));
        }
    }
}
```
<a name="DzRgN"></a>
## 三、添加响应拦截器
刚开始以为OpenFeign官方有响应拦截器的接口，但是翻了半天源码也没找到，所以就想在OkHttp上打主意。<br />网上很多文章里的办法都是重新构造`OkHttpClient`的Bean，<br />但是通过查看源码`org.springframework.cloud.openfeign.FeignAutoConfiguration`发现有这个配置：`@ConditionalOnMissingBean(okhttp3.OkHttpClient.class)`，大概意思就是如果没有OkHttpClient这个Bean，就启动这个配置，如果存在`OkHttpCLient`的Bean，则不启用这个配置。<br />如果自定义`OkHttpClient`的Bean，将导致该配置不能生效，可能就需要添加很多配置，这简直就是费力不讨好嘛，那有没有更好的方法呢？？肯定是有的！！
```java
@Configuration
@ConditionalOnClass(OkHttpClient.class)
@ConditionalOnMissingClass("com.netflix.loadbalancer.ILoadBalancer")
@ConditionalOnMissingBean(okhttp3.OkHttpClient.class)
@ConditionalOnProperty(value = "feign.okhttp.enabled")
protected static class OkHttpFeignConfiguration {

    private okhttp3.OkHttpClient okHttpClient;

    @Bean
    @ConditionalOnMissingBean(ConnectionPool.class)
    public ConnectionPool httpClientConnectionPool(FeignHttpClientProperties httpClientProperties,
                                                   OkHttpClientConnectionPoolFactory connectionPoolFactory) {
        Integer maxTotalConnections = httpClientProperties.getMaxConnections();
        Long timeToLive = httpClientProperties.getTimeToLive();
        TimeUnit ttlUnit = httpClientProperties.getTimeToLiveUnit();
        return connectionPoolFactory.create(maxTotalConnections, timeToLive, ttlUnit);
    }

    @Bean
    public okhttp3.OkHttpClient client(OkHttpClientFactory httpClientFactory,
                                       ConnectionPool connectionPool, FeignHttpClientProperties httpClientProperties) {
        Boolean followRedirects = httpClientProperties.isFollowRedirects();
        Integer connectTimeout = httpClientProperties.getConnectionTimeout();
        Boolean disableSslValidation = httpClientProperties.isDisableSslValidation();
        this.okHttpClient = httpClientFactory.createBuilder(disableSslValidation).
            connectTimeout(connectTimeout, TimeUnit.MILLISECONDS).
            followRedirects(followRedirects).
            connectionPool(connectionPool).build();
        return this.okHttpClient;
    }

    @PreDestroy
    public void destroy() {
        if(okHttpClient != null) {
            okHttpClient.dispatcher().executorService().shutdown();
            okHttpClient.connectionPool().evictAll();
        }
    }

    @Bean
    @ConditionalOnMissingBean(Client.class)
    public Client feignClient(okhttp3.OkHttpClient client) {
        return new OkHttpClient(client);
    }
}
```
仔细观察`OkHttpFeignConfiguration`这个内部类里是怎么配置`OkHttpClient`的，发现是通过`Builer`建造者模式创建的对象，`OkHttpClient.Builder`又是通过`OkHttpClientFactory`接口来生成的，`OkHttpClientFactory`接口的默认实现`DefaultOkHttpClientFactory`的构造参数又是`OkHttpClient.Builder`。<br />还有很重要的一点，`OkHttpClientFactory`在`FeignAutoConfiguration`整个类中并没有实例化，只有在方法参数中引用了，这说明`OkHttpClientFactory`在其他地方已经实例化了，已经是spring管理的Bean了。接下来要找到`OkHttpClientFactory`是在哪里实例化的。在Idea中打开`OkHttpClientFactory`这个接口的代码，按住Ctrl加鼠标左键，出现以下内容<br />![2022-10-19-21-05-00.353777200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1666184861206-30ec2bf9-eda3-4fa4-88c2-919e836b7532.png#clientId=u1daaff91-8c0d-4&from=ui&id=u25c43bfe&originHeight=264&originWidth=1069&originalType=binary&ratio=1&rotation=0&showTitle=false&size=848357&status=done&style=none&taskId=u29129e66-2dc1-4435-a47a-ebaf532f74e&title=)<br />一共有7个地方引用了`OkHttpClientFactory`，但是只有`HttpClientConfiguration`是没有接触过的，其他的上面都有涉及。所以`OkHttpClientFactory`很有可能就是在这个类里实例化的，打开这个类，果然如此！！<br />![2022-10-19-21-05-00.368081800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1666184861479-5d015202-00b0-4732-ad9a-6ed525a172f9.png#clientId=u1daaff91-8c0d-4&from=ui&id=j4ATg&originHeight=626&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2032219&status=done&style=none&taskId=u91e26610-80b5-4d1b-93cb-1265f5ce181&title=)<br />前面说到，`OkHttpClient`的实例化和`OkHttpClient.Builder`、`OkHttpClientFactory`有关，`OkHttpClientFactory`的实例化又和`OkHttpClient.Builder`有关。<br />所以，只要自定义`OkHttpClient.Builder`就能完美解决这个问题！！自定义`OkHttpClient.Builder`的时候添加响应拦截器就可以了。切记重新构造`Response`返回，如果直接返回`Response`，会导致Feign直接走降级。
```java
@Slf4j
@Configuration
public class FeignOkHttpClientConfig {
 
    @Bean
    public OkHttpClient.Builder okHttpClientBuilder() {
        return new OkHttpClient.Builder().addInterceptor(new FeignOkHttpClientResponseInterceptor());
    }
 
    /**
     * okHttp响应拦截器
     */
    public static class FeignOkHttpClientResponseInterceptor implements Interceptor{
 
 
        @Override
        public Response intercept(Chain chain) throws IOException {
 
            Request originalRequest = chain.request();
            Response response = chain.proceed(originalRequest);
          
            MediaType mediaType = response.body().contentType();
            String content = response.body().string();
            
            //解析content，做你想做的事情！！
 
            //生成新的response返回，网络请求的response如果取出之后，直接返回将会抛出异常
            return response.newBuilder()
                    .body(ResponseBody.create(mediaType, content))
                    .build();
        }
    }
 
}
```
<a name="XITMc"></a>
## 最后
刚开始也是直接复制网上的方法，发现不太好用，然后花了一点时间看了下源码，找到了解决方法。所以有时候直接百度也不太好用，还是得靠自己。
