Python 线程锁
<a name="s4wiL"></a>
## 线程安全
线程安全是多线程或多进程编程中的一个概念，在拥有共享数据的多条线程并行执行的程序中，线程安全的代码会通过同步机制保证各个线程都可以正常且正确的执行，不会出现数据污染等意外情况。<br />线程安全的问题最主要还是由线程切换导致的，比如一个房间（进程）中有10颗糖（资源），除此之外还有3个小人（1个主线程、2个子线程），当小人A吃了3颗糖后被系统强制进行休息时他认为还剩下7颗糖，而当小人B工作后又吃掉了3颗糖，那么当小人A重新上岗时会认为糖还剩下7颗，但是实际上只有4颗了。<br />上述例子中线程A和线程B的数据不同步，这就是线程安全问题，它可能导致非常严重的意外情况发生，按下面这个示例来进行说明。<br />下面有一个数值num初始值为0，开启2条线程：

- 线程1对num进行一千万次+1的操作
- 线程2对num进行一千万次-1的操作

结果可能会令人咋舌，num最后并不是所想象的结果0：
```python
import threading

num = 0


def add():
    global num
    for i in range(10_000_000):
        num += 1


def sub():
    global num
    for i in range(10_000_000):
        num -= 1


if __name__ == "__main__":
    subThread01 = threading.Thread(target=add)
    subThread02 = threading.Thread(target=sub)

    subThread01.start()
    subThread02.start()

    subThread01.join()
    subThread02.join()

    print("num result : %s" % num)

# 结果三次采集
# num result : 669214
# num result : -1849179
# num result : -525674
```
上面这就是一个非常好的案例，想要解决这个问题就必须通过锁来保障线程切换的时机。<br />需要值得留意的是，在Python基本数据类型中list、tuple、dict本身就是属于线程安全的，所以如果有多个线程对这3种容器做操作时，不必考虑线程安全问题。
<a name="NaJnc"></a>
## 锁的作用
锁是Python提供给开发者能够自行操控线程切换的一种手段，使用锁可以让线程的切换变的有序。<br />一旦线程的切换变的有序后，各个线程之间对数据的访问、修改就变的可控，所以若要保证线程安全，就必须使用锁。<br />threading模块中提供了5种最常见的锁，下面是按照功能进行划分：

- 同步锁：`lock`（一次只能放行一个）
- 递归锁：`rlock`（一次只能放行一个）
- 条件锁：`condition`（一次可以放行任意个）
- 事件锁：`event`（一次全部放行）
- 信号量锁：`semaphore`（一次可以放行特定个）
<a name="ru559"></a>
## 1、`Lock()` 同步锁
<a name="ViwGm"></a>
### 基本介绍
`Lock`锁的称呼有很多，如：

- 同步锁
- 互斥锁

它们是什么意思呢？如下所示：

1. 互斥指的是某一资源同一时刻仅能有一个访问者对其进行访问，具有唯一性和排他性，但是互斥无法限制访问者对资源的访问顺序，即访问是无序的
2. 同步是指在互斥的基础上（大多数情况），通过其他机制实现访问者对资源的有序访问
3. 同步其实已经实现了互斥，是互斥的一种更为复杂的实现，因为它在互斥的基础上实现了有序访问的特点

下面是threading模块与同步锁提供的相关方法：

| 方法 | 描述 |
| --- | --- |
| `threading.Lock()` | 返回一个同步锁对象 |
| `lockObject.acquire(blocking=True, timeout=1)` | 上锁，当一个线程在执行被上锁代码块时，将不允许切换到其他线程运行，默认锁失效时间为1秒 |
| `lockObject.release()` | 解锁，当一个线程在执行未被上锁代码块时，将允许系统根据策略自行切换到其他线程中运行 |
| `lockObject.locaked()` | 判断该锁对象是否处于上锁状态，返回一个布尔值 |

