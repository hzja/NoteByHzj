SpringBoot<br />随着Spring Boot的流行，大家体验到只需构建输出一个jar文件，然后只需一个java -jar命令就能部署运行应用的爽快。常见一些单体应用随着项目规模的扩展单个jar文件的大小越来越大，动辄两三百MB。如果再引入微服务架构，动辄一二十个微服务，所有模块jar加起来整个系统光部署文件就一两个GB。<br />一个系统一旦上线运行，无论新需求迭代还是Bug修复，免不了需要做部署更新，尤其对于一些交付类型项目，首次部署或异地更新， 动不动就需要传输几百MB或几个GB的部署文件，确实是一个让人头疼的问题。<br />可以想象一下，线上系统发现一个紧急严重Bug捅到了主管那里，交代马上紧急修复解决，研发同事火速分析排查分分钟搞定提交代码并完成构建打包并交付给运维。过一会领导着急上火来过问问题更新解决了吗？运维只能很尴尬的回答：还没呢，部署包文件比较大，正在上传有点慢...
<a name="4pykK"></a>
### 1、正常打包SpringBoot项目-得到lib jar包目录
```bash
mvn package -DskipTests
```
将打包的jar包进行解压，得到lib文件夹<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600702116725-f3cb2d8c-0753-4092-acf4-49eb657e1eb1.png#align=left&display=inline&height=454&originHeight=1363&originWidth=2018&size=532889&status=done&style=shadow&width=672.6666666666666)
<a name="hVaf5"></a>
### 2、修改pom文件配置，编译不打包lib文件的jar包
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-maven-plugin</artifactId>
      <configuration>
        <mainClass>com.fcant.nginxloadbalance.NginxLoadBalanceApplication</mainClass>
        <includes>
          <include>
            <groupId>nothing</groupId>
            <artifactId>nothing</artifactId>
          </include>
        </includes>
      </configuration>
      <executions>
        <execution>
          <goals>
            <goal>repackage</goal>
          </goals>
        </execution>
      </executions>
    </plugin>
  </plugins>
</build>
```
配置完成后重新执行打包
```bash
mvn package -DskipTests
```
得到未引入lib目录的jar包<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600702825568-6f0a01a2-4bc7-4939-8228-210057e1ef78.png#align=left&display=inline&height=454&originHeight=1363&originWidth=2018&size=278643&status=done&style=shadow&width=672.6666666666666)
<a name="r3Rcw"></a>
### 3、运行编译后的jar包
将步骤一解压出来的 lib 文件夹、步骤二编译的 jar 包放在同一个目录, 运行下面命令：
```bash
java -Dloader.path=/path/to/lib -jar /path/to/springboot-jsp-0.0.1-SNAPSHOT.jar
```
或者在 maven 中输入一下命令导出需要用到的 jar 包
```
mvn dependency:copy-dependencies -DoutputDirectory=F:\ideaWorkPlace\AnalysisEngine\lib  -DincludeScope=runtime
```
备注：<br />将 / path/to / 改成实际的路径。<br />-Dloader.path=lib 文件夹路径<br />最终目录文件结构是：
```
├── lib   #lib文件夹
└── springboot-jsp-0.0.1-SNAPSHOT.jar
```
说明<br />1、通常，一个工程项目架构确定后，引入的 jar 包基本上不会变，改变的大部分是业务逻辑；<br />2、后面如果需要变更业务逻辑，只需要轻量地编译工程，大大提高项目部署的效率。
<a name="9Yrzt"></a>
### Level 0：常规的Fat Jar构建
参考项目目录：package-optimize-level0
<a name="eXUVt"></a>
#### 主要配置
```xml
<build>
    <finalName>${project.artifactId}</finalName>
    <!--
    特别注意：
    项目仅仅是为了演示配置方便，直接在parent的build部分做了插件配置和运行定义。
    但是实际项目中需要把这些定义只放到spring boot模块项目（可优化使用pluginManagement形式），避免干扰其他util、common等模块项目
    -->
    <plugins>
        <plugin>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-maven-plugin</artifactId>
        </plugin>
    </plugins>
