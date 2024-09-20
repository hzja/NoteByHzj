JavaSpringBootElasticsearch<br />这里使用的SpringBoot版本号是2.1.0.RELEASE，服务端 es 的版本号是6.8.2，客户端采用的是官方推荐的Elastic Java High Level Rest Client版本号是6.4.2，方便与SpringBoot的版本兼容。<br />es 最大的亮点就是查询非常丰富，可以在上亿的数据里面快速搜索出目标数据，查询如果实现呢？请看下面的内容
<a name="NViKH"></a>
### 单条件精确查询
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 单条件精确查询
     * @throws IOException
     */
    @Test
    public void search0() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.termsQuery("name", "赵里"));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="vEdtW"></a>
### 多条件精确查询，取并集
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 多条件精确查询，取并集
     * @throws IOException
     */
    @Test
    public void search1() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.termsQuery("name", "张", "陈"));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="NXUo7"></a>
### 范围查询
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 范围查询，包括from、to
     * @throws IOException
     */
    @Test
    public void search2() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.rangeQuery("age").from(20).to(32));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }


    /**
     * 范围查询，不包括from、to
     * @throws IOException
     */
    @Test
    public void search3() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.rangeQuery("age").from(20,false).to(30, false));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }


    /**
     * 范围查询, lt:小于，gt:大于
     * @throws IOException
     */
    @Test
    public void search4() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.rangeQuery("age").lt(30).gt(20));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="FurPp"></a>
### 模糊查询，支持通配符
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 模糊查询，支持通配符
     * @throws IOException
     */
    @Test
    public void search5() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.wildcardQuery("name","张三"));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="WxB18"></a>
### 不使用通配符的模糊查询，左右匹配
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 不使用通配符的模糊查询，左右匹配
     * @throws IOException
     */
    @Test
    public void search6() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.queryStringQuery("张三").field("name"));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="CRgmn"></a>
### 多字段模糊查询
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 多字段模糊查询
     * @throws IOException
     */
    @Test
    public void search7() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.multiMatchQuery("长", "name", "city"));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="odCrO"></a>
