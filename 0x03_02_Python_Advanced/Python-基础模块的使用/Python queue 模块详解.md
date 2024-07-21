Python<br />queue 模块即队列，特别适合处理信息在多个线程间安全交换的多线程程序中。下面对 queue 模块进行一个详细的使用介绍。
<a name="HxEUS"></a>
## 1、queue 模块定义的类和异常
queue 模块定义了以下四种不同类型的队列，它们之间的区别在于数据入队列之后出队列的顺序不同。
<a name="oqxKU"></a>
### 1.1 `queue.Queue(maxsize=0)`
先进先出(First In First Out: FIFO)队列，最早进入队列的数据拥有出队列的优先权，就像看电影入场时排队一样，排在队伍前头的优先进入电影院。<br />入参 maxsize 是一个整数，用于设置队列的最大长度。一旦队列达到上限，插入数据将会被阻塞，直到有数据出队列之后才可以继续插入。如果 maxsize 设置为小于或等于零，则队列的长度没有限制。<br />示例如下：
```python
import queue
q = queue.Queue()  # 创建 Queue 队列
for i in range(3):
    q.put(i)  # 在队列中依次插入0、1、2元素
for i in range(3):
    print(q.get())  # 依次从队列中取出插入的元素，数据元素输出顺序为0、1、2
```
<a name="gVuRX"></a>
### 1.2 `queue.LifoQueue(maxsize=0)`
后进先出(Last In First Out: LIFO)队列，最后进入队列的数据拥有出队列的优先权，就像栈一样。<br />入参 maxsize 与先进先出队列的定义一样。<br />示例如下：
```python
import queue
q = queue.LifoQueue()  # 创建 LifoQueue 队列
for i in range(3):
    q.put(i)  # 在队列中依次插入0、1、2元素
for i in range(3):
    print(q.get())  # 依次从队列中取出插入的元素，数据元素输出顺序为2、1、0
```
<a name="FuXLw"></a>
### 1.3 `PriorityQueue(maxsize=0)`
优先级队列，比较队列中每个数据的大小，值最小的数据拥有出队列的优先权。数据一般以元组的形式插入，典型形式为(priority_number, data)。如果队列中的数据没有可比性，那么数据将被包装在一个类中，忽略数据值，仅仅比较优先级数字。<br />入参 maxsize 与先进先出队列的定义一样。<br />示例如下：
```python
import queue
q = queue.PriorityQueue()  # 创建 PriorityQueue 队列
data1 = (1, 'python')
data2 = (2, '-')
data3 = (3, '100')
style = (data2, data3, data1)
for i in style:
    q.put(i)  # 在队列中依次插入元素 data2、data3、data1
for i in range(3):
    print(q.get())  # 依次从队列中取出插入的元素，数据元素输出顺序为 data1、data2、data3
```
<a name="cU02g"></a>
### 1.4 `queue.SimpleQueue`
先进先出类型的简单队列，没有大小限制。由于它是简单队列，相比于 Queue 队列会缺少一些高级功能。<br />示例如下：
```python
import queue
q = queue.SimpleQueue()  # 创建 SimpleQueue 队列
for i in range(3):
    q.put(i)  # 在队列中依次插入0、1、2元素
for i in range(3):
    print(q.get())  # 依次从队列中取出插入的元素，数据元素输出顺序为0、1、2
```
<a name="G7InS"></a>
### 1.5 `queue.Empty` 异常
当队列中没有数据元素时，取出队列中的数据会引发 `queue.Empty` 异常，主要是不正当使用 `get()` 和 `get_nowait()` 引起的。<br />示例如下：
```python
import queue
try:
    q = queue.Queue(3)  # 设置队列上限为3
    q.put('python')  # 在队列中插入字符串 'python'
    q.put('-') # 在队列中插入字符串 '-'
    q.put('100') # 在队列中插入字符串 '100'
    for i in range(4):  # 从队列中取数据，取出次数为4次，引发 queue.Empty 异常
        print(q.get(block=False))
except queue.Empty:
    print('queue.Empty')
```
<a name="aNID5"></a>
### 1.6 `queue.Full` 异常
当队列数据元素容量达到上限时，继续往队列中放入数据会引发 `queue.Empty` 异常，主要是不正当使用 `put()` 和 `put_nowait()` 引起的。<br />示例如下：
```python
import queue
try:
    q = queue.Queue(3)  # 设置队列上限为3
    q.put('python')  # 在队列中插入字符串 'python'
    q.put('-') # 在队列中插入字符串 '-'
    q.put('100') # 在队列中插入字符串 '100'
    q.put('stay hungry, stay foolish', block=False)  # 队列已满，继续往队列中放入数据，引发 queue.Full 异常
except queue.Full:
    print('queue.Full')
```
<a name="uzaft"></a>
## 2、Queue、LifoQueue、PriorityQueue 和 SimpleQueue 对象的基本使用方法
Queue、LifoQueue、PriorityQueue 和 SimpleQueue 四种队列定义的对象均提供了以下函数使用方法，下面以 Queue 队列为例进行介绍。
<a name="mjSqP"></a>
### 2.1 `Queue.qsize()`
返回队列中数据元素的个数。<br />示例如下：
```python
import queue
q = queue.Queue()
q.put('python-100')  # 在队列中插入元素 'python-100'
print(q.qsize())  # 输出队列中元素个数为1
```
<a name="HTAO7"></a>
### 2.2 `Queue.empty()`
如果队列为空，返回 True，否则返回 False。<br />示例如下：
```python
import queue
q = queue.Queue()
print(q.empty())  # 对列为空，返回 True
q.put('python-100')  # 在队列中插入元素 'python-100'
print(q.empty())  # 对列不为空，返回 False
```
<a name="uREes"></a>
### 2.3 `Queue.full()`
如果队列中元素个数达到上限，返回 True，否则返回 False。<br />示例如下：
```python
import queue
q = queue.Queue(3)  # 定义一个长度为3的队列
print(q.full())  # 元素个数未达到上限，返回 False
q.put('python')  # 在队列中插入字符串 'python'
q.put('-') # 在队列中插入字符串 '-'
q.put('100') # 在队列中插入字符串 '100'
print(q.full())  # 元素个数达到上限，返回 True
```
<a name="GmSfi"></a>
### 2.4 `Queue.put(item, block=True, timeout=None)`

