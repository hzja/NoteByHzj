系统中经常需要调用第三方接口实现业务功能，为了方便调试和定位问题，通常需要将接口调用参数和返回结果打印到日志文件中。在Spring项目中一般会用RestTemplate来调用第三方接口。 通过在RestTemplate调用过程中统一打印日志，可以保持代码的整洁，也可以统一日志格式，比在业务逻辑中到处打印接口调用日志要方便的多。<br />具体的代码参照示例项目：[https://github.com/qihaiyan/springcamp/tree/master/spring-rest-template-log](https://github.com/qihaiyan/springcamp/tree/master/spring-rest-template-log)
<a name="iFjqq"></a>
## 一、概述
RestTemplate使用前需要先定义bean，在定义bean时可以通过指定`interceptors`来打印日志。
<a name="CP6sg"></a>
## 二、定义`RestTemplate`的bean，并指定`interceptors`
`RestTemplate`的bean的定义在`RestTemplateConfig`类中实现。
```java
@Bean
public RestTemplate restTemplate(RestTemplateBuilder builder) {
  return builder
                .requestFactory(() -> new HttpComponentsClientHttpRequestFactory(httpClient()))
                .interceptors(new CustomClientHttpRequestInterceptor())
                .build();
}
```
其中 `interceptors` 方法用来指定自己实现的日志打印 `interceptors` 。
<a name="pBhJP"></a>
## 三、实现日志打印 `interceptors`
自定义的`interceptors`需要实现 `ClientHttpRequestInterceptor` 这个 `interface`。
```java
static class CustomClientHttpRequestInterceptor implements ClientHttpRequestInterceptor {
    @Override
    @NonNull
    public ClientHttpResponse intercept(HttpRequest request, @NonNull byte[] bytes, @NonNull ClientHttpRequestExecution execution) throws IOException {
        log.info("HTTP Method: {}, URI: {}, Headers: {}", request.getMethod(), request.getURI(), request.getHeaders());
        request.getMethod();
        if (request.getMethod().equals(HttpMethod.POST)) {
            log.info("HTTP body: {}", new String(bytes, StandardCharsets.UTF_8));
        }

        ClientHttpResponse response = execution.execute(request, bytes);
        ClientHttpResponse responseWrapper = new BufferingClientHttpResponseWrapper(response);

        String body = StreamUtils.copyToString(responseWrapper.getBody(), StandardCharsets.UTF_8);
        log.info("RESPONSE body: {}", body);

        return responseWrapper;
    }
}
```
接口请求地址和请求参数的日志打印比较简单，将`intercept`方法中的参与打印到日志中即可。<br />返回结果的body的日志打印需要做一些特殊处理。<br />`ClientHttpResponse response = execution.execute(request, bytes);` 这行代码拿到了返回结果，但是不能直接读取返回的数据。<br />因为返回结果中的`getBody()`方法返回的是 InputStream ，直接读取后，会导致后续的处理拿不到结果。<br />因此需要对返回`execution.execute`方法返回的结果进行包装，将返回结果放到自定义的`BufferingClientHttpResponseWrapper`类中。
```java
ClientHttpResponse responseWrapper = new BufferingClientHttpResponseWrapper(response);
```
`BufferingClientHttpResponseWrapper`类会将body的数据复制到一个本地变量中，用于支持多次读取。<br />`BufferingClientHttpResponseWrapper`类的实现如下：
```java
static class BufferingClientHttpResponseWrapper implements ClientHttpResponse {

    private final ClientHttpResponse response;
    private byte[] body;

    BufferingClientHttpResponseWrapper(ClientHttpResponse response) {
        this.response = response;
    }

    @NonNull
    public HttpStatusCode getStatusCode() throws IOException {
        return this.response.getStatusCode();
    }

    @NonNull
    public int getRawStatusCode() throws IOException {
        return this.response.getRawStatusCode();
    }

    @NonNull
    public String getStatusText() throws IOException {
        return this.response.getStatusText();
    }

    @NonNull
    public HttpHeaders getHeaders() {
        return this.response.getHeaders();
    }

    @NonNull
    public InputStream getBody() throws IOException {
        if (this.body == null) {
            this.body = StreamUtils.copyToByteArray(this.response.getBody());
        }
        return new ByteArrayInputStream(this.body);
    }

    public void close() {
        this.response.close();
    }
}
```
这个类主要是对 `getBody()` 进行了特殊处理，在方法调用时，通过 `treamUtils.copyToByteArray` 将body数据复制到本地变量中。<br />每次读取body时，都会从本地变量中读取，避免了第一次读取body后，后续再读取body会读不到数据的问题。
<a name="KvtKo"></a>
## 四、调用接口查看日志内容
在代码中模拟调用一个第三方接口 http://someservice/foo ，接口调用在 DemoController 类中实现：
```java
@GetMapping("/demo/get")
public Object demoGet(String arg){
    return restTemplate.postForObject("http://someservice/foo", newBodyRequest("test"), BodyRequest.class);
}
```
在单元测试代码 DemoApplicationTest 中调用这个接口。
```java
String resp = testRestTemplate.getForObject("/demo/get?arg=test", String.class)
```
执行单元测试代码后，可以看到日志中打印的接口调用参数和返回结果：
```
HTTP Method: POST, URI: http://someservice/foo, Headers: [Accept:"application/json, application/*+json", Content-Type:"application/json", Content-Length:"15"]
HTTP body: {"arg1":"test"}
RESPONSE body: {"code": 200}
```
日志中打印的返回结果 {"code": 200} 是在单元测试中对 http://someservice/foo 接口mock的数据。
