Maven
<a name="grE7Z"></a>
## 需求背景
需要做一个电商项目，一般都会做成微服务的形式，按业务进行划分，本次主要以**账户业务**和**订单业务**为例，将这两块业务分别作为2个大的模块来开发，而订单模块又会调用账户模块中的接口，所以对账户模块接口部分会有依赖。<br />以Maven来搭建项目，项目的目录结构如下：
```
b2b
    b2b-account
        b2b-account-api
        b2b-account-service
    b2b-order
        b2b-order-api
        b2b-order-service
```
<a name="BvOs0"></a>
### b2b-account
账户模块，其中包含2个小模块：b2b-account-api和b2b-account-service
<a name="ByLgy"></a>
#### b2b-account-api
账户模块对外暴露的接口部分，以供外部调用
<a name="ApTza"></a>
#### b2b-account-service
账户模块具体业务实现，依赖于b2b-account-api模块
<a name="lo3UE"></a>
### b2b-order
订单模块，也是包含2个小模块：b2b-order-api和b2b-order-service
<a name="k7OaA"></a>
#### b2b-order-api
订单模块对外暴露的接口部分，以供外部调用
<a name="wrYYz"></a>
#### b2b-order-service
订单模块具体业务实现，依赖2个模块b2b-order-api、b2b-account-api
<a name="mgnTR"></a>
## 搭建项目结构
按照上面的需求，使用Maven搭建项目结构，具体过程如下。
<a name="yUlWd"></a>
### 创建b2b项目
打开idea，点击File->New->Project，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211381950-455f77f2-c63f-4565-8ff0-ee02909693af.png#averageHue=%23e4dfdc&clientId=u6c47a73a-3479-4&from=paste&id=u810956b9&originHeight=442&originWidth=671&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u16e02a65-4458-45b6-9d72-3cdc40a8f5e&title=)<br />选择Maven，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690211382049-9a17a8af-8bf4-4529-a17f-36e272e884c2.jpeg#averageHue=%23e5e1e0&clientId=u6c47a73a-3479-4&from=paste&id=ue86fd23e&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u82d4dc60-deac-43dd-9dd1-6ee716935e6&title=)<br />点击上图的Next，输入坐标信息，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211382042-375b60bb-5ee8-4959-b074-f93b88867dd8.png#averageHue=%23e9e7e7&clientId=u6c47a73a-3479-4&from=paste&id=u2a9f2498&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u39f44fa1-43f0-433f-9ec6-eb615297870&title=)<br />点击上图的Next，输入Project Name为b2b，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211382035-f8233593-7b07-4ae9-b139-112f6a4cd305.png#averageHue=%23e8e6e6&clientId=u6c47a73a-3479-4&from=paste&id=u095dded6&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7141d36f-aa13-4135-be54-5caa4be65ef&title=)<br />点击上图的Finish，完成项目的创建，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211382328-d1020412-c40f-420f-97d9-6708e3996df2.png#averageHue=%23f4f3f2&clientId=u6c47a73a-3479-4&from=paste&id=u5a7ac0a7&originHeight=527&originWidth=1075&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u897b7a58-48ee-413f-8a63-03d14ecbcca&title=)<br />删除下图中无用的代码：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211383607-e61fa5df-05fd-4e07-9cf6-915ee3d58e68.png#averageHue=%23f5f1f0&clientId=u6c47a73a-3479-4&from=paste&id=u192e5671&originHeight=285&originWidth=331&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uae635ba9-8977-45e2-a71f-4ea1dcdea0c&title=)<br />变成了下面这样：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211383680-17c43336-9778-4945-a026-5030788688d9.png#averageHue=%23d2aa70&clientId=u6c47a73a-3479-4&from=paste&id=u50fb9709&originHeight=243&originWidth=328&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u58a799a9-8e4f-4cd2-bdd2-78615c2fd4a&title=)<br />配置一下idea的Maven环境，点击File->Settings，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211383682-858ff788-1d5e-4ba6-bd88-5e158b5ebd5a.png#averageHue=%23d17863&clientId=u6c47a73a-3479-4&from=paste&id=u2cc27298&originHeight=641&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub5a692c5-e190-4541-bfc5-448bfc728fa&title=)<br />点击上面的OK完成配置。
<a name="VkDE8"></a>
### 在b2b下创建b2b-account模块
注意这里是创建模块，不是项目了，注意下面的操作过程。<br />选中b2b项目目录，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211383605-d36dd8ed-ff5b-41fa-bdf4-730eb41f0bf0.png#averageHue=%23fcf4f3&clientId=u6c47a73a-3479-4&from=paste&id=u982fff01&originHeight=233&originWidth=432&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufab1b12a-7dae-44a9-8b49-bef2daedf94&title=)<br />点击File->New->Module，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211383685-43366ffe-a0c4-4218-afc3-b19e36a390f6.png#averageHue=%23e4dedc&clientId=u6c47a73a-3479-4&from=paste&id=ub44e19b0&originHeight=466&originWidth=672&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubad47c4f-bece-4b10-a8ba-ce1e4821ac7&title=)<br />选择Maven，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690211384587-90b15c0e-5343-4206-85e9-58dc76366c1b.jpeg#averageHue=%23e6e2e2&clientId=u6c47a73a-3479-4&from=paste&id=u4df92e53&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufd083756-3353-48ca-905d-d975e812040&title=)<br />点击Next，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211384563-e85c3cd1-6027-47e3-8cd8-80c0d24a6adb.png#averageHue=%23e7e7e7&clientId=u6c47a73a-3479-4&from=paste&id=u03613fa7&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1458ec00-e2e4-4413-95be-449bc002f80&title=)<br />点击上面第二个...，将Parent置为None，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211385485-82f512c9-7fa1-4cc2-b44c-410d0b314789.png#averageHue=%23e7e7e6&clientId=u6c47a73a-3479-4&from=paste&id=u20f0513d&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc34ca171-723e-448e-a4a3-cddac865434&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211385223-956a9eca-76e5-4f44-a8d0-b7b895e9d6b3.png#averageHue=%23ebe7e6&clientId=u6c47a73a-3479-4&from=paste&id=ubfdb7f7b&originHeight=243&originWidth=440&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9844ea5d-d97b-4367-bdc9-57c527428ce&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211385668-3b8ce633-1ee1-485b-8c08-d751053d3dee.png#averageHue=%23e8e8e8&clientId=u6c47a73a-3479-4&from=paste&id=uda1140c5&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uefed2ec7-0b1e-4c76-a00e-3292188266b&title=)<br />输入坐标信息，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211386599-ea35a10e-cf71-4bb3-954d-c693f2d920e4.png#averageHue=%23e8e8e7&clientId=u6c47a73a-3479-4&from=paste&id=ub174d194&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9693db83-1218-49b8-8a64-16f2556fb0d&title=)<br />点击上图中的Next，输入Project name为b2b-account，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211386587-2800656e-40d1-419c-afe3-1e029fbd12c6.png#averageHue=%23e9e7e6&clientId=u6c47a73a-3479-4&from=paste&id=u00bdd976&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u362d4495-1a42-4cb8-ae00-cc90b93f905&title=)<br />点击Finish，完成b2b-account模块的创建，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211386888-8ad74afa-f6f2-47f8-a6fe-1d65f3446f8a.png#averageHue=%23f4f3f2&clientId=u6c47a73a-3479-4&from=paste&id=u0586ae30&originHeight=455&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u88d737b4-679f-4a23-8e8a-c7ac849c21a&title=)<br />删除b2b-account中src目录，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211386973-5e04762c-e8c1-4c80-a3e6-583c6a8b4952.png#averageHue=%23dcae6d&clientId=u6c47a73a-3479-4&from=paste&id=ud3b9852b&originHeight=373&originWidth=459&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u86d64dbb-41c3-41bb-a91c-3a0690cbf10&title=)<br />项目结构变成了下面这样：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211387290-1a675d9b-7231-402f-8be0-3eb867522ec6.png#averageHue=%23f7f6f5&clientId=u6c47a73a-3479-4&from=paste&id=ue3b460c5&originHeight=355&originWidth=467&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud862d242-0e13-4934-9166-ecc43d50888&title=)
<a name="LSDg3"></a>
### 在b2b-account模块下面创建b2b-account-api模块
选中b2b-account，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211388013-abd31554-16f9-4891-afb5-96dfad5538a5.png#averageHue=%23faf5f3&clientId=u6c47a73a-3479-4&from=paste&id=u066e25fc&originHeight=296&originWidth=448&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf1ece69a-b9a3-45b4-b9f4-66b26ae7ae4&title=)<br />点击File->New->Module，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211388620-d6ff23a3-0e39-49e7-8c60-aa07ece846ac.png#averageHue=%23e3e1df&clientId=u6c47a73a-3479-4&from=paste&id=u1109f7cf&originHeight=217&originWidth=674&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue79522e4-0f84-496d-97f1-9e1456ce455&title=)<br />选择Maven，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1690211388988-c6bf9210-57e4-4348-acae-0886e852905c.jpeg#averageHue=%23e5e2e1&clientId=u6c47a73a-3479-4&from=paste&id=u03298f42&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4d24b03b-a04c-400b-a464-85ee95788f8&title=)<br />点击上图中的Next，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211388941-c89e61de-0da5-4d18-ab72-e38093a91d4f.png#averageHue=%23e7e5e5&clientId=u6c47a73a-3479-4&from=paste&id=u71d16d60&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u11e15c25-d059-469c-8340-a9dae8ad72e&title=)<br />将上面的Add as module to设置为b2b-account模块，表示新增的这个模块被b2b-account聚合进行管理。<br />点击Parent后面的...，选择None，将Parent对应的值置为None。<br />如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211389246-2ba39e72-9660-432f-a446-50dc6995bd9f.png#averageHue=%23e7e7e6&clientId=u6c47a73a-3479-4&from=paste&id=u31c4dc22&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u77f7f059-2ce7-418d-a73b-e43c1d650a1&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211389977-bce1bb2f-4186-42f1-92cf-86fd636a2dd4.png#averageHue=%23e9e7e7&clientId=u6c47a73a-3479-4&from=paste&id=u86a35643&originHeight=223&originWidth=440&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u09adc482-8fef-4b74-a021-523a398c5f8&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211390575-99ec25b3-617a-43a1-9081-42b0abe04b48.png#averageHue=%23e8e8e8&clientId=u6c47a73a-3479-4&from=paste&id=u8cf34bb4&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5c64b27f-a755-49b5-81b7-42fa3b97163&title=)<br />输入坐标信息，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211390857-88e7da6e-6e88-4f8c-81d5-56553197744a.png#averageHue=%23e9e6e5&clientId=u6c47a73a-3479-4&from=paste&id=u20c7dce8&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uda733502-92fd-447a-88fa-bfb2d325eb5&title=)<br />点击Next，默认如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211390937-605e8009-e9d8-45ee-a6a1-a7b858d6f35e.png#averageHue=%23e8e8e8&clientId=u6c47a73a-3479-4&from=paste&id=uc36a5ea7&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4f4ce153-dd55-40b7-a274-0dcd040f79f&title=)<br />需要对上面3个输入框的值进行修改，修改成下面这样：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211390936-622310fa-2cb5-4b2e-9286-44fe51f8e72b.png#averageHue=%23e9e6e6&clientId=u6c47a73a-3479-4&from=paste&id=u0e3a2615&originHeight=696&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufdf291a8-1390-4bb3-8f3d-9476919c82f&title=)<br />点击Finish，完成创建，如下图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211391846-fa5131ef-6a4c-4cfe-a764-101d7fc86090.png#averageHue=%23f3f2f1&clientId=u6c47a73a-3479-4&from=paste&id=ud58367ed&originHeight=541&originWidth=1051&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufdcd0e79-500c-4a84-94cc-a773971a666&title=)
<a name="B8iVU"></a>
### 在b2b-account模块下创建b2b-account-service模块
这个过程参考在b2b-account模块下面创建b2b-account-api模块。
<a name="oSZ8A"></a>
### 在b2b下创建b2b-order模块
这个过程参考在b2b下创建b2b-account模块。
<a name="QCft5"></a>
### 在b2b-order模块下面创建b2b-order-api模块
这个过程参考在b2b-account模块下面创建b2b-account-api模块。
<a name="oE2VR"></a>
### 在b2b-order模块下面创建b2b-order-service模块
这个过程参考在b2b-account模块下面创建b2b-account-api模块。
<a name="JywaT"></a>
### 项目结构创建成功
上面都创建成功之后，项目结构如下图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690211392254-576ffac5-da80-4c63-91f5-de652c13844f.png#averageHue=%23f9f8f7&clientId=u6c47a73a-3479-4&from=paste&id=u4856243e&originHeight=744&originWidth=448&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u23d92875-e225-46b8-a3e0-d21ccfeceda&title=)<br />参与过大型Maven项目开发的是不是很熟悉很亲切，多数互联网项目的Maven结构就是这样一大片Maven模块。
<a name="vLjGw"></a>
### 引入项目依赖
b2b-account-service依赖于b2b-account-api模块，所以在b2b-account-service/pom.xml中加入下面配置：
```xml
<dependencies>
  <dependency>
    <groupId>${project.groupId}</groupId>
    <artifactId>b2b-account-api</artifactId>
    <version>${project.version}</version>
  </dependency>
</dependencies>
```
由于项目的groupId,version都是一样的，所以直接通过${}这种方式引用了。<br />b2b-order-service依赖2个模块b2b-order-api、b2b-account-api，所以在b2b-order-service/pom.xml中加入下面配置：
```xml
<dependencies>
  <dependency>
    <groupId>${project.groupId}</groupId>
    <artifactId>b2b-account-api</artifactId>
    <version>${project.version}</version>
  </dependency>
  <dependency>
    <groupId>${project.groupId}</groupId>
    <artifactId>b2b-order-api</artifactId>
    <version>${project.version}</version>
  </dependency>
</dependencies>
```
<a name="tPXx6"></a>
### 此时每个模块pom.xml如下
<a name="KuTJ0"></a>
#### b2b/pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>b2b</artifactId>
  <packaging>pom</packaging>
  <version>1.0-SNAPSHOT</version>
  <modules>
    <module>b2b-account</module>
    <module>b2b-order</module>
  </modules>

