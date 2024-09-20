JavaTomcatWeblogic
<a name="eRFPz"></a>
## 直接访问Servlet的配置
url 访问方式：http://host/servlet/packageName.servletName，不推荐使用
<a name="gqNXx"></a>
### Weblogic
```xml
<servlet>
  <servlet-name>WeblogicServlet</servlet-name>
  <servlet-class>weblogic.servlet.ServletServlet</servlet-class>
  <load-on-startup>0</load-on-startup>
</servlet>
<servlet-mapping>
  <servlet-name>WeblogicServlet</servlet-name>
  <url-pattern>/servlet/*</url-pattern>
</servlet-mapping>
```
<a name="P2tqK"></a>
### Tomcat6
Tomcat6有一个类org.apache.catalina.servlets.InvokerServlet，它负责处理没有做ServletMapping的那些Servlet。但是在tomcat安装目录的conf/web.xml中，系统默认情况下把org.apache.catalina.servlets.InvokerServlet的注册与地址映射注释了。<br />解决办法是修改服务器的两个文件：
<a name="JyP8B"></a>
#### 修改Tomcat配置文件conf/web.xml文件
<a name="UoIRW"></a>
##### 配置listings值为true
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681195700866-f3b125d4-ee94-4b93-af8b-0b57034f9e06.png#averageHue=%23fcfcfc&clientId=u7927ff9a-7197-4&from=paste&height=348&id=u483f53d3&originHeight=870&originWidth=2735&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=92183&status=done&style=shadow&taskId=u876b90e5-a3e2-4730-abdb-f8cdad9d0ec&title=&width=1094)
```xml
<servlet>
    <servlet-name>default</servlet-name>
    <servlet-class>org.apache.catalina.servlets.DefaultServlet</servlet-class>
    <init-param>
        <param-name>debug</param-name>
        <param-value>0</param-value>
    </init-param>
    <init-param>
        <param-name>listings</param-name>
        <param-value>true</param-value>
    </init-param>
    <load-on-startup>1</load-on-startup>
</servlet>
```
<a name="AUTv9"></a>
##### 去掉InvokerServlet配置的注释符使其生效
```xml
<!--
    <servlet>
        <servlet-name>invoker</servlet-name>
        <servlet-class>
          org.apache.catalina.servlets.InvokerServlet
        </servlet-class>
        <init-param>
            <param-name>debug</param-name>
            <param-value>0</param-value>
        </init-param>
        <load-on-startup>2</load-on-startup>
    </servlet>
-->

<!--
    <servlet-mapping>
        <servlet-name>invoker</servlet-name>
        <url-pattern>/servlet/*</url-pattern>
    </servlet-mapping>
-->
```
在启动Tomcat时报错：
> org.apache.catalina.servlets.InvokerServlet is privileged and cannot be loaded by this web application.

可以按照以下解决办法处理：
<a name="h43nB"></a>
#### 修改Tomcat配置文件conf/context.xml文件
新加两个属性，**开启Servelt调用器**即可。
```xml
<Context reloadable="true" privileged="true">
```
在调用时必须是完整的包名和类名
<a name="aoaHg"></a>
### Tomcat7之后
:::danger
结论：`InvokerServlet` 在 tomcat 7 + 已经彻底移除，不安全，使用标准的方式、`@WebServlet("/path")`等代替。
:::
