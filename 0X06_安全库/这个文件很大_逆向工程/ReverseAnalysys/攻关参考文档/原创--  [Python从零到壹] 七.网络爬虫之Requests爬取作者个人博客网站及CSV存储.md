# 原创
：  [Python从零到壹] 七.网络爬虫之Requests爬取作者个人博客网站及CSV存储

# [Python从零到壹] 七.网络爬虫之Requests爬取作者个人博客网站及CSV存储

欢迎大家来到“Python从零到壹”，在这里我将分享约200篇Python系列文章，带大家一起去学习和玩耍，看看Python这个有趣的世界。所有文章都将结合案例、代码和作者的经验讲解，真心想把自己近十年的编程经验分享给大家，希望对您有所帮助，文章中不足之处也请海涵。Python系列整体框架包括基础语法10篇、网络爬虫30篇、可视化分析10篇、机器学习20篇、大数据分析20篇、图像识别30篇、人工智能40篇、Python安全20篇、其他技巧10篇。您的关注、点赞和转发就是对秀璋最大的支持，知识无价人有情，希望我们都能在人生路上开心快乐、共同成长。

**前一篇文章讲述了 BeautifulSoup 爬取作者个人博客网站，通过案例的方式让大家熟悉Python网络爬虫。这篇文章将详细讲解Requests库爬取作者个人博客网站，并存储至CSV文件。原本想讲解DB-TOP250案例，但因为某些原因修改成了个人博客爬取，方法类似，都能普及简单的预处理知识。** 希望对您有所帮助，本文参考了作者CSDN的文章和学生杨友的博客，从学生的角度实现网络爬虫，可能对读者更友好。参考链接如下：

#### 文章目录

最后推荐大家关注我学生CSDN的博客，十分怀恋给他们上课的情形，博客也写得不错，写作风格和我也很像，哈哈~ 学生杨友问我 <mark>“他现在不编程该行了，觉得遗憾吗？”</mark> 我的回答是 <mark>“有点遗憾，但只要是我学生的选择，自己喜欢，我都支持；也希望他们积极的去做，把每一件事做好做深，如有需要定会帮助，一起加油！”</mark>

> 
作者新开的“娜璋AI安全之家”将专注于Python和安全技术，主要分享Web渗透、系统安全、人工智能、大数据分析、图像识别、恶意代码检测、CVE复现、威胁情报分析等文章。虽然作者是一名技术小白，但会保证每一篇文章都会很用心地撰写，希望这些基础性文章对你有所帮助，在Python和安全路上与大家一起进步。


**前文赏析：**

---


## 一.requests基本用法

requests模块是用Python语言编写的、基于urllib的第三方库，采用Apache2 Licensed开源协议的http库。它比urllib更方便简洁，既可以节约大量的工作，又完全满足http测试需求。requests是一个很实用的Python库，编写爬虫和测试服务器响应数据时经常会用到，使用requests可以轻而易举的完成浏览器相关操作。功能包括：

推荐大家从requests官方网站进行学习，这里只做简单介绍。官方文档地址：

---


假设读者已经使用“pip install requests”安装了requests模块，下面讲解该模块的基本用法。

**1.导入requests模块**<br/> 使用语句如下：

```
import requests

```

**2.发送请求**<br/> requests模块可以发送http常用的两种请求：GET请求和POST请求。其中GET请求可以采用url参数传递数据，它是从服务器上获取数据；而POST请求是向服务器传递数据，该方法更为安全，更多用法请读者下来学习。

下面给出使用GET请求和POST请求获取某个网页的方法，得到一个命名为r的Response对象，通过这个对象获取我们所需的信息。

```
import requests

r = requests.get('https://github.com/timeline.json')
r = requests.post("http://httpbin.org/post")

```

其他方法如下：

```
requests.put("http://httpbin.org/put")
requests.delete("http://httpbin.org/delete")
requests.head("http://httpbin.org/get")
requests.options("http://httpbin.org/get") 

```

