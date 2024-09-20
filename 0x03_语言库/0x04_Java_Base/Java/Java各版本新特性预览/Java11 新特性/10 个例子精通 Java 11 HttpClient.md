Java
<a name="Tw5AX"></a>
## 一、前言
Java11 之前 Java 只提供了 HttpURLConnection API，它并不太好用，而且性能差。因此，通常大家使用的第三方库，如 Apache HttpClient、Jetty、okhttp 和 Spring 的 RestTemplate。<br />与 HttpURLConnection 不同，Java11 的 HTTP Client 提供了同步和异步请求机制。<br />API 包括3个中心类：**HttpRequest** 表示 HttpClient 需要发送的请求。**HttpClient** 用于发送同步或异步请求。**HttpResponse** 表示 HttpRequest 调用的最终结果。
<a name="wuyVb"></a>
## 二、HttpClient 同步和异步
先看看 Javadoc 中的同步示例，了解如何使用  HttpClient 发送同步请求：
```java
HttpClient client = HttpClient.newBuilder()
        .version(Version.HTTP_1_1)
        .followRedirects(Redirect.NORMAL)
        .connectTimeout(Duration.ofSeconds(20))
        .proxy(ProxySelector.of(new InetSocketAddress("proxy.example.com", 80)))
        .authenticator(Authenticator.getDefault())
        .build();

HttpRequest request = HttpRequest.newBuilder()
        .uri(URI.create("https://www.dreamlu.net"))
        .timeout(Duration.ofMinutes(2))
        .header("Content-Type", "application/json")
        .POST(BodyPublishers.ofFile(Paths.get("file.json")))
        .build();

HttpResponse<String> response = client.send(request, BodyHandlers.ofString());
System.out.println(response.statusCode());
System.out.println(response.body());
```
在这种情况下，程序将发送请求并等待响应，一旦收到响应，它将打印状态代码和响应主体。<br />现在，看一个使用 HttpClient API 在 Java 中异步调用 REST API 的示例：
```java
HttpClient client = HttpClient.newBuilder()
    .build();

HttpRequest request = HttpRequest.newBuilder()
        .uri(URI.create("https://www.dreamlu.net"))
        .timeout(Duration.ofMinutes(2))
        .header("Content-Type", "application/json")
        .POST(BodyPublishers.ofFile(Paths.get("file.json")))
        .build();

client.sendAsync(request, BodyHandlers.ofString())
        .thenApply(HttpResponse::body)
        .thenAccept(System.out::println);
```
上面示例中使用了 `**sendAsync()**` 方法而不是 `**send()**`  方法。这意味着，使用 `HttpClient.send()` 发送同步请求，使用 `HttpClient.sendAsync()` 向任何HTTP服务器或 RESTful Web 服务发出异步请求。<br />现在，来看看如何在 HttpRequest 添加超时、Headers 和 cookie 等参数。
<a name="kgxSo"></a>
## 三、HttpRequest 请求
<a name="EDAh1"></a>
### 1、HttpRequest
HttpRequest 是一个表示需要发送的请求。使用 `HttpRequest.Builder` 可以创建新的实例。可以通过调用 `HttpRequest.newBuilder()` 来实现。<br />在 JDK16 中，有一个全新的 `HttpRequest.newBuilder(HttpRequest request, BiPredicate filter)` 方法，它创建了一个生成器，它的配置是从已经存在的 HttpRequest 复制。
<a name="qFONN"></a>
### 2、如何设置 URI
> Setting up a URI is actually the first thing you need to do when creating a new request. That is, you need to provide a URL. This can be done in a couple of ways. You can use a constructor for a Builder along with a URI parameter. You can then call uri(URI) on the instance Builder.

设置 URI 实际上是创建新 request 时需要做的第一件事。可以通过多种方式实现。可以将 Builder 与URI参数一起用于构造器。然后可以在实例生成器上调用 URI(uri) 方法。
```java
HttpRequest.newBuilder(new URI("https://www.dreamlu.net/get"))
 
HttpRequest.newBuilder()
           .uri(new URI("https://www.dreamlu.net/get"))
```
<a name="cqFG1"></a>
### 3、如何制定 HTTP Method
可以定义请求使用的 HTTP method。这可以通过调用 Builder 中的方法来完成。这些方法是：

- GET()
- POST(BODYPUBLISHER Body)
- PUT(BODYPUBLISHER Body)
- DELETE()

