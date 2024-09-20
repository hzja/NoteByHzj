Python爬虫
<a name="zBWay"></a>
## **Requests-Cache的安装与测试**
Requests-Cache模块是requests模块的一个扩展功能，用于为requests模块提供持久化缓存支持。如果requests模块向一个URL发送重复请求时，Requests-Cache模块将会自动判断当前的网络请求是否产生了缓存。如果已经产生了缓存，就会从缓存中读取数据作为响应内容。如果没有缓存就会向服务器发送网络请求，获取服务器所返回的响应内容。使用Requests-Cache模块可以减少网络资源避免重复请求的次数，这样可以**变相**地躲避一些反爬机制。
<a name="SdnTF"></a>
### 说 明
无论是否使用了Anaconda，都需要单独安装Requests-Cache模块，因为Anaconda中并不包含该模块。
```python
Python 3.9.9 (v3.9.9:ccb0e6a345, Nov 15 2021, 13:29:20) 
[Clang 6.0 (clang-600.0.57)] on darwin
>>>import requests_cache
>>>version = requests_cache.__version__
>>>print(version)
0.9.1
```
<a name="GmJC3"></a>
### 缓存的应用
调用`install_cache()`函数实现Requests-Cache的请求，语法格式如下：
```python
install_cache(cache_name='cache', backend=None, expire_after=None, allowable_codes=(200, ), allowable_methods=('GET', ), session_factory=<class 'requests_cache.core.CachedSession'>,**backend_options)
```
`Install_cache()`函数中包含了多个参数，每个参数的含义如下：<br />§ cache_name：表示缓存文件的名称，默认为cache<br />§ backend：表示设置缓存的存储机制，默认为None，表示默认使用sqlite进行存储<br />§ expire_after：表示设置缓存的有效时间，默认为None，表示永久有效<br />§ allowable_codes：表示设置状态码，默认为200<br />§ allowable_methods：表示设置请求方式，默认为GET，表示只有GET请求才可以生产缓存<br />§ session_factory：表示设置缓存执行的对象，需要实现`CachedSession`类<br />§ backend_options：如果缓存的存储方式为sqlite、mongo、redis数据库，该参数表示设置数据库的连接方式<br />在使用`install_cache()`函数实现请求缓存时，一般情况下时不需要单独设置任何参数的，只需要使用默认参数即可。判断是否存在缓存的代码如下：
```python
#_*_coding:utf-8_*_

# 导入requests_cache模块
import requests_cache

# 导入网络请求模块
import requests

# 设置缓存
requests_cache.install_cache()

# 清理缓存
requests_cache.clear()

# 定义测试地址
url = 'http://httpbin.org/get'

# 第一次发送网络请求
r = requests.get(url)

# False表示不存在缓存
print('是否存在缓存：', r.from_cache)

# 第二次发送网络请求
r = requests.get(url)

# True表示存在缓存
print('是否存在缓存：', r.from_cache)
```
程序运行结果如下：
```python
是否存在缓存： False
是否存在缓存： True
```
针对反爬措施，在多次请求中设置延时是不错的选择。但是如果在第一次请求后生成了缓存，那么第二次请求时就无需设置延时，为此Requests-Cache模块可以使用自定义钩子函数的方式，合理判断是否需要设置延时操作。示例代码如下：
```python
#_*_coding:utf-8_*_

# 导入Requests_Cache模块
import requests_cache

# 导入时间模块
import time

# 设置缓存
requests_cache.install_cache()

# 清理缓存
requests_cache.clear()

# 定义钩子函数
def make_throttle_hook(timeout = 0.1):
    def hook(response, *args, **kwargs):
        print(response.text)
        # 判断没有缓存时就添加延时
        if not getattr(response, 'from_cache', False):
            print('等待', timeout, '秒！')
            # 等待指定时间
            time.sleep(timeout)
        else:
            # 存在缓存输出True
            print('是否存在请求缓存！', response.from_cache)
        return response
    return hook

if __name__ == '__main__':
    requests_cache.install_cache()
    requests_cache.clear()
    s = requests_cache.CachedSession()
    s.hooks = {'response': make_throttle_hook(2)}   # 配置钩子函数

    # 模拟发送第一次网络请求
    s.get('http://httpbin.org/get')
    # 模拟发送第二次网络请求
    s.get('http://httpbin.org/get')
```
程序运行结果：<br />_第一次运行结果：_
```python
{
  "args": {}, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Host": "httpbin.org", 
    "User-Agent": "python-requests/2.27.1", 
    "X-Amzn-Trace-Id": "Root=1-61f5e91a-5c923abe41da544561b5f400"
  }, 
  "origin": "139.209.219.102", 
  "url": "http://httpbin.org/get"
}
```
_等待2秒_<br />_第二次请求结果_
```python
{
  "args": {}, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Host": "httpbin.org", 
    "User-Agent": "python-requests/2.27.1", 
    "X-Amzn-Trace-Id": "Root=1-61f5e91a-5c923abe41da544561b5f400"
  }, 
  "origin": "139.209.219.102", 
  "url": "http://httpbin.org/get"
}
```
_二次请求存在缓存_
```python
是否存在请求缓存！ True
```
从以上的运行结果看，通过配置钩子函数可以实现：在第一次请求时，因为没有请求缓存所以执行了2秒等待延时；第二次请求时则没有执行2秒 延时，并输出是否存在请求缓存行为True。<br />**说 明**<br />Requests-Cache模块支持4种不同的存储机制，分别为money、sqlite、mongoDB以及redid，具体说明如下：<br />§ memory：以字典的形式将缓存存储在内存当中，程序运行完后缓存将被销毁<br />§ sqlite：将缓存存储在sqlite数据库中<br />§ mongoDB：将缓存存储在mongoDB数据库中<br />§ redis：将缓存存储在redis数据库当中<br />使用Requests-Cache模块指定缓存不同的存储机制时，只需要为install_cache()函数中backend参数赋值即可，设置方式如下：
```python
# 导入Requests_Cache模块
import requests_cache

# 设置缓存为内存的存储机制
requests_cache.install_cache(backend='memory')

# 设置缓存为sqlite数据库的存储机制
requests_cache.install_cache(backend='sqlite')

# 设置缓存为mongoDB数据库的存储机制
requests_cache.install_cache(backend='mongoDB')

# 设置缓存为redis数据库的存储机制
requests_cache.install_cache(backend='redis')
```
设置存储机制为mongoDB与redis数据库时，需要提取安装对应的操作模块与数据库。安装模块的命令如下：
```bash
pip install pymongo
pip install redis
```
<a name="Vn8nk"></a>
## **强大的Requests-HTML模块**
Requests-HTML 模块是requets模块的亲兄弟，是同一个开发者所开发的。Requests-HTML模块不仅包含了requests模块中的所有功能，还增加了对JavaScript的支持、数据提取以及模拟真实浏览器等功能。
<a name="nLhLl"></a>
### 使用Requests-HTML模块实现网络请求
<a name="Wi67Y"></a>
#### § 1、`get()`请求
通过'`pip install requests-html`'命令进行模块的安装，然后导入Requests-HTML模块中的HTMLSession类，接着需要创建HTML会话对象，通过会话实例进行网络请求的发送，示例代码如下：
```python
#_*_coding:utf-8_*_

# 导入HTMLSession类
from requests_html import HTMLSession

# 创建HTML会话对象
session = HTMLSession()

# 定义请求地址
url = 'http://news.youth.cn/'

# 发送网络请求
r = session.get(url)

# 打印网络请求的URL地址
print(r.html)
```
程序运行结果如下：
```python
<HTML url='http://news.youth.cn/'>
```
<a name="UVeZv"></a>
#### § 2、`post()`请求
在实现网络请求时，POST请求也是一种比较常见的请求方式，使用Requests-HTML实现POST请求与requests的实现方法类似都需要单独设置表单参数data，不过它也是需要通过会话实例进行网络请求的发送，示例代码如下：
```python
#_*_coding:utf-8_*_

# 导入HTMLSession类
from requests_html import HTMLSession

# 创建HTML会话对象
session = HTMLSession()

# 模拟表单登录的数据
data = {'user':'admin', 'password':123456}

# 发送POST请求
r = session.post('http://httpbin.org/post', data = data)
# 判断请求是否成功
if r.status_code == 200:
    print(r.text)
```
程序运行结果如下：
```python
{
  "args": {}, 
  "data": "", 
  "files": {}, 
  # 表单数据
  "form": {
    "password": "123456", 
    "user": "admin"
  }, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Content-Length": "26", 
    "Content-Type": "application/x-www-form-urlencoded", 
    "Host": "httpbin.org", 
    # User-Agent对应的值
    "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/603.3.8 (KHTML, like Gecko) Version/10.1.2 Safari/603.3.8", 
    "X-Amzn-Trace-Id": "Root=1-61f6509d-56a381b4543229480fe07e56"
  }, 
  "json": null, 
  "origin": "139.209.219.102", 
  "url": "http://httpbin.org/post"
}
```
从以上的运行结果中，不仅可以看到form所对应的表单内容，还可以看到User-Agent所对应的值，并不是像requests模块发送网络请求时返回的默认值(python-requests/2.27.1)，而是一个真实的浏览器请求头信息，这与requests模块所发送的网络请求有着细小的改进。
<a name="dpCfS"></a>
#### § 3、修改请求头信息
说到请求头信息，Requests-HTML模块是可通过指定headers参数来对默认的浏览器请求头信息进行修改的，修改请求头信息的关键代码如下：
```python
ua = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36 Edg/97.0.1072.76'
}
r = session.post('http://httpbin.org/post', data = data, headers = ua)
```
§ Requests-HTML模块中添加了UserAgent类，使用该类可以实现随机生成请求头。示例代码如下：
```python
#_*_coding:utf-8_*_

# 导入HTMLSession类
from requests_html import HTMLSession, UserAgent

# 创建HTML会话对象
session = HTMLSession()

# 创建随机请求头
ua =UserAgent().random
r = session.get('http://httpbin.org/get', headers = {'user-agent': ua})

# 判断请求是否成功
if r.status_code == 200:
    # 以文本形式打印返回结果
    print(r.text)
```
程序运行结果如下：
```python
{
  "args": {}, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Host": "httpbin.org", 
    # 请求头信息
    "User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:17.0) Gecko/20100101 Firefox/17.0.6", 
    "X-Amzn-Trace-Id": "Root=1-61f69454-0b37eb40198f0f4b237fc7e8"
  }, 
  "origin": "139.209.219.102", 
  "url": "http://httpbin.org/get"
}

{
  "args": {}, 
  "headers": {
    "Accept": "*/*", 
    "Accept-Encoding": "gzip, deflate", 
    "Host": "httpbin.org", 
    # 请求头信息
    "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_2) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1309.0 Safari/537.17", 
    "X-Amzn-Trace-Id": "Root=1-61f693aa-0d20428a3a73766723017536"
  }, 
  "origin": "139.209.219.102", 
  "url": "http://httpbin.org/get"
}
```
_备注_<br />以上运行是在Anaconda的Jupiter notebook 下运行。Pycharm 报错，应该是解释器的问题。
<a name="SlMCy"></a>
## **数据的提取**
以往使用requests模块实现爬虫程序时，还需要为其配置一个解析HTML代码的搭档。Requests-HTML模块对此进行了一个比较大的升级，不仅支持CSS选择器还支持XPath的节点提取方式。
<a name="fyJoY"></a>
### 1、CSS选择器
CSS选择器中需要使用HTML的`find()`方法，该方法中包含5个参数，其语法格式与参数含义如下：
```python
find(selector:str='*', containing:_Containing=None, clean:bool=False,first:bool=False,_encoding:str=None)
```
§ `selector`：使用CSS选择器定位网页元素<br />§ `containing`：通过指定文本获取网页元素<br />§ `clean`：是否清除HTML中的
