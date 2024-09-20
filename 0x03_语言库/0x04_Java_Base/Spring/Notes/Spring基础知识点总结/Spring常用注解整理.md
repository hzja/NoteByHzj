Java Spring
<a name="NOFia"></a>
## 一、Spring 部分
<a name="oW1OC"></a>
### 1、声明bean的注解
`@Component` 组件，没有明确的角色<br />`@Service` 在业务逻辑层使用（service层）<br />`@Repository` 在数据访问层使用（dao层）<br />`@Controller` 在展现层使用，控制器的声明（C）
<a name="PCSLU"></a>
### 2、注入bean的注解
`@Autowired`：由Spring提供<br />`@Inject`：由JSR-330提供<br />`@Resource`：由JSR-250提供<br />都可以注解在set方法和属性上，推荐注解在属性上（一目了然，少写代码）。
<a name="bXbe3"></a>
### 3、java配置类相关注解
`@Configuration` 声明当前类为配置类，相当于xml形式的Spring配置（类上）<br />`@Bean` 注解在方法上，声明当前方法的返回值为一个bean，替代xml中的方式（方法上）<br />`@Configuration` 声明当前类为配置类，其中内部组合了`@Component`注解，表明这个类是一个bean（类上）<br />`@ComponentScan` 用于对Component进行扫描，相当于xml中的（类上）<br />`@WishlyConfiguration` 为`@Configuration`与`@ComponentScan`的组合注解，可以替代这两个注解
<a name="NSlDS"></a>
### 4、切面（AOP）相关注解
Spring支持AspectJ的注解式切面编程。<br />`@Aspect` 声明一个切面（类上）<br />使用`@After`、`@Before`、`@Around`定义建言（advice），可直接将拦截规则（切点）作为参数。<br />`@After` 在方法执行之后执行（方法上）<br />`@Before` 在方法执行之前执行（方法上）<br />`@Around` 在方法执行之前与之后执行（方法上）<br />`@PointCut` 声明切点<br />在java配置类中使用`@EnableAspectJAutoProxy`注解开启Spring对`AspectJ`代理的支持（类上）
<a name="miNk9"></a>
### 5、`@Bean`的属性支持
`@Scope` 设置Spring容器如何新建Bean实例（方法上，得有`@Bean`）<br />其设置类型包括：<br />Singleton （单例，一个Spring容器中只有一个bean实例，默认模式），<br />Protetype （每次调用新建一个bean），<br />Request （web项目中，给每个http request新建一个bean），<br />Session （web项目中，给每个http session新建一个bean），<br />GlobalSession（给每一个 global http session新建一个Bean实例）<br />`@StepScope` 在Spring Batch中还有涉及<br />`@PostConstruct` 由JSR-250提供，在构造函数执行完之后执行，等价于xml配置文件中bean的`initMethod`<br />`@PreDestory` 由JSR-250提供，在Bean销毁之前执行，等价于xml配置文件中bean的`destroyMethod`
<a name="rjvgY"></a>
### 6、`@Value`注解
`@Value` 为属性注入值（属性上）<br />支持如下方式的注入：
<a name="pF0av"></a>
#### 》注入普通字符
```java
@Value("Fcink")
String name;
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593874617808-180eecca-70ad-4e11-8dcc-2b0866240848.png#averageHue=%23fcfcfb&height=54&id=ABap3&originHeight=162&originWidth=1267&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48123&status=done&style=shadow&title=&width=422.3333333333333)
<a name="yCcEw"></a>
#### 》注入操作系统属性
```java
@Value("#{systemProperties['os.name']}")
String osName;
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593874677233-ca1e0b26-126f-49dd-8285-6490396fa904.png#averageHue=%23fbfbfa&height=62&id=fSMte&originHeight=187&originWidth=2081&originalType=binary&ratio=1&rotation=0&showTitle=false&size=76489&status=done&style=shadow&title=&width=693.6666666666666)
<a name="qExLC"></a>
#### 》注入表达式结果
```java
@Value("#{ T(java.lang.Math).random() * 100}")
String randomNumber;
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593874714252-e4409d1d-0bf8-473a-acab-34eb91b96eb8.png#averageHue=%23fcfbfb&height=64&id=gLFOE&originHeight=193&originWidth=2345&originalType=binary&ratio=1&rotation=0&showTitle=false&size=84625&status=done&style=shadow&title=&width=781.6666666666666)
<a name="kk77b"></a>
#### 》注入其它bean属性
```java
@Value("#{Student.studentName}")
String student;
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593874735056-8609bb5d-28e2-49bd-9dce-3ea9da4db1a2.png#averageHue=%23f9f8f2&height=55&id=V3h8J&originHeight=164&originWidth=1105&originalType=binary&ratio=1&rotation=0&showTitle=false&size=30604&status=done&style=shadow&title=&width=368.3333333333333)
<a name="SZ8H4"></a>
#### 》注入文件资源
```java
@Value("classpath:com/fcink/log.txt")
String resourceFile;
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593874752850-2147699a-0d2d-4248-b31e-9ef9b5509242.png#averageHue=%23fefdfd&height=58&id=Yi7ul&originHeight=175&originWidth=1272&originalType=binary&ratio=1&rotation=0&showTitle=false&size=30088&status=done&style=shadow&title=&width=424)
<a name="r5EWn"></a>
#### 》注入网站资源
```java
@Value("http://www.yuque.com/fcant")
Resource url;
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593874771938-7e9e85b8-ae4b-4d54-99ee-ede30e537755.png#averageHue=%23fcfcfb&height=61&id=DtmZc&originHeight=183&originWidth=1219&originalType=binary&ratio=1&rotation=0&showTitle=false&size=27549&status=done&style=shadow&title=&width=406.3333333333333)
<a name="XaOUz"></a>
#### 》注入配置文件
```java
@Value("${fcink.value}")
String fcinkVlaue;
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593874909452-abb68efa-2314-4fcd-ab7b-8accc1f45ec0.png#averageHue=%23f9f8f6&height=130&id=Xqf2T&originHeight=390&originWidth=1876&originalType=binary&ratio=1&rotation=0&showTitle=false&size=114845&status=done&style=shadow&title=&width=625.3333333333334)
```java
package com.fcant.fcink.bean;

