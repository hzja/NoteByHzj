<a name="IrdUx"></a>
## 1、为什么升级JDK11
<a name="s1Sjk"></a>
### 1.1 性能提升
更好的垃圾收机制、更快的类加载器，加快应用程序的运行速度。综合评估，从Java 8 升级到 Java 11，G1GC平均速度提升16.1%，ParallelGC为4.5%（基于OptaPlanner的用例基准测试表明[https://www.optaplanner.org/blog/2019/01/17/HowMuchFasterIsJava11.html](https://www.optaplanner.org/blog/2019/01/17/HowMuchFasterIsJava11.html)）。
<a name="V8wbN"></a>
### 1.2 特性和改进
局部变类型推断、新的 API、HTTP/2客户端、Lambda表达式的新特性等，这些新特性可以提高开发效率。
<a name="SSO4B"></a>
### 1.3 支持最新的技术和框架
许多新的技术和框架已经或即将开始依赖于JDK11或以上版本，升级后可以保证应用程序能够分利用这些新的技术和框架。
<a name="P2sAj"></a>
### 1.4 长期支持版本
JDK11是Oracle官方发布的一个长期支持（LTS），意味着它将获得长期的更新和支持，有助于保持用程序的稳定性和可靠性。
<a name="dC34m"></a>
### 1.5 行业趋势
数据来自 New Relic （[https://newrelic.com/resources/report/2023-state-of-the-java-ecosystem](https://newrelic.com/resources/report/2023-state-of-the-java-ecosystem)）在2023年1月发布的Java生态报告，从下图可以看出：

1. 目前市面上有超过 56%的应用程序使用了JDK 11，Java 8 的使用从2020年的84%降低到了现在的32%左右。大部分公司在这三年之间都升级到了JDK 11 或者 JDK 17这两个LTS版本上面。
2. 垃圾收集器使用情况来看，JDK11版本及以上G1使用率最高，占比高达65%。

![图1.Java LTS版本百分比](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779315899-df59576e-b2d7-4d1b-906b-42a1f95d0d92.png#averageHue=%232fd4ac&clientId=ua36ad781-9a05-4&from=paste&id=u598f86b8&originHeight=799&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ucafd4c21-5a8c-4be7-8995-af7ac90759c&title=%E5%9B%BE1.Java%20LTS%E7%89%88%E6%9C%AC%E7%99%BE%E5%88%86%E6%AF%94 "图1.Java LTS版本百分比")<br />![图2.垃圾回收器使用百分比](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779315845-95f8a7cf-3428-49e7-abca-024a68457fe6.png#averageHue=%2326e084&clientId=ua36ad781-9a05-4&from=paste&id=boRvV&originHeight=1215&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ubd43d2dd-50a4-42d3-9e9d-420e43e82f8&title=%E5%9B%BE2.%E5%9E%83%E5%9C%BE%E5%9B%9E%E6%94%B6%E5%99%A8%E4%BD%BF%E7%94%A8%E7%99%BE%E5%88%86%E6%AF%94 "图2.垃圾回收器使用百分比")
<a name="QEMqw"></a>
## 2、升级后GC效果
先给出结论：<br />1、JDK11相对于JDK8，所有垃圾回收器的性能都有提升，特别是大内存机器下G1的提升最明显；<br />2、8G内存以下的机器，推荐使用Parallel GC，如果特别追求低延迟，选择牺牲吞吐量，可以使用G1，并设置期望的最大垃圾回收停顿时间来控制；<br />3、8G及以上的大内存机器，推荐使用G1 <br />4、不推荐使用CMS，升级后从各项数据来看，CMS收集器都不如G1。<br />在JDOS平台上选择了不同配置的机器（2C4G、4C8G、8C16G），并分别使用JDK8和JDK11进行部署和压测。<br />整个压测过程限时60分钟，用180个虚拟用户并发请求一个接口，每次接口请求都创建512Kb的数据。最终产出不同GC回收器的各项指标数据，来分析GC的性能提升效果。<br />**以下是压测的性能情况：**

| **机器配置** | **垃圾回收器** | **指标项** | **JDK8** | **JDK11** | **JDK11比JDK8提升** | **总结** |
| --- | --- | --- | --- | --- | --- | --- |
| 2C4G | Parallel GC（标记复制+标记整理） | 吞吐量 | 88.805% | 92.821% | 4% | 1、JDK11各项指标都有提升 2、当前机器配置下，综合评估，Parallel GC的综合指标比G1高 |
|  |  | 平均停顿GC时间 | 28.3ms | 19.6ms | 30% |  |
|  |  | 最大停顿GC时间 | 720ms | 720ms | 0 |  |
|  | CMS（标记复制+标记清除） | 吞吐量 | 58.551% | 63.923% | 5% |  |
|  |  | 平均停顿GC时间 | 28.0ms | 26.5ms | 7% |  |
|  |  | 最大停顿GC时间 | 300ms | 250ms | 16% |  |
|  | G1收集器 | 吞吐量 | 83.046% | 68.371% | -15% |  |
|  |  | 平均停顿GC时间 | 125ms | 49.9ms | 60% |  |
|  |  | 最大停顿GC时间 | 1170ms | 610ms | 47% |  |
| 4C8G | Parallel GC（标记复制+标记整理） | 吞吐量 | 90.851% | 95.252% | 5% | 1、JDK11各项指标都有明显提升 2、当前机器配置下，综合评估，G1的综合指标比Parallel GC高 |
|  |  | 平均停顿GC时间 | 27.1ms | 15.3ms | 43% |  |
|  |  | 最大停顿GC时间 | 580ms | 680ms | -17% |  |
|  | CMS（标记复制+标记清除） | 吞吐量 | 49.812% | 56.55% | 7% |  |
|  |  | 平均停顿GC时间 | 38.3ms | 32.3ms | 15% |  |
|  |  | 最大停顿GC时间 | 180ms | 150ms | 16% |  |
|  | G1收集器 | 吞吐量 | 96.333% | 97.328% | 1% |  |
|  |  | 平均停顿GC时间 | 18.4ms | 18.7ms | 0.01% |  |
|  |  | 最大停顿GC时间 | 980ms | 190ms | 80% |  |
| 8C16G | Parallel GC（标记复制+标记整理） | 吞吐量 | 90.114% | 94.718% | 4% | 1、JDK11各项指标都有明显提升 2、当前机器配置，综合评估，大内存机器，G1的综合指标比Parallel GC高很多 |
|  |  | 平均停顿GC时间 | 30.8ms | 16.8ms | 46% |  |
|  |  | 最大停顿GC时间 | 940ms | 770ms | 18% |  |
|  | CMS（标记复制+标记清除） | 吞吐量 | 53.893% | 60.168% | 7% |  |
|  |  | 平均停顿GC时间 | 32.2ms | 27.2ms | 15% |  |
|  |  | 最大停顿GC时间 | 260ms | 100ms | 61% |  |
|  | G1收集器 | 吞吐量 | 96.359% | 97.143% | 1% |  |
|  |  | 平均停顿GC时间 | 20.1ms | 17.3ms | 14% |  |
|  |  | 最大停顿GC时间 | 260ms | 120ms | 53% |  |

* 上面给出的GC升级效果，采用的是默认的配置，没有做任何优化，只提供参考。真正的GC调优是个技术活，需要根据业务需求、机器配置和实际压测效果等综合评估来选出最合适的GC垃圾回收器。<br />* 不同垃圾回收器的特点：

1. Parallel GC - JDK 8及以下版本的默认收集器，关注吞吐量，尝试在最小延迟的情况下尽快完成工作并提高吞吐量。
2. CMS - 一个老年代收集器，基于标记-清除算法实现，关注延迟，以最短回收停顿时间为目标。
3. Garbage First（G1）- JDK 9以后的默认收集器，G1 关注总体的性能，会尝试在吞吐量和延迟之间做平衡。
<a name="kK2c8"></a>
## 3、JDK11带来了哪些新特性
<a name="UeJ7Q"></a>
### 3.1 GC改进
**默认垃圾回收器改为G1，废弃CMS垃圾回收器**

- G1特点：目标是降低应用程序的停顿时间并提高吞吐量。

**引入ZGC垃圾回收器（可伸缩低延迟垃圾收集器）；**但由于JDK11中ZGC还不够完善，推荐在JDK17中再使用稳定版ZGC

- Full GC的停顿不超过10毫秒
- 支持TB级堆内存回收
- 相对于G1吞吐量下降不超过15%
<a name="OyVMC"></a>
### 3.2 模块化
**Java9引入了对于模块化软件支持，而Java11进一步扩展了这种特性。模块化让应用程序更精简，减少对其他类库的依赖和冗余代码，提高运行效率和安全性。**<br />然而，目前不推荐使用模块化，因为相关组件生态还不完善，并且模块化带来的价值不够突出。具体原因请看后面章节的详细分析：新特性实践-模块化。
<a name="qLYqW"></a>
### 3.3 语法增强

- 局部变量推断，引入var局部变量类型，允许开发人员省略通常不必要的局部变量类型初始化声明

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779315783-fc2b1f7f-ab07-412f-9d98-95b810acd755.png#averageHue=%23212226&clientId=ua36ad781-9a05-4&from=paste&id=u899a7ad9&originHeight=332&originWidth=832&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1ae50837-08c1-4517-9644-e09a139f5b5&title=)

- Lambda表达式简化，内部可以使用var

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779315793-18ca1900-0431-4c76-977b-e4afd5877084.png#averageHue=%23222326&clientId=ua36ad781-9a05-4&from=paste&id=ud67a059f&originHeight=282&originWidth=830&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uff2b3d63-0d9e-49d8-8bac-e4055bd46e5&title=)

- 接口中可以定义私有方法，可以实现接口方法的访问控制和代码复用

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779315845-8cabc361-9671-467f-bbea-54612d8b60a3.png#averageHue=%23202124&clientId=ua36ad781-9a05-4&from=paste&id=u784d0339&originHeight=388&originWidth=860&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u041f8170-99f4-4947-a5f5-9af12bc9ac5&title=)
<a name="lkM9O"></a>
### 3.4 API增强

- **HTTPClient标准化支持**：强大而灵活的HTTP客户端API，支持多协议（HTTP/2、WebSocket）、异步非阻塞、流操作和连接池等特性。ps：再也不需要用第三包 HttpClient 工具包
- **字符串方法增强**：`isBlank`、`lines`、`strip`、`stripLeading`、`stripTrailing`和`repeat`
- **Files增强**：`readString`、`WriteString`
- **InputStream增强**：`transferTo`（流快速拷贝）
- **stream增强**，`dropWhile`（从集合中删除满足的）、`takeWhile`（从集合中获取满足的）、`ofNullable`
- **集合工厂方法**：`Sets.of()`、`List.of()`、`Map.of()`、`Map.ofEntries()`，举例：`List<String> list = List.of("Java", "Python", "C++")`
<a name="rLprC"></a>
## 4、如何升级  
<a name="JnRjW"></a>
### 4.1 升级应用评估

- 为保证稳定性，优先在新业务新应用来落地实施JDK11的升级。
<a name="LNPFD"></a>
### 4.2 JDK选择
自从2019年1月起，Oracle JDK后续的版本开始商用收费，所以推荐大家选择OpenJDK11，OpenJDK和OracleJDK功能上没有差异，支持免费商用。<br />OpenJDK11下载地址：[https://jdk.java.net/archive/](https://jdk.java.net/archive/)
<a name="ae9yV"></a>
### 4.3 GC配置
根据自身需求和机器配置选择GC，不同GC的JVM启动参数配置：

- **G1垃圾回收器**（JDK11默认，不需要手动配置）：`-XX:+UseG1GC`
- **Parallel GC垃圾回收器**：XX:+UseParallelGC
<a name="xFySW"></a>
### 4.4 升级过程踩坑
整个升级过程还是比较简单的，除了升级JDK版本，实际遇到的问题如下：

| 分类 | 依赖名 | 支持情况 | 说明 |
| --- | --- | --- | --- |
| 框架 | Spring2.X/boot | 支持 | 使用JDK11自带原生HttpClient时，会遇到：1、spring启动时，会遇到注入某些类时，无法通过反射的方式访问其所在的包，报错：module java.net.http does not"opens jdk.internal.net.http"to unnamed module @5eb2172<br />**原因**：模块化引入了包之间的访问权限控制，如果没有对一个包显示地使用open/opens关键字对外开放，那么其他包中的类无法通过反射的方式访问此包。<br />**解决方案**：需要手动设置JVM参数，比如：`--add-opens java.net.http/jdk.internal.net.http=ALL-UNNAMED` |
| 中间件 | JSF | 支持 | 

 |
|  | AKS | 支持 | 出现异常Causedby: java.lang.NoClassDefFoundError: javax/xml/bind/JAXBException<br />**原因**：Java11 删除了 Java EE modules，其中就包括 java.xml.bind (JAXB)。<br />**解决方案**：手动引入包即可 <!-- API, java.xml.bind module --> <dependency>      <groupId>jakarta.xml.bind</groupId>      <artifactId>jakarta.xml.bind-api</artifactId>      <version>2.3.2</version> </dependency> <!-- Runtime, com.sun.xml.bind module --> <dependency>       <groupId>org.glassfish.jaxb</groupId>       <artifactId>jaxb-runtime</artifactId>       <version>2.3.2</version> </dependency> |
|  | Mybatis | 支持 | 

 |
|  | Concrete | 支持 | 

 |
|  | R2M | 支持 | 

 |
|  | EasyJob | 支持 | 

 |
|  | OSS | 支持 | 

 |
|  | FMQ | 支持 | 

 |
|  | 

 | 

 | 

 |
| 监控运维 | SGM | 支持 | 

 |
|  | UMP | 支持 | 

 |
|  | UWC | 支持 | 

 |
| CICD | JDOS部署 | 支持 | **JDK11镜像**：java-jdt-centos7.4-jdk1.11.0_13-tomcat9.0.54:latest |

<a name="C03wr"></a>
### 4.5 升级后验证
升级后完成，做好单测和回归测试，推荐能做个压测验证，防止影响线上服务稳定性。
<a name="jWrj6"></a>
## 5、新特性实践-模块化  
Java一直是构建大型应用程序的主流语言之一。然而随着Java生态系统中存在着大量库和复杂的代码块之间关系难以理清的问题，构建系统变得困难且超出了大家的理解和有效开发的范围。特别是在使用繁多的Java存档文件（Java Archive, JAR）时，这一问题变得更加突出。为了应对这种复杂性，模块化能够很好地管理和减少代码的复杂性。因此自Java9开始，引入了模块化系统。通过模块化，Java本身也得以进行模块化改进。
<a name="u1TEi"></a>
### 5.1 模块化是什么
模块化指的是JAVA平台的模块系统（Java Platform Module System），简称JPMS。JPMS引入一种新方式来组织和构建Java应用程序，它将代码分为相互独立、可复用的模块。每个块都有自己的命名空间，明确声明并控制其他模块的访问权限。这种模块化设计使得开发人员能够更好地维护复杂的应用程序，提高代码的复用性、可维护性和安全性，同时提升应用的加载速度和性能。最大的特点是可以定义模块描述符来隔离module（Jar包）内部类的访问权限。<br />**模块化的几点关键说明：**<br />1）相对于JDK8的变动

- JDK9以后引入了一个新组件module：模块描述符module-info.java，用于将一组相关的包放入一个组中。
- 在Java8和更早的应用程序中，应用程序将包作为顶级组件，Java9以后应用程序将模块作为顶级组件。
- 一个模块（Jar包）只能有一个module-info.java。

2）和Maven的关系<br />模块化并不是要替代Maven，和Maven本身并不冲突，Maven定义jar之间的依赖关系，模块化是对已经依赖的jar下的包进行更细粒度依赖控制。<br />3）如何兼容旧应用<br />天然兼容旧应用。为了向后兼容旧项目，一些库本身并未模块化，其仍然可以作为模块在模块路径中使用，而这些库在模块路径上时会被转化为自动模块，例如：jackson-databind-1.0.0.jar将成为自动模块jackson.databind。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779318569-5ffdd5e4-81b1-45f0-9ff1-0ec7c6f634d4.png#averageHue=%236ab691&clientId=ua36ad781-9a05-4&from=paste&id=u8139c581&originHeight=812&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9424927c-4a03-4370-8738-ce7cef5dbe3&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779319168-03fb57ae-8b42-4b7d-beb7-6358bbb3c34c.png#averageHue=%23c8d2c5&clientId=ua36ad781-9a05-4&from=paste&id=IF589&originHeight=722&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u525b79a7-14e1-48e4-8c6a-76c80b594df&title=)
<a name="yGPq5"></a>
### 5.2 带来了哪些好处
<a name="Xh4Te"></a>
#### 1）封装和隔离，更好的访问控制
模块化允许开发者将代码和资源封装在独立的模块中。模块之间可以明确地定义公开和私有的API，提供了更好的代码隔离性和可维护性。<br />ps：新业务单应用可以按照领域模型来进行多模块的划分，以避免代码腐化。简单举例单应用下存在产品.jar、订单.jar。订单依赖产品，通过模块化的限制，订单只能使用产品中明确对外暴露的类，这样就避免传统模式订单.jar可能依赖了产品.jar中普通的类导致代码腐化的问题，也降低后续领域服务拆分的复杂度。
<a name="Wk16f"></a>
#### 2）更好的可伸缩性，加载速度的提升
模块化系统使得Java平台更加可伸缩，通过模块化定义，可以仅加载需要的模块，从而提升加载类的效率，最终减少了应用程序的内存占用和启动时间，同时打包后的程序也更小。
<a name="lxjqQ"></a>
#### 3）明确的依赖关系
模块化系统要求在模块之间明确定义依赖关系。在编译或运行代码之前，模块系统会检查模块是否满足所有依赖关系，从而导致更少的运行时错误。
<a name="OgNvb"></a>
#### 4）安全
在JVM的最深层次上执行强封装，减少Java运行时的攻击面，同时无法获得对敏感内部类的反射访问。
<a name="DuUWp"></a>
### 5.3 如何使用
<a name="XUV6j"></a>
#### 1）定义module-a.jar
包结构如下：
```
com.jdt.a
        person
            Men.java
        reflect
            ReflectModel.java
        module-info.java
```
module-info文件内容如下：
```java
module module.a { 
    //指令用于指定一个模块中哪些包下的public对外是可访问的，包括直接引入和反射使用 
    exports com.jdt.a.person; 
    // 只能被反射调用，用于指定某个包下所有的 public 类都只能在运行时可被别的模块进行反射，并且该包下的所有的类及其乘员都可以通过反射进行访问。
    opens com.jdt.a.refect; 
}
```
<a name="IEn2a"></a>
#### 2）定义module-b.jar包的pom中指定依赖了module-a
包结构如下：
```
com.jdt.b
        test
            Test.java
        module-info.java
```
module-info文件内容如下：
```java
module module.b {
     //依赖a下的包
     requires module.a;
}
```
<a name="bwqbl"></a>
#### 3）此时module-b.jar在编写编码时，会遇到如下问题
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779320411-c4275589-c374-4719-9bb5-0e95ab627ac3.png#averageHue=%23222326&clientId=ua36ad781-9a05-4&from=paste&id=ahS8I&originHeight=189&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4dc818ce-9797-489e-9af0-b50f0353f0d&title=)
<a name="GX20j"></a>
### 5.4 实践过程的坑
上面简单介绍了模块化的知识，具体在落地过程中，主要踩了以下的坑，供大家参考
<a name="mEZTe"></a>
#### 1）依赖JSF包时无法模块化
* JSF是京东内部使用的高性能RPC框架<br />进行模块化时，pom中依赖了jsf包，模块定义如下：
```java
module module.a {
    requires  fastjson;
    //依赖jsf包名
    requires  jsf.lite;

    exports com.jd.jdk.test.module;
}
```
此时编译报错如下：提示找不到模块：jsf.lite，但是pom中明明指定依赖了jsf.lite<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779321430-04d8cced-e753-421d-9622-983cfc22a2de.png#averageHue=%2327292f&clientId=ua36ad781-9a05-4&from=paste&id=uf97a54bf&originHeight=116&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u781f5e06-68aa-4669-9aae-f9a688387a6&title=)<br />**问题原因**：<br />经过一系列定位研究，发现jsf-lite包中，/META-INF/services下的文件org.glassfish.jersey.internal.spi.AutoDiscoverable里面写的类是com.alibaba.fastjson.support.jaxrs.FastJsonAutoDiscoverable，此类并未在当前jsf.lite包中定义，属于com.alibaba.fastjson包的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701779322564-2b031f74-25ea-4405-9d5f-feccd64cb82c.png#averageHue=%233d413f&clientId=ua36ad781-9a05-4&from=paste&id=u04dec29c&originHeight=759&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufe916dce-c7ef-4ce1-847c-0b018e272cf&title=)<br />但是pom中明明也依赖了com.alibaba.fastjson包，为什么模块化后，就找不到了呢？<br />**主要原因在于模块化遇到SPI（Service Provider Interface）时的约束：模块化时，SPI机制要求配置中定义依赖的类必须本模块定义的，不能是其他模块的包（来自它不拥有的包），否则，此包将无法被模块化。**<br />这样也就解释了，为什么上面jsf无法找到module的问题，jsf-lite里面设置了它不拥有的包：com.alibaba.fastjson.support.jaxrs.FastJsonAutoDiscoverable，导致jsf-lite包无法被自动模块化。<br />**解决方案**：<br />联系JSF团队，升级JSF包，修复上面说的`FastJsonAutoDiscoverable`配置错误的问题。
<a name="QVo20"></a>
#### 2）拆包问题（模块隔离）
**模块化约束：jdk9以上，使用模块化时不支持拆分包的形式依赖。**<br />拆分包意味着两个模块包含相同的包，Java模块系统不允许拆分包。拆分包始终是不正常的，而当使用解析可传递依赖项的构建工具（如Maven等）时，很容易出现同一个库的多个版本，当Java模块系统检测到一个包存在于模块路径上的多个模块中时，就会拒绝启动。<br />**例如**：
```
module-a.jar包结构定义：
com.foo.package
    A.java

module-b.jar包结构定义：
com.foo.package
    B.java
```
当module-c同时依赖module-a和module-b时，如上编译时会报一个错，Package com.foo.package in both module module.b and module module.a，这就是JAVA9的模块隔离，要求只能从一个模块（module）中读取同一个包（package），不能跨模块读取。<br />**解决方案**：<br />如果在使用模块化时，遇到了拆分包问题，无论如何都是无法绕过的。即使从用户角度来看基于类路径的应用程序可以正确工作，最终也需要处理这些问题。此时只能停用模块化或升级jar包，避免拆分包问题
<a name="SKFrA"></a>
### 5.5  模块化落地总结
目前不推荐使用模块化，因为相关组件生态还不完善，并且模块化带来的价值不够突出：

1. 很多中间件都是基于jdk8构建的，都有可能遇到模块化兼容的问题，比如：jsf，需要jsf强制升级才可以使用模块化；
2. 拆包问题无法解决，比如：aws-java-sdk-s3、fluent等。
<a name="sM4OG"></a>
## 6、总结  
1、升级过程简单，升级后可以使用更多新特性和更好的GC性能，所以建议升级到JDK11。<br />2、现阶段不推荐使用模块化，但是不用担心会影响JDK11的升级。<br />另外听说JDK17的 ZGC可以达到亚秒级停顿，但考虑到JDK11的ZGC还不是很稳定，所以本次不做测试，后面升级到JDK17后有机会再给大家分享ZGC压测效果。
