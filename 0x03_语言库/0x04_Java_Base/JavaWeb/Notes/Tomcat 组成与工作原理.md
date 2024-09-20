Java JavaWeb Tomcat
> 开源的 Java Web 应用服务器，实现了 Java EE(Java Platform Enterprise Edition)的部 分技术规范，比如 Java Servlet、Java Server Page、JSTL、Java WebSocket。Java EE 是 Sun 公 司为企业级应用推出的标准平台，定义了一系列用于企业级开发的技术规范，除了上述的之外，还有 EJB、Java Mail、JPA、JTA、JMS 等，而这些都依赖具体容器的实现。

| 服务器 | 开源 | Servlet | JSP | Java EE other |
| --- | --- | --- | --- | --- |
| Tomcat | 是 | √ | √ | N/A |
| Jetty | 是 | √ | √ | N/A |
| Jboss/WildFly | 是 | √ | √ | JavaEE 7 |
| Glassfish | 是 | √ | √ | JavaEE 7 |
| Websphere | 否 | √ | √ | JavaEE 7 |
| WebLogic | 否 | √ | √ | JavaEE 7 |

上图对比了 Java EE 容器的实现情况，Tomcat 和 Jetty 都只提供了 Java Web 容器必需的 Servlet 和 JSP 规范，开发者要想实现其他的功能，需要自己依赖其他开源实现。<br />Glassfish 是由 sun 公司推出，Java EE 最新规范出来之后，首先会在 Glassfish 上进行实 现，所以是研究 Java EE 最新技术的首选。<br />最常见的情况是使用 Tomcat 作为 Java Web 服务器，使用 Spring 提供的开箱即用的强大 的功能，并依赖其他开源库来完成负责的业务功能实现。
<a name="pkohQ"></a>
## Servlet 容器
Tomcat 组成如下图：主要有 Container 和 Connector 以及相关组件构成。<br />![2021-08-05-07-48-01-553047.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121514770-0e8339ed-907c-4e66-b272-203ada08a329.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u15097fce&originHeight=550&originWidth=640&originalType=binary&ratio=1&size=27637&status=done&style=none&taskId=u11eed151-6ad5-4079-a795-80b1d899378)<br />Server：指的就是整个 Tomcat 服 务器，包含多组服务，负责管理和 启动各个 Service，同时监听 8005 端口发过来的 shutdown 命令，用 于关闭整个容器 。<br />Service：Tomcat 封装的、对外提 供完整的、基于组件的 web 服务， 包含 Connectors、Container 两个 核心组件，以及多个功能组件，各 个 Service 之间是独立的，但是共享 同一 JVM 的资源 。<br />Connector：Tomcat 与外部世界的连接器，监听固定端口接收外部请求，传递给 Container，并 将 Container 处理的结果返回给外部。<br />Container：Catalina，Servlet 容器，内部有多层容器组成，用于管理 Servlet 生命周期，调用 servlet 相关方法。<br />Loader：封装了 Java ClassLoader，用于 Container 加载类文件；Realm：Tomcat 中为 web 应用程序提供访问认证和角色管理的机制。<br />JMX：Java SE 中定义技术规范，是一个为应用程序、设备、系统等植入管理功能的框架，通过 JMX 可以远程监控 Tomcat 的运行状态。<br />Jasper：Tomcat 的 Jsp 解析引擎，用于将 Jsp 转换成 Java 文件，并编译成 class 文件。Session：负责管理和创建 session，以及 Session 的持久化(可自定义)，支持 session 的集 群。<br />Pipeline：在容器中充当管道的作用，管道中可以设置各种 valve(阀门)，请求和响应在经由管 道中各个阀门处理，提供了一种灵活可配置的处理请求和响应的机制。<br />Naming：命名服务，JNDI， Java 命名和目录接口，是一组在 Java 应用中访问命名和目录服务的 API。命名服务将名称和对象联系起来，可以用名称访问对象，目录服务也是一种命名 服务，对象不但有名称，还有属性。Tomcat 中可以使用 JNDI 定义数据源、配置信息，用于开发 与部署的分离。
<a name="w6JXT"></a>
##  Container 组成
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628120958804-795959bc-a758-49de-9844-78ba8d66d559.webp#clientId=u665eb4b4-983f-4&from=paste&id=u476c3c27&originHeight=510&originWidth=640&originalType=url&ratio=1&status=done&style=none&taskId=u9d909331-e1f6-4e1f-a2ff-1574f6528a2)<br />Engine：Servlet 的顶层容器，包含一 个或多个 Host 子容器；Host：虚拟主机，负责 web 应用的部 署和 Context 的创建；Context：Web 应用上下文，包含多个 Wrapper，负责 web 配置的解析、管 理所有的 Web 资源；Wrapper：最底层的容器，是对 Servlet 的封装，负责 Servlet 实例的创 建、执行和销毁。<br />生命周期管理Tomcat 为了方便管理组件和容器的生命周期，定义了从创建、启动、到停止、销毁共 12 中状态，tomcat 生命周期管理了内部状态变化的规则控制，组件和容器只需实现相应的生命周期 方法即可完成各生命周期内的操作(initInternal、startInternal、stopInternal、 destroyInternal)；<br />比如执行初始化操作时，会判断当前状态是否 New，如果不是则抛出生命周期异常；是的 话则设置当前状态为 Initializing，并执行 initInternal 方法，由子类实现，方法执行成功则设置当 前状态为 Initialized，执行失败则设置为 Failed 状态；<br />![2021-08-05-07-48-01-813012.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121529389-938b60e5-7101-41d7-970c-bef1660aefc1.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=ueb216cd8&originHeight=302&originWidth=640&originalType=binary&ratio=1&size=18083&status=done&style=shadow&taskId=u006c4d3d-3667-4c02-a544-efe8c847c20)<br />Tomcat 的生命周期管理引入了事件机制，在组件或容器的生命周期状态发生变化时会通 知事件监听器，监听器通过判断事件的类型来进行相应的操作。事件监听器的添加可以在 server.xml 文件中进行配置。<br />Tomcat 各类容器的配置过程就是通过添加 listener 的方式来进行的，从而达到配置逻辑与 容器的解耦。如 EngineConfig、HostConfig、ContextConfig。EngineConfig：主要打印启动和停止日志 HostConfig：主要处理部署应用，解析应用 META-INF/context.xml 并创建应用的 Context ContextConfig：主要解析并合并 web.xml，扫描应用的各类 web 资源 (filter、servlet、listener)。<br />![2021-08-05-07-48-01-903013.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121539831-2fde25e7-a9c7-419a-8cf2-d924033d1eee.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=uf6ceada2&originHeight=418&originWidth=640&originalType=binary&ratio=1&size=25519&status=done&style=shadow&taskId=ud57453e3-a9b8-4b9c-8acd-f93f851b3c9)
<a name="J025g"></a>
## Tomcat 的启动过程
![2021-08-05-07-48-02-100013.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121549437-cae03a50-91d2-44e5-ac34-e5ef9474daef.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u23b04316&originHeight=492&originWidth=640&originalType=binary&ratio=1&size=29617&status=done&style=shadow&taskId=u6eb32ef0-d36c-4089-9ca9-dab409ab31d)<br />启动从 Tomcat 提供的 start.sh 脚本开始，shell 脚本会调用 Bootstrap 的 main 方法，实际 调用了 Catalina 相应的 load、start 方法。<br />load 方法会通过 Digester 进行 config/server.xml 的解析，在解析的过程中会根据 xml 中的关系 和配置信息来创建容器，并设置相关的属性。接着 Catalina 会调用 StandardServer 的 init 和 start 方法进行容器的初始化和启动。<br />按照 xml 的配置关系，server 的子元素是 service，service 的子元素是顶层容器 Engine，每层容器有持有自己的子容器，而这些元素都实现了生命周期管理 的各个方法，因此就很容易的完成整个容器的启动、关闭等生命周期的管理。<br />StandardServer 完成 init 和 start 方法调用后，会一直监听来自 8005 端口(可配置)，如果接收 到 shutdown 命令，则会退出循环监听，执行后续的 stop 和 destroy 方法，完成 Tomcat 容器的 关闭。同时也会调用 JVM 的 `Runtime.getRuntime()﴿.addShutdownHook` 方法，在虚拟机意外退 出的时候来关闭容器。<br />所有容器都是继承自 ContainerBase，基类中封装了容器中的重复工作，负责启动容器相关的组 件 Loader、Logger、Manager、Cluster、Pipeline，启动子容器(线程池并发启动子容器，通过 线程池 submit 多个线程，调用后返回 Future 对象，线程内部启动子容器，接着调用 Future 对象 的 get 方法来等待执行结果)。
```java
List<Future<Void>> results = new ArrayList<Future<Void>>();
for (int i = 0; i < children.length; i++) {
    results.add(startStopExecutor.submit(new StartChild(children[i])));
}
boolean fail = false;
for (Future<Void> result ：results) {
    try {
        result.get();
    } catch (Exception e) {
        log.error(sm.getString("containerBase.threadedStartFailed")， e);
        fail = true;
    }
}
```
Web 应用的部署方式注：catalina.home：安装目录;catalina.base：工作目录;默认值 user.dir：

