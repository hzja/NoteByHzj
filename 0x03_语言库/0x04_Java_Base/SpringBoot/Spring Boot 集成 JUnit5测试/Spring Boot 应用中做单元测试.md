JavaSpringBoot<br />Spring Boot 提供了丰富的测试功能，主要由以下两个模块组成：

- spring-boot-test：提供测试核心功能。
- spring-boot-test-autoconfigure：提供对测试的自动配置。

Spring Boot 提供了一个 spring-boot-starter-test一站式启动器，如以下依赖配置所示。
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-test</artifactId>
  <scope>test</scope>
</dependency>
```
测试启动器依赖不仅包含以上两个 Spring Boot模块，还包含 Spring Test 测试模块，以及其他<br />第三方测试类库，如下所示。

- JUnit 5：Java 最主流的单元测试框架。
- AssertJ：一款快速断言库。
- Hamcrest：一款单元测试匹配库。
- Mockito：一款 Mock 测试框架。
- JSONassert：一款 JSON 断言库。
- JsonPath：一款 JSON XPath 库。

更多测试相关的依赖可见具体的依赖关系树，如下图所示。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1682491786234-13d9e020-afec-44f7-9e7f-797541fd785a.png#averageHue=%23f6f5f4&clientId=u10106604-b4ec-4&from=paste&height=320&id=u24bd52fc&originHeight=801&originWidth=1503&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=110838&status=done&style=none&taskId=u08363ce5-9984-4151-b6ce-496ca72d6a4&title=&width=601.2)<br />以上这些都是 Spring Boot 提供的常用的测试类库，如果上面的测试类库还不能满足需要，也可以任意添加以上没有的类库。<br />现在基本上使用的是 JUnit 5，如果应用还在使用JUnit 4 写的单元测试用例，那么也可以使用JUnit 5 的 Vintage 引擎来运行，如下面的依赖配置所示。
```xml
<dependency>
  <groupId>org.junit.vintage</groupId>
  <artifactId>junit-vintage-engine</artifactId>
  <scope>test</scope>
  <exclusions>
    <exclusion>
      <groupId>org.hamcrest</groupId>
      <artifactId>hamcrest-core</artifactId>
    </exclusion>
  </exclusions>
