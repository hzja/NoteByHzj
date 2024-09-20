Maven<br />使用Maven过程中，用到了一些命令，如下：
```bash
mvn clean
mvn compile
mvn test
mvn package
mvn install
mvn install -Dmaven.test.skip=true
mvn deploy
mvn help:system
```
上面这些命令，如果玩过Maven，估计大家还是比较眼熟的，只是大家有没有想过这些命令为什么这么写，为什么`-Dmaven.test.skip=true`可以跳过测试，大家需要知道mvn命令背后的一些原理。
<a name="PwrMx"></a>
## 用户属性Properties的使用
项目pom.xml中，有下面这样一段依赖：
```xml
<dependencies>
  <dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-core</artifactId>
    <version>5.2.1.RELEASE</version>
  </dependency>
  <dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-beans</artifactId>
    <version>5.2.1.RELEASE</version>
  </dependency>
  <dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-aop</artifactId>
    <version>5.2.1.RELEASE</version>
  </dependency>
</dependencies>
```
大家看一下上面的配置，有没有什么问题？<br />他们的groupId和version都是一样的，程序员面对与重复的代码，需要提取，如果是java代码中，可以将同样的代码或者变量值，提取成方法或者变量，做到重用，方便维护。<br />那么Maven的pom.xml中也支持这么做：
```xml
<properties>
  <spring.group>org.springframework</spring.group>
  <spring.version>5.2.1.RELEASE</spring.version>
</properties>

<dependencies>
  <dependency>
    <groupId>${spring.group}</groupId>
    <artifactId>spring-core</artifactId>
    <version>${spring.version}</version>
  </dependency>
  <dependency>
    <groupId>${spring.group}</groupId>
    <artifactId>spring-beans</artifactId>
    <version>${spring.version}</version>
  </dependency>
  <dependency>
    <groupId>${spring.group}</groupId>
    <artifactId>spring-aop</artifactId>
    <version>${spring.version}</version>
  </dependency>
</dependencies>
```
大家看一下上面的代码，properties位于pom.xml中的，是project元素的子元素，用户可以在properties中自定义一些用户属性，然后可以在其他地方使用${属性名称}这种方式进行引用。
<a name="js7ke"></a>
## 生命周期
开发一个项目的时候，通常有这些环节：创建项目、编写代码、清理已编译的代码、编译代码、执行单元测试、打包、集成测试、验证、部署、生成站点等，这些环节组成了项目的生命周期，这些过程也叫做项目的**构建过程**，几乎所有的项目都由这些环节中的其中几个，创建项目和编写代码是程序员需要多参与的，其他的都可以做成自动化的方式。<br />用过ant的朋友回忆一下，在Maven出现以前，开发人员每天都在对项目进行清理、编译、执行单元测试、打包、部署等操作，虽然大家都在做这些工作，但是没有一个统一的标准，项目和项目之间，公司和公司之间，大多数都是各写各的，写法是千奇百怪，能满足自身需求就可以了，但是换个项目就得从头再来，这些操作又需要重新编写脚本。<br />而Maven出来之后，项目生命周期中的这些环节都被简化了，被规范化了，Maven出现之前，项目的结构没有一个统一的标准，所以生命周期中各个环节对应的自动化脚本也是各种各样，而Maven约定好了项目的结构，源码的位置、资源文件的位置、测试代码的位置、测试用到的资源文件的位置、静态资源的位置、打包之后文件的位置等，这些都是Maven约定好的，所以清理代码用一个命令`mvn clean`就可以完成，不需要去配置清理的目标目录；用`mvn compile`命令就可以完成编译的操作；用`mvn test`就可以自动运行测试用例；用`mvn package`就可以将项目打包为jar、war格式的包，能够如此简单，主要还是Maven中约定大于配置的结果。
<a name="fvN82"></a>
## Maven中生命周期详解
Maven将项目的生命周期抽象成了3套生命周期，每套生命周期又包含多个阶段，每套中具体包含哪些阶段是Maven已经约定好的，但是每个阶段具体需要做什么，是用户可以自己指定的。<br />Maven中定义的3套生命周期：

1. **clean生命周期**
2. **default生命周期**
3. **site生命周期**

上面这3套生命周期是相互独立的，没有依赖关系的，而每套生命周期中有多个阶段，每套中的多个阶段是有先后顺序的，并且后面的阶段依赖于前面的阶段，而用户可以直接使用mvn命令来调用这些阶段去完成项目生命周期中具体的操作，命令是：
```
mvn 生命周期阶段
```
**通俗点解释：**<br />Maven中的3套生命周期相当于Maven定义了3个类来解决项目生命周期中需要的各种操作，每个类中有多个方法，这些方法就是指具体的阶段，方法名称就是阶段的名称，每个类的方法是有顺序的，当执行某个方法的时候，这个方法前面的方法也会执行。具体每个方法中需要执行什么，这个是通过插件的方式让用户去配置的，所以非常灵活。<br />用户执行mvn 阶段名称就相当于调用了具体的某个方法。<br />下面来看看每个生命周期中有哪些阶段（也就是每个类中有哪些方法，顺序是什么样的）。
<a name="OUTdT"></a>
### clean生命周期
clean生命周期的目的是清理项目，它包含三个阶段：

| 生命周期阶段 | 描述 |
| --- | --- |
| pre-clean | 执行一些需要在clean之前完成的工作 |
| clean | 移除所有上一次构建生成的文件 |
| post-clean | 执行一些需要在clean之后立刻完成的工作 |

用户可以通过mvn pre-clean来调用clean生命周期中的pre-clean阶段需要执行的操作。<br />调用mvn post-clean会执行上面3个阶段所有的操作，上文中有说过，每个生命周期中的后面的阶段会依赖于前面的阶段，当执行某个阶段的时候，会先执行其前面的阶段。
<a name="wSP06"></a>
### default生命周期
这个是Maven主要的生命周期，主要被用于构建应用，包含了23个阶段。

| 生命周期阶段 | 描述 |
| --- | --- |
| validate | 校验：校验项目是否正确并且所有必要的信息可以完成项目的构建过程。 |
| initialize | 初始化：初始化构建状态，比如设置属性值。 |
| generate-sources | 生成源代码：生成包含在编译阶段中的任何源代码。 |
| process-sources | 处理源代码：处理源代码，比如说，过滤任意值。 |
| generate-resources | 生成资源文件：生成将会包含在项目包中的资源文件。 |
| process-resources | 编译：复制和处理资源到目标目录，为打包阶段最好准备。 |
| compile | 处理类文件：编译项目的源代码。 |
| process-classes | 处理类文件：处理编译生成的文件，比如说对Java class文件做字节码改善优化。 |
| generate-test-sources | 生成测试源代码：生成包含在编译阶段中的任何测试源代码。 |
| process-test-sources | 处理测试源代码：处理测试源代码，比如说，过滤任意值。 |
| generate-test-resources | 生成测试源文件：为测试创建资源文件。 |
| process-test-resources | 处理测试源文件：复制和处理测试资源到目标目录。 |
| test-compile | 编译测试源码：编译测试源代码到测试目标目录. |
| process-test-classes | 处理测试类文件：处理测试源码编译生成的文件。 |
| test | 测试：使用合适的单元测试框架运行测试（Juint是其中之一）。 |
| prepare-package | 准备打包：在实际打包之前，执行任何的必要的操作为打包做准备。 |
| package | 打包：将编译后的代码打包成可分发格式的文件，比如JAR、WAR或者EAR文件。 |
| pre-integration-test | 集成测试前：在执行集成测试前进行必要的动作。比如说，搭建需要的环境。 |
| integration-test | 集成测试：处理和部署项目到可以运行集成测试环境中。 |
| post-integration-test | 集成测试后：在执行集成测试完成后进行必要的动作。比如说，清理集成测试环境。 |
| verify | 验证：运行任意的检查来验证项目包有效且达到质量标准。 |
| install | 安装：安装项目包到本地仓库，这样项目包可以用作其他本地项目的依赖。 |
| deploy | 部署：将最终的项目包复制到远程仓库中与其他开发者和项目共享。 |