</project>
```
<a name="oxvGA"></a>
#### b2b-account/pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>b2b-account</artifactId>
  <packaging>pom</packaging>
  <version>1.0-SNAPSHOT</version>
  <modules>
    <module>b2b-account-api</module>
    <module>b2b-account-service</module>
  </modules>

</project>
```
<a name="sNGzu"></a>
#### b2b-account-api/pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>b2b-account-api</artifactId>
  <version>1.0-SNAPSHOT</version>

</project>
```
<a name="Th8FH"></a>
#### b2b-account-service/pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>b2b-account-service</artifactId>
  <version>1.0-SNAPSHOT</version>

  <dependencies>
    <dependency>
      <groupId>${project.groupId}</groupId>
      <artifactId>b2b-account-api</artifactId>
      <version>${project.version}</version>
    </dependency>
  </dependencies>

</project>
```
<a name="zYm2O"></a>
#### b2b-order/pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>b2b-order</artifactId>
  <packaging>pom</packaging>
  <version>1.0-SNAPSHOT</version>
  <modules>
    <module>b2b-order-api</module>
    <module>b2b-order-service</module>
  </modules>

</project>
```
<a name="L11gU"></a>
#### b2b-order-api/pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>b2b-order-api</artifactId>
  <version>1.0-SNAPSHOT</version>

</project>
```
<a name="pNQuy"></a>
#### b2b-order-service/pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>b2b-order-service</artifactId>
  <version>1.0-SNAPSHOT</version>

  <dependencies>
    <dependency>
      <groupId>${project.groupId}</groupId>
      <artifactId>b2b-account-api</artifactId>
      <version>${project.version}</version>
    </dependency>
    <dependency>
      <groupId>${project.groupId}</groupId>
      <artifactId>b2b-order-api</artifactId>
      <version>${project.version}</version>
    </dependency>
  </dependencies>

