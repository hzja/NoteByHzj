# Session

## Session是什么

​		客户端请求服务端，服务端会为这次请求开辟一块`内存空间`，这个对象便是 Session 对象，存储结构为 `ConcurrentHashMap`。Session 弥补了 HTTP 无状态特性，服务器可以利用 Session 存储客户端在同一个会话期间的一些操作记录



## Session如何判断是否同一会话

​		服务器第一次接收到请求时，开辟了一块 Session 空间（创建了Session对象），同时生成一个 sessionId ，并通过响应头的 Set-Cookie：JSESSIONID=XXXXXXX 命令，向客户端发送要求设置 Cookie 的响应；客户端收到响应后，在本机客户端设置了一个 JSESSIONID=XXXXXXX 的 Cookie 信息，该 Cookie 的过期时间为浏览器会话结束。

![1586267497590-d40b2dac-92e3-4fe5-92f1-4a1d7a8b43d0](D:\Note\python\Django\图片\1586267497590-d40b2dac-92e3-4fe5-92f1-4a1d7a8b43d0.png)

​		接下来客户端每次向同一个网站发送请求时，请求头都会带上该 Cookie 信息（包含 sessionId ）， 然后，服务器通过读取请求头中的 Cookie 信息，获取名称为 JSESSIONID 的值，得到此次请求的 sessionId。



## Session缺点

​		Session 机制有个缺点如 A 服务器存储了 Session，即做了负载均衡后如一段时间内 A 访问量激增会转发到 B 访问，但B 服务器并不存储 A 的 Session则会导致 Session 失效。



## 什么是Session Cookies

​		Session Cookies 也称为`会话 Cookies`，在Session Cookies 中用户登录状态保存在`服务器`的`内存`而用户登录时Session 被服务端安全创建；

​		每次请求时服务器会从会话 Cookie 中读取 SessionId，如服务端数据和读取 SessionId 相同则服务器发送响应给浏览器允许用户登录；

![1586267993220-fa0beb94-3b2d-42b7-ade4-55b5397a5b89](D:\Note\python\Django\图片\1586267993220-fa0beb94-3b2d-42b7-ade4-55b5397a5b89.png)



## Session特点

- session是以键值对进行存储的
- session依赖于cookie。唯一的标识码保存在sessionid cookie中
- session也是有过期时间，如果不指定，默认两周就会过期
- session存储在服务器端



**典型应用：**对于敏感、重要的信息，建议要储在服务器端，不能存储在浏览器中，如用户名、余额、等级、验证码等信息。![1580484960768-52f9cd93-a0a9-4923-a0dd-f972f7e4f2cd](D:\Note\python\Django\图片\1580484960768-52f9cd93-a0a9-4923-a0dd-f972f7e4f2cd.png)



## 依赖于Cookie

有请求者的Session都会存储在服务器中，服务器如何区分请求者和Session数据的对应关系呢？

答：在使用Session后，会在Cookie中存储一个sessionid的数据，每次请求时浏览器都会将这个数据发给服务器，服务器在接收到sessionid后，会根据这个值找出这个请求者的Session。

结果：如果想使用Session，浏览器必须支持Cookie，否则就无法使用Session了。

存储Session时，键与Cookie中的sessionid相同，值是开发人员设置的键值对信息，进行了base64编码，过期时间由开发人员设置。



### 禁用 Cookies，如何使用 Session ？

- 用户禁止cookie后，服务器的sessionId还会发给用户么？

会，服务器仍会将sessionId以cookie的方式发送给浏览器，但是，浏览器不再保存这个cookie(即sessionId)了。

- 如何设置才能在每个url后加上sessionId的值？

设置session.use_trans_sid = 1或者编译时打开打开了--enable-trans-sid选项

这样他会在每个url后面自动加上PHPSESSID的值，然后正常使用session就可以了。

- session的生命周期中特别注意的？

只要活动就不会过期：session是一个只要活动就不会过期的东西，只要开启cookie，每一次会话，session_id都不会改变



## 存储方式