<a name="xwFRf"></a>
### site生命周期
site生命周期的目的是建立和发布项目站点，Maven能够基于pom.xml所包含的信息，自动生成一个友好的站点，方便团队交流和发布项目信息。主要包含以下4个阶段：

| 阶段 | 描述 |
| --- | --- |
| pre-site | 执行一些需要在生成站点文档之前完成的工作 |
| site | 生成项目的站点文档 |
| post-site | 执行一些需要在生成站点文档之后完成的工作，并且为部署做准备 |
| site-deploy | 将生成的站点文档部署到特定的服务器上 |

<a name="pUBiX"></a>
### mvn命令和生命周期
从命令行执行Maven任务的最主要方式就是调用Maven生命周期的阶段，需要注意的是，每套生命周期是相互独立的，但是每套生命周期中阶段是有前后依赖关系的，执行某个的时候，会按序先执行其前面所有的。<br />mvn执行阶段的命令格式是：
```
mvn 阶段1 [阶段2] [阶段n]
```
多个阶段的名称之间用空格隔开。<br />下面举一些常见的例子来说明一下：
<a name="rfuw3"></a>
#### `mvn clean`
该命令是调用clean生命周期的clean阶段，实际执行的阶段为clean生命周期中的pre-clean和clean阶段。
<a name="HV7BX"></a>
#### `mvn test`
该命令调用default生命周期的test阶段，实际上会从default生命周期的第一个阶段（validate）开始执行一直到test阶段结束。这里面包含了代码的编译，运行测试用例。
<a name="tuxjr"></a>
#### `mvn clean install`
这个命令中执行了两个阶段：clean和install，从上面3个生命周期的阶段列表中找一下，可以看出clean位于clean生命周期的表格中，install位于default生命周期的表格中，所以这个命令会先从clean生命周期中的pre-clean阶段开始执行一直到clean生命周期的clean阶段；然后会继续从default生命周期的validate阶段开始执行一直到default生命周期的install阶段。<br />这里面包含了清理上次构建的结果，编译代码，测试，打包，将打好的包安装到本地仓库。
<a name="HejJF"></a>
#### `mvn clean deploy`
这个命令也比较常用，会先按顺序执行clean生命周期的[pre-clean,clean]这个闭区间内所有的阶段，然后按序执行default生命周期的[validate,deploy]这个闭区间内的所有阶段（也就是default生命周期中的所有阶段）。这个命令内部包含了清理上次构建的结果、编译代码、运行单元测试、打包、将打好的包安装到本地仓库、将打好的包发布到私服仓库。<br />上面说了这么多理论，来看一下效果。
<a name="fzDXA"></a>
### 案例
<a name="mbIHs"></a>
#### 创建一个Maven项目
打开idea，点击File->New->Project，选择Maven，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690189296841-a3bc8bf7-b7e9-456c-9391-820059a26b32.jpeg#averageHue=%23dcc5be&clientId=uc1a212f9-0166-4&from=paste&id=ue86967c9&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue2050dbf-ec95-43bd-85bf-ca657475a65&title=)<br />点击Next，输入项目坐标信息，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690189296786-ec60648a-3d44-4793-957f-36b8c6f0f629.png#averageHue=%23e9e7e7&clientId=uc1a212f9-0166-4&from=paste&id=u4540cc12&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udca3ac59-4589-4890-94b5-17ae6a1a091&title=)<br />点击Next，输入Project name 为maven-chat06，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690189296982-ebf951da-0b74-4116-83b3-7674481affd9.png#averageHue=%23e8e6e6&clientId=uc1a212f9-0166-4&from=paste&id=u7098d6e0&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud7f01927-c39b-4ef3-b067-a2fbe45d53a&title=)<br />点击Finish，创建成功，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690189296867-30b9796f-7039-4003-b802-82d50fd452fb.jpeg#averageHue=%23e9e7dc&clientId=uc1a212f9-0166-4&from=paste&id=u0597a7b0&originHeight=650&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5aa38013-46f2-4833-939c-f7d5b7b2d94&title=)<br />配置一下idea的Maven环境，点击File->Settings，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690189296845-52054e73-2bae-4832-a6d1-c8ee379e7134.png#averageHue=%23c59c63&clientId=uc1a212f9-0166-4&from=paste&id=u25cc6fc9&originHeight=647&originWidth=1066&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucce4785c-a624-4928-ab23-14aa2db3923&title=)<br />点击上面的OK完成配置。<br />还原~/.m2/settings.xml的配置到初始状态，操作如下：
> 将M2_HOME/conf/settings.xml复制到~/.m2/settings.xml目录，如果存在先备份一个，然后进行覆盖。

Maven项目是约定大于配置的，项目结构是按照Maven的约定生成好的，关于Maven约定项目结构，再来回顾一下。<br />Maven 提倡使用一个共同的标准目录结构，Maven 使用约定优于配置的原则，大家尽可能的遵守这样的目录结构，如下所示：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690189297698-9825d557-bc58-4d56-bf32-fce5d0af2dc8.png#averageHue=%23f8f7f6&clientId=uc1a212f9-0166-4&from=paste&id=u2d356b71&originHeight=465&originWidth=1051&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5daafd9d-8247-4b68-b376-88a96f9713e&title=)<br />结合刚才项目的结构和这个表格领会一下，下面来感受一下执行生命周期中的阶段产生的效果。<br />修改pom.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>maven-chat06</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>jar</packaging>
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
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.12</version>
      <scope>test</scope>
    </dependency>
  </dependencies>

</project>
```
上面properties这个元素中的配置，可能大家看不懂，先略过，后面会详解。<br />创建一个Demo类，源码是放在src/main/java目录中，如下：
```java
package com.javacode2018.maven;

import java.util.ArrayList;
import java.util.List;

