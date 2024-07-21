Python 爬虫
<a name="A6nSH"></a>
## 前言
在爬虫过程中，经常会由于一些网络或其他不可控因素，从而遇到一些功能性问题。比如在发送请求时，会因为网络不稳定，往往会有请求超时的问题。这种情况下，通常会在代码中加入重试的代码。重试的代码本身不难实现，但如何写得优雅、易用，是要考虑的问题。Tenacity是一个通用的retry库，简化为任何任务加入重试的功能，它实现了几乎可以使用到的所有重试场景。先使用pip安装：
```bash
pip install tenacity
```
<a name="5cC4Y"></a>
## 无条件重试，重试之间无间隔
```python
import tenacity
import requests
# 直接加上retry装饰器，代码抛出异常会一直重试，直到代码运行成功
@tenacity.retry()
def baidu():
    response = requests.get(url = 'http://www.baidu.com')
    if response.status_code == 200:
        return response.text
    raise Exception
res = baidu()
print(res)
```
这里使用requests请求了百度，并输出response，在code=200的位置上，如果code不等于200，这个程序就会被retry重试。无条件重试，但是在重试之前要等待 3秒：
```python
from tenacity import retry , stop_after_attempt , stop_after_delay , wait_fixed
import requests
# 在程序重试前固定等待时间
@retry(wait = wait_fixed(3))
def baidu():
    web = requests.get(url = 'http://www.baidu.com')
    if web.status_code == 200:
        return web.text
    raise Exception
res = baidu()
print(res)
```
<a name="CYI4L"></a>
## 设置停止的基本条件
<a name="BWaPK"></a>
### 只重试3次
```python
from tenacity import retry,stop_after_attempt
import requests
# 加上终止条件的retry
# 重试三次之后不再重试
@retry(stop=stop_after_attempt(3))
def baidu():
    res = requests.get(url = 'http://www.baidu.com')
    if res.status_code == 200:
        return res.text
    raise Exception
web = baidu()
print(web)
```
这次还是访问百度，但是加了一个条件，可以看到重试三次之后不再重试，一般用于使用账号密码登录的爬虫，有的账号一天只能登录5次，超过5次就无法登录了，在运行程序的时候，使用retry + `stop_after_attempt()`可以完美解决。
<a name="PAJji"></a>
### 重试5秒后不再重试
```python
from tenacity import retry , stop_after_attempt , stop_after_delay
import requests
# 指定5s重试间隔
@retry(stop=stop_after_delay(5))
def baidu():
    web = requests.get(url = 'http://www.baidu.com')
    if web.status_code == 200:
        return web.text
    raise Exception
res = baidu()
print(res)
```
<a name="p13TJ"></a>
### 用|连接多个重试条件
```python
from tenacity import retry , stop_after_attempt , stop_after_delay
import requests
# 使用'|' 连接多个条件组合使用
@retry(stop=stop_after_delay(5) | stop_after_attempt(3))
def baidu():
    web = requests.get(url = 'http://www.baidu.com')
    if web.status_code == 200:
        return web.text
    raise Exception
res = baidu()
print(res)
```
<a name="sqs8S"></a>
## 在重试前设置随机等待时间
```python
from tenacity import retry , stop_after_attempt , stop_after_delay , wait_fixed , wait_random
import requests
# 在程序重试前设置随机等待时间
@retry(wait = wait_random(min=1,max=2))
def baidu():
    web = requests.get(url = 'http://www.baidu.com')
    if web.status_code == 200:
        return web.text
    raise Exception
res = baidu()
print(res)
```
<a name="Wz6ST"></a>
## 按照指定数等待时间
```python
from tenacity import retry , stop_after_attempt , stop_after_delay , wait_fixed , wait_random , wait_exponential
import requests
# 按照指定数等待时间
@retry(wait = wait_exponential(multiplier=2,min = 3,max = 100))
def baidu():
    web = requests.get(url = 'http://www.baidu.com')
    if web.status_code == 200:
        return web.text
    raise Exception
res = baidu()
print(res)
```
<a name="b7hTR"></a>
## 有触发条件的retry语句
```python
from tenacity import retry ,retry_if_exception_type ,retry_if_result
import requests
# 有触发条件的retry语句
@ retry(retry = retry_if_exception_type(IOError))
def fun_1():
    print('巴拉巴拉巴拉巴拉')
    raise Exception
def fun_2(value):
    return value isNone
@ retry(retry = retry_if_exception_type(fun_2))
def fun_3():
    print('滴滴滴滴滴')
@ retry(retry=(retry_if_result(fun_2)|retry_if_exception_type()))
def fun_4():
    print('呼呼呼呼呼呼')
```
<a name="V6BMS"></a>
## 在retry前后增加log
```python
from tenacity import retry , stop_after_attempt ,before_log ,after_log , before_sleep_log
import logging
logger = logging.getLogger(__name__)
@ retry(stop = stop_after_attempt(3),before = before_log(logger,logging.DEBUG))
def fun_1():
    raise Exception('Error')
@ retry(stop = stop_after_attempt(3),after = after_log(logger,logging.DEBUG))
def fun_2():
    raise Exception('Error')
@ retry(stop = stop_after_attempt(3),before_sleep = before_sleep_log(logger,logging.DEBUG))
def fun_3():
    raise Exception('Error')
```

