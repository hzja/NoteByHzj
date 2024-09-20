JavaSpringBootElasticsearch<br />简单介绍一下需求

1. 能支持文件的上传，下载
2. 要能根据关键字，搜索出文件，要求要能搜索到文件里的文字，文件类型要支持word，pdf，txt

文件上传，下载比较简单，要能检索到文件里的文字，并且要尽量精确，这种情况下很多东西就需要考虑进去了。这种情况下，决定使用Elasticsearch来实现。
<a name="lHioe"></a>
## **Elasticsearch简介**
Elasticsearch是一个开源的搜索文献的引擎，大概含义就是通过Rest请求告诉它关键字，他返回对应的内容，就这么简单。<br />Elasticsearch封装了Lucene，Lucene是apache软件基金会一个开放源代码的全文检索引擎工具包。Lucene的调用比较复杂，所以Elasticsearch就再次封装了一层，并且提供了分布式存储等一些比较高级的功能。<br />基于Elasticsearch有很多的插件，这次用到的主要有两个，一个是kibana，一个是Elasticsearch-head。

- kibana主要用来构建请求，它提供了很多自动补全的功能。
- Elasticsearch-head主要用来可视化Elasticsearch。
<a name="bbU00"></a>
## **开发环境**
首先安装Elasticsearch，Elasticsearch-head，kibana，三个东西都是**开箱即用，双击运行** 。需要注意的是kibana的版本要和Elasticsearch的版本对应。<br />Elasticsearch-head是Elasticsearch的可视化界面，Elasticsearch是基于Rest风格的API来操作的，有了可视化界面，就不用每次都使用Get操作来查询了，能提升开发效率。<br />Elasticsearch-head是使用node.js开发的，在安装过程中可能会遇到跨域的问题：Elasticsearch的默认端口是9200，而Elasticsearch-head的默认端口是9100，需要改一下配置文件，具体怎么改就不详细说啦，毕竟有万能的搜索引擎。<br />Elasticsearch安装完成之后，访问端口，就会出现版本信息。
<a name="QqtF2"></a>
## **核心问题**
有两个需要解决的核心问题，文件上传和输入关键词查询。
<a name="OUC27"></a>
### 文件上传
首先对于txt这种纯文本的形式来说，比较简单，直接将里面的内容传入即可。但是对于pdf，word这两种特殊格式，文件中除了文字之外有很多无关的信息，比如图片，pdf中的标签等这些信息。这就要求对文件进行预处理。<br />Elasticsearch5.x以后提供了名为ingest node的功能，ingest node可以对输入的文档进行预处理。如图，PUT请求进入后会先判断有没有pipline，如果有的话会进入Ingest Node进行处理，之后才会正式被处理。<br />![引用自Elastic 中国社区官方博客](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334836946-7ac0a889-29fd-4d37-92fd-b136ca37b4c7.png#clientId=u43f27d75-45c6-4&from=ui&id=u3e4326b5&originHeight=435&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=1412198&status=done&style=shadow&taskId=ua213c398-76e8-4090-a1b4-d5bba40c472&title=%E5%BC%95%E7%94%A8%E8%87%AAElastic%20%E4%B8%AD%E5%9B%BD%E7%A4%BE%E5%8C%BA%E5%AE%98%E6%96%B9%E5%8D%9A%E5%AE%A2 "引用自Elastic 中国社区官方博客")<br />Ingest Attachment Processor Plugin是一个文本抽取插件，本质上是利用了Elasticsearch的ingest node功能，提供了关键的预处理器attachment。在安装目录下运行以下命令即可安装。
```bash
./bin/elasticsearch-plugin install ingest-attachment
```
<a name="Ww0Ht"></a>
#### 定义文本抽取管道
```http
PUT /_ingest/pipeline/attachment
{
    "description": "Extract attachment information",
    "processors": [
        {
            "attachment": {
                "field": "content",
                "ignore_missing": true
            }
        },
        {
            "remove": {
                "field": "content"
            }
        }
    ]
}
```
在attachment中指定要过滤的字段为content，所以写入Elasticsearch时需要将文档内容放在content字段。<br />运行结果如图：<br />![定义文本抽取管道](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334136392-08a21c77-adce-4767-9ac8-69288fd4b8ce.png#clientId=u77348d5f-11d5-4&from=paste&id=u100c763c&originHeight=251&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u27e42f0a-7be4-4cbd-b756-64694e2002d&title=%E5%AE%9A%E4%B9%89%E6%96%87%E6%9C%AC%E6%8A%BD%E5%8F%96%E7%AE%A1%E9%81%93 "定义文本抽取管道")
<a name="TyTrG"></a>
#### 建立文档结构映射
文本文件通过预处理器上传后以何种形式存储，需要建立文档结构映射来定义。PUT定义文档结构映射的时候就会自动创建索引，所以先创建一个docwrite的索引，用于测试。
```http
PUT /docwrite
{
  "mappings": {
    "properties": {
      "id":{
        "type": "keyword"
      },
      "name":{
        "type": "text",
        "analyzer": "ik_max_word"
      },
      "type":{
        "type": "keyword"
      },
      "attachment": {
        "properties": {
          "content":{
            "type": "text",
            "analyzer": "ik_smart"
          }
        }
      }
    }
  }
}
```
在ElasticSearch中增加了`attachment`字段，这个字段是`attachment`命名pipeline抽取文档附件中文本后自动附加的字段。这是一个嵌套字段，其包含多个子字段，包括抽取文本 content 和一些文档信息元数据。<br />同是对文件的名字name指定分析器analyzer为ik_max_word，以让ElasticSearch在建立全文索引时对它们进行中文分词。<br />![建立文档结构](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334136401-5fe9c636-d73d-4955-a87e-170db16fc25e.png#clientId=u77348d5f-11d5-4&from=paste&id=u23601a01&originHeight=361&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u40cf7369-3990-4f7c-9d72-079c156ce12&title=%E5%BB%BA%E7%AB%8B%E6%96%87%E6%A1%A3%E7%BB%93%E6%9E%84 "建立文档结构")
<a name="y87tA"></a>
#### 测试
经过上面两步，进行简单的测试。因为ElasticSearch是基于JSON格式的文档数据库，所以附件文档在插入ElasticSearch之前必须进行Base64编码。先通过下面的网站将一个pdf文件转化为base64的文本。PDF to Base64<br />测试文档如图：<br />![测试文档](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334136522-b78db165-05be-4893-8367-21562215130c.png#clientId=u77348d5f-11d5-4&from=paste&id=u17962319&originHeight=530&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ucd102498-19cc-4e2e-9140-69f425a7403&title=%E6%B5%8B%E8%AF%95%E6%96%87%E6%A1%A3 "测试文档")<br />然后通过以下请求上传上去，找了一个很大的pdf文件。需要指定的是刚创建的pipeline，结果如图所示。<br />![文件上传测试](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334136752-c7d5d94a-5ef9-432e-b437-1d697babfc11.png#clientId=u77348d5f-11d5-4&from=paste&id=ub9ce77fc&originHeight=312&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ue3a44920-b8d3-4375-8a8a-7b69869e62c&title=%E6%96%87%E4%BB%B6%E4%B8%8A%E4%BC%A0%E6%B5%8B%E8%AF%95 "文件上传测试")<br />原来的索引有个type类型，新版本后面会被弃用，默认的版本都是_doc<br />然后通过GET操作看看文档是否上传成功。可以看到已经被解析成功。<br />![文件上传结果查看](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334136720-46190547-9226-4b2e-b301-9c6d4800e0dc.png#clientId=u77348d5f-11d5-4&from=paste&id=u7b2ccb93&originHeight=549&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u7b4a8538-8641-42cb-8181-656538f2ad3&title=%E6%96%87%E4%BB%B6%E4%B8%8A%E4%BC%A0%E7%BB%93%E6%9E%9C%E6%9F%A5%E7%9C%8B "文件上传结果查看")<br />如果不指定pipline的话，就会出现无法解析的情况。<br />![没有指定pipeline的情况](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334136746-6ea032b0-949c-4578-9e22-13508eb1b981.png#clientId=u77348d5f-11d5-4&from=paste&id=u799ce037&originHeight=552&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u86fe22d7-c9de-4ee3-b7f3-3cb0ec24ce8&title=%E6%B2%A1%E6%9C%89%E6%8C%87%E5%AE%9Apipeline%E7%9A%84%E6%83%85%E5%86%B5 "没有指定pipeline的情况")<br />根据结果可以看到，PDF文件已经通过自行定义的pipline，然后才正式进入索引数据库docwrite。
<a name="qTQxG"></a>
### 关键字查询
关键字查询即对输入的文字，能进行一定的分词处理。比如说对于“数据库计算机网络我的电脑”这一串词来说，要能将其分为“数据库”，“计算机网络”，“我的电脑”三个关键词，然后分别根据关键字查询。<br />Elasticsearch自带了分词器，支持所有的Unicode字符，但是它只会做最大的划分，比如对于进口红酒这四个字，会被分为“进”，“口”，“红”，“酒”这四个字，这样查询出来的结果就会包括“进口”，“口红”，“红酒”。<br />![默认分词器](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334137176-31ce0556-27e1-4052-b476-6bd8ee2cabfa.png#clientId=u77348d5f-11d5-4&from=paste&id=u81dad954&originHeight=452&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u60624398-75d3-4c6c-9977-104b696a189&title=%E9%BB%98%E8%AE%A4%E5%88%86%E8%AF%8D%E5%99%A8 "默认分词器")<br />这并不是想要的结果。想要的结果是，只分为“进口”，“红酒”这两段，然后查询相应的结果。这就需要使用支持中文的分词器了。
<a name="FIp4Q"></a>
#### ik分词器
ik分词器是开源社区比较流行的中文分词插件，首先安装ik分词器，注意以下代码不能直接使用。
```bash
./bin/elasticsearch-plugin install https://github.com/medcl/elasticsearch-analysis-ik/releases/download/...这里找你的版本
```
ik分词器包括两种模式。

1. ik_max_word会把中文尽可能的拆分。
2. ik_smart会根据常用的习惯进行划分，比如"进口红酒”会被划分为“进口”，“红酒”。

![ik_smart模式](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334137252-cbb1aee1-e8e2-4d44-badc-1a716df0d1a9.png#clientId=u77348d5f-11d5-4&from=paste&id=u44b7f258&originHeight=294&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua697e569-0d15-4c85-880b-ddfffb727e4&title=ik_smart%E6%A8%A1%E5%BC%8F "ik_smart模式")<br />在使用查询时，指定ik分词器进行查询文档，比如对于插入的测试文档，使用ik_smart模式搜索，结果如图。
```http
GET /docwrite/_search
{
  "query": {
    "match": {
      "attachment.content": {
        "query": "实验一",
        "analyzer": "ik_smart"
      }
    }
  }
}
```
可以指定Elasticsearch中的高亮，来为筛选到的文字添加标签。这样的话文字前后都会被添加上标签。如图。<br />![highlight效果](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334137180-63b604e2-13e3-4a21-927b-f72a393a8833.png#clientId=u77348d5f-11d5-4&from=paste&id=NMbzQ&originHeight=521&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u4ec6105c-d961-4026-9bab-ede728a1f13&title=highlight%E6%95%88%E6%9E%9C "highlight效果")
<a name="nrycA"></a>
## **编码**
编码使用Idea+maven的开发环境，首先导入依赖，依赖一定要与Elasticsearch的版本相对应。
<a name="eyYmJ"></a>
### 导入依赖
Elstacisearch对于Java来说有两个API，使用的封装的比较完善的高级API。
```xml
<dependency>
	<groupId>org.elasticsearch.client</groupId>
	<artifactId>elasticsearch-rest-high-level-client</artifactId>
	<version>7.9.1</version>
</dependency>
```
<a name="JRa4W"></a>
### 文件上传
先建立一个与上文对应的fileObj对象
```java
public class FileObj {
    String id; //用于存储文件id
    String name; //文件名
    String type; //文件的type，pdf，word，or txt
    String content; //文件转化成base64编码后所有的内容。
}
```
首先根据上文所诉，要先将文件以字节数组的形式读入，然后转化成Base64编码。
```java
public FileObj readFile(String path) throws IOException {
    //读文件
    File file = new File(path);
    
    FileObj fileObj = new FileObj();
    fileObj.setName(file.getName());
    fileObj.setType(file.getName().substring(file.getName().lastIndexOf(".") + 1));
    
    byte[] bytes = getContent(file);
    
    //将文件内容转化为base64编码
    String base64 = Base64.getEncoder().encodeToString(bytes);
    fileObj.setContent(base64);
    
    return fileObj;
}
```
`java.util.Base64`已经提供了现成的函数`Base64.getEncoder().encodeToString`可以使用。<br />接下来就可以使用Elasticsearch的API将文件上传了。<br />上传需要使用`IndexRequest`对象，使用FastJson将fileObj转化为Json后，上传。需要使用`indexRequest.setPipeline`函数指定上文中定义的pipline。这样文件就会通过pipline进行预处理，然后进入`fileindex`索引中。
```java
public void upload(FileObj file) throws IOException {
    IndexRequest indexRequest = new IndexRequest("fileindex");
    
    //上传同时，使用attachment pipline进行提取文件
    indexRequest.source(JSON.toJSONString(file), XContentType.JSON);
    indexRequest.setPipeline("attatchment");
    
    IndexResponse indexResponse = client.index(indexRequest, RequestOptions.DEFAULT);
    System.out.println(indexResponse);
}
```
<a name="cZ4Vv"></a>
### 文件查询
文件查询需要使用`SearchRequest`对象，首先要指定对关键字使用ik分词器的ik_smart模式分词
```java
SearchSourceBuilder srb = new SearchSourceBuilder();
srb.query(QueryBuilders.matchQuery("attachment.content", keyword).analyzer("ik_smart"));
searchRequest.source(srb);
```
之后就可以通过返回的`Response`对象获取每一个hits，之后获取返回的内容。
```java
Iterator<SearchHit> iterator = hits.iterator();
int count = 0;
while (iterator.hasNext()) {
    SearchHit hit = iterator.next();
}
```
Elasticsearh一个非常强大的功能是文件的高亮（highlight）功能，所以可以设置一个highlighter，对查询到的文本进行高亮操作。
```java
HighlightBuilder highlightBuilder = new HighlightBuilder();
HighlightBuilder.Field highlightContent = new HighlightBuilder.Field("attachment.content");
highlightContent.highlighterType();
highlightBuilder.field(highlightContent);
highlightBuilder.preTags("<em>");
highlightBuilder.postTags("</em>");
srb.highlighter(highlightBuilder);
```
设置了前置`<em></em>`标签对对查询的结果进行包裹。这样查询到的结果中就会包含对应的结果。
<a name="nSZRF"></a>
## **多文件测试**
简单的demo写好了，但是效果怎么样还需要使用多个文件进行测试。这是一个测试文件夹，里面下面放了各种类型的文件。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334137218-79a297c2-1a5f-4089-98d5-410ae7f72700.png#clientId=u77348d5f-11d5-4&from=paste&id=u18a0de50&originHeight=569&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3128f2f0-2f17-4833-a264-3417a5b7ee6&title=)<br />将这个文件夹里面的全部文件上传之后，使用elestacisearch``-head可视化界面查看导入的文件。<br />![导入的文件](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334137438-a82d4efc-9927-435a-9b4f-ccdc062c7fea.png#clientId=u77348d5f-11d5-4&from=paste&id=u5d94061b&originHeight=302&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u29adb2db-4746-4be6-9b57-3c7e5037562&title=%E5%AF%BC%E5%85%A5%E7%9A%84%E6%96%87%E4%BB%B6 "导入的文件")<br />搜索代码：
```java
/**
* 这部分会根据输入的关键字去查询数据库中的信息，然后返回对应的结果
* @throws IOException
*/
@Test
public void fileSearchTest() throws IOException {
	ElasticOperation elo = eloFactory.generate();

	elo.search("数据库国务院计算机网络");
}
```
运行demo，查询的结果如图所示。<br />![搜索结果](https://cdn.nlark.com/yuque/0/2022/png/396745/1652334137572-39b4f38f-1727-4033-b79e-8272e1c17ca8.png#clientId=u77348d5f-11d5-4&from=paste&id=u57572c10&originHeight=369&originWidth=909&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u319e315a-4144-4f35-b1fb-4d2d7118ef9&title=%E6%90%9C%E7%B4%A2%E7%BB%93%E6%9E%9C "搜索结果")
<a name="iInyi"></a>
### 还存在的一些问题
<a name="J96Sg"></a>
#### 1、文件长度问题
通过测试发现，对于文本内容超过10万字的文件，elasticsearch只保留10w字，后面的就被截断了，这就需要进一步了解Elasticsearch对10w字以上的文本的支持。
<a name="fAtmE"></a>
#### 2、编码上的一些问题
代码中，**是将文件全部读入内存之后，在进行一系列的处理** ，毫无疑问，必定会带来问题，比如假如是一个超出内存的超大文件，或者是若干个大文件，在实际生产环境中，文件上传就会占用服务器的相当一大部分内存和带宽，这就要根据具体的需求，做进一步的优化。