如下，创建一个简单的 GET 请求。
```java
HttpRequest request = HttpRequest.newBuilder()
  .uri(new URI("https://www.dreamlu.net/get"))
  .GET()
  .build();
```
<a name="Nu6vu"></a>
### 4、如何设置 HTTP 协议版本
HttpClient API 完全支持 HTTP 2 协议，并且默认情况下使用 HTTP 2。可以通过 Builder 的 `version()` 方法来设置：
```java
HttpRequest request = HttpRequest.newBuilder()
  .uri(new URI("https://www.dreamlu.net/get"))
  .version(HttpClient.Version.HTTP_2)
  .GET()
  .build();
```
注意：如果不支持 HTTP2，HttpClient 会自动切换到 HTTP1.1。
<a name="iXTF0"></a>
### 5、如何设置 Headers 请求头
向请求中添加 headers 也非常容易。这可以通过使用 Builder 的 headers() 方法轻松完成。
```java
HttpRequest request = HttpRequest.newBuilder()
  .uri(new URI("https://www.dreamlu.net/get"))
  .headers("key1", "value1", "key2", "value2")
  .GET()
  .build()

HttpRequest request2 = HttpRequest.newBuilder()
  .uri(new URI("https://www.dreamlu.net/get"))
  .header("key1", "value1")
  .header("key2", "value2")
  .GET()
  .build();
```
<a name="ytfG2"></a>
### 6、如何设置 Timeout
可以使用 Timeout 设置等待某个响应所需的时间。如果超时，将抛出 `HttpTimeoutException`。默认情况下，它实际上设置为无穷大。它也可以通过使用 `Timeout()` 方法使用 Duration 对象设置。
```java
HttpRequest request = HttpRequest.newBuilder()
  .uri(new URI("https://www.dreamlu.net/get"))
  .timeout(Duration.of(10, SECONDS))
  .GET()
  .build()
```
<a name="W5NQx"></a>
### 7、如何设置请求 Body
可以通过使用 request builder 方法（如 POST(BodyPublisher body)、PUT(BodyPublisher body) 和 DELETE() 将主体添加到请求中。如果没有请求报文，可以简单地传递 `HttpRequest.BodyPublishers.noBody()`。
```java
HttpRequest request = HttpRequest.newBuilder()
  .uri(new URI("https://www.dreamlu.net/post"))
  .POST(HttpRequest.BodyPublishers.noBody())
  .build();
```
<a name="O8W5P"></a>
### 8、如何发送 String Body
可以用 BodyPublishers 以非常简单直观的方式设置任何 request body。可以简单地将 String 作为 Body 传递。或者也可以使用 `StringBodyPublisher`。也可以使用 String() 的简单工厂方法创建此对象。这实际上将以字符串对象作为参数，然后创建一个 request Body。
```java
HttpRequest request = HttpRequest.newBuilder()
  .uri(new URI("https://www.dreamlu.net/post"))
  .headers("Content-Type", "text/plain;charset=UTF-8")
  .POST(HttpRequest.BodyPublishers.ofString("Sample request body"))
  .build();
```
<a name="yrJYK"></a>
### 9、如何发送 InputStream Body
必须以 Supplier 传递 `InputStream`。这与 `StringBodyPublishers` 有点不同。
```java
byte[] sampleData = "Sample request body".getBytes();

HttpRequest request = HttpRequest.newBuilder()
  .uri(new URI("https://www.dreamlu.net/post"))
  .headers("Content-Type", "text/plain;charset=UTF-8")
  .POST(HttpRequest.BodyPublishers
   .ofInputStream(() -> new ByteArrayInputStream(sampleData)))
  .build();
```
在这段代码中使用简单的 `ByteArrayInputStream`，也可以使用其他任何 InputStream 实现。
<a name="oMiwg"></a>
### 10、如何发送 ByteArray
可以使用 `ByteArrayProcessor` 来传递字节数组作为参数。
```java
byte[] sampleData = "Sample request body".getBytes();

HttpRequest request = HttpRequest.newBuilder()
  .uri(new URI("https://www.dreamlu.net/post"))
  .headers("Content-Type", "text/plain;charset=UTF-8")
  .POST(HttpRequest.BodyPublishers.ofByteArray(sampleData))
  .build();
```
<a name="awenf"></a>
## 四、总结
单纯就使用体验来说 Java11 HttpClient 使用并不如mica-http（基于 okhttp）方便。示例（mica-http 请求代码）：
```java
// 同步，异常时 返回 null
String html = HttpRequest.get("https://www.baidu.com")
 .connectTimeout(Duration.ofSeconds(1000))
 .query("test", "a")
 .query("name", "張三")
 .query("x", 1)
 .query("abd", Base64Util.encode("123&$#%"))
 .queryEncoded("abc", Base64Util.encode("123&$#%"))
 .execute()
 .onFailed(((request, e) -> {
  e.printStackTrace();
 }))
 .onSuccess(ResponseSpec::asString);
```
随着 Spring boot 3.x 的推进，更多项目将会升级到 Java17，那么 jdk11 开始自带的这个 HttpClient 将会是 sdk 开发和中间件开发最佳选择。
