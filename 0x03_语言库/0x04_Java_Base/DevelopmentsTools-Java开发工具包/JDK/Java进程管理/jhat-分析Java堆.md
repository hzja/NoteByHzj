Java jhat<br />`jhat` 分析 Java 堆的命令，可以将堆中对象以 `html` 的形式显示出来，支持对象查询语言 OQL，命令内容如下：

| -j <flag> | 启动JVM时传入启动参数<br />(例如: J-Xmx512m 则指定运行jhat的Java虚拟机使用的最大堆内存为512 MB) |
| --- | --- |
| -stack falseltrue | 关闭对象分配调用栈跟踪(tracking object llocation call stack)。默认值为true |
| -refs falseltrue | 关闭对象引用跟踪(tracking of references to objects)。默认值为true |
| -port port-number | 设置jhat HTTP server的端口号;默认值7000 |
| -exclude exclude file | 指定文件对象查询时需要排除的数据成员列表<br />(例如:如果文件列出了java.lang.String,value，那么当从某个特定对象Object 0计算可达的对象列表时，引用路径涉及java.lang String,value的都会被排除。) |
| -baseline exclude-file | 指定一个基准堆转储在两个heap dumps中有相同object ID的对象会被标记为不是新的(marked as not being new),其他对象被标记为新的(new),在比较两个不同的堆转储时很有用。 |
| -debug int | 设置debug级别。0表示不输出调试信息;值越大则表示输出更详细的debug信息 |
| -version | 启动后只显示版本信息就退出 |