import lombok.Data;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.io.Resource;
import org.springframework.stereotype.Component;


/**
 * Service
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 22:21 2020/7/4/0004
 */
@Data
@Component
public class Person {

    @Value("Fcink")
    String name;

    @Value("#{systemProperties['os.name']}")
    String osName;

    @Value("#{ T(java.lang.Math).random() * 100}")
    String randomNumber;

    @Value("resources:application.yml")
    Resource resourceFile;

    @Value("http://www.yuque.com/fcant")
    Resource url;

    @Value("${fcink.value}")
    String fcinkVlaue;

    public Person() {
    }

    @Override
    public String toString() {
        return "Person{" + "\n" +
                "    name='" + name + '\'' + ",\n" +
                "    osName='" + osName + '\'' + ",\n" +
                "    randomNumber='" + randomNumber + '\'' + ",\n" +
                "    resourceFile='" + resourceFile + '\'' + ",\n" +
                "    url=" + url + ",\n" +
                "    fcinkVlaue='" + fcinkVlaue + '\'' + ",\n" +
                '}';
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593913305367-ab053633-c859-445f-a00a-0aa308e76018.png#averageHue=%23f7f7f6&height=687&id=dXFAS&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1267910&status=done&style=shadow&title=&width=1280)<br />注入配置使用方法：<br />① 编写配置文件（test.properties）<br />book.name=《三体》<br />② `@PropertySource` 加载配置文件(类上)
```java
@PropertySource("classpath:com/fcant/fcink/test.properties")
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593875301998-c82c5ba2-f58d-4a7a-b85b-1a40f09db2cd.png#averageHue=%23fdfcf7&height=45&id=i31Zn&originHeight=135&originWidth=1990&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31214&status=done&style=shadow&title=&width=663.3333333333334)<br />③ 还需配置一个`PropertySourcesPlaceholderConfigurer`的bean。
<a name="qeXI1"></a>
### 7、环境切换
`@Profile` 通过设定`Environment`的`ActiveProfiles`来设定当前`context`需要使用的配置环境。（类或方法上）<br />`@Conditional` Spring4中可以使用此注解定义条件话的bean，通过实现`Condition`接口，并重写`matches`方法，从而决定该bean是否被实例化。（方法上）
<a name="Cqb8u"></a>
### 8、异步相关
`@EnableAsync` 配置类中，通过此注解开启对异步任务的支持，叙事性`AsyncConfigurer`接口（类上）<br />`@Async` 在实际执行的bean方法使用该注解来申明其是一个异步任务（方法上或类上所有的方法都将异步，需要`@EnableAsync`开启异步任务）
<a name="ei3QX"></a>
### 9、定时任务相关
`@EnableScheduling` 在配置类上使用，开启计划任务的支持（类上）<br />`@Scheduled` 来申明这是一个任务，包括`cron`,`fixDelay`,`fixRate`等类型（方法上，需先开启计划任务的支持）
<a name="CJlDl"></a>
### 10、`@Enable*`注解说明
这些注解主要用来开启对xxx的支持。<br />`@EnableAspectJAutoProxy` 开启对AspectJ自动代理的支持<br />`@EnableAsync` 开启异步方法的支持<br />`@EnableScheduling` 开启计划任务的支持<br />`@EnableWebMvc` 开启Web MVC的配置支持<br />`@EnableConfigurationProperties` 开启对`@ConfigurationProperties`注解配置Bean的支持<br />`@EnableJpaRepositories` 开启对SpringData JPA Repository的支持<br />`@EnableTransactionManagement` 开启注解式事务的支持<br />`@EnableTransactionManagement` 开启注解式事务的支持<br />`@EnableCaching` 开启注解式的缓存支持
<a name="Ni3tq"></a>
### 11、测试相关注解
`@RunWith` 运行器，Spring中通常用于对JUnit的支持
```java
@RunWith(SpringJUnit4ClassRunner.class)
class FcinkApplicationTests {

    @Test
    void contextLoads() {
    }

}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593875122515-1cd66b38-7ba3-4812-9038-97ba98bab389.png#averageHue=%23f9f9f8&height=64&id=OrX8V&originHeight=193&originWidth=1453&originalType=binary&ratio=1&rotation=0&showTitle=false&size=39559&status=done&style=shadow&title=&width=484.3333333333333)<br />`@ContextConfiguration` 用来加载配置`ApplicationContext`，其中`classes`属性用来加载配置类
```java
@ContextConfiguration(classes={TestConfig.class})
```
<a name="AfV1B"></a>
## 二、SpringMVC部分
`@EnableWebMvc` 在配置类中开启Web MVC的配置支持，如一些`ViewResolver`或者`MessageConverter`等，若无此句，重写`WebMvcConfigurerAdapter`方法（用于对SpringMVC的配置）。<br />`@Controller` 声明该类为SpringMVC中的Controller<br />`@RequestMapping` 用于映射Web请求，包括访问路径和参数（类或方法上）<br />`@ResponseBody` 支持将返回值放在response内，而不是一个页面，通常用户返回json数据（返回值旁或方法上）<br />`@RequestBody` 允许request的参数在request体中，而不是在直接连接在地址后面。（放在参数前）<br />`@PathVariable` 用于接收路径参数，比如`@RequestMapping("/hello/{name}")`申明的路径，将注解放在参数中前，即可获取该值，通常作为Restful的接口实现方法。<br />`@RestController` 该注解为一个组合注解，相当于`@Controller`和`@ResponseBody`的组合，注解在类上，意味着，该Controller的所有方法都默认加上了`@ResponseBody`。<br />`@ControllerAdvice` 通过该注解，可以将对于控制器的全局配置放置在同一个位置，注解了`@Controller`的类的方法可使用`@ExceptionHandler`、`@InitBinder`、`@ModelAttribute`注解到方法上，<br />这对所有注解了 `@RequestMapping`的控制器内的方法有效。<br />`@ExceptionHandler` 用于全局处理控制器里的异常<br />`@InitBinder` 用来设置`WebDataBinder`，`WebDataBinder`用来自动绑定前台请求参数到`Model`中。<br />`@ModelAttribute` 本来的作用是绑定键值对到Model里，在`@ControllerAdvice`中是让全局的`@RequestMapping`都能获得在此处设置的键值对。

