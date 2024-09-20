JavaWeb Servlet
<a name="246bdb0a"></a>
## 基于 Servlet 开发
在还没有框架之前，编程式基于原始的 `Servlet` 进行开发，下面就基于原生的 `Servlet` 来完成一个简单的接口调用。
<a name="Ku89n"></a>
### 1、`pom` 文件引入依赖
需要注意的是，`package` 属性要设置成 `war` 包，为了节省篇幅，这里没有列出 `pom` 完整的信息：
```xml
<packaging>war</packaging> 
<dependencies>
  <dependency>
    <groupId>javax.servlet</groupId>
    <artifactId>servlet-api</artifactId>
    <version>2.4</version>
  </dependency>
  <dependency>
    <groupId>org.apache.commons</groupId>
    <artifactId>commons-lang3</artifactId>
    <version>3.7</version>
  </dependency>
  <dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>fastjson</artifactId>
    <version>1.2.72</version>
  </dependency>
</dependencies>
```
<a name="bwnzC"></a>
### 2、在 `src/main` 下面新建文件夹 `webapp/WEB-INF`
然后在 `WEB-INF` 下面新建一个 `web.xml` 文件：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns="http://java.sun.com/xml/ns/j2ee" xmlns:javaee="http://java.sun.com/xml/ns/javaee"
         xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
         xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd"
         version="2.4">
  <display-name>Lonely Wolf Web Application</display-name>
  <servlet>
    <servlet-name>helloServlet</servlet-name>
    <servlet-class>com.lonely.wolf.mini.spring.servlet.HelloServlet</servlet-class>
  </servlet>
  <servlet-mapping>
    <servlet-name>helloServlet</servlet-name>
    <url-pattern>/hello/*</url-pattern>
  </servlet-mapping>
</web-app>
```
这里面定义了 `selvlet` 和 `servlet-mapping` 两个标签，这两个标签必须一一对应，上面的标签定义了 `servlet` 的位置，而下面的 `servlet-mapping` 文件定义了路径的映射，这两个标签通过 `servlet-name` 标签对应。
<a name="JWHi9"></a>
### 3、新建一个 `HelloServlet` 类继承 `HttpServlet`
```java
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
/**
 * 原始Servlet接口编写，一般需要实现GET和POST方法，其他方法可以视具体情况选择性继承
 */
