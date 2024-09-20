JavaSpringBoot
<a name="XxOXC"></a>
## 背景
toB 的本地化 java 应用程序，通常是部署在客户机器上，为了保护知识产权，需要将核心代码（例如 Lience，Billing，Pay 等）进行加密或混淆，防止使用 jadx 等工具轻易反编译。同时，为了更深层的保护程序，也要防止三方依赖细节被窥探；
<a name="XgPmI"></a>
## 业界方案
<a name="bDHuB"></a>
### ProGuard
[https://github.com/Guardsquare/proguard](https://github.com/Guardsquare/proguard)

- 简介：开源社区有名的免费混淆工具，相较于字节码加密，对性能基本无影响；
- 优势：打包阶段混淆字节码，各种变量方法名都变成了abcdefg 等等无意义的符号，字节码可被反编译，但几乎无法阅读，通常被 Android App 用来防止逆向；
- 不足1：只能混淆部分代码，打包阶段较为耗时，对于三方包混淆，并没有什么好办法。
- 不足2：混淆后的代码，会影响 arthas 工具的使用，导致排查问题变慢。
- 不足3：配置比较复杂，令人眼花缭乱。
- 不足4：无法加密三方依赖所有信息；
<a name="oh78u"></a>
### jar-protect
[https://gitee.com/chejiangyi/jar-protect](https://gitee.com/chejiangyi/jar-protect)

- 简介：一款国人开发的 springboot jar 加密工具；需要配合 javaagent 解密；
- 优势：打包阶段使用 javassist 重写 class 文件；jadx 反编译后看到的都是空方法。反编译后只能看到类信息和方法签名，无法看到具体内容。
- 不足1：使用 DES 方案，对于几百个三方 jar 的场景，加密手段过重，且加密后的不够完整；
- 不足2：类文件放在一个目录（META-INF/.encode/），非常容易类冲突；
- 不足3：无法加密三方依赖所有信息；
<a name="zAQrZ"></a>
### GraalVM
[https://javakk.com/tag/graalvm](https://javakk.com/tag/graalvm)

- 简介：Oracle GraalVM 提前将 Java 应用程序编译为独立的二进制文件。与在 Java 虚拟机 （JVM） 上运行的应用程序相比，这些二进制文件更小，启动速度提高了 100 倍，无需预热即可提供峰值性能，并且使用的内存和 CPU 更少， 并且无法反编译。
- 不足：无法支持我司业务程序框架。
<a name="hNXSH"></a>
### core-lib/xjar
[https://github.com/core-lib/xjar](https://github.com/core-lib/xjar)

- 简介：国人开源的，基于 golang 的加密工具。使用 Maven 插件加密，启动时 golang 解密；性能影响未知。
- 优势：可对所有 class 文件加密。
- 不足1：加密后 jar 文件体积翻倍；
- 不足2：依赖 golang 编译，依赖 golang 启动；
- 不足3：无法加密三方依赖所有信息；
- 不足4：开源项目，3年未有新提交。

思考：需求到底是什么？a：保护知识产权。具体手段为：

1. 对本司项目代码进行加密，使其无法被 jadx 工具轻易反编译，
2. 对本司三方依赖进行加密，使其无法窥探我司三方依赖细节；

但上面的几个项目，基本都是围绕着 class 加密(除了GraalVM)，这无法实现第二个需求。
<a name="w2iG9"></a>
## 方案
设计目标：

1. 将项目三方依赖 jar 进行加密，使其无法使用 jadx 反编译，但运行时会生成解密后的临时文件。
2. 将项目本身的 class 进行加密，使其无法使用 jadx 反编译运行时解密后的文件。
3. 加密策略要灵活，轻量，对启动速度，包体积，内存消耗，接口性能的影响要控制在 5% 以内；

设计方案：

1. 加密jar时，使用 Maven 打包工具，repackage fat jar；将其内部 lib 目录的依赖进行加密；使 jadx 无法反编译；
2. 加密class时，对于核心业务代码，使用 javassist 工具将其重写，清空方法体，重置属性值；
3. 解密jar时，将指定目录的 加密包 解密 到指定目录，并将其放入 springboot classloader classpath 里。
4. 解密class时，agent 配合判断是否是加密 class，如果是，则寻找加密 class 文件，找到后解密，返回解密后的 classBytes。

逻辑如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1699968494067-a7fcf773-467d-479e-a2e2-5627914ff7a1.jpeg)<br />注意点：

1. javassist 重写方法体时，需要将 lib 里的所有代码都加入 classpool 的 classpath 里。
2. javassist 加密后的类，需将其放入到当前 lib 的单独目录进行个例，防止类冲突。
3. agent 解密要轻量，不能影响程序性能；
4. 三方包的加解密重新打包后，jar 顺序发生变化，较小可能会导致类冲突（比如 log4j）。需要在测试环境验证，如果存在冲突，则需要排包。
<a name="DGgst"></a>
## End
通过以上方案，实现了一个极其轻量的 Maven 加密，agent 解密插件。他能够将三方包彻底加密，使 jadx 等工具无法反编译 ，屏蔽三方依赖细节，同时，该插件也可以加密业务 class 代码，使 jadx 无法反编译运行时生成的代码，从而一定程度的保护知识产权；<br />另外，私有的加密算法，在性能，体积，内存等方便的影响都控制在 5% 以内。<br />为了防止混淆后的代码影响 arthas 的使用和 bug patch 的应用，放弃了混淆方案，只能说是一种权衡与取舍吧。<br />从软件防破解的角度来理解，通常只能是加大破解的难度，铁了心想要破解的话，就算是 ProGuard 混淆，也无法解决。也许只能用 GraalVM，但不是每一个客户都会用这个。
