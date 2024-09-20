<a name="L05Nn"></a>
## 一、服务器和容器
<a name="wFmlu"></a>
### 1、Web服务器
广义的Web服务器（Web Server）：提供web服务的软件或主机，即Web服务器软件或装有Web服务器软件的计算机。<br />Web服务器可以处理 HTTP 协议，响应针对静态页面或图片的请求（静态请求），进行页面跳转。<br />有的Web服务器还能处理动态请求，它会把动态请求委托其它程序（它的扩展、某种语言的解释引擎(php)、Web容器）。<br />常见的Web服务器有：Apache、IIS、Tomcat、Jetty、JBoss、webLogic等。
<a name="FoJbq"></a>
### 2、Web容器
Web容器是一种服务程序，在服务器一个端口就有一个提供相应服务的程序，这个程序就是用来处理从客户端发出的请求的。一个服务器可以有多个容器。<br />常见的Web容器有：IIS(asp容器)、Tomcat(servlet容器)、Jboss(EJB容器)。<br />其实现在在很多时候，大家对Web服务器和Web容器已经没有了明确的界限。通常把Web服务器和Web容器当成同一个东西在讲。<br />后面部分所讲的web容器都是以Tomcat容器为例。
<a name="FflWK"></a>
#### ① Web容器的组成
Web容器管理Servlet（通过Servlet容器）、监听器(Listener)、过滤器(Filter)等。

- **Servlet**：称为小服务程序或服务连接器，用Java编写的服务器端程序，具有独立于平台和协议的特性，是一个Java类。它负责客户端和服务端的信息交互和处理。Servlet不能独立运行（没有main方法），必须存放在Servlet容器中，由Servlet容器去管理Servlet。
- **Filter**：Servlet的作用是处理请求，而Filter的作用是拦截请求和放行，在拦截请求后可以修改request和response，实现想实现的功能。Filter不能产生一个请求或者响应，只是能对请求或者响应进行拦截。
- **Listener**：监听器。它可以监听Application、Session、Request对象，当这些对象发生变化的时候就会调用对应的监听方法。
<a name="R8hjK"></a>
#### ② Web容器的作用

- 管理和布置Web应用
- 响应针对静态页面或图片的请求（静态请求）
- 将动态请求委托给其他程序（Servlet容器、CGI等等技术）

Servlet、Filter、Listener这些都是在Web容器的掌控范围里。但不在Spring容器和SpringMVC容器的掌控范围里。所以无法在这些类中直接使用Spring注解的方式来自动装配需要的对象，因为Web容器是无法识别Spring注解的。<br />Web容器没有Servlet容器，也是可以直接访问静态页面的，比如Apache服务器。但是如果要显示jsp/servlet，就必需要Servlet容器。可是光有Servlet容器也是不够的，因为它需要被解析成html输出，所以仍需要一个Web容器。<br />大多数的Web容器都包含Servlet容器，如：Tomcat、Jetty、webLogic。
<a name="sfYoA"></a>
### 3、Servlet容器
Servlet容器全称server applet，意为服务程序。Servlet容器负责管理Servlet，用与处理动态请求。<br />Servlet容器是与Servlet交互的Web服务器的一部分，它可以从Web页面接收请求后再将这些请求重定向到Servlet对象中，然后将动态生成的结果返回到正确的位置中。<br />Servlet容器给上级容器(Tomcat)提供doGet()和doPost()等方法。其生命周期实例化、初始化、调用、销毁受控于Tomcat容器。<br />利用Servlet容器提供的方法，能轻松的让Servlet与Web服务器对话，而不用自己建立serversocket、监听某个端口、创建流等等。容器知道自己与Web服务器之间的协议，不用担心Web服务器和你自己的Web代码之间的API，只需要考虑如何在Servlet中实现业务逻辑。<br />大家通常把Tomcat称作Servlet容器，因为Tomcat就是一个围绕着Servlet工作的一个服务器软件。<br />**「ServletContext与Servlet容器的关系」**<br />ServletContext是Servlet与Servlet容器之间的直接通信的接口。<br />Servlet容器在启动一个Web应用时，会为它创建一个ServletContext对象。每个web应用有唯一的ServletContext对象。 同一个Web应用的所有Servlet对象共享一个ServletContext，Servlet对象可以通过它来访问容器中的各种资源。<br />为了方便描述，后文直接将ServletContext称作Servlet容器。
<a name="rBNAY"></a>
#### ① Servlet容器的作用

