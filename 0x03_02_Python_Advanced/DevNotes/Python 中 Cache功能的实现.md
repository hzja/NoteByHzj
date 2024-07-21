Python
<a name="MZIQm"></a>
## 介绍
cachetools，这是一个可扩展的基于内存的 Collections、Decorators 的封装实现。<br />因为是 Cache，那么就一定有它的页面置换算法。根据操作系统学过的一些知识，置换算法就会有 LRU、LFU、FIFO 等等。比如说，当 Cache 已经满了的情况下，如果这时候再插入一个新的数据，那么这时候就需要根据页面置换算法对已有的数据进行置换，用新的数据替代旧的数据，保证 Cache 最大占用量不会超标。<br />这里来体验下这个库的具体用法吧。首先是安装，直接使用 pip3 安装即可：
```bash
pip3 install cachetools
```
安装好之后，再来看看它的具体用法。
<a name="dPUAE"></a>
## 基本 Cache 的使用
来看一个简单的实例：
```python
from cachetools import Cache

cache = Cache(maxsize=3)
cache['1'] = 'Hello'
cache['2'] = 'World'
print('current size', cache.currsize)
cache.pop('2')
print(cache.items)
print('length', len(cache))
cache['3'] = 'Hello'
cache['4'] = 'World'
print('current size', cache.currsize)
cache['5'] = 'Hello'
print('current size', cache.currsize)
print(cache.items)
```
运行结果如下：
```python
current size 2
<bound method Mapping.items of Cache([('1', 'Hello')], maxsize=3, currsize=1)>
length 1
current size 3
current size 3
<bound method Mapping.items of Cache([('3', 'Hello'), ('4', 'World'), ('5', 'Hello')], maxsize=3, currsize=3)>
```
首先这里声明了一个 Cache 对象，有一个必传的参数是 maxsize，这里设置为 3，这里的 3 其实就是长度的意思，并不是实际内存占用大小。<br />接着赋值了 1 和 2 两个键名，接着打印出来了当前 Cache 的大小，所以结果就是 2，这个 size 就是一个单纯的数量值。<br />然后接着调用了 pop 方法移除了 2 对应的内容，然后打印 Cache 的所有内容和对应长度，理所应当，长度就是 2，然后就剩下一个值。<br />接着又赋值了 3 和 4 两个键名，然后打印了当前 Cache 的大小，这会 Cache 达到了 maxsize，结果就是 3。<br />最后又赋值了 5 这个键名，然后打印了当前 Cache 的大小和 Cache 的所有内容，因为 Cache 已经达到了 maxsize 了，所以结果依然是 3，最前面的 1 这个键名对应的内容就被移除了。<br />所以，这个 Cache 对象可以维持一个最大恒定大小，并且保证长度不会超过 maxsize。
<a name="lzvxD"></a>
## 其他 Cache 的使用
当然除了 Cache，还有一些 Cache 的子类，比如说 FIFOCache、LFUCahce、LRUCache、MRUCache、RRCache，这里简单说下：

- FIFO：First In、First Out，就是先进先出。
- LFU：Least Frequently Used，就是淘汰最不常用的。
- LRU：Least Recently Used，就是淘汰最久不用的。
- MRU：Most Recently Used，与 LRU 相反，淘汰最近用的。
- RR：Random Replacement，就是随机替换。

具体的实例这里就不再讲解了。
<a name="qk8Si"></a>
## 特殊 TTLCache 的使用
当然除了基本的 Cache，cachetools 还提供了一种特殊的 Cache 实现，叫做 TTLCache。<br />TTL 就是 time-to-live 的简称，也就是说，Cache 中的每个元素都是有过期时间的，如果超过了这个时间，那这个元素就会被自动销毁。如果都没过期并且 Cache 已经满了的话，那就会采用 LRU 置换算法来替换掉最久不用的，以此来保证数量。<br />下面来看一个样例：
```python
from datetime import timedelta, datetime
from cachetools import TTLCache
from time import sleep

cache = TTLCache(maxsize=3, ttl=timedelta(seconds=5), timer=datetime.now)
cache['1'] = 'Hello'
sleep(1)
cache['2'] = 'World'
print(cache.items)
sleep(4.5)
print(cache.items)
sleep(1)
print(cache.items)
```
运行结果如下：
```python
<bound method Mapping.items of TTLCache([('1', 'Hello'), ('2', 'World')], maxsize=3, currsize=2)>
<bound method Mapping.items of TTLCache([('2', 'World')], maxsize=3, currsize=1)>
<bound method Mapping.items of TTLCache([], maxsize=3, currsize=0)>
```
这里声明了一个 TTLCache，maxsize 是 3，然后 ttl 设置为了 5 秒，也就是说，每个元素 5 秒之后都会过期。<br />首先赋值 1 这个键名为 Hello，然后 1 秒之后赋值 2 这个键名为 World，接着将现有 Cache 的结果输出出来。<br />接着等待 4.5 秒，这时候 1 这个键名就已经超过 5 秒了，所以 1 这个键名理应就被销毁了。<br />接着再等待 1 秒，这时候 2 这个键名也超过 5 秒了，所以 2 这个键名也理应就被销毁了。<br />最后看运行结果也如期望的一样。
<a name="rcZNy"></a>
## 大小计算
有的同学说，这里 maxsize 用的这个数字指的是内容的长度，但实际上不同的内容占用的空间是完全不一样的，有没有根据实际内存占用来计算 size 的方法呢？<br />有的！<br />这里只需要替换掉 Cache 的 getsizeof 方法即可。<br />这里需要额外引入一个库，叫做 pympler，它提供了一个 asizeof 方法可以计算实际 Object 的占用内存大小，单位是 bytes。<br />pympler 安装：
```bash
pip3 install pympler
```
所以，如果要设置 Cache 占用的最大内存大小，比如 2MB，那就可以这么设置：
```python
from cachetools import Cache
from pympler import asizeof

cache = Cache(maxsize=2 * 1024 * 1024, getsizeof=asizeof.asizeof)
cache['a'] = '123'
print(cache.currsize)
cache['b'] = '123'
print(cache.currsize)
cache['c'] = '456'
print(cache.currsize)
cache['d'] = {
    'a': 'b',
    'b': 'c',
    'c': 'd'
}
print(cache.currsize)
```
这里 maxsize 就设置为了 2MB，同时 `getsizeof` 方法设置为了 pympler 的 `asizeof` 方法，这样 Cache 在计算 size 的时候就会用 `asizeof` 方法了。<br />这里随便插入一些数据，看看实际的 size 变化，运行结果如下：
```python
56
112
168
640
```
其结果就是 Cache 占用的字节数。可以看到数据的复杂度高，占用的空间越大。
