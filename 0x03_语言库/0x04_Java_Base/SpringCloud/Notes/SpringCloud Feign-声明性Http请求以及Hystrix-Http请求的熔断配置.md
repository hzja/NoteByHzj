Java SpringCloud Feign
<a name="VdWdi"></a>
## 1、OpenFeign
<a name="0gD3D"></a>
### 1.1 概述
– Feign是一个声明性web服务客户机。它使编写web服务客户机变得更容易。<br />– 它的使用方法是定义一个服务接口并在上面添加注解。<br />– Feign支持可插拔编码器和解码器。<br />– Spring Cloud对`Feign`进行了封装，使其支持SpringMVC标准注解和`HttpttpMessageConverters`。Feign可以与Eureka和Ribbon组合使用以支持负载均衡。
<a name="O9wnS"></a>
### 1.2 Feign的作用
Feign旨在使编写Java Http客户端变得更容易。<br />前面在使用`Ribbon+RestTemplate`时，利用`RestTemplate`对http请求的封装处理，形成了一套模版化的调用方法。但在实际开发中，由于对服务依赖的调用可能不止一处，往往一个接口可能被多处调用，所以通常都会针对每个微服务自行封装一些客户端类来包装这些依赖服务的调用。所以，Feign在此基础上做了进一步封装，由它来定义和实现依赖服务接口的定义。在Feign的实现下，只需要创建一个接口并使用注解的方式来配置它（以前是Dao接口上面标注Mapper注解，现在是一个微服务接口上标注一个Feign注解即可），即可完成对服务提供方的的接口绑定，简化了使用Spring Cloud Ribbon时，自动封装服务调用客户端的开发量。
<a name="tMOVB"></a>
### 1.3 Feign集成了Ribbon
利用Ribbon维护了Payment的服务列表信息，并且通过轮询实现了客户端的负载均衡。而与Ribbon不同的是，通过Feign只需要定义服务绑定接口且以声明式的方法，优雅而简单的实现了服务的调用。
<a name="8DC06"></a>
### 1.4 Feign和OpenFeign的区别
<a name="9Q9UM"></a>
#### Feign
Feign是SpringCloud组件中一个轻量级RESTful的HTTP服务客户端，Feign内置了Ribbon,用来做客户端负载均衡，去调用服务注册中心的服务。Feign的使用方式是：使用Feign的注解定义接口，调用这个接口，就可以调用服务注册中心的服务。
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-feign</artifactId>
</dependency>
```
<a name="4ZWnM"></a>
#### OpenFeign
OpenFeign是SpringCloud在Feign的基础上支持了SpringMVC的注解，如`@RequestMapping`等。OpenFeign 的`@FeignClient`可以解析SpringMVC的`@RequestMapping`注解下的接口，并通过动态代理的方式产生实现类，实现类中做负载均衡并调用其他服务。
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-openfeign</artifactId>
</dependency>
```
<a name="79ff33bd"></a>
## 2、Fegin的使用
<a name="bLK4B"></a>
### 在启动类上使用`@EnableFeignClients`注解开启Feign客户端，并注入RestTemplate
```java
package com.fcant.userservice;

import org.springframework.boot.SpringApplication;
import org.springframework.cloud.client.SpringCloudApplication;
import org.springframework.cloud.client.loadbalancer.LoadBalanced;
import org.springframework.cloud.openfeign.EnableFeignClients;
import org.springframework.context.annotation.Bean;
import org.springframework.web.client.RestTemplate;

/**
 * UserServiceApplication
 *
 * encoding:UTF-8
 * @author Fcant 9:54 2019/12/9
 */
@EnableFeignClients
@SpringCloudApplication
public class UserServiceApplication {

	public static void main(String[] args) {
		SpringApplication.run(UserServiceApplication.class, args);
	}

	@Bean
    @LoadBalanced
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }
}

```
<a name="4uuSs"></a>
### Fegin的Service层