- `item`，放入队列中的数据元素。
- `block`，当队列中元素个数达到上限继续往里放数据时：如果 `block=False`，直接引发 `queue.Full` 异常；如果 `block=True`，且 `timeout=None`，则一直等待直到有数据出队列后可以放入数据；如果 `block=True`，且 `timeout=N`，N 为某一正整数时，则等待 N 秒，如果队列中还没有位置放入数据就引发 `queue.Full` 异常。
- `timeout`，设置超时时间。

示例如下：
```python
import queue
try:
    q = queue.Queue(2)  # 设置队列上限为2
    q.put('python')  # 在队列中插入字符串 'python'
    q.put('-') # 在队列中插入字符串 '-'
    q.put('100', block = True, timeout = 5) # 队列已满，继续在队列中插入字符串 '100'，等待5秒后会引发 queue.Full 异常
except queue.Full:
    print('queue.Full')
```
<a name="tG8GY"></a>
### 2.5 `Queue.put_nowait(item)`
相当于 `Queue.put(item, block=False)`，当队列中元素个数达到上限继续往里放数据时直接引发 `queue.Full` 异常。
```python
import queue
try:
    q = queue.Queue(2)  # 设置队列上限为2
    q.put_nowait('python')  # 在队列中插入字符串 'python'
    q.put_nowait('-') # 在队列中插入字符串 '-'
    q.put_nowait('100') # 队列已满，继续在队列中插入字符串 '100'，直接引发 queue.Full 异常
except queue.Full:
    print('queue.Full')
```
<a name="p48VQ"></a>
### 2.6 `Queue.get(block=True, timeout=None)`
从队列中取出数据并返回该数据内容。