</project>
```
注意：上面项目的结构属于父子结构，使用Maven中聚合的方式进行管理的。<br />b2b中包含了2个子模块b2b_account和b2b_order，这3个package都是pom。<br />b2b_account中包含了2个子模块，2个子模块package是默认的jar类型。<br />b2b_order中包含了2个子模块，2个子模块package是默认的jar类型。
<a name="Hk34l"></a>
## 反应堆
上面项目都开发好了，需要安装到本地仓库，一般情况下会这么做，在b2b/pom.xml所在目录执行下面命令：
```bash
mvn clean install
```
来感受一下这个命令的效果：
```
D:\code\IdeaProjects\b2b>mvn clean install
[INFO] Scanning for projects...
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-account-api                                                    [jar]
[INFO] b2b-account-service                                                [jar]
[INFO] b2b-account                                                        [pom]
[INFO] b2b-order-api                                                      [jar]
[INFO] b2b-order-service                                                  [jar]
[INFO] b2b-order                                                          [pom]
[INFO] b2b                                                                [pom]
[INFO]
[INFO] ------------------< com.javacode2018:b2b-account-api >------------------
[INFO] Building b2b-account-api 1.0-SNAPSHOT                              [1/7]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-account-api ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-account-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-account-api ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-account-api ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.pom
[INFO]
[INFO] ----------------< com.javacode2018:b2b-account-service >----------------
[INFO] Building b2b-account-service 1.0-SNAPSHOT                          [2/7]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-account-service ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-account-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-account-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-account-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-account-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-account-service ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-account-service ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\target\b2b-account-service-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-account-service ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\target\b2b-account-service-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-service\1.0-SNAPSHOT\b2b-account-service-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-service\1.0-SNAPSHOT\b2b-account-service-1.0-SNAPSHOT.pom
[INFO]
[INFO] --------------------< com.javacode2018:b2b-account >--------------------
[INFO] Building b2b-account 1.0-SNAPSHOT                                  [3/7]
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-account ---
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-account ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account\1.0-SNAPSHOT\b2b-account-1.0-SNAPSHOT.pom
[INFO]
[INFO] -------------------< com.javacode2018:b2b-order-api >-------------------
[INFO] Building b2b-order-api 1.0-SNAPSHOT                                [4/7]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-order-api ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-order-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-order-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-order-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-order-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-order-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-order-api ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\target\b2b-order-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-order-api ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\target\b2b-order-api-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-api\1.0-SNAPSHOT\b2b-order-api-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-api\1.0-SNAPSHOT\b2b-order-api-1.0-SNAPSHOT.pom
[INFO]
[INFO] -----------------< com.javacode2018:b2b-order-service >-----------------
[INFO] Building b2b-order-service 1.0-SNAPSHOT                            [5/7]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-order-service ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-order-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-order-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-order-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-order-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-order-service ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-order-service ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\target\b2b-order-service-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-order-service ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\target\b2b-order-service-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-service\1.0-SNAPSHOT\b2b-order-service-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-service\1.0-SNAPSHOT\b2b-order-service-1.0-SNAPSHOT.pom
[INFO]
[INFO] ---------------------< com.javacode2018:b2b-order >---------------------
[INFO] Building b2b-order 1.0-SNAPSHOT                                    [6/7]
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-order ---
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-order ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order\1.0-SNAPSHOT\b2b-order-1.0-SNAPSHOT.pom
[INFO]
[INFO] ------------------------< com.javacode2018:b2b >------------------------
[INFO] Building b2b 1.0-SNAPSHOT                                          [7/7]
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b ---
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b ---
[INFO] Installing D:\code\IdeaProjects\b2b\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b\1.0-SNAPSHOT\b2b-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary for b2b 1.0-SNAPSHOT:
[INFO]
[INFO] b2b-account-api .................................... SUCCESS [  1.806 s]
[INFO] b2b-account-service ................................ SUCCESS [  0.298 s]
[INFO] b2b-account ........................................ SUCCESS [  0.082 s]
[INFO] b2b-order-api ...................................... SUCCESS [  0.229 s]
[INFO] b2b-order-service .................................. SUCCESS [  0.254 s]
[INFO] b2b-order .......................................... SUCCESS [  0.058 s]
[INFO] b2b ................................................ SUCCESS [  0.069 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  3.072 s
[INFO] Finished at: 2019-11-20T16:03:27+08:00
[INFO] ------------------------------------------------------------------------
```
注意上面有这样的输出：
```
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-account-api                                                    [jar]
[INFO] b2b-account-service                                                [jar]
[INFO] b2b-account                                                        [pom]
[INFO] b2b-order-api                                                      [jar]
[INFO] b2b-order-service                                                  [jar]
[INFO] b2b-order                                                          [pom]
[INFO] b2b                                                                [pom]
```
Maven会根据模块之间的依赖关系，然后会得到所有模块的构建顺序，上面共有7个pom.xml文件，也就是7个Maven构件，按照上面列出的顺序依次进行构建。<br />可以看到b2b-account-api是被其他一些模块依赖的，所以这个放在了第一个。<br />mvn命令对多模块构件时，会根据模块的依赖关系而得到模块的构建顺序，这个功能就是Maven的反应堆（reactor）做的事情，反应堆会根据模块之间的依赖关系、聚合关系、继承关系等等，从而计算得出一个合理的模块构建顺序，所以反应堆的功能是相当强大的。
<a name="aqQ4L"></a>
## 按需随意构建
有这样的一种场景：b2b-account-api被b2b-account-service和b2b-order-service依赖了，所以当b2b-account-api有修改的时候，希望他们3个都能够被重新构建一次，而不是去对所有的模块都进行重新构建，只希望被影响的模块都能够参与重新构建，大家有什么好的办法？<br />上面列出的只是2个模块的功能，真正的电商项目还有很多模块，如果每次修改一个模块，都去重新打包所有的模块，这个构建过程耗时是非常久的，只能干等着，需要的是按需构建，需要构建哪些模块让开发者自己能够随意指定，这样也可以加快构建的速度，所以需要这样的功能<br />Maven反应堆考虑到了这种情况，mvn命令提供了一些功能可以实现这些操作，看一下mvn的帮助文档，mvn -h可以查看帮助，如下：
```bash
D:\code\IdeaProjects\b2b>mvn -h

usage: mvn [options] [<goal(s)>] [<phase(s)>]

Options:
 -am,--also-make                        If project list is specified, also
                                        build projects required by the
                                        list
 -amd,--also-make-dependents            If project list is specified, also
                                        build projects that depend on
                                        projects on the list
 -B,--batch-mode                        Run in non-interactive (batch)
                                        mode (disables output color)
 -b,--builder <arg>                     The id of the build strategy to
                                        use
 -C,--strict-checksums                  Fail the build if checksums don't
                                        match
 -c,--lax-checksums                     Warn if checksums don't match
 -cpu,--check-plugin-updates            Ineffective, only kept for
                                        backward compatibility
 -D,--define <arg>                      Define a system property
 -e,--errors                            Produce execution error messages
 -emp,--encrypt-master-password <arg>   Encrypt master security password
 -ep,--encrypt-password <arg>           Encrypt server password
 -f,--file <arg>                        Force the use of an alternate POM
                                        file (or directory with pom.xml)
 -fae,--fail-at-end                     Only fail the build afterwards;
                                        allow all non-impacted builds to
                                        continue
 -ff,--fail-fast                        Stop at first failure in
                                        reactorized builds
 -fn,--fail-never                       NEVER fail the build, regardless
                                        of project result
 -gs,--global-settings <arg>            Alternate path for the global
                                        settings file
 -gt,--global-toolchains <arg>          Alternate path for the global
                                        toolchains file
 -h,--help                              Display help information
 -l,--log-file <arg>                    Log file where all build output
                                        will go (disables output color)
 -llr,--legacy-local-repository         Use Maven 2 Legacy Local
                                        Repository behaviour, ie no use of
                                        _remote.repositories. Can also be
                                        activated by using
                                        -Dmaven.legacyLocalRepo=true
 -N,--non-recursive                     Do not recurse into sub-projects
 -npr,--no-plugin-registry              Ineffective, only kept for
                                        backward compatibility
 -npu,--no-plugin-updates               Ineffective, only kept for
                                        backward compatibility
 -nsu,--no-snapshot-updates             Suppress SNAPSHOT updates
 -ntp,--no-transfer-progress            Do not display transfer progress
                                        when downloading or uploading
 -o,--offline                           Work offline
 -P,--activate-profiles <arg>           Comma-delimited list of profiles
                                        to activate
 -pl,--projects <arg>                   Comma-delimited list of specified
                                        reactor projects to build instead
                                        of all projects. A project can be
                                        specified by [groupId]:artifactId
                                        or by its relative path
 -q,--quiet                             Quiet output - only show errors
 -rf,--resume-from <arg>                Resume reactor from specified
                                        project
 -s,--settings <arg>                    Alternate path for the user
                                        settings file
 -t,--toolchains <arg>                  Alternate path for the user
                                        toolchains file
 -T,--threads <arg>                     Thread count, for instance 2.0C
                                        where C is core multiplied
 -U,--update-snapshots                  Forces a check for missing
                                        releases and updated snapshots on
                                        remote repositories
 -up,--update-plugins                   Ineffective, only kept for
                                        backward compatibility
 -v,--version                           Display version information
 -V,--show-version                      Display version information
                                        WITHOUT stopping build
 -X,--debug                             Produce execution debug output
```
上面列出了mvn命令后面的一些选项，有几个选项本次需要用到，如下：
<a name="hVp21"></a>
### -pl,--projects <arg>
构件指定的模块，arg表示多个模块，之间用逗号分开，模块有两种写法
```
-pl 模块1相对路径 [,模块2相对路径] [,模块n相对路径]
-pl [模块1的groupId]:模块1的artifactId [,[模块2的groupId]:模块2的artifactId] [,[模块n的groupId]:模块n的artifactId]
```
举例：<br />下面命令都是在b2b/pom.xml来运行
```bash
mvn clean install -pl b2b-account
mvn clean install -pl b2b-account/b2b-account-api
mvn clean install -pl b2b-account/b2b-account-api,b2b-account/b2b-account-service
mvn clean install -pl :b2b-account-api,b2b-order/b2b-order-api
mvn clean install -pl :b2b-account-api,:b2b-order-service
```
<a name="Fd6kl"></a>
### -rf,--resume-from <arg>
从指定的模块恢复反应堆
<a name="sjuoU"></a>
### -am,--also-make
同时构建所列模块的依赖模块
<a name="VyTTJ"></a>
### -amd,--also-make-dependents
同时构件依赖于所列模块的模块<br />下面通过6个案例细说上面这些用法。<br />**注意：下面所有案例都在b2b/pom.xml所在目录执行。**
<a name="bC5PH"></a>
## 案例1
只构建p-account模块，运行命令
```
mvn clean install -pl b2b-account
```
效果如下：
```
D:\code\IdeaProjects\b2b>mvn clean install -pl b2b-account
[INFO] Scanning for projects...
[INFO]
[INFO] --------------------< com.javacode2018:b2b-account >--------------------
[INFO] Building b2b-account 1.0-SNAPSHOT
[INFO] --------------------------------[ pom ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-account ---
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-account ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account\1.0-SNAPSHOT\b2b-account-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.907 s
[INFO] Finished at: 2019-11-20T16:12:51+08:00
[INFO] ------------------------------------------------------------------------
```
可以看到只构建了b2b-account。
<a name="Ehcpc"></a>
## 案例2
只构建b2b-account-api模块，运行命令：
```bash
mvn clean install -pl b2b-account/b2b-account-api
```
效果如下：
```
D:\code\IdeaProjects\b2b>mvn clean install -pl b2b-account/b2b-account-api
[INFO] Scanning for projects...
[INFO]
[INFO] ------------------< com.javacode2018:b2b-account-api >------------------
[INFO] Building b2b-account-api 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-account-api ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-account-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-account-api ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-account-api ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  2.853 s
[INFO] Finished at: 2019-11-20T16:14:37+08:00
[INFO] ------------------------------------------------------------------------
```
上面使用的是相对路径的写法，还有2种写法，大家可以去试试，如下：
```bash
mvn clean install -pl :b2b-account-api
mvn clean install -pl com.javacode2018:b2b-account-api
```
<a name="H5HID"></a>
## 案例3
构建b2b-account-api和b2b-order-api，运行下面命令：
```bash
mvn clean install -pl b2b-account/b2b-account-api,b2b-order/b2b-order-api
```
效果如下：
```
D:\code\IdeaProjects\b2b>mvn clean install -pl b2b-account/b2b-account-api,b2b-order/b2b-order-api
[INFO] Scanning for projects...
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-account-api                                                    [jar]
[INFO] b2b-order-api                                                      [jar]
[INFO]
[INFO] ------------------< com.javacode2018:b2b-account-api >------------------
[INFO] Building b2b-account-api 1.0-SNAPSHOT                              [1/2]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-account-api ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-account-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-account-api ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-account-api ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.pom
[INFO]
[INFO] -------------------< com.javacode2018:b2b-order-api >-------------------
[INFO] Building b2b-order-api 1.0-SNAPSHOT                                [2/2]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-order-api ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-order-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-order-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-order-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-order-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-order-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-order-api ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\target\b2b-order-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-order-api ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\target\b2b-order-api-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-api\1.0-SNAPSHOT\b2b-order-api-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-api\1.0-SNAPSHOT\b2b-order-api-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary for b2b-account-api 1.0-SNAPSHOT:
[INFO]
[INFO] b2b-account-api .................................... SUCCESS [  2.874 s]
[INFO] b2b-order-api ...................................... SUCCESS [  0.393 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  3.554 s
[INFO] Finished at: 2019-11-20T16:15:57+08:00
[INFO] ------------------------------------------------------------------------
```
上面输出中可以看到只构建了2个目标模块。
<a name="Eorbu"></a>
## 案例4
只修改了b2b-account-api代码，所以希望对这个重新打包，并且对这个有依赖的也重新打包，所以需要打包下面3个模块：
```
b2b-account-api
b2b-account-service
b2b-order-service
```
上面列表中的后面两个是依赖于b2b-account-api的，可以在b2b/pom.xml中执行下面命令：
```bash
mvn clean install -pl b2b-account/b2b-account-api -amd
```
看一下效果：
```
D:\code\IdeaProjects\b2b>mvn clean install -pl b2b-account/b2b-account-api -amd
[INFO] Scanning for projects...
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-account-api                                                    [jar]
[INFO] b2b-account-service                                                [jar]
[INFO] b2b-order-service                                                  [jar]
[INFO]
[INFO] ------------------< com.javacode2018:b2b-account-api >------------------
[INFO] Building b2b-account-api 1.0-SNAPSHOT                              [1/3]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-account-api ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-account-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-account-api ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-account-api ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.pom
[INFO]
[INFO] ----------------< com.javacode2018:b2b-account-service >----------------
[INFO] Building b2b-account-service 1.0-SNAPSHOT                          [2/3]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-account-service ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-account-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-account-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-account-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-account-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-account-service ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-account-service ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\target\b2b-account-service-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-account-service ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\target\b2b-account-service-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-service\1.0-SNAPSHOT\b2b-account-service-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-service\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-service\1.0-SNAPSHOT\b2b-account-service-1.0-SNAPSHOT.pom
[INFO]
[INFO] -----------------< com.javacode2018:b2b-order-service >-----------------
[INFO] Building b2b-order-service 1.0-SNAPSHOT                            [3/3]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-order-service ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-order-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-order-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-order-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-order-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-order-service ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-order-service ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\target\b2b-order-service-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-order-service ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\target\b2b-order-service-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-service\1.0-SNAPSHOT\b2b-order-service-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-service\1.0-SNAPSHOT\b2b-order-service-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary for b2b-account-api 1.0-SNAPSHOT:
[INFO]
[INFO] b2b-account-api .................................... SUCCESS [  2.440 s]
[INFO] b2b-account-service ................................ SUCCESS [  0.381 s]
[INFO] b2b-order-service .................................. SUCCESS [  0.364 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  3.508 s
[INFO] Finished at: 2019-11-20T16:05:00+08:00
[INFO] ------------------------------------------------------------------------
```
从上面输出中看一下反应堆列出的构建顺序：b2b-account-api、b2b-account-service、b2b-order-service。<br />上面过程给大家捋一捋：<br />上面命令先会运行-pl b2b-account-api得到一个反应堆列表，如下，只有一个模块：
```
b2b-account-api
```
然后后面又会执行amd，这个命令会找到对-pl b2b-account-api有依赖的构件，也就是：
```
b2b-account-service
b2b-order-serivce
```
然后反应堆会对3个构件进行排序，得到一个正确的构件顺序，如下：
```
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-account-api                                                    [jar]
[INFO] b2b-account-service                                                [jar]
[INFO] b2b-order-service                                                  [jar]
```
然后Maven会依次对他们执行：
```bash
mvn clean install
```
<a name="TCoU4"></a>
## 案例5
需求：来构建b2b-order-service，希望b2b-account-service依赖的构件也能被同时构建，b2b-account-service依赖于b2b-account-api和b2b-order-api，可能b2b-account-service会依赖的更多。<br />可以使用下面命令：
```bash
mvn clean install -pl b2b-order/b2b-order-service -am
```
效果如下：
```
D:\code\IdeaProjects\b2b>mvn clean install -pl b2b-order/b2b-order-service -am
[INFO] Scanning for projects...
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-account-api                                                    [jar]
[INFO] b2b-order-api                                                      [jar]
[INFO] b2b-order-service                                                  [jar]
[INFO]
[INFO] ------------------< com.javacode2018:b2b-account-api >------------------
[INFO] Building b2b-account-api 1.0-SNAPSHOT                              [1/3]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-account-api ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-account-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-account-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-account-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-account-api ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-account-api ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\target\b2b-account-api-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-account\b2b-account-api\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-account-api\1.0-SNAPSHOT\b2b-account-api-1.0-SNAPSHOT.pom
[INFO]
[INFO] -------------------< com.javacode2018:b2b-order-api >-------------------
[INFO] Building b2b-order-api 1.0-SNAPSHOT                                [2/3]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-order-api ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-order-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-order-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-order-api ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-order-api ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-order-api ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-order-api ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\target\b2b-order-api-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-order-api ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\target\b2b-order-api-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-api\1.0-SNAPSHOT\b2b-order-api-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-api\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-api\1.0-SNAPSHOT\b2b-order-api-1.0-SNAPSHOT.pom
[INFO]
[INFO] -----------------< com.javacode2018:b2b-order-service >-----------------
[INFO] Building b2b-order-service 1.0-SNAPSHOT                            [3/3]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ b2b-order-service ---
[INFO] Deleting D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\target
[INFO]
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ b2b-order-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ b2b-order-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ b2b-order-service ---
[WARNING] Using platform encoding (GBK actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ b2b-order-service ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ b2b-order-service ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ b2b-order-service ---
[INFO] Building jar: D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\target\b2b-order-service-1.0-SNAPSHOT.jar
[INFO]
[INFO] --- maven-install-plugin:2.4:install (default-install) @ b2b-order-service ---
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\target\b2b-order-service-1.0-SNAPSHOT.jar to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-service\1.0-SNAPSHOT\b2b-order-service-1.0-SNAPSHOT.jar
[INFO] Installing D:\code\IdeaProjects\b2b\b2b-order\b2b-order-service\pom.xml to C:\Users\Think\.m2\repository\com\javacode2018\b2b-order-service\1.0-SNAPSHOT\b2b-order-service-1.0-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary for b2b-account-api 1.0-SNAPSHOT:
[INFO]
[INFO] b2b-account-api .................................... SUCCESS [  2.509 s]
[INFO] b2b-order-api ...................................... SUCCESS [  0.343 s]
[INFO] b2b-order-service .................................. SUCCESS [  0.340 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  3.455 s
[INFO] Finished at: 2019-11-20T16:46:45+08:00
[INFO] ------------------------------------------------------------------------
```
从上面输出中看一下反应堆列出的构建顺序：b2b-account-api、b2b-order-api、b2b-order-service。<br />上面过程给大家捋一捋：<br />上面命令先会运行-pl b2b-order-service得到一个反应堆列表：
```
b2b-order-service
```
然后后面又会执行am，这个命令会找到-pl b2b-order-service依赖的构件，也就是：
```
b2b-account-api
b2b-order-api
```
然后反应堆会对3个构件进行排序，得到一个正确的构件顺序，如下：
```
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-account-api                                                    [jar]
[INFO] b2b-order-api                                                      [jar]
[INFO] b2b-order-service                                                  [jar]
```
然后Maven会依次对他们执行：
```bash
mvn clean install
```
<a name="eDwIB"></a>
## 案例6
分别执行下面2条mvn命令，先看一下效果：
```bash
mvn clean install
mvn clean install -rf b2b-order/b2b-order-service
```
输出中取出部分内容，如下。<br />第一条命令，反应堆产生的构建顺序是：
```
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-account-api                                                    [jar]
[INFO] b2b-account-service                                                [jar]
[INFO] b2b-account                                                        [pom]
[INFO] b2b-order-api                                                      [jar]
[INFO] b2b-order-service                                                  [jar]
[INFO] b2b-order                                                          [pom]
[INFO] b2b                                                                [pom]
```
第2条命令，反应堆产生的构建顺序是：
```
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-order-service                                                  [jar]
[INFO] b2b-order                                                          [pom]
[INFO] b2b                                                                [pom]
```
在仔细看一下上面2条命令的差别，后面的命令多了-rf b2b-order/b2b-order-service，具体过程如下：<br />会先执行下面命令
```bash
mvn clean install
```
反应堆会计算出需要构件的模块顺序，如下：
```
[INFO] Reactor Build Order:
[INFO]
[INFO] b2b-account-api                                                    [jar]
[INFO] b2b-account-service                                                [jar]
[INFO] b2b-account                                                        [pom]
[INFO] b2b-order-api                                                      [jar]
[INFO] b2b-order-service                                                  [jar]
[INFO] b2b-order                                                          [pom]
[INFO] b2b                                                                [pom]
```
-rf b2b-order/b2b-order-service对上面的反应堆构件顺序进行裁剪，将b2b-order/b2b-order-service前面的部分干掉，从b2b-order/b2b-order-service开始执行构建操作，所以剩下了3个需要构建的模块。
<a name="O4vsR"></a>
## 总结

1. 需要掌握mvn命令中`-pl`、`-am`、`-amd`、`-rf`的各种用法
2. 注意`-pl`后面的参数的写法：模块相对路径、或者[groupId].artifactId
