# Celery

> 以下demo比较简单，分布式场景请参考：https://www.cnblogs.com/wdliu/p/9517535.html



## 应用场景

用户发起request，并等待response返回，在这些views中，可能需要执行一段耗时的程序，那么用户就会等待很长时间，造成不好的用户体验，Celery多用来执行异步任务，将耗时的操作交由Celery去异步执行，比如发送邮件、下发装机任务等。



## Celery部分组成

1. **消息中间件（broker）：**Celery 用消息通信，通常使用中间人（Broker）在客户端和 worker 之前传递，这个过程从客户端向队列添加消息开始，之后中间人把消息派送给 worker。
2. **任务执行单元（worker）：**worker 是任务执行单元，是属于任务队列的消费者，它持续地监控任务队列，当队列中有新地任务时，它便取出来执行。worker 可以运行在不同的机器上，只要它指向同一个中间人即可，worker还可以监控一个或多个任务队列， Celery 是分布式任务队列的重要原因就在于 worker 可以分布在多台主机中运行。修改配置文件后不需要重启 worker，它会自动生效。
3. **任务执行结果存储（task result store）：**用来存储Worker执行的任务的结果，Celery支持以不同方式存储任务的结果，包括RabbitMQ, Redis等。

![1582177308599-5a9099d9-28eb-4f7f-ad70-fc01c18635d9](D:\Note\python\Django\图片\1582177308599-5a9099d9-28eb-4f7f-ad70-fc01c18635d9.png)



## 安装环境

~~~ python
pip install celery 
pip install redis
~~~



## 配置Celery

### 异步执行代码

tasks.py

~~~ python
from django.core.mail import send_mail
from django.conf import settings
from django.template import loader, RequestContext
from celery import Celery, platforms


# 初始化一个Celery类的实例对象
app = Celery('celery_tasks.tasks')

# 配置应用
app.conf.update(
    # 配置broker, 这里我们用redis作为broker
    BROKER_URL='redis://127.0.0.1:6379/1',
)

# 设置app自动加载任务
# 从已经安装的app中查找任务
app.autodiscover_tasks(settings.INSTALLED_APPS)


# 调用celery发邮件
@app.task
def send_register_active_email(to_email, username, token):
    '''发送激活邮件'''
    subject = '新浪私有云欢迎信息'
    message = ''
    sender = settings.EMAIL_FROM
    receiver = [to_email]
    html_message = '<h1>%s, 欢迎您成为新浪私有云注册用户</h1>请点击下面链接激活您的账户<br/><a href="http://127.0.0.1:8000/user/active/%s">http://127.0.0.1:8000/user/active/%s</a>' % (username, token, token)
    send_mail(subject, message, sender, receiver, html_message=html_message)
~~~



views.py

~~~ python
from celery_tasks.tasks import send_register_active_email


# 将send_register_active_email任务加入到celery队列中，如果my_task函数有参数，可通过delay()传递
send_register_active_email.delay(email, username, token)
~~~

**记住：**当有多个装饰器的时候，app.task一定要在最外层



**启动服务**

~~~ bash
celery -A tasks worker -Q queue -l info
~~~

![1587025587798-469999dd-9abb-4f92-a288-59899b382ecb](D:\Note\python\Django\图片\1587025587798-469999dd-9abb-4f92-a288-59899b382ecb.png)

- -A参数指定创建的celery对象的位置，该app.celery_tasks.celery指的是app包下面的celery_tasks.py模块的celery实例，注意一定是初始化后的实例，后面加worker表示该实例就是任务执行者
- -Q参数指的是该worker接收指定的队列的任务，这是为了当多个队列有不同的任务时可以独立；如果不设会接收所有的队列的任务
- -l参数指定worker输出的日志级别



任务执行完毕后结果存储在redis中，查看redis中的数据，发现存在一个string类型的键值对：

~~~ python
celery-task-meta-064e4262-e1ba-4e87-b4a1-52dd1418188f:data
~~~



### 存储任务结果

此处需要用到额外包django_celery_results, 先安装包:

~~~ shell
pip install django-celery-results
~~~



settings.py

~~~ python
INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'demo',
    'django_celery_results',  # 注意此处应用名为下划线
]
~~~



回到celery_demo/celery.py模块中，增加配置信息如下:

~~~ python 
from django.core.mail import send_mail
from django.conf import settings
from django.template import loader, RequestContext
from celery import Celery, platforms


