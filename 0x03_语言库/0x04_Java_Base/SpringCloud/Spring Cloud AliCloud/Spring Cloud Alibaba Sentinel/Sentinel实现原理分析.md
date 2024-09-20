JavaSpringCloudSentinel
<a name="Xx7s6"></a>
## Sentinel简介
Sentinel是阿里开源的一款面向分布式、多语言异构化服务架构的流量治理组件。<br />主要以流量为切入点，从流量路由、流量控制、流量整形、熔断降级、系统自适应过载保护、热点流量防护等多个维度来帮助开发者保障微服务的稳定性。<br />上面两句话来自Sentinel官网的介绍，从这短短的两句话就可以看出Sentinel的定位和拥有的强大功能。
<a name="isCoV"></a>
## 核心概念
要想理解一个新的技术，那么首先得理解它的一些核心概念
<a name="R2F2I"></a>
### 资源
资源是Sentinel中一个非常重要的概念，资源就是Sentinel所保护的对象。<br />资源可以是一段代码，又或者是一个接口，Sentinel中并没有什么强制规定，但是实际项目中一般以一个接口为一个资源，比如说一个http接口，又或者是rpc接口，它们就是资源，可以被保护。<br />资源是通过Sentinel的API定义的，每个资源都有一个对应的名称，比如对于一个http接口资源来说，Sentinel默认的资源名称就是请求路径。
<a name="cSN0v"></a>
### 规则
规则也是一个重要的概念，规则其实比较好理解，比如说要对一个资源进行限流，那么限流的条件就是规则，后面在限流的时候会基于这个规则来判定是否需要限流。<br />Sentinel的规则分为流量控制规则、熔断降级规则以及系统保护规则，不同的规则实现的效果不一样。
<a name="ZsJRC"></a>
## 来个Demo
如果已经使用过了Sentinel，那么就可以直接pass这一节，直接快进到核心原理。
<a name="GYX2M"></a>
### 1、基本使用
引入依赖
```xml
<dependency>
  <groupId>com.alibaba.csp</groupId>
  <artifactId>sentinel-core</artifactId>
  <version>1.8.6</version>
</dependency>
```
测试代码
```java
public class SentinelSimpleDemo {

    public static void main(String[] args) {
        //加载流控规则
        initFlowRules();

        for (int i = 0; i < 5; i++) {
            Entry entry = null;
            try {
                entry = SphU.entry("sayHello");
                //被保护的逻辑
                System.out.println("访问sayHello资源");
            } catch (BlockException ex) {
                System.out.println("被流量控制了，可以进行降级处理");
            } finally {
                if (entry != null) {
                    entry.exit();
                }
            }
        }
    }

    private static void initFlowRules() {
        List<FlowRule> rules = new ArrayList<>();

        //创建一个流控规则
        FlowRule rule = new FlowRule();
        //对sayHello这个资源限流
        rule.setResource("sayHello");
        //基于qps限流
        rule.setGrade(RuleConstant.FLOW_GRADE_QPS);
        //qps最大为2，超过2就要被限流
        rule.setCount(2);

        rules.add(rule);

        //设置规则
        FlowRuleManager.loadRules(rules);
    }

}
```
解释一下上面这段代码的意思

- `initFlowRules`方法就是加载一个限流的规则，这个规则作用于sayHello这个资源，基于qps限流，当qps超过2之后就会触发限流。
- `SphU.entry("sayHello")`这行代码是Sentinel最最核心的源码，这行代码表面看似风平浪静，实则暗流涌动。这行代码表明接下来需要访问某个资源（参数就是资源名称），会去检查需要被访问的资源是否达到设置的流控、熔断等规则。对于demo来说，就是检查sayHello这个资源是否达到了设置的流量控制规则。
- `catch BlockException`也很重要，当抛出`BlockException`这个异常，说明触发了一些设置的保护规则，比如限流了，这里面就可以进行降级操作。
- `System.out.println("访问sayHello资源")`这行代码表面是一个打印语句，实则就是前面一直在说的需要被保护的资源。

