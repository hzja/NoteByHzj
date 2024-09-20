Python爬虫Beautiful Soup
<a name="BaYdy"></a>
## **使用Beautiful Soup解析数据**
Beautiful Soup是一个用于从HTML和XML文件中提取数据的Python模块。Beautiful Soup提供一些简单的函数用来处理导航、搜索、修改分析树等功能。Beautiful Soup 模块中的查找提取功能非常强大，而且非常便捷。Beautiful Soup自动输入文档转换为Unicode编码，输出文档转换为**UTF-8**编码。开发者**不需要考虑**编码方式，除非文档没有指定一个编码方式，这时，Beautiful Soup就不能自动识别编码方式了。
<a name="GYGox"></a>
### Beautiful Soup 的安装
目前推荐使用的是Beautiful Soup 4, 已经被移植到bs4当中，需要from bs4 然后导入Beautiful Soup 。
```bash
pip install bs4
```
<a name="KWLQl"></a>
### 解析器
Beautiful Soup支持Python标准库中包含的HTML解析器，但它也支持许多第三方Python解析器，其中包含lxml解析器。根据不同的操作系统，可以使用以下命令之一安装lxml:
```bash
$ apt-get install python-lxml
$ easy_install lxml
$ pip install lxml
```
另一个解析器是html5lib,它是一个用于解析HTML的Python库，按照Web浏览器的方式解析HTML。可以使用以下命令之一安装html5lib：
```bash
$ apt-get install python-html5lib
$ easy_install html5lib
$ pip install html5lib
```
关于每个解析器的优缺点如下表：

| 解析器 | 用   法 | 优 点 | 缺 点 |
| --- | --- | --- | --- |
| Python标准库 | `BeautifulSoup(markup, 'html.parser')` | Python 标准库执行速度适中 | （在Python2.7.3或3.2.2之前的版本中）文档容错能力差 |
| lxml的HTML解析器 | `BeautifulSoup(markup, 'lxml')` | 速度快文档容错能力强 | 需要安装C语言库 |
| lxml的XML解析器 | `BeautifulSoup(markup, 'lxml-xml')`<br />`BeautifulSoup(markup,'xml')` | 速度快唯一支持XML的解析器 | 需要安装C语言库 |
| html5lib | `BeautifulSoup(markup, 'html5lib')` | 最好的容错性，以浏览器的方式解析文档生成HTML5格式文档 | 速度慢，不依赖外部扩展 |

<a name="jqaUQ"></a>
### Beautiful Soup的简单应用
Beautiful Soup安装完成以后，下面将将介绍如何通过Beautiful Soup 库进行HTML的解析工作，具体示例步骤如下：<br />（1）导入bs4库，然后创建一个模拟HTML代码的字符串，代码如下：
```python
# 文件      ：使用BeautifulSoup解析HTML代码.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库

# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
<title>第一个 HTML 页面</title>
</head>
<body>
<p>body 元素的内容会显示在浏览器中。</p>
<p>title 元素的内容会显示在浏览器的标题栏中。</p>
</body>
</html>
"""
```
（2）创建BeautifulSoup对象，并指定解析器为lxml，最后通过打印的方式将解析的HTML代码显示在控制台当中，代码如下：
```python
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print(soup)                  # 打印解析的HTML代码
print(type(soup))            # 打印数据类型
```
程序运行结果如下：
```python
<html>
<head>
<title>第一个 HTML 页面</title>
</head>
<body>
<p>body 元素的内容会显示在浏览器中。</p>
<p>title 元素的内容会显示在浏览器的标题栏中。</p>
</body>
</html>

<class 'bs4.BeautifulSoup'>
```
<a name="Htr9k"></a>
#### 说 明
如果将html_doc字符串中的代码，保存在index.html文件中，可以通过打开HTML文件的方式进行代码解析，并且可以通过`prettify()`方法进行代码的格式化处理，代码如下：
```python
with open('index.html', 'w') as f:
	f.write('html_doc')
soup = BeautifulSoup(open("index.html"), "lxml")
print(soup.prettify())
```
<a name="CFGmr"></a>
## **获取节点内容**
使用Beautiful Soup 可以直接调用节点的名称，然后再调用对应的string属性便可以获取到节点内的文本信息。在单个节点结构层次非常清晰的情况下，使用这种方式提取节点信息的速度是非常快的。
<a name="jroeB"></a>
### 获取节点对应的代码
示例代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：使用BeautifulSoup解析HTML代码.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库

# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
<title>第一个 HTML 页面</title>
</head>
<body>
<p>body 元素的内容会显示在浏览器中。</p>
<p>title 元素的内容会显示在浏览器的标题栏中。</p>
</body>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文

with open('index.html', 'w') as f:
    f.write(html_doc)
soup = BeautifulSoup(open('index.html'), "lxml")
print(soup.prettify())
```
程序运行结果如下：
```python
head节点内容为：
 <head>
<title>第一个 HTML 页面</title>
</head>
body节点内容为：
 <body>
<p>body 元素的内容会显示在浏览器中。</p>
<p>title 元素的内容会显示在浏览器的标题栏中。</p>
</body>
title节点内容为：
 <title>第一个 HTML 页面</title>
p节点内容为：
 <p>body 元素的内容会显示在浏览器中。</p>
```
_注 意_<br />在打印p节点对应的代码时，会发现只打印了第一个P节点内容，这说明当多个节点时，该选择方式只会获取第一个节点中的内容，其他后面的节点将被忽略。<br />说明除了通过制订节点名称的方式获取节点内容以外，还可以使用name属性获取节点的名称，示例代码如下：
```python
# 获取节点名称
print(soup.head.name)
print(soup.body.name)
print(soup.title.name)
print(soup.p.name)
```
<br />程序运行结果如下：
```python
head
body
title
p
```
<a name="U1UfU"></a>
### 获取节点属性
每个节点可能都会含有多个属性，例如， class或者id等。如果已经选择了一个指定的节点名称，那么只需要调用attrs即可获取这个节点下的所有属性。代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：获取节点属性.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库

# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>横排响应式登录</title>
    <meta http-equiv="Content-Type" content="text/html" charset="utf-8"/>
    <meta name="viewport" content="width=device-width"/>
    <link href="font/css/bootstrap.min.css" type="text/css" rel="stylesheet">
    <link href="css/style.css" type="text/css" rel="stylesheet">
</head>
<body>
<h3>登录</h3>
<div class="glyphicon glyphicon-envelope"><input type="text" placeholder="请输入邮箱"></div>
<div class="glyphicon glyphicon-lock"><input type="password" placeholder="请输入密码"></div>
</body>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print('meta节点中属性如下：\n',soup.meta.attrs)
print('link节点中属性如下：\n',soup.link.attrs)
print('div节点中属性如下：\n',soup.div.attrs)
print('div节点中input属性如下：\n', soup.div.input.attrs)
```
程序运行结果如下：
```python
meta节点中属性如下：
 {'http-equiv': 'Content-Type', 'content': 'text/html', 'charset': 'utf-8'}
link节点中属性如下：
 {'href': 'font/css/bootstrap.min.css', 'type': 'text/css', 'rel': ['stylesheet']}
div节点中属性如下：
 {'class': ['glyphicon', 'glyphicon-envelope']}
div节点中input属性如下：
 {'type': 'text', 'placeholder': '请输入邮箱'}
```
在以上的运行结果中可以发现，attrs的返回结果为字典类型，字典中的元素分别是对应属性名称与对应的值。所以<br />在attrs后面添加[]括号并在括号内添加属性名称即可获取指定属性对应的值。代码如下：
```python
print('meta节点中http-equiv属性对应的值为：', soup.meta.attrs['http-equiv'])
print('link节点中href属性对应的值为：', soup.link.attrs['href'])
print('div节点中class属性对应的值为：', soup.div.attrs['class'])
```
程序运行结果如下：
```python
meta节点中http-equiv属性对应的值为：Content-Type
link节点中href属性对应的值为：font/css/bootstrap.min.css
div节点中class属性对应的值为： ['glyphicon', 'glyphicon-envelope']
```
在获取节点中指定属性所对应的值时，除了使用上面的方式外，还可以不写attrs，直接在节点后面以中括号的形式直接添加属性名称，来获取对应的值。代码如下：
```python
print('meta节点中http-equiv属性对应的值为：', soup.meta['http-equiv'])
print('link节点中href属性对应的值为：', soup.link['href'])
print('div节点中class属性对应的值为：', soup.div['class'])
```
<a name="UhMWS"></a>
### 获取节点包含的文本内容
实现获取节点包含的文本内容是非常简单的，只需要在节点名称后面添加string属性即可。代码如下：
```python
print('title节点内包含的文本内容为：', soup.title.string)
print('h3节点所包含的文本内容为：', soup.h3.string)
```
程序运行结果如下：
```python
title节点内包含的文本内容为： 横排响应式登录
h3节点所包含的文本内容为： 登录
```
<a name="Bpf1K"></a>
### 嵌套获取节点内容
HTML代码中的每个节点都会出现嵌套的可能，而使用Beautiful Soup获取每个节点的内容时，以通过“."直接获取下一个节点中的内容（当前节点的子节点）。代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：获取嵌套节点内容.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库

# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>横排响应式登录</title>
    <meta http-equiv="Content-Type" content="text/html" charset="utf-8"/>
    <meta name="viewport" content="width=device-width"/>
    <link href="font/css/bootstrap.min.css" type="text/css" rel="stylesheet">
    <link href="css/style.css" type="text/css" rel="stylesheet">
</head>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print('head节点内容如下：\n',soup.head)
print('head节点数据类型为：',type(soup.head))
print('head节点中title节点内容如下：\n',soup.head.title)
print('head节点中title节点数据类型为：',type(soup.head.title))
print('head节点中title节点中的文本内容为：',soup.head.title.string)
print('head节点中title节点中文本内容的数据类型为：',type(soup.head.title.string))
```
程序运行结果如下：
```python
head节点内容如下：
 <head>
<title>横排响应式登录</title>
<meta charset="utf-8" content="text/html" http-equiv="Content-Type"/>
<meta content="width=device-width" name="viewport"/>
<link href="font/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>
<link href="css/style.css" rel="stylesheet" type="text/css"/>
</head>
head节点数据类型为： <class 'bs4.element.Tag'>
head节点中title节点内容如下：
 <title>横排响应式登录</title>
head节点中title节点数据类型为： <class 'bs4.element.Tag'>
head节点中title节点中的文本内容为： 横排响应式登录
head节点中title节点中文本内容的数据类型为： <class 'bs4.element.NavigableString'>
```
<a name="LZB5w"></a>
#### 说 明
在上面的运行结果中可以看出，在获取head与其内部的title节点内容时数据类型均为“<class 'bs4.element.Tag'>“，也就说明在Tag类型的基础上可以获取当前节点的子节点内容，这样的获取方式叫做嵌套获取节点内容。
<a name="QL28q"></a>
### 关联获取
在获取节点内容时，不一定都能做到一步获取指定节点中的内容，有时还需要先确认某一个节点，然后以该节点为中心获取对应的子节点、孙节点、父节点以及兄弟节点。
<a name="wOF9n"></a>
#### 1、获取子节点
在获取某节点下面的所有子节点时，可以使用contents或者是children属性来实现，其中contents返回的是一个列表，在这列表中的每个元素都是一个子节点内容，而children返回的则是一个"list_iterator"类型的可迭代对象。获取所有子节点的代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：实现获取某节点下所有子节点内容.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库

# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>关联获取演示</title>
    <meta charset="utf-8"/>
</head>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print(soup.head.contents)           # 列表形式打印head下所有子节点
print(soup.head.children)           # 可迭代对象形式打印head下所有子节点
```
程序运行结果如下：
```python
['\n', <title>关联获取演示</title>, '\n', <meta charset="utf-8"/>, '\n']
<list_iterator object at 0x7f7cf94fcfa0>
```
从上面结果可以看出，通过head.contents所获取的所有子节点中有三个换行符\n以及两个子标题(title与meta)对应的所有内容。head.children所获取的则是一个'list_iterator'可迭代对象，如果需要的获取该对象中的所有内容可以直接将其转换为list类型或者通过for循环遍历的方式进行获取。代码如下：
```python
print(list(soup.head.children))   # 打印将可迭代对象转换为列表形式的所有子节点
for i in soup.head.children:    # 循环遍历可迭代对象中的所有子节点
	print(i)             # 打印子节点内容
```
程序运行结果如下：
```python
['\n', <title>关联获取演示</title>, '\n', <meta charset="utf-8"/>, '\n']


<title>关联获取演示</title>


<meta charset="utf-8"/>
```
<a name="vQbjZ"></a>
#### 2、获取孙节点
在获取某节点下面所有的子孙节点时，可以使用descendants属性来实现，该属性会返回一个generator对象，获取该对象中的所有内容时，同样可以直接将其转换为list 类型或者通过for循环遍历的方式进行获取。这里以for循环遍历方式为例，代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：使用descendants属性获取子孙节点内容.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库

