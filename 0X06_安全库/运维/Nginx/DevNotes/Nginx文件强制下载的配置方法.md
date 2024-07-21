Nginx<br />有时可能需要强制用户从网站下载内容，而不是在他们的网站上查看。如果网站有很多可下载的内容，这一点很重要。在本文中将了解如何在 NGINX 中强制下载文件。强制文件下载可防止用户在服务器上流式传输这些文件并减少网站的服务器负载。<br />现在的浏览器能够识别很多文件类型(txt,pdf,jpg...)，并在浏览器中自动打开<br />**情况一：**<br />添加头信息`Content-Disposition: "attachment;"`会使浏览器强制下载：<br />代码如下：
```nginx
#表示浏览器内嵌显示一个文件
Content-disposition: inline; filename=foobar.pdf
 
#表示会下载文件,如火狐浏览器中
Content-disposition: attachment; filename=foobar.pdf
```
Nginx配置如下，在相应的server中添加如下location：<br />代码如下：
```nginx
location /download {
    add_header Content-Disposition "attachment;";
}
```
**情况二：**<br />有这么一个需求，对于图片文件和pdf等文件链接，只要访问实在某个路径下，不能在浏览器中打开图片，而是提示用户保存到本地，文件名使用访问的文件名。<br />这个问题主要是IE造成的，不管mime类型是什么，比如人工把图片的mime类型设置为octet-stream，如果浏览器认识文件后缀的话，仍然会在浏览器中打开图片。<br />解决方案：<br />在响应的http头添加：`Content-Disposition: attachment; filename=文件名`<br />Nginx配置如下：
```nginx
location ~ ^/somepath/(.*)$ {
  add_header Content-Disposition "attachment; filename=$1";
  alias "E:/apache-tomcat-7.0.32/webapps/upload/$1";
}
```
这里使用到了正则表达式，捕获请求的文件名。<br />另外，需要注意Nginx的location优先级，首先是`=`，然后是`^~`，最后才是`~`。<br />基本上，需要在要强制下载的 URL 的位置块中添加以下行。
```nginx
add_header Content-disposition "attachment; filename=$1";
default_type application/octet-stream;
```
上面两行将内容处置标头设置为“附件”，将内容类型设置为“应用程序/八位字节流”以启用下载。<br />例如，如果想对以 /downloads 开头的所有 URL 强制下载，则在该文件夹的位置块中添加上述行，如下所示。
```nginx
location /downloads {
   ...
   add_header Content-disposition "attachment; filename=$1";
   default_type application/octet-stream;
   ...
}
```
如果想强制下载以某些文件类型和扩展名（例如 .jpg、.png、.mp3 等）结尾的所有文件，请在用于这些文件类型的位置块中添加上述 2 行。
```nginx
location ~* ^/.+\.(?:gif|jpe?g|png|mp4|mp3)$ {
   ...
   add_header Content-disposition "attachment; filename=$1";
   default_type application/octet-stream;
   ...
}
```
以上修改完成后需要重启Nginx，或重新加载
```bash
nginx -s reload
```
