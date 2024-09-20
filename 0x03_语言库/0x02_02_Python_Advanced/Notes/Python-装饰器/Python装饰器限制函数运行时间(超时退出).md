Python装饰器<br />实际项目中会涉及到需要对有些函数的响应时间做一些限制，如果超时就退出函数的执行，停止等待。<br />可以利用Python中的装饰器实现对函数执行时间的控制。<br />Python装饰器简单来说可以在不改变某个函数内部实现和原来调用方式的前提下对该函数增加一些附件的功能，提供了对该函数功能的扩展。
<a name="eKLD2"></a>
### 方法一、使用 signal
使用signal有所限制，需要在**linux**系统上，并且需要在**主线程**中使用。方法二使用线程计时，不受此限制。
```python
# coding=utf-8
import signal
import time
 
def set_timeout(num, callback):
    def wrap(func):
        def handle(signum, frame):  # 收到信号 SIGALRM 后的回调函数，第一个参数是信号的数字，第二个参数是the interrupted stack frame.
            raise RuntimeError
 
        def to_do(*args, **kwargs):
            try:
                signal.signal(signal.SIGALRM, handle)  # 设置信号和回调函数
                signal.alarm(num)  # 设置 num 秒的闹钟
                print('start alarm signal.')
                r = func(*args, **kwargs)
                print('close alarm signal.')
                signal.alarm(0)  # 关闭闹钟
                return r
            except RuntimeError as e:
                callback()
 
        return to_do
 
    return wrap
 
def after_timeout():  # 超时后的处理函数
    print("Time out!")
 
@set_timeout(2, after_timeout)  # 限时 2 秒超时
def connect():  # 要执行的函数
    time.sleep(3)  # 函数执行时间，写大于2的值，可测试超时
    print('Finished without timeout.')
 
if __name__ == '__main__':
    connect()
```
<a name="o1cRE"></a>
### 方法二、使用Thread
Thread方法在**linux和windows**环境下都可以使用，不过适用于Python**3.3及以下**版本，从Python3.4版本开始Thread部分函数有更改。
```python
# -*- coding: utf-8 -*-
from threading import Thread
import time
 
class TimeoutException(Exception):
    pass
 
ThreadStop = Thread._Thread__stop
 
def timelimited(timeout):
    def decorator(function):
        def decorator2(*args,**kwargs):
            class TimeLimited(Thread):
                def __init__(self,_error= None,):
                    Thread.__init__(self)
                    self._error =  _error
 
                def run(self):
                    try:
                        self.result = function(*args,**kwargs)
                    except Exception,e:
                        self._error = str(e)
 
                def _stop(self):
                    if self.isAlive():
                        ThreadStop(self)
 
            t = TimeLimited()
            t.start()
            t.join(timeout)
 
            if isinstance(t._error,TimeoutException):
                t._stop()
                raise TimeoutException('timeout for %s' % (repr(function)))
 
            if t.isAlive():
                t._stop()
                raise TimeoutException('timeout for %s' % (repr(function)))
 
            if t._error is None:
                return t.result
 
        return decorator2
    return decorator
 
@timelimited(2)  # 设置运行超时时间2S
def fn_1(secs):
    time.sleep(secs)
    return 'Finished without timeout'
 
def do_something_after_timeout():
    print('Time out!')
 
if __name__ == "__main__":
    try:
        print(fn_1(3))  # 设置函数执行3S
    except TimeoutException as e:
        print(str(e))
        do_something_after_timeout()
```
<a name="RAt5r"></a>
### 方法三、使用eventlet
eventlet在Python**3**下可用。程序进入睡眠时，如果超时可以正常跳出， 而没有睡眠时不会正常跳出。
```python
import requests
import eventlet
import time

eventlet.monkey_patch(time=True)

time_limit = 3  #set timeout time 3s
with eventlet.Timeout(time_limit,False):
    time.sleep(5) # 没有这个则不会跳出
    r=requests.get("https://me.csdn.net/dcrmg", verify=False)
    print('error')
print('over')
```