# 创建模拟HTML代码的字符串
html_doc = """
<html>
…此处省略…
<body>
<div id="test1">
    <div id="test2">
        <ul>
            <li class="test3" value = "user1234">
                此处为演示信息
            </li>
        </ul>
    </div>
</div>
</body>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print(soup.body.descendants)          # 打印body节点下所有子孙节点内容的generator对象
for i in soup.body.descendants:       # 循环遍历generator对象中的所有子孙节点
    print(i)                            # 打印子孙节点内容
```
程序运行如下：

1. 所有子孙节点内容的generator对象
```python
<generator object Tag.descendants at 0x7ff1b24f3580>
<div id="test1">
<div id="test2">
<ul>
<li class="test3" value="user1234">
                此处为演示信息
            </li>
</ul>
</div>
</div>
```

1. body节点下所有子节点内容
2. body节点下所有孙节点及以下内容
```python
<div id="test2">
<ul>
<li class="test3" value="user1234">
                此处为演示信息
            </li>
</ul>
</div>


<ul>
<li class="test3" value="user1234">
                此处为演示信息
            </li>
</ul>


<li class="test3" value="user1234">
                此处为演示信息
            </li>

                此处为演示信息
```
<a name="yXTiB"></a>
#### 3、获取父节点
获取父节点有两种方式：一种是通过parent属性直接获取指定节点的父节点内容，还可以 通过parents属性获取指定节点的父节点及以上（祖先节点）内容，只是parents属性会返回一个generator对象，获取该对象中的所有内容时，同样可以直接将其转换为list类型或者通过for 循环遍历的方式进行获取。这里以for循环遍历方式为例，代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：获取父节点及祖先节点内容.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库

# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>关联获取演示</title>
    <meta charset="utf-8"/>
</head>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print(soup.title.parent)                # 打印title节点的父节点内容
print(soup.title.parents)               # 打印title节点的父节点及以上内容的generator对象
for i in soup.title.parents:           # 循环遍历generator对象中的所有父节点及以上内容
    print(i.name)                       # 打印父节点及祖先节点名称
```
直接获取title节点的父节点内容
```python
<head>
<title>关联获取演示</title>
<meta charset="utf-8"/>
</head>
```
父节点及以上内容的generator对象
```python
<generator object PageElement.parents at 0x7fb0b8df36d0>
```
循环遍历父节点及祖先节点的名称
```python
head
html
[document]
```
_说 明_<br />在上面的运行结果可以看出，parents属性所获取父节点的顺序为head,html，最后的[document]表示文档对象，既是整个HTML文档，也是BeautifulSoup对象。
<a name="PUPKX"></a>
#### 4、获取兄弟节点
兄弟节点也就是同级节点，表示在同一级节点内的所有子节点间的关系。如，在一段HTML代码中获取第一个p节点的下一个div兄弟节点时可以使用next_sibling属性，如果想获取当前div节点的上一个兄弟节点p时可以使用previous_sibling属性。通过这两个属性获取兄弟节点时，如果两个节点之间含有换行符（\n)、空字符或者是其他文本内容时，将返回这些文本节点。代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：获取兄弟节点.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库

# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>关联获取演示</title>
    <meta charset="utf-8"/>
</head>
<body>
<p class="p-1" value = "1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>
第一个p节点下文本
<div class="div-1" value = "2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></div>
<p class="p-3" value = "3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>
<div class="div-2" value = "4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></div>
</body>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print(soup.p.next_sibling)               # 打印第一个p节点下一个兄弟节点（文本节点内容）
print(list(soup.p.next_sibling))         # 以列表形式打印文本节点中的所有元素
div = soup.p.next_sibling.next_sibling  # 获取p节点同级的第一个div节点
print(div)                               # 打印第一个div节点内容
print(div.previous_sibling)              # 打印第一个div节点上一个兄弟节点（文本节点内容）
```
程序运行结果，第一个p节点的下一个兄弟节点：
```python
第一个p节点下文本
```
文本节点中的所有元素
```python
['\n', '第', '一', '个', 'p', '节', '点', '下', '文', '本', '\n']
```
第一个div节点内容
```python
<div class="div-1" value="2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></div>
```
第一个div节点上一个兄弟节点
```python
第一个p节点下文本
```
如果想获取当前节点后面的所有兄弟节点时，可以使用next_siblings属性。如果想获取当前节点前面的所有兄弟节点时可以使用previsou_siblings属性。通过这两个属性所获取的节点都将以generator(可迭代对象)的形式返回。在获取节点的内容时，同样可以直接将其转换为list类型或者通过for循环遍历的方式进行获取。这里以转换list类型为例，代码如下：
```python
print('获取p节点后面的所有兄弟节点如下：\n', list(soup.p.next_siblings))
print('获取p节点前面的所有兄弟节点如下：\n', list(soup.p.previous_siblings))
```
程序运行结果如下：
```python
获取p节点后面的所有兄弟节点如下：
 ['\n第一个p节点下文本\n', <div class="div-1" value="2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></div>, '\n', <p class="p-3" value="3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>, '\n', <div class="div-2" value="4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></div>, '\n']
获取p节点前面的所有兄弟节点如下：
 ['\n']
```
<a name="QJA58"></a>
## **使用**`**find()**`**方法获取内容**
在HTML代码中获取比较复杂的内容时，可以使用`find_all()`方法与`find()`方法。调用这些方法，然后传入指定的参数即可灵活的获取节点中的内容。
<a name="cnRAi"></a>
### `find_all()`——获取所有符合条件的内容
Beautiful Soup提供了一个find_all()方法，该方法可以获取所有符合条件的内容。语法格式如下：
```python
find_all(name = None, attrs = {}, recursive = True, text = None, limit = None, **kwargs)
```
<a name="w0waC"></a>
#### 1、name参数
name参数用来指定节点名称，指定该参数以后将返回一个可迭代对象，所有符合条件的内容均为对象中的一个元素。代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：find_all(name)通过节点名称获取内容.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库

# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>关联获取演示</title>
    <meta charset="utf-8"/>
</head>

<body>
<p class="p-1" value = "1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>
<p class="p-2" value = "2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>
<p class="p-3" value = "3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>
<div class="div-2" value = "4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></div>
</body>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print(soup.find_all(name='p'))           # 打印名称为p的所有节点内容
print(type(soup.find_all(name='p')))     # 打印数据类型
```
程序运行结果如下：
```python
[<p class="p-1" value="1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>, <p class="p-2" value="2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>, <p class="p-3" value="3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>]
<class 'bs4.element.ResultSet'>
```
<a name="vTX28"></a>
##### 说 明
`bs4.element.ResultSet`类型的数据与Python中的列表类似，如果想获取可迭代对象中的某条件数据可以使用切片的方式进行，如获取所有P节点中的第一个可以参考如下代码：
```python
print(soup.find_all(name='p')[0])   # 打印所有p节点中的第一个元素
```
因为`bs4.element.ResultSet`数据中的每一个元素都是bs4.element.Tag类型，所以可以直接对某一个元素进行嵌套获取。代码如下：
```python
print(type(soup.find_all(name='p')[0]))   # 打印数据类型
print(soup.find_all(name = 'p')[0].find_all(name = 'a'))  # 打印第一个p节点内的子节点a
```
程序运行结果如下：
```python
<class 'bs4.element.Tag'>
[<a href="https://item.jd.com/12353915.html">零基础学Python</a>]
```
<a name="bnZ95"></a>
#### 2、attrs参数
attrs参数表示通过指定属性进行数据的获取工作，在填写attrs参数时，默认情况下需要填写**字典类型**的参数值，但也可以通过**赋值**的方式填写参数。代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：find_all(attrs)通过指定属性获取内容.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库
import re                      # 导入正则表达式模块
# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>关联获取演示</title>
    <meta charset="utf-8"/>
</head>
<body>
<p class="p-1" value = "1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>
<p class="p-1" value = "2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>
<p class="p-3" value = "3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>
<div class="div-2" value = "4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></div>
</body>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print('指定字符串所获取的内容如下：')
print(soup.find_all(text='零基础学Python'))         # 打印指定字符串所获取的内容
print('指定正则表达式对象所获取的内容如下：')
print(soup.find_all(text=re.compile('Python')))    # 打印指定正则表达式对象所获取的内容
```
程序运行结果如下：
```python
字典参数结果如下：
[<p class="p-1" value="1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>]
赋值参数结果如下：
[<p class="p-1" value="1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>, <p class="p-1" value="2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>]
[<p class="p-3" value="3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>]
```
<a name="rSwWi"></a>
#### 3、text参数
指定text参数可以获取节点中的文本，该参数可以指定字符串或者正则表达式对象。代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：find_all(attrs)通过指定属性获取内容.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库
import re                      # 导入正则表达式模块
# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>关联获取演示</title>
    <meta charset="utf-8"/>
</head>
<body>
<p class="p-1" value = "1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>
<p class="p-1" value = "2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>
<p class="p-3" value = "3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>
<div class="div-2" value = "4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></div>
</body>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print('指定字符串所获取的内容如下：')
print(soup.find_all(text='零基础学Python'))         # 打印指定字符串所获取的内容
print('指定正则表达式对象所获取的内容如下：')
print(soup.find_all(text=re.compile('Python')))    # 打印指定正则表达式对象所获取的内容
```
程序运行结果如下：
```python
指定字符串所获取的内容如下：
['零基础学Python']
指定正则表达式对象所获取的内容如下：
['零基础学Python', 'Python从入门到项目实践', 'Python项目开发案例集锦', 'Python编程锦囊']
```
<a name="Uddda"></a>
### `find()`——获取第一个匹配的节点内容
`find_all()`方法可以获取所有符合条件的节点内容，而find()方法只能获取第一个匹配的节点内容。<br />代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：find()方法获取第一个匹配的节点内容.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库
import re                      # 导入正则表达式模块
# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>关联获取演示</title>
    <meta charset="utf-8"/>
</head>
<body>
<p class="p-1" value = "1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>
<p class="p-1" value = "2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>
<p class="p-3" value = "3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>
<div class="div-2" value = "4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></div>
</body>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print(soup.find(name='p'))                  # 打印第一个name为p的节点内容
print(soup.find(class_='p-3'))              # 打印第一个class为p-3的节点内容
print(soup.find(attrs={'value':'4'}))       # 打印第一个value为4的节点内容
print(soup.find(text=re.compile('Python'))) # 打印第一个文本中包含Python的文本信息
```
程序运行结果如下：
```python
<p class="p-1" value="1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>
<p class="p-3" value="3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>
<div class="div-2" value="4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></div>
零基础学Python
```
<a name="BKDMx"></a>
#### 其他方法
除了`find_all()`和`find()`方法可以实现按照指定条件获取节点内容以外，Beautiful Soup模块还提供了多个其他方法，这些方法的使用方式与`find_all()`和`find()`方法相同，只是查询的范围不同，各个方法的具体说明如下：<br />**根据条件获取节点内容的其他方法及描述**

| 方 法 名 称 | 描  述 |
| --- | --- |
| `find_parent()` | 获取父节点内容 |
| `find_parents()` | 获取所有祖先节点内容 |
| `find_next_sibling()` | 获取后面第一个兄弟节点内容 |
| `find_previous_siblings()` | 获取前面第一个兄弟节点内容 |
| `find_next_siblings()` | 获取后面所有兄弟节点内容 |
| `find_previous_siblings()` | 获取前面所有兄弟节点内容 |
| `find_next()` | 获取当前节点的下一个符合条件的节点内容 |
| `find_all_next()` | 获取当前节点的下一个所有符合条件的节点内容 |
| `find_previous()` | 获取第一个符合条件的节点内容 |
| `find_all_previous()` | 获取所有符合条件的节点内容 |

<a name="nQjGO"></a>
## **CSS选择器**
Beautiful Soup模块还提供了CSS选择器来获取节点内容，如果是Tag或者是Beautiful Soup对象都可以直接调用`select()`方法，然后填写指定参数即可通过CSS选择器获取到节点中的内容。
<a name="QSXBG"></a>
### 说 明
CSS选择器参考手册<br />[https://www.w3school.com.cn/cssref/css_selectors.asp](https://www.w3school.com.cn/cssref/css_selectors.asp)<br />在使用CSS选择器获取节点内容时，首先需要调用select()方法，然后为其指定字符串类型的CSS选择器。常见的CSS选择器如下：<br />§ 直接填写字符串类型的节点名称<br />§ .class：表示指定class属性值<br />§ #id：表示指定id属性的值<br />使用CSS选择器调用`select()`方法获取节点内容<br />示例代码如下：
```python
#_*_coding:utf-8_*_
# 文件      ：使用CSS选择器获取节点内容.py
# IDE      ：PyCharm

from bs4 import BeautifulSoup  # 导入BeautifulSoup库
# 创建模拟HTML代码的字符串
html_doc = """
<html>
<head>
    <title>关联获取演示</title>
    <meta charset="utf-8"/>
