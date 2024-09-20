Java<br />推荐的，就是一款xml解析利器，并且是能够应付复杂xml解析的那种。<br />TA就是XPath。<br />简单来讲，XPath和jsonPath比较类似，使用起来十分简单，能够比较轻松地解析出需要的数据。<br />同时XPath和jsonpath一样，支持java、前端、Python等多种开发语言。<br />下面一起来看一下XPath的真面目吧。
<a name="V1eGW"></a>
### **XPath初体验**
下面是一个结构比较简单的xml文件。<br />本文演示，使用的编程语言是java，例子足够简单。<br />所以不管是用什么语言程序员，都能通过本文看懂XPath的用法。
```xml
<?xml version="1.0" encoding="utf-8"?>
<computerList>
	<computer id="001">
		<name>戴尔</name> <!--品牌-->
		<price>5000</price> <!--售价-->
		<stock>20</stock> <!--库存-->
		<product>北京</product> <!--产地-->
	</computer>
	<computer id="002">
		<name>华硕</name>
		<price>4000</price>
		<stock>300</stock>
		<product>浙江</product>
	</computer>
	<computer id="003">
		<name>荣耀</name>
		<price>3500</price>
		<stock>100</stock>
		<product>四川</product>
	</computer>
</computerList>
```
通过XPath表达式，提取出戴尔电脑的产地。
```java
public static void main(String[] args) {
	XPathFactory xPathFactory = XPathFactory.newInstance();
	XPath xPath = xPathFactory.newXPath();
	//读取xml文件,文件位于src目录下
	InputStream in = XmlTest.class.getResourceAsStream("/computer.xml");
	InputSource source = new InputSource(in);
	//开始解析
	String str = (String)xPath.evaluate("/computerList/computer[name='戴尔']/product", source);
	//打印结果
	System.out.println("xml获取的数据："+str);
}
```
下面是控制台打印结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658926494926-78a54bb3-7297-4d01-bf4e-8627094e23ee.png#clientId=ufea28b2a-1044-4&from=paste&id=ue925a41d&originHeight=50&originWidth=238&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5a1b873f-7ddf-49c6-a99b-432b48485c8&title=)<br />可以看到，通过XPath可以非常容易地对xml文件进行解析，提取出目标数据。<br />不管是多么复杂的xml文件，哪怕xml节点有100层，XPath都能轻松应对！<br />在这一点上，XPath和jsonpath十分类似，都依赖于表达式来完成。<br />下面来看一下XPath都有哪些常用的表达式吧。
<a name="sNBxg"></a>
### **XPath表达式**
常用的表达式如下。

- `**computerList**`**：获取 computerList 元素的所有子节点**
- `**/computerList **`**：获取根节点 computerList**
- `**//computer**`** ：获取所有 computer 子节点**
- `**/computerList/computer[1]**`** ：获取属于 computerList 子节点的第一个 computer 节点**
- `**/computerList/computer[last()]**`** ：获取属于 computerList 子节点的最后一个 computer 节点**
- `**/computerList/computer[position()<3]**`** ：获取最前面的两个属于 computerList 元素的子元素的 computer 节点**
- `**/computerList/computer[price>3500]**`** ：获取 price 大于3500的 computer 节点**
- `**//computer/name | //computer/price**`** ：获取 computer 元素的所有 name 和 price 节点**