public class Demo1 {
    public static void main(String[] args) {
        System.out.println("maven高手！");
        List<Integer> list = new ArrayList<>();
        for (int i = 0; i < 5; i++) {
            list.add(i);
        }
        list.forEach(System.out::print);
    }
}
```
<a name="tV6kk"></a>
#### `mvn clean`命令效果
在当前项目pom.xml所在目录中执行下面命令：
```bash
mvn clean
```
效果如下：
```
D:\code\IdeaProjects\maven-chat06>mvn clean
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ maven-chat06 ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat06\target
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.477 s
[INFO] Finished at: 2019-11-15T18:46:13+08:00
[INFO] ------------------------------------------------------------------------
```
上面有提到编译、打包的内容都放在target目录，看上面输出中有个Deleting target目录，说明`mvn clean`是对这个目录进行清理，这个目录中目前是空的。
<a name="HYhFz"></a>
#### `mvn compile`命令效果
先看一下项目的目录中是没有target目录的，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690189297761-a2a3d60a-8f64-422a-84b3-8a30404d880e.png#averageHue=%23fbfbfa&clientId=uc1a212f9-0166-4&from=paste&id=ubfc47252&originHeight=442&originWidth=479&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u06757b04-4e78-4deb-a2dc-00d7874e40b&title=)<br />cmd中执行：
```
mvn compile
```
输出：
```
D:\code\IdeaProjects\maven-chat06>mvn compile
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat06\target\classes
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.897 s
[INFO] Finished at: 2019-11-15T18:53:12+08:00
[INFO] ------------------------------------------------------------------------
```
可以看到上面有Compiling 1 source ....，这个是编译Demo.java，然后输出到了target中的classes目录，再来看一下项目的结构，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690189297897-2b4e8db1-d4ff-43a5-b2f7-c87be07c13bc.png#averageHue=%23f8f5ec&clientId=uc1a212f9-0166-4&from=paste&id=u07c6cc3c&originHeight=665&originWidth=480&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5c205b17-9640-49b7-99c4-e586ac6800c&title=)<br />上图中匡红的是新生成的。
<a name="OVldU"></a>
#### `mvn clean package`效果
```
D:\code\IdeaProjects\maven-chat06>mvn clean package
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ maven-chat06 ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat06\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat06\target\classes
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ maven-chat06 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ maven-chat06 ---
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ maven-chat06 ---
[INFO] Building jar: D:\code\IdeaProjects\maven-chat06\target\maven-chat06-1.0-SNAPSHOT.jar
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.177 s
[INFO] Finished at: 2019-11-15T18:56:59+08:00
[INFO] ------------------------------------------------------------------------
```
从输出中看一下，有个Building jar ...，生成了一个jar包，这个项目的pom.xml中的packaging元素没有指定值，那就取默认值jar，表示这个构件是一个jar包，`mvn clean package`先清理编译的代码，然后执行了default生命周期的compile阶段，将项目打成了jar放在了target目录，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690189297970-864b152e-f138-4a1b-9316-b35c4d33fc48.png#averageHue=%23fdfbdf&clientId=uc1a212f9-0166-4&from=paste&id=ud4aabf2a&originHeight=314&originWidth=465&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u55a41eb9-dfa3-43c5-9eab-5db218244cb&title=)<br />大家看到上面还有很多其他的输出，这个大家可以先忽略，本文看完了，都会明白的。
<a name="WJWCE"></a>
#### `mvn clean install`效果
```
D:\code\IdeaProjects\maven-chat06>mvn clean install
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ maven-chat06 ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat06\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat06\target\classes
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ maven-chat06 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ maven-chat06 ---
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ maven-chat06 ---
[INFO] Building jar: D:\code\IdeaProjects\maven-chat06\target\maven-chat06-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ maven-chat06 ---
[INFO] Installing D:\code\IdeaProjects\maven-chat06\target\maven-chat06-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat06\1.0-SNAPSHOT\maven-chat06-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\maven-chat06\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat06\1.0-SNAPSHOT\maven-chat06-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.977 s
[INFO] Finished at: 2019-11-15T19:05:21+08:00
[INFO] ------------------------------------------------------------------------
```
和`mvn clean package`的输出对比一下，多了几行输出，主要是多了2个Installing...，将项目打成jar包以及项目的pom文件放到本地仓库去了，也就是将构件打包安装到本地仓库了。<br />上面几个mvn命令的案例，都是通过mvn命令去执行了mvn中定义的生命周期中的阶段，然后完成了很多看似内部很复杂的操作。比如打包，内部包含很多复杂的操作，Maven都屏蔽了，通过一个简单的`mvn package`就完成了。<br />上面也有说过，每个阶段具体做的事情是由Maven插件来完成的。<br />再回头看一下上面一个输出中，有很多类似于maven-xxxx-plugin:版本:xxx这样的内容，这个就是表示当前在运行这个插件来完成对应阶段的操作，mvn 阶段明明执行的是阶段，但是实际输出中确实插件在干活，那么阶段是如何和插件关联起来的呢？插件又是什么呢？<br />**注意以下所有命令都在cmd窗口执行，执行位置位于上面这个项目的pom.xml所在目录。**
<a name="cfII3"></a>
## Maven插件
Maven插件主要是为Maven中生命周期中的阶段服务的，Maven中只是定义了3套生命周期，以及每套生命周期中有哪些阶段，具体每个阶段中执行什么操作，完全是交给插件去干的。<br />Maven中的插件就相当于一些工具，比如编译代码的工具，运行测试用例的工具，打包代码的工具，将代码上传到本地仓库的工具，将代码部署到远程仓库的工具等等，这些都是Maven中的插件。<br />插件可以通过mvn命令的方式调用直接运行，或者将插件和Maven生命周期的阶段进行绑定，然后通过mvn 阶段的方式执行阶段的时候，会自动执行和这些阶段绑定的插件。
<a name="XwUdL"></a>
### 插件目标
Maven中的插件以jar的方式存在于仓库中，和其他构件是一样的，也是通过坐标进行访问，每个插件中可能为了代码可以重用，一个插件可能包含了多个功能，比如编译代码的插件，可以编译源代码、也可以编译测试代码；**插件中的每个功能就叫做插件的目标（Plugin Goal），每个插件中可能包含一个或者多个插件目标（Plugin Goal）**。
<a name="crQAP"></a>
#### 目标参数
插件目标是用来执行任务的，那么执行任务肯定是有参数配的，这些就是目标的参数，每个插件目标对应于java中的一个类，参数就对应于这个类中的属性。
<a name="q6bXa"></a>
#### 列出插件所有目标
```
mvn 插件goupId:插件artifactId[:插件version]:help
mvn 插件前缀:help
```
上面插件前缀的先略过，先看第一种效果。<br />如：
```
D:\code\IdeaProjects\maven-chat06>mvn org.apache.maven.plugins:maven-clean-plugin:help
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:help (default-cli) @ maven-chat06 ---
[INFO] org.apache.maven.plugins:maven-clean-plugin:2.5

Maven Clean Plugin
  The Maven Clean Plugin is a plugin that removes files generated at build-time
  in a project's directory.

This plugin has 2 goals:

clean:clean
  Goal which cleans the build.
  This attempts to clean a project's working directory of the files that were
  generated at build-time. By default, it discovers and deletes the directories
  configured in project.build.directory, project.build.outputDirectory,
  project.build.testOutputDirectory, and project.reporting.outputDirectory.

  Files outside the default may also be included in the deletion by configuring
  the filesets tag.

clean:help
  Display help information on maven-clean-plugin.
  Call
    mvn clean:help -Ddetail=true -Dgoal=<goal-name>
  to display parameter details.
```
上面列出了maven-clean-plugin这个插件所有的目标，有2个，分别是clean:clean、clean:help，分号后面的部分是目标名称，分号前面的部分是插件的前缀，每个目标的后面包含对这个目标的详细解释说明，关于前缀的后面会有详细介绍。
<a name="QogHe"></a>
#### 查看插件目标参数列表
```
mvn 插件goupId:插件artifactId[:插件version]:help -Dgoal=目标名称 -Ddetail
mvn 插件前缀:help -Dgoal=目标名称 -Ddetail
```
上面命令中的`-Ddetail`用户输出目标详细的参数列表信息，如果没有这个，目标的参数列表不会输出出来，看效果。<br />如：
```
D:\code\IdeaProjects\maven-chat06>mvn org.apache.maven.plugins:maven-clean-plugin:help -Dgoal=help -Ddetail
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:help (default-cli) @ maven-chat06 ---
[INFO] org.apache.maven.plugins:maven-clean-plugin:2.5

Maven Clean Plugin
  The Maven Clean Plugin is a plugin that removes files generated at build-time
  in a project's directory.

clean:help
  Display help information on maven-clean-plugin.
  Call
    mvn clean:help -Ddetail=true -Dgoal=<goal-name>
  to display parameter details.

  Available parameters:

    detail (Default: false)
      If true, display all settable properties for each goal.
      Expression: ${detail}

    goal
      The name of the goal for which to show help. If unspecified, all goals
      will be displayed.
      Expression: ${goal}

    indentSize (Default: 2)
      The number of spaces per indentation level, should be positive.
      Expression: ${indentSize}

    lineLength (Default: 80)
      The maximum length of a display line, should be positive.
      Expression: ${lineLength}
