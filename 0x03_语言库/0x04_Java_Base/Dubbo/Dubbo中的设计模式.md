Java Dubbo
<a name="VsG6f"></a>
## 介绍
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624167172328-182d6d68-8ca1-4f58-b323-4594a894979e.webp#clientId=u79c5510f-25e1-4&from=paste&id=u209f40c4&originHeight=467&originWidth=663&originalType=url&ratio=3&status=done&style=none&taskId=u3e494053-b500-45a6-9013-bf076578972)
<a name="BcMSi"></a>
## 策略模式
这个毫无争议，Dubbo是基于SPI来扩展的，SPI就是典型的策略模式。<br />Dubbo中可替换的组件太多了，例如负载均衡策略

| 实现类 | 解释 |
| --- | --- |
| `RandomLoadBalance` | 随机策略（默认） |
| `RoundRobinLoadBalance` | 轮询策略 |
| `LeastActiveLoadBalance` | 最少活跃调用数 |
| `ConsistentHashLoadBalance` | 一致性hash策略 |

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624167171352-6034a669-440b-4ec0-9312-146249627ae2.webp#clientId=u79c5510f-25e1-4&from=paste&id=u341a7a8b&originHeight=344&originWidth=1052&originalType=url&ratio=3&status=done&style=none&taskId=u17bf2498-ef38-431d-9e76-635d14f456a)
<a name="zEtMg"></a>
## 工厂模式
「简单工厂模式」：提供一个方法，返回创建好的对象
```java
public class VideoFactory {

    public static Video getVideo(String type) {
        if ("java".equalsIgnoreCase(type)) {
            return new JavaVideo();
        } else if ("python".equalsIgnoreCase(type)) {
            return new PythonVideo();
        }
        return null;
    }
}
```
「工厂方法模式」：当工厂想提供更多产品时，还得对创建过程进行修改，因此抽象出一个工厂类，当增加一种产品，就增加一个工厂类（继承抽象工厂类或实现接口）。这样就实现了对扩展开发，对修改关闭
```java
public abstract class VideoFactory {

    public abstract Video getVideo();
}

public class JavaVideoFactory extends VideoFactory {
    public Video getVideo() {
        return new JavaVideo();
    }
}

public class Test {

    public static void main(String[] args) {
        VideoFactory videoFactory = new JavaVideoFactory();
        Video video = videoFactory.getVideo();
        // 学习Java视频
        video.study();
    }
}
```
「抽象工厂模式」：当生产的产品较多时，如果用工厂方法模式会造成类爆照，此时就可以把相关的产品生产放到一个工厂类中
```java
public abstract class CourseFactory {

    public abstract Video getVideo();
    public abstract Article getArticle();
}

public class JavaCourseFactory extends CourseFactory {
    public Video getVideo() {
        return new JavaVideo();
    }

    public Article getArticle() {
        return new JavaArticle();
    }
}
```
因为JavaVideo和JavaArticle都是Java相关的资料，所以可以用一个工厂类来生产。如果用工厂方法模式来设计的话，JavaVideo和JavaArticle都会有一个对应的工厂类
<a name="DoHt8"></a>
### 简单工厂模式
```java
public class LoggerFactory {

    public static Logger getLogger(Class<?> key) {
        return LOGGERS.computeIfAbsent(key.getName(), name -> new FailsafeLogger(LOGGER_ADAPTER.getLogger(name)));
    }
}
```
<a name="WUZnY"></a>
### 工厂方法模式
Dubbo可以对结果进行缓存，缓存的策略有很多种，一种策略对应一个缓存工厂类
```java
@SPI("lru")
public interface CacheFactory {

    @Adaptive("cache")
    Cache getCache(URL url, Invocation invocation);

}
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624167172593-cb268d0c-7fc1-4d46-b212-095961ba18da.webp#clientId=u79c5510f-25e1-4&from=paste&id=u3392e9cb&originHeight=239&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u413dd1d8-3f19-4c79-99c6-ef3ba9536b5)
<a name="QHR25"></a>
### 抽象工厂模式
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624167172092-e586be5f-291b-44bc-ae2b-d16a7b72b40d.webp#clientId=u79c5510f-25e1-4&from=paste&id=u29acb86d&originHeight=261&originWidth=861&originalType=url&ratio=3&status=done&style=shadow&taskId=ua6efbf7f-e97c-4416-94df-605418ec8f0)在RPC框架中，客户端发送请求和服务端执行请求的过程都是由代理类来完成的。客户端的代理对象叫做Client Stub，服务端的代理对象叫做Server Stub。<br />在Dubbo中用了ProxyFactory来创建这2个相关的对象，有两种实现一种是基于jdk动态代理，一种是基于javaassist
```java
@SPI("javassist")
public interface ProxyFactory {

