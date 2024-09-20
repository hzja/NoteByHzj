JavaCompletableFuture
<a name="a6Yi9"></a>
## 自定义线程池
```java
@Configuration
public class ThreadPoolConfig {

    public static ThreadPoolExecutor getThreadPoolExecutor() {
        int availableProcessors = Runtime.getRuntime().availableProcessors();
        return new ThreadPoolExecutor(
            availableProcessors,
            availableProcessors,
            0L,
            TimeUnit.MILLISECONDS,
            new LinkedBlockingQueue<>(9999),
            new ThreadFactoryBuilder().setNameFormat("custom-thread-pool-%d").build(),
            new ThreadPoolExecutor.CallerRunsPolicy());
    }
}
```
<a name="RiDLz"></a>
## 程序存在异常，却返回成功
写一个存在异常的程序，让其异步执行
```java
public static final ThreadPoolExecutor CUSTOM_THREAD_POOL = ThreadPoolConfig.getThreadPoolExecutor();

/**
 * 异步执行异常测试
 */
@ApiOperation(value = "异步执行异常测试", code = 800)
@GetMapping("/asyncException")
public ResponseData<Object> asyncException() {
    try {
        try {
            CompletableFuture.runAsync(() -> {
                int i = 1 / 0;
            }, CUSTOM_THREAD_POOL);
        } catch (Exception e) {
            log.error("异常信息: " + e.getMessage(), e);
            throw new BusinessException(e.getMessage());
        }
        return new ResponseData<>(StatusCodeEnum.SUCCESS_CODE.getStatusCode(), "操作成功");
    } catch (Exception e) {
        return new ResponseData<>(StatusCodeEnum.ERROR_CODE.getStatusCode(), "操作失败:" + e.getMessage());
    }
}
```
结果：接口返回成功，控制台没有打印错误信息。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687617633498-a7f4d048-ffb9-4913-8e27-6540cfd9c770.png#averageHue=%23faf9f8&clientId=uabe5defa-710a-4&from=paste&id=ue43f167d&originHeight=646&originWidth=1374&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uae638165-2280-43d0-b11f-765065d1ba4&title=)
<a name="PFZu3"></a>
## 异步调用`join()`
```java
// join方法获取异常信息: 将异步线程中发生的异常信息抛到主线程, 这样异常可被主线程捕获
try {
    CompletableFuture.runAsync(() -> {
        int i = 1 / 0;
    }, CUSTOM_THREAD_POOL).join();
} catch (Exception e) {
    log.error("外层异常信息: " + e.getMessage(), e);
    throw new BusinessException(e.getMessage());
}
```
结果：接口返回失败，控制台打印异常日志。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687617633533-c30d37c5-055d-4378-b21a-c6442df4b637.png#averageHue=%23faf8f7&clientId=uabe5defa-710a-4&from=paste&id=u6df5b6e1&originHeight=764&originWidth=1340&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uff753c03-fa79-470b-aa9d-7738cdc33ff&title=)
<a name="nRPoG"></a>
## 异步调用`get()`
异步方法中`get()`是阻塞的，在使用时要设置超时时间。
```java
// get方法获取异常信息: 将异步线程中发生的异常信息抛到主线程, 这样异常可被主线程捕获
try {
    CompletableFuture.runAsync(() -> {
        int i = 1 / 0;
    }, CUSTOM_THREAD_POOL).get(2, TimeUnit.SECONDS);
} catch (Exception e) {
    log.error("外层异常信息: " + e.getMessage(), e);
    throw new BusinessException(e.getMessage());
}
```
结果：接口返回成功，控制台打印异常信息。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687617633545-c8336ad1-84ca-4308-b440-46ff31f56998.png#averageHue=%23faf9f7&clientId=uabe5defa-710a-4&from=paste&id=u059d12a0&originHeight=791&originWidth=1379&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7127b853-e58e-4b24-af08-e651a392f2a&title=)
<a name="DTsc4"></a>
## 异步调用`exception()`
```java
// exceptionally获取异常信息: 异常是存在于异步当中的, 不能被主线程捕获
try {
    CompletableFuture.runAsync(() -> {
        int i = 1 / 0;
    }, CUSTOM_THREAD_POOL)
    .exceptionally(e -> {
        log.error("异步运行异常信息: " + e.getMessage(), e);
        throw new BusinessException(e.getMessage());
    });
} catch (Exception e) {
    log.error("异常信息: " + e.getMessage(), e);
    throw new BusinessException(e.getMessage());
}
```
结果：接口返回成功，控制台打印异步线程异常日志，主线程没有打印异常日志<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687617633585-6906b505-9c9b-481b-911d-31a16f7b7c04.png#averageHue=%23faf8f7&clientId=uabe5defa-710a-4&from=paste&id=u23b161a6&originHeight=791&originWidth=1314&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6d01623f-c7ff-4123-8501-9beb64db26e&title=)
<a name="f2yNd"></a>
## 异步调用`whenComplete()`
```java
// whenComplete获取异常信息: 异常是存在于异步当中的, 不能被主线程捕获
try {
    CompletableFuture.runAsync(() -> {
        int i = 1 / 0;
    }, CUSTOM_THREAD_POOL)
    .whenComplete((r, e) -> {
        if (e != null) {
            log.error("异步执行异常信息: " + e.getMessage(), e);
            throw new BusinessException(e.getMessage());
        }
    });
} catch (Exception e) {
    log.error("异常信息: " + e.getMessage(), e);
    throw new BusinessException(e.getMessage());
}
```
结果：结果返回成功，控制台打印异步线程异常信息，主线程没有打印异常信息<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687617633514-43ab0d9e-dcd2-49c8-93b1-1019233eae97.png#averageHue=%23faf9f7&clientId=uabe5defa-710a-4&from=paste&id=u58fa77fe&originHeight=755&originWidth=1307&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5a234106-9737-49fe-bdf2-8143c3c0cc7&title=)
<a name="ZQfwc"></a>
## 异步调用`handle()`
```java
// handle获取异常信息: 异常是存在于异步当中的, 不能被主线程捕获
try {
    CompletableFuture.runAsync(() -> {
        int i = 1 / 0;
    }, CUSTOM_THREAD_POOL)
    .handle((r, e) -> {
        if (e != null) {
            log.error("异步执行异常信息: " + e.getMessage(), e);
            throw new BusinessException(e.getMessage());
        }
        return null;
    });
} catch (Exception e) {
    log.error("异常信息: " + e.getMessage(), e);
    throw new BusinessException(e.getMessage());
}
```
结果：结果返回成功，控制台打印异步线程异常信息，主线程没有打印异常信息<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687617633951-becfba7e-4b30-4946-bca2-212267a7ff48.png#averageHue=%23faf9f7&clientId=uabe5defa-710a-4&from=paste&id=ue224b85a&originHeight=790&originWidth=1309&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3142cc34-20f0-4b05-abd7-d26e14801d2&title=)
<a name="imkgc"></a>
## 程序发生异常时需要做处理，可以调用`get()`/`join()`
```java
try {
    CompletableFuture.runAsync(() -> {
        int i = 1 / 0;
    }, CUSTOM_THREAD_POOL)
    .exceptionally(e -> {
        log.error("异步执行异常信息: " + e.getMessage(), e);
        throw new BusinessException(e.getMessage());
    }).join();
} catch (Exception e) {
    log.error("异常信息: " + e.getMessage(), e);
    throw new BusinessException(e.getMessage());
}

try {
    CompletableFuture.runAsync(() -> {
        int i = 1 / 0;
    }, CUSTOM_THREAD_POOL)
    .exceptionally(e -> {
        log.error("异步执行异常信息: " + e.getMessage(), e);
        throw new BusinessException(e.getMessage());
    }).get(2, TimeUnit.SECONDS);
} catch (Exception e) {
    log.error("异常信息: " + e.getMessage(), e);
    throw new BusinessException(e.getMessage());
}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687617634098-8d349217-c0e7-49c9-8837-405e07b31224.png#averageHue=%23f9f6f4&clientId=uabe5defa-710a-4&from=paste&id=u4d66af0c&originHeight=859&originWidth=1371&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9e7087fb-5dcb-4a05-9673-2fb73759fca&title=)
<a name="rM1vH"></a>
## 程序发生异常时不做处理直接报错，直接调用`get()`/`join()`
直接的异步方法后调用`get()`/`join()`。
<a name="O6YuI"></a>
## 总结
在使用异步`CompletableFuture`时，无论是否有返回值都要调用`get()`/`join()`方法，避免程序执行报错了，仍然返回成功。如果在程序报错时需要对上一个异步任务结果做其他操作，可以调用`whenComplete()`、`handle()`处理，如果只是对异常做处理，不涉及对上一个异步任务结果的情况，调用`exceptionally()`处理。