```
上面列出了clean插件的help目标的详细参数信息。<br />注意上面参数详细参数说明中有Expression: ${xxx}这样的部分，这种表示给这个运行的目标传参，可以通过mvn -Dxxx这种方式传参，xxx为${xxx}中的xxx部分，这个xxx有时候和目标参数的名称不一致，所以这点需要注意，运行带参数的目标，看一下效果：
```
D:\code\IdeaProjects\maven-chat06>mvn org.apache.maven.plugins:maven-clean-plugin:help -Dgoal=help -Ddetail=false
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:help (default-cli) @ maven-chat06 ---
[INFO] org.apache.maven.plugins:maven-clean-plugin:2.5

Maven Clean Plugin
  The Maven Clean Plugin is a plugin that removes files generated at build-time
  in a project's directory.

clean:help
  Display help information on maven-clean-plugin.
  Call
    mvn clean:help -Ddetail=true -Dgoal=<goal-name>
  to display parameter details.


[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.332 s
[INFO] Finished at: 2019-11-18T15:14:56+08:00
[INFO] ------------------------------------------------------------------------
```
上面传了一个`detail=false`，上面未输出目标的详细参数信息。
<a name="rIGMF"></a>
#### 命令行运行插件
```
mvn 插件goupId:插件artifactId[:插件version]:插件目标 [-D目标参数1] [-D目标参数2] [-D目标参数n]
mvn 插件前缀:插件目标  [-D目标参数1] [-D目标参数2] [-D目标参数n]
```
案例：<br />Maven中运行测试用例使用到的插件坐标是：
```xml
<dependency>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-surefire-plugin</artifactId>
  <version>2.12.4</version>
</dependency>
```
看一下这个插件有哪些目标：
```
D:\code\IdeaProjects\maven-chat06>mvn org.apache.maven.plugins:maven-surefire-plugin:help
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:help (default-cli) @ maven-chat06 ---
[INFO] Maven Surefire Plugin 2.12.4
  Surefire is a test framework project.

This plugin has 2 goals:

surefire:help
  Display help information on maven-surefire-plugin.
  Call mvn surefire:help -Ddetail=true -Dgoal=<goal-name> to display parameter
  details.

surefire:test
  Run tests using Surefire.


[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.662 s
[INFO] Finished at: 2019-11-18T15:26:26+08:00
[INFO] ------------------------------------------------------------------------
```
maven-surefire-plugin插件有2个目标help和test，描述中可以看出test目标是用来运行测试用例的。<br />看一下test目标对应的参数列表：<br />test目标对应的参数太多，只列出了部分参数，如下：
```
D:\code\IdeaProjects\maven-chat06>mvn org.apache.maven.plugins:maven-surefire-plugin:help -Dgoal=test -Ddetail=true
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:help (default-cli) @ maven-chat06 ---
[INFO] Maven Surefire Plugin 2.12.4
  Surefire is a test framework project.

surefire:test
  Run tests using Surefire.

  Available parameters:

    skip (Default: false)
      Set this to 'true' to bypass unit tests entirely. Its use is NOT
      RECOMMENDED, especially if you enable it using the 'maven.test.skip'
      property, because maven.test.skip disables both running the tests and
      compiling the tests. Consider using the skipTests parameter instead.
```
大家认真看一下skip这个参数说明，这个参数默认是false，如果设置为true的时候，项目将跳过测试代码的编译和测试用例的执行，可以`maven.test.skip`这个属性来进行命令行传参，将其传递给test目标的skip属性，这个通过`-D`传递的参数名称就和目标参数名称不一样了，所以需要注意-D后面并不一定是参数名称。<br />来运行一下test目标看看效果。<br />先看一下不加参数的效果：
```
D:\code\IdeaProjects\maven-chat06>mvn org.apache.maven.plugins:maven-surefire-plugin:test
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-cli) @ maven-chat06 ---
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.640 s
[INFO] Finished at: 2019-11-18T15:33:48+08:00
[INFO] ------------------------------------------------------------------------
```
加`maven.skip.test=true`的效果如下：
```
D:\code\IdeaProjects\maven-chat06>mvn org.apache.maven.plugins:maven-surefire-plugin:test -Dmaven.test.skip=true
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-cli) @ maven-chat06 ---
[INFO] Tests are skipped.
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.652 s
[INFO] Finished at: 2019-11-18T15:34:45+08:00
[INFO] ------------------------------------------------------------------------
```
对比一下上面2个输出，下面的多了一行如下：
```
[INFO] Tests are skipped.
```
说明跳过了测试的执行。
<a name="hv6iy"></a>
#### 插件传参的2种方式
刚才上面讲了一种通过`-D`后面跟用户属性的方式给用户传参，还有一种方式，在pom.xml中properties的用户自定义属性中进行配置，如下：<br />修改项目maven-chat06的pom.xml，properties中加入：
```xml
<maven.test.skip>true</maven.test.skip>
```
cmd中运行：
```
mvn org.apache.maven.plugins:maven-surefire-plugin:test
```
效果如下：
```
D:\code\IdeaProjects\maven-chat06>mvn org.apache.maven.plugins:maven-surefire-plugin:test
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-cli) @ maven-chat06 ---
[INFO] Tests are skipped.
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.638 s
[INFO] Finished at: 2019-11-18T15:46:04+08:00
[INFO] ------------------------------------------------------------------------
```
输出中也有Tests are skipped.，说明也跳过了测试，和`-Dmaven.test.skip=true`效果一样。<br />上面说的都是插件目标的东西，那么插件目标是如何和生命周期关联起来的呢？继续向下看。
<a name="Ro91i"></a>
#### 获取插件目标详细描述信息的另外一种方式
```bash
mvn help:describe -Dplugin=插件goupId:插件artifactId[:插件version] -Dgoal=目标名称 -Ddetail
mvn help:describe -Dplugin=插件前缀 -Dgoal=目标名称 -Ddetail
```
上面这个命令调用的是help插件的describe这个目标，这个目标可以列出其他指定插件目标的详细信息，看效果：
```
D:\code\IdeaProjects\maven-chat06>mvn help:describe -Dplugin=org.apache.maven.plugins:maven-surefire-plugin -Dgoal=test -Ddetail
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-help-plugin:3.2.0:describe (default-cli) @ maven-chat06 ---
[INFO] Mojo: 'surefire:test'
surefire:test
  Description: Run tests using Surefire.
  Implementation: org.apache.maven.plugin.surefire.SurefirePlugin
  Language: java
  Bound to phase: test

  Available parameters:

    additionalClasspathElements
      Additional elements to be appended to the classpath.

    argLine
      User property: argLine
      Arbitrary JVM options to set on the command line.

    skip (Default: false)
      User property: maven.test.skip
      Set this to 'true' to bypass unit tests entirely. Its use is NOT
      RECOMMENDED, especially if you enable it using the 'maven.test.skip'
      property, because maven.test.skip disables both running the tests and
      compiling the tests. Consider using the skipTests parameter instead.
```
可以拿这种和上面获取插件目标参数详情列表对比一下，上面这个更详细一些，参数说明中多了一行User property: 属性名称，这个属性名称可以通过两种方式传递：

1. mvn命令-D属性名称的方式传递
2. pom.xml中properties中定义的方式指定。

现在可以大家估计可以知道一直用的`-Dmaven.test.skip`为什么可以跳过测试代码的编译和单元测试的执行了吧。
<a name="PVskb"></a>
### 插件前缀
运行插件的时候，可以通过指定插件坐标的方式运行，但是插件的坐标信息过于复杂，也不方便写和记忆，所以Maven中给插件定义了一些简捷的插件前缀，可以通过插件前缀来运行指定的插件。<br />可以通过下面命令查看到插件的前缀：
```bash
mvn help:describe -Dplugin=插件goupId:插件artifactId[:插件version]
```
示例效果：
```
D:\code\IdeaProjects\maven-chat06>mvn help:describe -Dplugin=org.apache.maven.plugins:maven-surefire-plugin
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-help-plugin:3.2.0:describe (default-cli) @ maven-chat06 ---
[INFO] org.apache.maven.plugins:maven-surefire-plugin:2.12.4

