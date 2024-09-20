众所周知，mongodb的count查询是相当慢的，但是count的查询又是非常常见的操作。最近就有一项需要，需要在200万条数据中执行count查询，并且使用`MongoTemplate.count()`查询，结果查询结果很慢。那么如何解决这个问题呢?查询了相关的资料。采用了以下方案供大家参考。<br />首先，在mongo shell中执行`db.collection.find({}).count()`不用1s的时间就出来结果，因此，首先想到的是能不能在Java mongo中直接执行mongo shell的命令，很不幸，这里使用的是4.4版本的mongodb，不在支持`db.eval()`操作. 因此只能选择其他方法<br />相关资料可以参考这个：<br />[https://stackoverflow.com/questions/16239592/raw-javascript-mongodb-queries-using-db-eval-in-java https://docs.mongodb.com/manual/reference/method/db.eval/](https://stackoverflow.com/questions/16239592/raw-javascript-mongodb-queries-using-db-eval-in-java https://docs.mongodb.com/manual/reference/method/db.eval/)<br />经过网上查询和官方文档提示，mongodb支持以下的count查询

- `db.collection.count()`
- `db.collection.countDocuments()`
- `db.collection.estimatedDocumentCount()`

这三个方法对比着看，会有更深的认识
<a name="wjpRj"></a>
## 方法对比
<a name="UbyVY"></a>
### `count`

1. 采用`metadata`来返回数据总量
<a name="ilWKt"></a>
### `countDocuments`

1. 未采用`metadata`
2. 通过`aggregation`来实时计算出数量

改语法封装了下面的语句，来返回数量
```sql
db.collection.aggregate([
  {$match: <query>},
  {$group: {_id: null, n: {$sum: 1}}}
])
```
<a name="QQo2C"></a>
### `estimatedDocumentCount`
<a name="Tt7yZ"></a>
#### 说明

1. 该方法返回一个collection中的所有`documents`的数量
2. 该方法封装了`count`命令
3. 该方法采用了metadata
<a name="ZpiAk"></a>
#### 使用
```sql
db.collection.estimatedDocument(<options>)
```
