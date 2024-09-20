JavaSpringSpringMVC
<a name="NyCp4"></a>
## 容器初始化过程
web.xml配置文件
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd"
  version="4.0">

  <!--定义web应用的名称-->
  <display-name>ssmdemo</display-name>

  <context-param>
    <param-name>contextConfigLocation</param-name>
    <param-value>/WEB-INF/spring/spring-context.xml</param-value>
  </context-param>

  <!-- Spring上下文监听器，用来加载Spring的上下文配置并初始化Spring -->
  <listener>
    <description>启动spring容器</description>
    <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
  </listener>
  <!-- LOG4J上下文监听器，用来加载LOG4J2的配置并初始化LOG4J -->
  <listener>
    <listener-class>org.apache.logging.log4j.web.Log4jServletContextListener</listener-class>
  </listener>

  <!-- 字符编码过滤器，将编码改为UTF-8-->
  <filter>
    <filter-name>encodingFilter</filter-name>
    <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
    <init-param>
      <param-name>encoding</param-name>
      <param-value>UTF-8</param-value>
    </init-param>
    <init-param>
      <param-name>forceEncoding</param-name>
      <param-value>true</param-value>
    </init-param>
  </filter>

  <!--对所有的请求都进行过滤-->
  <filter-mapping>
    <filter-name>encodingFilter</filter-name>
    <url-pattern>/*</url-pattern>
  </filter-mapping>

  <!-- SpringMVC前置控制器，拦截匹配的请求，把拦截下来的请求，根据相应的规则分发到目标Controller来处理-->
  <servlet>
    <servlet-name>spring-mvc</servlet-name>
    <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
    <!-- 指定路径SpringMVC上下文配置路径，也可以使用默认的规则，即：/WEB-INF/<servlet-name>-servlet.xml，如spring-mvc-servlet.xml-->
    <init-param>
      <param-name>contextConfigLocation</param-name>
      <param-value>/WEB-INF/spring/spring-mvc.xml</param-value>
    </init-param>
    <!-- 随web应用启动而启动 -->
    <load-on-startup>1</load-on-startup>
  </servlet>

  <servlet-mapping>
    <servlet-name>spring-mvc</servlet-name>
    <!--指定所有请求都通过DispatcherServlet来处理-->
    <url-pattern>/</url-pattern>
  </servlet-mapping>
</web-app>
```
<a name="tSE6Y"></a>
### 初始化过程

1. Web服务器（如Tomcat）启动
2. Tomcat读取server.xml配置文件，加载里面配置的Web应用，为每个应用创建一个全局上下文环境`ServletContext`（`ApplicationContextFacade`）；
3. Tomcat读取web.xml配置文件，开始按顺序加载（context-param、listener、filter、servlet）；
4. 获取context-param节点信息，保存到`ServletContext`中（值为Spring配置文件的位置）；
5. 获取listener节点信息，创建Listener实例集合（开始监听），其中的`ContextLoaderListener`监听器就是用来完成Spring容器初始化的。
6. `ContextLoaderListener`监听触发，完成Spring容器（`XmlWebApplicationContext`）的初始化；
- 创建Spring容器，通过`ServletContext.getinitialParameter("contextConfigLoaction")`从`ServletContext`中获取Spring配置文件的位置；
- 读取Spring配置文件信息，将相关Bean加载到Spring容器中。
7. Spring容器加载完成，将容器保存到`ServletContext`中；
- Spring容器对象以`org.springframework.web.context.WebApplicationContext.ROOT`为key的key-value形式存入到`ServletContext.context.attributes`属性中。
8. 获取filter节点信息，创建Filter实例集合，保存到`ServletContext`中；
9. 获取servlet节点信息，创建Servlet实例集合，其中的`DispatcherServlet`就是用来完成SpringMVC容器（`XmlWebApplicationContext`）初始化的；
- 根据servlet节点中的servlet-class创建相应的Servlet；
- 其中`DispatcherServlet`的初始过程，进行SpringMVC容器初始化；
- 读取该servlet节点中的SpringMVC配置文件信息，将相关Bean加载到SpringMVC容器中；
- 设置Spring容器为SpringMVC容器的父容器。
- 当执行SpringMVC容器的getBean方法在自己的context中找不到对应的bean，则会在父容器Spring容器的context中找对应的bean。
10. SpringMVC容器初始化完成，将容器保存到ServletContext中。
- SpringMVC容器对象以`org.springframework.web.servlet.FrameworkServlet.CONTEXT.xxx`（其中xxx为servlet-name节点的值）为key的key-value形式存入到`ServletContext.context.attributes`属性中。

至此，SSM项目启动完成。
<a name="NeNZl"></a>
### 简单概括初始化过程

- Web服务器启动
- 加载Web应用
- 创建ServletContext
- 创建Listener，完成Spring容器初始化
- 创建Filter
- 创建Servlet，完成SpringMVC容器初始化
<a name="kizGO"></a>
### 初始化流程图
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1695609559805-30000787-8fe8-4824-8d98-e9b2f93fd378.jpeg)
<a name="BV9KK"></a>
## Filter、Servlet等无法自动注入Bean
`Listener`、`Filter`、`Servlet`无法使用`@Autowired`自动装配bean。<br />并不是因为他们初始化的时候bean还没初始化。而是因为`Listener`、`Filter`、`Servlet`是Web容器管理的，而bean是Spring容器或SpringMVC容器管理的。Web容器根部就不认识`@Autowired`注解，当然不可能注入成功。
