Java
<a name="ImFpK"></a>
## 一、系统选择
关于最基础的底层镜像，通常大多数只有三种选择：Alpine、Debian、CentOS；这三者中对于运维最熟悉的一般为CentOS，但是很不幸的是CentOS后续已经不存在稳定版，关于它的稳定性问题一直是个谜一样的问题；这是一个仁者见仁智者见智的问题。<br />排除掉CentOS只讨论是Alpine还是Debian；从镜像体积角度考虑无疑Alpine完胜，但是Alpine采用的是musl的C库，在某些深度依赖glibc的情况下可能会有一定兼容问题。当然关于**深度依赖 glibc** 究竟有多深度取决于具体应用，就目前来说个人也只是遇到过Alpine官方源中的OpneJDK一些字体相关的BUG。<br />综合来说，建议是如果应用深度依赖glibc，比如包含一些JNI相关的代码，那么选择Debian或者说基于Debian的基础镜像是一个比较稳的选择；如果没有这些重度依赖问题，那么在考虑镜像体积问题上可以选择使用Alpine。事实上OpneJDK本身体积也不小，即使使用Alpine版本，再安装一些常用软件后也不会小太多，习惯是使用基于Debian的基础镜像。
<a name="qZUA3"></a>
## 二、JDK OR JRE
大多数人似乎从不区分JDK与JRE，所以要确定这事情需要先弄明白JDK和JRE到底是什么：

- JDK：Java Development Kit
- JRE：Java Runtime Environment

JDK是一个开发套件，它会包含一些调试相关的工具链，比如javac、jps、jstack、jmap等命令，这些都是为了调试和编译Java程序所必须的工具，同时JDK作为开发套件是包含JRE的；而JRE仅为Java运行时环境，它只包含Java程序运行时所必须的一些命令以及依赖类库，所以JRE会比JDK体积更小、更轻量。<br />如果只需要运行Java程序比如一个jar包，那么JRE足以；但是如果期望在运行时捕获一些信息进行调试，那么应该选择JDK。个人的习惯是为了解决一些生产问题，通常选择直接使用JDK作为基础镜像，避免一些特殊情况还需要挂载JDK的工具链进行调试。当然如果没有这方面需求，且对镜像体积比较敏感，那么可以考虑使用JRE作为基础镜像。
<a name="bVyNv"></a>
## 三、JDK 选择
<a name="H5pNg"></a>
### 3.1、OracleJDK 还是 OpenJDK
针对于这两者的选择，取决于一个最直接的问题：应用代码中是否有使用OracleJDK私有API。<br />**通常 “使用这些私有 API” 指的是引入了一些 com.sun.* 包下的相关类、接口等，这些API很多是OracleJDK私有的，在OpneJDK中可能完全不包含或已经变更。所以如果代码中包含相关调用则只能使用OracleJDK。**<br />**值得说明的是很多时候使用这些API并不是真正的业务需求，很可能是开发在导入包时“手滑”并且凑巧被导入的Class等也能实现对应功能；对于这种导入是可以被平滑替换的，比如换成ApacheCommons相关的实现。还有一种情况是开发误导入后及时发现了，但是没有进行代码格式化和包清理，这是会在代码头部遗留相关的import引用，而Java是允许存在这种无用的import的；针对这种只需要重新格式化和优化导入即可。**<br />Tips：IDEA按Option+Command+L(格式化)还有Control+Option+O(自动优化包导入)。
<a name="Jq21T"></a>
### 3.2、OracleJDK 重建问题
当没有办法必须使用OracleJDK时，推荐自行下载OracleJDK压缩包并编写Dockerfile创建基础镜像。但是这会涉及到一个核心问题：**OracleJDK一般不提供历史版本**，所以如果要考虑未来的重新构建问题，建议保留好下载的OralceJDK压缩包。
<a name="mIzc6"></a>
### 3.3、OpenJDK 发行版
众所周知OpenJDK是一个开源发行版，基于开源协议各大厂商都提供一些增值服务，同时也预编译了一些Docker镜像供开发者使用；目前主流的一些发行版本如下：

