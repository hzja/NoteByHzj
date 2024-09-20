JavaSpringMVC
<a name="Pm8AM"></a>
## 1、本文内容

- 全注解方式使用 SpringMVC
- 全注解方式原理解析
<a name="nxlhO"></a>
## 2、全注解方式使用 SpringMVC
<a name="yhINY"></a>
### 2.1、新建 Maven Web 项目
项目中不需要 web.xml 配置文件，Maven 配置如下
```xml
<?xml version="1.0" encoding="UTF-8"?>

<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>chat12-annotation</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>war</packaging>

  <name>chat12-annotation Maven Webapp</name>
  <description>springmvc全注解方式</description>

  <properties>
    <maven.compiler.source>8</maven.compiler.source>
    <maven.compiler.target>8</maven.compiler.target>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>

  <dependencies>
    <!-- 添加springmvc依赖 -->
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-webmvc</artifactId>
      <version>5.3.6</version>
    </dependency>

    <!-- 添加jackson配置 -->
    <dependency>
      <groupId>com.fasterxml.jackson.core</groupId>
      <artifactId>jackson-core</artifactId>
      <version>2.11.4</version>
    </dependency>

    <dependency>
      <groupId>com.fasterxml.jackson.core</groupId>
      <artifactId>jackson-databind</artifactId>
      <version>2.11.4</version>
    </dependency>

    <!-- 添加servlet 依赖 -->
    <dependency>
      <groupId>javax.servlet</groupId>
      <artifactId>javax.servlet-api</artifactId>
      <version>4.0.1</version>
      <scope>provided</scope>
    </dependency>

    <!-- 日志 -->
    <dependency>
      <groupId>ch.qos.logback</groupId>
      <artifactId>logback-classic</artifactId>
      <version>1.2.3</version>
    </dependency>

    <!--文件上传的jar包-->
    <dependency>
      <groupId>commons-fileupload</groupId>
      <artifactId>commons-fileupload</artifactId>
      <version>1.4</version>
    </dependency>
  </dependencies>

  <build>
    <finalName>chat12-annotation</finalName>
    <resources>
      <resource>
        <directory>src/main/resources</directory>
        <filtering>false</filtering>
        <includes>
          <include>**/*.*</include>
        </includes>
      </resource>
    </resources>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-war-plugin</artifactId>
        <version>2.2</version>
        <configuration>
          <failOnMissingWebXml>false</failOnMissingWebXml>
        </configuration>
      </plugin>
    </plugins>
  </build>

</project>
```
**注意**：上面配置中多了一个插件的配置，由于 Maven 在 web 项目打包的时候，发现项目中没有 web.xml，会报错，所以需要加入下面配置，让插件忽略这个问题<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683909592133-00f8d656-e648-40eb-98c2-636f7ce9378d.png#averageHue=%23fefdfd&clientId=ua25baf77-38c3-4&from=paste&id=u8c739153&originHeight=258&originWidth=736&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue421a575-06a4-4d13-a5ac-a982ee184ef&title=)
<a name="IwdIX"></a>
### 2.2、创建初始化类，代替 web.xml
在 Servlet3.0 环境中，容器会在类路径中查找实现 `javax.servlet.ServletContainerInitializer` 接口的类，如果找到的话就用它来配置 Servlet 容器。Spring 提供了这个接口的实现，名为 `SpringServletContainerInitializer`，这个类反过来又会查找实现 `WebApplicationInitializer` 的类并将配置的任务交给它们来完成。Spring3.2 引入了一个便利的 `WebApplicationInitializer` 基础实现，名为 `AbstractAnnotationConfigDispatcherServletInitializer`，当自定义的类扩展了 `AbstractAnnotationConfigDispatcherServletInitializer` 并将其部署到 Servlet3.0 容器的时候，容器会自动发现它，并用它来配置 Servlet 上下文。<br />来创建的 `MvcInit`类，需继承`AbstractAnnotationConfigDispatcherServletInitializer` ，项目启动的时候，servlet容器会自动加载这个类，这个类相当于 web.xml 的功能。
```java
import org.springframework.web.filter.CharacterEncodingFilter;
import org.springframework.web.servlet.support.AbstractAnnotationConfigDispatcherServletInitializer;

import javax.servlet.Filter;

/**
 * ①：1、创建Mvc初始化类，需要继承AbstractAnnotationConfigDispatcherServletInitializer类
 */
public class MvcInit extends AbstractAnnotationConfigDispatcherServletInitializer {
    /**
     * springmvc容器的父容器spring配置类
     * 实际工作中我们的项目比较复杂，可以将controller层放在springmvc容器中
     * 其他层，如service层、dao层放在父容器了，bean管理起来更清晰一些
     * 也可以没有父容器，将所有bean都放在springmvc容器中
     *
     * @return
     */
    @Override
    protected Class<?>[] getRootConfigClasses() {
        return new Class[0];
    }

    /**
     * ②：2、设置springmvc容器的spring配置类
     *
     * @return
     */
    @Override
    protected Class<?>[] getServletConfigClasses() {
        return new Class[]{MvcConfig.class};
    }

    /**
     * ③：3、配置DispatcherServlet的url-pattern
     *
     * @return
     */
    @Override
    protected String[] getServletMappings() {
        return new String[]{"/"};
    }

    /**
     * ④：4、注册拦截器
     *
     * @return
     */
    @Override
    protected Filter[] getServletFilters() {
        //添加拦截器，解决乱码问题
        CharacterEncodingFilter characterEncodingFilter = new CharacterEncodingFilter();
        characterEncodingFilter.setEncoding("UTF-8");
        characterEncodingFilter.setForceRequestEncoding(true);
        characterEncodingFilter.setForceResponseEncoding(true);
        return new Filter[]{characterEncodingFilter};
    }
}
```
<a name="DKci9"></a>
### 2.3、创建配置 SpringMVC 配置类，代替 SpringMVC 配置文件
下面这个类相当于 SpringMVC 配置文件的功能，SpringMVC 需要的各种组件可以在这个里面配置，大家注意啦，这个类的特点