打开test3/settings.py文件，设置SESSION_ENGINE项指定Session数据存储的方式，可以存储在数据库、缓存、Redis等。

1）存储在数据库中，如下设置可以写，也可以不写，这是默认存储方式

~~~ python
SESSION_ENGINE='django.contrib.sessions.backends.db'
~~~

2）存储在缓存中：存储在本机内存中，如果丢失则不能找回，比数据库的方式读写更快

~~~ python
SESSION_ENGINE='django.contrib.sessions.backends.cache'
~~~

3）混合存储：优先从本机内存中存取，如果没有则从数据库中存取

~~~ python
SESSION_ENGINE='django.contrib.sessions.backends.cached_db'
~~~

4）如果存储在数据库中，需要在项INSTALLED_APPS中安装Session应用

![1580485330452-8bc37a09-603e-40f6-b485-c67f6791e729](D:\Note\python\Django\图片\1580485330452-8bc37a09-603e-40f6-b485-c67f6791e729.png)

5）迁移后会在数据库中创建出存储Session的表

![1580485330451-704fe611-40d8-4f85-9d31-33930415cf8b](D:\Note\python\Django\图片\1580485330451-704fe611-40d8-4f85-9d31-33930415cf8b.png)

6）表结构如下图

![1580485330443-4c641f53-8f3b-4b12-8ff6-ba98efd02186](D:\Note\python\Django\图片\1580485330443-4c641f53-8f3b-4b12-8ff6-ba98efd02186.png)

由表结构可知操作Session包括三个数据：键，值，过期时间



## 设置session

1）打开booktest/views.py文件，创建视图set_session，代码如下：

~~~ python
def set_session(request):
    '''设置session'''
    request.session['username'] = 'smart'
    request.session['age'] = 18
    # request.session.set_expiry(5)
    return HttpResponse('设置session')
~~~

2）开booktest/urls.py文件配置url

~~~python
url(r'^set_session$',views.set_session),
~~~

3）运行服务器打开浏览器请求如下网址

~~~ python
http://127.0.0.1:8000/set_session/
~~~

4）浏览效果如下图，在"开发者工具"中可以查看到响应头中创建了Cookie，里面有个键为sessionid

![1580485486067-2c4f93aa-9f78-4fd1-a8f3-7897ab77407c](D:\Note\python\Django\图片\1580485486067-2c4f93aa-9f78-4fd1-a8f3-7897ab77407c.png)



## 读取session

1）打开booktest/views.py文件修改get_session视图如下：

~~~ python
def get_session(request):
    '''获取session'''
    username = request.session['username']
    age = request.session['age']
    return HttpResponse(username+':'+str(age))
~~~

2）打开booktest/urls.py文件配置url

~~~ python
url(r'^get_session$',views.get_session),
~~~

3）运行服务器打开浏览器请求如下网址

~~~ python
http://127.0.0.1:8000/get_session/
~~~

4）刷新浏览器效果如下图

![1580485607063-1296c1d3-91f0-401a-ad59-e55e7da0e433](D:\Note\python\Django\图片\1580485607063-1296c1d3-91f0-401a-ad59-e55e7da0e433.png)



## 删除Session

1）打开booktest/views.py文件修改clear_session视图如下

~~~ python
def clear_session(request):
    '''清除session信息'''
    request.session.clear()
    return HttpResponse('清除成功')
~~~



2）刷新浏览器在数据库命令行中查询，base64解码后如下图

![1580485768010-dd84bcc7-6237-4f85-ab6e-ed1ce6776083](D:\Note\python\Django\图片\1580485768010-dd84bcc7-6237-4f85-ab6e-ed1ce6776083.png)



3）清除所有Session

~~~ python
def clear_session(request):
    '''清除所有session信息'''
    request.session.flush()
    return HttpResponse('清除成功')
~~~



## 使用Redis存储Session

会话还支持文件、纯cookie、Memcached、Redis等方式存储，下面演示使用redis存储。

1）安装包

~~~ python
pip install django-redis-sessions==0.5.6
~~~

2）修改test3/settings文件增加如下项：

