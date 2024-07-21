Python爬虫<br />爬虫时，很多网站返回的是HTML文件，可以用正则表达式（re库）或XPath语法来匹配目标内容，这两种方法属于爬虫的基本技能，实战中要会灵活运用。<br />对于一些结构和内容复杂的HTML，用re匹配会感觉力不从心，XPath可以解决问题。<br />快速入门XPath语法，当需要使用时，对照本文的语法和伪代码，可以快速写出正确的XPath代码，提取到目标内容。
<a name="gdqMc"></a>
## HTML、XML、XPath简介

---

- HTML是Hyper Text Markup Language（超文本标记语言）的缩写，在浏览器中看到的内容都是HTML代码经过浏览器渲染的结果。
- XML是EXtensible Markup Language（可扩展标记语言）的缩写，XML是一种很类似HTML的标记语言，不过XML的设计宗旨是传输数据，而非显示数据。

XML的标签可以自行定义，具有自我描述性。可以根据XML中的标签来获取对应的数据。

- XPath是XML Path Language（XML路径语言）的缩写，是一门在XML文档中查找信息的语言，用来提取XML文档中的元素和属性。

用XPath语言获取HTML中的内容时，先将HTML转换成XML文档，然后根据XML的树形结构来定位到指定的元素和属性，提取数据。如果有数据结构树的基础知识，理解起来很容易。<br />本文只分别用了一句话简短介绍，HTML、XML和树的知识可以自己扩展。
<a name="hIrhE"></a>
## XPath语法介绍

---

XPath使用路径表达式来选取XML文档中的节点或者节点集。
<a name="P3bn7"></a>
### 路径表达式
```bash
nodename   选取此节点的所有子节点。
/   从根节点选取。正斜杠也是路径分隔符。
//  从任意位置选取文档中的节点。
.   选取当前节点。
..  选取当前节点的父节点。
@   选取当前节点的属性
```
这些路径表达式和在常规的电脑文件系统中看到的表达式非常相似。<br />假如路径起始于正斜杠( `/` )，则此路径代表到某元素的绝对路径。
<a name="H9Fiq"></a>
### 通配符
```bash
*    任意元素。
@*   任意属性。
node()  任意子节点（元素，属性，内容)。
```
XPath的通配符可用来选取未知的XML元素。
<a name="KH0S9"></a>
### 谓语
```bash
//a[n] n为1开始的整数，选取排在第n个位置的<a>元素。
//a[last()] last()表示选取排在最后位置的<a>元素。
//a[last()-1] 和上面同理，表示选取倒数第二个<a>元素。
//a[position()<3] 选取第一个和第二个<a>元素。
//a[@href] 选取拥有href属性的<a>元素。
//a[@href='www.baidu.com'] 选取href属性值为'www.baidu.com'的<a>元素。
//a[@price>10] 选取price属性值大于10的<a>元素。
//a[@price>10]/span  选取price属性值大于10的<a>元素下的<span>元素。
```
谓语用来查找某个特定的节点，或包含某个指定值的节点，语法写在元素名后的方括号中，可以写元素的位置编号、函数、用`@`选取属性等。
<a name="m028f"></a>
### 选取多个路径
```bash
//book/title | //book/price  选取<book>元素的所有<title>和<price>元素。
//title | //price  选取所有<title>和<price>元素。
/bookstore/book/title | //price  选取属于<bookstore>元素的<book>元素的所有<title>元素，以及所有的<price>元素。
```
选取多个路径时，使用“|”运算符表示“或”的关系。
<a name="PT9rV"></a>
### 运算符
```bash
+ - * div 加减乘除。
= != 等于，不等于。
< <= 小于，小于等于。
> >= 大于，大于等于。
or and  或，与
mod   计算余数
```
运算符基本都是运用在谓语中，选取指定元素。
<a name="RIuxv"></a>
### 常用函数
`contains(@属性,string)` 选取属性里包含字符串string的元素。<br />`text()`  获取元素中的内容。<br />`last()`  选取最后一个元素。<br />`position()` 用于选取多个元素中某些位置（数字编号）的元素。<br />`count()`  返回元素的数量。<br />`max()` 返回最大的元素，`min()`,`avg()`,`sum()`同理。<br />函数也基本都是运用在谓语中，帮助选取指定元素。
<a name="loJT4"></a>
## lxml库使用

---

lxml是Python中用来解析HTML/XML的第三方库，lxml的主要功能是生成一个解析器，解析和提取HTML/XML中的数据。<br />先用lxml生成解析器，然后用XPath语法从解析器中提取HTML/XML中的内容。

- 安装命令：
```bash
pip install lxml
```

- 实战演练：
```python
# coding=utf-8
from lxml import etree

text = '''
<!DOCTYPE html>
<html>
<head>
    <title>XPath Test</title>
</head>
<body>
<div>
    <ul>
         <li id="1" class="item"><a href="link1.html">first item</a></li>
         <li id="2" class="item"><a href="link2.html">second item</a></li>
         <li id="3" class="item-inactive"><a href="link3.html">third item</a></li>
         <li id="4" class="project"><a href="link4.html">first project</a></li>
         <li id="5" class="project"><a href="link5.html">second project</a></li>
    </ul>
</div>
</body>
</html>
'''
# 利用etree.HTML，将字符串解析为HTML文档
html = etree.HTML(text)
# 读取html文件
# html = etree.parse('test.html')
print(html)
# 按字符串序列化HTML文档
result = etree.tostring(html, pretty_print=True)
print(result)
```
```python
<Element html at 0x1c3be4c9740>
b'<html>\n
<head>\n<title>XPath Test</title>\n</head>\n
<body>\n<div>\n    
<ul>\n         
<li id="1" class="item"><a href="link1.html">first item</a></li>\n         
<li id="2" class="item"><a href="link2.html">second item</a></li>\n         
<li id="3" class="item-inactive"><a href="link3.html">third item</a></li>\n         
<li id="4" class="project"><a href="link4.html">first project</a></li>\n         
<li id="5" class="project"><a href="link5.html">second project</a></li>\n    
</ul>\n
</div>\n</body>\n
</html>\n'
```
使用`from lxml import etree`从`lxml`库中导入`etree`解析器，利用`etree.HTML`将HTML格式的字符串解析成树状的结构，然后用XPath语法从中提取数据。<br />爬虫获取到的HTML内容一般是`response.text`中返回的，可以用`etree.HTML`解析，也可以用`etree.parse`解析本地的HTML文件。<br />直接打印`etree.HTML`或`etree.parse`解析的结果是一个`Element`对象，可以用`etree.tostring`将Element中的内容序列化，打印出来。

- 用XPath从解析器中提取数据：
```python
# 获取所有class属性值为'item'的<li>元素
infos = html.xpath("//li[@class='item']")
print(infos)
```
```python
[<Element li at 0x25951272840>, <Element li at 0x25951272780>]
```
```python
# 获取所有class属性中包含'pro'字符串的<li>元素
infos = html.xpath("//li[contains(@class, 'pro')]")
for info in infos:
    # 获取<li>元素中<a>元素的文本内容
    print(info.xpath("a/text()"))
```
```python
['first project']
['second project']
```
```python
# 获取倒数第二个<li>元素下的最后一个<a>元素
info = html.xpath("//li[last()-1]/a[last()]")
# 打印<a>元素的内容
print(info[0].text)
```
```python
first project
```
```python
# 获取前三个<li>元素下的<a>元素的内容
infos = html.xpath("//li[position()<4]/a/text()")
for info in infos:
	# 打印<a>元素的文本内容
    print(info)
```
```python
first item
second item
third item
```
