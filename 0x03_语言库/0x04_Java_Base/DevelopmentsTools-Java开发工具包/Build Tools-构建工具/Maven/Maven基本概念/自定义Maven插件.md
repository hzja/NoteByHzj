Maven<br />Maven默认提供了很多插件，功能也非常强大，但是如果想自己开发一些插件，比如自定义一款自动打包并且发布到服务器然后重启服务器的插件；或者定义一款插件自动打包自动运行打包好的构件。各种好玩的东西只要能想到，都可以通过Maven插件去完成，不过需要先了解如何自定义Maven插件。
<a name="pA9iu"></a>
## 自定义插件详细步骤
Maven中的插件是有很多目标（goal）组成的，开发插件，实际上就是去编写插件中目标的具体代码。每个目标对应一个Java类，这个类在Maven中叫做MOJO，Maven提供了一个Mojo的接口，开发插件也就是去实现这个接口的方法，这个接口是：
```
org.apache.maven.plugin.Mojo
```
接口有3个方法：
```java
void execute() throws MojoExecutionException, MojoFailureException;
void setLog( Log log );
Log getLog();
```

- `**execute**`：这个方法比较重要，目标的主要代码就在这个方法中实现，当使用mvn命令调用插件的目标的时候，最后具体调用的就是这个方法。
- `**setLog**`：注入一个标准的Maven日志记录器，允许这个Mojo向用户传递事件和反馈
- `**getLog**`：获取注入的日志记录器