---


**3.传递参数**<br/> url通常会传递某种数据，这种数据采用键值对的参数形式置于url中，比如：

requests通过params关键字设置url参数，以一个字符串字典来提供这些参数。假设作者想传递 key1=value1 和 key2=value2 到httpbin.org/get ，那么你可以使用如下代码：

```
import requests

payload = {'key1':'value1', 'key2':'value2'}
r = requests.get('http://httpbin.org/get', params=payload)
print(r.url)
print(r)

```

输出结果如下图所示：

---


**4.响应内容**<br/> requests会自动解码来自服务器的内容，并且大多数Unicode字符集都能被无缝地解码。当请求发出后，Requests会基于HTTP头部对响应的编码作出有根据的推测。

使用语句如下：

```
import requests

r = requests.get('https://github.com/timeline.json')
print(r.text)

```

输出结果如下图所示：

常用响应内容包括：

post发送json请求：

```
import requests
import json
  
r = requests.post('https://api.github.com/some/endpoint',
                  data=json.dumps({'some': 'data'}))
print(r.json())

```

---


**5.定制请求头**<br/> 如果你想为请求添加http头部，只要简单地传递一个字典（dict）给消息头headers参数即可。例如，我们给github网站指定一个消息头，则语句如下：

```
import requests

data = {'some': 'data'}
headers = {'content-type': 'application/json',
           'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:22.0) Gecko/20100101 Firefox/22.0'}
 
r = requests.post('https://api.github.com/some/endpoint',
                  data=data,
                  headers=headers)
print(r.text)

```

输出结果如下图所示：

---


**6.获取状态码和Cookies**

具体示例如下：

```
import requests

#获取返回状态
r = requests.get('https://github.com/Ranxf')
print(r.status_code)
print(r.headers)
print(r.cookies)

#打印解码后的返回数据
r1 = requests.get(url='http://dict.baidu.com/s',
                  params={'wd': 'python'})
print(r1.url)
print(r1.text)

```

输出结果如下图所示：

同时响应状态码可以结合异常处理，如下：

```
import requests

URL = 'http://ip.taobao.com/service/getIpInfo.php'  # 淘宝IP地址库API
try:
    r = requests.get(URL, params={'ip': '8.8.8.8'}, timeout=1)
    r.raise_for_status()  # 如果响应状态码不是 200，就主动抛出异常
except requests.RequestException as e:
    print(e)
else:
    result = r.json()
    print(type(result), result, sep='\n')

```

---


**7.超时设置**<br/> 设置秒数超时，仅对于链接有效。

```
r = requests.get('url',timeout=1)

```

**8.代理设置**

```
proxies = {'http':'ip1','https':'ip2' }
requests.get('url',proxies=proxies)

```

本小节只是简单介绍了requests模块，推荐读者下来结合案例和官方网站进行更深入的学习和操作。

---


## 二.网页DOM树分析

### 1.网页源码分析

作者的个人网站主要是对博客的介绍，其它网站类似，大家要学会如何进行网站的DOM树结构分析，包括DB。本文主要介绍BeautifulSoup技术爬取作者博客信息。第一部分将介绍分析网页DOM树结构。爬取作者博客的地址为：

上图中显示了作者的博客信息，包括博客名称、博客原文链接、博客摘要等信息，接下来需要对其进行DOM树结构分析。HTML网页是以标签对的形式出现，如&lt; html &gt;&lt; /html &gt;、&lt; div &gt;&lt; /div &gt;等，这种标签对呈树形结构显示，通常称为 **DOM树结构**。

当我们拿到一个网页的时候，第一步并不是去测试它能否能使用requests简单请求到html，而是要去选择合适的方法进行爬取该网页，弄明白它数据的加载方式，才可以让我们的事半功倍，选择一个好的请求方法也可以提升我们爬虫程序的效率。

右键审查元素显示HTML源代码，如下图所示。