### 多字段模糊查询
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 分页搜索
     * @throws IOException
     */
    @Test
    public void search8() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .from(0).size(2);

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="jkuhd"></a>
### 字段排序
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 排序，字段的类型必须是：integer、double、long或者keyword
     * @throws IOException
     */
    @Test
    public void search9() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .sort("createTime", SortOrder.ASC);

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="iYAfw"></a>
### 精确统计筛选文档数
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 精确统计筛选文档数,查询性能有所降低
     * @throws IOException
     */
    @Test
    public void search10() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .trackTotalHits(true);

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="VVplK"></a>
### 设置源字段过滤返回
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 设置源字段过虑,第一个参数结果集包括哪些字段，第二个参数表示结果集不包括哪些字段
     * @throws IOException
     */
    @Test
    public void search11() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .fetchSource(new String[]{"name","age","city","createTime"},new String[]{});

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="yYIHn"></a>
### 根据 id 精确匹配
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 根据id精确匹配
     * @throws IOException
     */
    @Test
    public void search12() throws IOException {
        String[] ids = new String[]{"1","2"};
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.termsQuery("_id", ids));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }

}
```
<a name="UTFbG"></a>
### matchAllQuery 搜索全部
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * matchAllQuery搜索全部
     * @throws IOException
     */
    @Test
    public void search21() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.matchAllQuery());

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }
}
```
<a name="D1rTZ"></a>
### match 搜索匹配
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * match搜索匹配
     * @throws IOException
     */
    @Test
    public void search22() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder()
                .query(QueryBuilders.matchQuery("name", "张王"));

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }
}
```
<a name="PY3sT"></a>
### bool组合查询
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * bool组合查询
     * @throws IOException
     */
    @Test
    public void search23() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder();

        BoolQueryBuilder boolQueryBuilder=new BoolQueryBuilder();
        boolQueryBuilder.must(QueryBuilders.matchQuery("name", "张王"));
        boolQueryBuilder.must(QueryBuilders.rangeQuery("age").lte(30).gte(20));
        builder.query(boolQueryBuilder);

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }
}
```
<a name="w3LtP"></a>
### nested 类型嵌套查询
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * nested类型嵌套查询
     * @throws IOException
     */
    @Test
    public void search24() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder();

        //条件查询
        BoolQueryBuilder mainBool=new BoolQueryBuilder();
        mainBool.must(QueryBuilders.matchQuery("name", "赵六"));

        //nested类型嵌套查询
        BoolQueryBuilder boolQueryBuilder=new BoolQueryBuilder();
        boolQueryBuilder.must(QueryBuilders.matchQuery("products.brand", "A"));
        boolQueryBuilder.must(QueryBuilders.matchQuery("products.title", "巧克力"));
        NestedQueryBuilder nested = QueryBuilders.nestedQuery("products",boolQueryBuilder, ScoreMode.None);
        mainBool.must(nested);

        builder.query(mainBool);

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }
}
```
<a name="t1hwH"></a>
### 多条件查询 + 排序 + 分页
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 多条件查询 + 排序 + 分页
     * @throws IOException
     */
    @Test
    public void search29() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder();

        //条件搜索
        BoolQueryBuilder boolQueryBuilder=new BoolQueryBuilder();
        boolQueryBuilder.must(QueryBuilders.matchQuery("name", "张王"));
        boolQueryBuilder.must(QueryBuilders.rangeQuery("age").lte(30).gte(20));
        builder.query(boolQueryBuilder);

        //结果集合分页
        builder.from(0).size(2);

        //排序
        builder.sort("createTime",SortOrder.ASC);

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }
}
```
<a name="Cd34J"></a>
### 聚合查询-求和
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 聚合查询 sum
     * @throws IOException
     */
    @Test
    public void search30() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder();

        //条件搜索
        builder.query(QueryBuilders.matchAllQuery());
        //聚合查询
        AggregationBuilder aggregation = AggregationBuilders.sum("sum_age").field("age");
        builder.aggregation(aggregation);

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }
}
```
<a name="J5Cnm"></a>
### 聚合查询-求平均值
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 聚合查询 avg
     * @throws IOException
     */
    @Test
    public void search31() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder();

        //条件搜索
        builder.query(QueryBuilders.matchAllQuery());
        //聚合查询
        AggregationBuilder aggregation = AggregationBuilders.avg("avg_age").field("age");
        builder.aggregation(aggregation);

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }
}
```
<a name="TlHBF"></a>
### 聚合查询-计数
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {

    @Autowired
    private RestHighLevelClient client;

    /**
     * 聚合查询 count
     * @throws IOException
     */
    @Test
    public void search32() throws IOException {
        // 创建请求
        SearchSourceBuilder builder = new SearchSourceBuilder();

        //条件搜索
        builder.query(QueryBuilders.matchAllQuery());
        //聚合查询
        AggregationBuilder aggregation = AggregationBuilders.count("count_age").field("age");
        builder.aggregation(aggregation);

        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices("cs_index");
        searchRequest.types("_doc");
        searchRequest.source(builder);
        // 执行请求
        SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
        // 解析查询结果
        System.out.println(response.toString());
    }
}
```
<a name="bhTHk"></a>
### 聚合查询-分组
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class SearchJunit {
	
	@Autowired
	private RestHighLevelClient client;
	
	/**
	* 聚合查询 分组
	* @throws IOException
	*/
	@Test
	public void search33() throws IOException {
		// 创建请求
		SearchSourceBuilder builder = new SearchSourceBuilder();
		
		//条件搜索
		builder.query(QueryBuilders.matchAllQuery());
		//聚合查询
		AggregationBuilder aggregation = AggregationBuilders.terms("tag_createTime").field("createTime")
			.subAggregation(AggregationBuilders.count("count_age").field("age")) //计数
			.subAggregation(AggregationBuilders.sum("sum_age").field("age")) //求和
			.subAggregation(AggregationBuilders.avg("avg_age").field("age")); //求平均值
		
		builder.aggregation(aggregation);
		
		//不输出原始数据
		builder.size(0);
		
		//搜索
		SearchRequest searchRequest = new SearchRequest();
		searchRequest.indices("cs_index");
		searchRequest.types("_doc");
		searchRequest.source(builder);
		// 执行请求
		SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
		// 解析查询结果
		System.out.println(response.toString());
	}
}
```
