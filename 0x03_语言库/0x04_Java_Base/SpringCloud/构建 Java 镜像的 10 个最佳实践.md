JavaDocker<br />提供构建生产级 Java 容器的最佳实践，旨在优化和保护要投入生产环境中的 Docker 镜像。
<a name="uwlMB"></a>
### **构建一个简单的 Java 容器镜像**
从简单的 Dockerfile 开始，在构建 Java 容器时，经常会有如下类似的内容：
```dockerfile
FROM maven
RUN mkdir /app
WORKDIR /app
COPY . /app
RUN mvn clean install
CMD "mvn" "exec:java"
```
```bash
$ docker build . -t java-application
$ docker run -p 8080:8080 java-application
```
这很简单，而且有效。但是，此镜像充满错误。<br />不仅应该了解如何正确使用 Maven，而且还应避免像上述示例那样构建 Java 容器。<br />下面，开始逐步改进这个Dockerfile，使Java应用程序生成高效，安全的Docker镜像。
<a name="EjNE7"></a>
### **1、Docker 镜像使用确定性的标签**
当使用 Maven 构建 Java 容器镜像时，首先需要基于 Maven 镜像。但是，使用 Maven 基本镜像时实际上引入了哪些内容吗？<br />当使用下面的代码行构建镜像时，将获得该 Maven 镜像的最新版本：
```dockerfile
FROM maven
```
**这似乎是一个有趣的功能，但是这种采用 Maven 默认镜像的策略可能存在一些潜在问题：**

- **你的 Docker 构建不是幂等的。这意味着每次构建的结果可能会完全不同，今天的最新镜像可能不同于明天或下周的最新镜像，导致应用程序的字节码也是不同的，并且可能发生意外。因此，构建镜像时，希望具有可复制的确定性行为；**
- **Maven Docker 镜像是基于完整的操作系统镜像。这样会导致许多其他二进制文件出现在最终的生产镜像中，但是运行 Java 应用程序不需要很多这些二进制文件。因此，将它们作为 Java 容器镜像的一部分存在一些缺点：1) 镜像体积变大，导致更长的下载和构建时间。2) 额外的二进制文件可能会引入安全漏洞。**

**如何解决？**

- **使用适合需求的最小基础镜像。考虑一下——是否需要一个完整的操作系统（包括所有额外的二进制文件）来运行程序？如果没有，也许基于 alpine 镜像或 Debian 的镜像会更好；**
- **使用特定的镜像 如果使用特定的镜像，则已经可以控制和预测某些行为。如果使用 maven:3.6.3-jdk-11-slim 镜像，则已经确定正在使用 JDK 11 和 Maven 3.6.3。JDK 和 Maven 的更新，将不再影响 Java 容器的行为。为了更加精确，也可以使用镜像的 SHA256 哈希值。使用哈希将确保每次构建镜像时都使用完全相同的基础镜像。**