说一下上面这个Log，这是一日志接口，里面定义了很多方法，主要用户向交互者输出日志，比如运行`mvn clean`，会输出很多提示信息，这些输出的信息就是通过Log来输出的。<br />Mojo接口有个默认的抽象类：
```
org.apache.maven.plugin.AbstractMojo
```
这个类中把Mojo接口中的`setLog`和`getLog`实现了，而`execute`方法没有实现，交给继承者去实现，这个类中Log默认可以向控制台输出日志信息，Maven中自带的插件都继承这个类，一般情况下开发插件目标可以直接继承这个类，然后实现`execute`方法就可以了。
<a name="ZJGnu"></a>
### 实现一个插件的具体步骤
<a name="mpxJf"></a>
#### 1、 创建一个Maven构件，这个构件的packaging比较特殊，必须为maven-plugin，表示这个构件是一个插件类型，如下：
pom.xml中的packageing元素必须如下值：
```xml
<packaging>maven-plugin</packaging>
```
<a name="y4P8C"></a>
#### 2、导入Maven插件依赖：
```xml
<dependency>
  <groupId>org.apache.maven</groupId>
  <artifactId>maven-plugin-api</artifactId>
  <version>3.0</version>
</dependency>

<dependency>
  <groupId>org.apache.maven.plugin-tools</groupId>
  <artifactId>maven-plugin-annotations</artifactId>
  <version>3.4</version>
  <scope>provided</scope>
</dependency>
```
<a name="S7IXp"></a>
#### 3、创建一个目标类，需要继承org.apache.maven.plugin.AbstractMojo
<a name="zN5Io"></a>
#### 4、目标类中添加注解`@Mojo`注解：
```java
@org.apache.maven.plugins.annotations.Mojo(name="目标名称")
```
注意`@Mojo`注解用来标注这个类是一个目标类，Maven对插件进行构建的时候会根据这个注解来找到这个插件的目标，这个注解中还有其他参数，后面在详细介绍。
<a name="xkZ6R"></a>
#### 5、在目标类的`execute`方法中实现具体的逻辑
<a name="T4gbq"></a>
#### 6、安装插件到本地仓库：插件的pom.xml所在目录执行下面命令
```bash
mvn clean install
```
或者可以部署到私服仓库，部署方式和其他构件的方式一样，这个具体去看前面文章的私服的文章。
<a name="JeYMO"></a>
#### 7、让使用者去使用插件
<a name="kRLK7"></a>
### 案例1
下面来实现第一个插件，插件构件信息：
```xml
<groupId>com.javacode2018</groupId>
<artifactId>demo1-maven-plugin</artifactId>
<version>1.0-SNAPSHOT</version>
<packaging>maven-plugin</packaging>
```
创建一个目标类demo1，调用这个目标的时候，希望他能够输出：
```
hello my first maven plugin!
```
<a name="FuxMn"></a>
#### 创建一个Maven项目
打开idea，点击File->New->Project，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250395566-62ee6978-ce5a-47d4-a757-d0056da0dd92.png#averageHue=%23e3e1de&clientId=u94698b64-1f92-4&from=paste&id=uae9a44d4&originHeight=191&originWidth=671&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1d99ea30-ba24-48b7-9028-de094ac306e&title=)<br />选择Maven，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690250396135-0c00790d-006f-4e4e-8b5e-feff528548a4.jpeg#averageHue=%23e6e2e1&clientId=u94698b64-1f92-4&from=paste&id=ubd544240&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8539552d-3683-499c-8c91-29d29192694&title=)<br />点击上图中的Next,如下图，输入项目坐标信息：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250395653-d7a85c9e-eb4f-40b1-91d1-32638bed197c.png#averageHue=%23e9e7e7&clientId=u94698b64-1f92-4&from=paste&id=u197ac322&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u49d9c335-2cd1-4bd1-ab7f-a2a82d60e98&title=)<br />点击上图中的Next，如下图，输入Project name为maven-chat10：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250395605-ea4e95ca-bf4b-4774-b644-1764d512ecc8.png#averageHue=%23e9e7e6&clientId=u94698b64-1f92-4&from=paste&id=ue12b8ae1&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud1d38bd4-17c1-4875-af95-438f8fe4c70&title=)<br />点击上图中的Finish，完成创建，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250395554-47056118-1c13-4c5a-99a7-d5e201995784.png#averageHue=%23f7f6f5&clientId=u94698b64-1f92-4&from=paste&id=ue6888b84&originHeight=387&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubdf8b390-374f-4aae-bc86-7e57ee33e52&title=)<br />配置一下idea的Maven环境，点击File->Settings，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250396214-5ddbad43-9230-4725-8e3d-b5c705e0d6e1.png#averageHue=%23b49d82&clientId=u94698b64-1f92-4&from=paste&id=u9f3535ac&originHeight=649&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucb1172de-2496-4120-9c2c-983e77a9580&title=)<br />删除下面2个无用的文件夹：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250396418-b9ec0f97-bf9f-415b-92c7-e414958dda56.png#averageHue=%23f3edc0&clientId=u94698b64-1f92-4&from=paste&id=u02ec7f64&originHeight=205&originWidth=482&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6ecebb0f-2c36-4192-83a1-9abc7c2f6dc&title=)
<a name="JMSI9"></a>
#### 创建插件模块demo1-maven-plugin
这次用idea创建一个插件模块demo1-maven-plugin，具体过程如下。<br />在刚才的maven-chat10项目窗口中，点击File->Project Structure，如下图：<br />也可以使用快捷键Ctrl+Alt+Shift+S打开<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250396491-d39415ab-0f01-498d-8f91-eb2a9357905e.png#averageHue=%23e5dcda&clientId=u94698b64-1f92-4&from=paste&id=uf77354d7&originHeight=316&originWidth=359&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u330598cb-06ef-4b37-8e7f-57f28a381c9&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250396394-536391be-5fcc-4ea9-a7b0-500b65f9b673.png#averageHue=%23e6e5e4&clientId=u94698b64-1f92-4&from=paste&id=u3d776a8a&originHeight=649&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua4896a08-06d0-4438-8076-a7aa0d09344&title=)<br />选择上图找Modules，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250397009-465712e9-725c-4896-853a-ae8dd375e08e.png#averageHue=%23ebebea&clientId=u94698b64-1f92-4&from=paste&id=uf46f7619&originHeight=649&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u436c66da-c256-4fc1-a2ff-9b938d3166c&title=)<br />点击+，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250397011-da04ac98-d009-4f59-be12-b871fde67bc9.png#averageHue=%23ebeaea&clientId=u94698b64-1f92-4&from=paste&id=ub71d5723&originHeight=649&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2f53e4d3-06b3-4b1d-88e7-007d4d67bd6&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250397029-faaf8da8-74f0-48d0-9c07-9a848f45f964.png#averageHue=%23f5f3f1&clientId=u94698b64-1f92-4&from=paste&id=u97ea60d7&originHeight=547&originWidth=376&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u61e3eff9-481f-4102-bab3-18e09061711&title=)<br />选择上图中的New Module，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690250397079-13a4e76f-87e6-4d9c-9d92-ecd4936a63c9.jpeg#averageHue=%23e5e5e4&clientId=u94698b64-1f92-4&from=paste&id=u4c76bf86&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf9dfb82b-cc88-40a6-bf8f-80be22d6307&title=)<br />选择上图左侧的Maven，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690250397059-b884093e-3608-4219-bcd2-100ea3201573.jpeg#averageHue=%23e5e2e2&clientId=u94698b64-1f92-4&from=paste&id=ubf7342f6&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubba59a5a-6809-4253-978e-75bc67054ec&title=)<br />点击上图中的Next，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250398022-4fd5626f-0fc8-4413-8605-20641328ad1f.png#averageHue=%23e9e9e8&clientId=u94698b64-1f92-4&from=paste&id=ua25320a3&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u28c71df2-6dc3-4110-8e90-a8f066cf5b6&title=)<br />将Add as module to设置为maven-chat10，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250397829-dce207d1-8dbf-48c3-b51c-07202d1ac9fc.png#averageHue=%23e9e8e8&clientId=u94698b64-1f92-4&from=paste&id=u20876689&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub438d46f-8c0c-49b9-a68a-b8b0a213511&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250397838-7956ca05-89c6-4615-b641-e629df6704ea.png#averageHue=%23f0e9e9&clientId=u94698b64-1f92-4&from=paste&id=uc2495491&originHeight=275&originWidth=440&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3a0f346e-efdb-4b16-8478-d66baaa0dba&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250397803-fb3db5a2-0619-481e-a99b-32204f0a77e7.png#averageHue=%23e9e8e7&clientId=u94698b64-1f92-4&from=paste&id=u58c94a47&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u77873659-dbbb-4815-a103-3ee265b5ac7&title=)<br />上图中输入坐标信息，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250397753-39e98d99-e3ba-456a-908c-760224c27545.png#averageHue=%23e9e6e6&clientId=u94698b64-1f92-4&from=paste&id=ua5c2cfc1&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uec183c6c-6f19-4929-9626-c82ec607905&title=)<br />点击上图中的Next，默认如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250398566-a00e933a-5fb4-46c1-89d4-c789c615de3f.png#averageHue=%23e8e8e8&clientId=u94698b64-1f92-4&from=paste&id=u5dbfde99&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u16c862f5-85a8-4e76-b442-786f28d42c2&title=)<br />修改上图中Module name为demo1-maven-plugin，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250398488-8e753f65-c344-4c0c-acd4-103a0c1c512f.png#averageHue=%23e9e6e6&clientId=u94698b64-1f92-4&from=paste&id=u6bc086a0&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubcbbb81b-cee2-4f26-b928-b67008e8ff9&title=)<br />点击上图的Finish，如下图:<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250398743-013ae055-9269-48e0-8133-f4c2a0e9b351.png#averageHue=%23eae9e9&clientId=u94698b64-1f92-4&from=paste&id=uab302221&originHeight=649&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u140592ea-fada-4b9a-a478-b20ef9338a9&title=)<br />点击上图中的OK按钮，完成创建工作，目前项目结构如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250398520-714cf6b1-9b21-4701-92bf-28e5cb0216fa.png#averageHue=%23f6f2f1&clientId=u94698b64-1f92-4&from=paste&id=u3ff65c6a&originHeight=520&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8a939a4d-acbd-4090-abbb-d2d96c6ea4b&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250398701-036bb31b-376e-49d0-9a6a-412102cc7f7e.png#averageHue=%23f6f2f0&clientId=u94698b64-1f92-4&from=paste&id=u179dc5e5&originHeight=461&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaee8900e-3662-4a19-977b-dbc6c5f58de&title=)
<a name="LUZQj"></a>
#### 设置demo1-maven-plugin/pom.xml中packaging的值为maven-plugin，如下
```xml
<packaging>maven-plugin</packaging>
```
<a name="rWipP"></a>
#### demo1-maven-plugin/pom.xml引入插件需要的依赖
```xml
<dependency>
  <groupId>org.apache.maven</groupId>
  <artifactId>maven-plugin-api</artifactId>
  <version>3.0</version>
</dependency>

<dependency>
  <groupId>org.apache.maven.plugin-tools</groupId>
  <artifactId>maven-plugin-annotations</artifactId>
  <version>3.4</version>
  <scope>provided</scope>
</dependency>
```
最后demo1-maven-plugin/pom.xml内容如下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>demo1-maven-plugin</artifactId>
  <version>1.0-SNAPSHOT</version>
  <!-- 插件必须为maven-plugin这个类型 -->
  <packaging>maven-plugin</packaging>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <!-- 配置maven编译的时候采用的编译器版本 -->
    <maven.compiler.compilerVersion>1.8</maven.compiler.compilerVersion>
    <!-- 指定源代码是什么版本的，如果源码和这个版本不符将报错，maven中执行编译的时候会用到这个配置，默认是1.5，这个相当于javac命令后面的-source参数 -->
    <maven.compiler.source>1.8</maven.compiler.source>
    <!-- 该命令用于指定生成的class文件将保证和哪个版本的虚拟机进行兼容，maven中执行编译的时候会用到这个配置，默认是1.5，这个相当于javac命令后面的-target参数 -->
    <maven.compiler.target>1.8</maven.compiler.target>
  </properties>

  <dependencies>
    <!-- maven插件依赖 start -->
    <dependency>
      <groupId>org.apache.maven</groupId>
      <artifactId>maven-plugin-api</artifactId>
      <version>3.0</version>
    </dependency>

    <dependency>
      <groupId>org.apache.maven.plugin-tools</groupId>
      <artifactId>maven-plugin-annotations</artifactId>
      <version>3.4</version>
      <scope>provided</scope>
    </dependency>
    <!-- maven插件依赖 end -->
  </dependencies>

  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-plugin-plugin</artifactId>
        <version>3.4</version>
      </plugin>
    </plugins>
  </build>

