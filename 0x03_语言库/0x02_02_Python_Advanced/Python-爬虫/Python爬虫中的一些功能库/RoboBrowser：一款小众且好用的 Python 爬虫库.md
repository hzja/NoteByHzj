Python 爬虫
<a name="ZoNV1"></a>
## 1、前言
RoboBrowser，Your friendly neighborhood web scraper！由纯 Python 编写，运行无需独立的浏览器，它不仅可以做爬虫，还可以实现 Web 端的自动化<br />项目地址：[https://github.com/jmcarp/robobrowser](https://github.com/jmcarp/robobrowser)
<a name="nHsRq"></a>
## 2、安装及用法
在实战之前，先安装依赖库及解析器<br />PS：官方推荐的解析器是 「lxml」
```bash
# 安装依赖
pip3 install robobrowser

# lxml解析器（官方推荐）
pip3 install lxml
```
RoboBrowser 常见的 2 个功能为：

- 模拟表单 Form 提交
- 网页数据爬取

使用 RoboBrowser 进行网页数据爬取，常见的 3 个方法如下：

- find查询当前页面满足条件的第一个元素
- find_all查询当前页面拥有共同属性的一个列表元素
- select通过 CSS 选择器，查询页面，返回一个元素列表

需要指出的是，RoboBrowser 依赖于 BS4，所以它的使用方法和 BS4 类似<br />更多功能可以参考：[https://www.crummy.com/software/BeautifulSoup/bs4/doc.zh/](https://www.crummy.com/software/BeautifulSoup/bs4/doc.zh/)
<a name="N7BQr"></a>
## 3、实战一下
以「 百度搜索及爬取搜索结果列表 」为例
<a name="kBcfT"></a>
### 3-1  打开目标网站
首先，实例化一个 RoboBrowser 对象
```python
from time import sleep

from robobrowser import RoboBrowser

home_url = 'https://baidu.com'

#  parser: 解析器，HTML parser; used by BeautifulSoup
#  官方推荐：lxml
rb = RoboBrowser(history=True, parser='lxml')

# 打开目标网站
rb.open(home_url)
```
然后，使用 RoboBrowser 实例对象中的 `open()` 方法打开目标网站
<a name="rnG18"></a>
### 3-2  自动化表单提交
首先，使用 RoboBrowser 实例对象获取网页中的表单 Form<br />然后，通过为表单中的输入框赋值模拟输入操作<br />最后，使用 `submit_form()` 方法进行表单提交，模拟一次搜索操作
```python
# 获取表单对象
bd_form = rb.get_form()

print(bd_form)

bd_form['wd'].value = "AirPython"

# 提交表单，模拟一次搜索
rb.submit_form(bd_form)
```
<a name="sAbvw"></a>
### 3-3  数据爬取
分析搜索页面的网页结构，利用 RoboBrowser 中的 select() 方法匹配出所有的搜索列表元素<br />遍历搜索列表元素，使用 find() 方法查询出每一项的标题及 href 链接地址
```python
# 查看结果
result_elements = rb.select(".result")

# 搜索结果
search_result = []

# 第一项的链接地址
first_href = ''

for index, element in enumerate(result_elements):
    title = element.find("a").text
    href = element.find("a")['href']
    search_result.append(title)

    if index == 0:
        first_href = element.find("a")
        print('第一项地址为:', href)

print(search_result)

最后，使用 RoboBrowser 中的 follow_link() 方法模拟一下「点击链接，查看网页详情」的操作
# 跳转到第一个链接
rb.follow_link(first_href)

# 获取历史
print(rb.url)
```
需要注意的是，`follow_link()` 方法的参数为带有 href 值的 a 标签
<a name="O2Vkk"></a>
## 4、最后
文中结合百度搜索实例，使用 RoboBrowser 完成了一次自动化及爬虫操作<br />相比 Selenium、Helium 等，RoboBrowser 更轻量级，不依赖独立的浏览器及驱动<br />如果想处理一些简单的爬虫或 Web 自动化，RoboBrowser 完全够用；但是面对一些复杂的自动化场景，更建议使用 Selenium、Pyppeteer、Helium 等
