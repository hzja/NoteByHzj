Java SpringBoot Elasticsearch
<a name="76WYv"></a>
## 一、ElasticSearch 简介
<a name="Tfkmf"></a>
### 1、简介
ElasticSearch 是一个基于 Lucene 的搜索服务器。它提供了一个分布式多员工能力的全文搜索引擎，基于 RESTful web 接口。Elasticsearch 是用 Java 语言开发的，并作为 Apache 许可条款下的开放源码发布，是一种流行的企业级搜索引擎。<br />ElasticSearch 用于云计算中，能够达到实时搜索，稳定，可靠，快速，安装使用方便。
<a name="9kXJL"></a>
### 2、特性

- 分布式的文档存储引擎
- 分布式的搜索引擎和分析引擎
- 分布式，支持PB级数据
<a name="Nm6bv"></a>
### 3、使用场景

- 搜索领域：如百度、谷歌，全文检索等。
- 门户网站：访问统计、文章点赞、留言评论等。
- 广告推广：记录员工行为数据、消费趋势、员工群体进行定制推广等。
- 信息采集：记录应用的埋点数据、访问日志数据等，方便大数据进行分析。
<a name="hHlH8"></a>
## 二、ElasticSearch 基础概念
<a name="FWn63"></a>
### 1、ElaticSearch 和 DB 的关系
在 Elasticsearch 中，文档归属于一种类型 type，而这些类型存在于索引 index 中，可以列一些简单的不同点，来类比传统关系型数据库：

- Relational DB -> Databases -> Tables -> Rows -> Columns
- Elasticsearch -> Indices -> Types -> Documents -> Fields

Elasticsearch 集群可以包含多个索引 indices，每一个索引可以包含多个类型 types，每一个类型包含多个文档 documents，然后每个文档包含多个字段 Fields。而在 DB 中可以有多个数据库 Databases，每个库中可以有多张表 Tables，没个表中又包含多行Rows，每行包含多列Columns。
<a name="CRQb7"></a>
### 2、索引
<a name="lahPw"></a>
#### 索引基本概念（indices）
索引是含义相同属性的文档集合，是 ElasticSearch 的一个逻辑存储，可以理解为关系型数据库中的数据库，ElasticSearch 可以把索引数据存放到一台服务器上，也可以 sharding 后存到多台服务器上，每个索引有一个或多个分片，每个分片可以有多个副本。
<a name="g1WE1"></a>
#### 索引类型（index_type）
索引可以定义一个或多个类型，文档必须属于一个类型。在 ElasticSearch 中，一个索引对象可以存储多个不同用途的对象，通过索引类型可以区分单个索引中的不同对象，可以理解为关系型数据库中的表。每个索引类型可以有不同的结构，但是不同的索引类型不能为相同的属性设置不同的类型。
<a name="XMUDQ"></a>
### 3、**文档（**document**）**
文档是可以被索引的基本数据单位。存储在 ElasticSearch 中的主要实体叫文档 document，可以理解为关系型数据库中表的一行记录。每个文档由多个字段构成，ElasticSearch 是一个非结构化的数据库，每个文档可以有不同的字段，并且有一个唯一的标识符。
<a name="CGI0z"></a>
### 4、**映射**（mapping）
ElasticSearch 的 Mapping 非常类似于静态语言中的数据类型：声明一个变量为 int 类型的变量，以后这个变量都只能存储 int 类型的数据。同样的，一个 number 类型的 mapping 字段只能存储 number 类型的数据。<br />同语言的数据类型相比，Mapping 还有一些其他的含义，Mapping 不仅告诉 ElasticSearch 一个 Field 中是什么类型的值， 它还告诉 ElasticSearch 如何索引数据以及数据是否能被搜索到。<br />ElaticSearch 默认是动态创建索引和索引类型的 Mapping 的。这就相当于无需定义 Solr 中的 Schema，无需指定各个字段的索引规则就可以索引文件，很方便。但有时方便就代表着不灵活。比如，ElasticSearch 默认一个字段是要做分词的，但有时要搜索匹配整个字段却不行。如有统计工作要记录每个城市出现的次数。对于 name 字段，若记录 new york 文本，ElasticSearch 可能会把它拆分成 new 和 york 这两个词，分别计算这个两个单词的次数，而不是期望的 new york。
<a name="Ph1My"></a>
## 三、SpringBoot 项目引入 ElasticSearch 依赖
下面介绍下 SpringBoot 如何通过 elasticsearch-rest-high-level-client 工具操作 ElasticSearch，这里需要说一下，为什么没有使用 Spring 家族封装的 spring-data-elasticsearch。<br />主要原因是灵活性和更新速度，Spring 将 ElasticSearch 过度封装，让开发者很难跟 ES 的 DSL 查询语句进行关联。再者就是更新速度，ES 的更新速度是非常快，但是 spring-data-elasticsearch 更新速度比较缓慢。<br />由于上面两点，所以选择了官方推出的 Java 客户端 elasticsearch-rest-high-level-client，它的代码写法跟 DSL 语句很相似，懂 ES 查询的使用其上手很快。
<a name="MU2On"></a>
### 1、Maven 引入相关依赖