所以上面这段代码的整体意思就是对sayHello这个需要访问的资源设置了一个流控规则，规则的内容是当qps到达2的时候触发限流，之后循环5次访问sayHello这个资源，在访问之前通过`SphU.entry("sayHello")`这行代码进行限流规则的检查，如果达到了限流的规则的条件，会抛出BlockException。<br />测试结果<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564760434-cba14baa-3331-4ecd-a2c3-c9515fd2e3fb.png#averageHue=%233e3837&clientId=u6154d4bb-73b4-4&from=paste&id=u7f341d31&originHeight=320&originWidth=946&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5146b1c3-d559-410c-8e53-1cd26cff940&title=)<br />从结果可以看出，当前两次访问sayHello成功之后，qps达到了2，之后再访问就被限流了，失败了。
<a name="FsyYp"></a>
### 2、集成Spring
在实际的项目使用中一般不会直接写上面的那段demo代码，而是集成到Spring环境底下。<br />引入依赖
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
  <version>2.2.5.RELEASE</version>
</dependency>
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-sentinel</artifactId>
  <version>2.2.5.RELEASE</version>
</dependency>
```
之后提供一个/sayHello接口
```java
@RestController
public class SentinelDemoController {

    @GetMapping("/sayHello")
    public String sayHello() throws InterruptedException {
        return "hello";
    }

}
```
配置文件
```yaml
server:
  port: 9527
  
spring:
  application:
    name: SentinelDemo
```
到这demo就搭建完成了。<br />此时心理肯定有疑问，那前面提到的资源和对应的规则去哪了？<br />前面在说资源概念的时候，提到Sentinel中默认一个http接口就是一个资源，并且资源的名称就是接口的请求路径。<br />而真正的原因是Sentinel实现了SpringMVC中的`HandlerInterceptor`接口，在调用Controller接口之前，会将一个调用接口设置为一个资源，代码如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564760457-add11d8b-df9c-4049-a3b4-dc4c147123f2.png#averageHue=%23302c2b&clientId=u6154d4bb-73b4-4&from=paste&id=ua3163f3d&originHeight=909&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8af99aa1-5ab4-4de2-91cb-54d3522bc79&title=)<br />而`getResourceName`方法就是获取资源名，其实就是接口的请求路径，比如前面提供的接口路径是/sayHello，那么资源名就是/sayHello。<br />再后面的代码就是调用上面demo中提到表面风平浪静，实则暗流涌动的`SphU.entry(..)`方法，检查被调用的资源是否达到了设置的规则。<br />好了，既然资源默认是接口，已经有了，那么规则呢？<br />规则当然可以按照第一个demo的方式来做，比如在Controller接口中加载，代码如下。
```java
@RestController
public class SentinelDemoController {

    static {
        List<FlowRule> rules = new ArrayList<>();

        //创建一个流控规则
        FlowRule rule = new FlowRule();
        //对/sayHello这个资源限流
        rule.setResource("/sayHello");
        //基于qps限流
        rule.setGrade(RuleConstant.FLOW_GRADE_QPS);
        //qps最大为2，超过2就要被限流
        rule.setCount(2);

        rules.add(rule);

        //设置规则
        FlowRuleManager.loadRules(rules);
    }

