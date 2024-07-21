Python 爬虫
<a name="LVu32"></a>
## 为什么要使用代理？
在写爬虫时是否遇到过这样的情况，测试时爬虫可以正常工作，但运行一段时间，就会发现报错或是没有数据返回，网页可能会提示“IP访问频繁”。这就说明网站对IP方面是有反爬措施的（IP一定时间内的请求次数及速度）。如果超过了某个阈值，就会直接拒绝服务，也就是经常说的“封IP”。<br />这种情况下，就到了代理IP出场了。代理实际就是代理服务器，它的工作原理其实很简单，在正常请求一个网站时，是直接发送请求给Web服务器，Web服务器把响应数据传给我们。而用了代理IP，就像是在本机和Web服务器之间搭建了一个“桥”，此时本机会先向代理服务器发出请求，再由代理服务器发送给Web服务器。响应数据的返回也是同样的道理，都需要代理服务器来中转。这样Web服务器就不容易识别出本机IP了。<br />但代理IP也有优劣之分，以隐匿性来分有3种：高匿代理，普匿代理，透明代理。<br />来看看 Github 热门的开源项目 ProxyPool 是如何使用吧！
<a name="iIIwq"></a>
## ProxyPool简介
一个爬虫代理IP池，定时采集网上发布的免费代理并验证入库，定时检测代理的可用性，提供 API 和 CLI 两种使用方式。同时也可以扩展代理源以增加代理池IP的质量和数量。<br />Github仓库地址：[https://github.com/jhao104/proxy_pool](https://github.com/jhao104/proxy_pool)
<a name="f0fEY"></a>
## 下载代码
通过 `git clone` 下载代码
```bash
git clone git@github.com:jhao104/proxy_pool.git
```
或者下载对应zip文件
<a name="bIre6"></a>
## 安装依赖库
```bash
pip install -r requirements.txt
```
<a name="QlUni"></a>
## 修改配置文件
打开代码文件中的 setting.py ，根据自己的需求修改项目配置。
```python
# 配置API服务
HOST = "0.0.0.0"               # IP
PORT = 5000                    # 监听端口


# 配置数据库
DB_CONN = 'redis://:pwd@127.0.0.1:8888/0'
# 无密码
DB_CONN = 'redis://:@127.0.0.1:8888/0'

# proxy table name 表名(自己建的)
TABLE_NAME = 'use_proxy'

# 配置 ProxyFetcher
PROXY_FETCHER = [
    "freeProxy01",      # 这里是启用的代理抓取方法名，所有fetch方法位于fetcher/proxyFetcher.py
    "freeProxy02",
    # ....
]
```
<a name="AHOH6"></a>
## 运行项目
项目有两种运行方式，自行选择。
<a name="eJWBd"></a>
### 1.启动调度程序
启动 redis 服务：`redis-server.exe`（可执行文件在 redis 安装路径下）。启动调度程序，在 Proxy_pool 项目路径下打开 cmd 输入：
```bash
python proxyPool.py schedule
```
读取数据库中的代理。
```python
import redis

r = redis.StrictRedis(host="127.0.0.1", port=6379, db=0)
result = r.hgetall('use_proxy')
result.keys()
```
<a name="SPiPp"></a>
### 2.启动webApi服务
```bash
python proxyPool.py server
```
启动web服务后，默认配置下会开启 http://127.0.0.1:5010 的api接口服务:

| api | method | Description | params |
| --- | --- | --- | --- |
| / | GET | api介绍 | None |
| /get | GET | 随机获取一个代理 | 可选参数: ?type=https 过滤支持https的代理 |
| /pop | GET | 获取并删除一个代理 | 可选参数: ?type=https 过滤支持https的代理 |
| /all | GET | 获取所有代理 | 可选参数: ?type=https 过滤支持https的代理 |
| /count | GET | 查看代理数量 | None |
| /delete | GET | 删除代理 | ?proxy=host:ip |

在爬虫中的使用方法，爬取数据的同时检测代理IP的可用性，超过5次连接失败，则删除代理IP。
```python
import requests

def get_proxy():
    return requests.get("http://127.0.0.1:5010/get/").json()

def delete_proxy(proxy):
    requests.get("http://127.0.0.1:5010/delete/?proxy={}".format(proxy))

# your spider code

def getHtml():
    # ....
    retry_count = 5
    proxy = get_proxy().get("proxy")
    while retry_count > 0:
        try:
            html = requests.get('http://www.example.com', proxies={"http": "http://{}".format(proxy)})
            # 使用代理访问
            return html
        except Exception:
            retry_count -= 1
    # 删除代理池中代理
    delete_proxy(proxy)
    return None
```
代理池中的代理毕竟是爬取的免费代理，IP质量那真是一言难尽，但也足够日常开发使用。对于使用还有不懂的话，可以看一下完整文档。
