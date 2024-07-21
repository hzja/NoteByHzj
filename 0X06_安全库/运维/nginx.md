<a name="vYyVU"></a>
# 一、http内容
<a name="Nid4D"></a>
## 1、请求方法
<a name="OFtsU"></a>
##### 1.OPTIONS：
返回服务器针对特定资源所支持的HTTP请求方法。也可以利用向Web服务器发送'*'的请求来测试服务器的功能性。
<a name="qjEFv"></a>
##### 2.HEAD：
向服务器索要与GET请求相一致的响应，只不过响应体将不会被返回。这一方法可以在不必传输整个响应内容的情况下，就可以获取包含在响应消息头中的元信息。
<a name="EXKIq"></a>
##### 3.PUT：
向指定资源位置上传其最新内容。
<a name="Extb1"></a>
##### 4.DELETE：
请求服务器删除Request-URI所标识的资源。
<a name="ipTZq"></a>
##### 5.TRACE：
回显服务器收到的请求，主要用于测试或诊断。
<a name="1p9cR"></a>
##### 6.CONNECT：
HTTP/1.1协议中预留给能够将连接改为管道方式的代理服务器。
<a name="OtZIG"></a>
##### 7.GET：
向特定的资源发出请求。
<a name="3q8fu"></a>
##### 8.POST：
向指定资源提交数据进行处理请求（例如提交表单或者上传文件）。数据被包含在请求体中。POST请求可能会导致新的资源的创建和/或已有资源的修改。

**重点掌握**<br />**get post put delete**
<a name="ucCWG"></a>
## 2、HTTP状态码分类
HTTP状态码由三个十进制数字组成，第一个十进制数字定义了状态码的类型，后两个数字没有分类的作用。HTTP状态码共分为5种类型：<br />HTTP状态码分类

| 分类 | 分类描述 |
| --- | --- |
| 1** | 信息，服务器收到请求，需要请求者继续执行操作 |
| 2** | 成功，操作被成功接收并处理 |
| 3** | 重定向，需要进一步的操作以完成请求 |
| 4** | 客户端错误，请求包含语法错误或无法完成请求 |
| 5** | 服务器错误，服务器在处理请求的过程中发生了错误 |

<a name="EmwWb"></a>
## 3、http消息结构
HTTP是基于客户端/服务端（C/S）的架构模型，通过一个可靠的链接来交换信息，是一个无状态的请求/响应协议。<br />一个HTTP"客户端"是一个应用程序（Web浏览器或其他任何客户端），通过连接到服务器达到向服务器发送一个或多个HTTP的请求的目的。<br />一个HTTP"服务器"同样也是一个应用程序（通常是一个Web服务，如Apache Web服务器或IIS服务器等），通过接收客户端的请求并向客户端发送HTTP响应数据。<br />HTTP使用统一资源标识符（Uniform Resource Identifiers, URI）来传输数据和建立连接。<br />一旦建立连接后，数据消息就通过类似Internet邮件所使用的格式[RFC5322]和多用途Internet邮件扩展（MIME）[RFC2045]来传送。

---

<a name="tPB21"></a>
## 4、客户端请求消息
客户端发送一个HTTP请求到服务器的请求消息包括以下格式：请求行（request line）、请求头部（header）、空行和请求数据四个部分组成，下图给出了请求报文的一般格式。
<a name="byTjJ"></a>
## ![](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601037880085-bd08daa1-5352-455d-8b8c-1d83027a13e3.png#height=165&id=SMJCj&originHeight=165&originWidth=466&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=466)

---

