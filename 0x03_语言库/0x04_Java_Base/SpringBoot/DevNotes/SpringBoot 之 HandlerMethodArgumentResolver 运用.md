JavaSpringBoot
<a name="daWft"></a>
### 介绍
在项目中，如果需要在 Header 中获取请求头，一般使用 `RequestHeader` 注解。代码案例如下：
```java
@RequestMapping("/normalHeaders")
 public Map<String, Object> normalHeaders(@RequestHeader("user-id")Long userId,
            @RequestHeader("tenant-id")Long tenantId,
            @RequestHeader("user-name")String userName){
  Map<String, Object> map = new HashMap<>();
  map.put("userId", userId);
  map.put("tenantId", tenantId);
  map.put("userName", userName);
  return map;
 }
```
请求curl
```bash
curl -X POST \
  http://127.0.0.1:8080/normalHeaders \
  -H 'tenant-id: 12' \
  -H 'user-id: 1' \
  -H 'user-name: buger'
```
使用 RequestHeader 注解获取请求头，如果获取一两个到不会写很多重复代码，但是如果需要获取很多个请求时，代码会变得重复。 下面介绍一种新的解决方案；不但减少了很多重复的代码，而且使得代码变得更简洁。
<a name="otogW"></a>
### pom.xml 文件引入依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0">http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.olive</groupId>
  <artifactId>springmvc-headers</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>jar</packaging>

  <name>springmvc-headers</name>
  <url>http://maven.apache.org</url>

  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.5.14</version>
    <relativePath /> <!-- lookup parent from repository -->
    </parent>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.source>8</maven.compiler.source>
    <maven.compiler.target>8</maven.compiler.target>
  </properties>

  <dependencies>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-test</artifactId>
      <scope>test</scope>
    </dependency>
    <dependency>
      <groupId>org.projectlombok</groupId>
      <artifactId>lombok</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
  </dependencies>
</project>
```
<a name="AHH30"></a>
### 解析请求头，并包装
实现 `HandlerMethodArgumentResolver` 类；解析请求头，包装成 `HeadersWrapperDTO` 类
```java
package com.olive.config;


import com.olive.dto.HeadersWrapperDTO;
import org.springframework.core.MethodParameter;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.support.WebDataBinderFactory;
import org.springframework.web.context.request.NativeWebRequest;
import org.springframework.web.method.support.HandlerMethodArgumentResolver;
import org.springframework.web.method.support.ModelAndViewContainer;

public class RequestHandlerMethodArgumentResolver implements HandlerMethodArgumentResolver {

    @Override
    public boolean supportsParameter(MethodParameter parameter) {
        return parameter.getParameterType().isAssignableFrom(HeadersWrapperDTO.class);
    }

    @Override
    public Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer,
                                  NativeWebRequest webRequest,
                                  WebDataBinderFactory binderFactory) throws Exception {
        String userId = webRequest.getHeader("user-id");
        String tenantId = webRequest.getHeader("tenant-id");
        String userName = webRequest.getHeader("user-name");

        HeadersWrapperDTO headersWrapperDTO = new HeadersWrapperDTO();
        if(StringUtils.hasText(userId)){
            headersWrapperDTO.setUserId(Long.parseLong(userId));
        }
        if(StringUtils.hasText(tenantId)){
            headersWrapperDTO.setTenantId(Long.parseLong(tenantId));
        }
        headersWrapperDTO.setUserName(userName);
        return headersWrapperDTO;
    }
}
```
`HeadersWrapperDTO` POJO类
```java
package com.olive.dto;

import lombok.Data;

import java.io.Serializable;

@Data
public class HeadersWrapperDTO implements Serializable {

    private Long userId;

    private Long tenantId;

    private String userName;

}
```
注册 `RequestHandlerMethodArgumentResolver` 到 Controller 参数解析器里，即添加自己的参数解析器
```java
package com.olive.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.method.support.HandlerMethodArgumentResolver;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

import java.util.List;

@Configuration
public class MethodArgumentResolverConfig {

    @Bean
    public WebMvcConfigurer getWebMvcConfigurer() {
        return new WebMvcConfigurer() {
            @Override
            public void addArgumentResolvers(List<HandlerMethodArgumentResolver> resolvers) {
                resolvers.add(new RequestHandlerMethodArgumentResolver());
            }
        };
    }
}
```
<a name="jniKM"></a>
### 测试
编码 Springboot 启动引导类
```java
package com.olive;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class);
    }

}
```
编写测试 Controller
```java
package com.olive.controller;

import java.util.HashMap;
import java.util.Map;

import com.olive.dto.HeadersWrapperDTO;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class TestController {

    @RequestMapping("/normalHeaders")
    public Map<String, Object> normalHeaders(@RequestHeader("user-id")Long userId,
                                             @RequestHeader("tenant-id")Long tenantId,
                                             @RequestHeader("user-name")String userName){
        Map<String, Object> map = new HashMap<>();
        map.put("userId", userId);
        map.put("tenantId", tenantId);
        map.put("userName", userName);
        return map;
    }

    @RequestMapping("/wrapperHeaders")
    public Map<String, Object> wrapperHeaders(HeadersWrapperDTO headers){
        Map<String, Object> map = new HashMap<>();
        map.put("userId", headers.getUserId());
        map.put("tenantId", headers.getTenantId());
        map.put("userName", headers.getUserName());
        return map;
    }

}
```
测试curl
```bash
curl -X POST \
  http://127.0.0.1:8080/wrapperHeaders \
  -H 'tenant-id: 12' \
  -H 'user-id: 1' \
  -H 'user-name: buger'
```
通过 `RequestHandlerMethodArgumentResolver` 可以对请求头进行解析并封装到 `HeadersWrapperDTO` 类中，这样减少了在 Controller 使用大量的 RequestHeader 注解获取请求头。