<a name="QkU8f"></a>
### 使用方式
同步锁一次只能放行一个线程，一个被加锁的线程在运行时不会将执行权交出去，只有当该线程被解锁时才会将执行权通过系统调度交由其他线程。<br />如下所示，使用同步锁解决最上面的问题：
```python
import threading

num = 0


def add():
    lock.acquire()
    global num
    for i in range(10_000_000):
        num += 1
    lock.release()


def sub():
    lock.acquire()
    global num
    for i in range(10_000_000):
        num -= 1
    lock.release()

if __name__ == "__main__":
    lock = threading.Lock()

    subThread01 = threading.Thread(target=add)
    subThread02 = threading.Thread(target=sub)

    subThread01.start()
    subThread02.start()

    subThread01.join()
    subThread02.join()

    print("num result : %s" % num)

# 结果三次采集
# num result : 0
# num result : 0
# num result : 0
```
这样这个代码就完全变成了串行的状态，对于这种计算密集型I/O业务来说，还不如直接使用串行化单线程执行来得快，所以这个例子仅作为一个示例，不能概述锁真正的用途。
<a name="ZQpRF"></a>
### 死锁现象
对于同步锁来说，一次`acquire()`必须对应一次`release()`，不能出现连续重复使用多次`acquire()`后再重复使用多次`release()`的操作，这样会引起死锁造成程序的阻塞，完全不动了，如下所示：
```python
import threading

num = 0


def add():
    lock.acquire()  # 上锁
    lock.acquire()  # 死锁
    # 不执行
    global num
    for i in range(10_000_000):
        num += 1
    lock.release()
    lock.release()


def sub():
    lock.acquire()  # 上锁
    lock.acquire()  # 死锁
    # 不执行
    global num
    for i in range(10_000_000):
        num -= 1
    lock.release()
    lock.release()


if __name__ == "__main__":
    lock = threading.Lock()

    subThread01 = threading.Thread(target=add)
    subThread02 = threading.Thread(target=sub)

    subThread01.start()
    subThread02.start()

    subThread01.join()
    subThread02.join()

    print("num result : %s" % num)
```
<a name="eyUap"></a>
### `with`语句
由于`threading.Lock()`对象中实现了`__enter__()`与`__exit__()`方法，故可以使用`with`语句进行上下文管理形式的加锁解锁操作：
```python
import threading

num = 0


def add():
    with lock:
        # 自动加锁
        global num
        for i in range(10_000_000):
            num += 1
        # 自动解锁


def sub():
    with lock:
        # 自动加锁
        global num
        for i in range(10_000_000):
            num -= 1
        # 自动解锁


if __name__ == "__main__":
    lock = threading.Lock()

    subThread01 = threading.Thread(target=add)
    subThread02 = threading.Thread(target=sub)

    subThread01.start()
    subThread02.start()

    subThread01.join()
    subThread02.join()

    print("num result : %s" % num)
    
# 结果三次采集
# num result : 0
# num result : 0
# num result : 0
```
<a name="KGvKR"></a>
## 2、`RLock()` 递归锁
<a name="Uu4ZH"></a>
### 基本介绍
递归锁是同步锁的一个升级版本，在同步锁的基础上可以做到连续重复使用多次`acquire()`后再重复使用多次`release()`的操作，但是一定要注意加锁次数和解锁次数必须一致，否则也将引发死锁现象。<br />下面是threading模块与递归锁提供的相关方法：

| 方法 | 描述 |
| --- | --- |
| `threading.RLock()` | 返回一个递归锁对象 |
| `lockObject.acquire(blocking=True, timeout=1)` | 上锁，当一个线程在执行被上锁代码块时，将不允许切换到其他线程运行，默认锁失效时间为1秒 |
| `lockObject.release()` | 解锁，当一个线程在执行未被上锁代码块时，将允许系统根据策略自行切换到其他线程中运行 |
| `lockObject.locaked()` | 判断该锁对象是否处于上锁状态，返回一个布尔值 |

