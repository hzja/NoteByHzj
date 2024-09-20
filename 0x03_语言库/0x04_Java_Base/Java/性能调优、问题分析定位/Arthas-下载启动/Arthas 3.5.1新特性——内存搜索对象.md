Java Arthas<br />Arthas是Alibaba开源的Java诊断工具，深受开发者喜爱。

- Github：[https://github.com/alibaba/arthas](https://github.com/alibaba/arthas)
- 文档：[https://arthas.aliyun.com/doc/](https://arthas.aliyun.com/doc/)

以前使用`watch`等命令时，通常要先知道哪个类，调用了哪个函数，然后触发调用。这样有局限：

1. 线上触发调用比较难
2. 要`watch`到正确的函数可能要选择多次
3. 条件表达式/结果表达式 可能需要多次测试

另外，如果想要查找内存里的对象，需要`heapdump`再分析。<br />Arthas在最新发布的 3.5.1 版本里，带来神级特性：通过`vmtool`命令，可以在JVM内存搜索对象。
<a name="zYROt"></a>
### vmtool在线教程
下面以`vmtool`在线教程为例，演示`vmtool`命令的功能：

- [https://arthas.aliyun.com/doc/arthas-tutorials.html?language=cn&id=command-vmtool](https://arthas.aliyun.com/doc/arthas-tutorials.html?language=cn&id=command-vmtool)

首先启动任意SpringBoot应用，比如：
```bash
wget https://github.com/hengyunabc/spring-boot-inside/raw/master/demo-arthas-spring-boot/demo-arthas-spring-boot.jar
java -jar demo-arthas-spring-boot.jar
```
然后用arthas attach目标进程，成功之后就可以使用`vmtool`命令了：
```bash
wget https://arthas.aliyun.com/arthas-boot.jar
java -jar arthas-boot.jar
```
<a name="F6san"></a>
### 查找jvm里的字符串对象
首先，`vmtool`命令通过`getInstances`这个action，在JVM里搜索字符串：
```bash
$ vmtool --action getInstances --className java.lang.String
@String[][
    @String[Sorry, deque too big],
    @String[head=%d tail=%d capacity=%d%n],
    @String[elements=%s%n],
    @String[sun/nio/ch/IOVecWrapper],
    @String[40252e37-8a73-4960-807e-3495addd5b08:1620922383791],
    @String[40252e37-8a73-4960-807e-3495addd5b08:1620922383791],
    @String[sun/nio/ch/AllocatedNativeObject],
    @String[sun/nio/ch/NativeObject],
    @String[sun/nio/ch/IOVecWrapper$Deallocator],
    @String[Java_sun_nio_ch_FileDispatcherImpl_writev0],
]
```
<a name="y3TKu"></a>
### limit参数
通过`--limit`参数，可以限制返回值数量，避免获取超大数据时对JVM造成压力。默认值是10。<br />所以上面的命令实际上等值于：
```bash
vmtool --action getInstances --className java.lang.String --limit 10
```
如果设置`--limit`为负数，则遍历所有对象。
<a name="TM0sy"></a>
### 查找spring context
以前的在线教程里，需要通过`tt`命令来拦载Spring调用，然后获取到spring context。<br />通过`vmtool`命令，可以直接获取到srping context：
```bash
$ vmtool --action getInstances \
--className org.springframework.context.ApplicationContext
@ApplicationContext[][
    @AnnotationConfigEmbeddedWebApplicationContext[org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@12028586: startup date [Thu May 13 16:08:38 UTC 2021]; root of context hierarchy],
]
```
<a name="bNsnn"></a>
### 指定返回结果展开层数
`getInstances` action返回结果绑定到`instances`变量上，它是数组。<br />通过`-x`/`--expand`参数可以指定结果的展开层次，默认值是1。
```bash
vmtool --action getInstances --className org.springframework.context.ApplicationContext -x 2
```
<a name="dKDpG"></a>
### 获取srping bean，执行表达式
`getInstances` action返回结果绑定到`instances`变量上，它是数组。可以通过`--express`参数执行指定的表达式。<br />比如，查找所有的Spring beans名字：
```bash
vmtool --action getInstances \
--className org.springframework.context.ApplicationContext \
--express 'instances[0].getBeanDefinitionNames()'
```
比如，调用`userController.findUserById(1)`函数：
```bash
$ vmtool --action getInstances \
--className org.springframework.context.ApplicationContext \
--express 'instances[0].getBean("userController").findUserById(1)'
@User[
    id=@Integer[1],
    name=@String[name1],
]
```
<a name="OVoZY"></a>
### 查找所有的Spring mapping对象
`vmtool --action getInstances --className org.springframework.web.servlet.HandlerMapping`
```bash
$ vmtool --action getInstances --className org.springframework.web.servlet.HandlerMapping
@HandlerMapping[][
    @SimpleUrlHandlerMapping[org.springframework.web.servlet.handler.SimpleUrlHandlerMapping@5d3819c8],
    @EmptyHandlerMapping[org.springframework.web.servlet.config.annotation.WebMvcConfigurationSupport$EmptyHandlerMapping@11d509ba],
    @RequestMappingHandlerMapping[org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping@56a5f2e3],
    @WelcomePageHandlerMapping[org.springframework.boot.autoconfigure.web.WebMvcAutoConfiguration$WelcomePageHandlerMapping@4c0a4ed3],
    @EmptyHandlerMapping[org.springframework.web.servlet.config.annotation.WebMvcConfigurationSupport$EmptyHandlerMapping@51e1f8c3],
    @BeanNameUrlHandlerMapping[org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping@68c0a39c],
    @SimpleUrlHandlerMapping[org.springframework.web.servlet.handler.SimpleUrlHandlerMapping@110b768d],
]
```
<a name="Ad6d5"></a>
### 查找所有的 javax.servlet.Filter
在Arthas的在线教程里，介绍过怎么排查http请求 404/401 的问题。使用的是`trace javax.servlet.Filter *`命令。<br />现在使用`vmtool`命令，可以直接查找出所有的Filter对象，加速定位过程。
```bash
$ vmtool --action getInstances --className javax.servlet.Filter
@Filter[][
    @OrderedCharacterEncodingFilter[org.springframework.boot.web.filter.OrderedCharacterEncodingFilter@49b69493],
    @OrderedHiddenHttpMethodFilter[org.springframework.boot.web.filter.OrderedHiddenHttpMethodFilter@5477cb9e],
    @AdminFilter[com.example.demo.arthas.AdminFilterConfig$AdminFilter@3b625385],
    @WsFilter[org.apache.tomcat.websocket.server.WsFilter@14875f22],
    @OrderedRequestContextFilter[org.springframework.boot.web.filter.OrderedRequestContextFilter@6bed550e],
    @OrderedHttpPutFormContentFilter[org.springframework.boot.web.filter.OrderedHttpPutFormContentFilter@3e538cba],
]
```
<a name="Vhc5S"></a>
### 指定 classloader name
在多classloader情况下，还可以指定classloader来查找对象：
```bash
vmtool --action getInstances \
 --classLoaderClass org.springframework.boot.loader.LaunchedURLClassLoader \
 --className org.springframework.context.ApplicationContext
```
<a name="iEUhd"></a>
### 指定 classloader hash
可以通过`sc`命令查找到加载class的 classloader。
```bash
$ sc -d org.springframework.context.ApplicationContext
 class-info        org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext
 code-source       file:/private/tmp/demo-arthas-spring-boot.jar!/BOOT-INF/lib/spring-boot-1.5.13.RELEASE.jar!/
 name              org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext
...
 class-loader      +-org.springframework.boot.loader.LaunchedURLClassLoader@19469ea2
                     +-sun.misc.Launcher$AppClassLoader@75b84c92
                       +-sun.misc.Launcher$ExtClassLoader@4f023edb
 classLoaderHash   19469ea2
```
然后用`-c`/`--classloader`参数指定：
```bash
vmtool --action getInstances \
-c 19469ea2 \
--className org.springframework.context.ApplicationContext
```
<a name="DDatV"></a>
### 强制GC
当启用`-XX:+DisableExplicitGC`的JVM参数之后，调用`System.Gc()`可能并不会触发GC行为。<br />`vmtool`里提供了强制GC的功能：
```bash
vmtool --action forceGc
```
如果应用配置了`-verbose:gc`参数，则可以在应用的标准输出里看到类似的日志：
```bash
[GC (JvmtiEnv ForceGarbageCollection)  25760K->17039K(349696K), 0.0015299 secs]
[Full GC (JvmtiEnv ForceGarbageCollection)  17039K->16840K(353792K), 0.0154049 secs]
```
