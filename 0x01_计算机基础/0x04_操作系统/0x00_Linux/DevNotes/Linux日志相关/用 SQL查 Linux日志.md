Linux 日志<br />平时用tail、head、cat、sed、more、less这些经典系统命令查Linux日志，或者awk这类三方数据过滤工具，配合起来查询效率很高。但在使用过程中有一点比较头疼，那就是命令参数规则太多了，记的人脑壳疼。<br />那查日志有没有一种通用的方式，比如用SQL查询，毕竟这是程序员都比较熟悉的表达式。<br />今天分享的工具q，就实现了以写SQL的方式来查询、统计文本内容。
<a name="J9095"></a>
## 安装
q是一个命令行工具，允许我们在任意文件或者查询结果，比如可以在ps -ef查询进程命令的结果集上，直接执行SQL语句查询。<br />宗旨就是文本即数据库表。它将普通文件或者结果集当作数据库表，几乎支持所有的SQL结构，如`WHERE`、`GROUP BY`、`JOINS`等，支持自动列名和列类型检测，支持跨文件连接查询，这两个后边详细介绍，支持多种编码。<br />安装比较简单，在Linux CentOS环境，只要如下三步搞定，Windows环境更是只需安装个exe就可以用了。
```bash
wget https://github.com/harelba/q/releases/download/1.7.1/q-text-as-data-1.7.1-1.noarch.rpm #下载版本

sudo rpm -ivh q-text-as-data-1.7.1-1.noarch.rpm # 安装

q --version  #查看安装版本
```
官方文档：[https://harelba.github.io/q](https://harelba.github.io/q)
<a name="QNa6g"></a>
## 语法
q支持所有SQLite SQL语法，标准命令行格式`q + 参数命令 + "SQL"`
```bash
q <命令> "<SQL>"
```
要查询myfile.log文件的内容，直接`q "SELECT * FROM myfile.log"`。
```bash
q "SELECT * FROM myfile.log"
```
q不附加参数使用是完全没有问题的，但利用参数会让显示结果更加美观，所以这里简单了解一下，它的参数分为 2种。<br />`input`输入命令：指的是对要查询的文件或结果集进行操作，比如：-H命令，表示输入的数据包含标题行。
```bash
q -H "SELECT * FROM myfile.log"
```
在这种情况下，将自动检测列名，并可在查询语句中使用。如果未提供此选项，则列将自动命名为cX，以c1起始以此类推。
```bash
q  "select c1，c2 from ..."
```

- `output`输出命令：作用在查询输出的结果集，比如：`-O`，让查询出来的结果显示列名。
```bash
[root@iZ2zebfzaequ90bdlz820sZ software]# ps -ef | q -H "select count(UID) from - where UID='root'"
104
[root@iZ2zebfzaequ90bdlz820sZ software]# ps -ef | q -H -O "select count(UID) from - where UID='root'"
count(UID)
104
```
还有很多参数就不一一列举了，感兴趣的同学在官网上看下，接下来重点演示一下使用SQL如何应对各种查询日志的场景。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624344381887-b8bd3e7f-a4ee-41d7-8932-41267ef514a7.webp#clientId=u54f24d69-f6e3-4&from=paste&id=ud5a76b7c&originHeight=911&originWidth=752&originalType=url&ratio=3&status=done&style=none&taskId=ua1c3172b-5fa7-46a1-9030-7771d1605ab)
<a name="VEBW2"></a>
## 使用示例
下边咱们一起看几个查询日志的经常场景中，这个SQL该如何写。
<a name="OgQwG"></a>
### 1、关键字查询
关键字检索，应该是日常开发使用最频繁的操作，不过这一点q并没有什么优势，因为它查询时必须指定某一列。
```bash
[root@iZ2zebfzaequ90bdlz820sZ software]# q "select * from douyin.log where c9 like '%待解析%'"
2021-06-11 14:46:49.323 INFO 22790 --- [nio-8888-exec-2] c.x.douyin.controller.ParserController : 待解析URL :url=https%3A%2F%2Fv.douyin.com%2Fe9g9uJ6%2F                                             
2021-06-11 14:57:31.938 INFO 22790 --- [nio-8888-exec-5] c.x.douyin.controller.ParserController : 待解析URL :url=https%3A%2F%2Fv.douyin.com%2Fe9pdhGP%2F                                             
2021-06-11 15:23:48.004 INFO 22790 --- [nio-8888-exec-2] c.x.douyin.controller.ParserController : 待解析URL :url=https%3A%2F%2Fv.douyin.com%2Fe9pQjBR%2F                                             
2021-06-11 2
```
而用`grep`命令则是全文检索。
```bash
[root@iZ2zebfzaequ90bdlz820sZ software]# cat douyin.log | grep '待解析URL'
2021-06-11 14:46:49.323  INFO 22790 --- [nio-8888-exec-2] c.x.douyin.controller.ParserController   : 待解析URL :url=https%3A%2F%2Fv.douyin.com%2Fe9g9uJ6%2F
2021-06-11 14:57:31.938  INFO 22790 --- [nio-8888-exec-5] c.x.douyin.controller.ParserController   : 待解析URL :url=https%3A%2F%2Fv.douyin.com%2Fe9pdhGP%2F
```
<a name="orSdR"></a>
### 2、模糊查询
`like`模糊搜索，如果文本内容列有名字直接用列名检索，没有则直接根据列号c1、c2、cN。
```bash
[root@iZ2zebfzaequ90bdlz820sZ software]# cat test.log 
abc
2
3
4
5
23
24
25
[root@iZ2zebfzaequ90bdlz820sZ software]# q -H -t "select * from test.log where abc like '%2%'"
Warning: column count is one - did you provide the correct delimiter?
2
23
24
25
```
<a name="qlEM4"></a>
### 3、交集并集
支持`UNION`和`UNION ALL`操作符对多个文件取交集或者并集。<br />如下建了test.log和test1.log两个文件，里边的内容有重叠，用`union`进行去重。
```bash
q -H -t "select * from test.log union select * from test1.log"

[root@iZ2zebfzaequ90bdlz820sZ software]# cat test.log 
abc
2
3
4
5
[root@iZ2zebfzaequ90bdlz820sZ software]# cat test1.log 
abc
3
4
5
6
[root@iZ2zebfzaequ90bdlz820sZ software]# q -H -t "select * from test.log union select * from test1.log"
Warning: column count is one - did you provide the correct delimiter?
Warning: column count is one - did you provide the correct delimiter?
2
3
4
5
6
```
<a name="x3IHk"></a>
### 4、内容去重
比如统计某个路径下的./clicks.csv文件中，`uuid`字段去重后出现的总个数。
```bash
q -H -t "SELECT COUNT(DISTINCT(uuid)) FROM ./clicks.csv"
```
<a name="F7lkT"></a>
### 5、列类型自动检测
注意：q会理解每列是数字还是字符串，判断是根据实数值比较，还是字符串比较进行过滤，这里会用到`-t`命令。
```bash
q -H -t "SELECT request_id,score FROM ./clicks.csv WHERE score > 0.7 ORDER BY score DESC LIMIT 5"
```
<a name="bIWia"></a>
### 6、字段运算
读取系统命令查询结果，计算/tmp目录中每个用户和组的总值。可以对字段进行运算处理。
```bash
sudo find /tmp -ls | q "SELECT c5,c6,sum(c7)/1024.0/1024 AS total FROM - GROUP BY c5,c6 ORDER BY total desc"

[root@iZ2zebfzaequ90bdlz820sZ software]# sudo find /tmp -ls | q "SELECT c5,c6,sum(c7)/1024.0/1024 AS total FROM - GROUP BY c5,c6 ORDER BY total desc"
www www 8.86311340332
root root 0.207922935486
mysql mysql 4.76837158203e-06
```
<a name="nvMPz"></a>
### 7、数据统计
统计系统拥有最多进程数的前 3个用户ID，按降序排序，这就需要和系统命令配合使用了，先查询所有进程再利用SQL筛选，这里的q命令就相当`grep`命令。
```bash
ps -ef | q -H "SELECT UID,COUNT(*) cnt FROM - GROUP BY UID ORDER BY cnt DESC LIMIT 3"

[root@iZ2zebfzaequ90bdlz820sZ software]# ps -ef | q -H "SELECT UID,COUNT(*) cnt FROM - GROUP BY UID ORDER BY cnt DESC LIMIT 3"
root 104
www 16
rabbitmq 4
[root@iZ2zebfzaequ90bdlz820sZ software]# ps -ef | q -H -O "SELECT UID,COUNT(*) cnt FROM - GROUP BY UID ORDER BY cnt DESC LIMIT 3"
UID cnt
root 110
www 16
rabbitmq 4
```
加与不加`-O`命令的区别就是否显示查询结果的标题。
<a name="Eky5C"></a>
### 8、连文件查
一般情况下，日志文件会按天分割成很多个固定容量的子文件，在没有统一的日志收集服务器的情况下，如果不给个报错时间区间去查一个关键词，那么无异于大海捞针。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624344381906-2982b804-f8bb-4367-9e52-1d341485586f.webp#clientId=u54f24d69-f6e3-4&from=paste&id=u06298f09&originHeight=150&originWidth=1080&originalType=url&ratio=3&status=done&style=none&taskId=u953114fd-c87f-4693-a697-2b4ffa1c9ae)如果可以将所有文件内容合并后在查就会省事很多，q支持将文件像数据库表那样联合查询。
```bash
q -H "select * from douyin.log a join douyin-2021-06-18.0.log b on (a.c2=b.c3) where b.c1='root'"
```
<a name="Trc0l"></a>
## 总结
看完可能会有人抬杠：q写这么多代码直接用awk不香吗？介绍这个工具的初衷并不是说要替换现有哪种工具，而是多提供一种更为便捷的查日志方法。<br />awk确实很强大没得说，但这里边涉及到一个学习成本的问题，琳琅满目的命令、匹配规则想玩转还是要下点功夫的。而对于新手程序员稍微有点数据库经验，写SQL问题都不大，上手q则会容易的多。