Name: Maven Surefire Plugin
Description: Surefire is a test framework project.
Group Id: org.apache.maven.plugins
Artifact Id: maven-surefire-plugin
Version: 2.12.4
Goal Prefix: surefire
```
输出中的Goal Prefix:部分对应的就是插件的前缀，上面这个插件的前缀是surefire。<br />使用前缀来运行一下插件感受一下效果：
```
D:\code\IdeaProjects\maven-chat06>mvn surefire:test
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-cli) @ maven-chat06 ---
[INFO] Tests are skipped.
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.934 s
[INFO] Finished at: 2019-11-18T16:18:42+08:00
[INFO] ------------------------------------------------------------------------
```
上面通过别名来运行插件maven-surefire-plugin的test目标，是不是简洁了很多。<br />上面用了很多mvn help:这个命令，这个调用的是maven-help-plugin插件的功能，help是插件的前缀，它的坐标是：
```xml
<dependency>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-help-plugin</artifactId>
  <version>3.2.0</version>
</dependency>
```
<a name="FiydM"></a>
### 插件和生命周期阶段绑定
Maven只是定义了生命周期中的阶段，而没有定义每个阶段中具体的实现，这些实现是由插件的目标来完成的，所以需要将阶段和插件目标进行绑定，来让插件目标帮助生命周期的阶段做具体的工作，生命周期中的每个阶段支持绑定多个插件的多个目标。<br />**将生命周期中的阶段和插件的目标进行绑定的时候，执行mvn 阶段就可以执行和这些阶段绑定的插件目标。**
<a name="fOksJ"></a>
### Maven内置插件以及绑定
Maven为了让我们不用做任何配置就可以实现一些项目的构建操作，比如运行`mvn clean`就可以清理代码，运行mvn install就可以将构件安装到本地仓库，所以Maven做了一些事情，Maven内部已经提供了很多默认的插件，而将一些阶段默认和这些插件阶段绑定好了，所以不用做任何配置就可以执行清理代码、编译代码、测试、打包、安装到本地仓库、上传到远程仓库等阶段的操作，是因为Maven已经默认给这些阶段绑定好了插件目标，所以不需要再去配置，就直接可以运行，这些都是Maven内置绑定做的事情，来看看Maven有哪些内置绑定。
<a name="F244T"></a>
### Maven内置绑定
<a name="seT4r"></a>
#### clean生命周期阶段与插件绑定关系
| 生命周期阶段 | 插件:目标 |
| --- | --- |
| pre-clean | <br /> |
| clean | maven-clean-plugin:clean |
| post-clean | <br /> |

clean周期中只有clean阶段默认绑定了maven-clean-plugin插件的clean目标。maven-clean-plugin插件的clean目标作用就是删除项目的输出目录。
<a name="hFg3W"></a>
#### default生命周期阶段与插件绑定关系
default生命周期中有23个阶段，只列出有默认绑定的，其他的没有列出的没有绑定任何插件，因此没有任何实际的行为。

| 生命周期阶段 | 插件:目标 | 执行任务 |
| --- | --- | --- |
| process-resources | maven-resources-plugin:resources | 复制主资源文件至主输出目录 |
| compile | maven-compiler-plugin:compile | 编译主代码至主输出目录 |
| process-test-resources | maven-resources-plugin:testResources | 复制测试资源文件至测试输出目录 |
| test-compile | maven-compiler-plugin:testCompile | 编译测试代码至测试输出目录 |
| test | maven-surefile-plugin:test | 执行测试用例 |
| package | maven-jar-plugin:jar | 创建项目jar包 |
| install | maven-install-plugin:install | 将输出构件安装到本地仓库 |
| deploy | maven-deploy-plugin:deploy | 将输出的构件部署到远程仓库 |

<a name="xri7S"></a>
#### site生命周期阶段与插件绑定关系
| 生命周期阶段 | 插件:目标 |
| --- | --- |
| pre-site | <br /> |
| site | maven-site-plugin:site |
| post-site | <br /> |
| site-deploy | maven-site-plugin:deploy |

来几个案例解说一下。
<a name="UHHih"></a>
#### `mvn clean`
该命令是调用clean生命周期的clean阶段，实际执行的阶段为clean生命周期中的pre-clean和clean阶段，从上面内置绑定表格中找一下，可以看到只有clean阶段绑定了maven-clean-plugin插件的clean目标，所以运行`mvn clean`的时候，实际上会调用maven-clean-plugin插件的clean目标来清理代码。<br />运行一下看一下效果：
```
D:\code\IdeaProjects\maven-chat06>mvn clean
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ maven-chat06 ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat06\target
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.408 s
[INFO] Finished at: 2019-11-18T16:34:14+08:00
[INFO] ------------------------------------------------------------------------
```
上面有一行输出如下：
```
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ maven-chat06 ---
```
这个表示调用的插件是：maven-clean-plugin，版本是：2.5，插件的目标是：clean
<a name="ABY8I"></a>
#### `mvn test`
该命令调用default生命周期的test阶段，实际上会从default生命周期的第一个阶段（validate）开始执行一直到test阶段结束。这里面包含了代码的编译，运行测试用例。还是和上面的分析过程一样，对照上面表格中的绑定关系，可以得到`mvn test`会调用下面一些插件的目标：
```
maven-resources-plugin:resources
maven-compiler-plugin:compile
maven-resources-plugin:testResources
maven-compiler-plugin:testCompile
maven-surefile-plugin:test
```
来验证一下，看看是不是和分析的一样：
```
D:\code\IdeaProjects\maven-chat06>mvn test
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat06\target\classes
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ maven-chat06 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat06\target\test-classes
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ maven-chat06 ---
[INFO] Tests are skipped.
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.880 s
[INFO] Finished at: 2019-11-18T16:36:55+08:00
[INFO] ------------------------------------------------------------------------
```
从上面输出中可以看到调用了5个插件的目标，和分析的一样。<br />再来看一个跳过测试的例子，如下：
```
D:\code\IdeaProjects\maven-chat06>mvn test -Dmaven.skip.test=true
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ maven-chat06 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ maven-chat06 ---
[INFO] Tests are skipped.
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.384 s
[INFO] Finished at: 2019-11-18T16:38:52+08:00
[INFO] ------------------------------------------------------------------------
```
上面这个是不是很熟悉，经常用到的跳过测试，为什么这么写，大家都知道了吧。<br />其他几个mvn compile、mvn install、mvn deploy建议大家也自己去玩玩，加深理解。
<a name="tSLkl"></a>
### 自定义绑定
除了默认绑定的一些操作，自己也可以将一些阶段绑定到指定的插件目标上来完成一些操作，这种自定义绑定让Maven项目在构件的过程中可以执行更多更丰富的操作。<br />常见的一个案例是：创建项目的源码jar包，将其安装到仓库中，内置插件绑定关系中没有涉及到这一步的任务，所以需要用户自己配置。<br />插件maven-source-plugin的jar-no-fork可以完成该任务，将这个目标绑定在default生命周期的verify阶段上面，这个阶段没有任何默认绑定，verify是在测试完成之后并将构件安装到本地仓库之前执行的阶段，在这个阶段生成源码，配置如下：<br />在maven-chat06中的pom.xml加入如下配置：
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-source-plugin</artifactId>
      <version>3.2.0</version>
      <executions>
        <!-- 使用插件需要执行的任务 -->
        <execution>
          <!-- 任务id -->
          <id>attach-source</id>
          <!-- 任务中插件的目标，可以指定多个 -->
          <goals>
            <goal>jar-no-fork</goal>
          </goals>
          <!-- 绑定的阶段 -->
          <phase>verify</phase>
        </execution>
      </executions>
    </plugin>
  </plugins>
</build>
```
注意上面配置的attach-source，后面输出中会有。<br />id：任务的id，需唯一，如果不指定，默认为default。<br />每个插件的配置在pom.xml的plugins元素中只能写一次，否则会有警告。<br />最终pom.xml如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>maven-chat06</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>jar</packaging>
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
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.12</version>
      <scope>test</scope>
    </dependency>
  </dependencies>

  <build>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-source-plugin</artifactId>
        <version>3.2.0</version>
        <executions>
          <!-- 使用插件需要执行的任务 -->
          <execution>
            <!-- 任务id -->
            <id>attach-source</id>
            <!-- 任务中插件的目标，可以指定多个 -->
            <goals>
              <goal>jar-no-fork</goal>
            </goals>
            <!-- 绑定的阶段 -->
            <phase>verify</phase>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>