- [lombok](http://mp.weixin.qq.com/s?__biz=MzU2NjIzNDk5NQ==&mid=2247486527&idx=1&sn=452fb57f768dfe9200b35c1b0b3e97eb&chksm=fcaed623cbd95f352ba0686c148284bcf5adb3ddc2d328141d6a23eca01c29880a9ffc1aa5ff&scene=21#wechat_redirect)：lombok 工具依赖。
- fastjson：用于将 JSON 转换对象的依赖。
- spring-boot-starter-web： SpringBoot 的 Web 依赖。
- elasticsearch：ElasticSearch：依赖，需要和 ES 版本保持一致。
- elasticsearch-rest-high-level-client：用于操作 ES 的 Java 客户端。<br />
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.2.4.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <groupId>club.mydlq</groupId>
    <artifactId>springboot-elasticsearch-example</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>springboot-elasticsearch-example</name>
    <description>Demo project for Spring Boot ElasticSearch</description>
    <properties>
        <java.version>1.8</java.version>
    </properties>
    <dependencies>
        <!--web-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <!--lombok-->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <optional>true</optional>
        </dependency>
        <!--fastjson-->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>fastjson</artifactId>
            <version>1.2.61</version>
        </dependency>
        <!--elasticsearch-->
        <dependency>
            <groupId>org.elasticsearch.client</groupId>
            <artifactId>elasticsearch-rest-high-level-client</artifactId>
            <version>6.5.4</version>
        </dependency>
        <dependency>
            <groupId>org.elasticsearch</groupId>
            <artifactId>elasticsearch</artifactId>
            <version>6.5.4</version>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
</project>
```
<a name="cBEq4"></a>
### 2、ElasticSearch 连接配置
<a name="PerNV"></a>
#### (1)、application.yml 配置文件
为了方便更改连接 ES 的连接配置，所以将配置信息放置于 application.yaml 中：
```yaml
#base
server:
  port: 8080
#spring
spring:
  application:
    name: springboot-elasticsearch-example
#elasticsearch
elasticsearch:
  schema: http
  address: 127.0.0.1:9200
  connectTimeout: 5000
  socketTimeout: 5000
  connectionRequestTimeout: 5000
  maxConnectNum: 100
  maxConnectPerRoute: 100
```
<a name="Ju83Y"></a>
#### (2)、java 连接配置类
这里需要写一个 Java 配置类读取 application 中的配置信息：
```java
import org.apache.http.HttpHost;
import org.elasticsearch.client.RestClient;
import org.elasticsearch.client.RestClientBuilder;
import org.elasticsearch.client.RestHighLevelClient;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import java.util.ArrayList;
import java.util.List;
/**
 * ElasticSearch 配置
 */
@Configuration
public class ElasticSearchConfig {
    /** 协议 */
    @Value("${elasticsearch.schema:http}")
    private String schema;
    /** 集群地址，如果有多个用“,”隔开 */
    @Value("${elasticsearch.address}")
    private String address;
    /** 连接超时时间 */
    @Value("${elasticsearch.connectTimeout:5000}")
    private int connectTimeout;
    /** Socket 连接超时时间 */
    @Value("${elasticsearch.socketTimeout:10000}")
    private int socketTimeout;
    /** 获取连接的超时时间 */
    @Value("${elasticsearch.connectionRequestTimeout:5000}")
    private int connectionRequestTimeout;
    /** 最大连接数 */
    @Value("${elasticsearch.maxConnectNum:100}")
    private int maxConnectNum;
    /** 最大路由连接数 */
    @Value("${elasticsearch.maxConnectPerRoute:100}")
    private int maxConnectPerRoute;
    @Bean
    public RestHighLevelClient restHighLevelClient() {
        // 拆分地址
        List<HttpHost> hostLists = new ArrayList<>();
        String[] hostList = address.split(",");
        for (String addr : hostList) {
            String host = addr.split(":")[0];
            String port = addr.split(":")[1];
            hostLists.add(new HttpHost(host, Integer.parseInt(port), schema));
        }
        // 转换成 HttpHost 数组
        HttpHost[] httpHost = hostLists.toArray(new HttpHost[]{});
        // 构建连接对象
        RestClientBuilder builder = RestClient.builder(httpHost);
        // 异步连接延时配置
        builder.setRequestConfigCallback(requestConfigBuilder -> {
            requestConfigBuilder.setConnectTimeout(connectTimeout);
            requestConfigBuilder.setSocketTimeout(socketTimeout);
            requestConfigBuilder.setConnectionRequestTimeout(connectionRequestTimeout);
            return requestConfigBuilder;
        });
        // 异步连接数配置
        builder.setHttpClientConfigCallback(httpClientBuilder -> {
            httpClientBuilder.setMaxConnTotal(maxConnectNum);
            httpClientBuilder.setMaxConnPerRoute(maxConnectPerRoute);
            return httpClientBuilder;
        });
        return new RestHighLevelClient(builder);
    }
}
```
<a name="OORcT"></a>
## 四、索引操作示例
这里示例会指出通过 Kibana 的 [Restful](http://mp.weixin.qq.com/s?__biz=MzU2NjIzNDk5NQ==&mid=2247488306&idx=2&sn=3db9ead02378a1b622bcc5e4411c6fa7&chksm=fcaec92ecbd94038c57b1e056a97226cbba4579535fc31a43584dbad3a4399ec1e1e4a65426b&scene=21#wechat_redirect) 工具操作与对应的 Java 代码操作的两个示例。扩展:[某小公司RESTful、共用接口、前后端分离、接口约定的实践](http://mp.weixin.qq.com/s?__biz=MzU2NjIzNDk5NQ==&mid=2247494589&idx=1&sn=e8feb5c29526daf96a0cd3ff5d9a1c36&chksm=fcad31a1cbdab8b7800875d79e3271e5a1b74535ff6b236b183962b1d647c86168fe888d1734&scene=21#wechat_redirect)
<a name="Ap43w"></a>
### 1、Restful 操作示例
<a name="uR3E3"></a>
#### 创建索引
创建名为 mydlq-user 的索引与对应 Mapping。
```json
PUT /mydlq-user
{
  "mappings": {
    "doc": {
      "dynamic": true,
      "properties": {
        "name": {
          "type": "text",
          "fields": {
            "keyword": {
              "type": "keyword"
            }
          }
        },
        "address": {
          "type": "text",
          "fields": {
            "keyword": {
              "type": "keyword"
            }
          }
        },
        "remark": {
          "type": "text",
          "fields": {
            "keyword": {
              "type": "keyword"
            }
          }
        },
        "age": {
          "type": "integer"
        },
        "salary": {
          "type": "float"
        },
        "birthDate": {
          "type": "date",
          "format": "yyyy-MM-dd"
        },
        "createTime": {
          "type": "date"
        }
      }
    }
  }
}
```
<a name="dkjgz"></a>
#### 删除索引
删除 mydlq-user 索引。
```http
DELETE /mydlq-user
```
<a name="objF6"></a>
### 2、Java 代码示例
```java
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.admin.indices.create.CreateIndexRequest;
import org.elasticsearch.action.admin.indices.create.CreateIndexResponse;
import org.elasticsearch.action.admin.indices.delete.DeleteIndexRequest;
import org.elasticsearch.action.support.master.AcknowledgedResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.elasticsearch.common.xcontent.XContentFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
@Slf4j
@Service
public class IndexService2 {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * 创建索引
     */
    public void createIndex() {
        try {
            // 创建 Mapping
            XContentBuilder mapping = XContentFactory.jsonBuilder()
                .startObject()
                    .field("dynamic", true)
                    .startObject("properties")
                        .startObject("name")
                            .field("type","text")
                            .startObject("fields")
                                .startObject("keyword")
                                    .field("type","keyword")
                                .endObject()
                            .endObject()
                        .endObject()
                        .startObject("address")
                            .field("type","text")
                            .startObject("fields")
                                .startObject("keyword")
                                    .field("type","keyword")
                                .endObject()
                            .endObject()
                        .endObject()
                        .startObject("remark")
                            .field("type","text")
                            .startObject("fields")
                                .startObject("keyword")
                                    .field("type","keyword")
                                .endObject()
                            .endObject()
                        .endObject()
                        .startObject("age")
                            .field("type","integer")
                        .endObject()
                        .startObject("salary")
                            .field("type","float")
                        .endObject()
                        .startObject("birthDate")
                            .field("type","date")
                            .field("format", "yyyy-MM-dd")
                        .endObject()
                        .startObject("createTime")
                            .field("type","date")
                        .endObject()
                    .endObject()
                .endObject();
            // 创建索引配置信息，配置
            Settings settings = Settings.builder()
                    .put("index.number_of_shards", 1)
                    .put("index.number_of_replicas", 0)
                    .build();
            // 新建创建索引请求对象，然后设置索引类型（ES 7.0 将不存在索引类型）和 mapping 与 index 配置
            CreateIndexRequest request = new CreateIndexRequest("mydlq-user", settings);
            request.mapping("doc", mapping);
            // RestHighLevelClient 执行创建索引
            CreateIndexResponse createIndexResponse = restHighLevelClient.indices().create(request, RequestOptions.DEFAULT);
            // 判断是否创建成功
            boolean isCreated = createIndexResponse.isAcknowledged();
            log.info("是否创建成功：{}", isCreated);
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 删除索引
     */
    public void deleteIndex() {
        try {
            // 新建删除索引请求对象
            DeleteIndexRequest request = new DeleteIndexRequest("mydlq-user");
            // 执行删除索引
            AcknowledgedResponse acknowledgedResponse = restHighLevelClient.indices().delete(request, RequestOptions.DEFAULT);
            // 判断是否删除成功
            boolean siDeleted = acknowledgedResponse.isAcknowledged();
            log.info("是否删除成功：{}", siDeleted);
        } catch (IOException e) {
            log.error("", e);
        }
    }
}
```
<a name="t5k3b"></a>
## 五、文档操作示例
<a name="vyDZR"></a>
### 1、Restful 操作示例
<a name="CgtS5"></a>
#### 增加文档信息
在索引 mydlq-user 中增加一条文档信息。
```
POST /mydlq-user/doc
{
    "address": "北京市",
    "age": 29,
    "birthDate": "1990-01-10",
    "createTime": 1579530727699,
    "name": "张三",
    "remark": "来自北京市的张先生",
    "salary": 100
}
```
<a name="mulcU"></a>
#### 获取文档信息
获取 mydlq-user 的索引 id=1 的文档信息。
```
GET /mydlq-user/doc/1
```
<a name="Xqpy4"></a>
#### 更新文档信息
更新之前创建的 id=1 的文档信息。
```
PUT /mydlq-user/doc/1
{
    "address": "北京市海淀区",
    "age": 29,
    "birthDate": "1990-01-10",
    "createTime": 1579530727699,
    "name": "张三",
    "remark": "来自北京市的张先生",
    "salary": 100
}
```
<a name="q6wgu"></a>
#### 删除文档信息
删除之前创建的 id=1 的文档信息。
```
DELETE /mydlq-user/doc/1
```
<a name="aB7CU"></a>
### 2、Java 代码示例
```java
import club.mydlq.elasticsearch.model.entity.UserInfo;
import com.alibaba.fastjson.JSON;
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.delete.DeleteRequest;
import org.elasticsearch.action.delete.DeleteResponse;
import org.elasticsearch.action.get.GetRequest;
import org.elasticsearch.action.get.GetResponse;
import org.elasticsearch.action.index.IndexRequest;
import org.elasticsearch.action.index.IndexResponse;
import org.elasticsearch.action.update.UpdateRequest;
import org.elasticsearch.action.update.UpdateResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.common.xcontent.XContentType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
import java.util.Date;
@Slf4j
@Service
public class IndexService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * 增加文档信息
     */
    public void addDocument() {
        try {
            // 创建索引请求对象
            IndexRequest indexRequest = new IndexRequest("mydlq-user", "doc", "1");
            // 创建员工信息
            UserInfo userInfo = new UserInfo();
            userInfo.setName("张三");
            userInfo.setAge(29);
            userInfo.setSalary(100.00f);
            userInfo.setAddress("北京市");
            userInfo.setRemark("来自北京市的张先生");
            userInfo.setCreateTime(new Date());
            userInfo.setBirthDate("1990-01-10");
            // 将对象转换为 byte 数组
            byte[] json = JSON.toJSONBytes(userInfo);
            // 设置文档内容
            indexRequest.source(json, XContentType.JSON);
            // 执行增加文档
            IndexResponse response = restHighLevelClient.index(indexRequest, RequestOptions.DEFAULT);
            log.info("创建状态：{}", response.status());
        } catch (Exception e) {
            log.error("", e);
        }
    }
    /**
     * 获取文档信息
     */
    public void getDocument() {
        try {
            // 获取请求对象
            GetRequest getRequest = new GetRequest("mydlq-user", "doc", "1");
            // 获取文档信息
            GetResponse getResponse = restHighLevelClient.get(getRequest, RequestOptions.DEFAULT);
            // 将 JSON 转换成对象
            if (getResponse.isExists()) {
                UserInfo userInfo = JSON.parseObject(getResponse.getSourceAsBytes(), UserInfo.class);
                log.info("员工信息：{}", userInfo);
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 更新文档信息
     */
    public void updateDocument() {
        try {
            // 创建索引请求对象
            UpdateRequest updateRequest = new UpdateRequest("mydlq-user", "doc", "1");
            // 设置员工更新信息
            UserInfo userInfo = new UserInfo();
            userInfo.setSalary(200.00f);
            userInfo.setAddress("北京市海淀区");
            // 将对象转换为 byte 数组
            byte[] json = JSON.toJSONBytes(userInfo);
            // 设置更新文档内容
            updateRequest.doc(json, XContentType.JSON);
            // 执行更新文档
            UpdateResponse response = restHighLevelClient.update(updateRequest, RequestOptions.DEFAULT);
            log.info("创建状态：{}", response.status());
        } catch (Exception e) {
            log.error("", e);
        }
    }
    /**
     * 删除文档信息
     */
    public void deleteDocument() {
        try {
            // 创建删除请求对象
            DeleteRequest deleteRequest = new DeleteRequest("mydlq-user", "doc", "1");
            // 执行删除文档
            DeleteResponse response = restHighLevelClient.delete(deleteRequest, RequestOptions.DEFAULT);
            log.info("删除状态：{}", response.status());
        } catch (IOException e) {
            log.error("", e);
        }
    }
}
```
<a name="DL9hR"></a>
## 六、插入初始化数据
执行查询示例前，先往[索引](http://mp.weixin.qq.com/s?__biz=MzU2NjIzNDk5NQ==&mid=2247494598&idx=1&sn=2ce6d04ab64ab6d2d0705d772f43898f&chksm=fcad31dacbdab8ccfe5e64bfca4e546e4de44145d59c64349a00c0c305f56265d9cf8d5bebc8&scene=21#wechat_redirect)中插入一批数据：
<a name="nIteY"></a>
### 1、单条插入
POST mydlq-user/_doc
```json
{"name":"零零","address":"北京市丰台区","remark":"低层员工","age":29,"salary":3000,"birthDate":"1990-11-11","createTime":"2019-11-11T08:18:00.000Z"}
```
<a name="R6gXA"></a>
### 2、批量插入
POST _bulk
```json
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"刘一","address":"北京市丰台区","remark":"低层员工","age":30,"salary":3000,"birthDate":"1989-11-11","createTime":"2019-03-15T08:18:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"陈二","address":"北京市昌平区","remark":"中层员工","age":27,"salary":7900,"birthDate":"1992-01-25","createTime":"2019-11-08T11:15:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"张三","address":"北京市房山区","remark":"中层员工","age":28,"salary":8800,"birthDate":"1991-10-05","createTime":"2019-07-22T13:22:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"李四","address":"北京市大兴区","remark":"高层员工","age":26,"salary":9000,"birthDate":"1993-08-18","createTime":"2019-10-17T15:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"王五","address":"北京市密云区","remark":"低层员工","age":31,"salary":4800,"birthDate":"1988-07-20","createTime":"2019-05-29T09:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"赵六","address":"北京市通州区","remark":"中层员工","age":32,"salary":6500,"birthDate":"1987-06-02","createTime":"2019-12-10T18:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"孙七","address":"北京市朝阳区","remark":"中层员工","age":33,"salary":7000,"birthDate":"1986-04-15","createTime":"2019-06-06T13:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"周八","address":"北京市西城区","remark":"低层员工","age":32,"salary":5000,"birthDate":"1987-09-26","createTime":"2019-01-26T14:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"吴九","address":"北京市海淀区","remark":"高层员工","age":30,"salary":11000,"birthDate":"1989-11-25","createTime":"2019-09-07T13:34:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"郑十","address":"北京市东城区","remark":"低层员工","age":29,"salary":5000,"birthDate":"1990-12-25","createTime":"2019-03-06T12:08:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"萧十一","address":"北京市平谷区","remark":"低层员工","age":29,"salary":3300,"birthDate":"1990-11-11","createTime":"2019-03-10T08:17:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"曹十二","address":"北京市怀柔区","remark":"中层员工","age":27,"salary":6800,"birthDate":"1992-01-25","createTime":"2019-12-03T11:09:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"吴十三","address":"北京市延庆区","remark":"中层员工","age":25,"salary":7000,"birthDate":"1994-10-05","createTime":"2019-07-27T14:22:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"冯十四","address":"北京市密云区","remark":"低层员工","age":25,"salary":3000,"birthDate":"1994-08-18","createTime":"2019-04-22T15:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"蒋十五","address":"北京市通州区","remark":"低层员工","age":31,"salary":2800,"birthDate":"1988-07-20","createTime":"2019-06-13T10:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"苗十六","address":"北京市门头沟区","remark":"高层员工","age":32,"salary":11500,"birthDate":"1987-06-02","createTime":"2019-11-11T18:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"鲁十七","address":"北京市石景山区","remark":"高员工","age":33,"salary":9500,"birthDate":"1986-04-15","createTime":"2019-06-06T14:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"沈十八","address":"北京市朝阳区","remark":"中层员工","age":31,"salary":8300,"birthDate":"1988-09-26","createTime":"2019-09-25T14:00:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"吕十九","address":"北京市西城区","remark":"低层员工","age":31,"salary":4500,"birthDate":"1988-11-25","createTime":"2019-09-22T13:34:00.000Z"}
{"index":{"_index":"mydlq-user","_type":"doc"}}
{"name":"丁二十","address":"北京市东城区","remark":"低层员工","age":33,"salary":2100,"birthDate":"1986-12-25","createTime":"2019-03-07T12:08:00.000Z"}
```
<a name="R0gZB"></a>
### 3、查询数据
插入完成后再查询数据，查看之前插入的数据是否存在：
```http
GET mydlq-user/_search
```
执行后得到下面记录:
```json
{
  "took": 2,
  "timed_out": false,
  "_shards": {
    "total": 1,
    "successful": 1,
    "skipped": 0,
    "failed": 0
  },
  "hits": {
    "total": 20,
    "max_score": 1,
    "hits": [
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "BeN0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "刘一",
          "address": "北京市丰台区",
          "remark": "低层员工",
          "age": 30,
          "salary": 3000,
          "birthDate": "1989-11-11",
          "createTime": "2019-03-15T08:18:00.000Z"
        }
      },
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "BuN0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "陈二",
          "address": "北京市昌平区",
          "remark": "中层员工",
          "age": 27,
          "salary": 7900,
          "birthDate": "1992-01-25",
          "createTime": "2019-11-08T11:15:00.000Z"
        }
      },
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "B-N0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "张三",
          "address": "北京市房山区",
          "remark": "中层员工",
          "age": 28,
          "salary": 8800,
          "birthDate": "1991-10-05",
          "createTime": "2019-07-22T13:22:00.000Z"
        }
      },
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "CON0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "李四",
          "address": "北京市大兴区",
          "remark": "高层员工",
          "age": 26,
          "salary": 9000,
          "birthDate": "1993-08-18",
          "createTime": "2019-10-17T15:00:00.000Z"
        }
      },
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "CeN0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "王五",
          "address": "北京市密云区",
          "remark": "低层员工",
          "age": 31,
          "salary": 4800,
          "birthDate": "1988-07-20",
          "createTime": "2019-05-29T09:00:00.000Z"
        }
      },
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "CuN0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "赵六",
          "address": "北京市通州区",
          "remark": "中层员工",
          "age": 32,
          "salary": 6500,
          "birthDate": "1987-06-02",
          "createTime": "2019-12-10T18:00:00.000Z"
        }
      },
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "C-N0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "孙七",
          "address": "北京市朝阳区",
          "remark": "中层员工",
          "age": 33,
          "salary": 7000,
          "birthDate": "1986-04-15",
          "createTime": "2019-06-06T13:00:00.000Z"
        }
      },
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "DON0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "周八",
          "address": "北京市西城区",
          "remark": "低层员工",
          "age": 32,
          "salary": 5000,
          "birthDate": "1987-09-26",
          "createTime": "2019-01-26T14:00:00.000Z"
        }
      },
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "DeN0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "吴九",
          "address": "北京市海淀区",
          "remark": "高层员工",
          "age": 30,
          "salary": 11000,
          "birthDate": "1989-11-25",
          "createTime": "2019-09-07T13:34:00.000Z"
        }
      },
      {
        "_index": "mydlq-user",
        "_type": "_doc",
        "_id": "DuN0BW8B7BNodGwRFTRj",
        "_score": 1,
        "_source": {
          "name": "郑十",
          "address": "北京市东城区",
          "remark": "低层员工",
          "age": 29,
          "salary": 5000,
          "birthDate": "1990-12-25",
          "createTime": "2019-03-06T12:08:00.000Z"
        }
      }
    ]
  }
}
```
<a name="SHWIU"></a>
## 七、查询操作示例
<a name="LbFmb"></a>
### 1、精确查询(term)
<a name="N9lJu"></a>
#### (1)、Restful 操作示例
**精确查询**<br />精确查询，查询地址为 北京市通州区 的人员信息：<br />查询条件不会进行分词，但是查询内容可能会分词，导致查询不到。之前在创建索引时设置 Mapping 中 address 字段存在 keyword 字段是专门用于不分词查询的子字段。
```
GET mydlq-user/_search
{
  "query": {
    "term": {
      "address.keyword": {
        "value": "北京市通州区"
      }
    }
  }
}
```
**精确查询-多内容查询**<br />精确查询，查询地址为 北京市丰台区、北京市昌平区 或 北京市大兴区 的人员信息：
```
GET mydlq-user/_search
{
  "query": {
    "terms": {
      "address.keyword": [
        "北京市丰台区",
        "北京市昌平区",
        "北京市大兴区"
      ]
    }
  }
}
```
<a name="YwS8G"></a>
#### (2)、Java 代码示例
```java
import club.mydlq.elasticsearch.model.entity.UserInfo;
import com.alibaba.fastjson.JSON;
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
@Slf4j
@Service
public class TermQueryService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * 精确查询（查询条件不会进行分词，但是查询内容可能会分词，导致查询不到）
     */
    public void termQuery() {
        try {
            // 构建查询条件（注意：termQuery 支持多种格式查询，如 boolean、int、double、string 等，这里使用的是 string 的查询）
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.termQuery("address.keyword", "北京市通州区"));
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 多个内容在一个字段中进行查询
     */
    public void termsQuery() {
        try {
            // 构建查询条件（注意：termsQuery 支持多种格式查询，如 boolean、int、double、string 等，这里使用的是 string 的查询）
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.termsQuery("address.keyword", "北京市丰台区", "北京市昌平区", "北京市大兴区"));
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
}
```
<a name="opsVe"></a>
### 2、匹配查询(match)
<a name="eNLXQ"></a>
#### (1)、Restful 操作示例
**匹配查询全部数据与分页**<br />匹配查询符合条件的所有数据，并且设置以 salary 字段升序排序，并设置分页：
```
GET mydlq-user/_search
{
  "query": {
    "match_all": {}
  },
  "from": 0,
  "size": 10,
  "sort": [
    {
      "salary": {
        "order": "asc"
      }
    }
  ]
}
```
**匹配查询数据**<br />匹配查询地址为 通州区 的数据：
```
GET mydlq-user/_search
{
  "query": {
    "match": {
      "address": "通州区"
    }
  }
}
```
**词语匹配查询**<br />词语匹配进行查询，匹配 address 中为 北京市通州区 的员工信息：
```
GET mydlq-user/_search
{
  "query": {
    "match_phrase": {
      "address": "北京市通州区"
    }
  }
}
```
**内容多字段查询**<br />查询在字段 address、remark 中存在 北京 内容的员工信息：
```
GET mydlq-user/_search
{
  "query": {
    "multi_match": {
      "query": "北京",
      "fields": ["address","remark"]
    }
  }
}
```
<a name="C3Nl2"></a>
#### (2)、Java 代码示例
```java
import club.mydlq.elasticsearch.model.entity.UserInfo;
import com.alibaba.fastjson.JSON;
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.index.query.MatchAllQueryBuilder;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.elasticsearch.search.sort.SortOrder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
@Slf4j
@Service
public class MatchQueryService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * 匹配查询符合条件的所有数据，并设置分页
     */
    public Object matchAllQuery() {
        try {
            // 构建查询条件
            MatchAllQueryBuilder matchAllQueryBuilder = QueryBuilders.matchAllQuery();
            // 创建查询源构造器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(matchAllQueryBuilder);
            // 设置分页
            searchSourceBuilder.from(0);
            searchSourceBuilder.size(3);
            // 设置排序
            searchSourceBuilder.sort("salary", SortOrder.ASC);
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 匹配查询数据
     */
    public Object matchQuery() {
        try {
            // 构建查询条件
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.matchQuery("address", "*通州区"));
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 词语匹配查询
     */
    public Object matchPhraseQuery() {
        try {
            // 构建查询条件
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.matchPhraseQuery("address", "北京市通州区"));
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 内容在多字段中进行查询
     */
    public Object matchMultiQuery() {
        try {
            // 构建查询条件
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.multiMatchQuery("北京市", "address", "remark"));
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
}
```
<a name="ei803"></a>
### 3、模糊查询(fuzzy)
<a name="DJ06z"></a>
#### (1)、Restful 操作示例
模糊查询所有以 三 结尾的姓名
```
GET mydlq-user/_search
{
  "query": {
    "fuzzy": {
      "name": "三"
    }
  }
}
```
<a name="cmQLj"></a>
#### (2)、Java 代码示例
```java
import club.mydlq.elasticsearch.model.entity.UserInfo;
import com.alibaba.fastjson.JSON;
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.common.unit.Fuzziness;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
@Slf4j
@Service
public class FuzzyQueryService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * 模糊查询所有以 “三” 结尾的姓名
     */
    public Object fuzzyQuery() {
        try {
            // 构建查询条件
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.fuzzyQuery("name", "三").fuzziness(Fuzziness.AUTO));
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
}
```
<a name="1reLb"></a>
### 4、范围查询(range)
<a name="o3PQz"></a>
#### (1)、Restful 操作示例
查询岁数 ≥ 30 岁的员工数据：
```
GET /mydlq-user/_search
{
  "query": {
    "range": {
      "age": {
        "gte": 30
      }
    }
  }
}
```
查询生日距离现在 30 年间的员工数据：
```
GET mydlq-user/_search
{
  "query": {
    "range": {
      "birthDate": {
        "gte": "now-30y"
      }
    }
  }
}
```
<a name="wLiCZ"></a>
#### (2)、Java 代码示例
```java
import club.mydlq.elasticsearch.model.entity.UserInfo;
import com.alibaba.fastjson.JSON;
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
@Slf4j
@Service
public class RangeQueryService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * 查询岁数 ≥ 30 岁的员工数据
     */
    public void rangeQuery() {
        try {
            // 构建查询条件
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.rangeQuery("age").gte(30));
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 查询距离现在 30 年间的员工数据
     * [年(y)、月(M)、星期(w)、天(d)、小时(h)、分钟(m)、秒(s)]
     * 例如：
     * now-1h 查询一小时内范围
     * now-1d 查询一天内时间范围
     * now-1y 查询最近一年内的时间范围
     */
    public void dateRangeQuery() {
        try {
            // 构建查询条件
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            // includeLower（是否包含下边界）、includeUpper（是否包含上边界）
            searchSourceBuilder.query(QueryBuilders.rangeQuery("birthDate")
                    .gte("now-30y").includeLower(true).includeUpper(true));
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
}
```
<a name="Lj5XJ"></a>
### 5、通配符查询(wildcard)
<a name="VREKp"></a>
#### (1)、Restful 操作示例
查询所有以 “三” 结尾的姓名：
```
GET mydlq-user/_search
{
  "query": {
    "wildcard": {
      "name.keyword": {
        "value": "*三"
      }
    }
  }
}
```
<a name="q5bhw"></a>
#### (2)、Java 代码示例
```java
import club.mydlq.elasticsearch.model.entity.UserInfo;
import com.alibaba.fastjson.JSON;
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
@Slf4j
@Service
public class WildcardQueryService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * 查询所有以 “三” 结尾的姓名
     *
     * *：表示多个字符（0个或多个字符）
     * ?：表示单个字符
     */
    public Object wildcardQuery() {
        try {
            // 构建查询条件
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.wildcardQuery("name.keyword", "*三"));
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
}
```
<a name="HfvXq"></a>
### 6、布尔查询(bool)
<a name="v7lwY"></a>
#### (1)、Restful 操作示例
查询出生在 1990-1995 年期间，且地址在 北京市昌平区、北京市大兴区、北京市房山区 的员工信息：
```
GET /mydlq-user/_search
{
  "query": {
    "bool": {
      "filter": {
        "range": {
          "birthDate": {
            "format": "yyyy", 
            "gte": 1990,
            "lte": 1995
          }
        }
      },
      "must": [
        {
          "terms": {
            "address.keyword": [
              "北京市昌平区",
              "北京市大兴区",
              "北京市房山区"
            ]
          }
        }
      ]
    }
  }
}
```
<a name="gVLKk"></a>
#### (2)、Java 代码示例
```java
import club.mydlq.elasticsearch.model.entity.UserInfo;
import com.alibaba.fastjson.JSON;
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.index.query.BoolQueryBuilder;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
@Slf4j
@Service
public class BoolQueryService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    public Object boolQuery() {
        try {
            // 创建 Bool 查询构建器
            BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();
            // 构建查询条件
            boolQueryBuilder.must(QueryBuilders.termsQuery("address.keyword", "北京市昌平区", "北京市大兴区", "北京市房山区"))
                    .filter().add(QueryBuilders.rangeQuery("birthDate").format("yyyy").gte("1990").lte("1995"));
            // 构建查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(boolQueryBuilder);
            // 创建查询请求对象，将查询对象配置到其中
            SearchRequest searchRequest = new SearchRequest("mydlq-user");
            searchRequest.source(searchSourceBuilder);
            // 执行查询，然后处理响应结果
            SearchResponse searchResponse = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
            // 根据状态和数据条数验证是否返回了数据
            if (RestStatus.OK.equals(searchResponse.status()) && searchResponse.getHits().totalHits > 0) {
                SearchHits hits = searchResponse.getHits();
                for (SearchHit hit : hits) {
                    // 将 JSON 转换成对象
                    UserInfo userInfo = JSON.parseObject(hit.getSourceAsString(), UserInfo.class);
                    // 输出查询信息
                    log.info(userInfo.toString());
                }
            }
        }catch (IOException e){
            log.error("",e);
        }
    }
}
```
<a name="vJ07L"></a>
## 八、聚合查询操作示例
<a name="DKgMq"></a>
### 1、Metric 聚合分析
<a name="veOmq"></a>
#### (1)、Restful 操作示例
统计员工总数、工资最高值、工资最低值、工资平均工资、工资总和：
```
GET /mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "salary_stats": {
      "stats": {
        "field": "salary"
      }
    }
  }
}
```
统计员工工资最低值：
```
GET /mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "salary_min": {
      "min": {
        "field": "salary"
      }
    }
  }
}
```
统计员工工资最高值：
```
GET /mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "salary_max": {
      "max": {
        "field": "salary"
      }
    }
  }
}
```
统计员工工资平均值：
```
GET /mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "salary_avg": {
      "avg": {
        "field": "salary"
      }
    }
  }
}
```
统计员工工资总值：
```
GET /mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "salary_sum": {
      "sum": {
        "field": "salary"
      }
    }
  }
}
```
统计员工总数：
```
GET /mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "employee_count": {
      "value_count": {
        "field": "salary"
      }
    }
  }
}
```
统计员工工资百分位：
```
GET /mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "salary_percentiles": {
      "percentiles": {
        "field": "salary"
      }
    }
  }
}
```
<a name="bMh8w"></a>
#### (2)、Java 代码示例
```java
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.search.aggregations.AggregationBuilder;
import org.elasticsearch.search.aggregations.AggregationBuilders;
import org.elasticsearch.search.aggregations.Aggregations;
import org.elasticsearch.search.aggregations.metrics.avg.ParsedAvg;
import org.elasticsearch.search.aggregations.metrics.max.ParsedMax;
import org.elasticsearch.search.aggregations.metrics.min.ParsedMin;
import org.elasticsearch.search.aggregations.metrics.percentiles.ParsedPercentiles;
import org.elasticsearch.search.aggregations.metrics.percentiles.Percentile;
import org.elasticsearch.search.aggregations.metrics.stats.ParsedStats;
import org.elasticsearch.search.aggregations.metrics.sum.ParsedSum;
import org.elasticsearch.search.aggregations.metrics.sum.SumAggregationBuilder;
import org.elasticsearch.search.aggregations.metrics.valuecount.ParsedValueCount;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
@Slf4j
@Service
public class AggrMetricService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * stats 统计员工总数、员工工资最高值、员工工资最低值、员工平均工资、员工工资总和
     */
    public Object aggregationStats() {
        String responseResult = "";
        try {
            // 设置聚合条件
            AggregationBuilder aggr = AggregationBuilders.stats("salary_stats").field("salary");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.aggregation(aggr);
            // 设置查询结果不返回，只返回聚合结果
            searchSourceBuilder.size(0);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status()) || aggregations != null) {
                // 转换为 Stats 对象
                ParsedStats aggregation = aggregations.get("salary_stats");
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                log.info("count：{}", aggregation.getCount());
                log.info("avg：{}", aggregation.getAvg());
                log.info("max：{}", aggregation.getMax());
                log.info("min：{}", aggregation.getMin());
                log.info("sum：{}", aggregation.getSum());
                log.info("-------------------------------------------");
            }
            // 根据具体业务逻辑返回不同结果，这里为了方便直接将返回响应对象Json串
            responseResult = response.toString();
        } catch (IOException e) {
            log.error("", e);
        }
        return responseResult;
    }
    /**
     * min 统计员工工资最低值
     */
    public Object aggregationMin() {
        String responseResult = "";
        try {
            // 设置聚合条件
            AggregationBuilder aggr = AggregationBuilders.min("salary_min").field("salary");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.aggregation(aggr);
            searchSourceBuilder.size(0);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status()) || aggregations != null) {
                // 转换为 Min 对象
                ParsedMin aggregation = aggregations.get("salary_min");
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                log.info("min：{}", aggregation.getValue());
                log.info("-------------------------------------------");
            }
            // 根据具体业务逻辑返回不同结果，这里为了方便直接将返回响应对象Json串
            responseResult = response.toString();
        } catch (IOException e) {
            log.error("", e);
        }
        return responseResult;
    }
    /**
     * max 统计员工工资最高值
     */
    public Object aggregationMax() {
        String responseResult = "";
        try {
            // 设置聚合条件
            AggregationBuilder aggr = AggregationBuilders.max("salary_max").field("salary");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.aggregation(aggr);
            searchSourceBuilder.size(0);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status()) || aggregations != null) {
                // 转换为 Max 对象
                ParsedMax aggregation = aggregations.get("salary_max");
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                log.info("max：{}", aggregation.getValue());
                log.info("-------------------------------------------");
            }
            // 根据具体业务逻辑返回不同结果，这里为了方便直接将返回响应对象Json串
            responseResult = response.toString();
        } catch (IOException e) {
            log.error("", e);
        }
        return responseResult;
    }
    /**
     * avg 统计员工工资平均值
     */
    public Object aggregationAvg() {
        String responseResult = "";
        try {
            // 设置聚合条件
            AggregationBuilder aggr = AggregationBuilders.avg("salary_avg").field("salary");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.aggregation(aggr);
            searchSourceBuilder.size(0);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status()) || aggregations != null) {
                // 转换为 Avg 对象
                ParsedAvg aggregation = aggregations.get("salary_avg");
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                log.info("avg：{}", aggregation.getValue());
                log.info("-------------------------------------------");
            }
            // 根据具体业务逻辑返回不同结果，这里为了方便直接将返回响应对象Json串
            responseResult = response.toString();
        } catch (IOException e) {
            log.error("", e);
        }
        return responseResult;
    }
    /**
     * sum 统计员工工资总值
     */
    public Object aggregationSum() {
        String responseResult = "";
        try {
            // 设置聚合条件
            SumAggregationBuilder aggr = AggregationBuilders.sum("salary_sum").field("salary");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.aggregation(aggr);
            searchSourceBuilder.size(0);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status()) || aggregations != null) {
                // 转换为 Sum 对象
                ParsedSum aggregation = aggregations.get("salary_sum");
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                log.info("sum：{}", String.valueOf((aggregation.getValue())));
                log.info("-------------------------------------------");
            }
            // 根据具体业务逻辑返回不同结果，这里为了方便直接将返回响应对象Json串
            responseResult = response.toString();
        } catch (IOException e) {
            log.error("", e);
        }
        return responseResult;
    }
    /**
     * count 统计员工总数
     */
    public Object aggregationCount() {
        String responseResult = "";
        try {
            // 设置聚合条件
            AggregationBuilder aggr = AggregationBuilders.count("employee_count").field("salary");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.aggregation(aggr);
            searchSourceBuilder.size(0);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status()) || aggregations != null) {
                // 转换为 ValueCount 对象
                ParsedValueCount aggregation = aggregations.get("employee_count");
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                log.info("count：{}", aggregation.getValue());
                log.info("-------------------------------------------");
            }
            // 根据具体业务逻辑返回不同结果，这里为了方便直接将返回响应对象Json串
            responseResult = response.toString();
        } catch (IOException e) {
            log.error("", e);
        }
        return responseResult;
    }
    /**
     * percentiles 统计员工工资百分位
     */
    public Object aggregationPercentiles() {
        String responseResult = "";
        try {
            // 设置聚合条件
            AggregationBuilder aggr = AggregationBuilders.percentiles("salary_percentiles").field("salary");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.aggregation(aggr);
            searchSourceBuilder.size(0);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status()) || aggregations != null) {
                // 转换为 Percentiles 对象
                ParsedPercentiles aggregation = aggregations.get("salary_percentiles");
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                for (Percentile percentile : aggregation) {
                    log.info("百分位：{}：{}", percentile.getPercent(), percentile.getValue());
                }
                log.info("-------------------------------------------");
            }
            // 根据具体业务逻辑返回不同结果，这里为了方便直接将返回响应对象Json串
            responseResult = response.toString();
        } catch (IOException e) {
            log.error("", e);
        }
        return responseResult;
    }
}
```
<a name="Sz1gi"></a>
### 2、Bucket 聚合分析
<a name="iwLDM"></a>
#### (1)、Restful 操作示例
按岁数进行聚合分桶，统计各个岁数员工的人数：
```
GET mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "age_bucket": {
      "terms": {
        "field": "age",
        "size": "10"
      }
    }
  }
}
```
按工资范围进行聚合分桶，统计工资在 3000-5000、5000-9000 和 9000 以上的员工信息：
```
GET mydlq-user/_search
{
  "aggs": {
    "salary_range_bucket": {
      "range": {
        "field": "salary",
        "ranges": [
          {
            "key": "低级员工", 
            "to": 3000
          },{
            "key": "中级员工",
            "from": 5000,
            "to": 9000
          },{
            "key": "高级员工",
            "from": 9000
          }
        ]
      }
    }
  }
}
```
按照时间范围进行分桶，统计 1985-1990 年和 1990-1995 年出生的员工信息：
```
GET mydlq-user/_search
{
  "size": 10,
  "aggs": {
    "date_range_bucket": {
      "date_range": {
        "field": "birthDate",
        "format": "yyyy", 
        "ranges": [
          {
            "key": "出生日期1985-1990的员工", 
            "from": "1985",
            "to": "1990"
          },{
            "key": "出生日期1990-1995的员工", 
            "from": "1990",
            "to": "1995"
          }
        ]
      }
    }
  }
}
```
按工资多少进行聚合分桶，设置统计的最小值为 0，最大值为 12000，区段间隔为 3000：
```
GET mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "salary_histogram": {
      "histogram": {
        "field": "salary",
        "extended_bounds": {
          "min": 0,
          "max": 12000
        }, 
        "interval": 3000
      }
    }
  }
}
```
按出生日期进行分桶：
```
GET mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "birthday_histogram": {
      "date_histogram": {
        "format": "yyyy", 
        "field": "birthDate",
        "interval": "year"
      }
    }
  }
}
```
<a name="zoOMu"></a>
#### (2)、Java 代码示例
```java
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.search.aggregations.AggregationBuilder;
import org.elasticsearch.search.aggregations.AggregationBuilders;
import org.elasticsearch.search.aggregations.Aggregations;
import org.elasticsearch.search.aggregations.bucket.histogram.DateHistogramInterval;
import org.elasticsearch.search.aggregations.bucket.histogram.Histogram;
import org.elasticsearch.search.aggregations.bucket.range.Range;
import org.elasticsearch.search.aggregations.bucket.terms.Terms;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
import java.util.List;
@Slf4j
@Service
public class AggrBucketService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * 按岁数进行聚合分桶
     */
    public Object aggrBucketTerms() {
        try {
            AggregationBuilder aggr = AggregationBuilders.terms("age_bucket").field("age");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.size(10);
            searchSourceBuilder.aggregation(aggr);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status())) {
                // 分桶
                Terms byCompanyAggregation = aggregations.get("age_bucket");
                List<? extends Terms.Bucket> buckets = byCompanyAggregation.getBuckets();
                // 输出各个桶的内容
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                for (Terms.Bucket bucket : buckets) {
                    log.info("桶名：{} | 总数：{}", bucket.getKeyAsString(), bucket.getDocCount());
                }
                log.info("-------------------------------------------");
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 按工资范围进行聚合分桶
     */
    public Object aggrBucketRange() {
        try {
            AggregationBuilder aggr = AggregationBuilders.range("salary_range_bucket")
                    .field("salary")
                    .addUnboundedTo("低级员工", 3000)
                    .addRange("中级员工", 5000, 9000)
                    .addUnboundedFrom("高级员工", 9000);
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.size(0);
            searchSourceBuilder.aggregation(aggr);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status())) {
                // 分桶
                Range byCompanyAggregation = aggregations.get("salary_range_bucket");
                List<? extends Range.Bucket> buckets = byCompanyAggregation.getBuckets();
                // 输出各个桶的内容
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                for (Range.Bucket bucket : buckets) {
                    log.info("桶名：{} | 总数：{}", bucket.getKeyAsString(), bucket.getDocCount());
                }
                log.info("-------------------------------------------");
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 按照时间范围进行分桶
     */
    public Object aggrBucketDateRange() {
        try {
            AggregationBuilder aggr = AggregationBuilders.dateRange("date_range_bucket")
                    .field("birthDate")
                    .format("yyyy")
                    .addRange("1985-1990", "1985", "1990")
                    .addRange("1990-1995", "1990", "1995");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.size(0);
            searchSourceBuilder.aggregation(aggr);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status())) {
                // 分桶
                Range byCompanyAggregation = aggregations.get("date_range_bucket");
                List<? extends Range.Bucket> buckets = byCompanyAggregation.getBuckets();
                // 输出各个桶的内容
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                for (Range.Bucket bucket : buckets) {
                    log.info("桶名：{} | 总数：{}", bucket.getKeyAsString(), bucket.getDocCount());
                }
                log.info("-------------------------------------------");
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 按工资多少进行聚合分桶
     */
    public Object aggrBucketHistogram() {
        try {
            AggregationBuilder aggr = AggregationBuilders.histogram("salary_histogram")
                    .field("salary")
                    .extendedBounds(0, 12000)
                    .interval(3000);
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.size(0);
            searchSourceBuilder.aggregation(aggr);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status())) {
                // 分桶
                Histogram byCompanyAggregation = aggregations.get("salary_histogram");
                List<? extends Histogram.Bucket> buckets = byCompanyAggregation.getBuckets();
                // 输出各个桶的内容
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                for (Histogram.Bucket bucket : buckets) {
                    log.info("桶名：{} | 总数：{}", bucket.getKeyAsString(), bucket.getDocCount());
                }
                log.info("-------------------------------------------");
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
    /**
     * 按出生日期进行分桶
     */
    public Object aggrBucketDateHistogram() {
        try {
            AggregationBuilder aggr = AggregationBuilders.dateHistogram("birthday_histogram")
                    .field("birthDate")
                    .interval(1)
                    .dateHistogramInterval(DateHistogramInterval.YEAR)
                    .format("yyyy");
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.size(0);
            searchSourceBuilder.aggregation(aggr);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status())) {
                // 分桶
                Histogram byCompanyAggregation = aggregations.get("birthday_histogram");
                List<? extends Histogram.Bucket> buckets = byCompanyAggregation.getBuckets();
                // 输出各个桶的内容
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                for (Histogram.Bucket bucket : buckets) {
                    log.info("桶名：{} | 总数：{}", bucket.getKeyAsString(), bucket.getDocCount());
                }
                log.info("-------------------------------------------");
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
}
```
<a name="3ie3p"></a>
### 3、Metric 与 Bucket 聚合分析
<a name="EQGzh"></a>
#### (1)、Restful 操作示例
按照员工岁数分桶、然后统计每个岁数员工工资最高值:
```
GET mydlq-user/_search
{
  "size": 0,
  "aggs": {
    "salary_bucket": {
      "terms": {
        "field": "age",
        "size": "10"
      },
      "aggs": {
        "salary_max_user": {
          "top_hits": {
            "size": 1,
            "sort": [
              {
                "salary": {
                  "order": "desc"
                }
              }
            ]
          }
        }
      }
    }
  }
}
```
<a name="GaACa"></a>
### (2)、Java 代码示例
```java
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.rest.RestStatus;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.aggregations.AggregationBuilder;
import org.elasticsearch.search.aggregations.AggregationBuilders;
import org.elasticsearch.search.aggregations.Aggregations;
import org.elasticsearch.search.aggregations.bucket.terms.Terms;
import org.elasticsearch.search.aggregations.metrics.tophits.ParsedTopHits;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.elasticsearch.search.sort.SortOrder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.io.IOException;
import java.util.List;
@Slf4j
@Service
public class AggrBucketMetricService {
    @Autowired
    private RestHighLevelClient restHighLevelClient;
    /**
     * topHits 按岁数分桶、然后统计每个员工工资最高值
     */
    public Object aggregationTopHits() {
        try {
            AggregationBuilder testTop = AggregationBuilders.topHits("salary_max_user")
                    .size(1)
                    .sort("salary", SortOrder.DESC);
            AggregationBuilder salaryBucket = AggregationBuilders.terms("salary_bucket")
                    .field("age")
                    .size(10);
            salaryBucket.subAggregation(testTop);
            // 查询源构建器
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.size(0);
            searchSourceBuilder.aggregation(salaryBucket);
            // 创建查询请求对象，将查询条件配置到其中
            SearchRequest request = new SearchRequest("mydlq-user");
            request.source(searchSourceBuilder);
            // 执行请求
            SearchResponse response = restHighLevelClient.search(request, RequestOptions.DEFAULT);
            // 获取响应中的聚合信息
            Aggregations aggregations = response.getAggregations();
            // 输出内容
            if (RestStatus.OK.equals(response.status())) {
                // 分桶
                Terms byCompanyAggregation = aggregations.get("salary_bucket");
                List<? extends Terms.Bucket> buckets = byCompanyAggregation.getBuckets();
                // 输出各个桶的内容
                log.info("-------------------------------------------");
                log.info("聚合信息:");
                for (Terms.Bucket bucket : buckets) {
                    log.info("桶名：{}", bucket.getKeyAsString());
                    ParsedTopHits topHits = bucket.getAggregations().get("salary_max_user");
                    for (SearchHit hit:topHits.getHits()){
                        log.info(hit.getSourceAsString());
                    }
                }
                log.info("-------------------------------------------");
            }
        } catch (IOException e) {
            log.error("", e);
        }
    }
}
```
