SpringBoot Swagger3.0
<a name="RqGnt"></a>
## 资料

- swagger 官网：[https://swagger.io](https://swagger.io)
- springfox 官网：[http://springfox.github.io](http://springfox.github.io)
- springfox Github 仓库：springfox / springfox
- springfox-demos Github 仓库：[https://github.com/springfox/springfox](https://github.com/springfox/springfox)
<a name="O2imf"></a>
## swagger介绍
对于 Rest API 来说很重要的一部分内容就是文档，Swagger 提供了一套通过代码和注解自动生成文档的方法，这一点对于保证 API 文档的及时性将有很大的帮助。<br />Swagger 是一套基于 OpenAPI 规范（OpenAPI Specification，OAS）构建的开源工具，可以帮助设计、构建、记录以及使用 Rest API。<br />OAS本身是一个API规范，它用于描述一整套API接口，包括一个接口是哪种请求方式、哪些参数、哪些header等，都会被包括在这个文件中。它在设计的时候通常是YAML格式，这种格式书写起来比较方便，而在网络中传输时又会以json形式居多，因为json的通用性比较强。<br />Swagger 主要包含了以下三个部分：

- Swagger Editor：基于浏览器的编辑器，可以使用它编写 OpenAPI 规范。
- Swagger UI：它会将编写的 OpenAPI 规范呈现为交互式的 API 文档。
- Swagger Codegen：它可以通过为 OpenAPI（以前称为 Swagger）规范定义的任何 API 生成服务器存根和客户端 SDK 来简化构建过程。
<a name="GvFqC"></a>
## springfox介绍
由于Spring的流行，Marty Pitt编写了一个基于Spring的组件swagger-springmvc，用于将swagger集成到springmvc中来，而springfox则是从这个组件发展而来。<br />通常SpringBoot项目整合swagger需要用到两个依赖：springfox-swagger2和springfox-swagger-ui，用于自动生成swagger文档。

- springfox-swagger2：这个组件的功能用于项目中自动生成描述API的json文件
- springfox-swagger-ui：就是将描述API的json文件解析出来，用一种更友好的方式呈现出来。
<a name="JPN0P"></a>
## SpringFox 3.0.0 发布
<a name="Qg0we"></a>
### 官方说明

- SpringFox 3.0.0 发布了，SpringFox 的前身是 swagger-springmvc，是一个开源的 API doc 框架，可以将 Controller 的方法以文档的形式展现。
<a name="eXr4z"></a>
### 新特性

- Remove explicit dependencies on springfox-swagger2
- Remove any `@EnableSwagger2`… annotations
- Add the springfox-boot-starter dependency
- Springfox 3.x removes dependencies on guava and other 3rd party libraries (not zero dep yet! depends on spring plugin and open api libraries for annotations and models) so if you used guava predicates/functions those will need to transition to java 8 function interfaces.
<a name="Wu8Gu"></a>
### 此版本的亮点

- Spring5，Webflux支持（仅支持请求映射，尚不支持功能端点）。
- Spring Integration支持（非常感谢反馈）。
- SpringBoot支持springfox Boot starter依赖性（零配置、自动配置支持）。
- 具有自动完成功能的文档化配置属性。
- 更好的规范兼容性与2.0。
- 支持OpenApi 3.0.3。
- 零依赖。几乎只需要spring-plugin，swagger-core ，现有的swagger2注释将继续工作并丰富openapi3.0规范。
<a name="TLbh2"></a>
#### OpenAPI
OpenAPI 规范其实就是以前的 Swagger 规范，它是一种 REST API 的描述格式，通过既定的规范来描述文档接口，它是业界真正的 API 文档标准，可以通过 YAML 或者 JSON 来描述。它包括如下内容：

- 接口（/users）和每个接口的操作（GET /users，POST /users）
- 输入参数和响应内容
- 认证方法
- 一些必要的联系信息、license 等。

关于 OpenAPI 的更多内容，可以在 GitHub 上查看：[https://github.com/OAI/OpenAPI-Specification/blob/master/versions/3.0.2.md](https://github.com/OAI/OpenAPI-Specification/blob/master/versions/3.0.2.md)
<a name="VvevO"></a>
### 兼容性说明

- 需要Java 8
- 需要Spring5.x（未在早期版本中测试）
- 需要SpringBoot 2.2+（未在早期版本中测试）
<a name="ba8d1dca"></a>
### 注意
:::danger
应用主类可以不用增加注解`@EnableOpenApi`，删除之前版本的SwaggerConfig.java。<br />在springfox-boot-starter-3.0.0.jar下可以找到一个spring.factories，这个是一个Spring Boot 特有的SPI文件，能够自动的发现并注册Starter组件的配置。里面有这样的配置：
:::
```java
# Auto Configure
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
springfox.boot.starter.autoconfigure.OpenApiAutoConfiguration
```
找到总的配置类`OpenApiAutoConfiguration`：
```java
@Configuration
@EnableConfigurationProperties(SpringfoxConfigurationProperties.class)
@ConditionalOnProperty(value = "springfox.documentation.enabled", havingValue = "true", matchIfMissing = true)
@Import({
    OpenApiDocumentationConfiguration.class,
    SpringDataRestConfiguration.class,
    BeanValidatorPluginsConfiguration.class,
    Swagger2DocumentationConfiguration.class,
    SwaggerUiWebFluxConfiguration.class,
    SwaggerUiWebMvcConfiguration.class
})
@AutoConfigureAfter({ WebMvcAutoConfiguration.class, JacksonAutoConfiguration.class,
    HttpMessageConvertersAutoConfiguration.class, RepositoryRestMvcAutoConfiguration.class })
public class OpenApiAutoConfiguration {

}
```
关键的一个地方是`@ConditionalOnProperty`注解声明了当`springfox.documentation.enabled`为`true`时启用配置，而且默认值就是`true`。这非常有用，Swagger仅仅建议在开发阶段使用，这个正好是个开关。另外有时候自定义配置的时候最好把这个开关也加上：
```java
// 自定义swagger3文档信息
@Configuration
@ConditionalOnProperty(value = "springfox.documentation.enabled", havingValue = "true", matchIfMissing = true)
public class Swagger3Config {
    @Bean
    public Docket createRestApi() {
        return new Docket(DocumentationType.OAS_30)
                .apiInfo(apiInfo())
                .select()
                .apis(RequestHandlerSelectors.withMethodAnnotation(ApiOperation.class))
                .paths(PathSelectors.any())
                .build();
    }

    private ApiInfo apiInfo() {
        return new ApiInfoBuilder()
                .title("Swagger3接口文档")
                .description("fcant.cn")
                .contact(new Contact("Fcant", "https://fcant.cn", "dax@fcant.cn"))
                .version("1.0.0")
                .build();
    }
}
```
Swagger3不需要使用`@EnableOpenApi`或者`@EnableSwagger2`开启，这里也能找到答案。
```java
@Import(OpenApiDocumentationConfiguration.class)
public @interface EnableOpenApi {
}
```
```java
@Import(Swagger2DocumentationConfiguration.class)
public @interface EnableSwagger2 {
}
```
上面的两个导入类都可以在`OpenApiAutoConfiguration`找到，所以Swagger3提供的是全自动的集成。
:::warning
启动项目，访问地址：http://localhost:8080/swagger-ui/index.html，<br />注意2.x版本中访问的地址的为http://localhost:8080/swagger-ui.html
:::
<a name="IlOS6"></a>
### 和全局统一参数不兼容
如果使用了统一返回体封装器来标准化Spring MVC接口的统一返回
```java
/**
 * 返回体统一封装器
 *
 * @author n1
 */
@RestControllerAdvice 
public class RestBodyAdvice implements ResponseBodyAdvice<Object> {
    @Override
    public boolean supports(MethodParameter returnType, Class<? extends HttpMessageConverter<?>> converterType) {
        return !returnType.hasMethodAnnotation(IgnoreRestBody.class);
    }

    @Override
    public Object beforeBodyWrite(Object body, MethodParameter returnType, MediaType selectedContentType, Class<? extends HttpMessageConverter<?>> selectedConverterType, ServerHttpRequest request, ServerHttpResponse response) {

        if (body == null) {
            return RestBody.ok();
        }
        if (Rest.class.isAssignableFrom(body.getClass())) {
            return body;
        }
        return RestBody.okData(body);
    }
}
```
可以发现Swagger3会报Unable to infer base url……的错误，这是因为统一返回体影响到了Swagger3的一些内置接口。解决方法是`@RestControllerAdvice`控制好生效的包范围,也就是配置其`basePackages`参数就行了。
<a name="hVkDT"></a>
### 安全框架放行
如果使用安全框架，Swagger3的内置接口就会访问受限，需要排除掉。Spring Security是这么配置的：
```java
@Override
public void configure(WebSecurity web) throws Exception {
    //忽略swagger3所需要用到的静态资源，允许访问
    web.ignoring().antMatchers( "/swagger-ui.html",
            "/swagger-ui/**",
            "/swagger-resources/**",
            "/v2/api-docs",
            "/v3/api-docs",
            "/webjars/**");
}
```
如果使用的版本是Spring Security 5.4，可以这么定制`WebSecurity`:
```java
@Bean
WebSecurityCustomizer swaggerWebSecurityCustomizer() {
    return (web) -> {
        web.ignoring().antMatchers(new String[]{"/swagger-ui.html", "/swagger-ui/**", "/swagger-resources/**", "/v2/api-docs", "/v3/api-docs", "/webjars/**"});
    };
}
```
<a name="NThfN"></a>
### 在Swagger2或Swagger3中增加Json Web Token
<a name="zEmFi"></a>
#### Swagger2 中添加 JWT
先来回顾在Swagger2中是如何添加JWT的。在Swagger2中声明`DocketBean`时利用全局参数注入一个`Authorization`请求头：
```java
private List<Parameter> jwtToken() {

    String jwt = "Bearer {jwt}";

    ParameterBuilder tokenPar = new ParameterBuilder();
    List<Parameter> pars = new ArrayList<>();
    // 声明 key
    tokenPar.name("Authorization")
        // 文字说明
        .description("jwt令牌")
        // 类型为字符串
        .modelRef(new ModelRef("string"))
        // 参数形式为 header 参数
        .parameterType("header")
        // 默认值
        .defaultValue(jwt)
        // 是否必须
        .required(false);
    pars.add(tokenPar.build());
    return pars;
}

@Bean
public Docket api() {

    return new Docket(DocumentationType.SWAGGER_2)
        .apiInfo(apiInfo())
        .globalOperationParameters(jwtToken())
        .select()
        .apis(RequestHandlerSelectors.any())
        .paths(PathSelectors.any())
        .build();
}
```
效果如下，只需要填充一个可用的Jwt Token即可。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1619183728654-3a945299-8f2e-490d-956d-19f3a03bc127.png#clientId=ud576d670-25e4-4&from=paste&id=u6d62979f&originHeight=251&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uddcb273d-5fca-4e54-86cc-f7e98604664&title=)<br />swagger2中注入jwt请求头<br />但是这种方式只能适用于 Swagger2，在 Swagger3 中并不凑效。
<a name="uhOH9"></a>
#### Swagger3 中添加 JWT
那么Swagger3中应该如何做呢？Swagger3同样也是在声明`DocketBean`中注入，如下：
```java
@Bean
public Docket api() {

    return new Docket(DocumentationType.OAS_30)
        .apiInfo(apiInfo())
        .securitySchemes(Collections.singletonList(HttpAuthenticationScheme.JWT_BEARER_BUILDER
                                                   //                        显示用
                                                   .name("JWT")
                                                   .build()))
        .securityContexts(Collections.singletonList(SecurityContext.builder()
                                                    .securityReferences(Collections.singletonList(SecurityReference.builder()
                                                                                                  .scopes(new AuthorizationScope[0])
                                                                                                  .reference("JWT")
                                                                                                  .build()))
                                                    // 声明作用域
                                                    .operationSelector(o -> o.requestMappingPattern().matches("/.*"))
                                                    .build()))
        .select()
        .apis(RequestHandlerSelectors.any())
        .paths(PathSelectors.any())
        .build();
}
```
通过Swagger3中`Docket`提供的`securitySchemes`和`securityReferences`方法进行JWT的配置。效果以及流程如下：<br />![Swagger3中jwt使用流程](https://cdn.nlark.com/yuque/0/2021/webp/396745/1619183729360-b06b1c8c-cbc4-42e2-874a-52193bcc651e.webp#clientId=ud576d670-25e4-4&from=paste&id=u39834408&originHeight=457&originWidth=1070&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucdd89fa6-f2c6-41d9-93f1-eaaa17a2143&title=Swagger3%E4%B8%ADjwt%E4%BD%BF%E7%94%A8%E6%B5%81%E7%A8%8B "Swagger3中jwt使用流程")<br />可以看到请求时会携带一个Bearer Token:<br />![Swagger3中携带jwt](https://cdn.nlark.com/yuque/0/2021/webp/396745/1619183728736-56c5111a-be42-45da-8a3c-9e6cbe2b7f4b.webp#clientId=ud576d670-25e4-4&from=paste&id=uebaa961d&originHeight=253&originWidth=938&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u07b1bd43-6f52-4b0b-80e5-d386deec322&title=Swagger3%E4%B8%AD%E6%90%BA%E5%B8%A6jwt "Swagger3中携带jwt")<br />感觉Swagger3中设置JWT比Swagger2中要麻烦一些，不过能用就行。
<a name="TTig0"></a>
## 依赖
以前在使用 2.9.2 这个版本的时候，一般来说可能需要添加如下两个依赖：
```xml
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger2</artifactId>
    <version>2.9.2</version>
</dependency>
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger-ui</artifactId>
    <version>2.9.2</version>
</dependency>
```
这两个，一个用来生成接口文档（JSON 数据），另一个用来展示将 JSON 可视化。<br />在 3.0 版本中，不需要这么麻烦了，一个 starter 就可以搞定：
```xml
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-boot-starter</artifactId>
    <version>3.0.0</version>
</dependency>
```
和 Spring Boot 中的其他 starter 一样，springfox-boot-starter 依赖可以实现零配置以及自动配置支持。也就是说，如果没有其他特殊需求，加一个这个依赖就行了，接口文档就自动生成了。
<a name="ZIYrn"></a>
## 接口地址
3.0 中的接口地址也和之前有所不同，以前在 2.9.2 中主要访问两个地址：

- 文档接口地址：http://localhost:8080/v2/api-docs
- 文档页面地址：http://localhost:8080/swagger-ui.html

现在在 3.0 中，这两个地址也发生了变化：

- 文档接口地址：http://localhost:8080/v3/api-docs
- 文档页面地址：http://localhost:8080/swagger-ui/index.html

特别是文档页面地址，如果用了 3.0，而去访问之前的页面，会报 404。
<a name="qm1rB"></a>
## 注解
旧的注解还可以继续使用，不过在 3.0 中还提供了一些其他注解。<br />例如可以使用 `@EnableOpenApi` 代替以前旧版本中的 `@EnableSwagger2`。<br />不过在实际体验中，感觉 `@EnableOpenApi` 注解的功能不明显，加不加都行。翻了下源码，`@EnableOpenApi` 注解主要功能是为了导入 `OpenApiDocumentationConfiguration` 配置类，如下：
```java
@Retention(value = java.lang.annotation.RetentionPolicy.RUNTIME)
@Target(value = {java.lang.annotation.ElementType.TYPE})
@Documented
@Import(OpenApiDocumentationConfiguration.class)
public @interface EnableOpenApi {
}
```
然后又看了下自动化配置类 `OpenApiAutoConfiguration`，如下：
```java
@Configuration
@EnableConfigurationProperties(SpringfoxConfigurationProperties.class)
@ConditionalOnProperty(value = "springfox.documentation.enabled", havingValue = "true", matchIfMissing = true)
@Import({
    OpenApiDocumentationConfiguration.class,
    SpringDataRestConfiguration.class,
    BeanValidatorPluginsConfiguration.class,
    Swagger2DocumentationConfiguration.class,
    SwaggerUiWebFluxConfiguration.class,
    SwaggerUiWebMvcConfiguration.class
        })
@AutoConfigureAfter({ WebMvcAutoConfiguration.class, JacksonAutoConfiguration.class,
                     HttpMessageConvertersAutoConfiguration.class, RepositoryRestMvcAutoConfiguration.class })
public class OpenApiAutoConfiguration {

}
```
可以看到，自动化配置类里边也导入了 `OpenApiDocumentationConfiguration`。<br />所以在正常情况下，实际上不需要添加 `@EnableOpenApi` 注解。<br />根据 `OpenApiAutoConfiguration` 上的 `@ConditionalOnProperty` 条件注解中的定义，可以发现，如果在 `application.properties` 中设置 `springfox.documentation.enabled=false`，即关闭了 swagger 功能，此时自动化配置类就不执行了，这个时候可以通过 `@EnableOpenApi` 注解导入 `OpenApiDocumentationConfiguration` 配置类。技术上来说逻辑是这样，不过应用中暂未发现这样的需求（即在 `application.properties` 中关闭 swagger，再通过 `@EnableOpenApi` 注解开启）。<br />以前用的 `@ApiResponses`/`@ApiResponse` 注解，在 3.0 中名字没变，但是所在的包变了，使用时注意导包问题。<br />之前用的 `@ApiOperation` 注解在 3.0 中可以使用 `@Operation` 代替。
<a name="z7S5Q"></a>
## 整合使用
Maven项目中引入springfox-boot-starter依赖：
```xml
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-boot-starter</artifactId>
    <version>3.0.0</version>
</dependency>
```
application.yml配置
```yaml
spring:
  application:
    name: springfox-swagger
server:
  port: 8080

# ===== 自定义swagger配置 ===== #
swagger:
  enable: true
  application-name: ${spring.application.name}
  application-version: 1.0
  application-description: springfox swagger 3.0整合Demo
  try-host: http://localhost:${server.port}
```
使用`@EnableOpenApi`注解，启用swagger配置
```java
@EnableOpenApi
@Configuration
public class SwaggerConfiguration {

}
```
自定义swagger配置类`SwaggerProperties`
```java
@Component
@ConfigurationProperties("swagger")
public class SwaggerProperties {
    /**
     * 是否开启swagger，生产环境一般关闭，所以这里定义一个变量
     */
    private Boolean enable;

    /**
     * 项目应用名
     */
    private String applicationName;

    /**
     * 项目版本信息
     */
    private String applicationVersion;

    /**
     * 项目描述信息
     */
    private String applicationDescription;

    /**
     * 接口调试地址
     */
    private String tryHost;

    public Boolean getEnable() {
        return enable;
    }

    public void setEnable(Boolean enable) {
        this.enable = enable;
    }

    public String getApplicationName() {
        return applicationName;
    }

    public void setApplicationName(String applicationName) {
        this.applicationName = applicationName;
    }

    public String getApplicationVersion() {
        return applicationVersion;
    }

    public void setApplicationVersion(String applicationVersion) {
        this.applicationVersion = applicationVersion;
    }

    public String getApplicationDescription() {
        return applicationDescription;
    }

    public void setApplicationDescription(String applicationDescription) {
        this.applicationDescription = applicationDescription;
    }

    public String getTryHost() {
        return tryHost;
    }

    public void setTryHost(String tryHost) {
        this.tryHost = tryHost;
    }
}
```
一个完整详细的springfox swagger配置示例：
```java
import io.swagger.models.auth.In;
import org.apache.commons.lang3.reflect.FieldUtils;
import org.springframework.boot.SpringBootVersion;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.util.ReflectionUtils;
import org.springframework.web.servlet.config.annotation.InterceptorRegistration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;
import springfox.documentation.builders.ApiInfoBuilder;
import springfox.documentation.builders.PathSelectors;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.oas.annotations.EnableOpenApi;
import springfox.documentation.service.*;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spi.service.contexts.SecurityContext;
import springfox.documentation.spring.web.plugins.Docket;

import java.lang.reflect.Field;
import java.util.*;

@EnableOpenApi
@Configuration
public class SwaggerConfiguration implements WebMvcConfigurer {
    private final SwaggerProperties swaggerProperties;

    public SwaggerConfiguration(SwaggerProperties swaggerProperties) {
        this.swaggerProperties = swaggerProperties;
    }

    @Bean
    public Docket createRestApi() {
        return new Docket(DocumentationType.OAS_30).pathMapping("/")
                // 定义是否开启swagger，false为关闭，可以通过变量控制
                .enable(swaggerProperties.getEnable())
                // 将api的元信息设置为包含在json ResourceListing响应中。
                .apiInfo(apiInfo())
                // 接口调试地址
                .host(swaggerProperties.getTryHost())
                // 选择哪些接口作为swagger的doc发布
                .select()
                .apis(RequestHandlerSelectors.any())
                .paths(PathSelectors.any())
                .build()
                // 支持的通讯协议集合
                .protocols(newHashSet("https", "http"))
                // 授权信息设置，必要的header token等认证信息
                .securitySchemes(securitySchemes())
                // 授权信息全局应用
                .securityContexts(securityContexts());
    }

    /**
     * API 页面上半部分展示信息
     */
    private ApiInfo apiInfo() {
        return new ApiInfoBuilder().title(swaggerProperties.getApplicationName() + " Api Doc")
                .description(swaggerProperties.getApplicationDescription())
                .contact(new Contact("lighter", null, "123456@gmail.com"))
                .version("Application Version: " + swaggerProperties.getApplicationVersion() + ", Spring Boot Version: " + SpringBootVersion.getVersion())
                .build();
    }

    /**
     * 设置授权信息
     */
    private List<SecurityScheme> securitySchemes() {
        ApiKey apiKey = new ApiKey("BASE_TOKEN", "token", In.HEADER.toValue());
        return Collections.singletonList(apiKey);
    }

    /**
     * 授权信息全局应用
     */
    private List<SecurityContext> securityContexts() {
        return Collections.singletonList(
                SecurityContext.builder()
                        .securityReferences(Collections.singletonList(new SecurityReference("BASE_TOKEN", new AuthorizationScope[]{new AuthorizationScope("global", "")})))
                        .build()
        );
    }

    @SafeVarargs
    private final <T> Set<T> newHashSet(T... ts) {
        if (ts.length > 0) {
            return new LinkedHashSet<>(Arrays.asList(ts));
        }
        return null;
    }

    /**
     * 通用拦截器排除swagger设置，所有拦截器都会自动加swagger相关的资源排除信息
     */
    @SuppressWarnings("unchecked")
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        try {
            Field registrationsField = FieldUtils.getField(InterceptorRegistry.class, "registrations", true);
            List<InterceptorRegistration> registrations = (List<InterceptorRegistration>) ReflectionUtils.getField(registrationsField, registry);
            if (registrations != null) {
                for (InterceptorRegistration interceptorRegistration : registrations) {
                    interceptorRegistration
                            .excludePathPatterns("/swagger**/**")
                            .excludePathPatterns("/webjars/**")
                            .excludePathPatterns("/v3/**")
                            .excludePathPatterns("/doc.html");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="9kaPN"></a>
## 一些常用注解说明

- `@Api`：用在controller类，描述API接口
- `@ApiOperation`：描述接口方法
- `@ApiModel`：描述对象
- `@ApiModelProperty`：描述对象属性
- `@ApiImplicitParams`：描述接口参数
- `@ApiResponses`：描述接口响应
- `@ApiIgnore`：忽略接口方法
