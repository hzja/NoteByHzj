Java<br />JAVA开发中经常会遇到不方便使用数据库，但又要进行结构化数据计算的场景。JAVA早期没有提供相关类库，即使排序、分组这种基本计算也要硬写代码，开发效率很低。后来JAVA8推出了Stream库，凭借Lambda表达式、链式编程风格、集合函数，才终于解决了结构化数据计算类库从无到有的问题。
<a name="zo2P4"></a>
## Stream可以简化结构化数据的计算
比如排序：
```java
Stream<Order> result=Orders
.sorted((sAmount1,sAmount2)->Double.compare(sAmount1.Amount,sAmount2.Amount))
.sorted((sClient1,sClient2)->CharSequence.compare(sClient2.Client,sClient1.Client));
```
上面代码中的`sorted`是集合函数，可方便地进行排序。"(参数)->函数体"的写法即Lambda表达式，可以简化匿名函数的定义。两个`sorted`函数连在一起用属于链式编程风格，可以使多步骤计算变得直观。
<a name="UbtJh"></a>
### Stream计算能力还不够强
仍然以上面的排序为例，sorted函数只需要知道排序字段和顺序/逆序就够了，参考SQL的写法"`…from Orders order by Client desc, Amount`"，但实际上还要额外输入排序字段的数据类型。顺序/逆序用asc/desc（或+/-）等符号就可以简单表示了，但这里却要用`compare`函数。另外，实际要排序的字段顺序和代码写出来的顺序是相反的，有些反直觉。<br />再比如分组汇总：
```java
Calendar cal=Calendar.getInstance();
Map<Object, DoubleSummaryStatistics> c=Orders.collect(Collectors.groupingBy(
	r->{
		cal.setTime(r.OrderDate);
		return cal.get(Calendar.YEAR)+"_"+r.SellerId;
	},
	Collectors.summarizingDouble(r->{
		return r.Amount;
	})
)
													 );
for(Object sellerid:c.keySet()){
	DoubleSummaryStatistics r =c.get(sellerid);
	String year_sellerid[]=((String)sellerid).split("_");
	System.out.println("group is (year):"+year_sellerid[0]+"\t (sellerid):"+year_sellerid[1]+"\t sum is："+r.getSum()+"\t count is："+r.getCount());
}
```
上面代码中，所有出现字段名的地方，都要先写上表名，即"表名.字段名"，而不能像SQL那样省略表名。匿名函数语法复杂，随着代码量的增加，复杂度迅速增长。两个匿名函数形成嵌套，代码更难解读。实现一个分组汇总功能要用多个函数和类，包括`groupingBy`、`collect`、`Collectors`、`summarizingDouble`、`DoubleSummaryStatistics`等，学习成本不低。分组汇总的结果是Map，而不是结构化数据类型，如果要继续计算，通常要定义新的结构化数据类型，并进行转换类型，处理过程很繁琐。两个分组字段在结构化数据计算中很常见，但函数grouping只支持一个分组变量，为了让一个变量代表两个字段，就要采取一些变通技巧，比如新建一个两字段的结构化数据类型，或者把两个字段用下划线拼起来，这让代码变得更加繁琐。<br />Stream计算能力不足，原因在于其基础语言JAVA是编译型语言，无法提供专业的结构化数据对象，缺少来自底层的有力支持。<br />JAVA是编译型语言，返回值的结构必须事先定义，遇到较多的中间步骤时，就要定义多个数据结构，这不仅让代码变得繁琐，还导致参数处理不灵活，要用一套复杂的规则来实现匿名语法。解释性语言则天然支持动态结构，还可以方便地将参数表达式指定为值参数或函数参数，提供更简单的匿名函数。<br />在这种情况下，Kotlin应运而生。Kotlin是基于JAVA的现代开发语言，所谓现代，重点体现在对JAVA语法尤其是Stream的改进上，即Lambda表达式更加简洁，集合函数更加丰富。
<a name="QIzVc"></a>
## Kotlin计算能力强于Stream
比如排序：
```kotlin
var resutl=Orders.sortedBy{it.Amount}.sortedByDescending{it.Client}
```
上面代码无须指明排序字段的数据类型，无须用函数表达顺序/逆序，直接引用it作为匿名函数的默认参数，而不是刻意定义，整体比Stream简短不少。
<a name="Jmot4"></a>
### Kotlin改进并不大，计算能力仍然不足
仍然以排序为例，Kotlin虽然提供了it这个默认参数，但理论上只要知道字段名就够了，没必要带上表名（it）。排序函数只能对一个字段进行排序，不能动态接收多个字段。<br />再比如分组汇总：
```kotlin
data class Grp(var OrderYear:Int,var SellerId:Int)
data class Agg(var sumAmount: Double,var rowCount:Int)
var result=Orders.groupingBy{Grp(it.OrderDate.year+1900,it.SellerId)}
	.fold(Agg(0.0,0),{
		acc, elem -> Agg(acc.sumAmount + elem.Amount,acc.rowCount+1)
	})
	.toSortedMap(compareBy<Grp> { it. OrderYear}.thenBy { it. SellerId})
result.forEach{println("group fields:${it.key.OrderYear}\t${it.key.SellerId}\t aggregate fields:${it.value.sumAmount}\t${it.value.rowCount}") }
```
上面代码中，一个分组汇总的动作，需要用到多个函数，包括复杂的嵌套函数。用到字段的地方要带上表名。分组汇总的结果不是结构化数据类型。要事先定义中间结果的数据结构。<br />如果继续考察集合、关联等更多的计算，就会发现同样的规律：Kotlin代码的确比Stream短一些，但大都是无关紧要的量变，并未发生深刻的质变，该有的步骤一个不少。<br />Kotlin也不支持动态数据结构，无法提供专业的结构化数据对象，难以真正简化Lambda语法，无法脱离表名直接引用字段，无法直接支持动态的多字段计算（比如多字段排序）。<br />esProc SPL的出现，将会彻底改观JAVA生态下结构化数据处理的困境。<br />esProc SPL是JVM下的开源结构化数据计算语言，提供了专业的结构化数据对象，内置丰富的计算函数，灵活简洁的语法，易于集成的JDBC接口，擅长简化复杂计算。
<a name="ZAh5F"></a>
## SPL内置丰富的计算函数实现基础计算
比如排序：`=Orders.sort(-Client, Amount)`<br />SPL无须指明排序字段的数据类型，无须用函数指明方向/逆序，使用字段时无须附带表名，一个函数就可以动态地对多个字段进行排序。<br />分组汇总：`=Orders.groups(year(OrderDate),Client; sum(Amount),count(1))`<br />上面的计算结果仍然是结构化数据对象，可以直接参与下一步计算。对双字段进行分组或汇总时，也不需要事先定义数据结构。整体代码没有多余的函数，sum和count用法简洁易懂，甚至很难觉察这是嵌套的匿名函数。<br />更多计算也同样简单：<br />去重：`=Orders.id(Client)`<br />模糊查询：`=Orders.select(Amount*Quantity>3000 && like(Client,"*S*"))`<br />关联：`=join(Orders:o,SellerId ; Employees:e,EId).groups(e.Dept; sum(o.Amount))`
<a name="Flmtj"></a>
### SPL提供了JDBC接口，可被JAVA代码无缝调用
```java
Class.forName("com.esproc.jdbc.InternalDriver");
Connection connection =DriverManager.getConnection("jdbc:esproc:local://");
Statement statement = connection.createStatement();
String str="=T(\"D:/Orders.xls\"). Orders.groups(year(OrderDate),Client; sum(Amount))";
ResultSet result = statement.executeQuery(str);
```
<a name="aCgw7"></a>
### SPL语法风格简洁灵活，具有强大的计算能力。
SPL可简化分步计算、有序计算、分组后计算等逻辑较复杂的计算，很多SQL/存储过程难以实现的计算，用SPL解决起来就很轻松。比如，找出销售额累计占到一半的前n个大客户，并按销售额从大到小排序：