<a name="F6pvH"></a>
## 5、服务器响应消息
HTTP响应也由四个部分组成，分别是：状态行、消息报头、空行和响应正文。<br />![](https://cdn.nlark.com/yuque/0/2020/jpeg/2476579/1601037880080-07dc27e4-0d3b-4809-9987-534bb6afa702.jpeg#height=305&id=YrE2O&originHeight=305&originWidth=683&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=683)
<a name="mFIBU"></a>
## 6、HTTP 响应头信息
HTTP请求头提供了关于请求，响应或者其他的发送实体的信息。<br />在本章节中我们将具体来介绍HTTP响应头信息。

| 应答头 | 说明 |
| --- | --- |
| Allow | 服务器支持哪些请求方法（如GET、POST等）。 |
| Content-Encoding | 文档的编码（Encode）方法。只有在解码之后才可以得到Content-Type头指定的内容类型。利用gzip压缩文档能够显著地减少HTML文档的下载时间。Java的GZIPOutputStream可以很方便地进行gzip压缩，但只有Unix上的Netscape和Windows上的IE 4、IE 5才支持它。因此，Servlet应该通过查看Accept-Encoding头（即request.getHeader("Accept-Encoding")）检查浏览器是否支持gzip，为支持gzip的浏览器返回经gzip压缩的HTML页面，为其他浏览器返回普通页面。 |
| Content-Length | 表示内容长度。只有当浏览器使用持久HTTP连接时才需要这个数据。如果你想要利用持久连接的优势，可以把输出文档写入 ByteArrayOutputStream，完成后查看其大小，然后把该值放入Content-Length头，最后通过byteArrayStream.writeTo(response.getOutputStream()发送内容。 |
| Content-Type | 表示后面的文档属于什么MIME类型。Servlet默认为text/plain，但通常需要显式地指定为text/html。由于经常要设置Content-Type，因此HttpServletResponse提供了一个专用的方法setContentType。 |
| Date | 当前的GMT时间。你可以用setDateHeader来设置这个头以避免转换时间格式的麻烦。 |
| Expires | 应该在什么时候认为文档已经过期，从而不再缓存它？ |
| Last-Modified | 文档的最后改动时间。客户可以通过If-Modified-Since请求头提供一个日期，该请求将被视为一个条件GET，只有改动时间迟于指定时间的文档才会返回，否则返回一个304（Not Modified）状态。Last-Modified也可用setDateHeader方法来设置。 |
| Location | 表示客户应当到哪里去提取文档。Location通常不是直接设置的，而是通过HttpServletResponse的sendRedirect方法，该方法同时设置状态代码为302。 |
| Refresh | 表示浏览器应该在多少时间之后刷新文档，以秒计。除了刷新当前文档之外，你还可以通过setHeader("Refresh", "5; URL=http://host/path")让浏览器读取指定的页面。<br />注意这种功能通常是通过设置HTML页面HEAD区的＜META HTTP-EQUIV="Refresh" CONTENT="5;URL=http://host/path"＞实现，这是因为，自动刷新或重定向对于那些不能使用CGI或Servlet的HTML编写者十分重要。但是，对于Servlet来说，直接设置Refresh头更加方便。<br /><br />注意Refresh的意义是"N秒之后刷新本页面或访问指定页面"，而不是"每隔N秒刷新本页面或访问指定页面"。因此，连续刷新要求每次都发送一个Refresh头，而发送204状态代码则可以阻止浏览器继续刷新，不管是使用Refresh头还是＜META HTTP-EQUIV="Refresh" ...＞。<br /><br />注意Refresh头不属于HTTP 1.1正式规范的一部分，而是一个扩展，但Netscape和IE都支持它。 |
| Server | 服务器名字。Servlet一般不设置这个值，而是由Web服务器自己设置。 |
| Set-Cookie | 设置和页面关联的Cookie。Servlet不应使用response.setHeader("Set-Cookie", ...)，而是应使用HttpServletResponse提供的专用方法addCookie。参见下文有关Cookie设置的讨论。 |
| WWW-Authenticate | 客户应该在Authorization头中提供什么类型的授权信息？在包含401（Unauthorized）状态行的应答中这个头是必需的。例如，response.setHeader("WWW-Authenticate", "BASIC realm=＼"executives＼"")。<br />注意Servlet一般不进行这方面的处理，而是让Web服务器的专门机制来控制受密码保护页面的访问（例如.htaccess）。 |

<a name="Ovqlh"></a>
# 二、nginx基础内容

<a name="SXQEy"></a>
## 1、基础知识
**代理服务器:**<br />    一般是指局域网内部的机器通过代理服务器发送请求到互联网上的服务器,代理服务器一般作用在客户端。应用比如：GoAgent，FQ神器.<br />    一个完整的代理请求过程为：客户端首先与代理服务器创建连接，接着根据代理服务器所使用的代理协议，请求对目标服务器创建连接、或者获得目标服务器的指定资源。 Web代理（proxy）服务器是网络的中间实体。 代理位于Web客户端和Web服务器之间，扮演“中间人”的角色。HTTP的代理服务器即是Web服务器又是Web客户端。<br />    代理服务器是介于客户端和Web服务器之间的另一台服务器，有了它之后，浏览器不是直接到Web服务器去取回网页而是向代理服务器发出请求，信号会先送到代理服务器，由代理服务器来取回浏览器所需要的信息并传送给你的浏览器。

**正向代理**<br />    是一个位于客户端和原始服务器(origin server)之间的服务器，为了从原始服务器取得内容，客户端向代理发送一个请求并指定目标(原始服务器)，然后代理向原始服务器转交请求并将获得的内容返回给客户端。客户端必须要进行一些特别的设置才能使用正向代理。<br />**反向代理服务器：**<br />    在服务器端接受客户端的请求，然后把请求分发给具体的服务器进行处理，然后再将服务器的响应结果反馈给客户端。Nginx就是其中的一种反向代理服务器软件。<br />    Nginx：Nginx ("engine x") ，Nginx (“engine x”) 是俄罗斯人Igor Sysoev(塞索耶夫)编写的一款高性能的 HTTP 和反向代理服务器。也是一个IMAP/POP3/SMTP代理服务器；也就是说，Nginx本身就可以托管网站，进行HTTP服务处理，也可以作为反向代理服务器使用。

    客户端必须设置正向代理服务器，当然前提是要知道正向代理服务器的IP地址，还有代理程序的端口。<br />    反向代理正好与正向代理相反，对于客户端而言代理服务器就像是原始服务器，并且客户端不需要进行任何特别的设置。客户端向反向代理的命名空间(name-space)中的内容发送普通请求，接着反向代理将判断向何处(原始服务器)转交请求，并将获得的内容返回给客户端。

**2.Nginx是什么，有什么优点？为什么选择Nginx做web服务器软件?**<br />    Nginx是一个高性能的 HTTP 和 反向代理 服务器，也是一个 IMAP/POP3/SMTP 代理服务器。因它的稳定性、丰富的功能集、示例配置文件和低系统资源的消耗而闻名。2011年6月1日，nginx 1.0.4发布。
<a name="5uFGo"></a>
## 2、优点：
**（1）更快**<br />    这表现在两个方面：一方面，在正常情况下，单次请求会得到更快的响应；另一方面，在高峰期（如有数以万计的并发请求），Nginx可以比其他Web服务器更快地响应请求。<br />**（2）高扩展性，跨平台**<br />    Nginx的设计极具扩展性，它完全是由多个不同功能、不同层次、不同类型且耦合度极低的模块组成。因此，当对某一个模块修复Bug或进行升级时，可以专注于模块自身，无须在意其他。而且在HTTP模块中，还设计了HTTP过滤器模块：一个正常的HTTP模块在处理完请求后，会有一串HTTP过滤器模块对请求的结果进行再处理。这样，当我们开发一个新的HTTP模块时，不但可以使用诸如HTTP核心模块、events模块、log模块等不同层次或者不同类型的模块，还可以原封不动地复用大量已有的HTTP过滤器模块。这种低耦合度的优秀设计，造就了Nginx庞大的第三方模块，当然，公开的第三方模块也如官方发布的模块一样容易使用。<br />    Nginx的模块都是嵌入到二进制文件中执行的，无论官方发布的模块还是第三方模块都是如此。这使得第三方模块一样具备极其优秀的性能，充分利用Nginx的高并发特性，因此，许多高流量的网站都倾向于开发符合自己业务特性的定制模块。<br />**（3）高可靠性：**用于反向代理，宕机的概率微乎其微<br />    高可靠性是我们选择Nginx的最基本条件，因为Nginx的可靠性是大家有目共睹的，很多家高流量网站都在核心服务器上大规模使用Nginx。Nginx的高可靠性来自于其核心框架代码的优秀设计、模块设计的简单性；另外，官方提供的常用模块都非常稳定，每个worker进程相对独立，master进程在1个worker进程出错时可以快速“拉起”新的worker子进程提供服务。

**（4）低内存消耗**<br />    一般情况下，10 000个非活跃的HTTP Keep-Alive连接在Nginx中仅消耗2.5MB的内存，这是Nginx支持高并发连接的基础。<br />**（5）单机支持10万以上的并发连接**<br />    这是一个非常重要的特性！随着互联网的迅猛发展和互联网用户数量的成倍增长，各大公司、网站都需要应付海量并发请求，一个能够在峰值期顶住10万以上并发请求的Server，无疑会得到大家的青睐。理论上，Nginx支持的并发连接上限取决于内存，10万远未封顶。当然，能够及时地处理更多的并发请求，是与业务特点紧密相关的。<br />**（6）热部署**<br />     master管理进程与worker工作进程的分离设计，使得Nginx能够提供热部署功能，即可以在7×24小时不间断服务的前提下，升级Nginx的可执行文件。当然，它也支持不停止服务就更新配置项、更换日志文件等功能。<br />**（7）最自由的BSD许可协议**<br />    这是Nginx可以快速发展的强大动力。BSD许可协议不只是允许用户免费使用Nginx，它还允许用户在自己的项目中直接使用或修改Nginx源码，然后发布。这吸引了无数开发者继续为Nginx贡献自己的智慧。<br />    以上7个特点当然不是Nginx的全部，拥有无数个官方功能模块、第三方功能模块使得Nginx能够满足绝大部分应用场景，这些功能模块间可以叠加以实现更加强大、复杂的功能，有些模块还支持Nginx与Perl、Lua等脚本语言集成工作，大大提高了开发效率。这些特点促使用户在寻找一个Web服务器时更多考虑Nginx。<br />    选择Nginx的核心理由还是它能在支持高并发请求的同时保持高效的服务。

<a name="1m20G"></a>
# 三、nginx的安装
<a name="su3By"></a>
## 1、yum官方源安装
[http://nginx.org/en/linux_packages.html#RHEL-CentOS](http://nginx.org/en/linux_packages.html#RHEL-CentOS)
```bash
[root@nginx ~]# vim /etc/yum.repos.d/nginx.repo
[nginx-stable]
name=nginx stable repo
baseurl=http://nginx.org/packages/centos/$releasever/$basearch/
gpgcheck=1
enabled=1
gpgkey=https://nginx.org/keys/nginx_signing.key
module_hotfixes=true

[nginx-mainline]
name=nginx mainline repo
baseurl=http://nginx.org/packages/mainline/centos/$releasever/$basearch/
gpgcheck=1
enabled=0
gpgkey=https://nginx.org/keys/nginx_signing.key
module_hotfixes=true
```
<a name="OQuOC"></a>
## 2、非官方epel源安装
```bash
[root@nginx ~]# wget -O /etc/yum.repos.d/epel.repo http://mirrors.aliyun.com/repo/epel-7.repo
[root@nginx ~]# yum install -y nginx
```
<a name="XeRID"></a>
### 查看nginx的版本
```bash
[root@nginx ~]# nginx -V
nginx version: nginx/1.16.1
built by gcc 4.8.5 20150623 (Red Hat 4.8.5-39) (GCC)
built with OpenSSL 1.0.2k-fips  26 Jan 2017
TLS SNI support enabled
configure arguments: --prefix=/usr/share/nginx --sbin-path=/usr/sbin/nginx --modules-path=/usr/lib64/nginx/modules --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --http-client-body-temp-path=/var/lib/nginx/tmp/client_body --http-proxy-temp-path=/var/lib/nginx/tmp/proxy --http-fastcgi-temp-path=/var/lib/nginx/tmp/fastcgi --http-uwsgi-temp-path=/var/lib/nginx/tmp/uwsgi --http-scgi-temp-path=/var/lib/nginx/tmp/scgi --pid-path=/run/nginx.pid --lock-path=/run/lock/subsys/nginx --user=nginx --group=nginx --with-file-aio --with-ipv6 --with-http_ssl_module --with-http_v2_module --with-http_realip_module --with-stream_ssl_preread_module --with-http_addition_module --with-http_xslt_module=dynamic --with-http_image_filter_module=dynamic --with-http_sub_module --with-http_dav_module --with-http_flv_module --with-http_mp4_module --with-http_gunzip_module --with-http_gzip_static_module --with-http_random_index_module --with-http_secure_link_module --with-http_degradation_module --with-http_slice_module --with-http_stub_status_module --with-http_perl_module=dynamic --with-http_auth_request_module --with-mail=dynamic --with-mail_ssl_module --with-pcre --with-pcre-jit --with-stream=dynamic --with-stream_ssl_module --with-google_perftools_module --with-debug --with-cc-opt='-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector-strong --param=ssp-buffer-size=4 -grecord-gcc-switches -specs=/usr/lib/rpm/redhat/redhat-hardened-cc1 -m64 -mtune=generic' --with-ld-opt='-Wl,-z,relro -specs=/usr/lib/rpm/redhat/redhat-hardened-ld -Wl,-E'

```
<a name="RsGOG"></a>
## 3、编译安装
** 安装依赖包**
```bash
[root@nginx ~]# yum -y install gcc gcc-c++ 
[root@nginx ~]# yum install -y pcre pcre-devel
[root@nginx ~]# yum install -y openssl openssl-devel
[root@nginx ~]# yum install -y zlib zlib-devel
```
**创建用户**
```c
[root@nginx ~]# useradd -s /sbin/nologin nginx
```
**编译**
```c
[root@nginx ~]# wget http://nginx.org/download/nginx-1.16.0.tar.gz
--2020-09-25 21:54:36--  http://nginx.org/download/nginx-1.16.0.tar.gz
Resolving nginx.org (nginx.org)... 3.125.197.172, 52.58.199.22, 2a05:d014:edb:5704::6, ...
Connecting to nginx.org (nginx.org)|3.125.197.172|:80... connected.
HTTP request sent, awaiting response... 200 OK
Length: 1032345 (1008K) [application/octet-stream]
Saving to: ‘nginx-1.16.0.tar.gz’

100%[=======================================================================================================================>] 1,032,345    486KB/s   in 2.1s

2020-09-25 21:54:39 (486 KB/s) - ‘nginx-1.16.0.tar.gz’ saved [1032345/1032345]

[root@nginx ~]# tar -xzvf nginx-1.16.0.tar.gz -C /usr/local/
nginx-1.16.0/
nginx-1.16.0/auto/
nginx-1.16.0/conf/
nginx-1.16.0/contrib/
nginx-1.16.0/src/
nginx-1.16.0/configure
nginx-1.16.0/LICENSE
nginx-1.16.0/README
nginx-1.16.0/html/
………………………………………………………………………………………………
[root@nginx nginx-1.16.0]# ./configure --prefix=/usr/local/nginx --group=nginx --user=nginx --sbin-path=/usr/local/nginx/sbin/nginx --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --http-client-body-temp-path=/tmp/nginx/client_body --http-proxy-temp-path=/tmp/nginx/proxy --http-fastcgi-temp-path=/tmp/nginx/fastcgi --pid-path=/var/run/nginx.pid --lock-path=/var/lock/nginx --with-http_stub_status_module --with-http_ssl_module --with-http_gzip_static_module --with-pcre --with-http_realip_module --with-stream
checking for OS
 + Linux 3.10.0-229.el7.x86_64 x86_64
checking for C compiler ... found
 + using GNU C compiler
 + gcc version: 4.8.5 20150623 (Red Hat 4.8.5-39) (GCC)
checking for gcc -pipe switch ... found
checking for -Wl,-E switch ... found
checking for gcc builtin atomic operations ... found
checking for C99 variadic macros ... found
checking for gcc variadic macros ... found
checking for gcc builtin 64 bit byteswap ... found
checking for unistd.h ... found
checking for inttypes.h ... found
checking for limits.h ... found
………………………………………………
[root@nginx nginx-1.16.0]# make
make -f objs/Makefile
make[1]: Entering directory `/usr/local/nginx-1.16.0'
cc -c -pipe  -O -W -Wall -Wpointer-arith -Wno-unused-parameter -Werror -g  -I src/core -I src/event -I src/event/modules -I src/os/unix -I objs \
        -o objs/src/core/nginx.o \
        src/core/nginx.c
cc -c -pipe  -O -W -Wall -Wpointer-arith -Wno-unused-parameter -Werror -g  -I src/core -I src/event -I src/event/modules -I src/os/unix -I objs \
        -o objs/src/core/ngx_log.o \
        src/core/ngx_log.c
………………………………………………………………
[root@nginx nginx-1.16.0]# make install
cp conf/scgi_params \
        '/etc/nginx/scgi_params.default'
test -f '/etc/nginx/nginx.conf' \
        || cp conf/nginx.conf '/etc/nginx/nginx.conf'
cp conf/nginx.conf '/etc/nginx/nginx.conf.default'
test -d '/var/run' \
        || mkdir -p '/var/run'
test -d '/var/log/nginx' \
        || mkdir -p '/var/log/nginx'
test -d '/usr/local/nginx/html' \
        || cp -R html '/usr/local/nginx'
test -d '/var/log/nginx' \
        || mkdir -p '/var/log/nginx'
```
**查看版本**
```c
[root@nginx nginx-1.16.0]# /usr/local/nginx/sbin/nginx -V
nginx version: nginx/1.16.0
built by gcc 4.8.5 20150623 (Red Hat 4.8.5-39) (GCC)
built with OpenSSL 1.0.2k-fips  26 Jan 2017
TLS SNI support enabled
configure arguments: --prefix=/usr/local/nginx --group=nginx --user=nginx --sbin-path=/usr/local/nginx/sbin/nginx --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --http-client-body-temp-path=/tmp/nginx/client_body --http-proxy-temp-path=/tmp/nginx/proxy --http-fastcgi-temp-path=/tmp/nginx/fastcgi --pid-path=/var/run/nginx.pid --lock-path=/var/lock/nginx --with-http_stub_status_module --with-http_ssl_module --with-http_gzip_static_module --with-pcre --with-http_realip_module --with-stream
[root@nginx nginx-1.16.0]#

```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601085748068-ddda83b3-241a-4389-b70a-efadc1d385dc.png#height=380&id=vNjuN&originHeight=760&originWidth=682&originalType=binary&ratio=1&rotation=0&showTitle=false&size=106227&status=done&style=none&title=&width=341)
```c
[root@nginx nginx-1.16.0]#  /usr/local/nginx/sbin/nginx -t
nginx: the configuration file /etc/nginx/nginx.conf syntax is ok
nginx: [emerg] mkdir() "/tmp/nginx/client_body" failed (2: No such file or directory)
nginx: configuration file /etc/nginx/nginx.conf test failed
[root@nginx nginx-1.16.0]# mkdir /tmp/nginx
[root@nginx nginx-1.16.0]#  /usr/local/nginx/sbin/nginx -t
nginx: the configuration file /etc/nginx/nginx.conf syntax is ok
nginx: configuration file /etc/nginx/nginx.conf test is successful
[root@nginx nginx-1.16.0]#
```
**启动服务**
```c
[root@nginx nginx-1.16.0]# /usr/local/nginx/sbin/ngin
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601085901383-a9e9153c-c411-43f6-89a7-ba8063d1fab4.png#height=270&id=GDXTg&originHeight=539&originWidth=1105&originalType=binary&ratio=1&rotation=0&showTitle=false&size=85090&status=done&style=none&title=&width=552.5)<br />**创建快捷启动方式**
```c
[root@nginx nginx-1.16.0]# ln -s /usr/local/nginx/sbin/nginx /usr/sbin/nginx
[root@nginx ~]# nginx  -V
nginx version: nginx/1.16.0
built by gcc 4.8.5 20150623 (Red Hat 4.8.5-39) (GCC)
built with OpenSSL 1.0.2k-fips  26 Jan 2017
TLS SNI support enabled
configure arguments: --prefix=/usr/local/nginx --group=nginx --user=nginx --sbin-path=/usr/local/nginx/sbin/nginx --conf-path=/etc/nginx/nginx.conf --error-log-path=/var/log/nginx/error.log --http-log-path=/var/log/nginx/access.log --http-client-body-temp-path=/tmp/nginx/client_body --http-proxy-temp-path=/tmp/nginx/proxy --http-fastcgi-temp-path=/tmp/nginx/fastcgi --pid-path=/var/run/nginx.pid --lock-path=/var/lock/nginx --with-http_stub_status_module --with-http_ssl_module --with-http_gzip_static_module --with-pcre --with-http_realip_module --with-stream
[root@nginx ~]#
```
<a name="dc5Si"></a>
## 4、日志解释
nginx 日志文件分为log_format和access_log两部分log_format 定义记录的格式，其语法格式为log_format 样式名称样式详情配置文件中默认有
```c
    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601089965380-08cf862e-981b-4eea-8cf9-1e3011fb017a.png#height=252&id=V3ZUN&originHeight=504&originWidth=1030&originalType=binary&ratio=1&rotation=0&showTitle=false&size=117907&status=done&style=none&title=&width=515)<br />**示范内容**
```c
[root@nginx ~]# tail -1 /var/log/nginx/access.log
192.168.40.1 - - [25/Sep/2020:20:54:16 -0400] "GET / HTTP/1.1" 304 0 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36" "-"
```
<a name="jigPK"></a>
# 四、nginx基础配置
<a name="xm0hS"></a>
## 1、虚拟主机介绍
nginx可以实现虚拟主机的配置，nginx支持三种类型的虚拟主机配置。 <br />1、基于域名的虚拟主机 （server_name来 区分虚拟主机——应用：外部网站）<br />2、基于ip的虚拟主机， （一块主机绑定多个ip地址） <br />3、基于端口的虚拟主机 （端口来区分虚拟主机——应用：公司内部网站，外部网站的管理后台）
<a name="UMjWD"></a>
## 2、基于域名的虚拟主机
```c
[root@nginx conf.d]# cat jw.conf
server     {
   listen    80;					#网站的端口
   server_name    www.jw.com;		#网站访问的域名
   location / {						#网站的区域
      root    /jw;					#网站的根目录
      index     index.html ;		#网站的首页
   }
}
[root@nginx conf.d]# mkdir /jw/
mkdir: cannot create directory ‘/jw/’: File exists
[root@nginx conf.d]# echo www.jw.com > /jw/index.html
[root@nginx conf.d]# systemctl reload nginx
[root@nginx conf.d]# cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
192.168.40.23 www.jw.com
[root@nginx conf.d]# curl www.jw.com
www.jw.com
[root@nginx conf.d]#
[root@nginx conf.d]# cat bbs.conf
server     {
   listen    80;
   server_name    bbs.jw.com;
   location / {
      root    /bbs;
      index     index.html ;
   }
}
[root@nginx conf.d]# cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
192.168.40.23 www.jw.com
192.168.40.23 bbs.jw.com
[root@nginx conf.d]# echo "bbs.jw.com" > /bbs/index.html
[root@nginx conf.d]# systemctl reload nginx
[root@ng[root@nginx conf.d]# curl bbs.jw.com
bbs.jw.com
[root@nginx conf.d]# curl www.jw.com
www.jw.com
```
**注：**虚拟主机域名的方式就可以实现一台服务器上面部署多台服务器
<a name="71tpO"></a>
## 3、基于IP的虚拟主机
```c
[root@nginx conf.d]# vim ip.conf
server     {
   listen    80;
   server_name    192.168.40.25;
   location / {
      root    /ip;
      index     index.html ;
   }
}
[root@nginx conf.d]# mkdir /ip
[root@nginx conf.d]# echo "192.168.40.25" >/ip/index.html
[root@nginx conf.d]# systemctl reload nginx
[root@nginx conf.d]# curl 192.168.40.25
192.168.40.25
[root@nginx conf.d]# curl 192.168.40.25:8081
192.168.40.25
[root@nginx conf.d]# cat ip.conf
server     {
   listen    8081;
   server_name    192.168.40.25;
   location / {
      root    /ip;
      index     index.html ;
   }
}
[root@nginx conf.d]# curl 192.168.40.25:8081
192.168.40.25
[root@nginx conf.d]#
```
<a name="QZZtE"></a>
## 4、nginx Proxy 代理
**1、代理原理**<br />反向代理产生的背景：在计算机世界里，由于单个服务器的处理客户端（用户）请求能力有一个极限，当用户的接入请求蜂拥而入时，会造成服务器忙不过来的局面，可以使用多个服务器来共同分担成千上万的用户请求，这些服务器提供相同的服务，对于用户来说，根本感觉不到任何差别。反向代理服务的实现：需要有一个负载均衡设备（即反向代理服务器）来分发用户请求，将用户请求分发到空闲的服务器上。服务器返回自己的服务到负载均衡<设备。负载均衡设备将服务器的服务返回用户。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601101332692-e5c7f705-37fd-4cdd-adb0-84f58cf7c8ee.png#height=287&id=MlnU9&originHeight=573&originWidth=946&originalType=binary&ratio=1&rotation=0&showTitle=false&size=271408&status=done&style=none&title=&width=473)<br />**2、正/反向代理的区别**<br />正向代理：举例：贷款正向代理的过程隐藏了真实的请求客户端，服务器不知道真实的客户端是谁，客户端请求的服务都被代理服务器代替请求。我们常说的代理也就是正向代理，正向代理代理的是请求方，也就是客户端；比如我们要访问youtube，可是不能访问，只能先安装个FQ软件代你去访问，通过FQ软件才能访问，FQ软件就叫作正向代理。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601101412459-980c44e7-0ab6-4062-8c60-aec00a03f27d.png#height=281&id=jktJb&originHeight=562&originWidth=980&originalType=binary&ratio=1&rotation=0&showTitle=false&size=269244&status=done&style=none&title=&width=490)<br />**3、反向代理：**<br />反向代理的过程隐藏了真实的服务器，客户不知道真正提供服务的人是谁，客户端请求的服务都被代理服务器处理。反向代理代理的是响应方，也就是服务端；我们请求www.baidu.com时这www.baidu.com就是反向代理服务器，真实提供服务的服务器有很多台，反向代理服务器会把我们的请求分转发到真实提供服务的各台服务器。Nginx就是性能非常好的反向代理服务器，用来做负载均衡。

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601101477825-371fb330-3186-4c20-bce6-bd54da3a3c3c.png#height=303&id=WAkAs&originHeight=605&originWidth=958&originalType=binary&ratio=1&rotation=0&showTitle=false&size=282137&status=done&style=none&title=&width=479)<br />反向代理中，proxy和server同属一个LAN反向代理中代理的对象是服务端，proxy和server同属一个LAN，对client透明。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601101511129-11c277db-2814-4aae-ae45-4ccd4b7d29fa.png#height=215&id=Vp91K&originHeight=430&originWidth=1058&originalType=binary&ratio=1&rotation=0&showTitle=false&size=298335&status=done&style=none&title=&width=529)

- 反向代理配置

**代理服务器设置**
```bash
[root@nginx-up conf.d]# cat upstream.conf
upstream jw{
    server 192.168.40.23;
    server 192.168.40.25:8081;
}
server {
    listen 80;
    server_name www.abc.com;
    location / {
        proxy_pass http://jw;
        proxy_set_header Host $http_host;
        proxy_set_header X-Real-IP  $remote_addr;
        proxy_set_header X-Forwarded-For  $proxy_add_x_forwarded_for;
    }
}
[root@nginx-up conf.d]#nginx -s reload
```
proxy_pass ：真实服务器的地址，可以是ip也可以是域名和url地址<br />proxy_set_header：重新定义或者添加发往后端服务器的请求头<br />proxy_set_header X-Real-IP ：启用客户端真实地址（否则日志中显示的是代理在访问网站）<br />proxy_set_header X-Forwarded-For：记录代理地址<br />proxy_connect_timeout：后端服务器连接的超时时间发起三次握手等候响应超时时间<br />proxy_send_timeout：后端服务器数据回传时间就是在规定时间之内后端服务器必须传完所有的数据<br />proxy_read_timeout ：nginx接收upstream（上游/真实） server数据超时, 默认60s, 如果连续的60s内没有收到1个字节, 连接关闭。像长连接

**web服务器的配置**
```bash
[root@nginx conf.d]# ls
bbs.conf
[root@nginx conf.d]# cat bbs.conf
server     {
   listen    8081;
   server_name    192.168.40.25;
   location / {
      root    /ip;
      index     index.html ;
   }
}
[root@nginx conf.d]# curl 192.168.40.25
192.168.40.25
[root@nginx conf.d]# curl 192.168.40.23
192.168.40.23
[root@nginx conf.d]#
[root@nginx conf.d]# nginx -s reload
```
**访问测试**
```c
[root@nginx-up conf.d]# while true;do curl www.abc.com;sleep 2;done
192.168.40.25
192.168.40.23
192.168.40.25
192.168.40.23
192.168.40.25
```
备注:默认是以轮询的方式进行工作<br />修改为**热备模式热备模式访问的时候全是一台服务器服务器挂了才访问另外的一台服务器例如AAAAAAAAA BBBBBB**
```c
[root@nginx-up conf.d]# cat upstream.conf
upstream jw{
    server 192.168.40.23 backup;
    server 192.168.40.25:8081;
}
server {
    listen 80;
    server_name www.abc.com;
    location / {
        proxy_pass http://jw;
        proxy_set_header Host $http_host;
        proxy_set_header X-Real-IP  $remote_addr;
        proxy_set_header X-Forwarded-For  $proxy_add_x_forwarded_for;
    }
}
[root@nginx-up conf.d]# while true;do curl www.abc.com;sleep 2;done
192.168.40.25
192.168.40.25
192.168.40.25
192.168.40.25
^C
```
**查看日志消息**
```bash
192.168.40.1 - - [26/Sep/2020:04:48:13 -0400] "GET / HTTP/1.1" 200 14 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36" "-"
192.168.40.1 - - [26/Sep/2020:04:48:14 -0400] "GET / HTTP/1.1" 200 14 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36" "-"
192.168.40.1 - - [26/Sep/2020:04:48:15 -0400] "GET / HTTP/1.1" 200 14 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36" "-"
192.168.40.1 - - [26/Sep/2020:04:48:15 -0400] "GET / HTTP/1.1" 200 14 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36" "-"
192.168.40.1 - - [26/Sep/2020:04:48:15 -0400] "GET / HTTP/1.1" 200 14 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36" "-"
192.168.40.1 - - [26/Sep/2020:04:48:15 -0400] "GET / HTTP/1.1" 200 14 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36" "-"
192.168.40.1 - - [26/Sep/2020:04:48:15 -0400] "GET / HTTP/1.1" 200 14 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36" "-"
192.168.40.1 - - [26/Sep/2020:04:48:15 -0400] "GET / HTTP/1.1" 200 14 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36" "-"
```
<a name="Ya1QS"></a>
## 5、Nginx负载均衡
**1、负载均衡的作用**

如果你的nginx服务器给2台web服务器做代理，负载均衡算法采用轮询，那么当你的一台机器web程序关闭造成web不能访问，那么nginx服务器分发请求还是会给这台不能访问的web服务器，如果这里的响应连接时间过长，就会导致客户端的页面一直在等待响应，对用户来说体验就打打折扣，这里我们怎么避免这样的情况发生呢。这里我配张图来说明下问题。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601110891157-ce7ec0f5-cc5d-4446-9ad3-9f508feccc39.png#height=289&id=bTFT4&originHeight=577&originWidth=1061&originalType=binary&ratio=1&rotation=0&showTitle=false&size=315481&status=done&style=none&title=&width=530.5)<br />如果负载均衡中其中web2发生这样的情况，nginx首先会去web1请求，但是nginx在配置不当的情况下会继续分发请求到web2，然后等待web2响应，直到我们的响应时间超时，才会把请求重新分发给web1，这里的响应时间如果过长，用户等待的时间就会越长。

**2、upstream配置**

这个配置是写一组被代理的服务器地址，然后配置负载均衡的算法。这里的被代理服务器地址有2种写法。
```bash
upstream jw {
      server 192.168.62.157:8080;
      server 192.168.62.158:8080;
	  } 
	  server {
	     listen 80;
	     server_name localhost;
	     location / {
	     proxy_pass  http://jw;
	  }
}
```
**  四层负载均衡配置**
```bash
#4层tcp负载
stream {
	upstream myweb {
	hash $remote_addr consistent;
	server 172.17.14.2:8080;
	server 172.17.14.3:8080;
	}
	server {
	listen 80;
	proxy_connect_timeout 10s;
	proxy_timeout 30s;
	proxy_pass myweb;
	}
}
```
**3、负载均衡算法**

upstream 支持4种负载均衡调度算法:<br />A、轮询(默认):每个请求按时间顺序逐一分配到不同的后端服务器;

B、ip_hash:每个请求按访问IP的hash结果分配，同一个IP客户端固定访问一个后端服务器。可以保证来自同一ip的请求被打到固定的机器上，可以解决session问题。

C、url_hash:按访问url的hash结果来分配请求，使每个url定向到同一个后端服务器。后台服务器为缓存的时候效率。

D、fair:这是比上面两个更加智能的负载均衡算法。此种算法可以依据页面大小和加载时间长短智能地进行负载均衡，也就是根据后端服务器的响应时间来分配请求，响应时间短的优先分配。Nginx本身是不支持fair的，如果需要使用这种调度算法，必须下载Nginx的upstream_fair模块

**4、热备：**<br />如果你有2台服务器，当一台服务器发生事故时，才启用第二台服务器给提供服务。服务器处理请求的顺序：AAAAAA突然A挂啦，BBBBBBBBBBBBBB.....

**5、轮询**<br />nginx默认就是轮询其权重都默认为1，服务器处理请求的顺序：ABABABABAB.

**6、加权轮询**<br />跟据配置的权重的大小而分发给不同服务器不同数量的请求。如果不设置，则默认为1。下面服务器的请求顺序为：ABBABBABBABBABB....
```bash
upstream myweb {
	server 192.168.62.157:8080 weight=1;
  server 192.168.62.158:8080 weight=2;
}
```
**7、ip_hash**<br />nginx会让相同的客户端ip请求相同的服务器。
```bash
upstream myweb {
	server 192.168.62.157:8080;
  server 192.168.62.158:8080;
  ip_hash;
}
```
**8、参数**<br />down，表示当前的server暂时不参与负载均衡。<br />backup，预留的备份机器。当其他所有的非backup机器出现故障或者忙的时候，才会请求backup机器，因此这台机器的压力最轻。<br />max_fails，允许请求失败的次数，默认为1。当超过最大次数时，返回错误。<br />fail_timeout，在经历了max_fails次失败后，暂停服务的时间单位秒。max_fails可以和fail_timeout一起使用。
```bash
upstream myweb {
	server 192.168.62.157:8080 weight=2 max_fails=2 fail_timeout=2;
	server 192.168.62.158:8080 weight=1 max_fails=2 fail_timeout=1;
}
```
<a name="2IBec"></a>
## 6、nginx 会话保持

**1、ip_hash**<br />ip_hash使用源地址哈希算法，将同一客户端的请求总是发往同一个后端服务器，除非该服务器不可用
```bash
upstream backend {
	ip_hash;
	server backend1.example.com;
	server backend2.example.com;
	server backend3.example.com down;
}
```
ip_hash简单易用，但有如下问题：当后端服务器宕机后，session会话丢失；同一客户端会被转发到同一个后端服务器，可能导致负载失衡；

**2、sticky_cookie_insert**<br />使用sticky_cookie_insert启用会话亲缘关系，这会导致来自同一客户端的请求被传递到一组服务器的同一台服务器。与ip_hash不同之处在于，它不是基于IP来判断客户端的，而是基于cookie来判断。因此可以避免上述ip_hash中来自同一客户端导致负载失衡的情况。(需要引入第三方模块才能实现)sticky模块
```bash
upstream backend {
    server backend1.example.com;
    server backend2.example.com;
    sticky_cookie_insert srv_id expires=1h domain=3evip.cn path=/;
}
server {
    listen 80;
    server_name 3evip.cn;
    location / {proxy_pass http://backen;
    }
}
```
说明： expires：设置浏览器中保持cookie的时间 domain：定义cookie的域 path：为cookie定义路径

**3.jvm_route**

jvm_route的原理<br />1. 一开始请求过来，没有带session信息，jvm_route就根据round robin的方法，发到一台tomcat上面。<br />2. tomcat添加上session 信息，并返回给客户。<br />3. 用户再此请求，jvm_route看到session中有后端服务器的名称，它就把请求转到对应的服务器上。

<a name="T59Jp"></a>
# 五、nginx 实现动静分离
为了加快网站的解析速度，可以把动态页面和静态页面由不同的服务器来解析，加快解析速度。降低原来单个服务器的压力。在动静分离的tomcat的时候比较明显，因为tomcat解析静态很慢，其实这些原理的话都很好理解，简单来说，就是使用正则表达式匹配过滤，然后交个不同的服务器。

| **角色** | **主机名称** | **IP地址** |
| --- | --- | --- |
| 负载均衡服务器 | upstream | 192.168.40.23 |
| 静态页面服务器 | static | 192.168.40.24 |
| 动态页面服务器 | dynamic | 192.168.40.25 |

<a name="s1Pvk"></a>
## 1、负载均衡配置
```bash
[root@upstream conf.d]# cat upstream.conf
upstream static {
       server 192.168.40.24:80 weight=1 max_fails=1 fail_timeout=60s;
    }
upstream dynamic {
       server 192.168.40.25:80 weight=1 max_fails=1 fail_timeout=60s;
    }
    server {
        listen 80;
        server_name 192.168.40.23;
        #动态资源加载
        location ~ \.(php|jsp)$ {
            proxy_pass http://dynamic;
            proxy_set_header Host $host:$server_port;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        }
        #静态资源加载
        location ~ \.(html|gif|htm|png|swf)$ {
            proxy_pass http://static;
            proxy_set_header Host $host:$server_port;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        }
    }
[root@upstream conf.d]#
```
<a name="Vh2sc"></a>
## 2、dynamic配置
```bash
[root@dynamic conf.d]# systemctl status php-fpm.service
● php-fpm.service - The PHP FastCGI Process Manager
   Loaded: loaded (/usr/lib/systemd/system/php-fpm.service; enabled; vendor preset: disabled)
   Active: active (running) since Sun 2020-09-27 03:02:47 EDT; 52min ago
  Process: 5297 ExecReload=/bin/kill -USR2 $MAINPID (code=exited, status=0/SUCCESS)
 Main PID: 5787 (php-fpm)
   Status: "Processes active: 0, idle: 5, Requests: 0, slow: 0, Traffic: 0req/sec"
   CGroup: /system.slice/php-fpm.service
           ├─5787 php-fpm: master process (/etc/php-fpm.conf)
           ├─5788 php-fpm: pool www
           ├─5789 php-fpm: pool www
           ├─5790 php-fpm: pool www
           ├─5791 php-fpm: pool www
           └─5792 php-fpm: pool www

Sep 27 03:02:47 dynamic systemd[1]: Stopped The PHP FastCGI Process Manager.
Sep 27 03:02:47 dynamic systemd[1]: Starting The PHP FastCGI Process Manager...
Sep 27 03:02:47 dynamic php-fpm[5787]: [27-Sep-2020 03:02:47] NOTICE: PHP message: PHP Warning:  PHP Startup: Unable to load dynamic library 'gd' (tried: /usr/lib64/php/modules/gd (/...
Sep 27 03:02:47 dynamic systemd[1]: Started The PHP FastCGI Process Manager.
Hint: Some lines were ellipsized, use -l to show in full.
[root@dynamic conf.d]# cat dynamic.conf
server {
    listen 80;
    server_name   192.168.40.24;
    location ~ \.php$ {
       root     /home/nginx/html;
       fastcgi_pass 192.168.40.24:9000;
       fastcgi_index index.php;
       fastcgi_param SCIPT_FILENAME $document_root$fastcgi_script_name;
       include      fastcgi_params;
    }
}
[root@dynamic conf.d]# cat /home/nginx/html/index.php
dynamic
```
<a name="9afa2e6c"></a>
## 3、static页面配置
```bash
[root@static conf.d]# cat static.conf
server{
     listen 80;
     server_name        192.168.40.25;

     location ~ \.(html|htm|png|js){
     root /home/www/nginx;
     index index.html;
     }
}
[root@static conf.d]# cat /home/www/nginx/index.html
static
```
<a name="8LXgy"></a>
## 4、访问测试
```bash
[root@upstream conf.d]# curl 192.168.40.23/index.php
dynamic
[root@upstream conf.d]# curl 192.168.40.23/index.html
static
```
<a name="XZgkH"></a>
# 六、基础配置知识
<a name="kQ2Q3"></a>
## 1、代理知识
代理<br />Syntax: proxy_pass URL; #代理的后端服务器<br />URLDefault:    —<br />Context: location, ifin location, limit_except

头信息<br />Syntax: proxy_set_header field value;<br />Default: proxy_set_header Host $proxy_host;  #设置真实客户端地址   proxy_set_header Connection close;<br />Context: http, server, location

超时<br />Syntax: proxy_connect_timeout time;<br />Default: proxy_connect_timeout 60s;#链接超时<br />Context: http, server, location<br />Syntax: proxy_read_timeout time;<br />Default: proxy_read_timeout 60s;<br />Context: http, server, location

Syntax: proxy_send_timeout time; #nginx进程向fastcgi进程发送request的整个过程的超时时间<br />Default: proxy_send_timeout 60s;<br />Context: http, server, location
<a name="t4wMB"></a>
## 2、反向代理配置


**a、反向代理配置**
```bash
[root@proxy conf.d]# cat proxy.conf
server {
     listen 80;
     server_name 192.168.40.23;

     location / {
     proxy_pass http://192.168.40.24:80;
     proxy_redirect default;
     proxy_set_header Host $http_host;
     proxy_set_header X-Real-IP $remote_addr;
     proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;


     proxy_connect_timeout 30;
     proxy_send_timeout 60;
     proxy_read_timeout 60;
     }

}
[root@proxy conf.d]# nginx -s reload
```
proxy_pass ：真实服务器的地址，可以是ip也可以是域名和url地址<br />proxy_set_header：重新定义或者添加发往后端服务器的请求头<br />proxy_set_header X-Real-IP ：启用客户端真实地址（否则日志中显示的是代理在访问网站）<br />proxy_set_header X-Forwarded-For：记录代理地址<br />proxy_connect_timeout：后端服务器连接的超时时间发起三次握手等候响应超时时间<br />proxy_send_timeout：后端服务器数据回传时间就是在规定时间之内后端服务器必须传完所有的数据<br />proxy_read_timeout ：nginx接收upstream（上游/真实） server数据超时, 默认60s, 如果连续的60s内没有收到1个字节, 连接关闭。像长连接

<a name="eee23b03"></a>
## 3、https实践
为什么需要使用HTTPS, 因为HTTP不安全。当我们使用http网站时，会遭到劫持和篡改，如果采用https协议，那么数据在传输过程中是加密的，所以黑客无法窃取或者篡改数据报文信息，同时也避免网站传输时信息泄露。<br />那么我们在实现https时，需要了解ssl协议，但我们现在使用的更多的是TLS加密协议。<br />那么TLS是怎么保证明文消息被加密的呢？在OSI七层模型中，应用层是http协议，那么在应用层协议之下，我们的表示层，是ssl协议所发挥作用的一层，它通过（握手、交换秘钥、告警、加密）等方式，使应用层http协议没有感知的情况下做到了数据的安全加密<br />![](https://cdn.nlark.com/yuque/0/2020/jpeg/2476579/1601715197167-d781acd1-e0ed-49c4-bd4f-406006b0375b.jpeg#height=1126&id=Ge3sl&originHeight=1126&originWidth=2036&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=2036)<br />那么在数据进行加密与解密过程中，如何确定双方的身份，此时就需要有一个权威机构来验证双方省份。那么这个权威机构则是CA机构。那CA机构又是如何颁发证书<br />![](https://cdn.nlark.com/yuque/0/2020/jpeg/2476579/1601715197257-f46f38f1-6dce-4f19-a792-38041ba7eb8e.jpeg#height=1813&id=uc7lG&originHeight=1813&originWidth=2924&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=2924)<br />我们首先需要申请证书，需要进行登记，登记我是谁，我是什么组织，我想做什么，到了登记机构在通过CSR发给CA，CA中心通过后，CA中心会生成一对公钥和私钥，那么公钥会在CA证书链中保存，公钥和私钥证书订阅人拿到后，会将其部署在WEB服务器上<br />1.当浏览器访问我们的https站点时，它会去请求我们的证书<br />2.Nginx这样的web服务器会将我们的公钥证书发给浏览器<br />3.浏览器会去验证我们的证书是否是合法和有效的。<br />4.CA机构会将过期的证书放置在CRL服务器，那么CRL服务的验证效率是非常差的，所以CA又推出了OCSP响应程序，OCSP响应程序可以查询指定的一个证书是否过期，所以浏览器可以直接查询OCSP响应程序，但OCSP响应程序性能还不是很高。<br />5.Nginx会有一个OCSP的开关，当我们开启后，Nginx会主动上OCSP上查询，这样大量的客户端直接从Nginx获取，证书是否有效。

**_那么证书究竟是怎样组成的呢，接下来我们看一下证书有哪几种类型？_**<br />![](https://cdn.nlark.com/yuque/0/2020/jpeg/2476579/1601715197301-979dca69-2f37-4350-8895-d70c1fcab04d.jpeg#height=1084&id=cef8k&originHeight=1084&originWidth=2280&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=2280)<br />**HTTPS证书购买选择**<br />保护1个域名 `www`<br />保护5个域名 `www images cdn test m`<br />通配符域名 `*.oldjiang.com`<br />**HTTPS注意事项**<br />Https不支持续费,证书到期需重新申请新并进行替换.<br />Https不支持三级域名解析, 如`test.m.oldjiang.com`<br />Https显示绿色, 说明整个网站的url都是https的。<br />Https显示黄色, 因为网站代码中包含http的不安全连接。<br />Https显示红色, 要么证书是假的，要么证书过期。

**配置https**

**1、环境准备**
```bash
[root@ngx ~]#  mkdir -p /etc/nginx/ssl_key
[root@ngx ~]# cd /etc/nginx/ssl_key
[root@ngx ssl_key]#
```
**2.使用openssl命令充当CA权威机构创建证书(生产不使用此方式生成证书，不被互联网认可的黑户证书)**
```bash
[root@ngx ssl_key]# openssl genrsa -idea -out server.key 2048
Generating RSA private key, 2048 bit long modulus
...........................................+++
..............................+++
e is 65537 (0x10001)
Enter pass phrase for server.key:									#记住配置密码
Verifying - Enter pass phrase for server.key:			#记住配置密码
[root@ngx ssl_key]#

```
**3.生成自签证书，同时去掉私钥的密码**
```c
[root@ngx ssl_key]#openssl req -days 36500 -x509 \
-sha256 -nodes -newkey rsa:2048 -keyout server.key -out server.crt
Generating a 2048 bit RSA private key
.....................+++
....+++
writing new private key to 'server.key'
-----
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [XX]:CN
State or Province Name (full name) []:CN
Locality Name (eg, city) [Default City]:BJ
Organization Name (eg, company) [Default Company Ltd]:docker
Organizational Unit Name (eg, section) []:docker
Common Name (eg, your name or your server's hostname) []:www.abc.com
Email Address []:admin@qq.com
```
# req  -->用于创建新的证书<br /># new  -->表示创建的是新证书<br /># x509 -->表示定义证书的格式为标准格式<br /># key  -->表示调用的私钥文件信息<br /># out  -->表示输出证书文件信息<br /># days -->表示证书的有效期

**3、创建配置文件**
```bash
[root@ngx conf.d]# cat https.conf
server {
    listen 443 default ssl;
    server_name www.abc.com;
    #ssl on;
    ssl_certificate   ssl_key/server.crt;
    ssl_certificate_key  ssl_key/server.key;
    ssl_ciphers  HIGH:!aNULL:!MD5;
    ssl_prefer_server_ciphers  on;
    location / {
        root /code;
        index index.html;
    }
}
[root@ngx conf.d]# nginx -s reload
[root@ngx conf.d]# systemctl reload nginx
```


**注意：在nginx1.16之后的版本中取消了ssl on 的配置上文配置当中就直接注销了，** **ssl_prefer_server_ciphers，  ssl_ciphers 可以理解为优化参数但是在1.16的版本是必须的要是没有配置启动之会导致无法访问**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601733366567-70d89eee-1018-4996-b4cb-9221782de4be.png#height=168&id=gMY2a&originHeight=336&originWidth=1401&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23783&status=done&style=none&title=&width=700.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601731788364-cad52c2e-f8fc-4604-9179-e352ef19435e.png#height=501&id=iUhK2&originHeight=1001&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=91010&status=done&style=none&title=&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601731817222-f40d8dcf-c9e4-44ea-920a-4aafbd076315.png#height=91&id=CsKOi&originHeight=182&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25552&status=done&style=none&title=&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601731913987-24268635-765a-4a34-8470-cd895a103235.png#height=747&id=vEYcr&originHeight=1494&originWidth=1918&originalType=binary&ratio=1&rotation=0&showTitle=false&size=173200&status=done&style=none&title=&width=959)
<a name="Y1OBC"></a>
## 4、重写模块rewrite
```bash
[root@ngx conf.d]# cat https.conf
server {
    listen 443 default ssl;
    server_name www.abc.com;
    #ssl on;
    ssl_certificate   ssl_key/server.crt;
    ssl_certificate_key  ssl_key/server.key;
    ssl_ciphers  HIGH:!aNULL:!MD5;
    ssl_prefer_server_ciphers  on;
    location / {
        root /code;
        index index.html;
    }
}

server {
    listen 80;
    server_name www.abc.com;
    return 302 https://$server_name$request_uri;
}
[root@ngx conf.d]# systemctl reload nginx
```
**在浏览器访问www.abc.com**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601733258966-a7590766-cf22-41bb-af21-7d44de39cf93.png#height=113&id=WVKfm&originHeight=226&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23013&status=done&style=none&title=&width=960)<br />备注：通过这种模式访问www.abc.com直接跳转到https://www.abc.com

<a name="toc_2"></a>
## 5、Nginx集群实现HTTPS实践-1

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601775536385-99ee9904-5def-46ea-89b9-db90e85e4d31.png#height=222&id=xj9P4&originHeight=444&originWidth=938&originalType=binary&ratio=1&rotation=0&showTitle=false&size=226297&status=done&style=none&title=&width=469)

| 角色 | ip地址 | 主机名称 |
| --- | --- | --- |
| 负载均衡 | 192.168.40.24 | LB |
| web服务器 | 192.168.40.25 | web-1 |
| web服务器 | 192.168.40.31 | web-2 |

**两台web服务器配置完全相同除了网页显示的文件内容（在生产的环境当中是完全相同的，在实验环境是为了观察到负载均衡的是否实现）**

**web1**
```c
[root@web-1 conf.d]# curl 192.168.40.25
<h1>web1</h1>
[root@web-1 conf.d]#
```


**web2**
```c
[root@web-2 nginx]# curl 192.168.40.31
<h1>web2<h1>
[root@web-2 nginx]#
```
**证书配置**
```c
[root@LB ~]#  mkdir -p /etc/nginx/ssl_key
[root@LB ~]# cd /etc/nginx/ssl_key
[root@LB ssl_key]# openssl genrsa -idea -out server.key 2048
Generating RSA private key, 2048 bit long modulus
...........................................+++
..............................+++
e is 65537 (0x10001)
Enter pass phrase for server.key:									#记住配置密码
Verifying - Enter pass phrase for server.key:			#记住配置密码
[root@LB ssl_key]#
[root@LB ssl_key]#openssl req -days 36500 -x509 \
-sha256 -nodes -newkey rsa:2048 -keyout server.key -out server.crt
Generating a 2048 bit RSA private key
.....................+++
....+++
writing new private key to 'server.key'
-----
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [XX]:CN
State or Province Name (full name) []:CN
Locality Name (eg, city) [Default City]:BJ
Organization Name (eg, company) [Default Company Ltd]:docker
Organizational Unit Name (eg, section) []:docker
Common Name (eg, your name or your server's hostname) []:www.abc.com
Email Address []:admin@qq.com
```
**LB配置**
```c

[root@lb conf.d]# cat lb.conf
# 定义后端资源池
upstream site {
    server 192.168.40.25:80 max_fails=2 fail_timeout=10s;
    server 192.168.40.31:80 max_fails=2 fail_timeout=10s;
}
# https配置
server {
    listen 443 default ssl;
    server_name www.abc.com;
    ssl_certificate  ssl_key/server.crt;
    ssl_certificate_key  ssl_key/server.key;
    ssl_ciphers  HIGH:!aNULL:!MD5;
    ssl_prefer_server_ciphers  on;
    location / {
        proxy_pass http://site;
        #include proxy_params;
    }
}
# 用户http请求跳转至https
server {
    listen 80;
    server_name www.abc.com;
    return 302 https://$server_name$request_uri;
}
[root@lb conf.d]#

```
**访问测试**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601777217733-22aa0295-2024-4ef6-bc3c-a2236e521ffe.png#height=126&id=c9MaN&originHeight=252&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=28370&status=done&style=none&title=&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601777231284-8d43f20d-995b-474a-92ad-348ecd4b9be6.png#height=96&id=e2gKx&originHeight=191&originWidth=1917&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26192&status=done&style=none&title=&width=958.5)<br />**总结：**web服务器监听的是80端口LB监听的是443端口。客户访问通过https协议进行、而LB是通过http协议发送给后端的web真实服务器上。
<a name="PRxrm"></a>
## 6、Nginx集群实现HTTPS实践-2

注意：环境和上面的一模一样增加的只是一行参数

**web-1**
```c
[root@web-1 conf.d]# cat /code/index.html
https-web1
[root@web-1 conf.d]# curl 192.168.40.25
<h1>web1</h1>
[root@web-1 conf.d]# curl www.abc.com
https-web1
[root@web-1 conf.d]# cat web-1.conf
server {
    listen 80;
    server_name www.abc.com;
    location / {
        root /code;
        index index.html;
    }
}

[root@web-1 conf.d]# cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
192.168.40.25   www.abc.com
[root@web-1 conf.d]# cat /code/index.html
https-web1
[root@web-1 conf.d]# curl www.abc.com
https-web1
[root@web-1 conf.d]#
```
**web-2**
```c
[root@web-2 conf.d]# cat web2.conf
server {
    listen 80;
    server_name www.abc.com;
    location / {
        root /code;
        index index.html;
    }
}

[root@web-2 conf.d]# cat /code/index.html
https-web2
[root@web-2 conf.d]# cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
192.168.40.31   www.abc.com
[root@web-2 conf.d]# curl www.abc.com
https-web2
[root@web-2 conf.d]#
```
**LB配置**
```bash
[root@lb conf.d]# cat lb.conf
# 定义后端资源池
upstream site {
    server 192.168.40.25:80 max_fails=2 fail_timeout=10s;
    server 192.168.40.31:80 max_fails=2 fail_timeout=10s;
}
# https配置
server {
    listen 443 default ssl;
    server_name www.abc.com;
    ssl_certificate  ssl_key/server.crt;
    ssl_certificate_key  ssl_key/server.key;
    ssl_ciphers  HIGH:!aNULL:!MD5;
    ssl_prefer_server_ciphers  on;
    location / {
        proxy_pass http://site;
        #include proxy_params;
        proxy_set_header Host $http_host;				#这个是增加的一行、具体是什么作用下面做解释说明
    }
}
# 用户http请求跳转至https
server {
    listen 80;
    server_name www.abc.com;
    return 302 https://$server_name$request_uri;
}
[root@lb conf.d]# nginx -s reload
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601778992547-d5d177fc-33e5-4136-94df-b747056a358b.png#height=80&id=nyzks&originHeight=160&originWidth=1919&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24239&status=done&style=none&title=&width=959.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601779007244-4ebf14f1-a381-44ea-a2a8-582759c52bff.png#height=72&id=ypNsF&originHeight=144&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24246&status=done&style=none&title=&width=960)<br />** **<br />**proxy_set_header Host $http_host; 注释掉重启服务**
```c
[root@lb conf.d]# cat lb.conf
# 定义后端资源池
upstream site {
    server 192.168.40.25:80 max_fails=2 fail_timeout=10s;
    server 192.168.40.31:80 max_fails=2 fail_timeout=10s;
}
# https配置
server {
    listen 443 default ssl;
    server_name www.abc.com;
    ssl_certificate  ssl_key/server.crt;
    ssl_certificate_key  ssl_key/server.key;
    ssl_ciphers  HIGH:!aNULL:!MD5;
    ssl_prefer_server_ciphers  on;
    location / {
        proxy_pass http://site;
        #include proxy_params;
        #proxy_set_header Host $http_host;
    }
}
# 用户http请求跳转至https
server {
    listen 80;
    server_name www.abc.com;
    return 302 https://$server_name$request_uri;
}
[root@lb conf.d]# nginx -s reload
[root@lb conf.d]#

```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601779098491-d1debdf5-0c31-46c0-9608-44ef696706c8.png#height=104&id=f6LrG&originHeight=207&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26462&status=done&style=none&title=&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601779108758-77028012-1312-4323-b635-92a0487f2025.png#height=87&id=ZjSWm&originHeight=173&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25777&status=done&style=none&title=&width=960)<br />**解释说明：**浏览器直接访问 nginx，获取到的 Host 包含浏览器请求的 IP 和端口，通俗的讲就是做反向代理的时候并没有读取web{1,2}中的配置信息而是直接做的负载到web{1、2}中将nginx中的默认配置返回给负载均衡、所以我们看到也就是默认的页面。

<a name="Z1nCT"></a>
# 七、nginx高可用

<a name="fSd0c"></a>
## 1、keepalived配置

服务的高可用一般是使用的是keepalived这种软件实现（可以用使用硬件实现）是基于VRRP协议实现的

**1、环境说明**

| 角色 | IP地址 | 主机名称 |
| --- | --- | --- |
| 高可用 | 192.168.40.13 | lb2 |
| 负载均衡 | 192.168.40.24 | lb |
| nginx | 192.168.40.25 | web1 |
| nginx | 192.168.40.31 | web2 |

**2、master配置**
```c
[root@lb ~]# cat /etc/keepalived/keepalived.conf
[root@lb nginx]# cat /etc/keepalived/keepalived.conf
global_defs {
   router_id lb1
}

vrrp_instance VI_1 {
    state MASTER
    interface eno16777736
    virtual_router_id 51
    priority 150
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass 1111
    }
    virtual_ipaddress {
        192.168.40.3
    }
}
[root@lb ~]# 
```
**3、backup配置**
```c
[root@lb2 ~]# cat /etc/keepalived/keepalived.conf
global_defs {
   router_id lb2
}

vrrp_instance VI_1 {
    state BACKUP
    interface eno16777728
    virtual_router_id 51
    priority 100
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass 1111
    }
    virtual_ipaddress {
        192.168.40.3
    }
}

[root@lb2 ~]#
```
**4、先启动backup观察日志**

```c
[root@lb2 ~]# tailf /var/log/messages
Oct  4 10:53:14 [localhost] systemd: Reloading.
Oct  4 10:53:21 [localhost] systemd: Starting LVS and VRRP High Availability Monitor...
Oct  4 10:53:21 [localhost] Keepalived[12805]: Starting Keepalived v1.3.5 (03/19,2017), git commit v1.3.5-6-g6fa32f2
Oct  4 10:53:21 [localhost] Keepalived[12805]: Opening file '/etc/keepalived/keepalived.conf'.
Oct  4 10:53:21 [localhost] systemd: Started LVS and VRRP High Availability Monitor.
Oct  4 10:53:21 [localhost] Keepalived[12806]: Starting Healthcheck child process, pid=12807
Oct  4 10:53:21 [localhost] Keepalived[12806]: Starting VRRP child process, pid=12808
Oct  4 10:53:21 [localhost] Keepalived_vrrp[12808]: Registering Kernel netlink reflector
Oct  4 10:53:21 [localhost] Keepalived_vrrp[12808]: Registering Kernel netlink command channel
Oct  4 10:53:21 [localhost] Keepalived_vrrp[12808]: Registering gratuitous ARP shared channel
Oct  4 10:53:21 [localhost] Keepalived_vrrp[12808]: Opening file '/etc/keepalived/keepalived.conf'.
Oct  4 10:53:21 [localhost] Keepalived_vrrp[12808]: VRRP_Instance(VI_1) removing protocol VIPs.
Oct  4 10:53:21 [localhost] Keepalived_vrrp[12808]: Using LinkWatch kernel netlink reflector...
Oct  4 10:53:21 [localhost] Keepalived_vrrp[12808]: VRRP_Instance(VI_1) Entering BACKUP STATE
Oct  4 10:53:21 [localhost] Keepalived_vrrp[12808]: VRRP sockpool: [ifindex(2), proto(112), unicast(0), fd(10,11)]
Oct  4 10:53:21 [localhost] kernel: IPVS: Registered protocols (TCP, UDP, SCTP, AH, ESP)
Oct  4 10:53:21 [localhost] kernel: IPVS: Connection hash table configured (size=4096, memory=64Kbytes)
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=0
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=1
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=2
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=3
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=4
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=5
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=6
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=7
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=8
Oct  4 10:53:21 [localhost] kernel: IPVS: Creating netns size=2048 id=9
[root@lb2 ~]# ip a|grep 10.0.0.3
    inet 10.0.0.3/32 scope global eno16777728

```
**5、在启动master节点查看日志信息**
```c
Oct  4 11:01:01 [localhost] systemd: Started Session 7 of user root.
Oct  4 11:01:33 [localhost] Keepalived_vrrp[12808]: VRRP_Instance(VI_1) Transition to MASTER STATE
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: VRRP_Instance(VI_1) Entering MASTER STATE
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: VRRP_Instance(VI_1) setting protocol VIPs.
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: Sending gratuitous ARP on eno16777728 for 192.168.40.3
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: VRRP_Instance(VI_1) Sending/queueing gratuitous ARPs on eno16777728 for 192.168.40.3
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: Sending gratuitous ARP on eno16777728 for 192.168.40.3
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: Sending gratuitous ARP on eno16777728 for 192.168.40.3
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: Sending gratuitous ARP on eno16777728 for 192.168.40.3
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: Sending gratuitous ARP on eno16777728 for 192.168.40.3
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: VRRP_Instance(VI_1) Received advert with higher priority 150, ours 100
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: VRRP_Instance(VI_1) Entering BACKUP STATE
Oct  4 11:01:34 [localhost] Keepalived_vrrp[12808]: VRRP_Instance(VI_1) removing protocol VIPs.
[root@lb ~]# ip a|grep 192.168.40.3
    inet 192.168.40.3/32 scope global eno16777736
[root@lb ~]#
```
<a name="XBM1c"></a>
## 2、keep访问测试
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601793717552-c922904b-8ed7-4236-9881-f06a9945f560.png#height=100&id=W721R&originHeight=199&originWidth=1917&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26103&status=done&style=none&title=&width=958.5)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601793727665-477bc5c7-899b-4ef0-b0e7-f49dd3173ace.png#height=75&id=u5Cvj&originHeight=150&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24290&status=done&style=none&title=&width=960)

**查看arp与Mac地址绑定信息**
```c
C:\Users\jiang>arp -a|find "192.168.40."
接口: 192.168.40.1 --- 0x3
  192.168.40.3          00-50-56-3d-37-eb     动态		#这个是集群的VIP地址
  192.168.40.13         00-0c-29-a3-92-22     动态
  192.168.40.24         00-50-56-3d-37-eb     动态		#这个是集群的中的master节点的Mac地址
  192.168.40.25         00-0c-29-da-ab-cb     动态
  192.168.40.31         00-0c-29-ca-cf-76     动态
  192.168.40.255        ff-ff-ff-ff-ff-ff     静态

C:\Users\jiang>
```
可以看出的是VIP绑定到的是master节点上面

**关闭其中的一台keepalived访问测试**
```c
[root@lb nginx]# systemctl stop keepalived.service
[root@lb nginx]# systemctl status keepalived.service
● keepalived.service - LVS and VRRP High Availability Monitor
   Loaded: loaded (/usr/lib/systemd/system/keepalived.service; enabled; vendor preset: disabled)
   Active: inactive (dead) since Sun 2020-10-04 02:42:35 EDT; 8s ago
  Process: 5938 ExecStart=/usr/sbin/keepalived $KEEPALIVED_OPTIONS (code=exited, status=0/SUCCESS)
 Main PID: 5939 (code=exited, status=0/SUCCESS)

Oct 04 02:41:49 lb Keepalived_vrrp[5941]: VRRP_Instance(VI_1) Sending/queueing gratuitous ARPs on eno16777736 for 192.168.40.3
Oct 04 02:41:49 lb Keepalived_vrrp[5941]: Sending gratuitous ARP on eno16777736 for 192.168.40.3
Oct 04 02:41:49 lb Keepalived_vrrp[5941]: Sending gratuitous ARP on eno16777736 for 192.168.40.3
Oct 04 02:41:49 lb Keepalived_vrrp[5941]: Sending gratuitous ARP on eno16777736 for 192.168.40.3
Oct 04 02:41:49 lb Keepalived_vrrp[5941]: Sending gratuitous ARP on eno16777736 for 192.168.40.3
Oct 04 02:42:34 lb Keepalived[5939]: Stopping
Oct 04 02:42:34 lb systemd[1]: Stopping LVS and VRRP High Availability Monitor...
Oct 04 02:42:34 lb Keepalived_vrrp[5941]: VRRP_Instance(VI_1) sent 0 priority
Oct 04 02:42:34 lb Keepalived_vrrp[5941]: VRRP_Instance(VI_1) removing protocol VIPs.
Oct 04 02:42:35 lb systemd[1]: Stopped LVS and VRRP High Availability Monitor.
[root@lb nginx]#
```
**查看ARP信息**
```c
C:\Users\jiang>arp -a|find "192.168.40."
接口: 192.168.40.1 --- 0x3
  192.168.40.3          00-0c-29-a3-92-22     动态		#这个是集群的VIP地址
  192.168.40.13         00-0c-29-a3-92-22     动态		#这个是集群的中的master节点的Mac地址
  192.168.40.24         00-50-56-3d-37-eb     动态
  192.168.40.25         00-0c-29-da-ab-cb     动态
  192.168.40.31         00-0c-29-ca-cf-76     动态
  192.168.40.255        ff-ff-ff-ff-ff-ff     静态

C:\Users\jiang>
```
可以看到的是VIP已经与backup绑定了、通过这种方式也可以验证keepalived是采用的是VRRP协议<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601793975218-33ed5eff-0d7b-40ed-9ce9-b5e1bb5fe9c2.png#height=80&id=Vn3qJ&originHeight=160&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25276&status=done&style=none&title=&width=960)



**nginx+keepalived实现网站的高可用就完成了、但是这种方式并不是非常的完美、比如在生产的环境当中是使用的用户名+密码的方式进行登录验证、如果采用这种模式进行高可用的话会导致页面的丢失、可以参考前面的会话保持这种模式实现考可用解决生产环境当中的cookie会话信息**
<a name="fxMAt"></a>
## 3、keepalived高可用脑裂

**背景：**<br />由于某些原因，导致两台keepalived高可用服务器在指定时间内，无法检测到对方的心跳消息，各自取得资源及服务的所有权，而此时的两台高可用服务器又都还活着。<br />产生这种事情的原因有哪些：服务器网线松动、服务器硬件发生损坏而导致崩溃、NGINX服务杀掉、主备都开启的firewalld防火墙。

**主备都开启firewalld**<br />lb2
```c
[root@lb2 keepalived]# systemctl start firewalld.service
[root@lb2 keepalived]# ip a|grep 192.168.40.3
    inet 192.168.40.3/32 scope global eno16777728
```
lb
```c
[root@lb nginx]# systemctl start firewalld.service
[root@lb nginx]# ip a|grep 192.168.40.3
    inet 192.168.40.3/32 scope global eno16777736

```

访问页面测试<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601799359741-929ec1f8-bbd7-467f-a1c7-e0dbf2feca93.png#height=422&id=RdArR&originHeight=843&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=97490&status=done&style=none&title=&width=960)<br />如果在真实的环境当中发生这种情况随机的关闭一台机器就可以正常的访问下面的环境是关闭其中一台的keepalived在另外的一台机器上面防火墙放行http和https
```c
[root@lb2 keepalived]# firewall-cmd --permanent --add-service=http
success
[root@lb2 keepalived]# firewall-cmd --permanent --add-service=https
success
[root@lb2 keepalived]# firewall-cmd --reload
success
[root@lb2 keepalived]#
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601799892915-79c0d4ec-e83e-40d5-916b-12c4d271f6b0.png#height=94&id=jYhV7&originHeight=188&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25648&status=done&style=none&title=&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1601799901677-7a8347e2-2b30-47ce-baf5-7b582fbecb6b.png#height=86&id=kR88X&originHeight=171&originWidth=1917&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25306&status=done&style=none&title=&width=958.5)<br />要是不关闭keepalived在两台机器上面都放行防火墙的话也可以解决访问的问题但是、消耗的资源特别的高、不到万不得已的环境不要这样做。

**监控方案在备机上面执行**
```c
[root@lb2 opt]# cat brain.sh
#!/usr/bin/bash
lb_vip=192.168.40.3
lb_ip=192.168.40.24
while true;
do
     ping -c 2 -W 3 $lb_ip &>/dev/null
     if [ $? -eq 0 -a `ip add|grep "lb_vip"|wc -l` -eq 1 ];then
        echo "this is server spilt brain warning."
     else
        echo "this is ok"
     fi
sleep 5
done
[root@lb2 opt]#
```
**脚本思路：在备机上面ping （lb_vip）master主机要是能够通信说明master机器在线要是结果不等于0，backup机器就会直接切换为master角色。要是(lb_vip)master机器在线并且backup机器上面还有192.168.40.3这个VIP地址这说明发生了脑裂。后面可以将这个通过zabbix通过钉钉、微信这些媒介报警。**

**备注：这个时候应该计算这段时间造成的损失有多大、数据的丢失问题、这个问题什么时候能够得到解决。**

<a name="zWi7c"></a>
## 4、nginx负载均衡宕机

影响：会导致用户无法正常的访问web服务器、该服务器的VIP也无法进行地址漂移

**解决思路：**要是nginx发生故障，则就把keepalived服务停止这样避免继续调用服务导致用户无法访问
```c
[root@lnmp opt]# cat code.sh
#!/usr/bin/bash
code=$(curl -I -s -o /dev/null -w %{http_code} 127.0.0.1)
while true;
do
if [ $code -ne 200 ];then
        echo "nginx error"
        sleep 3
        if [ $code -ne 200 ];then
                systemctl stop nginx
                systemctl stop keepalived
        fi
else
        echo "nginx health"
fi

sleep 2
done
```

<a name="L832E"></a>
## 5、LNMP架构 

1. **安装nginx**
```bash
[root@lnmp ~]# vim /etc/yum.repos.d/nginx.repo
[nginx-stable]
name=nginx stable repo
baseurl=http://nginx.org/packages/centos/$releasever/$basearch/
gpgcheck=1
enabled=1
gpgkey=https://nginx.org/keys/nginx_signing.key
module_hotfixes=true

[nginx-mainline]
name=nginx mainline repo
baseurl=http://nginx.org/packages/mainline/centos/$releasever/$basearch/
gpgcheck=1
enabled=0
gpgkey=https://nginx.org/keys/nginx_signing.key
module_hotfixes=true
[root@lnmp ~]# yum install -y nginx
```

2. **安装PHP-fpm**
```c
[root@lnmp ~]# yum install -y https://mirrors.tuna.tsinghua.edu.cn/remi/enterprise/remi-release-7.rpm
[root@lnmp ~]# sed -e 's!^mirrorlist=!#mirrorlist=!g'      -e 's!^#baseurl=!baseurl=!g' -e 's!http://rpms\.remirepo\.net!https://mirrors.tuna.tsinghua.edu.cn/remi!g' -i /etc/yum.repos.d/remi-safe.repo /etc/yum.repos.d/remi-php73.repo
[root@lnmp ~]# yum-config-manager --enable remi-php73
[root@lnmp ~]# yum install -y php php-pdo php-opcache php-mbstring php-fpm php-gd php-xml php-pdo php-pecl-mcrypt php-mysqlnd
```

3. **安装mysql**
```c
[root@lnmp yum.repos.d]# cat mariadb.repo
# MariaDB 10.2 CentOS repository list - created 2017-07-03 06:59 UTC
# http://downloads.mariadb.org/mariadb/repositories/
[mariadb]
name = MariaDB
baseurl = https://mirrors.ustc.edu.cn/mariadb/yum/10.2/centos7-amd64
gpgkey=https://mirrors.ustc.edu.cn/mariadb/yum/RPM-GPG-KEY-MariaDB
gpgcheck=1
[root@lnmp yum.repos.d]# yum install mariadb-server mariadb
Loaded plugins: fastestmirror, langpacks
base                                                                                                                                                              | 3.6 kB  00:00:00
docker-ce-stable                                                                                                                                                  | 3.5 kB  00:00:00
epel                                                                                                                                                              | 4.7 kB  00:00:00
extras                                                                                                                                                            | 2.9 kB  00:00:00
mariadb                                                                                                                                                           | 2.9 kB  00:00:00
remi-php73                                                                                                                                                        | 3.0 kB  00:00:00
remi-safe                                                                                                                                                         | 3.0 kB  00:00:00
tuna                                                                                                                                                              | 3.6 kB  00:00:00
updates                                                                                                                                                           | 2.9 kB  00:00:00
mariadb/primary_db                                                                                                  
```
**4、配置PHP**
```c
[root@lnmp ~]# egrep -v "^$|;" /etc/php-fpm.d/www.conf
[www]
user = nginx								#默认为apache换为nginx
group = nginx								#默认为apache换为nginx
listen = /var/run/php-fpm/php-fpm.sock		#默认是监听的是9000端口换成socket文件
listen.owner = nginx						#默认为www换为nginx
listen.group = nginxx						#默认为www换为nginx
listen.allowed_clients = 127.0.0.1
pm = dynamic
pm.max_children = 50
pm.start_servers = 5
pm.min_spare_servers = 5
pm.max_spare_servers = 35
slowlog = /var/log/php-fpm/www-slow.log
php_admin_value[error_log] = /var/log/php-fpm/www-error.log
php_admin_flag[log_errors] = on
php_value[session.save_handler] = files
php_value[session.save_path]    = /var/lib/php/session
php_value[soap.wsdl_cache_dir]  = /var/lib/php/wsdlcache
[root@lnmp ~]# systemctl restart php-fpm.service
[root@lnmp ~]#
```


**5、配置nginx支持PHP并测试**
```bash
[root@VM-0-3-centos conf.d]# pwd
/etc/nginx/conf.d
[root@VM-0-3-centos conf.d]# cat https.conf
server {
    listen 443 default ssl;
    server_name www.chuangyu.xyz;
    #ssl on;
    ssl_certificate   ssl_key/chuangyu.xyz_bundle.crt;
    ssl_certificate_key  ssl_key/chuangyu.xyz.key;
    ssl_ciphers  HIGH:!aNULL:!MD5;
    ssl_prefer_server_ciphers  on;
    root /code;
    index index.php;
        location / {
    try_files $uri $uri/ /index.php$is_args$args;
        }
        location ~ \.php$ {
                      root           /code; #html;  #/code;
                      fastcgi_pass   127.0.0.1:9000;
                      fastcgi_index  index.php;
                      fastcgi_param  SCRIPT_FILENAME $document_root$fastcgi_script_name;
                    include        fastcgi_params;
        }
}

server {
    listen 80;
    server_name www.chuangyu.xyz;
    return 302 https://$server_name$request_uri;
}

```
**6、配置mysql**
```bash
create database wordpress;
CREATE USER 'wordpress'@'localhost' IDENTIFIED BY 'wordpress';
GRANT ALL ON wordpress.* to wordpress@'localhost' IDENTIFIED BY 'wordpress';
FLUSH privileges;
```

![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1656997064242-b5e9e8ca-2bc6-4aee-b7cc-d8357be67379.png#clientId=ud2dc5c76-190e-4&from=paste&height=530&id=ub395e338&originHeight=663&originWidth=1515&originalType=binary&ratio=1&rotation=0&showTitle=false&size=47341&status=done&style=none&taskId=ucbed896b-8ac1-4a2a-80a1-04b6d0fa977&title=&width=1212)











