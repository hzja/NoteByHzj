JavaSpringBoot
<a name="jNrSk"></a>
### 简介
现在项目中大部分采用前后端分离的架构，采用这种架构的项目，在返回数据时，几乎都是采用返回 json 格式的数据。而 Spring 中返回 json 格式的数据一般采用 `@RestController` 或者 `@ResponseBody` 注解。代码样例
```java
@ResponseBody
@RequestMapping("/reqBody")
public ResultInfo<Map<String, Object>> reqBody(){
    ResultInfo<Map<String, Object>> resultInfo = new ResultInfo<>();
    resultInfo.setCode(200);
    resultInfo.setMessage("success");

    Map<String, Object> map = new HashMap<>();
    map.put("userId", 100);
    map.put("tenantId", 1001);
    map.put("userName", "bug");
    resultInfo.setBody(map);

    return resultInfo;
}
```
**定义一个注解对返回的 json 进行加密，来运用 **`**HandlerMethodReturnValueHandler**`
<a name="DAfdx"></a>
### pom.xml 文件引入依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.olive</groupId>
  <artifactId>springmvc-response-body</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>jar</packaging>

  <name>springmvc-response-body</name>
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
    <dependency>
      <groupId>com.alibaba.fastjson2</groupId>
      <artifactId>fastjson2</artifactId>
      <version>2.0.14</version>
    </dependency>

  </dependencies>
</project>
```
<a name="ML8bz"></a>
### 定义加密注解
```java
package com.olive.annotation;

import java.lang.annotation.*;

@Target({ElementType.TYPE, ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Encrypted {

    boolean value() default true;
}
```
`Encrypted` 注解，该注解是一个标识注解；如果打上该注解标识加密
<a name="BGsZG"></a>
### 统一返回定义
主要包含 code、message 和 body 属性定义
```java
package com.olive.dto;

import lombok.Data;

import java.io.Serializable;

@Data
public class ResultInfo<T> implements Serializable {

    public  int code;

    public String message;

    private T body;

    private boolean encrypt;

}
```
<a name="vQhmi"></a>
### 自定义 `ResponseBodyHandler`
该类实现 `HandlerMethodReturnValueHandler` 类，主要对  `@RestController` 或者 `@ResponseBody` 注解进行解析
```java
package com.olive.config;

import com.alibaba.fastjson2.JSON;
import com.olive.annotation.Encrypted;
import com.olive.dto.ResultInfo;
import org.springframework.core.MethodParameter;
import org.springframework.core.annotation.AnnotatedElementUtils;
import org.springframework.util.Base64Utils;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.context.request.NativeWebRequest;
import org.springframework.web.method.support.HandlerMethodReturnValueHandler;
import org.springframework.web.method.support.ModelAndViewContainer;

import java.nio.charset.StandardCharsets;

public class ResponseBodyHandler implements HandlerMethodReturnValueHandler {

    protected final HandlerMethodReturnValueHandler handlerMethodReturnValueHandler;

    public ResponseBodyHandler(HandlerMethodReturnValueHandler handlerMethodReturnValueHandler){
        this.handlerMethodReturnValueHandler = handlerMethodReturnValueHandler;
    }

    @Override
    public boolean supportsReturnType(MethodParameter returnType) {
        //如果被@ResponseBody注解修饰的 返回true
        return (AnnotatedElementUtils.hasAnnotation(returnType.getContainingClass(), ResponseBody.class) || returnType.hasMethodAnnotation(ResponseBody.class))
            && returnType.hasMethodAnnotation(Encrypted.class);
    }

    @Override
    public void handleReturnValue(Object returnValue,
                                  MethodParameter returnType,
                                  ModelAndViewContainer mavContainer,
                                  NativeWebRequest webRequest) throws Exception {
        if(returnValue instanceof ResultInfo){
            ResultInfo<?> resultInfo = (ResultInfo<?>)returnValue;
            ResultInfo<String> newResultInfo = new ResultInfo<>();
            newResultInfo.setCode(resultInfo.getCode());
            newResultInfo.setMessage(resultInfo.getMessage());
            newResultInfo.setEncrypt(true);
            newResultInfo.setBody(Base64Utils.encodeToString(JSON.toJSONString(resultInfo.getBody()).getBytes(StandardCharsets.UTF_8)));
            //ResponseBody注解执行器
            handlerMethodReturnValueHandler.handleReturnValue(newResultInfo,
                                                              returnType, mavContainer, webRequest);
        }else{
            handlerMethodReturnValueHandler.handleReturnValue(returnValue,
                                                              returnType, mavContainer,  webRequest);
        }
    }
}
```
注册 `ResponseBodyHandler` 到 controller 返回值处理器里，即添加自己的返回值处理器
```java
package com.olive.config;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.method.support.HandlerMethodReturnValueHandler;
import org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter;
import org.springframework.web.servlet.mvc.method.annotation.RequestResponseBodyMethodProcessor;

import java.util.ArrayList;
import java.util.List;

@Configuration
public class WebConfig implements InitializingBean {

    @Autowired
    private RequestMappingHandlerAdapter adapter;

    @Override
    public void afterPropertiesSet() throws Exception {
        List<HandlerMethodReturnValueHandler> unmodifiableList = adapter.getReturnValueHandlers();
        List<HandlerMethodReturnValueHandler> list = new ArrayList<>(unmodifiableList.size());
        for (HandlerMethodReturnValueHandler returnValueHandler : unmodifiableList) {
            if (returnValueHandler instanceof RequestResponseBodyMethodProcessor) {
                //将RequestResponseBodyMethodProcessor 实际返回值替换为自定义的，实际执行为RequestResponseBodyMethodProcessor
                //重要
                HandlerMethodReturnValueHandler handler = new ResponseBodyHandler(returnValueHandler);
                list.add(handler);
            } else {
                list.add(returnValueHandler);
            }
        }
        adapter.setReturnValueHandlers(list);
    }
}
```
<a name="wEfBW"></a>
### 测试
编写 SpringBoot 启动引导类
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

import com.olive.annotation.Encrypted;
import com.olive.dto.ResultInfo;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class TestController {

    @Encrypted
    @RequestMapping("/reqBody")
    public ResultInfo<Map<String, Object>> reqBody(){
        ResultInfo<Map<String, Object>> resultInfo = new ResultInfo<>();
        resultInfo.setCode(200);
        resultInfo.setMessage("success");

        Map<String, Object> map = new HashMap<>();
        map.put("userId", 100);
        map.put("tenantId", 1001);
        map.put("userName", "bug");
        resultInfo.setBody(map);

        return resultInfo;
    }

}
```
通过 `HandlerMethodReturnValueHandler` 可以对返回的数据进行进一步的封装，减少在业务代码中进行重复的返回值处理。例如，对返回数据进行统一加密。