- Server.xml 配置 Host 元素，指定 appBase 属性，默认$catalina.base/webapps/
- Server.xml 配置 Context 元素，指定 docBase，元素，指定 web 应用的路径
- 自定义配置：在$catalina.base/EngineName/HostName/XXX.xml 配置 Context 元素

HostConfig 监听了 StandardHost 容器的事件，在 start 方法中解析上述配置文件：

- 扫描 appbase 路径下的所有文件夹和 war 包，解析各个应用的 META-INF/context.xml，并 创建 StandardContext，并将 Context 加入到 Host 的子容器中。
- 解析$catalina.base/EngineName/HostName/下的所有 Context 配置，找到相应 web 应 用的位置，解析各个应用的 META-INF/context.xml，并创建 StandardContext，并将 Context 加入到 Host 的子容器中。

注：

- HostConfig 并没有实际解析 Context.xml，而是在 ContextConfig 中进行的。
- HostConfig 中会定期检查 watched 资源文件(context.xml 配置文件)

ContextConfig 解析 context.xml 顺序：

- 先解析全局的配置 config/context.xml
- 然后解析 Host 的默认配置 EngineName/HostName/context.xml.default
- 最后解析应用的 META-INF/context.xml

ContextConfig 解析 web.xml 顺序：

- 先解析全局的配置 config/web.xml
- 然后解析 Host 的默认配置 EngineName/HostName/web.xml.default 接着解析应用的 MEB-INF/web.xml
- 扫描应用 WEB-INF/lib/下的 jar 文件，解析其中的 META-INF/web-fragment.xml 最后合并 xml 封装成 WebXml，并设置 Context