</project>
```
运行下面命令：
```bash
mvn install
```
效果：
```
D:\code\IdeaProjects\maven-chat06>mvn install
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ maven-chat06 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ maven-chat06 ---
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ maven-chat06 ---
[INFO]
[INFO] --- maven-source-plugin:3.2.0:jar-no-fork (attach-source) @ maven-chat06 ---
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ maven-chat06 ---
[INFO] Installing D:\code\IdeaProjects\maven-chat06\target\maven-chat06-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat06\1.0-SNAPSHOT\maven-chat06-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\maven-chat06\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat06\1.0-SNAPSHOT\maven-chat06-1.0-SNAPSHOT.pom
[INFO] Installing D:\code\IdeaProjects\maven-chat06\target\maven-chat06-1.0-SNAPSHOT-sources.jar to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat06\1.0-SNAPSHOT\maven-chat06-1.0-SNAPSHOT-sources.jar
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.268 s
[INFO] Finished at: 2019-11-18T16:59:12+08:00
[INFO] ------------------------------------------------------------------------
```
上面有个输出如下：
```
maven-source-plugin:3.2.0:jar-no-fork (attach-source) @ maven-chat06 ---
```
可以看出调用了配置的插件生成源码jar，上面的括号中的attach-source就是pom.xml中配置的任务id。<br />最后有个输出：
```
[INFO] Installing D:\code\IdeaProjects\maven-chat06\target\maven-chat06-1.0-SNAPSHOT-sources.jar to C:\Users\Think\.m2\repository\com\javacode2018\maven-chat06\1.0-SNAPSHOT\maven-chat06-1.0-SNAPSHOT-sources.jar
```
可以看到将源码安装到本地仓库了。<br />有些插件的目标默认会绑定到一些生命周期的阶段中，那么如果刚好插件默认绑定的阶段和上面配置的一致，那么上面phase元素可以不写了，那么怎么查看插件的默认绑定呢？
```bash
mvn help:describe -Dplugin=插件goupId:插件artifactId[:插件version] -Dgoal=目标名称 -Ddetail
mvn help:describe -Dplugin=插件前缀 -Dgoal=目标名称 -Ddetail
```
看一下插件source的jar-no-fork目标默认的绑定：
```
D:\code\IdeaProjects\maven-chat06>mvn help:describe -Dplugin=source -Dgoal=jar-no-fork -Ddetail
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-help-plugin:3.2.0:describe (default-cli) @ maven-chat06 ---
[INFO] Mojo: 'source:jar-no-fork'
source:jar-no-fork
  Description: This goal bundles all the sources into a jar archive. This
    goal functions the same as the jar goal but does not fork the build and is
    suitable for attaching to the build lifecycle.
  Implementation: org.apache.maven.plugins.source.SourceJarNoForkMojo
  Language: java
  Bound to phase: package
```
上面输出中有个Bound to phase: package，表示默认绑定在了package阶段上。<br />3套生命周期的运行时没有依赖的，但是每套中的阶段是有先后顺序的，运行某个阶段的时候，会先执行他前面所有的阶段。清理代码使用的是clean周期中的clean阶段，编译代码用的是default周期中的compile阶段，当直接运行`mvn compile`编译代码的时候并不会去清理代码，编译代码的时候若发现文件没有变动，会跳过没有变化的文件进行编译。如果想每次编译之前强制先清理代码，经常这么写：
```bash
mvn clean compile
```
上面的写法是不是很熟悉，运行一下看看效果：
```
D:\code\IdeaProjects\maven-chat06>mvn clean compile
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ maven-chat06 ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat06\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat06\target\classes
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.666 s
[INFO] Finished at: 2019-11-18T17:16:53+08:00
[INFO] ------------------------------------------------------------------------
```
还有其他方式么？<br />刚才学了自定义绑定，可以在default生命周期的第一个阶段validate绑定清理代码的插件，来通过自定义绑定来实现一下，project->build->plugins元素中加入下面配置：
```xml
<plugin>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-clean-plugin</artifactId>
  <version>2.5</version>
  <executions>
    <!-- 使用插件需要执行的任务 -->
    <execution>
      <!-- 任务中插件的目标，可以指定多个 -->
      <id>clean-target</id>
      <goals>
        <goal>clean</goal>
      </goals>
      <!-- 绑定的阶段 -->
      <phase>validate</phase>
    </execution>
  </executions>
</plugin>
```
运行下面命令看效果：
```
D:\code\IdeaProjects\maven-chat06>mvn compile
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (clean-target) @ maven-chat06 ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat06\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat06\target\classes
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.630 s
[INFO] Finished at: 2019-11-18T17:22:37+08:00
[INFO] ------------------------------------------------------------------------
```
输出中有：
```
[INFO] --- maven-clean-plugin:2.5:clean (clean-target) @ maven-chat06 ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat06\target
```
这个表示运行了代码清理的功能，进行了代码清理，是不是感觉很爽，不用每次都写clean了。
<a name="dx3lh"></a>
### POM.xml插件配置详解
<a name="TDowe"></a>
#### 插件目标共享参数配置
build->plugins->plugin中配置：
```xml
<!-- 插件参数配置，对插件中所有的目标起效 -->
<configuration>
  <目标参数名>参数值</目标参数名>
</configuration>
```
configuration节点下配置目标参数的值，节点名称为目标的参数名称，上面这种配置对当前插件的所有目标起效，也就是说这个插件中所有的目标共享此参数配置。<br />案例：<br />将案例中的pom.xml中的build元素修改成下面这样。
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-surefire-plugin</artifactId>
      <version>2.12.4</version>
      <!-- 插件参数配置，对插件中所有的目标起效 -->
      <configuration>
        <skip>true</skip>
      </configuration>
    </plugin>
  </plugins>
</build>
```
运行下面命令，看效果：
```
D:\code\IdeaProjects\maven-chat06>mvn test
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ maven-chat06 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ maven-chat06 ---
[INFO] Tests are skipped.
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.328 s
[INFO] Finished at: 2019-11-18T17:30:58+08:00
[INFO] ------------------------------------------------------------------------
```
可以看到Test are skipped，说明跳过了测试，到此为止，跳过测试已经讲了3种了：

1. `mvn -Dmaven.test.skip=tue`
2. properties中配置`<maven.test.skip>true</maven.test.skip>`
3. build中配置插件参数的方式

