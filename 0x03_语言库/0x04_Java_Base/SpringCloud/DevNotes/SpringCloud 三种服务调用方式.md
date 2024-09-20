JavaSpringCloud<br />主要介绍SpringCloud中三种服务调用方式：

- **Spring DiscoveryClient**
- **支持Ribbon的RestTemplate**
- **Feign客户端**
<a name="gCe7V"></a>
## 搭建服务测试环境
测试中，服务发现层采用Netflix的Eureka搭建。<br />主要步骤如下：
<a name="Zey0M"></a>
### 1、引入Eureka所需依赖
```xml
<!--eureka服务端-->
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-eureka-server</artifactId>
</dependency>
<!--客户端-->
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-eureka</artifactId>
</dependency>
```
<a name="BzEoB"></a>
### 2、修改配置文件
服务端：
```yaml
eureka:
 instance:
  hostname: eureka9001.com #eureka服务端的实例名称
  instance-id: eureka9001
client:
  register-with-eureka: false #false表示不向注册中心注册自己
  fetch-registry: false # #false 表示自己就是注册中心，职责就是维护服务实例，并不需要去检索服务
  service-url:
  defaulteZone: http://127.0.0.1:9001
```
客户端1：
```yaml
server:
  port: 8002
spring:
  application:
    name: licensingservice
eureka:
  instance:
    instance-id: licensing-service-8002
    prefer-ip-address: true
  client:
    register-with-eureka: true
    fetch-registry: true
    service-url:
      defaultZone: http://127.0.0.1:9001/eureka/,
```
客户端2：
```yaml
server:
 port: 8002
spring:
 application:
   name: licensingservice
eureka:
 instance:
   instance-id: licensing-service-8002
   prefer-ip-address: true
 client:
   register-with-eureka: true
   fetch-registry: true
   service-url:
     defaultZone: http://127.0.0.1:9001/eureka/,
```
一组微服务的不同实例采服务名相同，不同的实例Id区分，分别对应，`spring.application.name` 和`eureka.instance.instance-id`。
<a name="rHfP2"></a>
### 3、启动服务
服务端：
```java
@SpringBootApplication
@EnableEurekaServer
public class EurekaServerPort9001_App {
    public static void main(String[] args) {
        SpringApplication.run(EurekaServerPort9001_App.class,args);
    }
}
```
客户端：
```java
@SpringBootApplication
@RefreshScope
@EnableEurekaClient
public class LicenseApplication_8002 {
    public static void main(String[] args) {
        SpringApplication.run(LicenseApplication_8002.class, args);
    }
}
```
<a name="hGjeY"></a>
### 4、测试
进入到Eureka服务端地址，这是127.0.0.1:9001，可以查看注册到注册中心的服务。<br />如图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662270430904-05295c9f-d363-46f1-ab4e-db78ba10caa6.png#clientId=ud2282ec6-75b4-4&from=paste&id=uc30670ea&originHeight=395&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u627bc501-a399-4207-8cb4-b27fb8abb8b&title=)<br />注意事项：Eureka通过三次心跳检测均通过，服务才会成功注册到注册中心，默认每次间隔10s，及初次启动服务需等待30s才能在Eureka中看到注册服务。
<a name="qoKcL"></a>
## 消费者搭建
<a name="SUWf9"></a>
### 1、Discover Client方式
微服务中服务既是消费者也可以是调用者，因此消费者配置和上面服务配置大体一致，依赖及配置参考上面服务端搭建方式。启动主类添加`@EnableEurekaClient`注释：
```java
@SpringBootApplication
@EnableEurekaClient
@EnableDiscoveryClient
public class ConsumerApplication_7002 {
    public static void main(String[] args) {
        SpringApplication.run(ConsumerApplication_7002.class, args);
    }
}
```
核心配置类：
```java
@Component
public class ConsumerDiscoveryClient {

    @Autowired
    private DiscoveryClient discoveryClient;

    public ServiceInstance getServiceInstance() {
        List<ServiceInstance> serviceInstances = discoveryClient.getInstances("licensingservice");
        if (serviceInstances.size() == 0) {
            return null;
        }
        return serviceInstances.get(0);
    }

    public String getUrl(String url) {
        ServiceInstance serviceInstance=this.getServiceInstance();
        if (serviceInstance==null)
            throw new RuntimeException("404 ,NOT FOUND");
        String urlR=String.format(url,serviceInstance.getUri().toString());
        return urlR;
    }
}
```
通过`DiscoveryClient`从Eureka中获取`licensingservice`服务的实例数组，并返回第一个实例。<br />测试Controller
```java
@RestController
@RequestMapping("test")
public class TestController {
    //注意必须new，否则会被ribbon拦截器拦截，改变URL行为
    private RestTemplate restTemplate=new RestTemplate();
    @Autowired
    private ConsumerDiscoveryClient consumerDiscoveryClient;
    @RequestMapping("/getAllEmp")
    public List<Emp> getAllLicense(){
        String url=consumerDiscoveryClient.getUrl("%s/test/getAllEmp");
        return restTemplate.getForObject(url,List.class);
    }
}
```
测试：

1. **调试信息**

