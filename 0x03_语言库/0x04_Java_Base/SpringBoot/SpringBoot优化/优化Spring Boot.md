JavaSpringBoot<br />微服务用到一时爽，没用好就惨了，特别是对于服务拆分没有把控好业务边界、拆分粒度过大等问题，某些 Spring Boot 启动速度太慢了，可能也会有这种体验，这里将探索一下关于 Spring Boot 启动速度优化的一些方方面面。
<a name="J9RNB"></a>
## **启动时间分析**
IDEA 自带集成了 async-profile 工具，所以可以通过火焰图来更直观的看到一些启动过程中的问题，比如下图例子当中，通过火焰图来看大量的耗时在 Bean 加载和初始化当中。<br />图来自 IDEA 自带集成的 async-profile 工具，可在 Preferences 中搜索 Java Profiler 自定义配置，启动使用 Run with xx Profiler。<br />y 轴表示调用栈，每一层都是一个函数，调用栈越深，火焰就越高，顶部就是正在执行的函数，下方都是它的父函数。<br />x 轴表示抽样数，如果一个函数在 x 轴占据的宽度越宽，就表示它被抽到的次数多，即执行的时间长。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572130-a2542ffb-be7b-4d84-bb7a-5d54316d93f9.jpeg#clientId=u806749cb-ab29-4&from=paste&id=uff833693&originHeight=650&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7da252e7-2e75-4dea-bc88-80adb036443&title=)
<a name="b9t6F"></a>
## **启动优化**
<a name="G73Y1"></a>
### 减少业务初始化
大部分的耗时应该都在业务太大或者包含大量的初始化逻辑，比如建立数据库连接、Redis连接、各种连接池等等，对于业务方的建议则是尽量减少不必要的依赖，能异步则异步。
<a name="Xw4Uv"></a>
### 延迟初始化
Spring Boot 2.2版本后引入 `spring.main.lazy-initialization`属性，配置为 true 表示所有 Bean 都将延迟初始化。<br />可以一定程度上提高启动速度，但是第一次访问可能较慢。
```xml
spring.main.lazy-initialization=true
```
<a name="WBJjO"></a>
### Spring Context Indexer
Spring5 之后版本提供了spring-context-indexer功能，主要作用是解决在类扫描的时候避免类过多导致的扫描速度过慢的问题。<br />使用方法也很简单，导入依赖，然后在启动类打上`@Indexed`注解，这样在程序编译打包之后会生成META-INT/spring.components文件，当执行`ComponentScan`扫描类时，会读取索引文件，提高扫描速度。
```xml
<dependency>
	<groupId>org.springframework</groupId>
	<artifactId>spring-context-indexer</artifactId>
	<optional>true</optional>
</dependency>
```
<a name="FNjkg"></a>
### 关闭JMX
Spring Boot 2.2.X 版本以下默认会开启 JMX，可以使用 jconsole 查看，对于无需这些监控的话可以手动关闭它。
```
spring.jmx.enabled=false
```
<a name="AHPNA"></a>
### 关闭分层编译
Java8 之后的版本，默认打开多层编译，使用命令`java -XX:+PrintFlagsFinal -version | grep CompileThreshold`查看。<br />Tier3 就是 C1、Tier4 就是 C2，表示一个方法解释编译 2000 次进行 C1编译，C1编译后执行 15000 次会进行 C2编译。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572071-dce70fa3-22a6-4b96-ac0f-fc38d7ac695c.jpeg#clientId=u806749cb-ab29-4&from=paste&id=uaf50c7cb&originHeight=214&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3e66af70-8c6b-435b-940c-f830cb44527&title=)<br />可以通过命令使用 C1 编译器，这样就不存在 C2 的优化阶段，能够提高启动速度，同时配合 `-Xverify:none/ -noverify` 关闭字节码验证，但是，尽量不要在线上环境使用。
```bash
-XX:TieredStopAtLevel=1 -noverify
```
<a name="IeT3s"></a>
## **另外的思路**
上面介绍了一些从业务层面、启动参数之类的优化，下面再看看基于 Java 应用本身有哪些途径可以进行优化。<br />在此之前，回忆一下 Java 创建对象的过程，首先要进行类加载，然后去创建对象，对象创建之后就可以调用对象方法了，这样就还会涉及到 JIT，JIT通过运行时将字节码编译为本地机器码来提高 Java 程序的性能。<br />因此，下面涉及到的技术将会概括以上涉及到的几个步骤。
<a name="oQCdF"></a>
### JAR Index
Jar包其实本质上就是一个 ZIP 文件，当加载类的时候，通过类加载器去遍历Jar包，找到对应的 class 文件进行加载，然后验证、准备、解析、初始化、实例化对象。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572147-eb3a0f3e-ab22-420f-9bd8-5dc6984a5093.jpeg#clientId=u806749cb-ab29-4&from=paste&id=udf3436e4&originHeight=353&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u43bb9322-5d46-49e8-bf15-bdf83e75941&title=)<br />JarIndex 其实是一个很古老的技术，就是用来解决在加载类的时候遍历 Jar 性能问题，早在 JDK1.3的版本中就已经引入。<br />假设要在A\B\C 3个Jar包中查找一个class，如果能够通过类型com.C，立刻推断出具体在哪个jar包，就可以避免遍历 jar 的过程。
```bash
A.jar
com/A

B.jar
com/B

C.jar
com/C
```
通过 Jar Index 技术，就可以生成对应的索引文件 INDEX.LIST。
```bash
com/A --> A.jar
com/B --> B.jar
com/C --> C.jar
```
不过对于现在的项目来说，Jar Index 很难应用：

