python入门学习<br />[https://github.com/jackfrued/Python-100-Days/tree/master/Day01-15](https://github.com/jackfrued/Python-100-Days/tree/master/Day01-15)

python官方文档<br />[https://docs.python.org/zh-cn/3.7/search.html?q=sys&check_keywords=yes&area=default](https://docs.python.org/zh-cn/3.7/search.html?q=sys&check_keywords=yes&area=default)
<a name="65b6f2b7fe9eed04527afc7f5e01c170"></a>
## 对象和类、继承
创建类和对象，简单的示例：<br />示例一：
```python
class Study():
    def test(x):
        print(x)

    def test2(a):
        print(2,a)


if __name__ == '__main__':
    a = input('请输入：')
    Study.test(a)
    Study.test2(a)
```
示例二：
```python
class Study:

    def __init__(self, name, age):
        self.name = name
        self.age = age

    def test2(self):
        print("年龄{},金钱{}".format(self.name,self.age))
    def test(self):
        print(self.name)


p = Study(1,2)
p.test2()
p.test()

输出：
-----------------------------------
年龄1,金钱2
1
```
继承<br />函数中需要self来进行实例化。
```python
class Study:

    def __init__(self, name, age):
        self.name = name
        self.age = age

    def test2(self):
        print("年龄{},金钱{}".format(self.name,self.age))
    def test(self):
        print(self.name)

class student(Study):
    def __init__(self,name,age,money):  # money是多出来的要进行修改的变量
        Study.__init__(self, name, age)  #调用父类
        self.money = money
    def test(self):  #复写父类的test方法
        print("%s,%d,%d"%(self.name,self.age,self.money))

p = Study(1,2)
p.test2()
p.test()

S = student('aaa', 18, 22222)
S.test()
```
<a name="71aadf5a43efcf8d1ab260b8703afe77"></a>
## 界面开发
基于tkinter类开发gui界面

command触发函数，实现功能。
```python
tkinter.Button(panel, text='修改', command=change_label_text)
```

```python
import tkinter
import tkinter.messagebox


def main():
    flag = True

    # 修改标签上的文字
    def change_label_text():
        nonlocal flag
        flag = not flag
        color, msg = ('red', 'Hello, world!')\
            if flag else ('blue', 'Goodbye, world!')
        label.config(text=msg, fg=color)

    # 确认退出
    def confirm_to_quit():
        if tkinter.messagebox.askokcancel('温馨提示', '确定要退出吗?'):
            top.quit()

    # 创建顶层窗口
    top = tkinter.Tk()
    # 设置窗口大小
    top.geometry('240x160')
    # 设置窗口标题
    top.title('小游戏')
    # 创建标签对象并添加到顶层窗口
    label = tkinter.Label(top, text='Hello, world!', font='Arial -32', fg='red')
    label.pack(expand=1)
    # 创建一个装按钮的容器
    panel = tkinter.Frame(top)
    # 创建按钮对象 指定添加到哪个容器中 通过command参数绑定事件回调函数
    button1 = tkinter.Button(panel, text='修改', command=change_label_text)
    button1.pack(side='left')
    button2 = tkinter.Button(panel, text='退出', command=confirm_to_quit)
    button2.pack(side='right')
    panel.pack(side='bottom')
    # 开启主事件循环
    tkinter.mainloop()


if __name__ == '__main__':
    main()
```
效果<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620120817809-0236ccb6-1cc7-4a2b-8185-37da2da46d40.png#align=left&display=inline&height=187&id=JJBr9&originHeight=249&originWidth=320&size=15230&status=done&style=none&width=240)<br />[https://blog.csdn.net/weixin_33739541/article/details/93964322?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control](https://blog.csdn.net/weixin_33739541/article/details/93964322?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control)

鱼C<br />[https://blog.csdn.net/sinat_41104353/article/details/79302618?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-13.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-13.control](https://blog.csdn.net/sinat_41104353/article/details/79302618?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-13.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-13.control)

[https://blog.csdn.net/qq_38830964/article/details/98665317](https://blog.csdn.net/qq_38830964/article/details/98665317)
<a name="6649418c7ef291aaf1e9a8d9e461d1f9"></a>
## 读写操作
读
```python
def main():
    f = None
    filename=r'C:\\Users\\19307\\Desktop\\1.txt'
    try:
        f = open(filename, 'r', encoding='utf-8')
        print(f.read())
    except FileNotFoundError:
        print('无法打开指定的文件!')
    except LookupError:
        print('指定了未知的编码!')
    except UnicodeDecodeError:
        print('读取文件时解码错误!')
    finally:
        if f:
            f.close()


if __name__ == '__main__':
    main()
```
写入
```python
f = open("/tmp/foo.txt", "w")

num = f.write( "Python 是一个非常好的语言。\n是的，的确非常好!!\n" )
print(num)
# 关闭打开的文件
f.close()
```
<a name="e30731d0de59975f8b459dba608c5b45"></a>
## 进行和线程
同时执行多个任务的方法：

一：启动多个进程，每个进程虽然只有一个线程，但多个进程可以一块执行多个任务。<br />二：是启动一个进程，在一个进程内启动多个线程，这样，多个线程也可以一块执行多个任务。<br />三：启动多个进程，每个进程再启动多个线程，这样同时执行的任务就更多了，当然这种模型更复杂，实际很少采用。


target=执行的函数，args=是对函数传入的值<br />t1 = threading.Thread(target=run_thread, args=(5,))<br />每次线程运行都需要start和join，防止堵塞。
```python
import time, threading

# 假定这是你的银行存款:
balance = 0

def change_it(n):
    # 先存后取，结果应该为0:
    global balance
    balance = balance + n
    balance = balance - n

def run_thread(n):
    for i in range(2000000):
        change_it(n)

t1 = threading.Thread(target=run_thread, args=(5,))
t1.start()
t1.join()
print(balance)
```
[https://blog.csdn.net/weixin_41598660/article/details/104286106](https://blog.csdn.net/weixin_41598660/article/details/104286106)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620217597143-10b66c0c-9efe-4a21-ae15-77f250557ea8.png#align=left&display=inline&height=494&id=LF9oK&originHeight=659&originWidth=716&size=46606&status=done&style=none&width=537)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620217640525-fb3c348b-48f1-45a5-89fe-ee2cf38885a3.png#align=left&display=inline&height=490&id=vvGQf&originHeight=654&originWidth=559&size=48215&status=done&style=none&width=419)<br />[https://github.com/jackfrued/Python-100-Days/blob/master/Day01-15/13.%E8%BF%9B%E7%A8%8B%E5%92%8C%E7%BA%BF%E7%A8%8B.md](https://github.com/jackfrued/Python-100-Days/blob/master/Day01-15/13.%E8%BF%9B%E7%A8%8B%E5%92%8C%E7%BA%BF%E7%A8%8B.md)

<a name="vXlWr"></a>
## 队列和线程
多线程容易死锁，防止有无锁的现象。可以用队列Queue模块。

需要把任务往队列里生成，比如下面20个，就是1-20生成在队列里头。需要用queue.put()，然后利用多线程去完成队列的任务。
```python
import threading
import time
import queue

# 下面来通过多线程来处理Queue里面的任务：
def work(q):
    while True:
        if q.empty(): #这一步很关键，如果缺乏这一点判断， 程序只是在简单的循环。
            return
        else:
            t = q.get()
            print("当前线程sleep {} 秒".format(t))

def main():
    q = queue.Queue()
    for i in range(20):
        q.put(i)  # 把任务放进队列里
    thread_num = 5
    threads = []
    for i in range(thread_num):
        t = threading.Thread(target=work, args=(q,))
        threads.append(t)
    # 创建5个线程
    for i in range(thread_num):
        threads[i].start()
    for i in range(thread_num):
        threads[i].join()

if __name__ == "__main__":
    start = time.time()
    main()
    print('耗时：', time.time() - start)

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622371945602-ed0153dc-3168-499d-a650-5cf9ff22ca4b.png#align=left&display=inline&height=356&originHeight=475&originWidth=701&size=66252&status=done&style=none&width=526)

多线程的代码示例
```python
import threading
import time


def func(i):           # 线程要执行的代码
    print("运行结果： 你新建了", i, "个对象")


def scan_thread():  # 线程函数
    threads = []    # 线程列表
    for i in range(100):
        threads.append(         # 添加线程
            threading.Thread(target=func, args=(i,))
        )

    for thread in threads:      # 遍历列表 启动线程
        thread.start()

    for thread in threads:      # 遍历列表 等待结束线程
        thread.join()


if __name__ == '__main__':
    print("开始时间：", time.time())
    scan_thread()    # 执行线程函数
    print("结束时间", time.time())
```
<a name="9vMnZ"></a>
## <br />
<a name="XIUcu"></a>
## Sys模块
命令行输入值<br />test1.py
```python
import sys

def abc(x):
    print('输入的值为: ',x)

if __name__ == '__main__':
    a = sys.argv[1]
    abc(a)
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620048724615-4c31e533-44f4-4c47-bf17-ebbae6bd7c19.png#align=left&display=inline&height=56&id=RzgGo&originHeight=75&originWidth=533&size=9532&status=done&style=none&width=400)

<a name="2ibza"></a>
## OS模块
```python
import os


#print(os.getcwd())
#print(sys.path)
command = input('请输入命令：')
a = os.system(command)
print(a)
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620049611113-072ba9cd-acba-43f7-ad3a-45213f07829b.png#align=left&display=inline&height=523&id=nPL8G&originHeight=696&originWidth=590&size=64896&status=done&style=none&width=443)<br />os模块还有切换目录、更改文件路径，文件名、删除目录等等功能点。<br />[https://blog.csdn.net/AiL598/article/details/83274449](https://blog.csdn.net/AiL598/article/details/83274449)

<a name="5lnqv"></a>
## requests模块
```python
import requests

def getTitle():
    url = input("请输入url：")
    #url = 'http://192.168.201.1/index.php'
    headers = {
            'Connection': 'close',
            'Cache-Control': 'max-age=0',
            'Upgrade-Insecure-Requests': '1',
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
            'Accept-Encoding':'gzip,deflate',
            'Accept-Charset':'ZWNobyAiSGVsbG8gYXVmZW5nIjs=',
            'Accept-Language': 'zh-CN,zh;q=0.8',
            }
    #req = requests.get(url=url,headers=headers)
    #r = req.text
    #print(r)
    try:
        req = requests.get(url=url,headers=headers)
        if req.status_code == 200:
            if 'aufeng' in req.text:
                print("存在phpstudy后门")
            else:
                print("没有phpstudy后门")
    except:
        print("请求出了问题")

if __name__ == '__main__':
    getTitle()
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620059224812-d6a981f3-d334-4137-9f1d-2a15a4b894d3.png#align=left&display=inline&height=81&id=wkb2r&originHeight=81&originWidth=528&size=10856&status=done&style=none&width=528)

如果有上传文件啥的，可以用files
```python
import requests
 
url = 'http://127.0.0.1:8080/upload'
files = {'file': ('test.txt', b'Hello Requests.')}     #必需显式的设置文件名
 
r = requests.post(url, files=files)
print(r.text)
```
[https://www.cnblogs.com/ranxf/p/7808537.html](https://www.cnblogs.com/ranxf/p/7808537.html)
<a name="eTyB4"></a>
## beautifulsoup4模块
```python
import re
from bs4 import BeautifulSoup

html_doc = """ #下面示例都是用此文本内容测试
<html><head><title>The Dormouse's story</title></head>
<body>
<p class="title"><b>The Dormouse's story</b></p>
<p class="story">Once upon a time there were three little sisters; and their names were
<a href="http://example.com/elsie" class="sister" id="link1">Elsie</a>
    ddd
<a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
<a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
and they lived at the bottom of a well.</p>
<p class="story">...</p>
<span>中文</span>
"""

soup=BeautifulSoup(html_doc,'lxml')
print(type(soup))
print(soup.find_all('span'))  #标签查找
print(soup.find_all('a') 输出所有a标签的值
print(soup.find_all('a',id='link1'))  #属性加标签过滤
print(soup.find_all('a',attrs={'class':'sister','id':'link3'})) #多属性
print(soup.find_all('p',class_='title'))  #class特殊性,此次传入的参数是**kwargs
print(soup.find_all(text=re.compile('Tillie')))  #文本过滤
print(soup.find_all('a',limit=2))  #限制输出数量
```
[https://www.cnblogs.com/zhangxinqi/p/9218395.html](https://www.cnblogs.com/zhangxinqi/p/9218395.html)
<a name="xs4G3"></a>
## csv模块
[https://blog.csdn.net/u014644167/article/details/89345085](https://blog.csdn.net/u014644167/article/details/89345085)<br />[https://blog.csdn.net/katyusha1/article/details/81606175](https://blog.csdn.net/katyusha1/article/details/81606175)<br />[https://www.cnblogs.com/superhin/p/11495956.html](https://www.cnblogs.com/superhin/p/11495956.html)
```python
import csv

header = ['name', 'password', 'status']

data = [
    ['abc', '123456', 'PASS'],
    ['张五', '123#456', 'PASS'],
    ['张#abc123', '123456', 'PASS'],
    ['666', '123456', 'PASS'],
    ['a b', '123456', 'PASS']
]

with open('result.csv', 'w', encoding='utf-8', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(header)
    writer.writerows(data)
```
<a name="yNNpO"></a>
## queue模块
Queue模块是队列模块，可以搭配多线程使用。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620389575920-a68f41a9-a69c-46ef-a9a8-c2b401eed6e2.png#align=left&display=inline&height=260&id=PWvht&originHeight=347&originWidth=919&size=26811&status=done&style=none&width=689)<br />需要把任务往队列里生成，比如下面20个，就是1-20生成在队列里头。需要用queue.put()，然后利用多线程去完成队列的任务。
```python
import threading
import time
import queue

# 下面来通过多线程来处理Queue里面的任务：
def work(q):
    while True:
        if q.empty():
            return
        else:
            t = q.get()
            print("当前线程sleep {} 秒".format(t))

def main():
    q = queue.Queue()
    for i in range(20):
        q.put(i)  # 把任务放进队列里
    thread_num = 5
    threads = []
    for i in range(thread_num):
        t = threading.Thread(target=work, args=(q,))
        threads.append(t)
    # 创建5个线程
    for i in range(thread_num):
        threads[i].start()
    for i in range(thread_num):
        threads[i].join()

if __name__ == "__main__":
    start = time.time()
    main()
    print('耗时：', time.time() - start)
```

<a name="6bHZo"></a>
## telnetlib模块
```python
import telnetlib

def duankousaomiao(ip,port):
    server = telnetlib.Telnet()
    try:
        server.open(ip,port)
        print('{}:{}'.format(ip,port))
    except:
        pass
    finally:
        server.close()

if __name__ == "__main__":
    duankousaomiao('127.0.0.1','135')
```
<a name="fTFRM"></a>
## <br />
<a name="UXRGv"></a>
## Python从json中提取数据
#json string:<br />s = json.loads('{"name":"test", "type":{"name":"seq", "parameter":["1", "2"]}}')<br />print s<br />print s.keys()<br />print s["name"]<br />print s["type"]["name"]<br />print s["type"]["parameter"][1]

<a name="E2uzK"></a>
## 列表推导式
```python
c = []
for b in range(1,5):
    c.apend(b)
print(c)

	||

c = [b for b in range(1,5)]
print(c)

====================输出
[1, 2, 3, 4]
[1, 2, 3, 4]
```






















<a name="RlQe0"></a>
## 需要实现的东西

有时候内网渗透，没办法传上其他工具，一般linux服务器自带python模块，可以写一个主机发现的脚本。端口探测的脚本。说不定可以逃过流量检测。 参考[https://www.cnblogs.com/nul1/p/9901942.html](https://www.cnblogs.com/nul1/p/9901942.html)

拿到一个OA，不用自己试，直接POC批量检测。<br />致远OA、通达OA、泛微OA

把域名批量筛CDN的脚本实现工具化，但是卡住了，主要是壳做出来之后，功能上的对接做不好，需要时间再去研究一下。  继续摸索一下[https://blog.csdn.net/rng_uzi_/article/details/89792518](https://blog.csdn.net/rng_uzi_/article/details/89792518)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620467799408-c24af36e-84ac-494d-b11a-5797c687a49a.png#align=left&display=inline&height=232&id=Ssi1O&originHeight=464&originWidth=626&size=21245&status=done&style=none&width=313)