</build>
```
<a name="a8lgi"></a>
#### 配置输出
```bash
cd package-optimize-level0
mvn clean install
ls -lh package-optimize-app1/target/package-optimize-app1.jar
-rw-r--r--  1 lixia  wheel    16M Feb 24 21:06 package-optimize-app1/target/package-optimize-app1.jar
java -jar package-optimize-app1/target/package-optimize-app1.jar
```
<a name="cmflJ"></a>
#### 重点说明

- （当前演示应用仅依赖了spring-boot-starter-web极少组件，所有构建输出只有十来MB）实际情况单一构建根据项目依赖组件量输出jar一般在几十MB到一两百MB甚至更大。
- 假如有十来个微服务需要部署，那就意味着需要传输一两个GB的文件，耗时可想而知。就算是单一更新个别微服务也需要传输一两百MB。
<a name="m7gOa"></a>
### Level 1：常见的依赖jar分离构建方式
参考项目目录：package-optimize-level1
<a name="169a3f39"></a>
#### 解决问题

- 降低单个微服务jar的文件大小，以便部署过程秒传文件。
<a name="wrZKG"></a>
#### 主要配置
重点配置说明请详见如下注释说明：
```xml
<build>
    <finalName>${project.artifactId}</finalName>
    <!--
    特别注意：
    项目仅仅是为了演示配置方便，直接在parent的build部分做了插件配置和运行定义。
    但是实际项目中需要把这些定义只放到spring boot模块项目（可优化使用pluginManagement形式），避免干扰其他util、common等模块项目
    -->
    <plugins>
        <!-- 拷贝项目所有依赖jar文件到构建lib目录下 -->
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-dependency-plugin</artifactId>
            <executions>
                <execution>
                    <id>copy-dependencies</id>
                    <phase>package</phase>
                    <goals>
                        <goal>copy-dependencies</goal>
                    </goals>
                    <configuration>
                        <outputDirectory>${project.build.directory}/lib</outputDirectory>
                        <excludeTransitive>false</excludeTransitive>
                        <stripVersion>false</stripVersion>
                        <silent>true</silent>
                    </configuration>
                </execution>
            </executions>
        </plugin>
        <!-- Spring Boot模块jar构建 -->
        <plugin>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-maven-plugin</artifactId>
            <configuration>
                <includes>
                    <!-- 不存在的include引用，相当于排除所有maven依赖jar，没有任何三方jar文件打入输出jar -->
                    <include>
                        <groupId>null</groupId>
                        <artifactId>null</artifactId>
                    </include>
                </includes>
                <layout>ZIP</layout>
            </configuration>
            <executions>
                <execution>
                    <goals>
                        <goal>repackage</goal>
                    </goals>
                </execution>
            </executions>
        </plugin>
    </plugins>
</build>
```
<a name="fYQ7a"></a>
#### 配置输出
```bash
cd package-optimize-level1
mvn clean install
ls -lh package-optimize-app1/target/package-optimize-app1.jar
-rw-r--r--  1 lixia  wheel   149K Feb 24 20:56 package-optimize-app1/target/package-optimize-app1.jar
java -jar -Djava.ext.dirs=lib package-optimize-app1/target/package-optimize-app1.jar
```
<a name="EuCkW"></a>
#### 实现效果

- 单一构建根据项目依赖组件量输出jar一般仅有一两百KB，基本可以做到秒传。
- 这个是网上可见最常见的优化方案，还值得继续深入：假如有十来个微服务，每个服务一个jar和一个lib目录文件，首次部署也差不多需要传输一两个GB文件。
<a name="N8flh"></a>
### Level 2：合并所有模块依赖jar到同一个lib目录
参考项目目录：package-optimize-level2
<a name="1UU7L"></a>
#### 解决问题

- 合并所有模块依赖jar到同一个lib目录，一般由于各模块项目依赖jar重叠程度很高，合并所有服务部署文件总计大小基本也就两三百MB
- 但是如果采用-Djava.ext.dirs=lib加载所有jar到每个JVM，一来每个JVM都完整加载了所有jar耗费资源，二来各微服务组件版本不同会出现版本冲突问题
<a name="f50a44a6"></a>
#### 主要配置
重点配置说明请详见如下注释说明：
```xml
<build>
    <finalName>${project.artifactId}</finalName>
    <!--
    特别注意：
    项目仅仅是为了演示配置方便，直接在parent的build部分做了插件配置和运行定义。
    但是实际项目中需要把这些定义只放到spring boot模块项目（可优化使用pluginManagement形式），避免干扰其他util、common等模块项目
    -->
    <plugins>
        <!-- 基于maven-jar-plugin插件实现把依赖jar定义写入输出jar的META-INFO/MANIFEST文件 -->
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-jar-plugin</artifactId>
            <configuration>
                <archive>
                    <manifest>
                        <addClasspath>true</addClasspath>
                        <classpathPrefix>lib/</classpathPrefix>
                        <useUniqueVersions>false</useUniqueVersions>
                    </manifest>
                </archive>
            </configuration>
        </plugin>
        <!-- 拷贝项目所有依赖jar文件到构建lib目录下 -->
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-dependency-plugin</artifactId>
            <executions>
                <execution>
                    <id>copy-dependencies</id>
                    <phase>package</phase>
                    <goals>
                        <goal>copy-dependencies</goal>
                    </goals>
                    <configuration>
                        <!--
                        各子模块按照实际层级定义各模块对应的属性值，检查所有微服务模块依赖jar文件合并复制到同一个目录
                        详见各子模块中 boot-jar-output 属性定义
                        -->
                        <outputDirectory>${boot-jar-output}/lib</outputDirectory>
                        <excludeTransitive>false</excludeTransitive>
                        <stripVersion>false</stripVersion>
                        <silent>false</silent>
                    </configuration>
                </execution>
            </executions>
        </plugin>
        <!-- Spring Boot模块jar构建 -->
        <plugin>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-maven-plugin</artifactId>
            <configuration>
                <includes>
                    <!-- 不存在的include引用，相当于排除所有maven依赖jar，没有任何三方jar文件打入输出jar -->
                    <include>
                        <groupId>null</groupId>
                        <artifactId>null</artifactId>
                    </include>
                </includes>
                <layout>ZIP</layout>
                <!--
                基于maven-jar-plugin输出微服务jar文件进行二次spring boot重新打包文件的输出目录
                所有微服务构建输出jar文件统一输出到与lib同一个目录，便于共同引用同一个lib目录
                详见各子模块中boot-jar-output属性定义
                -->
                <!--  -->
                <outputDirectory>${boot-jar-output}</outputDirectory>
            </configuration>
            <executions>
                <execution>
                    <goals>
                        <goal>repackage</goal>
                    </goals>
                </execution>
            </executions>
        </plugin>
    </plugins>