# 初始化一个Celery类的实例对象
app = Celery('celery_tasks.tasks')

# 配置应用
app.conf.update(
    # 配置broker, 这里我们用redis作为broker
    BROKER_URL='redis://127.0.0.1:6379/1',
    # 使用项目数据库存储任务执行结果
    CELERY_RESULT_BACKEND='django-db',
)

# 设置app自动加载任务
# 从已经安装的app中查找任务
app.autodiscover_tasks(settings.INSTALLED_APPS)


# 调用celery发邮件
@app.task
def send_register_active_email(to_email, username, token):
    '''发送激活邮件'''
    subject = '新浪私有云欢迎信息'
    message = ''
    sender = settings.EMAIL_FROM
    receiver = [to_email]
    html_message = '<h1>%s, 欢迎您成为新浪私有云注册用户</h1>请点击下面链接激活您的账户<br/><a href="http://127.0.0.1:8000/user/active/%s">http://127.0.0.1:8000/user/active/%s</a>' % (username, token, token)

    send_mail(subject, message, sender, receiver, html_message=html_message)
~~~



创建django_celery_results应用所需数据库表, 执行迁移文件：

~~~ shell
python manage.py migrate django_celery_results
~~~



我这里使用的是django默认的数据库sqlit, 执行迁移之后，会在数据库中创建一张用来存储任务结果的表: 

![1587025714879-515d8504-45c8-4c2d-ae15-ab08bc7f60fe](D:\Note\python\Django\图片\1587025714879-515d8504-45c8-4c2d-ae15-ab08bc7f60fe.png)

![1587025714863-fae1e092-a509-4201-adb7-98875fff3002](D:\Note\python\Django\图片\1587025714863-fae1e092-a509-4201-adb7-98875fff3002.png)



再次从浏览器发送请求， 任务执行结束之后，将任务结果保存在数据库中: 

![1587025714887-de8c3aaf-b18e-43ea-b89e-8eb2b5711b1a](D:\Note\python\Django\图片\1587025714887-de8c3aaf-b18e-43ea-b89e-8eb2b5711b1a.png)



### 定时任务

如果我们想某日某时执行某个任务，或者每隔一段时间执行某个任务，也可以使用celery来完成

#### 1.安装

~~~ shell 
pip install django-celery
pip install django-redis==4.12.1
pip install celery==4.4.0
pip install django-celery-beat==2.1.0
pip install redis==3.5.3
~~~

**注意版本之间的兼容性**，之前就踩过很多坑，运行时发现报错，就是因为版本之间不兼容导致，本例中使用的版本：



#### 2.文件结构

我的项目名称叫hrms，首先在项目根目录下新建celery_tasks包，作为celery服务的根目录。

**config.py是celery的配置文件；**

**main.py是celery服务启动的入口文件；**
**update_hruser包是定时更新脚本的任务目录；**

![image](D:\Note\python\Django\图片\image.png)



**config.py文件**

~~~ python
from celery.schedules import crontab
import logging.config

# 指定任务队列的位置
BROKER_URL = "redis://:123456@10.10.1.10/2"
# 指定消息执行结果的位置
CELERY_RESULT_BACKEND = "redis://:123456@10.10.1.10/2"
# 一定要写下面这句，指定时区，否则celery默认使用utc时间，设置的hour会延迟8小时执行
CELERY_TIMEZONE = 'Asia/Shanghai'
CELERY_TASK_SERIALIZER = 'json'
CELERY_RESULT_SERIALIZER = 'json'
CELERY_ENABLE_UTC = True

# 这里博主列出了三个最典型的定时任务，更多用法自行百度crontab语法
CELERYBEAT_SCHEDULE = {
    # # 定时任务一:　每天的凌晨00:00，执行任务(refresh)
    '测试': {
        "task": "celery_tasks.update_hruser.tasks.refresh",
        'schedule': crontab(minute=0, hour=0),
        "args": ()
    },
    # # 定时任务二:每分钟，执行任务(hello_celery)
    # '测试2': {
    #     'task': 'celery_tasks.update_hruser.tasks.hello_celery',
    #     'schedule': crontab(hour='*/1'),
    #     'args': (2, 3)
    # },
}

from celery.schedules import crontab, timedelta



