Java<br />多年来，Spring 框架的 RestTemplate 一直是客户端 HTTP 访问的首选解决方案，它提供同步、阻塞 API 来以简单的方式处理 HTTP 请求。<br />然而，随着对非阻塞、反应式编程以更少的资源处理并发的需求不断增加，特别是在微服务架构中，RestTemplate 已经显示出其局限性。从 Spring Framework 5 开始，RestTemplate 已被标记为已弃用，Spring 团队推荐 WebClient 作为其继任者。<br />在这里将通过实际示例深入探讨 RestTemplate 被弃用的原因、采用 WebClient 的优势以及如何有效过渡。
<a name="v3LnG"></a>
## 1、为什么 RestTemplate 被弃用
**阻塞性质：** RestTemplate 是一个阻塞、同步客户端。这意味着执行请求的线程会阻塞，直到操作完成，这可能会导致线程池耗尽，并在重负载下导致更高的延迟。此模型不能很好地扩展，特别是在应用程序必须有效处理数千个并发请求的微服务环境中。<br />**可扩展性有限：** RestTemplate 的同步特性限制了可扩展性。需要高吞吐量、低延迟能力的现代系统发现这种方法不够。事件驱动、反应式编程范式的兴起是对这些需求的回应，导致了 WebClient 等非阻塞 API 的采用。<br />**缺乏反应式编程支持：** RestTemplate 不支持反应式编程，而反应式编程在基于云的生态系统中日益增长。响应式编程使系统更具响应性、弹性和弹性，但这是 RestTemplate 的阻塞性质无法实现的。
<a name="QrIBL"></a>
## 2、WebClient 的兴起
WebClient 是 Spring WebFlux 库的一部分，随 Spring 5 引入。与 RestTemplate 相比，它具有许多优势：