    // 针对consumer端，创建出代理对象
    @Adaptive({Constants.PROXY_KEY})
    <T> T getProxy(Invoker<T> invoker) throws RpcException;

    // 针对consumer端，创建出代理对象
    @Adaptive({Constants.PROXY_KEY})
    <T> T getProxy(Invoker<T> invoker, boolean generic) throws RpcException;

    // 针对provider端，将服务对象包装成一个Invoker对象
    @Adaptive({Constants.PROXY_KEY})
    <T> Invoker<T> getInvoker(T proxy, Class<T> type, URL url) throws RpcException;

}
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624167173569-241abbf0-9c63-46d9-bcd1-882c5c3598c5.webp#clientId=u79c5510f-25e1-4&from=paste&id=ueff7e790&originHeight=465&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u6d0f8522-4f89-4386-9b45-a45f0e60457)
<a name="jqEbs"></a>
## 单例模式
服务导出的过程中，为了防止开启多个NettyServer，用了单例模式
```java
private void openServer(URL url) {
    // find server.
    String key = url.getAddress();
    //client can export a service which's only for server to invoke
    boolean isServer = url.getParameter(Constants.IS_SERVER_KEY, true);
    if (isServer) {
        ExchangeServer server = serverMap.get(key);
        if (server == null) {
            synchronized (this) {
                server = serverMap.get(key);
                if (server == null) {
                    // 创建服务器实例
                    serverMap.put(key, createServer(url));
                }
            }
        } else {
            // server supports reset, use together with override
            server.reset(url);
        }
    }
}
```
<a name="BZtu5"></a>
## 装饰者模式
Dubbo中网络传输层用到了Netty，当用Netty开发时，一般都是写多个`ChannelHandler`，然后将这些`ChannelHandler`添加到`ChannelPipeline`上，就是典型的责任链模式<br />但是Dubbo考虑到有可能替换网络框架组件，所以整个请求发送和请求接收的过程全部用的都是装饰者模式。即只有`NettyServerHandler`实现的接口是Netty中的`ChannelHandler`，剩下的接口实现的是Dubbo中的`ChannelHandler`<br />如下是服务端消息接收会经过的`ChannelHandler`![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624167172106-aee6fcf9-d83d-443a-a321-7a5dc37f63bf.webp#clientId=u79c5510f-25e1-4&from=paste&id=u5bc150af&originHeight=651&originWidth=921&originalType=url&ratio=3&status=done&style=shadow&taskId=uf5425803-46a9-4648-922c-69325a6ba69)
<a name="dWM94"></a>
## 代理模式
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624167172530-e61e0dbb-9ab8-4415-921f-521d3f36584e.webp#clientId=u79c5510f-25e1-4&from=paste&id=uda1a8c64&originHeight=261&originWidth=861&originalType=url&ratio=3&status=done&style=shadow&taskId=u1be47613-95e7-4347-b72c-187a9ff9863)前面说过了，Client Stub和Server Stub都是代理对象
<a name="ynJvV"></a>
## 适配器模式
Dubbo可以支持多个日志框架，每个日志框架的实现都有对应的Adapter类，为什么要搞Adapter类呢，因为Dubbo中日志接口Logger用的是自己的，而实现类是引入的。但这些日志实现类的等级和Dubbo中定义的日志等级并不完全一致，例如JdkLogger中并没有trace和debug这个等级，所以要用Adapter类把Logger中的等级对应到实现类中的合适等级
```java
public interface Logger

    // 省略部分代码
    
    void trace(String msg);

    void debug(String msg);

    void info(String msg);

    void warn(String msg);

}
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624167173024-635afdb8-c94f-4c51-b841-5e8ab1a3f506.webp#clientId=u79c5510f-25e1-4&from=paste&id=ufbccb59e&originHeight=127&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u281f5a08-d636-4046-89ca-249cf950c3b)

| Dubbo接口中定义的日志等级 | JdkLogger对应的日志等级 | Slf4jLogger对应的日志等级 |
| --- | --- | --- |
| trace | finer | trace |
| debug | finer | debug |
| info | info | info |

<a name="p8kv3"></a>
## 观察者模式
在Dubbo中提供了各种注册中心的实现，类图如下。`AbstractRegistry`对注册中心的内容进行了缓存，这样能保证当注册中心不可用的时候，还能正常提供服务![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624167173128-7fe5d207-cd05-4b83-b34b-b34a26938756.webp#clientId=u79c5510f-25e1-4&from=paste&id=u785adfb0&originHeight=462&originWidth=954&originalType=url&ratio=3&status=done&style=none&taskId=u1dc07f2b-3302-4332-ab15-b1f72b3eecf)「既然对注册中心的内容进行了缓存，那么注册中心的内容发生改变的时候，怎么通知客户端呢？」<br />例如客户端从注册中心获取到服务端的地址，并缓存到本地，如果服务端宕机了，本地缓存怎么清除呢？此时就得需要对有可能变动的节点进行订阅。当节点发生变化的时候，就能收到通知，这样就能更新本地缓存。<br />`NotifyListener`就是接收节点变动的接口，各种注册中心的节点发生变化都会主动回调这个接口
```java
public interface RegistryService {

    // 注册
    void register(URL url);

    // 注销
    void unregister(URL url);

    // 订阅，订阅的数据发生变化，会主动通知NotifyListener#notify方法
    void subscribe(URL url, NotifyListener listener);

    // 退订
    void unsubscribe(URL url, NotifyListener listener);

    // 查找服务地址
    List<URL> lookup(URL url);

}
```
<a name="ESImT"></a>
## 责任链模式
代理对象（Client Stub或者Server Stub）在执行的过程中会执行所有`Filter`的`invoke`方法，但是这个实现方法是对对象不断进行包装，看起来非常像装饰者模式，但是基于方法名和这个`Filter`的功能，更觉得这个是责任链模式
```java
private static <T> Invoker<T> buildInvokerChain(final Invoker<T> invoker, String key, String group) {
    Invoker<T> last = invoker;
    // 获取自动激活的扩展类
    List<Filter> filters = ExtensionLoader.getExtensionLoader(Filter.class).getActivateExtension(invoker.getUrl(), key, group);
    if (!filters.isEmpty()) {
        for (int i = filters.size() - 1; i >= 0; i--) {
            final Filter filter = filters.get(i);
            final Invoker<T> next = last;
            last = new Invoker<T>() {

                // 省略部分代码

                @Override
                public Result invoke(Invocation invocation) throws RpcException {
                    // filter 不断的套在 Invoker 上，调用invoke方法的时候就会执行filter的invoke方法
                    Result result = filter.invoke(next, invocation);
                    if (result instanceof AsyncRpcResult) {
                        AsyncRpcResult asyncResult = (AsyncRpcResult) result;
                        asyncResult.thenApplyWithContext(r -> filter.onResponse(r, invoker, invocation));
                        return asyncResult;
                    } else {
                        return filter.onResponse(result, invoker, invocation);
                    }
                }

            };
        }
    }
    return last;
}
```
