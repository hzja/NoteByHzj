Maven<br />**Maven**经常被拿来和**Gradle**做对比，最大的劣势之一就是**Maven**构建慢，**Gradle**比**Maven**构建速度快2到10倍，而如今**Maven**也可以更快了。**Apache Maven**团队从**Gradle**和[**Takari(Maven生命周期优化器)**](http://takari.io/book/40-lifecycle.html)中获得灵感对**Maven**进行了强化，衍生出** **[**maven-mvnd**](https://github.com/apache/maven-mvnd) 项目。
<a name="xcWND"></a>
## 简单介绍
**mvnd**并不是对**Maven**进行了重构，而是内置了**Maven**。它实际上是一个或者多个Maven守护进程，用来执行实际的构建服务。一个守护进程实例可以为来自**mvnd**客户端的多个连续请求提供**Maven**构建服务。当没有空闲的守护进程时来支撑构建请求时，**mvnd**可以并行生成多个守护程序。
<a name="K5zEP"></a>
### 使用GraalVM代替JVM
**mvnd**之所以快是因为它使用了**GraalVM**替代了传统的**JVM**，因此它启动更快，占用内存更少，而且在实行构建时不需要为每个构建启动新的**JVM**。另外**Maven**插件在构建时不再需要多次加载，而是缓存在多个构建中。**SNAPSHOT**版本的Maven插件不会被缓存。
<a name="lKBYn"></a>
### 支持JIT
**GraalVM**的**JIT**(Just In Time)实时编译特性也被运用到**Maven**构建作业中。**JIT**可以大大降低编译时间，在重复构建过程中，**JIT**优化代码立即可用，也极大提高了构建效率。
<a name="P1Xm0"></a>
### 并行构建
目前**Maven 3**的并行构建（parallel builds）还是实验特性，而且要求使用的插件必须是线程安全的。而**mvnd**默认就支持使用多个CPU内核进行并行构建。当然如果源代码不支持并行构建，也可以通过添加`- T1` 参数切换到串行构建。
<a name="JdWdZ"></a>
## 简单尝试
接下来简单尝试一下**mvnd**。
<a name="OnBXR"></a>
### 安装
目前**mvnd**已经支持**Linux**、**MacOS**、**Windows**平台。可以分别通过**SDKMAN**、**Homebrew**、**Chocolatey**这些包管理器进行安装。也可以直接去[**mvnd发行版仓库**](https://github.com/mvndaemon/mvnd/releases)下载对应的二进制进行安装。这里以**Windows**为例，这里选择**Chocolatey**安装：
```bash
choco install mvndaemon
```
时间可能比较长一些。
<a name="LiWM3"></a>
### 配置
配置很简单，只需要注意两点

- 如果不使用包管理器进行安装的话请确保mvnd的`bin`目录关联到PATH。
- 如果没有配置或者不想配置`JAVA_HOME`环境变量的话请在`~/.m2/mvnd.properties`中设置`java.home`以指定Java目录，例如：`java.home=C:\\AdoptOpenJDK\\jdk-8`。已经配置环境变量的请忽略该条。

其它的配置项可参考[mvnd配置](https://github.com/mvndaemon/mvnd/blob/master/dist/src/main/distro/conf/mvnd.properties)。
<a name="jZFhd"></a>
### 使用
安装配置完毕后命令行执行`mvnd --version`出现下列结果就表示安装成功了。
```bash
mvnd --version
mvnd native client 0.7.1-windows-amd64 (97c587c11383a67b5bd0ff8388bd94c694b91c1e)
Terminal: org.jline.terminal.impl.jansi.win.JansiWinSysTerminal
Apache Maven 3.8.3 (ff8e977a158738155dc465c6a97ffaf31982d739)
Maven home: C:\Program Files\mvndaemon\mvnd-0.7.1-windows-amd64\mvn
Java version: 1.8.0_282, vendor: AdoptOpenJDK, runtime: C:\Program Files\AdoptOpenJDK\jdk-8.0.282.8-hotspot\jre
Default locale: zh_CN, platform encoding: GBK
OS name: "windows 10", version: "10.0", arch: "amd64", family: "windows"
```
为了降低学习成本**mvnd**的方式和传统**Maven**类似，mvn变成了mvnd，也可以通过`mvnd --help`来查看完整的命令选项清单。<br />说实话并行构建就是快，简直坐火箭一般。另外，控制台输出也优化了。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1640261945729-2af9980e-eb1a-47ed-9494-baab1b841aa4.gif#clientId=u24d4d022-6279-4&from=paste&id=ue52b67ba&originHeight=412&originWidth=763&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u46f3522e-fd60-48f1-b27b-b3fa1f7cd25&title=)<br />由于**GraalVM**的加持，这个工具很可能会延长**Maven**的生命，避免过早被**Gradle**拉开优势。这个项目可以观望一下，看看**Apache Maven**官方的进一步动作。
<a name="JZVm7"></a>
### 参考资料
Takari(Maven生命周期优化器): [http://takari.io/book/40-lifecycle.html](http://takari.io/book/40-lifecycle.html)<br />maven-mvnd: [https://github.com/apache/maven-mvnd](https://github.com/apache/maven-mvnd)<br />mvnd发行版仓库: [https://github.com/mvndaemon/mvnd/releases](https://github.com/mvndaemon/mvnd/releases)<br />mvnd配置: [https://github.com/mvndaemon/mvnd/blob/master/dist/src/main/distro/conf/mvnd.properties](https://github.com/mvndaemon/mvnd/blob/master/dist/src/main/distro/conf/mvnd.properties)