public class HelloServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doPost(request,response);
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        response.setContentType("text/html;charset=utf-8");
        response.getWriter().write("Hello：" + request.getParameter("name"));
    }
}
```
<a name="bP8TI"></a>
### 4、执行Maven打包命令
确认成功打包成 `war` 包，在项目的目录下会生成target目录，项目会包含一个后缀为war的包文件。
<a name="3m1rY"></a>
### 5、IDEA配置Tomcat
`RUN-->Edit Configurations`，然后点击左上角的 `+` 号，新建一个 `Tomcat Server`，如果是第一次配置，默认没有 `Tomcat Server` 选项，需要点击底部的 `xx more items...`：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614670146743-c60b69e8-f3c5-476b-bf8c-1a5137e859a2.png#averageHue=%23f0f0f0&height=590&id=GoqMU&originHeight=1771&originWidth=2683&originalType=binary&ratio=1&rotation=0&showTitle=false&size=305975&status=done&style=none&title=&width=894.3333333333334)<br />点击右边的 `Deployment`，然后按照下图依次点击，最后在弹框内找到上面打包好的 `war` 包文件：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614670206088-da382a88-73f6-448b-a8d8-cea28c8f8b26.png#averageHue=%23f7f6f6&height=590&id=Zlwff&originHeight=1771&originWidth=2683&originalType=binary&ratio=1&rotation=0&showTitle=false&size=220187&status=done&style=none&title=&width=894.3333333333334)<br />选中之后，需要注意的是，下面 `Application Context` 默认会带上 `war` 包名，为了方便，需要把它删掉，即不用上下文路径，只保留一个根路径 `/` （当然上下文也可以保留，但是每次请求都要带上这一部分）， 再选择 `Apply`，点击 `OK`，即可完成部署。<br />最后在浏览器输入请求路径`http://localhost:8080/hello?name=Fcant`，即可得到返回：`Hello：Fcant`。<br />上面就完成了一个简单的 基于`Servlet` 的接口开发，可以看到，配置非常麻烦，每增加一个 `Servlet` 都需要增加对应的配置，所以才会有许多框架的出现来简化开发，比如原来很流行的 `Struts2` 框架，当然现在除了一些比较老的项目，一般都很少使用，而更多的是选择 `Spring` 框架来进行开发。
<a name="27d583f3"></a>
## 模仿Spring
`Spring` 的源码体系非常庞大，大部分人对其源码都敬而远之。确实，`Spring` 毕竟经过了这么多年的迭代，功能丰富，项目庞大，不是一下子就能看懂的。虽然 `Spring` 难以理解，但是其最核心的思想仍然是上面介绍的几点，接下来就基于 `Spring` 最核心的部分来模拟，实现一个超级迷你版本的 `Spring`（此版本并不包含 `AOP` 功能）。
<a name="RBXbW"></a>
### 1、配置请求拦截
`pom` 依赖和上面保持不变，然后 `web.xml` 作如下改变，这里会拦截所有的接口 `/*`，然后多配置了一个参数，这个参数其实也是为了更形象的模拟 `Spring`：
```xml
<servlet>
    <servlet-name>myDispatcherServlet</servlet-name>
    <servlet-class>com.lonely.wolf.mini.spring.v1.MyDispatcherServlet</servlet-class>
    <init-param>
        <param-name>defaultConfig</param-name>
        <param-value>application.properties</param-value>
    </init-param>
</servlet>
<servlet-mapping>
    <servlet-name>myDispatcherServlet</servlet-name>
    <url-pattern>/*</url-pattern>
</servlet-mapping>
```
<a name="9d208ca2"></a>
### 2、创建配置文件用来定义扫描的基本路径
在 `respurces` 下面新建一个配置文件 `application.properties`，用来定义扫描的基本路径：
```
basePackages=com.fcant.mini.spring
```
<a name="2ebd2d5e"></a>
### 3、创建一些相关的注解类
```java
package com.fcant.mini.spring.annotation;

import java.lang.annotation.*;

@Target({ElementType.FIELD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface WolfAutowired {
    String value() default "";
}

package com.fcant.mini.spring.annotation;

import java.lang.annotation.*;
@Target({ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface WolfController {
    String value() default "";
}

package com.fcant.mini.spring.annotation;

import java.lang.annotation.*;
@Target({ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface WolfGetMapping {
    String value() default "";
}

package com.fcant.mini.spring.annotation;

import java.lang.annotation.*;
@Target({ElementType.PARAMETER})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface WolfRequestParam {
    String value() default "";
}

package com.fcant.mini.spring.annotation;

import java.lang.annotation.*;
@Target({ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface WolfService {
    String value() default "";
}
```