~~~ python
SESSION_ENGINE = 'redis_sessions.session'
SESSION_REDIS_HOST = 'localhost'
SESSION_REDIS_PORT = 6379
SESSION_REDIS_DB = 2
SESSION_REDIS_PASSWORD = ''
SESSION_REDIS_PREFIX = 'session'
~~~

3）打开booktest/views.py文件，修改session_test视图如下：

~~~ python
def session_test(request):
    request.session['h1']='hello'
    return HttpResponse('ok')
~~~

4）管理redis命令需要保证redis服务被开启

~~~ python
查看：ps ajx|grep redis
启动：sudo service redis start
停止：sudo service redis stop
使用客户端连接服务器：redis-cli
切换数据库：select 2
查看所有的键：keys *
获取指定键的值：get name
~~~

5）刷新浏览器在redis客户端中查看数据如下

![1580485963783-5a1bb9e1-3481-4866-8449-ee6e5ddb51bc](D:\Note\python\Django\图片\1580485963783-5a1bb9e1-3481-4866-8449-ee6e5ddb51bc.png)



# Session共享

## 1.基于Nginx的ip_hash负载均衡

​		这个方案实现最为简单，Session保存在后端服务器的内存中，只要hash属性是均匀的，多台web服务器的负载就是均衡的，安全性高，缺点：用户浏览器的IP地址hash以后满足单调性。会可能造成资源的分配不均衡，负载均衡就达不到到目的。有的服务器负载过重，有的服务器负载过轻，显然没有充分利用资源。



## 2. 基于数据库的Session共享

​		首选当然是大名鼎鼎的Mysql数据库，并且建议使用内存表Heap，提高session操作的读写效率。这个方案的实用性比较强，相信大家普遍在使用，它的缺点在于session的并发读写能力取决于Mysql数据库的性能，同时需要自己实现session淘汰逻辑，以便定时从数据表中更新、删除 session记录，当并发过高时容易出现表锁，虽然我们可以选择行级锁的表引擎，但不得不否认使用数据库存储Session还是有些杀鸡用牛刀的架势。



## 3. 将信息放到cookie放在客户端

​		session存在服务器端，会对服务器产生压力。如果将信息保存到cookie中，减轻了服务器的压力，同时每个客户端的压力也很小。因为只保存自己的信息。这种方式在实际的开发中广泛的采用。

​		缺点：cookie可以被禁用，cookie要随着浏览器传递，增大了传输的内容，cookie大小有限制。



## 4. 基于Redis做缓存session的统一缓存

　	其实就是把每次用户的请求的时候生成的sessionID给放到Redis的服务器上。然后在基于Redis的特性进行设置一个失效时间的机制，这样就能保证用户在我们设置的Redis中的session失效时间内，都不需要进行再次登录。



# **cookie和session区别**

**第一：**由于HTTP 协议是无状态的协议，当我们在开发一些对状态有要求的接口时候，cookie和session弥补了这一块的能力；



**第二：**对于http协议，cookie只是请求头中的一个字段，他跟http协议请求头中的其他字段并没有太大区别，当我们把它放在四层tcp的角度来看，那么http的请求头和请求体更没有什么大的区别；



**第三：**浏览器对cookie做了默认的支持，同时也限制了cookie，比如同源策略，同源策略是浏览器基于安全的角度的一个机制，限制了同域才能访问cookie的内容，对于同域的定义就是相同的协议、域名和端口，当我们在做单点登陆sso功能的时候，就会考虑把cookie种在可访问的域下面，比如把cookie种在一级域名下面。session是服务器为每一个web用户分配的独立的状态存储空间，那么这里有个问题，用户的状态信息存在的某台服务器上面了，当我们是单台服务器提供服务时，那么问题还好，但是当我们以七层或四层转发请求到后端集群的时候，就存在session命中的问题，这个时候我们需要通过将session信息存在mysql或者redis里面；



**回到问题上来，**cookie和session本质是辅助http协议的无状态性，所以服务端需要记录用户的状态时，就需要用某种机制来识具体的用户



链接：https://www.cnblogs.com/shenzikun1314/p/6961020.html

