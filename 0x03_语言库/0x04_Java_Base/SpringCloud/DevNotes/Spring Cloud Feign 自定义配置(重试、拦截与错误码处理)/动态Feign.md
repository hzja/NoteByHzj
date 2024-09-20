JavaFeign<br />Feign在微服务框架中使得服务直接的调用变得很简洁、简单，而不需要再编写Java Http调用其他微服务的接口。
<a name="HrSq1"></a>
## 动态feign
对于fegin调用，一般的用法：为每个微服务都创建对应的feignclient接口，然后为每个微服务的controller接口，一一编写对应的方法，去调用对应微服务的接口。<br />例如下面这样：
```java
//system
@FeignClient(name = "system")
public interface SystemClient {
    @GetMapping("/system/test1")
    JsonResult test1(String test1);
    
    @GetMapping("/system/test2")
    JsonResult test2(String test2);
    
    ....
}

//user
@FeignClient(name = "user")
public interface UserClient {
    @GetMapping("/user/test1")
    JsonResult test1(String test1);
    
    @GetMapping("/user/test2")
    JsonResult test2(String test2);
    
    ....
}
```
这样写的话，可能会有些累赘，那么能不能创建一个动态的feign；当调用sytem微服务的时候，传递一个feignclient的name为system进去，然后定义一个通用的方法，指定调用的url，传递的参数，就可以了呢？<br />答案是可以的！！！^_^<br />定义一个通用的接口，通用的get，post方法
```java
public interface DynamicService {
    
    @PostMapping("{url}")
    Object executePostApi(@PathVariable("url") String url, @RequestBody Object params);

    @GetMapping("{url}")
    Object executeGetApi(@PathVariable("url") String url, @SpringQueryMap Object params);
}
```
executePostApi：(post方法)

- url，表示要调用微服务的接口url，一般来说是对应controller接口的url；
- params，为调用该接口所传递的参数，这里加了`@RequestBody`，那对应的controller接口，接收参数也需要加上该注解。

定义一个动态feignclient
```java
@Component
public class DynamicClient {

    @Autowired
    private DynamicFeignClientFactory<DynamicService> dynamicFeignClientFactory;

    public Object executePostApi(String feignName, String url, Object params) {
        DynamicService dynamicService = dynamicFeignClientFactory.getFeignClient(DynamicService.class, feignName);
        return dynamicService.executePostApi(url, params);
    }

    public Object executeGetApi(String feignName, String url, Object params) {
        DynamicService dynamicService = dynamicFeignClientFactory.getFeignClient(DynamicService.class, feignName);
        return dynamicService.executeGetApi(url, params);
    }
}
```
executePostApi：(post方法)

- feignName，表示需要调用的微服务的名称，一般对应application.name，例如：system
- url，表示要调用微服务的接口url，一般来说是对应controller接口的url；
- params，为调用该接口所传递的参数，这里加了`@RequestBody`，那对应的controller接口，接收参数也需要加上该注解。

定义一个动态feignclient工厂类
```java
@Component
public class DynamicFeignClientFactory<T> {

    private FeignClientBuilder feignClientBuilder;

    public DynamicFeignClientFactory(ApplicationContext appContext) {
        this.feignClientBuilder = new FeignClientBuilder(appContext);
    }

    public T getFeignClient(final Class<T> type, String serviceId) {
        return this.feignClientBuilder.forType(type, serviceId).build();
    }
}
```
主要的作用：是动态的创建一个feignclient对象<br />具体的操作步骤，就是上面所说的了！！！是不是很通用了呢？<br />通用是通用了，那怎么玩呢（如何使用）？<br />使用的方式，也是十分的简单啦：^_^
```java
DynamicClient dynamicClient = SpringUtil.getBean(DynamicClient.class);
Object result = dynamicClient.executePostApi("system", "/system/test", new HashMap<>());
System.out.println("==========>"+JSONObject.toJSONString(result));
```
先获取到`DynamicClient`对象，然后直接调用`executePostApi`方法

- "system"，表示调用微服务的名称，一般对应application.name
- "/system/test"，表示调用的url
- `new HashMap<>()`，为需要传递的参数

这样就实现了一个通用版的feignclient，就可以愉快的编写代码了！！！^_^