<a name="xwTdo"></a>
## 通过jhat查看在jmap生成的堆分析文件
<a name="eIcuk"></a>
### ①通过jmap生成分析文件
[jmap-Java 进程内存中 Object 的情况 · 语雀](https://www.yuque.com/fcant/sys/geluh6?inner=iKjyS&view=doc_embed)
<a name="6ev6S"></a>
### ②使用 `jhat -port 9999 heap.bin` 将保存的heap.bin以HTML文件展示出来
```bash
fcsca@FCANT D:\LinkSpace\Download\DevelopPackage\Arthas\arthas-bin                                          [16:14]
❯ jhat -port 9999 heap.bin
Reading from heap.bin...
Dump file created Thu Jul 16 16:14:38 GMT+08:00 2020
Snapshot read, resolving...
Resolving 1449061 objects...
Chasing references, expect 289 dots.................................................................................................................................................................................................................................................................................................
Eliminating duplicate references.................................................................................................................................................................................................................................................................................................
Snapshot resolved.
Started HTTP server on port 9999
Server is ready.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594887622810-158ac551-d951-4233-b2e0-bae8ee0fea52.png#averageHue=%23353535&height=297&id=ST3YH&originHeight=892&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1126657&status=done&style=none&title=&width=1107.6666666666667)<br />执行完毕后，打开 http://localhost:9999/ 就可以看到类的实例的堆占用情况，它是按照包名来分组的<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594887784436-30ca0898-1179-4927-bef8-259f96f27710.png#averageHue=%23fafafa&height=657&id=h8g8R&originHeight=1972&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=891572&status=done&style=shadow&title=&width=1280)<br />页面底部还有许多的Query方式：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594888023898-64b2e3cc-773b-4b6c-9f94-dd09eb3b80bd.png#averageHue=%23fafafa&height=657&id=XiXKP&originHeight=1972&originWidth=3831&originalType=binary&ratio=1&rotation=0&showTitle=false&size=709372&status=done&style=shadow&title=&width=1277)

- 显示出堆中所包含的所有的类
- 从根集能引用到的对象
- 显示所有类（包括平台）的实例计数
- 堆实例的分布表
- 执行对象查询语句

以 OQL 为例，打开后是一个类似 SQL 查询的窗口，比如输入 `select s from java.lang.String s where s.value.length >= 100` 就可以查询字符串长度大于 100 的实例：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594888086523-dbd8195a-9246-4176-aba1-f21d5ea4d3e6.png#averageHue=%23fcfcfc&height=659&id=lj0e5&originHeight=1976&originWidth=3829&originalType=binary&ratio=1&rotation=0&showTitle=false&size=429559&status=done&style=shadow&title=&width=1276.3333333333333)
<a name="GhuBW"></a>
## OQL（对象查询语言）
jhat中的OQL（对象查询语言），文档可以查看：http://localhost:9999/oqlhelp/<br />如果需要根据某些条件来过滤或查询堆的对象，这是可能的，可以在jhat的html页面中执行OQL，来查询符合条件的对象<br />基本语法：
```sql
select<javascript expression to select>
[from[instanceof]<class name><identifier>]
[where<javascript boolean expression to filter>]
```
解释：<br />(1)class name是java类的完全限定名，如：java.lang.String, java.util.ArrayList,[C是char数组,[Ljava.io.File是java.io.File[]<br />(2)类的完全限定名不足以唯一的辨识一个类，因为不同的ClassLoader载入的相同的类，它们在jvm中是不同类型的<br />(3)instanceof表示也查询某一个类的子类，如果不明确instanceof，则只精确查询class name指定的类<br />(4)from和where子句都是可选的<br />(5)java域表示：obj.field_name；java数组表示：array[index]
<a name="WK6E7"></a>
### 举例
<a name="UGSxC"></a>
#### (1)查询长度大于100的字符串
```sql
select s from java.lang.String s where s.count >100
```
<a name="gmpoW"></a>
#### (2)查询长度大于256的数组
```sql
select a from [I a where a.length >256
```
<a name="SnTQ3"></a>
#### (3)显示匹配某一正则表达式的字符串
```sql
select a.value.toString() from java.lang.String s where /java/(s.value.toString())
```
<a name="ZKdEe"></a>
#### (4)显示所有文件对象的文件路径
```sql
select file.path.value.toString() from java.io.File file
```
<a name="o0h48"></a>
#### (5)显示所有`ClassLoader`的类名
```sql
select classof(cl).name from instanceof java.lang.ClassLoader cl
```
<a name="JpbBL"></a>
#### (6)通过引用查询对象
```sql
select o from instanceof 0xd404d404 o
```
<a name="SPloa"></a>
### built-in对象-- heap 
<a name="JBCS4"></a>
#### (1)`heap.findClass(class name)`--找到类
```sql
select heap.findClass("java.lang.String").superclass
```
<a name="gFxTB"></a>
#### (2)`heap.findObject(object id)`--找到对象
```sql
select heap.findObject("0xd404d404")
```
<a name="mAOUt"></a>
#### (3)`heap.classes` --所有类的枚举
```sql
select heap.classes
```
<a name="O0jXx"></a>
#### (4)`heap.objects` --所有对象的枚举
```sql
select heap.objects("java.lang.String")
```
<a name="xagCN"></a>
#### (5)`heap.finalizables` --等待垃圾收集的java对象的枚举
<a name="wxDQ5"></a>
#### (6)`heap.livepaths` --某一对象存活路径
```sql
select heaplivepaths(s) from java.lang.String s
```
<a name="qpLC6"></a>
#### (7)`heap.roots` --堆根集的枚举
<a name="lSGEE"></a>
### 辨识对象的函数
<a name="gG8Ly"></a>
#### (1)`classof(class name)`--返回java对象的类对象
```sql
select classof(cl).name from instanceof java.lang.ClassLoader cl
```
<a name="nSWO0"></a>
#### (2)`identical(object1,object2)`--返回是否两个对象是同一个实例
```sql
select identical(heap.findClass("java.lang.String").name, heap.findClass("java.lang.String").name)
```
<a name="GpQ7A"></a>
#### (3)`objectid(object)`--返回对象的id
```sql
select objectid(s) from java.lang.String s
```
<a name="SRYlI"></a>
#### (4)`reachables` --返回可从对象可到达的对象
```sql
select reachables(p) from java.util.Properties p --查询从Properties对象可到达的对象
select reachables(u, "java.net.URL.handler") from java.net.URL u --查询从URL对象可到达的对象，但不包括从URL.handler可到达的对象
```
<a name="JZgZW"></a>
#### (5)`referrers(object)`--返回引用某一对象的对象
```sql
select referrers(s) from java.lang.String s where s.count >100
```
<a name="opDOF"></a>
#### (6)`referees(object)`--返回某一对象引用的对象
```sql
select referees(s) from java.lang.String s where s.count >100
```
<a name="cfcHr"></a>
#### (7)`refers(object1,object2)`--返回是否第一个对象引用第二个对象
```sql
select refers(heap.findObject("0xd4d4d4d4"),heap.findObject("0xe4e4e4e4"))
```
<a name="tgZVl"></a>
#### (8)`root(object)`--返回是否对象是根集的成员
```sql
select root(heap.findObject("0xd4d4d4d4"))
```
<a name="OrQY4"></a>
#### (9)`sizeof(object)`--返回对象的大小
```sql
selectsizeof(o) from [I o
```
<a name="zAoTw"></a>
#### (10)`toHtml(object)`--返回对象的html格式
```sql
select"<b>"+ toHtml(o)+"</b>" from java.lang.Object o
```
<a name="qr64x"></a>
#### (11)选择多值
```sql
select{name:t.name?t.name.toString():"null",thread:t} from instanceof java.lang.Thread t
```
<a name="r77M5"></a>
### 数组、迭代器等函数
<a name="EZOaL"></a>
#### (1)`concat(enumeration1,enumeration2)`--将数组或枚举进行连接
```sql
select concat(referrers(p),referrers(p)) from java.util.Properties p
```
<a name="iESKj"></a>
#### (2)`contains(array, expression)`--数组中元素是否满足某表达式
```sql
select p from java.util.Propertieswhere contains(referres(p),"classof(it).name == 'java.lang.Class'")
```
返回由java.lang.Class引用的java.util.Properties对象<br />built-in变量<br />it --当前的迭代元素<br />index --当前迭代元素的索引<br />array --被迭代的数组
<a name="UCiQG"></a>
#### (3)`count(array, expression)`--满足某一条件的元素的数量
```sql
select count(heap.classes(),"/java.io./(it.name)")
```
<a name="wqaB7"></a>
#### (4)`filter(array, expression)`--过滤出满足某一条件的元素
```sql
select filter(heap.classes(),"/java.io./(it.name)")
```
<a name="QuEfa"></a>
#### (5)`length(array)`--返回数组长度
```sql
select length(heap.classes())
```
<a name="WPAQs"></a>
#### (6)`map(array,expression)`--根据表达式对数组中的元素进行转换映射
```sql
select map(heap.classes(),"index + '-->' + toHtml(it)")
```
<a name="gRqEC"></a>
#### (7)`max(array,expression)`--最大值, `min(array,expression)`
```sql
select max(heap.objects("java.lang.String"),"lhs.count>rhs.count")
```
built-in变量<br />lhs --左边元素<br />rhs --右边元素
<a name="kIjZt"></a>
#### (8)`sort(array,expression)`--排序
```sql
select sort(heap.objects('[C'),'sizeof(lhs)-sizeof(rhs)')
```
<a name="hMZW9"></a>
#### (9)`sum(array,expression)`--求和
```sql
select sum(heap.objects('[C'),'sizeof(it)')
```
<a name="n4BTF"></a>
#### (10)`toArray(array)`--返回数组
<a name="QVsBJ"></a>
#### (11)`unique(array)`--唯一化数组
