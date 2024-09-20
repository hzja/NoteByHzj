Java SpringBoot Docker
<a name="dOQqG"></a>
## Spring Boot Docker
在Spring Boot应用中，可以约定不同的标识来定义不同的环境。例如dev表示开发环境、test表示测试环境，对应的配置文件为application-dev.yaml、application-test.yaml。通过声明`spring.profiles.active`来激活对应的环境配置，例如激活dev环境时`spring.profiles.active=dev`。完整的启动命令为：
```bash
java -Djava.security.egd=file:/dev/./urandom  -Dspring.profiles.active=dev -jar spring-boot-app.jar 
```
根据上面的命令编写一个能够适应多环境的Dockerfile：
```dockerfile
# 引入 openjdk 镜像
FROM adoptopenjdk/openjdk8
# 声明作者
LABEL AUTHOR=fcant OG=fcant.cn
# 挂载几个有用的文件夹 比如日志
VOLUME ["/tmp","/logs"]
# 声明一个环境参数用来动态启用配置文件 默认dev
ENV ACTIVE=dev
# 暴露端口
EXPOSE 8080
# 复制并修改应用打包后的jar文件名称
ADD /target/flyway-spring-boot-1.0.0.jar app.jar
# 容器启动时第一个运行的命令 用来启动应用
ENTRYPOINT ["java","-Djava.security.egd=file:/dev/./urandom","-Dspring.profiles.active=${ACTIVE}","-jar","app.jar"]
```
这样打包的Docker镜像就可以通过`docker run`添加额外的`--env ACTIVE=test`来动态的改变环境。单纯的编写Dockerfile不方便DevOps。<br />![docker 镜像生命周期](https://cdn.nlark.com/yuque/0/2021/png/396745/1620366985146-b54fa221-5957-414b-9b4a-b9251812845e.png#clientId=u89e100f1-3332-4&from=ui&id=uff36793f&originHeight=412&originWidth=791&originalType=binary&ratio=1&rotation=0&showTitle=true&size=134405&status=done&style=shadow&taskId=uf61f49dd-389c-45a4-b694-76443b43a66&title=docker%20%E9%95%9C%E5%83%8F%E7%94%9F%E5%91%BD%E5%91%A8%E6%9C%9F "docker 镜像生命周期")<br />需要能够自动地构建、推送到仓库、拉取镜像、运行一系列流水线操作。好在市面上有很多工具来实现这一过程。
<a name="hakOs"></a>
### spring-boot-maven-plugin
这个是Spring Boot官方的插件，在2.x的某个版本提供了Docker镜像构建能力。
```xml
<project>
  <build>
    <plugins>
      <plugin>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-maven-plugin</artifactId>
        <configuration>
          <image>
            <name>docker.repo.com/library/${project.artifactId}:${project.version}</name>
            <publish>true</publish>
          </image>
          <docker>
            <publishRegistry>
              <username>user</username>
              <password>secret</password>
              <url>https://docker.repo.com/v1/</url>
              <email>user@example.com</email>
            </publishRegistry>
          </docker>
        </configuration>
      </plugin>
    </plugins>
  </build>
</project>
```
配置好Docker私仓后就可以通过`mvn clean spring-boot:build-image`进行构建镜像了。<br />这种方式好处就是无额外依赖，缺点就是需要从github下载构建元件，网络如果不好就容易失败。
<a name="RnPet"></a>
### Spotify Maven Plugin
Spotify Maven 插件是一个目前比较普遍的选择。它要求应用程序开发人员编写Dockerfile，并把Dockerfile放在项目src/main/docker目录下。然后就可以通过引入：
```xml
<plugin>
  <groupId>com.spotify</groupId>
  <artifactId>dockerfile-maven-plugin</artifactId>
  <version>1.4.8</version>
  <configuration>
    <repository>repo.com/${project.artifactId}</repository>
  </configuration>
</plugin>
```
这个插件提供了`mvn dockerfile:build`、`mvn dockerfile:tag`、`mvn dockerfile:push`三个命令分别用来构建、打标签、发布到远端私有仓库，非常简单。<br />这个是一个非常容易上手的插件，唯一的要求就是需要会编写Dockerfile，对定制化要求高的可以使用这个。
<a name="WRAuE"></a>
### Jib Maven Plugin
它是谷歌开源的OCI镜像打包工具，可以用来打包Docker镜像，大部分情况下已经满足需要。但是如果要定制化的话还是不容易的，需要阅读官方给的文档。最开始的Dockerfile如果使用JIb的话需要这样配置：
```xml
<plugin>
    <groupId>com.google.cloud.tools</groupId>
    <artifactId>jib-maven-plugin</artifactId>
    <version>3.0.0</version>
    <configuration>
        <from>
            <image>adoptopenjdk/openjdk8</image>
        </from>
        <to>
            <image>docker.repo.com/library/${project.artifactId}</image>
            <auth>
                <username>fcant</username>
                <password>xxxxxx</password>
            </auth>
            <tags>
                <tag>${project.version}</tag>
            </tags>
        </to>
        <extraDirectories>
            <paths>
                <path>
                    <from>target/${project.artifactId}-${project.version}.jar</from>
                    <includes>*.jar</includes>
                    <into>/app.jar</into>
                </path>
            </paths>
        </extraDirectories>
        <containerizingMode>packaged</containerizingMode>
        <container>
            <volumes>/tmp,/logs</volumes>
            <ports>
                <port>8080</port>
            </ports>
            <environment>
                <active>dev</active>
            </environment>
            <entrypoint>
                java,-Djava.security.egd=file:/dev/./urandom,-Dspring.profiles.active=${active},-jar,/app.jar
            </entrypoint>
            <creationTime>USE_CURRENT_TIMESTAMP</creationTime>
        </container>
    </configuration>
</plugin>
```
优点是不需要本地Docker环境，而且支持分层构建、镜像瘦身，上手容易；缺点是定制化比较困难。
