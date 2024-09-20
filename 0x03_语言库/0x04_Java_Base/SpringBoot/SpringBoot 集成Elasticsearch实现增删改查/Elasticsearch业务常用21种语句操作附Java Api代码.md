Java Elasticsearch
<a name="WcdoX"></a>
## 概述
操作数据习惯使用sql语句，对于非sql语句的数据库中间肯定使用得很不习惯对于Elasticsearch经常常用的操作，总结一些语句(包括 json格式语句和对应java api编写)。
<a name="ST0yc"></a>
### 配置
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-data-elasticsearch</artifactId>
</dependency>
```
直接导入依赖，然后注入 RestHighLevelClient 对象即可使用
<a name="mmYfR"></a>
## 基础增删改
<a name="MQ5FI"></a>
### 添加文档
<a name="O4I6q"></a>
#### Java API
```java
/**
* 添加文档
* @param addDocDto
* @throws IOException
*/
public void addDoc(AddDocDto addDocDto) throws IOException {
	
	IndexRequest indexRequest = new IndexRequest(addDocDto.getIndexName());
	
	indexRequest.source(addDocDto.getData());
	indexRequest.id(addDocDto.getId());
	//创建索引
	IndexResponse index = restHighLevelClient.index(indexRequest, RequestOptions.DEFAULT);
	
	log.info("执行结果:"+JSONUtil.toJsonStr(index));
}
```
<a name="nttk5"></a>
#### Elasticsearch
```json
POST test-index-20220228/_doc
{
  "sex": "w",
  "balance2": 320,
  "balance": "501",
  "name": "liujing5",
  "age": 6
}
```
说明：用于插入文档，post 方法 自动生成 id
<a name="qNsU5"></a>
### 更新文档
<a name="ylAbX"></a>
#### Java API
```java
/**
 * 更新文档(仅仅根据id更新，是增量字段)
 * @param updateDocDto
 * @throws IOException
 */
public void updateDoc3(UpdateDocDto updateDocDto) throws IOException {

    UpdateRequest indexRequest = new UpdateRequest(updateDocDto.getIndexName(),updateDocDto.getId());

    //设置数据
    indexRequest.doc(updateDocDto.getData());

    //更新文档
    UpdateResponse update = restHighLevelClient.update(indexRequest, RequestOptions.DEFAULT);

    log.info("执行结果:"+JSONUtil.toJsonStr(update));
}
```
<a name="vhvsp"></a>
#### Elasticsearch
```json
POST test-index-20220228/_update/L1gtSX8BX_oAU-6DS9F3
{
  "doc": {
     "sex" : "w",
      "balance2" : 320,
      "balance" : "501",
      "name" : "liujing5",
      "age" : 7
  }
}
```
<a name="V7tuc"></a>
### 更新文档根据条件
<a name="vsfto"></a>
#### Java API
```java
/**
 * 更新文档（根据条件更新数据，删掉了老字段）
 * @param updateDocDto
 * @throws IOException
 */
public void updateDoc2(UpdateDocDto updateDocDto) throws IOException {

    UpdateByQueryRequest indexRequest = new UpdateByQueryRequest(updateDocDto.getIndexName());

    //设置更新条件
    Set<String> strings = updateDocDto.getQuery().keySet();
    for (String key:strings){
        QueryBuilder queryBuilder = new TermQueryBuilder(key,updateDocDto.getQuery().get(key));
        indexRequest.setQuery(queryBuilder);
    }
    //设置更新值
    JSONObject data = updateDocDto.getData();

    //更新值脚本
    String source = "ctx._source=params";
    Script script = new Script(ScriptType.INLINE, "painless", source, data);
    indexRequest.setScript(script);

    //更新文档
    BulkByScrollResponse index = restHighLevelClient.updateByQuery(indexRequest, RequestOptions.DEFAULT);

    log.info("执行结果:"+JSONUtil.toJsonStr(index));
}
```
<a name="efYR7"></a>
#### Elasticsearch
```json
POST test-index-20220228/_update_by_query
{
  "query": {
    "term": {
      "age": {
        "value" : 7,
        "boost" : 1.0
      }
    }
  },
  "script": {
    "source":"ctx._source=params", 
    "params":{"aaa":"50003"}
  }
}
```
说明：根据任意条件更新匹配文档
<a name="U1mfx"></a>
### 删除文档
<a name="d9bPm"></a>
#### Java API
```java
/**
 * 删除文档（根据id）
 * @param deleteDocDto
 * @throws IOException
 */
