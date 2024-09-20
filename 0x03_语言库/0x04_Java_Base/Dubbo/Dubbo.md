Java Dubbo
<a name="KmqcX"></a>
## RPC框架的实现
从源码的角度来了解一下Dubbo的整个调用过程（图示为主，辅助一少部分源码）<br />**「RPC框架的实现基本上都是如下架构」**![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644453795323-3f8ed269-5f35-4db2-bf3a-7a8601525cc8.webp#clientId=u831c67bc-06e4-4&from=paste&id=u2b390fa0&originHeight=261&originWidth=861&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u64c32730-1130-40f5-add6-c6bae5e4b84&title=)一个RPC调用的过程如下

1. 调用方发送请求后由代理类将调用的方法，参数组装成能进行网络传输的消息体
2. 调用方代理类将消息体发送到提供方
3. 提供方代理类将消息进行解码，得到调用的方法和参数
4. 提供方代理类执行相应的方法，并将结果返回

**「协议，编解码，序列化的部分不是本文的重点，就不分析了。」**<br />首先来手写一个极简版的RPC框架，以便对上面的流程有一个更深的认识
<a name="D1b4q"></a>
## 手写一个简单的PRC框架
<a name="nhR6u"></a>
### 封装网络请求对象
```java
@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class RpcRequest implements Serializable {

    private String interfaceName;
    private String methodName;
    private Class<?>[] paramTypes;
    private Object[] parameters;
}
```
根据interfaceName可以确定需要调用的接口，methodName和paramTypes则可以确定要调用接口的方法名，定位到具体的方法，传入参数即可调用方法
<a name="GDtBT"></a>
### 封装调用接口
封装接口到api模块，producer端写实现逻辑，consumer端写调用逻辑
```java
public interface HelloService {

    String sayHello(String content);
}

public interface UpperCaseService {

    String toUpperCase(String content);
}
```
<a name="E9NF2"></a>
### 开发producer端
```java
public class HelloServiceImpl implements HelloService {

    @Override
    public String sayHello(String content) {
        return "hello " + content;
    }
}

public class UpperCaseServiceImpl implements UpperCaseService {

    @Override
    public String toUpperCase(String content) {
        return content.toUpperCase();
    }
}
```
ServiceMap保存了producer端接口名和接口实现类的映射关系，这样可以根据请求对象的接口名，找到对应的实现类
```java
public class ServiceMap {

    // 接口名 -> 接口实现类
    private static Map<String, Object> serviceMap = new HashMap<>();

    public static void registerService(String serviceKey, Object service) {
        serviceMap.put(serviceKey, service);
    }

    public static Object lookupService(String serviceKey) {
        return serviceMap.get(serviceKey);
    }
}
```
为了提高服务端的并发度，将每一个请求的处理过程放到线程池中
```java
@Slf4j
public class RequestHandler implements Runnable {

    private Socket socket;

    public RequestHandler(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try (ObjectInputStream inputStream = new ObjectInputStream(socket.getInputStream());
             ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream())) {
            RpcRequest rpcRequest = (RpcRequest) inputStream.readObject();
            Object service = ServiceMap.lookupService(rpcRequest.getInterfaceName());
            Method method = service.getClass().getMethod(rpcRequest.getMethodName(), rpcRequest.getParamTypes());
            Object result = method.invoke(service, rpcRequest.getParameters());
            outputStream.writeObject(result);
        } catch (Exception e) {
            log.error("invoke method error", e);
            throw new RuntimeException("invoke method error");
        }
    }

}
```
启动服务端
```java
public class RpcProviderMain {

    private static final ExecutorService executorService = Executors.newCachedThreadPool();

    public static void main(String[] args) throws Exception {

        HelloService helloService = new HelloServiceImpl();
        UpperCaseService upperCaseService = new UpperCaseServiceImpl();
        // 将需要暴露的接口注册到serviceMap中
        ServiceMap.registerService(HelloService.class.getName(), helloService);
        ServiceMap.registerService(UpperCaseService.class.getName(), upperCaseService);

        ServerSocket serverSocket = new ServerSocket(8080);

        while (true) {
            // 获取一个套接字（阻塞）。所以为了并行，来一个请求，开一个线程处理
            // 为了复用线程，用了threadPool
            final Socket socket = serverSocket.accept();
            executorService.execute(new RequestHandler(socket));
        }
    }
}
```
<a name="XvBel"></a>
### 开发consumer端
要通过动态代理对象解耦方法调用和网络调用，所以接下来就写一下动态代理对象的实现逻辑<br />生成一个代理对象的过程很简单

1. 实现`InvocationHandler`接口，在invoke方法中增加代理逻辑
2. 调用`Proxy.newProxyInstance`方法生成代理对象，3个参数分别是`ClassLoader`，代理对象需要实现的接口数组，`InvocationHandler`接口实现类
3. 当执行代理执行实现的接口方法时，会调用到`InvocationHandler#invoke`，这个方法中增加了代理逻辑哈。
```java
public class ConsumerProxy {

    public static <T> T getProxy(final Class<T> interfaceClass, final String host, final int port) {

        return (T) Proxy.newProxyInstance(interfaceClass.getClassLoader(),
                new Class<?>[]{interfaceClass}, new ConsumerInvocationHandler(host, port));
    }
}
```
可以看到代理对象的主要功能就是组装请求参数，然后发起网络调用
```java
@Slf4j
public class ConsumerInvocationHandler implements InvocationHandler {

    private String host;
    private Integer port;

    public ConsumerInvocationHandler(String host, Integer port) {
        this.host = host;
        this.port = port;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {

        try (Socket socket = new Socket(host, port);
             ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream());
             ObjectInputStream inputStream = new ObjectInputStream(socket.getInputStream())) {
            RpcRequest rpcRequest = RpcRequest.builder()
                    .interfaceName(method.getDeclaringClass().getName())
                    .methodName(method.getName())
                    .paramTypes(method.getParameterTypes())
                    .parameters(args).build();
            outputStream.writeObject(rpcRequest);
            Object result = inputStream.readObject();
            return result;
        } catch (Exception e) {
            log.error("consumer invoke error", e);
            throw new RuntimeException("consumer invoke error");
        }
    }
}
```
此时只需要通过`ConsumerProxy#getProxy`方法，就能很方便的获取到代理对象。通过代理对象调用远程方法和调用本地方法一样方便
```java
public class RpcConsumerMain {

    public static void main(String[] args)  {

        // 因为这是一个小demo，就不拆分多模块了
        // 这个HelloService是通过网络调用的HelloServiceImpl，而不是本地调用
        HelloService helloService = ConsumerProxy.getProxy(HelloService.class, "127.0.0.1", 8080);
        // hello world
        System.out.println(helloService.sayHello("world"));
        UpperCaseService upperCaseService = ConsumerProxy.getProxy(UpperCaseService.class, "127.0.0.1", 8080);
        // THIS IS CONTENT
        System.out.println(upperCaseService.toUpperCase("this is content"));
    }
}
```
至此已经把一个RPC框架最核心的功能就实现了，是不是很简单。**「其实Dubbo的源码也很简单，只不过增加了很多扩展功能，所以大家有时候会认为比较难。」**<br />所以就来分析一下核心的扩展功能。比如Filter，服务降级，集群容错等是如何实现的？其他的扩展功能，比如支持多种注册中心，支持多种序列化框架，支持多种协议，基本不会打交道，所以就不浪费时间了<br />从前面的图示可以知道，代理类在服务调用和响应过程中扮演着重要的角色。**「在Dubbo中，代理类有个专有名词叫做Invoker，而Dubbo中就是通过对这个Invoker不断进行代理增加各种新功能的」**
<a name="pvj6Q"></a>
## Dubbo服务导出
**「当第三方框架想和Spring整合时，有哪些方式？」**

1. 实现`BeanFactoryPostProcessor`接口（对`BeanFactory`进行扩展）
2. 实现`BeanPostProcessor`接口（对Bean的生成过程进行扩展）

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644453795394-134fc9f2-27e9-44a1-a32f-383f0bd5cbf7.webp#clientId=u831c67bc-06e4-4&from=paste&id=u3c5e4110&originHeight=266&originWidth=291&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud2b768d4-0731-4a28-b5d3-afd05355983&title=)<br />Dubbo也不例外，当Dubbo和Spring整合时，会往容器中注入2个`BeanPostProcessor`，作用如下<br />`ServiceAnnotationBeanPostProcessor`，将`@Service`注解的类封装成`ServiceBean`注入容器 `ReferenceAnnotationBeanPostProcessor`，将`@Reference`注解的接口封装成`ReferenceBean`注入容器![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644453795502-c02eac10-2330-4a42-933e-a0b2e74cac40.webp#clientId=u831c67bc-06e4-4&from=paste&id=ude9a1033&originHeight=238&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uab307faf-57da-4490-9bc8-fadc613400a&title=)<br />所以服务导出和服务引入肯定和`ServiceBean`和`ReferenceBean`的生命周期有关。![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644453795339-b5e5704b-1c2c-4cb5-93a2-256e5e692c71.webp#clientId=u831c67bc-06e4-4&from=paste&id=uce84c7c1&originHeight=478&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue3ae77e1-df10-42df-9ddb-4f543e42efa&title=)**「ServiceBean实现了**`**ApplicationListener**`**接口，当收到**`**ContextRefreshedEvent**`**事件时（即Spring容器启动完成）开始服务导出。」**<br />服务导出比较重要的2个步骤就是

1. 将服务注册到zk（后面的分析，注册中心都基于zk）
2. 将服务对象包装成Invoker，并保存在一个map中，key为服务名，value为Invoker对象

**「当收到请求时，根据服务名找到Invoker对象，Invoker对象根据方法名和参数反射执行方法，然后将结果返回。」**<br />这里留个小问题，反射执行方式效率会很低，那么在Dubbo中还有哪些解决方案呢？<br />从图中可以看到`AbstractProxyInvoker`被其他`Invoker`进行代理了，而这些`Invoker`是用来执行`Filter`的，一个Invoker代理类执行一个`Filter`，层层进行代理<br />**「如下图为Dubbo收到请求层层调用的过程」**![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644453795336-f36ec478-9afa-47d3-8e44-8dcc84be0bdd.webp#clientId=u831c67bc-06e4-4&from=paste&id=u238bf435&originHeight=474&originWidth=696&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udd815fcf-6538-4a0d-b566-d79455a3b67&title=)
<a name="CiraG"></a>
## Dubbo服务引入
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644453795900-0a21c9be-aa39-4b41-b591-1639a7b8c091.webp#clientId=u831c67bc-06e4-4&from=paste&id=ua8b92710&originHeight=379&originWidth=991&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6f2faa3f-78cc-41a6-965e-0f2e6bbd30a&title=)前面已经推断出来服务导出和`ReferenceBean`有关。来看看具体在哪个阶段？`ReferenceBean`实现了`FactoryBean`接口，并重写了`getObject`方法，在这个方法中进行服务导出。因此推断服务导出的时机是`ReferenceBean`被其他对象注入时
```java
public Object getObject() {
    return get();
}
```
接下来就是从注册中心获取服务地址，构建Invoker对象，并基于Invoker对象构建动态代理类，赋值给接口。<br />最终能发起网络调用的是DubboInvoker，而这个Invoker被代理了很多层，用来实现各种扩展功能。
<a name="oHbIo"></a>
### 服务降级
第一个就是服务降级，什么是服务降级呢？<br />**「当服务可不用时，不希望抛出异常，而是返回特定的值（友好的提示等），这时候就可以用到服务降级。」**<br />dubbo中有很多服务降级策略，简单举几个例子<br />force: 代表强制使用 Mock 行为，在这种情况下不会走远程调用 fail: 只有当远程调用发生错误时才使用 Mock 行为<br />假如有如下一个controller，调用DemoService获取值，但是DemoService并没有启动
```java
@RestController
public class DemoController {

    @Reference(check = false, mock = "force:return mock")
    private DemoService demoService;

    @RequestMapping("hello")
    public String hello(@RequestParam("msg") String msg) {
        return demoService.hello(msg);
    }

}
```
可以看到直接返回mock字符串（也并不会发生网络调用）<br />将`@Reference`的mock属性改为如下，再次调用
```java
@RestController
public class DemoController {

    @Reference(check = false, mock = "fail:return fail")
    private DemoService demoService;

    @RequestMapping("hello")
    public String hello(@RequestParam("msg") String msg) {
        return demoService.hello(msg);
    }

}
```
会发起网络调用，调用失败，然后返回fail。<br />**「dubbo中的服务降级只用了MockClusterInvoker这一个类来实现，因此相对于Hystrix等功能很简单，实现也很简单，如下图。」**![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644453795931-dab3d106-ecce-4efc-9bae-a792b4561b0e.webp#clientId=u831c67bc-06e4-4&from=paste&id=u91fffacc&originHeight=516&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u46926774-8840-4e68-aa7c-cdba17b87b5&title=)

1. 当Reference不配置mock属性或者属性为false时，表示不进行降级，直接调用代理对象即可
2. 以属性以force开头时，表示直接进行降级，都不会发生网络调用
3. 其他请求就是在进行网络失败后才进行降级
<a name="T29yh"></a>
### 集群容错
过了服务降级这一层，接下来就到了集群容错了。![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644453796156-c143d4a4-7dd7-492f-b264-3ab530440f61.webp#clientId=u831c67bc-06e4-4&from=paste&id=u5bb18b24&originHeight=469&originWidth=953&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc2daa667-7ea7-400b-a8c6-ac599f66ec4&title=)<br />dubbo中有很多集群容错策略

| 容错策略 | 解释 | 代理类 |
| --- | --- | --- |
| AvailableCluster | 找到一个可用的节点，直接发起调用 | AbstractClusterInvoker匿名内部类 |
| FailoverCluster | 失败重试（默认） | FailoverClusterInvoker |
| FailfastCluster | 快速失败 | FailfastClusterInvoker |
| FailsafeCluster | 安全失败 | FailsafeClusterInvoker |
| FailbackCluster | 失败自动恢复 | FailbackClusterInvoker |
| ForkingCluster | 并行调用 | ForkingClusterInvoker |
| BroadcastCluster | 广播调用 | BroadcastClusterInvoker |

Failover Cluster：失败自动切换，当出现失败，重试其它服务器。通常用于读操作，但重试会带来更长延迟。<br />Failfast Cluster：快速失败，只发起一次调用，失败立即报错。通常用于非幂等性的写操作，比如新增记录。<br />Failsafe Cluster：失败安全，出现异常时，直接忽略。通常用于写入审计日志等操作。<br />Failback Cluster：失败自动恢复，后台记录失败请求，定时重发。通常用于消息通知操作。<br />Forking Cluster：并行调用多个服务器，只要一个成功即返回。通常用于实时性要求较高的读操作，但需要浪费更多服务资源。可通过 forks=”2″ 来设置最大并行数。<br />Broadcast Cluster：广播调用所有提供者，逐个调用，任意一台报错则报错 。通常用于通知所有提供者更新缓存或日志等本地资源信息。<br />**「读操作建议使用 Failover 失败自动切换，默认重试两次其他服务器。写操作建议使用 Failfast 快速失败，发一次调用失败就立即报错。」**<br />有没有发现，**「换集群容错策略就是换**`**DubboInvoker**`**的代理类」**<br />集群容错相关的代理类都有一个共同的属性`RegistryDirectory`，这个是一个很重要的组件，它用List保存了服务提供者对应的所有Invoker。<br />**「更牛逼的是这个List是动态变化的，当服务提供者下线时，会触发相应的事件，调用方会监听这个事件，并把对应的Invoker删除，这样后续就不会调用到下线的服务了。当有新的服务提供者时，会触发生成新的**`**Invoker**`**。」**<br />当一个服务的多个Invoker，该选择哪个来调用呢？这就不得不提到负载均衡策略了。

| 负载均衡策略实现类 | 解释 |
| --- | --- |
| RandomLoadBalance | 随机策略（默认） |
| RoundRobinLoadBalance | 轮询策略 |
| LeastActiveLoadBalance | 最少活跃调用数 |
| ConsistentHashLoadBalance | 一致性hash策略 |

**「只需要通过合适的负载均衡策略来选择即可」**<br />和服务端类似类似，最终能发送网络请求的Invoker还会被Filter对应的Invoker类所代理，一个Filter一个代理类，层层代理。<br />如下图为Dubbo发送请求时层层调用的过程![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644453795861-916d223c-ce15-42b0-8a74-5bbeb8cc71c3.webp#clientId=u831c67bc-06e4-4&from=paste&id=u86c6e4ec&originHeight=307&originWidth=769&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5b114503-f37e-438c-b8b5-5c62c4af047&title=)Dubbo一些比较重要的扩展点就分享完了，整个请求响应的基本过程也串下来了！