可以发现它是在&lt; div class=”essay”&gt;&lt; /div &gt;路径下，它包括一个&lt; h1&gt;&lt; /h1&gt;记录标题，一个&lt; p&gt;&lt; /p&gt;记录摘要信息，每一个&lt; div class=”essay”&gt;&lt; /div &gt;分别对应一篇论文的信息。其中，博客《再见北理工：忆北京研究生的编程时光》HTML中对应内容为：

```
&lt;div class=”essay”&gt;
	&lt;h1&gt;....&lt;/h1&gt;
	&lt;p&gt;.....&lt;/p&gt;
&lt;/div &gt;

```

通过class值为“essay”可以定位博客的信息。调用BeautifulSoup扩展包的find_all(attrs={“class”:“essay”}) 函数可以获取其信息。对应的HTML部分代码如下：

```
&lt;div class="essay"&gt;
	&lt;h1 style="text-align:center"&gt;
	&lt;a href="http://blog.csdn.net/eastmount/article/details/52201984"&gt;再见北理工：忆北京研究生的编程时光&lt;/a&gt;
	&lt;/h1&gt;
	&lt;p style="text-indent: 2em;"&gt;  两年前，我本科毕业写了这样一篇文章：《 回忆自己的大学四年得与失 》，
	感慨了自己在北理软院四年的所得所失；两年后，我离开了帝都，回到了贵州家乡，准备开启一段新的教师生涯，
	在此也写一篇文章纪念下吧！还是那句话：这篇文章是写给自己的，希望很多年之后，回想起自己北京的六年时光，
	也是美好的回忆。文章可能有点长，但希望大家像读小说一样耐心品读，....
	&lt;/p&gt;
&lt;/div&gt;

```

---


### 2.网页结构分析（翻页）

网站翻页是网络爬虫中至关重要的一环，我们进入网站，查看它的网页结构。点击 “下一页” ，查看它的URL链接，会发现下面的规律：

```
第1页URL：http://www.eastmountyxz.com/page=1
第2页URL：http://www.eastmountyxz.com/page=2
...
第14页URL：http://www.eastmountyxz.com/page=14
第15页URL：http://www.eastmountyxz.com/page=15

```

其它方式：

```
第1页URL：http://www.eastmountyxz.com/blog250?start=25&amp;filter= 
第2页URL：http://www.eastmountyxz.com/blog250?start=25&amp;filter= 
第3页URL：http://www.eastmountyxz.com/blog250?start=50&amp;filter= 
...
第10页URL：http://www.eastmountyxz.com/blog250?start=225&amp;filter=

```

它是存在一定规律的，blog250?start=25表示获取第2页（序号为26到50号）的博客信息；blog250?start=50表示获取第3页（序号为51到75号）的博客信息，依次类推。

<mark>**方法一：**</mark><br/> 我们结合数学公式写一个循环获取完整的250篇博客的信息。核心代码如下：

```
i = 0  
while i&lt;10:  
    num = i*25  #每次显示25部 URL序号按25增加  
    url = 'http://www.eastmountyxz.com/blog?start=' + str(num) + '&amp;filter='  
    crawl(url)  #爬虫博客信息
    i = i + 1

```

<mark>**方法二：**</mark><br/> 需要写一个for循环，生成从0到225的数字即可，从上面的链接可以看出来，它的间隔为25，for page in range(0, 226, 25) 必须要取超过停止数据225，因为255不包含在其中，25是它的公差，程序表示为：

```
for page in range(0, 226, 25):
	url = 'http://www.eastmountyxz.com/blog?start=%s&amp;filter=' % page
	print(url)

```

接下来使用python的requests库去代替浏览器请求网页的服务器，返回HTML文件，提取并保存信息，再生成下一页的链接，继续上面请求服务器的操作爬取信息。

---


## 三.Requests请求服务器

在向服务器发出请求时，我们先选择第一个链接来进行测试，完成本页所有内容的获取，然后再获取所有页面的信息。

### 1.导入包

### 2.设置浏览器代理

