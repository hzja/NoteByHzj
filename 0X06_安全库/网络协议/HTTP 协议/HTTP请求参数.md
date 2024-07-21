HTTP
<a name="eKIDZ"></a>
## HTTP Request的Header信息
<a name="LBocr"></a>
### 1、HTTP请求方式
如下表：

| GET | 向Web服务器请求一个文件 |
| --- | --- |
| POST | 向Web服务器发送数据让Web服务器进行处理 |
| PUT | 向Web服务器发送数据并存储在Web服务器内部 |
| HEAD | 检查一个对象是否存在 |
| DELETE | 从Web服务器上删除一个文件 |
| CONNECT | 对通道提供支持 |
| TRACE | 跟踪到服务器的路径 |
| OPTIONS | 查询Web服务器的性能 |

<a name="VJpWL"></a>
### 2、Host
请求的web服务器域名地址
<a name="BMHkT"></a>
### 3、User-Agent
HTTP客户端运行的浏览器类型的详细信息。通过该头部信息，web服务器可以判断到当前HTTP请求的客户端浏览器类别。<br />实例：
```
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.8.1.11) Gecko/20071127 Firefox/2.0.0.11
```
<a name="Tndp5"></a>
### 4、Accept
指定客户端能够接收的内容类型，内容类型中的先后次序表示客户端接收的先后次序。<br />例如：
```
Accept:text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5
```
备注：<br />在Prototyp（1.5）的Ajax代码封装中，将Accept默认设置为“text/javascript, text/html, application/xml, text/xml, */*”。这是因为Ajax默认获取服务器返回的Json数据模式。<br />在Ajax代码中，可以使用XMLHttpRequest 对象中setRequestHeader函数方法来动态设置这些Header信息。
<a name="8c536"></a>
### 5、Accept-Language
指定HTTP客户端浏览器用来展示返回信息所优先选择的语言。<br />实例：
```
Accept-Language: zh-cn,zh;q=0.5
```
这里默认为中文。
<a name="b6Nzd"></a>
### 6、Accept-Encoding
指定客户端浏览器可以支持的web服务器返回内容压缩编码类型。表示允许服务器在将输出内容发送到客户端以前进行压缩，以节约带宽。而这里设置的就是客户端浏览器所能够支持的返回压缩格式。<br />实例：
```
Accept-Encoding: gzip,deflate
```
备注：<br />其实在百度很多产品线中，apache在给客户端返回页面数据之前，将数据以gzip格式进行压缩。
<a name="4i872"></a>
### 7、Accept-Charset
浏览器可以接受的字符编码集。<br />实例：
```
Accept-Charset: gb2312,utf-8;q=0.7,*;q=0.7
```
<a name="U3EpH"></a>
### 8、Content-Type
显示此HTTP请求提交的内容类型。一般只有post提交时才需要设置该属性。<br />实例：
```
Content-type: application/x-www-form-urlencoded;charset:UTF-8
```
有关Content-Type属性值可以如下两种编码类型：<br />（1）“application/x-www-form-urlencoded”： 表单数据向服务器提交时所采用的编码类型，默认的缺省值就是“application/x-www-form-urlencoded”。 然而，在向服务器发送大量的文本、包含非ASCII字符的文本或二进制数据时这种编码方式效率很低。<br />（2）“multipart/form-data”： 在文件上载时，所使用的编码类型应当是“multipart/form-data”，它既可以发送文本数据，也支持二进制数据上载。<br />当提交为单单数据时，可以使用“application/x-www-form-urlencoded”；当提交的是文件时，就需要使用“multipart/form-data”编码类型。<br />在Content-Type属性当中还是指定提交内容的charset字符编码。一般不进行设置，它只是告诉web服务器post提交的数据采用的何种字符编码。<br />一般在开发过程，是由前端工程与后端UI工程师商量好使用什么字符编码格式来post提交的，然后后端ui工程师按照固定的字符编码来解析提交的数据。所以这里设置的charset没有多大作用。
<a name="CcjQP"></a>
### 9、Connection
表示是否需要持久连接。如果web服务器端看到这里的值为“Keep-Alive”，或者看到请求使用的是HTTP 1.1（HTTP 1.1默认进行持久连接），它就可以利用持久连接的优点，当页面包含多个元素时（例如Applet，图片），显著地减少下载所需要的时间。要实现这一点， web服务器需要在返回给客户端HTTP头信息中发送一个Content-Length（返回信息正文的长度）头，最简单的实现方法是：先把内容写入ByteArrayOutputStream，然 后在正式写出内容之前计算它的大小。<br />实例：
```
Connection: keep-alive
```
<a name="vitae"></a>
### 10、Keep-Alive
显示此HTTP连接的Keep-Alive时间。使客户端到服务器端的连接持续有效，当出现对服务器的后继请求时，Keep-Alive功能避免了建立或者重新建立连接。<br />以前HTTP请求是一站式连接，从HTTP/1.1协议之后，就有了长连接，即在规定的Keep-Alive时间内，连接是不会断开的。<br />实例：
```
Keep-Alive: 300
```
<a name="7PtBh"></a>
### 11、cookie
HTTP请求发送时，会把保存在该请求域名下的所有cookie值一起发送给web服务器。
<a name="Dts9W"></a>
### 12、Referer
包含一个URL，用户从该URL代表的页面出发访问当前请求的页面
<a name="1aXAs"></a>
## Responses 部分
| Header | 解释 | 示例 |
| --- | --- | --- |
| Accept-Ranges | 表明服务器是否支持指定范围请求及哪种类型的分段请求 | Accept-Ranges: bytes |
| Age | 从原始服务器到代理缓存形成的估算时间（以秒计，非负） | Age: 12 |
| Allow | 对某网络资源的有效的请求行为，不允许则返回405 | Allow: GET, HEAD |
| Cache-Control | 告诉所有的缓存机制是否可以缓存及哪种类型 | Cache-Control: no-cache |
| Content-Encoding | web服务器支持的返回内容压缩编码类型。 | Content-Encoding: gzip |
| Content-Language | 响应体的语言 | Content-Language: en,zh |
| Content-Length | 响应体的长度 | Content-Length: 348 |
| Content-Location | 请求资源可替代的备用的另一地址 | Content-Location: /index.htm |
| Content-MD5 | 返回资源的MD5校验值 | Content-MD5: Q2hlY2sgSW50ZWdyaXR5IQ== |
| Content-Range | 在整个返回体中本部分的字节位置 | Content-Range: bytes 21010-47021/47022 |
| Content-Type | 返回内容的MIME类型 | Content-Type: text/html; charset=utf-8 |
| Date | 原始服务器消息发出的时间 | Date: Tue, 15 Nov 2010 08:12:31 GMT |
| ETag | 请求变量的实体标签的当前值 | ETag: “737060cd8c284d8af7ad3082f209582d” |
| Expires | 响应过期的日期和时间 | Expires: Thu, 01 Dec 2010 16:00:00 GMT |
| Last-Modified | 请求资源的最后修改时间 | Last-Modified: Tue, 15 Nov 2010 12:45:26 GMT |
| Location | 用来重定向接收方到非请求URL的位置来完成请求或标识新的资源 | Location: http://www.zcmhi.com/archives/94.html |
| Pragma | 包括实现特定的指令，它可应用到响应链上的任何接收方 | Pragma: no-cache |
| Proxy-Authenticate | 它指出认证方案和可应用到代理的该URL上的参数 | Proxy-Authenticate: Basic |