上面这个配置参数方式对当前插件的所有目标有效，如果想对指定的目标进行配置呢，用下面的方式。
<a name="kSULH"></a>
#### 插件目标参数配置
project->build->plugins->plugin->executions->execution元素中进行配置，如下：
```xml
<!-- 这个地方配置只对当前任务有效 -->
<configuration>
  <目标参数名>参数值</目标参数名>
</configuration>
```
上面这种配置常用于自定义插件绑定，只对当前任务有效。<br />感受一下效果，将pom.xml中的build元素改为下面内容：
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-surefire-plugin</artifactId>
      <version>2.12.4</version>
      <executions>
        <execution>
          <goals>
            <goal>test</goal>
            <goal>help</goal>
          </goals>
          <phase>pre-clean</phase>
          <!-- 这个地方配置只对当前任务有效 -->
          <configuration>
            <skip>true</skip>
          </configuration>
        </execution>
      </executions>
    </plugin>
  </plugins>
</build>
```
上面自定义了一个绑定，在clean周期的pre-clean阶段绑定了插件maven-surefire-plugin的两个目标test和help，execution元素没有指定id，所以默认id是default。<br />运行下面命令，见效果：
```
D:\code\IdeaProjects\maven-chat06>mvn pre-clean
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default) @ maven-chat06 ---
[INFO] Tests are skipped.
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:help (default) @ maven-chat06 ---
[INFO] Maven Surefire Plugin 2.12.4
  Surefire is a test framework project.

This plugin has 2 goals:

surefire:help
  Display help information on maven-surefire-plugin.
  Call mvn surefire:help -Ddetail=true -Dgoal=<goal-name> to display parameter
  details.

surefire:test
  Run tests using Surefire.