public void deleteDoc(DeleteDocDto deleteDocDto) throws IOException {

    DeleteRequest indexRequest = new DeleteRequest(deleteDocDto.getIndexName(),deleteDocDto.getId());

    //更新文档
    DeleteResponse update = restHighLevelClient.delete(indexRequest, RequestOptions.DEFAULT);

    log.info("执行结果:"+JSONUtil.toJsonStr(update));
}
```
<a name="ANyKP"></a>
#### Elasticsearch
```json
DELETE test-index-20220228/_doc/L1gtSX8BX_oAU-6DS9F3
```
<a name="oCV6u"></a>
### 删除文档根据条件
<a name="FOXHe"></a>
#### Java API
```java
/**
 * 删除文档(根据任意条件)
 * @param deleteDocDto
 * @throws IOException
 */
public void deleteDoc2(DeleteDocDto deleteDocDto) throws IOException {

    DeleteByQueryRequest indexRequest = new DeleteByQueryRequest(deleteDocDto.getIndexName());

    //设置查询条件
    Set<String> strings = deleteDocDto.getQuery().keySet();
    for (String key:strings){
        QueryBuilder queryBuilder = new TermQueryBuilder(key,deleteDocDto.getQuery().get(key));
        indexRequest.setQuery(queryBuilder);
        log.info(queryBuilder.toString());
    }
    //更新文档
    BulkByScrollResponse response = restHighLevelClient.deleteByQuery(indexRequest, RequestOptions.DEFAULT);

    log.info("执行结果:"+JSONUtil.toJsonStr(response));
}
```
<a name="Ocfvv"></a>
#### Elasticsearch
```json
POST test-index-20220228/_delete_by_query
{
  "query":{
    "term" : {
    "age" : {
      "value" : 6,
      "boost" : 1.0
    }
  }
  }
}
```
说明：根据条件删除指定文档
<a name="FEgW9"></a>
## 非分词基础查询 (term)
<a name="tBCYn"></a>
### 单字段等值查询
<a name="VTDFx"></a>
#### SQL
```sql
select * from user where name = "张三"
```
<a name="Mo6S3"></a>
#### Java API
```java
/**
 * 单字段查询(等值)
 * @param searchDocDto
 * @throws IOException
 */
public void search(SearchDocDto searchDocDto) throws IOException {

    //构件查询
    TermQueryBuilder termQueryBuilder = QueryBuilders.termQuery(searchDocDto.getKey(), searchDocDto.getValue());
    //查询
    searchCommon(searchDocDto.getIndexName(),termQueryBuilder);
}
```
<a name="xTXfA"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  "query": {
    "term": {
      "name": {
        "value": "liujing2",
        "boost": 1
      }
    }
  }
}
```
说明：可以进行任意单字段进行等值查询操作
<a name="A9vRN"></a>
### 单字段多值查询
<a name="ssFqg"></a>
#### SQL
```sql
select * from user where name in ("张三","李四")
```
<a name="BtBYZ"></a>
#### Java API
```java
/**
 * 单字段查询(多值等值)
 * @param searchDocDto
 * @throws IOException
 */
public void search2(SearchDocDto searchDocDto) throws IOException {

    //构件查询
    TermsQueryBuilder termsQueryBuilder = QueryBuilders.termsQuery(searchDocDto.getKey(), searchDocDto.getValues());
    //查询
    searchCommon(searchDocDto.getIndexName(),termsQueryBuilder);
}
```
<a name="ROQBx"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  "query": {
    "terms": {
      "name": [
        "liujing3",
        "liujing"
      ],
      "boost": 1
    }
  }
}
```
说明：可以进行任意单字段多个值进行等值批量查询操作
<a name="bLBLI"></a>
## 分词基础查询 (match)
<a name="M3hts"></a>
### 单字段分词查询
<a name="snk9w"></a>
#### SQL
```sql
select * from user where name like "%liujjing%"
```
<a name="v0ZJv"></a>
#### Java API
```java
/**
 * 单字段查询（模糊-分词查询）
 * @param searchDocDto
 */
