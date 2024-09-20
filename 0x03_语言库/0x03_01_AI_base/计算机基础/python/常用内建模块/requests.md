Python 内置了 requests 模块，该模块主要用来发 送 HTTP 请求，requests 模块与 urllib 模块相比更简洁。
```python
# 导入 requests 包
import requests

# 发送请求
x = requests.get('https://www.runoob.com/')

# 返回网页内容
print(x.text)
```
<a name="TrrXe"></a>
# 响应信息
每次调用 requests 请求之后，会返回一个 response 对象，该对象包含了具体的响应信息。响应信息如下：

| 属性或方法 | 说明 |
| --- | --- |
| apparent_encoding | 编码方式 |
| close() | 关闭与服务器的连接 |
| content | 返回响应的内容，以字节为单位 |
| cookies | 返回一个 CookieJar 对象，包含了从服务器发回的 cookie |
| elapsed | 返回一个 timedelta 对象，包含了从发送请求到响应到达之间经过的时间量，可以用于测试响应速度。比如 r.elapsed.microseconds 表示响应到达需要多少微秒。 |
| encoding | 解码 r.text 的编码方式 |
| headers | 返回响应头，字典格式 |
| history | 返回包含请求历史的响应对象列表（url） |
| is_permanent_redirect | 如果响应是永久重定向的 url，则返回 True，否则返回 False |
| is_redirect | 如果响应被重定向，则返回 True，否则返回 False |
| iter_content() | 迭代响应 |
| iter_lines() | 迭代响应的行 |
| json() | 返回结果的 JSON 对象 (结果需要以 JSON 格式编写的，否则会引发错误) |
| links | 返回响应的解析头链接 |
| next | 返回重定向链中下一个请求的 PreparedRequest 对象 |
| ok | 检查 "status_code" 的值，如果小于400，则返回 True，如果不小于 400，则返回 False |
| raise_for_status() | 如果发生错误，方法返回一个 HTTPError 对象 |
| reason | 响应状态的描述，比如 "Not Found" 或 "OK" |
| request | 返回请求此响应的请求对象 |
| status_code | 返回 http 的状态码，比如 404 和 200（200 是 OK，404 是 Not Found） |
| text | 返回响应的内容，unicode 类型数据 |
| url | 返回响应的 URL |

<a name="X9hHd"></a>
## http状态码
当浏览者访问一个网页时，浏览者的浏览器会向网页所在服务器发出请求。当浏览器接收并显示网页前，此网页所在的服务器会返回一个包含 HTTP 状态码的信息头（server header）用以响应浏览器的请求。HTTP 状态码的英文为 HTTP Status Code。下面是常见的 HTTP 状态码：

| 200 |  请求成功 |
| --- | --- |
| 301 |  资源（网页等）被永久转移到其它URL |
| 404 |  请求的资源（网页等）不存在 |
| 500 |  内部服务器错误 |

HTTP 状态码由三个十进制数字组成，第一个十进制数字定义了状态码的类型。响应分为五类：信息响应(100–199)，成功响应(200–299)，重定向(300–399)，客户端错误(400–499)和服务器错误 (500–599)，更详细每个代码的含义见[HTTP状态码](https://www.runoob.com/http/http-status-codes.html)：

| 分类 | 分类描述 |
| --- | --- |
| 1** | 信息，服务器收到请求，需要请求者继续执行操作 |
| 2** | 成功，操作被成功接收并处理 |
| 3** | 重定向，需要进一步的操作以完成请求 |
| 4** | 客户端错误，请求包含语法错误或无法完成请求 |
| 5** | 服务器错误，服务器在处理请求的过程中发生了错误 |

<a name="uOpqX"></a>
# requests 方法
| 方法 | 描述 |
| --- | --- |
| delete(url, args) | 发送 DELETE 请求到指定 url |
| get(url, params, args) | 发送 GET 请求到指定 url |
| head(url, args) | 发送 HEAD 请求到指定 url |
| patch(url, data, args) | 发送 PATCH 请求到指定 url |
| post(url, data, json, args) | 发送 POST 请求到指定 url |
| put(url, data, args) | 发送 PUT 请求到指定 url |
| request(method, url, args) | 向指定的 url 发送指定的请求方法 |

```python
# 导入 requests 包
import requests

 
kw = {'s':'python 教程'}

# 设置请求头
headers = {"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36"}
 
# params 接收一个字典或者字符串的查询参数，字典类型自动转换为url编码，不需要urlencode()
response = requests.get("https://www.runoob.com/", params = kw, headers = headers)

# 查看响应状态码
print (response.status_code)

# 查看响应头部字符编码
print (response.encoding)

# 查看完整url地址
print (response.url)

# 查看响应内容，response.text 返回的是Unicode格式的数据
print(response.text)
```
<a name="YBv0g"></a>
# Source
[Python requests 模块 | 菜鸟教程](https://www.runoob.com/python3/python-requests.html)<br />[HTTP 状态码 | 菜鸟教程](https://www.runoob.com/http/http-status-codes.html)
