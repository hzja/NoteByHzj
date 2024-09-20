分布式 
<a name="PCBdG"></a>
### 一、介绍
公司最近在搞服务分离，数据切分的工作，因为订单和订单项表的数据量实在过大，而且每天都是以50万的数据量在增长，基于现状，项目组决定采用分库的方式来解决当前遇到的问题。<br />那具体怎么切分呢？<br />分库的策略其实还比较简单，主要是要确定分片的字段和策略。<br />最开始是想通过主键ID的奇、偶数来分两个库，order_1库主要用于存储奇数的ID，order_2库主要用于存储偶数的ID。<br />但是这种切分，局限性非常大，因为最多只能分两个库，如果随着数据量的增大，后面就没很难在分了。<br />之后又想到了另一个分片字段：城市ID，因为订单表上有城市ID的属性，可以基于此进行分库，但是全国有几百个城市，不可能分几百个库或者表，最后的讨论结果是：

- 城市ID的生成固定大小，默认三位数，100～999
- 将订单表分成三个库，order_1、order_2、order_3
- 当城市ID 在100~399区间，就存储到order_1库
- 当城市ID 在400~699区间，就存储到order_2库
- 当城市ID 在700～999区间，就存储到order_3库

通过城市ID进行分片，如果后期订单数据量进一步过大，也可以进一步的分库！<br />基于Mysql数据库，使用最广、最成熟的分布式中间件当属于Mycat。<br />但是，自从采用Mycat中间件进行分库之后，发现了非常多的坑，下面就一起来看看这些坑点！
<a name="dH77k"></a>
### 二、细数Mycat中的坑点
<a name="W1UCG"></a>
#### 2.1、分页查询会出现全表扫描
当功能上线之后，测试人员在页面上从末尾页不停的往前分页查询订单数据的时候，运维平台突然报监控到很多慢 SQL 报警。<br />以下是运维平台监控到的慢sql语句。
```sql
SELECT id FROM order
WHERE OrderCreateTime BETWEEN '2021-05-01 00:00:00' AND '2021-06-01 00:00:00'
ORDER BY id DESC
LIMIT 0, 151400
```
于是，运维同学说程序有问题，通过查询日志，排查到代码的查询语句是长这样的。
```sql
SELECT id FROM order
WHERE OrderCreateTime BETWEEN '2021-05-01 00:00:00' AND '2021-06-01 00:00:00'
ORDER BY id DESC
LIMIT 151300, 100
```
与实际运维给的慢sql语句中的LIMIT 0, 151400完全不符合。<br />之后，当测试人员再次点击分页查询的时候，运维又监控到了LIMIT 0, 151400这种怪异的SQL，花了好几个小时排查，在本地跑测试，还是没发现什么问题，真的感觉到了要怀疑人生了！<br />当多次测试的时候，这个问题每次都能复现，想起了一个问题，是不是 Mycat 分页的时候，对全表扫描了。<br />后来经过查阅资料，才发现真有这个坑！<br />在分库分表的情况下，宕 limit 的开始位置特别大的时候，例如大于某表的总行数时，mycat 将查询各个分表的结果集返，然后在mycat中进行合并和排序，再返回结果。<br />例如，原始的 sql 语句是这样的：
```sql
SELECT * FROM table_name WHERE type='xxx' ORDER BY create_time LIMIT 10000,1000
```
通过 mycat 执行的结果，会是这样的：
```sql
SELECT * FROM table_name WHERE type='xxx' ORDER BY create_time LIMIT 0,11000
```
结果集特别大的情况会导致查询很慢，严重的情况会直接导致 mycat OOM！<br />因此，在分库分表的情况，不要用 mycat 进行大批量的数据分页查询，通过条件过滤，减小分页的数据量大小！
<a name="MNnPZ"></a>
#### 2.2、子查询结果偶尔不完整
当通过某些条件，筛选订单项数据时，测试人员反馈某些数据偶尔出现不完整。<br />具体SQL操作如下：
```sql
select id,productName
from orderItem
where orderId in (
 select id from order where userName = '张三'
)
```
预期的查询结果时：
```sql
1,"巧克力"
2,"可乐"
3,"果冻"
4,"苹果手机"
```
但是实际查询的时候，有时候的结果如下：
```sql
1,"巧克力"
2,"可乐"
4,"苹果手机"
```
在网上查询了相关的问题，在分库分表的情况下，子查询出了偶尔查询不到完整数据外，还会出现 mycat 内部死锁，因此尽量在代码中不要使用子查询，而是采用主键ID或是索引字段进行单表查询，这样效率会大大提升！
<a name="HXBKs"></a>
#### 2.3、跨分片join问题
由于历史代码的缘故，订单服务里面存在很多各种连表操作，例如：
```sql
select a.*,b.accountName,c.address
from order a
left join account b on a.accountId = b.id
left join account_address c on  b.id = c.accountId
where a.orderId = 11110011
```
但是在走 mycat 查询之后，直接报错！<br />原因是：mycat 目前只支持两张分片表的 Join，如果要支持多张表需要自己改造程序代码或者改造Mycat的源代码。
<a name="SKoCe"></a>
#### 2.4、部分SQL语法不支持
在实际使用的时候，发现还有部分sql语句是不支持的。<br />复制插入(不支持)
```sql
insert into......select.....
```
复杂更新(不支持)
```sql
update a, b set a.remark='备注' where a.id=b.id;
```
复杂删除(不支持)
```sql
delete a from a join b on a.id=b.id;
```
还有就是不支持跨库连表操作！
<a name="n8Yu4"></a>
#### 2.5、不支持存储过程创建和调用
有一点，需要大家注意的，在走 mycat 中间件的方式与数据库连接的时候，如果代码中写了存储过程等语句，是 mycat 是不支持调用的，因此尽量不要使用！
<a name="Vqikq"></a>
### 三、小结
虽然上面介绍了 mycat 有一些坑，但是这些坑，通过一些优化手段还是可以避免的。<br />实际上，mycat 作为分库分表的中间件，也有许多的优势，例如下面官网的介绍。<br />![2021-05-20-09-38-54-482293.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621474748141-2720d1c1-df53-43db-b9ed-5ecb6120c428.jpeg#clientId=u2bbb4dc7-a1d8-4&from=ui&id=u549c5cda&originHeight=650&originWidth=1080&originalType=binary&size=62281&status=done&style=none&taskId=u5eadc24e-73fd-4f73-ab3d-cdfb321aff7)<br />据了解，mycat 是目前最成熟、使用最广的中间件，因此大家在使用的时候，不需要带有啥顾虑，对于以上的坑点，尽可能的避免。