public void search11(SearchDocDto searchDocDto) throws IOException {
    //构件match查询
    MatchQueryBuilder matchQueryBuilder = QueryBuilders.matchQuery(searchDocDto.getKey(), searchDocDto.getValue());

    //查询
    searchCommon(searchDocDto.getIndexName(),matchQueryBuilder);
}
```
<a name="YbxwT"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  "query": {
    "match": {
      "name": {
        "query": "liujing2",
        "operator": "OR",
        "prefix_length": 0,
        "max_expansions": 50,
        "fuzzy_transpositions": true,
        "lenient": false,
        "zero_terms_query": "NONE",
        "auto_generate_synonyms_phrase_query": true,
        "boost": 1
      }
    }
  }
}
```
说明：可以对任意字段进行分词查询
<a name="W85AX"></a>
### 多字段分词查询
<a name="Oxtxs"></a>
#### SQL
```sql
select * from user where name like "%liujjing%" or ...
```
<a name="CAA0w"></a>
#### Java API
```java
/**
 * 多字段查询（模糊多个字段-分词查询）
 * @param searchDocDto
 */
public void search12(SearchDocDto searchDocDto) throws IOException {
    //构件match查询
    MultiMatchQueryBuilder multiMatchQueryBuilder = QueryBuilders.multiMatchQuery(searchDocDto.getValue(),searchDocDto.getFields());

    //查询
    searchCommon(searchDocDto.getIndexName(),multiMatchQueryBuilder);
}
```
<a name="DRdaE"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  "query": {
    "multi_match": {
      "query": "liujing2 1200",
      "fields": [
        "name^1.0","balance"
      ],
      "type": "best_fields",
      "operator": "OR",
      "slop": 0,
      "prefix_length": 0,
      "max_expansions": 50,
      "zero_terms_query": "NONE",
      "auto_generate_synonyms_phrase_query": true,
      "fuzzy_transpositions": true,
      "boost": 1
    }
  }
}
```
说明：可以同时对多个任意字段进行分词查询
<a name="NVeZb"></a>
### 多字段词组分词查询
<a name="tYD10"></a>
#### SQL
```sql
select * from user where name like "%liujjing hello %"
```
<a name="rCsXg"></a>
#### Java API
```java
/**
 * 单字段查询（模糊-词组分词查询）
 * @param searchDocDto
 */
public void search13(SearchDocDto searchDocDto) throws IOException {
    //构件match查询
    MatchPhraseQueryBuilder matchPhraseQueryBuilder = QueryBuilders.matchPhraseQuery(searchDocDto.getKey(), searchDocDto.getValue());

    //查询
    searchCommon(searchDocDto.getIndexName(),matchPhraseQueryBuilder);
}
```
<a name="xktTi"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  "query": {
    "match_phrase": {
      "name": {
        "query": "liujing2 hello",
        "slop": 0,
        "zero_terms_query": "NONE",
        "boost": 1
      }
    }
  }
}
```
说明：可以任意字段进行词语分词查询
<a name="LJHuH"></a>
## 范围和分页查询
<a name="aZWXc"></a>
### 单字段范围查询
<a name="fXIof"></a>
#### SQL
```sql
select * from user where age >20 and age<30
```
<a name="PNmb9"></a>
#### Java API
```java
/**
 * 单字段查询(范围)
 * @param searchDocDto
 * @throws IOException
 */
public void search3(SearchDocDto searchDocDto) throws IOException {
    //构件查询
    RangeQueryBuilder rangeQueryBuilder = QueryBuilders.rangeQuery(searchDocDto.getKey());
    rangeQueryBuilder.from(searchDocDto.getStartValue());
    rangeQueryBuilder.to(searchDocDto.getEndValue());

    //查询
    searchCommon(searchDocDto.getIndexName(),rangeQueryBuilder);
}
```
<a name="zV130"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  "query": {
    "range": {
      "age": {
        "from": "2",
        "to": "19",
        "include_lower": true,
        "include_upper": true,
        "boost": 1
      }
    }
  }
}
```
说明：可以进行任意单字段范围批量查询操作
<a name="dchLw"></a>
### 分页查询
<a name="LTq30"></a>
#### SQL
```sql
select * from user limit 1 5
```
<a name="HYrX6"></a>
#### Java API
```java
/**
 * 分页
 * @param searchDocDto
 */
