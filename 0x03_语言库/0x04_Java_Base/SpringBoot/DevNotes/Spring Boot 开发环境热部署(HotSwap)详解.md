JavaSpringBoot
<a name="olu6v"></a>
## 前言
Spring Boot 提出了多项开箱即用的功能特性，但归根到底还是围绕简化应用的创建、开发、运行。开发环境下经常对项目代码进行变动，如果每次都重新启动应用会浪费大量时间，为此就产生了多种进行热部署的方案，可以在不重启的情况下使用新的代码。
<a name="n9Oqp"></a>
## 热部署常用实现方案
然而，在 Java 中实现热部署并不是一件容易的事情。
<a name="N0VMK"></a>
### ClassLoader 重新加载
Java 作为一种静态语言，类一经加载到 JVM 中，便无法修改，而且同一个类加载器对于同一个类只能加载一次，因此热部署常用的一种解决方案是创建新的 ClassLoader 加载新的 class 文件，然后替换之前创建的对象。
<a name="giwfB"></a>
### Java Agent
另一种解决方案是使用 Java Agent，Java Agent 可以理解为 JVM 层面的 AOP，可以在类加载时将 class 文件的内容修改为自定义的内容，并且支持修改已加载到 JVM 的 class，不过对于已加载到 JVM 的 class 只能修改方法体，因此具有一定的局限性。
<a name="ahRVO"></a>
## spring-boot-devtools
<a name="mUyeU"></a>
### spring-boot-devtools 快速上手
Spring Boot 通过 Maven 插件 spring-boot-devtools 提供对热部署的支持，只要将这个依赖添加到类路径，当类路径下的 class 发生变化时就会自动重启应用上下文，从而使用新的 class 文件中的代码。这个插件的坐标如下。
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-devtools</artifactId>
	<optional>true</optional>
</dependency>
```
引入依赖时指定 optional 避免依赖传递，同时 spring-boot-maven-plugin 打包时也会忽略 spring-boot-devtools 插件。
<a name="A1Dl9"></a>
### spring-boot-devtools 功能特性
spring-boot-devtools 作为一个开发环境的插件，不仅支持热部署，具体来说有以下特性。<br />将第三方库(如 thymeleaf、freemarker)缓存相关的属性配置到 `Environment`，以便开发环境禁用缓存。类路径下的 class 文件发生变更时触发 `ApplicationContext` 重启。内嵌 LiveReload 服务器，资源发生变化时触发浏览器刷新。支持全局配置，所有的 Spring Boot 应用的 spring-boot-devtools 插件使用同一套配置，如指定检查 class 文件变化的轮训时间。支持远程触发热部署(不推荐使用)。
<a name="tR01y"></a>
### spring-boot-devtools 实现原理
虽然 spring-boot-devtools 支持添加配置用来修改自身行为，通常情况下使用默认配置即可，不再赘述配置相关内容。下面把重点放到 spring-boot-devtools 热部署的具体实现上。<br />spring-boot-devtools 热部署使用了 ClassLoader 重新加载 的实现方式，具体来说使用两类 `ClassLoader`，一类是加载第三方库的 `CladdLoader`，另一类是加载应用类路径下 class 的自定义 `RestartClassLoader`，应用类路径下 class 变化会触发应用重新启动，由于不需要重新加载第三方库的 class，因此相比重新启动整个应用速度上会快一些。<br />那具体到代码层面怎么实现的呢？spring-boot-devtools 利用 Spring Boot 应用自动装配的特性，在 spring.factories 文件中添加了很多配置。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652092177636-b68aea02-5860-46fd-baf3-e180a3742fac.png#averageHue=%23fbfbfb&clientId=u18bfedfc-f66f-4&from=paste&id=uddc64a1a&originHeight=647&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc4d541d3-35a7-4691-88f4-f8ea5061ecc&title=)
<a name="FAuIY"></a>
#### SpringApplication 启动时触发应用重启
spring-boot-devtools 通过 `RestartApplicationListener` 监听 SpringApplication 的启动，监听到启动时关闭当前线程，并重启应用，重启时使用自定义的 `RestartClassLoader` 加载应用类路径下的 class。监听 Spring Boot 应用启动的核心代码如下。
```java
public class RestartApplicationListener implements ApplicationListener<ApplicationEvent>, Ordered {
	@Override
	public void onApplicationEvent(ApplicationEvent event) {
		if (event instanceof ApplicationStartingEvent) {
			onApplicationStartingEvent((ApplicationStartingEvent) event);
		}
		... 省略部分代码
		}

