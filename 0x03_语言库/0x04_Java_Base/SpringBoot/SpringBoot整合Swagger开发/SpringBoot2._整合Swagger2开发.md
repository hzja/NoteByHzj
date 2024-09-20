Java SpringBoot Swagger
<a name="vjcry"></a>
## 1、导入Swagger2依赖
```xml
<springfox-swagger.version>2.8.0</springfox-swagger.version>
<swagger-bootstrap-ui.version>1.7.2</swagger-bootstrap-ui.version>

<!-- https://mvnrepository.com/artifact/io.springfox/springfox-swagger2 -->
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger2</artifactId>
    <version>${springfox-swagger.version}</version>
</dependency>

<!-- https://mvnrepository.com/artifact/io.springfox/springfox-swagger-ui -->
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger-ui</artifactId>
    <version>${springfox-swagger.version}</version>
</dependency>

<!-- https://mvnrepository.com/artifact/com.github.xiaoymin/swagger-bootstrap-ui -->
<dependency>
    <groupId>com.github.xiaoymin</groupId>
    <artifactId>swagger-bootstrap-ui</artifactId>
    <version>${swagger-bootstrap-ui.version}</version>
</dependency>
```

<a name="NnwAW"></a>
## 2、配置Swagger
<a name="SwaggerConfig.java"></a>
### SwaggerConfig.java
```java
package com.fcant.service_acti.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;
import springfox.documentation.builders.ApiInfoBuilder;
import springfox.documentation.builders.PathSelectors;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.service.ApiInfo;
import springfox.documentation.service.Contact;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spring.web.plugins.Docket;
import springfox.documentation.swagger2.annotations.EnableSwagger2;

/**
 * SwaggerConfig
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 23:41 2020/8/1/0001
 */
@Configuration
@EnableSwagger2
public class SwaggerConfig implements WebMvcConfigurer {

    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry.addResourceHandler("swagger-ui.html")
                .addResourceLocations("classpath:/META-INF/resources/");
        registry.addResourceHandler("/webjars/**")
                .addResourceLocations("classpath:/META-INF/resources/webjars/");
    }

    /**
     * @author Fcant 13:44 2019/12/5
     */
    @Bean
    public Docket petApi() {
        return new Docket(DocumentationType.SWAGGER_2)
                .apiInfo(apiInfo())
                .select()
                .apis(RequestHandlerSelectors.basePackage("com.fcant.service_acti.controller"))
                .paths(PathSelectors.any())
                .build();
    }

    /**
     * 该套 API 说明，包含作者、简介、版本、host、服务URL
     * @return
     */
    private ApiInfo apiInfo() {
        return new ApiInfoBuilder()
                .title("Activiti Service API")
                .contact(new Contact("fcant","null","fcscanf@outlook.com"))
                .version("0.1")
                .termsOfServiceUrl("localhost:8080/swagger")
                .description("Activiti Service API")
                .build();
    }

}
```
<a name="lCcL2"></a>
## 3、访问
在浏览器地址输入-`localhost:8080/swagger-ui.html`进入<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575534942304-b4687d55-d130-4e9d-a928-3103aed0d9bb.png#averageHue=%23f9e7d1&height=726&id=Olbki&originHeight=726&originWidth=1366&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69855&status=done&style=none&title=&width=1366)

<a name="sy0vD"></a>
## 4、Swagger注解总结
<a name="3d63b2ea"></a>
### 官方总结
[Swagger相关注解官方说明地址](https://github.com/swagger-api/swagger-core/wiki/Annotations)<br />[https://github.com/swagger-api/swagger-core/wiki/Annotations](https://github.com/swagger-api/swagger-core/wiki/Annotations)
<a name="UUSnG"></a>
### 常见总结
<a name="6s74P"></a>
#### `@Api()`用于类
标识这个类是swagger的资源

- tags–表示分组说明标签
<a name="j9aGF"></a>
#### `@ApiOperation()`用于方法
表示一个http请求的操作

- value用于方法描述 
- notes用于提示内容
<a name="NXh24"></a>
#### `@ApiModel()`用于实体类
表示对类进行说明，用于参数用实体类接收

- value–表示对象名 
- description–描述
<a name="Zt6E7"></a>
#### `@ApiModelProperty()`用于实体类字段
表示对model属性的说明或者数据操作更改

- value–字段说明 
- name–重写属性名字 
- dataType–重写属性类型 
- required–是否必填 
- example–举例说明 
- hidden–隐藏
<a name="c9379719"></a>
#### `@ApiImplicitParam()` 用于 Controller 方法
表示单独的请求参数

- name–参数ming
- value–参数说明
- dataType–数据类型
- paramType–参数类型
- example–举例说明
<a name="RByST"></a>
#### `@ApiImplicitParams()` 用于 Controller 方法
包含多个 `@ApiImplicitParam`
<a name="PtFZM"></a>
#### `@ApiIgnore()`用于类或者方法上
可以不被swagger显示在页面上