public void search14(SearchDocDto searchDocDto) throws IOException {

    SearchRequest searchRequest = new SearchRequest(searchDocDto.getIndexName());
    //构建查询
    SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
    //分页
    searchSourceBuilder.from(1);
    searchSourceBuilder.size(2);

    searchRequest.source(searchSourceBuilder);

    log.info("查询条件"+searchSourceBuilder.toString());

    SearchResponse search = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);

    SearchHits hits = search.getHits();

    for (SearchHit searchHit:hits){
        log.info(JSONUtil.toJsonStr(searchHit.getSourceAsString()));
    }
}
```
<a name="q8f6q"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  //query  可以使用query分页
  
  "from": 1,
  "size": 5
}
```
说明：可以进行带条件分页查询
<a name="xABMe"></a>
## 复合条件查询
<a name="CW75I"></a>
### should
<a name="JBEGy"></a>
#### SQL
```sql
select * from user = "张三" or age=18
```
<a name="zTnbR"></a>
#### Java API
```java
/**
 * 复合多字段查询(should)
 * @param searchDocDto
 */
public void search4(SearchDocDto searchDocDto) throws IOException {
    //构件查询
    BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();

    JSONObject query = searchDocDto.getQuery();
    Set<String> strings = query.keySet();

    for(String key:strings) {
        TermQueryBuilder termQueryBuilder = QueryBuilders.termQuery(key, query.get(key));
        boolQueryBuilder.should(termQueryBuilder);
    }
    //查询
    searchCommon(searchDocDto.getIndexName(),boolQueryBuilder);
}
```
<a name="h3S3O"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  "query": {
    "bool": {
      "should": [
        {
          "term": {
            "name": {
              "value": "liujing",
              "boost": 1
            }
          }
        },
        {
          "term": {
            "age": {
              "value": 19,
              "boost": 1
            }
          }
        }
      ],
      "adjust_pure_negative": true,
      "boost": 1
    }
  }
}
```
说明：可以进行多字段进行 or 查询, 只要满足一个 shoud 的条件即可
<a name="pMoj1"></a>
### must
<a name="UWtLr"></a>
#### SQL
```sql
select * from user = "张三" and age=18
```
<a name="qxiOb"></a>
#### Java API
```java
/**
 * 复合多字段查询(must)
 * @param searchDocDto
 */