注：

- 扫描 web 应用和 jar 中的注解(Filter、Listener、Servlet)就是上述步骤中进行的。
- 容器的定期执行：`backgroundProcess`，由 `ContainerBase` 来实现的，并且只有在顶层容器 中才会开启线程。(`backgroundProcessorDelay=10` 标志位来控制)
<a name="RD73N"></a>
## Servlet 生命周期
![2021-08-05-07-48-02-285013.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121561108-43387331-b006-4665-b86d-3e7393fe3131.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u26ffc2aa&originHeight=524&originWidth=640&originalType=binary&ratio=1&size=26417&status=done&style=shadow&taskId=u945ccc79-4eca-4a1a-9669-6b5a789e844)<br />Servlet 是用 Java 编写的服务器端程序。其主要功能在于交互式地浏览和修改数据，生成动态 Web 内容。

1. 请求到达 server 端，server 根据 url 映射到相应的 Servlet
2. 判断 Servlet 实例是否存在，不存在则加载和实例化 Servlet 并调用 init 方法
3. Server 分别创建 Request 和 Response 对象，调用 Servlet 实例的 service 方法(service 方法 内部会根据 http 请求方法类型调用相应的 doXXX 方法)
4. doXXX 方法内为业务逻辑实现，从 Request 对象获取请求参数，处理完毕之后将结果通过 response 对象返回给调用方
5. 当 Server 不再需要 Servlet 时(一般当 Server 关闭时)，Server 调用 Servlet 的 `destroy()` 方 法。
<a name="IV1IQ"></a>
## load on startup
当值为 0 或者大于 0 时，表示容器在应用启动时就加载这个 servlet; 当是一个负数时或者没有指定时，则指示容器在该 servlet 被选择时才加载; 正数的值越小，启动该 servlet 的优先级越高。
<a name="f7F7O"></a>
### single thread model
每次访问 servlet，新建 servlet 实体对象，但并不能保证线程安全，同时 tomcat 会限制 servlet 的实例数目 最佳实践：不要使用该模型，servlet 中不要有全局变量。
<a name="Hl58q"></a>
## 请求处理过程
![2021-08-05-07-48-02-594013.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121571771-08871bdf-7829-4f38-b229-6b35795d259b.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u4c9cc80b&originHeight=665&originWidth=617&originalType=binary&ratio=1&size=48428&status=done&style=shadow&taskId=ue144c5a1-f6ab-4b7b-907c-86e0235345a)

