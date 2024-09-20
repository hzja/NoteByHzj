Python<br />装饰器是 Python 中非常强大的功能，可以更加灵活地修改函数的行为和功能。<br />起初，每个 Python 开发者的目标都是让代码正常运行。慢慢地，开始担心可读性和可扩展性。这时候开始考虑装饰器。<br />装饰器是给函数添加额外行为的绝佳方式。而且有些小东西数据科学家经常需要注入到函数定义中。<br />使用装饰器，你会惊讶地发现可以减少代码重复并提高可读性。<br />以下是几乎在每个数据项目中都会使用的五种最常见的工具。
<a name="eqCdF"></a>
## 1、重试装饰器
在数据科学和软件开发项目中，经常依赖于外部系统。但并不是所有事情都能掌控。<br />当发生意外事件时，可能希望代码等待一段时间，以便外部系统纠正自身并重新运行。<br />在 Python 装饰器中实现此重试逻辑，这样就可以注释任何函数以应用重试行为。<br />以下是一个重试装饰器的代码。
```python
import time
from functools import wraps

def retry(max_tries=3, delay_seconds=1):
    def decorator_retry(func):
        @wraps(func)
        def wrapper_retry(*args, **kwargs):
            tries = 0
            while tries < max_tries:
                try:
                    return func(*args, **kwargs)
                except Exception as e:
                    tries += 1
                    if tries == max_tries:
                        raise e
                    time.sleep(delay_seconds)
        return wrapper_retry
    return decorator_retry
@retry(max_tries=5, delay_seconds=2)
def call_dummy_api():
    response = requests.get("https://jsonplaceholder.typicode.com/todos/1")
    return response
```
在上面的代码中，尝试获取API响应。如果失败，会重试相同的任务5次。在每次重试之间，等待2秒钟。
<a name="N49uK"></a>
## 2、缓存函数结果
代码库中的某些部分很少改变其行为。然而，它可能占用大量计算资源。在这种情况下，可以使用装饰器来缓存函数调用。<br />如果输入相同，则该函数仅运行一次。在每个后续运行中，结果将从缓存中获取。因此不必一直执行昂贵的计算。
```python
def memoize(func):
    cache = {}
    def wrapper(*args):
        if args in cache:
            return cache[args]
        else:
            result = func(*args)
            cache[args] = result
            return result
    return wrapper
```
装饰器使用字典存储函数参数并返回值。当执行此函数时，装饰器将检查先前的结果字典。只有在没有存储的值时才会调用实际函数。<br />以下是一个斐波那契数计算函数。由于这是一个递归函数，同一函数被多次执行。但通过缓存，可以加快这个过程。
```python
@memoize
def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n-1) + fibonacci(n-2)
```
这个函数使用缓存和不使用缓存的执行时间如下。请注意，使用缓存的版本只需要几毫秒就能运行完毕，而未使用缓存的版本则需要近一分钟。
```
Function slow_fibonacci took 53.05560088157654 seconds to run.
Function fast_fibonacci took 7.772445678710938e-05 seconds to run.
```
使用字典来保存先前的执行数据是一种简单直接的方法。然而，还有一种更复杂的方式来存储缓存数据。可以使用内存数据库，比如Redis。
<a name="quuVj"></a>
## 3、计时函数
这个应该不会让人感到意外。在处理数据密集型函数时，希望知道它运行所需时间。<br />通常做法是收集两个时间戳，一个在函数开始处，另一个在结束处。然后可以计算持续时间，并将其与返回值一起打印出来。<br />但对于多个函数反复进行此操作很麻烦。<br />相反地，可以使用装饰器完成这项工作。可以注释任何需要打印持续时间的功能。<br />以下是一个示例Python装饰器，在调用时打印函数运行时间：
```python
import time

def timing_decorator(func):
    def wrapper(*args, **kwargs):
        start_time = time.time()
        result = func(*args, **kwargs)
        end_time = time.time()
        print(f"Function {func.__name__} took {end_time - start_time} seconds to run.")
        return result
    return wrapper
```
可以使用这个装饰器来计算函数的执行时间：
```python
@timing_decorator
def my_function():
    # some code here
    time.sleep(1)  # simulate some time-consuming operation
    return
```
调用该函数将打印运行所需的时间。
```
my_function()

>>> Function my_function took 1.0019128322601318 seconds to run.
```
<a name="JOSJe"></a>
## 4、记录函数调用
这个装饰器非常类似于前面的一个。但它有一些特定的用途。<br />如果遵循软件设计原则，你会欣赏单一职责原则。这意味着每个函数都应该只有一个职责。<br />当以这种方式设计代码时，你也会想要记录函数的执行信息。这就是日志装饰器派上用场的地方。<br />下面的示例说明了这一点。
```python
import logging
import functools

logging.basicConfig(level=logging.INFO)

def log_execution(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        logging.info(f"Executing {func.__name__}")
        result = func(*args, **kwargs)
        logging.info(f"Finished executing {func.__name__}")
        return result
    return wrapper

@log_execution
def extract_data(source):
    # extract data from source
    data = ...

    return data

@log_execution
def transform_data(data):
    # transform data
    transformed_data = ...

    return transformed_data

@log_execution
def load_data(data, target):
    # load data into target
    ...

def main():
    # extract data
    data = extract_data(source)

    # transform data
    transformed_data = transform_data(data)

    # load data
    load_data(transformed_data, target)
```
上面的代码是一个ETL管道的简化版本。有三个单独的函数来处理每个提取、转换和加载步骤。使用`log_execution`装饰器包装了它们中的每一个。<br />现在，每当执行该代码时，将看到类似于以下输出：
```
INFO:root:Executing extract_data
INFO:root:Finished executing extract_data
INFO:root:Executing transform_data
INFO:root:Finished executing transform_data
INFO:root:Executing load_data
INFO:root:Finished executing load_data
```
也可以在这个装饰器中打印执行时间。但我更喜欢将它们分别放在不同的装饰器中。这样，就可以选择为一个函数使用哪个（或两个）。<br />以下是如何在单个函数上使用多个装饰器。
```python
@log_execution
@timing_decorator
def my_function(x, y):
    time.sleep(1)
    return x + y
```
<a name="exHGi"></a>
## 5、通知装饰器
最后，在生产系统中非常有用的一个装饰器是通知装饰器。<br />即使经过多次重试，即使代码库经过了充分测试，也可能会出现故障。当这种情况发生时，需要通知某人采取快速行动。<br />如果您曾经构建过数据管道并希望它永远正常工作，则此内容并不新颖。<br />以下装饰器在内部函数执行失败时发送电子邮件。在您的情况下，它不必是电子邮件通知。您可以将其配置为发送 Teams/slack 通知。
```python
import smtplib
import traceback
from email.mime.text import MIMEText

def email_on_failure(sender_email, password, recipient_email):
    def decorator(func):
        def wrapper(*args, **kwargs):
            try:
                return func(*args, **kwargs)
            except Exception as e:
                # format the error message and traceback
                err_msg = f"Error: {str(e)}\n\nTraceback:\n{traceback.format_exc()}"

                # create the email message
                message = MIMEText(err_msg)
                message['Subject'] = f"{func.__name__} failed"
                message['From'] = sender_email
                message['To'] = recipient_email

                # send the email
                with smtplib.SMTP_SSL('smtp.gmail.com', 465) as smtp:
                    smtp.login(sender_email, password)
                    smtp.sendmail(sender_email, recipient_email, message.as_string())

                # re-raise the exception
                raise

        return wrapper

    return decorator

@email_on_failure(sender_email='your_email@gmail.com', password='your_password', recipient_email='recipient_email@gmail.com')
def my_function():
    # code that might fail
```
<a name="b8YwC"></a>
## 结论
装饰器是一种非常方便的方式，可以为函数应用新行为。如果没有它们，将会有很多代码重复。<br />在本文中，讨论了最常用的装饰器。可以根据自己的需求扩展这些内容。