</build>
```
所有lib目录文件及各微服务构建jar聚合到devops公共目录。<br />微服务jar文件中的META-INFO/MANIFEST文件中会生成根据模块依赖组件列表的Class-Path属性， 从而避免了不同版本jar：
```
Class-Path: lib/spring-boot-starter-web-2.4.3.jar lib/spring-boot-starte
 r-2.4.3.jar lib/spring-boot-2.4.3.jar lib/spring-boot-autoconfigure-2.4
 .3.jar lib/spring-boot-starter-logging-2.4.3.jar lib/logback-classic-1.
 2.3.jar lib/logback-core-1.2.3.jar lib/slf4j-api-1.7.30.jar lib/log4j-t
 o-slf4j-2.13.3.jar lib/log4j-api-2.13.3.jar lib/jul-to-slf4j-1.7.30.jar
  lib/jakarta.annotation-api-1.3.5.jar lib/spring-core-5.3.4.jar lib/spr
 ing-jcl-5.3.4.jar lib/snakeyaml-1.27.jar lib/spring-boot-starter-json-2
 .4.3.jar lib/jackson-databind-2.11.4.jar lib/jackson-annotations-2.11.4
 .jar lib/jackson-core-2.11.4.jar lib/jackson-datatype-jdk8-2.11.4.jar l
 ib/jackson-datatype-jsr310-2.11.4.jar lib/jackson-module-parameter-name
 s-2.11.4.jar lib/spring-boot-starter-tomcat-2.4.3.jar lib/tomcat-embed-
 core-9.0.43.jar lib/jakarta.el-3.0.3.jar lib/tomcat-embed-websocket-9.0
 .43.jar lib/spring-web-5.3.4.jar lib/spring-beans-5.3.4.jar lib/spring-
 webmvc-5.3.4.jar lib/spring-aop-5.3.4.jar lib/spring-context-5.3.4.jar 
 lib/spring-expression-5.3.4.jar