|  | A | B |
| --- | --- | --- |
| 1 | … | /取数据 |
| 2 | =A1.sort(amount:-1) | /销售额逆序排序 |
| 3 | =A2.cumulate(amount) | /计算累计序列 |
| 4 | =A3.m(-1)/2 | /最后的累计即总额 |
| 5 | =A3.pselect(~>=A4) | /超过一半的位置 |
| 6 | =A2(to(A5)) | /按位置取值 |

除了计算能力，SPL在系统架构、数据源、中间数据存储、计算性能上也有一些特有的优势，这些优势有助于SPL进行库外结构化数据计算。
<a name="icXME"></a>
### SPL支持计算热切换和代码外置，可降低系统耦合性。
比如，将上面的SPL代码存为脚本文件，再在JAVA中以存储过程的形式调用文件名：
```java
Class.forName("com.esproc.jdbc.InternalDriver");
Connection connection =DriverManager.getConnection("jdbc:esproc:local://");
Statement statement = connection.createStatement();
ResultSet result = statement.executeQuery("call getClient()");
```
SPL是解释型语言，修改后可直接运行，无须编译，不必重启JAVA服务。SPL代码外置于JAVA，通过文件名被调用，不依赖JAVA代码，耦合性低。
<a name="xa4Cm"></a>
### SPL支持多种数据源，可进行跨源计算和跨库计算。
SPL支持各类数据库，txt\csv\xls等文件，MongoDB、Hadoop、redis、ElasticSearch、Kafka、Cassandra等NoSQL，特别地，还支持WebService XML、Restful Json等多层数据：

|  | A |
| --- | --- |
| 1 | =json(file("d:/Orders.json").read()) |
| 2 | =json(A1).conj() |
| 3 | =A2.select(Amount>p_start && Amount<=p_end) |

对文本文件和数据库进行跨源关联：

|  | A |
| --- | --- |
| 1 | =T("Employees.csv") |
| 2 | =mysql1.cursor("select SellerId, Amount from Orders order by SellerId") |
| 3 | =joinx(A2:O,SellerId; A1:E,EId) |
| 4 | =A3.groups(E.Dept;sum(O.Amount)) |

<a name="EV1zb"></a>
### SPL提供了自有存储格式，可临时或永久存储数据，并进行高性能计算。
SPL支持btx存储格式，适合暂存来自于低速数据源的数据，比如CSV：

|  | A | B |
| --- | --- | --- |
| 1 | =[T("d:/orders1.csv"), T("d:/orders2.csv")].merge@u() | /对记录做并集 |
| 2 | file("d:/fast.btx").export@b(A1) | /写入集文件 |

btx体积小，读写速度快，可以像普通文本文件那样进行计算：
```java
=T("D:/fast.btx").sort(Client,- Amount)
```
如果对btx进行有序存储，还能获得高计算性能，比如并行计算、二分查找。SPL还支持更高性能的ctx存储格式，支持压缩、列存、行存、分布式计算、大并发计算，适合持久存储大量数据，并进行高性能计算。<br />在数据库外的结构化数据计算方面，Stream做出了突破性的贡献；Kotlin加强了这种能力，但编译性语言的特性使它无法走得更远；要想彻底解决库外计算的难题，还需要SPL这种专业的结构化数据计算语言。