设置的浏览器代理必须为字典型，如：

```
headers = {
    'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) \
     AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36'
}

```

### 3.请求服务器格式

请求源代码向服务器发出请求，200代表成功。如果在后面加上 `.text` 表示输出文本内容。

```
requests.get(url = url, headers = headers)

```

### 4.请求服务器代码汇总

这里以第一页内容为例，核心代码如下所示：

```
# -*- coding: utf-8 -*-
import requests

#设置浏览器代理,它是一个字典
headers = {
    'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) \
        AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36'
}
url = 'http://www.eastmountyxz.com/'

#向服务器发出请求
r = requests.get(url = url, headers = headers)
r.encoding = 'utf-8'
print(r.text)

```

输出结果如下图所示：

---


## 四.xpath提取信息

### 1.获取xpath节点方法

xpath是按照HTML标签的方式进行定位的，谷歌浏览器自带有xpath，可以直接复制过来使用，简单方便，运行速度快。

输出结果为：

我们使用xpath时，也必须先对网页进行 `lxml` 库中的 `etree` 解析，把它变为特有的树状形式，才能通过它进行节点定位。

```
from lxml import etree            #导入解析库
html_etree = etree.HTML(reponse)  #树状结构解析

```

---


### 2.xpath提取文本

当我们提取标签内的文本时，需要在复制到的xpath后面加上 `/text()` ，告诉它我们需要提取的内容是一个标签呈现的数据，如《再见北理工：忆北京研究生的编程时光》。

```
&lt;a href="xxxx"&gt;再见北理工：忆北京研究生的编程时光&lt;/a&gt;

```

结合xpath所提取的文字代码为：

```
# coding:utf-8
# By:Eastmount 2021-02-25
import requests
from lxml import etree

#设置浏览器代理,它是一个字典
headers = {
    'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) \
        AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36'
}
url = 'http://www.eastmountyxz.com/'

#向服务器发出请求
r = requests.get(url = url, headers = headers)
r.encoding = 'utf-8'
content = r.text

#解析DOM树结构
html_etree = etree.HTML(content)
name = html_etree.xpath('//*[@id="zw"]/div[2]/h1/a/text()')
print ("这是数组形式：",name)
print ("这是字符串形式：",name[0])

```

输出结果如下所示：

```
这是数组形式： ['再见北理工：忆北京研究生的编程时光']
这是字符串形式： 再见北理工：忆北京研究生的编程时光

```

---


### 3.xpath提取链接

每一个链接都是在标签内的，通常放在 `src=" "` 或者 `href=" "` 之中，如

xpath为：

```
//*[@id="zw"]/div[2]/h1/a

```

提取链接时，需要在复制到的xpath后面加上 `/@href` , 指定提取链接。

```
movie_url = html_etree.xpath('//*[@id="zw"]/div[2]/h1/a/@href')
print ("这是数组形式：",movie_url)
print ("这是字符串形式：",movie_url[0])

```

输出结果如下所示：

```
这是数组形式： ['http://blog.csdn.net/eastmount/article/details/52201984']
这是字符串形式： http://blog.csdn.net/eastmount/article/details/52201984

```

---


### 4.xpath提取标签元素

这个网页中博客的&lt; div&gt;的标签表示，如：

所以只需要取出 `class=" "` 中的内容就可以得到，复制它的xpath，和提取链接的方法一样，在后面加上 `/@class` 即可。

```
rating = html_etree.xpath('//*[@id="zw"]/div[2]/@class')
print ("这是数组形式：",rating)
print ("这是字符串形式：",rating[0])

```

输出结果如下所示：

---


## 五.正则表达式匹配信息

前面第四篇文章我们详细介绍了正则表达式的内容，它常常会与网络爬虫和数据预处理结合起来，简化我们的工作。这里需要把结果中的信息匹配出来，可以使用正在表达式，单独提取自己需要的信息，如DB评分的星级，它都是以 `rating5-t` 方式呈现的，但是我们只需要它数字5位置的部分，所以需要进行二次提取。

