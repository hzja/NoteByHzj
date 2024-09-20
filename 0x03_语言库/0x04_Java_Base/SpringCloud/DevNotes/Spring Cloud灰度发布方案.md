SpringCloud
<a name="GpddF"></a>
## 前言
在平时的业务开发过程中，后端服务与服务之间的调用往往通过fegin或者RestTemplate两种方式。但是在调用服务的时候往往只需要写服务名就可以做到路由到具体的服务，这其中的原理想必大家都知道是SpringCloud的ribbon组件做了负载均衡的功能。<br />灰度发布的核心就是路由，如果能够重写ribbon默认的负载均衡算法是不是就意味着能够控制服务的转发呢？<br />是的！
<a name="uMoGb"></a>
## 调用链分析
<a name="DHxMz"></a>
### 外部调用

- 请求==>zuul==>服务

zuul在转发请求的时候，也会根据 Ribbon从服务实例列表中选择一个对应的服务，然后选择转发。
<a name="D5FBa"></a>
### 内部调用

- 请求==>zuul==>服务Resttemplate调用==>服务
- 请求==>zuul==>服务Fegin调用==>服务

无论是通过 Resttemplate还是 Fegin的方式进行服务间的调用，他们都会从 Ribbon选择一个服务实例返回。<br />上面几种调用方式应该涵盖了平时调用中的场景，无论是通过哪种方式调用(排除直接ip:port调用)，最后都会通过Ribbon，然后返回服务实例。
<a name="g15Vx"></a>
## 预备知识
<a name="jFo4K"></a>
### eureka元数据
Eureka的元数据有两种，分别为标准元数据和自定义元数据。<br />标准元数据：主机名、IP地址、端口号、状态页和健康检查等信息，这些信息都会被发布在服务注册表中，用于服务之间的调用。<br />自定义元数据：自定义元数据可以使用`eureka.instance.metadata-map`配置，这些元数据可以在远程客户端中访问，但是一般不会改变客户端的行为，除非客户端知道该元数据的含义
<a name="zbcXx"></a>
### eureka RestFul接口
| 请求名称 | 请求方式 | HTTP地址 | 请求描述 |
| --- | --- | --- | --- |
| 注册新服务 | POST | /eureka/apps/{appID} | 传递JSON或者XML格式参数内容，HTTP code为204时表示成功 |
| 取消注册服务 | DELETE | /eureka/apps/{appID}/{instanceID} | HTTP code为200时表示成功 |
| 发送服务心跳 | PUT | /eureka/apps/{appID}/{instanceID} | HTTP code为200时表示成功 |
| 查询所有服务 | GET | /eureka/apps | HTTP code为200时表示成功，返回XML/JSON数据内容 |
| 查询指定appID的服务列表 | GET | /eureka/apps/{appID} | HTTP code为200时表示成功，返回XML/JSON数据内容 |
| 查询指定appID&instanceID | GET | /eureka/apps/{appID}/{instanceID} | 获取指定appID以及InstanceId的服务信息，HTTP code为200时表示成功，返回XML/JSON数据内容 |
| 查询指定instanceID服务列表 | GET | /eureka/apps/instances/{instanceID} | 获取指定instanceID的服务列表，HTTP code为200时表示成功，返回XML/JSON数据内容 |
| 变更服务状态 | PUT | /eureka/apps/{appID}/{instanceID}/status?value=DOWN | 服务上线、服务下线等状态变动，HTTP code为200时表示成功 |
| 变更元数据 | PUT | /eureka/apps/{appID}/{instanceID}/metadata?key=value | HTTP code为200时表示成功 |