1. 根据 server.xml 配置的指定的 connector 以及端口监听 http、或者 ajp 请求
2. 请求到来时建立连接,解析请求参数,创建 Request 和 Response 对象,调用顶层容器 pipeline 的 invoke 方法
3. 容器之间层层调用,最终调用业务 servlet 的 service 方法
4. Connector 将 response 流中的数据写到 socket 中
<a name="Z0Fd4"></a>
## Pipeline 与 Valve
![2021-08-05-07-48-02-826028.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121584189-67b49914-0a27-4d4c-b8b5-ce9f1d5e3777.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u8382dd82&originHeight=268&originWidth=640&originalType=binary&ratio=1&size=17308&status=done&style=shadow&taskId=u0daed5bc-19d2-4e19-8959-25b94035e81)<br />Pipeline 可以理解为现实中的管道,Valve 为管道中的阀门,Request 和 Response 对象在管道中 经过各个阀门的处理和控制。<br />每个容器的管道中都有一个必不可少的 basic valve,其他的都是可选的,basic valve 在管道中最 后调用,同时负责调用子容器的第一个 valve。<br />Valve 中主要的三个方法:setNext、getNext、invoke;valve 之间的关系是单向链式结构,本身 invoke 方法中会调用下一个 valve 的 invoke 方法。<br />各层容器对应的 basic valve 分别是 StandardEngineValve、StandardHostValve、 StandardContextValve、StandardWrapperValve。
<a name="MZ7s5"></a>
## JSP 引擎
![2021-08-05-07-48-03-042073.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121598166-331b3ef7-d05c-4d3d-9e81-1e9830c7fbf4.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=ub8142763&originHeight=427&originWidth=505&originalType=binary&ratio=1&size=20966&status=done&style=shadow&taskId=ud33e500e-51df-4b95-b407-4a2c038034d)
<a name="Wxvct"></a>
### JSP 生命周期

- 编译阶段:servlet 容器编译 servlet 源文 件,生成 servlet 类
- 初始化阶段:加载与 JSP 对应的 servlet 类, 创建其实例,并调用它的初始化方法
- 执行阶段:调用与 JSP 对应的 servlet 实例的 服务方法
- 销毁阶段:调用与 JSP 对应的 servlet 实例的 销毁方法,然后销毁 servlet 实例

