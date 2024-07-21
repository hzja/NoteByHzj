Nginx <br />root和alias属于nginx的核心模块ngx_http_core_module的两个指令。从官网的如下链接看到root和alias的基本语法。<br />[http://nginx.org/en/docs/http/ngx_http_core_module.html#root](http://nginx.org/en/docs/http/ngx_http_core_module.html#root)<br />[http://nginx.org/en/docs/http/ngx_http_core_module.html#alias](http://nginx.org/en/docs/http/ngx_http_core_module.html#alias)<br />root指令语法
```
语  法:    root path;
默认值:    root html;
配置段:    http, server, location, if in location
```
alias指令语法
```
语  法:    alias path;
默认值:    —
配置段:    location
```
从语法上看root和alias指令都可以定义在location模块中，都是用来指定映射服务器静态资源文件的命令。例如，访问的html文件、视频、图片文件等都可以通过root和alias指令配置访问。虽然使用方法比较相似，但是非常容易混淆，特别是root命令的使用方式，经常会被误解。
<a name="K5eIB"></a>
### 1、root与alias用法演示
<a name="aEETd"></a>
#### root配置如下
```nginx
location /blog/ {
  root /usr/local/nginx/html;
  index index.html index.htm;
}
```
请求 http://127.0.0.1:80/blog/root.html 这个地址时，那么在服务器里面对应的真正的资源是 /usr/local/nginx/html/blog/root.html文件。可以发现真实的路径是root指定的值加上location指定的值。
<a name="mPKcj"></a>
#### alias配置如下
```nginx
location /blog/ {
  alias /usr/local/nginx/html/;
  index index.html index.htm;
}
```
同样请求http://127.0.0.1:80/blog/alias.html时，在服务器查找的资源路径是：/usr/local/nginx/html/alias.html。正如其名，alias指定的路径是location的别名，不管location的值怎么写，资源的真实路径都是alias指定的路径.。
> 注：可以把/usr/local/nginx/html/目录下的alias.html移到/usr/local/nginx/html/blog/目录下，再访问http://127.0.0.1:80/blog/alias.html，直接返回404.html

<a name="663e0aac"></a>
### 2、root、alias与location
<a name="4OXhQ"></a>
#### location & root
location和root组合相当于在root指定目录下进行location匹配，location所匹配内容必须保证在root指定目录的子目录，否则配置无效，而且location只能向下匹配，不能匹配location指定目录上一级目录中的内容。
<a name="g5MMw"></a>
#### location & alias
location与alias组合，需要保证location匹配目录与alias指定目录级别相同，否则配置无效；与location和root组合相同的是，location所匹配内容也只能向下匹配。
<a name="khInZ"></a>
#### root与alias的区别

- root与alias主要区别是如何解释location后面的uri
```
root的处理结果：root路径＋location路径
alias的处理结果：使用alias路径替换location路径
alias是一个目录别名的定义，root则是最上层目录的定义。
```

- alias只能作用在location中，而root可以存在server、http和location中；
- alias后面必须要用 “/” 结束，不然会被认为是个文件，而找不到对应的目录；而root则对 “/” 可有可无
- alias在使用正则匹配时，必须捕捉要匹配的内容并在指定的内容处使用

这条可以参考官网如下章节:<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600615723447-8646f69a-b378-497e-be3e-03215363f170.png#height=121&id=lkSeQ&originHeight=363&originWidth=2265&originalType=binary&ratio=1&rotation=0&showTitle=false&size=78037&status=done&style=shadow&title=&width=755)<br />alias在使用正则匹配时，必须捕捉要匹配的内容并在指定的内容处使用，就是正则表达式一定要匹配到内容，然后再alias中引用。<br />然后官网给出了一种推荐使用的方案，如下图的情况使用root代替alias。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600615781510-44ef2f9c-cf77-443c-9e37-7ac2b5245ed9.png#height=322&id=Bxbih&originHeight=965&originWidth=2263&originalType=binary&ratio=1&rotation=0&showTitle=false&size=159862&status=done&style=shadow&title=&width=754.3333333333334)<br />演示：<br />在nginx的配置文件nginx.conf
```nginx
location ~ ^/users/(.+\.(?:gif|jpe?g|png))$ {
    alias /usr/local/nginx/html/images/$1;
}
```
`location ~ ^/users/(.+\.(?:gif|jpe?g|png))$`：这条正则表达式的意思是以/users/开头，区分大小写匹配gif、jpeg、jpg、png这四种图片文件<br />访问如下资源路径
```http
http://127.0.0.1/users/cover_2.png
http://127.0.0.1/users/800.jpg
```
