# Cookie

##  Cookie和Session的作用	

​		HTTP 协议是一无状态协议即每次服务端接收到客户端请求时都是一全新请求而服务器并不知道客户端历史请求记录；Session 和 Cookie 主要目的是为弥补 HTTP无状态特性；

​	

## Cookies 是什么

​		HTTP 协议中 Cookie 包括 `Web Cookie` 和`浏览器 Cookie`，其是服务器发送到 Web 浏览器的一小块数据；服务器发送到浏览器的 Cookie浏览器会存储并与下一请求一起发送到服务器；通常用于判断两个请求是否来自于同一个浏览器如用户保持登录状态。

> HTTP Cookie 机制是 HTTP 协议无状态的一种补充和改良



Cookie 主要用于下面三个目的：

- `会话管理`

​		登陆、购物车、游戏得分或者服务器应该记住的其他内容

- `个性化`

​		用户偏好、主题或者其他设置

- `追踪`

​		记录和分析用户行为

​		Cookie 曾经用于一般的客户端存储。虽然这是合法的，因为它们是在客户端上存储数据的唯一方法，但如今建议使用现代存储 API。Cookie 随每个请求一起发送，因此它们可能会降低性能（尤其是对于移动数据连接而言）。



### 创建 Cookie

​		当接收到客户端发出的 HTTP 请求时，服务器可以发送带有响应的 `Set-Cookie` 标头，Cookie 通常由浏览器存储，然后将 Cookie 与 HTTP 标头一同向服务器发出请求。



#### Set-Cookie 和 Cookie 标头

`Set-Cookie` HTTP 响应标头将 cookie 从服务器发到用户代理，下面是一发送 Cookie 例子：![1586267623403-dbe15d8a-6b74-4c12-bf13-00bcc850bd97](..\图片\1586267623403-dbe15d8a-6b74-4c12-bf13-00bcc850bd97.png)

此标头告诉客户端存储Cookie，现在随着对服务器每个新请求浏览器将用 Cookie 头将所有以前存储的 Cookie 发回服务器；

![1586267623502-0f26d6bc-a56b-4442-aa7b-4bca1ce297c5](..\图片\1586267623502-0f26d6bc-a56b-4442-aa7b-4bca1ce297c5.png)



​		有两种类型 Cookies，一种是Session Cookies另一种Persistent Cookies：

​				若Cookie 不包含到期日期则视为会话 Cookie而会话 Cookie 存储内存中永远不写入磁盘，当浏览器关闭时Cookie 将永久丢失；

​				若 Cookie 包含`有效期`则视为持久性 Cookie，到指定日期Cookie 将从磁盘中删除；

​				还有一种是 `Cookie  Secure 和  HttpOnly  标记；



下面依次来介绍一下：

#### 会话 Cookies

上面的示例创建的是会话 Cookie ，会话 Cookie 有个特征，客户端关闭时 Cookie 会删除，因为它没有指定`Expires`或 `Max-Age` 指令。

但是，Web 浏览器可能会使用会话还原，这会使大多数会话 Cookie 保持永久状态，就像从未关闭过浏览器一样。



#### 永久性 Cookies

永久性 Cookie 不会在客户端关闭时过期，而是在`特定日期（Expires）`或`特定时间长度（Max-Age）`外过期。例如

~~~python
Set-Cookie: id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT;
~~~



#### Cookie的 Secure 和 HttpOnly 标记

安全的 Cookie 需要经过 HTTPS 协议通过加密的方式发送到服务器。即使是安全的，也不应该将敏感信息存储在cookie 中，因为它们本质上是不安全的，并且此标志不能提供真正的保护。



**HttpOnly 的作用**

- 会话 Cookie 中缺少 HttpOnly 属性会导致攻击者可以通过程序(JS脚本、Applet等)获取到用户的 Cookie 信息，造成用户 Cookie 信息泄露，增加攻击者的跨站脚本攻击威胁。
- HttpOnly 是微软对 Cookie 做的扩展，该值指定 Cookie 是否可通过客户端脚本访问。
- 如果在 Cookie 中没有设置 HttpOnly 属性为 true，可能导致 Cookie 被窃取。窃取的 Cookie 可以包含标识站点用户的敏感信息，如 ASP.NET 会话 ID 或 Forms 身份验证票证，攻击者可以重播窃取的 Cookie，以便伪装成用户或获取敏感信息，进行跨站脚本攻击等。



### Cookie 的作用域

`Domain` 和 `Path` 标识定义了 Cookie 的作用域：即 Cookie 应该发送给哪些 URL。

`Domain` 标识指定了哪些主机可以接受 Cookie。如果不指定，默认为当前主机(**不包含子域名**）。如果指定了`Domain`，则一般包含子域名。

例如，如果设置 `Domain=mozilla.org`，则 Cookie 也包含在子域名中（如`developer.mozilla.org`）。

例如，设置 `Path=/docs`，则以下地址都会匹配：

- `/docs`
- `/docs/Web/`
- `/docs/Web/HTTP`



## Cookie的特点

- Cookie以键值对的格式进行信息的存储。
- Cookie基于域名安全，不同域名的Cookie是不能互相访问的，如访问itcast.cn时向浏览器中写了Cookie信息，使用同一浏览器访问baidu.com时，无法访问到itcast.cn写的Cookie信息。
- 当浏览器请求某网站时，会将浏览器存储的跟网站相关的所有Cookie信息提交给网站服务器。
- cookie是有过期时间的，如果不指定，默认关闭浏览器之后cookie就会过期。
- cookie是由服务器生成，存储在浏览器端的一小段文本信息。



**典型应用：记住用户名，网站的广告推送。**

![1580484855932-95d2a88d-7467-4e2e-b2af-98eb5c17589c](..\图片\1580484855932-95d2a88d-7467-4e2e-b2af-98eb5c17589c.png)

## 设置Cookie

1）打开booktest/views.py文件，创建视图set_cookie

~~~python 
def set_cookie(request):
    '''设置cookie信息'''
    response = HttpResponse('设置cookie')
    # 设置一个cookie信息,名字为num, 值为1，保存时间2周
    response.set_cookie('num', 1, max_age=14*24*3600)
    # 返回response
    return response
~~~

2）打开booktest/urls.py文件，配置url。

~~~python 
url(r'^set_cookie$',views.set_cookie),
~~~

3）在浏览器输入如下网址。

~~~python
http://127.0.0.1:8000/set_cookie/
~~~

![1580484774671-86d8c8eb-d119-4f26-82db-016414a95352](..\图片\1580484774671-86d8c8eb-d119-4f26-82db-016414a95352.png)



## 读取Cookie

Cookie信息被包含在请求头中，使用request对象的COOKIES属性访问。

1）打开booktest/views.py文件，创建视图get_cookie

~~~ python
def get_cookie(request):
    '''获取cookie的信息'''
    # 取出cookie num的值
    num = request.COOKIES['num']
    return HttpResponse(num)
~~~

2）打开booktest/urls.py文件，配置url。

~~~python
url(r'^get_cookie$',views.get_cookie),
~~~

3）在浏览器输入如下网址。

~~~ python
http://127.0.0.1:8000/get_cookie/
~~~

![1580484783086-c1c976bc-9b48-4f6a-8f3d-68220e7424d7](..\图片\1580484783086-c1c976bc-9b48-4f6a-8f3d-68220e7424d7.png)