```
<a name="PheoF"></a>
#### 配置输出
```bash
cd package-optimize-level2
mvn clean install
ls -lh devops/
total 912
drwxr-xr-x  34 lixia  wheel   1.1K Feb 24 22:27 lib
-rw-r--r--   1 lixia  wheel   150K Feb 24 22:31 package-optimize-app1.jar
-rw-r--r--   1 lixia  wheel   149K Feb 24 22:31 package-optimize-app2.jar
-rw-r--r--   1 lixia  wheel   149K Feb 24 22:31 package-optimize-app3.jar
java -jar devops/package-optimize-app1.jar
```
<a name="PwDrb"></a>
#### 实现效果

- 启动过程不再需要 `-Djava.ext.dirs=lib` 参数定义。
- 所有微服务jar引用所有项目合并依赖组件的公共目录，部署文件总计大小一般在两三百MB。
- 通过定制每个微服务jar文件中的META-INFO/MANIFEST文件中的Class-Path明确指明依赖版本组件类，解决各微服务不同组件版本冲突问题。
<a name="DJemq"></a>
### Level 3：支持system引入的非官方的三方依赖组件
参考项目目录：package-optimize-level3
<a name="PtBp6"></a>
#### 解决问题

- 有些非官方三方的诸如sdk jar，一种做法是提交到Maven本地私服中去引用，那和普通依赖jar处理相同；但是在没有Maven私服的情况下，常见的简化做法都是直接在项目中放置依赖jar然后在pom中以system scope方式定义。
- 对于在pom中是以systemPath方式引入的，maven-jar-plugin组件没有直接参数声明包含指定scope的组件， 如果不做特殊处理META-INFO/MANIFEST中不会出现这些scope定义的组件，导致运行时类找不到。
<a name="mJMTs"></a>
#### 主要配置
重点配置说明请详见如下注释说明：
```xml
<build>
    <finalName>${project.artifactId}</finalName>
    <!--
    特别注意：
    项目仅仅是为了演示配置方便，直接在parent的build部分做了插件配置和运行定义。
    但是实际项目中需要把这些定义只放到spring boot模块项目（可优化使用pluginManagement形式），避免干扰其他util、common等模块项目
    -->
    <plugins>
        <!-- 基于maven-jar-plugin插件实现把依赖jar定义写入输出jar的META-INFO/MANIFEST文件 -->
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-jar-plugin</artifactId>
            <configuration>
                <archive>
                    <manifest>
                        <addClasspath>true</addClasspath>
                        <classpathPrefix>lib/</classpathPrefix>
                        <useUniqueVersions>false</useUniqueVersions>
                    </manifest>
                    <manifestEntries>
                        <!--
                        有些非官方三方的诸如sdk jar在pom中是以systemPath方式引入的，maven-jar-plugin组件没有直接参数声明包含指定scope的组件
                        通过使用额外定义 Class-Path 值来追加指定依赖组件列表，在子模块按实际情况指定 jar-manifestEntries-classpath 值即可
                        例如(注意前面个点字符及各空格分隔符)：. lib/xxx-1.0.0.jar lib/yyy-2.0.0.jar
                        详见各子模块中 boot-jar-output 属性定义示例
                        -->
                        <Class-Path>${jar-manifestEntries-classpath}</Class-Path>
                    </manifestEntries>
                </archive>
            </configuration>
        </plugin>
        <!-- 拷贝项目所有依赖jar文件到构建lib目录下 -->
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-dependency-plugin</artifactId>
            <executions>
                <execution>
                    <id>copy-dependencies</id>
                    <phase>package</phase>
                    <goals>
                        <goal>copy-dependencies</goal>
                    </goals>
                    <configuration>
                        <!--
                        各子模块按照实际层级定义各模块对应的属性值，检查所有微服务模块依赖jar文件合并复制到同一个目录
                        详见各子模块中 boot-jar-output 属性定义
                        -->
                        <outputDirectory>${boot-jar-output}/lib</outputDirectory>
                        <excludeTransitive>false</excludeTransitive>
                        <stripVersion>false</stripVersion>
                        <silent>false</silent>
                    </configuration>
                </execution>
            </executions>
        </plugin>
        <!-- Spring Boot模块jar构建 -->
        <plugin>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-maven-plugin</artifactId>
            <configuration>
                <includes>
                    <!-- 不存在的include引用，相当于排除所有maven依赖jar，没有任何三方jar文件打入输出jar -->
                    <include>
                        <groupId>null</groupId>
                        <artifactId>null</artifactId>
                    </include>
                </includes>
                <layout>ZIP</layout>
                <!--
                基于maven-jar-plugin输出微服务jar文件进行二次spring boot重新打包文件的输出目录
                所有微服务构建输出jar文件统一输出到与lib同一个目录，便于共同引用同一个lib目录
                详见各子模块中boot-jar-output属性定义
                -->
                <!--  -->
                <outputDirectory>${boot-jar-output}</outputDirectory>
            </configuration>
            <executions>
                <execution>
                    <goals>
                        <goal>repackage</goal>
                    </goals>
                </execution>
            </executions>
        </plugin>
    </plugins>