JSP元素代码片段：<% 代码片段 %> JSP声明：<%! declaration; [ declaration; ]+ ... %> JSP表达式：<%= 表达式 %> JSP注释：<%-- 注释 --%> JSP指令：   <%@ directive attribute=“value” %> JSP行为：   <jsp:action_name attribute=“value” /> HTML元素：html/head/body/div/p/… JSP隐式对象：request、response、out、session、application、config、 pageContext、page、Exception<br />**JSP 元素说明 ** 代码片段:包含任意量的 Java 语句、变量、方法或表达式; JSP 声明:一个声明语句可以声明一个或多个变量、方法,供后面的 Java 代码使用; JSP 表达式:输出 Java 表达式的值,String 形式; JSP 注释:为代码作注释以及将某段代码注释掉 JSP 指令:用来设置与整个 JSP 页面相关的属性, <%@ page ... %>定义页面的依赖属性,比如 language、contentType、errorPage、 isErrorPage、import、isThreadSafe、session 等等 <%@ include ... %>包含其他的 JSP 文件、HTML 文件或文本文件,是该 JSP 文件的一部分,会 被同时编译执行 <%@ taglib ... %>引入标签库的定义,可以是自定义标签 JSP 行为:`jsp:include`、`jsp:useBean`、`jsp:setProperty`、`jsp:getProperty`、`jsp:forward`
<a name="Yx2sd"></a>
### Jsp 解析过程
![2021-08-05-07-48-03-302015.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121611282-5e0567de-28a3-4587-91b1-3016a73b22e2.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u562f7059&originHeight=304&originWidth=640&originalType=binary&ratio=1&size=14490&status=done&style=shadow&taskId=ub4f52878-b4c7-4f18-a07b-65e39e521fd)

- 代码片段:在`_jspService()`方法内直接输出
- JSP 声明: 在 servlet 类中进行输出
- JSP 表达式:在`_jspService()`方法内直接输出
- JSP 注释:直接忽略,不输出
- JSP 指令:根据不同指令进行区分,include:对引入的文件进行解析;page 相关的属性会做为 JSP 的属性,影响的是解析和请求处理时的行为
- JSP 行为:不同的行为有不同的处理方式,jsp:useBean 为例,会从 pageContext 根据 scope 的 类别获取 bean 对象,如果没有会创建 bean,同时存到相应 scope 的 pageContext 中
- HTML:在`_jspService()`方法内直接输出
- JSP 隐式对象:在`_jspService()`方法会进行声明,只能在方法中使用;
<a name="gFhCc"></a>
## Connector
![2021-08-05-07-48-03-642024.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121635652-00ed2b46-8451-4261-8d63-6aca835c7681.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u8de2e9ca&originHeight=255&originWidth=640&originalType=binary&ratio=1&size=13475&status=done&style=shadow&taskId=u98a91477-677c-4d9c-85cd-6323b516011)<br />Http:HTTP 是超文本传输协议,是客户端浏览器或其他程序与 Web 服务器之间的应用层通信协 议 AJP:Apache JServ 协议(AJP)是一种二进制协议,专门代理从 Web 服务器到位于后端的应用 程序服务器的入站请求阻塞 IO。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628120961698-68753f1f-779b-4173-b20e-23ce083f0f4d.webp#clientId=u665eb4b4-983f-4&from=paste&id=ufaae73b0&originHeight=342&originWidth=640&originalType=url&ratio=1&status=done&style=shadow&taskId=u464f2c9f-297e-4f4a-8db9-6a5c3bd3b01)
<a name="kp8XV"></a>
### 非阻塞 IO
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1628120961706-309b9c57-fc65-4ff4-8a9b-ea01f7212b82.webp#clientId=u665eb4b4-983f-4&from=paste&id=ud1dc2edc&originHeight=370&originWidth=640&originalType=url&ratio=1&status=done&style=shadow&taskId=u20dbddf7-9a9f-44ce-b7d7-16186e8d5e2)
<a name="jf3D1"></a>
### IO多路复用
![2021-08-05-07-48-03-912015.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121647972-ee45be0b-2d04-491b-98ba-7d884a1567a2.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u83999ad1&originHeight=357&originWidth=640&originalType=binary&ratio=1&size=16065&status=done&style=shadow&taskId=u23407f53-6a2c-482c-8e66-74e4fcd96b3)<br />阻塞与非阻塞的区别在于进行读操作和写操作的系统调用时，如果此时内核态没有数据可读或者没有缓冲空间可写时，是否阻塞。<br />IO多路复用的好处在于可同时监听多个socket的可读和可写事件，这样就能使得应用可以同时监听多个socket，释放了应用线程资源。
<a name="aQnnv"></a>
### Tomcat各类Connector对比
![2021-08-05-07-48-04-020607.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121658900-7f35b3ec-2c8f-4796-a184-c9e962fb4efb.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=udcdcd787&originHeight=307&originWidth=640&originalType=binary&ratio=1&size=26719&status=done&style=shadow&taskId=u927a2f29-ddfc-4466-8800-2e1b0ad16e2)<br />Connector的实现模式有三种，分别是BIO、NIO、APR，可以在server.xml中指定。

