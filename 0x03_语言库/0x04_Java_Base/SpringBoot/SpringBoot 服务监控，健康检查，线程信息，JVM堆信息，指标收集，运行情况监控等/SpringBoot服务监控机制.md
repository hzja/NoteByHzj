JavaSpringBoot
<a name="ILDy5"></a>
## 1、前言
任何一个服务如果没有监控，那就是两眼一抹黑，无法知道当前服务的运行情况，也就无法对可能出现的异常状况进行很好的处理，所以对任意一个服务来说，监控都是必不可少的。<br />就目前而言，大部分微服务应用都是基于 SpringBoot 来构建，所以了解 SpringBoot 的监控特性是非常有必要的，而 SpringBoot 也提供了一些特性来监控应用。<br />本文基于 SpringBoot 2.3.1.RELEASE 版本演示。
<a name="tURl3"></a>
## 2、SpringBoot 监控
SpringBoot 中的监控可以分为 HTTP 端点和 JMX 两种方式来监控当前应用的运行状况和指标收集
<a name="IffB0"></a>
## 3、HTTP Endpoints 监控
执行器端点允许监视应用程序并与之交互。SpringBoot 包括许多内置的端点，并允许添加自己的端点。可以通过 HTTP 或 JMX 启用或禁用每个端点，并公开（使其可以远程访问）。每个端点都有一个唯一的 id，访问时可以通过如下地址进行访问：http:ip:port/{id}（SpringBoot 1.x ），而在 SpringBoot 2.x 版本中，默认新增了一个 /actuator 作为基本路，访问地址则对应为 :http:ip:port/actuator/{id}。<br />使用 HTTP 监控非常简单，在 SpringBoot 项目中，引入如下依赖：
```xml
<dependencies>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
  </dependency>
</dependencies>
```
默认就可以通过地址 http:localhost:8080/actuator/health，访问之后得到如下结果：<br />![2022-12-01-13-51-05.082819800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874262143-b1f3018c-cb23-485f-9838-d0bae88128fe.png#averageHue=%23e4bb90&clientId=u7e298f34-3740-4&from=ui&id=u6c76395e&originHeight=89&originWidth=547&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3214&status=done&style=none&taskId=u962aa2c7-c6ed-4e9e-bb46-4bd45dea43c&title=)<br />SpringBoot 中提供了非常多的默认端点监控，但是出于安全考虑，默认情况下有些端点并不是开启状态，如 shutdown 端点就是默认关闭的。
<a name="rG6YD"></a>
### 内置端点
SpringBoot 中默认提供的常用内置端点如下：

| 端点 id | 描述 |
| --- | --- |
| auditevents | 公开当前应用程序的审计事件信息，需要 AuditEventRepository Bean。 |
| beans | 展示程序中所有的 Bean。 |
| caches | 公开可用的缓存。 |
| conditions | 展示配置类或者自动装配类中的条件，以及它们匹配或者不匹配的原因。 |
| configprops | 显示所有 `@ConfigurationProperties` 中的配置属性。 |
| env | 显示 `ConfigurableEnvironment` 中的所有环境。 |
| health | 显示应用程序运行状况信息。 |
| httptrace | 显示 HTTP 跟踪信息（默认情况下统计最近 100 次请求），需要 HttpTraceRepository Bean。 |
| info | 显示任意程序信息。 |
| integrationgraph | 显示 Spring 集成图，需要依赖 spring-integration-core。 |
| loggers | 展示和修改应用中的 loggers 配置。 |
| metrics | 展示当前应用监控指标的度量。 |
| mappings | 展示所有 `@RequestMapping` 路径。 |
| scheduledtasks | 展示应用中的所有定时任务信息。 |
| sessions | 允许从 Spring 会话支持的会话存储中检索和删除用户会话。需要使用基于 Spring Session web应用程序。 |
| shutdown | 优雅的关闭程序，默认禁止了该端点的访问。 |

虽然说这里的大部分端点都是默认开启的，但是默认暴露（允许对外访问）的只有 health 和 info 端点，所以如果需要允许端点对外暴露，可以通过如下配置（如果想要暴露所有的端点，则可以直接配置 "*" ）：
```yaml
management:
  endpoints:
    web:
      exposure:
        include: [health,info,mappings] //或者直接配置 "*"
```
另外，开启或禁用某一个端点，也可以通过通过如下配置进行动态控制：
```
management.endpoint.<id>.enabled=true
```
接下来挑选几个重点的端点来介绍一下。
<a name="fmiia"></a>
### health 端点
health 断点默认只是展示当前应用健康信息，但是可以通过另一个配置打开详细信息，这样不仅仅会监控当前应用，还会监控与当前应用相关的其他第三方应用，如 Redis。
```yaml
management:
  endpoint:
    health:
      show-details: always
```
这个配置打开之后，连接上 Redis 之后再次访问 health 端点，就可以展示 Redis 服务的健康信息了：![2022-12-01-13-51-05.140491600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874262136-ea3694cb-ecfc-4e7a-b19b-12498dc3fbbd.png#averageHue=%23fdfdfd&clientId=u7e298f34-3740-4&from=ui&id=TOOnK&originHeight=426&originWidth=851&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9328&status=done&style=none&taskId=u251c01ad-1fc1-4634-877a-f6a91eaf92f&title=)
<a name="DQc0s"></a>
### loggers 端点
访问 http://localhost:8080/actuator/loggers 可以查看当前应用的日志级别等信息：<br />![2022-12-01-13-51-05.183182700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874296849-5197c38a-e173-4fbb-a8ce-93ac35f6e6cf.png#averageHue=%23fdfcfc&clientId=u7e298f34-3740-4&from=ui&id=u42b2ddbd&originHeight=671&originWidth=929&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17719&status=done&style=none&taskId=ua7ade999-95a3-492a-b2df-960ec390732&title=)<br />这里面本身并不特别，但是有一个功能却非常有用，比如生产环境日志级别一般都是 info，但是现在有一个 bug 通过 info 级别无法排查，那么就可以临时修改 log 级别。<br />比如上图中的 ROOT 节点是 info 级别，那么可以通过 postman 等工具来发一个 post 请求修改日志级别。![2022-12-01-13-51-05.458029500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874296848-8f8ec567-b770-40fe-8303-eee631ac4a5c.png#averageHue=%23f9f5f4&clientId=u7e298f34-3740-4&from=ui&id=CggQG&originHeight=272&originWidth=953&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12698&status=done&style=none&taskId=u283327f4-7460-47a8-8564-bf46481e451&title=)修改之后就会发现，日志由原来的 info 变成了 debug：<br />![2022-12-01-13-51-05.662870000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874311730-a0bdabdb-1dd9-4824-9644-289a89b03eae.png#averageHue=%23f8f3f0&clientId=u7e298f34-3740-4&from=ui&id=ub71b1293&originHeight=312&originWidth=1164&originalType=binary&ratio=1&rotation=0&showTitle=false&size=98005&status=done&style=none&taskId=uc56cc269-7dda-43c5-b1b1-9dd6dbfaead&title=)
<a name="B3ySR"></a>
### metrics 端点
metrics 是一个非常重要的监控端点，其监控内容覆盖了 JVM 内存、堆、类加载、处理器和 tomcat 容器等一些重要指标：![2022-12-01-13-51-05.893601700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874311718-66d3597d-3c72-4043-aab9-d9a8d5743b75.png#averageHue=%23fefefe&clientId=u7e298f34-3740-4&from=ui&id=ejvH1&originHeight=624&originWidth=1069&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18772&status=done&style=none&taskId=u8e0f6c36-84e9-4398-8e0c-74c2a9dc075&title=)可以看到这里面包含了非常多的指标，任意访问一个指标就可以查看对应的指标信息：![2022-12-01-13-51-06.064727800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874311721-d5d57db4-1b92-42b7-b09f-29bb793433f0.png#averageHue=%23fdfdfc&clientId=u7e298f34-3740-4&from=ui&id=BUpTT&originHeight=252&originWidth=826&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8359&status=done&style=none&taskId=u6a2bdd4b-3432-4bb8-94ac-d22d6e02bba&title=)
<a name="ZFykP"></a>
## 4、自定义监控端点
通过上面的介绍，可以看到 SpringBoot 提供的监控非常强大，但是就算再全面的监控也不可能满足所有人的需求，所以 SpringBoot 也支持自定义监控端点。自定义一个监控端点主要有如下常用注解：

- `@Endpoint`：定义一个监控端点，同时支持 HTTP 和 JMX 两种方式。
- `@WebEndpoint`：定义一个监控端点，只支持 HTTP 方式。
- `@JmxEndpoint`：定义一个监控端点，只支持 JMX 方式。

以上三个注解作用在类上，表示当前类是一个监控端点，另外还有一些注解会用在方法和参数上：

- `@ReadOperation`：作用在方法上，可用来返回端点展示的信息（通过 Get 方法请求）。
- `@WriteOperation`：作用在方法上，可用来修改端点展示的信息（通过 Post 方法请求）。
- `@DeleteOperation`：作用在方法上，可用来删除对应端点信息（通过 Delete 方法请求）。
- `@Selector`：作用在参数上，用来定位一个端点的具体指标路由。

来，一起写一个自己的监控端点

- 定义一个类，并使用 `@Endpoint` 注解标注标识，同时定义几个方法用 `@ReadOperation` 和 `@WriteOperation` 注解来标注
```java
@Endpoint(id="myEndpoint")
@Component
public class MyEndpoint {
    private String STATUS = "up";
    private String DETAIL = "一切正常";

    //    @ReadOperation
    //    public String test1(){
    //        return "wolf";
    //    }

    //    @ReadOperation
    //    public Map<String,String> test2(){
    //        Map<String,String> map = new HashMap();
    //        map.put("status","up");
    //        return map;
    //    }

    @ReadOperation
    public JSONObject test3(){
        JSONObject jsonObject= new JSONObject();
        jsonObject.put("status",STATUS);
        jsonObject.put("detail",DETAIL);
        return jsonObject;
    }

    @ReadOperation
    public JSONObject test3_1(@Selector String name){
        JSONObject jsonObject= new JSONObject();
        if ("status".equals(name)){
            jsonObject.put("status",STATUS);
        }else if ("detail".equals(name)){
            jsonObject.put("detail",DETAIL);
        }
        return jsonObject;
    }

    @WriteOperation//动态修改指标
    public void test4(@Selector String name,@Nullable String value){
        if (!StringUtils.isEmpty(value)){
            if ("status".equals(name)){
                STATUS = value;
            }else if ("detail".equals(name)){
                DETAIL = value;
            }
        }
    }
}
```

1. `@Component` 注解表示将该类交给 Spring 进行管理，或者也可以再定义一个 Configuration 类来加载该 Bean 也可以，当然，如果需要提供给第三方使用，如果无法保证当前包名被扫描，则需要使用 SpringBoot 的自动装配机制将该类进行管理。
2. `@ReadOperation` 方法可以返回 String 或者 JSONObject 或者 Map 集合等。
3. 参数上加了 `@Selector` 注解则表示访问断端点的时候可以直接访问子节点。

完成了上面的类，启动 SpringBoot 应用，接下来就可以直接通过 http://localhost:8080/actuator/myEndpoint 进行访问了![2022-12-01-13-51-06.210844600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874354575-77dc8faa-d525-417f-a297-821ddadc0d76.png#averageHue=%23e5c59c&clientId=u7e298f34-3740-4&from=ui&id=u3a323a31&originHeight=124&originWidth=756&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4253&status=done&style=none&taskId=ucb203574-5845-488c-b6fc-fbd87b10c1c&title=)<br />同时，因为 test3_1 方法使用了 `@Selector` 注解，所以可以通过这个方法每一个指标的明细：![2022-12-01-13-51-06.270319300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874354581-62a0f038-13ed-4f24-83ad-ea7a57d3eecc.png#averageHue=%23fefcfb&clientId=u7e298f34-3740-4&from=ui&id=HkLeV&originHeight=108&originWidth=861&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4211&status=done&style=none&taskId=u255f12ec-545d-4604-ad30-2a61866458f&title=)而带有 `@WriteOperation` 注解的方法可以用来修改指标，这个方法需要用 post 进行访问，访问的参数可以直接使用字符串传参，也可以直接使用 json 进行传参，修改之后再次查看就可以发现指标已经被动态修改：![2022-12-01-13-51-06.369401200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874354583-cbd7b0cd-8b07-49a9-9d41-2cdd01e90cf9.png#averageHue=%23f8f5f5&clientId=u7e298f34-3740-4&from=ui&id=p30z1&originHeight=191&originWidth=952&originalType=binary&ratio=1&rotation=0&showTitle=false&size=11992&status=done&style=none&taskId=udcb0c5cd-ed94-40d1-81a5-c39d0ab1f81&title=)
<a name="qGHVd"></a>
## 5、JMX 监控
JMX 全称为 Java Management Extensions，即 Java 管理扩展。它提供了对 Java 应用程序和 JVM 的监控管理。通过JMX 可以监控服务器中各种资源的使用情况以及线程，内存和 CPU 等使用情况。<br />打开 jdk 下提供的工具 jConsole：<br />![2022-12-01-13-51-06.504040400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874408068-dbee77cd-9453-4073-a7cd-1d9aaad5a2bb.png#averageHue=%23edeceb&clientId=u7e298f34-3740-4&from=ui&id=u88affe21&originHeight=467&originWidth=395&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29151&status=done&style=none&taskId=u51548283-7ad0-42b3-a38d-3b211ed0dd0&title=)<br />打开之后这里会监控到已经启动的应用，双击进入：![2022-12-01-13-51-06.694483000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874408103-0945e7e1-64da-4792-8307-b9fa43f61b48.png#averageHue=%23f3f0ef&clientId=u7e298f34-3740-4&from=ui&id=HIuew&originHeight=564&originWidth=873&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42495&status=done&style=none&taskId=u7baada25-11ad-4793-84a2-61b4779ddf4&title=)
<a name="m1CjO"></a>
### 如何手动注册一个 JMX MBean
定义一个接口 `SystemInfoMBean`（注意名字必须要用 MBean 结尾）:
```java
public interface SystemInfoMBean {
    int getCpuCore();
    long getTotalMemory();
    void shutdown();
}
```
再定义一个类实现 SystemInfoMBean 接口，实现类的明明方式为接口名去掉 MBean：
```java
public class SystemInfo implements SystemInfoMBean {
    @Override
    public int getCpuCore() {
        return Runtime.getRuntime().availableProcessors();
    }
    @Override
    public long getTotalMemory() {
        return Runtime.getRuntime().totalMemory();
    }

    @Override
    public void shutdown() {
        System.exit(0);
    }
}
```
最后就是需要将该实现类进行注册：
```java
public class JmxRegisterMain {
    public static void main(String[] args) throws NotCompliantMBeanException, InstanceAlreadyExistsException, MBeanRegistrationException, MalformedObjectNameException, IOException {
        MBeanServer mBeanServer= ManagementFactory.getPlatformMBeanServer();
        ObjectName objectName=new ObjectName("com.lonely.wolf.note.springboot.actuator.jmx:type=SystemInfo");
        SystemInfo SystemInfo =new SystemInfo();
        mBeanServer.registerMBean(SystemInfo,objectName);//注册
        System.in.read();//防止程序结束
    }
}
```
运行该 main 方法，再打开 jConsole 就可以看到成功注册了一个 MBean：![2022-12-01-13-51-06.893635600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669874408070-357b323d-5c04-40e2-b7d7-e71709a6b60d.png#averageHue=%23f4f0f0&clientId=u7e298f34-3740-4&from=ui&id=jKcPl&originHeight=513&originWidth=873&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27646&status=done&style=none&taskId=u7a449621-d95f-4e57-b2f3-869d82d5748&title=)同样的，Spring 当中只要使用了 `@Endpoint` 或者 `@JmxEndpoint` 注解，就会自动注册一个 MBean，其原理也是利用了自动装配机制。
<a name="jD0Q1"></a>
## 6、其他监控
除了 SpringBoot 自带的监控之外，也有其他第三方开源的强大监控系统，如 Prometheus，而且 SpringBoot 也将其进行了集成，使用 Prometheus 时只需要引入如下 jar 包即可：
```xml
<dependency>
  <groupId>io.micrometer</groupId>
  <artifactId>micrometer-registry-prometheus</artifactId>
</dependency>
```
当然，如果使用 Prometheus 的话需要单独安装，而且一般会选择 Prometheus + Grafana 来共同实现一个监控平台。