- AdoptOpenJDK
- Amazon Corretto
- IBM Semeru Runtime
- Azul Zulu
- Liberica JDK

这些发行版很多是大同小异的，一些发行版可能提供的基础镜像选择更多，比如AdoptOpenJDK提供基于Alpine、Ubuntu、CentOS的三种基础镜像发行版；还有一些发行版提供其他的JVM实现，比如IBMSemeruRuntime提供OpenJ9JVM的预编译版本。<br />目前比较受欢迎的是AdoptOpenJDK，因为它是社区驱动的，由JUG成员还有一些厂商等社区成员组成；而AmazonCorretto和IBMSemeruRuntime看名字就可以知道是云高端玩家做的，可用性也比较棒。其他的类似AzulZulu、LibericaJDK则是一些JVM提供厂商，有些还有点算得上是黑料的东西，不算特别推荐。<br />目前AdoptOpenJDK已经合并到EclipseFoundation，现在叫做EclipseAdoptium；所以如果想要使用AdoptOpenJDK镜像，DockerHub中应该使用eclipse-temurin用户下的相关镜像。
<a name="deJIf"></a>
## 四、JVM 选择
对于JVM实现来说，Oracle有一个JVM实现规范，这个实现规范定义了兼容Java代码运行时的这个VM应当具备哪些功能；所以**只要满足这个JVM实现规范且经过了认证，那么这个JVM实现理论上就可以应用于生产**。目前市面上也有很多JVM实现：

- Hotspot
- OpenJ9
- TaobaoVM
- LiquidVM
- Azul Zing

这些JVM实现可能具有不同的特性和性能，比如Hotspot是最常用的JVM实现，综合性能、兼容性等最佳；由IBM创建目前属于Eclipse基金会的OpneJ9对容器化更友好，提供更快启动和内存占用等特性。<br />通常建议如果对JVM不是很熟悉的情况下，请使用“标准的”Hotspot；如果有更高要求且期望自行调试一些JVM优化参数，请考虑EclipseOpenJ9。OpenJ9的文档写的很不错，只要细心看可以读到很多不错的细节等；如果要使用OpenJ9镜像，推荐直接使用ibm-semeru-runtimes预编译的镜像。
<a name="um63r"></a>
## 五、信号量传递
当需要关闭一个程序时，通常系统会像该进程发送一个终止信号，同样在容器停止时Kubernetes或者其他容器工具也会像容器内PID1的进程发送终止信号；如果容器内运行一个Java程序，那么信号传递给JVM后Java相关的框架比如SpringBoot等就会检测到此信号，然后开始执行一些关闭前的清理工作，**这被称之为“优雅关闭(Gracefulshutdown)”**。<br />如果在容器化Java应用时没有正确的让信号传递给JVM，那么调度程序比如Kubernetes在等待容器关闭超时以后就会进行强制关闭，**这很可能导致一些Java程序无法正常释放资源，比如数据库连接没有关闭、注册中心没有反注册等**。为了验证这个问题，创建了一个SpringBoot样例项目来进行测试，其中项目中包含的核心文件如下(完整代码请看GitHub)：<br />BeanTest.java：使用`@PreDestroy`注册Hook来监听关闭事件模拟优雅关闭

- **Dockerfie.bad：**错误示范的 Dockerfile
- **Dockerfile.direct：**直接运行命令来实现优雅关闭
- **Dockerfile.exec：**利用 exec 来实现优雅关闭
- **Dockerfile.bash-c：**利用 bash -c 来实现优雅关闭
- **Dockerfile.tini：**验证 tini 在某些情况下无法实现优雅关闭
- **Dockerfile.dumb-init：**验证 dumb-init 在某些情况下无法实现优雅关闭