### 1.提取固定位置信息

正则表达式中可以使用 `.*?` 来进行匹配信息，没有加括号时可以去掉不一样的信息，不需要提取出来，加括号 `(.*?)` 可以提取出括号内的内容，如：

```
import re
test = "rating5-t"
text = re.findall('rating(.*?)-t', test)
print (text)

```

输出结果为：

```
['5']

```

这里再举一个简单的例子：

### 2.匹配数字

比如评价数，我们xpath提取到的数据格式为： 1056830人评价 ，保存的时候只需要数字即可，现在把数字提取出来：

```
import re 
data = "1059232人评价"
num = re.sub(r'\D', "", data)
print("这里的数字是:", num)

```

输出结果为：

```
这里的数字是: 1059232

```

---


## 六.CSV文件操作

我们在使用Python进行网络爬虫或数据分析时，通常会遇到CSV文件，类似于Excel表格。第三篇文章我们详细介绍了CSV文件的操作，保存内容与把大象放进冰箱是一样的，分别为打开冰箱，把大象装进去，关闭冰箱。这里我们进行简单说明。

### 1.CSV文件写

基本流程如下：

```
# -*- coding: utf-8 -*-
import csv

c = open("test-01.csv", "w", encoding="utf8", newline='')  #写文件
writer = csv.writer(c)
writer.writerow(['序号','姓名','年龄'])
 
tlist = []
tlist.append("1")
tlist.append("小明")
tlist.append("10")
writer.writerow(tlist)
print(tlist,type(tlist))
 
del tlist[:]  #清空
tlist.append("2")
tlist.append("小红")
tlist.append("9")
writer.writerow(tlist)
print(tlist,type(tlist))
 
c.close()

```

输出结果如下图所示：

---


### 2.CSV文件读

基本流程如下：

```
# -*- coding: utf-8 -*-
import csv
c = open("test-01.csv", "r", encoding="utf8")  #读文件
reader = csv.reader(c)
for line in reader:
    print(line[0],line[1],line[2])
c.close()

```

输出结果如下图所示：

在文件操作中编码问题是最让人头疼的，尤其Python2的时候。但只需要环境编码一致，注意相关转换也能有效解决，而Python3文件读写操作写清楚encoding编码方式就能正常显示。

---


## 七.完整代码

### 1.提取本页所有信息

通过前面的 xpath 只能提取到一条信息，如果我们要提取所有的信息，写一个 for 循环把它遍历出来即可。先复制几个名字的 xpath，如前三个的：

`li` 标签前的作为父级，后面的为子集，`./` 代替父级的位置，改写为：

```
li = html_etree.xpath('//*[@id="content"]/div/div[1]/ol/li')
for item in li:
    name = item.xpath('./div/div[2]/div[1]/a/span[1]/text()')[0]
    print (name)

```

此时的代码如下所示：

```
# coding:utf-8
# By:Eastmount &amp; ayouleyang 2021-02-25
import requests
from lxml import etree

#设置浏览器代理,它是一个字典
headers = {
    'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) \
        AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36'
}
url = 'https://xxxxxx/top250?start=0&amp;filter='

#向服务器发出请求
r = requests.get(url = url, headers = headers).text

#解析DOM树结构
html_etree = etree.HTML(r)
name = html_etree.xpath('//*[@id="content"]/div/div[1]/ol/li[1]/div/div[2]/div[1]/a/span[1]/text()')
print ("这是数组形式：",name)
print ("这是字符串形式：",name[0])

#提取链接
movie_url = html_etree.xpath('//*[@id="content"]/div/div[1]/ol/li[1]/div/div[2]/div[1]/a/@href')
print ("这是数组形式：",movie_url)
print ("这是字符串形式：",movie_url[0])

#提取打分
rating = html_etree.xpath('//*[@id="content"]/div/div[1]/ol/li[1]/div/div[2]/div[2]/div/span[1]/@class')
print ("这是数组形式：",rating)
print ("这是字符串形式：",rating[0])

#提取本页所有信息
li = html_etree.xpath('//*[@id="content"]/div/div[1]/ol/li')
for item in li:
    name = item.xpath('./div/div[2]/div[1]/a/span[1]/text()')[0]
    print (name)

```