- 管理Servlet(业务类)
   - 负责加载类、实例化和初始化Servlet
   - 调用Servlet的service方法提供服务
   - 管理Servlet实例的垃圾回收
   - 管理Servlet之间的共同资源(servletContext)
- 处理动态请求
   - 解析包装请求
   - 调用某个Servlet
   - 将Servlet的执行结果返回给请求端
- 多线程
   - 自动为它所接收的每个Servlet请求创建一个新的java线程
   - 但是也会出现线程安全问题
- 实现简便安全
   - 使用xml部署描述文件来配置和修改安全性
- JSP支持
   - 负责将jsp代码翻译为真正的java代码
- 管理通信
   - 负责管理servlet与web服务器之间的通信
<a name="Yqubn"></a>
#### ② Servlet容器的工作过程

- Web服务器接收HTTP请求。
- Web服务器将请求转发到Servlet容器。
- 如果对应的Servlet不在容器中，那么将被动态检索并加载到容器的地址空间中。
- 容器调用init()方法进行初始化（仅在第一次加载 Servlet 时调用一次）。
- 容器调用Servlet的service()方法来处理HTTP请求，即读取请求中的数据并构建响应。Servlet将暂时保留在容器的地址空间中，可以继续处理其它 HTTP 请求。
- Servlet容器将结果返回给Web服务器，Web服务器将动态生成的结果返回到浏览器/客户端。
<a name="tSH1r"></a>
#### ③ Servlet
Servlet是用来处理客户端请求并产生动态网页内容的Java类，Servlet主要是用来处理或者是存储HTML表单提交的数据，产生动态内容，在无状态的HTTP协议下管理状态信息。<br />所有的Servlet都必须要实现的核心的接口是`javax.servlet.Servlet`。每一个Servlet都必须要直接或者是间接实现这个接口，或者是继承`javax.servlet.GenericServlet`或者`javax.servlet.http.HTTPServlet`。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695276041769-be977c5c-9179-431b-a775-6e7872bec646.png#averageHue=%23f9f9f9&clientId=u90c6a55b-1aa6-4&from=paste&height=478&id=u8d575579&originHeight=1196&originWidth=2113&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=96399&status=done&style=none&taskId=ufd992241-2bb2-416e-a696-d4331063bba&title=&width=845.2)
<a name="AZ9dO"></a>
#### ④ Servlet的工作过程

- 客户端发送请求，请求传递到Servlet容器；
- Servlet容器将数据转换成服务端可以处理的数据后发给服务端；
- 当服务端处理完成之后，返回给Servlet容器；
- Servlet容器将其转换成客户端可以处理的数据交给客户端。
<a name="UmFZI"></a>
#### ⑤ Servlet变化过程

- 不同请求的处理逻辑不同，于是将逻辑处理这块单独抽取出来成了Servlet
- 但是Servlet并不擅长往浏览器输出HTML页面，所以出现了JSP
- 随着三层架构的出现，一些逻辑从Servlet抽取出来，分担到Service和Dao
- 随着Spring的出现，Servlet开始退居幕后，取而代之的是方便的SpringMVC。

SpringMVC的核心组件`DispatcherServlet`其实本质就是一个Servlet。它在原来HttpServlet的基础上，又封装了一条逻辑。
<a name="kMsHu"></a>
### 4、Spring容器
Spring容器用来管理Service和Dao。
<a name="dPvVk"></a>
### 5、SpringMVC容器
SpringMVC容器用来管理Controller、视图解析器（view resolvers）等。
<a name="mDqRL"></a>
### 6、SpringBoot容器
SpringBoot容器是一个应用容器，管理所有bean。<br />大家也常叫做Spring容器，这为了和上面的Spring容器区分开来，称他为SpringBoot容器。
<a name="RKjaZ"></a>
## 二、容器相关知识归纳
<a name="sKPG9"></a>
### 1、各种容器的管理范围