<a name="GOkRm"></a>
### 使用方式
以下是递归锁的简单使用，下面这段操作如果使用同步锁则会发生死锁现象，但是递归锁不会：
```python
import threading

num = 0


def add():
    lock.acquire()
    lock.acquire()
    global num
    for i in range(10_000_000):
        num += 1
    lock.release()
    lock.release()


def sub():
    lock.acquire()
    lock.acquire()
    global num
    for i in range(10_000_000):
        num -= 1
    lock.release()
    lock.release()


if __name__ == "__main__":
    lock = threading.RLock()

    subThread01 = threading.Thread(target=add)
    subThread02 = threading.Thread(target=sub)

    subThread01.start()
    subThread02.start()

    subThread01.join()
    subThread02.join()

    print("num result : %s" % num)

# 结果三次采集
# num result : 0
# num result : 0
# num result : 0

```
<a name="sgBt2"></a>
### `with`语句
由于`threading.RLock()`对象中实现了`__enter__()`与`__exit__()`方法，故可以使用`with`语句进行上下文管理形式的加锁解锁操作：
```python
import threading

num = 0


def add():
    with lock:
        # 自动加锁
        global num
        for i in range(10_000_000):
            num += 1
        # 自动解锁


def sub():
    with lock:
        # 自动加锁
        global num
        for i in range(10_000_000):
            num -= 1
        # 自动解锁


if __name__ == "__main__":
    lock = threading.RLock()

    subThread01 = threading.Thread(target=add)
    subThread02 = threading.Thread(target=sub)

    subThread01.start()
    subThread02.start()

    subThread01.join()
    subThread02.join()

    print("num result : %s" % num)

# 结果三次采集
# num result : 0
# num result : 0
# num result : 0
```
<a name="bGErf"></a>
## 3、`Condition()` 条件锁
<a name="D2U46"></a>
### 基本介绍
条件锁是在递归锁的基础上增加了能够暂停线程运行的功能。并且可以使用`wait()`与`notify()`来控制线程执行的个数。<br />注意：条件锁可以自由设定一次放行几个线程。<br />下面是`threading`模块与条件锁提供的相关方法：

| 方法 | 描述 |
| --- | --- |
| `threading.Condition()` | 返回一个条件锁对象 |
| `lockObject.acquire(blocking=True, timeout=1)` | 上锁，当一个线程在执行被上锁代码块时，将不允许切换到其他线程运行，默认锁失效时间为1秒 |
| `lockObject.release()` | 解锁，当一个线程在执行未被上锁代码块时，将允许系统根据策略自行切换到其他线程中运行 |
| `lockObject.wait(timeout=None)` | 将当前线程设置为“等待”状态，只有该线程接到“通知”或者超时时间到期之后才会继续运行，在“等待”状态下的线程将允许系统根据策略自行切换到其他线程中运行 |
| `lockObject.wait_for(predicate, timeout=None)` | 将当前线程设置为“等待”状态，只有该线程的predicate返回一个True或者超时时间到期之后才会继续运行，在“等待”状态下的线程将允许系统根据策略自行切换到其他线程中运行。注意：predicate参数应当传入一个可调用对象，且返回结果为bool类型 |
| `lockObject.notify(n=1)` | 通知一个当前状态为“等待”的线程继续运行，也可以通过参数n通知多个 |
| `lockObject.notify_all()` | 通知所有当前状态为“等待”的线程继续运行 |

