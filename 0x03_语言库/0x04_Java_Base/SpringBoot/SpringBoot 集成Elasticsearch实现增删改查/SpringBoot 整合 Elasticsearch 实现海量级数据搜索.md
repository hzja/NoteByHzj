JavaSpringBootElasticsearch
<a name="Pv6ZK"></a>
## 一、简介
实际的项目开发过程中，通常基于某些主流框架平台进行技术开发，比如 SpringBoot，以 SpringBoot 整合 ElasticSearch 为例，这里详细的介绍 ElasticSearch 的使用！<br />SpringBoot 连接 ElasticSearch，主流的方式有以下四种方式

- 方式一：通过Elastic Transport Client客户端连接 es 服务器，底层基于 TCP 协议通过 transport 模块和远程 ES 服务端通信，不过，从 V7.0 开始官方不建议使用，V8.0开始正式移除。
- 方式二：通过Elastic Java Low Level Rest Client客户端连接 es 服务器，底层基于 HTTP 协议通过 restful API 来和远程 ES 服务端通信，只提供了最简单最基本的 API，类似于上篇文章中给大家介绍的 API 操作逻辑
- 方式三：通过Elastic Java High Level Rest Client客户端连接 es 服务器，底层基于Elastic Java Low Level Rest Client客户端做了一层封装，提供了更高级得 API 且和Elastic Transport Client接口及参数保持一致，官方推荐的 es 客户端。
- 方式四：通过JestClient客户端连接 es 服务器，这是开源社区基于 HTTP 协议开发的一款 es 客户端，官方宣称接口及代码设计比 ES 官方提供的 Rest 客户端更简洁、更合理，更好用，具有一定的 ES 服务端版本兼容性，但是更新速度不是很快，目前 ES 版本已经出到 V7.9，但是JestClient只支持 V1.0~V6.X 版  本的 ES。