- **Web容器**：管理Listener（监听器）、Filter（过滤器）、间接管理Servlet（通过Servlet容器）
- **Servlet容器**：管理Servlet
- **Spring容器**：管理Service、Dao
- **SpringMVC容器**：管理Controller
- **SpringBoot容器**：管理所有Bean
<a name="psY12"></a>
### 2、为何设计父子容器
在早期的Spring+SpringMVC的项目中，会有父子容器这个概念（Spring容器：父容器；SpringMVC容器：子容器）。<br />早期为什么会这样设计呢？

- 早期Spring为了划分框架边界。将Service、Dao层交给Spring来管理（父容器）、Controller层交给SpringMVC管理（子容器）
- Spring规定：子容器可以获取父容器的bean，但父容器不可以获取子容器的bean（父子容器的关系就像子类和父类的关系）

**「可见，父子容器的设计主要是要是为了方便子容器的切换。」**<br />比如：把Web层从SpringMVC替换成Struts， 那么只需要将spring­mvc.xml替换成Struts的配置文件struts.xml即可，而spring­core.xml不需要改变。<br />因此才会有网络上大家常说的：Service、Dao无法注入Controller，Controller却可以注入Service、Dao。<br />但需注意：这概念只在Spring+SpringMVC项目中，SpringBoot项目并没有父子容器这个概念。
<a name="tCTaD"></a>
### 3、父子容器的事务问题
事务管理器属于Spring容器管理，SpringMVC容器无法访问到事务相关的对象，所以不能在Controller上配置事务。
<a name="nRy8s"></a>
## 三、各种项目中的容器
<a name="dJI08"></a>
### 1、SpringMVC项目
<a name="UHoTE"></a>
#### ① 项目背景

- SpringMVC+Tomcat
- 配置`DispatcherServlet`的为dispatcherServlet。

项目用到的容器：Web容器、Servlet容器、SpringMVC容器。
<a name="sozg9"></a>
#### ② Servlet容器
全局上下文`servletContext`（Servlet容器）是一个`ApplicationContextFacade`类型的对象。<br />Servlet容器的属性：
```java
private final Map<String, Class<?>[]> classCache;
private final Map<String, Method> objectCache;
private final ApplicationContext context;
```
servletContext中context属性就是Tomcat的上下文信息。它是一个ApplicationContext类型的对象。<br />context中有一个Map类型的属性attributes，SpringMVC容器就在里面。
<a name="KGfWl"></a>
#### ③ SpringMVC容器
SpringMVC容器是一个`XmlWebApplicationContext`类型的对象。<br />attributes中键为`org.springframework.web.servlet.FrameworkServlet.CONTEXT.dispatcherServlet`的值存的就是SpringMVC容器对象。<br />SpringMVC容器的主要属性：

- **servletContext**：Servlet容器的引用
- **servletConfig**：servletConfig的引用
- **beanFactory**：`DefaultListableBeanFactory`类型，管理SpringMVC容器中的所有bean
- **configLocations**：SpringMVC的配置文件路径，如classpath:springMVC.xml
- **parent**：父容器，值为null
<a name="YuG1I"></a>
#### ④ 容器关系图
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1695275969670-cc417a4f-2f18-4007-b4e6-d17f61bd409e.jpeg)
<a name="Y7ZaC"></a>
### 2、Spring+SpringMVC项目
<a name="ze08z"></a>
#### ① 项目背景

- Spring+SpringMVC+Mybatis+Tomcat
- 配置`DispatcherServlet`的为dispatcherServlet。

项目用到的容器：Web容器、Servlet容器、Spring容器、SpringMVC容器。
<a name="Zclcj"></a>
#### ② Servlet容器
全局上下文servletContext（Servlet容器）是一个`ApplicationContextFacade`类型的对象。<br />Servlet容器的属性：
```java
private final Map<String, Class<?>[]> classCache;
private final Map<String, Method> objectCache;
private final ApplicationContext context;
```
`servletContext`中context属性就是Tomcat的上下文信息。它是一个`ApplicationContext`类型的对象。<br />context中有一个Map类型的属性attributes，Spring容器和SpringMVC容器都在里面。
<a name="sfwio"></a>
#### ③ Spring容器
Spring容器是一个`XmlWebApplicationContext`类型的对象。<br />attributes中键为`org.springframework.web.context.WebApplicationContext.ROOT`的值存的就是Spring容器对象。<br />Spring容器主要属性：

