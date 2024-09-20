Java Maven Oracle JDBC Driver
<a name="vRksM"></a>
## 前言
**「由于 Oracle 并没有向公开 Maven 仓库提供任何 Oracle JDBC Driver 的 Jar 包」**，因此无法像 MySQL、SQLite 等那么轻松直接通过 Maven 加载依赖。而需要手动下载 Oracle JDBC Driver Jar 包，然后安装到本地仓库(`.m2目录或者自定义的localRepository`)，再通过 `Maven加载依赖` 则是常用手段。但此外还能通过`<scope>system</scope>`的方式引入。
<a name="ft3Af"></a>
## 手动安装到本地仓库

1. 将`ojdbc7-12.1.0.2.jar`放置到项目**「根目录的lib目录」**下，随项目进行版本管理；
2. 在POM.xml文件中加入依赖定义；
```xml
<dependency>
  <groupId>com.oracle</groupId>
  <artifactId>ojdbc7</artifactId>
  <version>12.1.0.2</version>
</dependency>
```

3. 安装JAR包到本地仓库。
```bash
# 在项目根目录下执行
mvn install:install-file -Dfile=./lib/ojdbc7-12.1.0.2.jar -DgroupId=com.oracle -DartifactId=ojdbc7 -Dversion=12.1.0.2 -Dpackaging=jar
```
<a name="wsycP"></a>
## scope为system在SpringBoot中的坑
除上述方式外，可以在`POM.xml`将依赖定义为
```xml
<dependency>
    <groupId>com.oracle</groupId>
    <artifactId>ojdbc7</artifactId>
    <version>12.1.0.2</version>
    <scope>system</scope>
    <systemPath>${project.basedir}/lib/ojdbc7-12.1.0.2.jar</systemPath>
</dependency>
```
那么即使本地仓库(.m2目录)下没有 Oracle JDBC Driver 依赖，执行`mvn compile`和`mvn spring-boot:run`依然成功执行。但**「请注意」**，执行`mvn package`打包出来的**「SpringBoot UberJar包中没有包含Oracle JDBC Driver依赖」**，直接部署到服务器上则报如下错误：
> ############ application failed to start
> Description: .................................. Reason: Failed to load driver class oracle.jdbc.driver.OracleDriver in either of HikariConfig class loader or Thread context classloader.
> Action: Update your application's configuration ############

解决方法：

1. 对于**「外置Web容器」**的SpringBoot应用，则将 Oracle JDBC Driver Jar 包放置在容器的lib目录下；
2. 对于**「内置Web容器」**的SpringBoot应用，则**「修改 spring-boot-maven-plugin 插件配置」**即可：
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-maven-plugin</artifactId>
      <configuration>
        <includeSystemScope>true</includeSystemScope>
      </configuration>
    </plugin>
  </plugins>
</build>
```
具体原因见下文。
<a name="MbKsL"></a>
## Maven依赖定义中的scope属性
作用：**「用于限制依赖在Maven项目各生命周期的作用范围。」**

- `compile`，默认值，依赖将参与编译阶段，并会被打包到最终发布包(如 Jar、War、UberJar )内的 Lib 目录下。具有传递性，即该依赖项对于依赖当前项目的其它项目同样生效；
- `provided`，依赖将参与编译阶段但不会被打包到最终的发布包，运行阶段由容器或 JDK 提供。不具备传递性。（如 Servlet API，JSP API，Lombok 等）；
- `runtime`，依赖不参与编译阶段（即不加入到 classpath），但会打包到最终发布包，从而参与运行和测试阶段。通常用于根据配置文件动态加载或接口反射加载的依赖（如 JDBC 驱动）；
- `test`，依赖参加编译阶段，但不打包到最终发布包，依赖仅参与测试阶段；
- `system`，表示该依赖项的路径为基于文件系统的 Jar 包路径，并且必须通过`systemPath`指定本地文件路径。依赖参与编译阶段，默认不会被打包到最终发布包。
- 对于Spring Boot项目，若要将 scope 为system 的 Jar 包打包到发布包，则需要配置 **「spring-boot-maven-plugin」**
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-maven-plugin</artifactId>
      <configuration>
        <includeSystemScope>true</includeSystemScope>
      </configuration>
    </plugin>
  </plugins>
</build>
```

- `import`，Maven2.0.9新增的scope值。仅能在`<dependencyManagement>`中使用，用于引用其它项目的依赖项。
- 示例
```xml
<!-- 引入项目io.fsjohnhuang.deps的依赖项 -->
<dependencyManagement>
  <dependency>
    <groupId>io.fsjohnhuang</groupId>
    <artifactId>deps</artifactId>
    <type>pom</type>
    <scope>import</scope>
  </dependency>
</dependencyManagement>
```

- `关于type pom`若项目拥有大量依赖项，那么会导致当前项目的**「POM.xml文件臃肿」**，即使采用父POM依然无法很好地解决该问题。而**「通过type为pom的项目」**则可以将依赖项分打包为独立项目，然后其它业务系统项目则可以通过引入这些项目导入相关依赖项，从而精简POM文件。
- `关于type属性`默认的 type 属性值为 jar，即 Maven 将项目编译打包为 Jar 包，当设置为 pom 时则表示该项目为一堆相关依赖项的打包定义而已，**「当设置为 apk 或 ejb 等时则表示 Maven 在编译打包时采用 andriod 或 ejb 相关的插件执行任务。」**
