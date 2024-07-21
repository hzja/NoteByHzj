Pyhton Python
<a name="o7pSF"></a>
## Event(事件)
Event 是一个事务处理的机制，用于通知多个异步任务某个事件已经发生了。比如在交通红绿灯中多辆在行驶中的汽车可以表示成程序中的多个异步任务，红绿灯的亮起可以表示成一个通知，红灯通知这个汽车的行驶任务必须停止了，绿灯通知这个汽车的行驶任务可以继续驾驶了。<br />Event 管理着一个全局的内部标志 flag，该 flag 默认为 False，当 flag 等于 False 的时候，调用 `wait()` 函数后程序就会被阻塞，只有当 flag 等于 True 的时候程序才会继续执行。在红绿灯中红灯相当于 flag 等于 False 的时候调用了 `wait()` 函数，绿灯相当于 flag 被设置为了 True，汽车允许通行，程序被允许执行 `wait()`之后的代码。
<a name="q62jk"></a>
### Event事件对象
Event 通过 threading 模块的 `Event()` 类创建对象
```python
event = threading.Event()
```
Event 是一个低配版的 `Condition`，在源代码 init 函数中创建了 Condition 对象，并将 flag 设置为 False
```python
def __init__(self):
    self._cond = Condition(Lock())
    self._flag = False
```
<a name="IVIXt"></a>
### 函数
Event 事件中只有4个函数：

1. `set()`: 将 flag 设为 True，通知所有处于阻塞状态的线程恢复运行状态。
2. `clear()`: 将 flag 设为 False。
3. `wait(timeout)`: 如果 flag 为 True 将立即返回，否则线程将处于阻塞状态，等待其他线程将 flag 设置为 True
4. `isSet()`: 获取 flag 的状态，返回 True 或 False。
<a name="txjm6"></a>
### 实例1
用行驶中的汽车碰到红绿灯做一个实例，红灯事件汽车停止不允许通行处于等待阻塞状态，绿灯事件汽车运行通行处于恢复运行状态。
```python
import threading
import time

event = threading.Event()


def drive(name):
    i = 0
    while True:
        i = i + 1
        print(name + "正在行驶中，行驶了" + str(i * 60) + "Km")
        time.sleep(1)
        event.wait()
        print(name + "通过了红灯")


def sign():
    print("绿灯初始化")
    event.set()
    while True:
        # 红绿灯切换时间为3秒
        time.sleep(3)
        if event.isSet():
            print("红灯亮起，所有行驶中的车辆不允许通过")
            event.clear()
        else:
            print("绿灯亮起，所有行驶中的车辆必须通过")
            event.set()


if __name__ == '__main__':

    # 设置公路线程组
    highwayThreads = []

    # 创建汽车新线程
    bmwCar = threading.Thread(target = drive, args = ("BMWCar", ))
    vwCar = threading.Thread(target = drive, args = ("VWCar", ))

    # 将汽车线程添加到公路线程组
    highwayThreads.append(bmwCar)
    highwayThreads.append(vwCar)

    # 汽车启动
    for thread in highwayThreads:
        thread.start()

    # 红绿灯发送事件通知
    sign()
```
运行结果：
```python
绿灯初始化
BMWCar正在行驶中，行驶了60Km
VWCar正在行驶中，行驶了60Km
BMWCar通过了红灯
BMWCar正在行驶中，行驶了120Km
VWCar通过了红灯
VWCar正在行驶中，行驶了120Km
BMWCar通过了红灯
BMWCar正在行驶中，行驶了180Km
VWCar通过了红灯
VWCar正在行驶中，行驶了180Km
红灯亮起，所有行驶中的车辆不允许通过
绿灯亮起，所有行驶中的车辆必须通过
VWCar通过了红灯
VWCar正在行驶中，行驶了240Km
BMWCar通过了红灯
BMWCar正在行驶中，行驶了240Km
VWCar通过了红灯
VWCar正在行驶中，行驶了300Km
BMWCar通过了红灯
BMWCar正在行驶中，行驶了300Km
VWCar通过了红灯
VWCar正在行驶中，行驶了360Km
BMWCar通过了红灯
BMWCar正在行驶中，行驶了360Km
红灯亮起，所有行驶中的车辆不允许通过
绿灯亮起，所有行驶中的车辆必须通过
VWCar通过了红灯
VWCar正在行驶中，行驶了420Km
BMWCar通过了红灯
```
<a name="Kcz6T"></a>
### 实例2
面试题：有两个线程 A、B，A 线程打印 a,b,c 的字母，B线程打印 2,4,6 的数字，A、B 线程将轮询打印，如：a2 b4 c6...
```python
import threading
from threading import Event


# 打印字母函数
def printLetter(letterEvent, numEvent):
    for item in ["a", "b", "c"]:
        letterEvent.wait()
        print(item, end="")
        letterEvent.clear()
        numEvent.set()


# 打印数字函数
def printNum(numEvent, letterEvent):
    for item in [2, 4, 6]:
        numEvent.wait()
        print(item, end=" ")
        numEvent.clear()
        letterEvent.set()


if __name__ == '__main__':
    letterEvent, numEvent = Event(), Event()
    t1 = threading.Thread(target = printLetter, args = (letterEvent, numEvent))
    t2 = threading.Thread(target = printNum, args = (numEvent, letterEvent))

    threads = []
    threads.append(t1)
    threads.append(t2)

    for t in threads:
        t.start()

    letterEvent.set()
```
运行结果：
```python
a2 b4 c6
```