</project>
```
<a name="uGgep"></a>
#### 创建目标类
在demo-maven-plugin中创建的目标类com.javacode2018.Demo1Mojo，需要继承`org.apache.maven.plugin.AbstractMojo`，需要实现`@Mojo`注解，如下：
```java
package com.javacode2018;

import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.Mojo;

@Mojo(name = "demo1")
public class Demo1Mojo extends AbstractMojo {
    public void execute() throws MojoExecutionException, MojoFailureException {
    }
}
```
注意上面注解`@Mojo(name = "demo1")`，name使用来标注目标的名称为demo1。
<a name="jgOpb"></a>
#### 实现目标类的`execute`方法
在`execute`方法中输出一句话
```java
this.getLog().info("hello my first maven plugin!");
```
目前execute方法代码如下：
```java
public void execute() throws MojoExecutionException, MojoFailureException {
    this.getLog().info("hello my first maven plugin!");
}
```
<a name="eqyMD"></a>
#### 安装插件到本地仓库
在maven-chat10/pom.xml目录执行下面命令：
```bash
mvn clean install -pl :demo1-maven-plugin
```
注意上面命令和在demo1-maven-plugin/pom中执行`mvn clean install`效果是一样的，只是这个地方使用了Maven裁剪的功能。<br />上面命令效果如下：
```
D:\code\IdeaProjects\maven-chat10>mvn clean install -pl :demo1-maven-plugin
[INFO] Scanning for projects...
[INFO]
[INFO] ----------------< com.javacode2018:demo1-maven-plugin >-----------------
[INFO] Building demo1-maven-plugin 1.0-SNAPSHOT
[INFO] ----------------------------[ maven-plugin ]----------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ demo1-maven-plugin ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ demo1-maven-plugin ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ demo1-maven-plugin ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin\target\classes
[INFO]
[INFO] --- maven-plugin-plugin:3.4:descriptor (default-descriptor) @ demo1-maven-plugin ---
[INFO] Using 'UTF-8' encoding to read mojo metadata.
[INFO] Mojo extractor with id: java-javadoc found 0 mojo descriptors.
[INFO] Mojo extractor with id: java-annotations found 1 mojo descriptors.
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ demo1-maven-plugin ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ demo1-maven-plugin ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ demo1-maven-plugin ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ demo1-maven-plugin ---
[INFO] Building jar: D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin\target\demo1-maven-plugin-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-plugin-plugin:3.4:addPluginArtifactMetadata (default-addPluginArtifactMetadata) @ demo1-maven-plugin ---
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ demo1-maven-plugin ---
[INFO] Installing D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin\target\demo1-maven-plugin-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\demo1-maven-plugin\1.0-SNAPSHOT\demo1-maven-plugin-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\demo1-maven-plugin\1.0-SNAPSHOT\demo1-maven-plugin-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  5.402 s
[INFO] Finished at: 2019-11-26T15:21:26+08:00
[INFO] ------------------------------------------------------------------------
```
<a name="rGPMH"></a>
#### 验证插件，调用插件的demo1目标看效果
maven-chat10/pom.xml所在目录执行：
```bash
mvn com.javacode2018:demo1-maven-plugin:demo1
```
效果如下：
```
D:\code\IdeaProjects\maven-chat10>mvn com.javacode2018:demo1-maven-plugin:demo1
[INFO] Scanning for projects...
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Build Order:
[INFO]
[INFO] demo1-maven-plugin                                        [maven-plugin]
[INFO] maven-chat10                                                       [pom]
[INFO]
[INFO] ----------------< com.javacode2018:demo1-maven-plugin >-----------------
[INFO] Building demo1-maven-plugin 1.0-SNAPSHOT                           [1/2]
[INFO] ----------------------------[ maven-plugin ]----------------------------
[INFO]
[INFO] --- demo1-maven-plugin:1.0-SNAPSHOT:demo1 (default-cli) @ demo1-maven-plugin ---
[INFO] hello my first maven plugin!
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat10 >--------------------
[INFO] Building maven-chat10 1.0-SNAPSHOT                                 [2/2]
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- demo1-maven-plugin:1.0-SNAPSHOT:demo1 (default-cli) @ maven-chat10 ---
[INFO] hello my first maven plugin!
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary for maven-chat10 1.0-SNAPSHOT:
[INFO]
[INFO] demo1-maven-plugin ................................. SUCCESS [  0.358 s]
[INFO] maven-chat10 ....................................... SUCCESS [  0.042 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.656 s
[INFO] Finished at: 2019-11-26T13:49:30+08:00
```
上面输出的东西比较多，主要看一下有这样的一句输出，如下：
```
[INFO] hello my first maven plugin!
```
上面这个输出就是在`execute`方法中输出的内容。
<a name="JK3P7"></a>
## 目标中参数的使用
上面介绍了开发一个插件目标详细的实现过程，然后写了一个简单的案例，比较简单。不过自定义的Mojo如果没有参数，那么这个Mojo基本上也实现不了什么复杂的功能，下面来看一下Mojo中如何使用参数。
<a name="sE1kv"></a>
### 需要先在mojo中定义参数
定义参数就像在mojo中创建一个实例变量并添加适当的注释一样简单。下面列出了一个简单mojo的参数示例：
```java
/**
 * 要显示的问候语。
 */
@Parameter( property = "sayhi.greeting", defaultValue = "Hello World!" )
private String greeting;
```
`@Parameter`注解之前的部分是参数的描述，这个注解将变量标识为mojo参数。注解的defaultValue参数定义变量的默认值，此值Maven的属性值，例如“${project.version}”，property参数可用于通过引用用户通过-D选项设置的系统属性，即通过从命令行配置mojo参数，如`mvn ... -Dsayhi.greeting=Hi`可以将Hi的值传递给greeting参数，这个注解还有几个属性大家有兴趣的可以自己去研究一下。
<a name="QTvrO"></a>
### 在pom.xml配置参数的值
```xml
<plugin>
  <groupId>com.javacode2018</groupId>
  <artifactId>demo1-maven-plugin</artifactId>
  <version>1.0-SNAPSHOT</version>
  <configuration>
    <greeting>Maven</greeting>
  </configuration>
</plugin>
```
上面设置的是一个string类型的greeting参数的值，还有其他很多类型定义以及使用，也来看一下。
<a name="f4ZWr"></a>
### Boolean参数
```java
/**
* My boolean.
*/
@Parameter
private boolean myBoolean;
```
```xml
<myBoolean>true</myBoolean>
```
<a name="DFhU0"></a>
### 数字类型参数
数字类型包含：byte, Byte, int, Integer, long, Long, short, Short，读取配置时，XML文件中的文本将使用适当类的`integer.parseInt()`或`valueOf()`方法转换为整数值，这意味着字符串必须是有效的十进制整数值，仅由数字0到9组成，前面有一个可选的-表示负值。例子：
```java
/**
* My Integer.
*/
@Parameter
private Integer myInteger;
```
```xml
<myInteger>10</myInteger>
```
<a name="HnQzJ"></a>
### File类型参数
读取配置时，XML文件中的文本用作所需文件或目录的路径。如果路径是相对的（不是以/或C:之类的驱动器号开头），则路径是相对于包含POM的目录的。例子：
```java
/**
* My File.
*/
@Parameter
private File myFile;
```
```xml
<myFile>c:\temp</myFile>
```
<a name="IZbiZ"></a>
### 枚举类型参数
```java
public enum Color {
    GREEN,
    RED,
    BLUE
}

/**
* My Enum
*/
@Parameter
private Color myColor;
```
```xml
<myColor>GREEN</myColor>
```
<a name="DE9Ci"></a>
### 数组类型参数
```java
/**
* My Array.
*/
@Parameter
private String[] myArray;
```
```xml
<myArray>
  <param>value1</param>
  <param>value2</param>
</myArray>
```
<a name="eiTMg"></a>
### `Collections`类型参数
```java
/**
* My List.
*/
@Parameter
private List myList;
```
```xml
<myList>
  <param>value1</param>
  <param>value2</param>
</myList>
```
<a name="irv4I"></a>
### `Maps`类型参数
```java
/**
* My Map.
*/
@Parameter
private Map myMap;
```
```xml
<myMap>
  <key1>value1</key1>
  <key2>value2</key2>
</myMap>
```
<a name="bPo0J"></a>
### `Properties`类型参数
`java.util.Properties`的类型
```java
/**
* My Properties.
*/
@Parameter
private Properties myProperties;
```
```xml
<myProperties>
  <property>
    <name>propertyName1</name>
    <value>propertyValue1</value>
  <property>
  <property>
    <name>propertyName2</name>
    <value>propertyValue2</value>
  <property>
</myProperties>
```
<a name="Cq6Jh"></a>
### 自定义类型参数
```java
/**
* My Object.
*/
@Parameter
private MyObject myObject;
```
```xml
<myObject>
  <myField>test</myField>
</myObject>
```
<a name="gROF1"></a>
### 案例2
<a name="f8BGY"></a>
#### 修改案例代码
将上面各种类型的参数都放到Demo1Mojo中，Demo1Mojo类如下：
```java
package com.javacode2018;

import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;

import java.io.File;
import java.lang.reflect.Field;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.Properties;

@Mojo(name = "demo1")
public class Demo1Mojo extends AbstractMojo {

    /**
     * 要显示的问候语。
     */
    @Parameter(property = "sayhi.greeting", defaultValue = "Hello World!")
    private String greeting;

    /**
     * My boolean.
     */
    @Parameter
    private boolean myBoolean;

    /**
     * My Integer.
     */
    @Parameter
    private Integer myInteger;

    /**
     * My File.
     */
    @Parameter
    private File myFile;

    public enum Color {
        GREEN,
        RED,
        BLUE
    }

    /**
     * My Enum
     */
    @Parameter
    private Color myColor;

    /**
     * My Array.
     */
    @Parameter
    private String[] myArray;

    /**
     * My List.
     */
    @Parameter
    private List myList;

    /**
     * My Map.
     */
    @Parameter
    private Map myMap;

    /**
     * My Properties.
     */
    @Parameter
    private Properties myProperties;

    public static class Person {
        private String name;
        private int age;

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public int getAge() {
            return age;
        }

        public void setAge(int age) {
            this.age = age;
        }

        @Override
        public String toString() {
            return "Person{" +
            "name='" + name + '\'' +
            ", age=" + age +
            '}';
        }
    }

    @Parameter
    private Person person;

    public void execute() throws MojoExecutionException, MojoFailureException {
        this.getLog().info("hello my first maven plugin!");

        Field[] declaredFields = Demo1Mojo.class.getDeclaredFields();

        Arrays.stream(declaredFields).forEach(f -> {
            if (f.isAccessible()) {
                f.setAccessible(true);
            }
            try {
                this.getLog().info(f.getName() + ":" + f.get(this));
            } catch (IllegalAccessException e) {
                this.getLog().warn(e);
            }
        });
    }
}
```
<a name="RQwHe"></a>
#### 将demo1-maven-plugin安装到本地仓库
maven-chat10/pom.xml所在目录运行：
```bash
mvn clean install -pl :demo1-maven-plugin
```
<a name="rvUpZ"></a>
#### 创建测试模块demo1-maven-plugin-test
使用idea创建，过程和demo1-maven-plugin过程类似，可以直接参考，创建好了，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250451478-9b143c0d-dbc3-4e76-8872-cc8a6457dfcf.png#averageHue=%23f5f4f2&clientId=u94698b64-1f92-4&from=paste&id=uffe83ee4&originHeight=395&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8e9ab7f0-031c-45cd-87ec-55ca8b146e0&title=)<br />修改demo1-mavein-plugin-test/pom.xml文件，加入下面内容：
```xml
<build>
  <plugins>
    <plugin>
      <groupId>com.javacode2018</groupId>
      <artifactId>demo1-maven-plugin</artifactId>
      <version>1.0-SNAPSHOT</version>
      <executions>
        <execution>
          <id>demo1 plugin test</id>
          <phase>pre-clean</phase>
          <goals>
            <goal>demo1</goal>
          </goals>
          <configuration>
            <myBoolean>true</myBoolean>
            <myInteger>30</myInteger>
            <myFile>${project.basedir}</myFile>
            <myColor>BLUE</myColor>
            <myArray>
              <array>maven</array>
              <array>spring</array>
              <array>mybatis</array>
              <array>springboot</array>
              <array>springcloud</array>
            </myArray>
            <myList>
              <list>30</list>
              <list>35</list>
            </myList>
            <myMap>
              <name>Java</name>
              <age>30</age>
            </myMap>
            <myProperties>
              <property>
                <name>name</name>
                <value>javacode2018</value>
              </property>
              <property>
                <name>age</name>
                <value>30</value>
              </property>
            </myProperties>
            <person>
              <name>Tom</name>
              <age>32</age>
            </person>
          </configuration>
        </execution>
      </executions>
    </plugin>
  </plugins>
</build>
```
上面是将生命周期的pre-clean阶段绑定插件demo1-maven-plugin的demo1目标，并且设置了demo1目标所需要的所有参数的值。
<a name="pIV7Q"></a>
#### 验证效果
在maven-chat10/pom.xml所在目录执行：
```
D:\code\IdeaProjects\maven-chat10>mvn pre-clean -pl :demo1-maven-plugin-test -Dsayhi.greeting="Hi"
[INFO] Scanning for projects...
[INFO]
[INFO] --------------< com.javacode2018:demo1-maven-plugin-test >--------------
[INFO] Building demo1-maven-plugin-test 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- demo1-maven-plugin:1.0-SNAPSHOT:demo1 (demo1 plugin test) @ demo1-maven-plugin-test ---
[INFO] hello my first maven plugin!
[INFO] greeting:Hi
[INFO] myBoolean:true
[INFO] myInteger:30
[INFO] myFile:D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin-test
[INFO] myColor:BLUE
[INFO] myArray:[Ljava.lang.String;@7bf9b098
[INFO] myList:[30, 35]
[INFO] myMap:{age=30, name=Java}
[INFO] myProperties:{age=30, name=javacode2018}
[INFO] person:Person{name='Tom', age=32}
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.517 s
[INFO] Finished at: 2019-11-26T15:42:53+08:00
[INFO] ------------------------------------------------------------------------
```
<a name="h2iEx"></a>
## 插件前缀
在案例1中，使用下面命令调用的插件：
```bash
mvn com.javacode2018:demo1-maven-plugin:demo1
```
这种是采用下面这种格式：
```bash
mvn 插件groupId:插件artifactId[:插件版本]:插件目标名称
```
命令中插件版本是可以省略的，Maven会自动找到这个插件最新的版本运行，不过最好不要省略版本号，每个版本的插件功能可能不一样，为了保证任何情况下运行效果的一致性，强烈建议指定版本号。<br />上面执行插件需要插件的坐标信息，一长串比较麻烦，Maven为了使用插件方便，提供了插件前缀来解决这个问题。
<a name="wLVZ8"></a>
### 自定义插件前缀的使用
<a name="iw3kp"></a>
#### 设置自定义插件的artifactId
自定义插件的artifactId满足下面的格式：
```
xxx-maven-plugin
```
如果采用这种格式的Maven会自动将xxx指定为插件的前缀，其他格式也可以，不过此处只说这种格式，这个是最常用的格式。<br />如上面的demo1-maven-plugin插件，他的前缀就是demo1。<br />当配置了插件前缀，可以插件前缀来调用插件的目标了，命令如下：
```bash
mvn 插件前缀:插件目标
```
Maven是如何通过插件前缀找到具体的插件的呢？<br />Maven默认会在仓库"org.apache.maven.plugins" 和 "org.codehaus.mojo"2个位置查找插件，比如：
```bash
mvn clean:help
```
这个是调用maven-clean-plugin插件的help目标，maven-clean-plugin的前缀就是clean，他的groupId是org.apache.maven.plugins，所以能够直接找到。<br />但是自己定义的插件，如果也让Maven能够找到，需要下面的配置。
<a name="KzN0b"></a>
#### 在`~/.m2/settings.xml`中配置自定义插件组
在pluginGroups中加入自定义的插件组groupId，如：
```xml
<pluginGroup>com.javacode2018</pluginGroup>
```
这样当通过前缀调用插件的时候，Maven除了会在2个默认的组中查找，还会在这些自定义的插件组中找，一般情况下自定义的插件通常使用同样的groupId。
<a name="dzaI0"></a>
#### 使用插件前缀调用插件
```bash
mvn 插件前缀:插件目标
```
案例3<br />在~/.m2/settings.xml中加入下面配置：
```xml
<pluginGroup>com.javacode2018</pluginGroup>
```
在maven-chat10/pom.xml所在目录执行：
```bash
mvn demo1:demo1 -pl demo1-maven-plugin-test
```
效果如下：
```
D:\code\IdeaProjects\maven-chat10>mvn demo1:demo1 -pl demo1-maven-plugin-test
[INFO] Scanning for projects...
[INFO]
[INFO] --------------< com.javacode2018:demo1-maven-plugin-test >--------------
[INFO] Building demo1-maven-plugin-test 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- demo1-maven-plugin:1.0-SNAPSHOT:demo1 (default-cli) @ demo1-maven-plugin-test ---
[INFO] hello my first maven plugin!
[INFO] greeting:Hello World!
[INFO] myBoolean:false
[INFO] myInteger:null
[INFO] myFile:null
[INFO] myColor:null
[INFO] myArray:null
[INFO] myList:null
[INFO] myMap:null
[INFO] myProperties:null
[INFO] person:null
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.786 s
[INFO] Finished at: 2019-11-26T16:10:41+08:00
[INFO] ------------------------------------------------------------------------
```
上面直接通过插件的前缀来调用插件的功能了，是不是很爽！
<a name="oVliG"></a>
## 手动实现打包之后自动运行的插件
<a name="CC8Ll"></a>
### 实现思路
<a name="YlnOe"></a>
#### 1、将目标构件打包为可以执行jar包到target目录
Maven中将构件打包为可以执行的jar的插件，Maven已经提供了，如下：
```xml
<plugin>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-shade-plugin</artifactId>
  <version>3.2.1</version>
  <executions>
    <execution>
      <phase>package</phase>
      <goals>
        <goal>shade</goal>
      </goals>
      <configuration>
        <transformers>
          <transformer implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
            <mainClass>启动类完整路径</mainClass>
          </transformer>
        </transformers>
      </configuration>
    </execution>
  </executions>
</plugin>
```
上面使用到了Maven官方提供的一个打包的插件，可以将构件打包为可以直接运行的jar包。
<a name="fQkC0"></a>
#### 2、自定义一个插件，然后执行上面打包好的插件
插件中需要通过java命令调用打包好的jar包，然后运行。
<a name="J2jQT"></a>
### 具体实现如下
<a name="mElVb"></a>
#### 创建自定义目标类
demo1-maven-plugin中创建一个插件目标类，如下：
```java
package com.javacode2018;

import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.plugins.annotations.Execute;
import org.apache.maven.plugins.annotations.LifecyclePhase;
import org.apache.maven.plugins.annotations.Mojo;
import org.apache.maven.plugins.annotations.Parameter;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

@Mojo(name = "run", defaultPhase = LifecyclePhase.PACKAGE)
@Execute(phase = LifecyclePhase.PACKAGE)
public class RunMojo extends AbstractMojo {

    /**
     * 打包好的构件的路径
     */
    @Parameter(defaultValue = "${project.build.directory}\\${project.artifactId}-${project.version}.jar")
    private String jarPath;

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {
        try {
            this.getLog().info("Started:" + this.jarPath);
            ProcessBuilder builder = new ProcessBuilder("java", "-jar", this.jarPath);
            final Process process = builder.start();

            new Thread(() -> {
                BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                try {
                    String s;
                    while ((s = reader.readLine()) != null) {
                        System.out.println(s);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }).start();
            Runtime.getRuntime().addShutdownHook(new Thread() {
                @Override
                public void run() {
                    RunMojo.this.getLog().info("Destroying...");
                    process.destroy();
                    RunMojo.this.getLog().info("Shutdown hook finished.");
                }
            });

            process.waitFor();
            this.getLog().info("Finished.");
        } catch (Exception e) {
            this.getLog().warn(e);
        }
    }
}
```
上面这个插件目标的名称为run<br />注意这个类上面多了一个注解`@Execute`，这个注解可以配置这个目标执行之前可以先执行的生命周期的阶段或者需要先执行的插件目标。<br />上面配置的是`phase = LifecyclePhase.PACKAGE`，也就是说当运行上面run目标的时候，会先执行构件的package阶段，也就是先执行项目的打包阶段，打包完成之后才会执行run目标。
<a name="QtTHa"></a>
#### 安装插件到本地仓库
maven-chat10/pom.xml所在目录运行：
```bash
mvn clean install -pl :demo1-maven-plugin
```
<a name="N466G"></a>
#### 创建测试模块demo1-maven-plugin-run
使用idea创建，过程和demo1-maven-plugin过程类似，可以直接参考，创建好了，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690250451354-c3e9e87f-e8e4-4352-b7e0-7ec263c55123.png#averageHue=%23f3f2f0&clientId=u94698b64-1f92-4&from=paste&id=uee242948&originHeight=455&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u48b9661b-216a-475b-9b90-e0d7a3832ce&title=)
<a name="HR7nm"></a>
#### 创建com.javacode2018.Demo类
在demo1-maven-plugin-run\src\main\java创建下面类：
```java
package com.javacode2018;

import java.util.Calendar;
import java.util.concurrent.TimeUnit;

public class Demo1 {
    public static void main(String[] args) throws InterruptedException {
        for (int i = 0; i < 1000; i++) {
            System.out.println(Calendar.getInstance().getTime() + ":" + i);
            TimeUnit.SECONDS.sleep(1);
        }
    }
}
```
<a name="EB2Jq"></a>
#### 修改demo1-maven-plugin-run/pom.xml，如下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>demo1-maven-plugin-run</artifactId>
  <version>1.0-SNAPSHOT</version>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <!-- 配置maven编译的时候采用的编译器版本 -->
    <maven.compiler.compilerVersion>1.8</maven.compiler.compilerVersion>
    <!-- 指定源代码是什么版本的，如果源码和这个版本不符将报错，maven中执行编译的时候会用到这个配置，默认是1.5，这个相当于javac命令后面的-source参数 -->
    <maven.compiler.source>1.8</maven.compiler.source>
    <!-- 该命令用于指定生成的class文件将保证和哪个版本的虚拟机进行兼容，maven中执行编译的时候会用到这个配置，默认是1.5，这个相当于javac命令后面的-target参数 -->
    <maven.compiler.target>1.8</maven.compiler.target>
  </properties>

  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-shade-plugin</artifactId>
        <version>3.2.1</version>
        <executions>
          <execution>
            <phase>package</phase>
            <goals>
              <goal>shade</goal>
            </goals>
            <configuration>
              <transformers>
                <transformer implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
                  <mainClass>com.javacode2018.Demo1</mainClass>
                </transformer>
              </transformers>
            </configuration>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>
</project>
```
<a name="GztXS"></a>
#### 验证效果
在maven-chat10/pom.xml所在目录执行下面的命令：
```bash
mvn clean demo1:run -pl demo1-maven-plugin-run
```
输出如下：
```
D:\code\IdeaProjects\maven-chat10>mvn clean demo1:run -pl demo1-maven-plugin-run
[INFO] Scanning for projects...
[INFO]
[INFO] --------------< com.javacode2018:demo1-maven-plugin-run >---------------
[INFO] Building demo1-maven-plugin-run 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ demo1-maven-plugin-run ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin-run\target
[INFO]
[INFO] >>> demo1-maven-plugin:1.0-SNAPSHOT:run (default-cli) > package @ demo1-maven-plugin-run >>>
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ demo1-maven-plugin-run ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ demo1-maven-plugin-run ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin-run\target\classes
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ demo1-maven-plugin-run ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin-run\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ demo1-maven-plugin-run ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ demo1-maven-plugin-run ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ demo1-maven-plugin-run ---
[INFO] Building jar: D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin-run\target\demo1-maven-plugin-run-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-shade-plugin:3.2.1:shade (default) @ demo1-maven-plugin-run ---
[INFO] Replacing original artifact with shaded artifact.
[INFO] Replacing D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin-run\target\demo1-maven-plugin-run-1.0-SNAPSHOT.jar with D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin-run\target\demo1-maven-plugin-run-1.0-SNAPSHOT-shaded.jar
[INFO]
[INFO] <<< demo1-maven-plugin:1.0-SNAPSHOT:run (default-cli) < package @ demo1-maven-plugin-run <<<
[INFO]
[INFO]
[INFO] --- demo1-maven-plugin:1.0-SNAPSHOT:run (default-cli) @ demo1-maven-plugin-run ---
[INFO] Started:D:\code\IdeaProjects\maven-chat10\demo1-maven-plugin-run\target\demo1-maven-plugin-run-1.0-SNAPSHOT.jar
Tue Nov 26 17:24:47 CST 2019:0
Tue Nov 26 17:24:48 CST 2019:1
Tue Nov 26 17:24:49 CST 2019:2
Tue Nov 26 17:24:50 CST 2019:3
Tue Nov 26 17:24:51 CST 2019:4
Tue Nov 26 17:24:52 CST 2019:5
Tue Nov 26 17:24:53 CST 2019:6
Tue Nov 26 17:24:54 CST 2019:7
Tue Nov 26 17:24:55 CST 2019:8
Tue Nov 26 17:24:56 CST 2019:9
Tue Nov 26 17:24:57 CST 2019:10
Tue Nov 26 17:24:58 CST 2019:11
Tue Nov 26 17:24:59 CST 2019:12
Tue Nov 26 17:25:00 CST 2019:13
```
**是不是打包+运行很简单，一行命令就搞定了。**
