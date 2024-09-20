JavaSpring<br />在最近发布的Spring 6.1 M2版本中，推出了一个全新的同步HTTP客户端：RestClient。用一句话来让Spring开发者认识RestClient的话：**像**`**WebClient**`**一样具备流畅API的**`**RestTemplate**`。<br />所以，`RestClient`的使命就是淘汰已经有14年历史的`RestTemplate`。
<a name="rcf8Y"></a>
## RestClient案例
下面通过几个官方给出的案例一起来快速的认识一下RestClient。
<a name="TbnxJ"></a>
### HTTP请求
下面是一个最简单的GET请求，返回一个字符串。从这个例子中，可以看到API形式跟WebClient类似。不像以前用RestTemplate的时候那么麻烦。
```java
RestClient restClient = RestClient.create();

String result = restClient.get()
  .uri("https://example.com")
  .retrieve()
  .body(String.class);
System.out.println(result);
```
关于GET请求，很多时候返回的不仅仅是String，更多的时候是一些实体；同时有时候还需要获取HTTP状态码以及头信息。这个时候，可以使用`toEntity`方法来返回一个更为通用的`ResponseEntity`来进行后续操作，比如下面这样：
```java
ResponseEntity<String> result = restClient.get()
  .uri("https://example.com")
  .retrieve()
  .toEntity(String.class);

System.out.println("Response status: " + result.getStatusCode());
System.out.println("Response headers: " + result.getHeaders());
System.out.println("Contents: " + result.getBody());
```
在业务层面，为了更方便的解析业务数据。`RestClient`还支持对结果进行对象转换。比如下面的例子，就是把HTTP请求返回的JSON数据转化为Pet对象。这样就免去了开发者手动从`ResponseEntity`中获取内容，再进行消息转化的麻烦。
```java
int id = ...
Pet pet = restClient.get()
  .uri("https://petclinic.example.com/pets/{id}", id)
  .accept(APPLICATION_JSON)
  .retrieve()
  .body(Pet.class);
```
关于其他请求，也是类似的，比如下面的POST请求：
```java
Pet pet = ...
ResponseEntity<Void> response = restClient.post()
  .uri("https://petclinic.example.com/pets/new")
  .contentType(APPLICATION_JSON)
  .body(pet)
  .retrieve()
  .toBodilessEntity();
```
<a name="WAdAh"></a>
### 错误处理
默认情况下，`RestClient`在接收到4xx和5xx状态码的时候，会抛出一个`RestClientException`的子类。对于这个动作，可以通过`onStatus`方法去重写它，比如下面这样：
```java
String result = restClient.get()
  .uri("https://example.com/this-url-does-not-exist")
  .retrieve()
  .onStatus(HttpStatusCode::is4xxClientError, (request, response) -> {
      throw new MyCustomRuntimeException(response.getStatusCode(), response.getHeaders())
  })
  .body(String.class);
```
上面的例子是进一步做了包装，并重新抛出。当然也可以做一些业务性的其他操作。
<a name="C51nQ"></a>
### 高级处理 `Exchange`
直接看下面的例子：
```java
Pet result = restClient.get()
.uri("https://petclinic.example.com/pets/{id}", id)
.accept(APPLICATION_JSON)
.exchange((request, response) -> {
    if (response.getStatusCode().is4xxClientError()) {
        throw new MyCustomRuntimeException(response.getStatusCode(), response.getHeaders());
    }
    else {
        Pet pet = convertResponse(response);
        return pet;
    }
});
```
exchange方法提供了更灵活且完整的请求处理入口。在这里，开发者里获取到request信息，也可以操作response信息。所以，如果有复杂的处理逻辑上一节中的请求方法无法满足需要的时候，就可以通过这里的`exchange`方法来定制复杂的处理逻辑。
<a name="gDxLT"></a>
## 小结
相信大家对RestTemplate一定都不陌生，但实际应用估计已经不是很多了，更多的会使用一些其他的客户端来实现HTTP的调用。如今Spring 6.1将推出的RestClient将很好的弥补这块不足，同时与WebClient互相补充。
