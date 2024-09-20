Linux tldr 
<a name="BZnqe"></a>
## tldr官网
[tldr官网](https://tldr.sh/)<br />[https://tldr.sh/](https://tldr.sh/)
<a name="x4rTj"></a>
## tldr简介
tldr 命令行工具显示简化的命令用法信息，主要包括示例。<br />这个工具不能代替 man。man 页面仍然是许多工具规范而完整的信息源。然而，在某些情况下，man 提供的信息太多了。有时候，不需要一个命令的所有信息；只是试着记住基本的选项。例如，curl 命令的 man 页面几乎有 3000 行。相反，curl 的 tldr 页面只有 40 行，如下所示：
```bash
$ tldr curl
# curl
Transfers data from or to a server.
Supports most protocols, including HTTP, FTP, and POP3.
More information: <https://curl.haxx.se>.
- Download the contents of an URL to a file:
curl http://example.com -o filename
- Download a file, saving the output under the filename indicated by the 
URL:  
curl -O http://example.com/filename
- Download a file, following [L]ocation redirects, and automatically 
[C]ontinuing (resuming) a previous file transfer:  
curl -O -L -C - http://example.com/filename
- Send form-encoded data (POST request of type `application/x-www-form-urlencoded`):
curl -d 'name=bob' http://example.com/form    
- Send a request with an extra header, using a custom HTTP method:  
curl -H 'X-My-Header: 123' -X PUT http://example.com 
- Send data in JSON format, specifying the appropriate content-type header:  
curl -d '{"name":"bob"}' -H 'Content-Type: application/json' 
http://example.com/users/1234
... TRUNCATED OUTPUT
```
TLDR 是网络俚语“too long; didn't read”的缩写，指对一篇很长的文章进行总结。这个名称很适合这个工具，因为 man 页面虽然有用，但有时太长了。<br />在 Fedora 中，tldr 客户端是用 Python 编写的。可以使用 dnf 安装它。<br />一般来说，tldr 工具需要访问互联网以查阅 tldr pages。Fedora 中的 Python 客户端允许下载并缓存这些页面以供脱机访问。<br />要了解有关 tldr 的更多信息，可以使用 tldr tldr。<br />