<a name="vapjz"></a>
### 使用方式
下面这个案例会启动10个子线程，并且会立即将10个子线程设置为等待状态。<br />然后可以发送一个或者多个通知，来恢复被等待的子线程继续运行：
```python
import threading

currentRunThreadNumber = 0
maxSubThreadNumber = 10


def task():
    global currentRunThreadNumber
    thName = threading.currentThread().name

    condLock.acquire()  # 上锁
    print("start and wait run thread : %s" % thName)

    condLock.wait()  # 暂停线程运行、等待唤醒
    currentRunThreadNumber += 1
    print("carry on run thread : %s" % thName)

    condLock.release()  # 解锁


if __name__ == "__main__":
    condLock = threading.Condition()

    for i in range(maxSubThreadNumber):
        subThreadIns = threading.Thread(target=task)
        subThreadIns.start()

    while currentRunThreadNumber < maxSubThreadNumber:
        notifyNumber = int(
            input("Please enter the number of threads that need to be notified to run："))

        condLock.acquire()
        condLock.notify(notifyNumber)  # 放行
        condLock.release()

    print("main thread run end")
    
# 先启动10个子线程，然后这些子线程会全部变为等待状态
# start and wait run thread : Thread-1
# start and wait run thread : Thread-2
# start and wait run thread : Thread-3
# start and wait run thread : Thread-4
# start and wait run thread : Thread-5
# start and wait run thread : Thread-6
# start and wait run thread : Thread-7
# start and wait run thread : Thread-8
# start and wait run thread : Thread-9
# start and wait run thread : Thread-10

# 批量发送通知，放行特定数量的子线程继续运行
# Please enter the number of threads that need to be notified to run：5  # 放行5个
# carry on run thread : Thread-4
# carry on run thread : Thread-3
# carry on run thread : Thread-1
# carry on run thread : Thread-2
# carry on run thread : Thread-5

# Please enter the number of threads that need to be notified to run：5  # 放行5个
# carry on run thread : Thread-8
# carry on run thread : Thread-10
# carry on run thread : Thread-6
# carry on run thread : Thread-9
# carry on run thread : Thread-7

# Please enter the number of threads that need to be notified to run：1
# main thread run end
```
<a name="FnCxR"></a>
### `with`语句
由于`threading.Condition()`对象中实现了`__enter__()`与`__exit__()`方法，故可以使用`with`语句进行上下文管理形式的加锁解锁操作：
```python
import threading

currentRunThreadNumber = 0
maxSubThreadNumber = 10


def task():
    global currentRunThreadNumber
    thName = threading.currentThread().name

    with condLock:
        print("start and wait run thread : %s" % thName)
        condLock.wait()  # 暂停线程运行、等待唤醒
        currentRunThreadNumber += 1
        print("carry on run thread : %s" % thName)


if __name__ == "__main__":
    condLock = threading.Condition()

    for i in range(maxSubThreadNumber):
        subThreadIns = threading.Thread(target=task)
        subThreadIns.start()

    while currentRunThreadNumber < maxSubThreadNumber:
        notifyNumber = int(
            input("Please enter the number of threads that need to be notified to run："))

        with condLock:
            condLock.notify(notifyNumber)  # 放行

    print("main thread run end")
```
<a name="BLzAP"></a>
## 4、`Event()` 事件锁
<a name="P5hxi"></a>
### 基本介绍
事件锁是基于条件锁来做的，它与条件锁的区别在于一次只能放行全部，不能放行任意个数量的子线程继续运行。<br />可以将事件锁看为红绿灯，当红灯时所有子线程都暂停运行，并进入“等待”状态，当绿灯时所有子线程都恢复“运行”。<br />下面是threading模块与事件锁提供的相关方法：

| 方法 | 描述 |
| --- | --- |
| `threading.Event()` | 返回一个事件锁对象 |
| `lockObject.clear()` | 将事件锁设为红灯状态，即所有线程暂停运行 |
| `lockObject.is_set()` | 用来判断当前事件锁状态，红灯为False，绿灯为True |
| `lockObject.set()` | 将事件锁设为绿灯状态，即所有线程恢复运行 |
| `lockObject.wait(timeout=None)` | 将当前线程设置为“等待”状态，只有该线程接到“绿灯通知”或者超时时间到期之后才会继续运行，在“等待”状态下的线程将允许系统根据策略自行切换到其他线程中运行 |