由于 BeanTest 只做打印测试都是通用的，所以这里直接贴代码：
```java
package com.example.springbootgracefulshutdownexample;

import org.springframework.stereotype.Component;

import javax.annotation.PreDestroy;

@Component
public class BeanTest {
    @PreDestroy
    public void destroy() {
        System.out.println("==================================");
        System.out.println("接收到终止信号, 正在执行优雅关闭...");
        System.out.println("==================================");
    }
}
```
<a name="F1KYK"></a>
### 5.1、错误的信号传递
在很多原始的 Java 项目中通常会存在一个启动运行脚本，这些脚本可能是自行编写的，也可能是一些比较老的Tomcat启动脚本等；当使用脚本启动并且没有合理的调整Dockerfile时就会出现信号无法正确传递的问题；例如下面的错误示范：<br />**entrypoint.bad.sh：负责启动**
```bash
#!/usr/bin/env bash

java -jar /SpringBootGracefulShutdownExample-0.0.1-SNAPSHOT.jar
```
**Dockerfie.bad：使用bash启动脚本，这会导致终止信号无法传递**
```dockerfile
FROM eclipse-temurin:11-jdk

COPY entrypoint.bad.sh /
COPY target/SpringBootGracefulShutdownExample-0.0.1-SNAPSHOT.jar /

# 下面几种种方式都无法转发信号
#CMD /entrypoint.bad.sh
#CMD ["/entrypoint.bad.sh"]
CMD ["bash", "/entrypoint.bad.sh"]
```
通过这个 Dockerfile 打包运行后，**在使用 **`**docker stop**`** 命令时明显卡顿一段时间(实际上是 docker 在等待容器内进程自己退出)，当到达预定的超时时间后容器内进程被强行终止，故没有打印优雅关闭的日志：**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301135364-72dbb76b-ecf5-40ac-9051-4da4c65385bb.png#averageHue=%232e323b&clientId=ue170d49d-35f4-4&from=paste&id=u45d5e036&originHeight=508&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2ccbeb65-4e2f-48a9-a238-79d0fc4ba6b&title=)
<a name="PTJ5M"></a>
### 5.2、正确的信号传递
<a name="Xk7N0"></a>
#### 5.2.1、直接运行方式
要解决信号传递这个问题其实很简单，也有很多方法；比如常见的直接使用 CMD 或 ENTRYPOINT 指令运行 java 程序：<br />**Dockerfile.direct：直接运行java程序，能够正常接受到终止信号**
```dockerfile
FROM eclipse-temurin:11-jdk

COPY target/SpringBootGracefulShutdownExample-0.0.1-SNAPSHOT.jar /

CMD ["java", "-jar", "/SpringBootGracefulShutdownExample-0.0.1-SNAPSHOT.jar"]
```
可以看到，在Dockerfile中直接运行java命令这种方式可以让jvm正确的通知应用完成优雅关闭：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301135353-b23b090c-f64f-4b72-9205-c9cdd17cc914.png#averageHue=%232f323c&clientId=ue170d49d-35f4-4&from=paste&id=u5eb86c9a&originHeight=503&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2c3b95c6-0623-4b37-a8c4-f29c364fa90&title=)
<a name="DKHWq"></a>
#### 5.2.2、间接 Exec 方式
熟悉Docker的同学都应该清楚，在Dockerfile里直接运行命令无法解析环境变量；但是有些时候又依赖脚本进行变量解析，这时候可以先在脚本内解析完成，并采用`exec`的方式进行最终执行；这种方式也可以保证信号传递。<br />**entrypoint.exec.sh：exec执行最终命令，可以转发信号**
```bash
#!/usr/bin/env bash

# 假装进行一些变量处理等操作...
export VERSION="0.0.1"

exec java -jar /SpringBootGracefulShutdownExample-${VERSION}-SNAPSHOT.jar
```
<a name="rJF33"></a>
#### 5.2.3、Bash-c 方式
除了直接执行和exec方式其实还有一个称之为“不稳定”的解决方案，就是使用bash-c来执行命令；在使用bash-c执行一些简单命令时，其行为会跟exec很相似，也会把子进程命令替换到父进程从而让-c后的命令直接接受到系统信号；**但需要注意的是，这种方式不一定百分百成功，比如当-c后面的命令中含有管道、重定向等可能仍会触发fork，这时子命令仍然无法完成优雅关闭。**<br />**Dockerfile.bash-c：采用 bash -c 执行，在命令简单情况下可以做到优雅关闭**
```dockerfile
FROM eclipse-temurin:11-jdk

COPY entrypoint.bad.sh /
COPY target/SpringBootGracefulShutdownExample-0.0.1-SNAPSHOT.jar /

CMD ["bash", "-c", "java -jar /SpringBootGracefulShutdownExample-0.0.1-SNAPSHOT.jar"]
```
关于 bash -c 的相关讨论，可以参考StackExchange。
<a name="nHsgJ"></a>
#### 5.2.4、tini 或 dump-init
守护工具并不是万能的，tini和dump-init都有一定问题。<br />这两个工具是大部分人都熟知的利器，甚至连Docker本身都集成了；不过似乎很多人都有一个误区(以前也是这么觉得的)，那就是**认为加了 tini 或者 dump-init 信号就可以转发，就可以优雅关闭了；而事实上并不是这样，很多时候加了这两个东西也只能保证僵尸进程的回收，但是子进程仍然可能无法优雅关闭。**比如下面的例子：<br />**Dockerfile.tini：加了 tini 也无法优雅关闭的情况**
```dockerfile
FROM eclipse-temurin:11-jdk

RUN set -e \
    && apt update \
    && apt install tini psmisc -y

COPY entrypoint.bad.sh /
COPY target/SpringBootGracefulShutdownExample-0.0.1-SNAPSHOT.jar /

ENTRYPOINT ["tini", "-vvv", "--"]

CMD ["bash", "/entrypoint.bad.sh"]
```
对于 dump-init 也有同样的问题，归根结底这个问题的根本还是在bash上：**当使用 bash 启动脚本后，bash会fork一个新的子进程；而不管是tini还是dump-init的转发逻辑都是将信号传递到进程组；只要进程组中的父进程响应了信号，那么就认为转发完成，但此时进程组中的子进程可能还没有完成优雅关闭父进程就已经死了，这会导致变为子进程最终还会被强制kill掉。**
<a name="QN4zY"></a>
### 5.3、最佳实践
根据上面的测试和验证结果，这里总结一下最佳实践：