- **非阻塞操作：** WebClient 使用 Project Reactor 在非阻塞、反应式范例上运行，使其能够以更少的线程和更少的开销处理并发，从而显着提高可扩展性和资源利用率。
- **反应式堆栈：** WebClient 支持反应式堆栈，使其适合基于事件循环的运行时环境。它可以在微服务架构中典型的高并发场景下高效工作。
- **JSON 处理及更多：** WebClient 通过 Jackson 库提供与 JSON 的无缝集成，类似于 RestTemplate，但具有增强的处理能力。它还支持服务器发送事件 (SSE)、流场景和其他高级用例。
<a name="Jt2Dm"></a>
## 3、从 RestTemplate 过渡到 WebClient
通过例子来探讨一下如何用WebClient替换RestTemplate。将从一个简单的 GET 请求开始。
<a name="l69C4"></a>
### 执行 GET 请求
RestTemplate：
```java
RestTemplate restTemplate = new RestTemplate();
ResponseEntity<String> response = restTemplate.getForEntity("http://example.com", String.class);
```
WebClient：
```java
WebClient webClient = WebClient.create();
Mono<String> response = webClient.get()
    .uri("http://example.com")
    .retrieve()
    .bodyToMono(String.class);
response.subscribe(result -> System.out.println(result));
```
在 WebClient 中，一切都是非阻塞的。该`retrieve()`方法发起请求，并将`bodyToMono`响应主体转换为 Reactor Mono。该`subscribe()`方法用于订阅结果，一旦可用就会对其进行处理。
<a name="hN8wW"></a>
### 处理错误
RestTemplate 的错误处理通过`ErrorHandler`接口进行，这需要单独的代码块。WebClient 通过更流畅的处理简化了这一过程。<br />WebClient：
```java
WebClient  webClient  = WebClient.create(); 
webClient.get() 
    .uri( "http://example.com/some-error-endpoint" ) 
    .retrieve() 
    .onStatus(HttpStatus::isError, response -> { 
        // 处理错误状态码
        return Mono.error ( new  CustomException ( "发生自定义错误。" )); 
    }) 
    .bodyToMono(String.class);
```
该`onStatus()`方法允许直接在操作链中处理特定的 HTTP 状态，从而提供更具可读性和可维护性的方法。
<a name="KQRkU"></a>
### 使用 JSON 的 POST 请求
当发出 POST 请求并提交 JSON 时，WebClient 以其流畅的 API 提供了更直接的方法。<br />RestTemplate：
```java
RestTemplate restTemplate = new RestTemplate();
HttpHeaders headers = new HttpHeaders();
headers.setContentType(MediaType.APPLICATION_JSON);
HttpEntity<String> request = new HttpEntity<>("{\"key\":\"value\"}", headers);
ResponseEntity<String> response = restTemplate.postForEntity("http://example.com", request, String.class);
```
WebClient：
```java
WebClient webClient = WebClient.create();
Mono<String> response = webClient.post()
    .uri("http://example.com")
    .contentType(MediaType.APPLICATION_JSON)
    .bodyValue("{\"key\":\"value\"}")
    .retrieve()
    .bodyToMono(String.class);
```
使用 WebClient，设置标头和正文内容更加直观，并且需要更少的样板代码。`contentType()`和`bodyValue()`方法允许直接设置内容类型和正文。
<a name="T9ziM"></a>
### 异步处理
使用 WebClient 最显着的优点之一是它支持异步处理。当应用程序需要进行多个独立的 API 调用时，这尤其有用；这些可以同时执行，从而大大减少这些操作所需的总时间。
```java
WebClient webClient = WebClient.create();
Mono<String> responseOne = webClient.get()
    .uri("http://example.com/endpointOne")
    .retrieve()
    .bodyToMono(String.class);

Mono<String> responseTwo = webClient.get()
    .uri("http://example.com/endpointTwo")
    .retrieve()
    .bodyToMono(String.class);

// Use Mono.zip to execute requests concurrently
Mono.zip(responseOne, responseTwo).subscribe(results -> {
    System.out.println("Result 1: " + results.getT1());
    System.out.println("Result 2: " + results.getT2());
});
```
在此示例中，Mono.zip用于组合多个请求的结果。这些请求是并发执行的，一旦所有请求完成，就会处理结果。这种方法比 RestTemplate 同步操作中固有的顺序执行效率要高得多。
<a name="hzThK"></a>
### 流数据
WebClient 还支持以数据流的形式检索响应主体，这在处理不想一次将其全部保存在内存中的大量数据时特别有用。
```java
WebClient  webClient  = WebClient.create(); 
webClient.get() 
    .uri( "http://example.com/stream" ) 
    .accept(MediaType.TEXT_EVENT_STREAM) // 用于服务器发送事件 (SSE)
     .retrieve() 
    .bodyToFlux(String.class) //将响应正文转换为 Flux
     .subscribe(data -> System.out.println( "Received: " + data));
```
在此场景中，`bodyToFlux`用于将响应正文转换为Flux，表示数据流。然后，该`subscribe`方法用于在每条数据到达时对其进行处理。这与 RestTemplate 形成鲜明对比，后者要求在处理之前将整个响应主体加载到内存中，无论大小如何。
<a name="ceJph"></a>
### 重试机制
WebClient 利用反应式编程模型提供了一种更复杂的方法来重试失败的请求。
```java
WebClient  webClient  = WebClient.builder().baseUrl( "http://example.com" ).build(); 
Mono<String> response = webClient.get() 
    .uri( "/retry-endpoint" ) 
    .retrieve() 
    .bodyToMono(String.class) 
    .retryWhen(Retry.backoff( 3 , Duration.ofSeconds( 1 )) //重试次数和退避配置
        .maxBackoff(Duration.ofSeconds( 10 ))) // 最大退避时间
    .onErrorResume(e -> Mono.just( "Fallback response" )); // 如果重试都失败则回退

response.subscribe(result -> System.out.println(result));
```
在此示例中，该retryWhen方法用于定义重试策略，指定重试次数和退避配置。如果所有重试都失败，`onErrorResume`则提供后备机制。
<a name="oTVRJ"></a>
### 自定义 Web 客户端配置
WebClient 具有高度可配置性，可能会发现默认设置无法满足需求。例如，可能想要调整连接超时，或者可能需要添加应随每个请求发送的默认标头。
```java
// 构建一个具有指定超时和默认标头的自定义 WebClient 
WebClient  customWebClient  = WebClient.builder() 
    .baseUrl( "http://example.com" ) 
    .clientConnector( new  ReactorClientHttpConnector ( 
        HttpClient.create() 
            .option(ChannelOption.CONNECT_TIMEOUT_MILLIS ) , 2000 ) // 2 秒超时
            .responseTimeout(Duration.ofSeconds( 2 )) // 设置响应超时
            .doOnConnected(conn -> 
                conn.addHandlerLast( new  ReadTimeoutHandler ( 2 )) // 2 秒读取超时
                    .addHandlerLast( new  WriteTimeoutHandler ( 2 ))))) // 2 秒写入超时
    .defaultHeader(HttpHeaders.CONTENT_TYPE, MediaType.APPLICATION_JSON_VALUE) // 默认标头
    .defaultHeader( "Another-Header" , "Another-Value" ) // 另一个默认标头
    .build ();
```
在此示例中，自定义 WebClient 以具有特定的超时配置和默认标头。WebClient 的此实例会将这些设置应用于它执行的所有请求，确保整个应用程序的行为一致。
<a name="f2T74"></a>
### 网络客户端过滤器
WebClient 支持使用过滤器来处理横切关注点。这些过滤器可用于操纵请求或响应，甚至可以处理日志记录、指标或授权等问题。
```java
// 带过滤器的自定义 WebClient 
WebClient  FilteredWebClient  = WebClient.builder() 
    .baseUrl( "http://example.com" ) 
    .filter((request, next) -> { 
        // 记录请求数据
        System.out.println( "Request: " + request.method() + " " + request.url()); 
        return next.exchange(request).doOnSuccessOrError((response, error) -> { 
            if (response != null ) { 
                // 日志响应数据
                System.out.println( "响应状态: " + response.statusCode()); 
            } 
            if (error != null ) { 
                // 记录错误
                System.out.println( "错误: " + error.getMessage() ); 
            } 
        }); 
    }) 
    .build();
```
此过滤器记录通过此 WebClient 实例发出的每个请求的 HTTP 方法和 URL，以及收到的每个响应的状态代码。它还记录交换期间可能发生的任何错误。
<a name="MRT42"></a>
### 相互 TLS 身份验证
在需要增强安全性的场景中（例如内部微服务通信），可能需要相互 TLS (mTLS) 身份验证。可以针对此类场景配置 WebClient。
```java
// 使用信任存储和密钥存储准备 SSL 上下文
SslContext  sslContext  = SslContextBuilder 
    .forClient() 
    .trustManager(InsecureTrustManagerFactory.INSTANCE) // 仅用于演示目的！
    .keyManager( new  File ( "path/to/client.crt" ), new  File ( "path/to/client.key" )) // 客户端证书和私钥
    .build(); 

// 使用 SSL 上下文配置 WebClient WebClient 
secureWebClient  =  WebClient.builder () 
    .clientConnector( new  ReactorClientHttpConnector (HttpClient.create().secure(sslContextSpec -> sslContextSpec.sslContext(sslContext)))) 
    .build();
```
在此示例中，通过使用包含客户端证书和私钥的 SSL 上下文配置 WebClient 来设置 mTLS。此设置可确保客户端和服务器在 SSL 握手期间相互验证。
<a name="DNYOb"></a>
## 4、网络客户端最佳实践
以下是一些确保高效且有效地使用 WebClient 的指南：
<a name="Ttya2"></a>
### 单例模式
与通常根据请求或服务实例化的 RestTemplate 不同，WebClient 被设计为用作单例。这意味着应该创建 WebClient 的单个实例并在应用程序中重用它。这种方式保证了资源的高效利用，避免了重复创建和销毁WebClient实例的开销。
```java
@Bean 
public WebClient.Builder webClientBuilder () { 
    return WebClient.builder(); 
}
```
通过在配置中定义 `WebClient.Builder` bean，可以在任何需要的地方自动装配它，并针对特定用例对其进行自定义，而无需每次都创建新的 WebClient 实例。
<a name="ytNEa"></a>
### 错误处理
反应性流向下游传播错误，直到错误被处理或到达流的末尾。始终处理反应链中的错误，以避免意外行为。`onErrorResume`、`onErrorReturn`和`doOnError`运算符特别有用。
```java
webClient.get()
    .uri("/endpoint")
    .retrieve()
    .bodyToMono(String.class)
    .doOnError(e -> log.error("Error occurred", e))
    .onErrorResume(e -> Mono.just("Fallback value"));
```
<a name="AtK06"></a>
### 超时配置
始终配置超时。如果没有超时，如果服务器没有响应，WebClient 请求可能会无限期挂起。使用timeout运算符设置特定的持续时间，在此之后请求将被终止。
```java
webClient.get() 
    .uri("/endpoint")"/endpoint" ) 
    .retrieve() 
    .bodyToMono(String.class) 
    .timeout(Duration.ofSeconds( 10 ));
```
<a name="m0peo"></a>
### 背压
反应式编程的核心原则之一是背压，它允许消费者向生产者发出他们可以处理多少数据的信号。处理Flux（0 到 N 个项目的流）时，请注意背压并确保不会让消费者感到不知所措。使用运算符来limitRate控制数据流的速率。
<a name="FVTAX"></a>
### 记录
日志记录对于调试和监控至关重要。WebClient 提供内置日志记录功能。通过将记录器设置`reactor.netty.http.client.HttpClient`为DEBUG，可以查看请求和响应的详细日志。
<a name="M3NLO"></a>
### 线程上下文
在反应式编程中，操作可能会多次切换线程。如果依赖线程局部变量（如日志记录或安全上下文中使用的变量），请注意这些变量可能不会自动跨线程传播。像reactor-context这样的库可以帮助在反应流中跨线程传播上下文。
<a name="AQqAa"></a>
### 避免阻塞呼叫
WebClient和反应式编程的主要优点之一是操作的非阻塞特性。但是，如果在反应链中引入阻塞调用，则此好处将被否定。始终避免在反应流中阻塞操作。如果必须使用阻塞调用，请考虑使用`subscribeOn`将其卸载到单独的线程池中。
```java
Mono.fromCallable(() ->blockingMethod()) 
    .subscribeOn(Schedulers.boundedElastic());
```
WebClient提供了一种现代的、非阻塞的、响应式的方法来生成HTTP请求，这使得它成为大多数用例中优于已被弃用的RestTemplate的选择。然而，伴随着它的力量而来的是正确使用它的责任。通过遵循最佳实践，了解反应式范例，并意识到潜在的缺陷，可以充分利用WebClient的潜力，并构建高效、可伸缩和响应性强的应用程序。
