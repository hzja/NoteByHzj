Java SpringMVC<br />现代开发过程中，开发流程与开发效率的不断提高，同时伴随着 Restful 与 Json 相结合的方式的兴起，使得多个设备跨平台的相互调用与访问变得简单了许多，所以 Spring MVC 简化 Web 开发的使命也自然而然的变为了简化服务端开发。抛开繁杂的代码，从宏观的角度来看一看 Spring MVC 对于处理请求，简化服务端开发的解决方案是如何实现的。
<a name="DabNm"></a>
## 1、曾经的王者——Servlet
在刚接触到使用 Java 进行 Web 开发的时候，Spring MVC 远没有今天这么流行，君不见曾经的王者 Servlet 繁盛一时的场面。现在回想起来，使用 Servlet 进行开发虽然不像现在这么容易，好多的事情需要自己做，但是 Servlet 使得开发的逻辑变得十分清晰，尤其是在 Servlet 与 jsp 很好的承担了各自的角色之后，再加上 mvc 分层思想的流行。编写 Web 应用程序在那时是一件快乐而又简单的事情。<br />实际上 Servlet 做的事情并不是很多，Servlet 想要完成的就是统一请求的接受、处理与响应的流程。<br />网络编程中绕不开的一个东东想必不用说大家也猜得到，那就是 Socket。但是网络需要传输的话是很复杂的，首先需要遵循一定的协议，现在一般使用 Http 与 Https 传输数据，而 Socket 就是在一些网络协议之上，屏蔽了底层协议的细节，为使用者提供一个统一的 api。但是 Servlet 认为 Socket 做的还不够，或者说还要进行相应的处理。于是 Servlet(就 HttpServlet 来说)，他将网络中的请求报文进行封装转化成为了 Request 表示，在 Http 通信过程之中就是 HttpServletRequest，而将服务端处理请求后返回的响应统一的封装为了 HttpServletResponse 对象。<br />这样做的好处是什么呢？<br />作为开发者，不必再去做一些处理网络请求与响应的繁琐之事，而只需要关注于业务逻辑开发。<br />每一次框架效率的提升很多时候都是在将最最重要的业务逻辑与其他任务尽可能完全的分离开，可以全身心的投入到业务逻辑的开发之中，Spring AOP 是不是就是一个很好的佐证呢！<br />那么 Servlet 如何使用呢？

1. 首先通常要编写一个自己的 Servlet 然后继承自 HttpServlet,然后重写其 `doGet()`与 `doPost()`方法。这两个方法都会将 HttpServletRequest 与 HttpServletResponse 作为参数传递进去，然后从 Request 中提取前端传来的参数，在相应的 doXXX 方法内调用事先编写好的 Service 接口，Dao 接口即可将数据准备好放置到 Response 中并跳转到指定的页面即可，跳转的方式可以选择转发或者重定向。
2. Servlet 使用的是模板方法的设计模式，在 Servlet 顶层将会调用 service 方法，该方法会构造 HttpServletRequest 与 HttpServletResponse 对象作为参数调用子类重写的 doXXX()方法。然后返回请求。
3. 最后需要将编写的自定义 Servlet 注册到 web.xml 中，在 web.xml 中配置 servlet-mapping 来为该 servlet 指定处理哪些请求。

Servlet 的使用就是这么简单！事实上，在很长的一段时间内他的流行也得益于他的简单易用易上手。
```xml
<web-app xmlns="http://java.sun.com/xml/ns/j2ee"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd"
    version="2.4">
    <servlet>
            <servlet-name>ShoppingServlet</servlet-name>
            <servlet-class>com.myTest.ShoppingServlet</servlet-class>
    </servlet>

    <servlet-mapping>
            <servlet-name>ShoppingServlet</servlet-name>
            <url-pattern>/shop/ShoppingServlet</url-pattern>
    </servlet-mapping>
</web-app>
```
<a name="akKP4"></a>
## 2、想要更进一步
当使用 Servlet 来进行业务逻辑开发的时候，有那么一点点不适。不适的地方主要有以下几点：

