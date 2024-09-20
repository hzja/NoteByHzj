JavaSpringBootElasticsearch
<a name="XDAez"></a>
## 一、简介
实际的软件项目开发过程中，因为业务上的需要，数据库表与表之间的结构是一对多的关系，以订单表与订单项表为例，在 mysql 数据库里面，他们的关系如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655684996829-1ca6cb51-0eaa-4a06-a8b1-cc789c36da45.png#clientId=uacf8ee33-b8ce-4&from=paste&id=u02e6be5a&originHeight=468&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3a430b94-241d-435c-a557-4d25ed24946&title=)<br />如果想查询哪些商品在某个时间段内被购买过，可以通过如下`join`连表的方式进行查询。
```sql
select t.*
from tb_order t
left join tb_order_item tt on t.order_id = tt.order_id
where tt.product_name like '%商品名称%'
and  t.createTime >= '2022-06-01 00:00:00'
and t.createTime <= '2022-06-16 00:00:00';
```
然后，ElasticsSearch 和大多数 NoSQL 数据库类似，是扁平化的存储结构，不能像关系型数据库那样，可以通过`join`方式进行连表搜索。<br />不过 ElasticsSearch （以下简称 ES）目前毕竟发展到 8.x 版本了，已经有几种可选的方式能够高效的支持这种一对多关系的映射和搜索。<br />比较常用的实践方案，有以下三种：

- **嵌套对象**
- **嵌套文档**
- **父子文档**

其中第二种，是要重点介绍的部分，以实际案例的方式给大家讲解具体的实践思路。
<a name="ps4V6"></a>
## 二、案例实践
<a name="edpjm"></a>
#### 2.1、嵌套对象
所谓嵌套对象，就是当前json对象内嵌了一个json对象，以订单数据为例，包含多个订单项数据，格式如下：
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
创建一个order_index的索引，将上面的文档数据存储到 ES 中，看看索引文档结构的mapping长啥样，内容如下：
```json
{
    "order_index":{
        "mappings":{
            "_doc":{
                "properties":{
                    "orderId":{
                        "type":"keyword"
                    },
                    "orderNo":{
                        "type":"keyword"
                    },
                    "orderUserName":{
                        "type":"keyword"
                    },
                    "orderItems":{
                        "properties":{
                            "orderItemId":{
                                "type":"keyword"
                            },
                            "orderId":{
                                "type":"keyword"
                            },
                            "productName":{
                                "type":"keyword"
                            },
                            "brandName":{
                                "type":"keyword"
                            },
                            "sellPrice":{
                                "type":"keyword"
                            }
                        }
                    }
                }
            }
        }
    }
}
```
可以很清晰的看到，订单索引映射字段里面，包含了一个orderItems的字段，它是对象类型，内部有自己的字段属性。这其实就是一个包含关系，表示一个订单可以有多个订单项信息。<br />可以查询下索引结果集看看结果，使用postman执行查询索引下的所有文档数据！
```json
POST order_index/_search
{
  "query": {
    "match_all": {}
  }
}
```
返回结果如下（去掉了部分非重要的数据，方便观察）：
```json
[
    {
        "_index":"order_index",
        "_type":"_doc",
        "_id":"1",
        "_score":1,
        "_source":{
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
    }
]
```
可以很清晰的看到，返回结果也是完美呈现，orderItems其实是个list，包含两个对象，所有的信息都在一个文档中。<br />再来试试在 ES 中通过商品名称和品牌名称，两个并集的条件筛选，来查询客户的订单信息，编写DSL查询语句，搜索商品名称为火腿肠并且品牌为汇源的订单，内容如下：
```json
POST order_index/_search

{
    "query":{
        "bool":{
            "must":[
                {
                    "match":{
                        "orderItems.productName":"火腿肠"
                    }
                },
                {
                    "match":{
                        "orderItems.brandName":"汇源"
                    }
                }
            ]
        }
    }
}
```
返回结果如下（去掉了部分非重要的数据，方便观察）：
```json
[
    {
        "_index":"order_index",
        "_type":"_doc",
        "_id":"1",
        "_score":1,
        "_source":{
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
    }
]
```
**从预计结果上分析，没有客户购买过品牌名称为汇源并且商品名称为火腿肠的订单，理论上，应该是无任何数据的**！<br />**但是，结果却返回有这条订单数据！这是为什么呢？**<br />原来 ES 对于json对象数组的做了压扁处理，比如上面的例子在 ES 存储的结构是这样的：
```json
{
  "orderId": [ 1 ],
  "orderItems.productName":["火腿肠","果冻"],
  "orderItems.brandName": ["双汇","汇源"],
  ...
}
```
很明显，这样的结构丢失了商品名称和品牌名称的关联，导致查询的时候，出现失效，如果业务要求实现精准搜索，**那么这种方案是不满足要求的**。<br />如果业务场景对这个问题不是敏感，就可以选择这种方式，因为它足够简单并且效率也比下面要介绍的两种方案高。
<a name="aJK6G"></a>
#### 2.2、嵌套文档
很明显上面对象数组的方案没有处理好内部对象的边界问题，JSON数组对象被 ES 强行存储成扁平化的键值对列表。为了解决这个问题，ES 推出了一种所谓的嵌套文档的方案，官方对这种方案的介绍是这样的：
> The nested type is a specialised version of the object datatype that allows arrays of objects to be indexed in a way that they can be queried independently of each other.