还有一个需要大家注意的地方，那就是版本号的兼容！<br />在开发过程中，大家尤其需要关注一下客户端和服务端的版本号，要尽可能保持一致，比如服务端 es 的版本号是6.8.2，那么连接 es 的客户端版本号，最好也是6.8.2，即使因项目的原因不能保持一致，客户端的版本号必须在6.0.0 ～6.8.2，不要超过服务器的版本号，这样客户端才能保持正常工作，否则会出现很多意想不到的问题，假如客户端是7.0.4的版本号，此时的程序会各种报错，甚至没办法用！<br />为什么要这样做呢？主要原因就是 es 的服务端，高版本不兼容低版本；es6 和 es7 的某些 API 请求参数结构有着很大的区别，所以客户端和服务端版本号尽量保持一致。
<a name="BcyFv"></a>
## 二、代码实践
本文采用的SpringBoot版本号是2.1.0.RELEASE，服务端 es 的版本号是6.8.2，客户端采用的是官方推荐的Elastic Java High Level Rest Client版本号是6.4.2，方便与SpringBoot的版本兼容。
<a name="XwqPi"></a>
### 2.1、导入依赖
```xml
<!--elasticsearch-->
<dependency>
	<groupId>org.elasticsearch</groupId>
	<artifactId>elasticsearch</artifactId>
	<version>6.4.2</version>
</dependency>
<dependency>
	<groupId>org.elasticsearch.client</groupId>
	<artifactId>elasticsearch-rest-client</artifactId>
	<version>6.4.2</version>
</dependency>
<dependency>
	<groupId>org.elasticsearch.client</groupId>
	<artifactId>elasticsearch-rest-high-level-client</artifactId>
	<version>6.4.2</version>
</dependency>
```
<a name="wKWRy"></a>
### 2.2、配置环境变量
在application.properties全局配置文件中，配置elasticsearch自定义环境变量
```
elasticsearch.scheme=http
elasticsearch.address=127.0.0.1:9200
elasticsearch.userName=
elasticsearch.userPwd=
elasticsearch.socketTimeout=5000
elasticsearch.connectTimeout=5000
elasticsearch.connectionRequestTimeout=5000
```
<a name="DGB8X"></a>
### 2.3、创建 elasticsearch 的 config 类
```java
@Configuration
public class ElasticsearchConfiguration {

    private static final Logger log = LoggerFactory.getLogger(ElasticsearchConfiguration.class);


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
至此，客户端配置完毕，项目启动的时候，会自动注入到Spring的ioc容器里面。
<a name="h3P56"></a>
### 2.4、索引管理
es 中最重要的就是索引库，客户端如何创建呢？
<a name="E1QFP"></a>
#### 创建索引
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class IndexJunit {


    @Autowired
    private RestHighLevelClient client;

    /**
     * 创建索引（简单模式）
     * @throws IOException
     */
    @Test
    public void createIndex() throws IOException {
        CreateIndexRequest request = new CreateIndexRequest("cs_index");
        CreateIndexResponse response = client.indices().create(request, RequestOptions.DEFAULT);
        System.out.println(response.isAcknowledged());
    }


    /**
     * 创建索引（复杂模式）
     * 可以直接把对应的文档结构也一并初始化
     * @throws IOException
     */
    @Test
    public void createIndexComplete() throws IOException {
        CreateIndexRequest request = new CreateIndexRequest();
        //索引名称
        request.index("cs_index");
        //索引配置
        Settings settings = Settings.builder()
                .put("index.number_of_shards", 3)
                .put("index.number_of_replicas", 1)
                .build();
        request.settings(settings);

        //映射结构字段
        Map<String, Object> properties = new HashMap();
        properties.put("id", ImmutableBiMap.of("type", "text"));
        properties.put("name", ImmutableBiMap.of("type", "text"));
        properties.put("sex", ImmutableBiMap.of("type", "text"));
        properties.put("age", ImmutableBiMap.of("type", "long"));
        properties.put("city", ImmutableBiMap.of("type", "text"));
        properties.put("createTime", ImmutableBiMap.of("type", "long"));
        Map<String, Object> mapping = new HashMap<>();
        mapping.put("properties", properties);
        //添加一个默认类型
        System.out.println(JSON.toJSONString(request));
        request.mapping("_doc",mapping);
        CreateIndexResponse response = client.indices().create(request, RequestOptions.DEFAULT);
        System.out.println(response.isAcknowledged());
    }

}
```
<a name="t1E5P"></a>
#### 删除索引
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class IndexJunit {


    @Autowired
    private RestHighLevelClient client;

    /**
     * 删除索引
     * @throws IOException
     */
    @Test
    public void deleteIndex() throws IOException {
        DeleteIndexRequest request = new DeleteIndexRequest("cs_index1");
        AcknowledgedResponse response = client.indices().delete(request, RequestOptions.DEFAULT);
        System.out.println(response.isAcknowledged());
    }


}
```
<a name="bxbyk"></a>
#### 查询索引
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class IndexJunit {


    @Autowired
    private RestHighLevelClient client;

    /**
     * 查询索引
     * @throws IOException
     */
    @Test
    public void getIndex() throws IOException {
        // 创建请求
        GetIndexRequest request = new GetIndexRequest();
        request.indices("cs_index");
        // 执行请求,获取响应
        GetIndexResponse response = client.indices().get(request, RequestOptions.DEFAULT);
        System.out.println(response.toString());
    }

}
```
<a name="Wrx56"></a>
#### 查询索引是否存在
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class IndexJunit {


    @Autowired
    private RestHighLevelClient client;

    /**
     * 检查索引是否存在
     * @throws IOException
     */
    @Test
    public void exists() throws IOException {
        // 创建请求
        GetIndexRequest request = new GetIndexRequest();
        request.indices("cs_index");
        // 执行请求,获取响应
        boolean response = client.indices().exists(request, RequestOptions.DEFAULT);
        System.out.println(response);
    }

}
```
<a name="bHrOV"></a>
#### 查询所有的索引名称
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class IndexJunit {


    @Autowired
    private RestHighLevelClient client;

    /**
     * 查询所有的索引名称
     * @throws IOException
     */
    @Test
    public void getAllIndices() throws IOException {
        GetAliasesRequest request = new GetAliasesRequest();
        GetAliasesResponse response =  client.indices().getAlias(request,RequestOptions.DEFAULT);
        Map<String, Set<AliasMetaData>> map = response.getAliases();
        Set<String> indices = map.keySet();
        for (String key : indices) {
            System.out.println(key);
        }
    }

}
```
<a name="wM4qU"></a>
#### 查询索引映射字段
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class IndexJunit {


    @Autowired
    private RestHighLevelClient client;

    /**
     * 查询索引映射字段
     * @throws IOException
     */
    @Test
    public void getMapping() throws IOException {
        GetMappingsRequest request = new GetMappingsRequest();
        request.indices("cs_index");
        request.types("_doc");
        GetMappingsResponse response = client.indices().getMapping(request, RequestOptions.DEFAULT);
        System.out.println(response.toString());
    }


}
```
<a name="q7uBh"></a>
#### 添加索引映射字段
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class IndexJunit {


    @Autowired
    private RestHighLevelClient client;

    /**
     * 添加索引映射字段
     * @throws IOException
     */
    @Test
    public void addMapping() throws IOException {
        PutMappingRequest request = new PutMappingRequest();
        request.indices("cs_index");
        request.type("_doc");

        //添加字段
        Map<String, Object> properties = new HashMap();
        properties.put("accountName", ImmutableBiMap.of("type", "keyword"));
        Map<String, Object> mapping = new HashMap<>();
        mapping.put("properties", properties);
        request.source(mapping);
        PutMappingResponse response = client.indices().putMapping(request, RequestOptions.DEFAULT);
        System.out.println(response.isAcknowledged());
    }
}
```
<a name="WynZI"></a>
### 2.5、文档管理
所谓文档，就是向索引里面添加数据，方便进行数据查询，详细操作内容，请看下文！
<a name="tCT7E"></a>
#### 添加文档
```java
public class UserDocument {

    private String id;
    private String name;
    private String sex;
    private Integer age;
    private String city;
    private Date createTime;

    //省略get、set...
}
```
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class DocJunit {

    @Autowired
    private RestHighLevelClient client;


    /**
     * 添加文档
     * @throws IOException
     */
    @Test
    public void addDocument() throws IOException {
        // 创建对象
        UserDocument user = new UserDocument();
        user.setId("1");
        user.setName("里斯");
        user.setCity("武汉");
        user.setSex("男");
        user.setAge(20);
        user.setCreateTime(new Date());

        // 创建索引,即获取索引
        IndexRequest request = new IndexRequest();
        // 外层参数
        request.id("1");
        request.index("cs_index");
        request.type("_doc");
        request.timeout(TimeValue.timeValueSeconds(1));
        // 存入对象
        request.source(JSON.toJSONString(user), XContentType.JSON);
        // 发送请求
        System.out.println(request.toString());
        IndexResponse response = client.index(request, RequestOptions.DEFAULT);
        System.out.println(response.toString());
    }

}
```
<a name="KXEk7"></a>
#### 更新文档
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class DocJunit {

    @Autowired
    private RestHighLevelClient client;


    /**
     * 更新文档（按需修改）
     * @throws IOException
     */
    @Test
    public void updateDocument() throws IOException {
        // 创建对象
        UserDocument user = new UserDocument();
        user.setId("2");
        user.setName("程咬金");
        user.setCreateTime(new Date());
        // 创建索引,即获取索引
        UpdateRequest request = new UpdateRequest();
        // 外层参数
        request.id("2");
        request.index("cs_index");
        request.type("_doc");
        request.timeout(TimeValue.timeValueSeconds(1));
        // 存入对象
        request.doc(JSON.toJSONString(user), XContentType.JSON);
        // 发送请求
        System.out.println(request.toString());
        UpdateResponse response = client.update(request, RequestOptions.DEFAULT);
        System.out.println(response.toString());
    }
}
```
<a name="oTp35"></a>
#### 删除文档
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class DocJunit {

    @Autowired
    private RestHighLevelClient client;


    /**
     * 删除文档
     * @throws IOException
     */
    @Test
    public void deleteDocument() throws IOException {
        // 创建索引,即获取索引
        DeleteRequest request = new DeleteRequest();
        // 外层参数
        request.id("1");
        request.index("cs_index");
        request.type("_doc");
        request.timeout(TimeValue.timeValueSeconds(1));
        // 发送请求
        System.out.println(request.toString());
        DeleteResponse response = client.delete(request, RequestOptions.DEFAULT);
        System.out.println(response.toString());
    }
}
```
<a name="dmzYL"></a>
#### 查询文档是不是存在
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class DocJunit {

    @Autowired
    private RestHighLevelClient client;


    /**
     * 查询文档是不是存在
     * @throws IOException
     */
    @Test
    public void exists() throws IOException {
        // 创建索引,即获取索引
        GetRequest request = new GetRequest();
        // 外层参数
        request.id("3");
        request.index("cs_index");
        request.type("_doc");
        // 发送请求
        System.out.println(request.toString());
        boolean response = client.exists(request, RequestOptions.DEFAULT);
        System.out.println(response);
    }
}
```
<a name="lC9fW"></a>
#### 通过 ID 查询指定文档
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class DocJunit {

    @Autowired
    private RestHighLevelClient client;


    /**
     * 通过ID，查询指定文档
     * @throws IOException
     */
    @Test
    public void getById() throws IOException {
        // 创建索引,即获取索引
        GetRequest request = new GetRequest();
        // 外层参数
        request.id("1");
        request.index("cs_index");
        request.type("_doc");
        // 发送请求
        System.out.println(request.toString());
        GetResponse response = client.get(request, RequestOptions.DEFAULT);
        System.out.println(response.toString());
    }
}
```
<a name="vbCEX"></a>
#### 批量添加文档
```java
@RunWith(SpringJUnit4ClassRunner.class)
@SpringBootTest(classes = ElasticSearchApplication.class)
public class DocJunit {
	