<a name="xz1ID"></a>
### 更改自定义元数据
配置文件方式：
```java
eureka.instance.metadata-map.version = v1
```
接口请求：
```http
PUT /eureka/apps/{appID}/{instanceID}/metadata?key=value
```
<a name="Afqm6"></a>
## 实现流程
![2021-05-30-13-09-41-172190.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622351723721-57792e92-dfa9-4688-b7bc-434ed8197089.png#height=440&id=u6538344a&originHeight=440&originWidth=1080&originalType=binary&size=145172&status=done&style=shadow&width=1080)

1. 用户请求首先到达Nginx然后转发到网关zuul，此时zuul拦截器会根据用户携带请求token解析出对应的userId。
2. 网关从Apollo配置中心拉取灰度用户列表，然后根据灰度用户策略判断该用户是否是灰度用户。如是，则给该请求添加请求头及线程变量添加信息version=xxx；若不是，则不做任何处理放行。
3. 在zuul拦截器执行完毕后，zuul在进行转发请求时会通过负载均衡器Ribbon。
4. 负载均衡Ribbon被重写。当请求到达时候，Ribbon会取出zuul存入线程变量值version。于此同时，Ribbon还会取出所有缓存的服务列表（定期从eureka刷新获取最新列表）及其该服务的metadata-map信息。然后取出服务metadata-map的version信息与线程变量version进行判断对比，若值一直则选择该服务作为返回。若所有服务列表的version信息与之不匹配，则返回null，此时Ribbon选取不到对应的服务则会报错！
5. 当服务为非灰度服务，即没有version信息时，此时Ribbon会收集所有非灰度服务列表，然后利用Ribbon默认的规则从这些非灰度服务列表中返回一个服务。
6. zuul通过Ribbon将请求转发到consumer服务后，可能还会通过fegin或resttemplate调用其他服务，如provider服务。但是无论是通过fegin还是resttemplate，他们最后在选取服务转发的时候都会通过Ribbon。
7. 那么在通过fegin或resttemplate调用另外一个服务的时候需要设置一个拦截器，将请求头version=xxx给带上，然后存入线程变量。
8. 在经过fegin或resttemplate 的拦截器后最后会到Ribbon，Ribbon会从线程变量里面取出version信息。然后重复步骤（4）和（5）。

![2021-05-30-13-09-41-304058.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622351743476-2060c98b-169a-4bcc-8de5-80624e46f5ff.png#height=693&id=ud446f97d&originHeight=693&originWidth=437&originalType=binary&size=17055&status=done&style=shadow&width=437)
<a name="yyRv1"></a>
## 设计思路
首先，通过更改服务在eureka的元数据标识该服务为灰度服务，这边用的元数据字段为version。

1. 首先更改服务元数据信息，标记其灰度版本。通过eureka RestFul接口或者配置文件添加如下信息
```java
eureka.instance.metadata-map.version=v1
```

2. 自定义zuul拦截器GrayFilter。此处获取的请求头为token，然后将根据JWT的思想获取userId，然后获取灰度用户列表及其灰度版本信息，判断该用户是否为灰度用户。

若为灰度用户，则将灰度版本信息version存放在线程变量里面。此处不能用`Threadlocal`存储线程变量，因为SpringCloud用hystrix做线程池隔离，而线程池是无法获取到`ThreadLocal`中的信息的! 所以这个时候可以参考Sleuth做分布式链路追踪的思路或者使用阿里开源的`TransmittableThreadLocal`方案。此处使用`HystrixRequestVariableDefault`实现跨线程池传递线程变量。

3. zuul拦截器处理完毕后，会经过ribbon组件从服务实例列表中获取一个实例选择转发。Ribbon默认的Rule为`ZoneAvoidanceRule`。而此处继承该类，重写了其父类选择服务实例的方法。

以下为Ribbon源码：
```java
public abstract class PredicateBasedRule extends ClientConfigEnabledRoundRobinRule {
    // 略....
    @Override
    public Server choose(Object key) {
        ILoadBalancer lb = getLoadBalancer();
        Optional<Server> server = getPredicate().chooseRoundRobinAfterFiltering(lb.getAllServers(), key);
        if (server.isPresent()) {
            return server.get();
        } else {
            return null;
        }       
    }
}
```
以下为自定义实现的伪代码:
```java
public class GrayMetadataRule extends ZoneAvoidanceRule {
   // 略....
    @Override
    public Server choose(Object key) {
      //1.从线程变量获取version信息
        String version = HystrixRequestVariableDefault.get();
        
      //2.获取服务实例列表
        List<Server> serverList = this.getPredicate().getEligibleServers(this.getLoadBalancer().getAllServers(), key);
        
       //3.循环serverList，选择version匹配的服务并返回
                for (Server server : serverList) {
            Map<String, String> metadata = ((DiscoveryEnabledServer) server).getInstanceInfo().getMetadata();
 
            String metaVersion = metadata.get("version);
            if (!StringUtils.isEmpty(metaVersion)) {
                if (metaVersion.equals(hystrixVer)) {
                    return server;
                }
            }
        }
    }
}
```

4. 此时只是已经完成了 请求==》zuul==》zuul拦截器==》自定义ribbon负载均衡算法==》灰度服务这个流程，并没有涉及到 服务==》服务的调用。

服务到服务的调用无论是通过resttemplate还是fegin，最后也会走ribbon的负载均衡算法，即服务==》Ribbon 自定义Rule==》服务。因为此时自定义的`GrayMetadataRule`并不能从线程变量中取到version，因为已经到了另外一个服务里面了。

5. 此时依然可以参考Sleuth的源码`org.springframework.cloud.sleuth.Span`，这里不做赘述只是大致讲一下该类的实现思想。就是在请求里面添加请求头，以便下个服务能够从请求头中获取信息。

此处，可以通过在 步骤2中，让zuul添加添加线程变量的时候也在请求头中添加信息。然后，再自定义`HandlerInterceptorAdapter`拦截器，使之在到达服务之前将请求头中的信息存入到线程变量`HystrixRequestVariableDefault`中。<br />然后服务再调用另外一个服务之前，设置resttemplate和fegin的拦截器，添加头信息。<br />resttemplate拦截器
```java
public class CoreHttpRequestInterceptor implements ClientHttpRequestInterceptor {
    @Override
    public ClientHttpResponse intercept(HttpRequest request, byte[] body, ClientHttpRequestExecution execution) throws IOException {
        HttpRequestWrapper requestWrapper = new HttpRequestWrapper(request);
        String hystrixVer = CoreHeaderInterceptor.version.get();
        requestWrapper.getHeaders().add(CoreHeaderInterceptor.HEADER_VERSION, hystrixVer);
        return execution.execute(requestWrapper, body);
    }
}
```
fegin拦截器
```java
public class CoreFeignRequestInterceptor implements RequestInterceptor {
    @Override
    public void apply(RequestTemplate template) {
        String hystrixVer = CoreHeaderInterceptor.version.get();
        logger.debug("====>fegin version:{} ",hystrixVer); 
        template.header(CoreHeaderInterceptor.HEADER_VERSION, hystrixVer);
    }

}
```

6. 到这里基本上整个请求流程就比较完整了，但是怎么让Ribbon使用自定义的Rule?这里其实非常简单，只需要在服务的配置文件中配置一下代码即可。
```java
yourServiceId.ribbon.NFLoadBalancerRuleClassName=自定义的负载均衡策略类
```
但是这样配置需要指定服务名，意味着需要在每个服务的配置文件中这么配置一次，所以需要对此做一下扩展。打开源码`org.springframework.cloud.netflix.ribbon.RibbonClientConfiguration`类，该类是Ribbon的默认配置类。可以清楚的发现该类注入了一个`PropertiesFactory`类型的属性，可以看到`PropertiesFactory`类的构造方法
```java
public PropertiesFactory() {
    classToProperty.put(ILoadBalancer.class, "NFLoadBalancerClassName");
    classToProperty.put(IPing.class, "NFLoadBalancerPingClassName");
    classToProperty.put(IRule.class, "NFLoadBalancerRuleClassName");
    classToProperty.put(ServerList.class, "NIWSServerListClassName");
    classToProperty.put(ServerListFilter.class, "NIWSServerListFilterClassName");
}
```
所以，可以继承该类从而实现扩展，这样一来就不用配置具体的服务名了。

7. 到这里基本上整个请求流程就比较完整了，上述例子中是以用户ID作为灰度的维度，当然这里可以实现更多的灰度策略，比如IP等，基本上都可以基于此方式做扩展。
<a name="MCdHM"></a>
## 灰度使用
<a name="Xa5My"></a>
### 配置文件示例
```java
spring.application.name = provide-test
server.port = 7770
eureka.client.service-url.defaultZone = http://localhost:1111/eureka/
#启动后直接将该元数据信息注册到eureka
#eureka.instance.metadata-map.version = v1
```
<a name="ZfhtA"></a>
### 测试案例
分别启动四个测试实例，有version代表灰度服务，无version则为普通服务。当灰度服务测试没问题的时候，通过PUT请求eureka接口将version信息去除，使其变成普通服务。
<a name="nTz5F"></a>
### 实例列表

- [x] zuul-server
- [x] provider-test<br />port:7770 version:无<br />port: 7771 version:v1
- [x] consumer-testport:8880 version:无port: 8881 version:v1
<a name="sUdhO"></a>
### 修改服务信息
服务在eureka的元数据信息可通过接口http://localhost:1111/eureka/apps访问到。
<a name="LZjzM"></a>
### 服务信息实例
访问接口查看信息http://localhost:1111/eureka/apps/PROVIDE-TEST<br />![2021-05-30-13-09-41-441976.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622351768725-04828d10-9071-4d12-b9f7-b5504eb6352b.png#height=962&id=u6c122909&originHeight=962&originWidth=609&originalType=binary&size=19393&status=done&style=shadow&width=609)
<a name="QNyZL"></a>
### 注意事项
通过此种方法更改server的元数据后，由于ribbon会缓存实力列表，所以在测试改变服务信息时，ribbon并不会立马从eureka拉去最新信息m，这个拉取信息的时间可自行配置。<br />同时，当服务重启时服务会重新将配置文件的version信息注册上去。
<a name="JSh4N"></a>
### 测试演示
<a name="c8vZ4"></a>
#### zuul==>provider服务
用户andy为灰度用户。 <br />1.测试灰度用户andy，是否路由到灰度服务 provider-test:7771 <br />2.测试非灰度用户andyaaa(任意用户)是否能被路由到普通服务 provider-test:7770<br />![2021-05-30-13-09-41-679143.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1622351782440-859816ee-7486-4d26-a085-97887bc60d1a.gif#height=536&id=udcd482f7&originHeight=536&originWidth=1078&originalType=binary&size=269090&status=done&style=shadow&width=1078)
<a name="dzezA"></a>
#### zuul==>consumer服务>provider服务
以同样的方式再启动两个consumer-test服务，这里不再截图演示。<br />请求从zuul==>consumer-test==>provider-test,通过fegin和resttemplate两种请求方式测试<br />Resttemplate请求方式<br />![2021-05-30-13-09-41-958062.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1622351793258-d5add402-3fe7-4c51-b042-f8125c2fce3e.gif#height=536&id=u86818020&originHeight=536&originWidth=1078&originalType=binary&size=294969&status=done&style=shadow&width=1078)<br />fegin请求方式<br />![2021-05-30-13-09-42-179361.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1622351800326-c70501cc-e47a-4172-85a0-40e1917ef897.gif#height=536&id=ua6d87dc0&originHeight=536&originWidth=1078&originalType=binary&size=275501&status=done&style=shadow&width=1078)
<a name="Uary5"></a>
## 自动化配置
与Apollo实现整合，避免手动调用接口。实现配置监听，完成灰度。