可以看到嵌套文档的方案其实是对普通内部对象方案的补充。将上面的订单索引结构中的orderItems数据类型，将其改成nested类型，重新创建索引。
```json
{
    "properties":{
        "orderItems":{
            "properties":{
                ....
            },
            "type":"nested"
        }
  ....
    }
}
```
orderItems数据类型，改成是nested，表示是个内嵌文档，其它的属性保持不变。<br />再来试试通过商品名称和品牌名称来查询客户的订单信息，**不同的地方在于，在查询的时候，需要指定nested关键字和路径path**，查询姿势如下：
```json
POST order_index/_search

{
    "query":{
        "nested":{
            "path":"orderItems",
            "query":{
                "bool":{
                    "must":[
                        {
                            "match":{
                                "orderItems.productName":"火腿肠"
                            }
                        },
                        {
                            "match":{
                                "orderItems.brandName":"汇源"
                            }
                        }
                    ]
                }
            }
        }
    }
}
```
**查询结果为空[]，与预期结果一致**！<br />再将查询条件换一下，查询商品名称为火腿肠和品牌名称为双汇的订单。
```json
POST order_index/_search

{
    "query":{
        "nested":{
            "path":"orderItems",
            "query":{
                "bool":{
                    "must":[
                        {
                            "match":{
                                "orderItems.productName":"火腿肠"
                            }
                        },
                        {
                            "match":{
                                "orderItems.brandName":"双汇"
                            }
                        }
                    ]
                }
            }
        }
    }
}
```
查询结果如下：
```json
[
    {
        "_index":"order_index",
        "_type":"_doc",
        "_id":"1",
        "_score":1,
        "_source":{
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
    }
]
```
与预期的结果一致，说了这么多，似乎嵌套文档很好用啊。没有前面那个方案对象边界缺失的问题，用起来似乎也不复杂。那么它没有缺点吗？当然，先来做个试验。<br />先看看当前索引的文档数量。
```http
GET _cat/indices?v
```
查询结果。
```http
green  open   order_index                   FJsEIFf_QZW4Q4SlZBsqJg   1   1          3            0     17.7kb          8.8kb
```
可能已经注意到这里查看文档数量并不是用的下面这个。
```http
GET order_index/_count
```
而是直接查看的索引信息，他们的区别在于：

- **第一个查询，是查询索引库里面每个索引总的文档数**
- **第二个查询，是查询当前索引的文档数，不包括嵌套文档数量**

