Java
<a name="D67ie"></a>
### **json为什么能够取xml而代之？**
在正式开始介绍jsonpath之前，有没有想过为什么在网络数据传输领域，json能取代xml？<br />json相比于xml，最大的优势在于：json的语法更加简洁。<br />表达同样的内容，json占用的空间更小。<br />而数据体积越小，在传输过程中，就能做到传输速度更快。<br />简单来看一个例子。<br />首先，用xml描述一个人的信息。
```xml
<person>
	<name>张三</name>
	<age>26</age>
	<city>北京</city>
</person>
```
然后，用json描述一个人的信息。
```json
{
	"name":"张三",
	"age":26,
	"city":"北京"
}
```
不出意外，一眼就能看出二者的区别，很明显json用到的字符更少，占用的存储空间更小。
<a name="SylUx"></a>
### **传统的json处理**
通常在进行json解析的时候，都不需要太过复杂的处理方式。<br />只需要将json转为对象，或者对象转为json即可。<br />能够处理这种需求的工具很多，比如阿里开发的Fastjson、谷歌开发的Gson，都能满足这样简单的需求。<br />可如果是十分复杂的json呢？<br />比如说一层套一层、json数组套json对象，对象里又套数组... ...循环多层<br />这种情况怎么解析？<br />单单只是想一下就十分头疼了。<br />这种情况下，使用jsonpath就很简单了。<br />因为jsonpath可以使用表达式的方式，对json数据进行方便、高效的解析。
<a name="VymFv"></a>
### **jsonpath引入**
不同的编程语言，引入jsonpath的方法不同，此处使用Java作为演示。<br />不管是什么编程语言，都能通过这个例子理解到jsonpath的用法。
```xml
<dependency>
	<groupId>com.jayway.jsonpath</groupId>
	<artifactId>json-path</artifactId>
	<version>2.4.0</version>
</dependency>
```
<a name="GnepH"></a>
### **jsonpath解析复杂json数据**
下面是一个多层嵌套的复杂的json数据。
```json
{
	"productName":"电脑",
	"brand":[
		{
			"name":"戴尔",
			"price":5000,
			"parts":{
				"parts_3":"键盘",
				"parts_2":"鼠标",
				"parts_1":"电源",
			}
		},
		{
			"name":"华硕",
			"price":8000
		}
	]
}
```
使用jsonpath来进行解析，获取到最深层的数据。<br />获取到第四层数据的表达式：`$.brand[0].parts.parts_1`。
```java
String json = "xxxx";//json字符串
//初始化解析
Object document = Configuration.defaultConfiguration().jsonProvider().parse(json);
//获取值
String parts1 = JsonPath.read(document, "$.brand[0].parts.parts_1");
//打印结果
System.out.println("取到的值："+parts1);
```
控制台打印结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658925983569-141282b4-c026-4ca3-b385-99645b4eb015.png#clientId=u296c563e-947c-4&from=paste&id=u56baba48&originHeight=41&originWidth=179&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua5c61759-c179-46d0-96bf-25a2468576b&title=)<br />通过表达式的写法，能够轻松地取出位于第n层地数据，哪怕n=8、n=9... ...n=100，jsonpath都能以非常快的速度返回结果。
<a name="pwtVi"></a>
### **jsonpath高级用法**

1. 举一个例子，展示一下jsonpath的高级用法：**取出价格大于5000的电脑数据。**

还是使用上文的json数据来解析。<br />价格大于5000数据的表达式：`$.brand[?(@.price > 5000)]`。
```java
//略。。。

//获取值
net.minidev.json.JSONArray parts1 = JsonPath.read(document, "$.brand[?(@.price > 5000)]");
//打印结果
System.out.println("取到的值："+parts1);
```
控制台打印结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658925983608-c429e27c-8606-430f-9b61-73af1833358c.png#clientId=u296c563e-947c-4&from=paste&id=u7bdc4229&originHeight=48&originWidth=537&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7b2efc11-ac14-4be9-96d1-0dd38d5f8db&title=)<br />jsonpath不仅解析数据很简单，还能对数据进行过滤处理。<br />数据过滤主要依赖逻辑运算符来完成。<br />**jsonpath的逻辑运算符（部分）：**

- **==  :  左边等于右边；**
- **!=   :  左边不等于右边；**
- **<     :  左边小于右边；**
- **<=  :  左边小于或等于右边；**
- **>     :  左边大于右边；**
- **>=  :  左边大于或等于右边**

除开逻辑运算符，jsonpath还提供了一些函数供开发者使用。

2. 举例：**统计品牌提供商的数量；**

获取数量的表达式：`$.brand.length()`。
```java
//略。。。
//获取值
Integer parts1 = JsonPath.read(document, "$.brand.length()");
//打印结果
System.out.println("取到的值："+parts1);
```
控制台打印结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658925983626-3907d9bc-cb08-468e-b39e-4168acc51509.png#clientId=u296c563e-947c-4&from=paste&id=uf7317da2&originHeight=39&originWidth=154&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud5093516-ea35-4866-8d12-a77180aa855&title=)<br />**jsonpath的函数（部分）：**

- `**min()**`**        ：获取数组最小值；**
- `**max()**`**        ：获取数组最大值；**
- `**avg()**`**        ：获取数组平均值；**
- `**length()**`**  ：获取数组长度；**