- **servletContext**：Servlet容器的引用
- **beanFactory**：DefaultListableBeanFactory类型，管理Spring容器中的所有bean
- **parent**：父容器，这里Spring容器就是父容器，所以值为null
<a name="TH6KI"></a>
#### ④ SpringMVC容器
SpringMVC容器是一个`XmlWebApplicationContext`类型的对象。<br />attributes中键为`org.springframework.web.servlet.FrameworkServlet.CONTEXT.dispatcherServlet`的值存的就是SpringMVC容器对象。<br />SpringMVC容器主要属性：

- `**servletContext**`：servletContext的引用
- `**servletConfig**`：servletConfig的引用
- `**beanFactory**`：`DefaultListableBeanFactory`类型，管理Spring容器中的所有bean
- `**configLocations**`：SpringMVC的配置文件路径，如classpath:springMVC.xml
- `parent`：父容器，也就是Spring容器
<a name="JGVVK"></a>
#### ⑤ 父子容器
在Spring+SpringMVC项目中，Spring容器和SpringMVC容器为父子容器：

- Spring容器和SpringMVC容器共同管理所有的bean
- Spring容器管理Service和Dao，SpringMVC容器管理Controller
- Spring容器为父容器，SpringMVC容器为子容器
- SpringMVC容器有指向Spring容器的引用，但Spring容器没有指向SpringMVC容器的引用

所以Controller可以注入Service和Dao对象，但是在Service和Dao里不可以注入Controller对象。<br />SpringMVC的拦截器（`Interceptor`）也是SpringMVC容器管理的，所以Interceptor也是可以可以注入Service和Dao对象的。
<a name="VzUAB"></a>
#### ⑥ 容器创建过程
容器的创建过程：

- Tomcat服务器启动，Servlet容器启动
- 创建Spring容器的实例
- 创建SpringMVC容器的实例
<a name="MX2Pn"></a>
#### ⑦ 容器关系图
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1695275838493-7021cf43-94e7-4a6f-8bc2-f57f05445894.jpeg)
<a name="GSsyC"></a>
### 3、SpringBoot项目
<a name="bIEGb"></a>
#### ① 项目背景
SpringBoot+内置Tomcat<br />SpringBoot项目中的容器：Web容器、Servlet容器、SpringBoot容器。<br />对于SpringBoot项目中的容器，大家也常叫做Spring容器，这为了和上面的Spring容器区分开来，称他为SpringBoot容器。
<a name="BWVoi"></a>
#### ② Servlet容器
全局上下文servletContext（Servlet容器）是一个`ApplicationContextFacade`类型的对象。<br />servletContext对象有三个属性：
```java
private final Map<String, Class<?>[]> classCache;
private final Map<String, Method> objectCache;
private final ApplicationContext context;
```
servletContext中context属性就是Tomcat的上下文信息。它是一个`ApplicationContext`类型的对象。<br />context中有一个Map类型的属性attributes，SpringBoot容器就在里面。
<a name="gTary"></a>
#### ③ SpringBoot容器
SpringBoot容器是一个`AnnotationConfigServletWebServerApplicationContext`类型的对象。<br />attributes中键为`org.springframework.web.context.WebApplicationContext.ROOT`的值存的就是SpringBoot容器对象。<br />attributes中键为`org.springframework.web.servlet.FrameworkServlet.CONTEXT.dispatcherServlet`的值存的也是SpringBoot容器对象。<br />SpringBoot容器主要属性：

- **servletContext**：servletContext的引用
- **beanFactory**：`DefaultListableBeanFactory`类型，管理SpringBoot容器中的所有bean
- **parent**：null
<a name="NaS7f"></a>
#### ④ 容器创建过程
<a name="BI6MU"></a>
#### ⑤ 容器关系图
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1695275971829-b46c0779-8c7b-4e25-893f-8aa7c7e4567b.jpeg)
