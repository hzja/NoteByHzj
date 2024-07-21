Linux httpie 
<a name="bwVC1"></a>
## httpie的官网
[httpie的官网](https://httpie.org/)<br />[https://httpie.org/](https://httpie.org/)
<a name="TmvyI"></a>
## httpie的下载
```bash
$ yum install httpie
```
<a name="P8x7P"></a>
## httpie的使用
<a name="nZiSb"></a>
### ①自定义的HTTP方法，请求和json数据
```bash
$ http PUT httpbin.org/put X-API-Token:123 name=John
```
<a name="4e340038"></a>
### ②提交表单
```bash
$ http -f POST httpbin.org/post hello=World
```
<a name="go2QJ"></a>
### ③查看正在发送的请求
```bash
$ http -v httpbin.org/get
```
<a name="mYGh9"></a>
### ④上传文件
```bash
$ http httpbin.org/post < files/data.json
```
<a name="561f8f5e"></a>
### ⑤下载文件并重定向到指定目录文件
```bash
$ http httpbin.org/image/png > image.png
```
<a name="w4Oyw"></a>
### ⑥使用session会话
```bash
$ http --session=logged-in -a username:password httpbin.org/get API-Key:123
```