</head>
<body>
    <div class="test_1" id="class_1"> 
        <p class="p-1" value = "1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>
        <p class="p-2" value = "2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>
        <p class="p-3" value = "3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>
        <p class="p-4" value = "4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></p>
    </div>
    <div class="test_2" id="class_2">
        <p class="p-5"><a href="https://item.jd.com/12185501.html">零基础学Java（全彩版）</a></p>
        <p class="p-6"><a href="https://item.jd.com/12199033.html">零基础学Android（全彩版）</a></p>
        <p class="p-7"><a href="https://item.jd.com/12250414.html">零基础学C语言（全彩版）</a></p>
    </div>    
</body>
</html>
"""
# 创建一个BeautifulSoup对象，获取页面正文
soup = BeautifulSoup(html_doc, features="lxml")
print('所有p节点内容如下：')
print(soup.select('p'))                # 打印所有p节点内容
print('所有p节点中的第二个p节点内容如下：')
print(soup.select('p')[1])             # 打印所有p节点中的第二个p节点
print('逐层获取的title节点如下：')
print(soup.select('html head title'))  # 打印逐层获取的title节点
print('类名为test_2所对应的节点如下：')
print(soup.select('.test_2'))          # 打印类名为test_2所对应的节点
print('id值为class_1所对应的节点如下：')
print(soup.select('#class_1'))         # 打印id值为class_1所对应的节点
```
程序运行结果如下：
```python
所有p节点内容如下：
[<p class="p-1" value="1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>, <p class="p-2" value="2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>, <p class="p-3" value="3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>, <p class="p-4" value="4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></p>, <p class="p-5"><a href="https://item.jd.com/12185501.html">零基础学Java（全彩版）</a></p>, <p class="p-6"><a href="https://item.jd.com/12199033.html">零基础学Android（全彩版）</a></p>, <p class="p-7"><a href="https://item.jd.com/12250414.html">零基础学C语言（全彩版）</a></p>]
所有p节点中的第二个p节点内容如下：
<p class="p-2" value="2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>
逐层获取的title节点如下：
[<title>关联获取演示</title>]
类名为test_2所对应的节点如下：
[<div class="test_2" id="class_2">
 <p class="p-5"><a href="https://item.jd.com/12185501.html">零基础学Java（全彩版）</a></p>
 <p class="p-6"><a href="https://item.jd.com/12199033.html">零基础学Android（全彩版）</a></p>
 <p class="p-7"><a href="https://item.jd.com/12250414.html">零基础学C语言（全彩版）</a></p>
 </div>]
id值为class_1所对应的节点如下：
[<div class="test_1" id="class_1">
 <p class="p-1" value="1"><a href="https://item.jd.com/12353915.html">零基础学Python</a></p>
 <p class="p-2" value="2"><a href="https://item.jd.com/12451724.html">Python从入门到项目实践</a></p>
 <p class="p-3" value="3"><a href="https://item.jd.com/12512461.html">Python项目开发案例集锦</a></p>
 <p class="p-4" value="4"><a href="https://item.jd.com/12550531.html">Python编程锦囊</a></p>
</div>]
```
`select()`方法除了以上的基本使用方法以外，还可以实现**嵌套获取**、**获取属性值**以及**获取文本**等。<br />**根据条件获取节点内容的其他方法及描述**

| 获取节点内容的方式 | 描   述 |
| --- | --- |
| `soup.select('div[class="test_1"]')[0].<br />select('p')[0]` | 嵌套获取class名为test_1对应的div中所有p节点中的第一个 |
| `soup.select('p')[0]['value']`<br />`soup.select('p')[0].attrs['value']` | 获取所有p节点中第一个节点内**value属性**对应的值（两种方式） |
| `soup.select('p')[0].get_text()`<br />`soup.select('p')[0].string` | 获取所有p节点中第一个节点内的**文本**（两种方式） |
| `soup.select（'p'([1:])` | 获取所有p节点中第二个以后的p节点 |
| `soup.select('.p-1, .p-5')` | 获取class名为p-1与p-5对应的节点 |
| `soup.select('a[href]')` | 获取存在href属性的所有a节点 |
| `soup.select('p[value = "1"]')` | 获取所有属性值为value = "1"的p节点 |

<a name="VBipg"></a>
### 说 明
Beautiful Soup 模块还提供了一个`select_one()`方法，用于获取所有符合条件节点的第一个节点，例如`soup.select_one('a')`将获取所有a节点中的第一个a节点内容。
