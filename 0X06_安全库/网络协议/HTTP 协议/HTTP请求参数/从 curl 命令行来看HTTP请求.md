网络协议 HTTP<br />发送一个HTTP请求有很多途径，例如浏览器、curl命令行、wget命令行等。从curl命令行视角来讲解一个http请求中各个不同数据字段的含义<br />先来简单讲解一下 curl 命令。<br />curl 是一个用来请求 Web 服务器的命令行工具，-v 代表输出通信的整个过程，-d代表发送 一个POST 请求并在请求正文中带上指定数据，使用方式如下
```bash
curl -v -d 'name=hagan' http://www.fcant.com # 使用post方式请求url并带上数据
```
请求完整过程如下
```bash
Fcant@localhost life % curl -v -d 'name=hagan' http://www.fcant.com 
*   Trying 42.193.104.246...
* TCP_NODELAY set
* Connected to www.fcant.com (42.193.104.246) port 80 (#0)
> POST / HTTP/1.1
> Host: www.fcant.com
> User-Agent: curl/7.64.1
> Accept: */*
> Content-Length: 10
> Content-Type: application/x-www-form-urlencoded
> 
* upload completely sent off: 10 out of 10 bytes
< HTTP/1.1 301 Moved Permanently
< Server: nginx/1.18.0
< Date: Fri, 30 Jul 2021 01:26:20 GMT
< Content-Type: text/html
< Content-Length: 169
< Connection: keep-alive
< Location: https://www.fcant.com/
< 
<html>
<head><title>301 Moved Permanently</title></head>
<body>
<center><h1>301 Moved Permanently</h1></center>
<hr><center>nginx/1.18.0</center>
</body>
</html>
* Connection #0 to host www.fcant.com left intact
* Closing connection 0
```
详细剖析以上数据
<a name="XoDAG"></a>
# Request
Request代表发送请求部分，示例中Request部分代码如下
```bash
Fcant@localhost life % curl -v -d 'name=hagan' http://www.fcant.com 
*   Trying 42.193.104.246...
* TCP_NODELAY set
* Connected to www.fcant.com (42.193.104.246) port 80 (#0)
> POST / HTTP/1.1 # [请求类型] [请求URL] [协议版本]
> Host: www.fcant.com # 消息报头
> User-Agent: curl/7.64.1 # 消息报头
> Accept: */* # 消息报头
> Content-Length: 10 # 消息报头
> Content-Type: application/x-www-form-urlencoded # 消息报头
> 
* upload completely sent off: 10 out of 10 bytes
```
Request包含三个部分，分别为 请求行 消息报头(Header) 请求正文(Body)<br />下面的讲解请参考示例中Request部分代码来理解。
<a name="JjyCU"></a>
## 1.请求行
请求行包含三个部分，分别为 请求类型 请求URL 协议版本
```bash
> POST / HTTP/1.1 # [请求类型] [请求URL] [协议版本]
```
<a name="PjHc5"></a>
### 1.请求类型
本次请求的请求类型为 POST，常见的请求类型如下

- GET # 请求获取Request-URL所标识的资源
- POST # 在Request-URL所标识的资源后附加新的数据
- HEAD # 请求获取由Request-URL所标识的资源的响应消息报头
- PUT # 请求服务器存储一个资源并用Request-URL作为标识
- DELETE # 请求服务器删除Request-URL所标识的资源
- TRACE # 请求服务器回送收到的请求消息，主要用于测试或诊断
- CONNECT # 保留将来使用
- OPTIONS # 请求查询服务器性能、查询资源相关选项、预检请求
<a name="t5jhB"></a>
### 2.请求URL
本次请求的请求URL为 `/`
<a name="eO8OR"></a>
### 3.协议版本
本次请求的协议版本为HTTP 1.1，常见的协议版本如下

- HTTP/1.0
- HTTP/1.1
- HTTP/2.0
<a name="MHUhq"></a>
## 2.消息报头(Header)
消息报头有很多种字段类型，本次请求包含如下五个消息报头
```bash
> Host: www.fcant.com # 消息报头
> User-Agent: curl/7.64.1 # 消息报头
> Accept: */* # 消息报头
> Content-Length: 10 # 消息报头
> Content-Type: application/x-www-form-urlencoded # 消息报头
```
常见的消息报头如下
<a name="Qpybi"></a>
### Accept
指定客户端接受哪些类型的信息

