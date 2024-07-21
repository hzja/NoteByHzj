Python
<a name="ypX2K"></a>
### **方法一：使用全局变量的列表，来保存返回值**
```python
ret_values = []

def thread_func(*args):
    ...
    value = ...
    ret_values.append(value)
```
选择列表的一个原因是：列表的 `append()` 方法是线程安全的，CPython 中，GIL 防止对它们的并发访问。如果使用自定义的数据结构，在并发修改数据的地方需要加线程锁。<br />如果事先知道有多少个线程，可以定义一个固定长度的列表，然后根据索引来存放返回值，比如：
```python
from threading import Thread

threads = [None] * 10
results = [None] * 10

def foo(bar, result, index):
    result[index] = f"foo-{index}"

for i in range(len(threads)):
    threads[i] = Thread(target=foo, args=('world!', results, i))
    threads[i].start()

for i in range(len(threads)):
    threads[i].join()

print (" ".join(results))
```
<a name="Wmp50"></a>
### **方法二：重写 Thread 的 **`**join**`** 方法，返回线程函数的返回值**
默认的 `thread.join()` 方法只是等待线程函数结束，没有返回值，可以在此处返回函数的运行结果，代码如下：
```python
from threading import Thread


def foo(arg):
    return arg


class ThreadWithReturnValue(Thread):
    def run(self):
        if self._target is not None:
            self._return = self._target(*self._args, **self._kwargs)

    def join(self):
        super().join()
        return self._return


twrv = ThreadWithReturnValue(target=foo, args=("hello world",))
twrv.start()
print(twrv.join()) # 此处会打印 hello world。
```
这样当调用 `thread.join()` 等待线程结束的时候，也就得到了线程的返回值。
<a name="hrbLD"></a>
### **方法三：使用标准库 **`**concurrent.futures**`
前两种方式实在太低级了，<br />Python 的标准库`concurrent.futures` 提供更高级的线程操作，可以直接获取线程的返回值，相当优雅，代码如下：
```python
import concurrent.futures


def foo(bar):
    return bar


with concurrent.futures.ThreadPoolExecutor(max_workers=10) as executor:
    to_do = []
    for i in range(10):  # 模拟多个任务
        future = executor.submit(foo, f"hello world! {i}")
        to_do.append(future)

    for future in concurrent.futures.as_completed(to_do):  # 并发执行
        print(future.result())
```
某次运行的结果如下：
```python
hello world! 8
hello world! 3
hello world! 5
hello world! 2
hello world! 9
hello world! 7
hello world! 4
hello world! 0
hello world! 1
hello world! 6
```
