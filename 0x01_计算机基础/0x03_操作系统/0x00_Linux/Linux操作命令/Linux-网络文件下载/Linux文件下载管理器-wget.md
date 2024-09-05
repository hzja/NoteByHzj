Linux 文件下载 wget
<a name="YRxtT"></a>
### 使用wget下载文件或网页
只需要提供文件或网页的URL。它将以默认原始名称下载文件到当前命令执行的目录中。
```bash
wget URL
```
如果提供一个默认为 index.html 的 URL，那么就会下载该索引页。默认情况下，文件会被下载到当前的工作目录，并保持原来的名字。
```bash
$ wget http://example.com
--2021-09-20 17:23:47-- http://example.com/
Resolving example.com... 93.184.216.34, 2606:2800:220:1:248:1893:25c8:1946
Connecting to example.com|93.184.216.34|:80... connected.
HTTP request sent, awaiting response... 200 OK
Length: 1256 (1.2K) [text/html]
Saving to: 'index.html'
```
通过使用 `--output-document` 和 `-` 符号，可以指示 [wget](https://mp.weixin.qq.com/s?__biz=MjM5NjQ4MjYwMQ==&mid=2664644297&idx=3&sn=1312548adf5ce1b85090170542071e9a&chksm=bdcf058f8ab88c99b848c7ff93d4405bc8683c3dff7a65b9d96c830f3222d321f9bfc9bcadf5&mpshare=1&scene=23&srcid=1122ReyKZT1VBB21O5B9dBwH&sharer_sharetime=1637579881789&sharer_shareid=8ed0e7a09ec3771f5e5e6747e35e611c) 将数据发送到 标准输出(stdout)：
```bash
$ wget http://example.com --output-document - | head -n4
<!doctype html>
<html>
<head>
   <title>Example Domain</title>
```
可以使用 `--output-document` 选项（简写为 `-O`）将下载文件命名为任何想要的名称：
```bash
$ wget http://example.com --output-document foo.html
```
<a name="lTOU6"></a>
### 使用wget下载多个文件
要下载多个文件，必须将它们的URL保存在一个文本文件中，并提供该文本文件作为wget的输入，如下所示：
```bash
wget -i download_files.txt
```
<a name="EKGWg"></a>
### 使用wget下载名称不同的文件
网页几乎总是以index.html的形式保存在wget中。为下载的文件提供自定义名称。可以使用-O（大写字母O）选项来提供下载时的输出文件名。
```bash
wget -O filename URL
```
<a name="SWgHI"></a>
### 使用wget下载文件夹
浏览FTP服务器，并且需要下载整个目录，则可以使用递归选项
```bash
wget -r ftp://server-address.com/directory
```
<a name="XkUZL"></a>
### 使用wget下载整个网站
可以用wget镜像整个网站。整个网站是指整个面向公众的网站结构。虽然可以直接使用mirror选项`-m`，但是最好添加：

- `–convert-links`：转换链接，以便内部链接指向下载的资源而不是Web
- `–page-conditions`：下载其他内容，例如样式表，以使页面在脱机状态下看起来更好
```bash
wget -m --convert-links --page-requisites website_addres
```
<a name="tGauB"></a>
### 恢复未完成的下载
如果因为某些原因按C键中止了下载，可以用选项`-c`恢复之前的下载。
```bash
wget -c
```
<a name="eQ4R3"></a>
### 断点续传
如果正在下载一个超大文件，可能会遇到中断下载的情况。使用 `--continue`（简写为 `-c`），wget 可以确定从文件的哪个位置开始继续下载。也就是说，下次在下载一个 4 GB 的 Linux 发行版 ISO 时，如果出现了错误导致中断，不必重新从头开始。
```bash
$ wget --continue https://example.com/linux-distro.iso
```
<a name="fppRA"></a>
### 下载一系列的文件
如果要下载的不是单个大文件，而是一系列的文件，wget 也能提供很好的帮助。假如知道要下载文件的路径以及文件名的通用范式，可以使用 Bash 语法指示一个数字范围的起始和终点来表示这一系列文件名:
```bash
$ wget http://example.com/file_{1..4}.webp
```
<a name="ypmbn"></a>
### 镜像整个站点
使用 `--mirror` 选项可以下载整个站点，包括它的目录结构。这与使用选项 `--recursive --level inf --timestamping --no-remove-listing` 的效果是一样的，该选项表明可以进行无限制的递归，得到指定域下的所有内容。但也可能会得到比预期多得多的内容，这取决于站点本身的老旧程度。<br />如果正在使用 wget 来打包整个站点，选项 `--no-cookies --page-requisites --convert-links` 非常有用，它可以确保打包的站点是全新且完整的，站点副本或多或少是自包含的(self-contained)。
<a name="gr1wx"></a>
### 修改 HTML 标头
在计算机发送的通信报文里含有大量用于数据交换的元数据(metadata)。HTTP 标头是初始数据的组成部分。当浏览某个站点时，浏览器会发送 HTTP 请求标头。使用 `--debug` 选项可以查看 wget 为每个请求发送了什么样的标头信息:
```bash
$ wget --debug example.com
---request begin---
GET / HTTP/1.1
User-Agent: Wget/1.19.5 (linux-gnu)
Accept: */*
Accept-Encoding: identity
Host: example.com
Connection: Keep-Alive
---request end---
```
可以使用 `--header` 选项修改请求标头。实际上经常使用这种方式来模仿某特定浏览器，来测试或兼容某些编码糟糕、只能与特定代理通信的站点。<br />让请求被识别为来自 Windows 系统的 Microsoft Edge：
```bash
$ wget --debug --header="User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36 Edg/91.0.864.59" http://example.com
```
也可以假装为某个移动设备：
```bash
$ wget --debug --header="User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 13_5_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.1.1 Mobile/15E148 Safari/604.1" http://example.com
```
<a name="CAHKg"></a>
### 查看响应标头
与浏览器发送请求标头的方式一样，响应也包含有标头信息。可以使用 `--debug` 选项来查看响应中的标头信息：
```bash
$ wget --debug example.com
[...]
---response begin---
HTTP/1.1 200 OK
Accept-Ranges: bytes
Age: 188102
Cache-Control: max-age=604800
Content-Type: text/html; charset=UTF-8
Etag: "3147526947"
Server: ECS (sab/574F)
Vary: Accept-Encoding
X-Cache: HIT
Content-Length: 1256
---response end---
200 OK
Registered socket 3 for persistent reuse.
URI content encoding = 'UTF-8'
Length: 1256 (1.2K) [text/html]
Saving to: 'index.html'
```
<a name="YCHzQ"></a>
### 处理 301 响应
200 响应码意味着一切都在预料之中。而 301 响应则表示 URL 已经被永久迁移到了另外一个地方。这是站点管理员的一种常用手段，内容迁移后，为访问旧地址的用户留下寻找新地址的“线索”。wget 会默认跟随重定向(redirect)，这也是大部分情况下用户所希望的。<br />当然，可以使用 `--max-redirect` 选项，用于控制 wget 默认处理 301 响应重定向的次数。设置为 0 意味着不会自动重定向到新的地址:
```bash
$ wget --max-redirect 0 http://iana.org
--2021-09-21 11:01:35-- http://iana.org/
Resolving iana.org... 192.0.43.8, 2001:500:88:200::8
Connecting to iana.org|192.0.43.8|:80... connected.
HTTP request sent, awaiting response... 301 Moved Permanently
Location: https://www.iana.org/ [following]
0 redirections exceeded.
```
同时，也可以设置为其他的数值来控制 [wget](https://mp.weixin.qq.com/s?__biz=MjM5NjQ4MjYwMQ==&mid=2664644297&idx=3&sn=1312548adf5ce1b85090170542071e9a&chksm=bdcf058f8ab88c99b848c7ff93d4405bc8683c3dff7a65b9d96c830f3222d321f9bfc9bcadf5&mpshare=1&scene=23&srcid=1122ReyKZT1VBB21O5B9dBwH&sharer_sharetime=1637579881789&sharer_shareid=8ed0e7a09ec3771f5e5e6747e35e611c) 能重定向多少次。
<a name="SO1kg"></a>
### 展开 URL 缩写
使用`--max-redirect` 选项用于在实际访问之前查看 URL 缩写(shortened URL)非常有用。缩写 URL 可用于用户无法完整拷贝和粘贴一个长 URL 时的印刷媒体(print media)，或是具有字数限制的社交网络（在类似 Mastondon🔗 opensource.com 这种现代开源的社交网络上这并不是一个问题）。这种缩写具有一定的风险，因为本质上这些目的地是隐藏的。组合使用 `--head` 选项和 `--location` 选项来来查看 HTTP 头部并解开最终的目的地，可以在不加载整个资源的情况下查看到缩写 URL 的完整内容：
```bash
$ wget --max-redirect 0 "https://bit.ly/2yDyS4T"
--2021-09-21 11:32:04-- https://bit.ly/2yDyS4T
Resolving bit.ly... 67.199.248.10, 67.199.248.11
Connecting to bit.ly|67.199.248.10|:443... connected.
HTTP request sent, awaiting response... 301 Moved Permanently
Location: http://example.com/ [following]
0 redirections exceeded.
```
从 Location 开始的倒数第二行输出，展示了实际的目的地。