举一个实际例子：**获取价格大于3500的电脑数据**。<br />用到的表达式为："/computerList/computer[price>3500]"
```java
public static void main(String[] args) {
	try{
		XPathFactory xPathFactory = XPathFactory.newInstance();
		XPath xPath = xPathFactory.newXPath();
		//读取xml文件,文件位于resource目录下
		InputStream in = XmlTest.class.getResourceAsStream("/computer.xml");
		InputSource source = new InputSource(in);
		//开始解析
		NodeList nodes = (NodeList)xPath.evaluate("/computerList/computer[price>3500]", source,XPathConstants.NODESET);
		for(int i=0; i<nodes.getLength(); i++){
			//创建解码器
			JAXBContext jc = JAXBContext.newInstance(Computer.class);
			Unmarshaller u = jc.createUnmarshaller();
			//进行解码
			Node item = nodes.item(i);
			Computer result =(Computer)u.unmarshal(item);
			//打印结果
			System.out.println("xml获取的数据-" +i+":"+result);
		}
	}catch (Exception e){
		System.out.println(e);
	}
}
```
控制台打印结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658926495006-026893f7-c3dd-4f25-845d-4b8ee321e3bb.png#clientId=ufea28b2a-1044-4&from=paste&id=uaee5b6fd&originHeight=85&originWidth=1025&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0bde4329-31f8-4e2c-9316-394f502e88a&title=)<br />小贴士：在java代码中，创建xml节点对应的实体类需要添加 `@XmlRootElement(name="computer")`注解，解析结果才能成功映射。
```java
@Data
@XmlRootElement(name="computer")
public class Computer {
    private String name;
    private Double price;
    private Integer stock;
    private String product;
}
```
<a name="Ekurq"></a>
### **XPath轴**
通过XPath轴，可以方便的取得当前节点的兄弟节点、父子节点等。<br />常用节点集参看下方列表：

- **ancestor ：获取当前节点的所有父、祖父节点**
- **ancestor-or-self ：获取当前节点的所有父、祖父节点、当前节点自身**
- **child ：获取当前节点的所有子节点**
- **descendant ：获取当前节点的所有子、孙节点**
- **descendant-or-self ：获取当前节点的所有子、孙节点、当前节点自身**
- **following-sibling ：获取当前节点之后的所有兄弟节点**
- **preceding-sibling ：获取当前节点之前的所有兄弟节点**

举一个实际例子：**获取computerList的所有子节点**。<br />用到的表达式为："  /computerList/child::*  "<br />Java代码同上一个例子一样，这里只贴出不同的一行。
```java
//开始解析
NodeList nodes = (NodeList)xPath.evaluate("/computerList/child::*", source,XPathConstants.NODESET);
```
控制台打印的结果为：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658926495018-6b317066-59f5-4344-b231-067bd8daeaa4.png#clientId=ufea28b2a-1044-4&from=paste&id=uae1187ef&originHeight=109&originWidth=999&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u14069bb0-684f-4188-9802-dc204dd2360&title=)
<a name="VC20o"></a>
### **XPath运算符**
在XPath的表达式中可以运用运算符做一些基本的计算、判断操作。<br />常用的运算符有：

- **+、-、*、div、mod ：加、减、乘、除、余数，计算结果为数值**
- **=、!=、<、<=、>、\>= ：这一批运算符，用来对两个值进行比较，返回true或false**

下面用两个例子来演示运算符的用法。<br />**戴尔电脑的库存 + 荣耀电脑的库存**
```java
//库存相加
String stock = (String)xPath.evaluate("/computerList/computer[name='戴尔']/stock+/computerList/computer[name='荣耀']/stock", source);
//打印结果
System.out.println("戴尔电脑的库存 + 荣耀电脑的库存:" +stock);
```
计算结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658926494956-395afd86-5990-466d-af19-05b80d051643.png#clientId=ufea28b2a-1044-4&from=paste&id=u45040d8f&originHeight=36&originWidth=429&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucad24fa0-5345-48f2-8d83-55d859c236a&title=)<br />**戴尔电脑的价格，是否等于荣耀电脑的价格**
```java
//比较价格是否相等
String priceEqual = (String)xPath.evaluate("/computerList/computer[name='戴尔']/price=/computerList/computer[name='荣耀']/price", source);
//打印结果
System.out.println("戴尔电脑的价格，是否等于荣耀电脑的价格:" +priceEqual);
```
计算结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658926494980-4ad599ca-f5ee-4769-89e5-42d9c46a522d.png#clientId=ufea28b2a-1044-4&from=paste&id=ue54674b8&originHeight=42&originWidth=527&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaa96f7e2-d69f-47bf-a84b-6558e397c81&title=)<br />对于XPath的介绍就讲解到这里，如果正好有解析复杂xml的需求，不妨试试TA吧。
