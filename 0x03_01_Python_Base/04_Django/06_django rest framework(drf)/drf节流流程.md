# drf节流流程

匿名用户通过ip地址来控制访问频率

已登录用户通过token来控制



## 实现原理

把所有登录记录时间放在一个列表中，当用户请求网页的时候，用现在的时间减去约束的时间间隔，然后把小于这个时间记录排除，再计算出时间间隙的记录条数，如果其中的条数小于规定的条数则可以访问并且把当前时间添加进列表中，如果大于或等于则不让其访问。



##  流程

1. 当用户请求网页的时候，后台允许该界面的url中的as_views()，运行了as_view方法，进入了APIView类的dispatch方法
2. 进入self.initial方法中的self.check_throttles(request)方法
3. 循环运行节流类中的allow_request方法

 \# 以上是drf源码内容，下面为自定义方法

1. 如果可以访问返回True，如果不能访问则返回False，之后返回check_throttles，如果是False则运行SimpleRateThrottle类中的wait方法得到需要等待的时间在页面上显示



## 返回值

- True，允许访问
- False，访问太频繁
  wait()返回值：返回一个整数，表示下次还有多久可以访问



## 示例

### 方法一：重写原生方法BaseThrottle.allow_request

utils/throttle.py

~~~ python
from rest_framework.throttling import BaseThrottle
import time

# 定义全局变量，用于存放访问记录
VISIT_RECORD = {}

# 需要继承BaseThrottle
class VisitThrottle(BaseThrottle):
    '''60s内只能访问3次'''

    def __init__(self): # 用于await计算剩余访问时间
        self.history = []

    # 在定义的类中复写allow_request方法，返回True或者False表示可以访问或者访问频率太高
    def allow_request(self, request, view):
        # 1.获取用户ip
        # 方法一：通过原生方法获取用户ip
        # remote_addr = request.META.get('REMOTE_ADDR')

        # 方法二：通过父方法获取用户唯一标识ip
        remote_addr = self.get_ident(request)
        ctime = time.time()
        # 这是用户第一次访问,将其进行记录，并且返回True，允许继续访问
        if remote_addr not in VISIT_RECORD:
            print("没有此IP")
            VISIT_RECORD[remote_addr] = [ctime,]
            return True
        # 如果不是第一次访问，获取所有的记录
        history = VISIT_RECORD.get(remote_addr)
        self.history = history

        # 判断最开始的时刻与现在的时刻的差值是否在规定的时间范围内，比如在60s内，如果不在，
        # 可以去除最开始的时刻记录
        while history and history[-1] < ctime - 60:
            # 删掉最后一个
            history.pop()
		# 此时列表中的时刻记录都是在规定的时间范围内，判断时刻的个数也就是访问的次数
        if len(history) < 3:
            history.insert(0, ctime)
            return True
        
	# 还需要等多少秒才能访问
    def wait(self):
        ctime = time.time()
        return 8-(ctime-self.history[-1])
~~~



1. **全局**

~~~ python
# settings.py
'DEFAULT_THROTTLE_CLASSES': ['apps.api.utils.throttle.VisitThrottle'],
~~~



2. 局部

~~~ python
class AuthView(APIView):
    """
    用于用户登录认证
    """
    throttle_classes = [VisitThrottle,]
~~~



### 方法二：继承内置类

utils/throttle.py

~~~ python
from rest_framework.throttling import  SimpleRateThrottle

# 对游客的限制
class VisitThrottle(SimpleRateThrottle):
    scope = "Vistor"
    def get_cache_key(self, request, view):
        # 唯一表示是IP
        return self.get_ident(request)
    
# 对登陆用户的限制
class UserThrottle(SimpleRateThrottle):
    scope = "User"
    def get_cache_key(self, request, view):
        # 唯一表示是用户名
        return self.user.username
~~~

scope从settings.py中寻找DEFAULT_THROTTLE_RATES字典的Key,就是访问频率限制，scope可以区分不同的函数的不同限制；get_cache_key(self, request, view)返回一个唯一标示用以区分不同的用户，对于匿名用户返回IP保存到缓存中限制访问，对于注册的用户取用户名（唯一）来区分就可以。



全局应用

~~~ python
REST_FRAMEWORK = {
    ...
    'DEFAULT_THROTTLE_RATES': {
        # 对游客的限制每分钟3次
        'Vistor': '3/m',
        # 对登陆用户的限制每分钟10次
        'User': '10/m'
    }
}
~~~

