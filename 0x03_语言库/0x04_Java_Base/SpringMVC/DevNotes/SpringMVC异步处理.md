JavaSpringMVC
<a name="KapaK"></a>
## 看段代码，分析问题
```java
@ResponseBody
@RequestMapping("/async/m1.do")
public String m1() throws InterruptedException {
    long st = System.currentTimeMillis();
    System.out.println("主线程：" + Thread.currentThread() + "," + st + ",开始");
    //休眠3秒，模拟耗时的业务操作
    TimeUnit.SECONDS.sleep(3);
    long et = System.currentTimeMillis();
    System.out.println("主线程：" + Thread.currentThread() + "," + st + ",结束,耗时(ms):" + (et - st));
    return "ok";
}
```
这段代码很简单

- 这段代码是 SpringMVC 提供的一个接口
- 内部休眠了 3 秒钟，用来模拟耗时的操作
- 方法内部有 2 条日志（日志中包含了当前线程、开始时间、结束时间、耗时）

浏览器中访问下这个接口，效果如下，可以看到接口耗时 3s 左右。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683905701116-c85e2b7d-7533-49ac-a7ba-92fb52ba3eec.png#averageHue=%23e6c99a&clientId=u19798bbc-2c85-4&from=paste&id=ue265fb01&originHeight=636&originWidth=930&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u24b29543-4235-4f1d-91f4-7dd4a12f765&title=)<br />控制台输出
```
主线程：Thread[http-nio-8080-exec-1,5,main],1624889293055,开始
主线程：Thread[http-nio-8080-exec-1,5,main],1624889293055,结束,耗时(ms):3002
```
从输出中，可以看出，这个接口从开始到结束都是由 tomcat 中的线程来处理用户请求的，也就是说，3 秒这段时间内，tomcat 中的一个线程会被当前请求一直占用了则，tomcat 线程是有最大值的，默认情况下好像是 75，那么问题来了。<br />当 3 秒之内，来的请求数量超过了 tomcat 最大线程数的时候，其他请求就无法处理了，而此时 tomcat 中这些线程都处理 sleep 3s 的休眠状态，cpu 此时没活干，此时就会造成机器没活干，但是呢又不能处理新的请求，这就是坑，浪费资源，怎么办呢？<br />遇到这种场景的，也就是说接口内部比价耗时，但是又不能充分利用 cpu 的，可以采用异步的方式来处理请求，过程如下：<br />tomcat 线程，将请求转发给自定义的子线程去处理这个请求，然后 tomcat 就可以继续去接受新的请求了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683905701122-29305b86-2b80-4405-9d52-acca8224ce8b.png#averageHue=%23f8f8f8&clientId=u19798bbc-2c85-4&from=paste&id=u9fb1886c&originHeight=698&originWidth=639&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u950dc783-efec-4eb4-8358-bc5cdb030a2&title=)
<a name="pwONl"></a>
## SpringMVC 中异步处理
主要有 3 个大的步骤。
<a name="XJNw1"></a>
### step1：servlet 开启异步处理支持
web.xml 中开启 servlet 异步支持<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683905701197-34d87ee1-dfd4-43b3-a542-d8ea14025a4c.png#averageHue=%23fcfbf9&clientId=u19798bbc-2c85-4&from=paste&id=u6474c220&originHeight=369&originWidth=753&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u348d8972-c3d5-460b-ae6a-a4790d48538&title=)
<a name="V38Bs"></a>
### step2：Filter 中添加异步支持
如果异步请求需要经过 Filter 的，那么需要在 web.xml 对这个 Filter 添加异步支持。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683905701232-5b1ec313-19ef-476d-9266-d5bec35bf661.png#averageHue=%23fcfbf9&clientId=u19798bbc-2c85-4&from=paste&id=u764368bb&originHeight=317&originWidth=740&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9658a7cb-335c-48a2-ab7a-bf7caa5f6e4&title=)
<a name="vBJEx"></a>
### step3：接口返回值为 `DeferredResult`
这个步骤中细节比较多，当需要异步响应请求的时候，返回值需要为 `DeferredResult`，具体参考下面案例代码，详细信息都在注释中了，大家注意看注释。