public void search5(SearchDocDto searchDocDto) throws IOException {
    //构件查询
    BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();

    JSONObject query = searchDocDto.getQuery();
    Set<String> strings = query.keySet();

    for(String key:strings) {
        TermQueryBuilder termQueryBuilder = QueryBuilders.termQuery(key, query.get(key));
        boolQueryBuilder.must(termQueryBuilder);
    }

    //查询
    searchCommon(searchDocDto.getIndexName(),boolQueryBuilder);
}
```
<a name="RLOwL"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "term": {
            "name": {
              "value": "liujing",
              "boost": 1
            }
          }
        },
        {
          "term": {
            "age": {
              "value": 19,
              "boost": 1
            }
          }
        }
      ],
      "adjust_pure_negative": true,
      "boost": 1
    }
  }
}
```
说明：可以进行多字段进行 and 查询，要同时满足多个条件才返回
<a name="UDPqj"></a>
### must not
<a name="al8Yc"></a>
#### SQL
```sql
select * from user != "张三" and age!=18
```
<a name="MdRHU"></a>
#### Java API
```java
/**
 * 复合多字段查询(must not,不计算相关性分数，和must查询到数据相反)
 * @param searchDocDto
 */
public void search6(SearchDocDto searchDocDto) throws IOException {
    //构件查询
    BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();

    JSONObject query = searchDocDto.getQuery();
    Set<String> strings = query.keySet();

    for(String key:strings) {
        TermQueryBuilder termQueryBuilder = QueryBuilders.termQuery(key, query.get(key));
        boolQueryBuilder.mustNot(termQueryBuilder);
    }
    //查询
    searchCommon(searchDocDto.getIndexName(),boolQueryBuilder);
}
```
<a name="TqLHa"></a>
#### Elasticsearch
```json
{
  "query": {
    "bool": {
      "must_not": [
        {
          "term": {
            "name": {
              "value": "liujing",
              "boost": 1
            }
          }
        },
        {
          "term": {
            "age": {
              "value": 19,
              "boost": 1
            }
          }
        }
      ],
      "adjust_pure_negative": true,
      "boost": 1
    }
  }
}
```
说明：可以进行多字段进行 and 查询，要同时不满足多个条件才返回
<a name="W8Fn7"></a>
### filter
<a name="ASZJb"></a>
#### SQL
```sql
select * from user = "张三" and age=18
```
<a name="iQzOI"></a>
#### Java API
```java
/**
 * 复合多字段查询(filter,不计算相关性分数，和must查询到数据相同)
 * @param searchDocDto
 */
public void search7(SearchDocDto searchDocDto) throws IOException {
    //构件查询
    BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();

    JSONObject query = searchDocDto.getQuery();
    Set<String> strings = query.keySet();

    for(String key:strings) {
        TermQueryBuilder termQueryBuilder = QueryBuilders.termQuery(key, query.get(key));
        boolQueryBuilder.filter(termQueryBuilder);
    }
    //查询
    searchCommon(searchDocDto.getIndexName(),boolQueryBuilder);
}
```
<a name="gD3kg"></a>
#### Elasticsearch
```json
{
  "query": {
    "bool": {
      "filter": [
        {
          "term": {
            "name": {
              "value": "liujing",
              "boost": 1
            }
          }
        },
        {
          "term": {
            "age": {
              "value": 19,
              "boost": 1
            }
          }
        }
      ],
      "adjust_pure_negative": true,
      "boost": 1
    }
  }
}
```
说明：和 must 效果一样，只是不做评分
<a name="Uaon7"></a>
### should,must,must_not,filter 复合
<a name="BxweE"></a>
#### SQL
```sql
select * from ((user = "张三" and age=18) and (balance=1000 or balance=2000))
```
<a name="BOPo9"></a>
#### Java API
```java
/**
 * 复合多字段查询(should,must,must_not,filter进行复合)
 * @param searchDocDto
 */
public void search8(SearchDocDto searchDocDto) throws IOException {
    //构件查询
    BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();

    //构件should条件
    JSONObject query = searchDocDto.getShouldQuery();
    Set<String> strings = query.keySet();

    for(String key:strings) {
        TermQueryBuilder termQueryBuilder = QueryBuilders.termQuery(key, query.get(key));
        boolQueryBuilder.should(termQueryBuilder);
    }
    //构件must条件
    JSONObject query1 = searchDocDto.getMustQuery();
    Set<String> strings1 = query1.keySet();

    for(String key:strings1) {
        TermQueryBuilder termQueryBuilder = QueryBuilders.termQuery(key, query1.get(key));
        boolQueryBuilder.must(termQueryBuilder);
    }
    //查询
    searchCommon(searchDocDto.getIndexName(),boolQueryBuilder);
}
```
<a name="Wlspj"></a>
#### Elasticsearch
```json
POST test-index-20220228/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "term": {
            "sex": {
              "value": "w",
              "boost": 1
            }
          }
        },
        {
          "term": {
            "name": {
              "value": "liujing2",
              "boost": 1
            }
          }
        }
      ],
      "should": [
        {
          "term": {
            "balance": {
              "value": 1200,
              "boost": 1
            }
          }
        },
        {
          "term": {
            "age": {
              "value": 5,
              "boost": 1
            }
          }
        }
      ],
      "adjust_pure_negative": true,
      "boost": 1
    }
  }
}
```
说明：should,must,filter,must not 这些查询的条件都满足
<a name="xZUqq"></a>
### should,must,must_not,filter 和 bool 复合
<a name="qJd4Z"></a>
#### SQL
```sql
select * from ((user = "张三" and age=18) and (balance=1000 or balance=2000)) or((....))
```
<a name="oevDp"></a>
#### Java API
```java
/**
 * 复合多字段查询(（should,must,must_not,filter）（bool）进行复合)
 * @param searchDocDto
 */
public void search9(SearchDocDto searchDocDto) throws IOException {
    //构件查询
    BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();

    //构件should条件
    JSONObject query = searchDocDto.getShouldQuery();
    Set<String> strings = query.keySet();

    for(String key:strings) {
        TermQueryBuilder termQueryBuilder = QueryBuilders.termQuery(key, query.get(key));
        boolQueryBuilder.should(termQueryBuilder);
    }
    //构件内部bool
    BoolQueryBuilder boolQueryBuilder1 = QueryBuilders.boolQuery();
    JSONObject query1 = searchDocDto.getInnerBoolQuery();
    Set<String> strings1 = query1.keySet();
    for(String key:strings1) {
        TermQueryBuilder termQueryBuilder = QueryBuilders.termQuery(key, query1.get(key));
        boolQueryBuilder1.must(termQueryBuilder);
    }

    boolQueryBuilder.should(boolQueryBuilder1);
    //查询
    searchCommon(searchDocDto.getIndexName(),boolQueryBuilder);
}
```
<a name="lmB36"></a>
#### Elasticsearch
```json
GET test-index-20220228/_search
{
  "query": {
    "bool": {
      "should": [
        {
          "term": {
            "balance": {
              "value": 1200,
              "boost": 1
            }
          }
        },
        {
          "term": {
            "age": {
              "value": 5,
              "boost": 1
            }
          }
        },
        {
          "bool": {
            "must": [
              {
                "term": {
                  "balance": {
                    "value": "50",
                    "boost": 1
                  }
                }
              },
              {
                "term": {
                  "age": {
                    "value": 2,
                    "boost": 1
                  }
                }
              }
            ],
            "adjust_pure_negative": true,
            "boost": 1
          }
        }
      ],
      "adjust_pure_negative": true,
      "boost": 1
    }
  }
}
```
说明：should,must,filter,must not 内部可以再次嵌套 bool 系列条件
<a name="fqpsl"></a>
### should,must,must_not,filter 内部可使用基本查询
<a name="ahrBK"></a>
#### SQL
```sql
select * from ()
```
<a name="clXon"></a>
#### Java API
```java
/**
 * 复合多字段查询(（should,must,must_not,filter）（term,much,range）进行复合)
 * @param searchDocDto
 */
public void search10(SearchDocDto searchDocDto) throws IOException {
    //构件range条件
    RangeQueryBuilder rangeQueryBuilder = QueryBuilders.rangeQuery(searchDocDto.getKey());
    rangeQueryBuilder.from(searchDocDto.getStartValue());
    rangeQueryBuilder.to(searchDocDto.getEndValue());

    //构件should条件
    BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();
    boolQueryBuilder.should(rangeQueryBuilder);

    //查询
    searchCommon(searchDocDto.getIndexName(),boolQueryBuilder);
}
```
<a name="cxVv0"></a>
#### Elasticsearch
```json
GET test-index-20220228/_search
{
  "query": {
    "bool": {
      "should": [
        {
          "range": {
            "name": {
              "from": "1",
              "to": "19",
              "include_lower": true,
              "include_upper": true,
              "boost": 1
            }
          }
        }
      ],
      "adjust_pure_negative": true,
      "boost": 1
    }
  }
}
```
说明：should,must,filter,must not 内部可以使用基本查询 (term,match,range)
<a name="XwkcT"></a>
## 聚合查询
<a name="bfnM6"></a>
### 常用聚合
<a name="g9Iuw"></a>
#### SQL
```sql
select count(*) from user where age>20 group by name,age
```
<a name="ZWizE"></a>
#### Java API
```java
/**
* 聚合(terms--统计文档数量)==(group(可以嵌套group,相当于多字段group)+count(*)聚合函数)
* 聚合（max）==(max(*)聚合函数)
* 聚合（min）==(min(*)聚合函数)
* 聚合（avg）==(avg(*)聚合函数)
* 聚合（sum）==(sum(*)聚合函数)
* 聚合（top_hits）==保存聚合后记录
* @param searchDocDto
* @throws IOException
*/
public void search15(SearchDocDto searchDocDto) throws IOException {
    
    SearchRequest searchRequest = new SearchRequest(searchDocDto.getIndexName());
    //构建查询
    SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
    //聚合
    TermsAggregationBuilder ageAgg = AggregationBuilders.terms("ageAgg");
    //设置聚合字段
    ageAgg.field(searchDocDto.getAggField());
    //设置展示聚合分布的结果数量
    ageAgg.size(searchDocDto.getAggSize());
    //设置聚合结果文档数下限
    ageAgg.minDocCount(searchDocDto.getAggMinDocCount());
    //设置排序
    BucketOrder count = BucketOrder.count(true);
    ageAgg.order(count);
    
    //设置子聚合
    AggregatorFactories.Builder builder = AggregatorFactories.builder();
    //设置平均值聚合函数
    AvgAggregationBuilder balanceAvgAgg = AggregationBuilders.avg("balanceAvgAgg");
    balanceAvgAgg.field("balance2");
    builder.addAggregator(balanceAvgAgg);
    
    //设置最大值聚合函数
    MaxAggregationBuilder maxAgg = AggregationBuilders.max("balanceMaxAgg");
    maxAgg.field("balance2");
    builder.addAggregator(maxAgg);
    
    //设置最小值聚合函数
    MinAggregationBuilder minAgg = AggregationBuilders.min("balanceMinAgg");
    minAgg.field("balance2");
    builder.addAggregator(minAgg);
    
    //设置总数聚合函数
    SumAggregationBuilder sumAggregationBuilder = AggregationBuilders.sum("balanceSumAgg");
    sumAggregationBuilder.field("balance2");
    builder.addAggregator(sumAggregationBuilder);
    
    //设置聚合中各值对应记录
    TopHitsAggregationBuilder aggRecord = AggregationBuilders.topHits("aggRecord");
    aggRecord.size(1);
    builder.addAggregator(aggRecord);
    
    ageAgg.subAggregations(builder);
    
    searchSourceBuilder.aggregation(ageAgg);
    
    searchRequest.source(searchSourceBuilder);
    
    log.info("查询条件"+searchSourceBuilder.toString());
    
    SearchResponse search = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);
    
    //获取聚合结果
    Aggregations aggregations = search.getAggregations();
    
    for (Aggregation aggr:aggregations.asList()){
        ParsedLongTerms parsedLongTerms=(ParsedLongTerms)aggr;
        List<? extends Terms.Bucket> buckets = parsedLongTerms.getBuckets();
        for(Terms.Bucket bucket:buckets){
            log.info(JSONUtil.toJsonStr(bucket));
        }
    }
}
```
<a name="kSOIv"></a>
#### Elasticsearch
```json
GET test-index-20220228/_search
{
  "aggregations": {
    "ageAgg": {
      "terms": {
        "field": "age",
        "size": 10,
        "min_doc_count": 1,
        "shard_min_doc_count": 0,
        "show_term_doc_count_error": false,
        "order": [
          {
            "_count": "asc"
          },
          {
            "_key": "asc"
          }
        ]
      },
      "aggregations": {
        "balanceAvgAgg": {
          "avg": {
            "field": "balance2"
          }
        },
        "balanceMaxAgg": {
          "max": {
            "field": "balance2"
          }
        },
        "balanceMinAgg": {
          "min": {
            "field": "balance2"
          }
        },
        "balanceSumAgg": {
          "sum": {
            "field": "balance2"
          }
        },
        "aggRecord": {
          "top_hits": {
            "from": 0,
            "size": 1,
            "version": false,
            "seq_no_primary_term": false,
            "explain": false
          }
        }
      }
    }
  }
}
```
说明：