- 1、容器内内置 tini 或者 dump-init 是比较好的做法可以防止僵尸进程
- 2、tini 或者 dump-init 并不能百分百实现优雅关闭
- 3、简单命令直接 CMD 执行可以接受信号转发实现优雅关闭
- 4、复杂命令在脚本内进行 exec 执行也可以接受信号转发实现优雅关闭
- 5、直接使用 bash -c 运行在简单命令执行时也可以优雅关闭，但需要实际测试来确定准确性
<a name="EaXEb"></a>
## 六、内存限制
Java 应用的容器化内存限制是一个老生常谈的问题，国内也有很多资料，不过这些文章很多都过于老旧或者直接翻译自国外的文章；很少有人去深究和测试这个问题，随着这两年容器化的发展其实很多东西早已不适用，为此在这里决定专门仔细的测试一下这个内存问题(只想看结论的可直接观看6.3章节)。<br />众所周知，Java是有虚拟机的，Java代码被编译成Class文件然后在JVM中运行；JVM默认会根据操作系统环境来自动设置堆内存(HeapSize)， 而**容器化 Java 应用面临的挑战其一就是如何让 JVM 获取到正确的可用内存避免被 kill。**
<a name="glLVC"></a>
### 6.1、无配置下的自适应
在默认不配置时，理想状态的JVM应当能识别到对容器施加的内存limit，从而自动调整堆内存大小；为了验证这种理想状态下哪些版本的OpenJDK能做到，抽取一些特定版本进行了以下测试：