- text/html # HTML文本
- image/gif # gif图片
- */* # 啥都行
<a name="vqmFQ"></a>
### Accept-Charset
客户端接受的字符集

- gb2312 # 中文字符
- iso-8859-1 # 西文字符集
- utf-8 # 多语言字符
<a name="xhnIY"></a>
### Accept-Encoding
可接受的内容编码

- gzip # 压缩类型
- deflate # 压缩类型
- identity # 默认
<a name="lWlf3"></a>
### Accept-Language
指定一种自然语言

- zh-cn # 中文
<a name="NxUSG"></a>
### Authorization
证明客户端有权查看某个资源
<a name="iEZRc"></a>
### Host
指定被请求资源的Internet主机和端口号

- fcant.com:8080
<a name="cwXpb"></a>
### User-Agent
用户代理

- 操作系统及版本
- CPU类型
- 浏览器及版本
- 浏览器渲染引擎
- 浏览器语言
- 浏览器插件
<a name="M7Z2V"></a>
### Content-Type
Body编码方式
<a name="to291"></a>
## 3.请求正文(Body)
请求正文有很多种编码方式，请求正文的类型需要根据消息报头的Content-Type字段来确定，本次请求的编码方式为application/x-www-form-urlencoded
```bash
> Content-Type: application/x-www-form-urlencoded # 消息报头
```
常见的Content-Type类型如下
<a name="je729"></a>
### application/x-www-form-urlencoded
默认数据编码方式<br />`?name=hagan&age=22`
<a name="a9SFA"></a>
### application/json
序列化后的JSON字符串
<a name="ZhRvG"></a>
### text/xml
XML作为编码方式的远程调用规范
<a name="j7x1u"></a>
### text/plain
数据以纯文本形式进行编码
<a name="Uyq9h"></a>
### multipart/form-data
允许body里面包含多个模块，每个模块可以是不同类型，常用于文件上传<br />需要规定一个用于分割模块的分隔符 boundary
```bash
Content-Type: multipart/form-data; boundary=hagan # boundary为分隔符
```
当指定分隔符为 hagan 时请求体格式如下
```bash
--hagan
Content-Disposition: form-data; name="fullname" # name为参数名
  
haganhan # 参数值 # 规范规定参数值前面必须有两个换行符
--hagan
Content-Disposition: form-data; name="head"; filename="head.png"
Content-Type: image/png # 指定类型

<Buffer 00> # 参数值
--hagan-- # 请求体结束
```
整个请求体拼装完成后 , 最后会以--分隔符--结尾 `--hagan--`
```bash
--分隔符(boundary)
Content-Disposition: form-data; name="参数名1"  

参数值1
--分隔符(boundary)
Content-Disposition: form-data; name="参数名2"  

参数值2
--分隔符(boundary)
Content-Disposition: form-data; name="参数名3"  

参数值3
--分隔符(boundary)
Content-Disposition: form-data; name="图片名1"; filename="图片文件名1"  
Content-Type: 类型  

图片文件的二进制内容1
--分隔符(boundary)
Content-Disposition: form-data; name="图片名2"; filename="图片文件名2"  
Content-Type: 类型  

图片文件的二进制内容2
--分隔符(boundary)
Content-Disposition: form-data; name="图片名3"; filename="图片文件名3"  
Content-Type: 类型  

图片文件的二进制内容3
--分隔符(boundary)--
```
<a name="zEpct"></a>
### application/octet-stream
只能通过流的方式提交一个二进制或文件
<a name="vhl2h"></a>
# Response
Response代表服务端响应部分，示例中Response部分代码如下
```bash
< HTTP/1.1 301 Moved Permanently # 状态行 # [协议版本] [状态码] [状态信息]
< Server: nginx/1.18.0 # 响应报头
< Date: Fri, 30 Jul 2021 01:26:20 GMT # 响应报头
< Content-Type: text/html # 实体报头
< Content-Length: 169 # 实体报头
< Connection: keep-alive # 实体报头
< Location: https://www.fcant.com/ # 响应报头
< 
<html>
<head><title>301 Moved Permanently</title></head>
<body>
<center><h1>301 Moved Permanently</h1></center>
<hr><center>nginx/1.18.0</center>
</body>
</html>
* Connection #0 to host www.fcant.com left intact
* Closing connection 0
```
Response也包含三个部分，分别为 状态行 消息报头(Header) 响应正文(Body)，下面将详细讲解这三部分。<br />下面的讲解请参考示例中Response部分代码来理解。
<a name="RHGQx"></a>
## 1.状态行
状态行包含三个部分，分别为 协议版本 状态码 状态信息
```bash
< HTTP/1.1 301 Moved Permanently # 状态行 # [协议版本] [状态码] [状态信息]
```
<a name="dTLIg"></a>
### 1.协议版本
本次响应的协议版本为 HTTP/1.1，常见协议版本如下

- HTTP/1.0
- HTTP/1.1
- HTTP/2.0
<a name="vL7OA"></a>
### 2.状态码
本次响应的状态码为 301，常见状态码如下
<a name="uHVrq"></a>
#### 1xx
指示信息，表示请求已接收，继续处理
<a name="vrobr"></a>
#### 2xx
成功，表示请求已被成功接收、理解、接受

- 200 OK，请求成功
- 201 Created，对于那些要服务器创建对象的请求来说，资源已创建完毕
- 202 Accepted，请求已接受，但服务器尚未处理
- 204 No Content，Response包含状态行和消息报头，但不包含响应正文
- 206 Partial Content，部分请求成功
<a name="RdL2D"></a>
#### 3xx
重定向，要完成请求必须进行更进一步的操作

- 301 请求永久重定向至新URL
- 302 请求临时重定向至新URL
<a name="zD29o"></a>
#### 4xx
客户端错误。请求有语法错误或请求无法实现

- 400 Bad Request，错误的请求
- 401 Unauthorized，需要客户端认证
- 403 Forbidden，请求被服务器拒绝
- 404 Not Found，未找到资源
<a name="cIIJs"></a>
#### 5xx
服务端错误，服务端未能实现合法的请求

- 500 Internal Server Error，内部服务器错误
- 503 Server Unavailable，服务器暂时无法提供服务，一段时间后便可恢复服务
<a name="WWX9x"></a>
### 3.状态信息
本次响应的状态信息为 Moved Permanently，常见状态信息如下

- OK
- Moved Permanently
- Not Found
<a name="XUEge"></a>
## 2.消息报头(Header)
Response中的消息报头主要分为 响应报头 和 实体报头
<a name="OF1Gw"></a>
### 响应报头
本次响应中的响应报头包含如下三个
```bash
< Server: nginx/1.18.0 # 响应报头
< Date: Fri, 30 Jul 2021 01:26:20 GMT # 响应报头
< Location: https://www.fcant.com/ # 响应报头
```
常见的响应报头如下
<a name="jFCJg"></a>
#### Location
重定向接受者到一个新的位置
<a name="USq23"></a>
#### WWW- Authenticate
包含在401响应消息中，客户端收到401
<a name="QxYwi"></a>
#### Server
包含了服务器用来处理请求的软件信息<br />Apache- Coyote/1.1
<a name="ao6Bt"></a>
### 实体报头
实体报头也叫正文报头，本次响应中的实体报头包含如下三个
```bash
< Content-Type: text/html # 实体报头
< Content-Length: 169 # 实体报头
< Connection: keep-alive # 实体报头
```
常见的实体报头如下
<a name="N71Np"></a>
#### Content-Encoding
编码类型是压缩还是非压缩<br />eg: Content-Encoding: gzip
<a name="mTdfQ"></a>
#### Content-Language
资源所用的自然语言，没有设置该域则认为实体内容将提供给所有的语言阅读
<a name="hRldH"></a>
#### Content-Length
正文的长度，以字节方式存储的十进制数字表示
<a name="bTeB1"></a>
#### Content-Type
响应正文的媒体类型

- 实体类型列表
<a name="dZnLf"></a>
#### Expires
响应过期的日期和时间
<a name="bRhm9"></a>
## 3.响应正文(Body)
响应正文有很多种类型，响应正文的类型需要根据消息报头的`Content-Type`字段来确定，本次响应的编码方式为`text/html`
```
< Content-Type: text/html # 实体报头
```
响应正文如下
```
<html>
<head><title>301 Moved Permanently</title></head>
<body>
<center><h1>301 Moved Permanently</h1></center>
<hr><center>nginx/1.18.0</center>
</body>
</html>
* Connection #0 to host www.fcant.com left intact
* Closing connection 0
```
curl命令只展示数据，不提供数据的解析，如果是浏览器，则会将响应正文以 `text/html` 的方式进行解析，此时便得到了一个HTML页面