可以很清晰的看到，order_index索引，在 ES 中总的文档数据是 3，为啥不是 1 呢？<br />这是因为nested子文档在 ES 内部其实也是独立的 lucene 文档，只是在查询的时候，ES 内部做了类似数据库的join处理。最终看起来好像是一个独立的文档一样。<br />如果一个订单，有1000个订单项，那么在 ES 中存在的文档数就是1001，会随着订单数的增加而成倍上升。<br />那可想而知，同样的条件下，这个性能肯定不如普通内部对象的方案。在实际的业务应用中要根据实际情况决定是否选择这种方案。<br />有一点是可以肯定的是，他能满足内部对象数据精准搜索的要求！
<a name="OkDts"></a>
#### 2.3、父子文档
还是看上面那个例子，假如需要更新文档的orderNo属性的值，ES 对于文档的更新，操作原理是：**把原来的数据删掉，再重新插入一条，但索引 id 是相同的**。<br />这就意味，即使orderItems字段，不需要更新，它也会随着主文档一起被重新索引。<br />还有就是如果某个表跟某个表有多对多的关系，比如一个子文档可以属于多个主文档的场景，用nested无法实现，此时可以考虑采用**父子文档**结构来处理。<br />下面以考试题为例，**一个问题可能有多个答案，一个答案可能对应多个问题**。<br />首先定义索引文档结构，如下：
```json
PUT exam_index

{
    "mappings":{
        "_doc":{
            "properties":{
                "my_id":{
                    "type":"keyword"
                },
                "parent_join_child":{
                    "type":"join",
                    "relations":{
                        "question":"answer"
                    }
                }
            }
        }
    }
}
```
my_id是自定义的字段，parent_join_child是给父子文档关系的名字，这个可以自定义，join表示这是一个父子文档关系，relations里面表示question是父，answer是子。<br />首先插入两个父文档。
```json
PUT exam_index/_doc/1

{
    "my_id":"1",
    "text":"这是一个问题1",
    "parent_join_child":{
        "name":"question"
    }
}

PUT exam_index/_doc/2

{
    "my_id":"2",
    "text":"这是一个问题2",
    "parent_join_child":{
        "name":"question"
    }
}
```
其中"name":"question"表示插入的是父文档。<br />然后插入两个子文档
```json
PUT exam_index/_doc/3?routing=1

{
    "my_id":"3",
    "text":"这是一个回答1，对应问题1",
    "parent_join_child":{
        "name":"answer",
        "parent":"1"
    }
}

PUT exam_index/_doc/4?routing=1

{
    "my_id":"4",
    "text":"这是一个回答2，对应问题1",
    "parent_join_child":{
        "name":"answer",
        "parent":"1"
    }
}
```
子文档要解释的东西比较多，首先从文档id可以判断子文档都是独立的文档（跟nested不一样）。其次routing关键字指明了路由的id是父文档1， 这个id和下面的parent关键字对应的id是一致的。<br />需要强调的是，索引子文档的时候，routing是必须的，因为要确保子文档和父文档在同一个分片上。<br />"name":"answer"关键字指明了这是一个子文档。<br />现在exam_index索引中有四个独立的文档，来看父子文档在搜索的时候是什么样的。<br />先来一个无条件查询，返回所有的文档数据。
```json
POST exam_index/_search
{
    "query":{
        "match_all":{
        }
    },
    "sort":["my_id"]
}
```
返回的结果如下：
```json
[
    {
        "_index":"crm_exam_index",
        "_type":"_doc",
        "_id":"1",
        "_score":null,
        "_source":{
            "my_id":"1",
            "text":"这是一个问题1",
            "parent_join_child":{
                "name":"question"
            }
        }
    },
    {
        "_index":"crm_exam_index",
        "_type":"_doc",
        "_id":"2",
        "_score":null,
        "_source":{
            "my_id":"2",
            "text":"这是一个问题2",
            "parent_join_child":{
                "name":"question"
            }
        }
    },
    {
        "_index":"crm_exam_index",
        "_type":"_doc",
        "_id":"3",
        "_score":null,
        "_routing":"1",
        "_source":{
            "my_id":"3",
            "text":"这是一个回答1，对应问题1",
            "parent_join_child":{
                "name":"answer",
                "parent":"1"
            }
        }
    },
    {
        "_index":"crm_exam_index",
        "_type":"_doc",
        "_id":"4",
        "_score":null,
        "_routing":"1",
        "_source":{
            "my_id":"4",
            "text":"这是一个回答2，对应问题1",
            "parent_join_child":{
                "name":"answer",
                "parent":"1"
            }
        }
    }
]
```
可以看到返回的结果带了parent_join_child关键字，指明这是个父文档还是子文档。<br />如果想通过子文档信息，查询父文档，可以通过如下方式实现：
```json
POST exam_index/_search

{
    "query":{
        "has_child":{
            "type":"answer",
            "query":{
                "match":{
                    "text":"回答"
                }
            }
        }
    }
}
```
返回结果：
```json
[
    {
        "_index":"exam_index",
        "_type":"_doc",
        "_id":"1",
        "_score":1,
        "_source":{
            "my_id":"1",
            "text":"这是一个问题1",
            "parent_join_child":{
                "name":"question"
            }
        }
    }
]
```
如果想通过父文档信息，查询子文档，可以通过如下方式实现：
```json
POST exam_index/_search

{
    "query":{
        "has_parent":{
            "parent_type":"question",
            "query":{
                "match":{
                    "text":"问题"
                }
            }
        }
    }
}
```
返回结果：
```json
[
    {
        "_index":"crm_exam_index",
        "_type":"_doc",
        "_id":"3",
        "_score":1,
        "_routing":"1",
        "_source":{
            "my_id":"3",
            "text":"这是一个回答1，对应问题1",
            "parent_join_child":{
                "name":"answer",
                "parent":"1"
            }
        }
    },
    {
        "_index":"crm_exam_index",
        "_type":"_doc",
        "_id":"4",
        "_score":1,
        "_routing":"1",
        "_source":{
            "my_id":"4",
            "text":"这是一个回答2，对应问题1",
            "parent_join_child":{
                "name":"answer",
                "parent":"1"
            }
        }
    }
]
```
如果想通过父 ID 查询子文档，可以通过如下方式实现：
```json
POST exam_index/_search

{
	"query":{
		"parent_id":{
			"type":"answer",
			"id":"1"
		}
	}
}
```
返回结果和上面一样，区别在于parent_id搜索默认使用相关性算分，而has_parent默认情况下不使用算分。<br />使用父子文档的模式有一些需要特别关注的点：

- **每一个索引只能定义一个**`**join field**`
- **父子文档必须在同一个分片上，意味着查询，更新操作都需要加上**`**routing**`
- **可以向一个已经存在的**`**join field**`**上新增关系**
- **父子文档，适合那种数据结构基本一致的场景，如果两个表结构完全不一致，不建议使用这种结构**
- **父子文档也有缺点，查询速度是这三个方案里面最慢的一个**
<a name="gvNhM"></a>
## 三、小结
整体总结下来，嵌套对象通过冗余数据来提高查询性能，适用于读多写少的场景，由于 ES 会对json数组对象进行压平处理，导致内嵌对象搜索不会很精准，如果业务场景搜索要求不高，推荐采用这种方案。<br />如果业务场景要求搜索必须精准，可以采用嵌套文档的方案来实现，每次更新的时候，文档数据会删除然后再插入，写入和查询性能比嵌套对象要低。<br />如果表与表之间是多对多的场景，可以采用父子文档的方案来实现，每次更新只会更新单个文档数据，写入会比嵌套文档更快，缺点是其查询速度会比同等的嵌套文档查询慢 5 到 10 倍！<br />具体的方案选型，还需要根据当下的业务场景进行合理的选择。