	@Autowired
	private RestHighLevelClient client;
	
	
	/**
	* 批量添加文档
	* @throws IOException
	*/
	@Test
	public void batchAddDocument() throws IOException {
		// 批量请求
		BulkRequest bulkRequest = new BulkRequest();
		bulkRequest.timeout(TimeValue.timeValueSeconds(10));
		// 创建对象
		List<UserDocument> userArrayList = new ArrayList<>();
		userArrayList.add(new UserDocument("张三", "男", 30, "武汉"));
		userArrayList.add(new UserDocument("里斯", "女", 31, "北京"));
		userArrayList.add(new UserDocument("王五", "男", 32, "武汉"));
		userArrayList.add(new UserDocument("赵六", "女", 33, "长沙"));
		userArrayList.add(new UserDocument("七七", "男", 34, "武汉"));
		// 添加请求
		for (int i = 0; i < userArrayList.size(); i++) {
			userArrayList.get(i).setId(String.valueOf(i));
			IndexRequest indexRequest = new IndexRequest();
			// 外层参数
			indexRequest.id(String.valueOf(i));
			indexRequest.index("cs_index");
			indexRequest.type("_doc");
			indexRequest.timeout(TimeValue.timeValueSeconds(1));
			indexRequest.source(JSON.toJSONString(userArrayList.get(i)), XContentType.JSON);
			bulkRequest.add(indexRequest);
		}
		// 执行请求
		BulkResponse response = client.bulk(bulkRequest, RequestOptions.DEFAULT);
		System.out.println(response.status());
	}
	
}
```
