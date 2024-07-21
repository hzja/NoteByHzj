Python 重试机制<br />为了避免由于一些网络或等其他不可控因素，而引起的功能性问题。比如在发送请求时，会因为网络不稳定，往往会有请求超时的问题。<br />这种情况下，通常会在代码中加入重试的代码。重试的代码本身不难实现，但如何写得优雅、易用，是要考虑的问题。<br />这里介绍的是一个第三方库 - `Tenacity` （标题中的重试机制并并不准确，它不是 Python 的内置模块，因此并不能称之为机制），它实现了日常开发中可以使用到的所有重试场景，比如：

1. 在什么情况下才进行重试？
2. 重试几次呢?
3. 重试多久后结束？
4. 每次重试的间隔多长呢？
5. 重试失败后的回调？

在使用它之前 ，先要安装它
```bash
$ pip install tenacity
```
<a name="4WRMs"></a>
## 1. 最基本的重试
无条件重试，重试之间无间隔
```python
from tenacity import retry
@retry
def test_retry():
    print("等待重试，重试无间隔执行...")
    raise Exception
test_retry()
```
无条件重试，但是在重试之前要等待 2 秒
```python
from tenacity import retry, wait_fixed
@retry(wait=wait_fixed(2))
def test_retry():
    print("等待重试...")
    raise Exception
test_retry()
```
<a name="hFffH"></a>
## 2. 设置停止基本条件
只重试7 次
```python
from tenacity import retry, stop_after_attempt
@retry(stop=stop_after_attempt(7))
def test_retry():
    print("等待重试...")
    raise Exception
test_retry()
```
重试 10 秒后不再重试
```python
from tenacity import retry, stop_after_delay
@retry(stop=stop_after_delay(10))
def test_retry():
    print("等待重试...")
    raise Exception
test_retry()
```
或者上面两个条件满足一个就结束重试
```python
from tenacity import retry, stop_after_delay, stop_after_attempt
@retry(stop=(stop_after_delay(10) | stop_after_attempt(7)))
def test_retry():
    print("等待重试...")
    raise Exception
test_retry()
```
<a name="vpNnd"></a>
## 3. 设置何时进行重试
在出现特定错误/异常（比如请求超时）的情况下，再进行重试
```python
from requests import exceptions
from tenacity import retry, retry_if_exception_type
@retry(retry=retry_if_exception_type(exceptions.Timeout))
def test_retry():
    print("等待重试...")
    raise exceptions.Timeout
test_retry()
```
在满足自定义条件时，再进行重试。<br />如下示例，当 `test_retry` 函数返回值为 False 时，再进行重试
```python
from tenacity import retry, stop_after_attempt, retry_if_result
def is_false(value):
    return value is False
@retry(stop=stop_after_attempt(3),
       retry=retry_if_result(is_false))
def test_retry():
    return False
test_retry()
```
<a name="lTIFm"></a>
## 4. 重试后错误重新抛出
当出现异常后，tenacity 会进行重试，若重试后还是失败，默认情况下，往上抛出的异常会变成 RetryError，而不是最根本的原因。<br />因此可以加一个参数（`reraise=True`），使得当重试失败后，往外抛出的异常还是原来的那个。
```python
from tenacity import retry, stop_after_attempt
@retry(stop=stop_after_attempt(7), reraise=True)
def test_retry():
    print("等待重试...")
    raise Exception
test_retry()
```
<a name="tHUS7"></a>
## 5. 设置回调函数
当最后一次重试失败后，可以执行一个回调函数
```python
from tenacity import *
def return_last_value(retry_state):
    print("执行回调函数")
    return retry_state.outcome.result()  # 表示返回原函数的返回值
def is_false(value):
    return value is False
@retry(stop=stop_after_attempt(3),
       retry_error_callback=return_last_value,
       retry=retry_if_result(is_false))
def test_retry():
    print("等待重试中...")
    return False
print(test_retry())
```
输出如下
```
等待重试中...
等待重试中...
等待重试中...
执行回调函数
False
```