<a name="KtwUG"></a>
### 使用方式
事件锁不能利用`with`语句来进行使用，只能按照常规方式。<br />如下所示，来模拟线程和红绿灯的操作，红灯停，绿灯行：
```python
import threading

maxSubThreadNumber = 3


def task():
    thName = threading.currentThread().name
    print("start and wait run thread : %s" % thName)
    eventLock.wait()  # 暂停运行，等待绿灯
    print("green light, %s carry on run" % thName)
    print("red light, %s stop run" % thName)
    eventLock.wait()  # 暂停运行，等待绿灯
    print("green light, %s carry on run" % thName)
    print("sub thread %s run end" % thName)


if __name__ == "__main__":

    eventLock = threading.Event()

    for i in range(maxSubThreadNumber):
        subThreadIns = threading.Thread(target=task)
        subThreadIns.start()

    eventLock.set()  # 设置为绿灯
    eventLock.clear()  # 设置为红灯
    eventLock.set()  # 设置为绿灯

# start and wait run thread : Thread-1
# start and wait run thread : Thread-2
# start and wait run thread : Thread-3

# green light, Thread-1 carry on run
# red light, Thread-1 stop run
# green light, Thread-1 carry on run
# sub thread Thread-1 run end

# green light, Thread-3 carry on run
# red light, Thread-3 stop run
# green light, Thread-3 carry on run
# sub thread Thread-3 run end

# green light, Thread-2 carry on run
# red light, Thread-2 stop run
# green light, Thread-2 carry on run
# sub thread Thread-2 run end
```
<a name="Vhy6N"></a>
## 5、`Semaphore()` 信号量锁
<a name="qwcqY"></a>
### 基本介绍
信号量锁也是根据条件锁来做的，它与条件锁和事件锁的区别如下：

- 条件锁：一次可以放行任意个处于“等待”状态的线程
- 事件锁：一次可以放行全部的处于“等待”状态的线程
- 信号量锁：通过规定，成批的放行特定个处于“上锁”状态的线程

下面是threading模块与信号量锁提供的相关方法：

| 方法 | 描述 |
| --- | --- |
| `threading.Semaphore()` | 返回一个信号量锁对象 |
| `lockObject.acquire(blocking=True, timeout=1)` | 上锁，当一个线程在执行被上锁代码块时，将不允许切换到其他线程运行，默认锁失效时间为1秒 |
| `lockObject.release()` | 解锁，当一个线程在执行未被上锁代码块时，将允许系统根据策略自行切换到其他线程中运行 |

