Java SpringBoot Elasticsearch
<a name="oixIz"></a>
## 前言
Spring Boot 中如何整合 ES，以及如何在 Spring Cloud 微服务项目中使用 ES 来实现全文检索，来达到搜索题库的功能。<br />而且题库的数据量是非常大的，题目的答案也是非常长的，通过 ES 正好可以解决 mysql 模糊搜索的低效性。<br />通过本实战可以学到如下知识点：

- Spring Boot 如何整合 ES。
- 微服务中 ES 的 API 使用。
- 项目中如何使用 ES 来达到全文检索。

本篇主要内容如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249453131-143570f5-5f40-40e8-8005-45a4617ee768.png#clientId=u9376670f-d04f-4&from=paste&id=u49f05582&originHeight=958&originWidth=844&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uc0aac58c-2392-4c26-952b-fe17e2e539e&title=)<br />主要内容<br />本文案例都是基于 PassJava 实战项目来演示的。<br />Github 地址：[https://github.com/Jackson0714/PassJava-Platform](https://github.com/Jackson0714/PassJava-Platform)<br />为了让大家更清晰地理解 PassJava 项目中 ES 是如何使用的，画了三个流程图：
<a name="J9tkL"></a>
### 第一步：创建 question 索引
首先定义 question 索引，然后在 ES 中创建索引。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249453144-ad1ff048-e469-4881-8cbb-f77855822ab4.png#clientId=u9376670f-d04f-4&from=paste&id=u56e887fb&originHeight=436&originWidth=688&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubdec5449-6717-4f7e-8f3e-0faa92b0f6c&title=)
<a name="uPQjn"></a>
### 第二步：存 question 数据进 ES 
前端保存数据时，保存的 API 请求先经过网关，然后转发到 passjava-question 微服务，然后远程调用 passjava-search 微服务，将数据保存进 ES 中。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249453143-5c854f58-81bd-4140-9231-6531b01e4e62.png#clientId=u9376670f-d04f-4&from=paste&id=ucd53b4b3&originHeight=890&originWidth=1050&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud3ba23b7-d0f5-482e-a75c-488ab4873fe&title=)
<a name="fJxtH"></a>
### 第三步：从 ES 中查数据
前端查询数据时，先经过网关，然后将请求转发给 passjava-search 微服务，然后从 ES 中查询数据。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249453142-96061aa4-2cc0-48de-a2f5-7c047d429c63.png#clientId=u9376670f-d04f-4&from=paste&id=u55a0d4fa&originHeight=618&originWidth=1072&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7826202a-9e59-41b7-a0ed-d482c8a0b5c&title=)
<a name="GFECM"></a>
## 一、Elasticsearch 组件库介绍
在讲解之前，在这里再次提下全文检索是什么：<br />全文检索： 指以全部文本信息作为检索对象的一种信息检索技术。而使用的数据库，如 Mysql，MongoDB 对文本信息检索能力特别是中文检索并没有 ES 强大。所以来看下 ES 在项目中是如何来代替 SQL 来工作的。<br />这里使用的 Elasticsearch 服务是 7.4.2 的版本，然后采用官方提供的 Elastiscsearch-Rest-Client 库来操作 ES，而且官方库的 API 上手简单。<br />该组件库的官方文档地址：<br />[https://www.elastic.co/guide/en/elasticsearch/client/java-rest/current/java-rest-high.html](https://www.elastic.co/guide/en/elasticsearch/client/java-rest/current/java-rest-high.html)<br />另外这个组件库是支持多种语言的：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249453144-675eb3c8-3854-4b83-8828-f63a99162080.png#clientId=u9376670f-d04f-4&from=paste&id=u6a43d212&originHeight=506&originWidth=466&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6eb164d3-192d-452a-85ab-a7eab0b581d&title=)<br />支持多语言<br />注意：Elasticsearch Clients 就是指如何用 API 操作 ES 服务的组件库。<br />可能有同学会提问，Elasticsearch 的组件库中写着 JavaScript API，是不是可以直接在前端访问 ES 服务？可以是可以，但是会暴露 ES 服务的端口和 IP 地址，会非常不安全。所以还是用后端服务来访问 ES 服务。<br />这个项目是 Java 项目，自然就是用上面的两种：Java Rest Client 或者 Java API。先看下 Java API，但是会发现已经废弃了。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249453835-6e6d09ea-89d8-4497-a322-591a8561eb60.png#clientId=u9376670f-d04f-4&from=paste&id=u939161eb&originHeight=325&originWidth=557&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u16abb8c6-bdc9-4253-9cbe-9f6877fc4e5&title=)<br />Java API 已经废弃了<br />所以只能用 Java REST Client 了。而它又分成两种：高级和低级的。高级包含更多的功能，如果把高级比作MyBatis的话，那么低级就相当于JDBC。所以用高级的 Client。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249453774-e0fada1f-ac3f-40f0-9204-5ae8a696ffa3.png#clientId=u9376670f-d04f-4&from=paste&id=ufee5d4e9&originHeight=163&originWidth=399&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u574c4c13-9f10-4fc2-92f8-ce8c1f3dee6&title=)<br />高级和低级 Client
<a name="H9Alr"></a>
## 二、整合检索服务
把检索服务单独作为一个服务。就称作 passjava-search 模块吧。
<a name="OWZfS"></a>
### 1.1 添加搜索服务模块

- 创建 passjava-search 模块。

首先在 PassJava-Platform 模块创建一个 搜索服务模块 passjava-search。然后勾选 spring web 服务。如下图所示。<br />第一步：选择 Spring Initializr，然后点击 Next。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249453820-74b17c49-72bc-4cea-be44-f319b9287b56.png#clientId=u9376670f-d04f-4&from=paste&id=u73f8d84d&originHeight=310&originWidth=608&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u36886bd7-0fb2-4332-8e78-2c4359fcd88&title=)<br />选择 Spring Initializr<br />第二步：填写模块信息，然后点击 Next。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249453991-a1961d95-dbaa-44a0-b7a2-a7550533d7fc.png#clientId=u9376670f-d04f-4&from=paste&id=u5cf43c26&originHeight=525&originWidth=452&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u8d31e321-fb9a-4b09-b80e-400db386bea&title=)<br />passjava-search 服务模块<br />第三步：选择 Web->Spring Web 依赖，然后点击 Next。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249454080-69394cf2-a599-476e-b716-9b89acac6d8d.png#clientId=u9376670f-d04f-4&from=paste&id=u4d9e274c&originHeight=296&originWidth=668&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u54510716-78cf-46f0-9673-7f5364df527&title=)
<a name="o1q3c"></a>
### 1.2 配置 Maven 依赖

- 参照 ES 官网配置。

进入到 ES 官方网站，可以看到有低级和高级的 Rest Client，选择高阶的（High Level Rest Client）。然后进入到高阶 Rest Client 的 Maven 仓库。官网地址如下所示：<br />[https://www.elastic.co/guide/en/elasticsearch/client/java-rest/7.9/index.html](https://www.elastic.co/guide/en/elasticsearch/client/java-rest/7.9/index.html)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624249454347-4186599e-bd28-4c86-a472-23948c20901e.webp#clientId=u9376670f-d04f-4&from=paste&id=u7e23051d&originHeight=524&originWidth=537&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u29dca3e7-ad62-47b7-a48e-cf85c89e30a&title=)<br />Rest Client 官方文档

- 加上 Maven 依赖。对应文件路径：\passjava-search\pom.xml
```xml
<dependency>
    <groupId>org.elasticsearch.client</groupId>
    <artifactId>elasticsearch-rest-high-level-client</artifactId>
    <version>7.4.2</version>
</dependency>
```

- 配置 elasticsearch 的版本为7.4.2因加上 Maven 依赖后，elasticsearch 版本为 7.6.2，所以遇到这种版本不一致的情况时，需要手动改掉。对应文件路径：\passjava-search\pom.xml
```xml
<properties>
	<elasticsearch.version>7.4.2</elasticsearch.version>
</properties>
```
刷新 Maven Project 后，可以看到引入的 elasticsearch 都是 7.4.2 版本了，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249454471-254a2db7-5a7c-4380-9754-867b46923ebc.png#clientId=u9376670f-d04f-4&from=paste&id=u90a3f41a&originHeight=349&originWidth=635&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uda91fd7f-6e4c-414a-ba87-04bd803dd0c&title=)<br />设置版本为 7.4.2

- 引入 PassJava 的 Common 模块依赖。Common 模块是 PassJava 项目独立的出来的公共模块，引入了很多公共组件依赖，其他模块引入 Common 模块依赖后，就不需要单独引入这些公共组件了，非常方便。对应文件路径：\passjava-search\pom.xml
```xml
 <dependency>
     <groupId>com.jackson0714.passjava</groupId>
     <artifactId>passjava-common</artifactId>
     <version>0.0.1-SNAPSHOT</version>
</dependency>
```
添加完依赖后，就可以将搜索服务注册到 Nacos 注册中心了。Nacos 注册中心的用法在前面几篇文章中也详细讲解过，这里需要注意的是要先启动 Nacos 注册中心，才能正常注册 passjava-search 服务。
<a name="WjRLr"></a>
### 1.3 注册搜索服务到注册中心
修改配置文件：src/main/resources/application.properties。配置应用程序名、注册中心地址、注册中心的命名中间。
```xml
spring.application.name=passjava-search
spring.cloud.nacos.config.server-addr=127.0.0.1:8848
spring.cloud.nacos.config.namespace=passjava-search
```
给启动类添加服务发现注解：`@EnableDiscoveryClient`。这样 passjava-search 服务就可以被注册中心发现了。<br />因 Common 模块依赖数据源，但 search 模块不依赖数据源，所以 search 模块需要移除数据源依赖：
```java
exclude = DataSourceAutoConfiguration.class
```
以上的两个注解如下所示：
```java
@EnableDiscoveryClient
@SpringBootApplication(exclude = DataSourceAutoConfiguration.class)
public class PassjavaSearchApplication {
    public static void main(String[] args) {
        SpringApplication.run(PassjavaSearchApplication.class, args);
    }
}
```
接下来添加一个 ES 服务的专属配置类，主要目的是自动加载一个 ES Client 来供后续 ES API 使用，不用每次都 new 一个 ES Client。
<a name="Pd19T"></a>
### 1.4 添加 ES 配置类
配置类：PassJavaElasticsearchConfig.java<br />核心方法就是 RestClient.builder 方法，设置好 ES 服务的 IP 地址、端口号、传输协议就可以了。最后自动加载了 RestHighLevelClient。
```java
import org.apache.http.HttpHost;
import org.elasticsearch.client.RestClient;
import org.elasticsearch.client.RestHighLevelClient;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class PassJavaElasticsearchConfig {

    @Bean
    // 给容器注册一个 RestHighLevelClient，用来操作 ES
    // 参考官方文档：https://www.elastic.co/guide/en/elasticsearch/client/java-rest/7.9/java-rest-high-getting-started-initialization.html
    public RestHighLevelClient restHighLevelClient() {
        return new RestHighLevelClient(
                RestClient.builder(
                        new HttpHost("192.168.56.10", 9200, "http")));
    }
}
```
接下来测试下 ES Client 是否自动加载成功。
<a name="oe4xV"></a>
### 1.5 测试 ES Client 自动加载
在测试类 PassjavaSearchApplicationTests 中编写测试方法，打印出自动加载的 ES Client。期望结果是一个 RestHighLevelClient 对象。
```java
import org.elasticsearch.client.RestHighLevelClient;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.test.context.SpringBootTest;

@SpringBootTest
class PassjavaSearchApplicationTests {

    @Qualifier("restHighLevelClient")
    @Autowired
    private RestHighLevelClient client;

    @Test
    public void contextLoads() {
        System.out.println(client);
    }
}
```
运行结果如下所示，打印出了 RestHighLevelClient。说明自定义的 ES Client 自动装载成功。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249454564-e3c2b29d-3d5b-49fc-b2f2-8e2ee13bef35.png#clientId=u9376670f-d04f-4&from=paste&id=u6377780a&originHeight=112&originWidth=712&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uafcc1277-4418-4070-abda-f36314bac71&title=)<br />ES 测试结果
<a name="eU228"></a>
### 1.6 测试 ES 简单插入数据
测试方法 testIndexData，省略 User 类。users 索引在 ES 中是没有记录的，所以期望结果是 ES 中新增了一条 users 数据。
```java
/**
 * 测试存储数据到 ES。
 * */
@Test
public void testIndexData() throws IOException {
    IndexRequest request = new IndexRequest("users");
    request.id("1"); // 文档的 id
    
    //构造 User 对象
    User user = new User();
    user.setUserName("PassJava");
    user.setAge("18");
    user.setGender("Man");
    
    //User 对象转为 JSON 数据
    String jsonString = JSON.toJSONString(user);
    
    // JSON 数据放入 request 中
    request.source(jsonString, XContentType.JSON);

    // 执行插入操作
    IndexResponse response = client.index(request, RequestOptions.DEFAULT);

    System.out.println(response);
}
```
执行 test 方法，可以看到控制台输出以下结果，说明数据插入到 ES 成功。另外需要注意的是结果中的 result 字段为 updated，是因为本地为了截图，多执行了几次插入操作，但因为 id = 1，所以做的都是 updated 操作，而不是 created 操作。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249454804-33fa75ff-c946-4f7b-a883-4a66f8c2f1fc.png#clientId=u9376670f-d04f-4&from=paste&id=uf392713a&originHeight=242&originWidth=853&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u5baa2b74-305c-4fb9-889a-f33c72c8692&title=)<br />控制台输出结果<br />再来到 ES 中看下 users 索引中数据。查询 users 索引：
```http
GET users/_search
```
结果如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249454892-fcf3bf3d-9b50-4a61-a02b-b7af8b53dabe.png#clientId=u9376670f-d04f-4&from=paste&id=u0b5de941&originHeight=524&originWidth=737&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2a82ecaf-438c-46e7-bd69-ff5da999233&title=)<br />查询 users 索引结果<br />可以从图中看到有一条记录被查询出来，查询出来的数据的 _id = 1，和插入的文档 id 一致。另外几个字段的值也是一致的。说明插入的数据没有问题。
```json
"age" : "18",
"gender" : "Man",
"userName" : "PassJava"
```
<a name="JZuHZ"></a>
### 1.7 测试 ES 查询复杂语句
示例：搜索 bank 索引，address 字段中包含 big 的所有人的年龄分布 ( 前 10 条 ) 以及平均年龄，以及平均薪资。
<a name="oL8Hr"></a>
#### 1.7.1 构造检索条件
可以参照官方文档给出的示例来创建一个 SearchRequest 对象，指定要查询的索引为 bank，然后创建一个 SearchSourceBuilder 来组装查询条件。总共有三种条件需要组装：

- address 中包含 road 的所有人。
- 按照年龄分布进行聚合。
- 计算平均薪资。

![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249455252-c9945149-d056-4d80-a620-4d65e6063e02.png#clientId=u9376670f-d04f-4&from=paste&id=ua4f72783&originHeight=339&originWidth=896&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uef2023ad-41fe-4bdc-a3ec-5f83f8a86be&title=)<br />查询复杂语句示例<br />将打印出来的检索参数复制出来，然后放到 JSON 格式化工具中格式化一下，再粘贴到 ES 控制台执行，发现执行结果是正确的。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249455138-b8d3cd0e-28be-4d08-9789-a7178213c906.png#clientId=u9376670f-d04f-4&from=paste&id=u44fe5f87&originHeight=185&originWidth=841&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u642dd54a-49eb-4822-ae2f-53edce01e8a&title=)<br />打印出检索参数<br />用在线工具格式化 JSON 字符串，结果如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249455296-5e968c02-f01a-4ac2-8426-57348921d173.png#clientId=u9376670f-d04f-4&from=paste&id=ub42402c5&originHeight=1330&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueca6075b-3d1c-4653-a12d-fbe4be62b3a&title=)<br />然后去掉其中的一些默认参数，最后简化后的检索参数放到 Kibana 中执行。<br />Kibana Dev Tools 控制台中执行检索语句如下图所示，检索结果如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249455771-f67a5d13-a5d7-4b70-988d-2cfc79e53a9e.png#clientId=u9376670f-d04f-4&from=paste&id=ufb30cd7a&originHeight=904&originWidth=1027&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubffe72b8-7f82-4833-ad3d-0f4d3d6e156&title=)<br />控制台中执行检索语句<br />找到总记录数：29 条。<br />第一条命中记录的详情如下：
```
平均 balance：13136。
平均年龄：26。
地址中包含 Road 的：263 Aviation Road。
```
和 IDEA 中执行的测试结果一致，说明复杂检索的功能已经成功实现。
<a name="KKxCv"></a>
#### 17.2 获取命中记录的详情
而获取命中记录的详情数据，则需要通过两次 `getHists()` 方法拿到，如下所示：
```java
// 3.1）获取查到的数据。
SearchHits hits = response.getHits();
// 3.2）获取真正命中的结果
SearchHit[] searchHits = hits.getHits();
```
可以通过遍历 searchHits 的方式打印出所有命中结果的详情。
```java
// 3.3）、遍历命中结果
for (SearchHit hit: searchHits) {
    String hitStr = hit.getSourceAsString();
    BankMember bankMember = JSON.parseObject(hitStr, BankMember.class);
}
```
拿到每条记录的 hitStr 是个 JSON 数据，如下所示：
```json
{
  "account_number": 431,
  "balance": 13136,
  "firstname": "Laurie",
  "lastname": "Shaw",
  "age": 26,
  "gender": "F",
  "address": "263 Aviation Road",
  "employer": "Zillanet",
  "email": "laurieshaw@zillanet.com",
  "city": "Harmon",
  "state": "WV"
}
```
而 BankMember 是根据返回的结果详情定义的的 JavaBean。可以通过工具自动生成。在线生成 JavaBean 的网站如下：[https://www.bejson.com/json2javapojo/new/](https://www.bejson.com/json2javapojo/new/)<br />把这个 JavaBean 加到 PassjavaSearchApplicationTests 类中：
```java
@ToString
@Data
static class BankMember {
    private int account_number;
    private int balance;
    private String firstname;
    private String lastname;
    private int age;
    private String gender;
    private String address;
    private String employer;
    private String email;
    private String city;
    private String state;
}
```
然后将 bankMember 打印出来：
```java
System.out.println(bankMember);
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249455827-f76044f6-ecce-4edd-8f24-6b79d7d07cf3.png#clientId=u9376670f-d04f-4&from=paste&id=ua119709f&originHeight=235&originWidth=516&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=ue8da7f85-2984-4908-9609-b47c554a530&title=)<br />bankMember<br />得到的结果确实是封装的 BankMember 对象，而且里面的属性值也都拿到了。
<a name="g8nhQ"></a>
#### 1.7.3 获取年龄分布聚合信息
ES 返回的 response 中，年龄分布的数据是按照 ES 的格式返回的，如果想按照自己的格式来返回，就需要将 response 进行处理。<br />如下图所示，这个是查询到的年龄分布结果，需要将其中某些字段取出来，比如 buckets，它代表了分布在 21 岁的有 4 个。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249455904-e8956272-714f-4ca2-908a-874a6fe251b9.png#clientId=u9376670f-d04f-4&from=paste&id=u87e3a79b&originHeight=232&originWidth=412&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5916f15e-ece7-4e2c-ab44-0cc9dca7e8d&title=)<br />ES 返回的年龄分布信息<br />下面是代码实现：
```java
Aggregations aggregations = response.getAggregations();
Terms ageAgg1 = aggregations.get("ageAgg");
for (Terms.Bucket bucket : ageAgg1.getBuckets()) {
    String keyAsString = bucket.getKeyAsString();
    System.out.println("用户年龄： " + keyAsString + " 人数：" + bucket.getDocCount());
}
```
最后打印的结果如下，21 岁的有 4 人，26 岁的有 4 人，等等。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249456007-08fc1aa0-4325-42d9-93e9-6c6245c3ca5e.png#clientId=u9376670f-d04f-4&from=paste&id=uad09524d&originHeight=282&originWidth=262&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uce66f6f9-221f-4f4e-aa05-ffc75fb3af3&title=)<br />打印结果：用户年龄分布
<a name="hY1za"></a>
#### 1.7.4 获取平均薪资聚合信息
现在来看看平均薪资如何按照所需的格式返回，ES 返回的结果如下图所示，需要获取 balanceAvg 字段的 value 值。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249456109-11b9f526-c629-4172-9a55-ed228fe4808e.png#clientId=u9376670f-d04f-4&from=paste&id=u6bd20846&originHeight=70&originWidth=270&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u729f1257-dfd1-40d3-a4b0-5ba40f5f3ef&title=)<br />ES 返回的平均薪资信息<br />代码实现：
```java
Avg balanceAvg1 = aggregations.get("balanceAvg");
System.out.println("平均薪资：" + balanceAvg1.getValue());
```
打印结果如下，平均薪资 28578 元。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249456280-2cf02f4a-4940-443a-afa3-5789fdba98b1.png#clientId=u9376670f-d04f-4&from=paste&id=udbb58f13&originHeight=87&originWidth=295&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u72278dfe-26a0-4d5d-8813-21220039386&title=)<br />打印结果：平均薪资
<a name="L5En4"></a>
## 三、实战：同步 ES 数据
<a name="aG0ny"></a>
### 3.1 定义检索模型
PassJava 这个项目可以用来配置题库，如果想通过关键字来搜索题库，该怎么做呢？<br />类似于百度搜索，输入几个关键字就可以搜到关联的结果，这个功能也是类似，通过 Elasticsearch 做检索引擎，后台管理界面和小程序作为搜索入口，只需要在小程序上输入关键字，就可以检索相关的题目和答案。<br />首先需要把题目和答案保存到 ES 中，在存之前，第一步是定义索引的模型，如下所示，模型中有 title 和 answer 字段，表示题目和答案。
```json
"id": {
    "type": "long"
},
"title": {
    "type": "text",
    "analyzer": "ik_smart"
},
"answer": {
    "type": "text",
    "analyzer": "ik_smart"
},
"typeName": {
    "type": "keyword"
}
```
<a name="daOqX"></a>
### 3.2 在 ES 中创建索引
上面已经定义了索引结构，接着就是在 ES 中创建索引。<br />在 Kibana 控制台中执行以下语句：
```json
PUT question
{
  "mappings" : {
    "properties": {
      "id": {
        "type": "long"
      },
      "title": {
        "type": "text",
        "analyzer": "ik_smart"
      },
      "answer": {
        "type": "text",
        "analyzer": "ik_smart"
      },
      "typeName": {
        "type": "keyword"
      }
    }
  }
}
```
执行结果如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249456373-3adb4370-985c-47e7-8729-c357a6571140.png#clientId=u9376670f-d04f-4&from=paste&id=u797397a9&originHeight=457&originWidth=884&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1fd08038-fdb7-48e6-8b67-7a03df2167e&title=)<br />创建 question 索引<br />可以通过以下命令来查看 question 索引是否在 ES 中：
```http
GET _cat/indices
```
执行结果如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624249456411-c955990d-a58d-475e-ae51-67789a28b8fe.webp#clientId=u9376670f-d04f-4&from=paste&id=uc228e8ff&originHeight=217&originWidth=757&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u053abc2e-d1a2-4e55-98e2-77a1f235e5c&title=)<br />查看 ES 中所有的索引
<a name="Wd8YN"></a>
### 3.3 定义 ES model
上面定义 ES 的索引，接着就是定义索引对应的模型，将数据存到这个模型中，然后再存到 ES 中。<br />ES 模型如下，共四个字段：id、title、answer、typeName。和 ES 索引是相互对应的。
```java
@Data
public class QuestionEsModel {
    private Long id;
    private String title;
    private String answer;
    private String typeName;
}
```
<a name="oT4Rt"></a>
### 3.4 触发保存的时机
当在后台创建题目或保存题目时，先将数据保存到 mysql 数据库，然后再保存到 ES 中。<br />如下图所示，在管理后台创建题目时，触发保存数据到 ES 。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249456585-67a7aa9f-46f1-4c17-959a-9dfc27269a7c.png#clientId=u9376670f-d04f-4&from=paste&id=u037d9be6&originHeight=646&originWidth=943&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud8609b6e-dc97-44e5-ad77-bb2fd8c0df1&title=)<br />第一步，保存数据到 mysql 中，项目中已经包含此功能，就不再讲解了，直接进入第二步：保存数据到 ES 中。<br />而保存数据到 ES 中，需要将数据组装成 ES 索引对应的数据，所以用了一个 ES model，先将数据保存到 ES model 中。
<a name="HAlp6"></a>
### 3.5 用 model 来组装数据
这里的关键代码时 `copyProperties`，可以将 question 对象的数据取出，然后赋值到 ES model 中。不过 ES model 中还有些字段是 question 中没有的，所以需要单独拎出来赋值，比如 typeName 字段，question 对象中没有这个字段，它对应的字段是 question.type，所以把 type 取出来赋值到 ES model 的 typeName 字段上。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249456754-7eb93d84-f794-4ff9-98a0-213f2344901d.png#clientId=u9376670f-d04f-4&from=paste&id=ua17c6a15&originHeight=363&originWidth=800&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=ue2a28c80-4156-407a-bb39-781cb575393&title=)<br />用 model 来组装数据
<a name="yAb1u"></a>
### 3.6 保存数据到 ES
在 passjava-search 微服务中写了一个保存题目的 api 用来保存数据到 ES 中。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249456843-58705590-81e4-4063-9f0b-9a72cb7fda6e.png#clientId=u9376670f-d04f-4&from=paste&id=u231c6e6c&originHeight=581&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u55b10664-e3c6-4901-87b7-22687724487&title=)<br />保存数据到 ES<br />然后在 passjava-question 微服务中调用 search 微服务的保存 ES 的方法就可以了。
```java
// 调用 passjava-search 服务，将数据发送到 ES 中保存。
searchFeignService.saveQuestion(esModel);
```
<a name="gG3RY"></a>
### 3.7 检验 ES 中是否创建成功
可以通过 kibana 的控制台来查看 question 索引中的文档。通过以下命令来查看：
```http
GET question/_search
```
执行结果如下图所示，有一条记录：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249456870-812f4c12-f6ba-43ba-ba51-21f4f17edc55.png#clientId=u9376670f-d04f-4&from=paste&id=u25a19593&originHeight=709&originWidth=812&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub6f5bdbe-206b-49d9-b8cc-022e2d8be25&title=)<br />另外大家有没有疑问：可以重复更新题目吗？<br />答案是可以的，保存到 ES 的数据是幂等的，因为保存的时候带了一个类似数据库主键的 id。
<a name="n2nHE"></a>
## 四、实战：查询 ES 数据
已经将数据同步到了 ES 中，现在就是前端怎么去查询 ES 数据中，这里还是使用 Postman 来模拟前端查询请求。
<a name="Oa8xD"></a>
### 4.1 定义请求参数
请求参数定义了三个：

- keyword：用来匹配问题或者答案。
- id：用来匹配题目 id。
- pageNum：用来分页查询数据。

这里将这三个参数定义为一个类：
```java
@Data
public class SearchParam {
    private String keyword; // 全文匹配的关键字
    private String id; // 题目 id
    private Integer pageNum; // 查询第几页数据
}
```
<a name="e7BMQ"></a>
### 4.2 定义返回参数
返回的 response 也定义了四个字段：

- questionList：查询到的题目列表。
- pageNum：第几页数据。
- total：查询到的总条数。
- totalPages：总页数。

定义的类如下所示：
```java
@Data
public class SearchQuestionResponse {
    private List<QuestionEsModel> questionList; // 题目列表
    private Integer pageNum; // 查询第几页数据
    private Long total; // 总条数
    private Integer totalPages; // 总页数
}
```
<a name="Jq3kr"></a>
### 4.3 组装 ES 查询参数
调用 ES 的查询 API 时，需要构建查询参数。<br />组装查询参数的核心代码如下所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249457008-9ace20ac-fa64-4110-9c57-ebf205e74aa2.png#clientId=u9376670f-d04f-4&from=paste&id=uafa33752&originHeight=721&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=u4bf18915-de38-4a1f-b721-5f45b2e674f&title=)<br />组装查询参数

- 第一步：创建检索请求。
- 第二步：设置哪些字段需要模糊匹配。这里有三个字段：title，answer，typeName。
- 第三步：设置如何分页。这里分页大小是 5 个。
- 第四步：调用查询 api。
<a name="bvQZJ"></a>
### 4.4 格式化 ES 返回结果
ES 返回的数据是 ES 定义的格式，真正的数据被嵌套在 ES 的 response 中，所以需要格式化返回的数据。<br />核心代码如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249457231-a7c8e7c6-c176-4e1b-bb2e-abb7b1c60248.png#clientId=u9376670f-d04f-4&from=paste&id=u40916e5f&originHeight=762&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uc7914e14-f947-4eb2-83cd-b037c5ca298&title=)<br />格式化 ES 返回结果

- 第一步：获取查到的数据。
- 第二步：获取真正命中的结果。
- 第三步：格式化返回的数据。
- 第四步：组装分页参数。
<a name="x6HJt"></a>
### 4.5 测试 ES 查询
<a name="rPlL3"></a>
#### 4.5.1 实验一：测试 title 匹配
现在想要验证 title 字段是否能匹配到，传的请求参数 keyword = 111，匹配到了 title = 111 的数据，且只有一条。页码 pageNum 传的 1，表示返回第一页数据。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249457794-fae3ced7-81a4-4ed7-bbcb-f0d16df0b042.png#clientId=u9376670f-d04f-4&from=paste&id=uc2baaaba&originHeight=788&originWidth=784&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9b62ac37-1a37-4e41-a276-7dc12f0dab5&title=)<br />测试匹配 title
<a name="wkEta"></a>
#### 4.5.2 实验二：测试 answer 匹配
现在想要验证 answer 字段是否能匹配到，传的请求参数 keyword = 测试答案，匹配到了 title =  测试答案的数据，且只有一条，说明查询成功。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1624249457620-f405b0ff-2fba-4526-8d26-13487012ddd7.png#clientId=u9376670f-d04f-4&from=paste&id=u6698ae8c&originHeight=803&originWidth=699&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=uba67a7f1-0b43-4e29-82f3-3b65135dd1c&title=)<br />测试匹配 answer
<a name="WY4SU"></a>
#### 4.5.2 实验三：测试 id 匹配
现在想要匹配题目 id 的话，需要传请求参数 id，而且 id 是精确匹配。另外 id 和 keyword 是取并集，所以不能传 keyword 字段。<br />请求参数 id = 5，返回结果也是 id =5 的数据，说明查询成功。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624249457706-fc7d6cb7-7250-4835-add9-16a0ee7b0c7a.webp#clientId=u9376670f-d04f-4&from=paste&id=u5004c81b&originHeight=795&originWidth=749&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueab7945e-1bce-4d83-8544-6c72b982f41&title=)<br />测试 id 匹配