LOG_CONFIG = {
    'version': 1,
    'disable_existing_loggers': False,
    'formatters': {
        'simple': {
            'format': '%(asctime)s \"%(pathname)s：%(module)s:%(funcName)s:%(lineno)d\" [%(levelname)s]- %(message)s'
        }
    },
    'handlers': {
        'celery': {
            'level': 'INFO',
            'formatter': 'simple',
            'class': 'logging.handlers.RotatingFileHandler',
            'filename': 'celery.log',
            'encoding': 'utf-8',
        },
    },
    'loggers': {
        'celery': {
            'handlers': ['celery'],
            'level': 'INFO',
            'propagate': True,
        }
    }
}
logging.config.dictConfig(LOG_CONFIG)

~~~



**main.py文件**

~~~ python 
from celery import Celery
import os

# 为celery使用django配置文件进行设置，注意main.py和settings.py的相对位置关系
if not os.getenv('DJANGO_SETTINGS_MODULE'):
    os.environ['DJANGO_SETTINGS_MODULE'] = 'devops.settings'

# 创建celery对象,并起别名
app = Celery('devops')

# 从配置文件加载配置
app.config_from_object('celery_tasks.config')

# 此时用定时任务来触发celery任务，因此不需要再注册celery任务，后来我发现同时存在定时任务触发异步任务和手动触发异步任务时，
# 定时任务触发的异步任务必须要注册到这里才能生效，而且任务函数都需要 @ celery_app.task()
# 装饰器注册，不知道是我代码的问题还是celery的问题。
app.autodiscover_tasks(['celery_tasks.update_hruser.tasks.hello_celery', 'celery_tasks.update_hruser.tasks.refresh'])
~~~



**tasks.py文件**

~~~ python 
import sys
import os
import django

sys.path.append("/Users/zhangyingguang/Desktop/Sina-IDC-api/devops")
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "devops.settings")
django.setup()

from datetime import datetime, timedelta
from celery_tasks.main import app
from celery.utils.log import get_task_logger
from apps.assets.models import ApprovalInfo

logger = get_task_logger('celery_log')


@app.task(name='celery_tasks.update_hruser.tasks.refresh')
def refresh():
    ApprovalInfo.objects.filter(end_time=None).update(state=4)
    print("数据库更新成功")


@app.task(name='celery_tasks.update_hruser.tasks.hello_celery')
def hello_celery(x, y):
    print(x + y)
    
    
import json
import requests

# 叮叮机器人
@app.task()
def DingTask():
    #钉钉机器人的webhook地址
    url = 'https://oapi.dingtalk.com/robot/send?access_token=2d33d53383aaae6199e81d569c31d5d2f8e872b9e2d61c31a636ae31b8c108f4'
    headers = {
        'Content-Type': 'application/json',
        'Chartset': 'utf-8'
    }
 
    requests_data = {
        'msgtype': 'text',
        #发送的内容
        'text': {
            'content': '啤酒饮料矿泉水，花生瓜子八宝粥,没买的走了啊'
        },
        'at': {
            'atMobiles': [],
        },
        'isAtAll': True,
    }
 
    sendData = json.dumps(requests_data)
    response = requests.post(url, headers=headers, data=sendData)
    content = response.json()
    print(content)
~~~



#### 3.启动定时任务

切换到项目根目录下执行，也可以是使用supervisor

~~~ python
python manage.py runserver  启动django
celery worker -A celery_tasks.main -l debug -f  logs/celery.log   启动celery worker 服务
celery beat -A celery_tasks.main -l info  检查定时任务，并启动定时任务丢给worker执行。
~~~

进入钉钉选择设置钉钉机器人，如下位置就是钉钉机器人的webhook 

![1586261240978-86ec346c-4fa5-4286-b74b-a8960e85f9c3](D:\Note\python\Django\图片\1586261240978-86ec346c-4fa5-4286-b74b-a8960e85f9c3.png)



#### Celery常用的定时方法

~~~ sca
# 每2个小时中每分钟执行1次任务
crontab(hour='*/2')
 
# 每3个小时的0分时刻执行1次任务
# 即[0,3,6,9,12,15,18,21]点0分
crontab(minute=0, hour='*/3')
 
# 每3个小时或8点到12点的0分时刻执行1次任务
# 即[0,3,6,9,12,15,18,21]+[8,9,10,11,12]点0分
crontab(minute=0, hour='*/3,8-12')
 
# 每个季度的第1个月中，每天每分钟执行1次任务
# 月份范围是1-12，每3个月为[1,4,7,10]
crontab(month_of_year='*/3')
 
