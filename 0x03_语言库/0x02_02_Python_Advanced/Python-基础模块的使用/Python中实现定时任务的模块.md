Python<br />介绍一下Python当中的定时任务，主要用到的模块是sched，当然除了该模块之外，还有其他的例如ApScheduler，但是相比较与sched模块而言，后者还需要另外通过pip命令来下载，稍显麻烦。那么在这来看看sched模块的使用教程。
<a name="nmPzQ"></a>
## 牛刀小试
先来看下面这个案例，代码如下
```python
import sched
import time

def say_hello(name):
    print(f"Hello, world, {name}")

scheduler = sched.scheduler(time.time, time.sleep)

scheduler.enter(5, 1, say_hello, ("张三", ))
scheduler.run()
```
那么上述的代码中，第一步首先则是实例化一个定时器，通过如下的代码
```python
import sched

scheduler = sched.scheduler()
```
接下来通过`enter()`方法来执行定时任务的操作，其中的参数分别是延迟的时间、任务的优先级以及具体的执行函数和执行函数中的参数。像如上的代码就会在延迟5秒钟之后执行`say_hello()`函数<br />当然要是延迟的时间相等的时候，可以设置任务执行的优先级来指定函数方法运行的顺序，例如有如下的代码
```python
import sched
import time

def say_hello(name):
    print(f"Hello, world, {name}")

def say_hello_2(name):
    print(f"Hello, {name}")

scheduler = sched.scheduler(time.time, time.sleep)

scheduler.enter(5, 2, say_hello, ("张三", ))
scheduler.enter(5, 1, say_hello_2, ("李四", ))
scheduler.run()
```
如上述代码，尽管延迟的时间都是一样的，但是`say_hello()`方法的优先级明显要比`say_hello_2()`方法要低一些，因此后者会优先执行。
<a name="b4zDr"></a>
## 进阶使用
除了让函数延迟执行，还可以让其重复执行，具体这样来操作，代码如下
```python
import sched
import time

def say_hello():
    print("Hello, world!")

scheduler = sched.scheduler(time.time, time.sleep)

def repeat_task():
    scheduler.enter(5, 1, say_hello, ())
    scheduler.enter(5, 1, repeat_task, ())

repeat_task()
scheduler.run()
```
这里新建了一个`repeat_task()`自定义函数，调用了`scheduler.enter()`方法5秒钟执行一次之前定义的`say_hello()`函数
<a name="LSecz"></a>
### 在固定时间执行任务
同时还可以让任务在指定的时间执行，这里用到`scheduler.entertabs()`方法，代码如下
```python
import sched
import time

def say_hello():
    print("Hello, world!")

scheduler = sched.scheduler(time.time, time.sleep)

# 指定时间执行任务
specific_time = time.time() + 5  # 距离现在的5秒钟之后执行
scheduler.enterabs(specific_time, 1, say_hello, ())

scheduler.run()
```
传入其中参数使其在指定的时间，也就是距离当下的5秒钟之后来执行任务
<a name="mXQPr"></a>
### 执行多个任务
这里仍然是调用`enter()`方法来运行多个任务，代码如下
```python
import sched
import time

def task_one():
    print("Task One - Hello, world!")

def task_two():
    print("Task Two - Hello, world!")

scheduler = sched.scheduler(time.time, time.sleep)

# 任务一在两秒钟只有执行
scheduler.enter(2, 1, task_one, ())

# 任务二在五秒钟之后运行
scheduler.enter(5, 1, task_two, ())

scheduler.run()
```
这里定义了两个函数，task_one和task_two里面分是同样的执行逻辑，打印出“Hello, world!”，然后`task_one()`是在两秒钟之后执行而`task_two()`则是在5秒钟之后执行，两者执行的优先级都是一样的。
<a name="VcrkN"></a>
### 以不同的优先级执行不同的任务
这回给`task_one()`和`task_two()`赋予不同的优先级，看一看执行的结果如下
```python
import sched
import time

def task_one():
    print("Task One - Hello, world!")

def task_two():
    print("Task Two - Hello, world!")

scheduler = sched.scheduler(time.time, time.sleep)

# 优先级是1
scheduler.enter(2, 2, task_one, ())

# 优先级是2
scheduler.enter(5, 1, task_two, ())

scheduler.run()
```
output
```
Task One - Hello, world!
Task Two - Hello, world!
```
上述的代码会在停顿两秒之后运行`task_one()`函数，再停顿3秒之后执行`task_two()`函数
<a name="BHTPh"></a>
### 定时任务加上取消方法
给定时任务添加上取消的方法，代码如下
```python
import sched
import time

def task_one():
    print("Task One - Hello, world!")

def task_two():
    print("Task Two - Hello, world!")

scheduler = sched.scheduler(time.time, time.sleep)

# 任务一在两秒钟只有执行
task_one_event = scheduler.enter(2, 1, task_one, ())

# 任务二在五秒钟之后运行
task_two_event = scheduler.enter(5, 1, task_two, ())

# 取消执行task_one
scheduler.cancel(task_one_event)

scheduler.run()
```
将两秒钟之后执行的`task_one()`方法给取消掉，最后就只执行了`task_two()`方法，也就打印出来“Task Two - Hello, world!”
<a name="URJIV"></a>
## 执行备份程序
来写一个备份的脚本，在每天固定的时间将文件备份，代码如下
```python
import sched
import time
import shutil

def backup_files():
    source = '路径/files'
    destination = '路径二'
    shutil.copytree(source, destination)

def schedule_backup():
    # 创建新的定时器
    scheduler = sched.scheduler(time.time, time.sleep)

    # 备份程序在每天的1点来执行
    backup_time = time.strptime('01:00:00', '%H:%M:%S')
    backup_event = scheduler.enterabs(time.mktime(backup_time), 1, backup_files, ())

    # 开启定时任务
    scheduler.run()

schedule_backup()
```
通过`shutil`模块当中的`copytree()`方法来执行拷贝文件，然后在每天的1点准时执行
<a name="HX0l7"></a>
## 执行定时分发邮件的程序
最后来执行定时分发邮件的程序，代码如下
```python
import sched
import time
import smtplib
from email.mime.text import MIMEText

def send_email(subject, message, from_addr, to_addr, smtp_server):
    # 邮件的主体信息
    email = MIMEText(message)
    email['Subject'] = subject
    email['From'] = from_addr
    email['To'] = to_addr

    # 发邮件
    with smtplib.SMTP(smtp_server) as server:
        server.send_message(email)

def send_scheduled_email(subject, message, from_addr, to_addr, smtp_server, scheduled_time):
    # 创建定时任务的示例
    scheduler = sched.scheduler(time.time, time.sleep)

    # 定时邮件
    scheduler.enterabs(scheduled_time, 1, send_email, argument=(subject, message, from_addr, to_addr, smtp_server))

    # 开启定时器
    scheduler.run()

subject = 'Test Email'
message = 'This is a test email'
from_addr = 'test@example.com'
to_addr = 'test@example.com'
smtp_server = 'smtp.test.com'

scheduled_time = time.time() + 60 # 一分钟之后执行程序
send_scheduled_email(subject, message, from_addr, to_addr, smtp_server, scheduled_time)
```
