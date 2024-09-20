Python 多线程
<a name="pq3Z4"></a>
### 相关概念
线程（Thread）也叫轻量级进程，是操作系统能够进行运算调度的最小单位，它被包含在进程之中，是进程中的实际运作单位。线程自己不拥有系统资源，只拥有一点在运行中必不可少的资源，但它可与同属的一个进程的其它线程共享进程所拥有的全部资源。一个线程可以创建和撤销另一个线程，同一进程中的多个线程之间可以并发执行。
<a name="Ksqt1"></a>
### 多线程语法
在Python中实现多线程编程需要用到的就是threading模块中的Thread类，来看看最简单的语法，首先来一个简单的函数。
```python
def task(num):
    count = 0
    for i in range(num):
        count += 1
    print(count)


nums = [100, 1000, 10000]
for num in nums:
    task(num)

# 100
#1000
#10000
```
用三个子线程分别计算。
```python
import threading


def task(num):
    count = 0
    for i in range(num):
        count += 1
    print(count)


nums = [100, 1000, 10000]
for num in nums:
    t = threading.Thread(target=task, args=(num,))
    t.start()
```
利用Thread创建线程，`target`参数接收函数名，`args`参数接收函数的参数，`start`方法启动线程。<br />这里还需要讲解一下`join`方法，他的作用是让主线程等待，直到该子线程结束。来看看加该方法和不加该方法，最终的结果是怎么样的。
```python
import threading


def task():
    num = 0
    for i in range(10000000):
        num += 1
    print(num)


t = threading.Thread(target=task)
t.start()
print('end')

# end
# 10000000

import threading


def task():
    num = 0
    for i in range(10000000):
        num += 1
    print(num)


t = threading.Thread(target=task)
t.start()
t.join()
print('end')

# 10000000
# end
```
<a name="GY465"></a>
### GIL
在说概念之前，还是以上面的代码为例，分别求单线程和多线程代码运行的时间。<br />单线程
```python
import time

def task(num):
    count = 0
    for i in range(num):
        count += 1
    print(count)


nums = [1000000, 100000000, 1000000000]
start = time.time()
for num in nums:
    task(num)
end = time.time()
print(end - start)

# 50.44705629348755
```
多线程
```python
import threading
import time


def task(num):
    count = 0
    for i in range(num):
        count += 1
    print(count)


nums = [1000000, 100000000, 1000000000]
ts = []
start = time.time()

for num in nums:
    t = threading.Thread(target=task, args=(num,))
    t.start()
    ts.append(t)

for t in ts:
    t.join()

end = time.time()
print(end - start)

# 55.022353172302246
```
可以发现多线程比单线程花费的时间还要更多，这是因为GIL的原因。<br />GIL的全称是Global Interpreter Lock（全局解释器锁），Python最初的设计理念在于，为了解决多线程之间数据完整性和状态同步的问题，设计为在任意时刻只能由一个线程在解释器中运行。因此Python中的多线程是表面上的多线程（同一时刻只有一个线程），不是真正的多线程。<br />但是如果是因为GIL的原因，就说多线程无用是不对的，对于IO密集的程序，多线程是要比单线程快的。举一个简单的爬虫案例。<br />单线程
```python
import time


def task(url):
    s = url.split('_')[-1]
    time.sleep(int(s)) #这里模拟请求等待


urls = ['url_1', 'url_2', 'url_3']
start = time.time()
for url in urls:
    task(url)
end = time.time()
print(end - start)

# 6.013520002365112
```
多线程
```python
import threading
import time


def task(url):
    s = url.split('_')[-1]
    time.sleep(int(s))


ts = []
urls = ['url_1', 'url_2', 'url_3']
start = time.time()

for url in urls:
    t = threading.Thread(target=task, args=(url,))
    t.start()
    ts.append(t)

for t in ts:
    t.join()
    
end = time.time()
print(end - start)

# 3.005527973175049
```
这时候就能看到多线程的优势了，虽然多线程只是在各线程来回切换，但是可以让IO堵塞的时间切换到其他线程做其他的任务，很适合爬虫或者文件的操作。
