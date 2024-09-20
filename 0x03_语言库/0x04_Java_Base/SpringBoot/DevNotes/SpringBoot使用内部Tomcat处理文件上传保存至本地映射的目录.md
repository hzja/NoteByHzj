Java SpringBoot
<a name="6180e2a3"></a>
## 1、前端上传
<a name="iZaP7"></a>
### A、Form表单上传

<a name="2oFnl"></a>
### B、AJAX上传

<a name="NS6bk"></a>
## 2、后台保存
<a name="WVgjG"></a>
### A、保存本地进行目录映射
<a name="jCgUP"></a>
#### ①、在配置文件配置请求的目录和本地的映射目录-配置式便于更改，不用直接写死在文件中
<a name="nkBgl"></a>
##### application.yml
```xml
uploadFile:
  # 请求 url 中的资源映射配置
  resourceHandler: /uploadFiles/**
  # 自定义上传文件本地保存路径
  location: D:\Competition\uploadFiles\
```
<a name="2RwgF"></a>
#### ②、添加配置类实现WebMvcConfigurer接口
<a name="rw1wS"></a>
##### InterceptorConfig.java
```java
package com.fcant.competition.config;

import com.fcant.competition.config.interceptor.AuthenticationInterceptor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;
import org.springframework.web.servlet.resource.PathResourceResolver;
import org.springframework.web.servlet.resource.WebJarsResourceResolver;

/**
 * InterceptorConfig
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 15:11:18 2020/4/6/0006
 */
@Configuration
public class InterceptorConfig implements WebMvcConfigurer {

    @Value("${uploadFile.resourceHandler}")
    private String resourceHandler;

    @Value("${uploadFile.location}")
    private String location;

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(authenticationInterceptor())
                .addPathPatterns("/**");
    }

    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry.addResourceHandler("/static/**")
                .addResourceLocations("classpath:/static/");
        // 配置文件上传的本地映射路径
        registry.addResourceHandler(resourceHandler)
                .addResourceLocations("file:///" + location);
        /**
         * 注册静态资源webjar的映射路径
         */
        registry.addResourceHandler("/webjars/**")
                .addResourceLocations("classpath:/META-INF/resources/webjars/")
                .resourceChain(false)
                .addResolver(new WebJarsResourceResolver())
                .addResolver(new PathResourceResolver());
    }
}
```
<a name="Db6P5"></a>
#### ③、文件保存处理的接口
<a name="wJZPx"></a>
##### FileUploadController.java
```java
package com.fcant.competition.api.v1;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.IOException;

/**
 * FileUploadController
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 上午 09:35:01 2020/4/10/0010
 */
@Controller
@ResponseBody
@RequestMapping("/file")
public class FileController {

    @Value("${uploadFile.location}")
    private String location;

    @RequestMapping("/upload")
    public void saveFile(MultipartFile uploadFile) {
        File file = new File(location, uploadFile.getOriginalFilename());
        File dir = new File(location);
        if (!dir.exists()) {
            // 使用mkdirs创建多级目录
            dir.mkdirs();
        }
        try {
            uploadFile.transferTo(file);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

```
<a name="b6a887dc"></a>
### B、保存至class文件的编译目录（不推荐：不利于打包部署至服务器）
<a name="8MMVZ"></a>
## 3、配置大文件上传
SpringBoot默认每个文件的配置最大为1Mb，单次请求的文件的总数不能大于10Mb。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1609210365123-24d26c48-3965-4080-9bba-92119de29a2c.png#averageHue=%23e8e7e7&height=70&id=p326l&originHeight=210&originWidth=2520&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70120&status=done&style=none&title=&width=840)<br />`**multipart.maxFileSize=10Mb**`是设置单个文件的大小,** **`**multipart.maxRequestSize=100Mb**`是设置单次请求的文件的总大小，如果是想要不限制文件上传的大小，那么就把两个值都设置为-1就可以了。<br />**SpringBoot1.4**之前的版本配置参数
```
multipart.maxFileSize=10Mb
multipart.maxRequestSize=100Mb
```
**Spring Boot1.4**版本后配置更改为:
```
spring.http.multipart.maxFileSize=10Mb  
spring.http.multipart.maxRequestSize=100Mb
```
**Spring Boot2.0**之后的版本配置修改为:
```
spring.servlet.multipart.max-file-size=10MB  
spring.servlet.multipart.max-request-size=100MB
```