1. term 聚合类型相当于 group by 可以用于分布数据, 当然可以嵌套 term, 相当于根据多个字段分组
2. term 聚合后得到的聚合结果会有基本的文档数量
3. max,min,sum,avg 相当于一些计算聚合函数
<a name="LCCIc"></a>
### 常用聚合 2(聚合条件过滤)
<a name="b2trB"></a>
#### SQL
```sql
select count(*) count from user where age>20 group by name,age having count>1
```
<a name="RP68E"></a>
#### Java API
```java
/**
 * 聚合（bucket_selector）==(having in)聚合函数)
 * @param searchDocDto
 * @throws IOException
 */
public void search17(SearchDocDto searchDocDto) throws IOException {

    SearchRequest searchRequest = new SearchRequest(searchDocDto.getIndexName());
    //构建查询
    SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
    //聚合term聚合
    TermsAggregationBuilder ageAgg = AggregationBuilders.terms("ageAgg");
    ageAgg.field(searchDocDto.getAggField());
    searchSourceBuilder.aggregation(ageAgg);

    //设置子聚合
    AggregatorFactories.Builder builder = AggregatorFactories.builder();
    //设置bucket_selector
    Map<String, String> bucketsPath = new HashMap<>();
    bucketsPath.put("count","_count");
    BucketSelectorPipelineAggregationBuilder pipelineAggregationBuilder = new BucketSelectorPipelineAggregationBuilder
            ("filterAgg",bucketsPath, Script.parse("params.count>1"));
    builder.addPipelineAggregator(pipelineAggregationBuilder);

    ageAgg.subAggregations(builder);

    searchRequest.source(searchSourceBuilder);

    log.info("查询条件"+searchSourceBuilder.toString());

    SearchResponse search = restHighLevelClient.search(searchRequest, RequestOptions.DEFAULT);

    Aggregations aggregations = search.getAggregations();

    for (Aggregation aggr:aggregations.asList()){
        ParsedLongTerms parsedLongTerms=(ParsedLongTerms)aggr;
        List<? extends Terms.Bucket> buckets = parsedLongTerms.getBuckets();
        for(Terms.Bucket bucket:buckets){
            log.info(JSONUtil.toJsonStr(bucket));
        }
    }
}
```
<a name="lzjI4"></a>
#### Elasticsearch
```json
GET test-index-20220228/_search
{
  "aggregations": {
    "ageAgg": {
      "terms": {
        "field": "age",
        "size": 10,
        "min_doc_count": 1,
        "shard_min_doc_count": 0,
        "show_term_doc_count_error": false,
        "order": [
          {
            "_count": "desc"
          },
          {
            "_key": "asc"
          }
        ]
      },
      "aggregations": {
        "filterAgg": {
          "bucket_selector": {
            "buckets_path": {
              "count": "_count"
            },
            "script": {
              "source": "params.count>1",
              "lang": "painless"
            },
            "gap_policy": "skip"
          }
        }
      }
    }
  }
}
```
说明：bucket_selector 类型聚合可以将上一级聚合桶进行条件过滤
<a name="tDWkD"></a>
## 总结

1. 有基本的 term,match,range 查询
2. should,must,must not filter 复合可以内部使用基本查询
3. should,must,must not filter 内部可以组合 bool 实现更复杂的复合查询
4. aggregations 实现分组和聚合函数计算