![2022-09-04-13-44-57.095805700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662270771017-3fc9ca3a-5184-487a-9967-403f0e731944.png#clientId=ud2282ec6-75b4-4&from=ui&id=ucd584661&originHeight=644&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2090651&status=done&style=shadow&taskId=u489f095b-43d9-4f03-8a35-5b6788fb2c4&title=)<br />从该图可以直观看到`licensingservice`，拥有两个服务实例，并可以查看实例信息。

1. **页面返回信息**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662270430939-2656b0a9-dbbb-4bb8-8603-c3917edb8001.png#clientId=ud2282ec6-75b4-4&from=paste&id=uf4d7304d&originHeight=159&originWidth=311&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u50a5c71e-e0f0-4498-ab72-8c304b8285c&title=)<br />成功查询到数据库存储信息。
<a name="tAbNP"></a>
### 2、Ribbon功能的Spring RestTemplate方式
依赖同上。<br />核心配置类：
```java
//指明负载均衡算法
@Bean
public IRule iRule() {
	return new RoundRobinRule();
}

@Bean
@LoadBalanced //告诉Spring创建一个支持Ribbon负载均衡的RestTemplate
public RestTemplate restTemplate() {
	return new RestTemplate();
}
```
设置负载均衡方式为轮询方式。<br />测试类：
```java
@RestController
@RequestMapping("rest")
public class ConsumerRestController {

    @Autowired
    private RestTemplate restTemplate;
    private final static String SERVICE_URL_PREFIX = "http://LICENSINGSERVICE";

    @RequestMapping("/getById")
    public Emp getById(Long id) {
        MultiValueMap<String, Object> paramMap = new LinkedMultiValueMap<String, Object>();
        paramMap.add("id", id);
        return restTemplate.postForObject(SERVICE_URL_PREFIX + "/test/getById", paramMap, Emp.class);
    }
}
```
测试结果：

- **第一次：**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662270430909-9b7c3eaa-fcfe-4a36-8fd8-c46f222199d5.png#clientId=ud2282ec6-75b4-4&from=paste&id=u9d8526c9&originHeight=131&originWidth=372&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1c536342-f252-44aa-b040-4a144140478&title=)

- **第二次：**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662270430911-8630a3f3-4170-4755-a72c-0a98f4bbea04.png#clientId=ud2282ec6-75b4-4&from=paste&id=u816801e8&originHeight=143&originWidth=436&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue9e202f7-bfd4-412f-a825-3e76f242be5&title=)

- **第三次：**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662270431221-677ef4be-eea4-441a-b467-70f383a86eb3.png#clientId=ud2282ec6-75b4-4&from=paste&id=u32494830&originHeight=131&originWidth=372&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5c287133-4845-4992-bae8-732accf2a64&title=)<br />因为采用轮询负载均衡方式分别调用不同服务实例，未区别，将name做出了一定更改。<br />以上两种方式对比，Ribbon方式是对第一种方式的封装且内置不同的负载算法，支持自定义。使用更加简单，但此两次均需编写RestTemplate的请求方法，较为繁琐且容易出错，第三种方式Feign客户端则极大的降低了开发难度和提升速度。
<a name="h6Pbd"></a>
### 3、feign客户端方式
引入依赖：
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-feign</artifactId>
</dependency>
```
主要代码：
```java
@FeignClient(value = "LICENSINGSERVICE",fallbackFactory = ServiceImp.class)
public interface ServiceInterface {

    @RequestMapping("/test/getById")
    Emp getById(@RequestParam("id") Long id);

    @RequestMapping("/test/getLicenseById")
    License getLicenseById(@RequestParam("id") Long id);

    @RequestMapping("/test/getAllEmp")
    List<Emp> getAllLicense();
}
```
```java
@Component
public class ServiceImp implements FallbackFactory<ServiceInterface> {

    @Override
    public ServiceInterface create(Throwable throwable) {
        return new ServiceInterface() {
            @Override
            public Emp getById(Long id) {
                Emp emp = new Emp();
                emp.setName("i am feign fallback create");
                return emp;
            }

            @Override
            public License getLicenseById(Long id) {
                return null;
            }

            @Override
            public List<Emp> getAllLicense() {
                return null;
            }
        };
    }
}
```
采用接口模式开发，通过注解指明服务名以及后备方法，在服务表现不佳时，方便返回默认的结果，而不是一个不友好的错误。<br />主启动类：
```java
@SpringBootApplication
@EnableEurekaClient
@EnableFeignClients
public class Consumer_feign_Application_7004 {
    public static void main(String[] args) {
        SpringApplication.run(Consumer_feign_Application_7004.class, args);
    }
}
```
测试类：
```java
@RestController
@RequestMapping("rest")
public class ConsumerRestController {
    @Autowired
    private ServiceInterface serviceInterface;

    @Autowired
    private RestTemplate restTemplate;
    @RequestMapping("/getById")
    public Emp getById(Long id) {
        return serviceInterface.getById(id);
    }
}
```
测试结果：

- **正常测试：**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662270431199-197457a8-57c8-4135-b299-7a7ade66d293.png#clientId=ud2282ec6-75b4-4&from=paste&id=ue0882da8&originHeight=131&originWidth=372&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1a273d11-8088-46e6-bf0d-a91a6f31082&title=)

- **关闭两个服务实例，模拟服务实例死亡：**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662270431213-7462efa1-6fc6-42d4-a2b0-9e64d2e6fa63.png#clientId=ud2282ec6-75b4-4&from=paste&id=u440b55ef&originHeight=150&originWidth=451&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf343cb92-7666-4f59-b823-b00c68aecaa&title=)<br />Feign除了能简化服务调用，也可以实现当调用的服务失败时，友好的反馈信息。<br />此三种调用方式，由低至上，从不同层次实现了SpringCloud中的微服务互相调用。