</dependency>
```
需要排除 hamcrest-core 依赖，因为该依赖已经改坐标了，并且默认内置在Spring Boot依赖管理中，如上面的依赖关系树所示，最新的 Hamcrest依赖已经是org.hamcrest:hamcrest坐标了。<br />Spring Boot提供了一个 `@SpringBootTest` 注解，用在单元测试类上以启用支持Spring Boot特性的单元测试，如果使用的是JUnit 4，那么测试类上还需要额外的`@RunWith(SpringRunner. class)`注解，然后在测试类方法上添加 `@Test` 注解即可，每一个 `@Test` 注解修饰的方法就是一个单元测试方法。<br />`@SpringBootTest` 注解有一个最重要的 `webEnvironment` 环境参数，支持以下几种环境设置：

- **MOCK（默认）：** 加载一个 Web ApplicationContext 并提供一个 Mock Web Environment，但不会启动内嵌的 Web 服务器，并可以结合 `@AutoConfifigureMockMvcor` 和 `@AutoConfifigure-WebTestClient` 注解一起使用进行 Mock 测试。
- **RANDOM_PORT：** 加载一个 `WebServerApplicationContext`，以及提供一个真实的 `WebEnvironment`，并以随机端口启动内嵌服务器。
- **DEFINED_PORT：** 和 RANDOM_PORT 一样，不同的是 DEFINED_PORT 是以应用指定的端口运行的，默认端口为 8080。
- **NONE：** 加载一个 `ApplicationContext`，但不会提供任何 Web Environment。

如果使用的 `@SpringBootTest` 注解不带任何参数，则默认为 Mock 环境。
<a name="ROWwO"></a>
### 真实环境测试
在 `@SpringBootTest` 注解中指定基于随机端口的真实Web环境，然后在类成员变量或者方法参数上注入 `TestRestTemplate` 实例，就可以完成对 Spring MVC接口的真实环境测试。<br />下面是一个基于随机端口的真实环境的测试用例：
```java
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class MvcTest {
    @Test
    public void getUserTest(@Autowired TestRestTemplate testRestTemplate) {
        Map<String, String> multiValueMap = new HashMap<>();
        multiValueMap.put("username", "Java");
        Result result = testRestTemplate.getForObject("/user/get?username={username}",
                                                      Result.class, multiValueMap);
        assertThat(result.getCode()).isEqualTo(0);
        assertThat(result.getMsg()).isEqualTo("ok");
    }

}
```
测试当前应用下的 /user/get 接口，传入对应的用户名参数，最后检查接口返回结果是否和预期一致。<br />单元测试通过，从执行日志可以看到，它启动了一个嵌入式的 Tomcat 容器来测试真实的 Web应用环境。
<a name="DMvEV"></a>
### Mock 环境测试
通过在类上面使用 `@AutoConfifigureMockMvc` 注解，然后在类成员变量或者方法参数上注入MockMvc 实例，就可以完成对 Spring MVC 接口的 Mock 测试。<br />下面是一个基于默认 Mock 环境的测试用例：
```java
@SpringBootTest
@AutoConfigureMockMvc
class MockMvcTests {
    @Test
    public void getUserTest(@Autowired MockMvc mvc) throws Exception {
        mvc.perform(MockMvcRequestBuilders.get("/user/get?username={username}", "test"))
        .andExpect(status().isOk())
        .andExpect(content().string("{\"code\":0,\"msg\":\"ok\",\"
                                    data\":\"test\"}"));
    }
}
```
测试当前应用下的 /user/get 接口，传入对应的用户名参数，最后检查请求状态是否OK（200），响应的内容是否和预期一致。<br />单元测试通过，从执行日志可以看到，它并未启动真实的 Web 环境来测试，而是使用 Mock 环境测试的。
<a name="Tp1UP"></a>
### Mock 组件测试
某些时候可能还需要模拟一些组件，比如某些服务只有上线之后才能调用，在开发阶段不可用，这时就需要 Mock 模拟测试了，提供各种模拟组件以完成测试。<br />Spring Boot 提供了一个 `@MockBean` 注解，可为 Spring 中的 Bean 组件定义基于 Mockito 的Mock 测试，它可以创建一个新 Bean 以覆盖 Spring 环境中已有的 Bean，它可以用在测试类、成员变量上，或者 `@Confifiguration` 配置类、成员变量上，被模拟的 Bean 在每次测试结束后自动重置。<br />假现现在有一个远程的服务 userService，本地不能调用，现在进行 Mock 测试，如以下使用示例所示。
```java
@SpringBootTest
class MockBeanTests {
    // @Autowired
    // private UserService userService;
    @MockBean
    private UserService userService;
    @Test
    public void countAllUsers() {
        BDDMockito.given(this.userService.countAllUsers()).willReturn(88);
        assertThat(this.userService.countAllUsers()).isEqualTo(88);
    }
}
```
这里的 `@MockBean` 注解使用在 UserService 变量上，表明这个`userService`实例在当前测试用例中是被 Mock 覆盖的，如果要模拟的 Bean 有多个，则可以使用`@Qualififier`注解指定，然后通过Mockito 提供的代理工具类方法创建模拟返回数据，运行该服务的测试方法，当模拟数据和预期结果一致时才会测试通过。<br />这里通过 `BDDMockito` 工具类模拟 `userService#countAllUsers`方法并让它返回统计的用户总数（88），最后检查该方法的返回值是否和预期一致。<br />单元测试通过，也可以使用 `@SpyBean` 注解代替 `@MockBean` 注解，两者的区别是：

- `@SpyBean`—如果没有提供 Mockito 代理方法，则会调用真实的 Bean 来获取数据。
- `@MockBean`—不管有没有提供 Mockito 代理方法，都会调用 Mock 的 Bean 来获取数据。

`@MockBean`、`@SpyBean` 注解既可作用于 Mock 环境，也可作用于真实环境，它只是用来模拟、替换环境中指定的 Bean 而已，但不能用于模拟在应用上下文刷新期间 Bean 的行为，因为在执行测试用例时应用上下文已经刷新完成了，所以不可能再去模拟了，这种情况下建议使用 `@Bean` 方法来创建模拟配置。
