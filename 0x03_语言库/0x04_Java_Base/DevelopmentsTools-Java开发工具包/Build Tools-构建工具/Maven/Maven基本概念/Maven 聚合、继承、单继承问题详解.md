Maven
<a name="uCzWz"></a>
## 聚合
<a name="GPTW0"></a>
### 原始需求
需要使用Java做一个电商网站，涉及到：pc端网站、h5微站、移动端接口部分，那么可以使用Maven创建3个项目用于这3块业务的开发，3个项目名称如下：
```
javacode2018-pc
javacode2018-h5
javacode2018-api
```
这3个项目的groupId都是com.javacode2018，artifactId取上面的，使用Maven来搭建项目结构。
<a name="qqmDf"></a>
### 实现
<a name="IoooL"></a>
#### 创建第一个javacode2018-pc项目
打开idea，点击File->New->Project，选择Maven，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209744044-e5f60b69-b496-49ab-a8e2-6ffbfd1a741e.jpeg#averageHue=%23e5e1e0&clientId=uac2b062c-923f-4&from=paste&id=ud1cbb86d&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue5dc3889-c407-48c4-920e-ef26649be5b&title=)<br />点击Next，输入项目坐标信息，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209743950-8b6123db-56cc-4f4e-b470-5334c166e25e.png#averageHue=%23e9e7e6&clientId=uac2b062c-923f-4&from=paste&id=ua231c409&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua657e476-79fa-4145-96aa-1a28df0d78a&title=)<br />点击Next，输入Project name 为javacode2018-pc，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209743924-386f69cf-72ba-4399-a8af-a6e959ced9c0.png#averageHue=%23e8e6e6&clientId=uac2b062c-923f-4&from=paste&id=ua3036215&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubcc3264d-122b-4310-b1e6-1d38347b2cd&title=)<br />点击Finish，创建成功，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209743855-5d88fc69-51ab-4529-94a2-75ad303e34d9.jpeg#averageHue=%23ebe9de&clientId=uac2b062c-923f-4&from=paste&id=ue4c22e5a&originHeight=767&originWidth=1033&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u88f334c9-3e3d-4ddd-9582-8dc87364758&title=)<br />配置一下idea的Maven环境，点击File->Settings，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209743814-9f4b59c7-58c4-4c9f-a3fb-f59d78a2cd3e.png#averageHue=%23c7a169&clientId=uac2b062c-923f-4&from=paste&id=u45d5d948&originHeight=694&originWidth=1053&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u85400030-cf46-4529-b2fc-df71576bdab&title=)<br />点击上面的OK完成配置。<br />大家再按照上面一样的操作，分别创建另外2个项目javacode2018-h5和javacode2018-api，创建完成之后，另外2个项目如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209745272-610576ab-2214-48df-a7d8-111a02338b3e.jpeg#averageHue=%23f3f3f2&clientId=uac2b062c-923f-4&from=paste&id=u68f5b152&originHeight=776&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u98d6ee4c-3ff6-49b0-99d7-a79e79e5d64&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209745184-2223ddbe-f5ad-4d4c-b545-f33064052226.jpeg#averageHue=%23f3f1f0&clientId=uac2b062c-923f-4&from=paste&id=u017a927c&originHeight=774&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udbe1e260-bb79-49b8-84a0-ce436ce1cab&title=)<br />上面3个项目由整个项目组6个人共同开发，这几个项目经常一起上线，上线过程，先由开发进行打包，然后将几个包发给运行进行发布。使用`mvn package`进行打包，需要在每个项目的pom.xml所在目录都去执行一次这个命令，也就是说需要执行3次，这个电商项目还会涉及到后台系统、bi系统、监控系统等等，可能最后会多达10个小项目，那时候每次上线都需要执行10次打包操作，这个过程是相当繁琐的。<br />那么Maven有没有更好的办法来解决这个事情呢？<br />这个用到的就是本次要说的Maven中的聚合。<br />整个电商可以作为一个大的系统，上面的pc端、h5微站、api接口、后台系统、bi系统、监控系统都可以作为里面的一个具体比较大一个模块。<br />使用Maven聚合功能来实现上面的需求，需要创建一个额外的Maven项目javacode-aggregator来管理上面3个项目，然后只用在javacode-aggregator项目中执行mvn命令，就会自动为其他3个项目自动执行同样的mvn命令。
<a name="WO3GI"></a>
### Maven聚合
Maven聚合需要创建一个新的Maven项目， 用来管理其他的Maven构件模块，新的Maven项目中加入如下配置：
```xml
<modules>
  <module>模块1</module>
  <module>模块2</module>
  <module>模块n</module>
</modules>
<package>pom</package>
```
新的项目中执行任何mvn命令，都会modules中包含的所有模块执行同样的命令，而被包含的模块不需要做任何特殊的配置，正常的maven项目就行。<br />注意上面的module元素，这部分是被聚合的模块pom.xml所在目录的相对路径。<br />package的值必须为pom，这个需要注意。<br />下面看案例。
<a name="fV9EB"></a>
### 实操案例
下面过程仔细看了。
<a name="jjrRq"></a>
#### 创建项目javacode-aggregator
打开idea，点击File->New->Project，选择Maven，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209745324-13296e0e-2cf3-4dc9-b3b0-84d5046041a5.jpeg#averageHue=%23e5e2e1&clientId=uac2b062c-923f-4&from=paste&id=u405d67bf&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub5d3a663-7261-49f3-9ab7-bdfbc522a37&title=)<br />点击Next，输入项目坐标信息，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209745333-21f5303b-259c-46aa-b551-84dbba65f2f0.png#averageHue=%23e8e7e6&clientId=uac2b062c-923f-4&from=paste&id=u7f626467&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubd763b7c-b0e0-47fd-ae86-20e8b8aa584&title=)<br />点击Next，输入Project name 为javacode-aggregator，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209745314-0bc8c9cf-f2ac-413d-a444-aea610714dd9.png#averageHue=%23e8e6e6&clientId=uac2b062c-923f-4&from=paste&id=uf1c1a9d0&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u65129daf-08de-4321-a382-d11d35eb61d&title=)<br />点击Finish，创建成功，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209746801-20fbaea2-a700-482e-9485-2a3634079db8.jpeg#averageHue=%23f4f3f2&clientId=uac2b062c-923f-4&from=paste&id=ua8013c35&originHeight=788&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucbd95735-92e6-4f02-a923-adff1710ab9&title=)<br />配置一下idea的Maven环境，点击File->Settings，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209746718-4f969624-fcfe-48b6-a391-3ccbb919cf96.png#averageHue=%23bbaa91&clientId=uac2b062c-923f-4&from=paste&id=u888014b6&originHeight=641&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5e4438ea-da50-4d67-82cf-2ad5e57d4e9&title=)<br />删除下面无用的代码：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209746713-77f8f477-3720-41b1-b189-eefdbd321f89.png#averageHue=%23dba866&clientId=uac2b062c-923f-4&from=paste&id=u504464fd&originHeight=331&originWidth=364&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udaa34cf9-44e6-41a2-adb0-bfdec7fbd61&title=)<br />项目变成了下面这样：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209746659-b9a7ae55-721b-4613-a094-4e548521c195.png#averageHue=%23d2a463&clientId=uac2b062c-923f-4&from=paste&id=ue15a8936&originHeight=232&originWidth=350&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1878ef79-6982-450a-a10c-86f27c904a1&title=)<br />pom.xml中加入下面配置：
```xml
<packaging>pom</packaging>
```
最后pom.xml内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>javacode-aggregator</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>pom</packaging>

