Java
<a name="ZjcXn"></a>
## 一、摘要
在实际的系统开发过程中，通常会基于项目环境，封装一个公共的网络请求工具库，以便开发接口请求相关的需求时，能更加方便的使用。<br />以服务端编程为例，以下是封装工具类的实践过程。
<a name="SJnlN"></a>
## 二、案例实践
<a name="OE65v"></a>
#### 2.1、首先在项目中导入相关的依赖包
```xml
<!-- okhttp3 -->
<dependency>
  <groupId>com.squareup.okhttp3</groupId>
  <artifactId>okhttp</artifactId>
  <version>3.14.9</version>
</dependency>
```
```xml
<!--jackson依赖包-->
<dependency>
  <groupId>com.fasterxml.jackson.core</groupId>
  <artifactId>jackson-core</artifactId>
  <version>2.9.8</version>
</dependency>
<dependency>
  <groupId>com.fasterxml.jackson.core</groupId>
  <artifactId>jackson-annotations</artifactId>
  <version>2.9.8</version>
</dependency>
<dependency>
  <groupId>com.fasterxml.jackson.core</groupId>
  <artifactId>jackson-databind</artifactId>
  <version>2.9.8</version>
</dependency>
<dependency>
  <groupId>commons-collections</groupId>
  <artifactId>commons-collections</artifactId>
  <version>3.2.2</version>
  <scope>compile</scope>
</dependency>
```
<a name="vsyIN"></a>
#### 2.2、封装 OkHttpClient 相关请求方法
首先将 OkHttpClient 客户端配置参数单独拎出来，方便后续独立配置。
```java
public class OkHttpClientFactory {

    /**
     * 默认允许的最大并发数
     */
    private static int concurrentSize = 100;

    private OkHttpClientFactory(){}

    /**
     * 实例化客户端配置
     * @return
     */
    public static OkHttpClient.Builder newInstance() {
        OkHttpClient.Builder httpClientBuilder = new OkHttpClient.Builder();

        // 设置超时时间
        httpClientBuilder.connectTimeout(5, TimeUnit.SECONDS);
        httpClientBuilder.writeTimeout(5, TimeUnit.SECONDS);
        httpClientBuilder.readTimeout(5, TimeUnit.SECONDS);

        // 设置连接池，连接池的最大数量=并发数，30秒空闲的连接会释放掉
        httpClientBuilder.connectionPool(new ConnectionPool(concurrentSize, 30, TimeUnit.SECONDS));

        // 设置分发器处理的最大容量，这里的数量=连接池数量=并发数，表示此客户端有同时能够处理xx并发数的能力
        Dispatcher dispatcher = new Dispatcher();
        dispatcher.setMaxRequests(concurrentSize);
        dispatcher.setMaxRequestsPerHost(concurrentSize);
        httpClientBuilder.dispatcher(dispatcher);

        return httpClientBuilder;
    }

}
```
然后，构建一个 OkHttpClient 客户端单例对象。
```java
public class OkHttpClientBuilder {

    private static final Logger LOGGER = LoggerFactory.getLogger(OkHttpClientBuilder.class);

    /**
     * 同步阻塞等待执行结果的时间，单位毫秒
     */
    private static Integer DEFAULT_WAIT_TIMEOUT = 3000;

    /**
     * 服务启动时初始化OkHttpClient对象，确保客户端对象单例
     */
    private static OkHttpClient okHttpClient = OkHttpClientFactory.newInstance().build();

    private OkHttpClientBuilder() {}


    static {
        // 向JVM注册一个关闭钩子，当服务准备停止时，等待 OkHttpClient 中任务执行完毕再停止，防止线程池中正在执行的任务突然中断
        Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
            @Override
            public void run() {
                final Logger logger = LoggerFactory.getLogger(OkHttpClient.class);
                int count = 100;
                Dispatcher dispatcher = okHttpClient.dispatcher();
                logger.info("ShutdownHook start：queuedCallsCount {} , runningCallsCount {}", dispatcher.queuedCallsCount(), dispatcher.runningCallsCount());
                while (dispatcher.queuedCallsCount() > 0 || dispatcher.runningCallsCount() > 0) {
                    try {
                        TimeUnit.MILLISECONDS.sleep(100);
                    } catch (InterruptedException e) {
                        logger.error("ShutdownHook interrupted：queuedCallsCount {} , runningCallsCount {}", dispatcher.queuedCallsCount(), dispatcher.runningCallsCount());
                        break;
                    }
                    // 防止无限循环
                    if (--count == 0) {
                        LOGGER.error("ShutdownHook timeout：queuedCallsCount {} , runningCallsCount {}", dispatcher.queuedCallsCount(), dispatcher.runningCallsCount());
                        break;
                    }
                }
                logger.info("ShutdownHook end：queuedCallsCount {} , runningCallsCount {}", dispatcher.queuedCallsCount(), dispatcher.runningCallsCount());
            }
        }));
    }


    /**
     * 同步执行请求，公共方法
     * @param request
     * @return
     */
    public static OkHttpClientResult syncRequest(Request request){
        try (Response response = okHttpClient.newCall(request).execute()){
            return buildResponseResult(request, response);
        } catch (Exception e) {
            LOGGER.error("request error，request:" +  request, e);
            return new OkHttpClientResult(false, 500, "request error");
        }
    }



    /**
     * 异步执行请求，同步阻塞编程等待返回结果
     * 此方式在多线程环境下请求处理依然能保持高性能，根据不同的场景显式对ConnectionPool进行调优处理
     * @param request
     * @return
     */
    public static OkHttpClientResult syncResponse(Request request){
        if(LOGGER.isDebugEnabled()){
            LOGGER.debug("request start，request:{}", request);
        }
        // 使用异步编程，在指定时间内阻塞获取 OKHttp 异步执行结果
        CompletableFuture<Response> completableFuture = new CompletableFuture();
        // 发起异步请求调用
        Call call = okHttpClient.newCall(request);
        call.enqueue(new Callback(){

            @Override
            public void onFailure(Call call, IOException e) {
                completableFuture.completeExceptionally(e);
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                completableFuture.complete(response);
            }
        });
        // 这里的异步执行结果等待时间，取决于get同步获取时间的设定，而不是全局请求超时配置
        try (Response response = completableFuture.get(DEFAULT_WAIT_TIMEOUT, TimeUnit.MILLISECONDS);) {
            return buildResponseResult(request, response);
        } catch (TimeoutException e){
            call.cancel();
            LOGGER.error("request timeout，request:" +  request, e);
            return new OkHttpClientResult(false, 500, "request timeout");
        } catch (Exception e){
            LOGGER.error("request error，request:" +  request, e);
            return new OkHttpClientResult(false, 500, "request error");
        }
    }

    /**
     * 封装返回值
     * @param request
     * @param response
     * @return
     * @throws IOException
     */
    private static OkHttpClientResult buildResponseResult(Request request, Response response) throws IOException {
        if(LOGGER.isDebugEnabled()){
            LOGGER.debug("request end，request:{}, response:{}", request, response);
        }
        ResponseBody responseBody = response.body();
        OkHttpClientResult result = new OkHttpClientResult();
        result.setSuccess(response.isSuccessful());
        result.setCode(response.code());
        result.setMessage(response.message());
        result.setHeaders(response.headers().toMultimap());
        if(Objects.nonNull(responseBody)){
            result.setBody(responseBody.bytes());
        }
        if(!result.isSuccess()){
            LOGGER.warn("request fail, request:{}, response:{}", request, response);
        }
        return result;
    }
}
```
相关的返回实体类对象
```java
public class OkHttpClientResult {

    /**
     * 是否成功
     */
    private boolean success = false;

    /**
     * http级，状态标识码
     */
    private Integer code;

    /**
     * http级，错误信息
     */
    private String message;

    /**
     * http级，返回头部
     */
    private Map<String, List<String>> headers;

    /**
     * http级，返回body
     */
    private byte[] body;

    public boolean isSuccess() {
        return success;
    }

    public void setSuccess(boolean success) {
        this.success = success;
    }

    public Integer getCode() {
        return code;
    }

    public void setCode(Integer code) {
        this.code = code;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public Map<String, List<String>> getHeaders() {
        return headers;
    }

    public void setHeaders(Map<String, List<String>> headers) {
        this.headers = headers;
    }

    public byte[] getBody() {
        return body;
    }

    public void setBody(byte[] body) {
        this.body = body;
    }

    public OkHttpClientResult() {
    }

    public OkHttpClientResult(boolean success, int code, String message) {
        this.success = success;
        this.code = code;
        this.message = message;
    }
}
```
最后，基于以上的方法，封装一个独立的网络请求工具类
```java
public class OkHttpClientUtils {

    /**
     * get请求
     * @param url
     * @param headers
     * @return
     */
    public static String get(String url, Map<String, String> headers){
        Request request = new Request.Builder()
                .url(url)
                .headers(buildHeaders(headers))
                .get()
                .build();
        OkHttpClientResult result = commonRequest(request);
        return buildResponse(result);
    }

    /**
     * get请求（支持范型对象返回参数）
     * @param url
     * @param headers
     * @param responseType
     * @param <T>
     * @return
     */
    public static <T> T get(String url, Map<String, String> headers, TypeReference<T> responseType){
        Request request = new Request.Builder()
                .url(url)
                .headers(buildHeaders(headers))
                .get()
                .build();
        OkHttpClientResult result = commonRequest(request);
        return buildResponse(result, responseType);
    }

    /**
     * post表单请求
     * @param url
     * @param paramMap
     * @param headers
     * @return
     */
    public static String postByForm(String url, Map<String, String> paramMap, Map<String, String> headers){
        Request request = new Request.Builder()
                .url(url)
                .headers(buildHeaders(headers))
                .post(buildFormBody(paramMap))
                .build();
        OkHttpClientResult result = commonRequest(request);
        return buildResponse(result);
    }

    /**
     * post表单请求（支持范型对象返回参数）
     * @param url
     * @param paramMap
     * @param headers
     * @param responseType
     * @param <T>
     * @return
     */
    public static <T> T postByForm(String url, Map<String, String> paramMap, Map<String, String> headers, TypeReference<T> responseType){
        Request request = new Request.Builder()
                .url(url)
                .headers(buildHeaders(headers))
                .post(buildFormBody(paramMap))
                .build();
        OkHttpClientResult result = commonRequest(request);
        return buildResponse(result, responseType);
    }


    /**
     * post + json请求
     * @param url
     * @param value
     * @param headers
     * @return
     */
    public static String postByJson(String url, Object value, Map<String, String> headers){
        Request request = new Request.Builder()
                .url(url)
                .headers(buildHeaders(headers))
                .post(buildJsonBody(value))
                .build();
        OkHttpClientResult result = commonRequest(request);
        return buildResponse(result);
    }


    /**
     * post + json请求（支持范型对象返回参数）
     * @param url
     * @param value
     * @param headers
     * @param responseType
     * @param <T>
     * @return
     */
    public static <T> T postByJson(String url, Object value, Map<String, String> headers, TypeReference<T> responseType){
        Request request = new Request.Builder()
                .url(url)
                .headers(buildHeaders(headers))
                .post(buildJsonBody(value))
                .build();
        OkHttpClientResult result = commonRequest(request);
        return buildResponse(result, responseType);
    }


    /**
     * 包装请求头部
     * @param headers
     * @return
     */
    private static Headers buildHeaders(Map<String, String> headers){
        Headers.Builder headerBuilder = new Headers.Builder();
        if(MapUtils.isNotEmpty(headers)){
            headers.entrySet().forEach(entry -> headerBuilder.add(entry.getKey(), entry.getValue()));
        }
        return headerBuilder.build();
    }


    /**
     * 包装请求表单
     * @param paramMap
     * @return
     */
    private static FormBody buildFormBody(Map<String, String> paramMap){
        FormBody.Builder formBodyBuilder = new FormBody.Builder();
        if(MapUtils.isNotEmpty(paramMap)){
            paramMap.entrySet().forEach(entry -> formBodyBuilder.add(entry.getKey(), entry.getValue()));
        }
        return formBodyBuilder.build();
    }

    /**
     * 包装请求json数据
     * @param request
     * @return
     */
    private static RequestBody buildJsonBody(Object request){
        MediaType contentType = MediaType.parse("application/json; charset=utf-8");
        RequestBody requestBody = RequestBody.create(contentType, JacksonObjectUtil.objToJson(request));
        return requestBody;
    }

    /**
     * 包装返回结果，针对字符串
     * @param result
     * @return
     */
    private static String buildResponse(OkHttpClientResult result){
        if(!result.isSuccess() && StringUtils.isNotBlank(result.getMessage())){
            throw new RuntimeException(result.getMessage());
        }
        return byteToString(result.getBody());
    }

    /**
     * 包装返回结果，针对返回范型对象
     * @param result
     * @return
     */
    private static <T> T buildResponse(OkHttpClientResult result, TypeReference<T> responseType){
        if(!result.isSuccess() && StringUtils.isNotBlank(result.getMessage())){
            throw new RuntimeException(result.getMessage());
        }
        return JacksonObjectUtil.byteToObj(result.getBody(), responseType);
    }

    /**
     * 获取内容
     * @param bytes
     * @return
     */
    private static String byteToString(byte[] bytes){
        if(Objects.nonNull(bytes)){
            return new String(bytes, Charset.forName("utf-8"));
        }
        return StringUtils.EMPTY;
    }

    /**
     * 公共请求调用
     * @param request
     * @return
     */
    private static OkHttpClientResult commonRequest(Request request){
        return OkHttpClientBuilder.syncResponse(request);
    }
}
```
以上就是基于 OkHttp 封装的整套工具类，可能有的网友对`OkHttpClientBuilder.syncResponse`编程方法不太理解，会在下面进行详细的讲解。
<a name="NJDKF"></a>
#### 2.3、封装对象序列化工具
在上文的`OkHttpClientUtils`工具类中，提到了范型对象的返回。<br />其中用到了一个很重要的类库工具，那就是jackson类库，所以在上文第一段就介绍了导入jackson相关的库包。<br />基于jackson库封装的工具类JacksonObjectUtil，代码如下：
```java
public class JacksonObjectUtil {

    private static final Logger LOGGER = LoggerFactory.getLogger(JacksonObjectUtil.class);


    private static ObjectMapper objectMapper = new ObjectMapper();


    static {
        // 忽略未知的字段
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
        // 读取不认识的枚举时，当null值处理
        objectMapper.configure(DeserializationFeature.READ_UNKNOWN_ENUM_VALUES_AS_NULL, true);
        // 全部输出
        objectMapper.configure(JsonParser.Feature.AUTO_CLOSE_SOURCE, true);
    }


    /**
     * 将对象序列化成字节流
     * @param value
     * @return
     */
    public static byte[] objToByte(Object value){
        try {
            if(Objects.nonNull(value)){
                return objectMapper.writeValueAsBytes(value);
            }
        } catch (Exception e){
            LOGGER.warn("将对象序列化成字节失败",e);
        }
        return null;
    }

    /**
     * 将对象序列化成json字符串
     * @param value
     * @return
     */
    public static String objToJson(Object value){
        try {
            if(Objects.nonNull(value)){
                return objectMapper.writeValueAsString(value);
            }
        } catch (Exception e){
            LOGGER.warn("将对象序列化成json字符串失败",e);
        }
        return "";
    }

    /**
     * 将json字符串转对象，支持指定类
     * @param value
     * @return
     */
    public static <T> T jsonToObj(String value, Class<T> classType){
        try {
            if(StringUtils.isNotBlank(value)){
                return objectMapper.readValue(value, classType);
            }
        } catch (Exception e){
            LOGGER.warn("将json字符串转对象失败，字符串：" + value, e);
        }
        return null;
    }


    /**
     * 将json字符串转对象，支持范型类
     * @param value
     * @return
     */
    public static <T> T jsonToObj(String value, TypeReference<T> referenceType){
        try {
            if(StringUtils.isNotBlank(value)){
                return objectMapper.readValue(value, referenceType);
            }
        } catch (Exception e){
            LOGGER.warn("将json字符串转对象失败，字符串：" + value, e);
        }
        return null;
    }

    /**
     * 将字节流转对象，支持范型类
     * @param value
     * @return
     */
    public static <T> T byteToObj(byte[] value, TypeReference<T> referenceType){
        try {
            if(Objects.nonNull(value)){
                return objectMapper.readValue(value, referenceType);
            }
        } catch (Exception e){
            LOGGER.warn("将字节流转对象失败", e);
        }
        return null;
    }
}
```
<a name="kjRuA"></a>
## 三、性能测试
OkHttp 支持同步、异步两种模式的网络请求编程方法。<br />其实每种请求方式，带来的性能体验也大为不同。<br />以 200 个线程同时发起网络请求，同步请求／异步请求耗时如下：
```
同步请求平均耗时：1997 ms
异步请求平均耗时：1798 ms
```
从数据显示结果，两者差异并不大。<br />下面以循环发起 200 请求为例，同步请求／异步请求耗时如下：
```
同步请求平均耗时：6478 ms
异步请求平均耗时：1794 ms
```
两者所消耗的时间差异 3 倍，随着请求次数越多，差异越大。<br />**总结下来，采用异步请求所消耗的时间比同步请求低很多**。<br />基于以上结论，回到上文中介绍的`OkHttpClientBuilder`类，提供了两个方法。<br />一个是同步请求的编程方案
```java
/**
 * 同步执行请求，公共方法
 * @param request
 * @return
 */
public static OkHttpClientResult syncRequest(Request request){
    try (Response response = okHttpClient.newCall(request).execute()){
        return buildResponseResult(request, response);
    } catch (Exception e) {
        LOGGER.error("request error，request:" +  request, e);
        return new OkHttpClientResult(false, 500, "request error");
    }
}
```
另一个是异步请求，同步阻塞等待返回结果的编程方案。
```java
/**
 * 异步执行请求，同步阻塞编程等待返回结果
 * 此方式在多线程环境下请求处理依然能保持高性能，根据不同的场景显式对ConnectionPool进行调优处理
 * @param request
 * @return
 */
public static OkHttpClientResult syncResponse(Request request){
    if(LOGGER.isDebugEnabled()){
        LOGGER.debug("request start，request:{}", request);
    }
    // 使用异步编程，在指定时间内阻塞获取 OKHttp 异步执行结果
    CompletableFuture<Response> completableFuture = new CompletableFuture();
    // 发起异步请求调用
    Call call = okHttpClient.newCall(request);
    call.enqueue(new Callback(){

        @Override
        public void onFailure(Call call, IOException e) {
            completableFuture.completeExceptionally(e);
        }

        @Override
        public void onResponse(Call call, Response response) throws IOException {
            completableFuture.complete(response);
        }
    });
    // 这里的异步执行结果等待时间，取决于get同步获取时间的设定，而不是全局请求超时配置
    try (Response response = completableFuture.get(DEFAULT_WAIT_TIMEOUT, TimeUnit.MILLISECONDS);) {
        return buildResponseResult(request, response);
    } catch (TimeoutException e){
        call.cancel();
        LOGGER.error("request timeout，request:" +  request, e);
        return new OkHttpClientResult(false, 500, "request timeout");
    } catch (Exception e){
        LOGGER.error("request error，request:" +  request, e);
        return new OkHttpClientResult(false, 500, "request error");
    }
}
```
此方案其实本质是采用 OkHttp 异步请求的方式来编程，**只是为了满足业务上同步返回请求结果**，加入了`CompletableFuture`异步编程类，通过同步阻塞的方式来获取响应结果，以满足高性能的网络请求需要。<br />为了眼见为实，同步请求和异步请求同步阻塞等待结果，看看不同的请求模式下，请求所耗时的时间情况。<br />以循环发起多次请求为例，统计结果如下：