</build>
```
子模块主要配置：
```xml
<properties>
        <!-- 按各模块实际目录层次定义相对数据，使所有服务模块输出资源汇聚到相同目录 -->
        <boot-jar-output>../devops</boot-jar-output>
        <!--
        有些供应商的sdk jar在pom中是以systemPath方式引入的，maven-jar-plugin组件没有直接参数声明包含指定scope的组件
        通过使用额外定义 Class-Path 值来追加指定依赖组件列表，按实际情况指定 jar-manifestEntries-classpath 值即可
        例如(注意前面个点字符及各空格分隔符，lib后面部分是 artifactId-version.jar 格式而不是实际文件名)：. lib/xxx-1.0.0.jar lib/yyy-2.0.0.jar
        -->
        <jar-manifestEntries-classpath>. lib/hik-sdk-1.0.0.jar</jar-manifestEntries-classpath>
    </properties>
    <dependencies>
        <!-- 以相对路径方式定义非官方三方依赖组件 -->
        <dependency>
            <groupId>com.hik</groupId>
            <artifactId>hik-sdk</artifactId>
            <version>1.0.0</version>
            <scope>system</scope>
            <systemPath>${project.basedir}/lib/hik-sdk-1.0.0.jar</systemPath>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
    </dependencies>
```
微服务输出jar文件中的META-INFO/MANIFEST文件中会生成根据模块依赖组件列表的Class-Path属性， 最前面会追加 jar-manifestEntries-classpath 属性定义值：
```
Class-Path: . lib/hik-sdk-1.0.0.jar lib/spring-boot-starter-web-2.4.3.ja
 r lib/spring-boot-starter-2.4.3.jar lib/spring-boot-2.4.3.jar lib/sprin
 g-boot-autoconfigure-2.4.3.jar lib/spring-boot-starter-logging-2.4.3.ja
 r lib/logback-classic-1.2.3.jar lib/logback-core-1.2.3.jar lib/slf4j-ap
 i-1.7.30.jar lib/log4j-to-slf4j-2.13.3.jar lib/log4j-api-2.13.3.jar lib
 /jul-to-slf4j-1.7.30.jar lib/jakarta.annotation-api-1.3.5.jar lib/sprin
 g-core-5.3.4.jar lib/spring-jcl-5.3.4.jar lib/snakeyaml-1.27.jar lib/sp
 ring-boot-starter-json-2.4.3.jar lib/jackson-databind-2.11.4.jar lib/ja
 ckson-annotations-2.11.4.jar lib/jackson-core-2.11.4.jar lib/jackson-da
 tatype-jdk8-2.11.4.jar lib/jackson-datatype-jsr310-2.11.4.jar lib/jacks
 on-module-parameter-names-2.11.4.jar lib/spring-boot-starter-tomcat-2.4
 .3.jar lib/tomcat-embed-core-9.0.43.jar lib/jakarta.el-3.0.3.jar lib/to
 mcat-embed-websocket-9.0.43.jar lib/spring-web-5.3.4.jar lib/spring-bea
 ns-5.3.4.jar lib/spring-webmvc-5.3.4.jar lib/spring-aop-5.3.4.jar lib/s
 pring-context-5.3.4.jar lib/spring-expression-5.3.4.jar
```
<a name="ie0e8"></a>
#### 配置输出
```bash
cd package-optimize-level3
mvn clean install
ls -lh devops/
total 912
drwxr-xr-x  36 lixia  wheel   1.1K Feb 24 23:14 lib
-rw-r--r--@  1 lixia  wheel   150K Feb 24 23:14 package-optimize-app1.jar
-rw-r--r--   1 lixia  wheel   150K Feb 24 23:14 package-optimize-app2.jar
-rw-r--r--   1 lixia  wheel   150K Feb 24 23:14 package-optimize-app3.jar
java -jar devops/package-optimize-app1.jar
```
<a name="pySKy"></a>
### 最终实现效果

- 所有服务的依赖组件合并到一个目录，总计大小在两三百MB，首次部署传输效率明显提速。
- 各微服务jar一两百KB大小，日常紧急修复Bug更新个别jar基本就是瞬间秒传。
- 各微服务jar中各自定义依赖指定版本组件列表，不会出现组件不同版本加载冲突问题。
- 非官方的三方依赖组件也能正常引用处理。
<a name="8kh5Z"></a>
### 特别提示
上述通过部署组件分离处理后，日常更新只需要传输一两百KB的业务jar文件即可。但是如果某个项目的maven依赖组件做了变更配置，则需要注意把变更的jar文件要同步到公共的lib目录。<br />最小化变更jar文件的小技巧：可以把构建部署资源目录提交到GIT库，以后每次版本发布同时commit到GIT库， 通过提交视图可以清晰的识别出lib目录下和业务jar本次版本发布的变更文件清单，包括微服务jar和依赖jar变更文件，以此最小化传输文件。