    @GetMapping("/sayHello")
    public String sayHello() throws InterruptedException {
        return "hello";
    }

}
```
此时启动项目，在浏览器输入以下链接<br />http://localhost:9527/sayHello<br />疯狂快速使劲地多点几次，就出现下面这种情况<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564760437-86f29389-4954-463e-8f1f-b1fe5918d59e.png#averageHue=%23f9f9f9&clientId=u6154d4bb-73b4-4&from=paste&id=ub03edb45&originHeight=246&originWidth=614&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u32d8a099-847b-4ab7-8f37-c9bfdb2c052&title=)<br />可以看出规则生效了，接口被Sentinel限流了，至于为什么出现这个提示，是因为Sentinel有默认的处理`BlockException`的机制，就在前面提到的进入资源的后面。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564760444-33b2b103-acf0-4829-92d7-5548d11b6434.png#averageHue=%23312c2b&clientId=u6154d4bb-73b4-4&from=paste&id=ub7718358&originHeight=467&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5f7ef05e-05f9-442a-8bf6-298d84d4ae0&title=)<br />当然，也可以自定义处理的逻辑，实现`BlockExceptionHandler`接口就可以了。<br />虽然上面这种硬编码规则的方式可以使用，但是在实际的项目中，肯定希望能够基于系统当期那运行的状态来动态调整规则，所以Sentinel提供了一个叫Dashboard应用的控制台，可以通过控制台来动态修改规则。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564761062-5eeca398-1c58-4238-937c-5dbe1a154cfb.png#averageHue=%23f7f6f4&clientId=u6154d4bb-73b4-4&from=paste&id=ub04c81a5&originHeight=141&originWidth=421&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u74a68eeb-c390-4853-aad4-d4c9358de4c&title=)<br />控制台其实就是一个jar包，可以从Sentinel的github仓库上下载。<br />之后通过`java -jar`命令启动就可以了，端口默认8080，浏览器访问http://ip:8080/#/login就可以登录控制台了，用户名和密码默认都是sentinel。<br />此时服务要接入控制台，只需要在配置文件上加上控制台的ip和端口即可
```yaml
spring:
  cloud:
    sentinel:
      transport:
        # 指定控制台的ip和端口
        dashboard: localhost:8080
