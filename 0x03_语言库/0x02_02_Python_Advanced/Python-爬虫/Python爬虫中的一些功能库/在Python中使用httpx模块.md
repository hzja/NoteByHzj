Python httpx
<a name="eS9IY"></a>
### 导入`httpx`
```python
In [25]: import httpx
```
获取一个网页
```python
In [26]: r = httpx.get("https://httpbin.org/get")
In [27]: r
Out[27]: <Response [200 OK]>
```
同样,发送`HTTP POST`请求:
```python
In [28]: r = httpx.post("https://httpbin.org/post", data={"key": "value"})
In [29]: r
Out[29]: <Response [200 OK]>
```
PUT,DELETE,HEAD和OPTIONS请求都遵循相同的方式:
```python
In [35]: r = httpx.put("https://httpbin.org/put", data={"key": "value"})
In [36]: r = httpx.delete("https://httpbin.org/delete")
In [37]: r = httpx.head("https://httpbin.org/get")
In [38]: r = httpx.options("https://httpbin.org/get")
```
<a name="Vuqdm"></a>
### 在`URL`中传递参数

1. 在请求URL中传递查询参数,请使用`params`关键字
```python
In [41]: r = httpx.get("https://httpbin.org/get", params=params)
In [42]: r
Out[42]: <Response [200 OK]>
# 检查发出请求结果的`URL`
In [43]: r.url
Out[43]: URL('https://httpbin.org/get?key1=value2&key2=value2')
```