- JIO：用java.io编写的TCP模块，阻塞IO
- NIO：用java.nio编写的TCP模块，非阻塞IO，（IO多路复用）
- APR：全称Apache Portable Runtime，使用JNI的方式来进行读取文件以及进行网络传输

Apache Portable Runtime是一个高度可移植的库，它是Apache HTTP Server 2.x的核心。APR具有许多用途，包括访问高级IO功能（如sendfile，epoll和OpenSSL），操作系统级功能（随机数生成，系统状态等）和本地进程处理（共享内存，NT管道和Unix套接字）。<br />表格中字段含义说明：

- Support Polling：是否支持基于IO多路复用的socket事件轮询
- Polling Size：轮询的最大连接数
- Wait for next Request：在等待下一个请求时，处理线程是否释放，BIO是没有释放的，所以在keep-alive=true的情况下处理的并发连接数有限
- Read Request Headers：由于request header数据较少，可以由容器提前解析完毕，不需要阻塞
- Read Request Body：读取request body的数据是应用业务逻辑的事情，同时Servlet的限制，是需要阻塞读取的
- Write Response：跟读取request body的逻辑类似，同样需要阻塞写

NIO处理相关类<br />![2021-08-05-07-48-04-102135.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121671505-962668c3-4dfc-4e18-a057-ef3afaf628db.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=uce9558d2&originHeight=520&originWidth=640&originalType=binary&ratio=1&size=27718&status=done&style=shadow&taskId=u799b84f3-92b9-470d-ac1d-46bda9ed5c2)<br />Acceptor线程负责接收连接，调用accept方法阻塞接收建立的连接，并对socket进行封装成PollerEvent，指定注册的事件为op_read，并放入到EventQueue队列中，PollerEvent的run方法逻辑的是将Selector注册到socket的指定事件。<br />Poller线程从EventQueue获取PollerEvent，并执行PollerEvent的run方法，调用Selector的select方法，如果有可读的Socket则创建Http11NioProcessor，放入到线程池中执行。<br />CoyoteAdapter是Connector到Container的适配器，Http11NioProcessor调用其提供的service方法，内部创建Request和Response对象，并调用最顶层容器的Pipeline中的第一个Valve的invoke方法。<br />Mapper主要处理http url 到servlet的映射规则的解析，对外提供map方法。
<a name="fe53j"></a>
## NIO Connector 主要参数
![2021-08-05-07-48-04-254166.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121684232-b6ce0e5d-52df-4bc6-8f49-2740c08f25ad.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u233152e9&originHeight=337&originWidth=640&originalType=binary&ratio=1&size=38785&status=done&style=shadow&taskId=ufb3d4f59-0832-4415-b6ae-75ed0c7bce8)
<a name="Ik7Kw"></a>
## Comet
Comet是一种用于web的推送技术，能使服务器实时地将更新的信息传送到客户端，而无须客户端发出请求 在WebSocket出来之前，如果不使用comet，只能通过浏览器端轮询Server来模拟实现服务器端推送。Comet支持servlet异步处理IO，当连接上数据可读时触发事件，并异步写数据(阻塞)。<br />![2021-08-05-07-48-04-330166.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121695107-5326b967-ea2e-4958-ac0b-3964f15545b7.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u433d6ba1&originHeight=203&originWidth=640&originalType=binary&ratio=1&size=11415&status=done&style=shadow&taskId=uda569d09-69d4-42d9-afa1-892779130ed)<br />Tomcat要实现Comet，只需继承HttpServlet同时，实现CometProcessor接口：