# 每月偶数天数的0点0分时刻执行1次任务
crontab(minute=0, hour=0, day_of_month='2-31/2')
 
# 每年5月11号的0点0分时刻执行1次任务
crontab(0, 0, day_of_month='11', month_of_year='5')
~~~



- **minute**：分钟，范围0-59；
- **hour**：小时，范围0-23；
- **day_of_week**：星期几，范围0-6。以星期天为开始，即0为星期天。这个星期几还可以使用英文缩写表示，例如“sun”表示星期天；
- **day_of_month**：每月第几号，范围1-31；
- **month_of_year**：月份，范围1-12。



这些参数可以设置表达式，表达稍微复杂的设置。默认值都是"*"星号，代表任意时刻。即crontab()相当与：

crontab(minute='*', hour='*', day_of_week='*', day_of_month='*', month_of_year='*')

含义是每天、每小时、每分钟执行一次任务，简单说就是每1分钟执行一次任务。



### Celery比Crontab有什么优势？

- Celery粒度比Cron更细。 Cron不能每分钟运行一次，而Celery可以（每隔90秒运行一次任务，检查电子邮件队列发送消息，另一个清除在线用户列表）。
- Cron行必须使用绝对路径和用户信息调用脚本或唯一命令。Celery调用Python函数，不需要编写代码。
- Celery要部署到另一台机器，通常只需要Push/Copy你的代码，然后Cron部署将需要更多的工作（您可以自动化但是...）



## 常用配置参数

~~~ python
# 注意，celery4版本后，CELERY_BROKER_URL改为BROKER_URL
BROKER_URL = 'amqp://username:passwd@host:port/虚拟主机名'

# 指定结果的存储地址
CELERY_RESULT_BACKEND = 'redis://username:passwd@host:port/db'

# 指定任务序列化方式
CELERY_TASK_SERIALIZER = 'msgpack' 

# 指定结果序列化方式
CELERY_RESULT_SERIALIZER = 'msgpack'

# 任务过期时间,celery任务执行结果的超时时间
CELERY_TASK_RESULT_EXPIRES = 60 * 20

# 指定任务接受的内容序列化类型(序列化)，一个列表
CELERY_ACCEPT_CONTENT = ["msgpack"]

# 任务发送完成是否需要确认，这一项对性能有一点影响     
CELERY_ACKS_LATE = True

# 压缩方案选择，可以是zlib, bzip2，默认是发送没有压缩的数据
CELERY_MESSAGE_COMPRESSION = 'zlib'

# 规定完成任务的时间
CELERYD_TASK_TIME_LIMIT = 5  # 在5s内完成任务，否则执行该任务的worker将被杀死，任务移交给父进程

# 设置worker的并发数，默认是服务器的内核数目,也是命令行-c参数指定的数目
CELERYD_CONCURRENCY = 4

# celery worker 每次去redis预取任务的数量
CELERYD_PREFETCH_MULTIPLIER = 4

# 每个worker最多执行100个任务被销毁，可以防止内存泄漏
CELERYD_MAX_TASKS_PER_CHILD = 100

# 设置默认的队列名称，如果一个消息不符合其他的队列就会放在默认队列里面，如果什么都不设置的话，数据都会发送到默认的队列中
CELERY_DEFAULT_QUEUE = "default"

# 设置详细的队列
CELERY_QUEUES = {
    "default": { # 这是上面指定的默认队列
        "exchange": "default",
        "exchange_type": "direct",
        "routing_key": "default"
    },
    "topicqueue": { # 这是一个topic队列 凡是topictest开头的routing key都会被放到这个队列
        "routing_key": "topic.#",
        "exchange": "topic_exchange",
        "exchange_type": "topic",
    },
    "task_eeg": { # 设置扇形交换机
        "exchange": "tasks",
        "exchange_type": "fanout",
        "binding_key": "tasks",
    },
}
~~~



如果你的项目中有多个任务，但每个任务执行的周期和执行的耗时都不一样，可以使用@task装饰器的time_limit或soft_time_limit属性来给每个任务设置超时时间。两者区别是：

- time_limit参数超时直接kill掉当前worker；
- soft_time_limit参数超时时会报错而不会kill当前worker，可以捕获。



~~~ python 
@app.task(name='tasks.main', bind=True, soft_time_limit=2, errback=error_fun)
def task_main(self, param):
    try:
        time.sleep(10)
    except SoftTimeLimitExceeded as e:
        print e
~~~