1. **通过 jar -i 生成的索引文件是基于 META-INF/MANIFEST.MF 中的 Class-Path 来的，目前大多项目都不会涉及到这个，所以索引文件的生成需要自己去做额外处理**
2. **只支持 URLClassloader，需要自己自定义类加载逻辑**
<a name="ga5vF"></a>
### APPCDS
App CDS 全称为 Application Class Data Sharing，主要是用于启动加速和节省内存，其实早在在 JDK1.5 版本就已经引入，只是在后续的版本迭代过程中在不断的优化升级，JDK13 版本中则是默认打开，早期的 CDS 只支持`BootClassLoader`， 在 JDK8 中引入了 AppCDS，支持 `AppClassLoader` 和 自定义的 `ClassLoader`。<br />都知道类加载的过程中伴随解析、校验这个过程，CDS 就是将这个过程产生的数据结构存储到归档文件中，在下次运行的时候重复使用，这个归档文件被称作 Shared Archive，以jsa作为文件后缀。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572093-2bb5392f-f5cd-4497-a4b7-83b6c6b72eb2.jpeg#clientId=u806749cb-ab29-4&from=paste&id=u633b00c6&originHeight=353&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9e26b989-95ca-4e45-80c3-79ed6feabcc&title=)<br />在使用时，则是将 jsa 文件映射到内存当中，让对象头中的类型指针指向该内存地址。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572151-f219e5cc-b6f7-4bd0-8763-64d7e136c4a6.jpeg#clientId=u806749cb-ab29-4&from=paste&id=ua110de25&originHeight=163&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u61281360-e4b0-4b6a-bb0f-cfd0a727d26&title=)<br />一起看看怎么使用。<br />首先，需要生成希望在应用程序之间共享的类列表，也即是 lst文件。对于 Oracle JDK 需要加入 `-XX:+UnlockCommercialFeature` 命令来开启商业化的能力，openJDK 无需此参数，JDK13的版本中将1、2两步合并为一步，但是低版本还是需要这样做。
```bash
java -XX:DumpLoadedClassList=test.lst
```
然后得到 lst 类列表之后，dump 到适合内存映射的 jsa 文件当中进行归档。
```bash
java -Xshare:dump -XX:SharedClassListFile=test.lst -XX:SharedArchiveFile=test.jsa
```
最后，在启动时加入运行参数指定归档文件即可。
```bash
-Xshare:on -XX:SharedArchiveFile=test.jsa
```
需要注意的是，AppCDS只会在包含所有 class 文件的 FatJar 生效，对于 SpringBoot 的嵌套 Jar 结构无法生效，需要利用 `maven shade plugin` 来创建 shade jar。
```xml
<build>
	<finalName>helloworld</finalName>
	<plugins>
		<plugin>
			<groupId>org.apache.maven.plugins</groupId>
			<artifactId>maven-shade-plugin</artifactId>
			<configuration>
				<keepDependenciesWithProvidedScope>true</keepDependenciesWithProvidedScope>
				<createDependencyReducedPom>false</createDependencyReducedPom>
				<filters>
					<filter>
						<artifact>*:*</artifact>
						<excludes>
							<exclude>META-INF/*.SF</exclude>
							<exclude>META-INF/*.DSA</exclude>
							<exclude>META-INF/*.RSA</exclude>
						</excludes>
					</filter>
				</filters>
			</configuration>
			<executions>
				<execution>
					<phase>package</phase>
					<goals><goal>shade</goal></goals>
					<configuration>
						<transformers>
							<transformer implementation="org.apache.maven.plugins.shade.resource.AppendingTransformer">
								<resource>META-INF/spring.handlers</resource>
							</transformer>
							<transformer implementation="org.springframework.boot.maven.PropertiesMergingResourceTransformer">
								<resource>META-INF/spring.factories</resource>
							</transformer>
							<transformer implementation="org.apache.maven.plugins.shade.resource.AppendingTransformer">
								<resource>META-INF/spring.schemas</resource>
							</transformer>
							<transformer implementation="org.apache.maven.plugins.shade.resource.ServicesResourceTransformer" />
							<transformer implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
								<mainClass>${mainClass}</mainClass>
							</transformer>
						</transformers>
					</configuration>
				</execution>
			</executions>
		</plugin>
	</plugins>
</build>
```
然后按照上述的步骤使用才可以，但是如果项目过大，文件数大于65535启动会报错：<br />Caused by: java.lang.IllegalStateException: Zip64 archives are not supported<br />源码如下：
```java
public int getNumberOfRecords() {
	long numberOfRecords = Bytes.littleEndianValue(this.block, this.offset + 10, 2);
	if (numberOfRecords == 0xFFFF) {
		throw new IllegalStateException("Zip64 archives are not supported");
	}
```
在 2.2 及以上版本修复了这个问题，所以使用的时候尽量使用高版本可以避免此类问题的出现。
<a name="yMnSa"></a>
### **Heap Archive**
JDK9 中引入了HeapArchive，并且 JDK12 中被正式使用，可以认为 Heap Archive 是对 APPCDS 的一个延伸。<br />APPCDS 是持久化了类加载过程中验证、解析产生的数据，Heap Archive 则是类初始化(执行 static 代码块 cinit 进行初始化) 相关的堆内存的数据。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572455-ebc6561f-57b3-43fa-9b97-bcde72f99a34.jpeg#clientId=u806749cb-ab29-4&from=paste&id=u7c896cb9&originHeight=254&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9598b917-f168-4604-9c2d-3cb5d1170b4&title=)<br />简单来讲，可以认为 HeapArchive 是在类初始化的时候通过内存映射持久化了一些 static 字段，避免调用类初始化器，提前拿到初始化好的类，提高启动速度。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572673-974f1974-0c16-48de-b107-cbd16eb301bf.jpeg#clientId=u806749cb-ab29-4&from=paste&id=uc6045301&originHeight=634&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u69e31f7d-a99f-4d69-bf38-cb57fac1f4b&title=)
<a name="c2beD"></a>
### AOT编译
JIT 是通过运行时将字节码编译为本地机器码，需要的时候直接执行，减少了解释的时间，从而提高程序运行速度。<br />上面提到的 3 个提高应用启动速度的方式都可以归为类加载的过程，到真正创建对象实例、执行方法的时候，由于可能没有被 JIT 编译，在解释模式下执行的速度非常慢，所以产生了 AOT 编译的方式。<br />AOT(Ahead-Of-Time) 指的是程序运行之前发生的编译行为，他的作用相当于是**预热**，提前编译为机器码，减少解释时间。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572673-a6e63bdf-f0af-4ad1-84b6-2ac7e946fb14.jpeg#clientId=u806749cb-ab29-4&from=paste&id=u3870caa3&originHeight=611&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua0dd9128-c331-4c43-9fb8-decce9f4701&title=)<br />比如现在 Spring Cloud Native 就是这样，在运行时直接静态编译成可执行文件，不依赖 JVM，所以速度非常快。<br />但是 Java 中 AOT 技术不够成熟，作为实验性的技术在 JDK8 之后版本默认关闭，需要手动打开。
```bash
java -XX:+UnlockExperimentalVMOptions -XX:AOTLibrary=
```
并且由于长期缺乏维护和调优这项技术，在 JDK 16 的版本中已经被移除，这里就不再赘述了。
<a name="uKGuX"></a>
## **下线时间优化**
<a name="pXuxU"></a>
### 优雅下线
Spring Boot 在 2.3 版本中增加了新特性**优雅停机**，支持Jetty、Reactor Netty、Tomcat 和 Undertow，使用方式：
```yaml
server:
  shutdown: graceful

# 最大等待时间
spring:
  lifecycle:
    timeout-per-shutdown-phase: 30s
```
如果低于 2.3 版本，官方也提供了低版本的实现方案，新版本中的实现基本也是这个逻辑，先暂停外部请求，关闭线程池处理剩余的任务。
```java
@SpringBootApplication
@RestController
public class Gh4657Application {
	
	public static void main(String[] args) {
		SpringApplication.run(Gh4657Application.class, args);
	}
	
	@RequestMapping("/pause")
	public String pause() throws InterruptedException {
		Thread.sleep(10000);
		return "Pause complete";
	}
	
	@Bean
	public GracefulShutdown gracefulShutdown() {
		return new GracefulShutdown();
	}
	
	@Bean
	public EmbeddedServletContainerCustomizer tomcatCustomizer() {
		return new EmbeddedServletContainerCustomizer() {
			
			@Override
			public void customize(ConfigurableEmbeddedServletContainer container) {
				if (container instanceof TomcatEmbeddedServletContainerFactory) {
					((TomcatEmbeddedServletContainerFactory) container)
					.addConnectorCustomizers(gracefulShutdown());
				}
				
			}
		};
	}
	
	private static class GracefulShutdown implements TomcatConnectorCustomizer,
	ApplicationListener<ContextClosedEvent> {
		
		private static final Logger log = LoggerFactory.getLogger(GracefulShutdown.class);
		
		private volatile Connector connector;
		
		@Override
		public void customize(Connector connector) {
			this.connector = connector;
		}
		
		@Override
		public void onApplicationEvent(ContextClosedEvent event) {
			this.connector.pause();
			Executor executor = this.connector.getProtocolHandler().getExecutor();
			if (executor instanceof ThreadPoolExecutor) {
				try {
					ThreadPoolExecutor threadPoolExecutor = (ThreadPoolExecutor) executor;
					threadPoolExecutor.shutdown();
					if (!threadPoolExecutor.awaitTermination(30, TimeUnit.SECONDS)) {
						log.warn("Tomcat thread pool did not shut down gracefully within "
								 + "30 seconds. Proceeding with forceful shutdown");
					}
				}
				catch (InterruptedException ex) {
					Thread.currentThread().interrupt();
				}
			}
		}
		
	}
	
}
```
<a name="kOo3Y"></a>
### Eureka服务下线时间
Eureka 使用了三级缓存来保存服务的实例信息。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572679-5c9ba7d9-78b4-440e-b3d8-2e521717a92a.jpeg#clientId=u806749cb-ab29-4&from=paste&id=ud407466c&originHeight=522&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u38e5e6d0-c677-45c8-982d-4c8b3e784ef&title=)<br />服务注册的时候会和 server 保持一个心跳，这个心跳的时间是 30 秒，服务注册之后，客户端的实例信息保存到 Registry 服务注册表当中，注册表中的信息会立刻同步到 readWriteCacheMap 之中。<br />而客户端如果感知到这个服务，要从 readOnlyCacheMap 去读取，这个只读缓存需要 30 秒的时间去从 readWriteCacheMap 中同步。<br />客户端和 Ribbon 负载均衡 都保持一个本地缓存，都是 30 秒定时同步。<br />按照上面所说，来计算一下客户端感知到一个服务下线极端的情况需要多久。

