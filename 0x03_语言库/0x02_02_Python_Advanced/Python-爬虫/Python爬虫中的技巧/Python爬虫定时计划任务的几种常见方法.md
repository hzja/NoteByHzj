Python 爬虫 定时任务
<a name="CQiPj"></a>
## 1、while True
首先最容易的是while true死循环挂起，看代码：
```python
import os
import time
import sys 
from datetime import datetime, timedelta
def One_Plan():
     # 设置启动周期
     Second_update_time = 24 * 60 * 60
    # 当前时间
    now_Time = datetime.now()
    # 设置 任务启动时间
    plan_Time = now_Time.replace(hour=9, minute=0, second=0, microsecond=0)  
    # 设置差值，-1 day, 21:48:53.246576，类似于这样
    # time.sleep()需要传入int，所以下面使用.total_seconds() 
    # 主要用来计算差值，返回int，具体功能可以自行查阅相关资料
    delta = plan_Time - now_Time
    first_plan_Time = delta.total_seconds() % Second_update_time
    print("距离第一次执行需要睡眠%d秒" % first_plan_Time)
    return first_plan_Time
# while Ture代码块，挂起程序，睡眠时间结束后调用函数名进行执行
while True:
    s1 = One_Plan()
    time.sleep(s1)
    # 下面这里是自己定义的函数，想跑代码的可以换成hellow world函数或者注释掉这行测试下
    exe_file(D_list)
    print("正在执行首次更新程序")
```
使用这种方式进行定时计划的启动如果为单个程序，并且一天执行一次的话没什么问题，如果要考虑到一天执行多个任务并且一天需要执行多次，短板一下就凸显出来了。<br />在工作的情况中还需要考虑到很多因素，比如爬虫程序需要在晚上12点和早上6点、9点，下午3点执行四次，并且需要同时执行4个爬虫，还需要考虑到网络是否稳定，如果程序挂掉该怎么处理等等因素。
<a name="kCXFB"></a>
## 2、Timer模块
现在需要启动一个selenium的爬虫，使用火狐驱动+多线程，CPU占用率20%，启动selenium后不停的开启浏览器+多线程，没过5分钟，CPU占用率直接拉到90%+，电脑卡到飞起，定时程序虽然还在运行，但是已经类似于待机状态：
```python
from datetime import datetime
from threading import Timer
import time
# 定时任务
def task():
    print(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
def timedTask():
    '''
    第一个参数: 延迟多长时间执行任务(秒)
    第二个参数: 要执行的函数
    第三个参数: 调用函数的参数(tuple)
    '''
    Timer(5, task, ()).start()
while True:
    timedTask()
    time.sleep(5)
```
```
2020-06-05 14:06:39
2020-06-05 14:06:44
2020-06-05 14:06:49
2020-06-05 14:06:54
2020-06-05 14:06:59
2020-06-05 14:07:04
2020-06-05 14:07:09
2020-06-05 14:07:14
2020-06-05 14:07:19
2020-06-05 14:07:24
```
<a name="prBWK"></a>
## 3、sched模块
代码如下：
```python
from datetime import datetime
import sched
import time
def timedTask():
    # 初始化 sched 模块的 scheduler 类,传入(time.time, time.sleep)这两个参数
    scheduler = sched.scheduler(time.time, time.sleep)
    # 增加调度任务，enter(睡眠时间，执行级别，执行函数)
    scheduler.enter(5, 1, task)
    # 运行任务
    scheduler.run()
# 定时任务
def task():
    print(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
if __name__ == '__main__':
    timedTask()
```
这个模块也很容易上手，需要注意，`scheduler()`只会执行一次就结束程序，可以在mian下面添加while Ture或者直接添加调度任务在timeTask中就可以，另外除了这样的写法外，还有另外一种写法，看代码：
```python
import schedule
import time
def hellow():
    print('hellow')
def Timer():
    schedule.every().day.at("09:00").do(hellow)
    schedule.every().day.at("18:00").do(hellow)
    while True:
        schedule.run_pending()
        time.sleep('需要睡眠的周期')
Timer()
```
可以看到，有day-hour-minute，定时任务非常的方便，在while True里添加需要睡眠的时间，在函数模块内添加需要执行的次数即可。
