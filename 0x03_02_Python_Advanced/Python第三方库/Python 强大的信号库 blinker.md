Python
<a name="RA9e1"></a>
## 1、信号
信号是一种通知或者说通信的方式，信号分为发送方和接收方。发送方发送一种信号，接收方收到信号的进程会跳入信号处理函数，执行完后再跳回原来的位置继续执行。<br />常见的 Linux 中的信号，通过键盘输入 Ctrl+C，就是发送给系统一个信号，告诉系统退出当前进程。<br />信号的特点就是发送端通知订阅者发生了什么。使用信号分为 3 步：定义信号，监听信号，发送信号。<br />![](./img/1637122424944-cb1def3a-3651-4c06-96c8-7d3cd582d01b.webp)<br />Python 中提供了信号概念的通信模块，就是blinker。<br />Blinker 是一个基于 Python 的强大的信号库，它既支持简单的点对点通信，也支持点对多点的组播。Flask 的信号机制就是基于它建立的。Blinker 的内核虽然小巧，但是功能却非常强大，它支持以下特性：

- 支持注册全局命名信号
- 支持匿名信号
- 支持自定义命名信号
- 支持与接收者之间的持久连接与短暂连接
- 通过弱引用实现与接收者之间的自动断开连接
- 支持发送任意大小的数据
- 支持收集信号接收者的返回值
- 线程安全
<a name="WdCbk"></a>
## 2、blinker 使用
安装方法：
```bash
pip install blinker
```
<a name="oF6fo"></a>
### 2.1 命名信号
```python
from blinker import signal

# 定义一个信号
s = signal('king')


def animal(args):
    print('我是小钻风，大王回来了，我要去巡山')

# 信号注册一个接收者
s.connect(animal)

if "__main__" == __name__:
    # 发送信号
    s.send()
```
![](./img/1637122424916-7bf5be47-598f-47db-8cfa-b1fff15eb99c.webp)
<a name="osXwH"></a>
### 2.2 匿名信号
blinker 也支持匿名信号，就是不需要指定一个具体的信号值。创建的每一个匿名信号都是互相独立的。
```python
from blinker import Signal

s = Signal()

def animal(sender):
    print('我是小钻风，大王回来了，我要去巡山')

s.connect(animal)

if "__main__" == __name__:
    s.send()
```
<a name="FzDUS"></a>
### 2.3 组播信号
组播信号是比较能体现出信号优点的特征。多个接收者注册到信号上，发送者只需要发送一次就能传递信息到多个接收者。
```python
from blinker import signal

s = signal('king')


def animal_one(args):
    print(f'我是小钻风，今天的口号是: {args}')

def animal_two(args):
    print(f'我是大钻风，今天的口号是: {args}')


s.connect(animal_one)
s.connect(animal_two)

if "__main__" == __name__:
    s.send('大王叫我来巡山，抓个和尚做晚餐！')
```
![](./img/1637122424879-f30c9212-9c5d-4e3b-9695-d42172387bb3.webp)
<a name="ycy9m"></a>
### 2.4 接收方订阅主题
接受方支持订阅指定的主题，只有当指定的主题发送消息时才发送给接收方。这种方法很好的区分了不同的主题。
```python
from blinker import signal

s = signal('king')


def animal(args):
    print(f'我是小钻风，{args} 是我大哥')

s.connect(animal, sender='大象')

if "__main__" == __name__:
    for i in ['狮子', '大象', '大鹏']:
        s.send(i)
```
![image.png](./img/1637122424788-73d386dc-35db-4079-99e4-b5c013445451.gif)
<a name="Rto9E"></a>
### 2.5 装饰器用法
除了可以函数注册之外还有更简单的信号注册方法，那就是装饰器。
```python
from blinker import signal

s = signal('king')

@s.connect
def animal_one(args):
    print(f'我是小钻风，今天的口号是: {args}')

@s.connect
def animal_two(args):
    print(f'我是大钻风，今天的口号是: {args}')

if "__main__" == __name__:
    s.send('大王叫我来巡山，抓个和尚做晚餐！')
```
<a name="TCFi9"></a>
### 2.6 可订阅主题的装饰器
`connect`的注册方法用着装饰器时有一个弊端就是不能够订阅主题，所以有更高级的`connect_via`方法支持订阅主题。
```python
from blinker import signal

s = signal('king')

@s.connect_via('大象')
def animal(args):
    print(f'我是小钻风，{args} 是我大哥')


if "__main__" == __name__:
    for i in ['狮子', '大象', '大鹏']:
        s.send(i)
```
<a name="MdVPG"></a>
### 2.7 检查信号是否有接收者
如果对于一个发送者发送消息前要准备的耗时很长，为了避免没有接收者导致浪费性能的情况，所以可以先检查某一个信号是否有接收者，在确定有接收者的情况下才发送，做到精确。
```python
from blinker import signal

s = signal('king')
q = signal('queue')


def animal(sender):
    print('我是小钻风，大王回来了，我要去巡山')

s.connect(animal)


if "__main__" == __name__:
    
    res = s.receivers
    print(res)
    if res:
        s.send()
    
    res = q.receivers
    print(res)
    if res:
        q.send()
    else:
        print("孩儿们都出去巡山了")
```
```python
{4511880240: <weakref at 0x10d02ae80; to 'function' at 0x10cedd430 (animal)>}
我是小钻风，大王回来了，我要去巡山
{}
孩儿们都出去巡山了
```
<a name="qRZdh"></a>
### 2.8 检查订阅者是否订阅了某个信号
也可以检查订阅者是否由某一个信号
```python
from blinker import signal

s = signal('king')
q = signal('queue')


def animal(sender):
    print('我是小钻风，大王回来了，我要去巡山')

s.connect(animal)


if "__main__" == __name__:
    
    res = s.has_receivers_for(animal)
    print(res)

    res = q.has_receivers_for(animal)
    print(res)
```
```python
True
False
```
<a name="lDqel"></a>
## 3、基于 blinker 的 Flask 信号
Flask 集成 blinker 作为解耦应用的解决方案。在 Flask 中，信号的使用场景如：请求到来之前，请求结束之后。同时 Flask 也支持自定义信号。
<a name="akEG6"></a>
### 3.1 简单 Flask demo
```python
from flask import Flask

app = Flask(__name__)

@app.route('/',methods=['GET','POST'],endpoint='index')
def index():
    return 'hello blinker'

if __name__ == '__main__':
    app.run()
```
访问127.0.0.1:5000时，返回给浏览器hello blinker。<br />![](./img/1637122424887-eb557415-d561-4dbc-9869-1e68ad6f27f9.webp)
<a name="JSo9J"></a>
### 3.2 自定义信号
因为 Flask 集成了信号，所以在 Flask 中使用信号时从 Flask 中引入。
```python
from flask import Flask
from flask.signals import _signals

app = Flask(__name__)

s = _signals.singal('msg')


def QQ(args):
    print('you have msg from QQ')

s.connect(QQ)

@app.route('/',methods=['GET','POST'],endpoint='index')
def index():
    s.send()
    return 'hello blinker'

if __name__ == '__main__':
    app.run()
```
![](./img/1637122425335-770ff4b1-915e-4053-b6ed-f5f6cb532ca1.webp)
<a name="h0UE6"></a>
### 3.3 Flask自带信号
在 Flask 中除了可以自定义信号，还可以使用自带信号。Flask 中自带的信号有很多种，具体如下：
<a name="vpr4l"></a>
#### 请求
```python
request_started = _signals.signal('request-started')                # 请求到来前执行
request_finished = _signals.signal('request-finished')              # 请求结束后执行
```
<a name="j7KUT"></a>
#### 模板渲染
```python
before_render_template = _signals.signal('before-render-template')  # 模板渲染前执行
template_rendered = _signals.signal('template-rendered')            # 模板渲染后执行
```
<a name="PrY47"></a>
#### 请求执行
```python
got_request_exception = _signals.signal('got-request-exception')    # 请求执行出现异常时执行
request_tearing_down = _signals.signal('request-tearing-down')      # 请求执行完毕后自动执行（无论成功与否）
appcontext_tearing_down = _signals.signal('appcontext-tearing-down') # 请求上下文执行完毕后自动执行（无论成功与否）
```
<a name="ewrVR"></a>
#### 请求上下文中
```python
appcontext_pushed = _signals.signal('appcontext-pushed')            # 请求上下文push时执行
appcontext_popped = _signals.signal('appcontext-popped')            # 请求上下文pop时执行

message_flashed = _signals.signal('message-flashed')                # 调用flask在其中添加数据时，自动触发
```
下面以请求到来之前为例，看 Flask 中信号如何使用
```python
from flask import Flask
from flask.signals import _signals, request_started
import time

app = Flask(__name__)

def wechat(args):
    print('you have msg from wechat')

# 从flask中引入已经定好的信号，注册一个函数
request_started.connect(wechat)

@app.route('/',methods=['GET','POST'],endpoint='index')
def index():
    return 'hello blinker'

if __name__ == '__main__':
    app.run()
```
当请求到来时，Flask 会经过`request_started` 通知接受方，就是函数`wechat`，这时`wechat`函数先执行，然后才返回结果给浏览器。![](./img/1637122425375-8182ccbb-4af3-46d7-9693-32ee72784105.webp)<br />但这种使用方法并不是很地道，因为信号并不支持异步方法，所以通常在生产环境中信号的接收者都是配置异步执行的框架，如 Python 中大名鼎鼎的异步框架 celery。
<a name="MonfL"></a>
## 4、总结
信号的优点：

1. 解耦应用：将串行运行的耦合应用分解为多级执行
2. 发布订阅者：减少调用者的使用，一次调用通知多个订阅者

信号的缺点：

1. 不支持异步
2. 支持订阅主题的能力有限
