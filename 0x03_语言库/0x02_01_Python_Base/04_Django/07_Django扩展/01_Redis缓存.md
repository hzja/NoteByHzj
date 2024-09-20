# Redis缓存

https://www.cnblogs.com/Ghost-bird/p/10455124.html

管理对一个redis server的所有连接，避免每次建立，释放连接的开销。默认，每个redis实例都会维护一个自己的连接池，可以直接建立一个连接池，作为参数传给redis，这样可以实现多个redis实例共享一个连接池。



## Python连接Redis

**第一步**：安装python操作redis模块

~~~ python
pip install redis
~~~

~~~ python
import redis

# decode_responses=True 自动解码
pool = redis.ConnectionPool(host='127.0.0.1', port=6379, db= 2, decode_responses=True)

r = redis.Redis(connection_pool=pool)

r.flushall() # 清空Redis

 
r.setex('name', value='natasha', time=2) # 设置新值，过期时间为3s
r.mset(k1 = 'v1', k2 = 'v2', k3 = 'v3') # 批量设置新值
print(r.mget('k1', 'k2', 'k3', 'k4')) # 批量获取新值
print(r.getset('name', 'natasha')) # 设置新值并获取原来的值
print(r.getrange('name', 0, 1)) # 获取子序列 0 <= x <= 1
r.setrange('name', 0, 'NATASHA') # 修改字符串内容，从指定字符串索引开始向后替换（新值太长时，则向后添加），返回值的长度
i = 0
while i < 2:
print(r.get('name'))
time.sleep(1)
i += 1
source = 'foo'
r.set('n1', source)
r.setbit('n1', 7, 1)

'''
注：如果在Redis中有一个对应： n1 = "foo"，
那么字符串foo的二进制表示为：01100110 01101111 01101111
所以，如果执行 setbit('n1', 7, 1)，则就会将第7位设置为1，
那么最终二进制则变成 01100111 01101111 01101111，即："goo"
'''

print(r.get('n1'))
print(r.getbit('n1', 7)) # 获取n1对应的值的二进制表示中的某位的值 （0或1）
r.set('n2', '娜塔莎')
print(r.strlen('n2')) # 返回对应的字节长度（一个汉字3个字节）
r.set('num', 1)
r.incr('num', amount=10)
r.decr('num', amount=1)
print(r.get('num')) # 自增num对应的值，当name不存在时，则创建name＝amount，否则，则自增。
r.append('num', 111)
print(r.get('num')) # 在redis num对应的值后面追加内容
~~~

~~~ python
[b'v1', b'v2', b'v3', None]
b'natasha'
b'goo'
b'NAtasha'
b'NAtasha'
b'NAtasha'
b'NAtasha'
b'goo'
1
7
b'10'
b'10111'
~~~



## Django连接Redis

官方文档 https://django-redis-chs.readthedocs.io/zh_CN/latest/

### 自定义连接池

按理说搞定上一步python代码操作redis之后，在django中应用只需要把上面的代码写到django就可以了。

例如：django的视图函数中操作redis

~~~ python
import redis
from django.shortcuts import HttpResponse

# 创建redis连接池
POOL = redis.ConnectionPool(host='10.211.55.28', port=6379, password='foobared', encoding='utf-8', max_connections=1000)

def index(request):
    
    # 去连接池中获取一个连接
    conn = redis.Redis(connection_pool=POOL)
    conn.set('name', "小明", ex=10)
    value = conn.get('name')
    print(value)
    return HttpResponse("ok")
~~~



### 第三方组件

上述可以实现在django中操作redis。**但是**，这种形式有点非主流，因为在django中一般不这么干，而是用另一种更加简便的的方式。

**第一步**：安装django-redis模块（内部依赖redis模块）

~~~ python 
pip3 install django-redis
~~~



**第二步**：在django项目的settings.py中添加相关配置

~~~python
# 上面是django项目settings中的其他配置....
CACHES = {
    'default': {
        'BACKEND': 'django_redis.cache.RedisCache',
        'LOCATION': 'redis://10.190.1.41:6379/1',
        "OPTIONS": {
            "CLIENT_CLASS": "django_redis.client.DefaultClient",
            # socket 建立连接超时设置
            "SOCKET_CONNECT_TIMEOUT": 5,
            # 连接建立后的读写操作超时设置
            "SOCKET_TIMEOUT": 5,
            "CONNECTION_POOL_KWARGS": {
                # 连接池最大数量
                "max_connections": 100,
                "encoding": 'utf-8'
            },
             "PASSWORD": "123456",
        },
    },
}
~~~



**第三步**：在django的视图中操作redis

~~~python 
from django.core.cache import cache


# timeout为过期时间，单位：秒，timeout=0为立即过期， timeout为None永不超时
cache.set('v', '555', 60*60)

# ttl搜索过期时间，返回值：0--标示key不存在或过期，
# None--key存在，但是没有设置过期时间
cache.ttl(key)

# expire指定一个key的过期时间
cache.expire(key, timeout)

# persist设置key永不过期
chache.persist(key)

# 判断key为v是否存在
cache.has_key('v')

# 获取key为v的缓存
cache.get('v')

# 删除key为v的缓存
cache.delete('v')

# 扫描key
cache.iter_keys("demo_*")
~~~



**第四步**：redis锁

~~~python
from django.core.cache import cache

with cache.lock(key):
    do_something()
~~~



### 全站缓存

使用中间件，经过一系列的认证等操作，如果内容在缓存中存在，则使用FetchFromCacheMiddleware获取内容并返回给用户； 	

当返回给用户之前，判断缓存中是否已经存在，如果不存在则UpdateCacheMiddleware会将缓存保存至缓存，从而实现全站缓存

~~~python 
MIDDLEWARE = [
    'django.middleware.cache.UpdateCacheMiddleware', # 写最上面
    # 其他中间件...
    'django.middleware.cache.FetchFromCacheMiddleware', # 写最下面
  ]
~~~



### 单独视图缓存

~~~ python
方式一：通过装饰器
from django.views.decorators.cache import cache_page

    class Personal_Assets_View(APIView):
        @cache_page(60 * 15)
        def get(self, request, *args, **kwargs):

            
方式二：通过url
from django.views.decorators.cache import cache_page

urlpatterns = [
    re_path(r'^assets/personal/list/$', cache_page(60 * 10)(views.Personal_Assets_View.as_view())),
]
~~~



### 局部页面缓存

1. 引入TemplateTag

~~~html
{% load cache %}
~~~



2. 使用缓存

~~~ html
{% cache 5000 缓存的key %}
    缓存内容
{% endcache %}
~~~