用这些知识更新 Dockerfile：
```dockerfile
FROM maven:3.6.3-jdk-11-slim@sha256:68ce1cd457891f48d1e137c7d6a4493f60843e84c9e2634e3df1d3d5b381d36c
RUN mkdir /app
WORKDIR /app
COPY . /app
RUN mvn clean package -DskipTests
```
<a name="gz2Tj"></a>
### **2、在 Java 镜像中仅安装需要的内容**
以下命令会在容器中构建 Java 程序，包括其所有依赖项。这意味着源代码和构建系统都将会是 Java 容器的一部分。
```dockerfile
RUN mvn clean package -DskipTests
```
都知道 Java 是一种编译语言。这意味着只需要由构建环境创建的工件，而不需要代码本身。这也意味着构建环境不应成为 Java 镜像的一部分。<br />要运行 Java 镜像，也不需要完整的 JDK。一个 Java 运行时环境（JRE）就足够了。因此，从本质上讲，如果它是可运行的 JAR，那么只需要使用 JRE 和已编译的 Java 构件来构建镜像。<br />使用 Maven 在 CI 流水线中都构建编译程序，然后将JAR复制到镜像中，如下面的更新的 Dockerfile 中所示：
```dockerfile
FROM openjdk:11-jre-slim@sha256:31a5d3fa2942eea891cf954f7d07359e09cf1b1f3d35fb32fedebb1e3399fc9e
RUN mkdir /app
COPY ./target/java-application.jar /app/java-application.jar
WORKDIR /app
CMD "java" "-jar" "java-application.jar"
```
<a name="TK8Zz"></a>
### **3、使用多阶段构建 Java 镜像**
在本文的前面，谈到了不需要在容器中构建 Java 应用程序。但是，在某些情况下，将应用程序构建为 Docker 镜像的一部分很方便。<br />可以将 Docker 镜像的构建分为多个阶段。可以使用构建应用程序所需的所有工具来构建镜像，并在最后阶段创建实际的生产镜像。
```dockerfile
FROM maven:3.6.3-jdk-11-slim@sha256:68ce1cd457891f48d1e137c7d6a4493f60843e84c9e2634e3df1d3d5b381d36c AS build
RUN mkdir /project
COPY . /project
WORKDIR /project
RUN mvn clean package -DskipTests

FROM adoptopenjdk/openjdk11:jre-11.0.9.1_1-alpine@sha256:b6ab039066382d39cfc843914ef1fc624aa60e2a16ede433509ccadd6d995b1f
RUN mkdir /app
COPY --from=build /project/target/java-application.jar /app/java-application.jar
WORKDIR /app
CMD "java" "-jar" "java-application.jar"
```
<a name="URhzA"></a>
### **4、防止敏感信息泄漏**
在创建 Java 应用程序和 Docker 镜像时，很有可能需要连接到私有仓库，类似 settings.xml 的配置文件经常会泄露敏感信息。但在使用多阶段构建时，可以安全地将 settings.xml 复制到构建容器中。带有凭据的设置将不会出现在最终镜像中。此外，如果将凭据用作命令行参数，则可以在构建镜像中安全地执行此操作。<br />使用多阶段构建，可以创建多个阶段，仅将结果复制到最终的生产镜像中。这种分离是确保在生产环境中不泄漏数据的一种方法。<br />使用 `docker history` 命令查看 Java 镜像的输出：
```bash
$ docker history java-application
```
输出仅显示来自容器镜像的信息，而不显示构建镜像的过程。
<a name="M7J0P"></a>
### **5、不要以 root 用户运行容器**
创建 Docker 容器时，需要应用最小特权原则，防止由于某种原因攻击者能够入侵应用程序，则不希望他们能够访问所有内容。<br />拥有多层安全性，可以减少系统威胁。因此，必须确保不以 root 用户身份运行应用程序。<br />但默认情况下，创建 Docker 容器时，将以 root 身份运行它。尽管这对于开发很方便，但是不希望在生产镜像中使用它。假设由于某种原因，攻击者可以访问终端或可以执行代码。在那种情况下，它对正在运行的容器具有显著的特权，并且访问主机文件系统。<br />解决方案非常简单。创建一个有限特权的特定用户来运行应用程序，并确保该用户可以运行该应用程序。最后，在运行应用程序之前，不要忘记使用新创建的用户。<br />相应地更新 Dockerfile。
```dockerfile
FROM maven:3.6.3-jdk-11-slim@sha256:68ce1cd457891f48d1e137c7d6a4493f60843e84c9e2634e3df1d3d5b381d36c AS build
RUN mkdir /project
COPY . /project
WORKDIR /project
RUN mvn clean package -DskipTests

FROM adoptopenjdk/openjdk11:jre-11.0.9.1_1-alpine@sha256:b6ab039066382d39cfc843914ef1fc624aa60e2a16ede433509ccadd6d995b1f
RUN mkdir /app
RUN addgroup --system javauser && adduser -S -s /bin/false -G javauser javauser
COPY --from=build /project/target/java-application.jar /app/java-application.jar
WORKDIR /app
RUN chown -R javauser:javauser /app
USER javauser
CMD "java" "-jar" "java-application.jar"
```
<a name="EbNzf"></a>
### **6、Java 应用程序不要使用 PID 为 1 的进程**
在许多示例中，看到了使用构建环境来启动容器化 Java 应用程序的常见错误。<br />上面，了解了要在  Java 容器中使用 Maven 或 Gradle 的重要性，但是使用如下命令，会有不同的效果：
```dockerfile
CMD "mvn" "exec:java"

CMD ["mvn", "spring-boot run"]

CMD "gradle" "bootRun"

CMD "run-app.sh"
```
在 Docker 中运行应用程序时，第一个应用程序将以进程 ID 为 1（PID=1）运行。Linux内核会以特殊方式处理 PID 为 1 的进程。通常，进程号为 1 的 PID 上的过程是初始化过程。如果使用 Maven 运行 Java 应用程序，那么如何确定 Maven 将类似 SIGTERM 信号转发给 Java 进程呢？<br />如果像下面的示例，那样运行 Docker 容器，则Java应用程序将具有 PID 为 1 的进程。
```dockerfile
CMD "java" "-jar" "application.jar"
```
请注意，`docker kill` 和 `docker stop` 命令仅向 PID 为 1 的容器进程发送信号。例如，如果正在运行 Java 应用的 shell 脚本，`/bin/sh` 不会将信号转发给子进程。<br />更为重要的是，在 Linux 中，PID 为 1 的容器进程还有一些其他职责。因此，在某些情况下，不希望应用程序成为 PID 为 1 的进程，因为不知道如何处理这些问题。一个很好的解决方案是使用 dumb-init。
```dockerfile
RUN apk add dumb-init
CMD "dumb-init" "java" "-jar" "java-application.jar"
```
像这样运行 Docker 容器时，dumb-init 会占用 PID 为1的容器进程并承担所有责任。Java 流程不再需要考虑这一点。<br />更新后的 Dockerfile 现在看起来像这样：
```dockerfile
FROM maven:3.6.3-jdk-11-slim@sha256:68ce1cd457891f48d1e137c7d6a4493f60843e84c9e2634e3df1d3d5b381d36c AS build
RUN mkdir /project
COPY . /project
WORKDIR /project
RUN mvn clean package -DskipTests

FROM adoptopenjdk/openjdk11:jre-11.0.9.1_1-alpine@sha256:b6ab039066382d39cfc843914ef1fc624aa60e2a16ede433509ccadd6d995b1f
RUN apk add dumb-init
RUN mkdir /app
RUN addgroup --system javauser && adduser -S -s /bin/false -G javauser javauser
COPY --from=build /project/target/java-code-workshop-0.0.1-SNAPSHOT.jar /app/java-application.jar
WORKDIR /app
RUN chown -R javauser:javauser /app
USER javauser
CMD "dumb-init" "java" "-jar" "java-application.jar"
```
<a name="kXIDV"></a>
### **7、优雅下线 Java 应用程序**
当应用程序收到关闭信号时，理想情况下，希望所有内容都能正常关闭。根据开发应用程序的方式，中断信号（SIGINT）或 CTRL + C 可能导致立即终止进程。<br />这可能不是你想要的东西，因为诸如此类的事情可能会导致意外行为，甚至导致数据丢失。<br />当你将应用程序作为 Payara 或 Apache Tomcat 之类的 Web 服务器的一部分运行时，该 Web 服务器很可能会正常关闭。对于某些支持可运行应用程序的框架也是如此。例如，Spring Boot 具有嵌入式 Tomcat 版本，可以有效地处理关机问题。<br />当创建一个独立的 Java 应用程序或手动创建一个可运行的 JAR 时，必须自己处理这些中断信号。<br />解决方案非常简单。添加一个退出钩子（hook），如下面的示例所示。收到类似 SIGINT 信号后，优雅下线应用程序的进程将会被启动。
```java
Runtime.getRuntime().addShutdownHook(new Thread() {
    @Override
    public void run() {
        System.out.println("Inside Add Shutdown Hook");
    }
});
```
诚然，与 Dockerfile 相关的问题相比，这是一个通用的 Web 应用程序问题，但在容器环境中更重要。
<a name="ChRmH"></a>
### **8、使用 .dockerignore 文件**
为了防止不必要的文件污染 git 存储库，可以使用 .gitignore 文件。<br />对于 Docker 镜像，有类似的东西—— .dockerignore 文件。类似于 git 的忽略文件，它是为了防止 Docker 镜像中出现不需要的文件或目录。同时，也不希望敏感信息泄漏到 Docker 镜像中。<br />请参阅以下示例的 .dockerignore：
```
.dockerignore
**/*.logDockerfile
.git
.gitignore
```
使用 .dockerignore 文件的要点是：

