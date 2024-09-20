JavaSpringBoot
<a name="hhZ4X"></a>
## 一、摘要
**如何在后端通过技术方式快速的实现 es 中内嵌对象的数据查询呢**？<br />为了方便更容易掌握技术，以**通过商品找订单**为案例，利用 SpringBoot 整合 ES 实现这个业务需求，介绍具体的技术实践方案，存入es中的json数据结构如下：
```json
{
	"orderId":"1",
	"orderNo":"123456",
	"orderUserName":"张三",
	"orderItems":[
		{
			"orderItemId":"12234",
			"orderId":"1",
			"productName":"火腿肠",
			"brandName":"双汇",
			"sellPrice":"28"
		},
		{
			"orderItemId":"12235",
			"orderId":"1",
			"productName":"果冻",
			"brandName":"汇源",
			"sellPrice":"12"
		}
	]
}
```
<a name="g81I0"></a>
## 二、项目实践
<a name="EdRWp"></a>
### 2.1、添加依赖
在SpringBoot项目中，添加rest-high-level-client客户端，方便与 ES 服务器连接通信，在这里需要注意一下，推荐客户端的版本与 ES 服务器的版本号一致，不然会出现接口请求错误等异常！<br />本次安装的ES服务端版本号为6.8.2，因此客户端也保持6.8.2，与之一致！
```xml
<!--elasticsearch-->
<dependency>
	<groupId>org.elasticsearch</groupId>
	<artifactId>elasticsearch</artifactId>
	<version>6.8.2</version>
</dependency>
<dependency>
	<groupId>org.elasticsearch.client</groupId>
	<artifactId>elasticsearch-rest-client</artifactId>
	<version>6.8.2</version>
</dependency>
<dependency>
	<groupId>org.elasticsearch.client</groupId>
	<artifactId>elasticsearch-rest-high-level-client</artifactId>
	<version>6.8.2</version>
</dependency>
```
<a name="fuM9v"></a>
### 2.2、配置 es 客户端
为了更佳方便的使用 es，可以将其各个配置类进行封装，方便后续进行维护。
<a name="oWwti"></a>
#### 在application.properties配置文件中，定义 es 配置连接地址
```
# 设置es参数
elasticsearch.scheme=http
elasticsearch.address=127.0.0.1:9200
elasticsearch.userName=
elasticsearch.userPwd=
elasticsearch.socketTimeout=5000
elasticsearch.connectTimeout=5000
elasticsearch.connectionRequestTimeout=5000
```
<a name="cqojJ"></a>
#### 创建ElasticSearch配置类，方便SpringBoot启动时注入
```java
import org.apache.http.HttpHost;
import org.apache.http.auth.AuthScope;
import org.apache.http.auth.UsernamePasswordCredentials;
import org.apache.http.client.CredentialsProvider;
import org.apache.http.impl.client.BasicCredentialsProvider;
import org.elasticsearch.client.RestClient;
import org.elasticsearch.client.RestClientBuilder;
import org.elasticsearch.client.RestHighLevelClient;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import java.util.Arrays;
import java.util.Objects;


@Configuration
public class ElasticSearchConfiguration {

    private static final Logger log = LoggerFactory.getLogger(ElasticSearchConfiguration.class);


    private static final int ADDRESS_LENGTH = 2;

    @Value("${elasticsearch.scheme:http}")
    private String scheme;

    @Value("${elasticsearch.address}")
    private String address;

    @Value("${elasticsearch.userName}")
    private String userName;

    @Value("${elasticsearch.userPwd}")
    private String userPwd;

    @Value("${elasticsearch.socketTimeout:5000}")
    private Integer socketTimeout;

    @Value("${elasticsearch.connectTimeout:5000}")
    private Integer connectTimeout;

    @Value("${elasticsearch.connectionRequestTimeout:5000}")
    private Integer connectionRequestTimeout;

    /**
     * 初始化客户端
     * @return
     */
    @Bean(name = "restHighLevelClient")
    public RestHighLevelClient restClientBuilder() {
        HttpHost[] hosts = Arrays.stream(address.split(","))
                .map(this::buildHttpHost)
                .filter(Objects::nonNull)
                .toArray(HttpHost[]::new);
        RestClientBuilder restClientBuilder = RestClient.builder(hosts);
        // 异步参数配置
        restClientBuilder.setHttpClientConfigCallback(httpClientBuilder -> {
            httpClientBuilder.setDefaultCredentialsProvider(buildCredentialsProvider());
            return httpClientBuilder;
        });

        // 异步连接延时配置
        restClientBuilder.setRequestConfigCallback(requestConfigBuilder -> {
            requestConfigBuilder.setConnectionRequestTimeout(connectionRequestTimeout);
            requestConfigBuilder.setSocketTimeout(socketTimeout);
            requestConfigBuilder.setConnectTimeout(connectTimeout);
            return requestConfigBuilder;
        });

        return new RestHighLevelClient(restClientBuilder);
    }


    /**
     * 根据配置创建HttpHost
     * @param s
     * @return
     */
    private HttpHost buildHttpHost(String s) {
        String[] address = s.split(":");
        if (address.length == ADDRESS_LENGTH) {
            String ip = address[0];
            int port = Integer.parseInt(address[1]);
            return new HttpHost(ip, port, scheme);
        } else {
            return null;
        }
    }

    /**
     * 构建认证服务
     * @return
     */
    private CredentialsProvider buildCredentialsProvider(){
        final CredentialsProvider credentialsProvider = new BasicCredentialsProvider();
        credentialsProvider.setCredentials(AuthScope.ANY, new UsernamePasswordCredentials(userName,
                userPwd));
        return credentialsProvider;
    }
}
```
<a name="xXDGP"></a>
#### 封装ElasticSearch客户端服务类，方便公共调用处理
```java
import com.fasterxml.jackson.databind.ObjectMapper;
import org.example.es.exception.CommonException;
import org.apache.commons.lang3.StringUtils;
import org.elasticsearch.action.admin.indices.alias.get.GetAliasesRequest;
import org.elasticsearch.action.admin.indices.create.CreateIndexRequest;
import org.elasticsearch.action.admin.indices.create.CreateIndexResponse;
import org.elasticsearch.action.admin.indices.delete.DeleteIndexRequest;
import org.elasticsearch.action.admin.indices.get.GetIndexRequest;
import org.elasticsearch.action.admin.indices.get.GetIndexResponse;
import org.elasticsearch.action.admin.indices.mapping.get.GetMappingsRequest;
import org.elasticsearch.action.admin.indices.mapping.get.GetMappingsResponse;
import org.elasticsearch.action.admin.indices.mapping.put.PutMappingRequest;
import org.elasticsearch.action.delete.DeleteRequest;
import org.elasticsearch.action.delete.DeleteResponse;
import org.elasticsearch.action.get.GetRequest;
import org.elasticsearch.action.get.GetResponse;
import org.elasticsearch.action.index.IndexRequest;
import org.elasticsearch.action.index.IndexResponse;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.action.support.master.AcknowledgedResponse;
import org.elasticsearch.action.update.UpdateRequest;
import org.elasticsearch.action.update.UpdateResponse;
import org.elasticsearch.client.GetAliasesResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.xcontent.XContentType;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import java.io.IOException;
import java.util.Collections;
import java.util.Map;
import java.util.Set;

@Component
public class ElasticSearchClient {

    private static final Logger log = LoggerFactory.getLogger(ElasticSearchClient.class);

    private static final ObjectMapper objectMapper = new ObjectMapper();

    @Autowired
    private RestHighLevelClient client;


    /**
     * 查询全部索引
     * @return
     */
    public Set<String> getAlias(){
        try {
            GetAliasesRequest request = new GetAliasesRequest();
            GetAliasesResponse response =  client.indices().getAlias(request, RequestOptions.DEFAULT);
            return response.getAliases().keySet();
        } catch (IOException e) {
            log.error("向es发起查询全部索引信息请求失败", e);
        }
        return Collections.emptySet();
    }

    /**
     * 检查索引是否存在
     * @param indexName
     * @return
     */
    public boolean existsIndex(String indexName){
        try {
            // 创建请求
            GetIndexRequest request = new GetIndexRequest().indices(indexName);
            // 执行请求,获取响应
            boolean response = client.indices().exists(request, RequestOptions.DEFAULT);
            return response;
        } catch (Exception e) {
            log.error("向es发起查询索引是否存在请求失败，请求参数：" + indexName, e);
        }
        return false;
    }


    /**
     * 查询索引
     * @param indexName
     * @return
     */
    public String getIndex(String indexName){
        try {
            // 创建请求
            GetIndexRequest request = new GetIndexRequest().indices(indexName);
            // 执行请求,获取响应
            GetIndexResponse response = client.indices().get(request, RequestOptions.DEFAULT);
            return response.toString();
        } catch (Exception e) {
            log.error("向es发起查询索引请求失败，请求参数：" + indexName, e);
        }
        return StringUtils.EMPTY;
    }

    /**
     * 创建索引
     * @param indexName
     * @param mapping
     * @return
     */
    public void createIndex(String indexName, Map<String, Object> mapping){
        try {
            CreateIndexRequest request = new CreateIndexRequest();
            //索引名称
            request.index(indexName);
            //索引配置
            Settings settings = Settings.builder()
                    .put("index.number_of_shards", 3)
                    .put("index.number_of_replicas", 1)
                    .put("index.max_inner_result_window", 5000)
                    .build();
            request.settings(settings);
            //索引结构
            request.mapping("_doc",mapping);
            //执行请求,获取响应
            CreateIndexResponse response = client.indices().create(request, RequestOptions.DEFAULT);
            if(!response.isAcknowledged()){
                throw new CommonException("向es发起创建索引请求失败");
            }
            log.info("向es发起创建索引请求成功，返回参数：{}", response.index());
        } catch (Exception e) {
            log.error("向es发起创建索引请求失败，请求参数：" + indexName, e);
            throw new CommonException("向es发起创建索引请求失败");
        }
    }


    /**
     * 删除索引
     * @param indexName
     * @return
     */
    public void deleteIndex(String indexName){
        try {
            DeleteIndexRequest request = new DeleteIndexRequest(indexName);
            AcknowledgedResponse response = client.indices().delete(request, RequestOptions.DEFAULT);
            if(!response.isAcknowledged()){
                throw new CommonException("向es发起删除索引请求失败");
            }
            log.info("向es发起删除索引请求成功，请求参数：{}", indexName);
        } catch (Exception e) {
            log.error("向es发起删除索引请求失败，请求参数：" + indexName, e);
            throw new CommonException("向es发起删除索引请求失败");
        }
    }


    /**
     * 查询索引映射字段
     * @param indexName
     * @return
     */
    public String getMapping(String indexName){
        try {
            GetMappingsRequest request = new GetMappingsRequest().indices(indexName).types("_doc");
            GetMappingsResponse response = client.indices().getMapping(request, RequestOptions.DEFAULT);
            return response.toString();
        } catch (Exception e) {
            log.error("向es发起查询索引映射字段请求失败，请求参数：" + indexName, e);
        }
        return StringUtils.EMPTY;
    }


    /**
     * 添加索引映射字段
     * @param indexName
     * @return
     */
    public void addMapping(String indexName, Map<String, Object> mapping){
        try {
            PutMappingRequest request = new PutMappingRequest();
            request.indices(indexName);
            request.type("_doc");
            //添加字段
            request.source(mapping);
            AcknowledgedResponse response = client.indices().putMapping(request, RequestOptions.DEFAULT);
            if(!response.isAcknowledged()){
                throw new CommonException("向es发起添加索引映射字段请求失败");
            }
            log.info("向es发起添加索引映射字段请求成功，请求参数：{}", toJson(request));
        } catch (Exception e) {
            log.error("向es发起添加索引映射字段请求失败，请求参数：" + indexName, e);
            throw new CommonException("向es发起添加索引映射字段请求失败");
        }
    }


    /**
     * 向索引中添加文档
     * @param indexName
     * @param id
     * @param obj
     */
    public void addDocument(String indexName, String id, Object obj){
        try {
            //向索引中添加文档
            IndexRequest request = new IndexRequest();
            // 外层参数
            request.id(id);
            request.index(indexName);
            request.type("_doc");

            // 存入对象
            request.source(toJson(obj), XContentType.JSON);
            // 发送请求
            IndexResponse response = client.index(request, RequestOptions.DEFAULT);
            if(response.status().getStatus() >= 400){
                log.warn("向es发起添加文档数据请求失败，请求参数：{}，返回参数：{}", request.toString(), response.toString());
                throw new CommonException("向es发起添加文档数据请求失败");
            }
        } catch (Exception e) {
            log.error("向es发起添加文档数据请求失败，请求参数：" + indexName, e);
            throw new CommonException("向es发起添加文档数据请求失败");
        }
    }

    /**
     * 修改索引中的文档数据
     * @param indexName
     * @param id
     * @param obj
     */
    public void updateDocument(String indexName, String id, Map<String,Object> obj){
        try {
            //修改索引中的文档数据
            UpdateRequest request = new UpdateRequest();
            // 外层参数
            request.id(id);
            request.index(indexName);
            request.type("_doc");
            // 存入对象
            request.doc(obj);
            request.doc(toJson(obj), XContentType.JSON);
            // 发送请求
            UpdateResponse response = client.update(request, RequestOptions.DEFAULT);
            if(response.status().getStatus() >= 400){
                log.warn("向es发起修改文档数据请求失败，请求参数：{}，返回参数：{}", request.toString(), response.toString());
                throw new CommonException("向es发起修改文档数据请求失败");
            }
        } catch (Exception e) {
            log.error("向es发起修改文档数据请求失败，请求参数：" + indexName, e);
            throw new CommonException("向es发起修改文档数据请求失败");
        }
    }



    /**
     * 删除索引中的文档数据
     * @param indexName
     * @param id
     */
    public void deleteDocument(String indexName, String id){
        try {
            //删除索引中的文档数据
            DeleteRequest request = new DeleteRequest();
            // 外层参数
            request.id(id);
            request.index(indexName);
            request.type("_doc");
            // 发送请求
            DeleteResponse response = client.delete(request, RequestOptions.DEFAULT);
            if(response.status().getStatus() >= 400){
                log.warn("向es发起删除文档数据请求失败，请求参数：{}，返回参数：{}", request.toString(), response.toString());
                throw new CommonException("向es发起删除文档数据请求失败");
            }
        } catch (Exception e) {
            log.error("向es发起删除文档数据请求失败，请求参数：" + indexName, e);
            throw new CommonException("向es发起删除文档数据请求失败");
        }
    }


    /**
     * 查询索引中的文档数据
     * @param indexName
     * @param id
     */
    public String getDocumentById(String indexName, String id){
        try {
            GetRequest request = new GetRequest();
            // 外层参数
            request.id(id);
            request.index(indexName);
            request.type("_doc");
            // 发送请求
            GetResponse response = client.get(request, RequestOptions.DEFAULT);
            response.getSourceAsString();
        } catch (Exception e) {
            log.error("向es发起查询文档数据请求失败，请求参数：" + indexName, e);
        }
        return StringUtils.EMPTY;
    }


    /**
     * 索引高级查询
     * @param indexName
     * @param source
     * @return
     */
    public SearchResponse searchDocument(String indexName, SearchSourceBuilder source){
        //搜索
        SearchRequest searchRequest = new SearchRequest();
        searchRequest.indices(indexName);
        searchRequest.source(source);
        try {
            // 执行请求
            SearchResponse response = client.search(searchRequest, RequestOptions.DEFAULT);
            return response;
        } catch (Exception e) {
            log.warn("向es发起查询文档数据请求失败，请求参数：" + searchRequest.toString(), e);
        }
        return null;
    }


    /**
     * 将对象格式化成json，并保持原字段类型输出
     * @param object
     * @return
     */
    private String toJson(Object object) {
        try {
            return objectMapper.writeValueAsString(object);
        } catch (Exception e) {
            throw new CommonException(e);
        }
    }

}
```
<a name="AObTn"></a>
### 2.3、初始化索引结构
在使用 es 对订单进行查询搜索时，需要先定义好对应的订单索引结构，内容如下：
```java
@ActiveProfiles("dev")
@RunWith(SpringRunner.class)
@SpringBootTest
public class OrderIndexServiceJunit {

    @Autowired
    private ElasticSearchClient elasticSearchClient;
    
    /**
     * 初始化索引结构
     *
     * @return
     */
    @Test
    public void initIndex(){
        String indexName = "orderIndex-2022-07";
        // 创建请求
        boolean existsIndex = elasticSearchClient.existsIndex(indexName);
        if (!existsIndex) {
            Map<String, Object> properties = buildMapping();
            elasticSearchClient.createIndex(indexName, properties);
        }
    }

    /**
     * 构建索引结构
     *
     * @return
     */
    private Map<String, Object> buildMapping() {
        Map<String, Object> properties = new HashMap();
        //订单id  唯一键ID
        properties.put("orderId", ImmutableBiMap.of("type", "keyword"));
        //订单号
        properties.put("orderNo", ImmutableBiMap.of("type", "keyword"));
        //客户姓名
        properties.put("orderUserName", ImmutableBiMap.of("type", "text"));
        
        //订单项
        Map<String, Object> orderItems = new HashMap();
        //订单项ID
        orderItems.put("orderItemId", ImmutableBiMap.of("type", "keyword"));
        //产品名称
        orderItems.put("productName", ImmutableBiMap.of("type", "text"));
        //品牌名称
        orderItems.put("brandName", ImmutableBiMap.of("type", "text"));
        //销售金额,单位分*100
        orderItems.put("sellPrice", ImmutableBiMap.of("type", "integer"));
        properties.put("orderItems", ImmutableBiMap.of("type", "nested", "properties", orderItems));

        //文档结构映射
        Map<String, Object> mapping = new HashMap();
        mapping.put("properties", properties);
        return mapping;
    }
}
```
<a name="ePQDD"></a>
### 2.4、向 es 中同步文档数据
索引结构创建好之后，需要将支持 es 搜索的订单数据同步进去。<br />将指定的订单 ID 从数据库查询出来，并封装成 es 订单数据结构，保存到 es 中！
```java
@ActiveProfiles("dev")
@RunWith(SpringRunner.class)
@SpringBootTest
public class OrderIndexServiceJunit {

    @Autowired
    private ElasticSearchClient elasticSearchClient;


    /**
     * 保存订单到ES中
     * @param request
     */
    @Test
    public void saveDocument(){
        String indexName =  "orderIndex-2022-07";
        //从数据库查询最新订单数据，并封装成对应的es订单结构
        String orderId = "202202020202";
        OrderIndexDocDTO indexDocDTO = buildOrderIndexDocDTO(orderId);
        //保存数据到ES中
        elasticSearchClient.addDocument(indexName, indexDocDTO.getOrderId(), indexDocDTO);
    }

}
```
<a name="w9otf"></a>
### 2.5、内嵌对象查询
内嵌对象查询分两种形式，比如，**第一种通过商品、品牌、价格等条件，分页查询订单数据**；**第二种是通过订单ID、商品、品牌、价格等，分页查询订单项数据**。具体的实践，请看下文。
<a name="lXTHn"></a>
#### 通过商品、品牌、价格等条件，分页查询订单数据
```java
@ActiveProfiles("dev")
@RunWith(SpringRunner.class)
@SpringBootTest
public class OrderIndexServiceJunit {

    @Autowired
    private ElasticSearchClient elasticSearchClient;


    /**
     * 通过商品、品牌、价格等条件，分页查询订单数据
     * @param request
     */
    @Test
    public void search1(){
        //查询索引，支持通配符
        String indexName = "orderIndex-*";
        String orderUserName = "张三";
        String productName = "薯条";
        // 条件搜索
        SearchSourceBuilder builder = new SearchSourceBuilder();

        //组合搜索
        BoolQueryBuilder mainBoolQuery = new BoolQueryBuilder();
        mainBoolQuery.must(QueryBuilders.matchQuery("orderUserName", orderUserName));

        //订单项相关信息搜索
        BoolQueryBuilder nestedBoolQuery = new BoolQueryBuilder(); nestedBoolQuery.must(QueryBuilders.matchQuery("orderItems.productName", productName));
        //内嵌对象搜索，需要指定path
        NestedQueryBuilder nestedQueryBuilder = QueryBuilders.nestedQuery("orderItems",nestedBoolQuery, ScoreMode.None);
        //子表查询
        mainBoolQuery.must(nestedQueryBuilder);

        //封装查询参数
        builder.query(mainBoolQuery);

        //返回参数
        builder.fetchSource(new String[]{}, new String[]{});

        //结果集合分页，从第一页开始，返回最多四条数据
        builder.from(0).size(4);

        //排序
        builder.sort("orderId", SortOrder.DESC);
        log.info("dsl：{}", builder.toString());
        // 执行请求
        SearchResponse response = elasticSearchClient.searchDocument(indexName, builder);
        // 当前返回的总行数
        long count = response.getHits().getTotalHits();
        // 返回的具体行数
        SearchHit[] searchHits = response.getHits().getHits();
  log.info("response：{}", response.toString());
    }

}
```
<a name="Js4gr"></a>
#### 通过订单ID、商品、品牌、价格等，分页查询订单项数据
```java
@ActiveProfiles("dev")
@RunWith(SpringRunner.class)
@SpringBootTest
public class OrderIndexServiceJunit {
	
	@Autowired
	private ElasticSearchClient elasticSearchClient;
	
	
	/**
	* 通过订单ID、商品、品牌、价格等，分页查询订单项数据
	* @param request
	*/
	@Test
	public void search2(){
		//查询索引，支持通配符
		String indexName = "orderIndex-*";
		String orderId = "202202020202";
		String productName = "薯条";
		// 条件搜索
		SearchSourceBuilder builder = new SearchSourceBuilder();
		
		//组合搜索
		BoolQueryBuilder mainBoolQuery = new BoolQueryBuilder();
		mainBoolQuery.must(QueryBuilders.termQuery("_id", orderId));
		
		//订单项相关信息搜索
		BoolQueryBuilder nestedBoolQuery = new BoolQueryBuilder(); nestedBoolQuery.must(QueryBuilders.matchQuery("orderItems.productName", productName));
		//内嵌对象搜索，需要指定path
		NestedQueryBuilder nestedQueryBuilder = QueryBuilders.nestedQuery("orderItems",nestedBoolQuery, ScoreMode.None);
		
		//内嵌对象分页查询
		InnerHitBuilder innerHitBuilder = new InnerHitBuilder();
		//结果集合分页，从第一页开始，返回最多四条数据
		innerHitBuilder.setFrom(0).setSize(4);
		//只返回订单项id
		innerHitBuilder.setFetchSourceContext(new FetchSourceContext(true, new String[]{"orderItems.orderItemId"}, new String[]{}));
		innerHitBuilder.addSort(SortBuilders.fieldSort("orderItems.orderItemId").order(SortOrder.DESC));
		nestedQueryBuilder.innerHit(innerHitBuilder);
		
		//子表查询
		mainBoolQuery.must(nestedQueryBuilder);
		
		//封装查询参数
		builder.query(mainBoolQuery);
		
		//返回参数
		builder.fetchSource(new String[]{}, new String[]{});
		
		//结果集合分页，从第一页开始，返回最多四条数据
		builder.from(0).size(4);
		
		//排序
		builder.sort("orderId", SortOrder.DESC);
		log.info("dsl：{}", builder.toString());
		// 执行请求
		SearchResponse response = elasticSearchClient.searchDocument(indexName, builder);
		// 当前返回的订单主表总行数
		long count = response.getHits().getTotalHits();
		// 返回的订单主表数据
		SearchHit[] searchHits = response.getHits().getHits();
		// 返回查询的的订单项分页数据
		Map<String, SearchHits> = searchHit[0].getInnerHits();
		log.info("response：{}", response.toString());
	}
	
}
```
<a name="k5Ieh"></a>
## 三、小结
主要通过商品名称查询订单数据为案例，介绍利用 SpringBoot 整合 es 实现数据的高效搜索！
