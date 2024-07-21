<a name="Sc38n"></a>
### 1、数据类型
```python
int_num = 42
float_num = 3.14
string_var = "Hello, Python!"
bool_var = True
```
<a name="JOsGW"></a>
### 2、变量和赋值
```python
x = 10
y = "Python"
```
<a name="FFTIO"></a>
### 3、列表和元组
```python
my_list = [1, 2, 3, "Python"]
my_tuple = (1, 2, 3, "Tuple")
```
<a name="SI3Ku"></a>
### 4、字典
```python
my_dict = {'name': 'John', 'age': 25, 'city': 'Pythonville'}
```
<a name="fMw3u"></a>
### 5、条件语句
```python
if x > 0:
    print("Positive")
elif x == 0:
    print("Zero")
else:
    print("Negative")
for item in my_list:
    print(item)
while condition:
    # code
```
<a name="HKlFx"></a>
### 6、函数
```python
def greet(name="User"):
    return f"Hello, {name}!"
result = greet("John")
```
<a name="d39Sd"></a>
### 7、 类和对象
```python
class Dog:
    def __init__(self, name):
        self.name = name
def bark(self):
        print("Woof!")
my_dog = Dog("Buddy")
my_dog.bark()
```
<a name="THsjf"></a>
### 8、文件处理
```python
with open("file.txt", "r") as file:
    content = file.read()
with open("new_file.txt", "w") as new_file:
    new_file.write("Hello, Python!")
```
<a name="JkUen"></a>
### 9、异常处理
```python
try:
    result = 10 / 0
except ZeroDivisionError:
    print("Cannot divide by zero!")
finally:
    print("Execution completed.")
```
<a name="RZd46"></a>
### 10、库和模块
```python
import math
from datetime import datetime
result = math.sqrt(25)
current_time = datetime.now()
```
<a name="Bc348"></a>
### 11、列表推导式
```python
squares = [x**2 for x in range(5)]
```
<a name="MBL8a"></a>
### 12、匿名函数
```python
add = lambda x, y: x + y
result = add(2, 3)
```
<a name="VagI0"></a>
### 13、虚拟环境
```bash
# 创建一个虚拟环境
python -m venv myenv

# 激活虚拟环境
source myenv/bin/activate  # On Unix or MacOS
myenv\Scripts\activate  # On Windows

# 退出虚拟环境
deactivate
```
<a name="aX4dS"></a>
### 14、安装包管理
```bash
# 安装Python库
pip install package_name

# 已安装的Python库列表
pip list

# 创建一个requirements.txt文件
pip freeze > requirements.txt

# 根据requirements.txt安装依赖的Python库
pip install -r requirements.txt
```
<a name="zYqcL"></a>
### 15、JSON数据处理
```python
import json

# 将Python对象转换成JSON
json_data = json.dumps({"name": "John", "age": 25})

# 将JSON转换成Python对象
python_obj = json.loads(json_data)
```
<a name="GXxKG"></a>
### 16、正则表达式
```python
import re
pattern = r'\d+'  # 匹配1个或多个数字
result = re.findall(pattern, "There are 42 apples and 123 oranges.")
```
<a name="TFQsk"></a>
### 17、日期操作
```python
from datetime import datetime, timedelta

current_date = datetime.now()
future_date = current_date + timedelta(days=7)
```
<a name="YLIm8"></a>
### 18、列表操作
```python
numbers = [1, 2, 3, 4, 5]

# 过滤
evens = list(filter(lambda x: x % 2 == 0, numbers))

# 映射
squared = list(map(lambda x: x**2, numbers))

# 减少
from functools import reduce
product = reduce(lambda x, y: x * y, numbers)
```
<a name="iI7fb"></a>
### 19、字典操作
```python
my_dict = {'a': 1, 'b': 2, 'c': 3}

# 获取值
value = my_dict.get('d', 0)

# 字典推导式
squared_dict = {key: value**2 for key, value in my_dict.items()}
```
<a name="hllQh"></a>
### 20、线程并发
```python
import threading
def print_numbers():
    for i in range(5):
        print(i)
thread = threading.Thread(target=print_numbers)
thread.start()
```
<a name="n92n1"></a>
### 21、使用Asyncio实现并发
```python
import asyncio
async def print_numbers():
    for i in range(5):
        print(i)
        await asyncio.sleep(1)
asyncio.run(print_numbers())
```
<a name="yNvyo"></a>
### 22、使用Beautiful Soup实现网页爬虫
```python
from bs4 import BeautifulSoup
import requests
url = "https://example.com"
response = requests.get(url)
soup = BeautifulSoup(response.text, 'html.parser')
title = soup.title.text
```
<a name="YfowJ"></a>
### 23、使用Flask实现RESTful API
```python
from flask import Flask, jsonify, request
app = Flask(__name__)

@app.route('/api/data', methods=['GET'])
def get_data():
    data = {'key': 'value'}
    return jsonify(data)
if __name__ == '__main__':
    app.run(debug=True)
```
<a name="xsUAb"></a>
### 24、使用unittest进行单元测试
```python
import unittest

def add(x, y):
    return x + y
class TestAddition(unittest.TestCase):
    def test_add_positive_numbers(self):
        self.assertEqual(add(2, 3), 5)
if __name__ == '__main__':
    unittest.main()
```
<a name="sCFB8"></a>
### 25、与SQLite的数据库交互
```python
import sqlite3
conn = sqlite3.connect('example.db')
cursor = conn.cursor()

# 只需SQL查询
cursor.execute('CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT)')

# 提交
conn.commit()

# 关闭链接
conn.close()
```
<a name="DO3BI"></a>
### 26、文件写入和读取
```python
# 保存文件
with open('example.txt', 'w') as file:
    file.write('Hello, Python!')
# 读取文件
with open('example.txt', 'r') as file:
    content = file.read()
```
<a name="Wd8wV"></a>
### 27、异常操作
```python
try:
    result = 10 / 0
except ZeroDivisionError as e:
    print(f"Error: {e}")
except Exception as e:
    print(f"Unexpected Error: {e}")
else:
    print("No errors occurred.")
finally:
    print("This block always executes.")
```
<a name="alivR"></a>
### 28、JSON数据处理
```python
import json

# 将Python对象转换成JSON
json_data = json.dumps({"name": "John", "age": 25})

# 将JSON转换成Python对象
python_obj = json.loads(json_data)
```
<a name="PvDqO"></a>
### 29、Python修饰符
```python
def decorator(func):
    def wrapper():
        print("Before function execution")
        func()
        print("After function execution")
    return wrapper
@decorator
def my_function():
    print("Inside the function")
my_function()
```
<a name="baOnK"></a>
### 30、使用枚举
```python
from enum import Enum
class Color(Enum):
    RED = 1
    GREEN = 2
    BLUE = 3
print(Color.RED)
```
<a name="sVdVR"></a>
### 31、集合操作
```python
set1 = {1, 2, 3}
set2 = {3, 4, 5}

# 合并
union_set = set1 | set2

# 交集
intersection_set = set1 & set2

# 差值
difference_set = set1 - set2
```
<a name="UM1fH"></a>
### 32、列表推导式
```python
numbers = [1, 2, 3, 4, 5]

# 偶数的平方
squares = [x**2 for x in numbers if x % 2 == 0]
```
<a name="WLJsa"></a>
### 33、匿名函数
```python
add = lambda x, y: x + y
result = add(3, 5)
```
<a name="vUKes"></a>
### 34、线程与`Concurrent.futures`
```python
from concurrent.futures import ThreadPoolExecutor
def square(x):
    return x**2
with ThreadPoolExecutor() as executor:
    results = executor.map(square, [1, 2, 3, 4, 5])
```
<a name="RdJNs"></a>
### 35、使用gettext国际化
```python
import gettext

# 设置语言
lang = 'en_US'
_ = gettext.translation('messages', localedir='locale', languages=[lang]).gettext
print(_("Hello, World!"))
```
<a name="si6u6"></a>
### 36、虚拟环境
```bash
# 创建一个虚拟环境
python -m venv myenv

# 激活虚拟环境
source myenv/bin/activate  # On Unix or MacOS
myenv\Scripts\activate  # On Windows

# 退出虚拟环境
deactivate
```
<a name="CsnKR"></a>
### 37、日期处理
```python
from datetime import datetime, timedelta
now = datetime.now()

# 日期格式化
formatted_date = now.strftime('%Y-%m-%d %H:%M:%S')

# 添加天数
future_date = now + timedelta(days=7)
```
<a name="xY3Im"></a>
### 38、使用字典
```python
my_dict = {'name': 'John', 'age': 30}

# 获取值
age = my_dict.get('age', 25)

# 遍历键和键值
for key, value in my_dict.items():
    print(f"{key}: {value}")
```
<a name="LUL6H"></a>
### 39、正则表达式
```python
import re
text = "Hello, 123 World!"

# 匹配数字
numbers = re.findall(r'\d+', text)
```
<a name="CdjIn"></a>
### 40、迭代器
```python
def square_numbers(n):
    for i in range(n):
        yield i**2
squares = square_numbers(5)
```
<a name="NovDs"></a>
### 41、与SQLite的数据库交互
```python
import sqlite3

# 链接SQLite数据库
conn = sqlite3.connect('mydatabase.db')
cursor = conn.cursor()

# 执行SQL查询语句
cursor.execute('SELECT * FROM mytable')
```
<a name="uU3R5"></a>
### 42、ZIP文件操作处理
```python
import zipfile
with zipfile.ZipFile('archive.zip', 'w') as myzip:
    myzip.write('file.txt')
with zipfile.ZipFile('archive.zip', 'r') as myzip:
    myzip.extractall('extracted')
```
<a name="dKtlN"></a>
### 43、网页抓取请求和BeautifulSoup
```python
import requests
from bs4 import BeautifulSoup
url = 'https://example.com'
response = requests.get(url)
soup = BeautifulSoup(response.text, 'html.parser')

# 获取网页数据
title = soup.title.text
```
<a name="yKnR1"></a>
### 44、使用smtplib发送邮件
```python
import smtplib
from email.mime.text import MIMEText

# 启动邮件服务
server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()

# 登录电子邮件账户
server.login('your_email@gmail.com', 'your_password')

# 发送邮件
msg = MIMEText('Hello, Python!')
msg['Subject'] = 'Python Email'
server.sendmail('your_email@gmail.com', 'recipient@example.com', msg.as_string())
```
<a name="L59UG"></a>
### 45、使用JSON文件
```python
import json
data = {'name': 'John', 'age': 30}

# 写入JSON文件
with open('data.json', 'w') as json_file:
    json.dump(data, json_file)

# 读取JSON文件
with open('data.json', 'r') as json_file:
    loaded_data = json.load(json_file)
```