1. 需要继承`WebMvcConfigurer`接口，这个接口中提供了很多方法，预留给开发者用来配置 SpringMVC 中的各种组件，SpringMVC 容器启动的过程中，会自动调用这些方法
2. 标注有`@Configuration` 注解，表示这是一个配置类
3. 标注有`@ComponentScan` 注解，用来扫描组件，将 bean 注册到 SpringMVC 容器
4. 需要标注`@EnableWebMvc` 注解，用来起来 SpringMVC 注解配置功能，有了这个注解，SpringMVC 容器才会自动调用 `WebMvcConfigurer` 接口中的方法
5. `WebMvcConfigurer` 接口中提供了一系列方法，用来配置视图解析器、静态资源处理器、拦截器
6. 在这个类中配置了（② 视图解析器、③ 拦截器、④ 静态资源处理器、⑤ 文件上传解析器）
```java
import com.javacode2018.springmvc.chat12.interceptor.MyInterceptor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.Ordered;
import org.springframework.web.multipart.commons.CommonsMultipartResolver;
import org.springframework.web.servlet.config.annotation.*;
import org.springframework.web.servlet.view.InternalResourceViewResolver;

/**
 * 1.开启springmvc注解配置
 * 2、配置视图解析器
 * 3、配置截器
 * 4、配置静态资源访问
 * 5、配置文件上传解析器
 * 6、配置全局异常处理器
 */
@Configuration
@ComponentScan("com.javacode2018.springmvc.chat12")
@EnableWebMvc //1：使用EnableWebMvc开启springmvc注解方式配置
public class MvcConfig implements WebMvcConfigurer {

    /**
     * ②：2、添加视图解析器（可以添加多个）
     *
     * @param registry
     */
    @Override
    public void configureViewResolvers(ViewResolverRegistry registry) {
        InternalResourceViewResolver resolver = new InternalResourceViewResolver();
        resolver.setPrefix("/WEB-INF/view/");
        resolver.setSuffix(".jsp");
        resolver.setOrder(Ordered.LOWEST_PRECEDENCE);
        registry.viewResolver(resolver);
    }

    @Autowired
    private MyInterceptor myInterceptor;

    /**
     * ③：3、添加拦截器（可以添加多个）
     *
     * @param registry
     */
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(this.myInterceptor).addPathPatterns("/**");
    }


    /**
     * ④：4、配置静态资源访问处理器
     *
     * @param registry
     */
    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry.addResourceHandler("/static/**").addResourceLocations("/static/");
    }

    /**
     * ⑤：5、配置文件上传解析器
     *
     * @return
     */
    @Bean
    public CommonsMultipartResolver multipartResolver() {
        CommonsMultipartResolver commonsMultipartResolver = new CommonsMultipartResolver();
        //maxUploadSizePerFile:单个文件大小限制（byte）
        //maxUploadSize：整个请求大小限制（byte）
        commonsMultipartResolver.setMaxUploadSizePerFile(10 * 1024 * 1024);
        commonsMultipartResolver.setMaxUploadSize(100 * 1024 * 1024);
        return commonsMultipartResolver;
    }
}
```
<a name="KQBSC"></a>
### 2.4、创建自定义拦截器
上面的MvcConfig配置类中，定义了一个拦截器`MyInterceptor myInterceptor;`，这个类的代码如下
```java
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.HandlerInterceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

@Component
public class MyInterceptor implements HandlerInterceptor {

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        System.out.println("这是MyInterceptor拦截器");
        return true;
    }
}
```
<a name="RA2Xl"></a>
### 2.5、创建全局异常处理类
异常处理，也添加上，添加一个类，当出错的时候，跳转到错误页面。
```java
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.servlet.ModelAndView;

/**
 * 异常处理
 */
@ControllerAdvice
public class GlobalExceptionHandler {
    @ExceptionHandler
    public ModelAndView doException(Exception e) {
        ModelAndView modelAndView = new ModelAndView();
        modelAndView.setViewName("error");
        modelAndView.addObject("ex", e);
        return modelAndView;
    }

}
```
<a name="K8Gs9"></a>
### 2.6、测试功能
添加一个 controller 及几个 jsp 页面，测效果
```java
@Controller
public class IndexController {
    /**
     * 首页
     *
     * @return
     */
    @RequestMapping("/")
    public String index() {
        return "index";
    }

    /**
     * 测试异常情况
     *
     * @return
     */
    @RequestMapping("/testError")
    public String testError() {
        System.out.println(10 / 0);
        return "success";
    }
}
```
webapp/WEB-INF/view 中创建 3 个页面<br />index.jsp:
```html
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
  <html>
    <head>
      <title>Title</title>
    </head>
    <body>
      <h2>全注解的方式配置springmvc</h2><br/>
      <a target="_blank" href="${pageContext.request.contextPath}/static/imgs/1.jpg">测试访问静态资源</a><br/>
      <a target="_blank" href="${pageContext.request.contextPath}/testError">测试触发全局异常处理</a>
    </body>
  </html>
```
error.jsp，错误跳转的页面，会显示异常信息
```html
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
  <html>
    <head>
      <title>Title</title>
    </head>
    <body>
      <h2>出错啦，错误信息如下：</h2>
      <h3>${ex}</h3>
    </body>
  </html>
```
success.jsp
```html
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
  <html>
    <head>
      <title>Title</title>
    </head>
    <body>
      <h2>success</h2>
    </body>
  </html>
```
搞一个图片放在 webapp/static/imgs 中，稍后测试静态资源访问的效果。
<a name="Xyffl"></a>
### 2.7、项目整体结构
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683909611280-b18a75b7-a8be-4b91-8656-3344737934f3.png#averageHue=%23fdfcfb&clientId=ua25baf77-38c3-4&from=paste&id=u1efed88b&originHeight=647&originWidth=504&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0a9cb58c-6161-46b1-a6da-8be3d963d27&title=)
<a name="LNUIA"></a>
### 2.8、测试效果
项目发布到 Tomcat，访问首页，首页上有 2 个连接，可以点击一下，分别用来测试静态资源是否可以访问，另外一个测试全局异常处理的效果。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683909611134-98d34096-3c3a-4fa1-a23b-3d22f429efb4.png#averageHue=%23f4f2f1&clientId=ua25baf77-38c3-4&from=paste&id=u67a9e074&originHeight=166&originWidth=393&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udd828c3c-55b4-464b-abb5-b2a56af81a9&title=)<br />点击连接 1 效果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683909611238-d65bc875-412e-44e8-aea9-379ec02c69e5.png#averageHue=%23719635&clientId=ua25baf77-38c3-4&from=paste&id=u24991c4e&originHeight=369&originWidth=564&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8fb2a1b6-f3f9-4340-b2d0-a58c6a865f3&title=)<br />点击连接 2 效果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683909611287-11950de2-e605-4c03-a624-d49cf09b3d37.png#averageHue=%23f1f0ee&clientId=ua25baf77-38c3-4&from=paste&id=ue52299fe&originHeight=158&originWidth=464&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u23a46eac-7328-4061-bd5b-f5f8445a553&title=)
<a name="U0qDj"></a>
## 3、原理：`ServletContainerInitializer` 接口
刚才上面 2.2 章节中有提到过，重点在于 Servlet3.0 环境中，容器会在类路径中查找实现 `javax.servlet.ServletContainerInitializer` 接口的类，如果找到的话就用它来配置 Servlet 容器，servlet3.0 赋予了 web 项目免去所有配置文件（web.xml）的能力。<br />所以重点就在于`ServletContainerInitializer`这个接口上，SpringMVC 全注解方式就是依靠这个接口来实现的，掌握了这个接口的用法，SpringMVC 全注解的原理大家基本上就搞懂了，对阅读 SpringMVC 源码也是非常有利的。<br />下面看来这个接口的用法。
<a name="VHs1k"></a>
### 3.1、`ServletContainerInitializer` 源码
这个接口比较简单，只有一个 `onStartup` 方法，web 容器启动的时候会自动调用这个方法，有 2 个参数，第 1 个参数稍后介绍，第 2 个参数 ctx 是 servlet 上下文，通过 servlet 上下文对象，可以在这个方法中实现 web.xml 的所有操作。
```java
public interface ServletContainerInitializer {
    public void onStartup(Set<Class<?>> c, ServletContext ctx)
    throws ServletException;
}
```
<a name="zDLBR"></a>
### 3.2、`ServletContainerInitializer` 使用
1、可以自定义一个实现 `ServletContainerInitializer` 接口，这个类必须在 jar 包的 `META-INF/services/javax.servlet.ServletContainerInitializer` 文件里面进行声明，这个文件的内容就是自定义类的全类名<br />2、Servlet 容器启动会在所有 jar 和 classes 目录中扫描 `META-INF/services/javax.servlet.ServletContainerInitializer` 文件，然后找到这个文件中的具体的类，然后会自动实例化这个类，调用这个类的`onStartup`方法
<a name="HPknr"></a>
### 3.2、`onStartup` 的第 1 个参数，`@HandlesTypes` 注解
提到 `onStartup` 方法的第一个参数，这里就需要介绍一下`@HandlesTypes` 这个注解，先来看一下其源码，比较简单，就只有一个 Calss 数组类型的 value 属性。
```java
Target({ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
public @interface HandlesTypes {
    Class<?>[] value();
}
```
1、`@HandlesTypes` 标签用在实现 `ServletContainerInitializer` 接口的类上面，比如：
```java
@HandlesTypes(WebApplicationInitializer.class)
public class SpringServletContainerInitializer implements ServletContainerInitializer
```
2、servlet 容器会扫描项目中的所有类（jar 包和 classes 路径中），如果符合`@HandlesTypes` 注解 value 值指定的类型，就会放在一个数组中，最终会传递给 onStartup 方法的第一个参数<br />3、当容器启动的时候，就可以通过拿到 `Set<Class<?>> c` 里面的类，然后做一些初始化的工作
<a name="m5Wpn"></a>
## 3、SpringMVC 全注解的原理
了解了`ServletContainerInitializer`接口的原理，来看 SpringMVC，spring-web.jar 中包含了`META-INF/services/javax.servlet.ServletContainerInitializer`文件<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683909610959-41912cbe-8db6-407d-b34c-09a4da154e09.png#averageHue=%23f3edd3&clientId=ua25baf77-38c3-4&from=paste&id=u83f03b83&originHeight=304&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue5445e58-0b64-4056-9385-23e84e7c63c&title=)<br />这个文件中指定的是`org.springframework.web.SpringServletContainerInitializer`这个类，重点来了，SpringMVC 就是依靠这个类来实现注解功能的，大家可以去看看这个类的源码，在其 `onStartup` 方法中添加断点，可以看到完整清晰的启动过程。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683909611617-0114197c-d6dc-48d6-ab72-9378885f18e4.png#averageHue=%23fcfbf9&clientId=ua25baf77-38c3-4&from=paste&id=u24b9763d&originHeight=111&originWidth=903&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u36f66af8-603c-42f9-9499-c0bcb5a036f&title=)