```
项目刚启动的时候控制台默认是没有数据的，需要访问一下接口，之后就有了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564761169-e4ead40a-1d7b-4886-9a9d-4d3f9cc563ed.png#averageHue=%23f9f8f8&clientId=u6154d4bb-73b4-4&from=paste&id=u5008e436&originHeight=433&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uab5eb613-0011-46dd-b4e4-b2360fc3bfc&title=)<br />之后就可以看到/sayHello这个资源，后面就可以通过页面设置规则。
<a name="ckRsV"></a>
## 核心原理
讲完demo，接下来就来讲一讲Sentinel的核心原理，也就是前面提到暗流涌动的`SphU.entry(..)`这行代码背后的逻辑。<br />Sentinel会为每个资源创建一个处理链条，就是一个责任链，第一次访问这个资源的时候创建，之后就一直复用，所以这个处理链条每个资源有且只有一个。<br />`SphU.entry(..)`这行代码背后就会调用责任链来完成对资源的检查逻辑。<br />这个责任链条中每个处理节点被称为`ProcessorSlot`，中文意思就是处理器槽<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564761262-d97e8b18-4bb5-4cbf-870a-b0ec4713044a.png#averageHue=%23302f2e&clientId=u6154d4bb-73b4-4&from=paste&id=u62452326&originHeight=506&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8709fed7-4dc2-467c-a949-9b2321a8765&title=)<br />这个ProcessorSlot有很多实现，但是Sentinel的核心就下面这8个：

- NodeSelectorSlot
- ClusterBuilderSlot
- LogSlot
- StatisticSlot
- AuthoritySlot
- SystemSlot
- FlowSlot
- DegradeSlot

这些实现会通过SPI机制加载，然后按照一定的顺序组成一个责任链。<br />默认情况下，节点是按照如下的顺序进行排序的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784096-d04a79a4-bc18-4421-a931-a25d6ccb5882.png#averageHue=%238bb871&clientId=u6154d4bb-73b4-4&from=paste&id=ubb0fca1d&originHeight=181&originWidth=604&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf8a25fe7-762e-4285-a87d-02448fcb6c7&title=)<br />虽然默认就8个，但是如果想扩展，只要实现`ProcessorSlot`，按照SPI的规定配置一下就行。<br />下面就来按照上面节点的处理顺序来好好扒一扒这8个`ProcessorSlot`。
<a name="Be0vh"></a>
### 1、`NodeSelectorSlot`
这个节点的作用是来设置当前资源**对应的入口**的**统计Node**。<br />首先什么是统计Node？<br />比如就拿上面的例子来说，当/sayHello这个资源的qps超过2的时候，要触发限流。<br />但是有个疑问，Sentinel是怎么知道/sayHello这个资源的qps是否达到2呢？<br />当然是需要进行数据统计的，只有通过统计，才知道qps是否达到2，这个进行数据统计的类在Sentinel中叫做Node。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784180-6dbefdb4-7f57-4615-933a-032fc459d2d9.png#averageHue=%23323130&clientId=u6154d4bb-73b4-4&from=paste&id=ub89c87df&originHeight=732&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u473f488c-5d8b-485d-9882-66c4523a8a3&title=)<br />通过Node这个统计的类就知道有多少请求，成功多少个，失败多少个，qps是多少之类的。底层其实是使用到了滑动窗口算法。<br />那么什么叫对应的入口？<br />在Sentinel中，支持同一个资源有不同的访问入口。<br />举个例子，这个例子后面会反复提到。<br />假设把杭州看做是服务，西湖看做是一个资源，到达西湖有两种方式，地铁和公交。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784023-5aa1ddbf-d477-4603-8ccc-8f05b41ac69d.png#averageHue=%23fbf7f3&clientId=u6154d4bb-73b4-4&from=paste&id=u947d4506&originHeight=181&originWidth=251&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2e2bff8d-63d7-478b-8ab2-a9f07f4a08c&title=)<br />所以要想访问西湖这个资源，就可以通过公交和地铁两种方式，而公交和地铁就对应前面说的入口的意思。<br />只不过一般一个资源就一个入口，比如一个http接口一般只能通过http访问，但是Sentinel支持多入口，可以不用，但是Sentinel有。<br />所以NodeSelectorSlot的作用就是选择资源在当前调用入口的统计Node，这样就实现了统计同一个资源在不同入口访问数据，用上面的例子解释，就可以实现分别统计通过公交和地铁访问西湖的人数。<br />资源的入口可以在进入资源之前通过`ContextUtil.enter("入口名", origin)`来指定，如果不指定，那么入口名称默认就是`sentinel_default_context`。<br />在SpringMVC环境底下，所有的http接口资源，默认的入口都是`sentinel_spring_web_context`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784025-be4fc7e8-77d1-4de2-abc8-f062dcec0247.png#averageHue=%23322c2b&clientId=u6154d4bb-73b4-4&from=paste&id=uca491715&originHeight=283&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6306c96b-44a7-44fa-820d-b38ca95a8ee&title=)<br />入口名称也可以通过控制台看到<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784067-3ea1caea-0d39-4e82-8411-7820ab7129d9.png#averageHue=%23fbf9f9&clientId=u6154d4bb-73b4-4&from=paste&id=uadafa290&originHeight=291&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue01fdf75-2025-49aa-aed5-20fc1016531&title=)<br />那么为什么要搞一个入口的概念呢？这里咱先留个悬念，后面再说。
<a name="rRnwf"></a>
### 2、`ClusterBuilderSlot`
`ClusterBuilderSlot`的作用跟`NodeSelectorSlot`其实是差不多的，也是用来选择统计Node，但是选择的Node的统计维护跟`NodeSelectorSlot`不一样。<br />`ClusterBuilderSlot`会选择两个统计Node：

- 第一个统计Node是资源的所有入口的统计数据之和，就是资源访问的总数据
- 第二个统计Node就是统计资源调用者对资源访问数据

资源调用者很好理解，比如一个http接口资源肯定会被调用，那么调用这个接口的服务或者应用其实就是资源的调用者，但是一般资源的调用者就是指某个服务，后面调用者可能会以服务来代替。<br />一个接口可以被很多服务调用，所以一个资源可以很多调用者，而不同调用者都会有单独的一个统计Node，用来分别统计不同调用者对资源的访问数据。<br />举个例子，现在访问西湖这个资源的大兄弟来自上海，那么就会为上海创建一个统计Node，用来统计所有来自上海的人数，如果是北京，那么就会为北京创建一个统计Node。<br />那么如何知道访问资源来自哪个服务（调用者）呢？<br />也是通过`ContextUtil.enter("入口名", origin)`来指定，这个方法的第二个参数origin就是代表服务名的意思，默认是空。<br />所以`ContextUtil.enter(..)`可以同时指定资源的入口和调用者，一个资源一定有入口，因为不指定入口默认就是`sentinel_default_context`，但是调用者不指定就会没有。<br />对于一个http请求来说，Sentinel默认服务名需要放到S-user这个请求头中，所以如果想知道接口的调用服务，需要在调用方发送请求的时候将服务名设置到S-user请求头中。<br />当资源所在的服务接收到请求时，Sentinel就会从S-user请求头获取到服务名，之后再通过`ContextUtil.enter("入口名", "调用者名")`来设置当前资源的调用者<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784579-7bdccaea-0ac8-49cf-8f8e-215beedc9c7e.png#averageHue=%232f2c2b&clientId=u6154d4bb-73b4-4&from=paste&id=ucf320eb5&originHeight=483&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9c6f58cf-a10d-4cf8-ab50-6267ba63a0b&title=)<br />这里原以为Sentinel会适配比如OpenFeign之类的框架，会自动将服务名携带到请求头中，但是翻了一下源码，发现并没有去适配，不知道是出于什么情况的考虑。<br />所以可以知道一个资源其实有三种维度的统计Node：

- 分别统计不同入口的访问数据
- 统计所有入口访问数据之和
- 分别统计来自某个服务的访问数据

为了方便区分，给这三个统计Node取个响亮的名字<br />不同入口的访问数据就叫他`DefaultNode`，统计所有入口访问数据之和就叫他`ClusterNode`，来自某个服务的访问数据就叫他`OriginNode`。<br />那么他们的关系就可以用下面这个图来表示<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784574-9712b3eb-c91e-4b12-9008-c2032cfb3d90.png#averageHue=%23f5f0e9&clientId=u6154d4bb-73b4-4&from=paste&id=u031eed64&originHeight=241&originWidth=431&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub3db6cec-6369-499f-86ec-1f467ce6f54&title=)
<a name="jOzPU"></a>
### 3、LogSlot
这个Slot没什么好说的，通过名字可以看出来，其实就是用来打印日志的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784583-60da6732-3aea-4cbd-bdca-43cd542f7132.png#averageHue=%232f2c2b&clientId=u6154d4bb-73b4-4&from=paste&id=u0b4952db&originHeight=450&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud76a3d73-368f-4f40-8ed6-7badd4813a4&title=)<br />当发生异常，就会打印日志。
<a name="XQaWr"></a>
### 4、StatisticSlot
这个Slot就比较重要了，就是用来统计数据的。<br />前面说的NodeSelectorSlot和ClusterBuilderSlot，他们的作用就是根据资源当前的入口和调用来源来选择对应的统计Node。<br />而StatisticSlot就是对这些统计Node进行实际的统计，比如加一下资源的访问线程数，资源的请求数量等等。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784703-780d964f-edb6-4a15-bd07-8d928e24a728.png#averageHue=%232e2c2c&clientId=u6154d4bb-73b4-4&from=paste&id=u200f6041&originHeight=893&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7dd0f3cd-3acd-4c91-aa70-f98bdf0bc36&title=)<br />前几个Slot其实都是准备、统计的作用，并没有涉及限流降级之类的，他们是为限流降级提供数据支持的。
<a name="dKXQl"></a>
### 5、`AuthoritySlot`
`Authority`是授权的意思，这个Slot的作用是对资源调用者进行授权，就是黑白名单控制。<br />可以通过控制台来添加授权规则。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564784842-ee998bc2-f471-408a-9f72-e89bb5e7e182.png#averageHue=%23b6b4b4&clientId=u6154d4bb-73b4-4&from=paste&id=uad0f42cc&originHeight=494&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub34db922-e622-47d1-9286-abecfe2229c&title=)<br />在`AuthoritySlot`中会去获取资源的调用者，之后会跟授权规则中的资源应用这个选项进行匹配，之后就会出现有以下2种情况：

- 授权类型是黑名单，匹配上了，说明在黑名单内，那么这个服务就不能访问这个资源，没匹配上就可以访问
- 授权类型是白名单。匹配上了，说明在白名单内，那么这个服务就可以访问这个资源，没匹配上就不可以访问
<a name="UabSS"></a>
### 6、SystemSlot
这个的作用是根据整个系统运行的统计数据来限流的，防止当前系统负载过高。<br />它支持入口qps、线程数、响应时间、cpu使用率、负载5个限流的维度。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564785038-ff07fbee-c26d-4e45-b352-35f50d263160.png#averageHue=%23afadac&clientId=u6154d4bb-73b4-4&from=paste&id=ubb7a86bc&originHeight=378&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud2486127-db89-4d04-a003-6a32811c27f&title=)<br />对于系统的入口qps、线程数、平均响应时间这些指标，也会有一个统计Node专门去统计，所以这个统计Node的作用就好比会去统计所有访问西湖的人数，统计也在`StatisticSlot`代码中，前面说的时候把代码隐藏了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564785145-2e927307-0648-495e-9b27-4bcf589c41cf.png#averageHue=%23302e2d&clientId=u6154d4bb-73b4-4&from=paste&id=u018531a7&originHeight=560&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6c0e4ebe-0e83-4ac2-a5ac-ffaace51da7&title=)<br />至于cpu使用率、负载指标，Sentinel会启动一个定时任务，每隔1s会去读取一次当前系统的cpu和负载。
<a name="fauaD"></a>
### 7、FlowSlot
这个Slot会根据预设的规则，结合前面的统计出来的实时信息进行流量控制。<br />在说FlowSlot之前，先来用之前画的那张图回顾一下一个资源的三种统计维度<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564785427-df9742c5-5d41-4288-98cd-83e6800864cf.png#averageHue=%23f5f0e9&clientId=u6154d4bb-73b4-4&from=paste&id=u526731b7&originHeight=241&originWidth=431&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u88c15c19-8376-4f89-b35e-aa49d1fb96b&title=)<br />这里默默地注视10s。<br />限流规则配置项比较多<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564785494-d34fd0bb-5648-49de-a8c5-67d980d01d17.png#averageHue=%23d3d3d3&clientId=u6154d4bb-73b4-4&from=paste&id=u0363d5ef&originHeight=681&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udd1156f3-a972-4caa-be66-c0769000490&title=)<br />这里来好好扒一扒这些配置项的意思。<br />**针对来源**，来源就是前面说的调用方，这个配置表明，这个规则适用于哪个调用方，默认是default，就是指规则适用于所有调用方，如果指定了调用方，那么这个规则仅仅对指定的调用方生效。<br />举个例子来说，比如说现在想限制来自上海的访问的人数，那么针对来源可以填上海，之后当访问的大兄弟来自上海的时候，Sentinel就会根据上海对应的OriginNode数据来判断是否达到限流的条件。<br />**阈值类型**，就是限流条件，当资源的qps或者访问的线程数到达设置的单机阈值，就会触发限流。<br />**是否集群**，这个作用是用来对集群控制的，因为一个服务可能在很多台机器上，而这个的作用就是将整个集群看成一个整体来限流，这里就不做深入讨论。<br />**流控模式**，这个流控模式的选项仅仅对阈值类型为qps有效，当阈值类型线程数时无效。<br />这个配置就比较有意思了，分为直接、关联、链路三种模式。<br />直接模式的意思就是当资源的ClusterNode统计数据统计达到了阈值，就会触发限流。<br />比如，当通过地铁和公交访问西湖人数之和达到单机阈值之后就会触发限流。<br />关联模式下需要填写关联的资源名称<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564785501-3d16273f-c0f2-4dbc-aa72-25b0a92305bb.png#averageHue=%23f9f5f4&clientId=u6154d4bb-73b4-4&from=paste&id=u0017b350&originHeight=417&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u66cc01cc-b4ab-47fe-8c4f-004ca6573c7&title=)<br />关联的意思就是当**关联资源**的ClusterNode统计的qps达到了设置的阈值时，就会触发**当前资源**的限流操作。<br />比如，假设现在西湖这个资源关联了雷峰塔这个资源，那么当访问雷峰塔的人数达到了指定的阈值之后，此时就触发西湖这个资源的限流，就是雷峰塔流量高了但是限流的是西湖。<br />链路模式也一样，它需要关联一个入口资源<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564785628-ca8416fb-19e2-4a84-a288-38107e6d5e2b.png#averageHue=%23f9f3f2&clientId=u6154d4bb-73b4-4&from=paste&id=u720fd952&originHeight=297&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud03f5bcd-c8f4-4f10-8cc8-2f6d3d1419e&title=)<br />关联入口的意思就是指，当访问资源的实际入口跟关联入口是一样的时候，就会根据这个入口对应的DefaultNode的统计数据来判断是否需要限流。<br />也就是可以单独限制通过公交和地铁的访问的人数的意思。<br />到这，其实前面说到的一个资源的三种统计维度的数据都用到了，现在应该明白了为什么需要这么多维度的数据，就是为不同维度限流准备的。<br />最后一个配置项，**流控效果**，这个就是如果是通过qps来限流，并且达到了限流的条件之后会做什么，如果是线程数，就直接抛出BlockException异常<br />也有三种方式，快速失败、Warm Up、排队等待<br />快速失败的意思就是指一旦触发限流了，那么直接抛出BlockException异常<br />Warm Up的作用就是为了防止系统流量突然增加时出现瞬间把系统压垮的情况。通过"冷启动"，让通过的流量缓慢增加，在一定时间内逐渐增加到阈值上限。<br />排队等待，很好理解，意思当出现限流了，不是抛异常，而是去排队等待一定时间，其实就是让请求均匀速度通过，内部使用的是传说中的漏桶算法。
<a name="qKPlr"></a>
### `DegradeSlot`
这是整个责任链中最后一个slot，这个slot的作用是用来熔断降级的。<br />Sentinel支持三种熔断策略：慢调用比例、异常比例 、异常数，通过规则配置也可以看出来。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564785920-5edd275f-9748-4295-9bb9-7dbec9af5e54.png#averageHue=%23e0dfdf&clientId=u6154d4bb-73b4-4&from=paste&id=uc28b703a&originHeight=691&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u346aec1e-a925-4763-990c-487e1a3ae7f&title=)<br />熔断器的工作流程大致如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1682564785906-be8eca6a-357f-4bf2-b74a-657b20832317.png#averageHue=%23221f1d&clientId=u6154d4bb-73b4-4&from=paste&id=u343b0848&originHeight=541&originWidth=382&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua3cd8a5f-5336-4723-8863-106342732e0&title=)<br />Sentinel会为每个设置的规则都创建一个熔断器，熔断器有三种状态，OPEN(打开)、HALF_OPEN(半开)、CLOSED(关闭)

- 当处于CLOSED状态时，可以访问资源，访问之后会进行慢调用比例、异常比例、异常数的统计，一旦达到了设置的阈值，就会将熔断器的状态设置为OPEN
- 当处于OPEN状态时，会去判断是否达到了熔断时间，如果没到，拒绝访问，如果到了，那么就将状态改成HALF_OPEN，然后访问资源，访问之后会对访问结果进行判断，符合规则设置的要求，直接将熔断器设置为CLOSED，关闭熔断器，不符合则还是改为OPEN状态
- 当处于HALF_OPEN状态时，直接拒绝访问资源

一般来说，熔断降级其实是对于服务的调用方来说的。<br />在项目中会经常调用其它服务或者是第三方接口，而对于这些接口，一旦它们出现不稳定，就有可能导致自身服务长时间等待，从而出现响应延迟等等问题。<br />此时服务调用方就可基于熔断降级方式解决。<br />一旦第三方接口响应时间过长，那么就可以使用慢调用比例规则，当出现大量长时间响应的情况，那么就直接熔断，不去请求。<br />虽然说熔断降级是针对服务的调用方来说，但是Sentinel本身并没有限制熔断降级一定是调用其它的服务。
<a name="SskB5"></a>
## 总结
通过分析之后，再回头看看Sentinel的简介的内容，其实就能更好地理解Sentinel的定位和拥有的强大功能。<br />Sentinel核心就是一堆统计数据和基于这些统计数据实现的流控和熔断的功能，源码并不复杂，而且Sentinel的代码写得非常好。