- 4、这个时候最核心的逻辑就是 `MyDispatcherServlet` 类了：
```java
package com.fcant.mini.spring.v1;
import com.lonely.wolf.mini.spring.annotation.*;
import com.lonely.wolf.mini.spring.v1.config.MyConfig;
import org.apache.commons.lang3.StringUtils;
import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.net.URL;
import java.util.*;
public class MyDispatcherServlet extends HttpServlet {
    private MyConfig myConfig = new MyConfig();
    private List<String> classNameList = new ArrayList<String>();
    private Map<String,Object> iocContainerMap = new HashMap<>();
    private Map<String,HandlerMapping> handlerMappingMap = new HashMap<>();
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doPost(request,response);
    }
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        try {
            this.doDispatch(request, response);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    private void doDispatch(HttpServletRequest request, HttpServletResponse response) throws Exception{
        String requestUrl = this.formatUrl(request.getRequestURI());
        HandlerMapping handlerMapping = handlerMappingMap.get(requestUrl);
        if (null == handlerMapping){
            response.getWriter().write("404 Not Found");
            return;
        }
        //获取方法中的参数类型
        Class<?>[] paramTypeArr = handlerMapping.getMethod().getParameterTypes();
        Object[] paramArr = new Object[paramTypeArr.length];
        for (int i=0;i<paramTypeArr.length;i++){
            Class<?> clazz = paramTypeArr[i];
            //参数只考虑三种类型，其他不考虑
            if (clazz == HttpServletRequest.class){
                paramArr[i] = request;
            }else if (clazz == HttpServletResponse.class){
                paramArr[i] = response;
            } else if (clazz == String.class){
                Map<Integer,String> methodParam = handlerMapping.getMethodParams();
                paramArr[i] = request.getParameter(methodParam.get(i));
            }else{
                System.out.println("暂不支持的参数类型");
            }
        }
        //反射调用controller方法
        handlerMapping.getMethod().invoke(handlerMapping.getTarget(), paramArr);
    }
    private String formatUrl(String requestUrl) {
        requestUrl = requestUrl.replaceAll("/+","/");
        if (requestUrl.lastIndexOf("/") == requestUrl.length() -1){
            requestUrl = requestUrl.substring(0,requestUrl.length() -1);
        }
        return requestUrl;
    }
    @Override
    public void init(ServletConfig config) throws ServletException {
        //1.加载配置文件
        try {
            doLoadConfig(config.getInitParameter("defaultConfig"));
        } catch (Exception e) {
            System.out.println("加载配置文件失败");
            return;
        }
        //2.根据获取到的扫描路径进行扫描
        doScanPacakge(myConfig.getBasePackages());
        //3.将扫描到的类进行初始化，并存放到IOC容器
        doInitializedClass();
        //4.依赖注入
        doDependencyInjection();
        System.out.println("DispatchServlet Init End..." );
    }
    private void doDependencyInjection() {
        if (iocContainerMap.size() == 0){
            return;
        }
        //循环IOC容器中的类
        Iterator<Map.Entry<String,Object>> iterator = iocContainerMap.entrySet().iterator();
        while (iterator.hasNext()){
            Map.Entry<String,Object> entry = iterator.next();
            Class<?> clazz = entry.getValue().getClass();
            Field[] fields = clazz.getDeclaredFields();
            //属性注入
            for (Field field : fields){
                //如果属性有WolfAutowired注解则注入值（暂时不考虑其他注解）
                if (field.isAnnotationPresent(WolfAutowired.class)){
                    String value = toLowerFirstLetterCase(field.getType().getSimpleName());//默认bean的value为类名首字母小写
                    if (field.getType().isAnnotationPresent(WolfService.class)){
                        WolfService wolfService = field.getType().getAnnotation(WolfService.class);
                        value = wolfService.value();
                    }
                    field.setAccessible(true);
                    try {
                        Object target = iocContainerMap.get(beanName);
                        if (null == target){
                            System.out.println(clazz.getName() + "required bean:" + beanName + ",but we not found it");
                        }
                        field.set(entry.getValue(),iocContainerMap.get(beanName));//初始化对象，后面注入
                    } catch (IllegalAccessException e) {
                        e.printStackTrace();
                    }
                }
            }
            //初始化HanderMapping
            String requestUrl = "";
            //获取Controller类上的请求路径
            if (clazz.isAnnotationPresent(WolfController.class)){
                requestUrl = clazz.getAnnotation(WolfController.class).value();
            }
            //循环类中的方法，获取方法上的路径
            Method[] methods = clazz.getMethods();
            for (Method method : methods){
                //假设只有WolfGetMapping这一种注解
                if(!method.isAnnotationPresent(WolfGetMapping.class)){
                    continue;
                }
                WolfGetMapping wolfGetMapping = method.getDeclaredAnnotation(WolfGetMapping.class);
                requestUrl = requestUrl + "/" + wolfGetMapping.value();//拼成完成的请求路径
                //不考虑正则匹配路径/xx/* 的情况，只考虑完全匹配的情况
                if (handlerMappingMap.containsKey(requestUrl)){
                    System.out.println("重复路径");
                    continue;
                }
                Annotation[][] annotationArr = method.getParameterAnnotations();//获取方法中参数的注解
                Map<Integer,String> methodParam = new HashMap<>();//存储参数的顺序和参数名
                retryParam:
                for (int i=0;i<annotationArr.length;i++){
                    for (Annotation annotation : annotationArr[i]){
                        if (annotation instanceof WolfRequestParam){
                            WolfRequestParam wolfRequestParam = (WolfRequestParam) annotation;
                            methodParam.put(i,wolfRequestParam.value());//存储参数的位置和注解中定义的参数名
                            continue retryParam;
                        }
                    }
                }
                requestUrl = this.formatUrl(requestUrl);//主要是防止路径多了/导致路径匹配不上
                HandlerMapping handlerMapping = new HandlerMapping();
                handlerMapping.setRequestUrl(requestUrl);//请求路径
                handlerMapping.setMethod(method);//请求方法
                handlerMapping.setTarget(entry.getValue());//请求方法所在controller对象
                handlerMapping.setMethodParams(methodParam);//请求方法的参数信息
                handlerMappingMap.put(requestUrl,handlerMapping);//存入hashmap
            }
        }
    }
    /**
     * 初始化类，并放入容器iocContainerMap内
     */
    private void doInitializedClass() {
        if (classNameList.isEmpty()){
            return;
        }
        for (String className : classNameList){
            if (StringUtils.isEmpty(className)){
                continue;
            }
            Class clazz;
            try {
                clazz = Class.forName(className);//反射获取对象
                if (clazz.isAnnotationPresent(WolfController.class)){
                    String value = ((WolfController)clazz.getAnnotation(WolfController.class)).value();
                    //如果直接指定了value则取value，否则取首字母小写类名作为key值存储类的实例对象
                    iocContainerMap.put(StringUtils.isBlank(value) ? toLowerFirstLetterCase(clazz.getSimpleName()) : value,clazz.newInstance());
                }else if(clazz.isAnnotationPresent(WolfService.class)){
                    String value = ((WolfService)clazz.getAnnotation(WolfService.class)).value();
                    iocContainerMap.put(StringUtils.isBlank(value) ? toLowerFirstLetterCase(clazz.getSimpleName()) : value,clazz.newInstance());
                }else{
                    System.out.println("不考虑其他注解的情况");
                }
            } catch (Exception e) {
                e.printStackTrace();
                System.out.println("初始化类失败，className为" + className);
            }
        }
    }
    /**
     * 将首字母转换为小写
     * @param className
     * @return
     */
    private String toLowerFirstLetterCase(String className) {
        if (StringUtils.isBlank(className)){
            return "";
        }
        String firstLetter = className.substring(0,1);
        return firstLetter.toLowerCase() + className.substring(1);
    }
    /**
     * 扫描包下所有文件获取全限定类名
     * @param basePackages
     */
    private void doScanPacakge(String basePackages) {
        if (StringUtils.isBlank(basePackages)){
            return;
        }
        //把包名的.替换为/
        String scanPath = "/" + basePackages.replaceAll("\\.","/");
        URL url = this.getClass().getClassLoader().getResource(scanPath);//获取到当前包所在磁盘的全路径
        File files = new File(url.getFile());//获取当前路径下所有文件
        for (File file : files.listFiles()){//开始扫描路径下的所有文件
            if (file.isDirectory()){//如果是文件夹则递归
                doScanPacakge(basePackages + "." + file.getName());
            }else{//如果是文件则添加到集合。因为上面是通过类加载器获取到的文件路径，所以实际上是class文件所在路径
                classNameList.add(basePackages + "." + file.getName().replace(".class",""));
            }
        }
    }
    /**
     * 加载配置文件
     * @param configPath - 配置文件所在路径
     */
    private void doLoadConfig(String configPath) {
        InputStream inputStream = this.getClass().getClassLoader().getResourceAsStream(configPath);
        Properties properties = new Properties();
        try {
            properties.load(inputStream);
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("加载配置文件失败");
        }
        properties.forEach((k, v) -> {
            try {
                Field field = myConfig.getClass().getDeclaredField((String)k);
                field.setAccessible(true);
                field.set(myConfig,v);
            } catch (Exception e) {
                e.printStackTrace();
                System.out.println("初始化配置类失败");
                return;
            }
        });
    }
}
```
<a name="eMuyM"></a>
### 4、Servlet处理类的说明
这个 `Servlet` 相比较于上面的 `HelloServlet` 多了一个 `init` 方法，这个方法中主要做了以下几件事情：<br />（1）初始化配置文件，拿到配置文件中配置的参数信息（对应方法：`doLoadConfig`）。<br />（2）拿到第 `1` 步加载出来的配置文件，获取到需要扫描的包路径，然后将包路径进行转换成实际的磁盘路径，并开始遍历磁盘路径下的所有 `class` 文件，最终经过转换之后得到扫描路径下的所有类的全限定类型，存储到全局变量 `classNameList` 中（对应方法：`doScanPacakge`）。<br />（3）根据第 `2` 步中得到的全局变量 `classNameList` 中的类通过反射进行初始化（需要注意的是只会初始化加了指定注解的类）并将得到的对应关系存储到全局变量 `iocContainerMap` 中（即传说中的 `IOC` 容器），其中 `key` 值为注解中的 `value` 属性，如 `value` 属性为空，则默认取首字母小写的类名作为 `key` 值进行存储（对应方法：`doInitializedClass`）。<br />（4）这一步比较关键，需要对 `IOC` 容器中的所有类的属性进行赋值并且需要对 `Controller` 中的请求路径进行映射存储，为了确保最后能顺利调用 `Controller` 中的方法，还需要将方法的参数进行存储 。对属性进行映射时只会对加了注解的属性进行映射，映射时会从 `IOC` 容器中取出第 `3` 步中已经初始化的实例对象进行赋值，最后将请求路径和 `Controller` 中方法的映射关系存入变量 `handlerMappingMap`，`key` 值为请求路径，`value` 为方法的相关信息 （对应方法：`doDependencyInjection`）。<br />存储请求路径和方法的映射关系时，需要用到 `HandlerMapping` 类来进行存储：
```java
package com.fcant.mini.spring.v1;

import java.lang.reflect.Method;
import java.util.Map;

//省略了getter/setter方法
public class HandlerMapping {
    private String requestUrl;
    private Object target;//保存方法对应的实例
    private Method method;//保存映射的方法
    private Map<Integer,String> methodParams;//记录方法参数
}
```
初始化完成之后，因为拦截了 `/*` ，所以调用任意接口都会进入 `MyDispatcherServlet` ，而且最终都会执行方法 `doDispatch`，执行这个方法时会拿到请求的路径，然后和全局变量 `handlerMappingMap` 进行匹配，匹配不上则返回 `404`，匹配的上则取出必要的参数进行赋值，最后通过反射调用到 `Controller` 中的相关方法。<br />新建一个 `HelloController` 和 `HelloService` 来进行测试：
```java
package com.fcant.mini.spring.controller;

import com.lonely.wolf.mini.spring.annotation.WolfAutowired;
import com.lonely.wolf.mini.spring.annotation.WolfController;
import com.lonely.wolf.mini.spring.annotation.WolfGetMapping;
import com.lonely.wolf.mini.spring.annotation.WolfRequestParam;
import com.lonely.wolf.mini.spring.service.HelloService;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

@WolfController
public class HelloController {
    @WolfAutowired
    private HelloService helloService;
    @WolfGetMapping("/hello")
    public void query(HttpServletRequest request,HttpServletResponse response, @WolfRequestParam("name") String name) throws IOException {
        response.setContentType("text/html;charset=utf-8");
        response.getWriter().write("Hello：" + name);
    }
}

package com.lonely.wolf.mini.spring.service;

import com.lonely.wolf.mini.spring.annotation.WolfService;

@WolfService(value = "hello_service")//为了演示能否正常取value属性
public class HelloService {
}
```
输入测试路径：`http://localhost:8080/hello?name=Fcant`， 进行测试发现可以正常输出：`Hello：Fcant`。
<a name="GpnT6"></a>
## 总结
例子中很多细节都没有进行处理，仅仅只是为了体验一下 `Spring` 的核心思想，并了解 `Spring` 到底做了什么，实际上 `Spring` 做的事情远比这个例子中多得多，`Spring` 体系庞大，设计优雅，经过了多年的迭代优化，是一款非常值得研究的框架。
