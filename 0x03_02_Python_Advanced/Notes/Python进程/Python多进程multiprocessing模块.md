Python multiprocessing<br />`threading`包为 Python 提供了线程模型，而`multiprocessing`包则为另一种并发模型 — 多进程模型提供了强大的解决方案。
<a name="tyHtU"></a>
# `multiprocessing`
`multiprocessing`包是Python中的多进程管理包。与之前的`threading.Thread`类似，它可以利用`multiprocessing.Process`对象来创建一个进程。<br />`multiprocessing`的模块的API非常简单直观，可以让新手迅速上手在多个进程之间划分工作。现在看一个官方简单的例子：
```python
# importing the multiprocessing module 
import multiprocessing 

def print_cube(num): 
    print("Cube: {}".format(num * num * num)) 

def print_square(num): 
    print("Square: {}".format(num * num)) 

if __name__ == "__main__": 
    # creating processes 
    p1 = multiprocessing.Process(target=print_square, args=(10, )) 
    p2 = multiprocessing.Process(target=print_cube, args=(10, )) 

    # starting process 1&2
    p1.start() 
    p2.start() 

    # wait until process 1&2 is finished 
    p1.join() 
    p2.join() 

    # both processes finished 
    print("Done!") 
```
运行结果是这样的：
```python
Square: 100
Cube: 1000
Done!
```
上面代码中导入了`multiprocessing`模块，随后定义了两个函数，它们的功能分别是打印一个数的三次方和打印一个数的平方。<br />之后关键的步骤来了，要创建多个进程，首先需要创建`Process`类的对象。在这个例子中`Process`类接收了两个参数：

- target：在进程中被执行的函数
- args：向被执行函数传递的参数
<a name="PBkIV"></a>
# 线程池
ThreadPool类提供一个线程池，该线程池可用于发送工作项、处理异步 I/O、代表其他线程等待以及处理计时器。<br />通常可导入模块：`from multiprocessing import Pool`。<br />创建线程池：`pool = Pool(10)`<br />官方提供了比较常见的三种使用线程池的方法。分别是`map`，`apply`和`apply_async`。
<a name="hQYcP"></a>
# Map
其中`map`规定线程池执行的任务：`result = pool.map(func,datalist)`<br />`func`为所要执行的函数，`datalist`为参数列表，线程池也会依次在参数列表中提取参数带入函数中来执行函数，参数列表的长度也就是线程池所要执行的任务数量。
```python
# 进程池即创建一个进程库，事先设置好需要多少进程，从进程库中提取这些已创建的进程即可。
# Pool.map()
import multiprocessing

def test(i):
    print(i)

if __name__ == '__main__':
    list1 = list(range(5))  # 创建一个列表，map方法会用到
    pool1 = multiprocessing.Pool(processes=4)  # 创建一个进程池，这里创建了含有4个进程的进程池
    pool1.map(test, list1)
    '''
    对pool1使用map方法，在这个例子中即形成（test(0),test(1),test(2),test(3),test(4) 这些任务）
    这些将执行的任务是按顺序执行的，也就是说进程1执行test(0)，进程2执行test(1)，进程3执行test(2)，
    进程4执行test(3)，还多出一个任务则需要等待，等到某个进程提前结束了就再执行这个任务。
    这样的话相当于一个进程执行一次单独的任务，十分方便某些任务
    如果函数有返回值即return，还可以使用result = pool1.map(test,list1)获取每个进程的返回值，
    但是这里是所有返回值在一起，需要自己对其按需处理。
    '''
    pool1.close()  # close方法用于关闭进程池，即恢复到没有子进程的情况
    pool1.join()
```
除了`map`方法，还有`apply`和`apply_async`方法用来执行进程，允许多个进程同时进入池子。
<a name="Mey5p"></a>
# apply
在`multiprocessing`模块中，`apply`阻塞主进程, 并且一个一个按顺序地执行子进程, 等到全部子进程都执行完毕后 ,继续执行`apply()`后面主进程的代码。
```python
import time
import multiprocessing

def doIt(num):
    print("Process num is : %s" % num)
    time.sleep(1)
    print('process  %s end' % num)

if __name__ == '__main__':
    print('mainProcess start')
    # 记录一下开始执行的时间
    start_time = time.time()
    # 创建三个子进程
    pool = multiprocessing.Pool(3)
    print('Child start')
    for i in range(3):
        pool.apply(doIt, [i])
    print('mainProcess done time:%s s' % (time.time() - start_time))
    pool.close()
    pool.join()
```
输出如下所示
```python
mainProcess start
Child start
Process num is : 0
process  0 end
Process num is : 1
process  1 end
Process num is : 2
process  2 end
mainProcess done time:3.7142281532287598 s
```
执行结果可以看到, 主进程开始执行之后, 创建的三个子进程也随即开始执行, 主进程被阻塞, 而且接下来三个子进程是一个接一个按顺序地执行, 等到子进程全部执行完毕之后, 主进程就会继续执行, 打印出最后一句。
<a name="tiHYe"></a>
# apply_async
接下来是使用`apply_async()`, 只需要把上面的代码使用`apply()`的地方改成`apply_async()`即可, 代码如下。
```python
import time
import multiprocessing

def doIt(num):
    print("Process num is : %s" % num)
    time.sleep(1)
    print('process  %s end' % num)

if __name__ == '__main__':
    print('mainProcess start')
    # 记录一下开始执行的时间
    start_time = time.time()
    # 创建三个子进程
    pool = multiprocessing.Pool(3)
    print('Child start')
    for i in range(3):
        pool.apply_async(doIt, [i])
    print('mainProcess done time:%s s' % (time.time() - start_time))
    pool.close()
    pool.join()
```
输出如下所示
```python
mainProcess start
Child start
mainProcess done time:0.060953378677368164 s
Process num is : 0
Process num is : 1
Process num is : 2
process  0 end
process  1 end
process  2 end
```
`apply_async()`非阻塞异步的, 他不会等待子进程执行完毕, 主进程会继续执行, 他会根据系统调度来进行进程切换。<br />CPU在执行第一个子进程的时候, 还没等第一个子进程结束, 系统调度到了按顺序调度到了第二个子进程, 以此类推, 一直调度运行子进程, 一个接一个地结束子进程的运行, 最后运行主进程, 而且可以看到使用`apply_async()`的执行效力会更高, 看一下他们各自执行结果最后一句的执行消耗时间就知道了, 这也是官方推荐使用`apply_async()`的主要原因吧
