RediSearch 这个项目，并在项目中使用过。<br />就使用体验来说，简单场景下，用来平替 Elasticsearch 的使用场景已经足够。像是 Elasticsearch 中常用中文分词插件可以用 RediSearch 替代，但是拼音转中文插件在 RediSearch 中还没有功能替代，只能通过个人手段处理。<br />RediSearch 相比 Elasticsearch 的最大优点就是 **内存占用非常低，查询性能也足够高**😂。<br />在低配 2 核 4g 内存的服务器上，通过官方提供的 Redis Stack 镜像部署 Redis 以及自带模块 RediSearch 后，内存占用才不到 100m。<br />相比部署一个 Elasticsearch 起码需要 1g 内存来说，更愿意部署 RediSearch。本文大纲如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702350120565-bf0d7519-09ba-49a9-bf83-57b21f201b95.png#averageHue=%23faf8f7&clientId=uf85bc660-f7ab-4&from=paste&id=u7309d960&originHeight=921&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u352472da-29b4-4141-bb30-890a4c09e19&title=)
<a name="SSBDs"></a>
## RediSearch 简介
RediSearch 是一个 Redis 模块，为 Redis 提供查询、二级索引和全文搜索功能。<br />要使用 RediSearch 的功能，需要要先声明一个 index（类似于 Elasticsearch 的索引）。然后就可以使用 RediSearch 的查询语言来查询该索引下的数据。<br />RediSearch 内部使用压缩的倒排索引，所以可以用较低的内存占用来实现索引的快速构建。<br />目前 RediSearch 最新版支持的查询功能也比较丰富了，除了基本的文本分词还支持聚合统计、停用词、同义词、拼写检查、结果排序、标签查询、向量相似度查询以及中文分词等。
<a name="Kzcb2"></a>
## 对比 Elasticsearch
<a name="Zc2yu"></a>
### 基本硬件
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702350143956-379ab8f8-981f-4bec-9855-a703690be155.png#averageHue=%23fefdfc&clientId=uf85bc660-f7ab-4&from=paste&id=u6c233490&originHeight=202&originWidth=1029&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uccfe18d3-63de-478e-b2c2-74c2812661c&title=)
<a name="vOrwS"></a>
### 数据源
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702350143952-d4b43ed0-21b9-462b-95a5-6cc27b73b8d6.png#averageHue=%23fefefd&clientId=uf85bc660-f7ab-4&from=paste&id=u1cccec01&originHeight=129&originWidth=1034&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u69249633-6f69-406b-b5c3-8aa1c82732b&title=)
<a name="doHPT"></a>
### RediSearch 配置
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702350143950-57d5c060-25d6-4304-9094-8aed82099b00.png#averageHue=%23fefefd&clientId=uf85bc660-f7ab-4&from=paste&id=u498ed072&originHeight=242&originWidth=1027&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u50ff89ab-a51e-4c26-8211-5bc018ce0f9&title=)
<a name="VN5iX"></a>
### Elasticsearch 配置
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702350143957-16bdccea-b9ca-4ea8-ba3d-7e4784311f24.png#averageHue=%23fefefd&clientId=uf85bc660-f7ab-4&from=paste&id=ue55aacc9&originHeight=349&originWidth=1025&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue57ead75-0d3a-4627-808c-037ee4c3356&title=)
<a name="W5kHA"></a>
### 版本
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702350144016-4bb84331-f436-4cbe-9440-dd7a0ba12a90.png#averageHue=%23fefefd&clientId=uf85bc660-f7ab-4&from=paste&id=uc56bab6f&originHeight=236&originWidth=1025&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uae40631d-7961-41ea-906d-669e707553b&title=)
<a name="L46t9"></a>
### 索引构建测试
在官方提供的索引构建测试中，RediSearch 用 221 秒的速度超过了 Elasticsearch 的 349 秒，领先 58%，<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702350144337-aeb6bc32-a307-47fb-9e86-b9c186280e2c.png#averageHue=%23fdfafa&clientId=uf85bc660-f7ab-4&from=paste&id=uaa415f4f&originHeight=355&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc2fb7f53-5a04-45d0-a974-c4474d8fa6a&title=)
<a name="lAvIO"></a>
### 查询性能测试
通过数据集导入索引数据后，官方使用运行在专用负载生成器服务器上的 32 个客户端启动了两个词的搜索查询。<br />如下图所示，RediSearch 的吞吐量达到了 12.5K ops/sec，而 Elasticsearch 的吞吐量只有了 3.1K ops/sec，快了 4 倍。此外 RediSearch 的延迟稍好一些，平均为 8 毫秒，而 Elasticsearch 为 10 毫秒。(ops/sec 每秒操作数）<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1702350144457-7dba0fe7-ba00-4ca5-ac61-c1436e534399.png#averageHue=%23ebdce0&clientId=uf85bc660-f7ab-4&from=paste&id=uecbddab9&originHeight=270&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u61b5a689-ecda-42a2-ad9d-a25387874fc&title=)<br />由此可见，RediSearch 在性能上对比 RediSearch 有比较大的优势。

---

目前 RediSearch 已经更新到 2.0+ 版本，根据官方对于 RediSearch 2.0 版本介绍，与 RediSearch 1.6 相比，吞吐量和延迟相关的指标都提高了 2.4 倍。
<a name="ovUeZ"></a>
## RediSearch 安装
对于目前最新的 RediSearch 2.0 版本来说，官方推荐直接使用 redis-stack-server 镜像进行进行部署，也比较简单，
```bash
docker run -d --name redis-stack-server -p 6379:6379 redis/redis-stack-server:latest
```
设置登录密码
```bash
// 设置登录密码
docker run -e REDIS_ARGS="--requirepass redis-stack" redis/redis-stack:latest
```
通过 redis-cli 连接查看 RediSearch 是否安装了 search 模块，
```
redis-cli -h localhost

> MODULE list
...
3) 1) "name"
   2) "search"
   3) "ver"
   4) "20809"
   5) "path"
   6) "/opt/redis-stack/lib/redisearch.so"
   7) "args"
   8) 1) "MAXSEARCHRESULTS"
      2) "10000"
      3) "MAXAGGREGATERESULTS"
      4) "10000"
...
```
<a name="vsH15"></a>
## 索引操作
<a name="OZMdU"></a>
### FT.CREATE 创建索引命令
```
> FT.CREATE idx:goods on hash prefix 1 "goods:" language chinese schema goodsName text sortable
"OK"
```

- FT.CREATE：创建索引命令
- idx:goods：索引名称
- on hash：索引关联的数据类型，这里指定索引基于 hash 类型的源数据构建
- prefix 1 "goods:"：表示索引关联的 hash 类型源数据前缀是 goods:
- language chinese：表示支持中文语言分词
- schema goodsName text sortable：表示字段定义，goodsName 表示元数据属性名，text 表示字段类型 sortable 表示该字段可以用于排序

添加索引时，直接使用 hset 命令添加一个 key 前缀是 "goods:" 的源数据。如下，
```
hset goods:1001 goodsName 小米手机
hset goods:1002 goodsName 华为手机
```
<a name="OvCWp"></a>
### FT.SEARCH 查询索引
```
> FT.SEARCH idx:goods1 "手机"
1) "2"
2) "goods:1001"
3) 1) "goodsName"
   2) "\xe5\xb0\x8f\xe7\xb1\xb3\xe6\x89\x8b\xe6\x9c\xba"
4) "goods:1002"
5) 1) "goodsName"
   2) "\xe5\x8d\x8e\xe4\xb8\xba\xe6\x89\x8b\xe6\x9c\xba"
```
<a name="byqwy"></a>
### FT.INFO 查询指定名称索引信息
```
> FT.INFO idx:goods
1) "index_name"
2) "idx:goods1"
3) "index_options"
4) (empty list or set)
5) "index_definition"
6) 1) "key_type"
   2) "HASH"
   3) "prefixes"
   4) 1) "goods:"
   5) "default_language"
   6) "chinese"
   7) "default_score"
   8) "1"
7) "attributes"
8) 1) 1) "identifier"
      2) "goodsName"
      3) "attribute"
      4) "goodsName"
      5) "type"
      6) "TEXT"
      7) "WEIGHT"
      8) "1"
      9) "SORTABLE"
...
```

- FT.INFO 查询指定名称的索引信息
<a name="BrjMP"></a>
### FT.DROPINDEX 删除索引名称
```
> FT.DROPINDEX idx:goods1
"OK"
```

- FT.DROPINDEX 删除指定名称索引，不会删除 hash 类型的源数据

如果需要删除索引数据，直接使用 del 命令删除索引关联的源数据即可。
<a name="uR88m"></a>
## Java 使用 RediSearch
对于 Java 项目直接选用 Jedis4.0 以上版本就可以使用 RediSearch 提供的搜索功能，Jedis 在 4.0 以上版本自动支持 RediSearch，编写 Jedis 连接 RedisSearch 测试用例，用 RedisSearch 命令创建如下，
<a name="BCQ4z"></a>
### Jedis 创建 RediSearch 客户端
```java
@Bean
public UnifiedJedis unifiedJedis(GenericObjectPoolConfig jedisPoolConfig) {
    UnifiedJedis client;
    if (StringUtils.isNotEmpty(password)) {
        client = new JedisPooled(jedisPoolConfig, host, port, timeout, password, database);
    } else {
        client = new JedisPooled(jedisPoolConfig, host, port, timeout, null, database);
    }
    return client;
}
```
<a name="JK5tQ"></a>
### Jedis 创建索引
```java
Schema schema = new Schema()
        .addSortableTextField("goodsName", 1.0)
        .addSortableTagField("tag", "|");
IndexDefinition rule = new IndexDefinition(IndexDefinition.Type.HASH)
        .setPrefixes("idx:goods")
        .setLanguage("chinese"); # 设置支持中文分词
client.ftCreate(idxName,
        IndexOptions.defaultOptions().setDefinition(rule),
        schema);
```
<a name="du7PE"></a>
### Jedis 添加索引源数据
```java
public boolean addGoodsIndex(String keyPrefix, Goods goods) {
    Map<String, String> hash = MyBeanUtil.toMap(goods);
    hash.put("_language", "chinese");
    client.hset("idx:goods" + goods.getGoodsId(), MyBeanUtil.toMap(goods));
    return true;
}
```
<a name="VSGyQ"></a>
### Jedis 中文查询
```java
public SearchResult search(String goodsIdxName, SearchObjVO searchObjVO, Page<SearchPageGoodsVO> page) {
    // 查询关键字
    String keyword = searchObjVO.getKeyword();
    String queryKey = String.format("@goodsName:(%s)", keyword);
    Query q = new Query(queryKey);
    String sort = searchObjVO.getSidx();
    String order = searchObjVO.getOrder();
    // 查询是否排序
    if (StringUtils.isNotBlank(sort)) {
        q.setSortBy(sort, Constants.SORT_ASC.equals(order));

    }
    // 设置中文分词查询
    q.setLanguage("chinese");
    // 设置分页
    q.limit((int) page.offset(), (int) page.getSize());
    // 返回查询结果
    return client.ftSearch(goodsIdxName, q);
}
```
<a name="Yni5Z"></a>
## 最后
RediSearch 是这几年新出的一个全文搜索引擎，借助于 Redis 的成功，RediSearch 一出场就获得了较高的关注度。<br />目前来看，个人使用 RediSearch 作为项目的全文搜索引擎已经够用了，它有易于安装、索引占用内存低、查询速度快等许多优点。不过在对 Redis 集群的支持上，RediSearch 目前只针对 Redis 企业版有解决方案，开源版还没有，这一点需要告诉大家。<br />如果想要在生产环境大规模使用，还是不太建议的。
