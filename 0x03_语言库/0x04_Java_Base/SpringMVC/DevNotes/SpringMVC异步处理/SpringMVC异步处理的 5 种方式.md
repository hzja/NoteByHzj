Java SpringMVC 异步<br />异步处理最大的好处是可以提高并发量，不阻塞当前线程。其实 Spring MVC 也支持了异步处理。
<a name="vRpCO"></a>
### 异步处理 demo
如果要启用异步返回，需要开启 `@EnableAsync`。如下的代码中，使用 `DeferredResult` 进行异步处理。<br />请求进来后，首先创建 `DeferredResult` 对象，设置超时时间为 60 秒。然后指定 DeferredResult 在异步完成和等待超时时的回调。同步的处理只需要创建异步任何，然后返回 `DeferredResult` 即可。这样 Spring MVC 处理完此次请求后，不会立即返回 response 给客户端，会一直等待 `DeferredResult` 处理完成。如果 `DeferredResult` 没有在 60 秒内处理完成，就会触发超时，然后返回 response 给客户端。
```java
@RequestMapping(value = "/async/demo")
public DeferredResult<String> async(){
    // 创建 DeferredResult，设置超时时间 60s
    DeferredResult<String> deferredResult = new DeferredResult<>((long)60 * 1000);
    String uuid = UUID.randomUUID().toString();
    Runnable callback = () -> manager.remove(deferredResult, uuid);
    // 设置完成和超时的回调
    deferredResult.onCompletion(callback);
    deferredResult.onTimeout(callback);
    // 创建异步任务
    manager.addAsyncTask(deferredResult, uuid);
    // 同步返回 DeferredResult
    return deferredResult;
}
```
对于异步任务来说，需要持有 `DeferredResult` 对象。在异步处理结束时，需要手动调用 `DeferredResult.setResult` 完成输出。调用 `setResult` 时，数据输出写到客户端，然后触发异步完成事件执行回调。
```java
task.getDeferredResult().setResult(ConfigJsonUtils.toJsonString(map));
```
<a name="rYFMK"></a>
### 使用 `DeferredResult` 进行异步处理
`DeferredResult` 这个类代表延迟结果。`DeferredResult` 可以用在异步任务中，其他线程能够获取 DeferredResult 并设置 `DeferredResult` 的返回数据。通常可以使用线程池、队列等配合 `DeferredResult` 实现异步处理。<br />根据官方描述，Spring MVC 处理流程如下：

1. 把 controller 返回的 `DeferredResult` 保存在内存队列或集合当中；
2. Spring MVC 调用 `request.startAsync()`，开启异步；
3. `DispatcherServlet` 和所有的 Filter 退出当前请求线程；
4. 业务应用在异步线程中设置 `DeferredResult` 的返回值，Spring MVC 会再次发送请求；
5. `DispatcherServlet` 再次被调用，并使用 `DeferredResult` 的返回值；
<a name="xNWjE"></a>
### 使用 `Callable` 进行异步处理
使用 Callable 进行异步处理与 `DeferredResult` 类似。不同的是，Callable 会交给系统指定的 `TaskExecutor` 执行。<br />根据官方描述，Spring MVC 处理流程如下：

1. Controller 返回 Callable；
2. Spring MVC 调用 `request.startAsync()`，开启异步，提交 Callable 到一个任务线程池；
3. `DispatcherServlet` 和所有的 Filter 退出当前请求线程；
4. 业务应用在异步线程中返回值，Spring MVC 会再次发送请求；
5. `DispatcherServlet` 再次被调用，并使用 Callable 的返回值；
```java
@RequestMapping(value = "/async/demo")
public Callable<String> async(){
    Callable<String> callable = () -> String.valueOf(System.currentTimeMillis());
    // 同步返回
    return callable;
}
```
<a name="C1BsH"></a>
### 使用 `ListenableFuture` 进行异步处理
`ListenableFuture` 作为返回值，与 `DeferredResult` 类似。也需要使用者自行处理异步线程，但不支持超时、完成回调，需要自行处理。
```java
@RequestMapping(value = "/async/demo")
public ListenableFuture<String> async(){
    ListenableFutureTask<String> ListenableFuture= new ListenableFutureTask<>(() -> {
        return String.valueOf(System.currentTimeMillis());
    });
    Executors.newSingleThreadExecutor().submit(ListenableFuture);
    return ListenableFuture;
}
```
<a name="7vYxH"></a>
### 使用 `ResponseBodyEmitter` 进行异步处理
`DeferredResult` 和 `Callable` 都只能返回一个异步值。如果需要返回多个对象，就要使用 `ResponseBodyEmitter`。返回的每个对象都会被 `HttpMessageConverter` 处理并写回输出流。如果希望设置更多返回数据，如 header、status 等，可以把 `ResponseBodyEmitter` 作为 `ResponseEntity` 的实体数据返回。
```java
@RequestMapping("/async/responseBodyEmitter")
public ResponseBodyEmitter responseBodyEmitter(){
    ResponseBodyEmitter responseBodyEmitter=new ResponseBodyEmitter();
    Executors.newSingleThreadExecutor().submit(() -> {
        try {
            responseBodyEmitter.send("demo");
            responseBodyEmitter.send("test");
            responseBodyEmitter.complete();
        } catch (Exception ignore) {}
    });
    return responseBodyEmitter;
}
```
<a name="DTEFg"></a>
### 使用 `StreamingResponseBody` 进行异步处理
如果希望跳过返回值的自动转换，直接把输出流写入 `OutputStream`，可以使用 `StreamingResponseBody`。也可以作为 `ResponseEntity` 的实体数据返回。
```java
@RequestMapping("/async/streamingResponseBody")
public StreamingResponseBody streamingResponseBody(){
    StreamingResponseBody streamingResponseBody = outputStream -> {
        Executors.newSingleThreadExecutor().submit(() -> {
            try {
                outputStream.write("<html>streamingResponseBody</html>".getBytes());
            } catch (IOException ignore) {}
        });
    };
    return streamingResponseBody;
}
```
<a name="UDn2g"></a>
### 各种处理方式的对比
以上几种异步处理方式各有差异，需要按需取舍。对比如下。

|  | 可返回次数 | 数据转换 | 回调 | 线程池 |
| --- | --- | --- | --- | --- |
| `DeferredResult` | 1 次 | 有 | 完成、超时 | 自行处理 |
| `Callable` | 1 次 | 有 | 无 | 系统处理 |
| `ListenableFuture` | 1 次 | 有 | 无 | 自行处理 |
| `ResponseBodyEmitter` | 多次 | 有 | 无 | 自行处理 |
| `StreamingResponseBody` | 多次 | 无 | 无 | 自行处理 |