2. 将项目列表作为值传递
```python
In [44]: params = {"key1": "value2", "key2": ["value2", "value3"]}
In [45]: r = httpx.get("https://httpbin.org/get", params=params)
In [46]: r.url
Out[46]: URL('https://httpbin.org/get?key1=value2&key2=value2&key2=value3')
```
<a name="Eph63"></a>
### 响应内容
HTTPX将自动处理响应内容解码为`Unicode`文本
```python
In [52]: r = httpx.get("https://www.example.org/")
In [53]: r.text
Out[53]: '<!doctype html>\n<html>\n<head>\n    <title>Example Domain</title>\n\n    <meta charset="utf-8" />\n    <meta http-equiv="Content-type" content="text/html; charset=utf-8" />\n    <meta name="viewport" content="width=device-width, initial-scale=1" />\n    <style type="text/css">\n    body {\n        background-color: #f0f0f2;\n        margin: 0;\n        padding: 0;\n        font-family: -apple-system, system-ui, BlinkMacSystemFont, "Segoe UI", "Open Sans", "Helvetica Neue", Helvetica, Arial, sans-serif;\n        \n    }\n    div {\n        width: 600px;\n        margin: 5em auto;\n        padding: 2em;\n        background-color: #fdfdff;\n        border-radius: 0.5em;\n        box-shadow: 2px 3px 7px 2px rgba(0,0,0,0.02);\n    }\n    a:link, a:visited {\n        color: #38488f;\n        text-decoration: none;\n    }\n    @media (max-width: 700px) {\n        div {\n            margin: 0 auto;\n            width: auto;\n        }\n    }\n    </style>    \n</head>\n\n<body>\n<div>\n    <h1>Example Domain</h1>\n    <p>This domain is for use in illustrative examples in documents. You may use this\n    domain in literature without prior coordination or asking for permission.</p>\n    <p><a href="https://www.iana.org/domains/example">More information...</a></p>\n</div>\n</body>\n</html>\n'
# 解码
In [54]: r.encoding
Out[54]: 'UTF-8'
# 设置需要使用的编码
In [55]: r.encoding = "ISO-8859-1"
In [56]: print(r.headers, r.http_version, r.url, r.status_code)
Headers({'content-encoding': 'gzip', 'age': '532553', 'cache-control': 'max-age=604800', 'content-type': 'text/html; charset=UTF-8', 'date': 'Sat, 06 Jun 2020 04:22:26 GMT', 'etag': '"3147526947+gzip"', 'expires': 'Sat, 13 Jun 2020 04:22:26 GMT', 'last-modified': 'Thu, 17 Oct 2019 07:18:26 GMT', 'server': 'ECS (oxr/830C)', 'vary': 'Accept-Encoding', 'x-cache': 'HIT', 'content-length': '648'}) HTTP/1.1 https://www.example.org/ 200
```
<a name="sdTOv"></a>
### 返回JSON响应内容
通常, Web API 响应将被编码为JSON
```python
In [59]: r = httpx.get("https://api.github.com/events")
In [60]: r.json()
```
<a name="En3bn"></a>
### 自定义`header`
要在传出请求中包含其他标头,请使用`header`关键字参数:
```python
In [60]: url = 'http://httpbin.org/headers'
In [61]: headers = {"user-agent": "my-app/0.0.1"}
In [62]: r = httpx.get(url, headers=headers)
```
<a name="EJ38B"></a>
### 发送表单数据
某些类型的HTTP请求,如`POST|PUT`
```python
In [64]: data = {"key1":"value1", "key1": "value2"}
In [65]: r = httpx.post("https://httpbin.org/post", data=data)
In [66]: print(r.text)
{
  "args": {},
  "data": "",
  "files": {},
  "form": {
    "key1": "value2"
  },
  "headers": {
    "Accept": "*/*",
    "Accept-Encoding": "gzip, deflate",
    "Content-Length": "11",
    "Content-Type": "application/x-www-form-urlencoded",
    "Host": "httpbin.org",
    "User-Agent": "python-httpx/0.12.1",
    "X-Amzn-Trace-Id": "Root=1-5edb1e61-86ac64e4f9f165244f4dbd68"
  },
  "json": null,
  "origin": "114.113.113.186",
  "url": "https://httpbin.org/post"
}
```
表单编码的数据还可以包括给定键的多个值:
```python
In [72]: data = {"userList": ["shuke", "jack"]}
In [73]: r = httpx.post("https://httpbin.org/post", data=data)
In [74]: print(r.text)
{
  "args": {},
  "data": "",
  "files": {},
  "form": {
    "userList": [
      "shuke",
      "jack"
    ]
  },
  "headers": {
    "Accept": "*/*",
    "Accept-Encoding": "gzip, deflate",
    "Content-Length": "28",
    "Content-Type": "application/x-www-form-urlencoded",
    "Host": "httpbin.org",
    "User-Agent": "python-httpx/0.12.1",
    "X-Amzn-Trace-Id": "Root=1-5edb1fc2-33761faf662f0b832cb806bb"
  },
  "json": null,
  "origin": "114.113.113.186",
  "url": "https://httpbin.org/post"
}
```
发送分段文件上传<br />使用HTTP分段编码上传文件
```python
In [76]: files = {"upload-file": open("/Users/shuke/Work/pha/docker-compose.yml", "rb")}
In [77]: r = httpx.post("https://httpbin.org/post", files=files)
In [78]: print(r.text)
```
发送JSON编码数据
```python
In [79]: data = {"integer": 123, "boolean": True, "list": ["a", "b", "c"]}
In [80]: r = httpx.post("https://httpbin.org/post", json=data)
In [81]: print(r.text)
{
  "args": {},
  "data": "{\"integer\": 123, \"boolean\": true, \"list\": [\"a\", \"b\", \"c\"]}",
  "files": {},
  "form": {},
  "headers": {
    "Accept": "*/*",
    "Accept-Encoding": "gzip, deflate",
    "Content-Length": "58",
    "Content-Type": "application/json",
    "Host": "httpbin.org",
    "User-Agent": "python-httpx/0.12.1",
    "X-Amzn-Trace-Id": "Root=1-5edb217c-cd09ef8caa55514051957eb8"
  },
  "json": {
    "boolean": true,
    "integer": 123,
    "list": [
      "a",
      "b",
      "c"
    ]
  },
  "origin": "114.113.113.186",
  "url": "https://httpbin.org/post"
}
```
<a name="fP8h0"></a>
### 响应状态码
检查响应的`HTTP`状态码
```python
In [82]: r = httpx.get("https://httpbin.org/get")
In [83]: r.status_code
Out[83]: 200
```
HTTPX还包括一个简单的快捷方式,用于通过其文本短语访问状态代码
```python
In [84]: r.status_code == httpx.codes.OK
Out[84]: True
```
针对任何客户端或服务器错误响应(4xx或5xx状态代码) 引发异常:
```python
In [85]: not_found = httpx.get("https://httpbin.org/status/404")
In [86]: not_found.status_code
Out[86]: 404
In [87]: not_found.rasise_for_status()
---------------------------------------------------------------------------
AttributeError                            Traceback (most recent call last)
<ipython-input-87-c7b57e835640> in <module>
----> 1 not_found.rasise_for_status()
AttributeError: 'Response' object has no attribute 'rasise_for_status'
```
<a name="HWYjf"></a>
### 响应Headers
响应标头可作为类似于字典的接口使用
```python
In [88]: r.headers
Out[88]: Headers({'date': 'Sat, 06 Jun 2020 04:56:56 GMT', 'content-type': 'application/json', 'content-length': '306', 'connection': 'keep-alive', 'server': 'gunicorn/19.9.0', 'access-control-allow-origin': '*', 'access-control-allow-credentials': 'true'})
```
该`Headers`数据类型是不区分大小写的，所以可以使用任何形式
```python
In [89]: r.headers["Content-Type"]
Out[89]: 'application/json'
In [90]: r.headers.get("content-type")
Out[90]: 'application/json'
```
<a name="0K3pw"></a>
### 流响应
> 可以流式传输响应的二进制内容

