Maven
<a name="UUyZu"></a>
## Maven 与构建
<a name="bssHh"></a>
### Maven
Maven是跨平台的项目管理工具。Apache 组织的开源项目。主要服务于基于 Java 平台的项目构建、依赖管理和项目信息管理。<br />类似于 linux 平台的 yum、apt，前端领域的 npm。Maven 前身为 Ant 目前 tomcat 的源码就是用 Ant 来构建和管理，更先进的工具有 Gradle， Spring 工程在用。
<a name="vHOz2"></a>
### 构建
构建：编译、运行单元测试、生成文档、打包、部署的过程，这就是构建。<br />构建的步骤：

- `清理 clean`：将以前编译得到的旧文件 class 字节码文件删除。
- `编译 compile`：将 java 源程序编译成 class 字节码文件。
- `测试 test`：自动测试，自动调用 junit 程序。
- `报告 report`：测试程序执行的结果。
- `打包 package`：动态 Web 工程打 War 包，java 工程打 jar 包。
- `安装 install`：将打包得到的文件复制到 “仓库” 中的指定位置（Maven特定的概念）。
- `部署 deploy`：将动态 Web 工程生成的 war 包复制到 Servlet 容器下，使其可以运行。
<a name="JG7FK"></a>
### Maven项目骨架
pom：Project Object Model
```
根目录：工程名
|---src：源码
|---|---main:主程序
|---|---|---java：主程序代码路径
|---|---|---resource：主程序配置文件路径
|---|---test：测试
|---|---|---java：测试代码路径
|---|---|---resource：测试配置文件路径
|---pom.xml：maven 配置文件
```
> 如果项目结构不符合Maven项目机构，则在打包编译时不能正确的识别需要编译的Java文件以及静态资源。

<a name="XKbct"></a>
### 简单演示
```bash
## 1. 使用 archetype 命令生成 maven 简单骨架
mvn archetype:generate -DarchetypeCatalog=internal
## 2. 编译当前生成的项目
mvn compile
## 3. 使用其他命令
mvn test-compile  
mvn package  
mvn clean 
mvn install
mvn depoly 暂时不演示
```
<a name="Msc7Y"></a>
## 坐标与依赖
<a name="Pawhc"></a>
### 坐标
类比为数学中平面几何，坐标（x、y ），任何一个坐标都能唯一标识该平面中的一个点。<br />该点对应到 Maven 就是 .jar、.war 等文件的文件。<br />Maven 使用 `groupId`、`artifactId`、`version`、`packaging`、`classifier` 等元素来组成自己的坐标，并定义一组这样的规则，只要能提供正确坐标元素 Maven 就能找到对应的构件。
<a name="6ahS6"></a>
### 坐标元素

- `groupId`：定义当前 Maven 项目隶属的实际项目。
- `artifactId`：定义实际项目中的一个 Maven 项目（模块）。
- `packaging`：定义 Maven 项目打包方式。jar、war、pom。默认为 jar。
- `version`：定义 Maven 项目当前所处的版本。
- `classifier`：区分从同一 artifact 构建的具有不同内容的构件。
<a name="u7UUb"></a>
#### classifier 使用场景
区分基于不同 JDK 版本的包
```xml
<dependency>  
  <groupId>net.sf.json-lib</groupId>   
  <artifactId>json-lib</artifactId>   
  <version>2.2.2</version>  
  <classifier>jdk13</classifier>    
  <!--<classifier>jdk15</classifier>-->
</dependency>
```
区分项目的不同组成部分
```xml
<dependency>  
  <groupId>net.sf.json-lib</groupId>   
  <artifactId>json-lib</artifactId>   
  <version>2.2.2</version>  
  <classifier>jdk15-javadoc</classifier>    
  <!--<classifier>jdk15-sources</classifier>  -->
</dependency>
```
构件名与坐标是对应的，一般规则是：artifactId-version[-classifier].packaging。
<a name="OptND"></a>
### 依赖声明
```xml
<dependencies>
  <dependency>
    <groupId></groupId>
    <artifactId></artifactId>
    <version></version>
    <type></type>
    <optional></optional>
    <exclusions>
      <exclusion>
        <artifactId></artifactId>
        <groupId></groupId>
      </exclusion>
      ...
    </exclusions>
  </dependency>
  ...
</dependencies>
```

- `groupId`、`artifactId`、`version`：依赖的基本坐标。
- `type`：依赖的类型，对应项目对应的 packaging，一般不必声明。
- `scope`：依赖的范围，后面详解。
- `optional`：标记依赖是否可选。
- `exclusions`：用来排除传递性依赖。
<a name="EMrDy"></a>
### 依赖范围

- compile：编译依赖范围<br />如果没有指定，默认使用该依赖范围。对于编译、测试、运行三种 classpath 都有效。如：spring-core。
- test：测试依赖范围<br />只对于测试 classpath 有效，只需要在编译测试及运行测试才需要，在打包的时候不会打进去。如：JUnit。
- provided：已提供依赖范围<br />对于编译和测试 classpath 有效，但运行时无效。如：servlet-api 编译和测试项目的时候都需要，但在实际运行中，容器已经提供，不需要 Maven 重复的引用。
- runtime：运行时依赖范围<br />对于测试和运行的 classpath 有效，但在编译主代码时无效。如：JDBC 驱动的实现包。只有在执行测试或者运行项目时，才需要具体的 JDBC 驱动。
- system：系统依赖范围<br />与 provided 依赖范围完全一致，但是使用该范围时必须通过 systemPath 元素显式地指定依赖文件的路径。由于此类依赖不是通过 Maven 仓库解析的，而且往往与本机系统绑定，可能造成构建不可移植，因此应该谨慎使用。systemPath 元素可以引用环境变量，如：
```xml
<dependencies>
  <dependency>
    <groupId>javax.sql</groupId>
    <artifactId>jdbc-stdxt</artifactId>
    <version>2.0</version>
    <scope>system</scope>
    <systemPath>${java.home}/lib/rt.jar</systemPath>
  </dependency>
</dependencies>
```