- `block`，当队列中没有数据元素继续取数据时：如果 `block=False`，直接引发 `queue.Empty` 异常；如果 `block=True`，且 `timeout=None`，则一直等待直到有数据入队列后可以取出数据；如果 `block=True`，且 `timeout=N`，N 为某一正整数时，则等待 N 秒，如果队列中还没有数据放入的话就引发 `queue.Empty` 异常。
- `timeout`，设置超时时间。

示例如下：
```python
import queue
try:
    q = queue.Queue()
    q.get(block = True, timeout = 5) # 队列为空，往队列中取数据时，等待5秒后会引发 queue.Empty 异常
except queue.Empty:
    print('queue.Empty')
```
<a name="ojlAJ"></a>
### 2.7 `Queue.get_nowait()`
相当于 `Queue.get(block=False)block`，当队列中没有数据元素继续取数据时直接引发 `queue.Empty` 异常。<br />示例如下：
```python
import queue
try:
    q = queue.Queue()
    q.get_nowait() # 队列为空，往队列中取数据时直接引发 queue.Empty 异常
except queue.Empty:
    print('queue.Empty')
```
<a name="lVbYQ"></a>
## 3、Queue、LifoQueue 和 PriorityQueue 对象的高级使用方法
SimpleQueue 是 Python 3.7 版本中新加入的特性，与 Queue、LifoQueue 和 PriorityQueue 三种队列相比缺少了 `task_done` 和 `join` 的高级使用方法，所以才会取名叫 Simple 了，下面介绍一下 task_done 和 join 的使用方法。

- `task_done`，表示队列内的数据元素已经被取出，即每个 get 用于获取一个数据元素， 后续调用 task_done 告诉队列，该数据的处理已经完成。如果被调用的次数多于放入队列中的元素个数，将引发 ValueError 异常。
- `join`，一直阻塞直到队列中的所有数据元素都被取出和执行，只要有元素添加到 queue 中就会增加。当未完成任务的计数等于0，join 就不会阻塞。

示例如下：
```python
import queue
q = queue.Queue()
q.put('python')
q.put('-')
q.put('100')
for i in range(3):
    print(q.get())
    q.task_done()  # 如果不执行 task_done，join 会一直处于阻塞状态，等待 task_done 告知它数据的处理已经完成
q.join()
```
下面是一个经典示例，生产者和消费者线程分别生产数据和消费数据，先生产后消费。采用 `task_done` 和 `join` 确保处理信息在多个线程间安全交换，生产者生产的数据能够全部被消费者消费掉。
```python

from queue import Queue
import random
import threading
import time

#生产者线程
class Producer(threading.Thread):
    def __init__(self, t_name, queue):
        threading.Thread.__init__(self, name=t_name)
        self.data=queue
    def run(self):
        for i in range(5):
            print ("%s: %s is producing %d to the queue!" %(time.ctime(), self.getName(), i))
            self.data.put(i)  # 将生产的数据放入队列
            time.sleep(random.randrange(10)/5)
        print ("%s: %s finished!" %(time.ctime(), self.getName()))

#消费者线程
class Consumer(threading.Thread):
    def __init__(self, t_name, queue):
        threading.Thread.__init__(self, name=t_name)
        self.data=queue
    def run(self):
        for i in range(5):
            val = self.data.get()  # 拿出已经生产好的数据
            print ("%s: %s is consuming. %d in the queue is consumed!" %(time.ctime(), self.getName(), val))
            time.sleep(random.randrange(5))
            self.data.task_done() # 告诉队列有关这个数据的任务已经处理完成
        print ("%s: %s finished!" %(time.ctime(), self.getName()))

#主线程
def main():
    queue = Queue()
    producer = Producer('Pro.', queue)
    consumer = Consumer('Con.', queue)
    producer.start()
    consumer.start()
    queue.join()  # 阻塞，直到生产者生产的数据全都被消费掉
    producer.join() # 等待生产者线程结束
    consumer.join() # 等待消费者线程结束
    print ('All threads terminate!')
 
if __name__ == '__main__':
    main()
```
<a name="V4TQv"></a>
## 参考资料
[https://docs.python.org/3/library/queue.html](https://docs.python.org/3/library/queue.html)
