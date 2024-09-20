Tomcat
<a name="cAZvl"></a>
## 问题背景
在项目部署后，由于网络等原因的限制，导致前端较大的资源文件加载时间特别长，特别是Vue相关的项目，较大的js文件更多，这时就可以考虑开启gizp，把较大的文件在传输时进行压缩，提高加载速度，注意Tomcat5.0以上才支持gzip。
<a name="nkww2"></a>
## 配置说明
<a name="YMcof"></a>
### 一、Tomcat下conf/server.xml文件
找到HTTP/1.1的Connector标签，添加如下配置
```xml
<Connector port="8091" protocol="HTTP/1.1" connectionTimeout="20000" redirectPort="8443" compression="on" noCompressionUserAgents="gozilla, traviata" compressionMinSize="2048" compressableMimeType="text/html,text/xml,text/javascript,application/javascript,text/css,text/plain,text/json"/>
```
说明：<br />1、`compression` 开启gizp压缩，默认关闭<br />2、`noCompressionUserAgents` 对于这两种浏览器不进行压缩<br />3、`compressionMinSize` 大于2KB的文件才进行压缩，对资源压缩时会对服务器消耗一定的cpu性能，对2KB以上的资源才进行压缩是官方给出的建议，实际使用时可以根据自己实际需要在响应时间和cpu性能之间做调整<br />4、`compressableMimeType` 配置压缩的文件格式<br />5、注：图片类型的文件一定不要压缩，大多数图片本身已是压缩文件，无法再使用gzip压缩，如果强行使用，不仅不会压缩文件，反而可能会使文件变大
<a name="VmHLD"></a>
### 二、Tomcat下conf/web.xml文件
此处配置根据自己实际需求选择是否配置<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695108752471-b21b7ed7-68ec-4abd-8e2f-a6c509115522.png#averageHue=%23f7f7f7&clientId=uc45b78df-48d3-4&from=paste&height=351&id=u810e4bb3&originHeight=878&originWidth=1677&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=199574&status=done&style=none&taskId=uba92ba7e-4cb9-4007-ad7b-7cc80cf44fa&title=&width=670.8)
```xml
<servlet>
  <servlet-name>default</servlet-name>
  <servlet-class>org.apache.catalina.servlets.DefaultServlet</servlet-class>
  <init-param>
    <param-name>debug</param-name>
    <param-value>0</param-value>
  </init-param>
  <init-param>
    <param-name>listings</param-name>
    <param-value>false</param-value>
  </init-param>
  <init-param>
    <param-name>sendfileSize</param-name>
    <param-value>2048</param-value>
  </init-param>
  <load-on-startup>1</load-on-startup>
</servlet>
```
说明：

- sendfileSize 不是用文件压缩直接发送文件的最小大小限制，默认48kb，该配置是使http在发送文件时保证和文件压缩之间有一个权衡
- 文件压缩：可以保证你的带宽，提高资源加载速度
- 发送文件大小限制：可以保证服务器CUP性能
- 如果当前服务端支持sendfile功能，sendfile的配置将优先于压缩，具体表现形式是默认将大于48kb的文件直接发送，
- 即使开启了压缩配置也不会进行压缩
- 如果想压缩较大的文件，需要在此配置发送文件的最小限制，保证该大小以下的文件都会进行压缩
<a name="fBN40"></a>
## Gzip压缩简介（扩展）

1. HTTP 协议支持GZIP 压缩机制，也称协议压缩。 HTTP GZIP压缩是由WEB服务器和浏览器共同遵守的协议，也就是说WEB服务器和浏览器都必须遵守。目前主流的服务器和浏览器都支持GZIP压缩技术。包括 IE、FireFox、Opera 等；服务器有 Tomcat、Apache 和 IIS 等。
2. GZIP 主要用来压缩html,css,javascript等静态文本文件，也支持对动态生成的，包括CGI、PHP , JSP , ASP , Servlet,SHTML等输出的网页也能进行压缩。
3. GZIP 压缩的比率通常在3~10 倍之间，这样可以大大节省服务器的网络带宽，大大提升浏览器的浏览速度。
4. GZIP 是一种数据压缩格式，默认且目前仅使用deflate算法压缩data部分；deflate是一种压缩算法,是huffman编码的一种加强。
5. 协议压缩就是依据HTTP协议进行压缩，不需要程序员进行压缩，解压编码，而是把压缩过程交给WEB服务器，将解压过程交给客户端。 如果客户端为支持GZIP压缩的浏览器，那么解压过程也不需要程序员参与，浏览器会按照一定的规则自动进行解压缩；如果客户端为HttpClient ，那么就需要手动进行GZIP解码了。
6. 压缩过程：客户端发送http请求，如果请求头header中携带`Accept-Encoding: gzip,deflate` (现在的浏览器一般默认都是这样)，那么浏览器的意思是:服务器需要进行GZIP压缩,再看响应内容的类型是否满足服务器配置的需要压缩的类型，如果符合，那么WEB服务器在传输响应内容之前，会对响应内容进行压缩，并在响应头中添加`Content-Encoding gzip;`如果不符合，那么将不压缩，直接返回。
7. 解压过程：（浏览器）客户端接收到响应，如果响应头中包含Content-Encoding GZIP，那么浏览器会自动将响应内容进行GZIP解压缩，然后再呈现在页面上。如果不包含，那么将直接呈现在页面上。
8. GZIP的缺点。相对于没有进行GZIP的工程来说，使用GZIP要增加服务器压缩的压力（cpu消耗）、客户端解压缩的压力，故而对服务器的配置需求更高。另外压缩也要耗费时间，想占用更小的空间，得到高压缩比率，肯定要牺牲较长的时间；反之，如果时间较为宝贵，要求快速，那么所得的压缩比率一定较小，当然会占用更大的空间了（压缩比率=原内容大小/压缩后大小，压缩比率越大，则表明压缩后占用空间的压缩包越小），这就是物理空间与时间的矛盾。
