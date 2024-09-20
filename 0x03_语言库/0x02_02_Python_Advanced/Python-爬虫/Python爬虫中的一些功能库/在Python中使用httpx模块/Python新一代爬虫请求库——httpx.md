Python 爬虫 <br />在 Python3 网络爬虫领域最新的比较火的工具莫过于 httpx 了，号称新一代网络请求神库<br />对比常用的 requests, 除了支持 requests 的所有操作之外，还具有以下特点：

- 同时支持同步和异步请求
- 支持 HTTP1.0/HTTP2.0
- 可直接向 WSGI 程序或 ASGI 程序发出请求
- 类型注释

从以上可以看出在 requests 的所有功能之上，增加了更多新的功能，相当于一个功能更强大的 requests !!
<a name="i2d2M"></a>
## 简单用法
首先需要使用 pip 进行安装`pip insatll httpx`<br />如果使用支持 HTTP/2 的功能, 使用以下安装`pip install httpx[http2]`<br />基本的使用方法和 requests 非常类似
```python
r = httpx.get('https://httpbin.org/post')
r.text
r.status_code
r.content
```
上面是 get 请求，post 请求也是一样
```python
r = httpx.post('https://httpbin.org/post', data={'key': 'value'})
r.json()
r.status_code
r.content
```
除了上面的发送表单格式数据之外，发送 json 类型数据时候使用 json 参数<br />requests 中习惯使用 `requests.Session()`, 在 httpx 中用 `httpx.Client()` 来代替，代码如下：
```python
>>> with httpx.Client() as client:
... r = client.get('https://example.com')
...
>>> r
<Response [200 OK]>
```
另外使用 Client 具有更高的性能，在使用 httpx 发送请求的时候会为每一个请求建立一个新的连接，如果请求量很大，效率相对于Client 会变得低效。<br />因为Client实例使用HTTP 连接池！在向同一主机发出多个请求时，Client 将重用底层 TCP 连接，而不是为每个请求重新创建一个。<br />所以它的性能会更好

- 减少跨请求的延迟
- 减少 CPU 使用率和往返次数
- 减少网络拥塞

同时还多了好几个功能，比如保持会话功能，维持整个请求过程中 `cookie`的一致性等
<a name="QofPk"></a>
## 事件监听
HTTPX 支持在请求和响应端监听的功能，常说的 hook 功能<br />可以非常方便的进行日志记录、监控或跟踪等
```python
def log_request(request):
    print(f"Request event hook: {request.method} {request.url} - Waiting for response")

def log_response(response):
    request = response.request
    print(f"Response event hook: {request.method} {request.url} - Status {response.status_code}")

client = httpx.Client(event_hooks={'request': [log_request], 'response': [log_response]})
```
如在请求完全准备好之后，但还未被发送到网络之前会调用 `log_request` 函数<br />在网络获取响应返回之后，但还未发送到调用着之前会调用 `log_response` 函数<br />通过上面两个函数，可以实现日志记录，请求监控等等功能<br />可以看到上面传参是通过列表 `[log_request]` 的方式，所以可以注册多个 `hook`函数
<a name="SQTzH"></a>
## 异步请求
默认情况下，HTTPX 使用同步 API 进行请求，但其也支持异步请求。<br />异步请求的方式如下，使用`AsyncClient`
```python
import asyncio
import httpx

async def main():
    async with httpx.AsyncClient() as client:
        response = await client.get('https://www.example.com/')
        print(response)

asyncio.run(main())
```
<a name="nukjE"></a>
## HTTP/2
HTTP/2 是 HTTP 协议的主要新迭代，它提供了更高效的传输，并具有潜在的性能优势。HTTP/2 不会改变请求或响应的核心语义，但会改变数据发送到服务器和从服务器发送的方式<br />使用 httpx 客户端时，默认情况下不启用 HTTP/2 , 在安装 HTTP/2 依赖后可使用,方法也很简单
```python
async with httpx.AsyncClient(http2=True) as client:
    ...
```
在安装好依赖之后，将 http2 参数设为 True即可，非常的方便<br />HTTP/2 支持可用于`Client`和`AsyncClient`, 如果要发送大并发请求，使用异步效果会更好<br />但是不是所有的网站都支持 HTTP/2 协议, 可通过下面代码判断
```python
client = httpx.AsyncClient(http2=True)
response = await client.get(...)
print(response.http_version)
# "HTTP/1.0", "HTTP/1.1", or "HTTP/2".
```