1. **客户端每隔 30 秒会发送心跳到服务端**
2. **registry 保存了所有服务注册的实例信息，他会和 readWriteCacheMap 保持一个实时的同步，而 readWriteCacheMap 和 readOnlyCacheMap 会每隔 30 秒同步一次。**
3. **客户端每隔 30 秒去同步一次 readOnlyCacheMap 的注册实例信息**
4. **考虑到如果使用 ribbon 做负载均衡的话，他还有一层缓存每隔 30 秒同步一次**

如果说一个服务的正常下线，极端的情况这个时间应该就是 30+30+30+30 差不多 120 秒的时间了。<br />如果服务非正常下线，还需要靠每 60 秒执行一次的清理线程去剔除超过 90 秒没有心跳的服务，那么这里的极端情况可能需要 3 次 60秒才能检测出来，就是 180 秒的时间。<br />累计可能最长的感知时间就是：180 + 120 = 300 秒，5分钟的时间。<br />解决方案当然就是改这些时间。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658394572725-0bc80c73-32f2-43f7-a82c-17e48444d7c1.jpeg#clientId=u806749cb-ab29-4&from=paste&id=u2761d046&originHeight=348&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u889bb081-5481-45a4-a8d2-7f61b1d5f60&title=)<br />**修改 ribbon 同步缓存的时间为 3 秒**：`ribbon.ServerListRefreshInterval = 3000`<br />**修改客户端同步缓存时间为 3 秒** ：`eureka.client.registry-fetch-interval-seconds = 3`<br />**心跳间隔时间修改为 3 秒**：`eureka.instance.lease-renewal-interval-in-seconds = 3`<br />**超时剔除的时间改为 9 秒**：`eureka.instance.lease-expiration-duration-in-seconds = 9`<br />**清理线程定时时间改为 5 秒执行一次**：`eureka.server.eviction-interval-timer-in-ms = 5000`<br />**同步到只读缓存的时间修改为 3 秒一次**：`eureka.server.response-cache-update-interval-ms = 3000`<br />如果按照这个时间参数设置重新计算可能感知到服务下线的最大时间：<br />正常下线就是 3+3+3+3=12 秒，非正常下线再加 15 秒为 27 秒。