- import：导入依赖范围<br />只在 dependencyManagement 标签中生效，导入已经定义好的 pom 文件中 dependencyManagement 节点内容
```xml
<dependencyManagement>
  <dependencies>
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-framework-bom</artifactId>
      <version>4.3.16.RELEASE</version>
      <type>pom</type>
      <scope>import</scope>
    </dependency>
  </dependencies>
</dependencyManagement>
```
<a name="QHJiL"></a>
### 依赖机制与特性
<a name="OZE7U"></a>
#### 依赖传递

- A->B(compile)：第一直接依赖
- B->C(compile)：第二直接依赖
- A->C(compile)：传递性依赖

当在A中配置
```xml
<dependency>  
    <groupId>com.B</groupId>  
    <artifactId>B</artifactId>  
    <version>1.0</version>  
</dependency>
```
则会自动导入 C 包。<br />传递性依赖的范围如下图所示：

|           传递依赖<br />直接依赖 | compile | test | provided | runtime |
| --- | --- | --- | --- | --- |
| compile | **compile** | **-** | **-** | **runtime** |
| test | **test** |  | **-** | **test** |
| provided | **provided** | **-** | **provided** | **provided** |
| runtime | **runtime** | **-** | **-** | **runtime** |

<a name="7yNVx"></a>
#### 依赖调解
当传递性依赖出现问题时，能够清楚地知道该传递性依赖是从哪条依赖路径中引入的。<br />一、路径最近者优先原则

- A->B->C->X(1.0)
- A->D->X(2.0)

由于只能导入一个版本的包，按照最短路径选择导入 X(2.0)<br />二、第一声明者优先原则

- A->B->Y(1.0)
- A->C->Y(2.0)

此时由于依赖路径长度一致，按照第一声明者优先原则。在路径长度一致的前提下，如果 B 依赖在 POM 文件中声明顺序在 C 依赖之前，那么 Y(1.0) 则会被引入。如下依赖可用于测试：
```xml
<dependencies>
  <dependency>
    <groupId>org.apache.httpcomponents</groupId>
    <artifactId>httpclient</artifactId>
    <version>4.4.1</version>
    <exclusions>
      <exclusion>
        <groupId>commons-codec</groupId>
        <artifactId>commons-codec</artifactId>
      </exclusion>
    </exclusions>
  </dependency>
  <dependency>
    <groupId>org.apache.poi</groupId>
    <artifactId>poi</artifactId>
    <version>3.9</version>
    <exclusions>
      <exclusion>
        <groupId>commons-codec</groupId>
        <artifactId>commons-codec</artifactId>
      </exclusion>
    </exclusions>
  </dependency>
  <dependency>
    <groupId>commons-codec</groupId>
    <artifactId>commons-codec</artifactId>
    <version>1.10</version>
  </dependency>
</dependencies>
```
这里有一点需要特别注意，看如下依赖：
```xml
<dependencies>
  <dependency>
    <groupId>commons-codec</groupId>
    <artifactId>commons-codec</artifactId>
    <version>1.11</version>
  </dependency>
  <dependency>
    <groupId>commons-codec</groupId>
    <artifactId>commons-codec</artifactId>
    <version>1.10</version>
  </dependency>
</dependencies>
```
按照两原则，期望得到的结果应该是 1.11 版本的构建将被依赖。但实际结果却依赖了 1.10 版本。这不是违反了 Maven 依赖调解的最先定义原则？<br />其实这个是 dependency 插件的功能，默认采用的是复写的策略，当构建声明处于同一 pom 中，且 groupid 和 artifactId 一致时，以最新声明为准，后面的覆盖前面的。<br />注意这里没涉及到依赖调解的功能。依赖调解只发生于构建来自不同 pom 时，而此时构建声明处于同一 pom，故不会触发依赖调解。
<a name="iHFF8"></a>
#### 可选依赖
A->B、B->X（可选）、B->Y（可选）。<br />项目 A 依赖于项目 B，项目 B 依赖于项目 X 和 Y。<br />理论上项目 A 中，会把 B、X、Y 项目都依赖进来。<br />但是 X、Y 两个依赖对于 B 来讲可能是互斥的，如 B 是数据库隔离包，支持多种数据库 MySQL、Oracle，在构建 B 项目时，需要这两种数据库的支持，但在使用这个工具包时，只会依赖一个数据库。<br />此时就需要在 B 项目 pom 文件中将 X、Y 声明为可选依赖，如下：
```xml
<dependency>  
    <groupId>com.X</groupId>  
    <artifactId>X</artifactId>  
    <version>1.0</version>  
    <optionnal>true</optionnal>
</dependency>
<dependency>  
    <groupId>com.Y</groupId>  
    <artifactId>Y</artifactId>  
    <version>1.0</version>  
    <optionnal>true</optionnal>
</dependency>
```
使用 optionnal 元素标识以后，只会对当前项目 B 产生影响，当其他的项目依赖 B 项目时，这两个依赖都不会被传递。<br />项目 A 依赖于项目 B，如果实际应用数据库是 X， 则在 A 的 pom 中就需要显式地声明 X 依赖。
<a name="N4ezi"></a>
#### 排除依赖
A项目的pom.xml中
```xml
<dependency>
  <groupId>com.javacode2018</groupId>
  <artifactId>B</artifactId>
  <version>1.0</version>
</dependency>
```
B项目1.0版本的pom.xml中
```xml
<dependency>
  <groupId>com.javacode2018</groupId>
  <artifactId>C</artifactId>
  <version>1.0</version>
</dependency>
```
上面A->B的1.0版本，B->C的1.0版本，而scope都是默认的compile，根据前面讲的依赖传递性，C会传递给A，会被A自动依赖，但是C此时有个更新的版本2.0，A想使用2.0的版本，此时A的pom.xml中可以这么写：
```xml
<dependency>
  <groupId>com.javacode2018</groupId>
  <artifactId>B</artifactId>
  <version>1.0</version>
  <exclusions>
    <exclusion>
      <groupId>com.javacode2018</groupId>
      <artifactId>C</artifactId>
    </exclusion>
  </exclusions>
</dependency>
```
上面使用使用`exclusions`元素排除了B->C依赖的传递，也就是B->C不会被传递到A中。<br />`exclusions`中可以有多个`exclusion`元素，可以排除一个或者多个依赖的传递，声明`exclusion`时只需要写上groupId、artifactId就可以了，version可以省略。
<a name="hiyJW"></a>
## 仓库
在 Maven 中，任何一个依赖、插件或者项目构建的输出，都可以称之为构件。<br />在 Maven 中，仓库是一个位置，这个位置是用来存放各种第三方构件的，所有Maven项目可以共享这个仓库中的构件。<br />Maven 仓库能管理构件（主要是jar包），它就是放置所有jar文件（jar、war、zip、pom等等）的地方。<br />仓库分类：包括本地仓库和远程仓库。其中远程仓库包括：私服和中央仓库。<br />搜索构建的顺序：

