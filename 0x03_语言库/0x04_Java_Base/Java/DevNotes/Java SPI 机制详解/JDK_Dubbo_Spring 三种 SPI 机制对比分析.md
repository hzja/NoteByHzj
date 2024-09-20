Java Spring Dubbo Spring SPI<br />SPI 全称为 Service Provider Interface，是一种服务发现机制。SPI 的本质是将接口实现类的全限定名配置在文件中，并由服务加载器读取配置文件，加载实现类。这样可以在运行时，动态为接口替换实现类。正因此特性，可以很容易的通过 SPI 机制为程序提供拓展功能。
<a name="zAqgb"></a>
## SPI 有什么用？
举个栗子，现在设计了一款全新的日志框架：super-logger。默认以XML文件作为这款日志的配置文件，并设计了一个配置文件解析的接口：
```java
public interface SuperLoggerConfiguration {
    void configure(String configFile);
}
```
然后来一个默认的XML实现：
```java
public class XMLConfiguration implements SuperLoggerConfiguration{
    public void configure(String configFile){
        ......
    }
}
```
那么在初始化，解析配置时，只需要调用这个`XMLConfiguration`来解析XML配置文件即可。
```java
public class LoggerFactory {
    static {
        SuperLoggerConfiguration configuration = new XMLConfiguration();
        configuration.configure(configFile);
    }

    public static getLogger(Class clazz){
        ......
    }
}
```
这样就完成了一个基础的模型，看起来也没什么问题。不过扩展性不太好，因为如果想定制/扩展/重写解析功能的话，还得重新定义入口的代码，LoggerFactory 也得重写，不够灵活，侵入性太强了。<br />比如现在用户/使用方想增加一个 yml 文件的方式，作为日志配置文件，那么只需要新建一个`YAMLConfiguration`，实现 `SuperLoggerConfiguration` 就可以。但是……怎么注入呢，怎么让 LoggerFactory中使用新建的这个 `YAMLConfiguration` ？难不成连 LoggerFactory 也重写了？<br />如果借助SPI机制的话，这个事情就很简单了，可以很方便的完成这个入口的扩展功能。<br />下面就先来看看，利用JDK 的 SPI 机制怎么解决上面的扩展性问题。
<a name="nQIHB"></a>
## JDK SPI
JDK 中 提供了一个 SPI 的功能，核心类是 java.util.ServiceLoader。其作用就是，可以通过类名获取在"META-INF/services/"下的多个配置实现文件。<br />为了解决上面的扩展问题，现在在**META-INF/services/**下创建一个`com.fcant.spisamples.SuperLoggerConfiguration`文件（没有后缀）。文件中只有一行代码，那就是默认的`com.fcant.spisamples.XMLConfiguration`（注意，一个文件里也可以写多个实现，回车分隔）
```java
META-INF/services/com.github.kongwu.spisamples.SuperLoggerConfiguration:

com.fcant.spisamples.XMLConfiguration
```
然后通过 ServiceLoader 获取 SPI 机制配置的实现类：
```java
ServiceLoader<SuperLoggerConfiguration> serviceLoader = ServiceLoader.load(SuperLoggerConfiguration.class);
Iterator<SuperLoggerConfiguration> iterator = serviceLoader.iterator();
SuperLoggerConfiguration configuration;

while(iterator.hasNext()) {
    //加载并初始化实现类
    configuration = iterator.next();
}

//对最后一个configuration类调用configure方法
configuration.configure(configFile);
```
最后在调整LoggerFactory中初始化配置的方式为现在的SPI方式：
```java
public class LoggerFactory {
    static {
        ServiceLoader<SuperLoggerConfiguration> serviceLoader = ServiceLoader.load(SuperLoggerConfiguration.class);
        Iterator<SuperLoggerConfiguration> iterator = serviceLoader.iterator();
        SuperLoggerConfiguration configuration;

        while(iterator.hasNext()) {
            configuration = iterator.next();//加载并初始化实现类
        }
        configuration.configure(configFile);
    }

    public static getLogger(Class clazz){
        ......
    }
}
```
这里为什么是用 iterator ? 而不是`get`之类的只获取一个实例的方法？<br />试想一下，如果是一个固定的`get`方法，那么get到的是一个固定的实例，SPI 还有什么意义呢？<br />SPI 的目的，就是增强扩展性。将固定的配置提取出来，通过 SPI 机制来配置。那既然如此，一般都会有一个默认的配置，然后通过 SPI 的文件配置不同的实现，这样就会存在一个接口多个实现的问题。要是找到多个实现的话，用哪个实现作为最后的实例呢？<br />所以这里使用iterator来获取所有的实现类配置。刚才已经在这个super-logger包里增加了默认的`SuperLoggerConfiguration` 实现。<br />为了支持 YAML 配置，现在在使用方/用户的代码里，增加一个`YAMLConfiguration`的 SPI 配置：
```java
META-INF/services/com.github.kongwu.spisamples.SuperLoggerConfiguration:

com.fcant.spisamples.ext.YAMLConfiguration
```
此时通过iterator方法，就会获取到默认的`XMLConfiguration`和扩展的这个`YAMLConfiguration`两个配置实现类了。<br />在上面那段加载的代码里，遍历iterator，遍历到最后，使用最后一个实现配置作为最终的实例。<br />最后一个？怎么算最后一个？<br />使用方/用户自定义的的这个 `YAMLConfiguration` 一定是最后一个吗？<br />这个真的不一定，取决于运行时的 ClassPath 配置，在前面加载的jar自然在前，最后的jar里的自然当然也在后面。所以如果用户的包在ClassPath中的顺序比super-logger的包更靠后，才会处于最后一个位置；如果用户的包位置在前，那么所谓的最后一个仍然是默认的`XMLConfiguration`。<br />举个栗子，如果程序的启动脚本为：
```java
java -cp super-logger.jar:a.jar:b.jar:main.jar example.Main
```
默认的`XMLConfiguration` SPI配置在super-logger.jar，扩展的`YAMLConfiguration` SPI配置文件在main.jar，那么iterator获取的最后一个元素一定为`YAMLConfiguration`。<br />但这个classpath顺序如果反了呢？main.jar 在前，super-logger.jar 在后
```java
java -cp main.jar:super-logger.jar:a.jar:b.jar example.Main
```
这样一来，iterator 获取的最后一个元素又变成了默认的`XMLConfiguration`，使用 JDK SPI 没啥意义了，获取的又是第一个，还是默认的`XMLConfiguration`。<br />由于这个加载顺序（classpath）是由用户指定的，所以无论加载第一个还是最后一个，都有可能会导致加载不到用户自定义的那个配置。
:::success
所以这也是JDK SPI机制的一个劣势，无法确认具体加载哪一个实现，也无法加载某个指定的实现，仅靠ClassPath的顺序是一个非常不严谨的方式
:::
<a name="mfTJ3"></a>
## Dubbo SPI
Dubbo 就是通过 SPI 机制加载所有的组件。不过，Dubbo 并未使用 Java 原生的 SPI 机制，而是对其进行了增强，使其能够更好的满足需求。在 Dubbo 中，SPI 是一个非常重要的模块。基于 SPI，可以很容易的对 Dubbo 进行拓展。如果大家想要学习 Dubbo 的源码，SPI 机制务必弄懂。接下来，先来了解一下 Java SPI 与 Dubbo SPI 的用法，然后再来分析 Dubbo SPI 的源码。<br />Dubbo 中实现了一套新的 SPI 机制，功能更强大，也更复杂一些。相关逻辑被封装在了 ExtensionLoader 类中，通过 ExtensionLoader，可以加载指定的实现类。Dubbo SPI 所需的配置文件需放置在 META-INF/dubbo 路径下，配置内容如下（以下demo来自dubbo官方文档）。
```java
optimusPrime = org.apache.spi.OptimusPrime
bumblebee = org.apache.spi.Bumblebee
```
与 Java SPI 实现类配置不同，Dubbo SPI 是通过键值对的方式进行配置，这样可以按需加载指定的实现类。另外在使用时还需要在接口上标注 `@SPI` 注解。下面来演示 Dubbo SPI 的用法：
```java
@SPI
public interface Robot {
    void sayHello();
}

public class OptimusPrime implements Robot {

    @Override
    public void sayHello() {
        System.out.println("Hello, I am Optimus Prime.");
    }
}

public class Bumblebee implements Robot {

    @Override
    public void sayHello() {
        System.out.println("Hello, I am Bumblebee.");
    }
}


public class DubboSPITest {

    @Test
    public void sayHello() throws Exception {
        ExtensionLoader<Robot> extensionLoader = 
            ExtensionLoader.getExtensionLoader(Robot.class);
        Robot optimusPrime = extensionLoader.getExtension("optimusPrime");
        optimusPrime.sayHello();
        Robot bumblebee = extensionLoader.getExtension("bumblebee");
        bumblebee.sayHello();
    }
}
```
Dubbo SPI 和 JDK SPI 最大的区别就在于支持“别名”，可以通过某个扩展点的别名来获取固定的扩展点。就像上面的例子中，可以获取 Robot 多个 SPI 实现中别名为“optimusPrime”的实现，也可以获取别名为“bumblebee”的实现，这个功能非常有用！<br />通过 `@SPI` 注解的 value 属性，还可以默认一个“别名”的实现。比如在Dubbo 中，默认的是Dubbo 私有协议：`**dubbo protocol - dubbo://****` 来看看Dubbo中协议的接口：
```java
@SPI("dubbo")
public interface Protocol {
    ......
}
```
在 Protocol 接口上，增加了一个 `@SPI` 注解，而注解的 value 值为 Dubbo ，通过 SPI 获取实现时就会获取      Protocol SPI 配置中别名为dubbo的那个实现，com.alibaba.dubbo.rpc.Protocol文件如下：
```java
filter=com.alibaba.dubbo.rpc.protocol.ProtocolFilterWrapper
listener=com.alibaba.dubbo.rpc.protocol.ProtocolListenerWrapper
mock=com.alibaba.dubbo.rpc.support.MockProtocol


dubbo=com.alibaba.dubbo.rpc.protocol.dubbo.DubboProtocol


injvm=com.alibaba.dubbo.rpc.protocol.injvm.InjvmProtocol
rmi=com.alibaba.dubbo.rpc.protocol.rmi.RmiProtocol
hessian=com.alibaba.dubbo.rpc.protocol.hessian.HessianProtocol
com.alibaba.dubbo.rpc.protocol.http.HttpProtocol
com.alibaba.dubbo.rpc.protocol.webservice.WebServiceProtocol
thrift=com.alibaba.dubbo.rpc.protocol.thrift.ThriftProtocol
memcached=com.alibaba.dubbo.rpc.protocol.memcached.MemcachedProtocol
redis=com.alibaba.dubbo.rpc.protocol.redis.RedisProtocol
rest=com.alibaba.dubbo.rpc.protocol.rest.RestProtocol
registry=com.alibaba.dubbo.registry.integration.RegistryProtocol
qos=com.alibaba.dubbo.qos.protocol.QosProtocolWrapper
```
然后只需要通过`getDefaultExtension`，就可以获取到 `@SPI` 注解上value对应的那个扩展实现了
```java
Protocol protocol = ExtensionLoader.getExtensionLoader(Protocol.class).getDefaultExtension();
//protocol: DubboProtocol
```
还有一个 Adaptive 的机制，虽然非常灵活，但……用法并不是很“优雅”，这里就不介绍了<br />Dubbo 的 SPI 中还有一个“加载优先级”，优先加载内置（internal）的，然后加载外部的（external），按优先级顺序加载，如果遇到重复就跳过不会加载了。<br />所以如果想靠classpath加载顺序去覆盖内置的扩展，也是个不太理智的做法，原因同上 - 加载顺序不严谨
<a name="GQGiO"></a>
## Spring SPI
Spring 的 SPI 配置文件是一个固定的文件 -**META-INF/spring.factories**，功能上和 JDK 的类似，每个接口可以有多个扩展实现，使用起来非常简单：
```java
//获取所有factories文件中配置的LoggingSystemFactory
List<LoggingSystemFactory>> factories = 
SpringFactoriesLoader.loadFactories(LoggingSystemFactory.class, classLoader);
```
下面是一段 Spring Boot 中 spring.factories 的配置
```java
# Logging Systems
org.springframework.boot.logging.LoggingSystemFactory=\
org.springframework.boot.logging.logback.LogbackLoggingSystem.Factory,\
org.springframework.boot.logging.log4j2.Log4J2LoggingSystem.Factory,\
org.springframework.boot.logging.java.JavaLoggingSystem.Factory

# PropertySource Loaders
org.springframework.boot.env.PropertySourceLoader=\
org.springframework.boot.env.PropertiesPropertySourceLoader,\
org.springframework.boot.env.YamlPropertySourceLoader

# ConfigData Location Resolvers
org.springframework.boot.context.config.ConfigDataLocationResolver=\
org.springframework.boot.context.config.ConfigTreeConfigDataLocationResolver,\
org.springframework.boot.context.config.StandardConfigDataLocationResolver

......
```
Spring SPI 中，将所有的配置放到一个固定的文件中，省去了配置一大堆文件的麻烦。至于多个接口的扩展配置，是用一个文件好，还是每个单独一个文件好这个，这个问题就见仁见智了（个人喜欢 Spring 这种，干净利落）。<br />Spring的SPI 虽然属于spring-framework(core)，但是目前主要用在spring boot中……<br />和前面两种 SPI 机制一样，Spring 也是支持 ClassPath 中存在多个 spring.factories 文件的，加载时会按照 classpath 的顺序依次加载这些 spring.factories 文件，添加到一个 ArrayList 中。由于没有别名，所以也没有去重的概念，有多少就添加多少。<br />但由于 Spring 的 SPI 主要用在 Spring Boot 中，而 Spring Boot 中的 ClassLoader 会优先加载项目中的文件，而不是依赖包中的文件。所以如果在项目中定义个spring.factories文件，那么项目中的文件会被第一个加载，得到的Factories中，项目中spring.factories里配置的那个实现类也会排在第一个<br />如果要扩展某个接口的话，只需要在项目（spring boot）里新建一个**META-INF/spring.factories**文件，只添加要的那个配置，不要完整的复制一遍 Spring Boot 的  **spring.factories** 文件然后修改** 比如只想添加一个新的 LoggingSystemFactory 实现，那么只需要新建一个**META-INF/spring.factories**文件，而不是完整的复制+修改：
```java
org.springframework.boot.logging.LoggingSystemFactory=\
com.example.log4j2demo.Log4J2LoggingSystem.Factory
```
<a name="pQ8nY"></a>
## 对比
![2021-05-03-11-08-02-753087.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620011581331-fd728925-1ab1-474e-984e-f6ef6f4f719c.png#clientId=u3811e8a8-3117-4&from=ui&id=u4ef05a40&originHeight=739&originWidth=822&originalType=binary&size=22301&status=done&style=shadow&taskId=ud2651bbf-cad7-44b1-93c7-ccde0964629)<br />三种 SPI 机制对比之下，JDK 内置的机制是最弱的，但是由于是 JDK 内置，所以还是有一定应用场景，毕竟不用额外的依赖；Dubbo 的功能最丰富，但机制有点复杂了，而且只能配合 Dubbo 使用，不能完全算是一个独立的模块；Spring 的功能和JDK的相差无几，最大的区别是所有扩展点写在一个 spring.factories 文件中，也算是一个改进，并且 IDEA 完美支持语法提示。
