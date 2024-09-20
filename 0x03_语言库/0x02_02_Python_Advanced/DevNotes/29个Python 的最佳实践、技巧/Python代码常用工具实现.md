Python
<a name="nDCuT"></a>
## 日期生成
很多时候需要批量生成日期，方法有很多，这里分享两段代码
<a name="c42SG"></a>
### 获取过去 N 天的日期
```python
import datetime

def get_nday_list(n):
    before_n_days = []
    for i in range(1, n + 1)[::-1]:
        before_n_days.append(str(datetime.date.today() - datetime.timedelta(days=i)))
    return before_n_days

a = get_nday_list(30)
print(a)
```
Output:
```python
['2021-12-23', '2021-12-24', '2021-12-25', '2021-12-26', '2021-12-27', '2021-12-28', '2021-12-29', '2021-12-30', '2021-12-31', '2022-01-01', '2022-01-02', '2022-01-03', '2022-01-04', '2022-01-05', '2022-01-06', '2022-01-07', '2022-01-08', '2022-01-09', '2022-01-10', '2022-01-11', '2022-01-12', '2022-01-13', '2022-01-14', '2022-01-15', '2022-01-16', '2022-01-17', '2022-01-18', '2022-01-19', '2022-01-20', '2022-01-21']
```
<a name="EhDMY"></a>
### 生成一段时间区间内的日期
```python
import datetime

def create_assist_date(datestart = None,dateend = None):
    # 创建日期辅助表

    if datestart is None:
        datestart = '2016-01-01'
    if dateend is None:
        dateend = datetime.datetime.now().strftime('%Y-%m-%d')

    # 转为日期格式
    datestart=datetime.datetime.strptime(datestart,'%Y-%m-%d')
    dateend=datetime.datetime.strptime(dateend,'%Y-%m-%d')
    date_list = []
    date_list.append(datestart.strftime('%Y-%m-%d'))
    while datestart<dateend:
        # 日期叠加一天
        datestart+=datetime.timedelta(days=+1)
        # 日期转字符串存入列表
        date_list.append(datestart.strftime('%Y-%m-%d'))
    return date_list

d_list = create_assist_date(datestart='2021-12-27', dateend='2021-12-30')
d_list
```
Output:
```python
['2021-12-27', '2021-12-28', '2021-12-29', '2021-12-30']
```
<a name="AtxdX"></a>
## 保存数据到CSV
保存数据到 CSV 是太常见的操作了
```python
def save_data(data, date):
    if not os.path.exists(r'2021_data_%s.csv' % date):
        with open("2021_data_%s.csv" % date, "a+", encoding='utf-8') as f:
            f.write("标题,热度,时间,url\n")
            for i in data:
                title = i["title"]
                extra = i["extra"]
                time = i['time']
                url = i["url"]
                row = '{},{},{},{}'.format(title,extra,time,url)
                f.write(row)
                f.write('\n')
    else:
        with open("2021_data_%s.csv" % date, "a+", encoding='utf-8') as f:
            for i in data:
                title = i["title"]
                extra = i["extra"]
                time = i['time']
                url = i["url"]
                row = '{},{},{},{}'.format(title,extra,time,url)
                f.write(row)
                f.write('\n')
```
<a name="c01Vg"></a>
## 带背景颜色的 Pyecharts
Pyecharts 作为 Echarts 的优秀 Python 实现，受到众多开发者的青睐，用 Pyecharts 作图时，使用一个舒服的背景也会给图表增色不少<br />以饼图为例，通过添加 JavaScript 代码来改变背景颜色
```python
def pie_rosetype(data) -> Pie:
    background_color_js = (
    "new echarts.graphic.LinearGradient(0, 0, 0, 1, "
    "[{offset: 0, color: '#c86589'}, {offset: 1, color: '#06a7ff'}], false)"
)
    c = (
        Pie(init_opts=opts.InitOpts(bg_color=JsCode(background_color_js)))
        .add(
            "",
            data,
            radius=["30%", "75%"],
            center=["45%", "50%"],
            rosetype="radius",
            label_opts=opts.LabelOpts(formatter="{b}: {c}"),
        )
        .set_global_opts(title_opts=opts.TitleOpts(title=""),
                        )
    )
    return c
```
<a name="anKgl"></a>
## requests 库调用
据统计，requests 库是 Python 家族里被引用的最多的第三方库，足见其江湖地位之高大！
<a name="fC4wF"></a>
### 发送 GET 请求
```python
import requests

headers = {
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36',
  'cookie': 'some_cookie'
}
response = requests.request("GET", url, headers=headers)
```
<a name="nfOLe"></a>
### 发送 POST 请求
```python
import requests


payload={}
files=[]
headers = {
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36',
  'cookie': 'some_cookie'
}
response = requests.request("POST", url, headers=headers, data=payload, files=files)
```
<a name="XFAdE"></a>
### 根据某些条件循环请求，比如根据生成的日期
```python
def get_data(mydate):
    date_list = create_assist_date(mydate)
    url = "https://test.test"
    files=[]
    headers = {
        'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36',
        'cookie': ''
        }
    for d in date_list:
        payload={'p': '10',
        'day': d,
        'nodeid': '1',
        't': 'itemsbydate',
        'c': 'node'}
        for i in range(1, 100):
            payload['p'] = str(i)
            print("get data of %s in page %s" % (d, str(i)))
            response = requests.request("POST", url, headers=headers, data=payload, files=files)
            items = response.json()['data']['items']
            if items:
                save_data(items, d)
            else:
                break
```
<a name="HJ3BW"></a>
## Python 操作各种数据库
<a name="PU4c6"></a>
### 操作 Redis
<a name="LgS8P"></a>
#### 连接 Redis
```python
import redis

def redis_conn_pool():
    pool = redis.ConnectionPool(host='localhost', port=6379, decode_responses=True)
    rd = redis.Redis(connection_pool=pool)
    return rd
```
<a name="FTisO"></a>
#### 写入 Redis
```python
from redis_conn import redis_conn_pool

rd = redis_conn_pool()
rd.set('test_data', 'mytest')
```
<a name="dH9e3"></a>
### 操作 MongoDB
<a name="Vocgw"></a>
#### 连接 MongoDB
```python
from pymongo import MongoClient

conn = MongoClient("mongodb://%s:%s@ipaddress:49974/mydb" % ('username', 'password'))
db = conn.mydb
mongo_collection = db.mydata
```
<a name="TVnnK"></a>
#### 批量插入数据
```python
res = requests.get(url, params=query).json()
commentList = res['data']['commentList']
mongo_collection.insert_many(commentList)
```
<a name="MyWPA"></a>
### 操作 MySQL
<a name="Sk5Ch"></a>
#### 连接 MySQL
```python
import MySQLdb

# 打开数据库连接
db = MySQLdb.connect("localhost", "testuser", "test123", "TESTDB", charset='utf8' )

# 使用cursor()方法获取操作游标 
cursor = db.cursor()
```
<a name="b7eBi"></a>
#### 执行 SQL 语句
```python
# 使用 execute 方法执行 SQL 语句
cursor.execute("SELECT VERSION()")

# 使用 fetchone() 方法获取一条数据
data = cursor.fetchone()

print "Database version : %s " % data

# 关闭数据库连接
db.close()
```
Output:
```python
Database version : 5.0.45
```
<a name="USjMZ"></a>
## 本地文件整理
整理文件涉及需求的比较多，这里分享的是将本地多个 CSV 文件整合成一个文件
```python
import pandas as pd
import os


df_list = []
for i in os.listdir():
    if "csv" in i:
        day = i.split('.')[0].split('_')[-1]
        df = pd.read_csv(i)
        df['day'] = day
        df_list.append(df)
df = pd.concat(df_list, axis=0)
df.to_csv("total.txt", index=0)
```
<a name="bvjiT"></a>
## 多线程代码
多线程也有很多实现方式，选择自己最为熟悉顺手的方式即可
```python
import threading
import time

exitFlag = 0

class myThread (threading.Thread):
    def __init__(self, threadID, name, delay):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.delay = delay
    def run(self):
        print ("开始线程：" + self.name)
        print_time(self.name, self.delay, 5)
        print ("退出线程：" + self.name)

def print_time(threadName, delay, counter):
    while counter:
        if exitFlag:
            threadName.exit()
        time.sleep(delay)
        print ("%s: %s" % (threadName, time.ctime(time.time())))
        counter -= 1

# 创建新线程
thread1 = myThread(1, "Thread-1", 1)
thread2 = myThread(2, "Thread-2", 2)

# 开启新线程
thread1.start()
thread2.start()
thread1.join()
thread2.join()
print ("退出主线程")
```
<a name="lRHb4"></a>
## 异步编程代码
异步爬取网站
```python
import asyncio
import aiohttp
import aiofiles

async def get_html(session, url):
    try:
        async with session.get(url=url, timeout=8) as resp:
            if not resp.status // 100 == 2:
                print(resp.status)
                print("爬取", url, "出现错误")
            else:
                resp.encoding = 'utf-8'
                text = await resp.text()
                return text
    except Exception as e:
        print("出现错误", e)
        await get_html(session, url)
```
使用异步请求之后，对应的文件保存也需要使用异步，即是一处异步，处处异步
```python
async def download(title_list, content_list):
    async with aiofiles.open('{}.txt'.format(title_list[0]), 'a',
                             encoding='utf-8') as f:
        await f.write('{}'.format(str(content_list)))
```