- 每个 Servlet 只能处理一个请求，这样当系统比较大，业务比较复杂的时候可能会存在成百上千的 Servlet，找起来都眼花。
- 每次都需要手动的从 Request 中获取请求参数，然后封装成想要的对象，这其中可能还要对参数进行校验，在调用业务逻辑层获取到数据之后，还要手动的设置到响应中，同时手动的选择转发或者重定向进行跳转。
- 请求的 url 是硬配置到 web.xml 中的，缺乏灵活性，如果可以动态的配置这种请求 url 与处理的对应关系就好了。
- Servlet 与前端的渲染框架紧耦合在一块，这样当前端换一种显示技术的时候就需要改动较大的代码，如果能把数据的处理与数据的显示分离，让其松散耦合就更好了。

带着这些思考，能不能进一步的来抽离业务逻辑的开发呢？<br />在早期的时候也曾进行一些尝试，其大概思路就是编写一个 BaseServlet，然后自己定义的 Servlet 继承自 BaseServlet，前端的请求需要指定 Servlet 的哪个方法进行处理，这样请求的时候将需要带上一个 method 参数，例如这样：<br />http://localhost:8080/myProject/MyServlet?method=getInfo<br />在 BaseServlet 中将提取该参数信息，并使用反射的方法调用子类的该方法，子类方法统一返回 String 类型的结果，代表要返回的逻辑视图名，也就是要跳转的路径，然后父类拿到结果，使用重定向或者转发进行跳转。<br />如果想要在 Servlet 上更进一步，想要进一步的将业务逻辑与其他工作相分离，那么就需要在 Servlet 之上，构建一个事无巨细，任劳任怨，神通过大，的超级 Servlet，来做这些工作，并正式命名为 DispatcherServlet。
<a name="zuhDO"></a>
## 3、Spring MVC——两级控制器方式
接下来就要正式的开始 Spring MVC 之旅了，通过前面的了解，DispatcherServlet为简化开发操碎了心，称之为_前端控制器。现在不禁思考，前面写的 BaseServlet 对应现在的 DispatcherServlet。那么定义业务逻辑的自定义 Servlet 叫啥呢？Spring MVC 管定义业务逻辑处理的类叫做 Handler，只不过他不再是一个 Servlet 了，而是一个普普通通的类，这也很好理解，毕竟 DispatcherServlet 做了太多，完全可以像对待 Servlet 一样对待一个普通的类，而这个 Handler 就叫做次级控制器_。<br />有的书上说了 Spring MVC 的次级控制器叫 Controller，不是 Handler。<br />其实 Spring MVC 的次级控制器确实是叫 Handler，只不过 Hander 是一个抽象的，而 Spring MVC 选择使用 Controller 来实现 Handler，能不能自定义一个 Handler 实现，叫做 Lellortnoc 呢？答案当然是可以的！就好像 List 是一个抽象的接口，而 List 的实现有 ArrayList，LinkedList 一样。
<a name="r7FFd"></a>
## 4、DispatcherServlet——前端控制器
DispatcherServlet 是整个 Spring MVC 的核心，超级牛逼 Servlet 这个荣誉称号他是名副其实。DispatcherServlet 和其家族成员兄弟一起完成了很多的工作，包括请求参数的自动绑定，参数的自动校验，请求 url 的自动匹配，逻辑视图名到真实页面的跳转，数据获取与数据渲染显示的分离等等。。。在此过程中他更像是一个指挥家，有条不紊的指挥着请求不断的向前处理，并最终完成服务端的响应数据。<br />想要了解具体 DispatcherServlet 都是怎么指挥的，那就继续往下看吧！
<a name="w7fdT"></a>
## 5、HandlerMapper——请求映射专家
在使用 Servlet 编写代码的时候，请求的映射工作是交给了 web.xml。但是现在 Spring MVC 采用了两级控制器的方式，就必须解决这个棘手的问题。<br />首先 DispatcherServlet 也是一个 Servlet，那么也应该在 web.xml 中配置其处理的请求路径。那么应该配置什么路径呢？ 希望DispatcherServlet能处理所有的请求，那么就可以让 DispatcherServlet 接受所有请求的处理。像下面这样配置：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="3.0"
    xmlns="http://java.sun.com/xml/ns/javaee"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee
    http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
 <servlet>
          <servlet-name>Spring MVC</servlet-name>
          <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
          <!-- 表示启动容器时初始化该servlet -->
          <init-param>
              <param-name>contextConfigLocation</param-name>
              <param-value>classpath:Spring-servlet.xml</param-value>
          </init-param>
          <load-on-startup>1</load-on-startup>
     </servlet>
     <servlet-mapping>
          <servlet-name>Spring MVC</servlet-name>
          <url-pattern>/*</url-pattern>
     </servlet-mapping>

</web-app>
```
现在所有的请求都被映射到了 DispatcherServlet，那么 DispatcherServlet 现在就有责任将请求分发至具体的次级控制器，如何找到或者说如何保存请求到具体的次级控制器的这种映射关系呢？DispatcherServlet 选择请求他的好兄弟 HandlerMapping。<br />在 HandlerMapping 中，保存了特定的请求 url 应该被哪一个 Handler(也就是通常的 Controller)所处理。HandlerMapping 根据映射策略的不同，大概有下面几种映射查找方式：

1. org.springframework.web.servlet.handler.SimpleUrlHandlerMapping 通过配置请求路径和 Controller 映射建立关系，找到相应的 Controller
2. org.springframework.web.servlet.mvc.support.ControllerClassNameHandlerMapping 通过 Controller 的类名找到请求的 Controller。
3. org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping 通过定义的 beanName 进行查找要请求的 Controller
4. org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping 通过注解 `@RequestMapping("/userlist")` 来查找对应的 Controller。

想必现在最常用的就是第四种了吧，直接在对应的 Controller 上以及其内部的方法之上加上相应的注解，就可以配置好请求的映射。
<a name="GTCli"></a>
## 6、Handler 的拦路虎——HandlerInterceptor
你以为 DispatcherServlet 把请求的 url 交给 HandlerMapping, HandlerMapping 根据请求查出对应的 Controller 来交给 DispatcherServlet, 然后 DispatcherServlet 交给 Controller 执行就完事了？那就 To young to native 了，这其中还有一些小插曲。比如不能什么请求都交给 Handler 执行，最起码要过滤一下不合理的请求，比如跳转页面的时候检查 Session，如果用户没登录跳转到登录界面啊，以及一些程序的异常以统一的方式跳转等等，都需要对请求进行拦截。<br />如果对 Servlet 了解的同学是不是有一点似曾相识的感觉？没错，Servlet 中的 Filter 也可以完成请求拦截与过滤的功能，不过既然 Spring MVC 是两级控制器结构，那么 HandlerInterceptor 就与 Filter 有一些细微的差别，其最主要的差别， HandlerInterceptor 提供了更细粒度的拦截。毕竟 Filter 拦截的对象是 Serlvet，而 HandlerInterceptor 拦截的则是 Handler(Controller)。用一张图可以生动的表现出来。<br />![2021-05-13-22-11-52-124647.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620915191337-1ef6e524-b027-4f67-89dd-4df7e809e225.png#clientId=u438144f8-2e60-4&from=ui&id=u85a6fef6&originHeight=252&originWidth=1080&originalType=binary&size=818124&status=done&style=shadow&taskId=u6a726766-859e-4e36-8351-f40467f4f0c)<br />从图中可以看出 HandlerInteceptor 可以配置多个，其中任何一个返回 false 的话，请求都将被拦截，直接返回。
<a name="vRZrH"></a>
## 7、次级控制器——Handler
前端控制器已经很熟悉了，而次级控制器也就是 Handler，是真正执行业务逻辑的类。通常在 Spring MVC 中，这个 Handler 就是很熟悉的 Controller。调用封装好的业务逻辑接口就是在这里进行处理的。可以说 Spring MVC 已经将业务逻辑与其他不相关的繁杂工作分离的较为彻底了。这样，就在 Handler(Controller)中专心的编写业务逻辑！
<a name="JEHz3"></a>
## 8、Handler 与 HandlerInterceptor 的桥梁——HandlerExecutionChain
前面讲到 DispatherServlet 求助 HandlerMapping 进行 url 与次级控制器的映射，但是 DispatherServlet 在将 url 交给特定的 HandlerMapping 之后，HandlerMapping 在进行了一顿猛如虎的操作之后，返回给 DispaterServlet 的却不是一个可执行的 Handler(Controller),而是一个 HandlerExecutionChain 对象。那么 HandlerMapping 究竟为什么要返回给这样的一个对象而不是返回 Handler 对象呢？<br />其实在看上面图的时候，HandlerInterceptor 与 Handler 是怎样联系在一起的呢？答案就是 HandlerExecutionChain。它就是若干的 HandlerInterceptor 与 Handler 的组合。那么是怎么组合的呢？<br />这里就涉及到设计模式中的责任链设计模式，HandlerExecutionChain 将 HandlerInterceptor 与 Handler 串成一个执行链的形式，首先请求会被第一个 HandlerInterceptor 拦截，如果返回 false,那么直接短路请求，如果返回 true,那么再交给第二个 HandlerInterceptor 处理，直到所有的 HandlerInterceptor 都检查通过，请求才到达 Handler(Controller)，交由 Handler 正式的处理请求。执行完成之后再逐层的返回。<br />而 DispatcherServlet 拿到的就是这样一个串联好的 HandlerExecutionChain，然后顺序的执行请求。
<a name="uenSN"></a>
## 9、解耦的关键——ModelAndView
到这里，请求终于来到了对应的 Handler。希望的是 Handler 只处理负责的业务逻辑即可，而一些 url 的跳转等无需 Handler 负责。那么 DispatcherServlet 就使用了 ModelAndView 保存数据和想要跳转的路径。<br />调用业务逻辑层获取数据，并将数据封装到 ModelAndView 中，同时设置 ModelAndView 的 view 逻辑视图名称。从 ModelAndView 的名称可以看出，它保存了 Handler 执行完成之后所需要发送到前端的数据，以及需要跳转的路径。这些是 DispatcherServlet 需要用到的。
<a name="D1cxb"></a>
## 10、视图渲染查找——ViewResolver
这一步是 Spring MVC 将数据的获取与数据的显示渲染相分离的关键，前端可能采用各种各样的方式显示数据，可能是 Jsp,可能是 Html,也可能是其他的方式。DispatcherServlet 已经拿到了 ModelAndView，这里面有执行完成请求后返回的响应结果数据，还有逻辑视图的路径，这个时候 DispatcherServlet 就需要根据这个逻辑视图的路径去查找谁能把数据进行解析与渲染。<br />比如说使用 FreeMarker 模板引擎渲染数据，那么这个时候就要找到能够胜任该工作的那个 View 实现类，那么问题来了，如何寻找呢？以什么策略寻找呢？这个就依赖 ViewResolver 了。<br />通常的寻找策略有以下几种：

- `BeanNameViewResolver` :将逻辑视图名解析为一个 Bean,Bean 的 id 等于逻辑视图名。
- `XmlViewResolver`:和 `BeanNameViewResolver` 类似，只不过目标视图 Bean 对象定义在一个独立的 XML 文件中，而非定义在 `DispatcherServlet` 上下文的主配置文件中
- `InternalResourceViewResovlver`:将视图名解析为一个 URL 文件，一般使用该解析器将视图名映射为保存在 WEB-INF 目录中的程序文件（如 JSP）
- `XsltViewResolver`:将视图名解析为一个指定 XSLT 样式表的 URL 文件
- `JasperReportsViewResolver`:`JasperReports` 是一个基于 java 的开源报表工具，该解析器将视图名解析为报表文件对应的 URL
- `FreeMarkerViewResolver`:解析为基于 FreeMarker 模板技术的模板文件
- `VelocityViewResolver` 和 `VelocityLayoutViewResolver`:解析为基于 Velocity 模板技术的模板文件
<a name="n4yAT"></a>
## 11、数据渲染——View
在根据逻辑视图名借助 ViewResolver 查找到对应的 View 实现类之后，`DispatcherServlet` 就会将 ModelAndView 中的数据交给 View 实现类来进行渲染，待该 View 渲染完成之后，会将渲染完成的数据交给 `DispatcherServlet`，这时候 DispatcherServlet 将其封装到 Response 返回给前端显示。<br />至此，整个 Spring MVC 的处理流程就算完成了，当然这其中还会有对于国际化的支持，主题的定义与设置等等，但是这些是不常用的，Spring MVC 最主要的处理流程所需要的用到的就是以上这些类。可以看到在此过程中，DispatcherServlet 起到了至关重要的作用，所以说 Spring MVC 的核心就在于 DispatcherServlet。<br />最后附上一张流程图作为以上内容的总结。<br />![2021-05-13-22-11-52-236345.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620915180021-3c92660c-0322-4f13-aee4-768553149ea5.png#clientId=u438144f8-2e60-4&from=ui&id=u5eaed4cb&originHeight=504&originWidth=1080&originalType=binary&size=1636181&status=done&style=shadow&taskId=ud01ae221-b878-44d5-8de2-77dc72a052c)
