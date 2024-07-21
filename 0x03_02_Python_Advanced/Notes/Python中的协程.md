Python 协程<br />说到并发，肯定想到了多线程+进程模型，确实，多线程+进程，正是解决并发问题的经典模型之一。但对于多核CPU，利用多进程+协程的方式，能充分利用CPU，获得极高的性能。协程也是实现并发编程的一种方式。
<a name="IHJva"></a>
## 协程
协程：是单线程下的并发，又称微线程。英文名是Coroutine。它和线程一样可以调度，但是不同的是线程的启动和调度需要通过操作系统来处理。<br />协程是一种比线程更加轻量级的存在，最重要的是，协程不被操作系统内核管理，协程是完全由程序控制的。<br />运行效率极高，协程的切换完全由程序控制，不像线程切换需要花费操作系统的开销,线程数量越多，协程的优势就越明显。<br />协程不需要多线程的锁机制，因为只有一个线程，不存在变量冲突。<br />对于多核CPU，利用多进程+协程的方式，能充分利用CPU，获得极高的性能。<br />注意协程这个概念完全是程序员自己想出来的东西，它对于操作系统来说根本不存在。操作系统只有进程和线程。
<a name="yDeRA"></a>
## Python中使用协程的例子
`yield`关键字相当于是暂停功能，程序运行到`yield`停止，`send`函数可以传参给生成器函数，参数赋值给`yield`。
```python
def customer():
    while True:
        number = yield
        print('开始消费：',number)
        
custom = customer()
next(custom)
for i in range(5):
    print('开始生产：',i)
    custom.send(i)
```
结果如下
```python
开始生产： 0
开始消费： 0
开始生产：1
开始消费：1
开始生产：2
开始消费：2
开始生产：3
开始消费：3
开始生产：4
开始消费：4
```
代码解析：

1. 协程使用生成器函数定义：定义体中有 `yield` 关键字。
2. `yield` 在表达式中使用；如果协程只需从客户custom接收数据，如果没有产出的值，那么产出的值是 None。
3. 首先要调用 `next(…)` 函数，因为生成器还没启动，没在 `yield` 语句处暂停，所以一开始无法发送数据。
4. 调用`send`方法，把值传给 `yield` 的变量，然后协程恢复，继续执行下面的代码，直到运行到下一个 `yield` 表达式，或者终止。
<a name="sS5bm"></a>
## `async`和`await`
`async`和`await`是原生协程，是Python3.5以后引入的两个关键词。<br />`async`是“异步”的简写，而`await`可以认为是`async` `wait`的简写。所以应该很好理解`async`用于申明一个function是异步的，而`await`用于等待一个异步方法执行完成。<br />下面，从一个demo示例看起，具体代码如下。
```python
import time 

def print_num(num):
    print("Maoli is printing " + str(num) + " nows" )
    time.sleep(1)
    print("Maoli prints" + str(num) + " OK")

def main(nums):
    for num in nums:
        print_num(num)
%time main([i for i in range(1,6)])
```
```python
Maoli is printing 1 nows
Maoli prints1 OK
Maoli is printing 2 nows
Maoli prints2 OK
Maoli is printing 3 nows
Maoli prints3 OK
Maoli is printing 4 nows
Maoli prints4 OK
Maoli is printing 5 nows
Maoli prints5 OK
Wall time: 5 s
```
`%time` 需要在jupyter notebook中运行，这是jupyter的语法糖。<br />上面代码是从上到下执行的。下面将上面的代码改成单线程协程版本。<br />**「注意py版本3.7以上」**，主要使用的是`asyncio`模块，如果出现AttributeError: module ‘asyncio‘ has no attribute ‘run‘报错，这是`asyncio`版本不兼容的原因，需要将Python版本提升至3.7以上。
```python
import asyncio

async def print_num(num):
    print("Maoli is printing " + str(num) + " nows" )
    await asyncio.sleep(1)
    print("Maoli prints" + str(num) + " OK")

async def main(nums):
    for num in nums:
        await print_num(num)
%time asyncio.run(main([i for i in range(1,6)]))
```
```python
Maoli is printing 1 nows
Maoli prints1 OK
Maoli is printing 2 nows
Maoli prints2 OK
Maoli is printing 3 nows
Maoli prints3 OK
Maoli is printing 4 nows
Maoli prints4 OK
Maoli is printing 5 nows
Maoli prints5 OK
Wall time: 5.01 s
```
`asyncio.run()` 函数用来运行最高层级的入口点 "`main()`" 函数。`await` 是同步调用等待一个协程。以下代码段会在等待 1 秒后打印 `num`，但在运行速度上没有发生改变。这里需要引入`asyncio.create_task`可等待对象才可以。
<a name="BXKNn"></a>
## `create_task`
如果一个对象可以在`await`语句中使用，那么它就是可等待对象。<br />协程中的还一个重要概念，任务（Task）。<br />如果写一个数字是一个任务，那么要完成5个任务。<br />写个1-5都这么慢，不行，要加速写。<br />`asyncio.create_task()`函数用来并发运行作为`asyncio`任务 的多个协程。
```python
import asyncio

async def print_num(num):
    print("Maoli is printing " + str(num) + " nows" )
    await asyncio.sleep(1)
    print("Maoli prints" + str(num) + " OK")

async def main(nums):
    tasks = [asyncio.create_task(print_num(num)) for num in nums]
    for task in tasks:
        await task
%time asyncio.run(main([i for i in range(1,6)]))
```
```python
Maoli is printing 1 nows
Maoli is printing 2 nows
Maoli is printing 3 nows
Maoli is printing 4 nows
Maoli is printing 5 nows
Maoli prints1 OK
Maoli prints3 OK
Maoli prints5 OK
Maoli prints2 OK
Maoli prints4 OK
Wall time: 1.01 s
```
还可以写成`await asyncio.gather(*tasks)`这种方法
```python
import asyncio

async def print_num(num):
    print("Maoli is printing " + str(num) + " nows" )
    await asyncio.sleep(1)
    print("Maoli prints" + str(num) + " OK")

async def main(nums):
    tasks = [asyncio.create_task(print_num(num)) for num in nums]
    await asyncio.gather(*tasks)
%time asyncio.run(main([i for i in range(1,6)]))
```
`*tasks`解包列表，将列表变成了函数的参数；与之对应的是， `**dict`将字典变成了函数的参数。<br />协程的写法简洁清晰，只要把 `async` / `await` 语法和 `create_task` 结合来用，就是Python中比较常见的协程写法。