- 第 1 步：创建 `DeferredResult<返回值类型>(超时时间[毫秒],超时回调的代码)`
- 第 2 步：在子线程中异步处理业务，调用 DeferredResult 的 setResult 方法，设置最终返回到客户端的结果，此方法调用以后，客户端将接收到返回值，然后响应过程请求就结束了
- 第 3 步：将 `DefaultResult` 作为方法返回值
```java
/**
 * 使用springmvc的异步功能，业务处理放在异步线程中执行
 *
 * @param timeout 异步处理超时时间（毫秒）
 * @return
 */
@ResponseBody
@RequestMapping("/async/m2/{timeout}.do")
public DeferredResult m2(@PathVariable("timeout") long timeout) {
    long st = System.currentTimeMillis();
    System.out.println("主线程：" + Thread.currentThread() + "," + st + ",开始");
    /**
     * 1、创建DeferredResult<返回值类型>(超时时间[毫秒],超时回调的代码)
     */
    DeferredResult result = new DeferredResult(timeout, () -> {
        System.out.println("超时了");
        return "timeout";
    });
    //2、异步处理业务，
    new Thread(() -> {
        //开启一个异步线程，在异步线程中进行业务处理操作
        try {
            TimeUnit.SECONDS.sleep(3);
            //3、调用DeferredResult的setResult方法，设置最终返回到客户端的结果，此方法调用以后，客户端将接收到返回值
            result.setResult("ok");
        } catch (InterruptedException e) {
            result.setResult("发生异常了:" + e.getMessage());
        }
    }).start();
    long et = System.currentTimeMillis();
    System.out.println("主线程：" + Thread.currentThread() + "," + st + ",结束,耗时(ms):" + (et - st));
    //3、将DefaultResult作为方法返回值
    return result;
}
```
上面的 m2 方法个 timeout 参数，调用者通过这个参数来指定接口的超时时间，未超时的情况下，也就是说 timeout 大于 3 秒的时候，此时会输出 ok，否则将出现超时，此时会将 `DeferredResult` 构造器第 2 个参数的执行结果作为最终的响应结果，即会向客户端输出 timeout。<br />使用建议：案例开启了一个新的子线程来执行业务操作，生产环境中，建议大家采用线程池的方式，效率更高。<br />下面来通过 2 个 case 来模拟下这个接口超时和正常的结果。
<a name="hSJd7"></a>
## 模拟非超时请求
当 timeout 大于 3 秒时，才不会出现超时，此时传递 4000 毫秒来试试<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683905701193-1b6a6e2d-be34-41bc-92bf-88919c23615c.png#averageHue=%23edd4a6&clientId=u19798bbc-2c85-4&from=paste&id=u9c3b0a54&originHeight=636&originWidth=803&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue42976fb-2345-466c-8a8c-25014e91d75&title=)<br />控制台输出如下，可以看到主线程瞬间就结束了。
```
主线程：Thread[http-nio-8080-exec-6,5,main],1624891886020,开始
主线程：Thread[http-nio-8080-exec-6,5,main],1624891886020,结束,耗时(ms):0
```
<a name="yq0Kf"></a>
## 模拟超时请求
当 timeout 小于 3 秒会出现超时，此时传递 1000 毫秒来试试<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683905701481-2a332171-4cd0-47b8-9ffb-293443796b95.png#averageHue=%23e5c796&clientId=u19798bbc-2c85-4&from=paste&id=ua34aed74&originHeight=636&originWidth=803&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud3fc716f-4be2-4133-9ef8-71347e5cc5e&title=)<br />控制台输出如下，输出了超时信息，且通过前两行输出看出主线程瞬间就结束了，不会被请求阻塞。
```
主线程：Thread[http-nio-8080-exec-1,5,main],1624892109695,开始
主线程：Thread[http-nio-8080-exec-1,5,main],1624892109695,结束,耗时(ms):0
超时了
```
<a name="cKI68"></a>
## 总结
当接口中有大量的耗时的操作，且这些耗时的操作让线程处于等待状态时，此时为了提升系统的性能，可以将接口调整为异步处理的方式。