[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.738 s
[INFO] Finished at: 2019-11-18T17:41:08+08:00
[INFO] ------------------------------------------------------------------------
```
可以看到上面输出中运行了插件的两个目标，和预期结果一致。
<a name="zaPFp"></a>
### 获取Maven插件信息
上面介绍了，可以通过下面命令获取插件详细介绍信息
```bash
mvn help:describe -Dplugin=插件goupId:插件artifactId[:插件version] -Dgoal=目标名称 -Ddetail
mvn help:describe -Dplugin=插件前缀 -Dgoal=目标名称 -Ddetail
```
更多Maven插件的帮助文档可以参考Maven的官方网站，上面有详细的介绍，建议大家去看看，地址：[http://maven.apache.org/plugins/](http://maven.apache.org/plugins/)<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690189347141-eb8c6497-1a5e-4310-b034-7a56b16e21e1.jpeg#averageHue=%23e8e7e2&clientId=uc1a212f9-0166-4&from=paste&id=u38630c56&originHeight=738&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2539d06a-98d8-4f4a-8d5b-a8a2ce68b9e&title=)
<a name="K21ta"></a>
### 插件解析机制
为了方便用户使用和配置插件，Maven不需要用户提供完整的插件坐标信息，就可以解析到正确的插件，不过建议使用插件配置的时候最好还是配置完整的坐标信息，不然不利于新人的理解和问题的排查。
<a name="KJ0q4"></a>
#### 插件仓库
与其他Maven构件一样，插件构件也是基于坐标存储在Maven仓库中，有需要的时候，Maven会从本地查找插件，如果不存在，则到远程仓库查找，找到了以后下载到本地仓库，然后使用。<br />pom.xml中可以配置依赖的构件的仓库地址，如下：
```xml
<repositories>
  <repository>
    <id>maven-nexus</id>
    <url>http://localhost:8081/repository/maven-public/</url>
    <releases>
      <enabled>true</enabled>
    </releases>
    <snapshots>
      <enabled>true</enabled>
    </snapshots>
  </repository>
</repositories>
```
但是插件仓库的配置和这个有一点不一样，插件的是在pluginRepositories->pluginRepository元素中配置的，如下：
```xml
<pluginRepositories>
  <pluginRepository>
    <id>myplugin-repository</id>
    <url>http://repo1.maven.org/maven2/</url>
    <releases>
      <enabled>true</enabled>
    </releases>
  </pluginRepository>
</pluginRepositories>
```
看一下上面2段配置，repository中的配置和pluginRepository中的子元素是一样的，这个主意下就可以了。
<a name="qKUPA"></a>
#### 插件的默认groupId
在pom.xml中配置插件的时候，如果是官方的插件，可以省略groupId。<br />案例：<br />修改本篇示例中的pom.xml，如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>maven-chat06</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>jar</packaging>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>

  <build>
    <plugins>
      <plugin>
        <artifactId>maven-compiler-plugin</artifactId>
        <version>3.1</version>
        <configuration>
          <compilerVersion>1.8</compilerVersion>
          <source>1.8</source>
          <target>1.8</target>
        </configuration>
      </plugin>
    </plugins>
  </build>

</project>
```
上面用到了maven-compiler-plugin，这个插件是编译代码的，是Maven官方提供的插件，省略了groupId。<br />上面这个插件用于编译代码的，编译代码的时候需要指定编译器的版本，源码的版本，目标代码的版本，都是用的是1.8。<br />最开始的时候，在properties中有几个属性值是1.8的配置，这几个值默认会被maven-compiler-plugin这个插件的上面3个参数获取，具体可以去看一下这个插件compile目标的参数说明。<br />运行下面命令：
```
D:\code\IdeaProjects\maven-chat06>mvn clean compile
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ maven-chat06 ---
[INFO] Deleting D:\code\IdeaProjects\maven-chat06\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat06 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat06\target\classes
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.692 s
[INFO] Finished at: 2019-11-18T18:11:34+08:00
[INFO] ------------------------------------------------------------------------
```
可以看到可以正常运行。<br />上面pom.xml省略了插件的groupId配置，如下：
```xml
<groupId>org.apache.maven.plugins</groupId>
```
Maven在解析该插件的时候，会自动给这个插件补上默认的官方的groupId，所以可以正常运行，但是不建议大家这么使用，容易让新手比较懵逼。
<a name="h2sPj"></a>
#### 插件前缀的解析
前面说过了使用mvn命令调用插件的时候，可以使用插件的前缀来代替繁琐的插件坐标的方式，那么Maven是如何根据插件的前缀找到对应的插件的呢？<br />插件前缀与插件groupId:artifactId是一一对应的关系，这个关系的配置存储在仓库的元数据中，元数据位于下面2个xml中：
```
~/.m2/repository/org/apache/maven/plugins/maven-metadata-central.xml
~/.m2/repository/org/codehaus/mojo/maven-metadata-central.xml
```
接几个图，大家感受一下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690189347205-d6eb4529-35c7-4c71-97bd-fc06da33432a.png#averageHue=%23fcf7f5&clientId=uc1a212f9-0166-4&from=paste&id=u55b3e189&originHeight=573&originWidth=686&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u255ad7e3-08a7-43ba-a2b4-7decd723c87&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690189347138-4dc9b41c-bcc5-4ee1-abc7-fe6b43cddfce.png#averageHue=%23fcfbf9&clientId=uc1a212f9-0166-4&from=paste&id=u86be505c&originHeight=677&originWidth=660&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u92082534-b1ef-4bd2-9f05-f93bf5985c7&title=)<br />也可以通过在settings.xml中配置，让Maven检查其他grouId上的插件元数据中前缀和插件关系的配置，如下：
```xml
<settings>
  <pluginGroups>
    <pluginGroup>com.your.plugins</pluginGroup>
  </pluginGroups>
</settings>
```
pluginGroups中有多个pluginGroup，可以配置你自己插件的元数据所在的groupId，然后可以通过前缀来访问你自己的插件元数据目录。
<a name="Vcd5z"></a>
### 查看项目最终pom.xml文件
pom.xml默认会继承Maven顶级的一个父类pom.xml，顶级的pom.xml中指定了很多默认的配置，如生命周期中的阶段和很多插件的绑定，这些如果想看到，到哪里看呢？<br />mvn命令在项目中执行的时候，pom.xml和父类的pom.xml最终会进行合并，当pom.xml写的比较复杂的时候，最终合并之后是什么效果呢，可以通过下面这个命令查看：
```
mvn help:effective-pom
```
效果：
```
D:\code\IdeaProjects\maven-chat06>mvn help:effective-pom > 1.xml
```
上面将命令产生的结果输出到项目的1.xml文件中了，看一下项目的1.xml的内容：
```xml
<?xml version="1.0" encoding="GBK"?>
<!-- ====================================================================== -->
<!--                                                                        -->
<!-- Generated by Maven Help Plugin on 2019-11-18T18:41:40+08:00            -->
<!-- See: http://maven.apache.org/plugins/maven-help-plugin/                -->
<!--                                                                        -->
<!-- ====================================================================== -->
<!-- ====================================================================== -->
<!--                                                                        -->
<!-- Effective POM for project                                              -->
<!-- 'com.javacode2018:maven-chat06:jar:1.0-SNAPSHOT'                       -->
<!--                                                                        -->
<!-- ====================================================================== -->
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.javacode2018</groupId>
    <artifactId>maven-chat06</artifactId>
    <version>1.0-SNAPSHOT</version>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    </properties>
    <repositories>
        <repository>
            <snapshots>
                <enabled>false</enabled>
            </snapshots>
            <id>central</id>
            <name>Central Repository</name>
            <url>https://repo.maven.apache.org/maven2</url>
        </repository>
    </repositories>
    <pluginRepositories>
        <pluginRepository>
            <releases>
                <updatePolicy>never</updatePolicy>
            </releases>
            <snapshots>
                <enabled>false</enabled>
            </snapshots>
            <id>central</id>
            <name>Central Repository</name>
            <url>https://repo.maven.apache.org/maven2</url>
        </pluginRepository>
    </pluginRepositories>
    <build>
        <sourceDirectory>D:\code\IdeaProjects\maven-chat06\src\main\java</sourceDirectory>
        <scriptSourceDirectory>D:\code\IdeaProjects\maven-chat06\src\main\scripts</scriptSourceDirectory>
        <testSourceDirectory>D:\code\IdeaProjects\maven-chat06\src\test\java</testSourceDirectory>
        <outputDirectory>D:\code\IdeaProjects\maven-chat06\target\classes</outputDirectory>
        <testOutputDirectory>D:\code\IdeaProjects\maven-chat06\target\test-classes</testOutputDirectory>
        <resources>
            <resource>
                <directory>D:\code\IdeaProjects\maven-chat06\src\main\resources</directory>
            </resource>
        </resources>
        <testResources>
            <testResource>
                <directory>D:\code\IdeaProjects\maven-chat06\src\test\resources</directory>
            </testResource>
        </testResources>
        <directory>D:\code\IdeaProjects\maven-chat06\target</directory>
        <finalName>maven-chat06-1.0-SNAPSHOT</finalName>
        <pluginManagement>
            <plugins>
                <plugin>
                    <artifactId>maven-antrun-plugin</artifactId>
                    <version>1.3</version>
                </plugin>
                <plugin>
                    <artifactId>maven-assembly-plugin</artifactId>
                    <version>2.2-beta-5</version>
                </plugin>
                <plugin>
                    <artifactId>maven-dependency-plugin</artifactId>
                    <version>2.8</version>
                </plugin>
                <plugin>
                    <artifactId>maven-release-plugin</artifactId>
                    <version>2.5.3</version>
                </plugin>
            </plugins>
        </pluginManagement>
        <plugins>
            <plugin>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.1</version>
                <executions>
                    <execution>
                        <id>default-compile</id>
                        <phase>compile</phase>
                        <goals>
                            <goal>compile</goal>
                        </goals>
                        <configuration>
                            <compilerVersion>1.8</compilerVersion>
                            <source>1.8</source>
                            <target>1.8</target>
                        </configuration>
                    </execution>
                    <execution>
                        <id>default-testCompile</id>
                        <phase>test-compile</phase>
                        <goals>
                            <goal>testCompile</goal>
                        </goals>
                        <configuration>
                            <compilerVersion>1.8</compilerVersion>
                            <source>1.8</source>
                            <target>1.8</target>
                        </configuration>
                    </execution>
                </executions>
                <configuration>
                    <compilerVersion>1.8</compilerVersion>
                    <source>1.8</source>
                    <target>1.8</target>
                </configuration>
            </plugin>
            <plugin>
                <artifactId>maven-clean-plugin</artifactId>
                <version>2.5</version>
                <executions>
                    <execution>
                        <id>default-clean</id>
                        <phase>clean</phase>
                        <goals>
                            <goal>clean</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
            <plugin>
                <artifactId>maven-resources-plugin</artifactId>
                <version>2.6</version>
                <executions>
                    <execution>
                        <id>default-testResources</id>
                        <phase>process-test-resources</phase>
                        <goals>
                            <goal>testResources</goal>
                        </goals>
                    </execution>
                    <execution>
                        <id>default-resources</id>
                        <phase>process-resources</phase>
                        <goals>
                            <goal>resources</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
            <plugin>
                <artifactId>maven-jar-plugin</artifactId>
                <version>2.4</version>
                <executions>
                    <execution>
                        <id>default-jar</id>
                        <phase>package</phase>
                        <goals>
                            <goal>jar</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
            <plugin>
                <artifactId>maven-surefire-plugin</artifactId>
                <version>2.12.4</version>
                <executions>
                    <execution>
                        <id>default-test</id>
                        <phase>test</phase>
                        <goals>
                            <goal>test</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
            <plugin>
                <artifactId>maven-install-plugin</artifactId>
                <version>2.4</version>
                <executions>
                    <execution>
                        <id>default-install</id>
                        <phase>install</phase>
                        <goals>
                            <goal>install</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
            <plugin>
                <artifactId>maven-deploy-plugin</artifactId>
                <version>2.7</version>
                <executions>
                    <execution>
                        <id>default-deploy</id>
                        <phase>deploy</phase>
                        <goals>
                            <goal>deploy</goal>
                        </goals>
                    </execution>
                </executions>
            </plugin>
            <plugin>
                <artifactId>maven-site-plugin</artifactId>
                <version>3.3</version>
                <executions>
                    <execution>
                        <id>default-site</id>
                        <phase>site</phase>
                        <goals>
                            <goal>site</goal>
                        </goals>
                        <configuration>
                            <outputDirectory>D:\code\IdeaProjects\maven-chat06\target\site</outputDirectory>
                            <reportPlugins>
                                <reportPlugin>
                                    <groupId>org.apache.maven.plugins</groupId>
                                    <artifactId>maven-project-info-reports-plugin</artifactId>
                                </reportPlugin>
                            </reportPlugins>
                        </configuration>
                    </execution>
                    <execution>
                        <id>default-deploy</id>
                        <phase>site-deploy</phase>
                        <goals>
                            <goal>deploy</goal>
                        </goals>
                        <configuration>
                            <outputDirectory>D:\code\IdeaProjects\maven-chat06\target\site</outputDirectory>
                            <reportPlugins>
                                <reportPlugin>
                                    <groupId>org.apache.maven.plugins</groupId>
                                    <artifactId>maven-project-info-reports-plugin</artifactId>
                                </reportPlugin>
                            </reportPlugins>
                        </configuration>
                    </execution>
                </executions>
                <configuration>
                    <outputDirectory>D:\code\IdeaProjects\maven-chat06\target\site</outputDirectory>
                    <reportPlugins>
                        <reportPlugin>
                            <groupId>org.apache.maven.plugins</groupId>
                            <artifactId>maven-project-info-reports-plugin</artifactId>
                        </reportPlugin>
                    </reportPlugins>
                </configuration>
            </plugin>
        </plugins>
    </build>
    <reporting>
        <outputDirectory>D:\code\IdeaProjects\maven-chat06\target\site</outputDirectory>
    </reporting>
</project>
```
上面这个文件，一定要认真多看几遍，理解一下，里面包含太多东西，