- **跳过仅用于测试目的的依赖项；**
- **使你免于泄露密钥或凭据信息进入 Java Docker 镜像的文件；**
- **另外，日志文件也可能包含不想公开的敏感信息；**
- **保持 Docker 镜像的美观和整洁，本质上是使镜像变小。除此之外，它还有助于防止意外行为。**
<a name="eDeka"></a>
### **9、确保 Java 版本支持容器**
Java 虚拟机（JVM）是一件了不起的事情。它会根据其运行的系统进行自我调整。有基于行为的调整，可以动态优化堆的大小。但是，在 Java 8 和 Java 9 等较旧的版本中，JVM 无法识别容器设置的CPU限制或内存限制。这些较旧的 Java 版本的 JVM 看到了主机系统上的全部内存和所有 CPU 容量。Docker 设置的限制将被忽略。<br />随着 Java 10 的发布，JVM 现在可以感知容器，并且可以识别容器设置的约束。该功能 `UseContainerSupport` 是 JVM 标志，默认情况下设置为活动状态。Java 10 中发布的容器感知功能也已移植到 Java-8u191。<br />对于 Java 8 之前的版本，可以手动尝试使用该 -Xmx 标志来限制堆大小，但这是一个痛苦的练习。紧接着，堆大小不等于 Java 使用的内存。对于 Java-8u131 和 Java 9，容器感知功能是实验性的，必须主动激活。
```bash
-XX：+UnlockExperimentalVMOptions -XX：+UseCGroupMemoryLimitForHeap
```
最好的选择是将 Java 更新到 10 以上的版本，以便默认情况下支持容器。不幸的是，许多公司仍然严重依赖 Java 8。这意味着应该在 Docker 镜像中更新到 Java 的最新版本，或者确保至少使用 Java 8 update 191 或更高版本。
<a name="H4LqT"></a>
### **10、谨慎使用容器自动化生成工具**
可能会偶然发现适用于构建系统的出色工具和插件。除了这些插件，还有一些很棒的工具可以创建 Java 容器，甚至可以根据需要自动发布应用。<br />从开发人员的角度来看，这看起来很棒，因为不必在创建实际应用程序时，还要花费精力维护 Dockerfile。<br />这样的插件的一个例子是 JIB。如下所示，只需要调用 `mvn jib:dockerBuild` 命令可以构建镜像：
```xml
<plugin>
  <groupId>com.google.cloud.tools</groupId>
  <artifactId>jib-maven-plugin</artifactId>
  <version>2.7.1</version>
  <configuration>
    <to>
      <image>myimage</image>
    </to>
  </configuration>
</plugin>
```
它将构建一个具有指定名称的 Docker 镜像，而没有任何麻烦。<br />使用 2.3 及更高版本时，可以通过调用 mvn 命令进行操作：
```bash
mvn spring-boot：build-image
```
在这种情况下，系统都会自动创建一个 Java 镜像。这些镜像还比较小，那是因为他们正在使用非发行版镜像或 buildpack 作为镜像的基础。但是，无论镜像大小如何，如何知道这些容器是安全的？需要进行更深入的调查，即使这样，也不确定将来是否会保持这种状态。<br />并不是说在创建 Java Docker 时不应使用这些工具。但是，如果打算发布这些镜像，则应研究 Java 镜像所有方面的安全。镜像扫描将是一个好的开始。从安全性的角度来看，以完全控制和正确的方式创建 Dockerfile，是创建镜像更好，更安全的方式。