- 使用 docker run -m 512m ... 将容器内存限制为512m，实际宿主机为16G
- 使用 `java -XX:+PrintFlagsFinal -version | grep MaxHeapSize` 命令查看 JVM 默认的最大堆内存(后来发现 `-XshowSettings:vm` 看起来更清晰)
<a name="UpvWg"></a>
#### 6.1.1、OpenJDK 8u111
这个版本的 OpenJDK 尚未对容器化做任何支持，所以理论上它是不可能能获取到limit的内存限制：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301135348-a46d1775-3781-4301-a0e8-05cf055e4b93.png#averageHue=%232c2f3a&clientId=ue170d49d-35f4-4&from=paste&id=u11d1f638&originHeight=301&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5e7c3df8-44b2-40ef-911c-f073a2615e6&title=)<br />可以看到 JVM 并没有识别到limit，仍然按照大约宿主机1/4的体量去分配的堆内存，所以如果里面的java应用内存占用高了可能会被直接kill。
<a name="tQLjU"></a>
#### 6.1.2、OpenJDK 8u131
选择 8u131 这个版本是因为在此版本添加了 `-XX:+UseCGroupMemoryLimitForHeap` 参数来支持内存自适应，这里先不开启，先直接进行测试：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301135357-14cf45bf-292c-47ec-bf59-dd3f875e5dcf.png#averageHue=%232c2f3a&clientId=ue170d49d-35f4-4&from=paste&id=u97ea5fba&originHeight=310&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf9837740-4723-45c3-868e-9bfc5b86cf6&title=)<br />同样在默认情况下是无法识别内存限制的。
<a name="bpwVc"></a>
#### 6.1.3、OpenJDK 8u222
8u191 版本从 OpneJDK 10 backport 回了 XX:+UseContainerSupport 参数来支持 JVM 容器化，不过该版本暂时无法下载，这里使用更高的8u222测试，测试时同样暂不开启特定参数进行测试：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301135324-50b0e9ad-207a-43c0-8e56-53d83d6f89a5.png#averageHue=%232a2e38&clientId=ue170d49d-35f4-4&from=paste&id=uc63e4d93&originHeight=304&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u92c46e5a-2cd7-4e82-911a-ae446b48b17&title=)<br />同样的内存无法正确识别.
<a name="rRFwW"></a>
#### 6.1.4、OpenJDK 11.0.15
OpenJDK 11 版本已经开始对容器化的全面支持，例如 XX:+UseContainerSupport 已被默认开启，所以这里仍然选择不去修改任何设置去测试：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301136355-354b6ae6-826a-46b5-b80f-310dca0887d3.png#averageHue=%23282b36&clientId=ue170d49d-35f4-4&from=paste&id=u4bafe98c&originHeight=308&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubf3578e7-d707-4fc9-8e46-fefceae510a&title=)<br />可以看到，即使默认打开了 UseContainerSupport 开关，仍然无法正常的自适应内存。
<a name="siaLM"></a>
#### 6.1.5、OpenJDK 11.0.16
可能很多人会好奇，都测试了 11.0.15 为什么还要测试 11.0.16? 因为这两个版本在不设置的情况下有个奇怪的差异：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301136422-a3789f3d-75b3-44eb-9bdf-df406566f51d.png#averageHue=%23282b36&clientId=ue170d49d-35f4-4&from=paste&id=u3598f4b9&originHeight=342&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0cb1c0c7-7667-4c82-8f2d-6ac79e9634d&title=)<br />**可以看到，11.0.16 版本在不做任何设置时自动适应了容器内存限制，堆内存从接近4G变为了120M。**
<a name="F4dG7"></a>
#### 6.1.6、OpenJDK 17
OPneJDK 17 是目前最新的 LTS 版本，这里再专门测试一下 OpneJDK 17 不调整任何参数时的内存自适应情况：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301136422-f177386d-3733-4f86-8410-5ef3ac75e5b7.png#averageHue=%232a2d38&clientId=ue170d49d-35f4-4&from=paste&id=u1650337e&originHeight=416&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u92d02b74-ade1-4a7e-a8ed-df7144ab692&title=)<br />可以看到 OpneJDK 17 与 OpenJDK 11.0.16 版本一样，都可以实现内存的自适应。
<a name="TGcyO"></a>
### 6.2、有配置下的自适应
在上面的无配置情况下进行了一些测试，测试结果从11。0。15版本开始出现了一些“令人费解”的情况；理论上11+已经自动打开了容器支持参数，但是某些版本内存自适应仍然无效，这促使我对其他参数的实际效果产生了怀疑；为此开始按照各个参数的添加版本手动启用这些参数进行了一些测试。
<a name="ppeUn"></a>
#### 6.2.1、OpenJDK 8u131
8u131 正式开始进行容器化支持，在这个版本增加了一个JVM选项来告诉JVM使用cgroup设置的内存限制；增加了 `-XX:+UnlockExperimentalVMOptions -XX:+UseCGroupMemoryLimitForHeap` 参数进行测试，测试结果是这个选项在当前的环境中似乎完全不生效：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301136427-a18d10cf-f13b-4bbd-b918-6c78e7cff8e5.png#averageHue=%23292c37&clientId=ue170d49d-35f4-4&from=paste&id=u447ecc69&originHeight=245&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9c546dfd-732a-4c0d-82d8-ad28b776860&title=)
<a name="oYwAE"></a>
#### 6.2.2、OpenJDK 8u222
从 8u191 版本开始，又增加了另一个开启容器化支持的参数 -XX:+UseContainerSupport，该参数从OpenJDK10反向合并而来；尝试使用这个参数来进行测试，结果仍然是没什么卵用：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301136381-bcba0836-81fe-4685-99bc-c961bb44dc32.png#averageHue=%23282c37&clientId=ue170d49d-35f4-4&from=paste&id=ua7febeed&originHeight=253&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua207f87e-7217-4caa-8da7-702646e0cff&title=)
<a name="rEDza"></a>
#### 6.2.3、OpenJDK 11+
从 11+ 版本开始 -XX:+UseContainerSupport 已经自动开启，不需要再做什么特殊设置，所以结果是跟无配置测试结果一致的: **从 11.0.15 以后的版本开始能够自适应，之前的版本(包括 11.0.15)都不支持自适应。**
<a name="aGKyn"></a>
### 6.3、分析与总结
经过上面的一些测试后会发现，在很多文章或文档中描述的参数出现了莫名其妙不好使的情况；这主要是因为容器化这两年一个很重要的更新：**Cgroups v2**；限于篇幅问题这里不在一一罗列测试截图，下面仅说一下结论。
<a name="imSOQ"></a>
#### 6.3.1、Cgroups V1
对于使用 Cgroups V1 的容器化环境来说，“旧的”一些规则仍然适用(新内核增加内核参数 `systemd.unified_cgroup_hierarchy=0` 回退到 Cgroups V1):