	private void onApplicationStartingEvent(ApplicationStartingEvent event) {
		String enabled = System.getProperty(ENABLED_PROPERTY);
		if (enabled == null || Boolean.parseBoolean(enabled)) {
			String[] args = event.getArgs();
			DefaultRestartInitializer initializer = new DefaultRestartInitializer();
			boolean restartOnInitialize = !AgentReloader.isActive();
			// 初始化 Restarter
			Restarter.initialize(args, false, initializer, restartOnInitialize);
		} else {
			Restarter.disable();
		}
	}
}
```
`RestartApplicationListener` 监听到 SpringApplication 启动事件后开始对 `Restarter` 进行初始化，`Restarter` 是重启应用的核心类，`Restarter` 初始化过程仅仅实例化自身并调用其初始化方法，初始化的核心代码如下。
```java
public class Restarter {
	protected void initialize(boolean restartOnInitialize) {
		preInitializeLeakyClasses();
		if (this.initialUrls != null) {
			this.urls.addAll(Arrays.asList(this.initialUrls));
			if (restartOnInitialize) {
				this.logger.debug("Immediately restarting application");
				immediateRestart();
			}
		}
	}

	private void immediateRestart() {
		try {
			// 等待新线程执行结束
			getLeakSafeThread().callAndWait(() -> {
				start(FailureHandler.NONE);
				cleanupCaches();
				return null;
			});
		} catch (Exception ex) {
			this.logger.warn("Unable to initialize restarter", ex);
		}
		// 再通过抛出异常的方式退出主线程
		SilentExitExceptionHandler.exitCurrentThread();
	}
}
```
`Restarter` 首先收集类路径的 URL，然后立即调用 `#immediateRestart` 方法重启应用，待新线程重启应用后再通过抛出异常的方式关闭 main 线程。启动应用的核心代码如下。
```java
public class Restarter {
	private Throwable doStart() throws Exception {
		Assert.notNull(this.mainClassName, "Unable to find the main class to restart");
		URL[] urls = this.urls.toArray(new URL[0]);
		ClassLoaderFiles updatedFiles = new ClassLoaderFiles(this.classLoaderFiles);
		// 使用新的类加载器加载变化的类
		ClassLoader classLoader = new RestartClassLoader(this.applicationClassLoader, urls, updatedFiles, this.logger);
		if (this.logger.isDebugEnabled()) {
			this.logger.debug("Starting application " + this.mainClassName + " with URLs " + Arrays.asList(urls));
		}
		return relaunch(classLoader);
	}
}
```
`Restarter` 先根据类路径下 URL 收集文件系统中的 class 文件到 `ClassLoaderFiles`，然后使用新的类加载器 `RestartClassLoader` 对应用重启，剩下的就很简单了，直接调用 main 方法即可。
<a name="wtOvY"></a>
#### 类路径 class 文件变化时触发应用重启
除了首次应用启动时切换 `ClassLoader` 重启应用，对开发者而言，最重要的就是 class 文件发生变化时重启应用了。自动配置类位于 `LocalDevToolsAutoConfiguration.RestartConfiguration`，spring-boot-devtools 提供了一个 `ClassPathFileSystemWatcher` bean 用于监听 class 文件的变化。
```java
@Configuration(proxyBeanMethods = false)
@ConditionalOnInitializedRestarter
@EnableConfigurationProperties(DevToolsProperties.class)
public class LocalDevToolsAutoConfiguration {

	@Lazy(false)
	@Configuration(proxyBeanMethods = false)
	@ConditionalOnProperty(prefix = "spring.devtools.restart", name = "enabled", matchIfMissing = true)
	static class RestartConfiguration {

		@Bean
		@ConditionalOnMissingBean
		ClassPathFileSystemWatcher classPathFileSystemWatcher(FileSystemWatcherFactory fileSystemWatcherFactory,
															  ClassPathRestartStrategy classPathRestartStrategy) {
			URL[] urls = Restarter.getInstance().getInitialUrls();
			ClassPathFileSystemWatcher watcher = new ClassPathFileSystemWatcher(fileSystemWatcherFactory,
																				classPathRestartStrategy, urls);
			watcher.setStopWatcherOnRestart(true);
			return watcher;
		}
	}
}
```
`ClassPathFileSystemWatcher` 实现了 `InitializingBean` 接口，会在初始化时启动一个线程监听 class 文件的变化，然后发送一个 `ClassPathChangedEvent` 事件，因此 spring-boot-devtools 还提供了一个对应的监听器。
```java
@Configuration(proxyBeanMethods = false)
@ConditionalOnInitializedRestarter
@EnableConfigurationProperties(DevToolsProperties.class)
public class LocalDevToolsAutoConfiguration {

	@Lazy(false)
	@Configuration(proxyBeanMethods = false)
	@ConditionalOnProperty(prefix = "spring.devtools.restart", name = "enabled", matchIfMissing = true)
	static class RestartConfiguration {

		@Bean
		ApplicationListener<ClassPathChangedEvent> restartingClassPathChangedEventListener(
			FileSystemWatcherFactory fileSystemWatcherFactory) {
			return (event) -> {
				if (event.isRestartRequired()) {
					// 类路径发生变化时重启应用上下文
					Restarter.getInstance().restart(new FileWatchingFailureHandler(fileSystemWatcherFactory));
				}
			};
		}
	}
}
```
监听器监听到 class 文件变化后通过 Restarter 再次重启了应用，流程与首次重启时类似，不再赘述。
<a name="edC2X"></a>
## JRebel
除了 spring-boot-devtools，Spring 官方推荐的另一个热部署工具是 JRebel。JRebel 的核心是一个普通的 jar 包，内置了对多种框架的支持，通过 java -jar 启动时指定 -javaagent 即可使用 JRebel，而无需修改代码。同时 JRebel 也提供了多种的 IDE 插件，避免了手动启动指定 agent。
<a name="LbaMn"></a>
### JRebel 在 Idea 中的使用
由于目前大家多使用 Idea 作为 IDE，因此这里介绍下 JRebel 在 Idea 中的使用。
<a name="SgUQZ"></a>
#### 1、下载
首先在 Idea 的插件市场搜索 JRebel，并选择 JRebel and XRebel，然后 install，之后重启 IDE 使插件生效。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661999881039-ce34662e-b13d-44cb-b6e0-6bc9ea4a260c.png#averageHue=%23cae09c&clientId=u69e0b21a-93d8-4&from=paste&id=ua28356a8&originHeight=779&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6f87f31b-5669-4342-b045-98a399d1c39&title=)
<a name="Lw7qt"></a>
#### 2、激活
然后点击 Help->JRebel->Activation 进入激活页面。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661999901747-f237aa79-122c-482f-8850-3f05c44ca438.png#averageHue=%23efefef&clientId=u69e0b21a-93d8-4&from=paste&id=u0dbab0fb&originHeight=1142&originWidth=1020&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u773519a9-fa6e-498a-a192-7d59d0436a2&title=)<br />选择 Team URL，在 [https://jrebel.qekang.com/](https://jrebel.qekang.com/) 网站可以查找 可用的 Team URL，然后输入任意邮箱即可激活。
<a name="QVE5U"></a>
#### 3、项目支持配置
选择 View->Tool Windows->JRebel 对项目进行配置。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661999925102-939551f2-c53c-467b-af31-c52ed0ddaf92.png#averageHue=%23f8f8f7&clientId=u69e0b21a-93d8-4&from=paste&id=u62b7e6cd&originHeight=609&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3e1c062b-6617-4640-8a9b-42b4757a75f&title=)<br />勾选项目名称右侧的第一个复选框即可快速开启 JRebel 对项目的支持。此时将在 resources 目录下生成一个 rebel.xml 文件，这个文件用于配置 JRebel 监听的类路径。
<a name="pcMEE"></a>
#### 4、自动编译配置
访问 Setting，在 Compiler 页面下勾选 Build project automatically 开启自动构建功能。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662000009738-31ab6123-71c7-4ed1-ba90-5d76bbba532b.png#averageHue=%23ea5841&clientId=u69e0b21a-93d8-4&from=paste&id=u4703c830&originHeight=779&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uce182986-60f9-459b-af6c-2f6d141477c&title=)<br />访问 Setting 页面，在 System Settings 页面下勾选 Save file if the IDE is idle for。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662000031499-597acbbf-f885-4ac0-845c-acbf9e2924d2.png#averageHue=%23ea5740&clientId=u69e0b21a-93d8-4&from=paste&id=ud5e07b17&originHeight=779&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7ab70239-e975-48d8-bd72-5a4c76eeb4e&title=)
<a name="wYM4R"></a>
#### 5、启动项目
然后使用 JRebel 进行 debug 就可以啦，当代码变更触发 IDE 构建后，JRebel 会自动使用新的 class 代码。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662000048599-b3541784-604a-44d9-a210-8f4a4cacc13b.png#averageHue=%23f1f1ef&clientId=u69e0b21a-93d8-4&from=paste&id=u245daf15&originHeight=626&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub4def570-cdf2-41e5-ab04-a882bd0cb04&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662000048454-1ac0313a-6b31-41ac-ab2f-1cacd3adefb9.png#averageHue=%23efeeed&clientId=u69e0b21a-93d8-4&from=paste&id=u73c2206e&originHeight=162&originWidth=882&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1fa8c6d9-de20-4dfd-b820-16b7e669021&title=)
<a name="nMLHq"></a>
### JRebel 实现原理
虽然 JRebel 在 Idea 中的使用方式比较简单，但当试图探究其实现方式时却发现并没有那么容易。网上的文章前篇一律介绍的是其使用方式，即便其官网也只是简简单单概述为：JRebel 主要在 ClassLoader 级别与 JVM 及应用集成。它不会创建新的 ClassLoader，当监测到 class 文件发生变化时通过扩展类加载器更新应用。<br />从官网的描述来看，也并没有深入到具体的实现方式上，真是 听君一席话，如听一席话，由于 JRebel 并未开源，并且其提供的 .jar 文件也进行了代码混淆，因此这里只能对其实现方式进行推测，并逐步验证。这里将推测及分析过程分享给大家。<br />推测：JRebel 通过 Java Agent 进行实现JRebel 核心为一个普通的 jar 包，并通过 -javaagent 指定这个 jar 包，因此可以猜测它使用到了 Java Agent 的某些特性。<br />Java Agent 的主要作用为替换加载的 class，运行时修改方法体。由于 JRebel 支持在运行时添加、删除方法，因此 JRebel 必然不是通过运行时修改已加载到 JVM 的类路径下 class 方法体的方式来实现热部署的。那么大概率 JRebel 是修改了某些加载到 JVM 的 class。<br />推测：JRebel 会在 class 文件发生变化后重新加载 class 文件推测 JRebel 使用了 Java Agent 之后，还是不能了解其主要实现方式，不过当 class 文件发生变动后，JRebel 必然会重新加载变动后的 class 文件，以便执行新的代码，因此可以在 ClassLoader 加载类的某个流程上打上断点，以便查看堆栈信息。<br />Spring Boot 项目的示例代码如下。
```java
@SpringBootApplication
@RestController
public class DemoApplication {
	public static void main(String[] args) {
		SpringApplication.run(DemoApplication.class, args);
	}

	@GetMapping("/hello")
	public String hello() {
		return "hallo";
	}
}
```
不过呢，由于误将 hello 拼成了 hallo，因此需要修改代码，改动后成功进入了断点。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662000090290-3d01d98d-07cb-422a-9ec5-59d674ed7bcf.png#averageHue=%23f7f7f0&clientId=u69e0b21a-93d8-4&from=paste&id=ub16f0119&originHeight=543&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u78b717fe-7f5e-48c8-8aa9-0772f81df05&title=)<br />JRebel 在 rebel-change-detector-thread 线程监测 class 文件的变动，文件变动后使用 AppClassLoader 加载了 com.zzuhkp.DemoApplication 开头的类，并且类名后还带了 $$M$_jr_ 开头的后缀。可以想到的是同一个 ClassLoader 只能加载一个类，因此 JRebel 对类名进行了修改。这也是官网所描述的，不创建新的 ClassLoader，当 class 发生变化时更新应用。<br />问题：JRebel 如何替换新的 class 的？JRebel 加载新的 class 后必然会实例化，然后替换旧的对象，那么它是怎么实例化的呢？有多个构造方法时又该如何选择？<br />修改示例代码如下。
```java
@SpringBootApplication
@RestController
public class DemoApplication {
	private String str;

	public DemoApplication() {
		this.str = "你好";
	}

	public DemoApplication(String str) {
		this.str = "你好呀";
	}

	public static void main(String[] args) {
		SpringApplication.run(DemoApplication.class, args);
	}

	@GetMapping("/hello")
	public String hello() {
		return str;
	}
}
```
这里添加了一个成员变量和两个构造方法，访问 /hello 接口，发现返回 你好 二字，可以看出 JRebel 会自动使用无参的构造方法实例化对象。<br />那么 JRebel 又是怎么替换旧对象的呢？对于注解驱动的 Spring MVC，Controller 方法会被当做一个 handler 处理请求，如果新添加一个 Controller 方法，那么它必然被注册为 handler 才能处理请求。添加一个 hello2 的方法，并在注册 handler 的流程上打断点。
```java
@GetMapping("/hello2")
public String hello2() {
   return str;
}
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662000108231-48786962-53f6-4162-8e90-2393cbb7cd21.png#averageHue=%23f5f5ec&clientId=u69e0b21a-93d8-4&from=paste&id=u8410191a&originHeight=617&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7febae76-9fe1-45de-8b32-ae9a90d85d6&title=)<br />当访问 /hello2 果然进入了断点。从堆栈信息来看，多出了 JRebel 的相关类，可以断定，JRebel 对 Spring 的某些 class 做出了修改，当 class 发生变动后，JRebel 自动使用新的 class 实例化的对象注册到 Spring 内部。
<a name="PKedL"></a>
### JRebel 小结
从上述推测和验证的过程来看，JRebel 对热部署的支持利用 Java Agent 修改了 Spring 的某些 class，当应用的 class 发生变化时，JRebel 自动加载新的 class 文件，并利用 Spring 的API 替换 Spring 中的旧对象，从而支持了 Spring 的热部署。
<a name="tqyWR"></a>
## 总结
由于 spring-boot-devtools 会引入新的依赖，并且 class 文件变更会引起应用重启，而 JRebel 只会加载变动的 class 并利用 Spring 的 API 替换新的对象，因此 JRebel 比 spring-boot-devtools 会快上不少，相对来说比较个人比较支持使用 JRebel。
