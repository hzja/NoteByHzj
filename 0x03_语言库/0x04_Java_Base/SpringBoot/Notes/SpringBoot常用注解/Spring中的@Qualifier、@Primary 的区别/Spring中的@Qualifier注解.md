JavaSpring<br />`@Qualifier`注解通常有 4 种用法，第 1 种比较常见，会另外 3 种的都是高手了，下面来细看下。
<a name="rC20h"></a>
## 第 1 种：从多个候选 bean 中选择一个
<a name="prTSi"></a>
### 案例代码
如下代码，先来看看其特点：

- IService 接口有 2 个实现类：Service1 和 Service2，这俩上使用了`@Component`注解，将其注册到 spring 容器
- Service3 中IService service标注有`@Autowired`注解，需要自动注入IService类型的一个 bean
```java
public interface IService {
}

@Component
public class Service1 implements IService {
}

@Component
public class Service2 implements IService {
}

@Component
public class Service3 {
    @Autowired
    private IService service;
}
```
<a name="NVOt6"></a>
### 这段代码有问题么？
Service3 中需要注入 service，此时会按照类型 IServce 从容器中找配置的 bean，找到了 2 个满足的：service1 和 service2，到底注入哪个呢？<br />spring 不知道如何选择了，此时可以调整一下 Service3 的代码，如下，使用`@Qualifier("service2")`来限定候选 bean 的名称，此时注入的是service2，这种用法想必很多人都很熟悉，下面再来看另外 2 种。
```java
@Component
public class Service3 {
    @Autowired
    @Qualifier("service2")
    private IService service;
}
```
<a name="VEQtS"></a>
## 第 2 种：标注在类上，对 bean 进行分组
注意看下面代码：

- IService 接口有 3 个实现类，都标注了@Component 注解
- Service2 和 Service3 上添加了`@Qualifier("group1")`注解，大家可以理解为给这 3 个 bean 打了一个标签，标签名字叫group1
- ServiceA 中的`List<IService> serviceList`上使用了`@Autowired`注解，表示从 spring 容器中查找 IService 类型 bean，注入给serviceList，此时容器中满足条件的有 3 个，3 个都会注入么？当然不是啦，serviceList上多了一个`@Qualifier("group1")`注解，这个会对候选的 bean 进行过滤，满足条件的就只有 service2 和 service3 了
```java
public interface IService {
}

@Component
public class Service1 implements IService {
}

@Component
@Qualifier("group1")
public class Service2 implements IService {
}

@Component
@Qualifier("group1")
public class Service3 implements IService {
}

@Component
public class ServiceA {
    @Autowired
    @Qualifier("group1")
    private List<IService> serviceList;

    @Override
    public String toString() {
        return "ServiceA{" +
        "serviceList=" + serviceList +
        '}';
    }
}
```
测试效果，运行下面代码
```java
@Configuration
@ComponentScan
public class Config {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(Config.class);
        ServiceA serviceA = context.getBean(ServiceA.class);
        System.out.println(serviceA);
    }
}


===输出===
ServiceA{serviceList=[test3.Service2@2e4b8173, test3.Service3@70e8f8e]}
```
<a name="wHY4F"></a>
## 第 3 种：标注在方法上，对 bean 进行分组
```java
public interface IService {
}

public class Service1 implements IService {
}

public class Service2 implements IService {
}

public class Service3 implements IService {
}

public class ServiceA {
    private List<IService> serviceList;

    public ServiceA(List<IService> serviceList) {
        this.serviceList = serviceList;
    }

    @Override
    public String toString() {
        return "ServiceA{" +
        "serviceList=" + serviceList +
        '}';
    }
}
```
重点代码如下：

- 4 个`@Bean` 标注的方法，用来定义 4 个 bean
- service1()上 service3()方法上使用了`@Qualifier("group1")`，其这 2 个 bean 打了一个标签，标签名称为group1
- serviceA(`@Qualifier("group1") List<IService> serviceList`)方法中需要 IService 列表，这个列表来源：从容器中查找 IService 类型的所有 bean，且标签名称为group1的，满足条件的只有service1和service3
```java
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.util.List;

@Configuration
public class Config {

    @Bean
    @Qualifier("group1")
    public Service1 service1() {
        return new Service1();
    }

    @Bean
    public Service2 service2() {
        return new Service2();
    }

    @Bean
    @Qualifier("group1")
    public Service3 service3() {
        return new Service3();
    }

    @Bean
    public ServiceA serviceA(@Qualifier("group1") List<IService> serviceList) {
        return new ServiceA(serviceList);
    }

    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(Config.class);
        ServiceA serviceA = context.getBean(ServiceA.class);
        System.out.println(serviceA);
    }
}
```
运行输出如下
```
ServiceA{serviceList=[test4.Service1@bef2d72, test4.Service3@69b2283a]}
```
这种方式其实和第二种方式类似。
<a name="M96kk"></a>
## 用法 3：自定义`@Qualifier`
自定义一个注解，上面使用`@Qulalifier` 标注
```java
@Target({ElementType.FIELD, ElementType.PARAMETER, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@Qualifier
public @interface LoadBalanced {
}
```
来一个普通的类，稍后会使用
```java
public class RestTemplate {
    private String name;

    public RestTemplate(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "RestTemplate{" +
        "name='" + name + '\'' +
        '}';
    }
}
```
来个配置类，这个是关键咯，代码如下

- 有 3 个方法标注了`@Bean` 注解
- rt2 和 rt3 方法上标注了`@LoadBalanced`注解
- loadBalancedRtList 方法需要注入`List<RestTemplate> rtList`，大家注意这个参数上有`@LoadBalanced`注解，此时参数的值为：从容器中查找类型为`RestTemplate`的且 bean 的定的时候有`@LoadBalanced`限定符的 bean，满足条件的有 rt2 和 rt3
```java
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.util.List;

@Configuration
public class Config {

    @Bean
    public RestTemplate rt1() {
        return new RestTemplate("rt1");
    }

    @Bean
    @LoadBalanced
    public RestTemplate rt2() {
        return new RestTemplate("rt2");
    }

    @Bean
    @LoadBalanced
    public RestTemplate rt3() {
        return new RestTemplate("rt3");
    }

    @Bean
    public List<RestTemplate> loadBalancedRtList(@LoadBalanced List<RestTemplate> rtList) {
        return rtList;
    }

    public static void main(String[] args) {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(Config.class);
        Object object = context.getBean("loadBalancedRtList");
        System.out.println(object);
    }
}
```
运行输出
```
[RestTemplate{name='rt2'}, RestTemplate{name='rt3'}]
```
用过 SpringCloud 的朋友，估计有点眼熟了，SpringCloud 中刚好也有个 `RestTemplate` 类，也有`@LoadBalanced` 注解，SpringCloud 会对标注有`@LoadBalanced` 的 RestTemplate 进行增强，让 RestTemplate 具有负载均衡的功能，可以去翻阅下 SpringCloud，看看是不是这回事。
