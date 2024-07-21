Nginx
<a name="XaKu5"></a>
## Location路由规则的使用
像一般的URL都是有一定的规则，类似下面的这几个配置，有些路径需要转发到后端api集群那边，有些的话是静态资源直接读取返回。<br />http://域名:端口/pub/api/xxx/yyyy 就是一个接口路径<br />http://域名:端口/static/css/xxx 就是一个css路径<br />http://域名:端口/static/js/xxx 就是一个css路径<br />配置的文件服务器
```nginx
server {
  listen       80;
  server_name  aabbccdd.com;
  location /app/img {
    alias /usr/local/software/img/;
  }
}
```
这一个location会涉及到正则表达式，简单回顾几个常用的<br />`^` 以什么开始<br />`$` 以什么结束<br />这个例子就是 用户访问路径 /api/user 必须是这个才可以命中location<br />`^/api/user$`<br />下面是常用的路径匹配，语法location [ = | ~ | ~* | ^~ ] uri { ...... }

- `location = /uri`<br /> = 表示精准匹配，只要完全匹配上才能生效
- `location /uri`<br />不带任何修饰符，表示前缀匹配
- `location ^~ /uri/`<br />匹配任何已 /uri/ 开头的任何查询并且停止搜索
- `location /`<br />通用匹配，任何未匹配到其他location的请求都会匹配到

注意：

- 正则匹配
- 区分大小写匹配（~）
- 不区分大小写匹配（~*）
- 优先级(不要写复杂，容易出问题和遗忘)
- 优先级：精准匹配 > 字符串匹配(若有多个匹配项匹配成功，那么选择匹配长的并记录) > 正则匹配

这边来尝试一个案例和对应的匹配结果
```nginx
server { 
   server_name xdclass.net;   

   location ~^/api/pub$ { 
      ...
    }
}
```
解释如下
```
^/api/pub$这个正则表达式表示字符串必须以/开始，以b $结束，中间必须是/api/pub

http://xdclass.net/api/v1 不匹配

http://xdclass.net/API/PUB 不匹配，大小写敏感

http://xdclass.net/api/pub?key1=value1 匹配

http://xdclass.net/api/pub/ 匹配（完全匹配）

http://xdclass.net/api/public 不匹配，不能匹配正则表达式
```
<a name="kn2Jk"></a>
## Rewrite规则的使用
rewrite 地址重定向，实现URL重定向的重要指令，他根据regex(正则表达式)来匹配内容跳转到<br />语法 `rewrite regex replacement[flag]`
```nginx
rewrite ^/(.*)  https://xdclass.net/$1 permanent

# 这是一个正则表达式，匹配完整的域名和后面的路径地址
# replacement部分是https://xdclass.net/$1，$1是取自regex部分()里的内容
```

- 常用正则表达式：
| 字符 | 描述 |
| --- | --- |
| ^ | 匹配输入字符串的起始位置 |
| $ | 匹配输入字符串的结束位置 |
| * | 匹配前面的字符零次或者多次 |
| + | 匹配前面字符串一次或者多次 |
| ? | 匹配前面字符串的零次或者一次 |
| . | 匹配除“\\n”之外的所有单个字符 |
| (pattern) | 匹配括号内的pattern |

注意下 rewrite 最后一项flag参数

| 标记符号 | 说明 |
| --- | --- |
| last | 本条规则匹配完成后继续向下匹配新的location URI规则 |
| break | 本条规则匹配完成后终止，不在匹配任何规则 |
| redirect | 返回302临时重定向 |
| permanent | 返回301永久重定向 |

那只有一个rewrite的应用场景有哪些呢? 这边举几个很常见的例子<br />场景一：访问一个不存在的链接，那那边就可以做一个rewrite进行跳转到首页。<br />场景二：网站域名做了更新，比如以前叫test.com,新的域名叫 xdclass.net，然后如果用户访问 test,com 之后就可以rewrite到 xdclass.net<br />场景三：一开始的域名是HTTP。后来进行了升级为HTTPS，那就可以在用户访问HTTP之后进行重写到HTTPS。<br />这一个rewrite跳转其实是返回了HTTP状态码是3xx开头的重定向，然后浏览器那边进行二次的跳转。
<a name="gOmxQ"></a>
## 浏览器跨域配置
关于跨域，不管是前端开发还是后端开发，肯定很多同学和有遇到，首先需要先知道什么是跨域？<br />这个来源是浏览器同源策略 1995年，同源政策由 Netscape 公司引入浏览器。目前所有浏览器都实行这个政策。最初它的含义是指，A网页设置的 Cookie，B网页不能打开，除非这两个网页"同源"。<br />所谓"同源"指的是"三个相同", 就下面的三个

1. 协议相同  http https
2. 域名相同  www.xdclass.net
3. 端口相同  80  81

一句话：浏览器从一个域名的网页去请求另一个域名的资源时，域名、端口、协议任一不同，都是跨域<br />浏览器控制台跨域提示：<br />No 'Access-Control-Allow-Origin' header is present on the requested resource. Origin 'null' is therefore not allowed access.<br />知道了什么是跨域，那如何去解决呢？有很多种解决方法，这边把他归为两大类<br />第一类是 JSONP，第一类是。这一个的话目前用的比较少，这一块不太灵活。<br />第二类是 Http响应头配置允许跨域，第二类配置跨越的话也有两种方式，<br />第一种是在这个程序代码中通过拦截器去配置跨域的头信息，另外一种就是在Nginx层配置跨域的信息<br />Nginx开启跨域配置，location下配置
```nginx
location / { 
  add_header 'Access-Control-Allow-Origin' $http_origin;
  add_header 'Access-Control-Allow-Credentials' 'true';
  add_header 'Access-Control-Allow-Headers' 'DNT,web-token,app-token,Authorization,Accept,Origin,Keep-Alive,User-Agent,X-Mx-ReqToken,X-Data-Type,X-Auth-Token,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Range';
  add_header Access-Control-Allow-Methods 'GET,POST,OPTIONS';

  #如果预检请求则返回成功,不需要转发到后端
  if ($request_method = 'OPTIONS') {
    add_header 'Access-Control-Max-Age' 1728000;
    add_header 'Content-Type' 'text/plain; charset=utf-8';
    add_header 'Content-Length' 0;
    return 200;
  }
}
```
那工作中一般采用哪一种呢？其实的话比较推荐是在Nginx层网关进行配置。可以根据不同不同的虚拟主机配置哪些系统需要跨越哪些系统，哪些系统不允许跨域，这样的话前端人员可以去配置跨域，也可以由后端进行配置。