- 1、OpenJDK 8u131 以及之后版本增加 `-XX:+UnlockExperimentalVMOptions -XX:+UseCGroupMemoryLimitForHeap` 参数支持内存自适应.
- 2、OpenJDK 8u191 以及之后版本增加 `-XX:+UseContainerSupport` 参数支持内存自适应。
- 3、OpenJDK 11 以及之后版本默认开启了 `-XX:+UseContainerSupport` 参数，自动支持内存自适应
<a name="nNjPi"></a>
#### 6.3.2、Cgroups V2
在新版本系统(具体自行查询)配合较新的 containerd 等容器化工具时，已经默认转换为 Cgroups V2，**需要注意的是针对于 Cgroups V2 的内存自适应只有在 OpneJDK 11.0.16 以及之后的版本才支持，在这之前开启任何参数都没用。**<br />关于 Cgroups V2 的一些支持细节具体请查看 JDK-8230305：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301137783-5f4c0f46-73aa-4ac3-a9dc-5995c71372d5.png#averageHue=%23fefefe&clientId=ue170d49d-35f4-4&from=paste&id=ufa46d9f9&originHeight=616&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc3ef0bcf-9dcc-4188-8e60-46d75753286&title=)
<a name="aDMNv"></a>
## 七、DNS 缓存
在大部分 Java 程序中都会使用域名去访问一些服务，可能是访问某些API端点或者是访问一些数据库，而不论哪样只要使用了域名就会涉及到DNS缓存问题；**Java 的 DNS 缓存是由 JVM 控制的，不要理所当然的以为JVMDNS缓存非常友好，某些时候DNS缓存可能超出预期。**为了测试 DNS 缓存情况从某大佬这里抄来一个测试脚本，该脚本会测试三个版本的 OpenJDK DNS 缓存情况：<br />**jvm-dns-ttl-policy.sh**
```bash
#!/usr/bin/env bash

set -e

for tag in 8-jdk 11-jdk 17-jdk; do

    tag_name="jvm-dns-ttl-policy"
    output_file="$(mktemp)"

    jvm_args=""
    if ! [ "${tag}" == "8-jdk" ]; then
        jvm_args="--add-exports java.base/sun.net=ALL-UNNAMED"
    fi

    ttl=""
    if ! [ "${1}" == "" ]; then
        ttl="-Dsun.net.inetaddr.ttl=${1}"
    fi

    dockerfile="
FROM        eclipse-temurin:${tag}
WORKDIR     /var/tmp
RUN         printf ' \\
              public class DNSTTLPolicy { \\
                public static void main(String args[]) { \\
                  System.out.printf(\"Implementation DNS TTL for JVM in Docker image based on 'eclipse-temurin:${tag}' is %%d seconds\\\\n\", sun.net.InetAddressCachePolicy.get()); \\
                } \\
              }' >DNSTTLPolicy.java
RUN         javac ${jvm_args} DNSTTLPolicy.java -XDignore.symbol.file
CMD         java ${jvm_args} ${ttl} DNSTTLPolicy
ENTRYPOINT  java ${jvm_args} ${ttl} DNSTTLPolicy
"

    dockerfile_security_manager="
FROM        eclipse-temurin:${tag}
WORKDIR     /var/tmp
RUN         printf ' \\
              public class DNSTTLPolicy { \\
                public static void main(String args[]) { \\
                  System.out.printf(\"Implementation DNS TTL for JVM in Docker image based on 'eclipse-temurin:${tag}' (with security manager enabled) is %%d seconds\\\\n\", sun.net.InetAddressCachePolicy.get()); \\
                } \\
              }' >DNSTTLPolicy.java
RUN         printf ' \\
              grant { \\
                permission java.security.AllPermission; \\
              };' >all-permissions.policy
RUN         javac ${jvm_args} DNSTTLPolicy.java -XDignore.symbol.file
CMD         java ${jvm_args} ${ttl} -Djava.security.manager -Djava.security.policy==all-permissions.policy DNSTTLPolicy
ENTRYPOINT  java ${jvm_args} ${ttl} -Djava.security.manager -Djava.security.policy==all-permissions.policy DNSTTLPolicy
"

    echo "Building Docker image based on eclipse-temurin:${tag} ..." >&2
    docker build -t "${tag_name}" - <<<"${dockerfile}" 2>&1 > /dev/null
    docker run --rm "${tag_name}" &>"${output_file}"
    cat "${output_file}"
    docker build -t "${tag_name}" - <<<"${dockerfile_security_manager}" 2>&1 > /dev/null
    docker run --rm "${tag_name}" &>"${output_file}"
    cat "${output_file}"
    echo ""

done
```
<a name="dhtXs"></a>
### 7.1、默认 DNS 缓存
默认不做任何设置的 DNS 缓存结果如下(直接运行脚本即可)：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301137657-9e4f73c6-95ba-4967-91b2-2ad84fe46b70.png#averageHue=%232b2e39&clientId=ue170d49d-35f4-4&from=paste&id=u360f4b40&originHeight=348&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8169070a-a119-4a0f-b381-96a3d4cf272&title=)<br />**可以看到，默认情况下DNSTTL被设置为30s，如果开启了 Security Manager 则变为 -1s，那么-1s什么意思呢(截取自 OpenJDK 11 源码)：**
```java
/* The Java-level namelookup cache policy for successful lookups:
 *
 * -1: caching forever
 * any positive value: the number of seconds to cache an address for
 *
 * default value is forever (FOREVER), as we let the platform do the
 * caching. For security reasons, this caching is made forever when
 * a security manager is set.
 */
private static volatile int cachePolicy = FOREVER;

/* The Java-level namelookup cache policy for negative lookups:
 *
 * -1: caching forever
 * any positive value: the number of seconds to cache an address for
 *
 * default value is 0. It can be set to some other value for
 * performance reasons.
 */
private static volatile int negativeCachePolicy = NEVER;
```
<a name="kLslL"></a>
### 7.2、设置 DNS 缓存
为了避免这种奇奇怪怪的 DNS 缓存策略问题，最好在启动时通过增加 `-Dsun.net.inetaddr.ttl=xxx` 参数手动设置 DNS 缓存时间：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301137654-38b559f5-a2f1-4ff1-b233-275040c606de.png#averageHue=%232b2e38&clientId=ue170d49d-35f4-4&from=paste&id=u3877e488&originHeight=363&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u62fe67e3-88a1-4669-b928-c6ca7b6a60a&title=)<br />**可以看到，一但手动设置了DNS缓存，那么不论是否开启 Security Manager 都会遵循用户的设置。**如果需要更细致的调试 DNS 缓存推荐使用 Alibaba 开源的 DCM 工具。
<a name="OLaMr"></a>
## 八、Native 编译
Native 编译优化是指通过 GraalVM 将 Java 代码编译为可以直接被平台执行的二进制文件，编译后的可执行文件运行速度会有极大提升。**但是 GraalVM 需要应用的代码层调整、框架升级等操作，总体来说比较苛刻；但是如果是新项目，最好让开发能支持一下GraalVM的Native编译，这对启动速度等有巨大提升。**<br />上面介绍的用于测试优雅关闭的项目已经内置了GraalVM支持，只需要下载 GraalVM 并设置 JAVA_HOME 和 PATH 变量，并使用 `mvn clean package -Dmaven.test.skip=true -Pnative` 编译即可：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301137619-171fe730-805e-4502-a0f3-0ba7eaf88ae5.png#averageHue=%232c303b&clientId=ue170d49d-35f4-4&from=paste&id=uc17d4efc&originHeight=417&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc7aa310a-2b0d-4083-9064-6cdb6959a2f&title=)<br />编译成功后将在 target 目录下生成可以直接执行的二进制文件，以下为启动速度对比测试：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689301137607-74eee8e4-a630-428b-9f54-90a5f7523eee.png#averageHue=%232d313b&clientId=ue170d49d-35f4-4&from=paste&id=u808f52e1&originHeight=545&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u19abc774-e887-4327-a597-150e17b013d&title=)<br />**可以看到 GraalVM 编译后启动速度具有碾压级的优势，基本差出一个数量级；但是综合来说这种方式目前还不是特别成熟，迄今为止国内 Java 生态仍是 OpneJDK 8 横行，老旧项目想要满足 GraalVM 需要调整的地方比较巨大；所以总结就是新项目能支持尽量支持，老项目不要作死。**
