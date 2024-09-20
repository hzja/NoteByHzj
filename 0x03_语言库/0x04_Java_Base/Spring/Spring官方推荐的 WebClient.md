JavaSpring<br />在 Spring Framework 5.0 及更高版本中，RestTemplate 已被弃用，取而代之的是较新的 WebClient。这意味着虽然 RestTemplate 仍然可用，但鼓励 Spring 开发人员迁移到新项目的 WebClient。<br />WebClient 优于 RestTemplate 的原因有几个：

- **「非阻塞 I/O」**：WebClient 构建在 Reactor 之上，它提供了一种非阻塞、反应式的方法来处理 I/O。这可以在高流量应用程序中实现更好的可扩展性和更高的性能。
- **「函数式风格」**：WebClient 使用函数式编程风格，可以使代码更易于阅读和理解。它还提供了流畅的 API，可以更轻松地配置和自定义请求。
- **「更好地支持流式传输」**：WebClient 支持请求和响应正文的流式传输，这对于处理大文件或实时数据非常有用。
- **「改进的错误处理」**：WebClient 提供比 RestTemplate 更好的错误处理和日志记录，从而更轻松地诊断和解决问题。

**重点：即使升级了spring web 6.0.0版本，也无法在**`**HttpRequestFactory**`**中设置请求超时，这是放弃使用 RestTemplate 的最大因素之一。**<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1701918304896-cd182036-4422-45c9-a512-791234f3fa90.png#averageHue=%23fdfde2&clientId=u062efd64-3875-4&from=paste&height=406&id=ub3793993&originHeight=1015&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=422519&status=done&style=none&taskId=u078cedcc-bb86-4614-897b-f9f148a551f&title=&width=1536)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1701918270341-453c80e0-456f-44a3-b1c3-9eb59b8710b8.png#averageHue=%23fefefd&clientId=u062efd64-3875-4&from=paste&height=204&id=ue9544b8a&originHeight=510&originWidth=2946&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=96656&status=done&style=none&taskId=ue81d6649-48b3-4ec4-b328-1daf249ef8a&title=&width=1178.4)<br />设置请求超时不会有任何影响<br />总的来说，虽然 RestTemplate 可能仍然适用于某些用例，但 WebClient 提供了几个优势，使其成为现代 Spring 应用程序的更好选择。
<a name="iUqwL"></a>
## WebClient使用案例
看看如何在 SpringBoot 3 应用程序中使用 WebClient。
<a name="rScZq"></a>
### (1) 创建网络客户端
```java
import io.netty.channel.ChannelOption;
import io.netty.channel.ConnectTimeoutException;
import io.netty.handler.timeout.ReadTimeoutException;
import io.netty.handler.timeout.ReadTimeoutHandler;
import io.netty.handler.timeout.TimeoutException;
import jakarta.annotation.PostConstruct;
import java.time.Duration;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpMethod;
import org.springframework.http.MediaType;
import org.springframework.http.client.reactive.ReactorClientHttpConnector;
import org.springframework.stereotype.Service;
import org.springframework.web.reactive.function.client.WebClient;
import org.springframework.web.reactive.function.client.WebClientRequestException;
import org.springframework.web.reactive.function.client.WebClientResponseException;
import reactor.core.publisher.Mono;
import reactor.netty.http.client.HttpClient;


HttpClient httpClient =
        HttpClient.create()
            .option(ChannelOption.CONNECT_TIMEOUT_MILLIS, connectionTimeout)
            .responseTimeout(Duration.ofMillis(requestTimeout))
            .doOnConnected(conn -> conn.addHandlerLast(new ReadTimeoutHandler(readTimeout)));

   WebClient client =
        WebClient.builder().clientConnector(new ReactorClientHttpConnector(httpClient)).build();
```
<a name="vtVog"></a>
### (2) 同步发送请求（就像RestTemplate一样）
如果想坚持使用发送 HTTP 请求并等待响应的老方法，也可以使用 WebClient 实现如下所示的相同功能：
```java
public String postSynchronously(String url, String requestBody) {
    LOG.info("Going to hit API - URL {} Body {}", url, requestBody);
    String response = "";
    try {
        response =
        client
        .method(HttpMethod.POST)
        .uri(url)
        .accept(MediaType.ALL)
        .contentType(MediaType.APPLICATION_JSON)
        .bodyValue(requestBody)
        .retrieve()
        .bodyToMono(String.class)
        .block();

    } catch (Exception ex) {
        LOG.error("Error while calling API ", ex);
        throw new RunTimeException("XYZ service api error: " + ex.getMessage());
    } finally {
        LOG.info("API Response {}", response);
    }

    return response;
}
```
`block()`用于同步等待响应，这可能并不适合所有情况，可能需要考虑`subscribe()`异步使用和处理响应。
<a name="Vpav4"></a>
### (3) 异步发送请求
有时不想等待响应，而是希望异步处理响应，这可以按如下方式完成：
```java
import org.springframework.http.MediaType;
import org.springframework.web.reactive.function.BodyInserters;
import org.springframework.web.reactive.function.client.WebClient;
import reactor.core.publisher.Mono;

public static Mono<String> makePostRequestAsync(String url, String postData) {
    WebClient webClient = WebClient.builder().build();
    return webClient.post()
            .uri(url)
            .contentType(MediaType.APPLICATION_FORM_URLENCODED)
            .body(BodyInserters.fromFormData("data", postData))
            .retrieve()
            .bodyToMono(String.class);
}
```
要使用此函数，只需传入要向其发送 POST 请求的 URL 以及要在请求正文中以 URL 编码字符串形式发送的数据。该函数将返回来自服务器的响应，或者如果请求由于任何原因失败，则返回一条错误消息。<br />请注意，在此示例中，WebClient是使用默认配置构建的。可能需要根据不同要求进行不同的配置。<br />另请注意，`block()`用于同步等待响应，这可能并不适合所有情况。可能需要考虑`subscribe()`异步使用和处理响应。<br />要使用响应，可以订阅Mono并异步处理响应。下面是一个例子：
```java
makePostRequestAsync( "https://example.com/api" , "param1=value1&m2=value2" ) 
.subscribe(response -> { 
    // 处理响应
    System.out.println ( response ); 
}, error -> { 
    / / 处理错误
    System.err.println ( error .getMessage ());     
    }
);
```
`subscribe()`用于异步处理响应，可以提供两个 lambda 表达式作为 `subscribe()` 的参数。如果请求成功并收到响应作为参数，则执行第一个 lambda 表达式；如果请求失败并收到错误作为参数，则执行第二个 lambda 表达式。
<a name="ChZWf"></a>
### (4) 处理4XX和5XX错误
```java
import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.web.reactive.function.BodyInserters;
import org.springframework.web.reactive.function.client.WebClient;
import reactor.core.publisher.Mono;

public static Mono<String> makePostRequestAsync(String url, String postData) {
    WebClient webClient = WebClient.builder()
            .baseUrl(url)
            .build();
    return webClient.post()
            .uri("/")
            .contentType(MediaType.APPLICATION_FORM_URLENCODED)
            .body(BodyInserters.fromFormData("data", postData))
            .retrieve()
            .onStatus(HttpStatus::is4xxClientError, clientResponse -> Mono.error(new RuntimeException("Client error")))
            .onStatus(HttpStatus::is5xxServerError, clientResponse -> Mono.error(new RuntimeException("Server error")))
            .bodyToMono(String.class);
}
```
在此示例中，该`onStatus()`方法被调用两次，一次针对 4xx 客户端错误，一次针对 5xx 服务器错误。`onStatus()` 每次调用都采用两个参数：

