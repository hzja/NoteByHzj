Java 日志
<a name="C9pRF"></a>
## Log日志体系
在日常工作中可能看到项目中依赖的跟日志相关的jar包有很多，commons-logging.jar、log4j.jar、sl4j-api.jar、logback.jar等等，眼花缭乱。要正确的配置，使得jar包相互作用生效之前，就先要理清它们之间的关系。
<a name="UeXKs"></a>
### 背景/发展史
那就要从Java Log的发展历程开始说起。

1. log4j（作者Ceki Gülcü）出来时就等到了广泛的应用（注意这里是直接使用），是Java日志事实上的标准，并成为了Apache的项目
2. Apache要求把log4j并入到JDK，SUN拒绝，并在jdk1.4版本后增加了JUL（java.util.logging）
3. 毕竟是JDK自带的，JUL也有很多人用。同时还有其他日志组件，如SimpleLog等。这时如果有人想换成其他日志组件，如log4j换成JUL，因为api完全不同，就需要改动代码。
4. Apache见此，开发了JCL（Jakarta Commons Logging），即commons-logging-xx.jar。它只提供一套通用的日志接口api，并不提供日志的实现。很好的设计原则嘛，依赖抽象而非实现。这样应用程序可以在运行时选择自己想要的日志实现组件。
5. 这样看上去也挺美好的，但是log4j的作者觉得JCL不好用，自己开发出slf4j，它跟JCL类似，本身不替供日志具体实现，只对外提供接口或门面。目的就是为了替代JCL。同时，还开发出logback，一个比log4j拥有更高性能的组件，目的是为了替代log4j。
6. Apache参考了logback,并做了一系列优化，推出了log4j2
<a name="wPXQH"></a>
### 关系/依赖
大概了解心路历程后，再详细看看它们之间的关系、依赖。
<a name="DJQNQ"></a>
#### JCL
commons-logging已经停止更新，最后的状态如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623990125055-e558ae00-a192-4347-9171-c72d235c05e6.png#clientId=uc74396be-2275-4&from=paste&id=ub690fc3b&originHeight=646&originWidth=978&originalType=url&ratio=3&status=done&style=shadow&taskId=uadc7242b-011d-459c-9208-9cf8730a7e4)<br />JCL支持日志组件不多，不过也有很人用的，例如Spring<br />现在用的也越来越少了，也不多讲了
<a name="r06XY"></a>
#### SLF4J
因为当时Java的日志组件比较混乱繁杂，Ceki Gülcü推出slf4j后，也相应为行业中各个主流日志组件推出了slf4j的适配<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623990125047-431bd1a0-01f1-4783-bd56-865c36e25ca7.webp#clientId=uc74396be-2275-4&from=paste&id=ub1af7026&originHeight=596&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=ufe5f30c0-1d81-43a8-9992-ea625c39a69)<br />图的意思为如果想用slf4j作为日志门面的话，如何去配合使用其他日志实现组件，这里说明一下（注意jar包名缺少了版本号，在找版本时也要注意版本之间是否兼容）

- slf4j + logback<br />slf4j-api.jar + logback-classic.jar + logback-core.jar
- slf4j + log4j<br />slf4j-api.jar + slf4j-log4j12.jar + log4j.jar
- slf4j + jul<br />slf4j-api.jar + slf4j-jdk14.jar
- 也可以只用slf4j无日志实现<br />slf4j-api.jar + slf4j-nop.jar
<a name="PZAG1"></a>
### SLF4J的适配
slf4j支持各种适配，无论现在是用哪种日志组件，都可以通过slf4j的适配器来使用上slf4j。<br />只要切换到了slf4j，那么再通过slf4j用上实现组件，即上面说的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623990125071-6786042f-52ac-44a7-8e70-d64a0d911ea9.webp#clientId=uc74396be-2275-4&from=paste&id=uef3c1eb0&originHeight=736&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=u2f631a52-3600-45fc-a15e-9e7a7895e04)<br />其实总的来说，无非就是以下几种情况

- 在用JCL<br />使用jcl-over-slf4j.jar适配
- 在用log4j<br />使用log4j-over-slf4j.jar适配
- 在用JUL<br />使用jul-to-slf4j.jar适配

一个整体的依赖图<br />![2021-06-18-12-21-43-239104.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623990157523-7080f787-e535-4ba8-b997-e6679ce29772.png#clientId=uc74396be-2275-4&from=ui&id=u672f8db9&originHeight=811&originWidth=1080&originalType=binary&ratio=3&size=2632775&status=done&style=shadow&taskId=ub4ba157d-4df8-494f-abca-07a755fe4b4)
<a name="TH5Th"></a>
## 让Spring统一输出
这就是为了对slf4j的适配做一个例子说明。<br />Spring是用JCL作为日志门面的，项目中是slf4j + logback，怎么让Spring也用到logback作为日志输出呢？这样的好处就是可以统一项目内的其他模块、框架的日志输出（日志格式，日志文件，存放路径等，以及其他slf4j支持的功能）<br />很简单，就是加入jcl-over-slf4j.jar就好了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623990124998-e77d661e-b73a-4527-bbc0-41ef3b6fd979.webp#clientId=uc74396be-2275-4&from=paste&id=u658dd850&originHeight=892&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=ub753258c-782e-47c7-b753-4d9c5780c3d)
<a name="Z2uY2"></a>
### 适配思路
其实很简单

1. 首先确认需要统一日志的模块、框架是使用哪个日志组件的，然后再找到sfl4j的适配器。
2. 记得去掉无用的日志实现组件，只保留要用的。
<a name="yBgdn"></a>
## 常见问题
slf4j的日志加载会在程序启动时把日志打出来，所以一定要注意，它会说明加载是否成功，加载了那个日志实现。<br />slf4j已经对错误作了说明：<br />[http://www.slf4j.org/codes.html](http://www.slf4j.org/codes.html)<br />下面讲一下可能经常遇到的问题
<a name="NX96D"></a>
### Failed to load class org.slf4j.impl.StaticLoggerBinder
没找到日志实现，如果已经写上了对应的日志实现依赖了，要检查一下了，一般来说极有可能是版本不兼容。
<a name="cdmsn"></a>
### Multiple bindings
找到多个日志实现，slf4j会找其中一个作为日志实现。
<a name="QETIT"></a>
## 代码规范
阿里对此的代码规范：<br />【强制】应用中不可直接使用日志系统（Log4j、Logback）中的 API，而应依赖使用日志框架 SLF4J 中的 API，使用门面模式的日志框架，有利于维护和各个类的日志处理方式统一。
```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
private static final Logger logger = LoggerFactory.getLogger(Abc.class);
```