- Begin：新的请求连接接入调用，可进行与Request和Response相关的对象初始化操作，并保存response对象，用于后续写入数据
- Read：请求连接有数据可读时调用
- End：当数据可用时，如果读取到文件结束或者response被关闭时则被调用
- Error：在连接上发生异常时调用，数据读取异常、连接断开、处理异常、socket超时

Note：

- Read：在post请求有数据，但在begin事件中没有处理，则会调用read，如果read没有读取数据，在会触发Error回调，关闭socket
- End：当socket超时，并且response被关闭时也会调用；server被关闭时调用
- Error：除了socket超时不会关闭socket，其他都会关闭socket
- End和Error时间触发时应关闭当前comet会话，即调用CometEvent的close方法 Note：在事件触发时要做好线程安全的操作
<a name="yOw85"></a>
## 异步Servlet
![2021-08-05-07-48-04-455135.jpg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121705088-8062a404-8ae5-49cf-9c96-841edfc281ec.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=u965f5da4&originHeight=368&originWidth=600&originalType=binary&ratio=1&size=9139&status=done&style=shadow&taskId=u90944833-199e-4019-b047-e13480fec1f)<br />传统流程：

- 首先，Servlet 接收到请求之后，request数据解析；
- 接着，调用业务接口的某些方法，以完成业务处理；
- 最后，根据处理的结果提交响应，Servlet 线程结束

![2021-08-05-07-48-04-690207.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1628121715484-f66688b8-f841-4cd8-93c8-78b246c4ca8f.jpeg#clientId=u665eb4b4-983f-4&from=ui&id=ufcdc25da&originHeight=396&originWidth=640&originalType=binary&ratio=1&size=9977&status=done&style=shadow&taskId=u89025488-b550-45b0-8849-85c8d0847e6)<br />异步处理流程：

- 客户端发送一个请求
- Servlet容器分配一个线程来处理容器中的一个servlet
- servlet调用`request.startAsync()`，保存`AsyncContext`, 然后返回
- 任何方式存在的容器线程都将退出，但是response仍然保持开放
- 业务线程使用保存的`AsyncContext`来完成响应（线程池）
- 客户端收到响应

Servlet 线程将请求转交给一个异步线程来执行业务处理，线程本身返回至容器，此时 Servlet 还没有生成响应数据，异步线程处理完业务以后，可以直接生成响应数据（异步线程拥有 ServletRequest 和 ServletResponse 对象的引用）。
<a name="b1uMi"></a>
## 为什么web应用中支持异步？
推出异步，主要是针对那些比较耗时的请求：比如一次缓慢的数据库查询，一次外部REST API调用, 或者是其他一些I/O密集型操作。这种耗时的请求会很快的耗光Servlet容器的线程池，继而影响可扩展性。<br />Note：从客户端的角度来看，request仍然像任何其他的HTTP的request-response交互一样，只是耗费了更长的时间而已。
<a name="onqSs"></a>
## 异步事件监听

- `onStartAsync`：Request调用`startAsync`方法时触发
- `onComplete`：`syncContext`调用`complete`方法时触发
- `onError`：处理请求的过程出现异常时触发
- `onTimeout`：`socket`超时触发

Note : `onError`/ `onTimeout`触发后，会紧接着回调onComplete onComplete 执行后，就不可再操作request和response。