- **在接口层使用注解**`**@FeignClient(name = "area-service")**`**指定请求的服务名**
- **请求中有参数需要通过**`**@PathVariable(name = "countryId")**`**注解的name属性指定才能解析反射值**
```java
package com.fcant.userservice.feign;

import com.fcant.userservice.bean.Country;
import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

/**
 * ICountryService
 * 通过@FeignClient注解指定Feign客户端，名称为要访问的服务名
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 10:24 2019/12/9
 */
@FeignClient(name = "area-service")
public interface ICountryService {

    /**
     * queryCountry
     * Feign中没有指定@PathVariable(name = "countryId")变量映射关系，必须通过name属性指定
     *
     * @param countryId 城市ID
     * @return com.fcant.userservice.bean.Country
     * @author Fcant 10:53 2019/12/9
     */
    @GetMapping("/country/{countryId}")
    Country queryCountry(@PathVariable(name = "countryId") long countryId);
}

```
<a name="rpeH4"></a>
### 接口层
接口层的调用和使用和平常的Controller没什么区别，把Service层注入即可
```java
package com.fcant.userservice.controller;

import com.fcant.userservice.bean.Country;
import com.fcant.userservice.bean.User;
import com.fcant.userservice.feign.ICountryService;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.List;

/**
 * FeignController
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 10:06 2019/12/9
 */
@RestController
@RequestMapping("/user")
public class FeignController {
    private static final List<User> USER_LIST;

    static {
        List<User> userList = new ArrayList<>();
        userList.add(new User(3L, "User 03", "男", "user03@fcant.com", 1L, ""));
        userList.add(new User(4L, "User 04", "女", "user04@fcant.com", 2L, ""));
        USER_LIST = userList;
    }

    private final ICountryService countryService;

    public FeignController(ICountryService countryService) {
        this.countryService = countryService;
    }


    @GetMapping("/feign")
    public List<User> queryUser() {
        List<User> userList = new ArrayList<>(USER_LIST);
        userList.forEach(user -> {
            Country country = countryService.queryCountry(user.getCountryId());
            user.setCountryName(country.getCountryName());
        });
        return userList;
    }
}

```
<a name="mvmNc"></a>
## 3、Fegin异常处理的回调
<a name="AnPM3"></a>
### 在接口的注解上添加fallback属性指定异常回调的类
```java
package com.fcant.userservice.feign;

import com.fcant.userservice.bean.Country;
import com.fcant.userservice.feign.impl.CountryServiceImpl;
import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

/**
 * ICountryService
 * 通过@FeignClient注解指定Feign客户端，名称为要访问的服务名
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 10:24 2019/12/9
 */
@FeignClient(name = "area-service", fallback = CountryServiceImpl.class)
public interface ICountryService {

    /**
     * queryCountry
     * Feign中没有指定@PathVariable(name = "countryId")变量映射关系，必须通过name属性指定
     *
     * @param countryId 城市ID
     * @return com.fcant.userservice.bean.Country
     * @author Fcant 10:53 2019/12/9
     */
    @GetMapping("/country/{countryId}")
    Country queryCountry(@PathVariable(name = "countryId") long countryId);
}

```
<a name="3AaX9"></a>
### 在指定的处理异常的类中实现接口的方法进行处理

- **该类实现**`**FeginClients**`**注解指定的接口**
- **该类添加**`@Component`**注解将其作为Bean注入**
```java
package com.fcant.userservice.feign.impl;

import com.fcant.userservice.bean.Country;
import com.fcant.userservice.feign.ICountryService;
import org.springframework.stereotype.Component;

/**
 * CountryServiceImpl
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 14:24 2019/12/9
 */
@Component
public class CountryServiceImpl implements ICountryService {

    private static final Country ERROR = new Country(1L, "ERROR");
    /**
     * queryCountry
     * Feign中没有指定@PathVariable(name = "countryId")变量映射关系，必须通过name属性指定
     *
     * @param countryId 城市ID
     * @return com.fcant.userservice.bean.Country
     * @author Fcant 10:53 2019/12/9
     */
    @Override
    public Country queryCountry(long countryId) {
        return ERROR;
    }
}

```
<a name="OArxo"></a>
### Fegin的异常处理属于熔断的范围，要在配置文件配置开启熔断-application.yml

```yaml
feign:
  hystrix:
    enabled: true
```
<a name="eA8Dp"></a>
## 4、关于Feign访问超时的配置
如果不配置访问超时则会报如下异常
```yaml
feign.RetryableException: Read timed out executing POST http://******
```
因为Feign接口调用分两层，`Ribbon的调用`和`Hystrix调用`，理论上`设置Ribbon的时间`即可，但是Ribbon的超时时间和Hystrix的超时时间需要结合起来，按照`木桶原则`最低的就是Feign的超时时间，建议最好配置`超时时间一致`。经过配置一下application设置后，完美解决了问题。因为第三方接口中需要3~20秒不等的时间，所以这个数值也是根据自己的业务系统情况设置的。
```yaml
# hystrix的超时时间
hystrix:
    command:
        default:
            execution:
              timeout:
                enabled: true
              isolation:
                    thread:
                        timeoutInMilliseconds: 30000
# ribbon的超时时间-设置feign客户端超时时间（openfeign默认支持ribbon）
ribbon:
	# 指的是建立连接所用的时间，适用于网络状况正常的情况下，两端连接所用的时间
  ReadTimeout: 30000
  # 指的是建立连接后从服务器读取到可用资源所用的时间
  ConnectTimeout: 30000
```
<a name="9ZAMc"></a>
## 5、OpenFeign日志增强
Feign提供了日志打印功能，可以通过配置来调整日志级别，从而了解Feign中Http请求的细节。对Feign接口的调用情况进行检控和输出。

- `NONE`：默认的，不显示任何日志。
- `BASIC`：仅记录请求方法、URL、响应状态吗即执行时间。
- `HEADERS`：除了BASIC中定义的信息之外，还有请求和响应的头信息。
- `FULL`：除了HEADERS中定义的信息之外，还有请求和响应的正文及元数据。
<a name="MDto2"></a>
### （1）添加配置类指明日志打印级别
```java
import feign.Logger;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
@Configuration
public class FeignConfig {
    @Bean
    Logger.Level feignLoggerLevel(){
        return Logger.Level.FULL;
    }
}
```
<a name="91e02f1c"></a>
### （2）application.yml/properties配置文件开启日志的Feign客户端
```yaml
logging:
  level:
    #feign日志以什么级别监控哪个接口
    com.fcant.springcloud.service.PaymentFeignService: debug
```
