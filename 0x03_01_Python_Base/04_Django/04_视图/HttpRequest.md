# HttpRequest

## Http的报文结构

### HTTP请求报文头部

| 请求头            | 具体解释                                                     |
| :---------------- | :----------------------------------------------------------- |
| User-Agent        | 产生请求的浏览器类型                                         |
| Content-Type      | 客户端实际返回的内容的内容类型                               |
| Accept            | 客户端可识别的响应内容类型列表                               |
| Accept-Language   | 客户端可接受的自然语言                                       |
| Accept-Encoding   | 客户端可接受的编码压缩格式                                   |
| Accept-Charset    | 可接受的应答的字符集                                         |
| Host              | 请求的主机名，允许多个域名同处一个IP 地址，即虚拟主机(必须)  |
| Connection        | 连接方式(close 或 keep-alive)                                |
| Cookie            | 存储于客户端扩展字段，向同一域名的服务端发送属于该域的cookie |
| 请求包体          | POST方法中使用                                               |
| Referer           | 包含一个URL，用户从该URL代表的页面出发访问当前请求的页面     |
| If-Modified-Since | 文档的最后改动时间                                           |



### HTTP响应头

| 响应头           | 具体解释                                                     |
| ---------------- | ------------------------------------------------------------ |
| Allow            | 服务器支持哪些请求方法（如GET、POST等）                      |
| Content-Encoding | 文档的编码（Encode）方法                                     |
| Content-Length   | 表示内容长度。只有当浏览器使用持久HTTP连接时才需要这个数据。 |
| Content-Type     | 表示后面的文档属于什么MIME类型                               |
| Date             | 当前的GMT时间。你可以用setDateHeader来设置这个头以避免转换时间格式的麻烦。 |
| Expires          | 应该在什么时候认为文档已经过期，从而不再缓存它。             |
| Last-Modified    | 文档的最后改动时间。                                         |
| Refresh          | 表示浏览器应该在多少时间之后刷新文档，以秒计                 |
| Server           | 服务器名字                                                   |
| Set-Cookie       | 设置和页面关联的Cookie                                       |
| ETag             | 被请求变量的实体值。ETag是一个可以与Web资源关联的记号（MD5值） |
| Cache-Control    | 这个字段用于指定所有缓存机制在整个请求/响应链中必须服从的指令 |

> 服务器接收到http协议请求后会根据报文创建HttpRequest对象，此对象不需我们创建而直接使用服务器构造好的对象就可；
>
> **视图第一个参数须是HttpRequest对象**而在django.http模块中定义了HttpRequest对象API；



## 属性

> 下面除非特别说明，属性都是只读的。

| 属性             | 具体解释                                                     |
| ---------------- | ------------------------------------------------------------ |
| **path**         | 一个字符串，表示请求的页面的完整路径，不包含域名和参数部分   |
| **method**       | 一个字符串，表示请求使用的HTTP方法，常用值包括：'GET'、'POST' |
|                  | 在浏览器中给出地址发出请求采用get方式，如超链接。            |
|                  | 在浏览器中点击表单的提交按钮发起请求，如果表单的method设置为post则为post请求。 |
| **encoding**     | 一个字符串，表示提交的数据的编码方式                         |
|                  | 如果为None则表示使用浏览器的默认设置，一般为utf-8            |
|                  | 这个属性是可写的，可以通过修改它来修改访问表单数据使用的编码，接下来对属性的任何访问将使用新的encoding值。 |
| **GET**          | QueryDict类型对象，类似于字典，包含get请求方式的所有参数     |
| **POST**         | QueryDict类型对象，类似于字典，包含post请求方式的所有参数    |
| **FILES**        | 一个类似于字典的对象，包含所有的上传文件                     |
| **COOKIES**      | 一个标准的Python字典，包含所有的cookie，键和值都为字符串     |
| **SESSION**      | 一个既可读又可写的类似于字典的对象，表示当前的会话，只有当Django 启用会话的支持时才可用，详细内容见"状态保持" |
| **body**         | http请求的主体，二进制格式                                   |
| **data**         | drf中可以返回请求体的解析内容。类似于Django中标准的request.POST和request.FILES属性 |
| **query_params** | request.query_params 等同于 request.GET，不过其名字更加容易理解。为了代码更加清晰可读，推荐使用 request.query_params ，而不是 Django 中的 request.GET，这样那够让你的代码更加明显的体现出 ----- 任何 HTTP method 类型都可能包含查询参数（query parameters），而不仅仅只是 'GET' 请求。 |
| **parsers**      | APIView 类或者 @api_view 装饰器将根据视图上设置的 parser_classes 或 settings 文件中的 DEFAULT_PARSER_CLASSES 设置来确保此属性（.parsers）自动设置为 Parser 实例列表。**通常不需要关注该属性** |



## QueryDict对象

### 特点

1. 定义在django.http.QueryDict
2. HttpRequest对象的属性GET、POST都是QueryDict类型的对象
3. 与python字典不同，QueryDict类型的对象用来处理同一个键带有多个值的情况



### 使用方法

+ **get()：**根据键获取值
+ + 如果一个键同时拥有多个值将获取最后一个值

+ + 如果键不存在则返回None值，可以设置默认值进行后续处理

~~~ python 
request.POST.get['键']
~~~



+ **getlist()：**根据键获取值，值以列表返回，可以获取指定键的所有值
+ + 如果键不存在则返回空列表[]，可以设置默认值进行后续处理

~~~ python
request.POST.getlist('键')
~~~



## request

### 获取Header中的数据

1. request.META.get("header key") 用于获取header的信息
2. 注意的是header key必须增加前缀HTTP，同时大写，例如你的key为username，那么应该写成：request.META.get("HTTP_USERNAME")
3. 另外就是当你的header key中带有中横线，那么自动会被转成下划线，例如my-user的写成： request.META.get("HTTP_MY_USER")



### 获取Body中的Json数据

1. ensure_ascii=False表示返回值不会被编码成二进制
2. ret = json.dumps(ser.data, ensure_ascii=False)



### 获取Url中的数据

1. username = request.GET.get(**'username'**)