- 本地仓库
- maven settings profile 中的 repository；
- pom.xml 中 profile 中定义的repository；
- pom.xml 中 repositorys (按定义顺序找)；
- maven settings mirror；
- central 中央仓库；
<a name="mlZBq"></a>
### 仓库的分类
主要分为2大类：

1. **本地仓库**
2. **远程仓库**

**而远程仓库又分为：中央仓库、私服、其他公共远程仓库**<br />当Maven根据坐标寻找构件的时候，会首先查看本地仓库，如果本地仓库存在，则直接使用；如果本地不存在，Maven会去远程仓库中查找，如果找到了，会将其下载到本地仓库中进行使用，如果本地和远程仓库都没有找到构件，Maven会报错，构件只有在本地仓库中存在了，才能够被Maven项目使用。
<a name="xkHGs"></a>
#### 本地仓库
默认情况下，Maven本地仓库默认地址是~/.m2/respository目录，这个默认也可以在~/.m2/settings.xml文件中进行修改：
```xml
<localRepository>本地仓库地址</localRepository>
```
当使用Maven的时候，依赖的构件都会从远程仓库下载到本地仓库目录中。<br />Maven 的本地仓库，在安装 Maven 后并不会创建，当执行第一条 Maven 命令的时候本地仓库才会创建，此时会从远程仓库下载构建到本地仓库给Maven项目使用。<br />需要注意，默认情况下，~/.m2/settings.xml这个文件是不存在的（~是指用户目录，前面的文章中有介绍过，此处不再做说明），需要从Maven安装目录中拷贝conf/settings.xml文件，将M2_HOME/conf/settings.xml拷贝到~/.m2目录中，然后对~/.m2/settings.xml进行编辑，M2_HOME/config/settings.xml这个文件其实也是可以使用的，不过不建议直接使用，这个修改可能会影响其他所有使用者，还有修改了这个文件，也不利于以后Maven的升级，如果使用~/.m2/settings.xml，而Maven安装目录中的配置不动，升级的时候只需要替换一下安装包就好了，所以建议将Maven安装目录中的settings.xml拷贝到~/.m2中进行编辑，这个是用户级别的，只会影响当前用户。
<a name="EpanO"></a>
#### 远程仓库
最开始使用Maven的时候，本地仓库中的构件是空的，此时Maven必须要提供一种功能，要能够从外部获取这些构件，这个外部就是所谓的远程仓库，远程仓库可以有多个，当本地仓库找不到构件时，可以去远程仓库找，然后放置到本地仓库中进行使用。
<a name="f0GP3"></a>
##### 中央仓库
由于Maven刚安装好的时候，本地仓库是空的，此时什么都没有配置，去执行Maven命令的时候，会看到Maven默认执行了一些下载操作，这个下载地址就是中央仓库的地址，这个地址是Maven社区提供的，是Maven内置的一个默认的远程仓库地址，不需要用户去配置。<br />这个地址在Maven安装包的什么地方呢？<br />这里使用的是3.6.1，在下面这个位置
```
apache-maven-3.6.1\lib\maven-model-builder-3.6.1.jar\org\apache\maven\model\pom-4.0.0.xml
```
在pom-4.0.0.xml中，如下：
```xml
<repositories>
  <repository>
    <id>central</id>
    <name>Central Repository</name>
    <url>https://repo.maven.apache.org/maven2</url>
    <layout>default</layout>
    <snapshots>
      <enabled>false</enabled>
    </snapshots>
  </repository>
</repositories>
```
就是：
```
https://repo.maven.apache.org/maven2
```
可以去访问一下，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690167040224-bb6a2a29-3354-4137-ae05-7261f098f7a0.png#averageHue=%23fefefe&clientId=u85964341-97ad-4&from=paste&id=u2f15e529&originHeight=673&originWidth=519&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3a69e7b3-cc5c-4b05-b6be-a90824b9902&title=)<br />上面站点中包含了很多常用的构建。<br />中央仓库有几个特点：

1. 中央仓库是由Maven官方社区提供给大家使用的
2. 不需要手动去配置，Maven内部集成好了
3. 使用中央仓库时，机器必须是联网状态，需要可以访问中央仓库的地址