|  | 循环请求200次 | 循环请求800次 |
| --- | --- | --- |
| 同步请求 | 6176 ms | 24276 ms |
| 异步请求同步阻塞等待结果 | 6150 ms | 24598ms |

以同一时刻，发起多次请求为例，统计结果如下：

|  | 同一时刻发起200次请求 | 同—时刻发起800次请录 |
| --- | --- | --- |
| 同步请求 | 1997ms | 6412ms |
| 异步请求同步阻塞等待结果 | 1223ms | 2308ms |

从数据的结论上看，如上面的分析一样，在循环请求场景下，两者几乎无差异，但是在多线程请求场景下，两者差异很明显。<br />采用**异步请求同步阻塞等待结果**这种编程方式，能有效的降低接口请求所消耗的时间。<br />为什么在多线程环境下，**异步请求同步阻塞等待结果**这种方式能显著的提升效率呢，其实答案也很简单，因为 OkHttp 在进行异步请求的时候，使用了线程池来进行执行任务，能有效的管理需要执行的任务。<br />而采用同步编程的方式，执行的时候是直接发起同步调用的，无线程池管理，这就可能会出现一个问题，当瞬间出现大量的线程发起请求的时候，可能会导致 cpu 突然利用率飙升，资源会严重挤压占用，反而不利于任务的有效执行。<br />最后有一个地方特别需要注意，如果项目中使用了自定义的线程池来异步的执行任务，**一定要向JVM注册一个关闭钩子**，当 JVM 监听到服务关闭时，可以通过代码检查逻辑，等待线程池中的任务执行完毕之后，再关停服务，以防止数据丢失。