---


### 2.最终代码

最终代码如下所示：

```
# coding:utf-8
# By:Eastmount &amp; ayouleyang 2021-02-25
import requests
from lxml import etree
import csv, re

#设置浏览器代理,它是一个字典
headers = {
    'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) \
        AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36'
}

#创建文件夹并打开
fp = open("./DB-top250.csv", 'a', newline='', encoding = 'utf-8-sig')
writer = csv.writer(fp) #写入
writer.writerow(('排名', '名称', '链接', '星级', '评分', '评价人数'))

#循环遍历TOP250的URL
for page in range(0, 226, 25):  #226
    print ("正在获取第%s页"%page)
    url = 'https://xxxxxxx/top250?start=%s&amp;filter='%page
    
    #请求源代码
    reponse = requests.get(url = url, headers = headers).text
    
    #解析DOM树结构
    html_etree = etree.HTML(reponse)
    
    #定位节点 注意迭代xpath应用
    li = html_etree.xpath('//*[@id="content"]/div/div[1]/ol/li')
    for item in li:
        #排名
        rank = item.xpath('./div/div[1]/em/text()')[0]
        #名称
        name = item.xpath('./div/div[2]/div[1]/a/span[1]/text()')[0]
        #链接
        dy_url = item.xpath('./div/div[2]/div[1]/a/@href')[0]
        
        #评分 正则表达式提取
        rating = item.xpath('./div/div[2]/div[2]/div/span[1]/@class')[0]
        rating = re.findall('rating(.*?)-t', rating)[0]
        if len(rating) == 2:
            star = int(rating) / 10  #int()转化为数字
        else:
            star = rating
            
        #评价人数
        rating_num = item.xpath('./div/div[2]/div[2]/div/span[2]/text()')[0]
        content = item.xpath('./div/div[2]/div[2]/div/span[4]/text()')[0]
        content = re.sub(r'\D', "", content)
        #print (rank, name, dy_url, star, rating_num, content)

        #写入内容
        writer.writerow((rank, name, dy_url, star, rating_num, content))
fp.close()

```

最终保存的文件。

---


## 八.总结

在学习网络爬虫之前，读者首先要掌握分析网页节点、审查元素定位标签，甚至是翻页跳转、URL分析等知识，然后才是通过Python、Java或C#实现爬虫的代码。本文作者结合自己多年的网络爬虫开发经验，深入讲解了Requests技术网页分析并爬取了个人博客信息，读者可以借用本章的分析方法，结合Requests库爬取所需的网页信息，并学会分析网页跳转，尽可能爬取完整的数据集。

该系列所有代码下载地址：

2020年在github的绿瓷砖终于贴完了第一年提交2100余次，获得1500多+stars，开源93个仓库，300个粉丝。挺开心的，希望自己能坚持在github打卡五年，督促自己不断前行。简单总结下，最满意的资源是YQ爆发时，去年2月分享的舆情分析和情感分析，用这系列有温度的代码为武汉加油；最高赞的是Python图像识别系列，也获得了第一位来自国外开发者的贡献补充；最花时间的是Wannacry逆向系列，花了我两月逆向分析，几乎成为了全网最详细的该蠕虫分析；还有AI系列、知识图谱实战、CVE复现、APT报告等等。当然也存在很多不足之处，希望来年分享更高质量的资源，也希望能将安全和AI顶会论文系列总结进来，真诚的希望它们能帮助到大家，感恩有你，一起加油~

(By:娜璋之家 Eastmount 2021-02-25 夜于贵阳 [https://blog.csdn.net/Eastmount](https://blog.csdn.net/Eastmount) )

---


**参考文献如下：**

---