中央仓库还提供了一个检索构件的站点：[https://search.maven.org/](https://search.maven.org/)<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690167040114-55506088-85b5-4f5e-8840-951d332b76af.jpeg#averageHue=%23f8f8f8&clientId=u85964341-97ad-4&from=paste&id=ud4a9f553&originHeight=469&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u576f9067-5851-4900-8806-2ac9d12d851&title=)<br />非常方便查找需要依赖的构件，大家可以去体验一下。<br />中央仓库中包含了这个世界上大多数流行的开源java构件，基本上所有的jave开发者都会使用这个仓库，一般需要的第三方构件在这里都可以找到。
<a name="o2zqa"></a>
##### 私服
私服也是远程仓库中的一种，为什么需要私服呢？<br />如果一个团队中有几百个人在开发一些项目，都是采用Maven的方式来组织项目，那么每个人都需要从远程仓库中把需要依赖的构件下载到本地仓库，这对公司的网络要求也比较高，为了节省这个宽带和加快下载速度，在公司内部局域网内部可以架设一台服务器，这台服务器起到一个代理的作用，公司里面的所有开发者去访问这个服务器，这台服务器将需要的构建返回给开发者，如果这台服务器中也没有需要的构建，那么这个代理服务器会去远程仓库中查找，然后将其先下载到代理服务器中，然后再返回给开发者本地的仓库。<br />还有公司内部有很多项目之间会相互依赖，你可能是架构组的，需要开发一些jar包给其他组使用，此时，可以将自己jar发布到私服中给其他同事使用，如果没有私服，可能需要手动发给别人或者上传到共享机器中，不过管理起来不是很方便。<br />**总体上来说私服有以下好处：**

1. 加速Maven构件的下载速度
2. 节省宽带
3. 方便部署自己的构件以供他人使用
4. 提高Maven的稳定性，中央仓库需要本机能够访问外网，而如果采用私服的方式，只需要本机可以访问内网私服就可以了
<a name="OQCnD"></a>
##### 其他远程仓库
中央仓库是在国外的，访问速度不是特别快，所以有很多比较大的公司做了一些好事，自己搭建了一些Maven仓库服务器，公开出来给其他开发者使用，比如像阿里、网易等等，他们对外提供了一些Maven仓库给全球开发者使用，在国内的访问速度相对于Maven中央仓库来说还是快了不少。<br />还有一些公司比较牛，只在自己公开的仓库中发布构件，这种情况如果要使用他们的构件时，需要去访问他们提供的远程仓库地址。
<a name="jccZs"></a>
### 构建文件的布局
来看一下构件在仓库的文件结构中是如何组成的？<br />这块以本地仓库来做说明，远程仓库中组织构件的方式和本地仓库是一样的，以fastjson在本地仓库中的信息为例来做说明，如下：
```xml
<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>fastjson</artifactId>
  <version>1.2.62</version>
</dependency>
```
上面是fastjson 1.2.62这个jar，看一下这个jar在本地仓库中的位置，如下图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690167040410-12dbb0d7-4949-4293-ae05-0c68c068fce8.png#averageHue=%23fbfaf9&clientId=u85964341-97ad-4&from=paste&id=ua955bdbc&originHeight=222&originWidth=660&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubae23560-f5d2-4977-ab8a-575a0b65a99&title=)<br />fastjson这个jar的地址是：
```
~\.m2\repository\com\alibaba\fastjson\1.2.62\fastjson-1.2.62.jar
```
~\.m2\repository\是仓库的目录，所有本地构件都位于该目录中，主要看一下后面的部分，是怎么构成的。<br />构件所在目录的构成如下：
```
groupId+"."+artifactId+"."+版本号
```
通过上面获取一个字符串，字符串由groupId、artifactId、版本号之间用.连接，然后将这个字符串中的.替换为文件目录分隔符然后创建多级目录。<br />而构件文件名称的组成如下：
```
[artifactId][-verion][-classifier].[type]
```
上面的fastjson-1.2.62.jar信息如下：
```
artifactId为fastjson
version为1.2.62
classifier为空
type没有指定，默认为jar
```
所以构件文件名称为fastjson-1.2.62.jar。
<a name="SSqvQ"></a>
### 关于构件版本问题
平时开发项目的时候，打包测试，或者将自己开发的构建提供给他人使用时，中间反反复复的打包测试，会给使用方提供很多不稳定的版本，最终经过同事和测试反复验证修改，会发布一个稳定的版本。<br />在发布稳定版本之前，会有很多个不稳定的测试版本，这些版本称为快照版本，用SNAPSHOT表示，回头去看看本文开头搭建的maven-cha03的pom.xml文件，默认是快照版本的，如下：
```
<version>1.0-SNAPSHOT</version>
```
version以-SNAPSHOT结尾的，表示这是一个不稳定的版本，这个版本最好只在公司内部测试的时候使用，最终发布的时候，需要将-SNAPSHOT去掉，然后发布一个稳定的版本，表示这个版本是稳定的，可以直接使用，这种稳定的版本叫做release版本。<br />想控制构件获取的远程地址时，该怎么做呢？此时需要使用远程仓库的配置功能。
<a name="FWvWO"></a>
### Maven中远程仓库的配置
此处讲解2种方式。
<a name="Gs3k4"></a>
#### 方式1
<a name="u1Q0X"></a>
##### pom.xml中配置远程仓库，语法如下：
```xml
<project>
  <repositories>
    <repository>
      <id>aliyun-releases</id>
      <url>https://maven.aliyun.com/repository/public</url>
      <releases>
        <enabled>true</enabled>
      </releases>
      <snapshots>
        <enabled>false</enabled>
      </snapshots>
    </repository>
  </repositories>
</project>
```
在repositories元素下，可以使用repository子元素声明一个或者多个远程仓库。<br />repository元素说明：

- id：远程仓库的一个标识，中央仓库的id是central，所以添加远程仓库的时候，id不要和中央仓库的id重复，会把中央仓库的覆盖掉
- url：远程仓库地址
- releases：主要用来配置是否需要从这个远程仓库下载稳定版本构建
- snapshots：主要用来配置是否需要从这个远程仓库下载快照版本构建

releases和snapshots中有个enabled属性，是个boolean值，默认为true，表示是否需要从这个远程仓库中下载稳定版本或者快照版本的构建，一般使用第三方的仓库，都是下载稳定版本的构建。<br />快照版本的构建以-SNAPSHOT结尾，稳定版没有这个标识。
<a name="w72w3"></a>
##### 示例
来感受一下pom方式配置远程仓库的效果。<br />文本编辑器打开maven-chat03/pom.xml，将下面内容贴进去：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>maven-chat03</artifactId>
  <version>1.0-SNAPSHOT</version>

  <dependencies>
    <dependency>
      <groupId>com.alibaba</groupId>
      <artifactId>fastjson</artifactId>
      <version>1.2.62</version>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
      <version>2.2.1.RELEASE</version>
    </dependency>
  </dependencies>

  <repositories>
    <repository>
      <id>aliyun-releases</id>
      <url>https://maven.aliyun.com/repository/public</url>
      <releases>
        <enabled>true</enabled>
      </releases>
      <snapshots>
        <enabled>false</enabled>
      </snapshots>
    </repository>
  </repositories>

</project>
```
上面配置了一个远程仓库，地址是阿里云的Maven仓库地址，releases的enabled为true，snapshots的enabled为false，表示这个远程仓库只允许下载稳定版本的构件，而不能从这个仓库中下载快照版本的构建。<br />删除本地仓库中以下几个目录：
```
~\.m2\repository\org\springframework
~\.m2\repository\com\alibaba
```
maven-chat03项目目录中打开cmd运行：
```bash
mvn compile
```
输出如下：
```
D:\code\IdeaProjects\maven-chat03>mvn compile
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat03 >--------------------
[INFO] Building maven-chat03 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
Downloading from aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-starter-web/2.2.1.RELEASE/spring-boot-starter-web-2.2.1.RELEASE.pom
Downloaded from aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-starter-web/2.2.1.RELEASE/spring-boot-starter-web-2.2.1.RELEASE.pom (3.3 kB at 5.1 kB/s)
Downloading from aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-starters/2.2.1.RELEASE/spring-boot-starters-2.2.1.RELEASE.pom
Downloaded from aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-starters/2.2.1.RELEASE/spring-boot-starters-2.2.1.RELEASE.pom (1.8 kB at 8.0 kB/s)
Downloading from aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-parent/2.2.1.RELEASE/spring-boot-parent-2.2.1.RELEASE.pom
Downloaded from aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-parent/2.2.1.RELEASE/spring-boot-parent-2.2.1.RELEASE.pom (1.8 kB at 8.6 kB/s)
Downloaded from aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/spring-expression/5.2.1.RELEASE/spring-expression-5.2.1.RELEASE.jar (282 kB at 82 kB/s)
Downloaded from aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/spring-webmvc/5.2.1.RELEASE/spring-webmvc-5.2.1.RELEASE.jar (946 kB at 219 kB/s)
Downloaded from aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/spring-beans/5.2.1.RELEASE/spring-beans-5.2.1.RELEASE.jar (684 kB at 56 kB/s)
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat03 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat03 ---
[INFO] Changes detected - recompiling the module!
[WARNING] File encoding has not been set, using platform encoding GBK, i.e. build is platform dependent!
[INFO] Compiling 1 source file to D:\code\IdeaProjects\maven-chat03\target\classes
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  24.079 s
[INFO] Finished at: 2019-11-12T15:33:27+08:00
[INFO] ------------------------------------------------------------------------
```
输出中有很多Downloaded from aliyun-releases，Downloaded from后面跟的aliyun-releases就是上面在pom.xml中配置的远程仓库repository元素中的id，后面还可以看到很多下载地址，这个地址就是上面在pom.xml中指定的远程仓库的地址，可以看到项目中依赖的构建从指定的远程仓库中下载了。<br />pom中配置远程仓库的方式只对当前项目起效，如果需要对所有项目起效，可以下面的方式2，向下看。
<a name="GjilL"></a>
#### 方式2
<a name="amgfs"></a>
##### 镜像的方式
如果仓库X可以提供仓库Y所有的内容，那么就可以认为X是Y的一个镜像，通俗点说，可以从Y获取的构件都可以从他的镜像中进行获取。<br />可以采用镜像的方式配置远程仓库，镜像在settings.xml中进行配置，对所有使用该配置的Maven项目起效，配置方式如下：
```xml
<mirror>
  <id>mirrorId</id>
  <mirrorOf>repositoryId</mirrorOf>
  <name>Human Readable Name for this Mirror.</name>
  <url>http://my.repository.com/repo/path</url>
</mirror>
```
mirrors元素下面可以有多个mirror元素，每个mirror元素表示一个远程镜像，元素说明：

- id：镜像的id，是一个标识
- name：镜像的名称，这个相当于一个描述信息，方便大家查看
- url：镜像对应的远程仓库的地址
- mirrorOf：指定哪些远程仓库的id使用这个镜像，这个对应pom.xml文件中repository元素的id，就是表示这个镜像是给哪些pom.xml文章中的远程仓库使用的，这里面需要列出远程仓库的id，多个之间用逗号隔开，*表示给所有远程仓库做镜像

这里主要对mirrorOf再做一下说明，上面在项目中定义远程仓库的时候，pom.xml文件的repository元素中有个id，这个id就是远程仓库的id，而mirrorOf就是用来配置哪些远程仓库会走这个镜像去下载构件。<br />mirrorOf的配置有以下几种:
```xml
<mirrorOf>*</mirrorOf>
```
上面匹配所有远程仓库id，这些远程仓库都会走这个镜像下载构件
```xml
<mirrorOf>远程仓库1的id,远程仓库2的id</mirrorOf>
```
上面匹配指定的仓库，这些指定的仓库会走这个镜像下载构件
```xml
<mirrorOf>*,! repo1</mirrorOf>
```
上面匹配所有远程仓库，repo1除外，使用感叹号将仓库从匹配中移除。<br />需要注意镜像仓库完全屏蔽了被镜像的仓库，所以当镜像仓库无法使用的时候，Maven是无法自动切换到被镜像的仓库的，此时下载构件会失败，这个需要了解。
<a name="QMcMb"></a>
##### 示例
将maven-chat03中的pom.xml修改为：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>maven-chat03</artifactId>
  <version>1.0-SNAPSHOT</version>

  <dependencies>
    <dependency>
      <groupId>com.alibaba</groupId>
      <artifactId>fastjson</artifactId>
      <version>1.2.62</version>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
      <version>2.2.1.RELEASE</version>
    </dependency>
  </dependencies>

</project>
```
修改~/.m2/settings.xml，加入镜像配置，如下：
```xml
<mirrors>
  <mirror>
    <id>mirror-aliyun-releases</id>
    <mirrorOf>*</mirrorOf>
    <name>阿里云maven镜像</name>
    <url>https://maven.aliyun.com/repository/public</url>
  </mirror>
</mirrors>
```
上面配置了一个阿里云的镜像，注意镜像的id是mirror-aliyun-releases<br />下面就来验证一下镜像的效果。<br />删除本地仓库中的以下几个目录：
```
~\.m2\repository\org\springframework
~\.m2\repository\com\alibaba
```
maven-chat03项目目录中打开cmd运行：
```bash
mvn compile
```
输出如下：
```
D:\code\IdeaProjects\maven-chat03>mvn compile
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat03 >--------------------
[INFO] Building maven-chat03 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
Downloading from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/com/alibaba/fastjson/1.2.62/fastjson-1.2.62.pom
Downloaded from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/com/alibaba/fastjson/1.2.62/fastjson-1.2.62.pom (9.7 kB at 17 kB/s)
Downloading from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-starter-web/2.2.1.RELEASE/spring-boot-starter-web-2.2.1.RELEASE.pom
Downloaded from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-starter-web/2.2.1.RELEASE/spring-boot-starter-web-2.2.1.RELEASE.pom (3.3 kB at 15 kB/s)
Downloading from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-starters/2.2.1.RELEASE/spring-boot-starters-2.2.1.RELEASE.pom
Downloaded from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-starters/2.2.1.RELEASE/spring-boot-starters-2.2.1.RELEASE.pom (1.8 kB at 8.3 kB/s)
Downloading from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-parent/2.2.1.RELEASE/spring-boot-parent-2.2.1.RELEASE.pom
Downloaded from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-parent/2.2.1.RELEASE/spring-boot-parent-2.2.1.RELEASE.pom (1.8 kB at 8.2 kB/s)
Downloading from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-dependencies/2.2.1.RELEASE/spring-boot-dependencies-2.2.1.RELEASE.pom
Downloaded from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/boot/spring-boot-dependencies/2.2.1.RELEASE/spring-boot-dependencies-2.2.1.RELEASE.pom (127 kB at 311 kB/s)
Downloading from mirror-aliyun-releases: https://maven.aliyun.com/repository/public/org/springframework/spring-framework-bom/5.2.1.RELEASE/spring-framework-bom-5.2.1.RELEASE.pom
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat03 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ maven-chat03 ---
[INFO] Nothing to compile - all classes are up to date
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  15.967 s
[INFO] Finished at: 2019-11-12T16:44:57+08:00
[INFO] ------------------------------------------------------------------------
```
上面复制了部分内容，大家仔细看一下Downloaded from后面显示的是mirror-aliyun-releases，这个和settings.xml中镜像的id一致，表示配置的镜像起效了，所有依赖的构建都从镜像来获取了。<br />**关于镜像一个比较常用的用法是结合私服一起使用，由于私服可以代理所有远程仓库（包含中央仓库），因此对于Maven用来来说，只需通过访问一个私服就可以间接访问所有外部远程仓库了。**
<a name="89A0b"></a>
## 生命周期
Maven 的生命周期是为了对所有构建过程进行的抽象和统一，其中包含项目的`清理`、`初始化`、`编译`、`测试`、`打包`、`集成测试`、`验证`、`部署`和`站点`生成等几乎所有的构建步骤。<br />Maven 的生命周期是抽象的，本身是不做任何实际的工作。实际的任务都交给插件来完成。<br />意味着 Maven 只在父类中定义了算法的整体结构，子类通过重写父类的方法，来控制实际行为（设计模式中的模板方法 Template Method）。伪代码如下：
```java
public abstract class AbstractBuilder {
    public void build() {
        init();
        compile();
        test();
        package();
        integrationTest();
        deploy();
    }
    
    protected abstract void init();
    protected abstract void compile();
    protected abstract void test();
    protected abstract void package();
    protected abstract void integrationTest();
    protected abstract void deploy();
}
```
<a name="U9WLt"></a>
### 三套生命周期
Maven 的生命周期并不是一个整体，Maven 拥有三套相互独立的生命周期，它们分别为 clean、default 和 site。

- `clean` 生命周期的目的是清理项目；
- `default` 生命周期的目的是构建项目；
- `site` 生命周期的目的是建立项目站点；
<a name="qDpNG"></a>
### 单个生命周期执行顺序
每个生命周期包含一些阶段(phase)，这些阶段是有顺序的，并且后面的阶段依赖于前面的阶段。<br />以 clean 生命周期为例，它包含的阶段有 pre-clean、clean和post-clean。当调用 pre-clean 时，只有 pre-clean 阶段得以执行；<br />当调用 clean 的时候，pre-clean和clean阶段会得以顺序执行，以此类推。
<a name="Ey8Cd"></a>
### 各个生命周期之间的关系
三套生命周期本身是相互独立的，用户可以仅调用 clean 生命周期的某个阶段，或者仅仅调用 default 生命周期的某个阶段，而不会对其他生命周期产生任何影响。<br />例如，当用户调用 clean 生命周期的 clean 阶段的时候，不会触发 default 生命周期的任何阶段，反之亦然。
<a name="GxvNj"></a>
### 生命周期各个阶段详解
<a name="1Lqer"></a>
#### clean
| 生命周期阶段 | 描述 |
| --- | --- |
| pre-clean | 执行一些清理前需要完成的工作。 |
| clean | 清理上一次构建生成的文件。 |
| post-clean | 执行一些清理后需要完成的工作。 |

<a name="lH1tX"></a>
#### default
包含 23 个阶段，此处只介绍重点步骤，如下表：

| 生命周期阶段 | 描述 |
| --- | --- |
| validate | 检查工程配置是否正确，完成构建过程的所有必要信息是否能够获取到。 |
| initialize | 初始化构建状态，例如设置属性。 |
| generate-sources |  |
| process-sources | 处理项目资源文件，处理项目主资源文件。一般来说，是对src/main/resources目录的内容进行变量替换等工作后，复制到项目输出的主classpath目录中。 |
| generate-resources |  |
| process-resources |  |
| compile | 编译项目的主源码。一般来说，是编译src/main/java目录下的Java文件至项目输出的主classpath目录中。 |
| process-classes | 处理编译生成的文件，例如 Java Class 字节码的加强和优化。 |
| generate-test-sources |  |
| process-test-sources | 处理项目测试资源文件。一般来说，是对src/test/resources目录的内容进行变量替换等工作后，复制到项目输出的测试classpath目录中。 |
| test-compile | 编译项目的测试代码。一般来说，是编译src/test/java目录下的Java文件至项目输出的测试classpath目录中。 |
| process-test-classes |  |
| test | 使用适当的单元测试框架（例如JUnit）运行测试。 |
| prepare-package | 在真正打包之前，为准备打包执行任何必要的操作。 |
| package | 获取编译后的代码，并按照可发布的格式进行打包，例如 JAR、WAR 或者 EAR 文件。 |
| pre-integration-test | 在集成测试执行之前，执行所需的操作。例如，设置所需的环境变量。 |
| integration-test | 处理和部署必须的工程包到集成测试能够运行的环境中。 |
| post-integration-test | 在集成测试被执行后执行必要的操作。例如，清理环境。 |
| verify | 运行检查操作来验证工程包是有效的，并满足质量要求。 |
| install | 安装工程包到本地仓库中，该仓库可以作为本地其他工程的依赖。 |
| deploy | 拷贝最终的工程包到远程仓库中，以共享给其他开发人员和工程。 |

<a name="WA5bC"></a>
#### site
| 生命周期阶段 | 描述 |
| --- | --- |
| pre-site | 执行一些在生成项目站点之前需要完成的工作。 |
| site | 生成项目站点文档。 |
| post-site | 执行一些在生成项目站点之后需要完成的工作。 |
| site-deploy | 将生成的项目站点发布到服务器上。 |

<a name="7IXvq"></a>
## 插件
Maven 三套生命周期定义各个阶段不做任何实际工作，实际工作都是由插件来完成的，每个生命周期阶段都是由插件的目标来完成。在 pom 文件中声明如下（打包源码文件插件）：
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-source-plugin</artifactId>
      <version>2.1.1</version>
      <executions>
        <execution>
          <id>attach-sources</id>
          <phase>verify</phase>
          <goals>
              <goal>jar-no-fork</goal>
          </goals>
        </execution>
      </executions>
    </plugin>
  </plugins>
</build>
```
<a name="vh6ax"></a>
### 插件目标
一个插件有可能有多个功能、每个功能就是一个目标。比如 maven-dependency-plugin 有十多个目标，每个目标对应了一个功能。<br />插件的目标为 dependency:analyze、dependency:tree和dependency:list。<br />通用写法：冒号前面是插件前缀，冒号后面是插件的目标。比如 compiler:compile。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605538476816-f79025e7-3b74-4060-9668-25b2ba26d0fa.png#averageHue=%23f8f7f6&height=322&id=cQJ8O&originHeight=966&originWidth=2842&originalType=binary&ratio=1&rotation=0&showTitle=false&size=332184&status=done&style=shadow&title=&width=947.3333333333334)
<a name="DfxxE"></a>
### 插件绑定
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1605537091806-8b65f628-afbd-45e3-8c7e-d8af628c81de.png#averageHue=%23f7f7f7&height=333&id=m9h6F&originHeight=333&originWidth=696&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=696)
<a name="4a3oH"></a>
### 内置绑定
为实现快速构建，Maven 有一套内置的插件绑定。三套生命周期的插件绑定具体如下（其实是各个生命周期阶段与插件的目标的绑定）。<br />其中 default 生命周期的构建方式会其打包类型有关、打包类型在POM中 packaging 指定。一般有 jar、war 两种类型。下面是默认绑定插件与生命周期关系图：

|  | **阶段** | **插件名** |
| --- | --- | --- |
| **clean** | **pre-clean** |  |
|  | **clean** | **maven-clean-plugin:clean** |
|  | **post-clean** |  |
| **site** | **pre-site** |  |
|  | **site** | **maven-site-plugin:site** |
|  | **post-site** |  |
|  | **site-deploy** | **maven-site-plugin:deploy** |
| **default** | **process-resources** | **maven-resources-plugin:resources** |
|  | **compile** | **maven-compiler-plugin:compile** |
|  | **process-test-resources** | **maven-resources-plugin:testResources** |
|  | **test-compile** | **maven-compiler-plugin:testCompile** |
|  | **test** | **maven-surefire-plugin:test** |
|  | **package** | **maven-jar-plugin:jar** |
|  | **install** | **maven-install-plugin:install** |
|  | **deploy** | **maven-deploy-plugin:deploy** |

<a name="Fhy5W"></a>
### 自定义绑定
自定义绑定允许自己掌控插件目标与生命周期的结合。以生成项目主代码的源码 jar 为例。<br />使用到的插件和它的目标为：maven-source-plugin:jar-no-fork。将其绑定到 default 生命周期阶段 verify 上（可以任意指定三套生命周期的任意阶段）。
```xml
<build>
  <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-source-plugin</artifactId>
        <version>2.1.1</version>
        <executions>
          <execution>
            <id>attach-sources</id> 
            <!-- 指定作用在生命周期的哪个阶段 -->
            <phase>verify</phase> 
            <goals>
               <!-- 指定执行绑定插件的哪些目标 -->
                <goal>jar-no-fork</goal>
            </goals>
          </execution>
        </executions>
      </plugin>
  </plugins>
</build>
```
<a name="G0TgT"></a>
### 插件配置

- 使用命令行配置<br />在 Maven 命令中加入 -D 参数，并伴随一个参数键=参数值的形式，来配置插件目标参数。<br />如：maven-surefire-plugin 插件提供一个 `maven.test.skip` 参数，当值为 true 时会跳过执行测试：
```bash
-- 对比 mvn install
mvn install –Dmaven.test.skip=true
```

- 使用 pom 全局配置<br />在声明插件的时候，对插件进行一个全局配置，后面所有使用该插件的都要遵循这个配置。比如指定 maven-compile-plugin 编译 1.7 版本的源文件：
```xml
<plugin>
   <groupId>org.apache.maven.plugins</groupId>
   <artifactId>maven-compiler-plugin</artifactId>
   <configuration>
       <fork>true</fork>
       <source>1.7</source>
       <target>1.7</target>
   </configuration>
</plugin>
```
<a name="G1TuK"></a>
## 聚合与继承
**聚合**：为了一次构建多个项目模块，就需要对多个项目模块进行聚合
```xml
<modules>
    <module>模块一</module>
    <module>模块二</module>
    <module>模块三</module>
</modules>
```
**继承**：为了消除重复，把很多相同的配置提取出来，例如：dependency、grouptId，version 等
```xml
<parent>  
    <groupId>com.xxxx.maven</groupId>
    <artifactId>parent-project</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <relativePath>../ParentProject/pom.xml</relativePath>  
</parent>
```
以下的元素是可以被继承的：

- groupId，项目组ID；
- version，项目版本；
- description，项目描述信息；
- organazation，项目的组织信息；
- inceptionYear，项目的创始年份；
- developers，项目开发者信息；
- contributors，项目的贡献者信息；
- distributionManagement，项目的部署信息；
- issueManagement，项目的缺陷跟踪系统信息；
- ciManagement，项目的持续集成系统信息；
- scm，项目的版本控制系统信息；
- mailingLists，项目的邮件列表信息；
- properties，自定义的Maven属性；
- dependencies，项目的依赖配置；
- dependencyManagement，项目的依赖管理配置；
- repositories，项目的仓库配置；
- build，包括项目的源码目录配置、输出目录配置、插件配置、插件管理配置等；
- reporting，包括项目的报告输出目录配置、报告插件配置。

注意下面的元素，这些都是**不能被继承**的：

- artifactId
- name
- prerequisites
<a name="BVVPP"></a>
### 聚合与继承之间的关系

- 两者共同点为，打方式必须都是 pom
- 在实际的项目中，一个 pom 既是聚合 pom 又是父 pom

`注`：父 pom 中使用 dependencies 引入的依赖也会被子 pom 继承，所以不要将过多的实际依赖放在父 pom，父 pom 只用于管理，使用 dependencyManagement 标签。
<a name="QCyhG"></a>
## 灵活构建
使用属性、 resources 插件资源过滤功能（filter）和 Maven 的 profile 功能，实现环境的灵活切换
<a name="wuhaX"></a>
### 属性
通过 properties 元素用户可以自定义一个或者多个 Maven 属性，然后在 pom 其他的地方使用 ${属性名} 的方式引用该属性，这种方式最大意义在于消除重复。
<a name="XDLzM"></a>
#### 一、内置属性

- `${basedir}` 表示项目根目录，即包含 pom.xml 文件的目录
- `${version}` 等同于 或者{pom.version} 表示项目版本
<a name="HWlJZ"></a>
#### 二、POM 属性
所有 pom 中的元素都可以用 project。例如 ${project.artifactId} 对应了 `<project>`元素的值。常用的 POM 属性包括：

- `${project.build.sourceDirectory}` : 项目的主源码目录，默认为 src/main/java/。
- `${project.build.testSourceDirectory}` : 项目的测试源码目录，默认为 /src/test/java/。
- `${project.build.directory}` : 项目构建输出目录，默认为 target/。
- `${project.build.outputDirectory}` : 项目主代码编译输出目录，默认为 target/classes/。
- `${project.build.testOutputDirectory}` : 项目测试代码编译输出目录，默认为 target/testclasses/。
- `${project.groupId}`: 项目的 groupId。
- `${project.artifactId}` : 项目的 artifactId。
- `${project.version}` : 项目的 version，等同于 `${version}`
- `${project.build.finalName}` : 项目打包输出文件的名称，默认为 `${project.artifactId}${project.version}`
<a name="7uIlr"></a>
#### 三、自定义属性
在 pom 中元素下自定义的 Maven 属性
```xml
<properties>
    <swagger.version>2.2.2</swagger.version>
</properties>
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger2</artifactId>
    <version>${swagger.version}</version>
</dependency>
```
<a name="DLW0G"></a>
#### 四、Settings 属性
所有用的的 settings.xml 中的设定都可以通过 settings。前缀进行引用与 POM 属性同理。<br />如 `${settings.localRepository}` 指向用户本地仓库的地址
<a name="zz81D"></a>
#### 五、Java 系统属性
所有 Java 系统属性都可以使用 Maven 属性引用，例如 `${user.home}` 指向了用户目录。<br />可以通过命令行 mvn help:system 查看所有的 Java 系统属性
<a name="0ig5p"></a>
#### 六、环境变量属性
所有环境变量都可以使用以 env. 开头的 Maven 属性引用。例如 `${env.JAVA_HOME}` 指代了 JAVA_HOME 环境变量的值。<br />也可以通过命令行 mvn help:system 查看所有环境变量。
<a name="WbcGV"></a>
#### 七、父级工程属性
上级工程的 pom 中的变量用前缀 引用。上级工程的版本也可以这样引用`{parent.version}`
<a name="4LkFc"></a>
### Profile
profile 特性可以定义多个 profile，然后每个 profile 对应不同的激活条件和配置信息，从而达到不同环境使用不同配置信息的效果。<br />profile 可以在以下几个地方声明：

- m.xml：这里声明的 profile 只对当前项目有效
- 用户 settings.xml：.m2/settings.xml 中的 profile 对该用户的 Maven 项目有效
- 全局 settings.xml：conf/settings.xml，对本机上所有 Maven 项目有效

示例：
```xml
<project>
  ...
  <profiles>
    <profile>
      <id>dev</id>
      <properties>
        <active.profile>dev</active.profile>
        <key1>value1</key1>
        <key2>value2</key2>
      </properties>
      <!-- 默认激活配置 -->
      <activation>
        <activeByDefault>true</activeByDefault>
      </activation>
      <!-- 在该 profile 下才会引入的依赖 -->
      <dependencies>
        <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>3.2.4.RELEASE</version>
        </dependency>
      <dependencies>
      <!-- 在该 profile 下才会加载的变量文件 -->
      <build>
        <filters>
          <filter>../profile/test-pre.properties</filter>
        </filters>
      </build>
    </profile>
  </profiles>
  ...
</project>
```