- `aPredicate`确定错误状态代码是否与条件匹配
- `aFunction`用于返回Mono，即要传播到订阅者的错误信息。

如果状态代码与条件匹配，Mono则会发出相应的状态代码，并且Mono链会因错误而终止。在此示例中，Mono 将发出一条 RuntimeException 错误消息，指示该错误是客户端错误还是服务器错误。
<a name="Iqhfh"></a>
### (5) 根据错误状态采取行动
要根据Mono的`subscribe()`方法中的错误采取操作，可以在`subscribe`函数中处理响应的lambda表达式之后添加另一个lambda表达。如果在处理`Monumber`的过程中出现错误，则执行第二个lambda表达式。<br />下面是如何使用`makePostRequestAsync`函数和处理`subscribe`方法中的错误的更新示例：
```java
makePostRequestAsync("https://example.com/api", "param1=value1&param2=value2")
.subscribe(response -> {
    // handle the response
    System.out.println(response);
}, error -> {
    // handle the error
    System.err.println("An error occurred: " + error.getMessage());
    if (error instanceof WebClientResponseException) {
        WebClientResponseException webClientResponseException = (WebClientResponseException) error;
        int statusCode = webClientResponseException.getStatusCode().value();
        String statusText = webClientResponseException.getStatusText();
        System.err.println("Error status code: " + statusCode);
        System.err.println("Error status text: " + statusText);
    }
});
```
`subscribe`方法中的第二个lambda表达式检查错误是否是`WebClientResponseException`的实例，这是WebClient在服务器有错误响应时抛出的特定类型的异常。如果它是`WebClientResponseException`的实例，则代码将从异常中提取状态代码和状态文本，并将它们记录到日志中。<br />还可以根据发生的特定错误在此lambda表达式中添加其他错误处理逻辑。例如，可以重试请求、回退到默认值或以特定方式记录错误。
<a name="r4Wxv"></a>
### (6) 处理成功响应和错误的完整代码
```java
responseMono.subscribe(
response -> {
    // handle the response
    LOG.info("SUCCESS API Response {}", response);
},
error -> {
    // handle the error
    LOG.error("An error occurred: {}", error.getMessage());
    LOG.error("error class: {}", error.getClass());

    // Errors / Exceptions from Server
    if (error instanceof WebClientResponseException) {
        WebClientResponseException webClientResponseException =
        (WebClientResponseException) error;
        int statusCode = webClientResponseException.getStatusCode().value();
        String statusText = webClientResponseException.getStatusText();
        LOG.info("Error status code: {}", statusCode);
        LOG.info("Error status text: {}", statusText);
        if (statusCode >= 400 && statusCode < 500) {
            LOG.info(
                "Error Response body {}", webClientResponseException.getResponseBodyAsString());
        }

        Throwable cause = webClientResponseException.getCause();
        LOG.error("webClientResponseException");
        if (null != cause) {
            LOG.info("Cause {}", cause.getClass());
            if (cause instanceof ReadTimeoutException) {
                LOG.error("ReadTimeout Exception");
            }
            if (cause instanceof TimeoutException) {
                LOG.error("Timeout Exception");
            }
        }
    }

    // Client errors i.e. Timeouts etc - 
    if (error instanceof WebClientRequestException) {
        LOG.error("webClientRequestException");
        WebClientRequestException webClientRequestException =
        (WebClientRequestException) error;
        Throwable cause = webClientRequestException.getCause();
        if (null != cause) {
            LOG.info("Cause {}", cause.getClass());
            if (cause instanceof ReadTimeoutException) {
                LOG.error("ReadTimeout Exception");
            }

            if (cause instanceof ConnectTimeoutException) {
                LOG.error("Connect Timeout Exception");
            }
        }
    }
});
```
<a name="P3Xo4"></a>
### 超时
可以在每个请求中设置超时，如下所示：
```java
return webClient
    .method(this.httpMethod)
    .uri(this.uri)
    .headers(httpHeaders -> httpHeaders.addAll(additionalHeaders))
    .bodyValue(this.requestEntity)
    .retrieve()
    .bodyToMono(responseType)
    .timeout(Duration.ofMillis(readTimeout))  // request timeout for this request
    .block();
```
但是，无法在每个请求中设置连接超时，这是WebClient 的属性，只能设置一次。如果需要，始终可以使用新的连接超时值创建一个新的 Web 客户端实例。<br />连接超时、读取超时和请求超时的区别如下：<br />![DM_20231005192645_001.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1696505254876-4b72dc5e-7eae-4550-9669-7b05c8cceaa5.png#averageHue=%23f8f8f8&clientId=ue868f8ed-674a-4&from=ui&id=uc12d29d9&originHeight=324&originWidth=641&originalType=binary&ratio=1.5&rotation=0&showTitle=false&size=624467&status=done&style=none&taskId=ufc38293a-d5bb-480a-8a54-80ee7701d3f&title=)
<a name="q2gWl"></a>
## 结论
由于 RestTemplace 已弃用，开发人员应开始使用 WebClient 进行 REST 调用，非阻塞 I/O 调用肯定会提高应用程序性能。它不仅提供了许多其他令人兴奋的功能，例如改进的错误处理和对流的支持，而且如果需要，它还可以在阻塞模式下使用来模拟 RestTemplate 行为。