</project>
```
<a name="dD5xn"></a>
#### 创建javacode2018-pc 模块
**注意这里说的是模块，不再是创建项目，过程仔细看。**<br />idea中选中javacode-aggregator<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209746755-22f4de4b-af00-4c09-b39c-e693f615c787.png#averageHue=%23f3eeed&clientId=uac2b062c-923f-4&from=paste&id=uf95109f9&originHeight=388&originWidth=537&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u58387b66-3e86-4708-979e-084e254e0dc&title=)<br />点击File->New->Module，准备创建模块，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209748284-4b7a3385-339b-4d39-96b7-d5a68b4efac3.png#averageHue=%23e3dfdd&clientId=uac2b062c-923f-4&from=paste&id=uafa37996&originHeight=512&originWidth=677&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u61482e2a-31a0-4109-a102-b15bc69f187&title=)<br />选择Maven，如图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209748689-43ea8870-6a6f-485c-a450-4cfaf12be1fc.jpeg#averageHue=%23dad0c8&clientId=uac2b062c-923f-4&from=paste&id=u1e4c4f7f&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc463a0e9-4eb5-4a1e-b36e-7a620855bfe&title=)<br />点击Next，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209748857-e9f27250-3b58-476b-a19d-77efa54ea3cd.png#averageHue=%23e6e6e6&clientId=uac2b062c-923f-4&from=paste&id=ue0a8b908&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0edafe33-4f10-4ffb-af8b-f3e3feed710&title=)<br />点击下面的...按钮，选择None，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209748640-3001ea70-2e38-4e79-84e3-04aa544d584b.png#averageHue=%23e7e6e6&clientId=uac2b062c-923f-4&from=paste&id=u41643e93&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u47517a4b-cc1d-4735-8220-81fa4b21349&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209748597-b3139603-77fd-4bd0-a550-d131084dc7c3.png#averageHue=%23edebea&clientId=uac2b062c-923f-4&from=paste&id=ub5064d02&originHeight=274&originWidth=440&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u86f09865-96d3-41ca-94b1-9a4a44a8c66&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209749385-7043130c-fbd1-4047-a8f3-d37d58d8cfbc.png#averageHue=%23e8e8e7&clientId=uac2b062c-923f-4&from=paste&id=u38db4eeb&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubf2f4b95-dbd8-4695-a949-fbdf68602de&title=)<br />输入坐标信息，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209749401-a635ca2a-ef2c-4e70-aa6f-3a089a63702d.png#averageHue=%23e8e6e5&clientId=uac2b062c-923f-4&from=paste&id=ua7073635&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u165629e0-6537-420b-9e02-5216bd7025b&title=)<br />点击Next，输入Module name：javacode2018-pc，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209749604-6e2c8ff3-0e21-42a0-8010-8705539bda19.png#averageHue=%23e8e6e5&clientId=uac2b062c-923f-4&from=paste&id=u7587f3f7&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u597f02a9-6e6d-40be-a509-9453fd2fff0&title=)<br />点击Finish，完成javacode-2018模块的创建，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209749959-538f7e0b-9988-42f0-a3fd-f30d8413211f.png#averageHue=%23f5f4f2&clientId=uac2b062c-923f-4&from=paste&id=ua0e99b96&originHeight=450&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud1246efa-e3d7-40ae-acb4-1bec373d48f&title=)<br />来看一下javacode-aggregator的pom.xml，多了一部分，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209749963-6fa5d8b6-580b-4baa-ade6-c446f2a8a1bf.jpeg#averageHue=%23f3f2f1&clientId=uac2b062c-923f-4&from=paste&id=ud30d2cec&originHeight=788&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7794e2d9-a832-4ff8-9b20-d78600bc4fd&title=)<br />注意上面的`<packaging>pom</packaging>`
<a name="OanZI"></a>
#### 创建javacode2018-h5模块
创建步骤和上面javacode2018-pc步骤一样。
<a name="sEpXE"></a>
#### 创建javacode2018-api模块
创建步骤和上面javacode2018-pc步骤一样。<br />看一下项目最终结构，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209751397-d6d2305d-2814-454c-9304-660ea449c020.jpeg#averageHue=%23eae7dc&clientId=uac2b062c-923f-4&from=paste&id=ud1ff7751&originHeight=599&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1253e89a-62c6-4dbe-bf32-13438138786&title=)<br />看一下上图中的javacode-aggregator项目的pom.xml，modules元素中多出了3部分。<br />再来看看其他3个模块的pom文件，和普通的pom.xml一样，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209751538-de83b869-9808-442a-97b9-d9050b209a71.png#averageHue=%23f2efec&clientId=uac2b062c-923f-4&from=paste&id=u41f62a6e&originHeight=390&originWidth=629&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua26302a5-2192-4135-8095-757edaae713&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209751636-a3d6540a-5a8d-41e7-831f-3014647f2bb2.png#averageHue=%23f4f1ef&clientId=uac2b062c-923f-4&from=paste&id=u2051b931&originHeight=412&originWidth=714&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u447e8f87-2e86-428a-b2d9-9e2fb451503&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209751885-0a439a75-b5bd-4599-b8a9-7d46b8e033d3.png#averageHue=%23f3f0ed&clientId=uac2b062c-923f-4&from=paste&id=ua0f413c0&originHeight=421&originWidth=635&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uae3772a8-ec9a-4f0d-84ba-02d90e4fd01&title=)<br />在javacode-aggregator/pom.xml目录中执行`mvn package`感受一下效果：
```
D:\code\IdeaProjects\javacode-aggregator>mvn package
[INFO] Scanning for projects...
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Build Order:
[INFO]
[INFO] javacode2018-pc                                                    [jar]
[INFO] javacode2018-h5                                                    [jar]
[INFO] javacode2018-api                                                   [jar]
[INFO] javacode-aggregator                                                [pom]
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-pc >------------------
[INFO] Building javacode2018-pc 1.0-SNAPSHOT                              [1/4]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode-aggregator\javacode2018-pc\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-pc ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-pc ---
[INFO] Building jar: D:\code\IdeaProjects\javacode-aggregator\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT.jar
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-h5 >------------------
[INFO] Building javacode2018-h5 1.0-SNAPSHOT                              [2/4]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-h5 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-h5 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-h5 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode-aggregator\javacode2018-h5\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-h5 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-h5 ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-h5 ---
[INFO] Building jar: D:\code\IdeaProjects\javacode-aggregator\javacode2018-h5\target\javacode2018-h5-1.0-SNAPSHOT.jar
[INFO]
[INFO] -----------------< com.javacode2018:javacode2018-api >------------------
[INFO] Building javacode2018-api 1.0-SNAPSHOT                             [3/4]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode-aggregator\javacode2018-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-api ---
[INFO] Building jar: D:\code\IdeaProjects\javacode-aggregator\javacode2018-api\target\javacode2018-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] ----------------< com.javacode2018:javacode-aggregator >----------------
[INFO] Building javacode-aggregator 1.0-SNAPSHOT                          [4/4]
[INFO] --------------------------------[ pom ]---------------------------------
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary for javacode-aggregator 1.0-SNAPSHOT:
[INFO]
[INFO] javacode2018-pc .................................... SUCCESS [  2.542 s]
[INFO] javacode2018-h5 .................................... SUCCESS [  0.195 s]
[INFO] javacode2018-api ................................... SUCCESS [  0.218 s]
[INFO] javacode-aggregator ................................ SUCCESS [  0.024 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  3.223 s
[INFO] Finished at: 2019-11-19T13:28:00+08:00
[INFO] ------------------------------------------------------------------------
```
分析一下上面的输出：
```
[INFO] Reactor Build Order:
[INFO]
[INFO] javacode2018-pc                                                    [jar]
[INFO] javacode2018-h5                                                    [jar]
[INFO] javacode2018-api                                                   [jar]
[INFO] javacode-aggregator                                                [pom]
```
上面这个列出了需要构件的Maven构件列表及顺序，共有4个构件，3个jar包，1个pom类型的构件。<br />然后开始按照上面列出的顺序，一个个开始执行`mvn package`命令，最后4个都执行成功了，来看一下最终产生的效果，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209751909-79bb8816-9648-4f21-b4e8-1487bb1b47ad.png#averageHue=%23faf4e7&clientId=uac2b062c-923f-4&from=paste&id=u880b021a&originHeight=851&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uecee64b0-e44d-4b01-86ce-8827b3cc364&title=)<br />从上图中可以看到3个模块都生成了jar包，说明执行了一次`mvn package`，分别在3个模块中都运行了，在来执行一下`mvn clean`清理代码，感受一下最终效果：
```
D:\code\IdeaProjects\javacode-aggregator>mvn clean
[INFO] Scanning for projects...
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Build Order:
[INFO]
[INFO] javacode2018-pc                                                    [jar]
[INFO] javacode2018-h5                                                    [jar]
[INFO] javacode2018-api                                                   [jar]
[INFO] javacode-aggregator                                                [pom]
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-pc >------------------
[INFO] Building javacode2018-pc 1.0-SNAPSHOT                              [1/4]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ javacode2018-pc ---
[INFO] Deleting D:\code\IdeaProjects\javacode-aggregator\javacode2018-pc\target
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-h5 >------------------
[INFO] Building javacode2018-h5 1.0-SNAPSHOT                              [2/4]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ javacode2018-h5 ---
[INFO] Deleting D:\code\IdeaProjects\javacode-aggregator\javacode2018-h5\target
[INFO]
[INFO] -----------------< com.javacode2018:javacode2018-api >------------------
[INFO] Building javacode2018-api 1.0-SNAPSHOT                             [3/4]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ javacode2018-api ---
[INFO] Deleting D:\code\IdeaProjects\javacode-aggregator\javacode2018-api\target
[INFO]
[INFO] ----------------< com.javacode2018:javacode-aggregator >----------------
[INFO] Building javacode-aggregator 1.0-SNAPSHOT                          [4/4]
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ javacode-aggregator ---
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary for javacode-aggregator 1.0-SNAPSHOT:
[INFO]
[INFO] javacode2018-pc .................................... SUCCESS [  0.250 s]
[INFO] javacode2018-h5 .................................... SUCCESS [  0.067 s]
[INFO] javacode2018-api ................................... SUCCESS [  0.068 s]
[INFO] javacode-aggregator ................................ SUCCESS [  0.034 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.605 s
[INFO] Finished at: 2019-11-19T13:34:42+08:00
[INFO] ------------------------------------------------------------------------
```
项目结构如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209753639-6c44d78e-8c9e-444e-af75-98576d6dbf0a.png#averageHue=%23faf9f8&clientId=uac2b062c-923f-4&from=paste&id=uea9da4aa&originHeight=488&originWidth=642&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u85a96fd0-780a-4973-8f6c-e6c3129ffd2&title=)<br />3个项目中的target都被清理掉了。<br />**上面介绍了pom.xml中的module元素的值为被聚合的模块pom.xml所在的目录路径，可以是相对路径，也可以是绝对路径，上面演示的是相对路径，可以自己去试一下绝对路径的情况。**<br />聚合的功能中，聚合模块的pom.xml中通过modules->module来引用被聚合的模块，被聚合的模块是不用感知自己被聚合了，所以被聚合的模块中pom.xml中是不知道javacode-aggregator的存在的。
<a name="PGYVM"></a>
## 继承
<a name="mXYVp"></a>
### 需求
细心的朋友已经发现了javacode2018-pc、javacode2018-h5、javacode2018-api3个项目的groupId、version都是一样的。还有这几个项目准备都是用springmvc、mybatis开发的，所以需要引入依赖如下：
```xml
<dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-web</artifactId>
  <version>5.2.1.RELEASE</version>
</dependency>
<dependency>
  <groupId>org.mybatis</groupId>
  <artifactId>mybatis-spring</artifactId>
  <version>2.0.3</version>
</dependency>
```
3个项目中出现了同样的配置，一个配置出现了多份，作为开发者，是无法接受的，必须要寻求一些方法来解决，需要将公共的部分提取出来共用。<br />Maven也考虑到了这种情况，Maven中使用继承来解决这个问题。
<a name="hllFH"></a>
### 继承的实现
操作分为3步骤：

1. 创建一个父Maven构件，将依赖信息放在pom.xml中
```xml
<dependencies>
  <dependency>依赖的构件的坐标信息</dependency>
  <dependency>依赖的构件的坐标信息</dependency>
  <dependency>依赖的构件的坐标信息</dependency>
</dependencies>
```

2. 将父构件的package元素的值置为pom
```xml
<packaging>pom</packaging>
```

3. 在子构件的pom.xml引入父构件的配置：
```xml
<parent>
  <groupId>父构件groupId</groupId>
  <artifactId>父构件artifactId</artifactId>
  <version>父构件的版本号</version>
  <relativePath>父构件pom.xml路径</relativePath>
</parent>
```
relativePath表示父构件pom.xml相对路径，默认是../pom.xml，所以一般情况下父子结构的Maven构件在目录结构上一般也采用父子关系。
<a name="Dp06x"></a>
### 实操案例
先创建一个父项目javacode-parent，坐标信息如下：
```xml
<groupId>com.javacode2018</groupId>
<artifactId>javacode2018-parent</artifactId>
<version>1.0-SNAPSHOT</version>
```
创建过程大家已经轻车熟路了，就不演示了，最终如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209753998-d922275a-df1a-4fcc-8442-c4bf04d49920.jpeg#averageHue=%23e6e4d9&clientId=uac2b062c-923f-4&from=paste&id=ua4fb82db&originHeight=553&originWidth=1031&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1c1ebc3a-33d5-473c-bdba-dd80d1fbd9f&title=)<br />删除无用的2目录.idea和src<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209754041-13235a45-2aa8-4c6e-97a6-8f119f65eb93.png#averageHue=%23ecebe9&clientId=uac2b062c-923f-4&from=paste&id=u9fe4998f&originHeight=250&originWidth=303&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0528ac1f-a825-4181-83c5-be8f0c32e6f&title=)
<a name="K4i78"></a>
#### 创建子模块javacode2018-pc 模块
**注意这里说的是模块了，不在是创建项目了，过程仔细看。**<br />idea中选中javacode-parent，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209754081-ee996b92-0c16-463f-8622-f392d8a8ef64.png#averageHue=%23f9f3f2&clientId=uac2b062c-923f-4&from=paste&id=u2a2755b0&originHeight=410&originWidth=493&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufe74954a-54c8-4665-ae0d-6020909c911&title=)<br />点击File->New->Module，准备创建模块，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209754006-f8858734-8a94-4d12-a400-9c7a1903f965.png#averageHue=%23e3dbd8&clientId=uac2b062c-923f-4&from=paste&id=u19a502f1&originHeight=281&originWidth=676&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2f33b33c-db6e-4ae6-9b8f-9eecc174610&title=)<br />选中Maven，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209755194-d0f08b15-3825-470c-99e2-11125243f9cb.jpeg#averageHue=%23dacbc3&clientId=uac2b062c-923f-4&from=paste&id=ub1ecbb47&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9df23576-5831-458c-8084-650214d4141&title=)<br />点击Next，这个页面需要注意了，默认如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209755203-76211a3e-346f-4046-8275-b91be8365356.png#averageHue=%23e6e6e6&clientId=uac2b062c-923f-4&from=paste&id=ud82c9463&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub3545efc-e50f-4bd0-adde-4626dc1755d&title=)<br />此时点击第一个...按钮，这个按钮是设置聚合的，所以选中None，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209756033-17d64b06-a1e8-4f4a-b4d1-41b461f009cc.png#averageHue=%23e6e6e5&clientId=uac2b062c-923f-4&from=paste&id=u3d459449&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u48776556-c1ed-409f-b567-7aa8b7180df&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209756017-9b05ae42-72b4-4ab8-acc6-2a1413df5ebe.png#averageHue=%23eeecec&clientId=uac2b062c-923f-4&from=paste&id=u16a74a5f&originHeight=293&originWidth=440&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud88f5f9c-598b-498c-a318-9e066eca5c9&title=)<br />变成了下面这样：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209756041-0b280e7c-5809-418f-9d5f-ed14c2142ab9.png#averageHue=%23e6e6e6&clientId=uac2b062c-923f-4&from=paste&id=u490e5823&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3a2028c6-c939-425f-85b3-12eea3da015&title=)<br />输入构件信息，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209757036-3e7e80b2-87d1-4d6b-aaab-ca19f16830f6.png#averageHue=%23e7e5e4&clientId=uac2b062c-923f-4&from=paste&id=u52b04563&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u61b85c17-b5e6-4fec-abab-9eff07c0e3e&title=)<br />点击Next，输入Module-name的值为javacode2018-pc，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209756933-81f456f6-5a62-46d8-a83f-98cb8720ee51.png#averageHue=%23e8e5e5&clientId=uac2b062c-923f-4&from=paste&id=u83de8c7a&originHeight=700&originWidth=995&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3340db8b-0cc9-4988-a78b-8eaa787f24d&title=)<br />点击Finish完成创建操作，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209757429-00336df6-c16f-404e-b95c-fd23e8cd88fb.png#averageHue=%23f7f5f3&clientId=uac2b062c-923f-4&from=paste&id=u87cc5487&originHeight=507&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ube4adfed-7825-4523-9cd7-265a5ef3105&title=)<br />上图中，看一下javacode2018-pc的pom.xml，多了个parent元素，并且这个pom.xml中构件的groupId、version都没有了，这个pom.xml继承了javacode2018-parent/pom.xml中的内容，他们的groupId、version都是一样的，子构件可以从父pom.xml中继承这些内容，所以如果是一样的情况，可以不写。
<a name="gBH33"></a>
#### 创建另外两个子模块javacode2018-h5、javacode2018-api
步骤参考javacode2018-pc模块的创建过程。<br />最终项目如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209757393-4500668b-22d3-4df6-a2a5-d03b3520d1df.png#averageHue=%23f6f5f4&clientId=uac2b062c-923f-4&from=paste&id=u812514aa&originHeight=467&originWidth=496&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3b5da5b1-561e-4f92-af5c-fb480cbcb17&title=)<br />下面来看一下4个pom.xml的内容。<br />javacode2018-parent/pom.xml的内容：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>javacode2018-parent</artifactId>
  <version>1.0-SNAPSHOT</version>

</project>
```
javacode2018-pc/pom.xml的内容：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <parent>
    <artifactId>javacode2018-parent</artifactId>
    <groupId>com.javacode2018</groupId>
    <version>1.0-SNAPSHOT</version>
  </parent>
  <modelVersion>4.0.0</modelVersion>

  <artifactId>javacode2018-pc</artifactId>

</project>
```
javacode2018-h5/pom.xml的内容：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <parent>
    <artifactId>javacode2018-parent</artifactId>
    <groupId>com.javacode2018</groupId>
    <version>1.0-SNAPSHOT</version>
  </parent>
  <modelVersion>4.0.0</modelVersion>

  <artifactId>javacode2018-h5</artifactId>

</project>
```
javacode2018-api/pom.xml的内容：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <parent>
    <artifactId>javacode2018-parent</artifactId>
    <groupId>com.javacode2018</groupId>
    <version>1.0-SNAPSHOT</version>
  </parent>
  <modelVersion>4.0.0</modelVersion>

  <artifactId>javacode2018-api</artifactId>

</project>
```
在父构件javacode2018-parent/pom.xml中加入下面配置：
```xml
<packaging>pom</packaging>
<dependencies>
  <dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-web</artifactId>
    <version>5.2.1.RELEASE</version>
  </dependency>
  <dependency>
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis-spring</artifactId>
    <version>2.0.3</version>
  </dependency>
</dependencies>
```
使用下面命令在javacode2018-parent/pom.xml所在目录看一下javacode2018-parent构件依赖情况:
```
D:\code\IdeaProjects\javacode2018-parent>mvn dependency:tree
[INFO] Scanning for projects...
[INFO]
[INFO] ----------------< com.javacode2018:javacode2018-parent >-----------------
[INFO] Building javacode2018-parent 1.0-SNAPSHOT
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- maven-dependency-plugin:2.8:tree (default-cli) @ javacode2018-parent ---
[INFO] com.javacode2018:javacode2018-parent:pom:1.0-SNAPSHOT
[INFO] +- org.springframework:spring-web:jar:5.2.1.RELEASE:compile
[INFO] |  +- org.springframework:spring-beans:jar:5.2.1.RELEASE:compile
[INFO] |  \- org.springframework:spring-core:jar:5.2.1.RELEASE:compile
[INFO] |     \- org.springframework:spring-jcl:jar:5.2.1.RELEASE:compile
[INFO] \- org.mybatis:mybatis-spring:jar:2.0.3:compile
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.544 s
[INFO] Finished at: 2019-11-19T14:58:21+08:00
[INFO] ------------------------------------------------------------------------
```
`mvn dependency:tree` 这个插件可以根据pom.xml的配置，列出构件的依赖树信息。<br />再来看看javacode2018-pc/pom.xml构件的依赖信息：
```
D:\code\IdeaProjects\javacode2018-parent>cd javacode2018-pc

D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc>mvn dependency:tree
[INFO] Scanning for projects...
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-pc >-------------------
[INFO] Building javacode2018-pc 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-dependency-plugin:2.8:tree (default-cli) @ javacode2018-pc ---
[INFO] com.javacode2018:javacode2018-pc:jar:1.0-SNAPSHOT
[INFO] +- org.springframework:spring-web:jar:5.2.1.RELEASE:compile
[INFO] |  +- org.springframework:spring-beans:jar:5.2.1.RELEASE:compile
[INFO] |  \- org.springframework:spring-core:jar:5.2.1.RELEASE:compile
[INFO] |     \- org.springframework:spring-jcl:jar:5.2.1.RELEASE:compile
[INFO] \- org.mybatis:mybatis-spring:jar:2.0.3:compile
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.093 s
[INFO] Finished at: 2019-11-19T14:59:34+08:00
[INFO] ------------------------------------------------------------------------
```
从上图中可以看到javacode2018-pc依赖的jar和父构件javacode2019-parent的一样，说明从父类继承了这些依赖。<br />其他2个子构件，大家也可以去看一下，依赖关系都从父构件继承过来了。
<a name="rkJAx"></a>
### `relativePath`元素
上面演示的父构件和子构件的目录结构刚好符合父子关系，如果父构件和子构件的目录不是父子关系，比如都位于同等级别的目录或者位于更复杂的目录的时候，此时需要在子pom.xml的parent元素中使用relativePath元素来指定父pom.xml相对路径位置，这个值上面没有指定，默认是../pom.xml，表示父pom.xml位于子pom.xml的上一级目录，模块刚好符合这种关系，所以这个值省略了。<br />正确的设置relativePath是非常重要的，这个需要注意，子模块中执行mvn命令的时候，会去找父pom.xml的配置，会先通过relativePath指定的路径去找，如果找不到，会尝试通过坐标在本地仓库进行查找，如果本地找不到，会去远程仓库找，如果远程仓库也没有，会报错。
<a name="HjPSc"></a>
### 可以通过继承的元素有以下这些
上面看到了groupId、version、dependency中的依赖在子pom.xml中都没有写，这些都是从父pom.xml中继承过来的，还有很多元素也可以被继承过来，下面列个清单：

- groupId：项目组ID，项目坐标的核心元素
- version：项目版本，项目坐标的核心元素
- description：项目的描述信息
- organization：项目的组织信息
- inceptionYear：项目的创始年份
- url：项目的url地址
- developers：项目的开发者信息
- contributors：项目的贡献者信息
- distributionManagement：项目的部署配置信息
- issueManagement：项目的缺陷跟踪系统信息
- ciManagement：项目的持续集成系统信息
- scm：项目的版本控制系统信息
- mailingLists：项目的邮件列表信息
- properties：自定义的Maven属性配置信息
- dependencyManagement：项目的依赖管理配置
- repositories：项目的仓库配置
- build：包括项目的源码目录配置、输出目录配置、插件管理配置等信息
- reporting：包括项目的报告输出目录配置、报告插件配置等信息
<a name="GYWl9"></a>
## 依赖管理(dependencyManagement)
大家是否发现了，上面的继承存在的一个问题，如果再新增一个子构件，都会默认从父构件中继承依赖的一批构建，父pom.xml中配置的这些依赖的构建可能是其他项目不需要的，可能某个子项目只是想使用其中一个构件，但是上面的继承关系却把所有的依赖都给传递到子构件中了，这种显然是不合适的。<br />Maven中也考虑到了这种情况，可以使用dependencyManagement元素来解决这个问题。<br />Maven提供的dependencyManagement元素既能让子模块继承到父模块的依赖配置，又能保证子模块依赖使用的灵活性，**在dependencyManagement元素下声明的依赖不会引入实际的依赖，他只是声明了这些依赖，不过它可以对dependencies中使用的依赖起到一些约束作用。**<br />修改javacode2018-parent/pom.xml配置如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>javacode2018-parent</artifactId>
  <version>1.0-SNAPSHOT</version>

  <packaging>pom</packaging>

  <dependencyManagement>
    <dependencies>
      <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-web</artifactId>
        <version>5.2.1.RELEASE</version>
      </dependency>
      <dependency>
        <groupId>org.mybatis</groupId>
        <artifactId>mybatis-spring</artifactId>
        <version>2.0.3</version>
      </dependency>
    </dependencies>
  </dependencyManagement>

</project>
```
可以看到将dependencies配置移到dependencyManagement下面了。<br />使用下面命令看一下项目的依赖情况：
```
D:\code\IdeaProjects\javacode2018-parent>mvn dependency:tree
[INFO] Scanning for projects...
[INFO]
[INFO] ----------------< com.javacode2018:javacode2018-parent >-----------------
[INFO] Building javacode2018-parent 1.0-SNAPSHOT
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- maven-dependency-plugin:2.8:tree (default-cli) @ javacode2018-parent ---
[INFO] com.javacode2018:javacode2018-parent:pom:1.0-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.756 s
[INFO] Finished at: 2019-11-19T15:44:52+08:00
[INFO] ------------------------------------------------------------------------

D:\code\IdeaProjects\javacode2018-parent>cd javacode2018-pc

D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc>mvn dependency:tree
[INFO] Scanning for projects...
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-pc >-------------------
[INFO] Building javacode2018-pc 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-dependency-plugin:2.8:tree (default-cli) @ javacode2018-pc ---
[INFO] com.javacode2018:javacode2018-pc:jar:1.0-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.676 s
[INFO] Finished at: 2019-11-19T15:45:08+08:00
[INFO] ------------------------------------------------------------------------
```
父子构件中都看不到依赖的jar包了，说明父pom.xml中dependencyManagement这些依赖的构建没有被子模块依赖进去。<br />子模块如果想用到这些配置，可以dependencies进行引用，引用之后，依赖才会真正的起效。<br />在在3个子模块的pom.xml中加入下面配置：
```xml
<dependencies>
  <dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-web</artifactId>
  </dependency>
  <dependency>
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis-spring</artifactId>
  </dependency>
</dependencies>
```
运行一下上面的3个命令，看效果：
```
D:\code\IdeaProjects\javacode2018-parent>mvn dependency:tree
[INFO] Scanning for projects...
[INFO]
[INFO] ----------------< com.javacode2018:javacode2018-parent >-----------------
[INFO] Building javacode2018-parent 1.0-SNAPSHOT
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- maven-dependency-plugin:2.8:tree (default-cli) @ javacode2018-parent ---
[INFO] com.javacode2018:javacode2018-parent:pom:1.0-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.759 s
[INFO] Finished at: 2019-11-19T15:48:28+08:00
[INFO] ------------------------------------------------------------------------

D:\code\IdeaProjects\javacode2018-parent>cd javacode2018-pc

D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc>mvn dependency:tree
[INFO] Scanning for projects...
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-pc >-------------------
[INFO] Building javacode2018-pc 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-dependency-plugin:2.8:tree (default-cli) @ javacode2018-pc ---
[INFO] com.javacode2018:javacode2018-pc:jar:1.0-SNAPSHOT
[INFO] +- org.springframework:spring-web:jar:5.2.1.RELEASE:compile
[INFO] |  +- org.springframework:spring-beans:jar:5.2.1.RELEASE:compile
[INFO] |  \- org.springframework:spring-core:jar:5.2.1.RELEASE:compile
[INFO] |     \- org.springframework:spring-jcl:jar:5.2.1.RELEASE:compile
[INFO] \- org.mybatis:mybatis-spring:jar:2.0.3:compile
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.875 s
[INFO] Finished at: 2019-11-19T15:48:38+08:00
[INFO] ------------------------------------------------------------------------
```
javacode2018-parent构件中没有列出依赖信息，而javacode2018-pc列出了依赖信息。<br />大家看一下下面的截图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209757417-dd3b257e-1b35-4fbc-9527-1fe6977f0606.png#averageHue=%23f6f3f2&clientId=uac2b062c-923f-4&from=paste&id=uc3f1fdb1&originHeight=738&originWidth=734&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u003b6a45-0d7b-4719-8dab-d9b83c78132&title=)<br />注意上面的红框中依赖的构建没有version，左边2个红圈中的2个向上的箭头，表示这个是从父pom.xml中传递过来的，所以version可以省略。<br />再看一下父pom.xml截图效果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209758920-1532fcca-35b9-4e6c-b4a0-ff341bebf478.png#averageHue=%23f6f5f4&clientId=uac2b062c-923f-4&from=paste&id=u5e25d74f&originHeight=850&originWidth=706&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue7c8a899-87f0-4e5b-8987-bee551c2d8d&title=)<br />dependencyManagement中定义了依赖的构建，2个向下的箭头表示被子模块有继承。<br />**dependencyManagement不会引入实际的依赖，只有在子类中使用dependency来引入父dependencyManagement声明的依赖之后，依赖的构建才会被真正的引入。**<br />**使用dependencyManagement来解决继承的问题，子pom.xml中只用写groupId,artifactId就可以了，其他信息都会从父dependencyManagement中声明的依赖关系中传递过来，通常使用这种方式将所有依赖的构建在父pom.xml中定义好，子构件中只需要通过groupId,artifactId就可以引入依赖的构建，而不需要写version，可以很好的确保多个子项目中依赖构件的版本的一致性，对应依赖构件版本的升级也非常方便，只需要在父pom.xml中修改一下就可以了。**
<a name="MzgHU"></a>
## 单继承问题
<a name="WITmF"></a>
### 存在的问题及解决方案
上面讲解了dependencyManagement的使用，但是有个问题，只有使用继承的时候，dependencyManagement中声明的依赖才可能被子pom.xml用到，如果项目本来就有父pom.xml了，但是现在想使用另外一个项目dependencyManagement中声明的依赖，此时怎么办？这就是单继承的问题，这种情况在spring-boot、spring-cloud中会遇到，所以大家需要注意，这块一定需要玩转。<br />想在项目中使用另外一个构件中dependencyManagement声明的依赖，而又不想继承这个项目的时候，可以在项目中使用加入下面配置：
```xml
<dependencyManagement>
  <dependencies>
    <dependency>
      <groupId>com.javacode2018</groupId>
      <artifactId>javacode2018-parent</artifactId>
      <version>1.0-SNAPSHOT</version>
      <type>pom</type>
      <scope>import</scope>
    </dependency>
    <dependency>构件2</dependency>
    <dependency>构件3</dependency>
    <dependency>构件n</dependency>
  </dependencies>
</dependencyManagement>
```
上面这个配置会将javacode2018-parent构件中dependencyManagement元素中声明的所有依赖导入到当前pom.xml的dependencyManagement中，相当于把下面部分的内容：
```xml
<dependency>
  <groupId>com.javacode2018</groupId>
  <artifactId>javacode2018-parent</artifactId>
  <version>1.0-SNAPSHOT</version>
  <type>pom</type>
  <scope>import</scope>
</dependency>
```
替换成了javacode2018-parent/pom.xml中dependencyManagement元素内容，替换之后变成：
```xml
<dependencyManagement>
  <dependencies>
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-web</artifactId>
      <version>5.2.1.RELEASE</version>
    </dependency>
    <dependency>
      <groupId>org.mybatis</groupId>
      <artifactId>mybatis-spring</artifactId>
      <version>2.0.3</version>
    </dependency>
    <dependency>构件2</dependency>
    <dependency>构件3</dependency>
    <dependency>构件n</dependency>
  </dependencies>
</dependencyManagement>
```
<a name="W4tUH"></a>
### 实操案例
创建项目javacode2018-pom-import，具体过程不演示了，坐标信息：
```xml
<groupId>com.javacode2018</groupId>
<artifactId>javacode2018-pom-import</artifactId>
<version>1.0-SNAPSHOT</version>
```
创建完毕之后，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690209759185-71e28013-83f0-4fea-ba3f-108ff1719f26.jpeg#averageHue=%23f3f3f2&clientId=uac2b062c-923f-4&from=paste&id=u764d286d&originHeight=871&originWidth=1009&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2ea226a1-324d-4883-a1ac-1674c4e7618&title=)<br />修改javacode2018-pom-import/pom.xml内容，如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>javacode2018-pom-import</artifactId>
  <version>1.0-SNAPSHOT</version>

  <dependencyManagement>
    <dependencies>
      <dependency>
        <groupId>com.alibaba</groupId>
        <artifactId>fastjson</artifactId>
        <version>1.2.62</version>
      </dependency>
      <dependency>
        <groupId>junit</groupId>
        <artifactId>junit</artifactId>
        <version>4.12</version>
        <scope>test</scope>
      </dependency>
    </dependencies>
  </dependencyManagement>

</project>
```
上面在dependencyManagement中声明了2个依赖：fastjson和junit。<br />在javacode2018-pom-import/pom.xml中执行`mvn install`将其安装到本地仓库
```
D:\code\IdeaProjects\javacode2018-pom-import>mvn install
[INFO] Scanning for projects...
[INFO]
[INFO] --------------< com.javacode2018:javacode2018-pom-import >--------------
[INFO] Building javacode2018-pom-import 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-pom-import ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-pom-import ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-pom-import ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode2018-pom-import\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-pom-import ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-pom-import ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-pom-import ---
[INFO] Building jar: D:\code\IdeaProjects\javacode2018-pom-import\target\javacode2018-pom-import-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ javacode2018-pom-import ---
[INFO] Installing D:\code\IdeaProjects\javacode2018-pom-import\target\javacode2018-pom-import-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pom-import\1.0-SNAPSHOT\javacode2018-pom-import-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\javacode2018-pom-import\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pom-import\1.0-SNAPSHOT\javacode2018-pom-import-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.692 s
[INFO] Finished at: 2019-11-19T17:23:16+08:00
[INFO] ------------------------------------------------------------------------
```
现在想在javacode2018-pc/pom.xml中使用javacode2018-pom-import/pom.xml的dependencyManagement元素中定义的fastjson和junit的依赖，而javacode2019-pc已经继承了javacode-parent了，Maven中只能单继承，所以没法通过继承的方式来实现了，那么可以在javacode2019-pc/pom.xml中加入下面配置：
```xml
<dependencyManagement>
  <dependencies>
    <dependency>
      <groupId>com.javacode2018</groupId>
      <artifactId>javacode2018-pom-import</artifactId>
      <version>1.0-SNAPSHOT</version>
      <type>pom</type>
      <scope>import</scope>
    </dependency>
  </dependencies>
</dependencyManagement>
```
此时可以使用fastjson的依赖了，javacode2018-pc/pom.xml中project->dependencies元素中加入下面配置：
```xml
<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>fastjson</artifactId>
</dependency>
```
看一下效果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209759093-2618d83d-2adf-4d69-9422-fab43f4433ee.png#averageHue=%23f8f3f2&clientId=uac2b062c-923f-4&from=paste&id=u94c955c3&originHeight=827&originWidth=742&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u62a012c2-53a6-43fc-a14d-d31c9c9d160&title=)<br />上面的fastjson没有指定版本号，直接可以使用，说明从javacode2018-pom-import传递过来了。<br />可以点击一下上面红圈中的箭头，会跳到本地仓库的javacode2018-pom-import-1.0-SNAPSHOT.pom文件，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209759454-21b3e19f-0ef3-4da5-a869-b09f4c471d2b.png#averageHue=%23f7f6f4&clientId=uac2b062c-923f-4&from=paste&id=ud3a25cd5&originHeight=757&originWidth=770&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9d61d9ff-3779-42b5-a3b3-eb2dfb6755a&title=)
<a name="vP9n6"></a>
## 插件管理(pluginManagement)
Maven中提供了dependencyManagement来解决继承的问题，同样也提供了解决插件继承问题的pluginManagement元素，在父pom中可以在这个元素中声明插件的配置信息，但是子pom.xml中不会引入此插件的配置信息，只有在子pom.xml中使用plugins->plugin元素正在引入这些声明的插件的时候，插件才会起效，子插件中只需要写groupId、artifactId，其他信息都可以从父构件中传递过来。
<a name="ezG7L"></a>
### 实操案例
在javacode2018-parent/pom.xml中加入下面配置：
```xml
<build>
  <pluginManagement>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-source-plugin</artifactId>
        <version>3.2.0</version>
        <executions>
          <execution>
            <id>attach-source</id>
            <phase>verify</phase>
            <goals>
              <goal>jar-no-fork</goal>
            </goals>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </pluginManagement>
</build>
```
maven-source-plugin这个插件在上一章有介绍过，源码打包的。<br />上面是将插件maven-source-plugin的jar-no-fork目标绑定在default生命周期的verify阶段，verify阶段会在default生命周期的install周期前面执行。<br />下面看效果。<br />在javacode2018-pc/pom.xml所在目录运行下面命令
```
D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc>mvn clean install
[INFO] Scanning for projects...
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-pc >-------------------
[INFO] Building javacode2018-pc 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ javacode2018-pc ---
[INFO] Deleting D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-pc ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-pc ---
[INFO] Building jar: D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ javacode2018-pc ---
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.254 s
[INFO] Finished at: 2019-11-19T16:24:39+08:00
[INFO] ------------------------------------------------------------------------
```
`mvn clean install` 会清理代码，然后将打包构件，将构建安装到本地仓库，从输入中可以看到javacode2018-pc-1.0-SNAPSHOT.jar被安装到本地仓库了。<br />但是没有看到打包源码的插件的运行，说明了javacode2018-pc没有从父pom.xml中继承插件的配置信息，所以插件配置没有起效，现在要让插件起效，继续看：<br />在javacode2018-pc/pom.xml中加入下面配置：
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-source-plugin</artifactId>
    </plugin>
  </plugins>
</build>
```
截图看效果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690209759587-eb5648a9-7fbb-4e3d-8eef-a0ff5befc133.png#averageHue=%23f8f7f5&clientId=uac2b062c-923f-4&from=paste&id=ud5563710&originHeight=711&originWidth=737&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uee30fd62-cd84-4195-83a3-b1c06cdfa0d&title=)<br />又一个线上的箭头，说明这个是从父pom.xml中传递过来了，大家仔细看一下上面的配置，插件只写了groupId、artifactId，其他信息都没有写，其他信息都可以从父pom.xml中传递过来，下面看一下插件是否起效了，运行下面命令，见证奇迹：
```
D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc>mvn clean install
[INFO] Scanning for projects...
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-pc >-------------------
[INFO] Building javacode2018-pc 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ javacode2018-pc ---
[INFO] Deleting D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-pc ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-pc ---
[INFO] Building jar: D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-source-plugin:3.2.0:jar-no-fork (attach-source) @ javacode2018-pc ---
[INFO] Building jar: D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT-sources.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ javacode2018-pc ---
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT.pom
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT-sources.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT-sources.jar
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.477 s
[INFO] Finished at: 2019-11-19T16:28:18+08:00
[INFO] ------------------------------------------------------------------------
```
注意上面的输出中有attach-source，这个就是上面插件任务的id，输出中还有javacode2018-pc-1.0-SNAPSHOT-sources.jar信息，说明源码也是打包成功了，最后也是上传到了本地仓库了。<br />上面演示了只用在子pom.xml中写上插件的groupId、artifactId就可以了，其他信息会从父pom.xml中插件的定义中传递过来，而子pom.xml中也可以自定义插件的这些配置，修改javacode2018-pc/pom.xml配置，如下：
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-source-plugin</artifactId>
      <executions>
        <execution>
          <id>attach-source</id>
          <goals>
            <goal>help</goal>
          </goals>
        </execution>
      </executions>
    </plugin>
  </plugins>
</build>
```
再看一下效果：
```
D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc>mvn clean install
[INFO] Scanning for projects...
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-pc >-------------------
[INFO] Building javacode2018-pc 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ javacode2018-pc ---
[INFO] Deleting D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-pc ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-pc ---
[INFO] Building jar: D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-source-plugin:3.2.0:help (attach-source) @ javacode2018-pc ---
[INFO] Apache Maven Source Plugin 3.2.0
  The Maven Source Plugin creates a JAR archive of the source files of the
  current project.

This plugin has 7 goals:

source:aggregate
  Aggregate sources for all modules in an aggregator project.

source:generated-test-jar
  This plugin bundles all the test sources into a jar archive.

source:help
  Display help information on maven-source-plugin.
  Call mvn source:help -Ddetail=true -Dgoal=<goal-name> to display parameter
  details.

source:jar
  This plugin bundles all the sources into a jar archive.

source:jar-no-fork
  This goal bundles all the sources into a jar archive. This goal functions the
  same as the jar goal but does not fork the build and is suitable for attaching
  to the build lifecycle.

source:test-jar
  This plugin bundles all the test sources into a jar archive.

source:test-jar-no-fork
  This goal bundles all the test sources into a jar archive. This goal functions
  the same as the test-jar goal but does not fork the build, and is suitable for
  attaching to the build lifecycle.


[INFO]
[INFO] --- maven-source-plugin:3.2.0:jar-no-fork (attach-source) @ javacode2018-pc ---
[INFO] Building jar: D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT-sources.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ javacode2018-pc ---
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT.pom
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT-sources.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT-sources.jar
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.752 s
[INFO] Finished at: 2019-11-19T16:41:21+08:00
[INFO] ------------------------------------------------------------------------
```
输出中有下面2行：
```
[INFO] --- maven-source-plugin:3.2.0:help (attach-source) @ javacode2018-pc ---
[INFO] --- maven-source-plugin:3.2.0:jar-no-fork (attach-source) @ javacode2018-pc ---
```
说明maven-source-plugin插件执行了2个目标：help和jar-no-fork。此时父子pom.xml中插件配置信息合并了，所以出现了2个目标。具体最终javacode2018-pc/pom.xml中的内容是什么样的，可以通过下面这个命令看到：
```bash
mvn help:effective-pom
```
上面这个命令大家最好去看一下效果，当pom.xml中存在复杂的关系的时候，可以通过这个命令解析得到这个构件最终pom.xml的内容。
<a name="uIrb8"></a>
## 聚合与继承的关系
前面已经详解了聚合和继承，想必大家对这块也有了自己的理解。<br />聚合主要是为了方便多模块快速构建。<br />而继承主要是为了重用相同的配置。<br />对于聚合来说，聚合模块是知道被聚合模块的存在的，而被聚合模块是感知不到聚合模块的存在。<br />对于继承来说，父构件是感知不到子构件的存在，而子构件需要使用parent来引用父构件。<br />两者的共同点是，聚合模块和继承中的父模块的package属性都必须是pom类型的，同时，聚合模块和父模块中的除了pom.xml，一般都是没有什么内容的。<br />实际使用是，经常将聚合和继承一起使用，能同时使用到两者的优点。<br />下面在javacode2018-parent中加入聚合的配置：
```xml
<modules>
  <module>javacode2018-pc</module>
  <module>javacode2018-h5</module>
  <module>javacode2018-api</module>
</modules>
```
javacode20180-parent/pom.xml目录运行一下`mvn install`，看看效果：
```
D:\code\IdeaProjects\javacode2018-parent>mvn install
[INFO] Scanning for projects...
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Build Order:
[INFO]
[INFO] javacode2018-parent                                                [pom]
[INFO] javacode2018-pc                                                    [jar]
[INFO] javacode2018-h5                                                    [jar]
[INFO] javacode2018-api                                                   [jar]
[INFO]
[INFO] ----------------< com.javacode2018:javacode2018-parent >----------------
[INFO] Building javacode2018-parent 1.0-SNAPSHOT                          [1/4]
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ javacode2018-parent ---
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-parent\1.0-SNAPSHOT\javacode2018-parent-1.0-SNAPSHOT.pom
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-pc >------------------
[INFO] Building javacode2018-pc 1.0-SNAPSHOT                              [2/4]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-pc ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-pc ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-pc ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-pc ---
[INFO]
[INFO] --- maven-source-plugin:3.2.0:help (attach-source) @ javacode2018-pc ---
[INFO] Apache Maven Source Plugin 3.2.0
  The Maven Source Plugin creates a JAR archive of the source files of the
  current project.

This plugin has 7 goals:

source:aggregate
  Aggregate sources for all modules in an aggregator project.

source:generated-test-jar
  This plugin bundles all the test sources into a jar archive.

source:help
  Display help information on maven-source-plugin.
  Call mvn source:help -Ddetail=true -Dgoal=<goal-name> to display parameter
  details.

source:jar
  This plugin bundles all the sources into a jar archive.

source:jar-no-fork
  This goal bundles all the sources into a jar archive. This goal functions the
  same as the jar goal but does not fork the build and is suitable for attaching
  to the build lifecycle.

source:test-jar
  This plugin bundles all the test sources into a jar archive.

source:test-jar-no-fork
  This goal bundles all the test sources into a jar archive. This goal functions
  the same as the test-jar goal but does not fork the build, and is suitable for
  attaching to the build lifecycle.


[INFO]
[INFO] --- maven-source-plugin:3.2.0:jar-no-fork (attach-source) @ javacode2018-pc ---
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ javacode2018-pc ---
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT.pom
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-pc\target\javacode2018-pc-1.0-SNAPSHOT-sources.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-pc\1.0-SNAPSHOT\javacode2018-pc-1.0-SNAPSHOT-sources.jar
[INFO]
[INFO] ------------------< com.javacode2018:javacode2018-h5 >------------------
[INFO] Building javacode2018-h5 1.0-SNAPSHOT                              [3/4]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-h5 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-h5 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-h5 ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode2018-parent\javacode2018-h5\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-h5 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-h5 ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-h5 ---
[INFO] Building jar: D:\code\IdeaProjects\javacode2018-parent\javacode2018-h5\target\javacode2018-h5-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ javacode2018-h5 ---
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-h5\target\javacode2018-h5-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-h5\1.0-SNAPSHOT\javacode2018-h5-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-h5\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-h5\1.0-SNAPSHOT\javacode2018-h5-1.0-SNAPSHOT.pom
[INFO]
[INFO] -----------------< com.javacode2018:javacode2018-api >------------------
[INFO] Building javacode2018-api 1.0-SNAPSHOT                             [4/4]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ javacode2018-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ javacode2018-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ javacode2018-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\javacode2018-parent\javacode2018-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ javacode2018-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ javacode2018-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ javacode2018-api ---
[INFO] Building jar: D:\code\IdeaProjects\javacode2018-parent\javacode2018-api\target\javacode2018-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ javacode2018-api ---
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-api\target\javacode2018-api-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-api\1.0-SNAPSHOT\javacode2018-api-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\javacode2018-parent\javacode2018-api\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\javacode2018-api\1.0-SNAPSHOT\javacode2018-api-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary for javacode2018-parent 1.0-SNAPSHOT:
[INFO]
[INFO] javacode2018-parent ................................ SUCCESS [  0.416 s]
[INFO] javacode2018-pc .................................... SUCCESS [  2.654 s]
[INFO] javacode2018-h5 .................................... SUCCESS [  0.262 s]
[INFO] javacode2018-api ................................... SUCCESS [  0.225 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  3.827 s
[INFO] Finished at: 2019-11-19T17:51:42+08:00
[INFO] ------------------------------------------------------------------------
```
<a name="nO7ZF"></a>
## 2个问题
<a name="f7Mkr"></a>
### 问题1：下面这个配置是干什么的？给哪个插件使用的？
```xml
<properties>
  <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
</properties>
```
编译代码的时候，涉及到资源文件和测试资源文件的拷贝，拷贝文件的时候涉及到文件的编码，这个是设置文件的编码为UTF-8格式的。<br />在项目的目录中执行`mvn compile`命令，效果如下：
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
[INFO] Nothing to compile - all classes are up to date
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.179 s
[INFO] Finished at: 2019-11-19T17:57:27+08:00
[INFO] ------------------------------------------------------------------------
```
可以看到上面有这样的输出：
```
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ maven-chat06 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
```
从上面可以看出maven-resources-plugin插件的resources目标中会使用这个编码的配置，来看一下这个目标具体参数配置，截取了部分输出，如下：
```
D:\code\IdeaProjects\maven-chat06>mvn help:describe -Dplugin=resources -Dgoal=resources  -Ddetail
[INFO] Scanning for projects...
[INFO]
[INFO] -------------------< com.javacode2018:maven-chat06 >--------------------
[INFO] Building maven-chat06 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-help-plugin:3.2.0:describe (default-cli) @ maven-chat06 ---
[INFO] Mojo: 'resources:resources'
resources:resources
  Description: Copy resources for the main source code to the main output
    directory. Always uses the project.build.resources element to specify the
    resources to copy.
  Implementation: org.apache.maven.plugin.resources.ResourcesMojo
  Language: java
  Bound to phase: process-resources

  Available parameters:

    delimiters
      Set of delimiters for expressions to filter within the resources. These
      delimiters are specified in the form 'beginToken*endToken'. If no '*' is
      given, the delimiter is assumed to be the same for start and end.

      So, the default filtering delimiters might be specified as:

      <delimiters>
       <delimiter>${*}</delimiter>
       <delimiter>@</delimiter>
      </delimiters>

      Since the '@' delimiter is the same on both ends, we don't need to
      specify '@*@' (though we can).

    encoding (Default: ${project.build.sourceEncoding})
      User property: encoding
      The character encoding scheme to be applied when filtering resources.
```
注意上面输出中最后的一部分，如下：
```
encoding (Default: ${project.build.sourceEncoding})
      User property: encoding
      The character encoding scheme to be applied when filtering resources.
```
encoding这个参数用来指定编码的，默认值是`${project.build.sourceEncoding}`，也可以通过`encoding`用户属性来设置。<br />所以这个设置编码的还有下面3种写法，共四种：<br />pom.xml中2种：
```xml
<encoding>UTF-8</encoding>
<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
```
mvn命令中2种：
```bash
mvn compile -Dencoding=UTF-8
mvn compile -Dproject.build.sourceEncoding=UTF-8
```
<a name="zg97L"></a>
### 问题2：`mvn test`运行测试用例的时候，测试用例类名的写法默认是有规则的，这些规则是什么？从哪里可以看到这些规则？如何自定义？
执行`mvn test`，看看效果：
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
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  1.411 s
[INFO] Finished at: 2019-11-19T18:07:52+08:00
[INFO] ------------------------------------------------------------------------
```
可以看到下面这行：
```
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ maven-chat06 ---
```
表示运行测试使用的是插件maven-surefire-plugin的test目标。<br />看看这个目标详细参数，只列出部分信息，如下：
```
D:\code\IdeaProjects\maven-chat06>mvn help:describe -Dplugin=surefire -Dgoal=test -Ddetail
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

    includes
      A list of <include> elements specifying the tests (by pattern) that
      should be included in testing. When not specified and when the test
      parameter is not specified, the default includes will be
      <includes>
       <include>**/IT*.java</include>
       <include>**/*IT.java</include>
       <include>**/*ITCase.java</include>
      </includes>
      Each include item may also contain a comma-separated sublist of items,
      which will be treated as multiple  <include> entries.
      This parameter is ignored if the TestNG suiteXmlFiles parameter is
      specified.


[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  4.458 s
[INFO] Finished at: 2019-11-19T18:10:09+08:00
[INFO] ------------------------------------------------------------------------
```
可以看到上面有个`includes`参数，可以用来配置需要运行的测试用例，可以配置通配符的方式。<br />上面还有一段信息：
```
Implementation: org.apache.maven.plugin.surefire.SurefirePlugin
```
上面这部分列出了这个目标的具体实现类是SurefirePlugin。<br />那么可以到本地仓库中看一下这个构件的源码，构件的坐标是：
```xml
<groupId>org.apache.maven.plugins</groupId>
<artifactId>maven-surefire-plugin</artifactId>
<version>2.12.4</version>
```
打开org.apache.maven.plugin.surefire.SurefirePlugin的源码，会找到下面代码：
```java
protected String[] getDefaultIncludes()
{
    return new String[]{ "**/Test*.java", "**/*Test.java", "**/*TestCase.java" };
}
```
这部分代码就是测试用例默认需要满足的格式，创建的测试用例默认情况下必须满足上面这3种格式，否则，测试用例不会被`mvn test`执行。