<a name="c3Vyh"></a>
### 使用方式
以下是使用示例，可以将它当做一段限宽的路段，每次只能放行相同数量的线程：
```python
import threading
import time

maxSubThreadNumber = 6


def task():
    thName = threading.currentThread().name
    semaLock.acquire()
    print("run sub thread %s" % thName)
    time.sleep(3)
    semaLock.release()


if __name__ == "__main__":
    # 每次只能放行2个
    semaLock = threading.Semaphore(2)

    for i in range(maxSubThreadNumber):
        subThreadIns = threading.Thread(target=task)
        subThreadIns.start()


# run sub thread Thread-1
# run sub thread Thread-2

# run sub thread Thread-3
# run sub thread Thread-4

# run sub thread Thread-6
# run sub thread Thread-5
```
<a name="rjsaj"></a>
### `with`语句
由于`threading.Semaphore()`对象中实现了`__enter__()`与`__exit__()`方法，故可以使用`with`语句进行上下文管理形式的加锁解锁操作：
```python
import threading
import time

maxSubThreadNumber = 6


def task():
    thName = threading.currentThread().name
    with semaLock:
        print("run sub thread %s" % thName)
        time.sleep(3)


if __name__ == "__main__":

    semaLock = threading.Semaphore(2)

    for i in range(maxSubThreadNumber):
        subThreadIns = threading.Thread(target=task)
        subThreadIns.start()
```
<a name="HFwAc"></a>
## 锁关系浅析
上面5种锁可以说都是基于同步锁来做的，这些都可以从源码中找到答案。<br />首先来看RLock递归锁，递归锁的实现非常简单，它的内部会维护着一个计数器，当计数器不为0的时候该线程不能被I/O操作和时间轮询机制切换。但是当计数器为0的时候便不会如此了：
```python
def __init__(self):
    self._block = _allocate_lock()
    self._owner = None
    self._count = 0  # 计数器
```
而`Condition`条件锁的内部其实是有两把锁的，一把底层锁（同步锁）一把高级锁(递归锁)。<br />低层锁的解锁方式有两种，使用`wait()`方法会暂时解开底层锁同时加上一把高级锁，只有当接收到别的线程里的`notfiy()`后才会解开高级锁和重新上锁低层锁，也就是说条件锁底层是根据同步锁和递归锁的不断切换来进行实现的：
```python
def __init__(self, lock=None):
    if lock is None:
        lock = RLock()  # 可以看到条件锁的内部是基于递归锁，而递归锁又是基于同步锁来做的
    self._lock = lock

    self.acquire = lock.acquire
    self.release = lock.release
    try:
        self._release_save = lock._release_save
    except AttributeError:
        pass
    try:
        self._acquire_restore = lock._acquire_restore
    except AttributeError:
        pass
    try:
        self._is_owned = lock._is_owned
    except AttributeError:
        pass
    self._waiters = _deque()


Event事件锁内部是基于条件锁来做的：
class Event:

    def __init__(self):
        self._cond = Condition(Lock())  # 实例化出了一个条件锁。
        self._flag = False

    def _reset_internal_locks(self):
        # private!  called by Thread._reset_internal_locks by _after_fork()
        self._cond.__init__(Lock())

    def is_set(self):
        """Return true if and only if the internal flag is true."""
        return self._flag

    isSet = is_set


Semaphore信号量锁内部也是基于条件锁来做的：
class Semaphore:

    def __init__(self, value=1):
        if value < 0:
            raise ValueError("semaphore initial value must be >= 0")
        self._cond = Condition(Lock()) # 可以看到，这里是实例化出了一个条件锁
        self._value = value
```
<a name="sMmL5"></a>
## 基本练习题
<a name="JjQjE"></a>
### 条件锁的应用
需求：一个空列表，两个线程轮番往里面加值（一个加偶数，一个加奇数），最终让该列表中的值为 1 - 100 ，且是有序排列的。
```python
import threading

lst = []


def even():
    """加偶数"""
    with condLock:
        for i in range(2, 101, 2):
            # 判断当前列表的长度处于2是否能处尽
            # 如果能处尽则代表需要添加奇数
            # 否则就添加偶数
            if len(lst) % 2 != 0:
                # 添偶数
                lst.append(i)      # 先添加值
                condLock.notify()  # 告诉另一个线程，你可以加奇数了，但是这里不会立即交出执行权
                condLock.wait()    # 交出执行权，并等待另一个线程通知加偶数
            else:
                # 添奇数
                condLock.wait()  # 交出执行权，等待另一个线程通知加偶数
                lst.append(i)    
                condLock.notify()
        condLock.notify()


def odd():
    """加奇数"""
    with condLock:
        for i in range(1, 101, 2):
            if len(lst) % 2 == 0:
                lst.append(i)
                condLock.notify()
                condLock.wait()
        condLock.notify()


if __name__ == "__main__":
    condLock = threading.Condition()

    addEvenTask = threading.Thread(target=even)
    addOddTask = threading.Thread(target=odd)

    addEvenTask.start()
    addOddTask.start()

    addEvenTask.join()
    addOddTask.join()

    print(lst)
```
<a name="flCkk"></a>
### 事件锁的应用
有2个任务线程来扮演李白和杜甫，如何让他们一人一句进行对答？文本如下：
> 杜甫：老李啊，来喝酒！
> 李白：老杜啊，不喝了我喝不下了！
> 杜甫：老李啊，再来一壶？
> 杜甫：...老李？
> 李白：呼呼呼...睡着了..

代码如下：
```python
import threading


def libai():
    event.wait()  
    print("李白：老杜啊，不喝了我喝不下了！")
    event.set()
    event.clear()
    event.wait()
    print("李白：呼呼呼...睡着了..")

def dufu():
    print("杜甫：老李啊，来喝酒！")
    event.set()  
    event.clear()
    event.wait()
    print("杜甫：老李啊，再来一壶？")
    print("杜甫：...老李？")
    event.set()


if __name__ == '__main__':

    event = threading.Event()

    t1 = threading.Thread(target=libai)
    t2 = threading.Thread(target=dufu)

    t1.start()
    t2.start()
    t1.join()
    t2.join()
```