```python
In [91]: with httpx.stream("GET", "https://www.example.com") as r:
    ...:     for data in r.iter_bytes():
    ...:         print(data)
```
或文字...
```python
In [93]: with httpx.stream("GET", "https://www.example.com") as r:
    ...:     for text in r.iter_text():
    ...:         print(text)
```
或逐行流文本...
```python
In [96]: with httpx.stream("GET", "https://www.example.com") as r:
    ...:     for text in r.iter_lines():
    ...:         print(text)
```
<a name="UlDbJ"></a>
### Cookies
可以轻松访问响应中设置的任何cookie:
```python
In [97]: r = httpx.get("http://httpbin.org/cookies/set?chocolate=chip", allow_redirects=False)
In [98]: r.cookies["chocolate"]
Out[98]: 'chip'
```
如果需要将`Cookies`包含在外发请求中,请使用`cookies`参数:
```python
In [99]: cookies = {"peanut": "butter"}
In [100]: r = httpx.get("http://httpbin.org/cookies", cookies=cookies)
In [101]: r.json()
Out[101]: {'cookies': {'peanut': 'butter'}}
```
`Cookies` 按 域访问设置
```python
In [102]: cookies = httpx.Cookies()
In [103]: cookies.set('cookie_on_domain', 'hello, there!', domain='httpbin.org')
In [104]: cookies.set('cookies_off_domain', 'nope', domain="example.org")
In [105]: r = httpx.get("http://httpbin.org/cookies", cookies=cookies)
In [106]: r.json()
Out[106]: {'cookies': {'cookie_on_domain': 'hello, there!'}}
```
`URL` 重定向和历史 默认情况下,HTTPX将对重定向执行除`HEAD`请求之外的任何操作。`history`响应的属性可用于检查所有后续重定向,它包含遵循他们的顺序的所有重定向响应的列表`GITHUB`将所有`HTTP`请求重定向到`HTTPS`
```python
In [107]: r = httpx.get("http://github.com")
In [108]: r.url
Out[108]: URL('https://github.com')
In [109]: r.status_code
Out[109]: 200
In [110]: r.history
Out[110]: []
```
可以使用allow_redirects参数修改默认的重定向处理:
```python
In [113]: r = httpx.head('http://github.com/', allow_redirects=True)
In [114]: r.url
Out[114]: URL('https://github.com/')
```
<a name="XznQJ"></a>
### 认证方式
<a name="IH48v"></a>
#### 1.HTTPX支持基本和摘要HTTP身份验证
```python
In [122]: httpx.get("https://example.com", auth=("my_user", "password123"))
     ...:
Out[122]: <Response [200 OK]>
```
<a name="RphsI"></a>
#### 2.摘要式身份验证的凭据
```python
In [123]: auth = httpx.DigestAuth("my_user", "password123")
In [124]: httpx.get("https://example.com", auth=auth)
Out[124]: <Response [200 OK]>
```
