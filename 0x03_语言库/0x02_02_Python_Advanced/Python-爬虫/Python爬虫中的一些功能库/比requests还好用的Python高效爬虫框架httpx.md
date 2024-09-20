Python
<a name="d8rrX"></a>
## **什么是协程？**
简单来说，协程是一种基于线程之上，但又比线程更加轻量级的存在。对于系统内核来说，协程具有不可见的特性，所以这种由 程序员自己写程序来管理 的轻量级线程又常被称作 "用户空间线程"。
<a name="E6AHB"></a>
## **协程比多线程好在哪呢？**

1. 线程的控制权在操作系统手中，而协程的控制权完全掌握在用户自己手中，因此利用协程可以减少程序运行时的上下文切换，有效提高程序运行效率。
2. 建立线程时，系统默认分配给线程的**栈**大小是 1 M，而协程更轻量，接近 1 K ，因此可以在相同的内存中开启更多的协程。
3. 由于协程的本质不是多线程而是单线程，所以不需要多线程的锁机制。因为只有一个线程，也不存在同时写变量而引起的冲突。在协程中控制共享资源不需要加锁，只需要判断状态即可。所以协程的执行效率比多线程高很多，同时也有效避免了多线程中的竞争关系。
<a name="atBTR"></a>
## **协程的适用 & 不适用场景**
**适用场景：协程适用于被阻塞的，且需要大量并发的场景。**<br />不适用场景：协程不适用于存在大量计算的场景（因为协程的本质是单线程来回切换），如果遇到这种情况，还是应该使用其他手段去解决。
<a name="lMtIZ"></a>
## **初探异步 http 框架 httpx**
至此对 "协程" 应该有了个大概的了解，但故事说到这里，相信有朋友还是满脸疑问："协程" 对于接口测试有什么帮助呢？不要着急，答案就在下面。<br />相信用过 Python 做接口测试的朋友都对 requests 库不陌生。requests 中实现的 http 请求是同步请求，但其实基于 http 请求 IO 阻塞的特性，非常适合用协程来实现 "异步" http 请求从而提升测试效率。 <br />相信早就有人注意到了这点，于是在 Github 经过了一番探索后，果不其然，最终寻找到了支持协程 "异步" 调用 http 的开源库: httpx
<a name="dBDsW"></a>
## **什么是 httpx**
httpx 是一个几乎继承了所有 requests 的特性并且支持 "异步" http 请求的开源库。简单来说，可以认为 httpx 是强化版 requests。<br />下面可以一起见识一下 httpx 的强大
<a name="boDhV"></a>
## **安装**
httpx 的安装非常简单，在 Python 3.6 以上的环境执行
```shell
pip install httpx
```
<a name="IiUUB"></a>
## **最佳实践**
这里分别使用了 httpx 异步 和 同步 的方式对批量 http 请求进行了耗时比较，来一起看看结果吧～<br />首先来看看同步 http 请求的耗时表现：
```python
import asyncio
import httpx
import threading
import time

def sync_main(url, sign):
    response = httpx.get(url).status_code
    print(f'sync_main: {threading.current_thread()}: {sign}2 + 1{response}')

sync_start = time.time()
[sync_main(url='http://www.baidu.com', sign=i) for i in range(200)]
sync_end = time.time()
print(sync_end - sync_start)
```
代码比较简单，可以看到在 sync_main 中则实现了同步 http 访问百度 200 次。<br />运行后输出如下（截取了部分关键输出...）：
```python
sync_main: <_MainThread(MainThread, started 4471512512)>: 192: 200
sync_main: <_MainThread(MainThread, started 4471512512)>: 193: 200
sync_main: <_MainThread(MainThread, started 4471512512)>: 194: 200
sync_main: <_MainThread(MainThread, started 4471512512)>: 195: 200
sync_main: <_MainThread(MainThread, started 4471512512)>: 196: 200
sync_main: <_MainThread(MainThread, started 4471512512)>: 197: 200
sync_main: <_MainThread(MainThread, started 4471512512)>: 198: 200
sync_main: <_MainThread(MainThread, started 4471512512)>: 199: 200
16.56578803062439
```
可以看到在上面的输出中, 主线程没有进行切换（因为本来就是单线程啊喂！）请求按照顺序执行（因为是同步请求）。<br />程序运行共耗时 16.6 秒<br />下面试试 "异步" http 请求：
```python
import asyncio
import httpx
import threading
import time

client = httpx.AsyncClient()

async def async_main(url, sign):
    response = await client.get(url)
    status_code = response.status_code
    print(f'async_main: {threading.current_thread()}: {sign}:{status_code}')

loop = asyncio.get_event_loop()
tasks = [async_main(url='http://www.baidu.com', sign=i) for i in range(200)]
async_start = time.time()
loop.run_until_complete(asyncio.wait(tasks))
async_end = time.time()
loop.close()
print(async_end - async_start)
```
上述代码在 async_main 中用 async await 关键字实现了"异步" http，通过 asyncio ( 异步 io 库请求百度首页 200 次并打印出了耗时）。<br />运行代码后可以看到如下输出（截取了部分关键输出...）
```python
async_main: <_MainThread(MainThread, started 4471512512)>: 56: 200
async_main: <_MainThread(MainThread, started 4471512512)>: 99: 200
async_main: <_MainThread(MainThread, started 4471512512)>: 67: 200
async_main: <_MainThread(MainThread, started 4471512512)>: 93: 200
async_main: <_MainThread(MainThread, started 4471512512)>: 125: 200
async_main: <_MainThread(MainThread, started 4471512512)>: 193: 200
async_main: <_MainThread(MainThread, started 4471512512)>: 100: 200
4.518340110778809
```
可以看到顺序虽然是乱的（56，99，67...） (这是因为程序在协程间不停切换) 但是主线程并没有切换 （协程本质还是单线程 ）。<br />程序共耗时 4.5 秒<br />比起同步请求耗时的 16.6 秒 缩短了接近 73 %！
